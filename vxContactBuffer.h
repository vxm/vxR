#ifndef VXIMAGE_H
#define VXIMAGE_H

#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>

#include <vxObject.h>
#include <vxVector.h>
#include <vxShader.h>
#include <vxPixel.h>

using std::vector;

namespace vxStorage {

struct Hit
{
	vxColor m_px;
	double m_xcoef;
	double m_ycoef;
	
//	std::shared_ptr<vxShader> m_sh;
};


class vxContactBuffer:public vxObject
{
private:

	//pxs stores the hits,
	std::vector<Hit> m_pxs;
	
	int m_scanX = {0};
	int m_scanY = {0};

public:

	vxContactBuffer()
	{
	}

	vxContactBuffer(unsigned int sz)
	{
		m_pxs.reserve(sz);
	}
	
	~vxContactBuffer()
	{
	}
	
	std::vector<Hit>* getHits()
	{
		return &m_pxs;
	}
	
	void append(const vxColor &px, double x, double y)
	{
		Hit h;
		
		h.m_px = px;
		h.m_xcoef = x;
		h.m_ycoef = y;
		
		//m_pxs.push_back(std::move(h));
		m_pxs.push_back(h);
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

	double getScanXd() const
	{
		return 0;
	}

	double getScanYd() const
	{
		return 0;
	}

};

}
#endif // VXIMAGE_H