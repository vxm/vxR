#include "TriangleMesh.h"
#define DRAWBBOX 0

using namespace vxCore;

const v2s TriangleMesh::defaultUV = {0.5,0.5};

TriangleMesh::TriangleMesh()
	:Geometry()
{
}

std::vector<TriRef>& TriangleMesh::triangles()
{
	return m_triangles;
}

std::vector<v2s>& TriangleMesh::uvs()
{
	return m_uvs;
}

std::vector<v2s> TriangleMesh::uvs() const
{
	return m_uvs;
}

void TriangleMesh::setUvs(const std::vector<v2s> &uvs)
{
	m_uvs = uvs;
}

std::vector<v3s>& TriangleMesh::vertexNormals()
{
	return m_vertexNormals;
}

std::vector<v3s> TriangleMesh::vertexNormals() const
{
	return m_vertexNormals;
}

void TriangleMesh::setVertexNormals(const std::vector<v3s> &vertexNormals)
{
	m_vertexNormals = vertexNormals;
}

std::vector<v3s> TriangleMesh::vertices() const
{
	return m_vertices;
}

std::vector<v3s>& TriangleMesh::vertices()
{
	return m_vertices;
}

void TriangleMesh::setVertices(const std::vector<v3s> &vertices)
{
	m_vertices = vertices;
}


void TriangleMesh::clear()
{
	m_vertices.clear();
	m_triangles.clear();
}

void TriangleMesh::open()
{
	m_openForEdition = true;
}

void TriangleMesh::close()
{
	auto triangleID = 0ul;

	updateAccelerationStuctures();
	
	for(auto &tref:m_triangles)
	{
		m_grid.locateAndRegister(tref, triangleID);
		triangleID++;
	}
	
	m_openForEdition = false;
}

void TriangleMesh::updateAccelerationStuctures()
{
	m_grid.setBb(m_bb);
	m_bb->close();
}


std::string TriangleMesh::constructionPath() const
{
	return m_constructionPath;
}

void TriangleMesh::setConstructionPath(const std::string &constructionPath)
{
	m_constructionPath = constructionPath;
}

void TriangleMesh::addVertexTransformed(const v3s &v3)
{
	if(!m_openForEdition)
	{
		std::cerr << "You tried to edit a geometry when is closed for edition" << std::endl;
		return;
	}
	
	const auto &&orig = m_transform.origin();
	const auto &&scale = m_transform.getScale();
	const auto &newPoint = (v3*scale)+orig;
	m_bb->extend(newPoint);
	m_vertices.emplace_back(newPoint);
}

void TriangleMesh::addVertexNormalTransformed(const v3s &normal)
{
	if(!m_openForEdition)
	{
		std::cerr << "You tried to edit a geometry when is closed for edition" << std::endl;
		return;
	}
	
	//TODO: get rotation matrix and transform normal.
	
	//TODO:emplace_back ?
	m_vertexNormals.emplace_back(normal);
}

void TriangleMesh::addUV(const v2s &uv)
{
	if(!m_openForEdition)
	{
		std::cerr << "You tried to edit a geometry when is closed for edition" << std::endl;
		return;
	}
	
	m_uvs.push_back(uv);
}


TriRef& TriangleMesh::addTriangle(unsigned long a, 
									  unsigned long b, 
									  unsigned long c)
{
	if(!m_openForEdition)
	{
		std::cerr << "You are editing a geometry when is closed for edition" << std::endl;
	}
	
	if(m_uvs.empty())
	{
		std::cerr << "Geometries cannot have 0 uv's" << std::endl;
	}
		
	auto&& uvRefA = m_uvs.size() ? m_uvs[a] : defaultUV;
	auto&& uvRefB = m_uvs.size() ? m_uvs[b] : defaultUV;
	auto&& uvRefC = m_uvs.size() ? m_uvs[c] : defaultUV;
	
	//TODO:emplace_back ?
	m_triangles.emplace_back(m_vertices[a],	m_vertices[b], m_vertices[c],
							 m_vertexNormals[a], m_vertexNormals[b], m_vertexNormals[c],
							 uvRefA, uvRefB, uvRefC);
	return m_triangles.back();
}

unsigned long TriangleMesh::vertexCount() const
{
	return m_vertices.size();
}

unsigned long TriangleMesh::triangleCount() const
{
	return m_triangles.size();
}

unsigned long TriangleMesh::vertexNormalsCount() const
{
	return m_vertexNormals.size();
}

unsigned long TriangleMesh::uvsCount() const
{
	return m_uvs.size();
}


bool TriangleMesh::throwRay(const Ray &ray) const
{
	return Geometry::throwRay(ray);
}

int TriangleMesh::throwRay(const Ray &ray, Collision &col) const
{
#if	DRAWBBOX
	col.setColor(m_baseColor);
	col.setValid(true);
	
	return 1;
#else
	auto&& p = ray.origin();
	
	std::vector<Collision> cols;
	auto&& sp =  col.position();
//			+ (col.normal().inverted() / (scalar)10000.0);
	
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
					cols.emplace_back(col);
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
		return 1;
	}
	
#endif
	col.setValid(false);
	return 0;
}

bool TriangleMesh::hasCollision(const Ray &ray) const
{
	return Geometry::hasCollision(ray);
}

