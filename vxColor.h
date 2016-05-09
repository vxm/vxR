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
			 const scalar b, 
			 const scalar alpha);
	
	vxColor (const vxColor& other);
	vxColor (const v3s& other);
	vxColor& operator=(const v3s &otro);
	///
	/// \brief lookup
	/// \param col
	/// \return 
	///
	static vxColor lookup(const vxColor &col);
	///
	/// \brief lookup
	/// \param r
	/// \param g
	/// \param b
	/// \param a
	/// \return 
	///
	static vxColor lookup(const scalar r,
							const scalar g,
							const scalar b,
							const scalar a = 1.0);
	///
	/// \brief lookup256
	/// \param r
	/// \param g
	/// \param b
	/// \return 
	///
	static vxColor lookup256(const int r,
								const int g,
								const int b, 
								const int a);
	///
	/// \brief lookup256
	/// \param col
	/// \return 
	///
	static vxColor lookup256(const vxColor &col);
	///
	/// \brief indexColor
	/// \param idx
	/// \return 
	///
	static vxColor indexColor(const unsigned char idx);
	///
	/// \brief set
	/// \param r
	/// \param g
	/// \param b
	/// \param alpha
	///
	void set(scalar r, scalar g, scalar b, scalar alpha);
	///
	/// \brief set
	/// \param r
	/// \param g
	/// \param b
	///
	void set(scalar r, scalar g, scalar b);
	///
	/// \brief set
	/// \param other
	///
	void set(const vxColor &other);
	///
	/// \brief reset
	///
	void reset();
	///
	/// \brief setR
	/// \param r
	///
	void setR(scalar r) {m_r=r;}
	///
	/// \brief setG
	/// \param g
	///
	void setG(scalar g) {m_g=g;}
	///
	/// \brief setB
	/// \param b
	///
	void setB(scalar b) {m_b=b;}
	///
	/// \brief setA
	/// \param a
	///
	void setA(scalar a) {m_a=a;}
	///
	/// \brief get
	/// \return 
	///
	vxColor get() const;
	///
	/// \brief get
	/// \param ri
	/// \param gi
	/// \param bi
	/// \param ai
	///
	void get(scalar &ri, scalar &gi, scalar &bi, scalar &ai) const;
	///
	/// \brief r
	/// \return 
	///
	scalar r() const {return m_r;}
	///
	/// \brief g
	/// \return 
	///
	scalar g() const {return m_g;}
	///
	/// \brief b
	/// \return 
	///
	scalar b() const {return m_b;}
	///
	/// \brief alpha
	/// \return 
	///
	scalar alpha() const {return m_a;}
	///
	/// \brief applyCurve
	/// \param gamma
	/// \param gain
	///
	void applyCurve(scalar gamma, scalar gain);
	///
	/// \brief lumma
	/// \return 
	///
	scalar lumma() const;
	///
	/// \brief add
	/// \param other
	///
	void add(const vxColor &other);
	///
	/// \brief blend
	/// \param other
	///
	void blend(const vxColor &other);
	///
	/// \brief mix
	/// \param other
	/// \param alpha
	///
	void mix(const vxColor &other, scalar alpha = 0.5);
	///
	/// \brief mixSumm
	/// \param other
	/// \param alpha
	///
	void mixSumm(const vxColor &other, scalar alpha = 0.5);
	///
	/// \brief setToGamma
	/// \param gamma
	/// \param offset
	///
	void setToGamma(scalar gamma=2.2, scalar offset = 1.0);
	///
	/// \brief gain
	/// \param gain
	///
	void gain(scalar gain);
	///
	/// \brief dimm
	/// \param factor
	/// \return 
	///
	vxColor dimm(scalar factor) const;
	///
	/// \brief gained
	/// \param gain
	/// \return 
	///
	vxColor gained(scalar gain) const;
	///
	/// \brief operator +
	/// \param other
	/// \return 
	///
	vxColor operator+(const vxColor &other) const;
	///
	/// \brief operator +
	/// \param factor
	/// \return 
	///
	vxColor operator+(scalar factor) const;
	///
	/// \brief operator -
	/// \param entrada
	/// \return 
	///
	vxColor operator-(const vxColor &entrada) const;
	///
	/// \brief operator -
	/// \param factor
	/// \return 
	///
	vxColor operator-(scalar factor) const;
	///
	/// \brief operator *
	/// \param entrada
	/// \return 
	///
	vxColor operator*(const vxColor &entrada) const;
	///
	/// \brief operator *
	/// \param factor
	/// \return 
	///
	vxColor operator*(scalar factor) const;
	///
	/// \brief operator /
	/// \param entrada
	/// \return 
	///
	vxColor operator/(const vxColor &entrada) const;
	///
	/// \brief operator /
	/// \param factor
	/// \return 
	///
	vxColor operator/(scalar factor) const;
	///
	/// \brief operator *=
	/// \param entrada
	/// \return 
	///
	vxColor& operator*=(const vxColor &entrada);
	///
	/// \brief operator *=
	/// \param factor
	/// \return 
	///
	vxColor& operator*=(scalar factor);
	///
	/// \brief operator +=
	/// \param entrada
	/// \return 
	///
	vxColor& operator+=(const vxColor &entrada);
	///
	/// \brief operator +=
	/// \param factor
	/// \return 
	///
	vxColor& operator+=(scalar factor);
	///
	/// \brief operator -=
	/// \param entrada
	/// \return 
	///
	vxColor& operator-=(const vxColor &entrada);
	///
	/// \brief operator -=
	/// \param factor
	/// \return 
	///
	vxColor& operator-=(scalar factor);
	///
	/// \brief toRGBA8888
	/// \param tbuff
	///
	void toRGBA8888(unsigned char tbuff[]) const;
	///
	/// \brief zero
	///
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
