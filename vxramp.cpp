#include "vxramp.h"
using namespace vxCore;

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

std::vector<scalar> vxRamp::values() const
{
	return m_values;
}

void vxRamp::setValues(const std::vector<scalar> &values)
{
	m_values = values;
}

