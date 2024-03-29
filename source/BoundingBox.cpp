#include "BoundingBox.h"
#include "Matrix44.h"

using namespace vxCore;

BoundingBox::BoundingBox(scalar mnx, scalar mny, scalar mnz, scalar mxx,
						 scalar mxy, scalar mxz)
{
	set(mnx, mny, mnz, mxx, mxy, mxz);
}

void BoundingBox::clear()
{
	m_maxX = -1.0;
	m_minX = 1.0;
}

void BoundingBox::set(const v3s &position, scalar size)
{
	m_minX = position.x() - (size / scalar(2.0));
	m_minY = position.y() - (size / scalar(2.0));
	m_minZ = position.z() - (size / scalar(2.0));

	m_maxX = position.x() + (size / scalar(2.0));
	m_maxY = position.y() + (size / scalar(2.0));
	m_maxZ = position.z() + (size / scalar(2.0));
}

void BoundingBox::set(scalar mnx, scalar mny, scalar mnz, scalar mxx,
					  scalar mxy, scalar mxz)
{
	m_minX = mnx;
	m_minY = mny;
	m_minZ = mnz;

	m_maxX = mxx;
	m_maxY = mxy;
	m_maxZ = mxz;
}

void BoundingBox::markAsInitialized()
{
	if (m_minX > m_maxX)
		m_minX = m_maxX;
}

void BoundingBox::close()
{
	//	m_scale.set( (scalar)1.0/fabs(m_maxx - m_minx),
	//				 (scalar)1.0/fabs(m_maxy - m_miny),
	//				 (scalar)1.0/fabs(m_maxz - m_minz));
}

void BoundingBox::extend(const v3s &limit)
{
	if (m_minX > m_maxX)
	{
		m_maxX = m_minX = limit.x();
		m_maxY = m_minY = limit.y();
		m_maxZ = m_minZ = limit.z();
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
	if (m_minX > m_maxX)
	{
		m_minX = box.m_minX;
		m_minY = box.m_minY;
		m_minZ = box.m_minZ;

		m_maxX = box.m_maxX;
		m_maxY = box.m_maxY;
		m_maxZ = box.m_maxZ;
	}

	m_minX = std::min(box.minX(), m_minX);
	m_minY = std::min(box.minY(), m_minY);
	m_minZ = std::min(box.minZ(), m_minZ);

	m_maxX = std::max(box.maxX(), m_maxX);
	m_maxY = std::max(box.maxY(), m_maxY);
	m_maxZ = std::max(box.maxZ(), m_maxZ);
}

scalar BoundingBox::xLength() const { return m_maxX - m_minX; }

scalar BoundingBox::yLength() const { return m_maxY - m_minY; }

scalar BoundingBox::zLength() const { return m_maxZ - m_minZ; }

v3s BoundingBox::max() const { return {m_maxX, m_maxY, m_maxZ}; }

v3s BoundingBox::min() const { return {m_minX, m_minY, m_minZ}; }

v3s BoundingBox::center() const
{
	return {midXValue(), midYValue(), midZValue()};
}

scalar BoundingBox::minX() const { return m_minX; }

void BoundingBox::setMinX(scalar minx) { m_minX = minx; }

scalar BoundingBox::minY() const { return m_minY; }

void BoundingBox::setMinY(scalar miny) { m_minY = miny; }

scalar BoundingBox::minZ() const { return m_minZ; }

void BoundingBox::setMinZ(scalar minz) { m_minZ = minz; }

scalar BoundingBox::maxX() const { return m_maxX; }

void BoundingBox::setMaxX(scalar maxx) { m_maxX = maxx; }

scalar BoundingBox::maxY() const { return m_maxY; }

void BoundingBox::setMaxY(scalar maxy) { m_maxY = maxy; }

scalar BoundingBox::maxZ() const { return m_maxZ; }

void BoundingBox::setMaxZ(scalar maxz) { m_maxZ = maxz; }

scalar BoundingBox::midXValue() const
{
	return (m_maxX + m_minX) / scalar(2.0);
}

scalar BoundingBox::midYValue() const
{
	return (m_maxY + m_minY) / scalar(2.0);
}

scalar BoundingBox::midZValue() const
{
	return (m_maxZ + m_minZ) / scalar(2.0);
}

bool BoundingBox::contains(const v3s &v) const
{
	return v.x() >= m_minX && v.y() >= m_minY && v.z() >= m_minZ &&
		   v.x() <= m_maxX && v.y() <= m_maxY && v.z() <= m_maxZ;
}

bool BoundingBox::contains(const v3s &v, scalar tolerance) const
{
	return v.x() >= (m_minX - tolerance) && v.y() >= (m_minY - tolerance) &&
		   v.z() >= (m_minZ - tolerance) && v.x() <= (m_maxX + tolerance) &&
		   v.y() <= (m_maxY + tolerance) && v.z() <= (m_maxZ + tolerance);
}

v3s BoundingBox::diagonal() const
{
	return {m_maxX - m_minX, m_maxY - m_minY, m_maxZ - m_minZ};
}

v3s BoundingBox::closestPointInside(const v3s &pos, scalar offset = 0.0) const
{
	scalar x = std::max(std::min(pos.x(), maxX() - offset), minX() + offset);
	scalar y = std::max(std::min(pos.y(), maxY() - offset), minY() + offset);
	scalar z = std::max(std::min(pos.z(), maxZ() - offset), minZ() + offset);

	return {x, y, z};
}

void BoundingBox::applyTransform(const Matrix44 &m)
{
	/// TODO: account rotations.
	auto &&o = m.origin();

	m_minX += o.x();
	m_minY += o.y();
	m_minZ += o.z();

	m_maxX += o.x();
	m_maxY += o.y();
	m_maxZ += o.z();
}

bool BoundingBox::throwRay(const Ray &ray) const { return hasCollision(ray); }

int BoundingBox::throwRay(const Ray &ray, Collision &col) const
{
	const auto &p = ray.origin();

	if (contains(p))
	{
		col.setPosition(p);
		col.setNormal(ray.direction().inverted());
		col.setValid(true);
		return 1;
	}

	scalar x, y, z;

	switch (ray.qd())
	{
	case Quadrant::k1:
		if (p.x() > m_maxX || p.y() > m_maxY || p.z() > m_maxZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_minX;
		y = m_minY;
		z = m_minZ;
		break;
	case Quadrant::k2:
		if (p.x() > m_maxX || p.y() > m_maxY || p.z() < m_minZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_minX;
		y = m_minY;
		z = m_maxZ;
		break;
	case Quadrant::k3:
		if (p.x() > m_maxX || p.y() < m_minY || p.z() > m_maxZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_minX;
		y = m_maxY;
		z = m_minZ;
		break;
	case Quadrant::k4:
		if (p.x() > m_maxX || p.y() < m_minY || p.z() < m_minZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_minX;
		y = m_maxY;
		z = m_maxZ;
		break;
	case Quadrant::k5:
		if (p.x() < m_minX || p.y() > m_maxY || p.z() > m_maxZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_maxX;
		y = m_minY;
		z = m_minZ;
		break;
	case Quadrant::k6:
		if (p.x() < m_minX || p.y() > m_maxY || p.z() < m_minZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_maxX;
		y = m_minY;
		z = m_maxZ;
		break;
	case Quadrant::k7:
		if (p.x() < m_minX || p.y() < m_minY || p.z() > m_maxZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_maxX;
		y = m_maxY;
		z = m_minZ;
		break;
	case Quadrant::k8:
		if (p.x() < m_minX || p.y() < m_minY || p.z() < m_minZ)
		{
			col.setValid(false);
			return 0;
		}
		x = m_maxX;
		y = m_maxY;
		z = m_maxZ;
		break;
	}

	auto v = MU::rayAndXPlane(ray, x);

	if (MU::inRange(v.y(), m_minY, m_maxY) && MU::inRange(v.z(), m_minZ, m_maxZ))
	{
		col.setPosition(v);
		col.setNormal(ray.direction().xPositive() ? v3s::constMinusY : v3s::constY);
		col.setValid();
		col.setColor(Color::red);
		auto y = (v.y() - m_minY) / (m_maxY - m_minY);
		auto z = (v.z() - m_minZ) / (m_maxZ - m_minZ);

		col.setUV({z, y});
		return 1;
	}
	v = MU::rayAndYPlane(ray, y);

	if (MU::inRange(v.x(), m_minX, m_maxX) && MU::inRange(v.z(), m_minZ, m_maxZ))
	{
		col.setPosition(v);
		col.setNormal(ray.direction().yPositive() ? v3s::constMinusY : v3s::constY);
		col.setValid();
		col.setColor(Color::red);
		auto x = (v.x() - m_minX) / (m_maxX - m_minX);
		auto z = (v.z() - m_minZ) / (m_maxZ - m_minZ);

		col.setUV({x, z});
		return 1;
	}
	v = MU::rayAndZPlane(ray, z);

	if (MU::inRange(v.x(), m_minX, m_maxX) && MU::inRange(v.y(), m_minY, m_maxY))
	{
		col.setPosition(v);
		col.setNormal(ray.direction().zPositive() ? v3s::constMinusZ : v3s::constZ);
		col.setValid();
		col.setColor(Color::red);
		auto x = (v.x() - m_minX) / (m_maxX - m_minX);
		auto y = (v.y() - m_minY) / (m_maxY - m_minY);

		col.setUV({x, y});
		return 1;
	}

	return 0;
}

bool BoundingBox::hasCollision(const Ray &ray) const
{
	const auto &p = ray.origin();

	if (contains(p))
	{
		return true;
	}

	scalar x, y, z;

	switch (ray.qd())
	{
	case Quadrant::k1:
		if (p.x() > m_maxX || p.y() > m_maxY || p.z() > m_maxZ)
		{
			return false;
		}
		x = m_minX;
		y = m_minY;
		z = m_minZ;
		break;
	case Quadrant::k2:
		if (p.x() > m_maxX || p.y() > m_maxY || p.z() < m_minZ)
		{
			return false;
		}
		x = m_minX;
		y = m_minY;
		z = m_maxZ;
		break;
	case Quadrant::k3:
		if (p.x() > m_maxX || p.y() < m_minY || p.z() > m_maxZ)
		{
			return false;
		}
		x = m_minX;
		y = m_maxY;
		z = m_minZ;
		break;
	case Quadrant::k4:
		if (p.x() > m_maxX || p.y() < m_minY || p.z() < m_minZ)
		{
			return false;
		}
		x = m_minX;
		y = m_maxY;
		z = m_maxZ;
		break;
	case Quadrant::k5:
		if (p.x() < m_minX || p.y() > m_maxY || p.z() > m_maxZ)
		{
			return false;
		}
		x = m_maxX;
		y = m_minY;
		z = m_minZ;
		break;
	case Quadrant::k6:
		if (p.x() < m_minX || p.y() > m_maxY || p.z() < m_minZ)
		{
			return false;
		}
		x = m_maxX;
		y = m_minY;
		z = m_maxZ;
		break;
	case Quadrant::k7:
		if (p.x() < m_minX || p.y() < m_minY || p.z() > m_maxZ)
		{
			return false;
		}
		x = m_maxX;
		y = m_maxY;
		z = m_minZ;
		break;
	case Quadrant::k8:
		if (p.x() < m_minX || p.y() < m_minY || p.z() < m_minZ)
		{
			return false;
		}
		x = m_maxX;
		y = m_maxY;
		z = m_maxZ;
		break;
	}

	auto v = MU::rayAndXPlane(ray, x);

	if (MU::inRange(v.y(), m_minY, m_maxY) && MU::inRange(v.z(), m_minZ, m_maxZ))
	{
		return true;
	}
	v = MU::rayAndYPlane(ray, y);

	if (MU::inRange(v.x(), m_minX, m_maxX) && MU::inRange(v.z(), m_minZ, m_maxZ))
	{
		return true;
	}

	v = MU::rayAndZPlane(ray, z);

	return MU::inRange(v.x(), m_minX, m_maxX) &&
		   MU::inRange(v.y(), m_minY, m_maxY);
}
