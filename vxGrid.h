#ifndef _VXGRIDMC_
#define _VXGRIDMC_
#include <memory>

#include <bitset>
#include <climits>
#include <cmath>

#include "vxObject.h"
#include "vxGeometry.h"
#include "vxGlobal.h"

#include <time.h>
#include <iostream>
#include <vector>
#include "vxRay.h"

#include "MathUtils.h"
#include "vxThreadPool.h"

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

struct vx
{
	unsigned char c{0b0000'0000};

	inline void activate(bool active)
	{
		active ? activate() : deactivate();
	}

	inline bool active() const
	{
		return (bool)c;
	}

	inline void activate()
	{
		c|=0b0000'0001;
	}
	
	inline void deactivate()
	{
		c&=0b0000'0000;
	}

	inline unsigned char colorIndex() const
	{
		return c;
	}

	inline void setColorIndex(const unsigned char ci)
	{
		c=ci|0b00000001;
	}
};

class vxGrid
{
protected:

	v3 m_position;
	scalar m_size				= {5.0};
	unsigned long m_resolution	= {5u};
	scalar m_invRes				= {1/(scalar)5.0};

	std::vector<vx>	m_data;
	vxBox m_boundingBox;

	scalar m_resDivTres	= {m_size/(scalar)3.0};
	scalar m_midSize	= {m_size/(scalar)2.0};

	// cache objects
	scalar m_boxSize			= {1.0};
	scalar m_midBoxSize			= {.5};	
	unsigned long m_resXres		= {25};
	unsigned long m_resXresXres	= {125};
	scalar m_xmin		= {0.0};
	scalar m_xmax		= {0.0};
	scalar m_ymin		= {0.0};
	scalar m_ymax		= {0.0};
	scalar m_zmin		= {0.0};
	scalar m_zmax		= {0.0};

public:

	vxGrid();
	vxGrid(const v3 &position, scalar size);
	vxGrid(scalar x, scalar y,  scalar z,  scalar size);
	~vxGrid();

// GETTER AND SETTERS
	
	void createGridData(const unsigned long resolution);
	void setResolution(unsigned long resolution);
	void setSize(const scalar size);
	unsigned long size() const;
	void setPosition(const v3 position);
	v3 position() const;
	unsigned long resolution() const;
	void setBoxSize();
	void updateBB();
	void createDiagonals();
	void createCorners();
	void createRoof(unsigned long offset = 0);
	void createGround(unsigned long offset = 0);
	void createEdges();
	void fill();
	void createSphere(const v3 &center, const scalar radio);
	void createSphere(scalar x, scalar y, scalar z, const scalar radio);
	bool getRandomBoolean(scalar ratio = 1.0);
	void createRandom(scalar ratio = 1.0);
	void addGeometry(const vxGeometryHandle geo, 
					 const v3 &offset, 
					 const v3 &scaleFactor);
	void dumpFileInMemory(const std::__cxx11::string &fileName);
	void dumpNumericTypeInMemory();

// OPERATION WITH GRID
	inline unsigned long index(const unsigned long x, 
								const unsigned long y, 
								const unsigned long z) const;
	
	static v3 nextVoxel(const vxRay &ray, v3 &exactCollision);
	//sets every single vxl to 0.
	void initialize(bool value = false);
	//returns number of active voxels
	unsigned long numActiveVoxels();
	long getNumberOfVoxels() const;
	// returns true if voxel at x y z is active
	inline bool active(const v3& pos) const;
	// returns true if voxel at index is active
	inline bool active(unsigned long idx) const;
	// returns true if voxel at index is active
	inline bool active(const unsigned long x, 
					   const unsigned long y, 
					   const unsigned long z) const;
	// sets active voxel at coordinates x y z
	void activate(const unsigned long x, 
				  const unsigned long y, 
				  const unsigned long z);
	// sets active voxel at world space position
	// returns true if could find a voxel.
	bool activate(const v3 &pos);
	bool activeInRange(const v3 &pos) const;

	// sets unactive vxl at coordinates x y z
	void deactivate(const unsigned long x, 
					const unsigned long y, 
					const unsigned long z);

	// returns true if element at local coords 
	// is true
	inline bool getElement(const unsigned long x, 
						   const unsigned long y, 
						   const unsigned long z) const;
	
	void setElementColorIndex(const unsigned long x, const unsigned long y, const unsigned long z, const unsigned char c);
	// changes the value of the element at local
	// coords x y z to be same as parameter value
	inline void setElement(const unsigned long x, 
						   const unsigned long y, 
						   const unsigned long z, 
							bool value);
	inline void setElement(unsigned long idx, bool value);
	v3 getVoxelPosition(const unsigned long iX, 
								const unsigned long iY, 
								const unsigned long iZ) const;
	v3 getVoxelPosition(unsigned long idx) const;
	inline unsigned long indexAtPosition(const v3 &position) const;
	inline vx& vxAt(const unsigned long idx);
	inline vx vxAt(const unsigned long idx) const;
	inline vx& vxAtPosition(const v3 &position);
	inline vx& vxAt(const unsigned long iX, 
					const unsigned long iY, 
					const unsigned long iZ);
	vx vxAtPosition(const v3 &position) const;
	vx vxAt(const unsigned long iX, 
			const unsigned long iY, 
			const unsigned long iZ) const;

	bool inGrid(const v3 &pnt, scalar tolerance) const;
	bool inGrid(const v3 &pnt) const;
	
	unsigned int getNearestCollision(const vxRay &ray, vxCollision &collide) const;
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
	unsigned char elementColorIndex(const unsigned long x, const unsigned long y, const unsigned long z) const;
	bool bitInBufferData(const unsigned long idx) const;
};

/*
 * 
	vxGrid grid(2.5, 2.5, 2.5, 5);
	grid.setResolution(5);
	
	for(int i=0; i<5; i++)
	{
		v3 pos(i+.1,i+.1,i+.1);
		
		auto idx = grid.indexAtPosition(pos);
		
		std::cout << "index at position " << pos
				  << std::endl;

		std::cout << " is " << idx
				  << std::endl;
	}
*/

}
#endif //_VXGRIDMC_
