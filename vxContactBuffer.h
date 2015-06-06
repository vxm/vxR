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

class Hit
{
public:
	Hit(const vxColor &px, const vxVector2d &coord)
		:m_px(px)
		,m_xyCoef(coord)
	{}
	
	Hit()
	{}
	
	vxColor m_px;
	vxVector2d m_xyCoef;
};

class vxContactBuffer
{
private:

	//pxs stores the hits,
	std::vector<Hit> m_pxs;
	
	int m_scanX = {0};
	int m_scanY = {0};
	 
	unsigned long m_k{0u};
	
public:

	vxContactBuffer()
	{
		m_pxs.resize(900);
	}
	
	~vxContactBuffer()
	{
	}
	
	std::vector<Hit>* getHits()
	{
		return &m_pxs;
	}

	unsigned int hitsCount()
	{
		return m_k;
	}

	void append(const vxColor &px, const vxVector2d &coord)
	{
		if(m_k>=m_pxs.size())
		{
			m_pxs.resize(m_k + m_pxs.size());
		}
		
		Hit t(px,coord);
		m_pxs[m_k]=t;
		
		//m_pxs.push_back(std::move(h));
		m_k++;
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
