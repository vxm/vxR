#ifndef VXDOME_H
#define VXDOME_H

#include "vxBitMap2d.h"
#include "vxRay.h"
#include "vxCollision.h"
namespace vxCore {


class vxDome
{
	// Light domm
	std::shared_ptr<vxBitMap2d> m_image;

	scalar m_radius{62.8};
	
public:
	///
	/// \brief vxDome
	///
	vxDome();
	///
	/// \brief vxDome
	/// \param env
	///
	vxDome(std::shared_ptr<vxBitMap2d> env);
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

	//renderable interface
	bool throwRay(const vxRay &) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &) const;
};

}
#endif // VXDOM_H
