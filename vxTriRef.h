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
	vxTriRef(const vxVector3d& ah,
			 const vxVector3d& b,
			 const vxVector3d& c);
	
	vxTriRef(vxTriRef&& other);
	
	const vxVector3d &p1;
	const vxVector3d &p2;
	const vxVector3d &p3;
	vxVector3d n;
	double ah{0.0};
	double &computeArea();
	
	vxVector3d &computeNormal();
	vxVector3d normal() const;

	double area() const;
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;

	friend std::ostream& operator<<(std::ostream &os, const vxTriRef& t)
	{
		return os << "a: " << t.p1 << std::endl 
				  << "b: " << t.p2 << std::endl 
				  << "c: " << t.p1 << std::endl
				  << "area: " << t.ah << std::endl 
				  << "normal: " << t.n << std::endl ;
	}
};

}
#endif // VXTRIREF_H
