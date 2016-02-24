#include <cmath>
#include "vxVector.h"

using namespace vxCore;

v3s v3s::constX	{1.0, 0.0, 0.0};
v3s v3s::constY	{0.0, 1.0, 0.0};
v3s v3s::constZ	{0.0, 0.0, 1.0};
v3s v3s::constXY	{1.0, 1.0, 0.0};
v3s v3s::constXZ	{1.0, 0.0, 1.0};
v3s v3s::constYZ	{0.0, 1.0, 1.0};
v3s v3s::constXYZ	{1.0, 1.0, 1.0};

v3s v3s::constMinusX	{-1.0,  0.0,  0.0};
v3s v3s::constMinusY	{ 0.0, -1.0,  0.0};
v3s v3s::constMinusZ	{ 0.0,  0.0, -1.0};
v3s v3s::constMinusXY	{-1.0, -1.0,  0.0};
v3s v3s::constMinusXZ	{-1.0,  0.0,  1.0};
v3s v3s::constMinusYZ	{ 0.0, -1.0, -1.0};
v3s v3s::constMinusXYZ{-1.0, -1.0, -1.0};

v3s v3s::zero{0.0, 0.0, 0.0};
v2s v2s::zero{0.0, 0.0};

v2s::v2s() 
{}

v2s::v2s(scalar x, scalar y) 
	: m_x(x)
	, m_y(y)
{}

v2s::v2s(v2s &&other)
	:m_x{std::move(other.m_x)}
	,m_y{std::move(other.m_y)}
{
}

v2s::v2s(const v2s &other)
	:m_x(other.m_x)
	,m_y(other.m_y)
{
}

v2s &v2s::operator=(const v2s &other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	
	return *this;
}

void v2s::set(scalar x, scalar y) 
{
	m_x=x;
	m_y=y;
}

void v2s::set(const v2s& enter) 
{
	m_x = enter.m_x;
	m_y = enter.m_y;
}

void v2s::setX(const scalar x)
{
	m_x=x;
}

void v2s::setY(const scalar y) 
{
	m_y=y;
}

v2s v2s::get() const
{
	return *this;
}

void v2s::get(scalar &xi, scalar &yi) const
{
	xi=m_x;
	yi=m_y;
}

scalar v2s::x() const
{
	return m_x;
}

scalar v2s::y() const
{
	return m_y;
}

scalar& v2s::operator[](const unsigned int idx)
{
	return idx == 0 ? m_x : m_y;
}

scalar v2s::operator[](const unsigned int idx) const
{
	return idx == 0 ? m_x : m_y;
}


v2s v2s::asIntPosition() const
{
	return (v2s(floor(m_x),
					   floor(m_y)));
}


scalar v2s::length() const 
{//TODO:consider hypot c++11
	return sqrt(m_x*m_x+m_y*m_y);
}

v2s v2s::unit() const 
{
	scalar lng=length();
	return (v2s(m_x/lng,
					   m_y/lng));
}

v2s v2s::inverted() const
{
	return (v2s(-m_x,
					   -m_y));
}

v2s v2s::operator+(int factor) const 
{
	return (v2s(factor+m_x,
					   factor+m_y));
}

v2s v2s::operator-(int factor) const 
{
	return (v2s(m_x-factor,
					   m_y-factor));
}

v2s v2s::operator*(int factor) const 
{
	return (v2s(factor*m_x,
					   factor*m_y));
}

v2s v2s::operator/(int factor) const 
{
	return (v2s(m_x/(scalar)factor,
					   m_y/(scalar)factor));
}

bool v2s::operator==(const v2s &other) const
{
	return other.m_x == m_x && other.m_y == m_y; 
}

bool v2s::operator!=(const v2s &other) const
{
	return other.m_x != m_x || other.m_y != m_y;
}

scalar v2s::angle(const v2s& other) const 
{
	return angle()-other.angle();
}

scalar v2s::angle() const 
{
	return atan2(m_y,m_x);
}

v2s v2s::operator/(scalar factor) const 
{
	return (v2s(m_x/(scalar)factor,
					   m_y/(scalar)factor));
}

v2s v2s::operator/(const v2s& other) const 
{
	return (v2s(m_x/other.m_x,
					   m_y/other.m_y));
}

v2s v2s::operator*(scalar factor) const 
{
	return (v2s(factor*m_x,factor*m_y));
}

v2s v2s::operator*(const v2s& other) const 
{
	return (v2s(other.m_x*m_x,
					   other.m_y*m_y));
}

v2s v2s::operator-(scalar factor) const 
{
	return (v2s(m_x-factor,
					   m_y-factor));
}

v2s v2s::operator-(const v2s& other) const 
{
	return (v2s(m_x-other.m_x,
					   m_y-other.m_y));
}


v2s v2s::operator+(scalar factor) const
{
	return (v2s(factor+m_x,
					   factor+m_y));
}

v2s v2s::operator+(const v2s& other) const
{
	return (v2s(other.m_x+m_x,
					   other.m_y+m_y));
}


v3s::v3s()
{}


v3s::v3s(const v3s &other)
	:m_x{other.m_x}
	,m_y{other.m_y}
	,m_z{other.m_z}
{}


v3s::v3s(const v3s &&other)
	:m_x{std::move(other.m_x)}
	,m_y{std::move(other.m_y)}
	,m_z{std::move(other.m_z)}
{}

v3s::v3s(scalar x, scalar y, scalar z)
	: m_x(x)
	, m_y(y)
	, m_z(z)
{
}

void v3s::set(scalar x, scalar y, scalar z) 
{
	m_x=x;
	m_y=y;
	m_z=z;
}

void v3s::set(const v3s& other) 
{
	m_x=other.m_x;
	m_y=other.m_y;
	m_z=other.m_z;
}

void v3s::setX(scalar x) 
{
	m_x=x;
}

void v3s::setY(scalar y) 
{
	m_y=y;
}

void v3s::setZ(scalar z) 
{
	m_z=z;
}

bool v3s::isCloser(const v3s &a, const v3s &b) const
{
	//TODO:sqrDistance
	return distance(a)<distance(b);
}

v3s v3s::tiny() const
{
	return {m_x / 10000.0,
				m_y / 10000.0,
				m_z / 10000.0};
}

scalar v3s::sqrDistance(const v3s &in) const
{
	return (in.m_x-m_x) 
			* (in.m_y-m_y) 
			* (in.m_z-m_z);
}

void v3s::get(scalar &x, scalar &y, scalar &z) const 
{
	x=m_x;
	y=m_y;
	z=m_z;
}

scalar v3s::x() const 
{
	return m_x;
}

scalar v3s::y() const 
{
	return m_y;
}

scalar v3s::z() const 
{
	return m_z;
}

scalar v3s::operator[](const unsigned int idx) const
{
	switch(idx)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	default:
		return m_z;
	}
}

scalar v3s::dot(const v3s& v) const
{
	return v.m_x * m_x 
			+ v.m_y * m_y 
			+ v.m_z * m_z;
}

v3s v3s::cross(const v3s &v) const
{
	return (*this)^v;
}

scalar& v3s::operator[](const unsigned int idx)
{
	switch(idx)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	default:
		return m_z;
	}
}


v3s v3s::inverted() const
{
	return v3s(-m_x,-m_y,-m_z);
}

v3s::axis v3s::mainAxis() const
{
	//TODO:be more clever, like commented code
	
	if(fabs(m_x)>fabs(m_y) && fabs(m_x)>fabs(m_z))
	{
		return axis::kX;
	}
	
	if(fabs(m_y)>fabs(m_x) && fabs(m_x)>fabs(m_z))
	{
		return axis::kY;
	}
	
	return axis::kZ;
}

scalar v3s::length() const
{
	return sqrt( fabs(m_x*m_x)
				 +fabs(m_y*m_y)
				 +fabs(m_z*m_z));
}

v3s v3s::midPoint(const v3s &other) const
{
	return ((*this)+other)/(scalar)2.0;
}

v3s v3s::aaVector() const
{
	return v3s(std::copysign(1.0,m_x),
			  std::copysign(1.0,m_y),
			  std::copysign(1.0,m_z));
}


scalar v3s::distance(const v3s &ref) const
{
	auto dif = (*this)-ref;
	return dif.length();
}

v3s v3s::floorVector() const
{
	return v3s(floor(m_x),floor(m_y),floor(m_z));
}


v3s v3s::ceilVector() const
{
	return v3s(ceil(m_x),ceil(m_y),ceil(m_z));
}

v3s v3s::unit() const
{
	const auto&& lng=length();
	return v3s(m_x/lng, m_y/lng, m_z/lng);
}

void v3s::setUnit() 
{
	scalar lng=length();
	m_x/=lng;
	m_y/=lng;
	m_z/=lng;
}

v3s v3s::operator+(int factor) const
{
	return v3s(factor+m_x, factor+m_y, factor+m_z);
}

v3s& v3s::operator-=(const v3s &other)
{
	m_x-=other.m_x;
	m_y-=other.m_y;
	m_z-=other.m_z;
	
	return (*this);
}

v3s& v3s::operator*=(const v3s &other)
{
	m_x*=other.m_x;
	m_y*=other.m_y;
	m_z*=other.m_z;
	
	return (*this);
}

v3s v3s::operator^(const v3s& b) const
{
	return v3s((m_y*b.m_z)-(m_z*b.m_y),
			  (m_z*b.m_x)-(m_x*b.m_z),
			  (m_x*b.m_y)-(m_y*b.m_x));
}

bool v3s::operator==(const v3s& other) const
{
	return m_x==other.m_x
			&& m_y==other.m_y
			&& m_z==other.m_z;
}

bool v3s::operator!=(const v3s& other) const
{
	return m_x!=other.m_x
			|| m_y!=other.m_y
			|| m_z!=other.m_z;
}

bool v3s::operator>(const v3s& other) const
{
	const auto&& a = m_x*m_x+m_y*m_y+m_z*m_z;
	const auto&& b = other.m_x*other.m_x+other.m_y*other.m_y+other.m_z*other.m_z;
	
	return a>b;
}

bool v3s::operator<(const v3s& other) const
{
	const auto&& a = m_x*m_x+m_y*m_y+m_z*m_z;
	const auto&& b = other.m_x*other.m_x+other.m_y*other.m_y+other.m_z*other.m_z;
	
	return a<b;
}

bool v3s::follows(const v3s &direction) const
{
	return dot(direction)<0.0;
}

scalar v3s::angle(const v3s& b) const
{
	scalar an = dot(b) / (length() * b.length());
	return acos(an);
}

scalar v3s::angleXY(const v3s& other) const
{
	return angleXY()-other.angleXY();
}

scalar v3s::angleYZ(const v3s &other) const
{
	return angleYZ()-other.angleYZ();
}

scalar v3s::angleZX(const v3s &other) const
{
	return angleZX()-other.angleZX();
}

scalar v3s::angleXY() const 
{
	return atan2(m_y,
				 m_x);
}

scalar v3s::angleYZ() const 
{
	return atan2(m_z,
				 m_y);
}

scalar v3s::angleZX() const 
{
	return atan2(m_x,
				 m_z);
}

v3s v3s::operator=(const v3s &otro)
{
	set(otro.m_x,otro.y(),otro.m_z);
	return (*this);
}

v3s v3s::abs() const
{
	return v3s(fabs(m_x),fabs(m_y),fabs(m_z));
}

v3s v3s::rotateX(scalar ang)
{
	setUnit();
	scalar angk = angleZX();
	angk+=ang;
	m_x=(sin(angk));
	m_z=(cos(angk));
	return *this;
}

v3s v3s::rotateY(scalar ang)
{
	const auto&& angk = ang + angleXY();
	return v3s(m_z, cos(angk), sin(angk));
}

v3s v3s::rotateZ(scalar ang)
{
	v3s ret;
	ang+=angleYZ();
	ret.set( cos(ang) , sin(ang) , m_x );
	return ret;
}

v3s v3s::operator/(scalar factor) const
{
	return v3s(m_x/(scalar)factor,
			  m_y/(scalar)factor,
			  m_z/(scalar)factor);
}

v3s v3s::operator/(const v3s &other) const
{
	return v3s(other.m_x/m_x,
			  other.m_y/m_y,
			  other.m_z/m_z);
}

v3s v3s::operator*(scalar factor) const 
{
	return v3s(factor*m_x,
			  factor*m_y,
			  factor*m_z);
}

v3s v3s::operator*(const v3s other) const 
{
	return v3s(other.m_x*m_x,
			  other.m_y*m_y,
			  other.m_z*m_z);
}

v3s v3s::operator-(scalar factor) const 
{
	return v3s(m_x-factor,
			  m_y-factor,
			  m_z-factor);
}

v3s v3s::operator-(const v3s &other) const 
{
	return v3s(m_x-other.m_x,
			  m_y-other.m_y,
			  m_z-other.m_z);
}

v3s v3s::operator+(scalar factor) const
{
	return v3s(factor+m_x,factor+m_y,factor+m_z);
}

v3s v3s::operator+(const v3s &other) const
{
	return v3s(other.m_x+m_x,
			  other.m_y+m_y,
			  other.m_z+m_z);
}

v3s& v3s::operator+=(const v3s &other)
{
	m_x+=other.m_x;
	m_y+=other.m_y;
	m_z+=other.m_z;
	return (*this);
}

v2s v3s::vectorXY() const
{
	return (v2s(m_x,m_y));
}

v2s v3s::vectorYZ() const
{
	return (v2s(m_x,m_y));
}

v2s v3s::vectorZX() const
{
	return (v2s(m_x,m_y));
}

bool v3s::xPositive() const
{
	return m_x>=0.0;
}

bool v3s::yPositive() const
{
	return m_y>=0.0;
}

bool v3s::zPositive() const
{
	return m_z>=0.0;
}

