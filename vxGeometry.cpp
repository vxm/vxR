#include "vxGeometry.h"

using namespace vxCore;

vxGeometry::vxGeometry()
{
	
}

void vxGeometry::addVertex(const vxVector3d &v3)
{
	m_vertices.push_back(v3);
}

void vxGeometry::addTriangle(unsigned int a, unsigned int b, unsigned int c)
{
	m_triangles.push_back(int3(a,b,c));
}

unsigned int vxGeometry::vertexCount() const
{
	return m_vertices.size();
}

unsigned int vxGeometry::triangleCount() const
{
	return m_triangles.size();
}

