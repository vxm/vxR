#ifndef VXCOLLISION_H
#define VXCOLLISION_H

#include<vxObject.h>
#include<vxPoint.h>

namespace vxStorage {


class vxCollision:public vxObject
{
private:

	vxPoint m_position;
	vxColor m_color;
	vxVector3d m_normal;

	bool m_valid {false};
	double m_u {0.0};
	double m_v {0.0};

public:

	vxCollision()
	{
	};

	void initialize()
	{
		setValid(false);
		m_position.set(0,0,0);
		m_color.set(0,0,0);
		m_normal.set(0,0,0);
		m_u=0.0;
		m_v=0.0;
	}


	void setValid(bool v = true)
	{
		m_valid = v;
	}

	bool isValid() const
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

	vxVector3d getPosition() const
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

	vxColor getColor() const
	{
		return m_color;
	}

	void setNormal(vxVector3d val)
	{
		m_normal = val;
	}

	vxVector3d getNormal() const
	{
		return m_normal;
	}

	void setU(double u)
	{
		this->m_u=u;
	}

	double getU() const
	{
		return m_u;
	}

	void setV(double v)
	{
		this->m_v=v;
	}

	double getV() const
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


}// vxStorage namespace.
#endif // VXCOLLISION_H
