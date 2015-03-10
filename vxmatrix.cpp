#include "vxmatrix.h"
#include <utility>
#include <cstring>

const vxMatrix vxMatrix::identity{  1.0, 0.0, 0.0, 0.0, 
									0.0, 1.0, 0.0, 0.0, 
									0.0, 0.0, 1.0, 0.0, 
									0.0, 0.0, 0.0, 1.0};

vxMatrix::vxMatrix()
{}

vxMatrix::vxMatrix(const vxMatrix &src)
{
	memcpy(m_matrix, src.m_matrix, 16 * sizeof(double));
}

vxMatrix::vxMatrix(const double m[16])
{
	memcpy(m_matrix, m, 16 * sizeof(double));
}

vxMatrix::vxMatrix (std::initializer_list<double> list)
{
	auto i=0;
	for(auto it = std::begin(list); it!= std::end(list);++it)
	{
		m_matrix[i] = *it;
		++i;
	}
}

vxMatrix::~vxMatrix()
{
	// array will be destructed by itself
}

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
	memcpy(dest, m_matrix, 16 * sizeof(double));
	return vxStatus::code::success;
}

vxStatus::code vxMatrix::get(float dest[]) const
{
	//!memcpy of doubles to floats?
	memcpy(dest, m_matrix, 16 * sizeof(double));
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
	this->operator=(vxMatrix::identity);
	
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
	const vxMatrix &a=right.m_matrix;
	const vxMatrix &b=m_matrix;
	
	mdata m{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	m[0] = a(0,0) * b(0,0) + a(0,1) * b(0,0) + a(0,2) * b(0,0) + a(0,3) * b(0,0);
	m[1] = a(1,0) * b(0,0) + a(1,1) * b(0,0) + a(1,2) * b(0,0) + a(1,3) * b(0,0);
	m[2] = a(2,0) * b(0,0) + a(2,1) * b(0,0) + a(2,2) * b(0,0) + a(2,3) * b(0,0);
	m[3] = a(3,0) * b(0,0) + a(3,1) * b(0,0) + a(3,2) * b(0,0) + a(3,3) * b(0,0);

	m[4] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[5] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[6] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[7] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);

	m[8] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[9] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[10] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[11] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);

	m[12] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[13] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[14] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	m[15] = a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0) + a(0,0) * b(0,0);
	
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
	auto r = memcmp(m_matrix, other.m_matrix, 16 * sizeof(double));
	return r==0;
}

bool vxMatrix::operator!=(const vxMatrix &other) const
{
	auto r = memcmp(m_matrix, other.m_matrix, 16 * sizeof(double));
	return r!=0;
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
