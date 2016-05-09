#include "vxColor.h"
#include "MathUtils.h"

using namespace vxCore;

//http://hyperphysics.phy-astr.gsu.edu/hbase/vision/cie.html#c4

scalar mxc = 220.0/256.0;
scalar mnc = 10.0/256.0;


vxColor::vxColor()
{}

vxColor::vxColor(const scalar r,
				 const scalar g,
				 const scalar b,
				 const scalar alpha)
	: m_r(r)
	, m_g(g)
	, m_b(b)
	, m_a(alpha)
{}

vxColor::vxColor(const vxColor &other)
	: m_r(other.m_r)
	, m_g(other.m_g)
	, m_b(other.m_b)
	, m_a(other.m_a)
{}

vxColor::vxColor(const v3s &other) 
	: m_r(other.x())
	, m_g(other.y())
	, m_b(other.z())
	, m_a(1.0)
{}

vxColor::vxColor(scalar other) 
	: m_r(other)
	, m_g(other)
	, m_b(other)
	, m_a(1.0)
{}


vxColor &vxColor::operator=(const v3s &otro)
{
	set(otro.x(), otro.y(), otro.z());
	return *this;
}

vxColor vxColor::lookup(const vxColor &col)
{
	return MU::remap(col, mnc, mxc);
}

vxColor vxColor::lookup(const scalar r, 
						const scalar g, 
						const scalar b,
						const scalar a)
{
	return {MU::remap(r, mnc, mxc),
			MU::remap(g, mnc, mxc),
			MU::remap(b, mnc, mxc),
			MU::remap(a, mnc, mxc)};
}

vxColor vxColor::lookup256(const int r, const int g, const int b, const int a)
{
	return {MU::remap(r/255.0, mnc, mxc),
			MU::remap(g/255.0, mnc, mxc),
			MU::remap(b/255.0, mnc, mxc),
			MU::remap(a/255.0, mnc, mxc)};
}

vxColor vxColor::lookup256(const vxColor &col)
{
	return std::move(MU::remap(col/(scalar)255.0, mnc, mxc));
}

vxColor vxColor::indexColor(const unsigned char idx)
{
	switch(idx%25)
	{
	case 0:
		return white;
	break;
	case 1:
		return red;
	break;
	case 2:
		return green;
	break;
	case 3:
		return blue;
	break;
	case 4:
		return yellow;
	break;
	case 5:
		return pink;
	break;
	case 6:
		return purple;
	break;
	case 7:
		return reddishPurple;
	break;
	case 8:
		return grey;
	break;
	case 9:
		return orange;
	break;
	case 10:
		return redPurple;
	break;
	case 11:
		return bluegreen;
	break;
	case 12:
		return reddishOrange;
	break;
	case 13:
		return bluishPurple;
	break;
	case 14:
		return orangePink;
	break;
	case 15:
		return greenishBlue;
	break;
	case 16:
		return bluishGreen;
	break;
	case 17:
		return greenishYellow;
	break;
	case 18:
		return yellowishGreen;
	break;
	case 19:
		return purplishBlue;
	break;
	case 20:
		return purplishPink;
	break;
	case 21:
		return purplishRed;
	break;
	case 22:
		return yellowGreen;
	break;
	case 23:
		return yellowishOrange;
	break;
	case 24:
		return black;
	break;
	default:
		return red;
	break;
	}
	
	return red;
}

void vxColor::set(scalar r, scalar g, scalar b, scalar a)
{
	m_r=r;
	m_g=g;
	m_b=b;
	m_a=a;
}

void vxColor::set(scalar r, scalar g, scalar b)
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

void vxColor::reset()
{
	m_r=0;
	m_g=0;
	m_b=0;
	m_a=1.0;
}

vxColor vxColor::get() const 
{
	return *this;
}

void vxColor::get(scalar &ri, scalar &gi, scalar &bi, scalar &ai) const
{
	ri=m_r;
	gi=m_g;
	bi=m_b;
	ai=m_a;
}

void vxColor::applyCurve(scalar gamma, scalar gain)
{
	m_r = pow(m_r + gain, gamma) - gain;
	m_g = pow(m_g + gain, gamma) - gain;
	m_b = pow(m_b + gain, gamma) - gain;
}

vxColor vxColor::gained(scalar gain) const 
{
	return {m_r*gain,
			m_g*gain,
			m_b*gain,
			m_a*gain};
}


scalar vxColor::lumma() const 
{
	return sqrt( 0.299 * m_r * m_r
				+ 0.587 * m_g * m_g
				+ 0.114 * m_b * m_b);
}

void vxColor::mix(const vxColor &other, scalar alpha)
{
	m_r += MU::lerp(m_r, other.m_r, alpha);
	m_g += MU::lerp(m_g, other.m_g, alpha);
	m_b += MU::lerp(m_b, other.m_b, alpha);
}

void vxColor::mixSumm(const vxColor &other, scalar alpha)
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
	m_a += other.m_a;
}


void vxColor::blend(const vxColor &other)
{
	m_r = (m_r + other.m_r) / 2.0;
	m_g = (m_g + other.m_g) / 2.0;
	m_b = (m_b + other.m_b) / 2.0;
	m_a = (m_a + other.m_a) / 2.0;
}

void vxColor::setToGamma(scalar gamma, scalar offset)
{
	m_r=pow(m_r+offset, gamma)-offset;
	m_g=pow(m_g+offset, gamma)-offset;
	m_b=pow(m_b+offset, gamma)-offset;
	m_a=pow(m_a+offset, gamma)-offset;
}

void vxColor::gain(scalar gain)
{
	m_r+=gain;
	m_g+=gain;
	m_b+=gain;
	m_a+=gain;
}

vxColor vxColor::dimm(scalar factor) const
{
	return (*this)*((scalar)1.0/factor);
}

vxColor &vxColor::operator*=(const vxColor &entrada)
{
	m_r *= entrada.m_r;
	m_g *= entrada.m_g;
	m_b *= entrada.m_b;
	m_a *= entrada.m_a;

	return *this;
}

vxColor &vxColor::operator*=(scalar factor)
{
	m_r *= factor;
	m_g *= factor;
	m_b *= factor;
	m_a *= factor;

	return *this;
}

vxColor &vxColor::operator+=(const vxColor &entrada)
{
	m_r += entrada.m_r;
	m_g += entrada.m_g;
	m_b += entrada.m_b;
	m_a += entrada.m_a;

	return *this;
}

vxColor &vxColor::operator+=(scalar factor)
{
	m_r += factor;
	m_g += factor;
	m_b += factor;
	m_a += factor;
	
	return *this;
}

vxColor &vxColor::operator-=(const vxColor &entrada)
{
	m_r -= entrada.m_r;
	m_g -= entrada.m_g;
	m_b -= entrada.m_b;
	m_a -= entrada.m_a;

	return *this;
}

vxColor &vxColor::operator-=(scalar factor)
{
	m_r -= factor;
	m_g -= factor;
	m_b -= factor;
	m_a -= factor;
	
	return *this;
}

vxColor vxColor::operator/(scalar factor) const 
{
	return {m_r/(scalar)factor,
			m_g/(scalar)factor,
			m_b/(scalar)factor,
			m_a/(scalar)factor};
}

vxColor vxColor::operator/(const vxColor &entrada) const 
{
	return {entrada.m_r/m_r,
			entrada.m_g/m_g,
			entrada.m_b/m_b,
			entrada.m_a/m_a};
}

vxColor vxColor::operator*(const vxColor &entrada) const 
{
	return {entrada.m_r*m_r,
			entrada.m_g*m_g,
			entrada.m_b*m_b,
			entrada.m_a*m_a};
}

vxColor vxColor::operator*(const scalar factor) const 
{
	return {factor*m_r,
			factor*m_g,
			factor*m_b,
			factor*m_a};
}

vxColor vxColor::operator-(scalar factor) const 
{
	return {m_r-factor,
			m_g-factor,
			m_b-factor,
			m_a-factor};
}

vxColor vxColor::operator-(const vxColor &entrada) const 
{
	return {m_r-entrada.m_r,
			m_g-entrada.m_g,
			m_b-entrada.m_b,
			m_a-entrada.m_a};
}

vxColor vxColor::operator+(scalar factor) const 
{
	return {factor+m_r,
			factor+m_g,
			factor+m_b,
			factor+m_a};
}

vxColor vxColor::operator+(const vxColor &other) const
{
	return {other.m_r+m_r, 
			other.m_g+m_g, 
			other.m_b+m_b, 
			other.m_a+m_a};
}

void vxColor::toRGBA8888(unsigned char tbuff[4]) const
{
	tbuff[0] = (unsigned char)char(MU::remap(m_r,255.0));
	tbuff[1] = (unsigned char)char(MU::remap(m_g,255.0));
	tbuff[2] = (unsigned char)char(MU::remap(m_b,255.0));
	tbuff[3] = (unsigned char)char(MU::remap(m_a,255.0));
}

vxColor vxColor::zero {(scalar)0.0, (scalar)0.0, (scalar)0.0, (scalar)1.0};

vxColor vxColor::blue			(vxColor::lookup256(92, 138, 202, 255));
vxColor vxColor::bluishGreen	(vxColor::lookup256(24, 162, 121, 255));
vxColor vxColor::bluegreen		(vxColor::lookup256(95, 164, 190, 255));
vxColor vxColor::bluishPurple	(vxColor::lookup256(92, 102, 177, 255));
vxColor vxColor::greenishYellow	(vxColor::lookup256(235, 233, 0, 255));
vxColor vxColor::green			(vxColor::lookup256(0, 163, 71, 255));
vxColor vxColor::greenishBlue	(vxColor::lookup256(110, 175, 199, 255));
vxColor vxColor::orangePink		(vxColor::lookup256(240, 204, 162, 255));
vxColor vxColor::orange			(vxColor::lookup256(228, 184, 29, 255));
vxColor vxColor::pink			(vxColor::lookup256(245, 220, 208, 255));
vxColor vxColor::reddishOrange	(vxColor::lookup256(216, 119, 51, 255));
vxColor vxColor::red			(vxColor::lookup256(191, 27, 75, 255));
vxColor vxColor::reddishPurple	(vxColor::lookup256(196, 64, 143, 255));
vxColor vxColor::redPurple		(vxColor::lookup256(175, 35, 132, 255));
vxColor vxColor::purple			(vxColor::lookup256(246, 85, 158, 255));
vxColor vxColor::purplishBlue	(vxColor::lookup256(88,  121, 191, 255));
vxColor vxColor::purplishPink	(vxColor::lookup256(243, 208, 219, 255));
vxColor vxColor::purplishRed	(vxColor::lookup256(209, 65, 136, 255));
vxColor vxColor::white			(vxColor::lookup256(255, 255, 255, 255));
vxColor vxColor::yellowGreen	(vxColor::lookup256(185, 214, 4, 255));
vxColor vxColor::yellowishOrange(vxColor::lookup256(231, 224, 0, 255));
vxColor vxColor::yellow			(vxColor::lookup256(234, 231, 94, 255));
vxColor vxColor::yellowishGreen	(vxColor::lookup256(170, 209, 60, 255));
vxColor vxColor::black			{mnc, mnc, mnc, 1.0};
vxColor vxColor::grey			{(mnc+mxc)/2.0, (mnc+mxc)/2.0, (mnc+mxc)/2.0, 1.0};


