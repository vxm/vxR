#include "vxGeometry.h"

using namespace vxCore;

vxGeometry::vxGeometry()
{
	
}

std::string vxGeometry::constructionPath() const
{
	return m_constructionPath;
}

void vxGeometry::setConstructionPath(const std::string &constructionPath)
{
	m_constructionPath = constructionPath;
}

void vxGeometry::setTransform(const vxMatrix &transform)
{
	m_transform = transform;
}

void vxGeometry::addVertexTransformed(const vxVector3d &v3)
{
	const auto &orig = m_transform.getOrigin();
	const auto &newPoint = v3+orig;
	m_bb.extend(newPoint);
	//TODO:push_back ?
	m_vertices.push_back(newPoint);
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

bool vxGeometry::throwRay(const vxRay &) const
{
	return false;
}

int vxGeometry::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(m_bb.hasCollision(ray))
	{
		collide.setValid();
		return 1;
	}
	
	return 0;
	
	for(const auto &tri:m_triangles)
	{
		if(tri.throwRay(ray,collide)==1)
		{
			return 1;
		}
	}
	return 0;
}

bool vxGeometry::hasCollision(const vxRay &ray) const
{
	return false;
}

