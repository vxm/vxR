#ifndef _VXGRIDMC_
#define _VXGRIDMC_

#include <memory>
#include <bitset>

#include "vxObject.h"
#include "vxGlobal.h"


#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>

#include "MathUtils.h"
#include "vxGlobal.h"

namespace vxCore {

class vxGrid;
class vxBoxN;

class vxOrthIter
{
	std::shared_ptr<vxGrid> m_grid;
	public:
		
		vxOrthIter(std::shared_ptr<vxGrid> grid)
			:m_grid(grid)
		{
			
		}
		
		
};

class vxGrid:public vxObject
{
protected:

	vxVector3d m_position;
	double m_size				= {5.0};
	unsigned int m_resolution	= {5};
	double m_invRes				= {1/5.0};

	std::vector<bool>		 m_data;
	std::unique_ptr<vxBoxN> m_boundingBox;

	double m_resDivTres	= {m_size/3.0};
	double m_midSize	= {m_size/2.0};

	// cache objects
	double m_boxSize			= {1.0};
	double m_midBoxSize	= {.5};	
	unsigned int m_resXres		= {25};
	unsigned int m_resXresXres	= {125};
	double m_xmin		= {0.0};
	double m_xmax		= {0.0};
	double m_ymin		= {0.0};
	double m_ymax		= {0.0};
	double m_zmin		= {0.0};
	double m_zmax		= {0.0};

public:

	vxGrid();
	vxGrid(const vxVector3d &position, double size);
	vxGrid(double x, double y,  double z,  double size);
	~vxGrid();

	void createGridData(const unsigned int resolution);
	
	void setResolution(unsigned int resolution);

	void setSize(const double size);

	void setPosition(const vxVector3d position);

	void setBoxSize();

	void updateBB();
	
	void createDiagonals();
	
	void createCorners();
	
	void createGround(unsigned int offset = 0);

	void createEdges();
	
	void createSphere(const vxVector3d &center, const double radio);

	void createSphere(int x, int y, int z, const double radio);

	bool getRandomBoolean(double ratio = 1.0);

	void createRandom(double ratio = 1.0);

	//sets every single vxl to 0.
	void initialize(bool value = false);
	
	//returns number of active voxels
	unsigned int numActiveVoxels();
	
	int getNumberOfVoxels() const;

	// returns true if voxel at x y z is active
	inline bool active( int x,  int y,  int z) const;
	
	// returns true if voxel at index is active
	inline bool active(unsigned int idx) const;

	// sets active voxel at coordinates x y z
	void activate(const int x, const int y, const int z);

	// sets unactive vxl at coordinates x y z
	void deactivate(const int x, const int y, const int z);

	// returns true if element at local coords 
	// is true
	inline bool getElement(int x,int y,int z) const;

	// changes the value of the element at local
	// coords x y z to be same as parameter value
	inline void setElement(int x, int y, int z, bool value);

	vxVector3d getVoxelPosition(int x, int y, int z) const;

	vxVector3d getVoxelPosition(unsigned int idx) const;

	inline unsigned int indexAtPosition(const vxVector3d &position) const;

	bool inGrid(const vxVector3d &pnt, double tolerance = 0.0) const;
	
	void getNearestCollision(const vxRayXYZ &ray, vxCollision &collide);
	
	//!! this shouldn't be like this
	void getNearestCollisionUsingX(const vxRayXYZ &ray, vxCollision &collide);

	
	//!! this shouldn't be like this
	void getNearestCollisionUsingY(const vxRayXYZ &ray, vxCollision &collide);

	
	//!! this shouldn't be like this
	void getNearestCollisionUsingZ(const vxRayXYZ &ray, vxCollision &collide);
	
	//!!	Brute Force search.
	//! what a shame.
	void getNearestCollisionBF(const vxRayXYZ &ray, vxCollision &collide);

	// devuelve 0 si no le da a la caja
	int throwRay(const vxRayXYZ &ray, vxCollision &collide);
	
	bool hasCollision(const vxVector3d &origin, const vxRayXYZ &ray);
};

/*
 * 
	vxGrid grid(2.5, 2.5, 2.5, 5);
	grid.setResolution(5);
	
	for(int i=0; i<5; i++)
	{
		vxVector3d pos(i+.1,i+.1,i+.1);
		
		auto idx = grid.indexAtPosition(pos);
		
		std::cout << "index at position " << pos
				  << std::endl;

		std::cout << " is " << idx
				  << std::endl;
	}
*/

}
#endif //_VXGRIDMC_
