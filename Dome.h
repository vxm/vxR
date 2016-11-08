#ifndef VXDOME_H
#define VXDOME_H

#include "Image.h"
#include "Ray.h"
#include "Collision.h"
namespace vxCore {


class Dome
{
	// Light domm
	ImageHandle m_image;

	scalar m_radius{62.8};
	
public:
	///
	/// \brief vxDome
	///
	Dome();
	///
	/// \brief vxDome
	/// \param env
	///
	Dome(ImageHandle env);
	///
	/// \brief gain
	/// \return 
	///
	scalar gain() const;
	///
	/// \brief setGain
	/// \param gain
	///
	void setGain(const scalar &gain);
	///
	/// \brief gamma
	/// \return 
	///
	scalar gamma() const;
	///
	/// \brief setGamma
	/// \param gamma
	///
	void setGamma(const scalar &gamma);
	///
	/// \brief radius
	/// \return 
	///
	scalar radius() const;
	///
	/// \brief setRadius
	/// \param radius
	///
	void setRadius(const scalar &radius);

	///
	/// \brief computeLight
	/// \param ray
	/// \param collide
	/// \return 
	///
	int computeLight(const Ray &ray, Collision &collide) const;

	//renderable interface
	bool throwRay(const Ray &) const;
	int throwRay(const Ray &ray, Collision &collide) const;
	bool hasCollision(const Ray &) const;
};

using vxDomeHandle = std::shared_ptr<Dome>;

}
#endif // VXDOM_H
