#ifndef VXCOLLISION_H
#define VXCOLLISION_H

#include "vxVector.h"
#include "vxPoint.h"
#include "vxColor.h"

namespace vxCore {


class vxCollision
{
private:

	v3 m_position;
	v3 m_normal;
	vxColor m_color;
	v2 m_uv;

	bool m_valid {false};

public:

	vxCollision();
	
	vxCollision(const v3 &p,
				const v3 &n = {0,0,0},
				const vxColor &c = {0,0,0},
				const v2 &uv = {.5,.5});

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
	void setPosition(const v3 &val);
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
	v3 position() const;
	///
	/// \brief setColor
	/// \param val
	///
	void setColor(const v3 &val);
	///
	/// \brief setColor
	/// \param val
	///
	void setColor(const vxColor &val);
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
	vxColor color() const;
	///
	/// \brief setNormal
	/// \param val
	///
	void setNormal(const v3 &val);
	///
	/// \brief normal
	/// \return 
	///
	v3 normal() const;
	///
	/// \brief setUV
	/// \param uvVec
	///
	void setUV(const v2 &&uvVec);
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
	v2 uv() const;

	//TODO:complete this.
	friend std::ostream& operator<<(std::ostream &os, const vxCollision& v)
	{
		return os << v.m_position
					<< ", "
					<< v.m_normal 
					<< ", "
					<< v.m_color 
					<< ", "
					<< v.m_uv;
	}
};


}// vxCore namespace.
#endif // VXCOLLISION_H
