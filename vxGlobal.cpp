#include "vxGlobal.h"

std::shared_ptr<vxLegoBlock> vxGlobal::m_lego = std::make_shared<vxLegoBlock>();
std::shared_ptr<vxBox> vxGlobal::m_box = std::make_shared<vxBox>(false);

vxGlobal::vxGlobal()
{
}

vxGlobal *vxGlobal::getInstance()
{
	static vxGlobal a;
	return &a;
}

std::shared_ptr<vxBox> vxGlobal::getExistingBox()
{
	return vxGlobal::m_box;
}


//TODO: this needs to be questioned.
std::shared_ptr<vxLegoBlock> vxGlobal::getExistingLegoBlock()
{
	return vxGlobal::m_lego;
}
