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

	bool m_valid {false};
	v2 m_uv;

public:

	vxCollision()
	{
	};

	void reset();
	void setValid(bool v = true);
	bool isValid() const;
	void setPosition(const v3 &val);
	void setPosition(scalar x, scalar y, scalar z);
	v3 position() const;
	void setColor(const v3 &val);
	void setColor(const vxColor &val);
	void setColor(scalar r, scalar g, scalar b);
	vxColor color() const;
	void setNormal(const v3 &val);
	v3 normal() const;
	void setUV(const v2 &&uvVec);
	void setU(scalar u);
	scalar u() const;
	void setV(scalar v);
	scalar v() const;
	v2 uv() const;

	//TODO:complete this.
	friend std::ostream& operator<<(std::ostream &os, const vxCollision& v)
	{
		return os << v.m_position;
	}
};


}// vxCore namespace.
#endif // VXCOLLISION_H
