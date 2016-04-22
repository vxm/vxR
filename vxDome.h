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
	
public:
	vxDome();
	vxDome(std::shared_ptr<vxBitMap2d> env);

	
	//renderable interface
	bool throwRay(const vxRay &) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &) const;

	scalar gain() const;
	void setGain(const scalar &gain);
	scalar gamma() const;
	void setGamma(const scalar &gamma);
};

}
#endif // VXDOM_H
