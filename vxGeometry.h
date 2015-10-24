#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <vector>
#include "vxVector.h"
#include "vxRay.h"
#include "vxCollision.h"
#include "vxTriRef.h"
#include "vxMatrix.h"
#include "vxBoundingBox.h"

namespace vxCore {

class vxGeometry
{
	std::string m_constructionPath;
	vxMatrix m_transform;
	vxBoundingBox m_bb;
	
public:

	vxGeometry();

	std::vector<vxVector3d> m_vertices;
	std::vector<vxTriRef> m_triangles;
	std::vector<vxVector3d> m_normals;

	void addVertexTransformed(const vxVector3d &v3);
	void addTriangle(unsigned int a,
					 unsigned int b,
					 unsigned int c);

	unsigned int vertexCount() const;
	unsigned int triangleCount() const;

	//renderable interface
	bool throwRay(const vxRay &) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
	std::string constructionPath() const;
	void setConstructionPath(const std::string &constructionPath);
	vxMatrix transform() const;
	void setTransform(const vxMatrix &transform);
};

}
#endif // VXGEOMETRY_H
