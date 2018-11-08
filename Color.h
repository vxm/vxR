#ifndef VXCOLOR_H
#define VXCOLOR_H
#include <iostream>

#include "Object.h"
#include "Vector.h"

namespace vxCore {
/*
 Munsell, hue, value, chroma
 Ostwald, dominant wavelength, purity, luminance
 CIE, Y to measure brightness, x, hue and y saturation.
 */

class Color
{
protected:
/*tristimulus values*/
	scalar m_r = {0.0};
	scalar m_g = {0.0};
	scalar m_b = {0.0};
	scalar m_a = {1.0};

public:

	Color () = default;
	Color (scalar other);
	Color (const scalar r, 
			 const scalar g, 
			 const scalar b, 
			 const scalar alpha);
	
	Color (const Color& other);
	Color (const v3s& other);
	Color& operator=(const v3s &otro);
	///
	/// \brief lookup
	/// \param col
	/// \return 
	///
	static Color lookup(const Color &col);
	///
	/// \brief lookup
	/// \param r
	/// \param g
	/// \param b
	/// \param a
	/// \return 
	///
	static Color lookup(const scalar r,
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
	static Color lookup256(const int r,
								const int g,
								const int b, 
								const int a);
	///
	/// \brief lookup256
	/// \param col
	/// \return 
	///
	static Color lookup256(const Color &col);
	///
	/// \brief indexColor
	/// \param idx
	/// \return 
	///
	static Color indexColor(const unsigned char idx);
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
	void set(const Color &other);
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
	Color get() const;
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
	void add(const Color &other);
	///
	/// \brief blend
	/// \param other
	///
	void blend(const Color &other);
	///
	/// \brief mix
	/// \param other
	/// \param alpha
	///
	void mix(const Color &other, scalar alpha = 0.5);
	///
	/// \brief mixSumm
	/// \param other
	/// \param alpha
	///
	void mixSumm(const Color &other, scalar alpha = 0.5);
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
	Color dimm(scalar factor) const;
	///
	/// \brief gained
	/// \param gain
	/// \return 
	///
	Color gained(scalar gain) const;
	///
	/// \brief operator +
	/// \param other
	/// \return 
	///
	Color operator+(const Color &other) const;
	///
	/// \brief operator +
	/// \param factor
	/// \return 
	///
	Color operator+(scalar factor) const;
	///
	/// \brief operator -
	/// \param entrada
	/// \return 
	///
	Color operator-(const Color &entrada) const;
	///
	/// \brief operator -
	/// \param factor
	/// \return 
	///
	Color operator-(scalar factor) const;
	///
	/// \brief operator *
	/// \param entrada
	/// \return 
	///
	Color operator*(const Color &entrada) const;
	///
	/// \brief operator *
	/// \param factor
	/// \return 
	///
	Color operator*(scalar factor) const;
	///
	/// \brief operator /
	/// \param entrada
	/// \return 
	///
	Color operator/(const Color &entrada) const;
	///
	/// \brief operator /
	/// \param factor
	/// \return 
	///
	Color operator/(scalar factor) const;
	///
	/// \brief operator *=
	/// \param entrada
	/// \return 
	///
	Color& operator*=(const Color &entrada);
	///
	/// \brief operator *=
	/// \param factor
	/// \return 
	///
	Color& operator*=(scalar factor);
	///
	/// \brief operator +=
	/// \param entrada
	/// \return 
	///
	Color& operator+=(const Color &entrada);
	///
	/// \brief operator +=
	/// \param factor
	/// \return 
	///
	Color& operator+=(scalar factor);
	///
	/// \brief operator -=
	/// \param entrada
	/// \return 
	///
	Color& operator-=(const Color &entrada);
	///
	/// \brief operator -=
	/// \param factor
	/// \return 
	///
	Color& operator-=(scalar factor);
	///
	/// \brief toRGBA8888
	/// \param tbuff
	///
	void toRGBA8888(unsigned char tbuff[]) const;
	///
	/// \brief zero
	///
	static Color zero;
	static Color blue;
	static Color bluishGreen;
	static Color bluegreen;
	static Color bluishPurple;
	static Color greenishYellow;
	static Color green;
	static Color greenishBlue;
	static Color orangePink;
	static Color orange;
	static Color pink;
	static Color reddishOrange;
	static Color red;
	static Color reddishPurple;
	static Color redPurple;
	static Color purple;
	static Color purplishBlue;
	static Color purplishPink;
	static Color purplishRed;
	static Color white;
	static Color yellowGreen;
	static Color yellowishOrange;
	static Color yellow;
	static Color yellowishGreen;
	static Color black;
	static Color grey;

	friend std::ostream& operator<<(std::ostream &os, const Color &c)
	{
		return os << c.m_r << " " << c.m_g << " " << c.m_b << " " << c.m_a;
	}
	
};


}
#endif // VXCOLOR_H
