#ifndef MATRIX44_H
#define MATRIX44_H
#include <cstring>
#include <array>
#include "Status.h"
#include "Vector.h"
#include <initializer_list>

namespace vxCore {

//using mdata = std::array<scalar,16>;
using mdata = scalar[16];

class Matrix44
{
	mutable mdata m_matrix = {  1.0, 0.0, 0.0, 0.0, 
								0.0, 1.0, 0.0, 0.0, 
								0.0, 0.0, 1.0, 0.0, 
								0.0, 0.0, 0.0, 1.0};

public:
	static const Matrix44 identity44;
	///
	/// \brief Matrix44
	///
	Matrix44();
	///
	/// \brief Matrix44
	/// \param src
	///
	Matrix44 (const Matrix44 &src);
	///
	/// \brief Matrix44
	/// \param m
	///
	Matrix44 (const scalar m[16]);
	///
	/// \brief Matrix44
	/// \param list
	///
	Matrix44 (std::initializer_list<scalar> list);
	/// destructor
	~Matrix44();

	// transform operations
	
	//Matrix44 
	
	///
	/// \brief get
	/// \param dest
	/// \return 
	///
	Status::code	get(scalar dest[]) const;
	///
	/// \brief transpose
	/// \return 
	///
	Matrix44 transpose() const;
	///
	/// \brief setToIdentity
	/// \return 
	///
	Matrix44& setToIdentity();
	///
	/// \brief setToProduct
	/// \return 
	///
	Matrix44& setToProduct(const Matrix44&, const Matrix44&);
	///
	/// \brief operator +=
	/// \param right
	/// \return 
	///
	Matrix44& operator+= (const Matrix44& right);
	///
	/// \brief operator +
	/// \param right
	/// \return 
	///
	Matrix44 operator+ (const Matrix44& right) const;
	///
	/// \brief operator -=
	/// \param right
	/// \return 
	///
	Matrix44& operator-= (const Matrix44& right);
	///
	/// \brief operator -
	/// \param right
	/// \return 
	///
	Matrix44 operator- (const Matrix44& right) const;
	///
	/// \brief operator *=
	/// \param right
	/// \return 
	///
	Matrix44& operator*= (const Matrix44& right);
	///
	/// \brief operator *
	/// \param right
	/// \return 
	///
	Matrix44 operator* (const Matrix44& right) const;
	///
	/// \brief operator *=
	/// \return 
	///
	Matrix44& operator*= (scalar);
	///
	/// \brief operator *
	/// \return 
	///
	Matrix44 operator* (scalar) const;
	///
	/// \brief operator ==
	/// \param other
	/// \return 
	///
	bool operator== (const Matrix44& other) const;
	///
	/// \brief operator !=
	/// \param other
	/// \return 
	///
	bool operator!= (const Matrix44& other) const;
	///
	/// \brief operator =
	/// \param m
	/// \return 
	///
	Matrix44& operator= (const Matrix44 &m);
	///
	/// \brief operator []
	/// \param row
	/// \return 
	///
	const scalar *operator[] (unsigned int row) const;
	///
	/// \brief operator ()
	/// \param row
	/// \param col
	/// \return 
	///
	scalar & operator() (unsigned int row, unsigned int col);
	///
	/// \brief operator ()
	/// \param row
	/// \param col
	/// \return 
	///
	scalar 	operator() (unsigned int row, unsigned int col) const;
	///
	/// \brief operator []
	/// \param row
	/// \return 
	///
	scalar * operator[] (unsigned int row);
	///
	/// \brief inverse
	/// \return 
	///
	Matrix44 inverse() const;
	///
	/// \brief adjoint
	/// \return 
	///
	Matrix44 adjoint() const;
	///
	/// \brief homogenize
	/// \return 
	///
	Matrix44 homogenize() const;
	///
	/// \brief det4x4
	/// \return 
	///
	scalar det4x4() const;
	///
	/// \brief det3x3
	/// \return 
	///
	scalar det3x3() const;
	///
	/// \brief isEquivalent
	/// \return 
	///
	bool isEquivalent (const Matrix44&, scalar =1.0e-10) const;
	///
	/// \brief isSingular
	/// \return 
	///
	bool isSingular() const;
	///
	/// \brief setOrigin
	/// \param orig
	///
	void setOrigin(const v3s& orig) const;
	///
	/// \brief getOrigin
	/// \return 
	///
	v3s getOrigin() const;
	///
	/// \brief getScale
	/// \return 
	///
	v3s getScale() const;
};

}

#endif // MATRIX44_H
