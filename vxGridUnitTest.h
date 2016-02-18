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
	v3s testNextVoxel(v3s org, v3s dir);
	int testFunction_nextVoxel();
	static int testGrid();
};

}
#endif // VXGRIDUNITTEST_H
