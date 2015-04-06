
#ifndef _VXVECTORMC_
#define _VXVECTORMC_

#include <math.h>
#include <stdio.h>
#include "vxObject.h"

namespace vxStorage {


class vxVector:public vxObject
{
private:


public:
	
	vxVector () {};

	void set(vxVector enter) {*this=enter;}

	vxVector get() {return *this;}

	virtual double length() {return 0;}

	//virtual double angle(vxVector other) {return 0;}

	//virtual double angle() {return 0;}
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

	double getX() const
	{
		return m_x;
	}
	
	double getY() const
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

	vxVector2d operator-(vxVector2d entrada) const {return vxVector2d(m_x-entrada.getX(),m_y-entrada.getY());}
	vxVector2d operator-(double factor) const {return vxVector2d(m_x-factor,m_y-factor);}
	vxVector2d operator-(float factor) const {return vxVector2d(m_x-factor,m_y-factor);}
	vxVector2d operator-(int factor) const {return vxVector2d(m_x-factor,m_y-factor);}

	vxVector2d operator*(vxVector2d entrada) const {return vxVector2d(entrada.getX()*m_x,entrada.getY()*m_y);}
	vxVector2d operator*(double factor) const {return vxVector2d(factor*m_x,factor*m_y);}
	vxVector2d operator*(float factor) const {return vxVector2d(factor*m_x,factor*m_y);}
	vxVector2d operator*(int factor) const {return vxVector2d(factor*m_x,factor*m_y);}

	vxVector2d operator/(vxVector2d entrada) const {return vxVector2d(entrada.getX()/m_x,entrada.getY()/m_y);}
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
private:
	double m_x {0.0};
	double m_y {0.0};
	double m_z {0.0};

public:

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

	double getX() const {return m_x;}
	double getY() const {return m_y;}
	double getZ() const {return m_z;}

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
		{return vxVector3d(entrada.getX()+m_x,entrada.getY()+m_y,entrada.getZ()+m_z);}
	
	vxVector3d operator+(double factor) const
		{return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}
	
	vxVector3d operator+(float factor) const
		{return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}
	
	vxVector3d operator+(int factor) const
		{return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}

	vxVector3d operator-(const vxVector3d &entrada) const 
		{return vxVector3d(m_x-entrada.getX(),m_y-entrada.getY(),m_z-entrada.getZ());}
	
	vxVector3d operator-(double factor) const 
		{return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}
	
	vxVector3d operator-(float factor) const 
		{return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}
	
	vxVector3d operator-(int factor) const 
		{return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}

	vxVector3d operator*(const vxVector3d other) const {return vxVector3d(other.getX()*m_x,other.getY()*m_y,other.getZ()*m_z);}
	vxVector3d operator*(double factor) const {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}
	vxVector3d operator*(float factor) const {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}
	vxVector3d operator*(int factor) const {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}

	vxVector3d operator/(const vxVector3d &entrada) 
		{return vxVector3d(entrada.getX()/m_x,entrada.getY()/m_y,entrada.getZ()/m_z);}
	
	vxVector3d operator/(double factor)
		{return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}
	
	vxVector3d operator/(float factor) const
	{return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}
	
	vxVector3d operator/(int factor) const 
		{return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}

	vxVector3d operator^(const vxVector3d &b) const
		{return vxVector3d((this->m_y*b.m_z)-(this->m_z*b.m_y),(this->m_x*b.m_z)-(this->m_z*b.m_x),(this->m_x*b.m_y)-(this->m_y*b.m_x));}

	double angle(vxVector3d b) const
	{
		double an=(this->m_x*b.m_x+this->m_y*b.m_y+this->m_z*b.m_z)/((sqrt((this->m_x*this->m_x)+(this->m_y*this->m_y)+(this->m_z*this->m_z)))*(sqrt((b.m_x*b.m_x)+(b.m_y*b.m_y)+(b.m_z*b.m_z))));
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

	void operator=(const vxVector3d &otro)
	{
		set(otro.getX(),otro.getY(),otro.getZ());
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
};

class vxColor:public vxObject
{
protected:
	double m_r = {0.0};
	double m_g = {0.0};
	double m_b = {0.0};
	double m_a = {1.0};
	
public:

	vxColor (){};
	vxColor (int r, int g, int b, int a) 
		: m_r(r)
		, m_g(g)
		, m_b(b)
		, m_a(a)
	{}
	
	vxColor (int r, int g, int b) 
		: m_r(r)
		, m_g(g)
		, m_b(b)
	{}
	
	vxColor& operator=(const vxVector3d &otro)
	{
		set(otro.getX(), otro.getY(), otro.getZ());
		return *this;
	}
	
	void set(int r, int g, int b, int a)
	{m_r=r;m_g=g;m_b=b;m_a=a;}
	
	void set(int r, int g, int b)
	{m_r=r;m_g=g;m_b=b;m_a=1.0;}
	
	void set(const vxColor &other)
	{
		m_r = other.m_r;
		m_g = other.m_g;
		m_b = other.m_b;
		m_a = other.m_a;
	}
	
	void reset() {m_r=0;m_g=0;m_b=0;m_a=1.0;}

	void setR(int r) {m_r=r;}
	void setG(int g) {m_g=g;}
	void setB(int b) {m_b=b;}
	void setA(int a) {m_a=a;}

	vxColor get() const 
		{return *this;}
	
	void get(int &ri, int &gi, int &bi, int &ai) const
		{ri=m_r;gi=m_g;bi=m_b;ai=m_a;}

	int getR() const {return m_r;}
	int getG() const {return m_g;}
	int getB() const {return m_b;}
	int getA() const {return m_a;}

	double lumma() const {return sqrt((double)m_r*m_r+m_g*m_g+m_b*m_b);}

	vxColor operator+(const vxColor &entrada) const {return vxColor(entrada.getR()+m_r,entrada.getG()+m_g,entrada.getB()+m_b,entrada.getA()+m_a);}
	vxColor operator+(double factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}
	vxColor operator+(float factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}
	vxColor operator+(int factor) const {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_b);}

	vxColor operator-(const vxColor &entrada) const {return vxColor(m_r-entrada.getR(),m_g-entrada.getG(),m_b-entrada.getB(),m_a-entrada.getA());}
	vxColor operator-(double factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}
	vxColor operator-(float factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}
	vxColor operator-(int factor) const {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

	vxColor operator*(const vxColor &entrada) const {return vxColor(entrada.getR()*m_r,entrada.getG()*m_g,entrada.getB()*m_b,entrada.getA()*m_a);}
	vxColor operator*(double factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}
	vxColor operator*(float factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}
	vxColor operator*(int factor) const {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

	vxColor operator/(const vxColor &entrada) const {return vxColor(entrada.getR()/(double)m_r,entrada.getG()/(double)m_g,entrada.getB()/(double)m_b,entrada.getA()/(double)m_a);}
	vxColor operator/(double factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}
	vxColor operator/(float factor) const {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}
	vxColor operator/(int factor) const {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

	void toRGBA8888(unsigned char *tbuff) const
	{
		*tbuff = (unsigned char)char(m_r);
		tbuff++;
		
		*tbuff = (unsigned char)char(m_g);
		tbuff++;

		*tbuff = (unsigned char)char(m_b);
	}
};



}
#endif
