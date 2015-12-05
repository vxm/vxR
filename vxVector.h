
#ifndef _VXVECTORMC_
#define _VXVECTORMC_

#include <math.h>
#include <stdio.h>
#include <iostream>

namespace vxCore {

//TODO: add literal for scalar 0.0r
using scalar = double;

class vxVector2d
{
private:
	scalar m_x {0.0};
	scalar m_y {0.0};

public:

	vxVector2d ();
	vxVector2d (scalar x, scalar y);
	vxVector2d (const vxVector2d& other);
	vxVector2d (vxVector2d&& other);
	vxVector2d& operator=(const vxVector2d& other);
	
	void set(scalar x, scalar y);
	void set(const vxVector2d &enter);
	void setX(const scalar x);
	void setY(const scalar y);
	vxVector2d get() const;
	void get(scalar &xi, scalar &yi) const;
	scalar x() const;
	scalar y() const;
	scalar& operator[](const unsigned int);
	scalar operator[](const unsigned int) const;
	vxVector2d asIntPosition() const;
	scalar length() const;
	vxVector2d inverted() const;
	vxVector2d unit() const;
	vxVector2d operator+(const vxVector2d& other) const;
	vxVector2d operator+(scalar factor) const;
	vxVector2d operator+(int factor) const;
	vxVector2d operator-(const vxVector2d& entrada) const;
	vxVector2d operator-(scalar factor) const;
	vxVector2d operator-(int factor) const;
	vxVector2d operator*(const vxVector2d& entrada) const;
	vxVector2d operator*(scalar factor) const;
	vxVector2d operator*(int factor) const;
	vxVector2d operator/(const vxVector2d& entrada) const;
	vxVector2d operator/(scalar factor) const;
	vxVector2d operator/(int factor) const;
	static vxVector2d zero;

	bool operator==(const vxVector2d &other) const;
	
	bool operator!=(const vxVector2d &other) const
	{
		return other.m_x != m_x || other.m_y != m_y;
	}
	
	
	scalar angle(const vxVector2d &other) const;
	scalar angle() const;
	
	friend std::ostream& operator<<(std::ostream &os, const vxVector2d& v)
	{
		return os << v.m_x << " " << v.m_y;
	}
	
};

class vxColor;

class vxVector3d
{

protected:
	scalar m_x {0.0};
	scalar m_y {0.0};
	scalar m_z {0.0};

	//std::valarray<scalar> m_v{0.0,3};
	
public:
	
	enum class axis{kX, kY, kZ};

	vxVector3d ();
	vxVector3d (const vxVector3d&& other);
	vxVector3d (const vxVector3d& other);
	
	vxVector3d (scalar x, scalar y, scalar z);
	~vxVector3d(){}
	

	void set(scalar x, scalar y, scalar z);
	void set(const vxVector3d &other);

	void setX(scalar x);
	void setY(scalar y);
	void setZ(scalar z);

	vxVector3d get() const;
	
	void get(scalar &x, scalar &y, scalar &z) const;

	scalar x() const;
	scalar y() const;
	scalar z() const;

	scalar& operator[](const unsigned int);
	scalar operator[](const unsigned int) const;

	scalar dot(const vxVector3d &v) const;
	vxVector3d cross(const vxVector3d& v) const;
	vxVector3d inverted() const;
	axis mainAxis() const;
	scalar length() const;
	scalar distance(const vxVector3d &ref) const;
	
	vxVector3d midPoint(const vxVector3d &other) const;
	vxVector3d aaVector() const;
	vxVector3d floorVector() const;
	vxVector3d ceilVector() const;
	//TODO:fix clash with math ceil
	vxVector3d unit() const;
	void setUnit();
	vxVector3d operator+(const vxVector3d &entrada) const;
	vxVector3d operator+=(const vxVector3d &entrada);
	vxVector3d operator+(scalar factor) const;
	vxVector3d operator+(int factor) const;

	vxVector3d operator-(const vxVector3d &entrada) const;
	vxVector3d operator-=(const vxVector3d &entrada);
	
	vxVector3d operator-(scalar factor) const;
	vxVector3d operator-(int factor) const;
	vxVector3d operator*(const vxVector3d other) const;
	vxVector3d operator*(scalar factor) const;
	vxVector3d operator*(int factor) const;

	vxVector3d operator/(const vxVector3d &entrada) const;
	vxVector3d operator/(scalar factor) const;
	vxVector3d operator/(int factor) const;
	vxVector3d operator^(const vxVector3d &b) const;

	// comparision
	bool operator==(const vxVector3d &other) const;
	bool operator!=(const vxVector3d &other) const;
	bool operator>(const vxVector3d &other) const;
	bool operator<(const vxVector3d &other) const;

	bool follows(const vxVector3d &direction) const;
	scalar angle(const vxVector3d &b) const;
	scalar angleXY(const vxVector3d &other) const;
	scalar angleYZ(const vxVector3d &other) const;
	scalar angleZX(const vxVector3d &other) const;

	scalar angleXY() const;
	scalar angleYZ() const;
	scalar angleZX() const;

	vxVector3d operator=(const vxVector3d &otro);

	vxVector3d abs() const;
/*
$rota=unit($rota);

float $angk=angle(<<$rota.x,0,$rota.z>>,<<0,0,1>>);
if ($rota.x<0) $angk*=-1;
$angk+=$ang;
return <<sin($angk)*mag($rota),$rota.y,cos($angk)*mag($rota)>>;
*/
	//TODO: revisit these active = "false"e rotations
	vxVector3d rotateX(scalar ang);

	//TODO: revisit these active = "false"e rotations
	vxVector3d rotateY(scalar ang);

	//TODO: revisit these three rotations
	vxVector3d rotateZ(scalar ang);

	vxVector2d vectorXY() const;
	vxVector2d vectorYZ() const;
	vxVector2d vectorZX() const;
	
	bool xPositive() const;
	bool yPositive() const;
	bool zPositive() const;
	
	static vxVector3d constX;
	static vxVector3d constY;
	static vxVector3d constZ;
	static vxVector3d constXY;
	static vxVector3d constXZ;
	static vxVector3d constYZ;
	static vxVector3d constXYZ;
	
	static vxVector3d constMinusX;
	static vxVector3d constMinusY;
	static vxVector3d constMinusZ;
	static vxVector3d constMinusXY;
	static vxVector3d constMinusXZ;
	static vxVector3d constMinusYZ;
	static vxVector3d constMinusXYZ;
	static vxVector3d zero;
	
	friend std::ostream& operator<<(std::ostream &os, const vxVector3d& v)
	{
		return os << v.m_x << " " << v.m_y << " " << v.m_z;
	}
};


using v2 = vxVector2d;
using v3 = vxVector3d;

}
#endif
