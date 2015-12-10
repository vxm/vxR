#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <typeinfo>
#include <vector>
#include <atomic>
#include "vxVector.h"
#include "vxRay.h"
#include "vxCollision.h"
#include "vxTriRef.h"
#include "vxMatrix.h"
#include "vxBoundingBox.h"
#include "vxGeoGrid.h"
#include "vxColor.h"
#include <set>

namespace vxCore {

class vxGeometry;

using vxGeometryHandle = std::shared_ptr<vxGeometry>;

class vxGeometry
{
	std::string m_constructionPath;
	vxMatrix m_transform;
	std::shared_ptr<vxBoundingBox> m_bb;
	vxGeoGrid m_grid;
	std::atomic_bool m_openForEdition{false};
	vxColor m_baseColor;
	
public:

	vxGeometry();

	std::vector<v3> m_vertices;
	std::vector<vxTriRef> m_triangles;
	std::vector<v3> m_normals;

	void clear()
	{
		m_vertices.clear();
		m_triangles.clear();
		m_normals.clear();
	}
	void open();
	void close();
	
	void addVertexTransformed(const v3 &v3);
	void addTriangle(unsigned long a,
					 unsigned long b,
					 unsigned long c);

	unsigned long vertexCount() const;
	unsigned long triangleCount() const;

	std::string constructionPath() const;
	void setConstructionPath(const std::string &constructionPath);
	vxMatrix transform() const;
	vxMatrix& transform();

	void setTransform(const vxMatrix &transform);
	void updateAccelerationStuctures();

	std::shared_ptr<vxBoundingBox> boundingBox();
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
	vxColor baseColor() const;
	void setBaseColor(const vxColor &baseColor);
};

}
#endif // VXGEOMETRY_H
