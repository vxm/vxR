#include "MathUtilsUnitTest.h"
#include "MathUtils.h"

using namespace vxCore;
using namespace std::string_literals;


MathUtilsUnitTest::MathUtilsUnitTest()
{
}


int MathUtilsUnitTest::testFunction_Intersection()
{
	std::cout << "Init:: Intersections tests"s << std::endl;
	vxStatus st;

	// TEST 6 //Ray and plane from 3 points
	{
		const auto a{vxVector3d(1.0, 2.0, 1.0)};
		const auto b{vxVector3d(4.0, 2.0, 1.0)};
		const auto c{vxVector3d(9.0, 5.0, 6.0)};
		
		const vxRay ray(vxVector3d(10,10,10), vxVector3d(9,8,9));
		
		const vxVector3d r = MathUtils::rectAndPlane(ray, a, b, c);

		std::cout << "A (" << a << "), B (" << b << "), C (" << c <<") : "s
					 << " ray (" 
					 << ray
					 << ") result for test is:"
					 << r
					 << std::endl;
		
		/*
		 * Wolfram Alpha
		 * intersection plane ([1.0, 2.0, 3.0],[4.0, 2.0, 1.0],[4.0, 5.0, 6.0]) line([10,10,10],[-10,-10,-10])
		 *	6 -15 9
		*/
		auto f = [r]{return r == vxVector3d(7, 10, -13.0);};
		st.examine(f, " result must be (7, 10, -13.0)"s);
	}
	
	if(st.isSuccess())
	{
		std::cout << "testFunction_rectAndXPlane() test - SUCCESS" << std::endl;
	}
	
	return 0;
}
	
int MathUtilsUnitTest::testFunction_VectorBasics()
{
	std::cout << "Init:: Vector Basics tests"s << std::endl;
	vxStatus st;

	// TEST 5 //Cross product
	{
		const vxVector3d a{vxVector3d(5.0, 3.0, 5.0)};
		const vxVector3d b{vxVector3d(6.0, 1.0, 4.0)};
		const vxVector3d n = a^b;
	
		std::cout << "A (" << a << "), B (" << b <<") cross is: "s
					 << n
					 << std::endl;
		/*
		 *	octave:1> a = [5,3,5]
		 *	octave:2> b = [6,1,4]
		 *	octave:3> n = cross(a,b)
		 *	n =
		 *		7   10  -13
		*/
		auto f = [n]{return n == vxVector3d(7, 10, -13.0);};
		st.examine(f, " result must be (7, 10, -13.0)"s);
	}
	
	
	if(st.isSuccess())
	{
		std::cout << "testFunction_rectAndXPlane() test - SUCCESS" << std::endl;
	}

	return 0;
}

int MathUtilsUnitTest::testFunction_rectAndXPlane()
{
	std::cout << "Init:: rectAndXPlane tests"s << std::endl;
	vxStatus st;

	// TEST 1
	{
		const vxVector3d origin{vxVector3d(4.5, 4.5, 4.5)};
		const vxVector3d direction{vxVector3d(1.0, 0.0, 0.0)};
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
		const vxVector3d origin{vxVector3d(-30, 5.0, 5.0)};
		const vxVector3d direction{vxVector3d(1.0, 0.0, 0.0)};
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
		const vxVector3d origin{vxVector3d(-30, -5.0, -5.0)};
		const vxVector3d direction{vxVector3d(-1.0, 0.0, 0.0)};
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
		const vxVector3d origin{vxVector3d(5.0, 5.0, 5.0)};
		const vxVector3d direction{vxVector3d(10.0, 1.0, 1.0)};
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
	result += mathTest.testFunction_VectorBasics();
	result += mathTest.testFunction_Intersection();
	return result;
}


