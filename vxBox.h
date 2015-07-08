#ifndef _VXBOXMC_
#define _VXBOXMC_

#include<memory>
#include<cmath>
#include<map>
#include <thread>

#include "vxVector.h"
#include "vxRay.h"
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
	mutable std::map<std::thread::id, vxBoxInGrid> m_instance;
	bool m_useDefault = {false};
	mutable vxBoxInGrid m_default;
public:
	vxBox(bool usesDefault);

	std::shared_ptr<vxBox> at(const vxVector3d &pos, double size);
	void set(const vxVector3d &pos, double size);
	
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif
