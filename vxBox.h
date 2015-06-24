#ifndef _VXBOXMC_
#define _VXBOXMC_

#include<memory>
#include<cmath>

#include "vxVector.h"
#include "vxCollision.h"
#include "MathUtils.h"



namespace vxCore{

class vxBoxInGrid
{
private:
	vxVector3d m_pos;
	double m_size;
public:
	vxBoxInGrid()
	{}
	vxVector3d position() const;
	void setPosition(const vxVector3d &value);
	double size() const;
	void setSize(double value);
};

class vxBox: public std::enable_shared_from_this<vxBox>
{
private:
	vxBoxInGrid m_instance;
	
public:
	vxBox();

	std::shared_ptr<vxBox> at(const vxVector3d &pos, double size);
	void set(const vxVector3d &pos, double size);
	
	int throwRay(const vxRayXYZ &ray, vxCollision &collide) const;
	bool hasCollision(const vxRayXYZ &ray) const;
};

}
#endif
