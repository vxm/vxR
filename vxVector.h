
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
	double m_x;
	double m_y;

public:

	vxVector2d () {m_x=0;m_y=0;};
	vxVector2d (double xi, double yi) {m_x=xi;m_y=yi;}

	void set(double xi, double yi) {m_x=xi;m_y=yi;}
	void set(vxVector2d enter) {*this=enter;}

	void putX(double xi) {m_x=xi;}
	void putY(double yi) {m_y=yi;}

	vxVector2d get() {return *this;}
	void get(double &xi, double &yi) {xi=m_x;yi=m_y;}

	double getX() {return m_x;}
	double getY() {return m_y;}

	double length() {return sqrt(m_x*m_x+m_y*m_y);}

	vxVector2d unit() {double lng=length();return vxVector2d(m_x/lng,m_y/lng);};

	vxVector2d operator+(vxVector2d entrada) {return vxVector2d(entrada.getX()+m_x,entrada.getY()+m_y);}
	vxVector2d operator+(double factor) {return vxVector2d(factor+m_x,factor+m_y);}
	vxVector2d operator+(float factor) {return vxVector2d(factor+m_x,factor+m_y);}
	vxVector2d operator+(int factor) {return vxVector2d(factor+m_x,factor+m_y);}

	vxVector2d operator-(vxVector2d entrada) {return vxVector2d(m_x-entrada.getX(),m_y-entrada.getY());}
	vxVector2d operator-(double factor) {return vxVector2d(m_x-factor,m_y-factor);}
	vxVector2d operator-(float factor) {return vxVector2d(m_x-factor,m_y-factor);}
	vxVector2d operator-(int factor) {return vxVector2d(m_x-factor,m_y-factor);}

	vxVector2d operator*(vxVector2d entrada) {return vxVector2d(entrada.getX()*m_x,entrada.getY()*m_y);}
	vxVector2d operator*(double factor) {return vxVector2d(factor*m_x,factor*m_y);}
	vxVector2d operator*(float factor) {return vxVector2d(factor*m_x,factor*m_y);}
	vxVector2d operator*(int factor) {return vxVector2d(factor*m_x,factor*m_y);}

	vxVector2d operator/(vxVector2d entrada) {return vxVector2d(entrada.getX()/m_x,entrada.getY()/m_y);}
	vxVector2d operator/(double factor) {return vxVector2d(m_x/(double)factor,m_y/(double)factor);}
	vxVector2d operator/(float factor) {return vxVector2d(m_x/(double)factor,m_y/(double)factor);}
	vxVector2d operator/(int factor) {return vxVector2d(m_x/(double)factor,m_y/(double)factor);}

	double angle(vxVector2d other) {return angle()-other.angle();}

	double angle() {return atan2(m_y,m_x);}
};

class vxVector3d:public vxVector
{
private:
	double m_x;
	double m_y;
	double m_z;

public:

	vxVector3d () {m_x=0;m_y=0;m_z=0;};
	vxVector3d (double x, double y, double z) {this->m_x=x;this->m_y=y;this->m_z=z;}

	void set(double xi, double yi, double zi) {m_x=xi;m_y=yi;m_z=zi;}
	void set(vxVector3d enter) {*this=enter;}

	void putX(double x) {this->m_x=x;}
	void putY(double y) {this->m_y=y;}
	void putZ(double z) {this->m_z=z;}

	vxVector3d get() {return *this;}
	void get(double &x, double &y, double &z) {x=this->m_x;y=this->m_y;z=this->m_z;}

	double getX() {return m_x;}
	double getY() {return m_y;}
	double getZ() {return m_z;}

	double length() {return sqrt(m_x*m_x+m_y*m_y+m_z*m_z);}
	double distance(vxVector3d ref) {return (*this-ref).length();}
	vxVector3d unit() {double lng=length();return vxVector3d(m_x/lng,m_y/lng,m_z/lng);};
	void setUnit() {double lng=length();m_x=m_x/lng,m_y=m_y/lng,m_z=m_z/lng;}

	vxVector3d operator+(vxVector3d entrada) {return vxVector3d(entrada.getX()+m_x,entrada.getY()+m_y,entrada.getZ()+m_z);}
	vxVector3d operator+(double factor) {return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}
	vxVector3d operator+(float factor) {return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}
	vxVector3d operator+(int factor) {return vxVector3d(factor+m_x,factor+m_y,factor+m_z);}

	vxVector3d operator-(vxVector3d entrada) {return vxVector3d(m_x-entrada.getX(),m_y-entrada.getY(),m_z-entrada.getZ());}
	vxVector3d operator-(double factor) {return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}
	vxVector3d operator-(float factor) {return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}
	vxVector3d operator-(int factor) {return vxVector3d(m_x-factor,m_y-factor,m_z-factor);}

	vxVector3d operator*(vxVector3d entrada) {return vxVector3d(entrada.getX()*m_x,entrada.getY()*m_y,entrada.getZ()*m_z);}
	vxVector3d operator*(double factor) {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}
	vxVector3d operator*(float factor) {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}
	vxVector3d operator*(int factor) {return vxVector3d(factor*m_x,factor*m_y,factor*m_z);}

	vxVector3d operator/(vxVector3d entrada) {return vxVector3d(entrada.getX()/m_x,entrada.getY()/m_y,entrada.getZ()/m_z);}
	vxVector3d operator/(double factor) {return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}
	vxVector3d operator/(float factor) {return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}
	vxVector3d operator/(int factor) {return vxVector3d(m_x/(double)factor,m_y/(double)factor,m_z/(double)factor);}

	vxVector3d operator^(vxVector3d b) {return vxVector3d((this->m_y*b.m_z)-(this->m_z*b.m_y),(this->m_x*b.m_z)-(this->m_z*b.m_x),(this->m_x*b.m_y)-(this->m_y*b.m_x));}

	double angle(vxVector3d b) 
	{
		double an=(this->m_x*b.m_x+this->m_y*b.m_y+this->m_z*b.m_z)/((sqrt((this->m_x*this->m_x)+(this->m_y*this->m_y)+(this->m_z*this->m_z)))*(sqrt((b.m_x*b.m_x)+(b.m_y*b.m_y)+(b.m_z*b.m_z))));
		return acos(an);
	}

	double angleXY(vxVector3d other) {return angleXY()-other.angleXY();} 
	double angleYZ(vxVector3d other) {return angleYZ()-other.angleYZ();} 
	double angleZX(vxVector3d other) {return angleZX()-other.angleZX();} 

	double angleXY() {return atan2(m_y,m_x);}
	double angleYZ() {return atan2(m_z,m_y);}
	double angleZX() {return atan2(m_x,m_z);}


	void operator=(vxVector3d otro)
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
	vxVector3d rotateX(double ang)
	{   
		setUnit();
		double angk = angleZX();
	//	if(x<0) angk*=-1;
		angk+=ang;
		m_z=(cos(angk));
		m_x=(sin(angk));
		return *this;
	}

	vxVector3d rotateY(double ang)
	{
		vxVector3d ret;
		ang+=angleXY();
		ret.set( m_z , cos(ang) , sin(ang) );
		return ret;
	}

	vxVector3d rotateZ(double ang)
	{
		vxVector3d ret;
		ang+=angleYZ();
		ret.set( cos(ang) , sin(ang) , m_x );
		return ret;
	}

	void vectorXY(vxVector2d &local) {local.set(m_x,m_y);}
	void vectorYZ(vxVector2d &local) {local.set(m_x,m_y);}
	void vectorZX(vxVector2d &local) {local.set(m_x,m_y);}
};

class vxColor:public vxObject
{
protected:
	double m_r;
	double m_g;
	double m_b;
	double m_a;
	
public:

	vxColor (){m_r=0;m_g=0;m_b=0;m_a=255;};
	vxColor (int ri, int gi, int bi, int ai) {m_r=ri;m_g=gi;m_b=bi;m_a=ai;}
	vxColor (int ri, int gi, int bi) {m_r=ri;m_g=gi;m_b=bi;m_a=255;}

	void set(int ri, int gi, int bi, int ai) {m_r=ri;m_g=gi;m_b=bi;m_a=ai;}
	void set(int ri, int gi, int bi) {m_r=ri;m_g=gi;m_b=bi;m_a=255;}
	void set(vxColor enter) {*this=enter;}

	void putR(int ri) {m_r=ri;}
	void putG(int gi) {m_g=gi;}
	void putB(int bi) {m_b=bi;}
	void putA(int ai) {m_a=ai;}

	vxColor get() {return *this;}
	void get(int &ri, int &gi, int &bi, int &ai) {ri=m_r;gi=m_g;bi=m_b;ai=m_a;}

	int getR() {return m_r;}
	int getG() {return m_g;}
	int getB() {return m_b;}
	int getA() {return m_a;}

	double lumma() {return sqrt((double)m_r*m_r+m_g*m_g+m_b*m_b);}

	void operator=(vxVector3d otro)
	{
		m_r=(int)otro.getX()*255;
		m_g=(int)otro.getY()*255;
		m_b=(int)otro.getZ()*255;
	}


	vxColor operator+(vxColor entrada) {return vxColor(entrada.getR()+m_r,entrada.getG()+m_g,entrada.getB()+m_b,entrada.getA()+m_a);}
	vxColor operator+(double factor) {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}
	vxColor operator+(float factor) {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_a);}
	vxColor operator+(int factor) {return vxColor(factor+m_r,factor+m_g,factor+m_b,factor+m_b);}

	vxColor operator-(vxColor entrada) {return vxColor(m_r-entrada.getR(),m_g-entrada.getG(),m_b-entrada.getB(),m_a-entrada.getA());}
	vxColor operator-(double factor) {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}
	vxColor operator-(float factor) {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}
	vxColor operator-(int factor) {return vxColor(m_r-factor,m_g-factor,m_b-factor,m_a-factor);}

	vxColor operator*(vxColor entrada) {return vxColor(entrada.getR()*m_r,entrada.getG()*m_g,entrada.getB()*m_b,entrada.getA()*m_a);}
	vxColor operator*(double factor) {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}
	vxColor operator*(float factor) {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}
	vxColor operator*(int factor) {return vxColor(factor*m_r,factor*m_g,factor*m_b,factor*m_a);}

	vxColor operator/(vxColor entrada) {return vxColor(entrada.getR()/(double)m_r,entrada.getG()/(double)m_g,entrada.getB()/(double)m_b,entrada.getA()/(double)m_a);}
	vxColor operator/(double factor) {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}
	vxColor operator/(float factor) {return vxColor(m_r/(double)factor,m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}
	vxColor operator/(int factor) {return vxColor(m_r/double(factor),m_g/(double)factor,m_b/(double)factor,m_a/(double)factor);}

};


class vxPoint:public vxVector3d
{
public:

	bool m_valid;
	
	vxPoint():vxVector3d()
	{
		m_valid=false;
	}

	void operator=(vxVector3d otro)
	{
		set(otro.getX(),otro.getY(),otro.getZ());
	}
};

class vxCollision:public vxObject
{
private:

	vxPoint m_position;
	vxColor m_color;
	vxVector3d m_normal;
	bool m_valid;
	double m_u;
	double m_v;

public:

	vxCollision() 
	{
		setValid(false);
		m_u=0;
		m_v=0;
	};

	void initialize()
	{
		setValid(false);
		m_position.set(0,0,0);
		m_color.set(0,0,0);
		m_normal.set(0,0,0);
		m_u=0;
		m_v=0;
	}


	void setValid(bool v)
	{
		m_valid = v;
	}

	bool isValid()
	{
		return m_valid;
	}

	void setPosition(vxVector3d val)
	{
		m_position = val;
	}

	void setPosition(double x, double y, double z)
	{
		m_position.set(x,y,z);;
	}


	vxVector3d getPosition()
	{
		return m_position;
	}

	void setColor(vxVector3d val)
	{
		m_color = val;
	}

	void setColor(vxColor val)
	{
		m_color = val;
	}

	void setColor(int r, int g, int b)
	{
		m_color.set(r,g,b);
	}

	vxColor getColor()
	{
		return m_color;
	}

	void setNormal(vxVector3d val)
	{
		m_normal = val;
	}

	vxVector3d getNormal()
	{
		return m_normal;
	}

	void setU(double u)
	{
		this->m_u=u;
	}

	double getU()
	{
		return m_u;
	}

	void setV(double v)
	{
		this->m_v=v;
	}

	double getV()
	{
		return m_v;
	}


/*	vxCollision operator=(vxCollision otro)
	{
		vxCollision salida;
		otro.setPosition(getPosition());
		otro.setColor(getColor());
		otro.setNormal(getNormal());
		otro.setValid(isValid());
		return salida;
	}*/
};

}
#endif
