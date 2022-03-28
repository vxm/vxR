#include "Global.h"

using namespace vxCore;

Global *Global::getInstance()
{
	static Global a;
	return &a;
}
