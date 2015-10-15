#ifndef MUUNITTEST_H
#define MUUNITTEST_H

#include <string>

#include "vxUnitTest.h"

namespace vxCore {

class MUUnitTest : public vxUnitTest
{
public:
	MUUnitTest();
	
	int testFunction_rectAndXPlane();
	static int testMU();
	int testFunction_VectorBasics();
	int testFunction_Intersection();
};
}
#endif // MUUNITTEST_H
