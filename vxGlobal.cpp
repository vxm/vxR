#include "vxGlobal.h"

std::shared_ptr<vxLegoBlock> vxGlobal::m_lego = std::make_shared<vxLegoBlock>();

vxGlobal::vxGlobal()
{


vxGlobal *vxGlobal::getInstance()
{
	static vxGlobal a;
	return &a;
}

vxBox *vxGlobal::getExistingBox(const vxVector3d &position, const double size)
{
	return vxGlobal::m_box;
}}


//TODO: this needs to be questioned.
std::shared_ptr<vxLegoBlock> vxGlobal::getExistingLegoBlock()
{
	return vxGlobal::m_lego;
}
