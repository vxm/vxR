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

	const auto origin{vxVector3d(4.5, 4.5, 4.5)};
	{// 
		const auto direction{vxVector3d(0.0, 0.0, -1.0)};
		auto nv = testNextVoxel(origin, direction);

		auto f = [nv]{return nv == vxVector3d(.5, -0.5, 0.5);};
		st.examine(f, "mid voxel, Y-"s);
	}

	{
		const auto origin{vxVector3d(.5,.5,.5)};
		
		{// +Z
			const auto direction{vxVector3d(0.0,0.0,1.0)};
			auto nv = testNextVoxel(origin, direction);
		
			auto f = [nv]{return nv == vxVector3d(0.5, 0.5, 1.5);};
			st.examine(f, "mid voxel, Z+"s);
		}
	
		{// -Z
			const auto direction{vxVector3d(0.0,0.0,-1.0)};
			auto nv = testNextVoxel(origin, direction);
		
			auto f = [nv]{return nv == vxVector3d(0.5, 0.5, -0.5);};
			st.examine(f, "mid voxel, Z-"s);
		}
	
		{// +X
			const auto direction{vxVector3d(1.0,0.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == vxVector3d(1.5, 0.5, 0.5);};
			st.examine(f, "mid voxel, X+"s);
		}
		
		{// -X
			const auto direction{vxVector3d(-1.0,0.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == vxVector3d(-0.5, 0.5, 0.5);};
			st.examine(f, "mid voxel, X-"s);
		}
	
		{// +Y
			const auto direction{vxVector3d(0.0,1.0,0.0)};
			auto nv = testNextVoxel(origin, direction);
	
			auto f = [nv]{return nv == vxVector3d(0.5, 1.5, 0.5);};
			st.examine(f, "mid voxel, Y+"s);
		}
		
		{// -Y
			const auto direction{vxVector3d(0.0,-1.0,0.0)};
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
