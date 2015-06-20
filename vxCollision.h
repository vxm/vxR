#ifndef VXCOLLISION_H
#define VXCOLLISION_H

#include "vxObject.h"
#include "vxPoint.h"
#include "vxColor.h"

namespace vxCore {


class vxCollision
{
private:

	vxPoint m_position;
	vxColor m_color;
	vxVector3d m_normal;

	bool m_valid {false};
	vxVector2d m_uv;

public:

	vxCollision()
	{
	};

	void initialize();
	void setValid(bool v = true);
	bool isValid() const;
	void setPosition(const vxVector3d &val);
	void setPosition(double x, double y, double z);
	vxVector3d position() const;
	void setColor(const vxVector3d &val);
	void setColor(const vxColor &val);
	void setColor(double r, double g, double b);
	vxColor color() const;
	void setNormal(const vxVector3d &val);
	vxVector3d normal() const;
	void setUV(const vxVector2d &uvVec);
	void setU(double u);
	double u() const;
	void setV(double v);
	double v() const;
	vxVector2d uv() const;

	//TODO:complete this.
	friend std::ostream& operator<<(std::ostream &os, const vxCollision& v)
	{
		return os << v.m_position;
	}
};


}// vxCore namespace.
#endif // VXCOLLISION_H
