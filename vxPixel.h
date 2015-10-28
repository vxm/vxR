#ifndef VXPIXEL_H
#define VXPIXEL_H

#include "vxColor.h"


namespace vxCore {

class vxPixel:public vxColor
{
private:
	int m_comp;
	scalar m_d;

public:

	vxPixel();

	vxPixel(scalar r, scalar g, scalar b, scalar a)
	: vxColor (r, g, b, a)
	{
		m_comp=0;
	}

	void add(scalar r, scalar g, scalar b)
	{
		m_r+=r;
		m_g+=g;
		m_b+=b;
		m_comp++;
	}

	void reset()
	{
		m_comp=0;
		m_r=0;
		m_g=0;
		m_b=0;
	}

	vxColor getResult()
	{
		return vxColor(m_r/(scalar)m_comp,m_g/(scalar)m_comp,m_b/(scalar)m_comp);
	}

	void setResult()
	{
		m_r/=(scalar)m_comp;
		m_g/=(scalar)m_comp;
		m_b/=(scalar)m_comp;
		m_comp=0;
	}

	void add(vxColor color)
	{
		add(color.r(),color.g(),color.b());	
		m_comp++;
	}

	void add(vxPixel color)
	{
		add(color.r(),color.g(),color.b());	
		m_comp++;
	}

	vxPixel operator=(vxPixel otro)
	{
		vxPixel salida;
		salida.set(otro.r(),otro.g(),otro.b());		
		return salida;
	}

	vxPixel operator=(vxColor otro)
	{
		vxPixel salida;
		salida.set(otro.r(),otro.g(),otro.b());		
		return salida;
	}
};

}
#endif // VXPIXEL_H

