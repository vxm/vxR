#ifndef VXDOME_H
#define VXDOME_H

#include "MathUtils.h"
#include "vxBitMap2d.h"
#include "vxRay.h"
#include "vxCollision.h"

class vxDome
{
	// Light domm
	std::shared_ptr<vxBitMap2d> m_image;
	
public:
	vxDome();
	vxDome(std::shared_ptr<vxBitMap2d> env);

	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;

};

#endif // VXDOM_H