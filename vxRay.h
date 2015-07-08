#ifndef VXRAY_H
#define VXRAY_H

#include "vxVector.h"

namespace vxCore{

class vxRay
{
private:
		vxVector3d m_origin;
		vxVector3d m_direction;

public:
		vxRay ();
		vxRay (const vxVector3d& other);
		vxRay (const vxVector3d& origin, 
				  const vxVector3d& direction);
		
		vxRay (double x, double y, double z);
		
		vxVector3d origin() const;
		void setOrigin(const vxVector3d &origin);
		vxVector3d direction() const;
		vxVector3d& direction();
		void setDirection(const vxVector3d &direction);
};
}
#endif // VXRAY_H
