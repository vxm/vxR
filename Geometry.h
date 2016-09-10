#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <typeinfo>
#include <vector>
#include <atomic>
#include "Vector.h"
#include "Ray.h"
//#include "Shader.h"
#include "Collision.h"
#include "TriRef.h"
#include "Matrix44.h"
#include "BoundingBox.h"
#include "GeoGrid.h"
#include "Color.h"
#include <set>

namespace vxCore {

class Geometry;
class Shader;

using vxGeometryHandle = std::shared_ptr<Geometry>;

class Geometry
{
protected:
	std::shared_ptr<BoundingBox> m_bb;
	Matrix44 m_transform;
	
	Color m_baseColor;
	Shader* m_shader;
	
public:
	///
	/// \brief Geometry
	///
	Geometry();
	virtual ~Geometry();
	///
	/// \brief transform
	/// \return 
	///
	Matrix44 transform() const;
	///
	/// \brief transform
	/// \return 
	///
	Matrix44& transform();
	///
	/// \brief setTransform
	/// \param transform
	///
	void setTransform(const Matrix44 &transform);
	///
	/// \brief boundingBox
	/// \return 
	///
	std::shared_ptr<BoundingBox> boundingBox();
	///
	/// \brief baseColor
	/// \return 
	///
	Color baseColor() const;
	///
	/// \brief setBaseColor
	/// \param baseColor
	///
	void setBaseColor(const Color &baseColor);
	///
	/// \brief shader
	/// \return 
	///
	Shader *shader() const;
	///
	/// \brief setShader
	/// \param shader
	///
	void setShader(Shader *shader);
	///
	/// \brief testBoundingBox
	/// \param ray
	/// \param collide
	/// \return 
	/// Will test the bounding box of this geometry for collision.
	int testBoundingBox(const Ray &ray, Collision &collide) const;
	
	//renderable interface
	virtual bool throwRay(const Ray &ray) const;
	virtual int throwRay(const Ray &ray, Collision &collide) const;
	virtual bool hasCollision(const Ray &ray) const;
};

}
#endif // VXGEOMETRY_H
