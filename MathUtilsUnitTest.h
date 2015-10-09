#ifndef MATHUTILSUNITTEST_H
#define MATHUTILSUNITTEST_H

#include <string>

#include "vxUnitTest.h"

namespace vxCore {

class MathUtilsUnitTest : public vxUnitTest
{
public:
	MathUtilsUnitTest();
	
	int testFunction_rectAndXPlane();
	static int testMathUtils();
	int testFunction_VectorBasics();
	int testFunction_Intersection();
};
}
#endif // MATHUTILSUNITTEST_H
