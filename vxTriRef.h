#ifndef VXTRIREF_H
#define VXTRIREF_H
#include <atomic>
#include <memory>
#include "vxVector.h"

namespace vxCore
{

class vxTriRef
{
	
public:
	vxTriRef(const vxVector3d& a,
			 const vxVector3d& b,
			 const vxVector3d& c);
	
	vxTriRef(vxTriRef&& other);
	
	const vxVector3d &p1;
	const vxVector3d &p2;
	const vxVector3d &p3;
	std::atomic<bool> normal;
	vxVector3d n;
	
	vxVector3d &getNormal();
};

}
#endif // VXTRIREF_H
