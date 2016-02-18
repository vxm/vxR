#ifndef VXTRIANGLEMESH_H
#define VXTRIANGLEMESH_H

#include "vxGeometry.h"

namespace vxCore {


class vxTriangleMesh final : public vxGeometry
{
	std::string m_constructionPath;
	std::atomic_bool m_openForEdition{false};
	vxGeoGrid m_grid;

public:
	vxTriangleMesh();
	
	std::vector<v3s> m_vertices;
	std::vector<vxTriRef> m_triangles;
	std::vector<v3s> m_normals;
	
	void updateAccelerationStuctures();

	void clear();
	void open();
	void close();
	
	void addVertexTransformed(const v3s &v3s);
	void addTriangle(unsigned long a,
					 unsigned long b,
					 unsigned long c);

	unsigned long vertexCount() const;
	unsigned long triangleCount() const;

	std::string constructionPath() const;
	void setConstructionPath(const std::string &constructionPath);
	
	// vxGeometry interface
	virtual bool throwRay(const vxRay &ray) const override;
	virtual int throwRay(const vxRay &ray, vxCollision &collide) const override;
	virtual bool hasCollision(const vxRay &ray) const override;
};

using vxTriangleMeshHandle = std::shared_ptr<vxTriangleMesh>;

}
#endif // VXTRIANGLEMESH_H
