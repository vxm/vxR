#include "vxColor.h"
#include "MathUtils.h"

using namespace vxCore;

double mxc = 220.0/256.0;
double mnc = 38.0/256.0;

vxColor vxColor::red	=	{mxc, mnc, mnc};
vxColor vxColor::green	=	{mnc, mxc, mnc};
vxColor vxColor::blue	=	{mnc, mnc, mxc};
vxColor vxColor::white	=	{mxc, mxc, mxc};
vxColor vxColor::black	=	{mnc, mnc, mnc};

vxColor vxColor::lookup(const vxColor col)
{
	return MathUtils::remap(col, mnc, mxc);
}

vxColor vxColor::lookup(const double r, const double g, const double b)
{
	return vxColor( MathUtils::remap(r, mnc, mxc),
					MathUtils::remap(g, mnc, mxc),
					MathUtils::remap(b, mnc, mxc));
}



vxColor::vxColor(double r, double g, double b, double a) 
	: m_r(r)
	, m_g(g)
	, m_b(b)
	, m_a(a)
{}

vxColor::vxColor()
{}

vxColor::vxColor(double r, double g, double b) 
	: m_r(r)
	, m_g(g)
	, m_b(b)
{}

vxColor::vxColor(const vxColor &other) 
	: m_r(other.m_r)
	, m_g(other.m_g)
	, m_b(other.m_b)
{}

vxColor &vxColor::operator=(const vxVector3d &otro)
{
	set(otro.x(), otro.y(), otro.z());
	return *this;
}

void vxColor::set(double r, double g, double b, double a)
{m_r=r;m_g=g;m_b=b;m_a=a;}

void vxColor::set(double r, double g, double b)
{
	m_r=r;
	m_g=g;
	m_b=b;
	m_a=1.0;
}

void vxColor::set(const vxColor &other)
{
	m_r = other.m_r;
	m_g = other.m_g;
	m_b = other.m_b;
	m_a = other.m_a;
}

void vxColor::reset() {
	m_r=0;
	m_g=0;
	m_b=0;
	m_a=1.0;
}

vxColor vxColor::get() const 
{
	return *this;
}

void vxColor::get(double &ri, double &gi, double &bi, double &ai) const
{
	ri=m_r;
	gi=m_g;
	bi=m_b;
	ai=m_a;
}


vxColor vxColor::gained(double gain) const 
{
	return vxColor(m_r*gain,m_g*gain,m_b*gain);
}


double vxColor::lumma() const 
{
	return sqrt((double)m_r*m_r+m_g*m_g+m_b*m_b);
}

void vxColor::mix(const vxColor &other, double alpha)
{
	m_r += MathUtils::lerp(m_r, other.m_r, alpha);
	m_g += MathUtils::lerp(m_g, other.m_g, alpha);
	m_b += MathUtils::lerp(m_b, other.m_b, alpha);
}

void vxColor::mixSumm(const vxColor &other, double alpha)
{
	m_r += other.m_r * alpha;
	m_g += other.m_g * alpha;
	m_b += other.m_b * alpha;
}

void vxColor::add(const vxColor &other)
{
	m_r += other.m_r;
	m_g += other.m_g;
	m_b += other.m_b;
}


void vxColor::blend(const vxColor &other)
{
	m_r = (m_r + other.m_r) / 2.0;
	m_g = (m_g + other.m_g) / 2.0;
	m_b = (m_b + other.m_b) / 2.0;
}

void vxColor::setToGamma(double gamma)
{
	m_r=pow(m_r, gamma);
	m_g=pow(m_g, gamma);
	m_b=pow(m_b, gamma);
}

vxColor vxColor::operator+(unsigned int factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_b);}

vxColor vxColor::operator-(unsigned int factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

vxColor vxColor::operator*(unsigned int factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

vxColor vxColor::operator/(unsigned int factor) const {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(int factor) const {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(double factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(float factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(const vxColor &entrada) const {return vxColor(entrada.m_r/(double)m_r,entrada.m_g/(double)m_g,entrada.m_b/(double)m_b,entrada.m_a/(double)m_a);}

vxColor vxColor::operator*(int factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

vxColor vxColor::operator*(float factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

vxColor vxColor::operator*(double factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

vxColor vxColor::operator*(const vxColor &entrada) const {return vxColor(entrada.m_r*m_r,entrada.m_g*m_g,entrada.m_b*m_b,entrada.m_a*m_a);}

vxColor vxColor::operator-(int factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

vxColor vxColor::operator-(float factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

vxColor vxColor::operator-(double factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

vxColor vxColor::operator-(const vxColor &entrada) const {return vxColor(m_r-entrada.m_r,m_g-entrada.m_g,m_b-entrada.m_b,m_a-entrada.m_a);}

vxColor vxColor::operator+(int factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_b);}

vxColor vxColor::operator+(float factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}

vxColor vxColor::operator+(double factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}

vxColor vxColor::operator+(const vxColor &other) const
{
	return vxColor(other.m_r+m_r, other.m_g+m_g, other.m_b+m_b, other.m_a+m_a);
}

void vxColor::toRGBA8888(unsigned char *tbuff) const
{
	*tbuff = (unsigned char)char(MathUtils::remap(m_r,255.0));
	tbuff++;
	
	*tbuff = (unsigned char)char(MathUtils::remap(m_g,255.0));
	tbuff++;
	
	*tbuff = (unsigned char)char(MathUtils::remap(m_b,255.0));
}


