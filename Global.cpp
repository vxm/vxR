#include "Global.h"

using namespace vxCore;

Global::Global()
{
}

Global *Global::getInstance()
{
	static Global a;
	return &a;
}
