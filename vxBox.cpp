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

int vxBox::throwRay(const vxRay &ray, vxCollision &collide) const
{
	const auto& instance = m_useDefault ? m_default : m_instance[std::this_thread::get_id()];

	const auto&& ip = instance.position();
	const auto&& p = ip - ray.origin();
	const double&& size = instance.size();
	const double&& mSize = size/2.0;
	
	const auto&& min = p - mSize;
	const auto&& max = p + mSize;
	
	bool bMax = std::signbit(max.x());
	const auto&& hitX = MathUtils::rectAndXPlane(ray.direction(), bMax ? max.x() : min.x());
	if( std::islessequal(hitX.z(),max.z()) && std::isgreaterequal(hitX.z(),min.z())
			&& std::islessequal(hitX.y(),max.y()) && std::isgreaterequal(hitX.y(),min.y()))
	{
		collide.setValid(true);
		collide.setNormal(bMax ? vxVector3d::constX : vxVector3d::constMinusX);
		collide.setPosition(hitX);
		collide.setUV(vxVector2d{(mSize + hitX.z() - p.z())/size,
								 (mSize + hitX.y() - p.y())/size});
		return 1;
	}
	
	bMax = std::signbit(max.z());
    const auto&& hitZ = MathUtils::rectAndZPlane(ray.direction(), bMax ? max.z() : min.z());
    if( std::islessequal(hitZ.x(),max.x()) && std::isgreaterequal(hitZ.x(),min.x())
		    && std::islessequal(hitZ.y(),max.y()) && std::isgreaterequal(hitZ.y(),min.y()))
    {
	    collide.setValid(true);
	    collide.setNormal(bMax ? vxVector3d::constZ : vxVector3d::constMinusZ);
	    collide.setPosition(hitZ);
	    collide.setUV(vxVector2d{(mSize + hitZ.x() - p.x())/size,
							     (mSize + hitZ.y() - p.y())/size});
	    return 1;
    }
    
	
	bMax = std::signbit(max.y());
	const auto&& hitY = MathUtils::rectAndYPlane(ray.direction(), bMax ? max.y() : min.y());
	if( std::islessequal(hitY.x(),max.x()) && std::isgreaterequal(hitY.x(),min.x())
			&&	std::islessequal(hitY.z(),max.z()) && std::isgreaterequal(hitY.z(),min.z()))
	{
		collide.setValid(true);
		collide.setNormal(bMax ? vxVector3d::constY : vxVector3d::constMinusY);
		collide.setPosition(hitY);
		collide.setUV(vxVector2d{(mSize + hitY.x() - p.x())/size,
								 (mSize + hitY.z() - p.z())/size});
		return 1;
	}

	
	return 0;
}

bool vxBox::hasCollision(const vxRay &ray) const
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
	const auto hitX = MathUtils::rectAndXPlane(ray.direction(), bMax ? maxX : minX);
	if( std::islessequal(hitX.z(),maxZ) 
		&& std::isgreaterequal(hitX.z(),minZ)
		&& std::islessequal(hitX.y(),maxY)
		&& std::isgreaterequal(hitX.y(),minY))
	{
		return true;
	}
	
	bMax = std::signbit(maxY);
	const auto hitY = MathUtils::rectAndYPlane(ray.direction(), bMax ? maxY : minY);
	if( std::islessequal(hitY.x(),maxX) && std::isgreaterequal(hitY.x(),minX)
			&&	std::islessequal(hitY.z(),maxZ) && std::isgreaterequal(hitY.z(),minZ))
	{
		return true;
	}
	
	bMax = std::signbit(maxZ);
	const auto hitZ = MathUtils::rectAndZPlane(ray.direction(), bMax ? maxZ : minZ);
	if( std::islessequal(hitZ.x(),maxX) 
			&& std::isgreaterequal(hitZ.x(),minX)
			&& std::islessequal(hitZ.y(),maxY) 
			&& std::isgreaterequal(hitZ.y(),minY))
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
