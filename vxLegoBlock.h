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
	scalar m_size;
public:
	vxLegoBlockInGrid()
	{}
	vxVector3d position() const;
	void setPosition(const vxVector3d &value);
	scalar size() const;
	void setSize(scalar value);
};

class vxLegoBlock: public std::enable_shared_from_this<vxLegoBlock>
{
private:
	
	vxLegoBlockInGrid m_instance;

public:
	vxLegoBlock();

	std::shared_ptr<vxLegoBlock> at(const vxVector3d &pos, scalar size)
	{
		m_instance.setPosition(pos);
		m_instance.setSize(size);
		return shared_from_this();
	}
	
	int throwRay(const vxRay &ray, vxCollision &collide) const
	{
		const vxVector3d p = m_instance.position() - ray.origin();
		const scalar mSize = m_instance.size()/2;
		
		scalar minX = p.x() - mSize;
		scalar minY = p.y() - mSize;
		scalar minZ = p.z() - mSize;

		scalar maxX = p.x() + mSize;
		scalar maxY = p.y() + mSize;
		scalar maxZ = p.z() + mSize;

		bool bMax = std::signbit(maxX);
		const auto hitX = MU::rectAndXPlane(ray.direction(), bMax ? maxX : minX);
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
		const auto hitY = MU::rectAndYPlane(ray.direction(), bMax ? maxY : minY);
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
		const auto hitZ = MU::rectAndZPlane(ray.direction(), bMax ? maxZ : minZ);
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
