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
			 const v3s& c);
	
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
	
	v3s n1{v3s::zero};
	v3s n2{v3s::zero};
	v3s n3{v3s::zero};
	
	scalar ah{0.0};
	
	scalar m_c_h1;
	scalar m_c_h2;

	scalar &computeArea();
	
	v3s computeNormals();
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
