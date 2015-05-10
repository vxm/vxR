#ifndef VXCONTACTBUFFER_H
#define VXCONTACTBUFFER_H

#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>

#include "vxObject.h"
#include "vxVector.h"
#include "vxPixel.h"

using std::vector;

namespace vxCore {

struct Hit
{
	vxColor m_px;
	vxVector2d m_xyCoef;
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
	
	void append(const vxColor &px, const vxVector2d &coord)
	{
		Hit h;
		
		h.m_px = px;
		h.m_xyCoef = coord;
		
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
