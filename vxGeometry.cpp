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
	m_triangles.push_back((vxTriRef(m_vertices[a],
											   m_vertices[b],
											   m_vertices[c])));
}

unsigned int vxGeometry::vertexCount() const
{
	return m_vertices.size();
}

unsigned int vxGeometry::triangleCount() const
{
	return m_triangles.size();
}

bool vxGeometry::throwRay(const vxRay &ray) const
{
	return false;
}

int vxGeometry::throwRay(const vxRay &ray, vxCollision &collide) const
{
	return 0;
}

bool vxGeometry::hasCollision(const vxRay &ray) const
{
	return false;
}

