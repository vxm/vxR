#ifndef VXBVH_H
#define VXBVH_H

#include <cstdint>
#include <vector>

#include "AccelStructure.h"
#include "BoundingBox.h"

namespace vxCore
{

///
/// Scene level bounding volume hierarchy, registered as --accel bvh.
///
/// Hybrid layout:
///  - bounded visibles go in a binned SAH tree, 4 or fewer per leaf. A leaf
///    visible keeps its own accelerator (voxel Grid, mesh GeoGrid), the tree
///    only decides which visibles a ray must be given to;
///  - visibles whose box is empty, not finite, or whose hits can fall outside
///    their box (Cylinder) stay in a short list that every ray goes through;
///  - emitters (lights that a ray can hit, which is SphereLight only) are in a
///    list that is only used for rays that can see lights;
///  - lights that can never be hit are not stored at all.
///
/// The leaf protocol is the one of BroadPhase, so for any ray the result is the
/// same as the flat loop (same hit, same visible), ties on the distance go to
/// the visible that was added first. Nothing is shared between threads while
/// tracing: the stack is on the call stack.
///
class Bvh final : public AccelStructure
{
public:
	Bvh() = default;

	const char *name() const override { return "bvh"; }
	void addVisible(VisibleHandle vis) override;
	void updateCache() override;
	int throwRay(const Ray &ray, Collision &collide) const override;
	int throwRayBounded(const Ray &ray, Collision &collide,
						scalar maxDist) const override;
	bool hasCollision(const Ray &ray) const override;
	const std::vector<VisibleHandle> &visibles() const override
	{
		return m_visibles;
	}

	/// Numbers about the last updateCache(), for logs and tests.
	struct BuildInfo
	{
		std::size_t visibles{0};
		std::size_t treePrims{0};
		std::size_t unbounded{0};
		std::size_t emitters{0};
		std::size_t droppedLights{0};
		std::size_t nodes{0};
		std::size_t leaves{0};
		std::size_t maxDepth{0};
		std::size_t maxLeafPrims{0};
	};
	const BuildInfo &buildInfo() const { return m_info; }

	/// Largest traversal stack the tree can need, checked at build.
	static constexpr std::size_t kStackSize = 96;
	static constexpr unsigned kMaxLeafPrims = 4;
	static constexpr unsigned kBins = 16;

private:
	/// 64 bytes, one cache line. Internal nodes keep their two children next to
	/// each other: left is `offset`, right is `offset + 1`.
	struct alignas(64) Node
	{
		scalar bmin[3];
		scalar bmax[3];
		std::uint32_t offset{0}; // first child or first primitive
		std::uint16_t count{0};  // 0 for an internal node
		std::uint16_t pad{0};
	};
	static_assert(sizeof(Node) == 64, "a BVH node should be one cache line");

	/// A visible plus the order in which it was added, which breaks ties.
	struct Item
	{
		Visible *vis;
		std::uint32_t order;
	};

	struct BuildItem
	{
		scalar lo[3];
		scalar hi[3];
		scalar c[3];
		std::uint32_t index; // into m_visibles
	};

	void buildNode(std::uint32_t nodeIndex, std::vector<BuildItem> &items,
				   std::uint32_t begin, std::uint32_t end, unsigned depth);

	int traverse(const Ray &ray, Collision &collide, scalar bound) const;

	std::vector<VisibleHandle> m_visibles;
	std::vector<Node> m_nodes;
	std::vector<Item> m_prims;     // leaf order
	std::vector<Item> m_unbounded; // insertion order
	std::vector<Item> m_emitters;  // insertion order
	BuildInfo m_info;
};

} // namespace vxCore
#endif // VXBVH_H
