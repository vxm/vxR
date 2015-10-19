#include "vxGlobal.h"

vxGlobal::vxGlobal()
{
}

vxGlobal *vxGlobal::getInstance()
{
	static vxGlobal a;
	return &a;
}
