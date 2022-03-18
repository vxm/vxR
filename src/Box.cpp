#include "Box.h"

using namespace vxCore;

v3s Box::position() const { return m_position; }

void Box::setPosition(const v3s &position) { m_position = position; }

scalar Box::size() const { return m_size; }

void Box::setSize(scalar size) { m_size = size; }

void Box::set(const v3s &pos) { m_position = pos; }

void Box::set(const v3s &pos, scalar size)
{
	m_position = pos;
	m_size = size;
}

bool Box::throwRay(const Ray &ray) const
{
	if (contains(ray.origin()))
	{
		return false;
	}

	const auto &&p = m_position - ray.origin();
	const auto &&min = p - m_size / scalar(2.0);
	const auto &&max = p + m_size / scalar(2.0);

	bool bMax = std::signbit(ray.direction().x());

	const auto hitX = MU::rayAndXPlane(ray, bMax ? max.x() : min.x());
	if (std::islessequal(hitX.z(), max.z()) &&
		std::isgreaterequal(hitX.z(), min.z()) &&
		std::islessequal(hitX.y(), max.y()) &&
		std::isgreaterequal(hitX.y(), min.y()))
	{
		return true;
	}

	bMax = std::signbit(ray.direction().z());
	const auto hitZ = MU::rayAndZPlane(ray, bMax ? max.z() : min.z());
	if (std::islessequal(hitZ.x(), max.x()) &&
		std::isgreaterequal(hitZ.x(), min.x()) &&
		std::islessequal(hitZ.y(), max.y()) &&
		std::isgreaterequal(hitZ.y(), min.y()))
	{
		return true;
	}

	bMax = std::signbit(ray.direction().y());
	const auto hitY = MU::rayAndYPlane(ray, bMax ? max.y() : min.y());

	return std::islessequal(hitY.x(), max.x()) &&
		   std::isgreaterequal(hitY.x(), min.x()) &&
		   std::islessequal(hitY.z(), max.z()) &&
		   std::isgreaterequal(hitY.z(), min.z());
}

int Box::throwRay(const Ray &ray, Collision &collide) const
{
	const auto &&p = m_position - ray.origin();
	const auto &&mSize = m_size / scalar(2.0);
	const auto &&min = p - mSize;
	const auto &&max = p + mSize;

	bool aMax = std::signbit(ray.direction().x());

	const auto &&hitX = MU::rayAndXPlane(ray, aMax ? max.x() : min.x());
	if (hitX.z() <= max.z() && hitX.z() >= min.z() && hitX.y() <= max.y() &&
		hitX.y() >= min.y())
	{
		collide.setValid(true);
		collide.setNormal(aMax ? v3s::constX : v3s::constMinusX);
		collide.setPosition(hitX + ray.origin());
		collide.setUV(v2s{(mSize + hitX.z() - p.z()) / m_size,
						  (mSize + hitX.y() - p.y()) / m_size});
		return 1;
	}

	bool bMax = std::signbit(ray.direction().z());
	const auto &&hitZ = MU::rayAndZPlane(ray, bMax ? max.z() : min.z());
	if (hitZ.x() <= max.x() && hitZ.x() >= min.x() && hitZ.y() <= max.y() &&
		hitZ.y() >= min.y())
	{
		collide.setValid(true);
		collide.setNormal(bMax ? v3s::constZ : v3s::constMinusZ);
		collide.setPosition(hitZ + ray.origin());
		collide.setUV(v2s{(mSize + hitZ.x() - p.x()) / m_size,
						  (mSize + hitZ.y() - p.y()) / m_size});
		return 1;
	}

	bool cMax = std::signbit(ray.direction().y());
	const auto &&hitY = MU::rayAndYPlane(ray, cMax ? max.y() : min.y());
	if (hitY.x() <= max.x() && hitY.x() >= min.x() && hitY.z() <= max.z() &&
		hitY.z() >= min.z())
	{
		collide.setValid(true);
		collide.setNormal(cMax ? v3s::constY : v3s::constMinusY);
		collide.setPosition(hitY + ray.origin());
		collide.setUV(v2s{(mSize + hitY.x() - p.x()) / m_size,
						  (mSize + hitY.z() - p.z()) / m_size});
		return 1;
	}
	collide.setValid(false);
	return 0;
}

bool Box::hasCollision(const Ray &ray) const
{
	if (contains(ray.origin()))
	{
		return false;
	}

	const auto &&p = m_position - ray.origin();
	const auto mSize = m_size / scalar(2.0);

	const auto &&min = p - mSize;
	const auto &&max = p + mSize;

	auto bMax = std::signbit(max.x());
	const auto &&hitX =
		MU::rectAndXPlane(ray.direction(), bMax ? max.x() : min.x());
	if (std::islessequal(hitX.z(), max.z()) &&
		std::isgreaterequal(hitX.z(), min.z()) &&
		std::islessequal(hitX.y(), max.y()) &&
		std::isgreaterequal(hitX.y(), min.y()))
	{
		return true;
	}

	bMax = std::signbit(max.y());
	const auto hitY =
		MU::rectAndYPlane(ray.direction(), bMax ? max.y() : min.y());
	if (std::islessequal(hitY.x(), max.x()) &&
		std::isgreaterequal(hitY.x(), min.x()) &&
		std::islessequal(hitY.z(), max.z()) &&
		std::isgreaterequal(hitY.z(), min.z()))
	{
		return true;
	}

	bMax = std::signbit(max.z());
	const auto hitZ =
		MU::rectAndZPlane(ray.direction(), bMax ? max.z() : min.z());
	return std::islessequal(hitZ.x(), max.x()) &&
		   std::isgreaterequal(hitZ.x(), min.x()) &&
		   std::islessequal(hitZ.y(), max.y()) &&
		   std::isgreaterequal(hitZ.y(), min.y());
}
