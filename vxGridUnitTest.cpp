#include <sstream>
#include <stdlib.h>
#include "vxGridUnitTest.h"

using namespace vxCore;
using namespace std::string_literals;


vxVector3d vxGridUnitTest::testNextVoxel(vxVector3d org, vxVector3d dir)
{
	const vxRay z{org, dir};
	vxVector3d exactIntersection;
	auto nv = vxGrid::nextVoxel(z, exactIntersection);
	
	//if(verbose())
	{
		std::cout << "Next voxel from:" 
				  << z.origin()
				  << " direction " 
				  << z.direction() 
				  << std::endl;
		
		std::cout << "Centered at " << nv << std::endl;
	}
	
	return nv;
}

int vxGridUnitTest::testFunction_nextVoxel()
{
	std::cout << "Init grid:: nextVoxel tests" << std::endl;	
	vxStatus st;

	{// 
		const vxVector3d origin{vxVector3d(1.2, -1.5, -1.95)};
		const vxVector3d direction{vxVector3d(0.1, -0.1, -0.1)};
		auto nv = testNextVoxel(origin, direction);

		auto f = [nv]{return nv == vxVector3d(0.57735, -0.57735, -0.57735);};
		st.examine(f, "mid voxel, Y-"s);
	}

	{// 
		const vxVector3d origin{vxVector3d(1.1, 0.5, -1.95)};
		const vxVector3d direction{vxVector3d(0.1, -0.4, -1.0)};
		auto nv = testNextVoxel(origin, direction);

		auto f = [nv]{return nv == vxVector3d(4.5, 4.5, 3.5);};
		st.examine(f, "mid voxel, Y-"s);
	}
	
	{// 
		const vxVector3d origin{vxVector3d(4.5, 4.5, 4.5)};
		const vxVector3d direction{vxVector3d(0.0, 0.0, -1.0)};
		auto nv = testNextVoxel(origin, direction);

		auto f = [nv]{return nv == vxVector3d(4.5, 4.5, 3.5);};
		st.examine(f, "mid voxel, Y-"s);
	}

	{
		const vxVector3d origin{vxVector3d(.5,.5,.5)};
		
		{// +Z
			const vxVector3d direction{vxVector3d(0.0,0.0,1.0)};
			auto nv = testNextVoxel(origin, direction);
		
			auto f = [nv]{return nv == vxVector3d(0.5, 0.5, 1.5);};
			st.examine(f, "mid voxel, Z+"s);
		}
	
		{// -Z
			const vxVector3d direction{vxVector3d(0.0,0.0,-1.0)};
			auto nv = testNextVoxel(origin, direction);
		
			auto f = [nv]{return nv == vxVector3d(0.5, 0.5, -0.5);};
			st.examine(f, "mid voxel, Z-"s);
		}
	
		{// +X
			const vxVector3d direction{vxVector3d(1.0,0.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == vxVector3d(1.5, 0.5, 0.5);};
			st.examine(f, "mid voxel, X+"s);
		}
		
		{// -X
			const vxVector3d direction{vxVector3d(-1.0,0.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == vxVector3d(-0.5, 0.5, 0.5);};
			st.examine(f, "mid voxel, X-"s);
		}
	
		{// +Y
			const vxVector3d direction{vxVector3d(0.0,1.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == vxVector3d(0.5, 1.5, 0.5);};
			st.examine(f, "mid voxel, Y+"s);
		}
		
		{// -Y
			const vxVector3d direction{vxVector3d(0.0,-1.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == vxVector3d(0.5, -0.5, 0.5);};
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
