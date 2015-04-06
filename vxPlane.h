#ifndef VXPLANE_H
#define VXPLANE_H

class vxPlane
{
	public:

	vxPlane();
	
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
	
	double m_x {0.0};
	double m_y {0.0};
	double m_z {0.0};
	double m_d {0.0};
};

#endif // VXPLANE_H
