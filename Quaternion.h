#ifndef Quaternion_H
#define Quaternion_H
#include "vxVector.h"

namespace vxCore {

class Quaternion
{
	scalar x;
	scalar y;
	scalar z;
	scalar w;
	
public:
	Quaternion();
	///
	/// \brief Quaternion
	/// \param ix
	/// \param iy
	/// \param iz
	/// \param iw
	///
	Quaternion(scalar ix,
			   scalar iy,
			   scalar iz,
			   scalar iw);
	///
	/// \brief Quaternion
	/// \param axis
	/// \param angle
	///
	Quaternion(const v3s& axis, const scalar angle);
	///
	/// \brief set
	/// \param axis
	/// \param angle
	///
	void set(const v3s &axis, const scalar angle);
	///
	/// \brief set
	/// \param ix
	/// \param iy
	/// \param iz
	/// \param iw
	///
	void set(scalar ix,
			 scalar iy,
			 scalar iz,
			 scalar iw);
	
};

}
#endif // Quaternion_H
