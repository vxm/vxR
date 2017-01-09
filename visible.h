#ifndef VISIBLE_H
#define VISIBLE_H

#include <functional>
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
#include <visible.h>

namespace vxCore {

class Shader;

class Visible
{
protected:
	
	std::shared_ptr<BoundingBox> m_bb;
	
	Matrix44 m_transform;
	
	Color m_color = Color::white;
	
	Shader* m_shader;
	
public:
	///
	/// \brief Visible
	///
	Visible();
	
	virtual ~Visible();
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
	Color color() const;
	///
	/// \brief setBaseColor
	/// \param baseColor
	///
	void setColor(const Color &color);
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
	
	virtual void updateBoundingBox() = 0;
	
	///
	/// \brief testBoundingBox
	/// \param ray
	/// \param collide
	/// \return 
	/// Will test the bounding box of this Visible for collision.
	int testBoundingBox(const Ray &ray, Collision &collide) const;
	
	
	//renderable interface
	virtual bool throwRay(const Ray &ray) const = 0;
	virtual int throwRay(const Ray &ray, Collision &collide) const = 0;
	virtual bool hasCollision(const Ray &ray) const = 0;
	
};

using VisibleHandle = std::shared_ptr<Visible>;

}
#endif // VISIBLE_H
