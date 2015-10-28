#include "vxGeometry.h"

#define DRAW_BB 0

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

void vxGeometry::addTriangle(unsigned long a, unsigned long b, unsigned long c)
{
	//TODO:push_back ?
	m_triangles.push_back((vxTriRef(m_vertices[a],
									m_vertices[b],
									m_vertices[c])));
	auto &&tref = m_triangles[m_triangles.size()-1];
	tref.computeNormal();
	tref.computeArea();

}

unsigned long vxGeometry::vertexCount() const
{
	return m_vertices.size();
}

unsigned long vxGeometry::triangleCount() const
{
	return m_triangles.size();
}

bool vxGeometry::throwRay(const vxRay &) const
{
	return false;
}

int vxGeometry::throwRay(const vxRay &ray, vxCollision &collide) const
{
#if	DRAW_BB
	if(m_bb.hasCollision(ray))
	{
		collide.setValid(true);
		return 1;
	}

#else
	if(!m_bb.hasCollision(ray))
	{
		return 0;
	}

	//std::cout << "Visiting triangles " << std::endl;	
	for(auto it = std::cbegin(m_triangles);
				it!=std::cend(m_triangles);
									 ++it)
	{
		if(it->throwRay(ray,collide))
		{
			return 1;
		}
	}
#endif
	return 0;
}

bool vxGeometry::hasCollision(const vxRay &) const
{
	return false;
}

