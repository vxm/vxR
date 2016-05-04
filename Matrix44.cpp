#include <cstring>
#include <utility>

#include "Matrix44.h"

using namespace vxCore;


const Matrix44 Matrix44::identity44{  1.0, 0.0, 0.0, 0.0, 
									0.0, 1.0, 0.0, 0.0, 
									0.0, 0.0, 1.0, 0.0, 
									0.0, 0.0, 0.0, 1.0};









Matrix44::~Matrix44()
{
	// array will be destructed by itself
}


vxStatus::code Matrix44::get(scalar dest[]) const
{
	memcpy(dest, m_matrix, 16 * sizeof(scalar));
	return vxStatus::code::kSuccess;
}

Matrix44 Matrix44::transpose() const
{
	std::swap(m_matrix[1],	m_matrix[4]);
	std::swap(m_matrix[2],	m_matrix[8]);
	std::swap(m_matrix[3],	m_matrix[12]);
	std::swap(m_matrix[6],	m_matrix[9]);
	std::swap(m_matrix[7],	m_matrix[13]);
	std::swap(m_matrix[11],	m_matrix[15]);

	return *this;
}

Matrix44& Matrix44::setToIdentity()
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

Matrix44& Matrix44::setToProduct(const Matrix44 &, const Matrix44 &)
{
	
	return *this;
}

Matrix44& Matrix44::operator+=(const Matrix44 &)
{
	
	return *this;
}

Matrix44 Matrix44::operator+(const Matrix44 &) const
{
	
	return *this;
}

Matrix44& Matrix44::operator-=(const Matrix44 &)
{
	
	return *this;
}

Matrix44 Matrix44::operator-(const Matrix44 &) const
{
	
	return *this;
}

Matrix44& Matrix44::operator*=(const Matrix44 &)
{
	
	return *this;
}

Matrix44 Matrix44::operator*(const Matrix44 &right) const
{
	const Matrix44 &a=right.m_matrix;
	const Matrix44 &b=m_matrix;
	
	Matrix44 m;//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	m(0,0) = a(0,0)*b(0,0) + a(0,1)*b(1,0) + a(0,2)*b(2,0) + a(0,3)*b(3,0);
	m(1,0) = a(1,0)*b(0,0) + a(1,1)*b(1,0) + a(1,2)*b(2,0) + a(1,3)*b(3,0);
	m(2,0) = a(2,0)*b(0,0) + a(2,1)*b(1,0) + a(2,2)*b(2,0) + a(2,3)*b(3,0);
	m(3,0) = a(3,0)*b(0,0) + a(3,1)*b(1,0) + a(3,2)*b(2,0) + a(3,3)*b(3,0);

	m(0,1) = a(0,0)*b(0,1) + a(0,1)*b(1,1) + a(0,2)*b(2,1) + a(0,3)*b(3,1);
	m(1,1) = a(1,0)*b(0,1) + a(1,1)*b(1,1) + a(1,2)*b(2,1) + a(1,3)*b(3,1);
	m(2,1) = a(2,0)*b(0,1) + a(2,1)*b(1,1) + a(2,2)*b(2,1) + a(2,3)*b(3,1);
	m(3,1) = a(3,0)*b(0,1) + a(3,1)*b(1,1) + a(3,2)*b(2,1) + a(3,3)*b(3,1);
	
	m(0,2) = a(0,0)*b(0,2) + a(0,1)*b(1,2) + a(0,2)*b(2,2) + a(0,3)*b(3,2);
	m(1,2) = a(1,0)*b(0,2) + a(1,1)*b(1,2) + a(1,2)*b(2,2) + a(1,3)*b(3,2);
	m(2,2) = a(2,0)*b(0,2) + a(2,1)*b(1,2) + a(2,2)*b(2,2) + a(2,3)*b(3,2);
	m(3,2) = a(3,0)*b(0,2) + a(3,1)*b(1,2) + a(3,2)*b(2,2) + a(3,3)*b(3,2);
	
	m(0,3) = a(0,3)*b(0,3) + a(0,1)*b(1,3) + a(0,2)*b(2,3) + a(0,3)*b(3,3);
	m(1,3) = a(1,3)*b(0,3) + a(1,1)*b(1,3) + a(1,2)*b(2,3) + a(1,3)*b(3,3);
	m(2,3) = a(2,3)*b(0,3) + a(2,1)*b(1,3) + a(2,2)*b(2,3) + a(2,3)*b(3,3);
	m(3,3) = a(3,3)*b(0,3) + a(3,1)*b(1,3) + a(3,2)*b(2,3) + a(3,3)*b(3,3);
	
	return *this;
}

Matrix44 &Matrix44::operator*=(scalar)
{
	
	return *this;
}

Matrix44 Matrix44::operator*(scalar) const
{
	
	return *this;
}

const scalar *Matrix44::operator[](unsigned int row) const
{
	return &m_matrix[row*4];
}

bool Matrix44::operator==(const Matrix44 &other) const
{
	auto r = memcmp(m_matrix, other.m_matrix, 16 * sizeof(scalar));
	return r==0;
}

bool Matrix44::operator!=(const Matrix44 &other) const
{
	auto r = memcmp(m_matrix, other.m_matrix, 16 * sizeof(scalar));
	return r!=0;
}

Matrix44 &Matrix44::operator=(const Matrix44 &other)
{
	memcpy(m_matrix, other.m_matrix, 16 * sizeof(scalar));
	return *this;
}

Matrix44 Matrix44::inverse() const
{
	
	return *this;
}

Matrix44 Matrix44::adjoint() const
{
	
	return *this;
}

Matrix44 Matrix44::homogenize() const
{
	
	return *this;
}

scalar Matrix44::det4x4() const
{
	
	return 1.0;
}

scalar Matrix44::det3x3() const
{
	
	return 1.0;
}

bool Matrix44::isEquivalent(const Matrix44 &, scalar ) const
{
	bool eq{false};
			
	return eq;
}

bool Matrix44::isSingular() const
{
	
	return true;
}

void Matrix44::setOrigin(const v3s &orig) const
{
	m_matrix[12] = orig.x();
	m_matrix[13] = orig.y();
	m_matrix[14] = orig.z();
}

v3s Matrix44::getOrigin() const
{
	return v3s(m_matrix[12],m_matrix[13],m_matrix[14]);
}

v3s Matrix44::getScale() const
{
	return v3s(m_matrix[0],m_matrix[5],m_matrix[10]);
}

scalar &Matrix44::operator()(unsigned int row, unsigned int col)
{
	return (m_matrix[4*row+col]);
}

scalar Matrix44::operator()(unsigned int row, unsigned int col) const
{
	return (m_matrix[4*row+col]);
}

scalar *Matrix44::operator[](unsigned int row)
{
	return &m_matrix[4*row];
}

