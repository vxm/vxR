#ifndef VXCOLLISION_H
#define VXCOLLISION_H

#include "vxVector.h"
#include "vxPoint.h"
#include "vxColor.h"

namespace vxCore {


class vxCollision
{
private:

	vxPoint m_position;
	vxVector3d m_normal;
	vxColor m_color;

	bool m_valid {false};
	vxVector2d m_uv;

public:

	vxCollision()
	{
	};

	void reset();
	void setValid(bool v = true);
	bool isValid() const;
	void setPosition(const vxVector3d &val);
	void setPosition(scalar x, scalar y, scalar z);
	vxVector3d position() const;
	void setColor(const vxVector3d &val);
	void setColor(const vxColor &val);
	void setColor(scalar r, scalar g, scalar b);
	vxColor color() const;
	void setNormal(const vxVector3d &val);
	vxVector3d normal() const;
	void setUV(const vxVector2d &&uvVec);
	void setU(scalar u);
	scalar u() const;
	void setV(scalar v);
	scalar v() const;
	vxVector2d uv() const;

	//TODO:complete this.
	friend std::ostream& operator<<(std::ostream &os, const vxCollision& v)
	{
		return os << v.m_position;
	}
};


}// vxCore namespace.
#endif // VXCOLLISION_H
