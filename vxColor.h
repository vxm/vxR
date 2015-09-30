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

class vxColor
{
protected:
/*tristimulus values*/
	double m_r = {0.0};
	double m_g = {0.0};
	double m_b = {0.0};
	double m_a = {1.0};

public:

	vxColor ();
	vxColor (const double r,
			 const double g,
			 const double b);

	vxColor (const double r, 
			 const double g, 
			 const double b, 
			 const double a);
	
	vxColor (const vxColor& other);
	vxColor (const vxVector3d& other);
	vxColor& operator=(const vxVector3d &otro);
	
	static vxColor lookup(const vxColor &col);
	static vxColor lookup(const double r,
							const double g,
							const double b);

	static vxColor lookup256(const int r,
								const int g,
								const int b);
	static vxColor lookup256(const vxColor &col);

	static vxColor indexColor(const int idx);
	
	void set(double r, double g, double b, double a);
	void set(double r, double g, double b);
	void set(const vxColor &other);
	
	void reset();

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
	void add(const vxColor &other);
	void blend(const vxColor &other);
	void mix(const vxColor &other, double alpha = 0.5);
	void mixSumm(const vxColor &other, double alpha = 0.5);
	void setToGamma(double gamma=2.2, double offset = 1.0);
	void gain(double gain);
	vxColor dimm(double factor) const;
	vxColor gained(double gain) const;
	
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
	
	static vxColor blue;
	static vxColor bluishGreen;
	static vxColor bluegreen;
	static vxColor bluishPurple;
	static vxColor greenishYellow;
	static vxColor green;
	static vxColor greenishBlue;
	static vxColor orangePink;
	static vxColor orange;
	static vxColor pink;
	static vxColor reddishOrange;
	static vxColor red;
	static vxColor reddishPurple;
	static vxColor redPurple;
	static vxColor purple;
	static vxColor purplishBlue;
	static vxColor purplishPink;
	static vxColor purplishRed;
	static vxColor white;
	static vxColor yellowGreen;
	static vxColor yellowishOrange;
	static vxColor yellow;
	static vxColor yellowishGreen;
	static vxColor black;
	static vxColor grey;

	friend std::ostream& operator<<(std::ostream &os, const vxColor &c)
	{
		return os << c.m_r << " " << c.m_g << " " << c.m_b << " " << c.m_a;
	}
	
};


}
#endif // VXCOLOR_H
