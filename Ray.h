#ifndef VXRAY_H
#define VXRAY_H

#include "Vector.h"
//#include "visible.h"

namespace vxCore
{

enum class VisionType
{
	kOpaque,
	kSemitransparent,
	kLight,
	kAll
};

class Ray
{
private:
	v3s m_direction{1.0, 0.0, 0.0};
	v3s m_origin{0.0, 0.0, 0.0};

	scalar m_length = -1.0;

public:
	Ray();
	Ray(const v3s &direction);
	Ray(const v3s &origin, const v3s &direction, VisionType vis);
	Ray(const v3s &origin, const v3s &direction);
	Ray(scalar x, scalar y, scalar z);
	Ray(const Ray &&ray);
	Ray(const Ray &ray);
	~Ray() {}

	VisionType m_vision = VisionType::kAll;
	v3s origin() const;
	void setOrigin(const v3s &origin);

	v3s direction() const;
	v3s &direction();
	void setDirection(const v3s &direction);

	///
	/// \brief incidence
	/// \param normal
	/// \return
	/// from -1 to +1, it is an angle ratio between
	/// vectors
	scalar incidence(const v3s &normal) const;

	///
	/// \brief distance
	/// \param final
	/// \return
	/// Computes the distance to the final vector
	/// from the origin.
	scalar distance(const v3s &final) const;

	///
	/// \brief isCloser
	/// \param a
	/// \param b
	/// \return
	/// Compares distance from origin to these.
	bool isCloser(const v3s &a, const v3s &b) const;

	///
	/// \brief operator <<
	/// \param os
	/// \param ray
	/// \return
	/// It will "print out" the ray vectors.
	friend std::ostream &operator<<(std::ostream &os, const Ray &ray)
	{
		const auto &org = ray.origin();
		const auto &dr = ray.direction();

		return os << org.x() << " " << org.y() << " " << org.z() << ", " << dr.x()
		          << " " << dr.y() << " " << dr.z();
	}
	scalar length() const;
	void setLength(const scalar &length);
};
}
#endif // VXRAY_H
