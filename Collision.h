#ifndef VXCOLLISION_H
#define VXCOLLISION_H

#include "Color.h"
#include "Point.h"
#include "Vector.h"

namespace vxCore
{

class Visible;
class Ray;

class Collision
{
private:
	v3s m_position;
	v3s m_normal;
	Color m_color;
	v2s m_uv;

	bool m_valid{false};
	scalar m_maxDistance = 10000000.0;

public:
	Visible *m_geo{nullptr};

	Collision() = default;

	Collision(const v3s &p, const v3s &n = {0, 0, 0},
			  const Color &c = {0, 0, 0, 1}, const v2s &uv = {.5, .5});
	///
	/// \brief setAlphaValue
	/// \param a
	/// Sets the alpha value in the color value.
	void setAlphaValue(const scalar a);
	///
	/// \brief reset
	///
	void reset();
	///
	/// \brief setValid
	/// \param v
	///
	void setValid(bool v = true);
	///
	/// \brief isValid
	/// \return
	///
	bool isValid() const;
	///
	/// \brief setPosition
	/// \param val
	///
	void setPosition(const v3s &val);
	///
	/// \brief setPosition
	/// \param x
	/// \param y
	/// \param z
	///
	void setPosition(scalar x, scalar y, scalar z);
	///
	/// \brief position
	/// \return
	///
	v3s position() const;
	///
	/// \brief setColor
	/// \param val
	///
	void setColor(const v3s &val);
	///
	/// \brief setColor
	/// \param val
	///
	void setColor(const Color &val);
	///
	/// \brief setColor
	/// \param r
	/// \param g
	/// \param b
	///
	void setColor(scalar r, scalar g, scalar b);
	///
	/// \brief color
	/// \return
	Color color() const;
	///
	/// \brief setNormal
	/// \param val
	///
	void setNormal(const v3s &val);
	///
	/// \brief normal
	/// \return
	///
	v3s normal() const;
	///
	/// \brief setUV
	/// \param uvVec
	///
	void setUV(const v2s &uvVec);
	///
	/// \brief setU
	/// \param u
	///
	void setU(scalar u);
	///
	/// \brief u
	/// \return
	///
	scalar u() const;
	///
	/// \brief setV
	/// \param v
	///
	void setV(scalar v);
	///
	/// \brief v
	/// \return
	///
	scalar v() const;
	///
	/// \brief uv
	/// \return
	///
	v2s uv() const;

	Ray nextRay() const;

	// TODO:complete this.
	friend std::ostream &operator<<(std::ostream &os, const Collision &v)
	{
		return os << v.m_position << ", " << v.m_normal << ", " << v.m_color << ", "
				  << v.m_uv;
	}
	scalar maxDistance() const;
	void setMaxDistance(const scalar &maxDistance);
};

} // namespace vxCore
#endif // VXCOLLISION_H
