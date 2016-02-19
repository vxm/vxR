#ifndef VXRAY_H
#define VXRAY_H

#include "vxVector.h"

namespace vxCore{

class vxRay
{
private:
	v3s m_direction{1.0,0.0,0.0};
	v3s m_origin{0.0,0.0,0.0};

public:
	vxRay();
	vxRay(const v3s& direction);
	vxRay(const v3s& origin, const v3s& direction);
	vxRay(scalar x, scalar y, scalar z);
	vxRay(const vxRay&& ray);
	vxRay(const vxRay& ray);
	~vxRay(){}

	v3s origin() const;
	void setOrigin(const v3s &origin);

	v3s direction() const;
	v3s& direction();
	void setDirection(const v3s &direction);

	///
	/// \brief incidence
	/// \param normal
	/// \return 
	///from -1 to +1, it is an angle ratio between 
	/// vectors
	scalar incidence(const v3s &normal) const;

	///
	/// \brief distance
	/// \param final
	/// \return 
	///Computes the distance to the final vector 
	/// from the origin.
	scalar distance(const v3s &final);
	
	///
	/// \brief compareDistance
	/// \param a
	/// \param b
	/// \return 
	///Compares distance from origin to these.
	bool compareDistance(const v3s &a,const v3s &b);
	
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
