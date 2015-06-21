#ifndef _VXBOXMC_
#define _VXBOXMC_

#include <memory>

#include "vxObject.h"
#include "vxVector.h"
#include "vxShader.h"

namespace vxCore {

#define APOT 1.5
#define SIDE 1
#define DIAG sqrt(pow(SIDE,2)*3)
#define MDIAG DIAG/2.0
#define PI 3.1415926535897932384626433832795


class vxBox
{
private:
	
	double m_size {1.0};
	vxVector3d m_position;
	vxVector3d m_point[8];
	void actualize();
	
public:
	vxBox()
	{
		actualize();
	};

	vxBox(const vxVector3d &pos, double sze)
		{m_position=pos;m_size=sze;actualize();};

	vxBox(const double sx, const double sy, const double sz, const double sze)
		{m_position.set(sx,sy,sz);m_size=sze;actualize();};

	void set(const vxVector3d &pos, double sze) {m_position=pos;m_size=sze;actualize();};
	void set(double sx, double sy, double sz, double sze) 
	{
		m_position.set(sx,sy,sz);
		m_size=sze;
		actualize();
	};

	bool inSight(const vxRayXYZ &ray);
	bool inSightXY(const vxRayXYZ &ray);
	bool inSightYZ(const vxRayXYZ &ray);
	bool inSightZX(const vxRayXYZ &ray);

	virtual int throwRay(const vxRayXYZ &ray, vxCollision &collision) const;
	virtual void throwXY(const vxRayXYZ &ray, vxCollision &collision) const = 0;
	virtual void throwYZ(const vxRayXYZ &ray, vxCollision &collision) const = 0;
	virtual void throwZX(const vxRayXYZ &ray, vxCollision &collision) const = 0;

	bool inSighte(const vxRayXYZ &ray, vxVector3d &normal);
	bool inSighteXY(double c);
	bool inSighteYZ(double c);
	bool inSighteZX(double c);
};



// necesito avanzar, pero:
// repasar esta clase. derivadas... acceso a puntos...
class vxBoxN
{
protected:

	double m_size {1.0};
	vxVector3d m_position;
	double m_apot;
	double m_ps[6];
	//TODO:question this variable, is really needed?
	bool m_bs[6];
	static vxVector3d m_normals[6];

public:
	vxBoxN();
	vxBoxN(const vxVector3d pos, const double size);
	vxBoxN(const double x, const double y, const double z, const double size);;

	void set(const vxVector3d pos, const double size);
	void set(const double x, const double y, const double z, const double size);
	void setSize(double size);
	void initialize();
	void clearPoints();
	void setPosition(const vxVector3d pos);
	void setPosition(const double x, const double y, const double z);
	virtual int throwRay(const vxRayXYZ &ray, vxCollision &collision) const;
	
	// funcion de acceso, mas adelante cuestionar si seria, o no, mejor 
	// calcular la matriz siempre al principio, en un actualize.
	double getPoint0() const;
	double getPoint1() const;
	double getPoint2() const;
	double getPoint3() const;
	double getPoint4() const;
	double getPoint5() const;

	virtual bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const =0;

	virtual int frontSigth(const vxRayXYZ &ray) const =0;
	virtual int topSigth(const vxRayXYZ &ray) const =0;
	virtual int rightSigth(const vxRayXYZ &ray) const =0;
		
};

class vxBoxN1:public vxBoxN
{
public:

	vxBoxN1()
		:vxBoxN() {};

	vxBoxN1(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override  //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double mid = atan2(getPoint2(),getPoint1());
		double max = atan2(getPoint5(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint5());
		double mid = atan2(getPoint0(),getPoint2());
		double max = atan2(getPoint3(),getPoint2());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}
	
	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint3());
		double mid = atan2(getPoint1(),getPoint0());
		double max = atan2(getPoint4(),getPoint0());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		unsigned int a = frontSigth(ray);
		if(a==0)
			return false;
		
		unsigned int b = topSigth(ray);
		if(b==0)
			return false;
		
		unsigned int c = rightSigth(ray);
		if(c==0)
			return false;
		
		auto inormal = (a==1 && c==2) ? 2 : (b==2 && c==1) ? 1 : 3;

		// por supuesto esto es solo para comprobar captura de normales.
		switch(inormal)
		{	
			case 1:
			{
				double t=getPoint0()/ray.x();

				collide.setColor(vxColor::lookup(0, 1.0, 0));
				collide.setNormal(m_normals[2]);
				collide.setPosition(getPoint0(),ray.y()*t,ray.z()*t);

				collide.setU( (ray.y() * t - getPoint1())  / m_size );
				collide.setV( (ray.z() * t - getPoint2())  / m_size );
				
				return true;
			}
			case 2:
			{
				double t=getPoint1()/ray.y();

				collide.setColor(vxColor::lookup(1.0, 0, 0));
				collide.setNormal(m_normals[3]);
				collide.setPosition(ray.x()*t,getPoint1(),ray.z()*t);

				collide.setU( (ray.x() * t - getPoint0())  / m_size );
				collide.setV( (ray.z() * t - getPoint2())  / m_size );
				return true;
			}
			case 3:
			{
				double t=getPoint2()/ray.z();

				collide.setColor(vxColor::lookup(0, 0, 1.0));
				collide.setNormal(m_normals[4]);
				collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());

				collide.setU( (ray.x() * t - getPoint0())  / m_size );
				collide.setV( (ray.y() * t - getPoint1())  / m_size );
				return true;
			}
		}

		return false;
	}

};


class vxBoxN5:public vxBoxN
{
public:

	vxBoxN5()
		:vxBoxN() {};

	vxBoxN5(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override//x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint5(),getPoint4());
		double mid = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint5());
		double mid = atan2(getPoint0(),getPoint2());
		double max = atan2(getPoint3(),getPoint2());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}
	
	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint0());
		double mid = atan2(getPoint4(),getPoint0());
		double max = atan2(getPoint4(),getPoint3());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			auto inormal = (a==2 && c==1) ? 2 : (b==2 && c==2) ? 1 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(inormal)
			{	
				case 1:
					{
						double t=getPoint0()/ray.x();

						collide.setColor(vxColor::lookup(0, 1.0, 0));
						collide.setNormal(m_normals[2]);
						collide.setPosition(getPoint0(),ray.y()*t,ray.z()*t);

						collide.setU( (ray.y() * t - getPoint1())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						
						return true;
					}
				case 2:
					{
						double t=getPoint4()/ray.y();

						collide.setColor(vxColor::lookup(1.0, 0, 0));
						collide.setNormal(m_normals[1]);
						collide.setPosition(ray.x()*t,getPoint4(),ray.z()*t);

						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );

						return true;

					}
				
				case 3:
					{
						double t=getPoint2()/ray.z();

						collide.setColor(vxColor::lookup(0, 0, 1.0));
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());
												
						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.y() * t - getPoint1())  / m_size );
						return true;
					}
			}
		
			
		}
		return false;
	}

};




class vxBoxN4:public vxBoxN
{
public:

	vxBoxN4()
		:vxBoxN() {};

	vxBoxN4(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double mid = atan2(getPoint2(),getPoint1());
		double max = atan2(getPoint5(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint2());
		double mid = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint3(),getPoint5());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}
	
	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint4(),getPoint3());
		double mid = atan2(getPoint1(),getPoint3());
		double max = atan2(getPoint1(),getPoint0());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}

	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			auto inormal = (a==2 && c==2) ? 2 : (b==2 && c==1) ? 1 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(inormal)
			{	
				case 1:
					{
						double t=getPoint3()/ray.x();

						collide.setColor(vxColor::lookup(1.0, 1.0, 0));
						collide.setNormal(m_normals[0]);
						collide.setPosition(getPoint3(),ray.y()*t,ray.z()*t);
						
						collide.setU( (ray.y() * t - getPoint1())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						
						return true;
					}
				case 2:
					{
						double t=getPoint1()/ray.y();

						collide.setColor(vxColor::lookup(1.0, 0, 1.0));
						collide.setNormal(m_normals[3]);
						collide.setPosition(ray.x()*t,getPoint1(),ray.z()*t);
												
						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						return true;
					}

				case 3:
					{
						double t=getPoint2()/ray.z();

						collide.setColor(vxColor::lookup(0, 0, 1.0));
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());
												
						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.y() * t - getPoint1())  / m_size );
						return true;
					}
			}
		}
		return false;
	}

};


class vxBoxN8:public vxBoxN
{
public:

	vxBoxN8()
		:vxBoxN() {};

	vxBoxN8(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint5(),getPoint4());
		double mid = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint2());
		double mid = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint3(),getPoint5());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}
	
	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint4(),getPoint0());
		double mid = atan2(getPoint4(),getPoint3());
		double max = atan2(getPoint1(),getPoint3());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b,c;
//previ
		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			auto inormal = (a==2 && c==2) ? 2 : (b==2 && c==1) ? 1 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(inormal)
			{	
				case 1:
					{
						double t=getPoint3()/ray.x();

						collide.setColor(vxColor::lookup(1.0, 100, 23));
						collide.setNormal(m_normals[0]);
						collide.setPosition(getPoint3(),ray.y()*t,ray.z()*t);

						collide.setU( (ray.y() * t - getPoint1())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						
						return true;
					}
				case 2:
					{
						double t=getPoint1()/ray.y();

						collide.setColor(vxColor::lookup(1.0, 0, 0));
						collide.setNormal(m_normals[1]);
						collide.setPosition(ray.x()*t,getPoint1(),ray.z()*t);
						
						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						return true;
					}

				case 3:
					{
						double t=getPoint2()/ray.z();

						collide.setColor(vxColor::lookup(0, 0, 1.0));
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());	
						
						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.y() * t - getPoint1())  / m_size );
						return true;
					}
			}
		}
		return false;
	}

};


class vxBoxN12:public vxBoxN
{
public:

	vxBoxN12()
		:vxBoxN() {};

	vxBoxN12(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double mid = atan2(getPoint2(),getPoint1());
		double max = atan2(getPoint5(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint0(),getPoint2());
		return (ang>max && ang<min) ?  1 : 0;
	}
	
	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint3());
		double max = atan2(getPoint1(),getPoint0());
		return (ang>min && ang<max) ?  1 : 0;
	}

	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
	//		if (b=topSigth(ray)) // si lo ven pr
		{
			auto inormal = a==1 ? 4 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(inormal)
			{	
				case 3:
					{
						double t=getPoint1()/ray.y();

						collide.setColor(vxColor::lookup(1.0, 0, 1.0));
						collide.setNormal(m_normals[3]);
						collide.setPosition(ray.x()*t,getPoint1(),ray.z()*t);

						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						return true;
					}

				case 4:
					{
						double t=getPoint2()/ray.z();

						collide.setColor(vxColor::lookup(0, 0, 1.0));
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());

						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.y() * t - getPoint1())  / m_size );
						return true;
					}
			}
		}
		return false;
	}

};


class vxBoxN20:public vxBoxN
{
public:

	vxBoxN20()
		:vxBoxN() {};

	vxBoxN20(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? 1 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint2());
		double mid = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint3(),getPoint5());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	} 

	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint3());
		double max = atan2(getPoint4(),getPoint3());
		return (ang<min || ang>max) ?  1 : 0;
	}

	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		//if ((a=frontSigth(ray)) && (c=rightSigth(ray)))
		{
			auto inormal = b==1 ? 2 : 1;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(inormal)
			{	
				case 1:
					{
						double t=getPoint3()/ray.x();

						collide.setColor(vxColor::lookup(1.0, 1.0, 0));
						collide.setNormal(m_normals[0]);
						collide.setPosition(getPoint3(),ray.y()*t,ray.z()*t);

						collide.setU( (ray.y() * t - getPoint1())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						
						return true;
					}

				case 2:
					{
						double t=getPoint2()/ray.z();

						collide.setColor(vxColor::lookup(0, 0, 1.0));
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());
												
						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.y() * t - getPoint1())  / m_size );
						return true;
					}
			}
		
			
		}
		return false;
	}

};


class vxBoxN17:public vxBoxN
{
public:

	vxBoxN17()
		:vxBoxN() {};

	vxBoxN17(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? 1 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint5());
		double mid = atan2(getPoint0(),getPoint2());
		double max = atan2(getPoint3(),getPoint2());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}
	
	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint0());
		double max = atan2(getPoint4(),getPoint0());
		return (ang>min && ang<max) ?  1 : 0;
	}

	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			auto inormal = b==1 ? 2 : 1;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(inormal)
			{	
				case 1:
					{
						double t=getPoint0()/ray.x();

						collide.setColor(vxColor::lookup(0, 1.0, 0));
						collide.setNormal(m_normals[2]);
						collide.setPosition(getPoint0(),ray.y()*t,ray.z()*t);

						collide.setU( (ray.y() * t - getPoint1())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );
						
						return true;
					}

				case 2:
					{
						double t=getPoint2()/ray.z();

						collide.setColor(vxColor::lookup(0, 0, 1.0));
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());

						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.y() * t - getPoint1())  / m_size );
						
						return true;
					}
			}
		}
		return false;
	}

};

class vxBoxN16:public vxBoxN
{
public:

	vxBoxN16()
		:vxBoxN() {};

	vxBoxN16(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint5(),getPoint4());
		double mid = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint0(),getPoint2());
		return (ang>max && ang<min) ?  1 : 0;
	}
	
	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint4(),getPoint3());
		double max = atan2(getPoint4(),getPoint0());
		return (ang>max && ang<min) ?  1 : 0;
	}
	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		//		if ((a=frontSigth(ray)) && (b=topSigth(ray))) // si lo ven pr
		{
			auto inormal = a==1 ? 2 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(inormal)
			{	

				case 2:
					{
						double t=getPoint2()/ray.z();

						collide.setColor(vxColor::lookup(0, 0, 1.0));
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());

						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.y() * t - getPoint1())  / m_size );

						return true;
					}
				case 3:
					{
						double t=getPoint1()/ray.y();

						collide.setColor(vxColor::lookup(1.0, 0, 0));
						collide.setNormal(m_normals[1]);
						collide.setPosition(ray.x()*t,getPoint1(),ray.z()*t);

						collide.setU( (ray.x() * t - getPoint0())  / m_size );
						collide.setV( (ray.z() * t - getPoint2())  / m_size );

						return true;
					}
			}
		}
		return false;
	}

};


class vxBoxN24:public vxBoxN
{
public:

	vxBoxN24()
		:vxBoxN() {};

	vxBoxN24(const double x, const double y, const double z, const double sze)
		:vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(const vxRayXYZ &ray) const override //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? 1 : 0;
	}


	virtual int topSigth(const vxRayXYZ &ray) const override//y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint0(),getPoint2());
		return (ang>max && ang<min) ?  1 : 0;
	}

	virtual int rightSigth(const vxRayXYZ &ray) const override//z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint4(),getPoint3());
		double max = atan2(getPoint4(),getPoint0());
		return (ang>max && ang<min) ?  1 : 0;
	}
	
	bool throwSpace(const vxRayXYZ &ray, vxCollision &collide) const override
	{
		int a,b;

		if ((a=frontSigth(ray)) && (b=topSigth(ray))) // si lo ven pr
		{
			double t=getPoint2()/ray.z();

			collide.setColor(vxColor::lookup(0, 0, 1.0));
			collide.setNormal(m_normals[4]);
			collide.setPosition(ray.x()*t,ray.y()*t,getPoint2());

			collide.setU( (ray.x() * t - getPoint0())  / m_size );
			collide.setV( (ray.y() * t - getPoint1())  / m_size );

			return true;

		}
		return false;
	}

};

}

#endif
