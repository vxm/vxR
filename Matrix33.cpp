#include <cstring>
#include <utility>

#include "Matrix33.h"

using namespace vxCore;

const Matrix33 Matrix33::identity33{1.0, 0.0, 0.0, 0.0, 1.0,
									0.0, 0.0, 0.0, 1.0};

Matrix33::Matrix33() {}

Matrix33::Matrix33(const Matrix33 &src)
{
	memcpy(m_matrix, src.m_matrix, 9 * sizeof(scalar));
}

Matrix33::Matrix33(const m33data m) { memcpy(m_matrix, m, 9 * sizeof(scalar)); }

Matrix33::Matrix33(std::initializer_list<scalar> list)
{
	auto i = 0;
	for (auto &&d : list)
	{
		m_matrix[i] = d;
		++i;
	}
}

Matrix33::~Matrix33()
{
	// array will be destructed by itself
}

void Matrix33::resetScale()
{
	auto lx = v3s(m_matrix[0], m_matrix[1], m_matrix[2]).length();
	auto ly = v3s(m_matrix[3], m_matrix[4], m_matrix[5]).length();
	auto lz = v3s(m_matrix[6], m_matrix[7], m_matrix[8]).length();

	m_matrix[0] /= lx;
	m_matrix[1] /= lx;
	m_matrix[2] /= lx;

	m_matrix[3] /= ly;
	m_matrix[4] /= ly;
	m_matrix[5] /= ly;

	m_matrix[6] /= lz;
	m_matrix[7] /= lz;
	m_matrix[8] /= lz;
}

Quaternion Matrix33::getQuaternion() const
{
	Matrix33 cp(*this);
	cp.resetScale();

	scalar temp[4];
	scalar trace = cp.get(0, 0) + cp.get(1, 1) + cp.get(2, 2);

	if (trace > 0.0)
	{
		scalar s = sqrt(trace + 1.0);
		temp[3] = (s * 0.5);
		s = 0.5 / s;

		temp[0] = ((cp.get(2, 1) - cp.get(1, 2)) * s);
		temp[1] = ((cp.get(0, 2) - cp.get(2, 0)) * s);
		temp[2] = ((cp.get(1, 0) - cp.get(0, 1)) * s);
	}
	else
	{
		int i = cp.get(0, 0) < cp.get(1, 1) ? (cp.get(1, 1) < cp.get(2, 2) ? 2 : 1)
											: (cp.get(0, 0) < cp.get(2, 2) ? 2 : 0);
		int j = (i + 1) % 3;
		int k = (i + 2) % 3;

		scalar s = sqrt(cp.get(i, i) - cp.get(j, j) - cp.get(k, k) + 1.0);
		temp[i] = s * 0.5;
		s = 0.5 / s;

		temp[3] = (cp.get(k, j) - cp.get(j, k)) * s;
		temp[j] = (cp.get(j, i) + cp.get(i, j)) * s;
		temp[k] = (cp.get(k, i) + cp.get(i, k)) * s;
	}

	return {temp[0], temp[1], temp[2], temp[3]};
}

scalar Matrix33::get(unsigned int i, unsigned int j) const
{
	return m_matrix[i * 4 + j];
}

scalar &Matrix33::get(unsigned int i, unsigned int j)
{
	return m_matrix[i * 4 + j];
}

Matrix33 Matrix33::transpose() const { return *this; }

Matrix33 &Matrix33::setToIdentity()
{
	*this = Matrix33::identity33;
	return *this;
}

Matrix33 &Matrix33::setToProduct(const Matrix33 &, const Matrix33 &)
{

	return *this;
}

Matrix33 &Matrix33::operator+=(const Matrix33 &) { return *this; }

Matrix33 Matrix33::operator+(const Matrix33 &) const { return *this; }

Matrix33 &Matrix33::operator-=(const Matrix33 &) { return *this; }

Matrix33 Matrix33::operator-(const Matrix33 &) const { return *this; }

Matrix33 &Matrix33::operator*=(const Matrix33 &) { return *this; }

v3s Matrix33::operator*(const v3s &v) const
{
	return {v3s(&m_matrix[0]).dot(v), v3s(&m_matrix[3]).dot(v),
			v3s(&m_matrix[6]).dot(v)};
}

Matrix33 Matrix33::operator*(const Matrix33 &right) const
{
	// TODO
	return *this;
}

Matrix33 &Matrix33::operator*=(scalar) { return *this; }

Matrix33 Matrix33::operator*(scalar) const { return *this; }

const scalar *Matrix33::operator[](unsigned int row) const
{
	return &m_matrix[row * 4];
}

bool Matrix33::operator==(const Matrix33 &other) const
{
	auto r = memcmp(m_matrix, other.m_matrix, 9 * sizeof(scalar));
	return r == 0;
}

bool Matrix33::operator!=(const Matrix33 &other) const
{
	auto r = memcmp(m_matrix, other.m_matrix, 9 * sizeof(scalar));
	return r != 0;
}

Matrix33 &Matrix33::operator=(const Matrix33 &other)
{
	memcpy(m_matrix, other.m_matrix, 9 * sizeof(scalar));
	return *this;
}

Matrix33 Matrix33::inverse() const { return *this; }

Matrix33 Matrix33::adjoint() const { return *this; }

Matrix33 Matrix33::homogenize() const { return *this; }

scalar Matrix33::det4x4() const { return 1.0; }

scalar Matrix33::det3x3() const { return 1.0; }

bool Matrix33::isEquivalent(const Matrix33 &, scalar) const
{
	bool eq{false};

	return eq;
}

bool Matrix33::isSingular() const { return true; }

scalar &Matrix33::operator()(unsigned int row, unsigned int col)
{
	return (m_matrix[4 * row + col]);
}

scalar Matrix33::operator()(unsigned int row, unsigned int col) const
{
	return (m_matrix[4 * row + col]);
}

scalar *Matrix33::operator[](unsigned int row) { return &m_matrix[4 * row]; }
