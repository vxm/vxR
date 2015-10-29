#ifndef VXMATRIX_H
#define VXMATRIX_H

#include "vxStatus.h"
#include "vxVector.h"
#include <initializer_list>

namespace vxCore {


using mdata = scalar[16];

class vxMatrix
{
	mutable mdata m_matrix{ 1.0, 0.0, 0.0, 0.0, 
							0.0, 1.0, 0.0, 0.0, 
							0.0, 0.0, 1.0, 0.0, 
							0.0, 0.0, 0.0, 1.0};

public:
	
	vxMatrix();
	vxMatrix (const vxMatrix &src);
	vxMatrix (const scalar m[16]);
	vxMatrix (std::initializer_list<scalar> list);
	~vxMatrix();

	vxStatus::code	get(scalar dest[]) const;
	vxMatrix		transpose() const;
	vxMatrix&		setToIdentity();
	vxMatrix&		setToProduct(const vxMatrix&, const vxMatrix&);
	vxMatrix&		operator+= (const vxMatrix& right);
	vxMatrix		operator+ (const vxMatrix& right) const;
	vxMatrix&		operator-= (const vxMatrix& right);
	vxMatrix		operator- (const vxMatrix& right) const;
	vxMatrix&		operator*= (const vxMatrix& right);
	vxMatrix		operator* (const vxMatrix& right) const;
	vxMatrix&		operator*= (scalar);
	vxMatrix		operator* (scalar) const;
	bool			operator== (const vxMatrix& other) const;
	bool			operator!= (const vxMatrix& other) const;
	vxMatrix&		operator= (const vxMatrix &m);
	const scalar *	operator[] (unsigned int row) const;
	scalar &		operator() (unsigned int row, unsigned int col);
	scalar			operator() (unsigned int row, unsigned int col) const;
	scalar *		operator[] (unsigned int row);
	vxMatrix		inverse() const;
	vxMatrix		adjoint() const;
	vxMatrix		homogenize() const;
	scalar			det4x4() const;
	scalar			det3x3() const;
	bool			isEquivalent (const vxMatrix&, scalar =1.0e-10) const;
	bool			isSingular() const;
	static const vxMatrix identity;
	
	vxVector3d getOrigin() const;
	
	//vxMatrix		operator* (scalar, const vxMatrix& right);
	//std::ostream &		operator<< (std::ostream &os, const vxMatrix&m);
};

}
#endif // VXMATRIX_H
