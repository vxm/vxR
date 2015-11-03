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
	vxTriRef(const v3& a,
			 const v3& b,
			 const v3& c);
	
	vxTriRef(vxTriRef&& other);
	
	const v3 &p1;
	const v3 &p2;
	const v3 &p3;
	v3 n;
	scalar ah{0.0};
	scalar &computeArea();
	
	v3 &computeNormal();
	v3 normal() const;

	scalar area() const;
	
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
				  << "normal: " << t.n << std::endl ;
	}
};

}
#endif // VXTRIREF_H
