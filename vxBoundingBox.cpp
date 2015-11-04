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

void vxBoundingBox::extend(const v3 &limit)
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

bool vxBoundingBox::contains(const v3 &v) const
{
	return v.x() >= m_minx
			&&  v.y() >= m_miny
			&&  v.z() >= m_minz
			&&  v.x() <= m_maxx
			&&  v.y() <= m_maxy
			&&  v.z() <= m_maxz;
}

int vxBoundingBox::throwRay(const vxRay &ray, vxCollision &collide) const
{
	auto&& d = ray.direction();

	auto t = ((d.xSign() ? m_maxx : m_minx) - d.x()) / -d.x();
	auto y = t * (-d.y()) + d.y();
	if( y<=m_maxy && y>=m_miny)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=m_maxz && z>=m_minz)
		{
			collide.setNormal(d.xSign() ? v3::constX : v3::constMinusX);
			collide.setValid();
			collide.setUV(v2(.5,.5));
			collide.setColor(vxColor::white);
			return 1;
		}
	}
	
	t = ((d.ySign() ? m_maxy : m_miny) - d.y()) / -d.y();
	auto x = t * (-d.x()) + d.x();
	if(x<=m_maxx && x>=m_minx)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=m_maxz && z>=m_minz)
		{
			collide.setNormal(d.ySign() ? v3::constY : v3::constMinusY);
			collide.setValid();
			collide.setUV(v2(.5,.5));
			collide.setColor(vxColor::white);
			return 1;
		}
	}

	t = ((d.zSign() ? m_maxz : m_minz) - d.z()) / -d.z();
	x = t * (-d.x()) + d.x();
	if(x<=m_maxx && x>=m_minx)
	{
		y = t * (-d.y()) + d.y();
		if(y<=m_maxy && y>=m_miny)
		{
			collide.setNormal(d.zSign() ? v3::constZ : v3::constMinusZ);
			collide.setValid();
			collide.setUV(v2(.5,.5));
			collide.setColor(vxColor::white);
			return 1;
		}
	}

	return 0;
}

bool vxBoundingBox::hasCollision(const vxRay &ray) const
{
	auto&& d = ray.direction();

	auto t = ((d.xSign() ? m_maxx : m_minx) - d.x()) / -d.x();
	auto y = t * (-d.y()) + d.y();
	if( y<=m_maxy && y>=m_miny)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=m_maxz && z>=m_minz)
		{
			return true;
		}
	}
	
	t = ((d.ySign() ? m_maxy : m_miny) - d.y()) / -d.y();
	auto x = t * (-d.x()) + d.x();
	if(x<=m_maxx && x>=m_minx)
	{
		auto z = t * (-d.z()) + d.z();
		if(z<=m_maxz && z>=m_minz)
		{
			return true;
		}
	}

	t = ((d.zSign() ? m_maxz : m_minz) - d.z()) / -d.z();
	x = t * (-d.x()) + d.x();
	if(x<=m_maxx && x>=m_minx)
	{
		y = t * (-d.y()) + d.y();
		if(y<=m_maxy && y>=m_miny)
		{
			return true;
		}
	}

	return false;
}
