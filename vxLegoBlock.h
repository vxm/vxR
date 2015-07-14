#ifndef VXLEGOBLOCK_H
#define VXLEGOBLOCK_H

#include<memory>
#include<cmath>

#include "vxRay.h"
#include "vxBox.h"

namespace vxCore{

class vxLegoBlockInGrid
{
private:
	vxVector3d m_pos;
	double m_size;
public:
	vxLegoBlockInGrid()
	{}
	vxVector3d position() const;
	void setPosition(const vxVector3d &value);
	double size() const;
	void setSize(double value);
};

class vxLegoBlock: public std::enable_shared_from_this<vxLegoBlock>
{
private:
	
	vxLegoBlockInGrid m_instance;

public:
	vxLegoBlock();

	std::shared_ptr<vxLegoBlock> at(const vxVector3d &pos, double size)
	{
		m_instance.setPosition(pos);
		m_instance.setSize(size);
		return shared_from_this();
	}
	
	int throwRay(const vxRay &ray, vxCollision &collide) const
	{
		const vxVector3d p = m_instance.position() - ray.origin();
		const double mSize = m_instance.size()/2;
		
		double minX = p.x() - mSize;
		double minY = p.y() - mSize;
		double minZ = p.z() - mSize;

		double maxX = p.x() + mSize;
		double maxY = p.y() + mSize;
		double maxZ = p.z() + mSize;

		bool bMax = std::signbit(maxX);
		const auto hitX = MathUtils::rectAndXPlane(ray.direction(), bMax ? maxX : minX);
		if( std::isless(hitX.z(),maxZ) && std::isgreater(hitX.z(),minZ)
			&& std::isless(hitX.y(),maxY) && std::isgreater(hitX.y(),minY))
		{
			collide.setValid(true);
			collide.setNormal(bMax ? vxVector3d::constX : vxVector3d::constMinusX);
			collide.setPosition(hitX);
			collide.setUV(vxVector2d{maxZ - hitX.z(), maxY - hitX.y()});
			return 1;
		}

		bMax = std::signbit(maxY);
		const auto hitY = MathUtils::rectAndYPlane(ray.direction(), bMax ? maxY : minY);
		if( std::isless(hitY.x(),maxX) && std::isgreater(hitY.x(),minX)
			&&	std::isless(hitY.z(),maxZ) && std::isgreater(hitY.z(),minZ))
		{
			collide.setValid(true);
			collide.setNormal(bMax ? vxVector3d::constY : vxVector3d::constMinusY);
			collide.setPosition(hitY);
			collide.setUV(vxVector2d{maxX - hitY.x(), maxZ - hitY.z()});
			return 1;
		}
		
		bMax = std::signbit(maxZ);
		const auto hitZ = MathUtils::rectAndZPlane(ray.direction(), bMax ? maxZ : minZ);
		if( std::isless(hitZ.x(),maxX) && std::isgreater(hitZ.x(),minX)
			&& std::isless(hitZ.y(),maxY) && std::isgreater(hitZ.y(),minY))
		{
			collide.setValid(true);
			collide.setNormal(bMax ? vxVector3d::constZ : vxVector3d::constMinusZ);
			collide.setPosition(hitZ);
			collide.setUV(vxVector2d{maxX - hitZ.x(), maxY - hitZ.y()});
			return 1;
		}
		
		return 0;
	}
};

}
#endif // VXLEGOBLOCK_H
