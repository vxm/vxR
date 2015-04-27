#ifndef VXCOLOR_H
#define VXCOLOR_H
#include<vxObject.h>
#include<vxVector.h>
#include <iostream>

namespace vxStorage {
/*
 Munsell, hue, value, chroma
 Ostwald, dominant wavelength, purity, luminance
 CIE, Y to measure brightness, x, hue and y saturation.
 */

class vxColor:public vxObject
{
protected:
/*tristimulus values*/
	double m_r = {0.0};
	double m_g = {0.0};
	double m_b = {0.0};
	double m_a = {1.0};
	
public:

	static vxColor red;
	static vxColor green;
	static vxColor blue;
	static vxColor white;
	static vxColor black;
	
	
	vxColor (){};
	vxColor (double r, double g, double b, double a) 
		: m_r(r)
		, m_g(g)
		, m_b(b)
		, m_a(a)
	{}
	
	vxColor (double r, double g, double b) 
		: m_r(r)
		, m_g(g)
		, m_b(b)
	{}

	vxColor (const vxColor& other) 
		: m_r(other.m_r)
		, m_g(other.m_g)
		, m_b(other.m_b)
	{}
	
	vxColor& operator=(const vxVector3d &otro)
	{
		set(otro.x(), otro.y(), otro.z());
		return *this;
	}
	
	void set(double r, double g, double b, double a)
	{m_r=r;m_g=g;m_b=b;m_a=a;}
	
	void set(double r, double g, double b)
	{m_r=r;m_g=g;m_b=b;m_a=1.0;}
	
	void set(const vxColor &other)
	{
		m_r = other.m_r;
		m_g = other.m_g;
		m_b = other.m_b;
		m_a = other.m_a;
	}
	
	void reset() {m_r=0;m_g=0;m_b=0;m_a=1.0;}

	void setR(int r) {m_r=r;}
	void setG(int g) {m_g=g;}
	void setB(int b) {m_b=b;}
	void setA(int a) {m_a=a;}

	vxColor get() const 
		{return *this;}
	
	void get(double &ri, double &gi, double &bi, double &ai) const
		{ri=m_r;gi=m_g;bi=m_b;ai=m_a;}

	int getR() const {return m_r;}
	int getG() const {return m_g;}
	int getB() const {return m_b;}
	int getA() const {return m_a;}

	double lumma() const {return sqrt((double)m_r*m_r+m_g*m_g+m_b*m_b);}

	vxColor operator+(const vxColor &other) const {return vxColor(other.getR()+m_r,other.getG()+m_g,other.getB()+m_b,other.getA()+m_a);}
	vxColor operator+(double factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}
	vxColor operator+(float factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}
	vxColor operator+(int factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_b);}
	vxColor operator+(unsigned int factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_b);}

	vxColor operator-(const vxColor &entrada) const {return vxColor(m_r-entrada.getR(),m_g-entrada.getG(),m_b-entrada.getB(),m_a-entrada.getA());}
	vxColor operator-(double factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}
	vxColor operator-(float factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}
	vxColor operator-(int factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}
	vxColor operator-(unsigned int factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

	vxColor operator*(const vxColor &entrada) const {return vxColor(entrada.getR()*m_r,entrada.getG()*m_g,entrada.getB()*m_b,entrada.getA()*m_a);}
	vxColor operator*(double factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}
	vxColor operator*(float factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}
	vxColor operator*(int factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}
	vxColor operator*(unsigned int factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

	vxColor operator/(const vxColor &entrada) const {return vxColor(entrada.getR()/(double)m_r,entrada.getG()/(double)m_g,entrada.getB()/(double)m_b,entrada.getA()/(double)m_a);}
	vxColor operator/(double factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}
	vxColor operator/(float factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}
	vxColor operator/(int factor) const {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}
	vxColor operator/(unsigned int factor) const {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

	
	void toRGBA8888(unsigned char *tbuff) const
	{
		*tbuff = (unsigned char)char(m_r);
		tbuff++;
		
		*tbuff = (unsigned char)char(m_g);
		tbuff++;

		*tbuff = (unsigned char)char(m_b);
	}
	
	friend std::ostream& operator<<(std::ostream &os, const vxColor& c)
	{
		return os << c.m_r << " " << c.m_g << " " << c.m_b << " " << c.m_a;
	}
	
};


}
#endif // VXCOLOR_H
