#ifndef _VXGLOBALMC_
#define _VXGLOBALMC_

#include <memory>
#include "vxObject.h"
#include "vxBox.h"
#include "vxLegoBlock.h"

namespace vxCore {

enum class Interpolation
	{kNone, kLinear, kSin};

class vxBox;
class vxLegoBlock;

class vxGlobal
{
private:
	vxGlobal();

public:
	
	static vxGlobal *getInstance();

	static std::shared_ptr<vxLegoBlock> m_lego;

	//static std::shared_ptr<vxShader> getLambert();

	vxBox* getExistingBox(const vxVector3d &position, const double size);
	static std::shared_ptr<vxLegoBlock> getExistingLegoBlock();
};

}
#endif

