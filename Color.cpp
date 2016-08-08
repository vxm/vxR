#include "Color.h"
#include "MathUtils.h"

using namespace vxCore;

//http://hyperphysics.phy-astr.gsu.edu/hbase/vision/cie.html#c4

scalar mxc = 220.0/256.0;
scalar mnc = 10.0/256.0;


Color::Color()
{}

Color::Color(const scalar r,
				 const scalar g,
				 const scalar b,
				 const scalar alpha)
	: m_r(r)
	, m_g(g)
	, m_b(b)
	, m_a(alpha)
{}

Color::Color(const Color &other)
	: m_r(other.m_r)
	, m_g(other.m_g)
	, m_b(other.m_b)
	, m_a(other.m_a)
{}

Color::Color(const v3s &other) 
	: m_r(other.x())
	, m_g(other.y())
	, m_b(other.z())
	, m_a(1.0)
{}

Color::Color(scalar other) 
	: m_r(other)
	, m_g(other)
	, m_b(other)
	, m_a(1.0)
{}


Color &Color::operator=(const v3s &otro)
{
	set(otro.x(), otro.y(), otro.z());
	return *this;
}

Color Color::lookup(const Color &col)
{
	return MU::remap(col, mnc, mxc);
}

Color Color::lookup(const scalar r, 
						const scalar g, 
						const scalar b,
						const scalar a)
{
	return {MU::remap(r, mnc, mxc),
			MU::remap(g, mnc, mxc),
			MU::remap(b, mnc, mxc),
			MU::remap(a, mnc, mxc)};
}

Color Color::lookup256(const int r, const int g, const int b, const int a)
{
	return {MU::remap(r/255.0, mnc, mxc),
			MU::remap(g/255.0, mnc, mxc),
			MU::remap(b/255.0, mnc, mxc),
			MU::remap(a/255.0, mnc, mxc)};
}

Color Color::lookup256(const Color &col)
{
	return std::move(MU::remap(col/(scalar)255.0, mnc, mxc));
}

Color Color::indexColor(const unsigned char idx)
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

void Color::set(scalar r, scalar g, scalar b, scalar a)
{
	m_r=r;
	m_g=g;
	m_b=b;
	m_a=a;
}

void Color::set(scalar r, scalar g, scalar b)
{
	m_r=r;
	m_g=g;
	m_b=b;
	m_a=1.0;
}

void Color::set(const Color &other)
{
	m_r = other.m_r;
	m_g = other.m_g;
	m_b = other.m_b;
	m_a = other.m_a;
}

void Color::reset()
{
	m_r=0;
	m_g=0;
	m_b=0;
	m_a=1.0;
}

Color Color::get() const 
{
	return *this;
}

void Color::get(scalar &ri, scalar &gi, scalar &bi, scalar &ai) const
{
	ri=m_r;
	gi=m_g;
	bi=m_b;
	ai=m_a;
}

void Color::applyCurve(scalar gamma, scalar gain)
{
	m_r = pow(m_r + gain, gamma) - gain;
	m_g = pow(m_g + gain, gamma) - gain;
	m_b = pow(m_b + gain, gamma) - gain;
}

Color Color::gained(scalar gain) const 
{
	return {m_r*gain,
			m_g*gain,
			m_b*gain,
			m_a*gain};
}


scalar Color::lumma() const 
{
	return sqrt( 0.299 * m_r * m_r
				+ 0.587 * m_g * m_g
				+ 0.114 * m_b * m_b);
}

void Color::mix(const Color &other, scalar alpha)
{
	m_r += MU::lerp(m_r, other.m_r, alpha);
	m_g += MU::lerp(m_g, other.m_g, alpha);
	m_b += MU::lerp(m_b, other.m_b, alpha);
}

void Color::mixSumm(const Color &other, scalar alpha)
{
	m_r += other.m_r * alpha;
	m_g += other.m_g * alpha;
	m_b += other.m_b * alpha;
}

void Color::add(const Color &other)
{
	m_r += other.m_r;
	m_g += other.m_g;
	m_b += other.m_b;
	m_a += other.m_a;
}


void Color::blend(const Color &other)
{
	m_r = (m_r + other.m_r) / 2.0;
	m_g = (m_g + other.m_g) / 2.0;
	m_b = (m_b + other.m_b) / 2.0;
	m_a = (m_a + other.m_a) / 2.0;
}

void Color::setToGamma(scalar gamma, scalar offset)
{
	m_r=pow(m_r+offset, gamma)-offset;
	m_g=pow(m_g+offset, gamma)-offset;
	m_b=pow(m_b+offset, gamma)-offset;
	m_a=pow(m_a+offset, gamma)-offset;
}

void Color::gain(scalar gain)
{
	m_r+=gain;
	m_g+=gain;
	m_b+=gain;
	m_a+=gain;
}

Color Color::dimm(scalar factor) const
{
	return (*this)*((scalar)1.0/factor);
}

Color &Color::operator*=(const Color &entrada)
{
	m_r *= entrada.m_r;
	m_g *= entrada.m_g;
	m_b *= entrada.m_b;
	m_a *= entrada.m_a;

	return *this;
}

Color &Color::operator*=(scalar factor)
{
	m_r *= factor;
	m_g *= factor;
	m_b *= factor;
	m_a *= factor;

	return *this;
}

Color &Color::operator+=(const Color &entrada)
{
	m_r += entrada.m_r;
	m_g += entrada.m_g;
	m_b += entrada.m_b;
	m_a += entrada.m_a;

	return *this;
}

Color &Color::operator+=(scalar factor)
{
	m_r += factor;
	m_g += factor;
	m_b += factor;
	m_a += factor;
	
	return *this;
}

Color &Color::operator-=(const Color &entrada)
{
	m_r -= entrada.m_r;
	m_g -= entrada.m_g;
	m_b -= entrada.m_b;
	m_a -= entrada.m_a;

	return *this;
}

Color &Color::operator-=(scalar factor)
{
	m_r -= factor;
	m_g -= factor;
	m_b -= factor;
	m_a -= factor;
	
	return *this;
}

Color Color::operator/(scalar factor) const 
{
	return {m_r/(scalar)factor,
			m_g/(scalar)factor,
			m_b/(scalar)factor,
			m_a/(scalar)factor};
}

Color Color::operator/(const Color &entrada) const 
{
	return {entrada.m_r/m_r,
			entrada.m_g/m_g,
			entrada.m_b/m_b,
			entrada.m_a/m_a};
}

Color Color::operator*(const Color &entrada) const 
{
	return {entrada.m_r*m_r,
			entrada.m_g*m_g,
			entrada.m_b*m_b,
			entrada.m_a*m_a};
}

Color Color::operator*(const scalar factor) const 
{
	return {factor*m_r,
			factor*m_g,
			factor*m_b,
			factor*m_a};
}

Color Color::operator-(scalar factor) const 
{
	return {m_r-factor,
			m_g-factor,
			m_b-factor,
			m_a-factor};
}

Color Color::operator-(const Color &entrada) const 
{
	return {m_r-entrada.m_r,
			m_g-entrada.m_g,
			m_b-entrada.m_b,
			m_a-entrada.m_a};
}

Color Color::operator+(scalar factor) const 
{
	return {factor+m_r,
			factor+m_g,
			factor+m_b,
			factor+m_a};
}

Color Color::operator+(const Color &other) const
{
	return {other.m_r+m_r, 
			other.m_g+m_g, 
			other.m_b+m_b, 
			other.m_a+m_a};
}

void Color::toRGBA8888(unsigned char tbuff[4]) const
{
	tbuff[0] = (unsigned char)char(MU::remap(m_r,255.0));
	tbuff[1] = (unsigned char)char(MU::remap(m_g,255.0));
	tbuff[2] = (unsigned char)char(MU::remap(m_b,255.0));
	tbuff[3] = (unsigned char)char(MU::remap(m_a,255.0));
}

Color Color::zero {(scalar)0.0, (scalar)0.0, (scalar)0.0, (scalar)1.0};

Color Color::blue			(Color::lookup256(92, 138, 202, 255));
Color Color::bluishGreen	(Color::lookup256(24, 162, 121, 255));
Color Color::bluegreen		(Color::lookup256(95, 164, 190, 255));
Color Color::bluishPurple	(Color::lookup256(92, 102, 177, 255));
Color Color::greenishYellow	(Color::lookup256(235, 233, 0, 255));
Color Color::green			(Color::lookup256(0, 163, 71, 255));
Color Color::greenishBlue	(Color::lookup256(110, 175, 199, 255));
Color Color::orangePink		(Color::lookup256(240, 204, 162, 255));
Color Color::orange			(Color::lookup256(228, 184, 29, 255));
Color Color::pink			(Color::lookup256(245, 220, 208, 255));
Color Color::reddishOrange	(Color::lookup256(216, 119, 51, 255));
Color Color::red			(Color::lookup256(191, 27, 75, 255));
Color Color::reddishPurple	(Color::lookup256(196, 64, 143, 255));
Color Color::redPurple		(Color::lookup256(175, 35, 132, 255));
Color Color::purple			(Color::lookup256(246, 85, 158, 255));
Color Color::purplishBlue	(Color::lookup256(88,  121, 191, 255));
Color Color::purplishPink	(Color::lookup256(243, 208, 219, 255));
Color Color::purplishRed	(Color::lookup256(209, 65, 136, 255));
Color Color::white			(Color::lookup256(255, 255, 255, 255));
Color Color::yellowGreen	(Color::lookup256(185, 214, 4, 255));
Color Color::yellowishOrange(Color::lookup256(231, 224, 0, 255));
Color Color::yellow			(Color::lookup256(234, 231, 94, 255));
Color Color::yellowishGreen	(Color::lookup256(170, 209, 60, 255));
Color Color::black			{mnc, mnc, mnc, 1.0};
Color Color::grey			{(mnc+mxc)/2.0, (mnc+mxc)/2.0, (mnc+mxc)/2.0, 1.0};


