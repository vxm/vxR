#include "BoundingBox.h"

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
	m_minx = position.x() - (size / 2.0);
	m_miny = position.y() - (size / 2.0);
	m_minz = position.z() - (size / 2.0);

	m_maxx = position.x() + (size / 2.0);
	m_maxy = position.y() + (size / 2.0);
	m_maxz = position.z() + (size / 2.0);

	m_init = true;
}

void BoundingBox::set(scalar mnx, scalar mny, scalar mnz, 
						scalar mxx, scalar mxy, scalar mxz)
{
	m_minx = mnx;
	m_miny = mny;
	m_minz = mnz;

	m_maxx = mxx;
	m_maxy = mxy;
	m_maxz = mxz;

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
		m_maxx = m_minx = limit.x();
		m_maxy = m_miny = limit.y();
		m_maxz = m_minz = limit.z();
		markAsInitialized();
		return;
	}
	
	m_minx = std::min(limit.x(), m_minx);
	m_miny = std::min(limit.y(), m_miny);
	m_minz = std::min(limit.z(), m_minz);
	
	m_maxx = std::max(limit.x(), m_maxx);
	m_maxy = std::max(limit.y(), m_maxy);
	m_maxz = std::max(limit.z(), m_maxz);
}


void BoundingBox::extend(const BoundingBox &box)
{
	if(!m_init)
	{
		m_minx = box.m_minx;
		m_miny = box.m_miny;
		m_minz = box.m_minz;
		
		m_maxx = box.m_maxx;
		m_maxy = box.m_maxy;
		m_maxz = box.m_maxz;
		
		markAsInitialized();
		return;
	}
	
	m_minx = std::min(box.minX(), m_minx);
	m_miny = std::min(box.minY(), m_miny);
	m_minz = std::min(box.minZ(), m_minz);
	
	m_maxx = std::max(box.maxX(), m_maxx);
	m_maxy = std::max(box.maxY(), m_maxy);
	m_maxz = std::max(box.maxZ(), m_maxz);
}

scalar BoundingBox::xLength() const
{
	return m_maxx - m_minx;
}

scalar BoundingBox::yLength() const
{
	return m_maxy - m_miny;
}

scalar BoundingBox::zLength() const
{
	return m_maxz - m_minz;
}

v3s BoundingBox::max() const
{
	return v3s(m_maxx,m_maxy,m_maxz);
}

v3s BoundingBox::min() const
{
	return v3s(m_minx,m_miny,m_minz);
}

v3s BoundingBox::center() const
{
	return v3s(m_minx+m_maxx/2.0,
			  m_miny+m_maxy/2.0,
			  m_minz+m_maxz/2.0);
}

scalar BoundingBox::minX() const
{
	return m_minx;
}

void BoundingBox::setMinX(scalar minx)
{
	m_minx = minx;
}

scalar BoundingBox::minY() const
{
	return m_miny;
}

void BoundingBox::setMinY(scalar miny)
{
	m_miny = miny;
}

scalar BoundingBox::minZ() const
{
	return m_minz;
}

void BoundingBox::setMinZ(scalar minz)
{
	m_minz = minz;
}

scalar BoundingBox::maxX() const
{
	return m_maxx;
}

void BoundingBox::setMaxX(scalar maxx)
{
	m_maxx = maxx;
}

scalar BoundingBox::maxY() const
{
	return m_maxy;
}

void BoundingBox::setMaxY(scalar maxy)
{
	m_maxy = maxy;
}

scalar BoundingBox::maxZ() const
{
	return m_maxz;
}

void BoundingBox::setMaxZ(scalar maxz)
{
	m_maxz = maxz;
}

bool BoundingBox::contains(const v3s &v) const
{
	return v.x() >= m_minx
			&&  v.y() >= m_miny
			&&  v.z() >= m_minz
			&&  v.x() <= m_maxx
			&&  v.y() <= m_maxy
			&&  v.z() <= m_maxz;
}

bool BoundingBox::contains(const v3s &v, scalar tolerance) const
{
	return v.x() >= (m_minx + tolerance)
			&&  v.y() >= (m_miny + tolerance)
			&&  v.z() >= (m_minz + tolerance)
			&&  v.x() <= (m_maxx - tolerance)
			&&  v.y() <= (m_maxy - tolerance)
			&&  v.z() <= (m_maxz - tolerance);
}

v3s BoundingBox::diagonal() const
{
	return v3s(m_maxx-m_minx, m_maxy-m_miny, m_maxz-m_minz);
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
	
	auto&& minX = m_minx - p.x();
	auto&& maxX = m_maxx - p.x();
	auto&& minY = m_miny - p.y();
	auto&& maxY = m_maxy - p.y();
	auto&& minZ = m_minz - p.z();
	auto&& maxZ = m_maxz - p.z();
	
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
			collide.setUV(v2s( ((y - minY)) / yLength(),
							  ((z - minZ)) / zLength()));
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
	
	auto&& minX = m_minx - p.x();
	auto&& maxX = m_maxx - p.x();
	auto&& minY = m_miny - p.y();
	auto&& maxY = m_maxy - p.y();
	auto&& minZ = m_minz - p.z();
	auto&& maxZ = m_maxz - p.z();
	
	
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
