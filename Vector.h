
#ifndef _VXVECTORMC_
#define _VXVECTORMC_

#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <quadmath.h>
#include <stdio.h>

namespace vxCore
{

// TODO: add literal for scalar 0.0r
using scalar = double; // float // double // __float128

constexpr const scalar minScalar = std::numeric_limits<scalar>::min();
constexpr const scalar tinyScalar = fabs(minScalar) * scalar(1000.0);
constexpr const scalar smallScalar = scalar(1.0) / scalar(100000.0);

constexpr const scalar maxScalar = std::numeric_limits<scalar>::max();
constexpr const scalar bigScalar = fabs(maxScalar) * scalar(0.001);
constexpr const scalar sZero = scalar(0.0);

class Color;

class v2s final
{
	scalar m_x{0.0};
	scalar m_y{0.0};

public:
	///
	/// \brief v2s
	///
	v2s();
	///
	/// \brief v2s
	/// \param x
	/// \param y
	///
	v2s(scalar x, scalar y);
	///
	/// \brief v2s
	/// \param other
	///
	v2s(const v2s &other);
	///
	/// \brief operator =
	/// \param other
	/// \return
	///
	v2s &operator=(const v2s &other);
	///
	/// \brief set
	/// \param x
	/// \param y
	///
	void set(scalar x, scalar y);
	///
	/// \brief set
	/// \param enter
	///
	void set(const v2s &enter);
	///
	/// \brief setX
	/// \param x
	///
	void setX(const scalar x);
	///
	/// \brief setY
	/// \param y
	///
	void setY(const scalar y);
	///
	/// \brief get
	/// \return
	///
	v2s get() const;
	///
	/// \brief get
	/// \param xi
	/// \param yi
	///
	void get(scalar &xi, scalar &yi) const;
	///
	/// \brief x
	/// \return
	///
	scalar x() const;
	///
	/// \brief y
	/// \return
	///
	scalar y() const;
	///
	/// \brief operator []
	/// \return
	///
	scalar &operator[](const unsigned int);
	///
	/// \brief operator []
	/// \return
	///
	scalar operator[](const unsigned int) const;
	///
	/// \brief asIntPosition
	/// \return
	///
	v2s asIntPosition() const;
	///
	/// \brief length
	/// \return
	///
	scalar length() const;
	///
	/// \brief inverted
	/// \return
	///
	v2s inverted() const;
	///
	/// \brief unit
	/// \return
	///
	v2s unit() const;
	///
	/// \brief dot
	/// \param v
	/// \return
	///
	scalar dot(const v2s &v)
	{
		scalar o = angle(v);
		return length() * v.length() * cos(o);
	}
	///
	/// \brief operator +
	/// \param other
	/// \return
	///
	v2s operator+(const v2s &other) const;
	///
	/// \brief operator +
	/// \param factor
	/// \return
	///
	v2s operator+(const scalar factor) const;
	///
	/// \brief operator +
	/// \param factor
	/// \return
	///
	v2s operator+(const int factor) const;
	///
	/// \brief operator -
	/// \param other
	/// \return
	///
	v2s operator-(const v2s &other) const;
	///
	/// \brief operator -
	/// \param factor
	/// \return
	///
	v2s operator-(const scalar factor) const;
	///
	/// \brief operator -
	/// \param factor
	/// \return
	///
	v2s operator-(const int factor) const;
	///
	/// \brief operator *
	/// \param other
	/// \return
	///
	v2s operator*(const v2s &other) const;
	///
	/// \brief operator *
	/// \param factor
	/// \return
	///
	v2s operator*(const scalar factor) const;
	///
	/// \brief operator *
	/// \param factor
	/// \return
	///
	v2s operator*(const int factor) const;
	///
	/// \brief operator /
	/// \param other
	/// \return
	///
	v2s operator/(const v2s &other) const;
	///
	/// \brief operator /
	/// \param factor
	/// \return
	///
	v2s operator/(const scalar factor) const;
	///
	/// \brief operator /
	/// \param factor
	/// \return
	///
	v2s operator/(const int factor) const;
	///
	/// \brief operator ==
	/// \param other
	/// \return
	///
	bool operator==(const v2s &other) const;
	///
	/// \brief operator !=
	/// \param other
	/// \return
	///
	bool operator!=(const v2s &other) const;
	///
	/// \brief angle
	/// \param other
	/// \return
	///
	scalar angle(const v2s &other) const;
	///
	/// \brief angle
	/// \return
	///
	scalar angle() const;
	///
	/// \brief sqrDistance
	/// \param other
	/// \return
	/// Returns the distance to the other vector
	/// skiping the square, this is to compare distances
	scalar sqrDistance(const v2s &other) const;
	///
	/// \brief distance
	/// \param other
	/// \return
	///
	scalar distance(const v2s &other) const;

	///
	/// \brief operator <<
	/// \param os
	/// \param v
	/// \return
	///
	friend std::ostream &operator<<(std::ostream &os, const v2s &v)
	{
		return os << v.m_x << " " << v.m_y;
	}
};

static v2s zero2;

class v3s final
{

protected:
	scalar m_x{0.0};
	scalar m_y{0.0};
	scalar m_z{0.0};
	// TODO::question this
	// TODO: do all todos :)
	scalar m_w{0.0};

public:
	///
	/// \brief The axis enum
	/// Identifies all different axis.
	enum class axis
	{
		kX,
		kY,
		kZ
	};

	///
	/// \brief v3s
	/// Default constructor.
	v3s();
	///
	/// \brief v3s
	/// \param vals
	/// Constructs from a pointer
	explicit v3s(scalar vals[3]);
	///
	/// \brief v3s
	/// \param other
	/// Move constructor
	v3s(const v3s &&other);
	///
	/// \brief v3s
	/// \param other
	/// Copy constructor.
	v3s(const v3s &other);
	///
	/// \brief v3s
	/// \param x
	/// \param y
	/// \param z
	/// Components constructor.
	v3s(scalar x, scalar y, scalar z);
	/// empty destructor
	~v3s() {}
	///
	/// \brief set
	/// \param x
	/// \param y
	/// \param z
	/// replaces xyz with the new values.
	void set(scalar x, scalar y, scalar z);
	///
	/// \brief set
	/// \param other
	/// Replaces the values with the new vector values
	void set(const v3s &other);
	///
	/// \brief setX
	/// \param x
	/// Sets the component X
	void setX(scalar x);
	///
	/// \brief setY
	/// \param y
	/// Sets the component Y
	void setY(scalar y);
	///
	/// \brief setZ
	/// \param z
	/// Sets the component Z
	void setZ(scalar z);

	///
	/// \brief isCloser
	/// \param a
	/// \param b
	/// \return
	///
	bool isCloser(const v3s &a, const v3s &b) const;
	///
	/// \brief tiny
	/// \return
	/// Returns a tiny version of this vecctor.
	v3s tiny() const;
	///
	/// \brief sqrDistance
	/// \param other
	/// \return
	/// Returns the distance to the other vector
	/// skiping the square, this is to compare distances
	scalar sqrDistance(const v3s &other) const;
	///
	/// \brief get
	/// \param x
	/// \param y
	/// \param z
	/// obtains by reference the components of the vector
	void get(scalar &x, scalar &y, scalar &z) const;
	///
	/// \brief x
	/// \return
	/// returns the x component
	scalar x() const;
	///
	/// \brief y
	/// \return
	/// returns the y component
	scalar y() const;
	///
	/// \brief z
	/// \return
	/// returns the z component
	scalar z() const;
	///
	/// \brief xy
	/// \return
	///
	v2s xy() const;
	///
	/// \brief xz
	/// \return
	///
	v2s xz() const;
	///
	/// \brief yz
	/// \return
	///
	v2s yz() const;
	///
	/// \brief operator []
	/// \return
	/// returns a reference to the index value, x y or z
	scalar &operator[](const unsigned int);
	///
	/// \brief operator []
	/// \return
	/// returns a copy to the value
	scalar operator[](const unsigned int) const;
	///
	/// \brief small
	/// \return
	///
	v3s small() const;
	///
	/// \brief dot
	/// \param v
	/// \return
	/// returns the dot product with the other vector.
	scalar dot(const v3s &v) const;
	///
	/// \brief cross
	/// \param v
	/// \return
	/// computes the cross vector of both vectors, this and v.
	v3s cross(const v3s &v) const;
	///
	/// \brief inverted
	/// \return
	/// returns an inverted copy of this vector.
	v3s inverted() const;
	///
	/// \brief mainAxis
	/// \return
	/// return the axis with the longer value.
	axis mainAxis() const;
	///
	/// \brief length
	/// \return
	/// computes the length of this vector to position 0
	scalar length() const;
	///
	/// \brief distance
	/// \param ref
	/// \return
	/// computes the distance to the other vector.
	scalar distance(const v3s &ref) const;
	///
	/// \brief midPoint
	/// \param other
	/// \return
	/// returns the position between this vector an the other.
	v3s midPoint(const v3s &other) const;
	///
	/// \brief aaVector
	/// \return
	/// returns an axis aligned copy of this vector.
	v3s aaVector() const;
	///
	/// \brief floorVector
	/// \return
	/// returns a vector copy of this after having floored all his components
	v3s floorVector() const;
	///
	/// \brief ceilVector
	/// \return
	/// returns a vector copy of this after having ceiled all his components
	v3s ceilVector() const;
	// TODO:fix clash with math ceil
	v3s unit() const;
	///
	/// \brief setUnit
	/// makes this vector to have a lenght of 1.
	void setUnit();
	///
	/// \brief operator +
	/// \param other
	/// \return
	/// adds other vector to this one.
	v3s operator+(const v3s &other) const;
	///
	/// \brief operator +=
	/// \param other
	/// \return
	/// Add the vector in place
	v3s &operator+=(const v3s &other);
	///
	/// \brief operator +
	/// \param factor
	/// \return
	/// Adds the factor and returns a new vector.
	v3s operator+(scalar factor) const;
	///
	/// \brief operator -
	/// \param other
	/// \return
	/// Vector substraction with other vector.
	v3s operator-(const v3s &other) const;
	///
	/// \brief operator -
	/// \param factor
	/// \return
	/// substracts in place each component with
	/// that factor.
	v3s operator-(scalar factor) const;
	///
	/// \brief operator -=
	/// \param other
	/// \return
	/// substracts in place
	v3s &operator-=(const v3s &other);
	///
	/// \brief operator -=
	/// \param f
	/// \return
	///
	v3s &operator-=(const scalar f);
	///
	/// \brief operator *
	/// \param other
	/// \return
	/// multiplies each component with each
	/// component of the other vector.
	v3s operator*(const v3s other) const;
	///
	/// \brief operator *
	/// \param factor
	/// \return
	/// Multiplies each component with the factor
	/// and returns the result.
	v3s operator*(scalar factor) const;
	///
	/// \brief operator *=
	/// \param other
	/// \return
	/// Multiply implace with the other vector.
	v3s &operator*=(const v3s &other);
	///
	/// \brief operator /
	/// \param other
	/// \return
	/// Divides with other vector
	v3s operator/(const v3s &other) const;
	///
	/// \brief operator /
	/// \param factor
	/// \return
	/// Divides each component with factor.
	v3s operator/(scalar factor) const;
	///
	/// \brief operator /=
	/// \param f
	/// \return
	/// divides itself with the factor
	v3s &operator/=(const scalar f);

	///
	/// \brief operator ^
	/// \param b
	/// \return
	/// same as cross function
	v3s operator^(const v3s &b) const;
	///
	/// \brief rotate
	/// \param axis
	/// \param angle
	/// \return
	/// returns a copy of this vector rotated over axis
	/// with the angle
	v3s rotate(const v3s &axis, const scalar angle);

	// comparision
	bool operator==(const v3s &other) const;
	bool operator!=(const v3s &other) const;
	bool operator>(const v3s &other) const;
	bool operator<(const v3s &other) const;
	///
	/// \brief invert
	///
	void invert();
	///
	/// \brief follows
	/// \param direction
	/// \return
	/// returns true or false if other vector follows this one
	bool follows(const v3s &direction) const;
	///
	/// \brief angle
	/// \param b
	/// \return
	/// computes the angle between this vector and the other one
	/// with pivot in 0
	scalar angle(const v3s &b) const;
	///
	/// \brief angleXY
	/// \param other
	/// \return
	///
	scalar angleXY(const v3s &other) const;
	///
	/// \brief angleYZ
	/// \param other
	/// \return
	///
	scalar angleYZ(const v3s &other) const;
	///
	/// \brief angleZX
	/// \param other
	/// \return
	///
	scalar angleZX(const v3s &other) const;
	///
	/// \brief angleXY
	/// \return
	///
	scalar angleXY() const;
	///
	/// \brief angleYZ
	/// \return
	///
	scalar angleYZ() const;
	///
	/// \brief angleZX
	/// \return
	///
	scalar angleZX() const;
	///
	/// \brief operator =
	/// \param otro
	/// \return
	///
	v3s operator=(const v3s &otro);
	///
	/// \brief abs
	/// \return
	///
	v3s abs() const;
	/*
	$rota=unit($rota);

	float $angk=angle(<<$rota.x,0,$rota.z>>,<<0,0,1>>);
	if ($rota.x<0) $angk*=-1;
	$angk+=$ang;
	return <<sin($angk)*mag($rota),$rota.y,cos($angk)*mag($rota)>>;
	*/
	// TODO: revisit these active = "false"e rotations
	v3s rotateX(scalar ang);

	// TODO: revisit these active = "false"e rotations
	v3s rotateY(scalar ang);

	// TODO: revisit these three rotations
	v3s rotateZ(scalar ang);

	v2s vectorXY() const;
	v2s vectorYZ() const;
	v2s vectorZX() const;

	bool xPositive() const;
	bool yPositive() const;
	bool zPositive() const;

	friend std::ostream &operator<<(std::ostream &os, const v3s &v)
	{
		return os << v.m_x << " " << v.m_y << " " << v.m_z;
	}
};

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
static v3s zero3;
}
#endif
