#ifndef VXBOUNDINGBOX_H
#define VXBOUNDINGBOX_H

#include "Collision.h"
#include "MathUtils.h"
#include "Ray.h"
#include "Vector.h"
#include <atomic>
#include <memory>

namespace vxCore
{

class BoundingBox;
class Matrix44;

using BoundingBoxHandle = std::shared_ptr<BoundingBox>;

// TODO: this class needs heavy caching on "close" method.
class BoundingBox : public std::enable_shared_from_this<BoundingBox>
{
	scalar m_minX = 1.0;
	scalar m_minY = 0.0;
	scalar m_minZ = 0.0;

	scalar m_maxX = -1.0;
	scalar m_maxY = 0.0;
	scalar m_maxZ = 0.0;

	//	v3 m_scale;

public:
	///
	/// \brief clear
	///
	void clear();
	///
	/// \brief set
	/// \param position
	/// \param size
	///
	void set(const v3s &position, scalar size);
	///
	/// \brief set
	/// \param mnx
	/// \param mny
	/// \param mnz
	/// \param mxx
	/// \param mxy
	/// \param mxz
	///
	void set(scalar mnx, scalar mny, scalar mnz, scalar mxx, scalar mxy,
	         scalar mxz);
	///
	/// \brief markAsInitialized
	///
	void markAsInitialized();
	///
	/// \brief close
	///
	void close();
	///
	/// \brief extend
	/// \param limit
	///
	void extend(const v3s &limit);
	///
	/// \brief extend
	/// \param box
	///
	void extend(const BoundingBox &box);
	///
	/// \brief xLength
	/// \return
	///
	scalar xLength() const;
	///
	/// \brief yLength
	/// \return
	///
	scalar yLength() const;
	///
	/// \brief zLength
	/// \return
	///
	scalar zLength() const;
	///
	/// \brief max
	/// \return
	///
	v3s max() const;
	///
	/// \brief min
	/// \return
	///
	v3s min() const;
	///
	/// \brief center
	/// \return
	///
	v3s center() const;
	///
	/// \brief minX
	/// \return
	///
	scalar minX() const;
	///
	/// \brief setMinX
	/// \param minx
	///
	void setMinX(scalar minx);
	///
	/// \brief minY
	/// \return
	///
	scalar minY() const;
	///
	/// \brief setMinY
	/// \param miny
	///
	void setMinY(scalar miny);
	///
	/// \brief minZ
	/// \return
	///
	scalar minZ() const;
	///
	/// \brief setMinZ
	/// \param minz
	///
	void setMinZ(scalar minz);
	///
	/// \brief maxX
	/// \return
	///
	scalar maxX() const;
	///
	/// \brief setMaxX
	/// \param maxx
	///
	void setMaxX(scalar maxx);
	///
	/// \brief maxY
	/// \return
	///
	scalar maxY() const;
	///
	/// \brief setMaxY
	/// \param maxy
	///
	void setMaxY(scalar maxy);
	///
	/// \brief maxZ
	/// \return
	///
	scalar maxZ() const;
	///
	/// \brief setMaxZ
	/// \param maxz
	///
	void setMaxZ(scalar maxz);
	///
	/// \brief midXValue
	/// \return
	///
	scalar midXValue() const;
	///
	/// \brief midYValue
	/// \return
	///
	scalar midYValue() const;
	///
	/// \brief midZValue
	/// \return
	///
	scalar midZValue() const;
	///
	/// \brief contains
	/// \param v
	/// \return
	///
	bool contains(const v3s &v) const;
	///
	/// \brief contains
	/// \param v
	/// \param tolerance
	/// \return
	///
	bool contains(const v3s &v, scalar tolerance) const;
	///
	/// \brief diagonal
	/// \return
	///
	v3s diagonal() const;
	///
	/// \brief applyTransform
	/// \param m
	///
	void applyTransform(const Matrix44 &m);

	// renderable interface
	bool throwRay(const Ray &ray) const;
	int throwRay(const Ray &ray, Collision &col) const;
	bool hasCollision(const Ray &ray) const;
};

} // namespace vxCore
#endif // VXBOUNDINGBOX_H
