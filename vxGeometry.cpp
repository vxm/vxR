#include "vxGeometry.h"

#define DRAW_BB 0

using namespace vxCore;


vxGeometry::vxGeometry()
{
	m_bb = std::make_shared<vxBoundingBox>();
}

void vxGeometry::open()
{
	m_openForEdition = true;
}

void vxGeometry::close()
{
	unsigned long triangleID;
	m_grid.setBb(m_bb);
	for(auto &&tref:m_triangles)
	{
		tref.computeNormal();
		tref.computeArea();

		m_grid.locateAndRegister(tref, triangleID);

		triangleID++;
	}
	
	updateAccelerationStuctures();
	
	m_openForEdition = false;
}

void vxGeometry::updateAccelerationStuctures()
{
	
	
	m_bb->close();
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
	if(!m_openForEdition)
	{
		std::cerr << "You tried to edit a geometry when is closed for edition" << std::endl;
		return;
	}
	
	const auto &orig = m_transform.getOrigin();
	const auto &newPoint = v3+orig;
	m_bb->extend(newPoint);
	//TODO:push_back ?
	m_vertices.push_back(newPoint);
}

void vxGeometry::addTriangle(unsigned long a, unsigned long b, unsigned long c)
{
	if(!m_openForEdition)
	{
		std::cerr << "You tried to edit a geometry when is closed for edition" << std::endl;
		return;
	}

	//TODO:push_back ?
	m_triangles.push_back((vxTriRef(m_vertices[a],
									m_vertices[b],
									m_vertices[c])));
}

unsigned long vxGeometry::vertexCount() const
{
	return m_vertices.size();
}

unsigned long vxGeometry::triangleCount() const
{
	return m_triangles.size();
}

bool vxGeometry::throwRay(const vxRay &ray) const
{
	if(!m_bb->hasCollision(ray))
	{
		return 0;
	}

	//std::cout << "Visiting triangles " << std::endl;	
	for(auto&& it = std::cbegin(m_triangles);
				it!=std::cend(m_triangles);
									 ++it)
	{
		if(it->throwRay(ray))
		{
			return 1;
		}
	}
	
	return 0;
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
	if(!m_bb->hasCollision(ray))
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
			collide.setColor(vxColor::white);
			collide.setValid(true);
			collide.setUV(vxVector2d(0.5,0.5));
			return 1;
		}
	}
#endif
	return 0;
}

bool vxGeometry::hasCollision(const vxRay &ray) const
{
	if(!m_bb->hasCollision(ray))
	{
		return false;
	}

	//std::cout << "Visiting triangles " << std::endl;	
	for(auto it = std::cbegin(m_triangles);
				it!=std::cend(m_triangles);
									 ++it)
	{
		if(it->throwRay(ray))
		{
			return true;
		}
	}

	return false;

}

