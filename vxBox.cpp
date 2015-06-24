#include "vxBox.h"

using namespace vxCore;

vxBox::vxBox()
{
}

std::shared_ptr<vxBox> vxBox::at(const vxVector3d &pos, double size)
{
	m_instance.setPosition(pos);
	m_instance.setSize(size);
	return shared_from_this();
}

void vxBox::set(const vxVector3d &pos, double size)
{
	m_instance.setPosition(pos);
	m_instance.setSize(size);
}

int vxBox::throwRay(const vxRayXYZ &ray, vxCollision &collide) const
{
	const vxVector3d p = m_instance.position() - ray.origin();
	const double mSize = m_instance.size()/2.0;
	
	double minX = p.x() - mSize;
	double minY = p.y() - mSize;
	double minZ = p.z() - mSize;
	
	double maxX = p.x() + mSize;
	double maxY = p.y() + mSize;
	double maxZ = p.z() + mSize;
	
	bool bMax = std::signbit(maxX);
	const auto hitX = MathUtils::rectAndXPlane(ray, bMax ? maxX : minX);
	if( std::isless(hitX.z(),maxZ) && std::isgreater(hitX.z(),minZ)
			&& std::isless(hitX.y(),maxY) && std::isgreater(hitX.y(),minY))
	{
		collide.setValid(true);
		collide.setNormal(bMax ? vxVector3d::constX : vxVector3d::constMinusX);
		collide.setPosition(hitX);
		collide.setUV(vxVector2d(maxZ - hitX.z(), maxY - hitX.y()));
		return 1;
	}
	
	bMax = std::signbit(maxY);
	const auto hitY = MathUtils::rectAndYPlane(ray, bMax ? maxY : minY);
	if( std::isless(hitY.x(),maxX) && std::isgreater(hitY.x(),minX)
			&&	std::isless(hitY.z(),maxZ) && std::isgreater(hitY.z(),minZ))
	{
		collide.setValid(true);
		collide.setNormal(bMax ? vxVector3d::constY : vxVector3d::constMinusY);
		collide.setPosition(hitY);
		collide.setUV(vxVector2d(maxX - hitY.x(), maxZ - hitY.z()));
		return 1;
	}
	
	bMax = std::signbit(maxZ);
	const auto hitZ = MathUtils::rectAndZPlane(ray, bMax ? maxZ : minZ);
	if( std::isless(hitZ.x(),maxX) && std::isgreater(hitZ.x(),minX)
			&& std::isless(hitZ.y(),maxY) && std::isgreater(hitZ.y(),minY))
	{
		collide.setValid(true);
		collide.setNormal(bMax ? vxVector3d::constZ : vxVector3d::constMinusZ);
		collide.setPosition(hitZ);
		collide.setUV(vxVector2d(maxX - hitZ.x(), maxY - hitZ.y()));
		return 1;
	}
	
	return 0;
}

bool vxBox::hasCollision(const vxRayXYZ &ray) const
{
	bool itHas{false};
	const vxVector3d &p = m_instance.position() - ray.origin();
	const double mSize = m_instance.size()/2.0;
	
	double minX = p.x() - mSize;
	double minY = p.y() - mSize;
	double minZ = p.z() - mSize;
	
	double maxX = p.x() + mSize;
	double maxY = p.y() + mSize;
	double maxZ = p.z() + mSize;
	
	bool bMax = std::signbit(maxX);
	const auto hitX = MathUtils::rectAndXPlane(ray, bMax ? maxX : minX);
	if( std::isless(hitX.z(),maxZ) 
			&& std::isgreater(hitX.z(),minZ)
			&& std::isless(hitX.y(),maxY)
			&& std::isgreater(hitX.y(),minY))
	{
		return true;
	}
	
	bMax = std::signbit(maxY);
	const auto hitY = MathUtils::rectAndYPlane(ray, bMax ? maxY : minY);
	if( std::isless(hitY.x(),maxX) && std::isgreater(hitY.x(),minX)
			&&	std::isless(hitY.z(),maxZ) && std::isgreater(hitY.z(),minZ))
	{
		return true;
	}
	
	bMax = std::signbit(maxZ);
	const auto hitZ = MathUtils::rectAndZPlane(ray, bMax ? maxZ : minZ);
	if( std::isless(hitZ.x(),maxX) 
			&& std::isgreater(hitZ.x(),minX)
			&& std::isless(hitZ.y(),maxY) 
			&& std::isgreater(hitZ.y(),minY))
	{
		return true;
	}
	
	return false;

}


double vxBoxInGrid::size() const
{
	return m_size;
}

void vxBoxInGrid::setSize(double value)
{
	m_size = value;
}
vxVector3d vxBoxInGrid::position() const
{
	return m_pos;
}

void vxBoxInGrid::setPosition(const vxVector3d &value)
{
	m_pos = value;
}
