#include "vxmatrix.h"
#include <utility>

vxMatrix::vxMatrix()
{}

vxMatrix::vxMatrix(const vxMatrix&src)
{}

vxMatrix::vxMatrix(const double m[16])
{}

vxMatrix::~vxMatrix()
{}

double vxMatrix::operator()(unsigned int row, unsigned int col) const
{
	double t = m_matrix[4*row+col];
	return t;
}

const double *vxMatrix::operator[](unsigned int row) const
{
	return &m_matrix[row*4];
}

vxStatus::code vxMatrix::get(double dest[]) const
{
	
	vxStatus::code::success;
}

vxStatus::code vxMatrix::get(float dest[]) const
{
	
	vxStatus::code::success;
}

vxMatrix vxMatrix::transpose() const
{
	std::swap(m_matrix[1],m_matrix[4]);
	std::swap(m_matrix[2],m_matrix[8]);
	std::swap(m_matrix[3],m_matrix[12]);
	
	std::swap(m_matrix[6],m_matrix[9]);
	std::swap(m_matrix[7],m_matrix[13]);

	std::swap(m_matrix[11],m_matrix[15]);

	return *this;
}

vxMatrix &vxMatrix::setToIdentity()
{
	
	return *this;
}

vxMatrix &vxMatrix::setToProduct(const vxMatrix &left, const vxMatrix &right)
{
	
	return *this;
}

vxMatrix &vxMatrix::operator+=(const vxMatrix &right)
{
	
	return *this;
}

vxMatrix vxMatrix::operator+(const vxMatrix &right) const
{
	
	return *this;
}

vxMatrix &vxMatrix::operator-=(const vxMatrix &right)
{
	
	return *this;
}

vxMatrix vxMatrix::operator-(const vxMatrix &right) const
{
	
	return *this;
}

vxMatrix &vxMatrix::operator*=(const vxMatrix &right)
{
	
	return *this;
}

vxMatrix vxMatrix::operator*(const vxMatrix &right) const
{
	
	return *this;
}

vxMatrix &vxMatrix::operator*=(double)
{
	
	return *this;
}

vxMatrix vxMatrix::operator*(double) const
{
	
	return *this;
}

bool vxMatrix::operator==(const vxMatrix &other) const
{
	
}

bool vxMatrix::operator!=(const vxMatrix &other) const
{
	
}

vxMatrix vxMatrix::inverse() const
{
	
}

vxMatrix vxMatrix::adjoint() const
{
	
}

vxMatrix vxMatrix::homogenize() const
{
	
}

double vxMatrix::det4x4() const
{
	
}

double vxMatrix::det3x3() const
{
	
}

bool vxMatrix::isEquivalent(const vxMatrix &other, double tolerance) const
{
	
}

bool vxMatrix::isSingular() const
{
	
}

double &vxMatrix::operator()(unsigned int row, unsigned int col)
{
	
}

double *vxMatrix::operator[](unsigned int row)
{
	
}
