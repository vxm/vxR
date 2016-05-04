#include <cstring>
#include <utility>

#include "Matrix33.h"

using namespace vxCore;


const Matrix33 Matrix33::identity33{  1.0, 0.0, 0.0, 
									0.0, 1.0, 0.0, 
									0.0, 0.0, 1.0};

Matrix33::Matrix33()
{}

Matrix33::Matrix33(const Matrix33 &src)
{
	memcpy(m_matrix, src.m_matrix, 9 * sizeof(scalar));
}

Matrix33::Matrix33(const m33data m)
{
	memcpy(m_matrix, m, 9 * sizeof(scalar));
}

Matrix33::Matrix33 (std::initializer_list<scalar> list)
{
	auto i=0;
	for(auto&& d:list)
	{
		m_matrix[i] = d;
		++i;
	}
}

Matrix33::~Matrix33()
{
	// array will be destructed by itself
}


vxStatus::code Matrix33::get(scalar dest[]) const
{
	memcpy(dest, m_matrix, 9 * sizeof(scalar));
	return vxStatus::code::kSuccess;
}

Matrix33 Matrix33::transpose() const
{
	
	return *this;
}

Matrix33& Matrix33::setToIdentity()
{
	*this = Matrix33::identity33;
	return *this;
}

Matrix33& Matrix33::setToProduct(const Matrix33 &, const Matrix33 &)
{
	
	return *this;
}

Matrix33& Matrix33::operator+=(const Matrix33 &)
{
	
	return *this;
}

Matrix33 Matrix33::operator+(const Matrix33 &) const
{
	
	return *this;
}

Matrix33& Matrix33::operator-=(const Matrix33 &)
{
	
	return *this;
}

Matrix33 Matrix33::operator-(const Matrix33 &) const
{
	
	return *this;
}

Matrix33& Matrix33::operator*=(const Matrix33 &)
{
	
	return *this;
}

Matrix33 Matrix33::operator*(const Matrix33 &right) const
{
	const Matrix33 &a=right.m_matrix;
	const Matrix33 &b=m_matrix;
	
	Matrix33 m;//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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

Matrix33 &Matrix33::operator*=(scalar)
{
	
	return *this;
}

Matrix33 Matrix33::operator*(scalar) const
{
	
	return *this;
}

const scalar *Matrix33::operator[](unsigned int row) const
{
	return &m_matrix[row*4];
}

bool Matrix33::operator==(const Matrix33 &other) const
{
	auto r = memcmp(m_matrix, other.m_matrix, 9 * sizeof(scalar));
	return r==0;
}

bool Matrix33::operator!=(const Matrix33 &other) const
{
	auto r = memcmp(m_matrix, other.m_matrix, 9 * sizeof(scalar));
	return r!=0;
}

Matrix33 &Matrix33::operator=(const Matrix33 &other)
{
	memcpy(m_matrix, other.m_matrix, 9 * sizeof(scalar));
	return *this;
}

Matrix33 Matrix33::inverse() const
{
	
	return *this;
}

Matrix33 Matrix33::adjoint() const
{
	
	return *this;
}

Matrix33 Matrix33::homogenize() const
{
	
	return *this;
}

scalar Matrix33::det4x4() const
{
	
	return 1.0;
}

scalar Matrix33::det3x3() const
{
	
	return 1.0;
}

bool Matrix33::isEquivalent(const Matrix33 &, scalar ) const
{
	bool eq{false};
			
	return eq;
}

bool Matrix33::isSingular() const
{
	
	return true;
}

void Matrix33::setOrigin(const v3s &orig) const
{
	m_matrix[12] = orig.x();
	m_matrix[13] = orig.y();
	m_matrix[14] = orig.z();
}

v3s Matrix33::getOrigin() const
{
	return v3s(m_matrix[12],m_matrix[13],m_matrix[14]);
}

v3s Matrix33::getScale() const
{
	return v3s(m_matrix[0],m_matrix[5],m_matrix[10]);
}

scalar &Matrix33::operator()(unsigned int row, unsigned int col)
{
	return (m_matrix[4*row+col]);
}

scalar Matrix33::operator()(unsigned int row, unsigned int col) const
{
	return (m_matrix[4*row+col]);
}

scalar *Matrix33::operator[](unsigned int row)
{
	return &m_matrix[4*row];
}
