#include "vxBox.h"

using namespace vxCore;


vxVector3d vxBox::position() const
{
	return m_position;
}

void vxBox::setPosition(const vxVector3d &position)
{
	m_position = position;
}

double vxBox::size() const
{
	return m_size;
}

void vxBox::setSize(double size)
{
	m_size = size;
}

void vxBox::set(const vxVector3d &pos)
{
	m_position = pos;
}

void vxBox::set(const vxVector3d &pos, double size)
{
	m_position = pos;
	m_size = size;
}

bool vxBox::throwRay(const vxRay &ray) const
{
	if(contains(ray.origin()))
	{
		return false;
	}
	
	const auto&& p = m_position - ray.origin();
	const auto&& min = p - m_size/2.0;
	const auto&& max = p + m_size/2.0;
	
	bool bMax = std::signbit(ray.direction().x());
	
	const auto hitX = MU::rayAndXPlane(ray, bMax ? max.x() : min.x());
	if( std::islessequal(hitX.z(),max.z()) 
			&& std::isgreaterequal(hitX.z(),min.z())
			&& std::islessequal(hitX.y(),max.y()) 
			&& std::isgreaterequal(hitX.y(),min.y()))
	{
		return true;
	}
	
	bMax = std::signbit(ray.direction().z());
	const auto hitZ = MU::rayAndZPlane(ray, bMax ? max.z() : min.z());
	if( std::islessequal(hitZ.x(),max.x()) 
			&& std::isgreaterequal(hitZ.x(),min.x())
			&& std::islessequal(hitZ.y(),max.y()) 
			&& std::isgreaterequal(hitZ.y(),min.y()))
	{
		return true;
	}
	
	bMax = std::signbit(ray.direction().y());
	const auto hitY = MU::rayAndYPlane(ray, bMax ? max.y() : min.y());
	if( std::islessequal(hitY.x(),max.x()) 
			&& std::isgreaterequal(hitY.x(),min.x())
			&& std::islessequal(hitY.z(),max.z()) 
			&& std::isgreaterequal(hitY.z(),min.z()))
	{
		return true;
	}
	return false;
}

int vxBox::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(contains(ray.origin()))
	{
		return 0;
	}
	
	const auto&& p = m_position - ray.origin();
	const auto&& mSize = m_size/2.0;
	const auto&& min = p - mSize;
	const auto&& max = p + mSize;
	
	bool aMax = std::signbit(ray.direction().x());
	
	const auto&& hitX = MU::rayAndXPlane(ray, aMax ? max.x() : min.x());
	if( hitX.z() <= max.z() && hitX.z()>=min.z()
			&& hitX.y()<=max.y() && hitX.y()>=min.y())
	{
		collide.setValid(true);
		collide.setNormal(aMax ? vxVector3d::constX : vxVector3d::constMinusX);
		collide.setPosition(hitX + ray.origin());
		collide.setUV(vxVector2d{(mSize + hitX.z() - p.z())/m_size,
								 (mSize + hitX.y() - p.y())/m_size});
		return 1;
	}
	
	bool bMax = std::signbit(ray.direction().z());
	const auto&& hitZ = MU::rayAndZPlane(ray, bMax ? max.z() : min.z());
	if( hitZ.x()<=max.x() && hitZ.x()>=min.x()
			&& hitZ.y()<=max.y() && hitZ.y()>=min.y())
	{
		collide.setValid(true);
		collide.setNormal(bMax ? vxVector3d::constZ : vxVector3d::constMinusZ);
		collide.setPosition(hitZ + ray.origin());
		collide.setUV(vxVector2d{(mSize + hitZ.x() - p.x())/m_size,
								 (mSize + hitZ.y() - p.y())/m_size});
		return 1;
	}
	
	bool cMax = std::signbit(ray.direction().y());
	const auto&& hitY = MU::rayAndYPlane(ray, cMax ? max.y() : min.y());
	if( hitY.x()<=max.x() && hitY.x()>=min.x()
			&& hitY.z()<=max.z() && hitY.z()>=min.z())
	{
		collide.setValid(true);
		collide.setNormal(cMax ? vxVector3d::constY : vxVector3d::constMinusY);
		collide.setPosition(hitY + ray.origin());
		collide.setUV(vxVector2d{(mSize + hitY.x() - p.x())/m_size,
								 (mSize + hitY.z() - p.z())/m_size});
		return 1;
	}
	collide.setValid(false);
	return 0;
}

bool vxBox::hasCollision(const vxRay &ray) const
{
	if(contains(ray.origin()))
	{
		return false;
	}

	const auto&& p = m_position - ray.origin();
	const auto mSize = m_size/2.0;
	
	const auto&& min = p - mSize;
	const auto&& max = p + mSize;
	
	auto bMax = std::signbit(max.x());
	const auto&& hitX = MU::rectAndXPlane(ray.direction(), bMax ? max.x() : min.x());
	if( std::islessequal(hitX.z(),max.z()) 
		&& std::isgreaterequal(hitX.z(),min.z())
		&& std::islessequal(hitX.y(),max.y())
		&& std::isgreaterequal(hitX.y(),min.y()))
	{
		return true;
	}
	
	bMax = std::signbit(max.y());
	const auto hitY = MU::rectAndYPlane(ray.direction(), bMax ? max.y() : min.y());
	if( std::islessequal(hitY.x(),max.x()) && std::isgreaterequal(hitY.x(),min.x())
			&&	std::islessequal(hitY.z(),max.z()) && std::isgreaterequal(hitY.z(),min.z()))
	{
		return true;
	}
	
	bMax = std::signbit(max.z());
	const auto hitZ = MU::rectAndZPlane(ray.direction(), bMax ? max.z() : min.z());
	if( std::islessequal(hitZ.x(),max.x()) 
			&& std::isgreaterequal(hitZ.x(),min.x())
			&& std::islessequal(hitZ.y(),max.y()) 
			&& std::isgreaterequal(hitZ.y(),min.y()))
	{
		return true;
	}
	
	return false;
}

