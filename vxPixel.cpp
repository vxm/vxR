#include "vxPixel.h"
using namespace vxCore;

vxPixel::vxPixel()
	: vxColor()
{
}

vxPixel::vxPixel(const scalar r, const scalar g, const scalar b, const scalar a)
	: vxColor (r, g, b, a)
{
}

void vxPixel::add(const scalar r, const scalar g, const scalar b, const scalar a)
{
	m_r+=r;
	m_g+=g;
	m_b+=b;
	m_a+=a;
	m_comp++;
}

void vxPixel::reset()
{
	m_comp=0;
	m_r=0;
	m_g=0;
	m_b=0;
	m_a=1;
}

vxColor vxPixel::getResult() const
{
	return {m_r/(scalar)m_comp,
			m_g/(scalar)m_comp,
			m_b/(scalar)m_comp,
			m_a/(scalar)m_comp};
}

void vxPixel::setResult()
{
	m_r/=(scalar)m_comp;
	m_g/=(scalar)m_comp;
	m_b/=(scalar)m_comp;
	m_a/=(scalar)m_comp;
	m_comp=0u;
}

void vxPixel::add(const vxColor &color)
{
	add(color.r(),color.g(),color.b(),color.alpha());
	m_comp++;
}

void vxPixel::add(const vxPixel &color)
{
	add(color.r(),color.g(),color.b(),color.alpha());
	m_comp++;
}

vxPixel vxPixel::operator=(const vxColor &other)
{
	return {other.r(),other.g(),other.b(),other.alpha()};
}

vxPixel vxPixel::operator=(const vxPixel &other)
{
	return {other.r(),other.g(),other.b(),other.alpha()};
}

