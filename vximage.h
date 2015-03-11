#ifndef VXIMAGE_H
#define VXIMAGE_H

#include "vxObject.h"
#include "vxVector.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using std::vector;

class vxPxBuffer:public vxObject
{
private:

	int m_xres;
	int m_yres;

	int m_depth;

	std::vector<double> m_pxs;
	
	int m_scanX;
	int m_scanY;

public:

	vxPxBuffer();

	vxPxBuffer(int xr, int yr, int dp)
	{
		m_xres = xr;
		m_yres = yr;
		m_depth = dp;

	}

	~vxPxBuffer()
	{
	}


	void reset()
	{
		m_scanX=0;
		m_scanY=0;
	}

	void next()
	{

	}

	bool isDone()
	{
		return true;
	}

	void set(int xr, int yr, int dp)
	{
		m_xres=xr;
		m_yres=yr;
		m_depth=dp;
	}

	void reserve()
	{
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
		return m_scanX/(double)m_xres;
	}

	double getScanYd()
	{
		return m_scanY/(double)m_yres;
	}

	void setColorAtPixel( vxColor color)
	{

	}
};

#endif // VXIMAGE_H
