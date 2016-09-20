#include "BoundingBox.h"
#include "Matrix44.h"

using namespace vxCore;

BoundingBox::BoundingBox()
{
}

void BoundingBox::clear()
{
	m_init = false;
}

void BoundingBox::set(v3s position, scalar size)
{
	m_minX = position.x() - (size / 2.0);
	m_minY = position.y() - (size / 2.0);
	m_minZ = position.z() - (size / 2.0);

	m_maxX = position.x() + (size / 2.0);
	m_maxY = position.y() + (size / 2.0);
	m_maxZ = position.z() + (size / 2.0);

	m_init = true;
}

void BoundingBox::set(scalar mnx, scalar mny, scalar mnz, 
						scalar mxx, scalar mxy, scalar mxz)
{
	m_minX = mnx;
	m_minY = mny;
	m_minZ = mnz;

	m_maxX = mxx;
	m_maxY = mxy;
	m_maxZ = mxz;

	m_init = true;
}

void BoundingBox::markAsInitialized()
{
	m_init = true;
}

void BoundingBox::close()
{
//	m_scale.set( (scalar)1.0/fabs(m_maxx - m_minx),
//				 (scalar)1.0/fabs(m_maxy - m_miny),
//				 (scalar)1.0/fabs(m_maxz - m_minz));
}

void BoundingBox::extend(const v3s &limit)
{
	if(!m_init)
	{
		m_maxX = m_minX = limit.x();
		m_maxY = m_minY = limit.y();
		m_maxZ = m_minZ = limit.z();
		markAsInitialized();
		return;
	}
	
	m_minX = std::min(limit.x(), m_minX);
	m_minY = std::min(limit.y(), m_minY);
	m_minZ = std::min(limit.z(), m_minZ);
	
	m_maxX = std::max(limit.x(), m_maxX);
	m_maxY = std::max(limit.y(), m_maxY);
	m_maxZ = std::max(limit.z(), m_maxZ);
}


void BoundingBox::extend(const BoundingBox &box)
{
	if(!m_init)
	{
		m_minX = box.m_minX;
		m_minY = box.m_minY;
		m_minZ = box.m_minZ;
		
		m_maxX = box.m_maxX;
		m_maxY = box.m_maxY;
		m_maxZ = box.m_maxZ;
		
		markAsInitialized();
		return;
	}
	
	m_minX = std::min(box.minX(), m_minX);
	m_minY = std::min(box.minY(), m_minY);
	m_minZ = std::min(box.minZ(), m_minZ);
	
	m_maxX = std::max(box.maxX(), m_maxX);
	m_maxY = std::max(box.maxY(), m_maxY);
	m_maxZ = std::max(box.maxZ(), m_maxZ);
}

scalar BoundingBox::xLength() const
{
	return m_maxX - m_minX;
}

scalar BoundingBox::yLength() const
{
	return m_maxY - m_minY;
}

scalar BoundingBox::zLength() const
{
	return m_maxZ - m_minZ;
}

v3s BoundingBox::max() const
{
	return {m_maxX,m_maxY,m_maxZ};
}

v3s BoundingBox::min() const
{
	return {m_minX,m_minY,m_minZ};
}

v3s BoundingBox::center() const
{
	return {m_minX+m_maxX/2.0,
		m_minY+m_maxY/2.0,
		m_minZ+m_maxZ/2.0};
}

scalar BoundingBox::minX() const
{
	return m_minX;
}

void BoundingBox::setMinX(scalar minx)
{
	m_minX = minx;
}

scalar BoundingBox::minY() const
{
	return m_minY;
}

void BoundingBox::setMinY(scalar miny)
{
	m_minY = miny;
}

scalar BoundingBox::minZ() const
{
	return m_minZ;
}

void BoundingBox::setMinZ(scalar minz)
{
	m_minZ = minz;
}

scalar BoundingBox::maxX() const
{
	return m_maxX;
}

void BoundingBox::setMaxX(scalar maxx)
{
	m_maxX = maxx;
}

scalar BoundingBox::maxY() const
{
	return m_maxY;
}

void BoundingBox::setMaxY(scalar maxy)
{
	m_maxY = maxy;
}

scalar BoundingBox::maxZ() const
{
	return m_maxZ;
}

void BoundingBox::setMaxZ(scalar maxz)
{
	m_maxZ = maxz;
}

scalar BoundingBox::midXValue() const
{
	return ( m_maxX + m_minX )/ 2.0;
}

scalar BoundingBox::midYValue() const
{
	return ( m_maxY + m_minY ) / 2.0;
}

scalar BoundingBox::midZValue() const
{
	return ( m_maxZ + m_minZ ) / 2.0;
}

bool BoundingBox::contains(const v3s &v) const
{
	return v.x() >= m_minX
			&&  v.y() >= m_minY
		&&  v.z() >= m_minZ
		&&  v.x() <= m_maxX
		&&  v.y() <= m_maxY
		&&  v.z() <= m_maxZ;
}

bool BoundingBox::contains(const v3s &v, scalar tolerance) const
{
	return v.x() >= (m_minX + tolerance)
			&&  v.y() >= (m_minY + tolerance)
			&&  v.z() >= (m_minZ + tolerance)
			&&  v.x() <= (m_maxX - tolerance)
			&&  v.y() <= (m_maxY - tolerance)
			&&  v.z() <= (m_maxZ - tolerance);
}

v3s BoundingBox::diagonal() const
{
	return {m_maxX-m_minX, m_maxY-m_minY, m_maxZ-m_minZ};
}

void BoundingBox::applyTransform(const Matrix44 &m)
{
	///TODO: account rotations.
	auto&& o = m.origin();

	m_minX+=o.x();
	m_minY+=o.y();
	m_minZ+=o.z();

	m_maxX+=o.x();
	m_maxY+=o.y();
	m_maxZ+=o.z();
}

bool BoundingBox::throwRay(const Ray &ray) const
{
	return hasCollision(ray);
}

int BoundingBox::throwRay(const Ray &ray, Collision &collide) const
{
	const auto& p = ray.origin();
	if(contains(p))
	{
		collide.setPosition(p);
		return 1;
	}
	
	const auto& d = ray.direction();
	
	auto&& minX = m_minX - p.x();
	auto&& maxX = m_maxX - p.x();
	auto&& minY = m_minY - p.y();
	auto&& maxY = m_maxY - p.y();
	auto&& minZ = m_minZ - p.z();
	auto&& maxZ = m_maxZ - p.z();
	
	auto x = d.xPositive() ? minX : maxX;
	auto t = (x - d.x()) / -d.x();
	auto y = t * (-d.y()) + d.y();
	if( y<=maxY && y>=minY)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=maxZ && z>=minZ)
		{
			auto fp = p + v3s(x, y, z);
			//TODO:this is not required to be 3d
			if(!(p-fp).follows(d))
			{
				return 0;
			}
			collide.setPosition(fp);
			collide.setNormal(d.xPositive() ? v3s::constMinusX : v3s::constX);
			collide.setValid();
			collide.setUV({(y - minY) / yLength(),
						   (z - minZ) / zLength()});
			return 1;
		}
	}
	
	y = d.yPositive() ? minY : maxY;
	t = (y - d.y()) / -d.y();
	x = t * (-d.x()) + d.x();
	if(x<=maxX && x>=minX)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=maxZ && z>=minZ)
		{
			auto fp = p + v3s(x, y, z);
			//TODO:this is not required to be 3d
			if(!(p-fp).follows(d))
			{
				return 0;
			}
			collide.setPosition(fp);
			collide.setNormal(d.yPositive() ? v3s::constMinusY : v3s::constY);
			collide.setValid();
			collide.setUV(v2s( ((x - minX)) / xLength(),
							  ((z - minZ)) / zLength()));
			return 1;
		}
	}

	auto z = d.zPositive() ? minZ : maxZ;
	t = (z - d.z()) / -d.z();
	x = t * (-d.x()) + d.x();
	if(x<=maxX && x>=minX)
	{
		y = t * (-d.y()) + d.y();
		if(y<=maxY && y>=minY)
		{
			auto fp = p + v3s(x, y, z);
			//TODO:this is not required to be 3d
			if(!(p-fp).follows(d))
			{
				return 0;
			}
			collide.setPosition(fp);
			collide.setNormal(d.zPositive() ? v3s::constMinusZ :  v3s::constZ);
			collide.setValid();
			collide.setUV(v2s( ((x - minX)) /  xLength(),
							  ((y - minY)) /  yLength()));
			return 1;
		}
	}

	return 0;
}

bool BoundingBox::hasCollision(const Ray &ray) const
{
	const auto& d = ray.direction();
	const auto& p = ray.origin();
	
	if(contains(p))
	{
		return true;
	}
	
	auto&& minX = m_minX - p.x();
	auto&& maxX = m_maxX - p.x();
	auto&& minY = m_minY - p.y();
	auto&& maxY = m_maxY - p.y();
	auto&& minZ = m_minZ - p.z();
	auto&& maxZ = m_maxZ - p.z();
	
	
	auto x = d.xPositive() ? minX : maxX;
	auto t = (x - d.x()) / -d.x();
	auto y = t * (-d.y()) + d.y();
	if( y<=maxY && y>=minY)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=maxZ && z>=minZ)
		{
			auto fp = v3s(x, y, z) + p;
			if(!(p-fp).follows(d))
			{
				return false;
			}
			return true;
		}
	}
	
	y = d.yPositive() ? minY : maxY;
	t = (y - d.y()) / -d.y();
	x = t * (-d.x()) + d.x();
	if(x<=maxX && x>=minX)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=maxZ && z>=minZ)
		{
			auto fp = v3s(x, y, z) + p;
			if(!(p-fp).follows(d))
			{
				return false;
			}
			return true;
		}
	}

	auto z = d.zPositive() ? minZ : maxZ;
	t = (z - d.z()) / -d.z();
	x = t * (-d.x()) + d.x();
	if(x<=maxX && x>=minX)
	{
		y = t * (-d.y()) + d.y();
		if(y<=maxY && y>=minY)
		{
			auto fp = v3s(x, y, z) + p;
			if(!(p-fp).follows(d))
			{
				return false;
			}
			return true;
		}
	}

	return false;
}
