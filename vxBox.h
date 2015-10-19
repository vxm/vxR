#ifndef _VXBOXMC_
#define _VXBOXMC_

#include<memory>
#include<cmath>
#include<map>
#include <thread>

#include "vxVector.h"
#include "vxRay.h"
#include "vxCollision.h"
#include "MathUtils.h"

namespace vxCore{

class vxBox
{
private:

	vxVector3d m_position;
	double m_size{1.0};

public:
	vxBox()
	{};

	vxBox(const vxVector3d &pos, double size)
		: m_position(pos)
		, m_size(size)
	{};

	vxBox(const vxVector3d &pos)
		: m_position(pos)
	{};
	
	void set(const vxVector3d &pos);
	void set(const vxVector3d &pos, double size);

	bool contains(const vxVector3d &v) const
	{
		auto min = m_position - (m_size/2.0);
		auto max = m_position + (m_size/2.0);
		return v.x() >= min.x()
				&&  v.y() >= min.y()
				&&  v.z() >= min.z()
				&&  v.x() <= max.x()
				&&  v.y() <= max.y()
				&&  v.z() <= max.z();
	}
	
	vxVector3d position() const;
	void setPosition(const vxVector3d &position);
	double size() const;
	void setSize(double size);

	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif
