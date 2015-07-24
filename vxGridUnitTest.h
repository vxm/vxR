#ifndef VXGRIDUNITTEST_H
#define VXGRIDUNITTEST_H

#include "vxUnitTest.h"
#include "vxGrid.h"
#include "vxVector.h"
#include "vxRay.h"


namespace vxCore {


class vxGridUnitTest : public vxUnitTest
{
public:
	vxGridUnitTest();
	static int testNextVoxel(vxVector3d org, vxVector3d dir);
	static int testFunction_nextVoxel();
	static int testGrid();
};

}
#endif // VXGRIDUNITTEST_H
