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

	static int testGrid();
};

}
#endif // VXGRIDUNITTEST_H
