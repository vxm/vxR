#include "Bvh.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <unordered_set>

#include "PerfStats.h"

using namespace vxCore;

namespace
{

constexpr scalar kMax = std::numeric_limits<scalar>::max();

// SAH constants. A primitive test is the unit, a node visit costs half.
constexpr scalar kTraversalCost = 0.5;
// Past this depth the tree is split in halves, so the depth stays bounded
// whatever the SAH finds, and the stack on the call stack is always enough.
constexpr unsigned kSahDepthLimit = 40;

/// Every box of the tree is grown by this much (relative to the coordinate) so
/// that the slab test, which rounds differently than the legacy box test,
/// never rejects a ray the legacy test accepts.
constexpr scalar kBoxPad = 1e-9;

/// isfinite() cannot be trusted under -ffast-math (it may be folded to true),
/// so look at the exponent bits.
bool finiteBits(scalar v)
{
	std::uint64_t bits;
	std::memcpy(&bits, &v, sizeof(bits));
	return ((bits >> 52) & 0x7ffu) != 0x7ffu;
}

/// A box the tree can store: not empty, all numbers finite and sane.
bool usableBox(const BoundingBox &bb)
{
	const scalar v[6] = {bb.minX(), bb.minY(), bb.minZ(),
						 bb.maxX(), bb.maxY(), bb.maxZ()};
	for (const auto x : v)
	{
		if (!finiteBits(x) || x > 1e15 || x < -1e15)
		{
			return false;
		}
	}
	return bb.minX() <= bb.maxX() && bb.minY() <= bb.maxY() &&
		   bb.minZ() <= bb.maxZ();
}

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

/// The distance after which a node cannot hold anything closer than limit.
scalar slackOf(scalar limit)
{
	return limit >= 1e300 ? limit : limit * (1.0 + 1e-9) + 1e-9;
}

} // namespace

void Bvh::addVisible(VisibleHandle vis)
{
	vis->updateBoundingBox();

	// never add a visible twice, same rule as BroadPhase
	if (std::find(m_visibles.begin(), m_visibles.end(), vis) == m_visibles.end())
	{
		m_visibles.emplace_back(std::move(vis));
	}
}

void Bvh::updateCache()
{
	m_nodes.clear();
	m_prims.clear();
	m_unbounded.clear();
	m_emitters.clear();
	m_info = BuildInfo{};
	m_info.visibles = m_visibles.size();

	std::vector<BuildItem> items;
	items.reserve(m_visibles.size());

	for (std::uint32_t i = 0; i < m_visibles.size(); i++)
	{
		auto &vis = m_visibles[i];
		vis->updateBoundingBox();

		if (!vis->isHittable())
		{
			m_info.droppedLights++;
			continue;
		}

		if (vis->type() == VisibleType::kLight)
		{
			m_emitters.push_back({vis.get(), i});
			continue;
		}

		const auto bb = vis->boundingBox();
		if (!bb || !usableBox(*bb) || !vis->hitsInsideBounds())
		{
			m_unbounded.push_back({vis.get(), i});
			continue;
		}

		BuildItem item{};
		const scalar lo[3] = {bb->minX(), bb->minY(), bb->minZ()};
		const scalar hi[3] = {bb->maxX(), bb->maxY(), bb->maxZ()};
		for (int a = 0; a < 3; a++)
		{
			item.lo[a] = lo[a] - kBoxPad * (1.0 + std::abs(lo[a]));
			item.hi[a] = hi[a] + kBoxPad * (1.0 + std::abs(hi[a]));
			item.c[a] = (lo[a] + hi[a]) * 0.5;
		}
		item.index = i;
		items.push_back(item);
	}

	m_info.emitters = m_emitters.size();
	m_info.unbounded = m_unbounded.size();
	m_info.treePrims = items.size();

	if (!items.empty())
	{
		m_nodes.reserve(items.size() * 2);
		m_nodes.resize(1);
		buildNode(0, items, 0, static_cast<std::uint32_t>(items.size()), 0);
		m_nodes.shrink_to_fit();

		m_prims.reserve(items.size());
		for (const auto &it : items)
		{
			m_prims.push_back({m_visibles[it.index].get(), it.index});
		}
	}
	m_info.nodes = m_nodes.size();

	if (m_info.maxDepth + 1 >= kStackSize)
	{
		// Cannot happen with the depth limit, but never overrun the stack.
		std::cerr << "bvh: tree too deep (" << m_info.maxDepth << ")" << std::endl;
	}

	std::cout << " -- bvh: " << m_info.visibles << " visibles, "
			  << m_info.treePrims << " in the tree (" << m_info.nodes << " nodes, "
			  << m_info.leaves << " leaves, depth " << m_info.maxDepth + 1
			  << "), " << m_info.unbounded << " unbounded, " << m_info.emitters
			  << " emitters, " << m_info.droppedLights << " lights never hit --"
			  << std::endl;
}

void Bvh::buildNode(std::uint32_t ni, std::vector<BuildItem> &items,
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

	// Binned SAH over the axes where the centroids are spread out.
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

			// right to left areas
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
			// Few primitives and no gain from splitting: a leaf.
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
		// Median split, also the answer to identical centroids.
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

namespace
{

struct Entry
{
	std::uint32_t node;
	scalar t;
};

/// Slab test of a node box. tEnter is in units of the ray direction.
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

} // namespace

int Bvh::throwRay(const Ray &ray, Collision &collide) const
{
	return traverse(ray, collide, kMax);
}

int Bvh::throwRayBounded(const Ray &ray, Collision &collide,
						 scalar maxDist) const
{
	return traverse(ray, collide, maxDist);
}

int Bvh::traverse(const Ray &ray, Collision &collide, scalar bound) const
{
	const bool opaqueOnly = ray.m_vision == VisionType::kOpaque;
	// Seeded the way BroadPhase seeds its temporary: a copy of what the caller
	// passed, with the distance limit opened. A prim never sees another prim's
	// leftovers.
	const Collision seed = [&] {
		Collision s = collide;
		s.setMaxDistance(kMax);
		return s;
	}();
	const RayInv ri(ray);
	const v3s origin = ray.origin();

	bool have = false;
	scalar bestDist = kMax;
	std::uint32_t bestOrder = 0;
	Collision best;

	scalar limit = bound;
	scalar limitSlack = slackOf(limit);

	std::uint64_t nodeVisits = 0, lightSkips = 0, boxTests = 0, boxRejects = 0,
				  primTests = 0, primHits = 0;

	// The legacy leaf protocol, line by line what BroadPhase does per visible.
	auto testItem = [&](const Item &it) {
		Collision temp = seed;
		++boxTests;
		if (!it.vis->testBoundingBox(ray, temp))
		{
			++boxRejects;
			return;
		}
		++primTests;
		if (!it.vis->throwRay(ray, temp))
		{
			return;
		}
		++primHits;

		const auto s = temp.position().distance(origin);
		if ((s < bestDist || (have && s == bestDist && it.order < bestOrder)) &&
			temp.isValid())
		{
			bestDist = s;
			bestOrder = it.order;
			temp.setMaxDistance(s);
			best = temp;
			best.m_geo = it.vis;
			have = true;
			limit = std::min(bestDist, bound);
			limitSlack = slackOf(limit);
		}
	};

	for (const auto &it : m_unbounded)
	{
		++nodeVisits;
		if (opaqueOnly && it.vis->type() == VisibleType::kLight)
		{
			++lightSkips;
			continue;
		}
		testItem(it);
	}

	if (opaqueOnly)
	{
		lightSkips += m_emitters.size();
	}
	else
	{
		for (const auto &it : m_emitters)
		{
			++nodeVisits;
			testItem(it);
		}
	}

	if (!m_nodes.empty())
	{
		Entry stack[kStackSize];
		std::size_t sp = 0;

		scalar t;
		++boxTests;
		if (slabTest(m_nodes[0], ri, t))
		{
			stack[sp++] = {0, t};
		}
		else
		{
			++boxRejects;
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
					testItem(m_prims[n.offset + i]);
				}
				continue;
			}

			const Node &l = m_nodes[n.offset];
			const Node &r = m_nodes[n.offset + 1];
			scalar tl, tr;
			const bool hl = slabTest(l, ri, tl);
			const bool hr = slabTest(r, ri, tr);
			boxTests += 2;
			boxRejects += (hl ? 0 : 1) + (hr ? 0 : 1);

			if (hl && hr)
			{
				// nearer one is popped first
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
	}

#ifndef VXR_NO_PERF_STATS
	if (PerfStats::enabled())
	{
		auto &c = PerfStats::local();
		const auto at = [](Stat s) { return static_cast<std::size_t>(s); };
		c[at(Stat::kAccelNodeVisit)] += nodeVisits;
		c[at(Stat::kAccelLightSkip)] += lightSkips;
		c[at(Stat::kAccelBoxTest)] += boxTests;
		c[at(Stat::kAccelBoxReject)] += boxRejects;
		c[at(Stat::kAccelPrimTest)] += primTests;
		c[at(Stat::kAccelPrimHit)] += primHits;
	}
#endif

	if (have)
	{
		collide = best;
		return 1;
	}
	return 0;
}

bool Bvh::hasCollision(const Ray &ray) const
{
	PERF_COUNT(kAccelHasCollision);
	// Not used by the renderer. Same flat loop as BroadPhase::hasCollision.
	for (auto &visbl : m_visibles)
	{
		if (ray.m_vision == VisionType::kOpaque &&
			visbl->type() == VisibleType::kLight)
		{
			continue;
		}
		if (visbl->boundingBox()->hasCollision(ray) == 1)
		{
			if (visbl->hasCollision(ray) == 1)
			{
				return true;
			}
		}
	}
	return false;
}
