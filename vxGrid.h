#ifndef _VXGRIDMC_
#define _VXGRIDMC_
#include <memory>

#include <bitset>
#include <climits>

#include "vxObject.h"
#include "vxGlobal.h"

#include <cmath>
#include <time.h>
#include <iostream>
#include <vector>
#include "vxRay.h"

#include "MathUtils.h"
#include "vxGlobal.h"

namespace vxCore {

class vxGrid;
class vxBox;

class vxOrthIter
{
	std::shared_ptr<vxGrid> m_grid;
	public:
		
		vxOrthIter(std::shared_ptr<vxGrid> grid)
			:m_grid(grid)
		{
			
		}
		
		
};

class vxGrid
{
protected:

	vxVector3d m_position;
	double m_size				= {5.0};
	unsigned int m_resolution	= {5u};
	double m_invRes				= {1/5.0};

	std::vector<bool>		 m_data;
	std::unique_ptr<vxBox> m_boundingBox;

	double m_resDivTres	= {m_size/3.0};
	double m_midSize	= {m_size/2.0};

	// cache objects
	double m_boxSize			= {1.0};
	double m_midBoxSize			= {.5};	
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

// GETTER AND SETTERS
	
	void createGridData(const unsigned int resolution);
	void setResolution(unsigned int resolution);
	void setSize(const double size);
	unsigned int size() const;
	void setPosition(const vxVector3d position);
	vxVector3d position() const;
	unsigned int resolution() const;
	void setBoxSize();
	void updateBB();
	void createDiagonals();
	void createCorners();
	void createGround(unsigned int offset = 0);
	void createEdges();
	void createSphere(const vxVector3d &center, const double radio);
	void createSphere(double x, double y, double z, const double radio);
	bool getRandomBoolean(double ratio = 1.0);
	void createRandom(double ratio = 1.0);
	void addVertices(const std::vector<vxVector3d> &verts, 
					 const vxVector3d &offset, 
					 const vxVector3d &scale);
	vxVector3d nextVoxel(const vxRay &ray) const;
	
// OPERATION WITH GRID
	
	//sets every single vxl to 0.
	void initialize(bool value = false);
	//returns number of active voxels
	unsigned int numActiveVoxels();
	int getNumberOfVoxels() const;
	// returns true if voxel at x y z is active
	inline bool active(const vxVector3d& pos) const;
	// returns true if voxel at index is active
	inline bool active(unsigned int idx) const;
	// returns true if voxel at index is active
	inline bool active(const unsigned int x, 
					   const unsigned int y, 
					   const unsigned int z) const;
	// sets active voxel at coordinates x y z
	void activate(const unsigned int x, 
				  const unsigned int y, 
				  const unsigned int z);
	// sets active voxel at world space position
	void activate(const vxVector3d &pos);
	bool activeInRange(const vxVector3d &pos) const;
	// sets unactive vxl at coordinates x y z
	void deactivate(const unsigned int x, 
					const unsigned int y, 
					const unsigned int z);
	// returns true if element at local coords 
	// is true
	inline bool getElement(const unsigned int x, 
						   const unsigned int y, 
						   const unsigned int z) const;
	// changes the value of the element at local
	// coords x y z to be same as parameter value
	inline void setElement(const unsigned int x, 
						   const unsigned int y, 
						   const unsigned int z, 
							bool value);
	inline void setElement(unsigned int idx, bool value);
	vxVector3d getVoxelPosition(const unsigned int iX, 
								const unsigned int iY, 
								const unsigned int iZ) const;
	vxVector3d getVoxelPosition(unsigned int idx) const;
	inline unsigned int indexAtPosition(const vxVector3d &position) const;
	bool inGrid(const vxVector3d &pnt, double tolerance) const;
	bool inGrid(const vxVector3d &pnt) const;
	
	unsigned int getNearestCollision(const vxRay &ray, vxCollision &collide) const;
	
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
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
