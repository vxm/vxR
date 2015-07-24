#include "vxGridUnitTest.h"

using namespace vxCore;

int vxGridUnitTest::testNextVoxel(vxVector3d org, vxVector3d dir)
{
	const vxRay z{org, dir};
	auto nv = vxGrid::nextVoxel(z);
	
	std::cout << "Next voxel from:" 
			  << z.origin()
			  << " direction " 
			  << z.direction() 
			  << std::endl;	
	
	std::cout << "Centered at " << nv << std::endl;
	
	return 1;
}

int vxGridUnitTest::testFunction_nextVoxel()
{
	std::cout << "Init grid:: nextVoxel tests" << std::endl;	

	{// +Z
		const auto origin{vxVector3d(.5,.5,.5)};
		const auto direction{vxVector3d(0.0,0.0,1.0)};
		testNextVoxel(origin, direction);
	}

	{// +Y
		const auto origin{vxVector3d(.5,.5,.5)};
		const auto direction{vxVector3d(0.0,1.0,0.0)};
		testNextVoxel(origin, direction);
	}

	{// +X
		const auto origin{vxVector3d(.5,.5,.5)};
		const auto direction{vxVector3d(1.0,0.0,0.0)};
		testNextVoxel(origin, direction);
	}

	{// -Z
		const auto origin{vxVector3d(.5,.5,.5)};
		const auto direction{vxVector3d(0.0,0.0,-1.0)};
		testNextVoxel(origin, direction);
	}

	{// -Y
		const auto origin{vxVector3d(.5,.5,.5)};
		const auto direction{vxVector3d(0.0,-1.0,0.0)};
		testNextVoxel(origin, direction);
	}

	{// -X
		const auto origin{vxVector3d(.5,.5,.5)};
		const auto direction{vxVector3d(-1.0,0.0,0.0)};
		testNextVoxel(origin, direction);
	}
	
	{// 
		const auto origin{vxVector3d(.5,.5,.5)};
		const auto direction{vxVector3d(0.5,-0.9,0.1)};
		testNextVoxel(origin, direction);
	}	
	return 0;
}


int vxGridUnitTest::testGrid()
{
	std::cout << "Init grid tests" << std::endl;	
	int result{0};
	
	result = testFunction_nextVoxel();
	
	return result;
}


vxGridUnitTest::vxGridUnitTest()
{
}
