#ifndef VXCLOCK_H
#define VXCLOCK_H

#include "vxVector.h"
namespace vxCore
{

class vxClock
{
	scalar m_time;
	
public:
	vxClock();
	
	scalar time() const;
	void setTime(const scalar &time);
};


}
#endif // VXCLOCK_H
