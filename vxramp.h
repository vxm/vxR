#ifndef VXRAMP_H
#define VXRAMP_H
#include<vector>
#include "vxMap.h"
#include "vxGlobal.h"
namespace vxCore {

class vxRamp : public vxMap
{
	public:
		vxRamp();

	std::vector<scalar> m_values;
	std::vector<scalar> m_colors;
	Interpolation m_interpolation;
	
	std::vector<scalar> values() const;
	Interpolation interpolation() const;
	
	void setValues(const std::vector<scalar> &values);
	void setInterpolation(const Interpolation &interpolation);
};

}
#endif // VXRAMP_H
