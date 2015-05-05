#include "vxramp.h"
using namespace vxStorage;

vxRamp::vxRamp()
{
}
Interpolation vxRamp::interpolation() const
{
	return m_interpolation;
}

void vxRamp::setInterpolation(const Interpolation &interpolation)
{
	m_interpolation = interpolation;
}

std::vector<double> vxRamp::values() const
{
	return m_values;
}

void vxRamp::setValues(const std::vector<double> &values)
{
	m_values = values;
}

