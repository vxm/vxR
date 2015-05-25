
#ifndef _VXVECTORMC_
#define _VXVECTORMC_

#include <math.h>
#include <stdio.h>
#include "vxObject.h"
#include <iostream>

namespace vxCore {

//TODO: do I need this base class?
class vxVector:public vxObject
{
private:


public:
	
	vxVector ();

	virtual double length() const = 0;
};


class vxVector2d:public vxVector
{
private:
	double m_x {0.0};
	double m_y {0.0};

public:

	vxVector2d ();;
	
	vxVector2d (double x, double y);

	void set(double x, double y);
	
	void set(vxVector2d enter);

	void setX(const double x);
	
	void setY(const double y);

	vxVector2d get() const;
	
	void get(double &xi, double &yi) const;

	double x() const;
	
	double y() const;

	double length() const;

	vxVector2d unit() const;;

	vxVector2d operator+(vxVector2d other) const;
	
	vxVector2d operator+(double factor) const;
	
	vxVector2d operator+(float factor) const;
	vxVector2d operator+(int factor) const;

	vxVector2d operator-(vxVector2d entrada) const;
	vxVector2d operator-(double factor) const;
	vxVector2d operator-(float factor) const;
	vxVector2d operator-(int factor) const;

	vxVector2d operator*(vxVector2d entrada) const;
	vxVector2d operator*(double factor) const;
	vxVector2d operator*(float factor) const;
	vxVector2d operator*(int factor) const;

	vxVector2d operator/(vxVector2d entrada) const;
	vxVector2d operator/(double factor) const;
	vxVector2d operator/(float factor) const;
	vxVector2d operator/(int factor) const;

	double angle(const vxVector2d &other) const;

	double angle() const;
};

class vxVector3d
{

protected:
	double m_x {0.0};
	double m_y {0.0};
	double m_z {0.0};

public:

	static vxVector3d constX;
	static vxVector3d constY;
	static vxVector3d constZ;
	static vxVector3d constXY;
	static vxVector3d constXZ;
	static vxVector3d constYZ;
	static vxVector3d constXYZ;
	
	enum class axis{kX, kY, kZ};

	vxVector3d ();;
	
	vxVector3d (double x, double y, double z);

	void set(double x, double y, double z);
	void set(const vxVector3d &other);

	void setX(double x);
	void setY(double y);
	void setZ(double z);

	vxVector3d get() const;
	
	void get(double &x, double &y, double &z) const;

	double x() const;
	double y() const;
	double z() const;
	
	vxVector3d inverted() const;
	axis mainAxis() const;
	double length() const;
	double distance(const vxVector3d &ref) const;
	
	vxVector3d unit() const;;
	void setUnit();
	vxVector3d operator+(const vxVector3d &entrada) const;
	vxVector3d operator+(double factor) const;
	vxVector3d operator+(float factor) const;
	vxVector3d operator+(int factor) const;

	vxVector3d operator-(const vxVector3d &entrada) const;
	vxVector3d operator-=(const vxVector3d &entrada);
	vxVector3d operator-(double factor) const;
	
	vxVector3d operator-(float factor) const;
	vxVector3d operator-(int factor) const;
	vxVector3d operator*(const vxVector3d other) const;
	vxVector3d operator*(double factor) const;
	vxVector3d operator*(float factor) const;
	vxVector3d operator*(int factor) const;

	vxVector3d operator/(const vxVector3d &entrada);
	vxVector3d operator/(double factor);
	vxVector3d operator/(float factor) const;
	vxVector3d operator/(int factor) const;
	vxVector3d operator^(const vxVector3d &b) const;

	double angle(const vxVector3d &b) const;
	double angleXY(const vxVector3d &other) const;
	double angleYZ(const vxVector3d &other) const;
	double angleZX(const vxVector3d &other) const;

	double angleXY() const;
	double angleYZ() const;
	double angleZX() const;

	vxVector3d operator=(const vxVector3d &otro);

/*
$rota=unit($rota);

float $angk=angle(<<$rota.x,0,$rota.z>>,<<0,0,1>>);
if ($rota.x<0) $angk*=-1;
$angk+=$ang;
return <<sin($angk)*mag($rota),$rota.y,cos($angk)*mag($rota)>>;
*/
	//TODO: revisit these three rotations
	vxVector3d rotateX(double ang);

	//TODO: revisit these three rotations
	vxVector3d rotateY(double ang);

	//TODO: revisit these three rotations
	vxVector3d rotateZ(double ang);

	void vectorXY(vxVector2d &local) const;
	void vectorYZ(vxVector2d &local) const;
	void vectorZX(vxVector2d &local) const;
	
	friend std::ostream& operator<<(std::ostream &os, const vxVector3d& v)
	{
		return os << v.m_x << " " << v.m_y << " " << v.m_z;
	}
};

class vxRayXYZ :public vxVector3d
{
	// vxVector interface
	public:
		vxRayXYZ ()
			: vxVector3d()
		{};
		
		vxRayXYZ (const vxVector3d& other)
			: vxVector3d(other)
		{
		};
		
		vxRayXYZ (const vxVector3d& origin, 
				  const vxVector3d& destiny)
		{
			set(destiny - origin);
		};
		
		vxRayXYZ (double x, double y, double z)
			: vxVector3d(x,y,z)
		{
		}
};

}
#endif
