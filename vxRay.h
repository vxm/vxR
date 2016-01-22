#ifndef VXRAY_H
#define VXRAY_H

#include "vxVector.h"

namespace vxCore{

class vxRay
{
private:
	v3 m_direction{1.0,0.0,0.0};
	v3 m_origin{0.0,0.0,0.0};

public:
	vxRay();
	vxRay(const v3& direction);
	vxRay(const v3& origin, const v3& direction);
	vxRay(scalar x, scalar y, scalar z);
	vxRay(const vxRay&& ray);
	vxRay(const vxRay& ray);
	~vxRay(){}

	v3 origin() const;
	void setOrigin(const v3 &origin);

	v3 direction() const;
	v3& direction();
	void setDirection(const v3 &direction);

	///
	/// \brief incidence
	/// \param normal
	/// \return 
	///from -1 to +1, it is an angle ratio between 
	/// vectors
	scalar incidence(const v3 &normal) const;

	///
	/// \brief operator <<
	/// \param os
	/// \param ray
	/// \return 
	///It will "print out" the ray vectors.
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
