#ifndef VXCOLOR_H
#define VXCOLOR_H
#include <iostream>

#include "vxObject.h"
#include "vxVector.h"

namespace vxCore {
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
	
	
	vxColor ();
	vxColor (double r, double g, double b);
	vxColor (double r, double g, double b, double a);
	vxColor (const vxColor& other);
	vxColor &operator=(const vxVector3d &otro);
	
	void set(double r, double g, double b, double a);
	
	void set(double r, double g, double b)
	{m_r=r;m_g=g;m_b=b;m_a=1.0;}
	
	void set(const vxColor &other)
	{
		m_r = other.m_r;
		m_g = other.m_g;
		m_b = other.m_b;
		m_a = other.m_a;
	}
	
	void reset() {
		m_r=0;
		m_g=0;
		m_b=0;
		m_a=1.0;
	}

	void setR(double r) {m_r=r;}
	void setG(double g) {m_g=g;}
	void setB(double b) {m_b=b;}
	void setA(double a) {m_a=a;}

	vxColor get() const;
	
	void get(double &ri, double &gi, double &bi, double &ai) const;

	double r() const {return m_r;}
	double g() const {return m_g;}
	double b() const {return m_b;}
	double a() const {return m_a;}

	double lumma() const;
	void setToGamma(double gamma=2.2);
	
	vxColor operator+(const vxColor &other) const;
	vxColor operator+(double factor) const;
	vxColor operator+(float factor) const;
	vxColor operator+(int factor) const;
	vxColor operator+(unsigned int factor) const;

	vxColor operator-(const vxColor &entrada) const;
	vxColor operator-(double factor) const;
	vxColor operator-(float factor) const;
	vxColor operator-(int factor) const;
	vxColor operator-(unsigned int factor) const;

	vxColor operator*(const vxColor &entrada) const;
	vxColor operator*(double factor) const;
	vxColor operator*(float factor) const;
	vxColor operator*(int factor) const;
	vxColor operator*(unsigned int factor) const;

	vxColor operator/(const vxColor &entrada) const;
	vxColor operator/(double factor) const;
	vxColor operator/(float factor) const;
	vxColor operator/(int factor) const;
	vxColor operator/(unsigned int factor) const;
	
	void toRGBA8888(unsigned char *tbuff) const;
	
	friend std::ostream& operator<<(std::ostream &os, const vxColor &c)
	{
		return os << c.m_r << " " << c.m_g << " " << c.m_b << " " << c.m_a;
	}
	
};


}
#endif // VXCOLOR_H
