#include "MeshBvh.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>

#include "Collision.h"
#include "PerfStats.h"
#include "Ray.h"
#include "TriRef.h"

using namespace vxCore;

namespace
{

constexpr scalar kMax = std::numeric_limits<scalar>::max();
constexpr scalar kTraversalCost = 0.5;
constexpr unsigned kSahDepthLimit = 40;
constexpr scalar kBoxPad = 1e-9;

scalar area(const scalar *lo, const scalar *hi)
{
	const scalar dx = hi[0] - lo[0];
	const scalar dy = hi[1] - lo[1];
	const scalar dz = hi[2] - lo[2];
	if (dx < 0.0 || dy < 0.0 || dz < 0.0)
	{
		return 0.0;
	}
	return scalar(2.0) * (dx * dy + dy * dz + dz * dx);
}

void grow(scalar *lo, scalar *hi, const scalar *blo, const scalar *bhi)
{
	for (int a = 0; a < 3; a++)
	{
		lo[a] = std::min(lo[a], blo[a]);
		hi[a] = std::max(hi[a], bhi[a]);
	}
}

scalar slackOf(scalar limit)
{
	return limit >= 1e300 ? limit : limit * (1.0 + 1e-9) + 1e-9;
}

struct Entry
{
	std::uint32_t node;
	scalar t;
};

template <class NodeT>
inline bool slabTest(const NodeT &n, const RayInv &r, scalar &tEnter)
{
	const scalar nx = ((r.sx ? n.bmax[0] : n.bmin[0]) - r.ox) * r.ix;
	const scalar fx = ((r.sx ? n.bmin[0] : n.bmax[0]) - r.ox) * r.ix;
	const scalar ny = ((r.sy ? n.bmax[1] : n.bmin[1]) - r.oy) * r.iy;
	const scalar fy = ((r.sy ? n.bmin[1] : n.bmax[1]) - r.oy) * r.iy;
	const scalar nz = ((r.sz ? n.bmax[2] : n.bmin[2]) - r.oz) * r.iz;
	const scalar fz = ((r.sz ? n.bmin[2] : n.bmax[2]) - r.oz) * r.iz;

	scalar lo = nx > ny ? nx : ny;
	lo = lo > nz ? lo : nz;
	lo = lo > 0.0 ? lo : 0.0;
	scalar hi = fx < fy ? fx : fy;
	hi = hi < fz ? hi : fz;
	tEnter = lo;
	return lo <= hi;
}

/// Same front-face filter GeoGrid uses after a triangle hit.
bool hitInFront(const Ray &ray, const v3s &origin, const Collision &col)
{
	return !ray.direction().follows((col.position() - origin).unit());
}

} // namespace

void MeshBvh::build(const std::vector<TriRef> &tris)
{
	m_nodes.clear();
	m_prims.clear();
	m_info = BuildInfo{};
	m_info.triangles = tris.size();

	std::vector<BuildItem> items;
	items.reserve(tris.size());

	for (std::uint32_t i = 0; i < tris.size(); i++)
	{
		const auto &t = tris[i];
		BuildItem item{};
		const scalar rawLo[3] = {std::min(t.p1.x(), std::min(t.p2.x(), t.p3.x())),
								 std::min(t.p1.y(), std::min(t.p2.y(), t.p3.y())),
								 std::min(t.p1.z(), std::min(t.p2.z(), t.p3.z()))};
		const scalar rawHi[3] = {std::max(t.p1.x(), std::max(t.p2.x(), t.p3.x())),
								 std::max(t.p1.y(), std::max(t.p2.y(), t.p3.y())),
								 std::max(t.p1.z(), std::max(t.p2.z(), t.p3.z()))};
		for (int a = 0; a < 3; a++)
		{
			item.lo[a] = rawLo[a] - kBoxPad * (1.0 + std::abs(rawLo[a]));
			item.hi[a] = rawHi[a] + kBoxPad * (1.0 + std::abs(rawHi[a]));
			item.c[a] = (rawLo[a] + rawHi[a]) * 0.5;
		}
		item.index = i;
		items.push_back(item);
	}

	if (items.empty())
	{
		std::cout << " -- mesh: bvh 0 tris --" << std::endl;
		return;
	}

	m_nodes.reserve(items.size() * 2);
	m_nodes.resize(1);
	buildNode(0, items, 0, static_cast<std::uint32_t>(items.size()), 0);
	m_nodes.shrink_to_fit();

	m_prims.resize(items.size());
	for (std::size_t i = 0; i < items.size(); i++)
	{
		m_prims[i] = items[i].index;
	}
	m_info.nodes = m_nodes.size();

	if (m_info.maxDepth + 1 >= kStackSize)
	{
		std::cerr << "mesh bvh: tree too deep (" << m_info.maxDepth << ")"
				  << std::endl;
	}

	std::cout << " -- mesh: bvh " << m_info.triangles << " tris, " << m_info.nodes
			  << " nodes, " << m_info.leaves << " leaves, depth "
			  << m_info.maxDepth + 1 << ", max leaf " << m_info.maxLeafPrims
			  << " --" << std::endl;
}

namespace
{

constexpr char kCacheMagic[8] = {'v', 'x', 'R', 'm', 'b', 'v', 'h', '1'};
constexpr std::uint32_t kCacheVersion = 1;
// Bump when the tree shape can change for the same triangles.
constexpr std::uint32_t kCacheBuilder = 1;

template <typename T>
bool writePod(std::ostream &out, const T &value)
{
	out.write(reinterpret_cast<const char *>(&value), sizeof(T));
	return static_cast<bool>(out);
}

template <typename T>
bool readPod(std::istream &in, T &value)
{
	in.read(reinterpret_cast<char *>(&value), sizeof(T));
	return static_cast<bool>(in);
}

} // namespace

bool MeshBvh::readCache(const std::string &path, std::size_t triangleCount,
						const CacheStamp &stamp)
{
	m_nodes.clear();
	m_prims.clear();
	m_info = BuildInfo{};

	std::ifstream in(path, std::ios::binary);
	if (!in)
	{
		return false;
	}

	char magic[8] = {};
	std::uint32_t version = 0;
	std::uint32_t builder = 0;
	std::uint32_t leaf = 0;
	std::uint32_t bins = 0;
	std::uint64_t triCount = 0;
	std::uint64_t nodeCount = 0;
	std::uint64_t primCount = 0;
	std::uint64_t sourceBytes = 0;
	std::int64_t sourceMtime = 0;
	scalar transform[16] = {};
	std::uint64_t leaves = 0;
	std::uint64_t maxDepth = 0;
	std::uint64_t maxLeafPrims = 0;

	if (!in.read(magic, 8) || std::memcmp(magic, kCacheMagic, 8) != 0 ||
		!readPod(in, version) || version != kCacheVersion ||
		!readPod(in, builder) || builder != kCacheBuilder ||
		!readPod(in, leaf) || leaf != kMaxLeafPrims || !readPod(in, bins) ||
		bins != kBins || !readPod(in, triCount) ||
		triCount != triangleCount || !readPod(in, nodeCount) ||
		!readPod(in, primCount) || primCount != triCount ||
		!readPod(in, sourceBytes) || sourceBytes != stamp.sourceBytes ||
		!readPod(in, sourceMtime) || sourceMtime != stamp.sourceMtime ||
		!in.read(reinterpret_cast<char *>(transform), sizeof(transform)) ||
		std::memcmp(transform, stamp.transform, sizeof(transform)) != 0 ||
		!readPod(in, leaves) || !readPod(in, maxDepth) ||
		!readPod(in, maxLeafPrims))
	{
		return false;
	}

	if (triCount == 0 || nodeCount == 0 || nodeCount > triCount * 2 ||
		maxDepth + 1 >= kStackSize)
	{
		return false;
	}

	m_nodes.resize(static_cast<std::size_t>(nodeCount));
	for (auto &node : m_nodes)
	{
		std::uint32_t count = 0;
		if (!in.read(reinterpret_cast<char *>(node.bmin), sizeof(node.bmin)) ||
			!in.read(reinterpret_cast<char *>(node.bmax), sizeof(node.bmax)) ||
			!readPod(in, node.offset) || !readPod(in, count) || count > 65535u)
		{
			m_nodes.clear();
			return false;
		}
		node.count = static_cast<std::uint16_t>(count);
		node.pad = 0;
	}

	m_prims.resize(static_cast<std::size_t>(primCount));
	if (!in.read(reinterpret_cast<char *>(m_prims.data()),
				 static_cast<std::streamsize>(m_prims.size() * sizeof(std::uint32_t))))
	{
		m_nodes.clear();
		m_prims.clear();
		return false;
	}

	for (const auto id : m_prims)
	{
		if (id >= triCount)
		{
			m_nodes.clear();
			m_prims.clear();
			return false;
		}
	}
	for (const auto &node : m_nodes)
	{
		if (node.count == 0)
		{
			if (static_cast<std::uint64_t>(node.offset) + 1 >= nodeCount)
			{
				m_nodes.clear();
				m_prims.clear();
				return false;
			}
		}
		else if (static_cast<std::uint64_t>(node.offset) + node.count > primCount)
		{
			m_nodes.clear();
			m_prims.clear();
			return false;
		}
	}

	m_info.triangles = static_cast<std::size_t>(triCount);
	m_info.nodes = static_cast<std::size_t>(nodeCount);
	m_info.leaves = static_cast<std::size_t>(leaves);
	m_info.maxDepth = static_cast<std::size_t>(maxDepth);
	m_info.maxLeafPrims = static_cast<std::size_t>(maxLeafPrims);
	return true;
}

bool MeshBvh::writeCache(const std::string &path, const CacheStamp &stamp) const
{
	if (m_nodes.empty() || m_prims.size() != m_info.triangles)
	{
		return false;
	}

	const auto temp = path + ".tmp";
	{
		std::ofstream out(temp, std::ios::binary | std::ios::trunc);
		if (!out)
		{
			return false;
		}

		const std::uint32_t version = kCacheVersion;
		const std::uint32_t builder = kCacheBuilder;
		const std::uint32_t leaf = kMaxLeafPrims;
		const std::uint32_t bins = kBins;
		const std::uint64_t triCount = m_info.triangles;
		const std::uint64_t nodeCount = m_nodes.size();
		const std::uint64_t primCount = m_prims.size();
		const std::uint64_t leaves = m_info.leaves;
		const std::uint64_t maxDepth = m_info.maxDepth;
		const std::uint64_t maxLeafPrims = m_info.maxLeafPrims;

		out.write(kCacheMagic, 8);
		if (!writePod(out, version) || !writePod(out, builder) ||
			!writePod(out, leaf) || !writePod(out, bins) ||
			!writePod(out, triCount) || !writePod(out, nodeCount) ||
			!writePod(out, primCount) || !writePod(out, stamp.sourceBytes) ||
			!writePod(out, stamp.sourceMtime) ||
			!out.write(reinterpret_cast<const char *>(stamp.transform),
					   sizeof(stamp.transform)) ||
			!writePod(out, leaves) || !writePod(out, maxDepth) ||
			!writePod(out, maxLeafPrims))
		{
			out.close();
			std::remove(temp.c_str());
			return false;
		}

		for (const auto &node : m_nodes)
		{
			const std::uint32_t count = node.count;
			if (!out.write(reinterpret_cast<const char *>(node.bmin),
						   sizeof(node.bmin)) ||
				!out.write(reinterpret_cast<const char *>(node.bmax),
						   sizeof(node.bmax)) ||
				!writePod(out, node.offset) || !writePod(out, count))
			{
				out.close();
				std::remove(temp.c_str());
				return false;
			}
		}

		if (!out.write(reinterpret_cast<const char *>(m_prims.data()),
					   static_cast<std::streamsize>(m_prims.size() *
													sizeof(std::uint32_t))))
		{
			out.close();
			std::remove(temp.c_str());
			return false;
		}
	}

	if (std::rename(temp.c_str(), path.c_str()) != 0)
	{
		std::remove(temp.c_str());
		return false;
	}
	return true;
}

void MeshBvh::buildNode(std::uint32_t ni, std::vector<BuildItem> &items,
						std::uint32_t begin, std::uint32_t end, unsigned depth)
{
	const std::uint32_t n = end - begin;

	scalar lo[3] = {kMax, kMax, kMax};
	scalar hi[3] = {-kMax, -kMax, -kMax};
	scalar clo[3] = {kMax, kMax, kMax};
	scalar chi[3] = {-kMax, -kMax, -kMax};
	for (auto i = begin; i < end; i++)
	{
		grow(lo, hi, items[i].lo, items[i].hi);
		grow(clo, chi, items[i].c, items[i].c);
	}
	for (int a = 0; a < 3; a++)
	{
		m_nodes[ni].bmin[a] = lo[a];
		m_nodes[ni].bmax[a] = hi[a];
	}
	m_info.maxDepth = std::max<std::size_t>(m_info.maxDepth, depth);

	auto makeLeaf = [&]() {
		m_nodes[ni].offset = begin;
		m_nodes[ni].count = static_cast<std::uint16_t>(n);
		m_info.leaves++;
		m_info.maxLeafPrims = std::max<std::size_t>(m_info.maxLeafPrims, n);
	};

	if (n == 1)
	{
		makeLeaf();
		return;
	}

	const scalar parentArea = area(lo, hi);
	const scalar ext[3] = {chi[0] - clo[0], chi[1] - clo[1], chi[2] - clo[2]};
	int widest = 0;
	if (ext[1] > ext[widest])
		widest = 1;
	if (ext[2] > ext[widest])
		widest = 2;

	std::uint32_t mid = begin;
	bool split = false;

	if (depth < kSahDepthLimit && parentArea > 0.0 && ext[widest] > 0.0)
	{
		int bestAxis = -1;
		unsigned bestBin = 0;
		scalar bestCost = kMax;

		for (int a = 0; a < 3; a++)
		{
			if (!(ext[a] > 0.0))
			{
				continue;
			}
			unsigned count[kBins] = {};
			scalar blo[kBins][3];
			scalar bhi[kBins][3];
			for (auto &b : blo)
				b[0] = b[1] = b[2] = kMax;
			for (auto &b : bhi)
				b[0] = b[1] = b[2] = -kMax;

			const scalar k = scalar(kBins) / ext[a];
			for (auto i = begin; i < end; i++)
			{
				auto bin = static_cast<unsigned>((items[i].c[a] - clo[a]) * k);
				bin = std::min(bin, kBins - 1);
				count[bin]++;
				grow(blo[bin], bhi[bin], items[i].lo, items[i].hi);
			}

			scalar rightArea[kBins];
			unsigned rightCount[kBins];
			{
				scalar rlo[3] = {kMax, kMax, kMax};
				scalar rhi[3] = {-kMax, -kMax, -kMax};
				unsigned c = 0;
				for (int b = kBins - 1; b >= 1; b--)
				{
					if (count[b])
					{
						grow(rlo, rhi, blo[b], bhi[b]);
					}
					c += count[b];
					rightCount[b] = c;
					rightArea[b] = c ? area(rlo, rhi) : 0.0;
				}
			}
			scalar llo[3] = {kMax, kMax, kMax};
			scalar lhi[3] = {-kMax, -kMax, -kMax};
			unsigned lc = 0;
			for (unsigned b = 0; b + 1 < kBins; b++)
			{
				if (count[b])
				{
					grow(llo, lhi, blo[b], bhi[b]);
				}
				lc += count[b];
				const unsigned rc = rightCount[b + 1];
				if (lc == 0 || rc == 0)
				{
					continue;
				}
				const scalar cost = kTraversalCost +
									(lc * area(llo, lhi) + rc * rightArea[b + 1]) /
										parentArea;
				if (cost < bestCost)
				{
					bestCost = cost;
					bestAxis = a;
					bestBin = b;
				}
			}
		}

		if (bestAxis >= 0)
		{
			if (n <= kMaxLeafPrims && scalar(n) <= bestCost)
			{
				makeLeaf();
				return;
			}

			const scalar k = scalar(kBins) / ext[bestAxis];
			auto it = std::partition(
				items.begin() + begin, items.begin() + end, [&](const BuildItem &b) {
					auto bin = static_cast<unsigned>((b.c[bestAxis] - clo[bestAxis]) * k);
					return std::min(bin, kBins - 1) <= bestBin;
				});
			mid = static_cast<std::uint32_t>(it - items.begin());
			split = mid > begin && mid < end;
		}
	}

	if (!split)
	{
		if (n <= kMaxLeafPrims)
		{
			makeLeaf();
			return;
		}
		mid = begin + n / 2;
		std::nth_element(items.begin() + begin, items.begin() + mid,
						 items.begin() + end,
						 [&](const BuildItem &a, const BuildItem &b) {
							 return a.c[widest] < b.c[widest];
						 });
	}

	const auto left = static_cast<std::uint32_t>(m_nodes.size());
	m_nodes.resize(m_nodes.size() + 2);
	m_nodes[ni].offset = left;
	m_nodes[ni].count = 0;

	buildNode(left, items, begin, mid, depth + 1);
	buildNode(left + 1, items, mid, end, depth + 1);
}

int MeshBvh::throwRay(const std::vector<TriRef> &tris, const Ray &ray,
					  Collision &col) const
{
	if (m_nodes.empty())
	{
		return 0;
	}

	const RayInv ri(ray);
	const v3s origin = ray.origin();
	const Collision seed = col;

	bool have = false;
	scalar bestDist = kMax;
	Collision best;
	scalar limit = kMax;
	scalar limitSlack = slackOf(limit);
	std::uint64_t nodeVisits = 0;

	auto testTri = [&](std::uint32_t id) {
		Collision temp = seed;
		if (!tris[id].throwRay(ray, temp))
		{
			return;
		}
		if (!hitInFront(ray, origin, temp))
		{
			return;
		}
		const auto d = temp.position().distance(origin);
		if (d < bestDist)
		{
			bestDist = d;
			best = temp;
			have = true;
			limit = bestDist;
			limitSlack = slackOf(limit);
		}
	};

	Entry stack[kStackSize];
	std::size_t sp = 0;

	scalar t;
	if (slabTest(m_nodes[0], ri, t))
	{
		stack[sp++] = {0, t};
	}

	while (sp)
	{
		const Entry e = stack[--sp];
		if (e.t * ri.dirLen > limitSlack)
		{
			continue;
		}

		const Node &n = m_nodes[e.node];
		++nodeVisits;

		if (n.count)
		{
			for (unsigned i = 0; i < n.count; i++)
			{
				testTri(m_prims[n.offset + i]);
			}
			continue;
		}

		const Node &l = m_nodes[n.offset];
		const Node &r = m_nodes[n.offset + 1];
		scalar tl, tr;
		const bool hl = slabTest(l, ri, tl);
		const bool hr = slabTest(r, ri, tr);

		if (hl && hr)
		{
			if (tl <= tr)
			{
				stack[sp++] = {n.offset + 1, tr};
				stack[sp++] = {n.offset, tl};
			}
			else
			{
				stack[sp++] = {n.offset, tl};
				stack[sp++] = {n.offset + 1, tr};
			}
		}
		else if (hl)
		{
			stack[sp++] = {n.offset, tl};
		}
		else if (hr)
		{
			stack[sp++] = {n.offset + 1, tr};
		}
	}

#ifndef VXR_NO_PERF_STATS
	if (PerfStats::enabled())
	{
		PerfStats::local()[static_cast<std::size_t>(Stat::kMeshBvhNodeVisit)] +=
			nodeVisits;
	}
#endif

	if (have)
	{
		col = best;
		return 1;
	}
	return 0;
}

bool MeshBvh::hasCollision(const std::vector<TriRef> &tris, const Ray &ray) const
{
	if (m_nodes.empty())
	{
		return false;
	}

	const RayInv ri(ray);
	const v3s origin = ray.origin();
	const scalar maxLen = ray.length();
	Collision temp;

	Entry stack[kStackSize];
	std::size_t sp = 0;
	scalar t;
	if (!slabTest(m_nodes[0], ri, t))
	{
		return false;
	}
	stack[sp++] = {0, t};

	while (sp)
	{
		const Entry e = stack[--sp];
		const Node &n = m_nodes[e.node];

		if (n.count)
		{
			for (unsigned i = 0; i < n.count; i++)
			{
				if (!tris[m_prims[n.offset + i]].throwRay(ray, temp))
				{
					continue;
				}
				if (!hitInFront(ray, origin, temp))
				{
					continue;
				}
				if (maxLen > 0.0 && temp.position().distance(origin) > maxLen)
				{
					continue;
				}
				return true;
			}
			continue;
		}

		const Node &l = m_nodes[n.offset];
		const Node &r = m_nodes[n.offset + 1];
		scalar tl, tr;
		const bool hl = slabTest(l, ri, tl);
		const bool hr = slabTest(r, ri, tr);
		if (hl)
		{
			stack[sp++] = {n.offset, tl};
		}
		if (hr)
		{
			stack[sp++] = {n.offset + 1, tr};
		}
	}
	return false;
}
