#ifndef _VXBOXMC_
#define _VXBOXMC_

#define APOT 1.5
#define SIDE 1
#define DIAG sqrt(pow(SIDE,2)*3)
#define MDIAG DIAG/2.0


#include "vxObject.h"
#include "vxVector.h"
#include "vxLight.h"
#include "vxShader.h"

#define PI 3.1415926535897932384626433832795

class vxVxl:public vxObject
{

protected:
	vxShader *m_shader;
	vxVector3d m_position;
	double m_size;

public:
	
	vxVxl();

	~vxVxl()
	{	
	}

	void setShader(vxShader *shader)
	{
		this->m_shader=shader;
	}

};


class vxBox:public vxVxl
{
private:
	
	vxVector3d m_point[8];
	void m_actualize();

public:
	vxBox()
	: vxVxl()
	{
		m_actualize();
	};

	vxBox(vxVector3d pos, double sze):vxVxl()
		{m_position=pos;m_size=sze;m_actualize();};

	vxBox(const double sx, const double sy, const double sz, const double sze):vxVxl()
		{m_position.set(sx,sy,sz);m_size=sze;m_actualize();};

	void set(vxVector3d pos, double sze) {m_position=pos;m_size=sze;m_actualize();};
	void set(double sx, double sy, double sz, double sze) 
	{
		m_position.set(sx,sy,sz);
		m_size=sze;
		m_actualize();
	};

	bool inSight(vxVector3d ray);
	bool inSightXY(vxVector3d ray);
	bool inSightYZ(vxVector3d ray);
	bool inSightZX(vxVector3d ray);

	vxCollision throwRay(vxVector3d ray);
	void throwXY(vxVector3d ray, vxCollision &collision);
	void throwYZ(vxVector3d ray, vxCollision &collision);
	void throwZX(vxVector3d ray, vxCollision &collision);

	bool inSighte(vxVector3d ray, vxVector3d &normal);
	bool inSighteXY(double c);
	bool inSighteYZ(double c);
	bool inSighteZX(double c);

};



// necesito avanzar, pero:
// repasar esta clase. derivadas... acceso a puntos...
class vxBoxN:public vxVxl
{
protected:

	double m_apot;
	double m_ps[6];
	bool m_bs[6];
	vxVector3d m_normals[6];

	// normal index
	
	int m_inormal;
	
public:

	vxBoxN():vxVxl()
	{
		setNormals();
		initialize();
		m_size=1;
		m_apot=.5;
	};

	vxBoxN(const vxVector3d pos, const double size):vxVxl()
	{
		setNormals();
		initialize();
		m_position=pos;
		setSize(size);
	}

	vxBoxN(const double x, const double y, const double z, const double size):vxVxl() 
	{
		setNormals();
		initialize();
		m_position.set(x,y,z);
		setSize(size);
	};

	void setNormals()
	{
		m_normals[0]=vxVector3d(  1 ,  0,  0);
		m_normals[1]=vxVector3d(  0 ,  1,  0);
		m_normals[2]=vxVector3d( -1 ,  0,  0);
		m_normals[3]=vxVector3d(  0 , -1,  0);
		m_normals[4]=vxVector3d(  0,   0, -1);
		m_normals[5]=vxVector3d(  0,   0,  1);
	}

	void set(const vxVector3d pos, const double size) 
	{
		initialize();
		m_position=pos;
		setSize(size);
	}
	
	void set(const double x, const double y, const double z, const double size)
	{
		initialize();
		m_position.set(x,y,z);
		setSize(size);
	}

	void setSize(double size)
	{
		this->m_size=size;
		m_apot=size/(double)2;
	}

	void initialize()
	{
		m_inormal=6;
		clearPoints();
	}

	void clearPoints()
	{
		m_bs[0]=m_bs[1]=m_bs[2]=m_bs[3]=m_bs[4]=m_bs[5]=false;
	}

	void setPosition(const vxVector3d pos)
	{
		m_position=pos;
	}

	void setPosition(const double x, const double y, const double z)
	{
		m_position.set(x,y,z);
	}

	virtual void throwRay(vxVector3d &ray, vxCollision &reted);

	// funcion de acceso, mas adelante cuestionar si seria, o no, mejor 
	// calcular la matriz siempre al principio, en un actualize.
	inline double getPoint0() {	return m_bs[0] ? m_ps[0] : m_bs[0]=true, m_ps[0]=m_position.getX()-(m_apot); }
	inline double getPoint1() {	return m_bs[1] ? m_ps[1] : m_bs[1]=true, m_ps[1]=m_position.getY()-(m_apot); }
	inline double getPoint2() {	return m_bs[2] ? m_ps[2] : m_bs[2]=true, m_ps[2]=m_position.getZ()-(m_apot); }
	inline double getPoint3() {	return m_bs[3] ? m_ps[3] : m_bs[3]=true, m_ps[3]=m_position.getX()+(m_apot); }
	inline double getPoint4() {	return m_bs[4] ? m_ps[4] : m_bs[4]=true, m_ps[4]=m_position.getY()+(m_apot); }
	inline double getPoint5() {	return m_bs[5] ? m_ps[5] : m_bs[5]=true, m_ps[5]=m_position.getZ()+(m_apot); }

	virtual bool throwSpace(vxVector3d &ray, vxCollision &collide) {return false;};
	virtual int frontSigth(vxVector3d &ray) {return 0;};
	virtual int topSigth(vxVector3d &ray) {return 0;};
	virtual int rightSigth(vxVector3d &ray) {return 0;};
		
};

class vxBoxN1:public vxBoxN
{
public:

	vxBoxN1():vxBoxN() {};

	vxBoxN1(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double mid = atan2(getPoint2(),getPoint1());
		double max = atan2(getPoint5(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint5());
		double mid = atan2(getPoint0(),getPoint2());
		double max = atan2(getPoint3(),getPoint2());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}
	
	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint3());
		double mid = atan2(getPoint1(),getPoint0());
		double max = atan2(getPoint4(),getPoint0());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			m_inormal = (a==1 && c==2) ? 2 : (b==2 && c==1) ? 1 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	
				case 1:
					{
						double t=getPoint0()/ray.getX();

						collide.setColor(0, 255, 0);
						collide.setNormal(m_normals[2]);
						collide.setPosition(getPoint0(),ray.getY()*t,ray.getZ()*t);

						collide.setU( (ray.getY() * t - getPoint1())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						
						return true;
					}
				case 2:
					{
						double t=getPoint1()/ray.getY();

						collide.setColor(255, 0, 255);
						collide.setNormal(m_normals[3]);
						collide.setPosition(ray.getX()*t,getPoint1(),ray.getZ()*t);

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						return true;
					}
				
				case 3:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );
						return true;
					}
			}
		
			
		}
		return false;
	}

};


class vxBoxN5:public vxBoxN
{
public:

	vxBoxN5():vxBoxN() {};

	vxBoxN5(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint5(),getPoint4());
		double mid = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint5());
		double mid = atan2(getPoint0(),getPoint2());
		double max = atan2(getPoint3(),getPoint2());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}
	
	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint0());
		double mid = atan2(getPoint4(),getPoint0());
		double max = atan2(getPoint4(),getPoint3());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			m_inormal = (a==2 && c==1) ? 2 : (b==2 && c==2) ? 1 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	
				case 1:
					{
						double t=getPoint0()/ray.getX();

						collide.setColor(0, 255, 0);
						collide.setNormal(m_normals[2]);
						collide.setPosition(getPoint0(),ray.getY()*t,ray.getZ()*t);

						collide.setU( (ray.getY() * t - getPoint1())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						
						return true;
					}
				case 2:
					{
						double t=getPoint4()/ray.getY();

						collide.setColor(0, 255, 255);
						collide.setNormal(m_normals[1]);
						collide.setPosition(ray.getX()*t,getPoint4(),ray.getZ()*t);

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );

						return true;

					}
				
				case 3:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());
												
						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );
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

	vxBoxN4():vxBoxN() {};

	vxBoxN4(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double mid = atan2(getPoint2(),getPoint1());
		double max = atan2(getPoint5(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint2());
		double mid = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint3(),getPoint5());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}
	
	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint4(),getPoint3());
		double mid = atan2(getPoint1(),getPoint3());
		double max = atan2(getPoint1(),getPoint0());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			m_inormal = (a==2 && c==2) ? 2 : (b==2 && c==1) ? 1 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	
				case 1:
					{
						double t=getPoint3()/ray.getX();

						collide.setColor(255, 255, 0);
						collide.setNormal(m_normals[0]);
						collide.setPosition(getPoint3(),ray.getY()*t,ray.getZ()*t);
						
						collide.setU( (ray.getY() * t - getPoint1())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						
						return true;
					}
				case 2:
					{
						double t=getPoint1()/ray.getY();

						collide.setColor(255, 0, 255);
						collide.setNormal(m_normals[3]);
						collide.setPosition(ray.getX()*t,getPoint1(),ray.getZ()*t);
												
						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						return true;
					}

				case 3:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());
												
						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );
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

	vxBoxN8():vxBoxN() {};

	vxBoxN8(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint5(),getPoint4());
		double mid = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint2());
		double mid = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint3(),getPoint5());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}
	
	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint4(),getPoint0());
		double mid = atan2(getPoint4(),getPoint3());
		double max = atan2(getPoint1(),getPoint3());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			m_inormal = (a==2 && c==2) ? 2 : (b==2 && c==1) ? 1 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	
				case 1:
					{
						double t=getPoint3()/ray.getX();

						collide.setColor(255, 255, 0);
						collide.setNormal(m_normals[0]);
						collide.setPosition(getPoint3(),ray.getY()*t,ray.getZ()*t);

						collide.setU( (ray.getY() * t - getPoint1())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						
						return true;
					}
				case 2:
					{
						double t=getPoint1()/ray.getY();

						collide.setColor(0, 255, 255);
						collide.setNormal(m_normals[1]);
						collide.setPosition(ray.getX()*t,getPoint1(),ray.getZ()*t);
						
						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						return true;
					}

				case 3:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());	
						
						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );
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

	vxBoxN12():vxBoxN() {};

	vxBoxN12(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double mid = atan2(getPoint2(),getPoint1());
		double max = atan2(getPoint5(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint0(),getPoint2());
		return (ang>max && ang<min) ?  1 : 0;
	}
	
	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint3());
		double max = atan2(getPoint1(),getPoint0());
		return (ang>min && ang<max) ?  1 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
	//		if (b=topSigth(ray)) // si lo ven pr
		{
			m_inormal = a==1 ? 4 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	
				case 3:
					{
						double t=getPoint1()/ray.getY();

						collide.setColor(255, 0, 255);
						collide.setNormal(m_normals[3]);
						collide.setPosition(ray.getX()*t,getPoint1(),ray.getZ()*t);

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						return true;
					}

				case 4:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );
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

	vxBoxN20():vxBoxN() {};

	vxBoxN20(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? 1 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint2());
		double mid = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint3(),getPoint5());
		return (ang>min && ang<max) ? ang>mid ? 2 : 1 : 0;
	} 

	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint3());
		double max = atan2(getPoint4(),getPoint3());
		return (ang<min || ang>max) ?  1 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		//if ((a=frontSigth(ray)) && (c=rightSigth(ray)))
		{
			m_inormal = b==1 ? 2 : 1;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	
				case 1:
					{
						double t=getPoint3()/ray.getX();

						collide.setColor(255, 255, 0);
						collide.setNormal(m_normals[0]);
						collide.setPosition(getPoint3(),ray.getY()*t,ray.getZ()*t);

						collide.setU( (ray.getY() * t - getPoint1())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						
						return true;
					}

				case 2:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());
												
						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );
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

	vxBoxN17():vxBoxN() {};

	vxBoxN17(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? 1 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint0(),getPoint5());
		double mid = atan2(getPoint0(),getPoint2());
		double max = atan2(getPoint3(),getPoint2());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}
	
	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint1(),getPoint0());
		double max = atan2(getPoint4(),getPoint0());
		return (ang>min && ang<max) ?  1 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

		if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		{
			m_inormal = b==1 ? 2 : 1;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	
				case 1:
					{
						double t=getPoint0()/ray.getX();

						collide.setColor(0, 255, 0);
						collide.setNormal(m_normals[2]);
						collide.setPosition(getPoint0(),ray.getY()*t,ray.getZ()*t);

						collide.setU( (ray.getY() * t - getPoint1())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );
						
						return true;
					}

				case 2:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );
						

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

	vxBoxN16():vxBoxN() {};

	vxBoxN16(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint5(),getPoint4());
		double mid = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? ang>mid ? 1 : 2 : 0;
	}

	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint0(),getPoint2());
		return (ang>max && ang<min) ?  1 : 0;
	}
	
	virtual int rightSigth(vxVector3d &ray) //z
	{
		double ang = ray.angleXY();
		double min = atan2(getPoint4(),getPoint3());
		double max = atan2(getPoint4(),getPoint0());
		return (ang>max && ang<min) ?  1 : 0;
	}
	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b,c;

	if ((a=frontSigth(ray)) && (b=topSigth(ray)) && (c=rightSigth(ray))) // si lo ven pr
		//		if ((a=frontSigth(ray)) && (b=topSigth(ray))) // si lo ven pr
		{
			m_inormal = a==1 ? 2 : 3;

			// por supuesto esto es solo para comprobar captura de normales.
			switch(m_inormal)
			{	

				case 2:
					{
						double t=getPoint2()/ray.getZ();

						collide.setColor(0, 0, 255);
						collide.setNormal(m_normals[4]);
						collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getY() * t - getPoint1())  / m_size );

						return true;
					}
				case 3:
					{
						double t=getPoint1()/ray.getY();

						collide.setColor(0, 255, 255);
						collide.setNormal(m_normals[1]);
						collide.setPosition(ray.getX()*t,getPoint1(),ray.getZ()*t);

						collide.setU( (ray.getX() * t - getPoint0())  / m_size );
						collide.setV( (ray.getZ() * t - getPoint2())  / m_size );

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

	vxBoxN24():vxBoxN() {};

	vxBoxN24(const double x, const double y, const double z, const double sze):vxBoxN(x, y, z, sze) {}

	virtual int frontSigth(vxVector3d &ray) //x
	{
		double ang = ray.angleYZ();
		double min = atan2(getPoint2(),getPoint4());
		double max = atan2(getPoint2(),getPoint1());
		return (ang>min && ang<max) ? 1 : 0;
	}


	virtual int topSigth(vxVector3d &ray) //y
	{
		double ang = ray.angleZX();
		double min = atan2(getPoint3(),getPoint2());
		double max = atan2(getPoint0(),getPoint2());
		return (ang>max && ang<min) ?  1 : 0;
	}

	
	bool throwSpace(vxVector3d &ray, vxCollision &collide)
	{
		int a,b;

		if ((a=frontSigth(ray)) && (b=topSigth(ray))) // si lo ven pr
		{
			double t=getPoint2()/ray.getZ();

			collide.setColor(0, 0, 255);
			collide.setNormal(m_normals[4]);
			collide.setPosition(ray.getX()*t,ray.getY()*t,getPoint2());

			collide.setU( (ray.getX() * t - getPoint0())  / m_size );
			collide.setV( (ray.getY() * t - getPoint1())  / m_size );

			return true;

		}
		return false;
	}

};


#endif
