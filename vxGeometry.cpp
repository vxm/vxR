#include "vxGeometry.h"

#define DRAW_BB 0

using namespace vxCore;

vxColor vxGeometry::baseColor() const
{
	return m_baseColor;
}

void vxGeometry::setBaseColor(const vxColor &baseColor)
{
	m_baseColor = baseColor;
}
/*
std::shared_ptr<vxShader> vxGeometry::shader() const
{
	return m_shader;
}

void vxGeometry::setShader(const std::shared_ptr<vxShader> &shader)
{
	m_shader = shader;
}*/

vxGeometry::vxGeometry()
{
	m_bb = std::make_shared<vxBoundingBox>();
}

void vxGeometry::clear()
{
	m_vertices.clear();
	m_triangles.clear();
	m_normals.clear();
}

void vxGeometry::open()
{
	m_openForEdition = true;
}

void vxGeometry::close()
{
	auto triangleID{0ul};

	updateAccelerationStuctures();
	
	for(auto &tref:m_triangles)
	{
		tref.computeNormal();
		tref.computeArea();

		m_grid.locateAndRegister(tref, triangleID);
		triangleID++;
	}
	
	m_openForEdition = false;
}

void vxGeometry::updateAccelerationStuctures()
{
	m_grid.setBb(m_bb);
	m_bb->close();
}

std::shared_ptr<vxBoundingBox> vxGeometry::boundingBox()
{
	return m_bb;
}

std::string vxGeometry::constructionPath() const
{
	return m_constructionPath;
}

void vxGeometry::setConstructionPath(const std::string &constructionPath)
{
	m_constructionPath = constructionPath;
}

vxMatrix vxGeometry::transform() const
{
	return m_transform;
}

vxMatrix& vxGeometry::transform()
{
	return m_transform;
}

void vxGeometry::setTransform(const vxMatrix &transform)
{
	m_transform = transform;
}

void vxGeometry::addVertexTransformed(const v3 &v3)
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
	auto col = vxCollision();
	return throwRay(ray, col);
}

int vxGeometry::throwRay(const vxRay &ray, vxCollision &col) const
{
#if	DRAW_BB
	if(m_bb->throwRay(ray, col))
	{
		col.setColor(m_baseColor);
		col.setValid(true);

		return 1;
	}
#else
	if(!m_bb->throwRay(ray, col))
	{
		return 0;
	}
	
	auto&& p = ray.origin();
	
	
	std::vector<vxCollision> cols;
	auto sp =  col.position() 
			+ (col.normal().inverted() / (scalar)10000.0);

	auto prev = m_grid.size();
	searchResult triangles;

	do
	{
		triangles = m_grid.getList(ray, sp);

		if(prev==triangles.index)
		{
			break;
		}
		
		prev = triangles.index;
		
		for(const auto &id: *(triangles.listRef) )
		{
			if(m_triangles[id].throwRay(ray,col))
			{
				if(!ray.direction().follows((col.position()-ray.origin()).unit()))
				{
					cols.push_back(col);
				}
			}
		}
	}
	while(!cols.size());

	if(cols.size())
	{
		auto mind  = (cols[0].position()-p).length();
		col = cols[0];
		int i{0};
		for(auto&& c:cols)
		{
			i++;
			auto ds = (c.position()-p).length();
			if( ds < mind )
			{
				col = c;
			}
		}

		col.setColor(m_baseColor);
		col.setValid(true);
		col.setUV(v2(0.5,0.5));
		return 1;
	}
	
#endif
	return 0;
}

bool vxGeometry::hasCollision(const vxRay &ray) const
{
	vxCollision col;

	if(throwRay(ray,col)==1)
	{
		return true;
	}
	
	return false;
}

