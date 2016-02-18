
#ifndef _VXVECTORMC_
#define _VXVECTORMC_

#include <math.h>
#include <stdio.h>
#include <iostream>

namespace vxCore {

//TODO: add literal for scalar 0.0r
using scalar = double;

class v2s
{
private:
	scalar m_x {0.0};
	scalar m_y {0.0};

public:

	v2s ();
	v2s (scalar x, scalar y);
	v2s (const v2s& other);
	v2s (v2s&& other);
	v2s& operator=(const v2s& other);
	
	void set(scalar x, scalar y);
	void set(const v2s &enter);
	void setX(const scalar x);
	void setY(const scalar y);
	v2s get() const;
	void get(scalar &xi, scalar &yi) const;
	scalar x() const;
	scalar y() const;
	scalar& operator[](const unsigned int);
	scalar operator[](const unsigned int) const;
	v2s asIntPosition() const;
	scalar length() const;
	v2s inverted() const;
	v2s unit() const;
	v2s operator+(const v2s& other) const;
	v2s operator+(scalar factor) const;
	v2s operator+(int factor) const;
	v2s operator-(const v2s& other) const;
	v2s operator-(scalar factor) const;
	v2s operator-(int factor) const;
	v2s operator*(const v2s& other) const;
	v2s operator*(scalar factor) const;
	v2s operator*(int factor) const;
	v2s operator/(const v2s& other) const;
	v2s operator/(scalar factor) const;
	v2s operator/(int factor) const;
	static v2s zero;

	bool operator==(const v2s &other) const;
	bool operator!=(const v2s &other) const;
	
	
	scalar angle(const v2s &other) const;
	scalar angle() const;
	
	friend std::ostream& operator<<(std::ostream &os, const v2s& v)
	{
		return os << v.m_x << " " << v.m_y;
	}
	
};

class vxColor;

class v3s
{

protected:
	scalar m_x {0.0};
	scalar m_y {0.0};
	scalar m_z {0.0};
//TODO::question this
	scalar m_w {0.0};

public:
	
	enum class axis{kX, kY, kZ};

	v3s ();
	v3s (const v3s&& other);
	v3s (const v3s& other);
	
	v3s (scalar x, scalar y, scalar z);
	~v3s(){}
	
	void set(scalar x, scalar y, scalar z);
	void set(const v3s &other);

	void setX(scalar x);
	void setY(scalar y);
	void setZ(scalar z);

	///
	/// \brief tiny
	/// \return 
	///Returns a tiny version of this vecctor.
	v3s tiny() const;
	///
	/// \brief sqrDistance
	/// \param other
	/// \return 
	///Returns the distance to the other vector 
	/// skiping the square, this is to compare distances
	scalar sqrDistance(const v3s &other) const;
	///
	/// \brief get
	/// \param x
	/// \param y
	/// \param z
	///obtains by reference the components of the vector
	void get(scalar &x, scalar &y, scalar &z) const;

	///
	/// \brief x
	/// \return 
	///returns the x component
	scalar x() const;
	///
	/// \brief y
	/// \return 
	///returns the y component
	scalar y() const;
	///
	/// \brief z
	/// \return 
	///returns the z component
	scalar z() const;

	///
	/// \brief operator []
	/// \return 
	///returns a reference to the index value, x y or z
	scalar& operator[](const unsigned int);
	///
	/// \brief operator []
	/// \return 
	///returns a copy to the value
	scalar operator[](const unsigned int) const;
	///
	/// \brief dot
	/// \param v
	/// \return 
	///returns the dot product with the other vector.
	scalar dot(const v3s &v) const;
	///
	/// \brief cross
	/// \param v
	/// \return 
	///computes the cross vector of both vectors, this and v.
	v3s cross(const v3s& v) const;
	///
	/// \brief inverted
	/// \return 
	///returns an inverted copy of this vector.
	v3s inverted() const;
	///
	/// \brief mainAxis
	/// \return 
	///return the axis with the longer value.
	axis mainAxis() const;
	///
	/// \brief length
	/// \return 
	///computes the length of this vector to position 0
	scalar length() const;
	///
	/// \brief distance
	/// \param ref
	/// \return 
	///computes the distance to the other vector.
	scalar distance(const v3s &ref) const;
	///
	/// \brief midPoint
	/// \param other
	/// \return 
	///returns the position between this vector an the other.
	v3s midPoint(const v3s &other) const;
	///
	/// \brief aaVector
	/// \return 
	///returns an axis aligned copy of this vector.
	v3s aaVector() const;
	///
	/// \brief floorVector
	/// \return 
	///returns a vector copy of this after having floored all his components
	v3s floorVector() const;
	///
	/// \brief ceilVector
	/// \return 
	///returns a vector copy of this after having ceiled all his components
	v3s ceilVector() const;
	//TODO:fix clash with math ceil
	v3s unit() const;
	///
	/// \brief setUnit
	///makes this vector to have a lenght of 1.
	void setUnit();
	///
	/// \brief operator +
	/// \param other
	/// \return 
	///adds other vector to this one.
	v3s operator+(const v3s &other) const;
	v3s& operator+=(const v3s &other);
	v3s operator+(scalar factor) const;
	v3s operator+(int factor) const;

	v3s operator-(const v3s &other) const;
	v3s operator-(scalar factor) const;
	v3s& operator-=(const v3s &other);
	
	v3s operator*(const v3s other) const;
	v3s operator*(scalar factor) const;
	v3s& operator*=(const v3s &other);

	v3s operator/(const v3s &other) const;
	v3s operator/(scalar factor) const;
	///
	/// \brief operator ^
	/// \param b
	/// \return 
	///same as cross function
	v3s operator^(const v3s &b) const;

	// comparision
	bool operator==(const v3s &other) const;
	bool operator!=(const v3s &other) const;
	bool operator>(const v3s &other) const;
	bool operator<(const v3s &other) const;
	///
	/// \brief follows
	/// \param direction
	/// \return 
	///returns true or false if other vector follows this one 
	bool follows(const v3s &direction) const;
	///
	/// \brief angle
	/// \param b
	/// \return 
	///computes the angle between this vector and the other one
	/// with pivot in 0
	scalar angle(const v3s &b) const;
	scalar angleXY(const v3s &other) const;
	scalar angleYZ(const v3s &other) const;
	scalar angleZX(const v3s &other) const;

	scalar angleXY() const;
	scalar angleYZ() const;
	scalar angleZX() const;

	v3s operator=(const v3s &otro);

	v3s abs() const;
/*
$rota=unit($rota);

float $angk=angle(<<$rota.x,0,$rota.z>>,<<0,0,1>>);
if ($rota.x<0) $angk*=-1;
$angk+=$ang;
return <<sin($angk)*mag($rota),$rota.y,cos($angk)*mag($rota)>>;
*/
	//TODO: revisit these active = "false"e rotations
	v3s rotateX(scalar ang);

	//TODO: revisit these active = "false"e rotations
	v3s rotateY(scalar ang);

	//TODO: revisit these three rotations
	v3s rotateZ(scalar ang);

	v2s vectorXY() const;
	v2s vectorYZ() const;
	v2s vectorZX() const;
	
	bool xPositive() const;
	bool yPositive() const;
	bool zPositive() const;
	
	static v3s constX;
	static v3s constY;
	static v3s constZ;
	static v3s constXY;
	static v3s constXZ;
	static v3s constYZ;
	static v3s constXYZ;
	
	static v3s constMinusX;
	static v3s constMinusY;
	static v3s constMinusZ;
	static v3s constMinusXY;
	static v3s constMinusXZ;
	static v3s constMinusYZ;
	static v3s constMinusXYZ;
	static v3s zero;
	
	friend std::ostream& operator<<(std::ostream &os, const v3s& v)
	{
		return os << v.m_x << " " << v.m_y << " " << v.m_z;
	}
};


}
#endif
