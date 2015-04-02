#ifndef VXPIXEL_H
#define VXPIXEL_H

#include <vxVector.h>
namespace vxStorage {

class vxPixel:public vxColor
{
private:
	int m_comp;
	double m_d;

public:

	vxPixel();

	vxPixel(double r, double g, double b, double a)
	: vxColor (r, g, b, a)
	{
		m_comp=0;
	}

	void add(int r, int g, int b)
	{
		this->m_r+=r;
		this->m_g+=g;
		this->m_b+=b;
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
		return vxColor(m_r/(float)m_comp,m_g/(float)m_comp,m_b/(float)m_comp);
	}

	void setResult()
	{
		m_r/=(float)m_comp;
		m_g/=(float)m_comp;
		m_b/=(float)m_comp;
		m_comp=0;
	}

	void add(vxColor color)
	{
		this->add(color.getR(),color.getG(),color.getB());	
		m_comp++;
	}

	void add(vxPixel color)
	{
		this->add(color.getR(),color.getG(),color.getB());	
		m_comp++;
	}

	vxPixel operator=(vxPixel otro)
	{
		vxPixel salida;
		salida.set(otro.getR(),otro.getG(),otro.getB());		
		return salida;
	}

	vxPixel operator=(vxColor otro)
	{
		vxPixel salida;
		salida.set(otro.getR(),otro.getG(),otro.getB());		
		return salida;
	}
};

}
#endif // VXPIXEL_H
