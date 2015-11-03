#ifndef VXGRIDUNITTEST_H
#define VXGRIDUNITTEST_H
#include <string>

#include "vxUnitTest.h"
#include "vxGrid.h"
#include "vxVector.h"
#include "vxRay.h"


namespace vxCore {



class vxGridUnitTest : public vxUnitTest
{
public:
	vxGridUnitTest();
	v3 testNextVoxel(v3 org, v3 dir);
	int testFunction_nextVoxel();
	static int testGrid();
};

}
#endif // VXGRIDUNITTEST_H
