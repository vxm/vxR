#ifndef VXLEGOBLOCK_H
#define VXLEGOBLOCK_H

#include<memory>
#include<cmath>
#include<map>
#include <thread>

#include "vxVector.h"
#include "vxRay.h"
#include "vxBox.h"

namespace vxCore{

class vxLegoBlockInGrid
{
private:
	vxVector3d m_pos;
	double m_size;
public:
	vxLegoBlockInGrid()
	{}
	vxVector3d position() const;
	void setPosition(const vxVector3d &value);
	double size() const;
	void setSize(double value);
};

class vxLegoBlock: public std::enable_shared_from_this<vxLegoBlock>
{
private:
	mutable std::map<std::thread::id, vxLegoBlockInGrid> m_instance;
	bool m_useDefault = {false};
	mutable vxLegoBlockInGrid m_default;

	double m_boxHeigh{0.7};
	double m_cRadius{0.3};
	
public:
	vxLegoBlock();

	std::shared_ptr<vxLegoBlock> at(const vxVector3d &pos, double size);
	void set(const vxVector3d &pos, double size);
	
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
	
	double boxHeigh() const;
	void setBoxHeigh(double boxHeigh);
	double cRadius() const;
	void setCRadius(double cRadius);
};

}
#endif // VXLEGOBLOCK_H
