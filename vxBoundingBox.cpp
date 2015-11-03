#include "vxBoundingBox.h"

using namespace vxCore;

vxBoundingBox::vxBoundingBox()
{
}

void vxBoundingBox::close()
{
	m_scale.set( (scalar)1.0/fabs(m_maxx - m_minx),
				 (scalar)1.0/fabs(m_maxy - m_miny),
				 (scalar)1.0/fabs(m_maxz - m_minz));
}

void vxBoundingBox::extend(const vxVector3d &limit)
{
	if(!m_init)
	{
		m_maxx = m_minx = limit.x();
		m_maxy = m_miny = limit.y();
		m_maxz = m_minz = limit.z();
		m_init = true;
		return;
	}
	
	m_minx = std::min(limit.x(), m_minx);
	m_miny = std::min(limit.y(), m_miny);
	m_minz = std::min(limit.z(), m_minz);
	
	m_maxx = std::max(limit.x(), m_maxx);
	m_maxy = std::max(limit.y(), m_maxy);
	m_maxz = std::max(limit.z(), m_maxz);
}

scalar vxBoundingBox::minX() const
{
	return m_minx;
}

void vxBoundingBox::setMinX(scalar minx)
{
	m_minx = minx;
}

scalar vxBoundingBox::minY() const
{
	return m_miny;
}

void vxBoundingBox::setMinY(scalar miny)
{
	m_miny = miny;
}

scalar vxBoundingBox::minZ() const
{
	return m_minz;
}

void vxBoundingBox::setMinZ(scalar minz)
{
	m_minz = minz;
}

scalar vxBoundingBox::maxX() const
{
	return m_maxx;
}

void vxBoundingBox::setMaxX(scalar maxx)
{
	m_maxx = maxx;
}

scalar vxBoundingBox::maxY() const
{
	return m_maxy;
}

void vxBoundingBox::setMaxY(scalar maxy)
{
	m_maxy = maxy;
}

scalar vxBoundingBox::maxZ() const
{
	return m_maxz;
}

void vxBoundingBox::setMaxZ(scalar maxz)
{
	m_maxz = maxz;
}

bool vxBoundingBox::contains(const vxVector3d &v) const
{
	return v.x() >= m_minx
			&&  v.y() >= m_miny
			&&  v.z() >= m_minz
			&&  v.x() <= m_maxx
			&&  v.y() <= m_maxy
			&&  v.z() <= m_maxz;
}

bool vxBoundingBox::hasCollision(const vxRay &ray) const
{
//	if(contains(ray.origin()))
//	{
//		return true;
//	}
	auto&& d = ray.direction();

	auto t = ((std::signbit(m_maxx) ? m_maxx : m_minx) - d.x()) / (-d.x());
	auto y = t * (-d.y()) + d.y();
	auto z = t * (-d.z()) + d.z();
	
	if( y<=m_maxy && y>=m_miny && z<=m_maxz && z>=m_minz)
	{
		return true;
	}

	t = ((std::signbit(m_maxy) ? m_maxy : m_miny) - d.y()) / (-d.y());
	auto x = t * (-d.x()) + d.x();
	z = t * (-d.z()) + d.z();
	
	if(x<=m_maxx && x>=m_minx && z<=m_maxz && z>=m_minz)
	{
		return true;
	}

	t = ((std::signbit(m_maxz) ? m_maxz : m_minz) - d.z()) / (-d.z());
	x = t * (-d.x()) + d.x();
	y = t * (-d.y()) + d.y();
	
	if(x<=m_maxx && x>=m_minx && y<=m_maxy && y>=m_miny)
	{
		return true;
	}

	return false;
}
