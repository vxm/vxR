#ifndef MATRIX44_H
#define MATRIX44_H
#include <cstring>
#include "vxStatus.h"
#include "vxVector.h"
#include <initializer_list>

namespace vxCore {

using mdata = scalar[16];

class Matrix44
{
	mutable mdata m_matrix{ 1.0, 0.0, 0.0, 0.0, 
							0.0, 1.0, 0.0, 0.0, 
							0.0, 0.0, 1.0, 0.0, 
							0.0, 0.0, 0.0, 1.0};

public:
	static const Matrix44 identity44;
	
	Matrix44()
	{}
	Matrix44 (const Matrix44 &src)
	{
		memcpy(m_matrix, src.m_matrix, 16 * sizeof(scalar));
	}
	Matrix44 (const scalar m[16])
	{
		memcpy(m_matrix, m, 16 * sizeof(scalar));
	}
	Matrix44 (std::initializer_list<scalar> list)
	{
		auto i=0;
		for(auto&& d:list)
		{
			m_matrix[i] = d;
			++i;
		}
	}
	~Matrix44();

	// transform operations
	
	//Matrix44 
	
	//
	vxStatus::code	get(scalar dest[]) const;
	Matrix44		transpose() const;
	Matrix44&		setToIdentity();
	Matrix44&		setToProduct(const Matrix44&, const Matrix44&);
	Matrix44&		operator+= (const Matrix44& right);
	Matrix44		operator+ (const Matrix44& right) const;
	Matrix44&		operator-= (const Matrix44& right);
	Matrix44		operator- (const Matrix44& right) const;
	Matrix44&		operator*= (const Matrix44& right);
	Matrix44		operator* (const Matrix44& right) const;
	Matrix44&		operator*= (scalar);
	Matrix44		operator* (scalar) const;
	bool			operator== (const Matrix44& other) const;
	bool			operator!= (const Matrix44& other) const;
	Matrix44&		operator= (const Matrix44 &m);
	const scalar *	operator[] (unsigned int row) const;
	scalar &		operator() (unsigned int row, unsigned int col);
	scalar			operator() (unsigned int row, unsigned int col) const;
	scalar *		operator[] (unsigned int row);
	Matrix44		inverse() const;
	Matrix44		adjoint() const;
	Matrix44		homogenize() const;
	scalar			det4x4() const;
	scalar			det3x3() const;
	bool			isEquivalent (const Matrix44&, scalar =1.0e-10) const;
	bool			isSingular() const;

	void setOrigin(const v3s& orig) const;
	v3s getOrigin() const;
	v3s getScale() const;
	
	//Matrix44		operator* (scalar, const Matrix44& right);
	//std::ostream &		operator<< (std::ostream &os, const Matrix44&m);
};

}

#endif // MATRIX44_H
