#include "Pixel.h"
using namespace vxCore;

Pixel::Pixel() : Color() {}

Pixel::Pixel(const scalar r, const scalar g, const scalar b, const scalar a)
	: Color(r, g, b, a)
{
}

void Pixel::add(const scalar r, const scalar g, const scalar b, const scalar a)
{
	m_r += r;
	m_g += g;
	m_b += b;
	m_a += a;
	m_comp++;
}

void Pixel::reset()
{
	m_comp = 0;
	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_a = 1;
}

Color Pixel::getResult() const
{
	return {m_r / (scalar)m_comp, m_g / (scalar)m_comp, m_b / (scalar)m_comp,
			m_a / (scalar)m_comp};
}

void Pixel::setResult()
{
	m_r /= (scalar)m_comp;
	m_g /= (scalar)m_comp;
	m_b /= (scalar)m_comp;
	m_a /= (scalar)m_comp;
	m_comp = 0u;
}

void Pixel::add(const Color &color)
{
	add(color.r(), color.g(), color.b(), color.alpha());
	m_comp++;
}

void Pixel::add(const Pixel &color)
{
	add(color.r(), color.g(), color.b(), color.alpha());
	m_comp++;
}

Pixel Pixel::operator=(const Color &other)
{
	return {other.r(), other.g(), other.b(), other.alpha()};
}

Pixel Pixel::operator=(const Pixel &other)
{
	return {other.r(), other.g(), other.b(), other.alpha()};
}
