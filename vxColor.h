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
	scalar m_r = {0.0};
	scalar m_g = {0.0};
	scalar m_b = {0.0};
	scalar m_a = {1.0};

public:

	vxColor ();
	vxColor (scalar other);
	vxColor (const scalar r,
			 const scalar g,
			 const scalar b);

	vxColor (const scalar r, 
			 const scalar g, 
			 const scalar b, 
			 const scalar a);
	
	vxColor (const vxColor& other);
	vxColor (const v3& other);
	vxColor& operator=(const v3 &otro);
	
	static vxColor lookup(const vxColor &col);
	static vxColor lookup(const scalar r,
							const scalar g,
							const scalar b);

	static vxColor lookup256(const int r,
								const int g,
								const int b);
	static vxColor lookup256(const vxColor &col);

	static vxColor indexColor(const unsigned int idx);
	
	void set(scalar r, scalar g, scalar b, scalar a);
	void set(scalar r, scalar g, scalar b);
	void set(const vxColor &other);
	
	void reset();

	void setR(scalar r) {m_r=r;}
	void setG(scalar g) {m_g=g;}
	void setB(scalar b) {m_b=b;}
	void setA(scalar a) {m_a=a;}

	vxColor get() const;
	
	void get(scalar &ri, scalar &gi, scalar &bi, scalar &ai) const;

	scalar r() const {return m_r;}
	scalar g() const {return m_g;}
	scalar b() const {return m_b;}
	scalar a() const {return m_a;}

	void applyCurve(scalar gamma, scalar gain);
	scalar lumma() const;
	void add(const vxColor &other);
	void blend(const vxColor &other);
	void mix(const vxColor &other, scalar alpha = 0.5);
	void mixSumm(const vxColor &other, scalar alpha = 0.5);
	void setToGamma(scalar gamma=2.2, scalar offset = 1.0);
	void gain(scalar gain);
	vxColor dimm(scalar factor) const;
	vxColor gained(scalar gain) const;
	
	vxColor operator+(const vxColor &other) const;
	vxColor operator+(scalar factor) const;
	vxColor operator+(int factor) const;
	vxColor operator+(unsigned int factor) const;

	vxColor operator-(const vxColor &entrada) const;
	vxColor operator-(scalar factor) const;
	vxColor operator-(int factor) const;
	vxColor operator-(unsigned int factor) const;

	vxColor operator*(const vxColor &entrada) const;
	vxColor operator*(scalar factor) const;
	vxColor operator*(int factor) const;
	vxColor operator*(unsigned int factor) const;

	vxColor operator/(const vxColor &entrada) const;
	vxColor operator/(scalar factor) const;
	vxColor operator/(int factor) const;
	vxColor operator/(unsigned int factor) const;

	vxColor& operator*=(const vxColor &entrada);
	vxColor& operator*=(scalar factor);
	vxColor& operator*=(int factor);
	vxColor& operator*=(unsigned int factor);

	vxColor& operator+=(const vxColor &entrada);
	vxColor& operator+=(scalar factor);
	vxColor& operator+=(int factor);
	vxColor& operator+=(unsigned int factor);

	vxColor& operator-=(const vxColor &entrada);
	vxColor& operator-=(scalar factor);
	vxColor& operator-=(int factor);
	vxColor& operator-=(unsigned int factor);
	
	void toRGBA8888(unsigned char *tbuff) const;

	static vxColor zero;
	
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
