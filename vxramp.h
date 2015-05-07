#ifndef VXRAMP_H
#define VXRAMP_H
#include<vector>
#include "vxMap.h"
#include "vxGlobal.h"
namespace vxStorage {

class vxRamp : public vxMap
{
	public:
		vxRamp();

	std::vector<double> m_values;
	std::vector<double> m_colors;
	Interpolation m_interpolation;
	
	std::vector<double> values() const;
	Interpolation interpolation() const;
	
	void setValues(const std::vector<double> &values);
	void setInterpolation(const Interpolation &interpolation);
};

}
#endif // VXRAMP_H
