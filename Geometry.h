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

	Geometry();
	virtual ~Geometry();
	Matrix44 transform() const;
	Matrix44& transform();

	void setTransform(const Matrix44 &transform);

	std::shared_ptr<BoundingBox> boundingBox();

	Color baseColor() const;
	void setBaseColor(const Color &baseColor);
	Shader *shader() const;
	void setShader(Shader *shader);
	
	//renderable interface
	virtual bool throwRay(const Ray &ray) const;
	virtual int throwRay(const Ray &ray, Collision &collide) const;
	virtual bool hasCollision(const Ray &ray) const;
};

}
#endif // VXGEOMETRY_H
