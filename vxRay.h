#ifndef VXRAY_H
#define VXRAY_H

#include "vxVector.h"

namespace vxCore{

class vxRay
{
private:
	vxVector3d m_direction{1.0,0.0,0.0};
	vxVector3d m_origin{0.0,0.0,0.0};

public:
	vxRay ();
	vxRay (const vxVector3d& direction);
	vxRay (const vxVector3d& origin, 
			  const vxVector3d& direction);
	vxRay (double x, double y, double z);
	vxRay (const vxRay&& ray)
		:m_direction{std::move(ray.m_direction)}
		,m_origin{std::move(ray.m_origin)}
	{
	}

	vxRay (const vxRay& ray)
		:m_direction{ray.m_direction}
		,m_origin{ray.m_origin}
	{
	}

	~vxRay(){}

	vxVector3d origin() const;
	void setOrigin(const vxVector3d &origin);

	vxVector3d direction() const;
	vxVector3d& direction();
	void setDirection(const vxVector3d &direction);
};

}
#endif // VXRAY_H
