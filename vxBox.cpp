#include "vxBox.h"
#include "vxGlobal.h"
#include <map>
namespace vxCore {

vxVector3d vxBoxN::m_normals[6] =  {vxVector3d( 1,  0,  0),
									vxVector3d( 0,  1,  0),
									vxVector3d(-1,  0,  0),
									vxVector3d( 0, -1,  0),
									vxVector3d( 0,  0, -1),
									vxVector3d( 0,  0,  1)};


void vxBox::actualize()
{
	auto ms=m_size/2.0;
	double x,y,z;

	m_position.get(x,y,z);

	m_point[0].set(x+ms,y+ms,z+ms);
	m_point[1].set(x-ms,y+ms,z+ms);
	m_point[2].set(x-ms,y-ms,z+ms);
	m_point[3].set(x+ms,y-ms,z+ms);

	m_point[4].set(x+ms,y+ms,z-ms);
	m_point[5].set(x-ms,y+ms,z-ms);
	m_point[6].set(x-ms,y-ms,z-ms);
	m_point[7].set(x+ms,y-ms,z-ms);
}


bool vxBox::inSighteXY(double c)
{
	double a=m_point[1].angleXY(),b=m_point[2].angleXY();
	bool ret=(b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[2].angleXY();
	b=m_point[3].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[3].angleXY();
	b=m_point[0].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[0].angleXY();
	b=m_point[1].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	return ret;
}

bool vxBox::inSighteYZ(double c)
{
	double a=m_point[4].angleXY(),b=m_point[0].angleXY();
	bool ret=(b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[0].angleXY();
	b=m_point[3].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[3].angleXY();
	b=m_point[7].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[7].angleXY();
	b=m_point[4].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	return ret;
}

bool vxBox::inSighteZX(double c)
{
	double a=m_point[1].angleXY(),b=m_point[0].angleXY();
	bool ret=(b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[0].angleXY();
	b=m_point[4].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[4].angleXY();
	b=m_point[5].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	a=m_point[5].angleXY();
	b=m_point[1].angleXY();
	ret=ret || (b+PI<a || a+PI<b) ? (a<b ? (c>b || c<a) : (c>a || c<b)) : (a<b ? (c>=a && c<=b) : (c>=b && c<=a));
	return ret;
}

bool vxBox::inSighte(const vxRayXYZ &ray, vxVector3d &normal)
{
	normal.set(0,1,0);
	return (inSighteXY(ray.angleXY()) && inSighteYZ(ray.angleYZ()) && inSighteZX(ray.angleZX()));
}


bool vxBox::inSightXY(const vxRayXYZ &ray)
{
	double mina, maxa;
	double ang;
	maxa=mina=m_point[0].angleXY();
	if((ang=m_point[1].angleXY())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	if((ang=m_point[2].angleXY())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	if((ang=m_point[3].angleXY())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	
	if (mina+PI<maxa) //qtb
	{	
		if (m_point[0].x()>0) return true;
		else return (ray.angleXY()>m_point[0].angleXY() || ray.angleXY()<m_point[3].angleXY());
	}
	else
	{	
		return (ray.angleXY()<maxa && ray.angleXY()>mina);
	}
}

bool vxBox::inSightYZ(const vxRayXYZ &ray)
{
	double mina, maxa;
	double ang;
	maxa=mina=m_point[0].angleYZ();
	if((ang=m_point[3].angleYZ())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	if((ang=m_point[4].angleYZ())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	if((ang=m_point[7].angleYZ())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	
	if (mina+PI<maxa) //qtb
	{	
		if (m_point[0].y()>0) return true;
		else return (ray.angleYZ()>m_point[0].angleYZ() || ray.angleYZ()<m_point[4].angleYZ());
	}
	else
	{	
		return (ray.angleYZ()<maxa && ray.angleYZ()>mina);
	}
}

bool vxBox::inSightZX(const vxRayXYZ &ray)
{
	double mina, maxa;
	double ang;
	maxa=mina=m_point[0].angleZX();
	if((ang=m_point[1].angleZX())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	if((ang=m_point[4].angleZX())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	if((ang=m_point[5].angleZX())<mina) mina=ang;
	if(ang>maxa) maxa=ang;
	
	if (mina+PI<maxa) //qtb
	{	
		if (m_point[0].x()>0) return true;
		else return (ray.angleZX()>m_point[0].angleZX() || ray.angleZX()<m_point[3].angleZX());
	}
	else
	{	
		return (ray.angleZX()<maxa && ray.angleZX()>mina);
	}
}

bool vxBox::inSight(const vxRayXYZ &ray)
{
	return (inSightXY(ray) && inSightYZ(ray) && inSightZX(ray));
}

vxBoxN::vxBoxN()
{
	initialize();
	m_size=1;
	m_apot=.5;
}

vxBoxN::vxBoxN(const vxVector3d pos, const double size)
{
	initialize();
	m_position=pos;
	setSize(size);
}

vxBoxN::vxBoxN(const double x, const double y, const double z, const double size) 
{
	initialize();
	m_position.set(x,y,z);
	setSize(size);
}

void vxBoxN::set(const vxVector3d pos, const double size) 
{
	initialize();
	m_position=pos;
	setSize(size);
}

void vxBoxN::set(const double x, const double y, const double z, const double size)
{
	initialize();
	m_position.set(x,y,z);
	setSize(size);
}

void vxBoxN::setSize(double size)
{
	m_size=size;
	m_apot=size/2.0;
}

void vxBoxN::initialize()
{
	clearPoints();
}

void vxBoxN::clearPoints()
{
	m_bs[0]=m_bs[1]=m_bs[2]=m_bs[3]=m_bs[4]=m_bs[5]=false;
}

void vxBoxN::setPosition(const vxVector3d pos)
{
	m_position=pos;
}

void vxBoxN::setPosition(const double x, const double y, const double z)
{
	m_position.set(x,y,z);
}

int vxBoxN::throwRay(const vxRayXYZ &ray, vxCollision &collide) const
{
	//if (true)
	if (throwSpace(ray, collide))
	{
		collide.setValid(true);
		return 1;
	}

	collide.setValid(false);
	return 0;
}

double vxBoxN::getPoint0() const
{
	return m_position.x()-m_apot;
}

double vxBoxN::getPoint1() const
{
	return m_position.y()-(m_apot);
}

double vxBoxN::getPoint2() const
{
	return m_position.z()-(m_apot); 
}

double vxBoxN::getPoint3() const
{
	return m_position.x()+(m_apot); 
}

double vxBoxN::getPoint4() const
{
	return m_position.y()+(m_apot); 
}

double vxBoxN::getPoint5() const
{
	return m_position.z()+(m_apot); 
}



} 
