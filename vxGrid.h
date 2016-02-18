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
#include "vxTriangleMesh.h"
#include "MathUtils.h"
#include "vxThreadPool.h"

namespace vxCore {

class vxGrid;
class vxBox;

struct vx
{
	unsigned char c{0b0000'0000};

	inline void activate(bool active)
	{
		active ? activate() : deactivate();
	}

	///
	/// \brief active
	/// \return 
	///Returns true if any bit is active
	inline bool active() const
	{
		return (bool)c;
	}

	///
	/// \brief activeBit
	/// \param bit
	/// \return 
	///Returns true if the bit occupying the parameter
	///position is 1 otherwise false.
	inline bool activeBit(unsigned int bit) const
	{
		switch(bit)
		{
		case 0:
			return c&0b0000'0001;
		break;
		case 1:
			return c&0b0000'0010;
		break;
		case 2:
			return c&0b0000'0100;
		break;
		case 3:
			return c&0b0000'1000;
		break;
		case 4:
			return c&0b0001'0000;
		break;
		case 5:
			return c&0b0010'0000;
		break;
		case 6:
			return c&0b0100'0000;
		break;
		case 7:
			return c&0b1000'0000;
		break;
		}

		return c&0b0000'0000;
	}
	
	inline void activateBit(unsigned int bit)
	{
		switch(bit)
		{
		case 0:
			c|=0b0000'0001;
		break;
		case 1:
			c|=0b0000'0010;
		break;
		case 2:
			c|=0b0000'0100;
		break;
		case 3:
			c|=0b0000'1000;
		break;
		case 4:
			c|=0b0001'0000;
		break;
		case 5:
			c|=0b0010'0000;
		break;
		case 6:
			c|=0b0100'0000;
		break;
		case 7:
			c|=0b1000'0000;
		break;
		}
	}
	
	inline void deactivateBit(unsigned int bit)
	{
		switch(bit)
		{
		case 0:
			c&=~0b0000'0001;
		break;
		case 1:
			c&=~0b0000'0010;
		break;
		case 2:
			c&=~0b0000'0100;
		break;
		case 3:
			c&=~0b0000'1000;
		break;
		case 4:
			c&=~0b0001'0000;
		break;
		case 5:
			c&=~0b0010'0000;
		break;
		case 6:
			c&=~0b0100'0000;
		break;
		case 7:
			c&=~0b1000'0000;
		break;
		}
	}

	inline void activate()
	{
		activateBit(7);
	}
	
	inline void deactivate()
	{
		deactivateBit(7);
		deactivateBit(6);
		deactivateBit(5);
		deactivateBit(4);

		activateBit(0);
		activateBit(1);
		activateBit(2);
	}

	inline unsigned char byte() const
	{
		return c;
	}

	inline void setByte(const unsigned char ci)
	{
		c = ci;
		activate();
	}
};

static_assert(sizeof(vx)==1, "vx size is wrong");

///
/// \brief The Voxel class
/// This is a temporary interchange struct
class Voxel
{
public:
	unsigned long long index{0ull};
	
	unsigned long x{0ul};
	unsigned long y{0ul};
	unsigned long z{0ul};
	
	vx data;
	v3s position;
	scalar size;
};


class vxGrid final : public vxGeometry
{
protected:
	std::vector<vx>	m_data;

	v3s m_position;
	scalar m_size				= {5.0};
	unsigned long m_resolution	= {5u};

	scalar m_c_invRes				= {1/(scalar)5.0};
	scalar m_c_resDivTres	= {m_size/(scalar)3.0};
	scalar m_c_midSize	= {m_size/(scalar)2.0};

	// cache objects
	scalar m_c_boxSize			= {1.0};
	scalar m_c_midBoxSize			= {.5};	
	unsigned long m_c_resXres		= {25};
	unsigned long m_c_resXresXres	= {125};
	scalar m_xmin		= {0.0};
	scalar m_xmax		= {0.0};
	scalar m_ymin		= {0.0};
	scalar m_ymax		= {0.0};
	scalar m_zmin		= {0.0};
	scalar m_zmax		= {0.0};

public:

	vxGrid();
	vxGrid(const v3s &position, scalar size);
	vxGrid(scalar x, scalar y,  scalar z,  scalar size);
	~vxGrid();

// GETTER AND SETTERS
	
	void createGridData(const unsigned long resolution);
	void setResolution(unsigned long resolution);
	void setSize(const scalar size);
	unsigned long size() const;
	void setPosition(const v3s &position);
	v3s position() const;
	unsigned long resolution() const;
	void setBoxSize();
	void updateBB();
	void createDiagonals(unsigned char colorIndex = 11);
	void createCorners(unsigned char colorIndex);
	void createRoof(unsigned long offset = 0, unsigned char colorIndex = 11);
	void createGround(unsigned long offset = 0, unsigned char colorIndex = 11);
	void createEdges(unsigned char colorIndex = 12);
	void fill(unsigned char colorIndex = 5);
	void createSphere(const v3s &center, 
					  const scalar radio,
					  unsigned char colorIndex = 11);
	
	void createSphere(scalar x,
					  scalar y,
					  scalar z,
					  const scalar radio,
					  unsigned char colorIndex = 11);
	
	bool getRandomBoolean(scalar ratio = 1.0);
	void createRandom(scalar ratio = 1.0, scalar y_threshold=-10000.0);
	void addGeometry(const vxTriangleMeshHandle geo);
	void dumpFileInMemory(const std::__cxx11::string &fileName);
	void dumpNumericTypeInMemory();
	
	
	//////////////////////////////////////////////////The game of life//////////
	
	void markCellAsDead(vx& cell);
	void markCellForGenesis(vx& cell);
	
	unsigned int neighboursAlive(unsigned long long idx);
	
	///
	/// \brief playGameOfLife
	///	Any live cell with fewer than two live neighbours dies, as if caused by under-population.
	///	Any live cell with two or three live neighbours lives on to the next generation.
	///	Any live cell with more than three live neighbours dies, as if by over-population.
	///	Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
	unsigned long long playGameOfLife();

	unsigned long long killTheDead();
	
	//////////////////////////////////////////////////The game of life//////////
	////////////////////////////////////////////////// END /////////////////////
	
	
// OPERATION WITH GRID
	inline unsigned long index(const unsigned long x, 
								const unsigned long y, 
								const unsigned long z) const;
	
	Voxel nextVoxel(const vxRay &ray, v3s &sp) const;
	//sets every single vxl to 0.
	void initialize(bool value = false);
	//returns number of active voxels
	unsigned long numActiveVoxels();
	long getNumberOfVoxels() const;
	// returns true if voxel at x y z is active
	inline bool active(const v3s& pos) const;
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
	bool activate(const v3s &pos);
	bool activeInRange(const v3s &pos) const;

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
	v3s getVoxelPosition(const unsigned long iX, 
								const unsigned long iY, 
								const unsigned long iZ) const;
	v3s getVoxelPosition(unsigned long long idx) const;
	inline unsigned long indexAtPosition(const v3s &pos) const;
	inline vx& vxAt(const unsigned long idx);
	inline vx vxAt(const unsigned long idx) const;
	inline vx& vxAtPosition(const v3s &position);
	inline vx& vxAt(const unsigned long iX, 
					const unsigned long iY, 
					const unsigned long iZ);
	vx vxAtPosition(const v3s &position) const;
	vx vxAt(const unsigned long iX, 
			const unsigned long iY, 
			const unsigned long iZ) const;

	bool inGrid(const v3s &pnt, scalar tolerance) const;
	bool inGrid(const v3s &pnt) const;
	
	unsigned char elementColorIndex(const unsigned long x, const unsigned long y, const unsigned long z) const;
	bool bitInBufferData(const unsigned long idx) const;
	
	//renderable interface
	virtual bool throwRay(const vxRay &ray) const override;
	virtual int throwRay(const vxRay &ray, vxCollision &col) const override;
	virtual bool hasCollision(const vxRay &ray) const override;
	void getComponentsOfIndex(const unsigned long long idx, 
							  long &retx, 
							  long &rety, 
							  long &retz) const;
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
