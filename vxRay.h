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
	vxRay (const vxRay&& ray);
	vxRay (const vxRay& ray);
	~vxRay(){}

	vxVector3d origin() const;
	void setOrigin(const vxVector3d &origin);


	vxVector3d direction() const;
	vxVector3d& direction();
	void setDirection(const vxVector3d &direction);

	double incidence(const vxVector3d &normal) const;

	friend std::ostream& operator<<(std::ostream &os, const vxRay& ray)
	{
		const auto &org = ray.origin();
		const auto &dr = ray.direction();

		return os << org.x()
					  << " " 
					  << org.y()
					  << " " 
					  << org.z()
					  << ", " 
					  << dr.x()
					  << " " 
					  << dr.y()
					  << " " 
					  << dr.z();
	}

};

}
#endif // VXRAY_H
