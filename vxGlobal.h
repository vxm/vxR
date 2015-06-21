#ifndef _VXGLOBALMC_
#define _VXGLOBALMC_

#include <memory>
#include "vxObject.h"
#include "vxBox.h"
#include "vxLegoBlock.h"

namespace vxCore {

enum class Interpolation
	{kNone, kLinear, kSin};

class vxBoxN1;
class vxBoxN4;
class vxBoxN5;
class vxBoxN8;
class vxBoxN20;
class vxBoxN17;
class vxBoxN12;
class vxBoxN16;
class vxBoxN24;
class vxBoxN;
class vxLegoBlock;

class vxGlobal
{
private:
	vxGlobal();

public:
	
	static vxGlobal *getInstance();

// probar si deja siendo privado.
	vxBoxN1 *m_num1;
	vxBoxN4 *m_num4;
	vxBoxN5 *m_num5;
	vxBoxN8 *m_num8;
	vxBoxN20 *m_num20;
	vxBoxN17 *m_num17;
	vxBoxN12 *m_num12;
	vxBoxN16 *m_num16;
	vxBoxN24 *m_num24;
	
	vxBoxN* getExistingBox(const vxVector3d &position, const double size);

	static std::shared_ptr<vxLegoBlock> m_lego;
	//static std::shared_ptr<vxShader> getLambert();

	static vxBoxN *getBox(const vxVector3d &position, const double size);

	static vxBoxN *getBox(const double x,
						  const double y, 
						  const double z, 
						  const double size);
	static std::shared_ptr<vxLegoBlock> getExistingLegoBlock(const vxVector3d &position, const double size);
};

}
#endif

