#ifndef _VXBOXMATRIXMC_
#define _VXBOXMATRIXMC_

#include <memory>
#include <bitset>

#include "vxObject.h"
#include "vxGlobal.h"
#include "vxBox.h"

#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>

#include<MathUtils.h>

namespace vxStorage {

class vxGrid;
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
	double m_boxSize			= {1.0};
	unsigned int m_resolution	= {5};
	double m_invRes				= {1/5.0};

	std::vector<bool>		 m_data;
	std::unique_ptr<vxBoxN> m_boundingBox;

	double m_resDivTres	= {m_size/3.0};
	double m_midSize	= {m_size/2.0};

	// cache objects
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

	vxGrid()
	{
		m_size=1;

		createGridData(5);
		initialize();
		updateBB();
		srand(time(NULL));
	}
	
	vxGrid(const vxVector3d &position, double size)
		: m_position(position)
	{
		setSize(size);
		createGridData(5);

		initialize();
		updateBB();
		srand(time(NULL));
	}
	
	vxGrid(double x, double y,  double z,  double size)
	{ 
		this->m_position.set(x,y,z);
		setSize(size);

		createGridData(5);

		initialize();
		updateBB();
		srand(time(NULL));
	}
	
	~vxGrid()
	{}

	void createGridData(const unsigned int resolution)
	{
		m_resolution = resolution;
		m_resXres=resolution*resolution;
		m_resXresXres=m_resXres*resolution;
		//m_data=(bool*)malloc(sizeof(bool)*m_resXresXres);
		m_data.resize(m_resXresXres, false);
		//invRes=1/resolution;
		m_resDivTres = m_midSize/(double)m_resolution;
		setBoxSize();
		
		m_xmin = m_position.x() - m_midSize;
		m_xmax = m_position.x() + m_midSize;
		m_ymin = m_position.y() - m_midSize;
		m_ymax = m_position.y() + m_midSize;
		m_zmin = m_position.z() - m_midSize;
		m_zmax = m_position.z() + m_midSize;
		
	}
	
	void setResolution(unsigned int resolution)
	{	
		if(resolution!=m_resolution)
		{
			createGridData(resolution);
		}
	}

	void setSize(const double size)
	{
		this->m_size=size;
		m_midSize = size/2.0;
		setBoxSize();
	}

	void setPosition(const vxVector3d position)
	{
		this->m_position=position;
	}

	void setBoxSize()
	{
		m_boxSize = m_size/double(m_resolution);
		m_midBoxSize = m_boxSize/2.0;
	}

	void updateBB()
	{
		//TODO:c++14 make unique
		m_boundingBox.reset(vxGlobal::getBox(m_position, m_size));
	}
	
	void createDiagonals()
	{
		unsigned int resminusone=m_resolution-1;
		for (unsigned int i=0;i<m_resolution;i++)
		{
			activate(i,i,i);
			activate(resminusone-i,resminusone-i,i);
			activate(i,resminusone-i,resminusone-i);
			activate(resminusone-i,i,resminusone-i);
		}
	}
	
	
	void createCorners()
	{
		unsigned int resminusone=m_resolution-1;
		activate(resminusone, resminusone, resminusone);
		activate(resminusone, resminusone,			 0);
		activate(resminusone,	0,			resminusone);
		activate(resminusone,	0,					0);
		activate(0, resminusone, resminusone);
		activate(0, resminusone,			 0);
		activate(0,	0,			resminusone);
		activate(0,	0,					0);	}
	
	void createGround(unsigned int offset = 0);

	void createEdges()
	{
		unsigned int resminusone=m_resolution-1;
		for (unsigned int i=0;i<m_resolution;i++)
		{
			activate(i,0,0);
			activate(i,resminusone,resminusone);
			activate(i,0,resminusone);
			activate(i,resminusone,0);

			activate(0,i,0);
			activate(resminusone,i,resminusone);
			activate(0,i,resminusone);
			activate(resminusone,i,0);

			activate(0,0,i);
			activate(resminusone,resminusone,i);
			activate(resminusone,0,i);
			activate(0,resminusone,i);
		}
	}
	
	void createSphere(const vxVector3d &center, const double radio)
	{
		unsigned int x, y, z;
		
		for(x=0;x<m_resolution;x++)
		{
			for(y=0;y<m_resolution;y++)
			{
				for(z=0;z<m_resolution;z++)
				{
					if(center.distance(getVoxelPosition(x, y, z))<radio)
					{
						setElement(x,y,z,true);
					}
				}
			}
		}
	}

	void createSphere(int x, int y, int z, const double radio)
	{
		createSphere(vxVector3d(x,y,z), radio);
	}

	bool getRandomBoolean(double ratio = 1.0)
	{
		double num = pow((rand()/(double)RAND_MAX), 1.0/ratio);
		return num>.5;
	}

	void createRandom(double ratio = 1.0)
	{
		for(auto it = begin(m_data);
				it!= end(m_data);
				++it)
		{
			if(getRandomBoolean(ratio))
			{
				*it = true;
			}
		}
	}

	//sets every single vxl to 0.
	void initialize(bool value = false)
	{
		for(unsigned int i=0;i<m_data.size();i++)
		{
			m_data[i]=value;
		}
	}
	
	//returns number of active voxels
	unsigned int numActiveVoxels()
	{
		unsigned int av{0};
		
		for(int i=0;i<m_data.size();i++)
		{
			if(m_data[i])
				av++;
		}
		
		return av;
	}
	
	int getNumberOfVoxels() const
	{
		return m_resXresXres;
	}

	// returns true if voxel at x y z is active
	inline bool active( int x,  int y,  int z) const
	{
		return getElement(x,y,z);
	}
	
	// returns true if voxel at index is active
	inline bool active(unsigned int idx) const
	{
		if (idx<m_resXresXres)
			return m_data[idx];
		else
			return false;
	}

	// sets active voxel at coordinates x y z
	void activate(const int x, const int y, const int z)
	{
		setElement(x,y,z,true);
	}

	// sets unactive vxl at coordinates x y z
	void deactivate(const int x, const int y, const int z)
	{
		setElement(x,y,z,false);
	}

	// returns true if element at local coords 
	// is true
	inline bool getElement(int x,int y,int z) const
	{
		auto p = x+(y*m_resolution)+(z*m_resXres);
		return m_data[p];
	}

	// changes the value of the element at local
	// coords x y z to be same as parameter value
	inline void setElement(int x, int y, int z, bool value)
	{
		m_data[x+(y*m_resolution)+(z*m_resXres)]=value;
	}

	vxVector3d getVoxelPosition(int x, int y, int z) const
	{
		double retx = (m_position.x() - m_midSize) + (x * m_boxSize) + m_resDivTres;
		double rety = (m_position.y() - m_midSize) + (y * m_boxSize) + m_resDivTres;
		double retz = (m_position.z() - m_midSize) + (z * m_boxSize) + m_resDivTres;
		return vxVector3d(retx, rety, retz);
	}

	vxVector3d getVoxelPosition(unsigned int idx) const
	{
		int retz = idx / m_resXres;
		int rety = (idx%m_resXres) / m_resolution;
		int retx = idx % m_resolution;

		return getVoxelPosition(retx, rety, retz);
	}
		
	inline unsigned int indexAtPosition(const vxVector3d &position) const
	{
		vxVector3d pos = position - (m_position-m_midSize); 
		unsigned int idx = floor(pos.x());
		idx += floor(pos.y()) * m_resolution;
		idx += floor(pos.z()) * m_resXres;
		return idx;
	}

	bool inGrid(const vxVector3d &pnt) const;
	
	void getNearestCollision(const vxVector3d &ray, vxCollision &collide);
	
	//!! this shouldn't be like this
	void getNearestCollisionUsingX(const vxVector3d &ray, vxCollision &collide);

	
	//!! this shouldn't be like this
	void getNearestCollisionUsingY(const vxVector3d &ray, vxCollision &collide);

	
	//!! this shouldn't be like this
	void getNearestCollisionUsingZ(const vxVector3d &ray, vxCollision &collide);
	
	//!!	Brute Force search.
	//! what a shame.
	void getNearestCollisionBF(const vxVector3d &ray, vxCollision &collide);

	// devuelve 0 si no le da a la caja
	// 1 si da y 2 y el resultado es optimo
	int throwRay(const vxVector3d &ray, vxCollision &collide);
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
#endif
