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
	
	const auto&& hitX = MU::rayAndXPlane(ray, std::signbit(m_maxx) ? m_maxx : m_minx);
	if( hitX.z()<=m_maxz && hitX.z()>=m_minz && hitX.y()<=m_maxy && hitX.y()>=m_miny)
	{
		return true;
	}
	
	const auto hitY = MU::rayAndYPlane(ray, std::signbit(m_maxy) ? m_maxy : m_miny);
	if( hitY.x()<=m_maxx && hitY.x()>=m_minx && hitY.z()<=m_maxz && hitY.z()>=m_minz)
	{
		return true;
	}
	
	const auto hitZ = MU::rayAndZPlane(ray, std::signbit(m_maxz) ? m_maxz : m_minz);
	if( hitZ.x()<=m_maxx && hitZ.x()>=m_minx && hitZ.y()<=m_maxy && hitZ.y()>=m_miny)
	{
		return true;
	}
	
	return false;
}
