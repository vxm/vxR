#ifndef _VXBOXMC_
#define _VXBOXMC_

#include<memory>
#include<cmath>
#include<map>
#include <thread>

#include "Vector.h"
#include "Ray.h"
#include "Collision.h"
#include "MathUtils.h"

namespace vxCore{

class Box
{
private:

	v3s m_position;
	scalar m_size{1.0};

public:
	Box()
	{}

	Box(const v3s &pos, scalar size)
		: m_position(pos)
		, m_size(size)
	{}

	Box(const v3s &pos)
		: m_position(pos)
	{}
	
	void set(const v3s &pos);
	void set(const v3s &pos, scalar size);

	bool contains(const v3s &v) const
	{
		auto min = m_position - (m_size/(scalar)2);
		auto max = m_position + (m_size/(scalar)2);
		return v.x() >= min.x()
				&&  v.y() >= min.y()
				&&  v.z() >= min.z()
				&&  v.x() <= max.x()
				&&  v.y() <= max.y()
				&&  v.z() <= max.z();
	}
	
	v3s position() const;
	void setPosition(const v3s &position);
	scalar size() const;
	void setSize(scalar size);

	//renderable interface
	bool throwRay(const Ray &ray) const;
	int throwRay(const Ray &ray, Collision &collide) const;
	bool hasCollision(const Ray &ray) const;
};

using BoxHandler = std::shared_ptr<Box>;

}
#endif
