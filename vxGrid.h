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

namespace vxStorage {

class vxGrid:public vxObject
{
protected:

	vxVector3d m_position;
	double m_size				= {5.0};
	double m_boxSize			= {1.0};
	unsigned int m_resolution	= {5};
	double m_invRes				= {1/5.0};

	std::unique_ptr<bool> m_data;
	std::unique_ptr<vxBoxN> m_boundingBox;

	double m_resDivTres	= {5/3.0};
	double m_midSize	= {0.5};

	// cache objects
	unsigned int m_resXres		= {25};
	unsigned int m_resXresXres	= {125};
public:	

	vxGrid()
	{
		m_boundingBox.reset(new vxBoxN);
		m_size=1;

		createGridData(5);
		initialize();
		updateBB();
		srand(time(NULL));
	}


	vxGrid( vxVector3d position, double size)
	{
		m_boundingBox.reset(new vxBoxN);
		this->m_position = position;
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

	void setResolution(int resolution)
	{
		createGridData(resolution);
	}

	// es necesario int ? mejor short?.
	bool getElement(int x,int y,int z)
	{
		auto p = x+(y*m_resolution)+(z*m_resXres);
		return m_data.get()[p];
	}

	void setElement(int x,int y,int z, bool value)
	{
		bool *bol=m_data.get();
		bol+=(x+(y*m_resolution)+(z*m_resXres));
		*bol=value;
	}

	void setPosition(const vxVector3d position)
	{
		this->m_position=position;
	}

	void setSize(const double size)
	{
		this->m_size=size;
		m_midSize = size/2.0;
	}

	void setBoxSize()
	{
		m_boxSize = m_size/double(m_resolution);
	}

	void updateBB()
	{
		m_boundingBox.reset(vxGlobal::getBox(m_position, m_size));
	}
	
	bool* getLastBit() const
	{
		return m_data.get() + m_resXresXres;
	}

	//!! 
	void initialize()
	{
		bool *pb = m_data.get();
		bool *lb = getLastBit();
		
		while(pb!=lb)
		{
			*pb = false;
			pb++;
		}
	}
	
	unsigned int numActiveVoxels()
	{
		unsigned int av{0};
		
		bool *pb = m_data.get();
		bool *lb = getLastBit();
		
		while(pb!=lb)
		{
			if(*pb)
				av++;
			pb++;
		}
		
		return av;
	}

	vxVector3d getBoxPosition(int x, int y, int z)
	{
		double retx = (m_position.getX() - m_midSize) + (x * m_boxSize) + m_resDivTres;
		double rety = (m_position.getY() - m_midSize) + (y * m_boxSize) + m_resDivTres;
		double retz = (m_position.getZ() - m_midSize) + (z * m_boxSize) + m_resDivTres;

		return vxVector3d( retx, rety, retz); 
	}

	void drawDiagonals()
	{
		int resminusone=m_resolution-1;
		for (int i=0;i<m_resolution;i++)
		{
			active(i,i,i);
			active(resminusone-i,resminusone-i,i);
			active(i,resminusone-i,resminusone-i);
			active(resminusone-i,i,resminusone-i);
		}
	}

	void drawMarcs()
	{
		int resminusone=m_resolution-1;
		for (int i=0;i<m_resolution;i++)
		{
			active(i,0,0);
			active(i,resminusone,resminusone);
			active(i,0,resminusone);
			active(i,resminusone,0);

			active(0,i,0);
			active(resminusone,i,resminusone);
			active(0,i,resminusone);
			active(resminusone,i,0);

			active(0,0,i);
			active(resminusone,resminusone,i);
			active(resminusone,0,i);
			active(0,resminusone,i);
		}
	}
	
	void createSphere(vxVector3d center, const float radio)
	{
		int x, y, z;
		
		for(x=0;x<m_resolution;x++)
			for(y=0;y<m_resolution;y++)
				for(z=0;z<m_resolution;z++)
				{
					if(center.distance(getBoxPosition(x, y, z))<radio)
						setElement(x,y,z,true);
					else
						setElement(x,y,z,false);
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
		int x, y, z;
		
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

	bool isActive( int x,  int y,  int z)
	{
		return getElement(x,y,z);
	}

	void active(const int x, const int y, const int z)
	{
		setElement(x,y,z,true);
	}

	void desactive(const int x, const int y, const int z)
	{
		setElement(x,y,z,false);
	}

	//!! this shouldn't be like this
	//! what a shame.
	void getNearestCollision(vxVector3d &ray, vxCollision &collide)
	{
		collide.initialize();

		bool pri=true;

		vxCollision minima;
		double min_length;

		vxBoxN *caja;	
	
		for(int x=0;x<m_resolution;x++)
		{
			for(int y=0;y<m_resolution;y++)
			{
				for(int z=0;z<m_resolution;z++)
				{
					if (getElement(x,y,z))
					{
						caja = vxGlobal::getInstance()->getExistingtBox( getBoxPosition(x, y, z), m_boxSize);
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
	int throwRay(vxVector3d &ray, vxCollision &collide)
	{ 
		if (!m_boundingBox) 
			return 0;
	
		// will test in 
		m_boundingBox->throwRay(ray, collide);

		if (collide.isValid()) 
		{
			collide.setColor(22,122,22);
			//if (getRandom() && getRandom())
			//getNearestCollision(ray, collide);
			//else
			//	collide.setColor(212,21,255);
			return 1;
		}
		else
		{
			collide.initialize();
			return 0;
		}

	}
};
}
#endif
