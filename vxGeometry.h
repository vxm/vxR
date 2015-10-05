#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <vector>
#include "vxVector.h"

namespace vxCore {


struct int3
{
	int3(unsigned int ia,
		 unsigned int ib,
		 unsigned int ic)
		:a{ia}
		,b{ib}
		,c{ic}
	{
	}
	
	unsigned int a;
	unsigned int b;
	unsigned int c;
};

class vxGeometry
{
public:

	vxGeometry();

	std::vector<vxVector3d> m_vertices;
	std::vector<int3> m_triangles;
	std::vector<vxVector3d> m_normals;

	void addVertex(const vxVector3d &v3);
	void addTriangle(unsigned int a,
					 unsigned int b,
					 unsigned int c)
	{
		m_triangles.push_back(int3(a,b,c));
	}

	unsigned int vertexCount() const;
	unsigned int triangleCount() const;
};

}
#endif // VXGEOMETRY_H
