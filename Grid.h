#ifndef _VXGRIDMC_
#define _VXGRIDMC_
#include <memory>

#include <bitset>
#include <climits>
#include <cmath>

#include "Object.h"
#include "Geometry.h"
#include "Global.h"

#include <time.h>
#include <iostream>
#include <vector>

#include "Ray.h"
#include "TriangleMesh.h"
#include "MathUtils.h"
#include "ThreadPool.h"

namespace vxCore {

class Grid;
class Box;

struct vx
{
	unsigned char c{0b0000'0000};
	///
	/// \brief activate
	/// \param active
	///
	 void activate(bool active);
	/// 
	///
	/// \brief active
	/// \return 
	///Returns true if any bit is active
	 bool active() const;
	///
	/// \brief activeBit
	/// \param bit
	/// \return 
	///Returns true if the bit occupying the parameter
	///position is 1 otherwise false.
	 bool activeBit(unsigned int bit) const;
	///
	/// \brief activateBit
	/// \param bit
	///
	 void activateBit(unsigned int bit);
	///
	/// \brief deactivateBit
	/// \param bit
	///
	 void deactivateBit(unsigned int bit);
	///
	/// \brief activate
	///
	 void activate();
	///
	/// \brief deactivate
	///
	 void deactivate();
	///
	/// \brief byte
	/// \return 
	///
	 unsigned char byte() const;
	///
	/// \brief setByte
	/// \param ci
	///
	 void setByte(const unsigned char ci);
};

static_assert(sizeof(vx)==1, "vx size is wrong");

///
/// \brief The Voxel class
/// This is a temporary interchange struct data 
/// it is not meant to be stored in the grid data.
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

///
/// \brief The vxGrid class
/// Represents a geometry defined by grid of active voxels 
class Grid final : public Geometry
{
protected:
	std::vector<vx>	m_data;

	v3s m_position;
	scalar m_size				= {5.0};
	unsigned long m_resolution	= {5u};

	scalar m_c_invRes				= {1/(scalar)5.0};
	scalar m_c_resDivTres	= {m_size/(scalar)3.0};
	scalar m_c_midSize	= {m_size/(scalar)2.0};

	/// cache objects
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
	///
	/// \brief vxGrid
	///
	Grid();
	///
	/// \brief vxGrid
	/// \param position
	/// \param size
	///
	Grid(const v3s &position, scalar size);
	///
	/// \brief vxGrid
	/// \param x
	/// \param y
	/// \param z
	/// \param size
	///
	Grid(scalar x, scalar y,  scalar z,  scalar size);
	
	~Grid();

// GETTER AND SETTERS
	///
	/// \brief createGridData
	/// \param resolution
	///
	void createGridData(const unsigned long resolution);
	///
	/// \brief setResolution
	/// \param resolution
	///
	void setResolution(unsigned long resolution);
	///
	/// \brief setSize
	/// \param size
	///
	void setSize(const scalar size);
	///
	/// \brief size
	/// \return 
	///
	unsigned long size() const;
	///
	/// \brief setPosition
	/// \param position
	///
	void setPosition(const v3s &position);
	///
	/// \brief position
	/// \return 
	///
	v3s position() const;
	///
	/// \brief resolution
	/// \return 
	///
	unsigned long resolution() const;
	///
	/// \brief setBoxSize
	///
	void setBoxSize();
	///
	/// \brief updateBB
	///
	void updateBB();
	///
	/// \brief createDiagonals
	/// \param colorIndex
	///
	void createDiagonals(unsigned char colorIndex = 11);
	///
	/// \brief createCorners
	/// \param colorIndex
	///
	void createCorners(unsigned char colorIndex);
	///
	/// \brief createRoof
	/// \param offset
	/// \param colorIndex
	///
	void createRoof(unsigned long offset = 0, unsigned char colorIndex = 11);
	///
	/// \brief createGround
	/// \param offset
	/// \param colorIndex
	///
	void createGround(unsigned long offset = 0, unsigned char colorIndex = 11);
	///
	/// \brief createEdges
	/// \param colorIndex
	///
	void createEdges(unsigned char colorIndex = 12);
	///
	/// \brief fill
	/// \param colorIndex
	///
	void fill(unsigned char colorIndex = 5);
	///
	/// \brief createSphere
	/// \param center
	/// \param radio
	/// \param colorIndex
	///
	void createSphere(const v3s &center, 
					  const scalar radio,
					  unsigned char colorIndex = 11);
	///
	/// \brief createSphere
	/// \param x
	/// \param y
	/// \param z
	/// \param radio
	/// \param colorIndex
	///
	void createSphere(scalar x,
					  scalar y,
					  scalar z,
					  const scalar radio,
					  unsigned char colorIndex = 11);
	///
	/// \brief getRandomBoolean
	/// \param ratio
	/// \return 
	///
	bool getRandomBoolean(scalar ratio = 1.0);
	///
	/// \brief createRandom
	/// \param ratio
	/// \param y_threshold
	///
	void createRandom(scalar ratio = 1.0, scalar y_threshold=-10000.0);
	///
	/// \brief addGeometry
	/// \param geo
	///
	void addGeometry(const vxTriangleMeshHandle geo);
	///
	/// \brief dumpFileInMemory
	/// \param fileName
	///
	void dumpFileInMemory(const std::string &fileName);
	///
	/// \brief dumpNumericTypeInMemory
	///
	void dumpNumericTypeInMemory();
	
	
	//////////////////////////////////////////////////The game of life//////////
	///
	void markCellAsDead(vx& cell);
	///
	/// \brief markCellForGenesis
	/// \param cell
	///
	void markCellForGenesis(vx& cell);
	///
	/// \brief neighboursAlive
	/// \param idx
	/// \return 
	///
	unsigned int neighboursAlive(unsigned long long idx);
	
	///
	/// \brief playGameOfLife
	///	Any live cell with fewer than two live neighbours dies, as if caused by under-population.
	///	Any live cell with two or three live neighbours lives on to the next generation.
	///	Any live cell with more than three live neighbours dies, as if by over-population.
	///	Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
	unsigned long long playGameOfLife();
	///
	/// \brief killTheDead
	/// \return 
	///
	unsigned long long killTheDead();
	
	//////////////////////////////////////////////////The game of life//////////
	////////////////////////////////////////////////// END /////////////////////
	
	
// OPERATION WITH GRID
	///
	/// \brief index
	/// \param x
	/// \param y
	/// \param z
	/// \return 
	///
	 unsigned long index(const unsigned long x, 
								const unsigned long y, 
								const unsigned long z) const;
	///
	/// \brief nextVoxel
	/// \param ray
	/// \param sp
	/// \return 
	///
	Voxel nextVoxel(const Ray &ray, v3s &sp) const;
	///
	/// \brief initialize
	/// \param value
	///sets every single vxl to 0.
	void initialize(bool value = false);
	///
	/// \brief numActiveVoxels
	/// \return 
	///returns number of active voxels
	unsigned long numActiveVoxels();
	///
	/// \brief getNumberOfVoxels
	/// \return 
	/// returns true if voxel at x y z is active
	long getNumberOfVoxels() const;
	///
	/// \brief active
	/// \param pos
	/// \return 
	/// returns true if voxel at index is active
	///
	 bool active(const v3s& pos) const;
	///
	/// \brief active
	/// \param idx
	/// \return 
	///
	 bool active(unsigned long idx) const;
	///
	////// \brief active
	////// \param x
	////// \param y
	////// \param z
	////// \return 
	// returns true if voxel at index is active
	 bool active(const unsigned long x, 
					   const unsigned long y, 
					   const unsigned long z) const;
	///
	/// \brief activate
	/// \param x
	/// \param y
	/// \param z
	/// sets active voxel at coordinates x y z
	void activate(const unsigned long x, 
				  const unsigned long y, 
				  const unsigned long z);
	///
	/// \brief activate
	/// \param pos
	/// \return 
	/// sets active voxel at world space position
	/// returns true if could find a voxel.
	bool activate(const v3s &pos);
	///
	/// \brief activeInRange
	/// \param pos
	/// \return 
	///
	bool activeInRange(const v3s &pos) const;
	///
	/// \brief deactivate
	/// \param x
	/// \param y
	/// \param z
	/// sets unactive vxl at coordinates x y z
	void deactivate(const unsigned long x, 
					const unsigned long y, 
					const unsigned long z);
	///
	/// \brief getElement
	/// \param x
	/// \param y
	/// \param z
	/// \return 
	/// returns true if element at local coords 
	/// is true
	 bool getElement(const unsigned long x, 
						   const unsigned long y, 
						   const unsigned long z) const;
	///
	/// \brief setElementColorIndex
	/// \param x
	/// \param y
	/// \param z
	/// \param c
	///
	void setElementColorIndex(const unsigned long x, const unsigned long y, const unsigned long z, const unsigned char c);
	///
	/// \brief setElement
	/// \param x
	/// \param y
	/// \param z
	/// \param value
	/// changes the value of the element at local
	/// coords x y z to be same as parameter value
	 void setElement(const unsigned long x, 
						   const unsigned long y, 
						   const unsigned long z, 
							bool value);
	///
	/// \brief setElement
	/// \param idx
	/// \param value
	///
	 void setElement(unsigned long idx, bool value);
	///
	/// \brief getVoxelPosition
	/// \param iX
	/// \param iY
	/// \param iZ
	/// \return 
	///
	v3s getVoxelPosition(const unsigned long iX, 
								const unsigned long iY, 
								const unsigned long iZ) const;
	///
	/// \brief getVoxelPosition
	/// \param idx
	/// \return 
	///
	v3s getVoxelPosition(unsigned long long idx) const;
	///
	/// \brief indexAtPosition
	/// \param pos
	/// \return 
	///
	 unsigned long indexAtPosition(const v3s &pos) const;
	///
	/// \brief vxAt
	/// \param idx
	/// \return 
	///
	 vx& vxAt(const unsigned long idx);
	///
	/// \brief vxAt
	/// \param idx
	/// \return 
	///
	 vx vxAt(const unsigned long idx) const;
	///
	/// \brief vxAtPosition
	/// \param position
	/// \return 
	///
	 vx& vxAtPosition(const v3s &position);
	///
	/// \brief vxAt
	/// \param iX
	/// \param iY
	/// \param iZ
	/// \return 
	///
	 vx& vxAt(const unsigned long iX, 
					const unsigned long iY, 
					const unsigned long iZ);
	///
	/// \brief vxAtPosition
	/// \param position
	/// \return 
	///
	vx vxAtPosition(const v3s &position) const;
	///
	/// \brief vxAt
	/// \param iX
	/// \param iY
	/// \param iZ
	/// \return 
	///
	vx vxAt(const unsigned long iX, 
			const unsigned long iY, 
			const unsigned long iZ) const;
	///
	/// \brief inGrid
	/// \param pnt
	/// \param tolerance
	/// \return 
	///
	bool inGrid(const v3s &pnt, scalar tolerance) const;
	///
	/// \brief inGrid
	/// \param pnt
	/// \return 
	///
	bool inGrid(const v3s &pnt) const;
	///
	/// \brief elementColorIndex
	/// \param x
	/// \param y
	/// \param z
	/// \return 
	///
	unsigned char elementColorIndex(const unsigned long x, const unsigned long y, const unsigned long z) const;
	///
	/// \brief bitInBufferData
	/// \param idx
	/// \return 
	///
	bool bitInBufferData(const unsigned long idx) const;
	///
	/// \brief getComponentsOfIndex
	/// \param idx
	/// \param retx
	/// \param rety
	/// \param retz
	///
	void getComponentsOfIndex(const unsigned long long idx, 
							  long &retx, 
							  long &rety, 
							  long &retz) const;
	
	//renderable interface
	virtual bool throwRay(const Ray &ray) const override;
	virtual int throwRay(const Ray &ray, Collision &col) const override;
	virtual bool hasCollision(const Ray &ray) const override;
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
