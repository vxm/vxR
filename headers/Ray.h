#ifndef VXRAY_H
#define VXRAY_H

#include "Vector.h"
//#include "visible.h"

namespace vxCore
{

enum class Quadrant
{
	k1,
	k2,
	k3,
	k4,
	k5,
	k6,
	k7,
	k8,
};

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

	scalar m_length = 1.0;

	Quadrant m_qd = Quadrant::k1;

public:
	Ray() = default;
	Ray(const v3s &direction);
	Ray(const v3s &origin, const v3s &direction, VisionType vis);
	Ray(const v3s &origin, const v3s &direction);
	Ray(scalar x, scalar y, scalar z);
	Ray(const Ray &&ray);
	Ray(const Ray &ray);
	~Ray() {}

	void placeQuadrant();

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

	v3s nextPosition() const;

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
	Quadrant qd() const;
	void setQd(const Quadrant &qd);
};

///
/// A ray prepared for slab tests against many boxes: origin and the inverse of
/// each direction component, computed once per ray.
///
/// Components of the direction smaller than 1e-12 are given a finite inverse of
/// +-1e12 (the sign of the component, + for zero) instead of +-infinity, so the
/// slab arithmetic never produces inf or NaN. That matters because the
/// renderer builds with -ffast-math, where tests on inf/NaN are unreliable.
///
/// Copy only the numbers it needs. It does not hold a reference to the Ray.
struct RayInv
{
	static constexpr scalar kMaxInverse = scalar(1e12);

	scalar ox{0.0}, oy{0.0}, oz{0.0};
	scalar ix{kMaxInverse}, iy{kMaxInverse}, iz{kMaxInverse};
	/// Length of the direction, converts the slab parameter into a distance.
	scalar dirLen{1.0};
	/// 1 when the inverse of that axis is negative.
	int sx{0}, sy{0}, sz{0};

	RayInv() = default;
	explicit RayInv(const Ray &ray)
	{
		const v3s o = ray.origin();
		const v3s d = ray.direction();
		ox = o.x();
		oy = o.y();
		oz = o.z();
		ix = invert(d.x());
		iy = invert(d.y());
		iz = invert(d.z());
		sx = ix < 0.0 ? 1 : 0;
		sy = iy < 0.0 ? 1 : 0;
		sz = iz < 0.0 ? 1 : 0;
		dirLen = d.length();
	}

	/// 1/d with |result| <= kMaxInverse and no infinities.
	static scalar invert(scalar d)
	{
		const scalar a = d < 0.0 ? -d : d;
		if (a < scalar(1.0) / kMaxInverse)
		{
			return d < 0.0 ? -kMaxInverse : kMaxInverse;
		}
		return scalar(1.0) / d;
	}
};
} // namespace vxCore
#endif // VXRAY_H
