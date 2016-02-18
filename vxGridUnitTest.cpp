#include <sstream>
#include <stdlib.h>
#include "vxGridUnitTest.h"

using namespace vxCore;
using namespace std::string_literals;


v3s vxGridUnitTest::testNextVoxel(v3s org, v3s dir)
{
	vxGrid grid;
	const vxRay z{org, dir};
	v3s exactIntersection;
	auto nv = grid.nextVoxel(z, exactIntersection);
	
	//if(verbose())
	{
		std::cout << "Next voxel from:" 
				  << z.origin()
				  << " direction " 
				  << z.direction() 
				  << std::endl;
		
		std::cout << "Centered at " << nv.position << std::endl;
	}
	
	return nv.position;
}

int vxGridUnitTest::testFunction_nextVoxel()
{
	std::cout << "Init grid:: nextVoxel tests" << std::endl;	
	vxStatus st;

	{// 
		const v3s origin{v3s(1.2, -1.5, -1.95)};
		const v3s direction{v3s(0.1, -0.1, -0.1)};
		auto nv = testNextVoxel(origin, direction);

		auto f = [nv]{return nv == v3s(0.57735, -0.57735, -0.57735);};
		st.examine(f, "mid voxel, Y-"s);
	}

	{// 
		const v3s origin{v3s(1.1, 0.5, -1.95)};
		const v3s direction{v3s(0.1, -0.4, -1.0)};
		auto nv = testNextVoxel(origin, direction);

		auto f = [nv]{return nv == v3s(4.5, 4.5, 3.5);};
		st.examine(f, "mid voxel, Y-"s);
	}
	
	{// 
		const v3s origin{v3s(4.5, 4.5, 4.5)};
		const v3s direction{v3s(0.0, 0.0, -1.0)};
		auto nv = testNextVoxel(origin, direction);

		auto f = [nv]{return nv == v3s(4.5, 4.5, 3.5);};
		st.examine(f, "mid voxel, Y-"s);
	}

	{
		const v3s origin{v3s(.5,.5,.5)};
		
		{// +Z
			const v3s direction{v3s(0.0,0.0,1.0)};
			auto nv = testNextVoxel(origin, direction);
		
			auto f = [nv]{return nv == v3s(0.5, 0.5, 1.5);};
			st.examine(f, "mid voxel, Z+"s);
		}
	
		{// -Z
			const v3s direction{v3s(0.0,0.0,-1.0)};
			auto nv = testNextVoxel(origin, direction);
		
			auto f = [nv]{return nv == v3s(0.5, 0.5, -0.5);};
			st.examine(f, "mid voxel, Z-"s);
		}
	
		{// +X
			const v3s direction{v3s(1.0,0.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == v3s(1.5, 0.5, 0.5);};
			st.examine(f, "mid voxel, X+"s);
		}
		
		{// -X
			const v3s direction{v3s(-1.0,0.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == v3s(-0.5, 0.5, 0.5);};
			st.examine(f, "mid voxel, X-"s);
		}
	
		{// +Y
			const v3s direction{v3s(0.0,1.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == v3s(0.5, 1.5, 0.5);};
			st.examine(f, "mid voxel, Y+"s);
		}
		
		{// -Y
			const v3s direction{v3s(0.0,-1.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == v3s(0.5, -0.5, 0.5);};
			st.examine(f, "mid voxel, Y-"s);
		}
	}
	
	
	
	if(st.isSuccess())
	{
		std::cout << "testFunction_nextVoxel() test - SUCCESS" << std::endl;
	}

	return 0;
}


int vxGridUnitTest::testGrid()
{
	std::cout << "Init grid tests" << std::endl;
	
	static vxGridUnitTest gridTest;

	int result{0};
	
	gridTest.setVerbose(false);
	result = gridTest.testFunction_nextVoxel();
	
	return result;
}


vxGridUnitTest::vxGridUnitTest()
{
}
