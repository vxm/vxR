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
class BoundingBox
{
	scalar m_minX = 1.0;
	scalar m_minY = 0.0;
	scalar m_minZ = 0.0;

	scalar m_maxX = -1.0;
	scalar m_maxY = 0.0;
	scalar m_maxZ = 0.0;

	//	v3 m_scale;

public:
	BoundingBox() = default;
	BoundingBox(scalar mnx, scalar mny, scalar mnz, scalar mxx, scalar mxy,
				scalar mxz);

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

	v3s closestPointInside(const v3s &pos, scalar offset) const;
	///
	/// \brief applyTransform
	/// \param m
	///
	void applyTransform(const Matrix44 &m);

	// renderable interface
	bool throwRay(const Ray &ray) const;
	int throwRay(const Ray &ray, Collision &col) const;
	bool hasCollision(const Ray &ray) const;

	///
	/// \brief intersect
	/// Slab test with a prepared ray. On return [t0, t1] is the part of the ray
	/// inside the box in units of the ray direction, with t0 >= 0 (a ray that
	/// starts inside has t0 = 0). Returns false when there is no such part.
	/// It does not fill a Collision, it is for traversal code. The legacy
	/// throwRay(ray, collision) remains the one that produces surface hits.
	/// Meant for a non empty box, it never divides and cannot give inf or NaN.
	bool intersect(const RayInv &r, scalar &t0, scalar &t1) const
	{
		const scalar txa = (m_minX - r.ox) * r.ix;
		const scalar txb = (m_maxX - r.ox) * r.ix;
		const scalar tya = (m_minY - r.oy) * r.iy;
		const scalar tyb = (m_maxY - r.oy) * r.iy;
		const scalar tza = (m_minZ - r.oz) * r.iz;
		const scalar tzb = (m_maxZ - r.oz) * r.iz;

		const scalar nx = r.sx ? txb : txa;
		const scalar fx = r.sx ? txa : txb;
		const scalar ny = r.sy ? tyb : tya;
		const scalar fy = r.sy ? tya : tyb;
		const scalar nz = r.sz ? tzb : tza;
		const scalar fz = r.sz ? tza : tzb;

		scalar lo = nx > ny ? nx : ny;
		lo = lo > nz ? lo : nz;
		lo = lo > scalar(0.0) ? lo : scalar(0.0);
		scalar hi = fx < fy ? fx : fy;
		hi = hi < fz ? hi : fz;

		t0 = lo;
		t1 = hi;
		return lo <= hi;
	}

	/// Area of the six faces, 0 for an empty box.
	scalar surfaceArea() const
	{
		const scalar dx = m_maxX > m_minX ? m_maxX - m_minX : scalar(0.0);
		const scalar dy = m_maxY > m_minY ? m_maxY - m_minY : scalar(0.0);
		const scalar dz = m_maxZ > m_minZ ? m_maxZ - m_minZ : scalar(0.0);
		return scalar(2.0) * (dx * dy + dy * dz + dz * dx);
	}
};

} // namespace vxCore
#endif // VXBOUNDINGBOX_H
