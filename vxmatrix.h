#ifndef VXMATRIX_H
#define VXMATRIX_H

#include <vxstatus.h>
#include <initializer_list>

using mdata = double[16];

class vxMatrix
{
	mutable mdata m_matrix{ 1.0, 0.0, 0.0, 0.0, 
							0.0, 1.0, 0.0, 0.0, 
							0.0, 0.0, 1.0, 0.0, 
							0.0, 0.0, 0.0, 1.0};

public:
	
	vxMatrix();
	vxMatrix (const vxMatrix&src);
	vxMatrix (const double m[16]);
	vxMatrix (std::initializer_list<double> list);
	~vxMatrix ();

	vxMatrix&		operator= (const vxMatrix &m);
	double			operator() (unsigned int row, unsigned int col) const;
	const double *	operator[] (unsigned int row) const;
	vxStatus::code	get(double dest[]) const;
	vxStatus::code	get(float dest[]) const;
	vxMatrix	transpose() const;
	vxMatrix&		setToIdentity();
	vxMatrix&		setToProduct(const vxMatrix&left, const vxMatrix&right);
	vxMatrix&		operator+= (const vxMatrix&right);
	vxMatrix		operator+ (const vxMatrix&right) const;
	vxMatrix&		operator-= (const vxMatrix&right);
	vxMatrix		operator- (const vxMatrix&right) const;
	vxMatrix&		operator*= (const vxMatrix&right);
	vxMatrix		operator* (const vxMatrix&right) const;
	vxMatrix&		operator*= (double);
	vxMatrix		operator* (double) const;
	bool			operator== (const vxMatrix&other) const;
	bool			operator!= (const vxMatrix&other) const;
	vxMatrix		inverse() const;
	vxMatrix		adjoint() const;
	vxMatrix		homogenize() const;
	double			det4x4() const;
	double			det3x3() const;
	bool			isEquivalent (const vxMatrix&other, double tolerance=1.0e-10) const;
	bool			isSingular() const;
	double &		operator() (unsigned int row, unsigned int col);
	double *		operator[] (unsigned int row);
	static const vxMatrix identity;
	
	//vxMatrix		operator* (double, const vxMatrix&right);
	//std::ostream &		operator<< (std::ostream &os, const vxMatrix&m);
};

#endif // VXMATRIX_H
