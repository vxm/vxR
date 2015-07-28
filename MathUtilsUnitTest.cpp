#include "MathUtilsUnitTest.h"

using namespace vxCore;
using namespace std::string_literals;


MathUtilsUnitTest::MathUtilsUnitTest()
{
}

int MathUtilsUnitTest::testFunction_rectAndXPlane()
{
	std::cout << "Init grid:: rectAndXPlane tests"s << std::endl;
	vxStatus st;

	// TEST 1
	{
		const auto origin{vxVector3d(4.5, 4.5, 4.5)};
		const auto direction{vxVector3d(1.0, 0.0, 0.0)};
		const auto& ray = vxRay{origin,direction};
		auto intersection = MathUtils::rayAndXPlane(ray, 13.0);
		
		std::cout << "From 4.5, 4.5, 4.5, with planeX 13 in x direction intersection is: "s
					 << intersection 
					 << std::endl;
		
		auto f = [intersection]{return intersection == vxVector3d(13, 4.5, 4.5);};
		st.examine(f, " 4.5 in all axys, direction is x=1, plane 3"s);
	}
	
	// TEST 2
	{
		const auto origin{vxVector3d(-30, 5.0, 5.0)};
		const auto direction{vxVector3d(1.0, 0.0, 0.0)};
		const auto& ray = vxRay{origin,direction};
		auto intersection = MathUtils::rayAndXPlane(ray, -20.0);
		
		std::cout << "From (-30, 5.0, 5.0), with planeX -20 in x direction intersection is: "s
					 << intersection 
					 << std::endl;
		
		auto f = [intersection]{return intersection == vxVector3d(-20, 5.0, 5.0);};
		st.examine(f, " 4.5 in all axys, direction is x=1, plane 3"s);
	}

	// TEST 3
	{
		const auto origin{vxVector3d(-30, -5.0, -5.0)};
		const auto direction{vxVector3d(-1.0, 0.0, 0.0)};
		const auto& ray = vxRay{origin,direction};
		auto intersection = MathUtils::rayAndXPlane(ray, -200.0);
		
		std::cout << "From (-30, -5.0, -5.0), with planeX -200 in x direction intersection is: "s
					 << intersection 
					 << std::endl;

		auto f = [intersection]{return intersection == vxVector3d(-200, -5.0, -5.0);};
		st.examine(f, " 4.5 in all axys, direction is x=1, plane 3"s);
	}
	
	// TEST 4
	{
		const auto origin{vxVector3d(5.0, 5.0, 5.0)};
		const auto direction{vxVector3d(10.0, 1.0, 1.0)};
		const auto& ray = vxRay{origin,direction};
		auto intersection = MathUtils::rayAndXPlane(ray, 40.0);

		std::cout << "From (5.0, 5.0, 5.0), with planeX 40 in (10.0, 1.0, 1.0) direction intersection is: "s
					 << intersection 
					 << std::endl;
		
		auto f = [intersection]{return intersection == vxVector3d(-200, -5.0, -5.0);};
		st.examine(f, " 4.5 in all axys, direction is x=1, plane 3"s);
	}
	
	if(st.isSuccess())
	{
		std::cout << "testFunction_rectAndXPlane() test - SUCCESS" << std::endl;
	}

	return 0;
}



int MathUtilsUnitTest::testMathUtils()
{
	std::cout << "Init MathUtils test" << std::endl;
	int result{0};

	static MathUtilsUnitTest mathTest;
	mathTest.setVerbose(false);
	result = mathTest.testFunction_rectAndXPlane();
	
	return result;
}


