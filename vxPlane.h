#ifndef VXPLANE_H
#define VXPLANE_H

class vxPlane
{
	double m_x {0.0};
	double m_y {0.0};
	double m_z {0.0};
	double m_d {0.0};
	
	public:

		enum class type
		{
			kX,					// x is constant
			kY,					// y is constant
			kZ,					// z is constant
			kX0,				// x is 0
			kY0,				// y is 0
			kZ0,				// z is 0
			kFree				// any set of values
		};
	
	type m_type;
	
	vxPlane();
	
	vxPlane(type pType)
		:m_type(pType)
	{
		
	}
	
	vxPlane(double x,
			double y = 0.0,
			double z = 0.0,
			double d = 0.0)
		: m_x(x)
		, m_y(y)
		, m_z(z)
		, m_d(d)
	{
		
	}
	
	double x() const;
	void setX(double x);

	double y() const;
	void setY(double y);

	double z() const;
	void setZ(double z);

	double d() const;
	void setD(double d);
};

#endif // VXPLANE_H