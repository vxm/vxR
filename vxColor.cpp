#include "vxColor.h"
#include "MathUtils.h"

using namespace vxCore;

//http://hyperphysics.phy-astr.gsu.edu/hbase/vision/cie.html#c4

double mxc = 220.0/256.0;
double mnc = 18.0/256.0;

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

vxColor vxColor::lookup256(const int r, const int g, const int b)
{
	return vxColor( MathUtils::remap(r/255.0, mnc, mxc),
					MathUtils::remap(g/255.0, mnc, mxc),
					MathUtils::remap(b/255.0, mnc, mxc));
}

vxColor vxColor::indexColor(const int idx)
{
	switch(idx%25)
	{
	case 0:
		return red;
	break;
	case 1:
		return green;
	break;
	case 2:
		return blue;
	break;
	case 3:
		return white;
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
		return black;
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
		return yellowishGreen;
	break;
	default:
		return red;
	break;
	}
	
	return red;
}

vxColor vxColor::blue			(vxColor::lookup256(92, 138, 202));
vxColor vxColor::bluishGreen	(vxColor::lookup256(24, 162, 121));
vxColor vxColor::bluegreen		(vxColor::lookup256(95, 164, 190));
vxColor vxColor::bluishPurple	(vxColor::lookup256(92, 102, 177));
vxColor vxColor::greenishYellow	(vxColor::lookup256(235, 233, 0));
vxColor vxColor::green			(vxColor::lookup256(0, 163, 71));
vxColor vxColor::greenishBlue	(vxColor::lookup256(110, 175, 199));
vxColor vxColor::orangePink		(vxColor::lookup256(240, 204, 162));
vxColor vxColor::orange			(vxColor::lookup256(228, 184, 29));
vxColor vxColor::pink			(vxColor::lookup256(245, 220, 208));
vxColor vxColor::reddishOrange	(vxColor::lookup256(216, 119, 51));
vxColor vxColor::red			(vxColor::lookup256(191, 27, 75));
vxColor vxColor::reddishPurple	(vxColor::lookup256(196, 64, 143));
vxColor vxColor::redPurple		(vxColor::lookup256(175, 35, 132));
vxColor vxColor::purple			(vxColor::lookup256(246, 85, 158));
vxColor vxColor::purplishBlue	(vxColor::lookup256(88,  121, 191));
vxColor vxColor::purplishPink	(vxColor::lookup256(243, 208, 219));
vxColor vxColor::purplishRed	(vxColor::lookup256(209, 65, 136));
vxColor vxColor::white			(vxColor::lookup256(255, 255, 255));
vxColor vxColor::yellowGreen	(vxColor::lookup256(185, 214, 4));
vxColor vxColor::yellowishOrange(vxColor::lookup256(231, 224, 0));
vxColor vxColor::yellow			(vxColor::lookup256(234, 231, 94));
vxColor vxColor::yellowishGreen	(vxColor::lookup256(170, 209, 60));
vxColor vxColor::black			{mnc, mnc, mnc};
vxColor vxColor::grey			{(mnc+mxc)/2.0, (mnc+mxc)/2.0, (mnc+mxc)/2.0};



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
{
	m_r=r;
	m_g=g;
	m_b=b;
	m_a=a;
}

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
	m_r=pow(m_r+1.0, gamma)-1.0;
	m_g=pow(m_g+1.0, gamma)-1.0;
	m_b=pow(m_b+1.0, gamma)-1.0;
}

void vxColor::gain(double gain)
{
	m_r+=gain;
	m_g+=gain;
	m_b+=gain;
}

vxColor vxColor::operator+(unsigned int factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_b);}

vxColor vxColor::operator-(unsigned int factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

vxColor vxColor::operator*(unsigned int factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

vxColor vxColor::operator/(unsigned int factor) const {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(int factor) const {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(double factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(float factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

vxColor vxColor::operator/(const vxColor &entrada) const 
{
	return vxColor(entrada.m_r/m_r,
				   entrada.m_g/m_g,
				   entrada.m_b/m_b,
				   entrada.m_a/m_a);
}

vxColor vxColor::operator*(const vxColor &entrada) const 
{
	return vxColor(entrada.m_r*m_r,
					entrada.m_g*m_g,
					entrada.m_b*m_b,
					entrada.m_a*m_a);
}
vxColor vxColor::operator*(const int factor) const 
{
	return vxColor(factor*m_r,
					factor*m_g,
					factor*m_b,
					factor*m_a);
}

vxColor vxColor::operator*(const float factor) const 
{
	return vxColor(factor*m_r,
					factor*m_g,
					factor*m_b,
					factor*m_a);
}

vxColor vxColor::operator*(double factor) const 
{
	return vxColor(factor*m_r,
					factor*m_g,
					factor*m_b,
					factor*m_a);
}


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
	*tbuff += (unsigned char)char(MathUtils::remap(m_r,255.0));
	tbuff++;
	
	*tbuff += (unsigned char)char(MathUtils::remap(m_g,255.0));
	tbuff++;
	
	*tbuff += (unsigned char)char(MathUtils::remap(m_b,255.0));
}


