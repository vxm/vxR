#include "vxGlobal.h"

using namespace vxCore;

vxGlobal::vxGlobal()
{
}

vxGlobal *vxGlobal::getInstance()
{
	static vxGlobal a;
	return &a;
}
