#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <vector>
#include "vxVector.h"
#include "vxRay.h"
#include "vxCollision.h"
#include "vxTriRef.h"

namespace vxCore {


class vxGeometry
{
public:

	vxGeometry();

	std::vector<vxVector3d> m_vertices;
	std::vector<vxTriRef> m_triangles;
	std::vector<vxVector3d> m_normals;

	void addVertex(const vxVector3d &v3);
	void addTriangle(unsigned int a,
					 unsigned int b,
					 unsigned int c);

	unsigned int vertexCount() const;
	unsigned int triangleCount() const;

	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif // VXGEOMETRY_H
