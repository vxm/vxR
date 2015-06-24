#include "vxBox.h"

using namespace vxCore;

vxBox::vxBox(bool usesDefault)
	:m_useDefault(usesDefault)
{
}

std::shared_ptr<vxBox> vxBox::at(const vxVector3d &pos, double size)
{
	set(pos,size);
	return shared_from_this();
}

void vxBox::set(const vxVector3d &pos, double size)
{
	if(!m_useDefault)
	{
		const auto& thID = std::this_thread::get_id();
		m_instance[thID].setPosition(pos);
		m_instance[thID].setSize(size);
	}
	else
	{
		m_default.setPosition(pos);
		m_default.setSize(size);
	}
}

int vxBox::throwRay(const vxRayXYZ &ray, vxCollision &collide) const
{
	const auto& instance = m_useDefault ? m_default : m_instance[std::this_thread::get_id()];
	const vxVector3d& p = instance.position() - ray.origin();
	const double mSize = instance.size()/2.0;
	
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
	const auto& instance = m_useDefault ? m_default : m_instance[std::this_thread::get_id()];
	bool itHas{false};
	const vxVector3d& p = instance.position() - ray.origin();
	const double mSize = instance.size()/2.0;
	
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
