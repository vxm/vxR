#ifndef VXTRIANGLEMESH_H
#define VXTRIANGLEMESH_H

#include <array>
#include <deque>
#include <vector>

#include "Geometry.h"
#include "MeshBvh.h"

namespace vxCore {

using TriIndices = std::vector<std::array<unsigned long,3u>>;

class TriangleMesh final : public Geometry
{
	///
	/// \brief m_constructionPath
	///
	std::string m_constructionPath;
	///
	std::atomic_bool m_openForEdition{false};
	///
	/// \brief m_grid
	///
	GeoGrid m_grid;
	///
	/// \brief m_bvh
	/// Per-mesh triangle BVH, built only when VXR_MESH=bvh.
	///
	MeshBvh m_bvh;
	///
	/// \brief m_vertices
	///
	std::vector<v3s> m_vertices;
	///
	/// \brief m_vertexNormals
	///
	std::vector<v3s> m_vertexNormals;
	///
	/// Face normals for meshes that have no vertex normals. TriRef stores
	/// normals by reference, so these have to outlive the triangles. A deque
	/// keeps existing elements put when a new face is added.
	///
	std::deque<v3s> m_faceNormals;
	///
	/// \brief m_uvs
	///
	std::vector<v2s> m_uvs;
	///
	/// \brief m_triangles
	///
	std::vector<TriRef> m_triangles;

public:
	///
	/// \brief defaultUV
	///
	static const v2s defaultUV;
	
	///
	/// \brief vxTriangleMesh
	///
	TriangleMesh();
	///
	/// \brief updateAccelerationStuctures
	///
	void updateAccelerationStuctures();
	///
	/// \brief clear
	///
	void clear();
	///
	/// \brief open
	///
	void open();
	///
	/// \brief close
	///
	void close();
	///
	/// \brief addVertexTransformed
	/// \param v3s
	///
	void addVertexTransformed(const v3s &v3s);
	///
	/// \brief addVertexNormalTransformed
	/// \param normal
	///
	void addVertexNormalTransformed(const v3s &normal);
	///
	/// \brief addUV
	/// \param uv
	///
	void addUV(const v2s &uv);
	///
	/// \brief addTriangle
	/// \param a
	/// \param b
	/// \param c
	/// \return 
	///
	TriRef& addTriangle(unsigned long a,
					 unsigned long b,
					 unsigned long c);
	///
	/// \brief triangles
	/// \return 
	/// Gets a reference to the triangle references of this object
	std::vector<TriRef>& triangles();
	///
	/// \brief vertices
	/// \return 
	/// Returns a reference to the vertices of this trimesh
	std::vector<v3s>& vertices();
	///
	/// \brief vertices
	/// \return 
	/// returns a copy of these vertices
	std::vector<v3s> vertices() const;
	///
	/// \brief setVertices
	/// \param vertices
	/// 
	void setVertices(const std::vector<v3s> &vertices);
	///
	/// \brief vertexNormals
	/// \return 
	///
	std::vector<v3s>& vertexNormals();
	///
	/// \brief vertexNormals
	/// \return 
	///
	std::vector<v3s> vertexNormals() const;
	///
	/// \brief setVertexNormals
	/// \param vertexNormals
	///
	void setVertexNormals(const std::vector<v3s> &vertexNormals);
	///
	/// \brief uvs
	/// \return 
	///
	std::vector<v2s>& uvs();
	///
	/// \brief uvs
	/// \return 
	///
	std::vector<v2s> uvs() const;
	///
	/// \brief setUvs
	/// \param uvs
	///
	void setUvs(const std::vector<v2s> &uvs);
	///
	/// \brief vertexCount
	/// \return 
	///
	unsigned long vertexCount() const;
	///
	/// \brief triangleCount
	/// \return 
	///
	unsigned long triangleCount() const;
	///
	/// \brief vertexNormalsCount
	/// \return 
	///
	unsigned long vertexNormalsCount() const;
	///
	/// \brief uvsCount
	/// \return 
	///
	unsigned long uvsCount() const;
	///
	/// \brief constructionPath
	/// \return 
	///
	std::string constructionPath() const;
	///
	/// \brief setConstructionPath
	/// \param constructionPath
	///
	void setConstructionPath(const std::string &constructionPath);
	
	// vxGeometry interface
	// Geometry interface
	virtual void updateBoundingBox() override;

	virtual bool throwRay(const Ray &ray) const override;
	virtual int throwRay(const Ray &ray, Collision &collide) const override;
	virtual bool hasCollision(const Ray &ray) const override;

	/// Reference GeoGrid walk (VXR_MESH=ref). Kept so A/B can match old images.
	const GeoGrid &grid() const { return m_grid; }
	const MeshBvh &meshBvh() const { return m_bvh; }

private:
	int throwRayRef(const Ray &ray, Collision &col) const;
	int throwRayCsr(const Ray &ray, Collision &col) const;
	int throwRayBvh(const Ray &ray, Collision &col) const;
	bool hasCollisionRef(const Ray &ray) const;
	bool hasCollisionCsr(const Ray &ray) const;
	bool hasCollisionBvh(const Ray &ray) const;
};

using vxTriangleMeshHandle = std::shared_ptr<TriangleMesh>;

}
#endif // VXTRIANGLEMESH_H
