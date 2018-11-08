#ifndef VXGRIDUNITTEST_H
#define VXGRIDUNITTEST_H
#include <string>

#include "UnitTest.h"
#include "Grid.h"
#include "Vector.h"
#include "Ray.h"


namespace vxCore {



class GridUnitTest : public UnitTest
{
public:
	GridUnitTest();
	v3s testNextVoxel(const v3s &org, const v3s &dir);
	int testFunction_nextVoxel();
	static int testGrid();
};

}
#endif // VXGRIDUNITTEST_H
