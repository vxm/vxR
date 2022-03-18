#ifndef VXRAMP_H
#define VXRAMP_H
#include<vector>
#include "Map.h"
#include "Global.h"
namespace vxCore {

class Ramp : public Map
{
	public:
		Ramp();

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
