#ifndef VXTRIREF_H
#define VXTRIREF_H
#include <atomic>
#include <memory>
#include "vxVector.h"
#include "vxRay.h"
#include "vxCollision.h"


namespace vxCore
{

class vxTriRef
{
	
public:
	vxTriRef(const v3s& a,
			 const v3s& b,
			 const v3s& c, 
			 const v3s &in1, 
			 const v3s &in2, 
			 const v3s &in3, 
			 const v2s &iuv1, 
			 const v2s &iuv2,
			 const v2s &iuv3);
	
	vxTriRef(vxTriRef&& other);
	///
	/// \brief p1
	///
	const v3s &p1;
	///
	/// \brief p2
	///
	const v3s &p2;
	///
	/// \brief p3
	///
	const v3s &p3;
	///
	/// \brief n1
	///
	const v3s &n1;
	///
	/// \brief n2
	///
	const v3s &n2;
	///
	/// \brief n3
	///
	const v3s &n3;
	///
	/// \brief uv1
	///
	const v2s &uv1;
	///
	/// \brief uv2
	///
	const v2s &uv2;
	///
	/// \brief uv3
	///
	const v2s &uv3;
	///
	scalar ah{0.0};
	///
	/// \brief m_c_h1
	///caches the first height
	scalar m_c_h1;
	///caches a second height
	scalar m_c_h2;
	///
	/// \brief computeArea
	/// \return 
	/// computes the area of this triangle
	/// returns it and stores it.
	scalar &computeArea();
	///
	/// \brief normal
	/// \return 
	/// Returns the main normal
	v3s normal() const;
	///
	/// \brief area
	/// \return 
	///
	scalar area() const;
	///
	/// \brief area
	/// \return 
	///
	scalar &area();
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;

	friend std::ostream& operator<<(std::ostream &os, const vxTriRef& t)
	{
		return os << "a: " << t.p1 << std::endl 
				  << "b: " << t.p2 << std::endl 
				  << "c: " << t.p3 << std::endl 
				  << "area: " << t.ah << std::endl 
				  << "normal 1: " << t.n1 << std::endl 
				  << "normal 2: " << t.n2 << std::endl 
				  << "normal 3: " << t.n3 << std::endl;
	}
	void averageNormalsAndNormalize();
};

}
#endif // VXTRIREF_H
