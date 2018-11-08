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

enum class VisibleType
{
	kOpaque, // it is opaque
	kOtherOpaque, // it is opaque but it can't see itself.
	kSemitransparent, // it is semi transparent
	kLight, // it is a light kind
	kAll // generic
};


class Visible
{
protected:
	
	std::shared_ptr<BoundingBox> m_bb;
	
	Matrix44 m_transform;
	
	Color m_color = Color::white;
	
	std::shared_ptr<Shader> m_shader;
	
	VisibleType m_type = VisibleType::kOpaque;
	
public:
	///
	/// \brief Visible
	///
	Visible();
	
	virtual ~Visible() = default;
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
	
	std::shared_ptr<Shader> shader() const;
	void setShader(const std::shared_ptr<Shader> &shader);
	VisibleType type() const;
	void setType(const VisibleType &type);
};

using VisibleHandle = std::shared_ptr<Visible>;

}
#endif // VISIBLE_H
