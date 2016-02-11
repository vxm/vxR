#include <cmath>
#include "vxVector.h"

using namespace vxCore;

vxVector3d vxVector3d::constX	{1.0, 0.0, 0.0};
vxVector3d vxVector3d::constY	{0.0, 1.0, 0.0};
vxVector3d vxVector3d::constZ	{0.0, 0.0, 1.0};
vxVector3d vxVector3d::constXY	{1.0, 1.0, 0.0};
vxVector3d vxVector3d::constXZ	{1.0, 0.0, 1.0};
vxVector3d vxVector3d::constYZ	{0.0, 1.0, 1.0};
vxVector3d vxVector3d::constXYZ	{1.0, 1.0, 1.0};

vxVector3d vxVector3d::constMinusX	{-1.0,  0.0,  0.0};
vxVector3d vxVector3d::constMinusY	{ 0.0, -1.0,  0.0};
vxVector3d vxVector3d::constMinusZ	{ 0.0,  0.0, -1.0};
vxVector3d vxVector3d::constMinusXY	{-1.0, -1.0,  0.0};
vxVector3d vxVector3d::constMinusXZ	{-1.0,  0.0,  1.0};
vxVector3d vxVector3d::constMinusYZ	{ 0.0, -1.0, -1.0};
vxVector3d vxVector3d::constMinusXYZ{-1.0, -1.0, -1.0};

vxVector3d vxVector3d::zero{0.0, 0.0, 0.0};
vxVector2d vxVector2d::zero{0.0, 0.0};

vxVector2d::vxVector2d() 
{}

vxVector2d::vxVector2d(scalar x, scalar y) 
	: m_x(x)
	, m_y(y)
{}

vxVector2d::vxVector2d(vxVector2d &&other)
	:m_x{std::move(other.m_x)}
	,m_y{std::move(other.m_y)}
{
}

vxVector2d::vxVector2d(const vxVector2d &other)
	:m_x(other.m_x)
	,m_y(other.m_y)
{
}

vxVector2d &vxVector2d::operator=(const vxVector2d &other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	
	return *this;
}

void vxVector2d::set(scalar x, scalar y) 
{
	m_x=x;
	m_y=y;
}

void vxVector2d::set(const vxVector2d& enter) 
{
	m_x = enter.m_x;
	m_y = enter.m_y;
}

void vxVector2d::setX(const scalar x)
{
	m_x=x;
}

void vxVector2d::setY(const scalar y) 
{
	m_y=y;
}

vxVector2d vxVector2d::get() const
{
	return *this;
}

void vxVector2d::get(scalar &xi, scalar &yi) const
{
	xi=m_x;
	yi=m_y;
}

scalar vxVector2d::x() const
{
	return m_x;
}

scalar vxVector2d::y() const
{
	return m_y;
}

scalar& vxVector2d::operator[](const unsigned int idx)
{
	return idx == 0 ? m_x : m_y;
}

scalar vxVector2d::operator[](const unsigned int idx) const
{
	return idx == 0 ? m_x : m_y;
}


vxVector2d vxVector2d::asIntPosition() const
{
	return (vxVector2d(floor(m_x),
					   floor(m_y)));
}


scalar vxVector2d::length() const 
{//TODO:consider hypot c++11
	return sqrt(m_x*m_x+m_y*m_y);
}

vxVector2d vxVector2d::unit() const 
{
	scalar lng=length();
	return (vxVector2d(m_x/lng,
					   m_y/lng));
}

vxVector2d vxVector2d::inverted() const
{
	return (vxVector2d(-m_x,
					   -m_y));
}

vxVector2d vxVector2d::operator+(int factor) const 
{
	return (vxVector2d(factor+m_x,
					   factor+m_y));
}

vxVector2d vxVector2d::operator-(int factor) const 
{
	return (vxVector2d(m_x-factor,
					   m_y-factor));
}

vxVector2d vxVector2d::operator*(int factor) const 
{
	return (vxVector2d(factor*m_x,
					   factor*m_y));
}

vxVector2d vxVector2d::operator/(int factor) const 
{
	return (vxVector2d(m_x/(scalar)factor,
					   m_y/(scalar)factor));
}

bool vxVector2d::operator==(const vxVector2d &other) const
{
	return other.m_x == m_x && other.m_y == m_y; 
}

scalar vxVector2d::angle(const vxVector2d& other) const 
{
	return angle()-other.angle();
}

scalar vxVector2d::angle() const 
{
	return atan2(m_y,m_x);
}

vxVector2d vxVector2d::operator/(scalar factor) const 
{
	return (vxVector2d(m_x/(scalar)factor,
					   m_y/(scalar)factor));
}

vxVector2d vxVector2d::operator/(const vxVector2d& entrada) const 
{
	return (vxVector2d(m_x/entrada.m_x,
					   m_y/entrada.m_y));
}

vxVector2d vxVector2d::operator*(scalar factor) const 
{
	return (vxVector2d(factor*m_x,factor*m_y));
}

vxVector2d vxVector2d::operator*(const vxVector2d& entrada) const 
{
	return (vxVector2d(entrada.m_x*m_x,
					   entrada.m_y*m_y));
}

vxVector2d vxVector2d::operator-(scalar factor) const 
{
	return (vxVector2d(m_x-factor,
					   m_y-factor));
}

vxVector2d vxVector2d::operator-(const vxVector2d& entrada) const 
{
	return (vxVector2d(m_x-entrada.m_x,
					   m_y-entrada.m_y));
}


vxVector2d vxVector2d::operator+(scalar factor) const
{
	return (vxVector2d(factor+m_x,
					   factor+m_y));
}

vxVector2d vxVector2d::operator+(const vxVector2d& other) const
{
	return (vxVector2d(other.m_x+m_x,
					   other.m_y+m_y));
}


vxVector3d::vxVector3d()
{}


vxVector3d::vxVector3d(const vxVector3d &other)
	:m_x{other.m_x}
	,m_y{other.m_y}
	,m_z{other.m_z}
{}


vxVector3d::vxVector3d(const vxVector3d &&other)
	:m_x{std::move(other.m_x)}
	,m_y{std::move(other.m_y)}
	,m_z{std::move(other.m_z)}
{}

vxVector3d::vxVector3d(scalar x, scalar y, scalar z)
	: m_x(x)
	, m_y(y)
	, m_z(z)
{
}

void vxVector3d::set(scalar x, scalar y, scalar z) 
{
	m_x=x;
	m_y=y;
	m_z=z;
}

void vxVector3d::set(const vxVector3d& other) 
{
	m_x=other.m_x;
	m_y=other.m_y;
	m_z=other.m_z;
}

void vxVector3d::setX(scalar x) 
{
	m_x=x;
}

void vxVector3d::setY(scalar y) 
{
	m_y=y;
}

void vxVector3d::setZ(scalar z) 
{
	m_z=z;
}

vxVector3d vxVector3d::tiny() const
{
	return {m_x / 10000.0,
			m_y / 10000.0,
			m_z / 10000.0};
}

void vxVector3d::get(scalar &x, scalar &y, scalar &z) const 
{
	x=m_x;
	y=m_y;
	z=m_z;
}

scalar vxVector3d::x() const 
{
	return m_x;
}

scalar vxVector3d::y() const 
{
	return m_y;
}

scalar vxVector3d::z() const 
{
	return m_z;
}

scalar vxVector3d::operator[](const unsigned int idx) const
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

scalar vxVector3d::dot(const vxVector3d& v) const
{
	return v.m_x * m_x 
			+ v.m_y * m_y 
			+ v.m_z * m_z;
}

vxVector3d vxVector3d::cross(const vxVector3d &v) const
{
	return (*this)^v;
}

scalar& vxVector3d::operator[](const unsigned int idx)
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


vxVector3d vxVector3d::inverted() const
{
	return v3(-m_x,-m_y,-m_z);
}

vxVector3d::axis vxVector3d::mainAxis() const
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

scalar vxVector3d::length() const
{
	return sqrt( fabs(m_x*m_x)
				 +fabs(m_y*m_y)
				 +fabs(m_z*m_z));
}

vxVector3d vxVector3d::midPoint(const vxVector3d &other) const
{
	return ((*this)+other)/(scalar)2.0;
}

vxVector3d vxVector3d::aaVector() const
{
	return v3(std::copysign(1.0,m_x),
			  std::copysign(1.0,m_y),
			  std::copysign(1.0,m_z));
}


scalar vxVector3d::distance(const vxVector3d &ref) const
{
	auto dif = (*this)-ref;
	return dif.length();
}

vxVector3d vxVector3d::floorVector() const
{
	return v3(floor(m_x),floor(m_y),floor(m_z));
}


vxVector3d vxVector3d::ceilVector() const
{
	return v3(ceil(m_x),ceil(m_y),ceil(m_z));
}

vxVector3d vxVector3d::unit() const
{
	const auto&& lng=length();
	return v3(m_x/lng, m_y/lng, m_z/lng);
}

void vxVector3d::setUnit() 
{
	scalar lng=length();
	m_x/=lng;
	m_y/=lng;
	m_z/=lng;
}

vxVector3d vxVector3d::operator+(int factor) const
{
	return v3(factor+m_x, factor+m_y, factor+m_z);
}

vxVector3d& vxVector3d::operator-=(const vxVector3d &entrada)
{
	m_x-=entrada.m_x;
	m_y-=entrada.m_y;
	m_z-=entrada.m_z;
	
	return (*this);
}

vxVector3d& vxVector3d::operator*=(const vxVector3d &entrada)
{
	m_x*=entrada.m_x;
	m_y*=entrada.m_y;
	m_z*=entrada.m_z;
	
	return (*this);
}

vxVector3d vxVector3d::operator^(const vxVector3d& b) const
{
	return v3((m_y*b.m_z)-(m_z*b.m_y),
			  (m_z*b.m_x)-(m_x*b.m_z),
			  (m_x*b.m_y)-(m_y*b.m_x));
}

bool vxVector3d::operator==(const vxVector3d& other) const
{
	return m_x==other.m_x
			&& m_y==other.m_y
			&& m_z==other.m_z;
}

bool vxVector3d::operator!=(const vxVector3d& other) const
{
	return m_x!=other.m_x
			|| m_y!=other.m_y
			|| m_z!=other.m_z;
}

bool vxVector3d::operator>(const vxVector3d& other) const
{
	const auto&& a = m_x*m_x+m_y*m_y+m_z*m_z;
	const auto&& b = other.m_x*other.m_x+other.m_y*other.m_y+other.m_z*other.m_z;
	
	return a>b;
}

bool vxVector3d::operator<(const vxVector3d& other) const
{
	const auto&& a = m_x*m_x+m_y*m_y+m_z*m_z;
	const auto&& b = other.m_x*other.m_x+other.m_y*other.m_y+other.m_z*other.m_z;
	
	return a<b;
}

bool vxVector3d::follows(const vxVector3d &direction) const
{
	return dot(direction)<0.0;
}

scalar vxVector3d::angle(const vxVector3d& b) const
{
	scalar an = dot(b) / (length() * b.length());
	return acos(an);
}

scalar vxVector3d::angleXY(const vxVector3d& other) const
{
	return angleXY()-other.angleXY();
}

scalar vxVector3d::angleYZ(const vxVector3d &other) const
{
	return angleYZ()-other.angleYZ();
}

scalar vxVector3d::angleZX(const vxVector3d &other) const
{
	return angleZX()-other.angleZX();
}

scalar vxVector3d::angleXY() const 
{
	return atan2(m_y,
				 m_x);
}

scalar vxVector3d::angleYZ() const 
{
	return atan2(m_z,
				 m_y);
}

scalar vxVector3d::angleZX() const 
{
	return atan2(m_x,
				 m_z);
}

vxVector3d vxVector3d::operator=(const vxVector3d &otro)
{
	set(otro.m_x,otro.y(),otro.m_z);
	return (*this);
}

vxVector3d vxVector3d::abs() const
{
	return vxVector3d(fabs(m_x),fabs(m_y),fabs(m_z));
}

vxVector3d vxVector3d::rotateX(scalar ang)
{
	setUnit();
	scalar angk = angleZX();
	angk+=ang;
	m_x=(sin(angk));
	m_z=(cos(angk));
	return *this;
}

vxVector3d vxVector3d::rotateY(scalar ang)
{
	const auto&& angk = ang + angleXY();
	return v3(m_z, cos(angk), sin(angk));
}

vxVector3d vxVector3d::rotateZ(scalar ang)
{
	vxVector3d ret;
	ang+=angleYZ();
	ret.set( cos(ang) , sin(ang) , m_x );
	return ret;
}

vxVector3d vxVector3d::operator/(scalar factor) const
{
	return v3(m_x/(scalar)factor,
			  m_y/(scalar)factor,
			  m_z/(scalar)factor);
}

vxVector3d vxVector3d::operator/(const vxVector3d &entrada) const
{
	return v3(entrada.m_x/m_x,
			  entrada.m_y/m_y,
			  entrada.m_z/m_z);
}

vxVector3d vxVector3d::operator*(scalar factor) const 
{
	return v3(factor*m_x,
			  factor*m_y,
			  factor*m_z);
}

vxVector3d vxVector3d::operator*(const vxVector3d other) const 
{
	return v3(other.m_x*m_x,
			  other.m_y*m_y,
			  other.m_z*m_z);
}

vxVector3d vxVector3d::operator-(scalar factor) const 
{
	return v3(m_x-factor,
			  m_y-factor,
			  m_z-factor);
}

vxVector3d vxVector3d::operator-(const vxVector3d &entrada) const 
{
	return v3(m_x-entrada.m_x,
			  m_y-entrada.m_y,
			  m_z-entrada.m_z);
}

vxVector3d vxVector3d::operator+(scalar factor) const
{
	return v3(factor+m_x,factor+m_y,factor+m_z);
}

vxVector3d vxVector3d::operator+(const vxVector3d &entrada) const
{
	return v3(entrada.m_x+m_x,
			  entrada.m_y+m_y,
			  entrada.m_z+m_z);
}

vxVector3d& vxVector3d::operator+=(const vxVector3d &entrada)
{
	m_x+=entrada.m_x;
	m_y+=entrada.m_y;
	m_z+=entrada.m_z;
	return (*this);
}

vxVector2d vxVector3d::vectorXY() const
{
	return (vxVector2d(m_x,m_y));
}

vxVector2d vxVector3d::vectorYZ() const
{
	return (vxVector2d(m_x,m_y));
}

vxVector2d vxVector3d::vectorZX() const
{
	return (vxVector2d(m_x,m_y));
}

bool vxVector3d::xPositive() const
{
	return m_x>=0.0;
}

bool vxVector3d::yPositive() const
{
	return m_y>=0.0;
}

bool vxVector3d::zPositive() const
{
	return m_z>=0.0;
}

