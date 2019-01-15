#include "Color.h"
#include "MathUtils.h"

using namespace vxCore;

// http://hyperphysics.phy-astr.gsu.edu/hbase/vision/cie.html#c4

scalar mxc = 220.0 / 256.0;
scalar mnc = 10.0 / 256.0;

Color::Color(const scalar r, const scalar g, const scalar b, const scalar alpha)
	: m_r(r), m_g(g), m_b(b), m_a(alpha)
{
}

Color::Color(const v3s &other) : m_r(other.x()), m_g(other.y()), m_b(other.z())
{
}

Color::Color(scalar other) : m_r(other), m_g(other), m_b(other) {}

Color &Color::operator=(const v3s &otro)
{
	set(otro.x(), otro.y(), otro.z());
	return *this;
}

Color Color::lookup(const Color &col) { return MU::remap(col, mnc, mxc); }

Color Color::lookup(const scalar r, const scalar g, const scalar b,
					const scalar a)
{
	return {MU::remap(r, mnc, mxc), MU::remap(g, mnc, mxc),
			MU::remap(b, mnc, mxc), MU::remap(a, mnc, mxc)};
}

Color Color::lookup256(const int r, const int g, const int b, const int a)
{
	return {MU::remap(r / scalar(255.0), mnc, mxc),
			MU::remap(g / scalar(255.0), mnc, mxc),
			MU::remap(b / scalar(255.0), mnc, mxc),
			MU::remap(a / scalar(255.0), mnc, mxc)};
}

Color Color::lookup256(const Color &col)
{
	return MU::remap(col / scalar(255.0), mnc, mxc);
}

Color Color::indexColor(const unsigned char idx)
{
	switch (idx % 25)
	{
    case 0:
        return red;
	case 1:
        return green;
	case 2:
        return blue;
	case 3:
        return yellow;
	case 4:
		return yellow;
	case 5:
        return grey;
	case 6:
        return grey;
	case 7:
        return green;
    case 8:
        return red;
	case 9:
        return blue;
	case 10:
        return yellow;
	case 11:
        return red;
	case 12:
        return grey;
	case 13:
        return red;
	case 14:
		return orangePink;
	case 15:
        return white;
	case 16:
        return red;
	case 17:
		return greenishYellow;
	case 18:
        return green;
	case 19:
        return green;
	case 20:
        return pink;
	case 21:
        return yellow;
	case 22:
        return red;
	case 23:
        return blue;
	case 24:
        return red;
	default:
		return red;
	}

	return red;
}

void Color::set(scalar r, scalar g, scalar b, scalar a)
{
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
}

void Color::set(scalar r, scalar g, scalar b)
{
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = 1.0;
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
	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_a = 1.0;
}

Color Color::get() const { return *this; }

void Color::get(scalar &ri, scalar &gi, scalar &bi, scalar &ai) const
{
	ri = m_r;
	gi = m_g;
	bi = m_b;
	ai = m_a;
}

void Color::applyCurve(scalar gamma, scalar gain)
{
	m_r = pow(m_r + gain, gamma) - gain;
	m_g = pow(m_g + gain, gamma) - gain;
	m_b = pow(m_b + gain, gamma) - gain;
}

Color Color::gained(scalar gain) const
{
	return {m_r * gain, m_g * gain, m_b * gain, m_a * gain};
}

scalar Color::lumma() const
{
	return sqrt(0.299 * m_r * m_r + 0.587 * m_g * m_g + 0.114 * m_b * m_b);
}

void Color::mix(const Color &other, scalar alpha)
{
	m_r = MU::lerp(m_r, other.m_r, alpha);
	m_g = MU::lerp(m_g, other.m_g, alpha);
	m_b = MU::lerp(m_b, other.m_b, alpha);
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
	m_a = std::min(scalar(1.0), m_a);
}

void Color::blend(const Color &other)
{
	m_r = (m_r + other.m_r) / scalar(2.0);
	m_g = (m_g + other.m_g) / scalar(2.0);
	m_b = (m_b + other.m_b) / scalar(2.0);
	m_a = (m_a + other.m_a) / scalar(2.0);
}

void Color::setToGamma(scalar gamma, scalar offset)
{
	m_r = pow(m_r + offset, gamma) - offset;
	m_g = pow(m_g + offset, gamma) - offset;
	m_b = pow(m_b + offset, gamma) - offset;
	m_a = pow(m_a + offset, gamma) - offset;
}

void Color::gain(scalar gain)
{
	m_r += gain;
	m_g += gain;
	m_b += gain;
	m_a += gain;
}

Color Color::dimm(scalar factor) const
{
	return (*this) * ((scalar)1.0 / factor);
}

Color &Color::operator*=(const Color &inputColor)
{
	m_r *= inputColor.m_r;
	m_g *= inputColor.m_g;
	m_b *= inputColor.m_b;
	m_a *= inputColor.m_a;

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

Color &Color::operator+=(const Color &inputColor)
{
	m_r += inputColor.m_r;
	m_g += inputColor.m_g;
	m_b += inputColor.m_b;
	m_a += inputColor.m_a;

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

Color &Color::operator-=(const Color &inputColor)
{
	m_r -= inputColor.m_r;
	m_g -= inputColor.m_g;
	m_b -= inputColor.m_b;
	m_a -= inputColor.m_a;

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
	return {m_r / (scalar)factor, m_g / (scalar)factor, m_b / (scalar)factor,
			m_a / (scalar)factor};
}

Color Color::operator/(const Color &inputColor) const
{
	return {inputColor.m_r / m_r, inputColor.m_g / m_g, inputColor.m_b / m_b,
			inputColor.m_a / m_a};
}

Color Color::operator*(const Color &inputColor) const
{
	return {inputColor.m_r * m_r, inputColor.m_g * m_g, inputColor.m_b * m_b,
			inputColor.m_a * m_a};
}

Color Color::operator*(const scalar factor) const
{
	return {factor * m_r, factor * m_g, factor * m_b, factor * m_a};
}

Color Color::operator-(scalar factor) const
{
	return {m_r - factor, m_g - factor, m_b - factor, m_a - factor};
}

Color Color::operator-(const Color &inputColor) const
{
	return {m_r - inputColor.m_r, m_g - inputColor.m_g, m_b - inputColor.m_b,
			m_a - inputColor.m_a};
}

Color Color::operator+(scalar factor) const
{
	return {factor + m_r, factor + m_g, factor + m_b, factor + m_a};
}

Color Color::operator+(const Color &other) const
{
	return {fabs(other.m_r + m_r), fabs(other.m_g + m_g), other.m_b + m_b,
			other.m_a + m_a};
}

void Color::toRGBA8888(std::array<unsigned char, 4> &tbuff) const
{
	tbuff[0] = (unsigned char)char(MU::remap(m_r, scalar(255.0)));
	tbuff[1] = (unsigned char)char(MU::remap(m_g, scalar(255.0)));
	tbuff[2] = (unsigned char)char(MU::remap(m_b, scalar(255.0)));
	tbuff[3] = (unsigned char)char(MU::remap(m_a, scalar(255.0)));
}

Color Color::zero{(scalar)0.0, (scalar)0.0, (scalar)0.0, (scalar)1.0};

Color Color::blue(Color::lookup256(22, 34, 232, 255));
Color Color::bluishGreen(Color::lookup256(124, 222, 121, 255));
Color Color::bluegreen(Color::lookup256(195, 164, 190, 255));
Color Color::bluishPurple(Color::lookup256(92, 202, 177, 255));
Color Color::greenishYellow(Color::lookup256(235, 233, 0, 255));
Color Color::green(Color::lookup256(33, 223, 38, 255));
Color Color::greenishBlue(Color::lookup256(110, 275, 199, 255));
Color Color::orangePink(Color::lookup256(240, 204, 162, 255));
Color Color::orange(Color::lookup256(228, 284, 129, 255));
Color Color::pink(Color::lookup256(245, 33, 108, 255));
Color Color::reddishOrange(Color::lookup256(216, 119, 51, 255));
Color Color::red(Color::lookup256(232, 34, 22, 255));
Color Color::reddishPurple(Color::lookup256(233, 64, 143, 255));
Color Color::redPurple(Color::lookup256(244, 35, 132, 255));
Color Color::purple(Color::lookup256(246, 23, 44, 255));
Color Color::purplishBlue(Color::lookup256(88, 121, 191, 255));
Color Color::purplishPink(Color::lookup256(243, 208, 219, 255));
Color Color::purplishRed(Color::lookup256(209, 165, 136, 255));
Color Color::white(Color::lookup256(222, 222, 222, 255));
Color Color::yellowGreen(Color::lookup256(215, 234, 114, 255));
Color Color::yellowishOrange(Color::lookup256(231, 224, 0, 255));
Color Color::yellow(Color::lookup256(222, 222, 44, 255));
Color Color::yellowishGreen(Color::lookup256(232, 231, 130, 255));
Color Color::black{mnc, mnc, mnc, 1.0};
Color Color::grey(Color::lookup256(33, 33, 33, 255));
