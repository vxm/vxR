#ifndef MATHUTILSUNITTEST_H
#define MATHUTILSUNITTEST_H

#include <string>

#include "vxUnitTest.h"
#include "MathUtils.h"

namespace vxCore {

class MathUtilsUnitTest : public vxUnitTest
{
public:
	MathUtilsUnitTest();
	
	int testFunction_rectAndXPlane();
	static int testMathUtils();
};
}
#endif // MATHUTILSUNITTEST_H
