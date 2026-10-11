#ifndef VXMESHBVH_H
#define VXMESHBVH_H

#include <cstdint>
#include <string>
#include <vector>

#include "BoundingBox.h"

namespace vxCore
{

class Collision;
class Ray;
class TriRef;

///
/// Per-mesh triangle BVH, selectable as VXR_MESH=bvh.
///
/// Built over triangle AABBs with the same binned SAH as the scene Bvh
/// (headers/Bvh.h). Traversal is closest-hit: a later triangle that is nearer
/// wins, unlike GeoGrid which returns the first occupied cell that hits.
/// The scene-level Bvh is unchanged; this tree only lives on TriangleMesh.
///
class MeshBvh
{
public:
	MeshBvh() = default;

	struct BuildInfo
	{
		std::size_t triangles{0};
		std::size_t nodes{0};
		std::size_t leaves{0};
		std::size_t maxDepth{0};
		std::size_t maxLeafPrims{0};
	};

	static constexpr std::size_t kStackSize = 96;
	static constexpr unsigned kMaxLeafPrims = 4;
	static constexpr unsigned kBins = 16;

	/// What a cache file is checked against. The tree is in world space, so a
	/// different placement of the same file is a different cache.
	struct CacheStamp
	{
		std::uint64_t sourceBytes{0};
		std::int64_t sourceMtime{0};
		scalar transform[16]{};
	};

	void build(const std::vector<TriRef> &tris);
	/// Read a tree written by writeCache. False leaves this object empty.
	bool readCache(const std::string &path, std::size_t triangleCount,
				   const CacheStamp &stamp);
	bool writeCache(const std::string &path, const CacheStamp &stamp) const;
	int throwRay(const std::vector<TriRef> &tris, const Ray &ray,
				 Collision &col) const;
	bool hasCollision(const std::vector<TriRef> &tris, const Ray &ray) const;

	bool empty() const { return m_nodes.empty(); }
	const BuildInfo &buildInfo() const { return m_info; }

private:
	/// Same 64-byte node as Bvh: children are adjacent, left is offset.
	struct alignas(64) Node
	{
		scalar bmin[3];
		scalar bmax[3];
		std::uint32_t offset{0};
		std::uint16_t count{0};
		std::uint16_t pad{0};
	};
	static_assert(sizeof(Node) == 64, "a mesh BVH node should be one cache line");

	struct BuildItem
	{
		scalar lo[3];
		scalar hi[3];
		scalar c[3];
		std::uint32_t index;
	};

	void buildNode(std::uint32_t nodeIndex, std::vector<BuildItem> &items,
				   std::uint32_t begin, std::uint32_t end, unsigned depth);

	std::vector<Node> m_nodes;
	std::vector<std::uint32_t> m_prims;
	BuildInfo m_info;
};

} // namespace vxCore
#endif // VXMESHBVH_H
