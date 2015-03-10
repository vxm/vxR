#ifndef VXIMAGE_H
#define VXIMAGE_H

#include "vxObject.h"
#include "vxVector.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

class vxPxBuffer:public vxObject
{
private:
	int m_xRes;
	int m_yRes;

	int m_depth;
	unsigned char *m_mapa;
	int m_scanX;
	int m_scanY;

public:

	vxPxBuffer();

	vxPxBuffer(int xr, int yr, int dp)
	{
		m_xRes=xr;
		m_yRes=yr;
		m_depth=dp;

	}

	vxPxBuffer(unsigned char *map)
	{
		m_mapa=map;
	}
		
	~vxPxBuffer() 
	{
		free(m_mapa);
	}


	unsigned char* getPixel(int x, int y)
	{
		return &m_mapa[x*y*4];
	}

	void reset()
	{
		itMapa=m_mapa;
		m_scanX=0;
		m_scanY=0;
	}

	void next()
	{
		itMapa+=4;
		m_scanX++;
		if ((m_scanX%(m_xRes+1))==m_xRes) 
		{
			m_scanY++;
			m_scanX=0;
		}
	}

	bool isDone()
	{
		return itMapa==&m_mapa[m_xRes*m_yRes*m_depth];
	}

	unsigned char *itMapa;

	void setPixels(unsigned char* map)
	{
		m_mapa=map;
	}

	void set(int xr, int yr, int dp)
	{
		m_xRes=xr;
		m_yRes=yr;
		m_depth=dp;
	}

	void reserve()
	{
		if(m_mapa!=NULL) m_mapa=(unsigned char*)malloc(sizeof(unsigned char)*m_xRes*m_yRes);
	}

	int getScanX()
	{
		return m_scanX;
	}

	int getScanY()
	{
		return m_scanY;
	}

	double getScanXd()
	{
		return m_scanX/(double)m_xRes;
	}

	double getScanYd()
	{
		return m_scanY/(double)m_yRes;
	}

	void setColorAtPixel( vxColor color)
	{
		*(itMapa) =  color.getR();
		*(itMapa+1) =  color.getG();
		*(itMapa+2) =  color.getB();
		*(itMapa+3) =  color.getA();
	}
};

#endif // VXIMAGE_H
