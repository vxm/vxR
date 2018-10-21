#ifndef VXLEGOBLOCK_H
#define VXLEGOBLOCK_H

#include <cmath>
#include <memory>

#include "Box.h"
#include "Ray.h"
#include "Vector.h"

namespace vxCore
{

class LegoBlockInGrid
{
private:
	v3s m_pos;
	scalar m_size;

public:
	LegoBlockInGrid() {}
	v3s position() const;
	void setPosition(const v3s &value);
	scalar size() const;
	void setSize(scalar value);
};

class LegoBlock : public std::enable_shared_from_this<LegoBlock>
{
private:
	LegoBlockInGrid m_instance;

public:
	LegoBlock();

	std::shared_ptr<LegoBlock> at(const v3s &pos, scalar size)
	{
		m_instance.setPosition(pos);
		m_instance.setSize(size);
		return shared_from_this();
	}

	int throwRay(const Ray &ray, Collision &collide) const
	{
		const v3s p = m_instance.position() - ray.origin();
		auto mSize = m_instance.size() / scalar(2.0);

		scalar minX = p.x() - mSize;
		scalar minY = p.y() - mSize;
		scalar minZ = p.z() - mSize;

		scalar maxX = p.x() + mSize;
		scalar maxY = p.y() + mSize;
		scalar maxZ = p.z() + mSize;

		bool bMax = std::signbit(maxX);
		const auto hitX = MU::rectAndXPlane(ray.direction(), bMax ? maxX : minX);
		if (std::isless(hitX.z(), maxZ) && std::isgreater(hitX.z(), minZ) &&
		    std::isless(hitX.y(), maxY) && std::isgreater(hitX.y(), minY))
		{
			collide.setValid(true);
			collide.setNormal(bMax ? constX : constMinusX);
			collide.setPosition(hitX);
			collide.setUV(v2s{maxZ - hitX.z(), maxY - hitX.y()});
			return 1;
		}

		bMax = std::signbit(maxY);
		const auto hitY = MU::rectAndYPlane(ray.direction(), bMax ? maxY : minY);
		if (std::isless(hitY.x(), maxX) && std::isgreater(hitY.x(), minX) &&
		    std::isless(hitY.z(), maxZ) && std::isgreater(hitY.z(), minZ))
		{
			collide.setValid(true);
			collide.setNormal(bMax ? constY : constMinusY);
			collide.setPosition(hitY);
			collide.setUV(v2s{maxX - hitY.x(), maxZ - hitY.z()});
			return 1;
		}

		bMax = std::signbit(maxZ);
		const auto hitZ = MU::rectAndZPlane(ray.direction(), bMax ? maxZ : minZ);
		if (std::isless(hitZ.x(), maxX) && std::isgreater(hitZ.x(), minX) &&
		    std::isless(hitZ.y(), maxY) && std::isgreater(hitZ.y(), minY))
		{
			collide.setValid(true);
			collide.setNormal(bMax ? constZ : constMinusZ);
			collide.setPosition(hitZ);
			collide.setUV(v2s{maxX - hitZ.x(), maxY - hitZ.y()});
			return 1;
		}

		return 0;
	}
};
}
#endif // VXLEGOBLOCK_H
