#ifndef MATRIX33_H
#define MATRIX33_H


#include "vxStatus.h"
#include "vxVector.h"
#include <initializer_list>

namespace vxCore {

using m33data = scalar[9];

class Matrix33
{
	mutable m33data m_matrix{1.0, 0.0, 0.0, 
							 0.0, 1.0, 0.0, 
							 0.0, 0.0, 1.0};

public:
	static const Matrix33 identity;
	
	Matrix33();
	Matrix33 (const Matrix33 &src);
	Matrix33 (const m33data m);
	Matrix33 (std::initializer_list<scalar> list);
	~Matrix33();

	// transform operations
	
	//Matrix33 
	
	//
	vxStatus::code	get(scalar dest[]) const;
	Matrix33		transpose() const;
	Matrix33&		setToIdentity();
	Matrix33&		setToProduct(const Matrix33&, const Matrix33&);
	Matrix33&		operator+= (const Matrix33& right);
	Matrix33		operator+ (const Matrix33& right) const;
	Matrix33&		operator-= (const Matrix33& right);
	Matrix33		operator- (const Matrix33& right) const;
	Matrix33&		operator*= (const Matrix33& right);
	Matrix33		operator* (const Matrix33& right) const;
	Matrix33&		operator*= (scalar);
	Matrix33		operator* (scalar) const;
	bool			operator== (const Matrix33& other) const;
	bool			operator!= (const Matrix33& other) const;
	Matrix33&		operator= (const Matrix33 &m);
	const scalar *	operator[] (unsigned int row) const;
	scalar &		operator() (unsigned int row, unsigned int col);
	scalar			operator() (unsigned int row, unsigned int col) const;
	scalar *		operator[] (unsigned int row);
	Matrix33		inverse() const;
	Matrix33		adjoint() const;
	Matrix33		homogenize() const;
	scalar			det4x4() const;
	scalar			det3x3() const;
	bool			isEquivalent (const Matrix33&, scalar =1.0e-10) const;
	bool			isSingular() const;

	void setOrigin(const v3s& orig) const;
	v3s getOrigin() const;
	v3s getScale() const;
	
	//Matrix33		operator* (scalar, const Matrix33& right);
	//std::ostream &		operator<< (std::ostream &os, const Matrix33&m);
};

}

#endif // MATRIX33_H
