#ifndef MATRIX33_H
#define MATRIX33_H


#include "vxStatus.h"
#include "vxVector.h"
#include <initializer_list>
#include "Quaternion.h"

namespace vxCore {

using m33data = scalar[9];

class Matrix33
{
	mutable m33data m_matrix{1.0, 0.0, 0.0, 
							 0.0, 1.0, 0.0, 
							 0.0, 0.0, 1.0};

public:
	static const Matrix33 identity33;
	///
	/// \brief Matrix33
	///
	Matrix33();
	///
	/// \brief Matrix33
	/// \param src
	///
	Matrix33 (const Matrix33 &src);
	///
	/// \brief Matrix33
	/// \param m
	///
	Matrix33 (const m33data m);
	///
	/// \brief Matrix33
	/// \param list
	///
	Matrix33 (std::initializer_list<scalar> list);
	/// destructor
	~Matrix33();
	///
	/// \brief resetScale
	///Resets the scale of the three axis in this matrix.
	void resetScale();
	///
	/// \brief getQuaternion
	/// \return 
	///This will create a matrix, remove scale on the copy
	/// and extract a quaternion which is the value returned
	Quaternion getQuaternion() const;
	///
	/// \brief get
	/// \param i
	/// \param j
	/// \return 
	///
	scalar get(unsigned int i, unsigned int j) const;
	///
	/// \brief get
	/// \param i
	/// \param j
	/// \return 
	///
	scalar& get(unsigned int i, unsigned int j);
	///
	/// \brief transpose
	/// \return 
	///
	Matrix33 transpose() const;
	///
	/// \brief setToIdentity
	/// \return 
	///
	Matrix33& setToIdentity();
	///
	/// \brief setToProduct
	/// \return 
	///
	Matrix33& setToProduct(const Matrix33&, const Matrix33&);
	///
	/// \brief operator +=
	/// \param right
	/// \return 
	///
	Matrix33& operator+= (const Matrix33& right);
	///
	/// \brief operator +
	/// \param right
	/// \return 
	///
	Matrix33 operator+ (const Matrix33& right) const;
	///
	/// \brief operator -=
	/// \param right
	/// \return 
	///
	Matrix33& operator-= (const Matrix33& right);
	///
	/// \brief operator -
	/// \param right
	/// \return 
	///
	Matrix33 operator- (const Matrix33& right) const;
	///
	/// \brief operator *=
	/// \param right
	/// \return 
	///
	Matrix33& operator*= (const Matrix33& right);
	///
	/// \brief operator *
	/// \param right
	/// \return 
	///
	Matrix33 operator* (const Matrix33& right) const;
	///
	/// \brief operator *
	/// \param other
	/// \return 
	///
	v3s operator*(const v3s &v) const;
	///
	/// \brief operator *=
	/// \return 
	///
	Matrix33& operator*= (scalar);
	///
	/// \brief operator *
	/// \return 
	///
	Matrix33 operator* (scalar) const;
	///
	/// \brief operator ==
	/// \param other
	/// \return 
	///
	bool operator== (const Matrix33& other) const;
	///
	/// \brief operator !=
	/// \param other
	/// \return 
	///
	bool operator!= (const Matrix33& other) const;
	///
	/// \brief operator =
	/// \param m
	/// \return 
	///
	Matrix33& operator= (const Matrix33 &m);
	///
	/// \brief operator []
	/// \param row
	/// \return 
	///
	const scalar *	operator[] (unsigned int row) const;
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
	scalar operator() (unsigned int row, unsigned int col) const;
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
	Matrix33 inverse() const;
	///
	/// \brief adjoint
	/// \return 
	///
	Matrix33 adjoint() const;
	///
	/// \brief homogenize
	/// \return 
	///
	Matrix33 homogenize() const;
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
	bool isEquivalent (const Matrix33&, scalar =1.0e-10) const;
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
	
	//Matrix33 operator* (scalar, const Matrix33& right);
	//std::ostream & operator<< (std::ostream &os, const Matrix33&m);
};

}

#endif // MATRIX33_H
