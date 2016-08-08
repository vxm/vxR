#ifndef _VXGLOBALMC_
#define _VXGLOBALMC_

#include <memory>
#include "Object.h"
#include "Box.h"
#include "LegoBlock.h"

namespace vxCore {

enum class Interpolation
	{kNone, kLinear, kSin};

class Box;
class LegoBlock;

class Global
{
private:
	Global();

public:
	
	static Global *getInstance();

	//static vxShaderHandle getLambert();
};

}
#endif

