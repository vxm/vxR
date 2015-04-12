#include "vxBox.h"
#include "vxGlobal.h"
#include <map>
namespace vxStorage {

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

bool vxBox::inSighte(const vxVector3d &ray, vxVector3d &normal)
{
	normal.set(0,1,0);
	return (inSighteXY(ray.angleXY()) && inSighteYZ(ray.angleYZ()) && inSighteZX(ray.angleZX()));
}


bool vxBox::inSightXY(const vxVector3d &ray)
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

bool vxBox::inSightYZ(const vxVector3d &ray)
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

bool vxBox::inSightZX(const vxVector3d &ray)
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

bool vxBox::inSight(const vxVector3d &ray)
{
	return (inSightXY(ray) && inSightYZ(ray) && inSightZX(ray));
}


void vxBoxN::throwRay(const vxVector3d &ray, vxCollision &collide)
{
	collide.initialize();

	//if (true)
	if (throwSpace(ray, collide))
	{
		m_shader->getColor(collide, .6);
		collide.setValid(true);
	}
	else// background
	{
		collide.setColor(1.0,0,0);
		collide.setValid(false);
	}

}

vxVxl::vxVxl()
	: m_size(1)
{ 
	m_shader = vxGlobal::getLambert();
}



}
