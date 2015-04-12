#ifndef _VXBOXMATRIXMC_
#define _VXBOXMATRIXMC_

#include <memory>
#include <bitset>

#include "vxObject.h"
#include <vxGlobal.h>
#include "vxBox.h"

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

	std::unique_ptr<bool[]> m_data;
	std::unique_ptr<vxBoxN> m_boundingBox;

	double m_resDivTres	= {m_size/3.0};
	double m_midSize	= {m_size/2.0};

	// cache objects
	unsigned int m_resXres		= {25};
	unsigned int m_resXresXres	= {125};

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
		m_data.reset(new bool[m_resXresXres]);
		//invRes=1/resolution;
		m_resDivTres = m_midSize/(double)m_resolution;
		setBoxSize();
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
	}

	void setPosition(const vxVector3d position)
	{
		this->m_position=position;
	}

	void setBoxSize()
	{
		m_boxSize = m_size/double(m_resolution);
	}

	void updateBB()
	{
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
	
	void createSphere(const vxVector3d &center, const float radio)
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
					else
					{
						setElement(x,y,z,false);
					}
				}
			}
		}
	}

	void createSphere(int x, int y, int z, const float radio)
	{
		createSphere(vxVector3d(x,y,z), radio);
	}

	bool getRandom()
	{
		float num=(rand()/(double)RAND_MAX)*1000;
		num=num-int(num);
		return num<.5;
	}

	void createRandom()
	{
		unsigned int x, y, z;
		
		for(x=0;x<m_resolution;x++)
			for(y=0;y<m_resolution;y++)
				for(z=0;z<m_resolution;z++)
				{
					if( getRandom() && getRandom())
						setElement(x,y,z,true);
					else
						setElement(x,y,z,false);
				}
	}

	//!! 
	void initialize(bool value = false)
	{
		bool *pb = m_data.get();
		bool *lb = getLastByte();
		
		while(pb!=lb)
		{
			*pb = value;
			pb++;
		}
	}
	
	unsigned int numActiveVoxels()
	{
		unsigned int av{0};
		
		bool *pb = m_data.get();
		bool *lb = getLastByte();
		
		while(pb!=lb)
		{
			if(*pb)
			{
				av++;
			}
			
			pb++;
		}
		
		return av;
	}

	inline bool active( int x,  int y,  int z) const
	{
		return getElement(x,y,z);
	}
	
	inline bool active(unsigned int idx) const
	{
		if (idx<m_resXresXres)
			return m_data.get()[idx];
		else
			return false;
	}

	void activate(const int x, const int y, const int z)
	{
		setElement(x,y,z,true);
	}

	void deactivate(const int x, const int y, const int z)
	{
		setElement(x,y,z,false);
	}

	bool* getLastByte() const
	{
		return m_data.get() + m_resXresXres;
	}

	// es necesario int ? mejor short?.
	inline bool getElement(int x,int y,int z) const
	{
		auto p = x+(y*m_resolution)+(z*m_resXres);
		return m_data.get()[p];
	}

	void setElement(int x, int y, int z, bool value)
	{
		bool *bit=m_data.get();
		bit+=(x+(y*m_resolution)+(z*m_resXres));
		*bit=value;
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
		
	unsigned int indexAtPosition(vxVector3d pos) const
	{
		pos -= (m_position-m_midSize); 

		unsigned int idx = floor(pos.x());
		idx += floor(pos.y()) * m_resolution;
		idx += floor(pos.z()) * m_resXres;
		return idx;
	}
	
	//!! this shouldn't be like this
	void getNearestCollision(const vxVector3d &ray, vxCollision &collide)
	{
		collide.initialize();

		double z;
		
		vxBoxN *boxInstance;
		auto init = m_position.z()-m_midSize;
		auto end = m_position.z()+m_midSize;
		for(z=init; z<end; z+=m_boxSize)
		{
			auto idx = indexAtPosition(MathUtils::rectAndZPlane(ray, z+.1));
			if(active(idx))
			{
				auto voxPos = getVoxelPosition(idx);
				boxInstance = vxGlobal::getInstance()->getExistingtBox( voxPos, m_boxSize);
				boxInstance->setShader( vxGlobal::getLambert() );
				boxInstance->throwRay( ray, collide );
				
				if (collide.isValid())
				{
					return;
				}
				else
				{
					collide.setColor(255,0,0);
				}
			}
		}
		
	}	

	//!! Brute Force search.
	//! what a shame.
	void getNearestCollisionBF(const vxVector3d &ray, vxCollision &collide)
	{
		collide.initialize();

		bool pri=true;

		vxCollision minima;

		vxBoxN *caja;
	
		for(uint x=0;x<m_resolution;x++)
		{
			for(uint y=0;y<m_resolution;y++)
			{
				for(uint z=0;z<m_resolution;z++)
				{
					if (getElement(x,y,z))
					{
						caja = vxGlobal::getInstance()->getExistingtBox( getVoxelPosition(x, y, z), m_boxSize);
						caja->setShader( vxGlobal::getLambert() );
						caja->throwRay( ray, collide );
						
						if (collide.isValid()) 
						{
							
							if (pri)
							{
								minima=collide;

								pri=false;
							}
							else
							{
								if(collide.getPosition().length()<minima.getPosition().length()) minima=collide;
							}
						}
						else
						{
							collide.setColor(255,0,0);
						}
					}
				}
			}
		}
		
		//
		collide=minima;
	}

	// devuelve 0 si no le da a la caja
	// 1 si da y 2 y el resultado es optimo
	int throwRay(const vxVector3d &ray, vxCollision &collide)
	{ 
		if (!m_boundingBox) 
			return 0;
	
		// will test in 
		m_boundingBox->throwRay(ray, collide);

		if (collide.isValid()) 
		{
			getNearestCollision(ray, collide);
			return 1;
		}
		else
		{
			collide.initialize();
			return 0;
		}
	}
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
