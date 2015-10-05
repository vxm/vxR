#ifndef VXDOM_H
#define VXDOM_H

#include "MathUtils.h"
#include "vxBitMap2d.h"

class vxDom
{
	// Light domm
	std::shared_ptr<vxBitMap2d> m_image;
	
public:
	vxDom();
	vxDom(std::shared_ptr<vxBitMap2d> env);

	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;

};

#endif // VXDOM_H
