#include "Ramp.h"
using namespace vxCore;

Ramp::Ramp() {}

Interpolation Ramp::interpolation() const { return m_interpolation; }

void Ramp::setInterpolation(const Interpolation &interpolation)
{
	m_interpolation = interpolation;
}

std::vector<scalar> Ramp::values() const { return m_values; }

void Ramp::setValues(const std::vector<scalar> &values) { m_values = values; }
