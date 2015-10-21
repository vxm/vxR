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

vxVector2d::vxVector2d(double x, double y) 
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

void vxVector2d::set(double x, double y) 
{
	m_x=x;
	m_y=y;
}

void vxVector2d::set(const vxVector2d& enter) 
{
	m_x = enter.m_x;
	m_y = enter.m_y;
}

void vxVector2d::setX(const double x)
{
	m_x=x;
}

void vxVector2d::setY(const double y) 
{
	m_y=y;
}

vxVector2d vxVector2d::get() const
{
	return *this;
}

void vxVector2d::get(double &xi, double &yi) const
{
	xi=m_x;
	yi=m_y;
}

double vxVector2d::x() const
{
	return m_x;
}

double vxVector2d::y() const
{
	return m_y;
}

double& vxVector2d::operator[](const unsigned int idx)
{
	return idx == 0 ? m_x : m_y;
}

double vxVector2d::operator[](const unsigned int idx) const
{
	return idx == 0 ? m_x : m_y;
}


vxVector2d vxVector2d::asIntPosition() const
{
	return std::move(vxVector2d(floor(m_x),
								floor(m_y)));
}


double vxVector2d::length() const 
{//TODO:consider hypot c++11
	return sqrt(m_x*m_x+m_y*m_y);
}

vxVector2d vxVector2d::unit() const 
{
	double lng=length();
	return std::move(vxVector2d(m_x/lng,
								m_y/lng));
}

vxVector2d vxVector2d::inverted() const
{
	return std::move(vxVector2d(-m_x,
								-m_y));
}

vxVector2d vxVector2d::operator+(int factor) const 
{
	return std::move(vxVector2d(factor+m_x,
								factor+m_y));
}

vxVector2d vxVector2d::operator-(int factor) const 
{
	return std::move(vxVector2d(m_x-factor,
					  m_y-factor));
}

vxVector2d vxVector2d::operator*(int factor) const 
{
	return std::move(vxVector2d(factor*m_x,
								factor*m_y));
}

vxVector2d vxVector2d::operator/(int factor) const 
{
	return std::move(vxVector2d(m_x/(double)factor,
								m_y/(double)factor));
}

bool vxVector2d::operator==(const vxVector2d &other) const
{
	return other.m_x == m_x && other.m_y == m_y; 
}

double vxVector2d::angle(const vxVector2d& other) const 
{
	return angle()-other.angle();
}

double vxVector2d::angle() const 
{
	return atan2(m_y,m_x);
}

vxVector2d vxVector2d::operator/(float factor) const 
{
	return std::move(vxVector2d(m_x/(double)factor,
					  m_y/(double)factor));
}

vxVector2d vxVector2d::operator/(double factor) const 
{
	return std::move(vxVector2d(m_x/(double)factor,
					  m_y/(double)factor));
}

vxVector2d vxVector2d::operator/(const vxVector2d& entrada) const 
{
	return std::move(vxVector2d(m_x/entrada.m_x,
					  m_y/entrada.m_y));
}

vxVector2d vxVector2d::operator*(float factor) const 
{
	return std::move(vxVector2d(factor*m_x,factor*m_y));
}

vxVector2d vxVector2d::operator*(double factor) const 
{
	return std::move(vxVector2d(factor*m_x,factor*m_y));
}

vxVector2d vxVector2d::operator*(const vxVector2d& entrada) const 
{
	return std::move(vxVector2d(entrada.m_x*m_x,
					  entrada.m_y*m_y));
}

vxVector2d vxVector2d::operator-(float factor) const 
{
	return std::move(vxVector2d(m_x-factor,
								m_y-factor));
}

vxVector2d vxVector2d::operator-(double factor) const 
{
	return std::move(vxVector2d(m_x-factor,
								m_y-factor));
}

vxVector2d vxVector2d::operator-(const vxVector2d& entrada) const 
{
	return std::move(vxVector2d(m_x-entrada.m_x,
					  m_y-entrada.m_y));
}

vxVector2d vxVector2d::operator+(float factor) const 
{
	return std::move(vxVector2d(factor+m_x,
					  factor+m_y));
}

vxVector2d vxVector2d::operator+(double factor) const
{
	return std::move(vxVector2d(factor+m_x,
								factor+m_y));
}

vxVector2d vxVector2d::operator+(const vxVector2d& other) const
{
	return std::move(vxVector2d(other.m_x+m_x,
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

vxVector3d::vxVector3d(double x, double y, double z)
	: m_x(x)
	, m_y(y)
	, m_z(z)
{
}

void vxVector3d::set(double x, double y, double z) 
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

void vxVector3d::setX(double x) 
{
	m_x=x;
}

void vxVector3d::setY(double y) 
{
	m_y=y;
}

void vxVector3d::setZ(double z) 
{
	m_z=z;
}

vxVector3d vxVector3d::get() const
{
	return *this;
}

void vxVector3d::get(double &x, double &y, double &z) const 
{
	x=m_x;
	y=m_y;
	z=m_z;
}

double vxVector3d::x() const 
{
	return m_x;
}

double vxVector3d::y() const 
{
	return m_y;
}

double vxVector3d::z() const 
{
	return m_z;
}

double vxVector3d::operator[](const unsigned int idx) const
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

double vxVector3d::dot(const vxVector3d &v) const
{
	const auto tmp(v*(*this));
	return (tmp.m_x + tmp.m_y + tmp.m_z) ;
}

vxVector3d vxVector3d::cross(const vxVector3d &v) const
{
	return (*this)^v;
}

double& vxVector3d::operator[](const unsigned int idx)
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
	return std::move(vxVector3d(-m_x,
								-m_y,
								-m_z));
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

double vxVector3d::length() const
{
	return sqrt( fabs(m_x*m_x)
				+fabs(m_y*m_y)
				+fabs(m_z*m_z));
}

vxVector3d vxVector3d::aaVector() const
{
	return std::move(vxVector3d(std::copysign(1.0,m_x),
								std::copysign(1.0,m_y),
								std::copysign(1.0,m_z)));
}


double vxVector3d::distance(const vxVector3d &ref) const
{
	auto dif = (*this)-ref;
	return dif.length();
}

vxVector3d vxVector3d::floorVector() const
{
	return std::move(vxVector3d(floor(m_x),floor(m_y),floor(m_z)));
}


vxVector3d vxVector3d::ceilVector() const
{
	return std::move(vxVector3d(ceil(m_x),ceil(m_y),ceil(m_z)));
}

vxVector3d vxVector3d::unit() const
{
	const auto&& lng=length();
	return std::move(vxVector3d(m_x/lng, m_y/lng, m_z/lng));
}

void vxVector3d::setUnit() 
{
	double lng=length();
	m_x/=lng;
	m_y/=lng;
	m_z/=lng;
}

vxVector3d vxVector3d::operator+(int factor) const
{
	return std::move(vxVector3d(factor+m_x,
								factor+m_y,
								factor+m_z));
}

vxVector3d vxVector3d::operator-=(const vxVector3d &entrada)
{
	m_x-=entrada.m_x;
	m_y-=entrada.m_y;
	m_z-=entrada.m_z;

	return std::move(*this);
}

vxVector3d vxVector3d::operator-(int factor) const 
{
	return std::move(vxVector3d(m_x-factor,
								m_y-factor,
								m_z-factor));
}

vxVector3d vxVector3d::operator*(float factor) const 
{
	return std::move(vxVector3d(factor*m_x,
								factor*m_y,
								factor*m_z));
}

vxVector3d vxVector3d::operator*(int factor) const 
{
	return std::move(vxVector3d(factor*m_x,
								factor*m_y,
								factor*m_z));
}

vxVector3d vxVector3d::operator/(int factor) const 
{
	return std::move(vxVector3d(m_x/(double)factor,
						m_y/(double)factor,
						m_z/(double)factor));
}

vxVector3d vxVector3d::operator^(const vxVector3d& b) const
{
	return std::move(vxVector3d((m_y*b.m_z)+(m_z*b.m_y),
								-((m_x*b.m_z)+(m_z*b.m_x)),
								(m_x*b.m_y)+(m_y*b.m_x)));
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
	return dot(direction)<=0.0;
}

double vxVector3d::angle(const vxVector3d& b) const
{
	double an = dot(b) / (length() * b.length());
	return acos(an);
}

double vxVector3d::angleXY(const vxVector3d& other) const
{
	return angleXY()-other.angleXY();
}

double vxVector3d::angleYZ(const vxVector3d &other) const
{
	return angleYZ()-other.angleYZ();
}

double vxVector3d::angleZX(const vxVector3d &other) const
{
	return angleZX()-other.angleZX();
}

double vxVector3d::angleXY() const 
{
	return atan2(m_y,
				 m_x);
}

double vxVector3d::angleYZ() const 
{
	return atan2(m_z,
				 m_y);
}

double vxVector3d::angleZX() const 
{
	return atan2(m_x,
				 m_z);
}

vxVector3d vxVector3d::operator=(const vxVector3d &otro)
{
	set(otro.m_x,otro.y(),otro.m_z);
	return std::move(*this);
}

vxVector3d vxVector3d::rotateX(double ang)
{
	setUnit();
	double angk = angleZX();
	angk+=ang;
	m_x=(sin(angk));
	m_z=(cos(angk));
	return *this;
}

vxVector3d vxVector3d::rotateY(double ang)
{
	const auto&& angk = ang + angleXY();
	return std::move(vxVector3d(m_z, cos(angk), sin(angk)));
}

vxVector3d vxVector3d::rotateZ(double ang)
{
	vxVector3d ret;
	ang+=angleYZ();
	ret.set( cos(ang) , sin(ang) , m_x );
	return ret;
}

vxVector3d vxVector3d::operator/(float factor) const
{
	return std::move(vxVector3d(m_x/(double)factor,
								m_y/(double)factor,
								m_z/(double)factor));
}

vxVector3d vxVector3d::operator/(double factor) const
{
	return std::move(vxVector3d(m_x/(double)factor,
								m_y/(double)factor,
								m_z/(double)factor));
}

vxVector3d vxVector3d::operator/(const vxVector3d &entrada) const
{
	return std::move(vxVector3d(entrada.m_x/m_x,
								entrada.m_y/m_y,
								entrada.m_z/m_z));
}

vxVector3d vxVector3d::operator*(double factor) const 
{
	return std::move(vxVector3d(factor*m_x,
								factor*m_y,
								factor*m_z));
}

vxVector3d vxVector3d::operator*(const vxVector3d other) const 
{
	return std::move(vxVector3d(other.m_x*m_x,
								other.m_y*m_y,
								other.m_z*m_z));
}

vxVector3d vxVector3d::operator-(float factor) const 
{
	return std::move(vxVector3d(m_x-factor,
					  m_y-factor,
					  m_z-factor));
}

vxVector3d vxVector3d::operator-(double factor) const 
{
	return std::move(vxVector3d(m_x-factor,
					  m_y-factor,
					  m_z-factor));
}

vxVector3d vxVector3d::operator-(const vxVector3d &entrada) const 
{
	return std::move(vxVector3d(m_x-entrada.m_x,
					  m_y-entrada.m_y,
					  m_z-entrada.m_z));
}

vxVector3d vxVector3d::operator+(float factor) const
{
	return std::move(vxVector3d(factor+m_x,factor+m_y,factor+m_z));
}

vxVector3d vxVector3d::operator+(double factor) const
{
	return std::move(vxVector3d(factor+m_x,factor+m_y,factor+m_z));
}

vxVector3d vxVector3d::operator+(const vxVector3d &entrada) const
{
	return std::move(vxVector3d(entrada.m_x+m_x,
								entrada.m_y+m_y,
								entrada.m_z+m_z));
}

vxVector3d vxVector3d::operator+=(const vxVector3d &entrada)
{
	m_x+=entrada.m_x;
	m_y+=entrada.m_y;
	m_z+=entrada.m_z;
	return std::move(*this);
}

vxVector2d vxVector3d::vectorXY() const
{
	return std::move(vxVector2d(m_x,m_y));
}

vxVector2d vxVector3d::vectorYZ() const
{
	return std::move(vxVector2d(m_x,m_y));
}

vxVector2d vxVector3d::vectorZX() const
{
	return std::move(vxVector2d(m_x,m_y));
}

bool vxVector3d::xSign() const
{
	return std::signbit(m_x);
}

bool vxVector3d::ySign() const
{
	return std::signbit(m_y);
}

bool vxVector3d::zSign() const
{
	return std::signbit(m_z);
}

