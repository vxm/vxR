#include "vxmatrix.h"
#include <utility>
#include <cstring>

vxMatrix::vxMatrix()
{}

vxMatrix::vxMatrix(const vxMatrix&src)
{
	memcpy(m_matrix, src.m_matrix, 16);
}

vxMatrix::vxMatrix(const double m[16])
{
	memcpy(m_matrix, m, 16);
}

vxMatrix::~vxMatrix()
{}

double vxMatrix::operator()(unsigned int row, unsigned int col) const
{
	return (m_matrix[4*row+col]);
}

const double *vxMatrix::operator[](unsigned int row) const
{
	return &m_matrix[row*4];
}

vxStatus::code vxMatrix::get(double dest[]) const
{
	memcpy(dest, m_matrix, 16);
	return vxStatus::code::success;
}

vxStatus::code vxMatrix::get(float dest[]) const
{
	//!memcpy of doubles to floats?
	memcpy(dest, m_matrix, 16);
	return vxStatus::code::success;
}

vxMatrix vxMatrix::transpose() const
{
	std::swap(m_matrix[1],	m_matrix[4]);
	std::swap(m_matrix[2],	m_matrix[8]);
	std::swap(m_matrix[3],	m_matrix[12]);
	std::swap(m_matrix[6],	m_matrix[9]);
	std::swap(m_matrix[7],	m_matrix[13]);
	std::swap(m_matrix[11],	m_matrix[15]);

	return *this;
}

vxMatrix &vxMatrix::setToIdentity()
{
	m_matrix[0] = 1.0;
	m_matrix[1] = 0.0;
	m_matrix[2] = 0.0;
	m_matrix[3] = 0.0;
	
	m_matrix[4] = 0.0;
	m_matrix[5] = 1.0;
	m_matrix[6] = 0.0;
	m_matrix[7] = 0.0;
	
	m_matrix[8] = 0.0;
	m_matrix[9] = 0.0;
	m_matrix[10] = 1.0;
	m_matrix[11] = 0.0;
	
	m_matrix[12] = 0.0;
	m_matrix[13] = 0.0;
	m_matrix[14] = 0.0;
	m_matrix[15] = 1.0;
	
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
	
	return false;
}

bool vxMatrix::operator!=(const vxMatrix &other) const
{
	
	return true;
}

vxMatrix vxMatrix::inverse() const
{
	
	return *this;
}

vxMatrix vxMatrix::adjoint() const
{
	
	return *this;
}

vxMatrix vxMatrix::homogenize() const
{
	
	return *this;
}

double vxMatrix::det4x4() const
{
	
	return 1.0;
}

double vxMatrix::det3x3() const
{
	
	return 1.0;
}

bool vxMatrix::isEquivalent(const vxMatrix &other, double tolerance) const
{
	bool eq{false};
			
	return eq;
}

bool vxMatrix::isSingular() const
{
	
	return true;
}

double &vxMatrix::operator()(unsigned int row, unsigned int col)
{
	return (m_matrix[4*row+col]);
}

double *vxMatrix::operator[](unsigned int row)
{
	return &m_matrix[4*row];
}
