#include "MeshBvh.h"

#include <algorithm>
#include <cmath>
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
