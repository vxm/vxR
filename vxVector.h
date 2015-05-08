
#ifndef _VXVECTORMC_
#define _VXVECTORMC_

#include <math.h>
#include <stdio.h>
#include "vxObject.h"
#include <iostream>

namespace vxStorage {

//TODO: do I need this base class?
class vxVector:public vxObject
{
private:


public:
	
	vxVector () {};

	virtual double length() const = 0;
};


class vxVector2d:public vxVector
{
private:
	double m_x {0.0};
	double m_y {0.0};

public:

	vxVector2d () 
	{};
	
	vxVector2d (double x, double y) 
		: m_x(x)
		, m_y(y)
	{}

	void set(double x, double y) 
	{
		m_x=x;
		m_y=y;
	}
	
	void set(vxVector2d enter) 
	{
		m_x = enter.m_x;
		m_y = enter.m_y;
	}

	void setX(const double x)
	{
		m_x=x;
	}
	
	void setY(const double y) 
	{
		m_y=y;
	}

	vxVector2d get() const
	{
		return *this;
	}
	
	void get(double &xi, double &yi) const
	{
		xi=m_x;
		yi=m_y;
	}

	double x() const
	{
		return m_x;
	}
	
	double y() const
	{
		return m_y;
	}

	double length() const 
	{//TODO:consider hypot c++11
		return sqrt(m_x*m_x+m_y*m_y);
	}

	vxVector2d unit() const 
	{
		double lng=length();
		return vxVector2d(m_x/lng,m_y/lng);
	};

	vxVector2d operator+(vxVector2d other) const
	{
		return vxVector2d(other.m_x+m_x,other.m_y+m_y);
	}
	
	vxVector2d operator+(double factor) const
	{
		return vxVector2d(factor+m_x,factor+m_y);
	}
	
	vxVector2d operator+(float factor) const {return vxVector2d(factor+m_x,factor+m_y);}
	vxVector2d operator+(int factor) const {return vxVector2d(factor+m_x,factor+m_y);}

	vxVector2d operator-(vxVector2d entrada) const {return vxVector2d(m_x-entrada.x(),m_y-entrada.y());}
	vxVector2d operator-(double factor) const {return vxVector2d(m_x-factor,m_y-factor);}
	vxVector2d operator-(float factor) const {return vxVector2d(m_x-factor,m_y-factor);}
	vxVector2d operator-(int factor) const {return vxVector2d(m_x-factor,m_y-factor);}

	vxVector2d operator*(vxVector2d entrada) const {return vxVector2d(entrada.x()*m_x,entrada.y()*m_y);}
	vxVector2d operator*(double factor) const {return vxVector2d(factor*m_x,factor*m_y);}
	vxVector2d operator*(float factor) const {return vxVector2d(factor*m_x,factor*m_y);}
	vxVector2d operator*(int factor) const {return vxVector2d(factor*m_x,factor*m_y);}

	vxVector2d operator/(vxVector2d entrada) const {return vxVector2d(entrada.x()/m_x,entrada.y()/m_y);}
	vxVector2d operator/(double factor) const {return vxVector2d(m_x/(double)factor,m_y/(double)factor);}
	vxVector2d operator/(float factor) const {return vxVector2d(m_x/(double)factor,m_y/(double)factor);}
	vxVector2d operator/(int factor) const {return vxVector2d(m_x/(double)factor,m_y/(double)factor);}

	double angle(vxVector2d other) const {return angle()-other.angle();}

	double angle() const 
	{
		return atan2(m_y,m_x);
	}
};

class vxVector3d:public vxVector
{
protected:
	double m_x {0.0};
	double m_y {0.0};
	double m_z {0.0};

public:

	enum class axis{kX, kY, kZ};

	vxVector3d ()
	{};
	
	vxVector3d (double x, double y, double z)
		: m_x(x)
		, m_y(y)
		, m_z(z)
	{
	}

	void set(double x, double y, double z) 
	{
		m_x=x;
		m_y=y;
		m_z=z;
	}
	void set(const vxVector3d &other) 
	{
		m_x=other.m_x;
		m_y=other.m_y;
		m_z=other.m_z;
	}

	void setX(double x) 
	{
		m_x=x;
	}
	void setY(double y) 
	{
		m_y=y;
	}
	void setZ(double z) 
	{
		m_z=z;
	}

	vxVector3d get() const
	{
		return *this;
	}
	
	void get(double &x, double &y, double &z) const 
	{
		x=m_x;
		y=m_y;
		z=m_z;
	}

	double x() const {return m_x;}
	double y() const {return m_y;}
	double z() const {return m_z;}

	axis mainAxis() const
	{
		if(m_x>m_y && m_x>m_z)
		{
			return axis::kX;
		}
		if(m_y>m_z && m_y>m_x)
		{
			return axis::kY;
		}
		else
			return axis::kZ;
	}
	
	double length() const
		{return sqrt(m_x*m_x+m_y*m_y+m_z*m_z);}
	
	double distance(vxVector3d ref) const
		{return (*this-ref).length();}
	
	vxVector3d unit() const
		{double lng=length();return vxVector3d(m_x/lng,m_y/lng,m_z/lng);};
	
	void setUnit() 
	{
		double lng=length();
		m_x=m_x/lng;
		m_y=m_y/lng;
		m_z=m_z/lng;
	}

	vxVector3d operator+(vxVector3d entrada) const
		{return vxVector3d(entrada.x()+m_x,entrada.y()+m_y,entrada.z()+m_z);}
	
	vxVector3d operator+(double factor) const
		{return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}
	
	vxVector3d operator+(float factor) const
		{return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}
	
	vxVector3d operator+(int factor) const
		{return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}

	vxVector3d operator-(const vxVector3d &entrada) const 
		{return vxVector3d(m_x-entrada.x(),m_y-entrada.y(),m_z-entrada.z());}
	vxVector3d operator-=(const vxVector3d &entrada)
	{	m_x-=entrada.m_x;
		m_y-=entrada.m_y;
		m_z-=entrada.m_z;
		return *this;
	}

	vxVector3d operator-(double factor) const 
		{return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}
	
	vxVector3d operator-(float factor) const 
		{return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}
	
	vxVector3d operator-(int factor) const 
		{return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}

	vxVector3d operator*(const vxVector3d other) const {return vxVector3d(other.x()*m_x,other.y()*m_y,other.z()*m_z);}
	vxVector3d operator*(double factor) const {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}
	vxVector3d operator*(float factor) const {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}
	vxVector3d operator*(int factor) const {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}

	vxVector3d operator/(const vxVector3d &entrada) 
		{return vxVector3d(entrada.x()/m_x,entrada.y()/m_y,entrada.z()/m_z);}
	
	vxVector3d operator/(double factor)
		{return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}
	
	vxVector3d operator/(float factor) const
	{return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}
	
	vxVector3d operator/(int factor) const 
		{return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}

	vxVector3d operator^(const vxVector3d &b) const
		{return vxVector3d((m_y*b.m_z)-(m_z*b.m_y),(m_x*b.m_z)-(m_z*b.m_x),(m_x*b.m_y)-(m_y*b.m_x));}

	double angle(vxVector3d b) const
	{
		double an=(m_x*b.m_x+m_y*b.m_y+m_z*b.m_z)/((sqrt((m_x*m_x)+(m_y*m_y)+(m_z*m_z)))*(sqrt((b.m_x*b.m_x)+(b.m_y*b.m_y)+(b.m_z*b.m_z))));
		return acos(an);
	}

	double angleXY(vxVector3d other) const
	{return angleXY()-other.angleXY();}
	
	double angleYZ(vxVector3d other) const
	{return angleYZ()-other.angleYZ();}
	
	double angleZX(vxVector3d other) const
	{return angleZX()-other.angleZX();}

	double angleXY() const 
		{return atan2(m_y,m_x);}
	double angleYZ() const 
		{return atan2(m_z,m_y);}
	double angleZX() const 
		{return atan2(m_x,m_z);}

	vxVector3d operator=(const vxVector3d &otro)
	{
		set(otro.x(),otro.y(),otro.z());
		return *this;
	}

/*
$rota=unit($rota);

float $angk=angle(<<$rota.x,0,$rota.z>>,<<0,0,1>>);
if ($rota.x<0) $angk*=-1;
$angk+=$ang;
return <<sin($angk)*mag($rota),$rota.y,cos($angk)*mag($rota)>>;
*/
	//TODO: revisit these three rotations
	vxVector3d rotateX(double ang)
	{
		setUnit();
		double angk = angleZX();
		angk+=ang;
		m_x=(sin(angk));
		m_z=(cos(angk));
		return *this;
	}

	//TODO: revisit these three rotations
	vxVector3d rotateY(double ang)
	{
		auto angk = ang + angleXY();
		return vxVector3d(m_z, cos(angk), sin(angk));
	}

	//TODO: revisit these three rotations
	vxVector3d rotateZ(double ang)
	{
		vxVector3d ret;
		ang+=angleYZ();
		ret.set( cos(ang) , sin(ang) , m_x );
		return ret;
	}

	void vectorXY(vxVector2d &local) const
		{local.set(m_x,m_y);}
	void vectorYZ(vxVector2d &local) const
		{local.set(m_x,m_y);}
	void vectorZX(vxVector2d &local) const
		{local.set(m_x,m_y);}
	
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
		
		vxRayXYZ (double x, double y, double z)
			: vxVector3d(x,y,z)
		{
		}
};

}
#endif
