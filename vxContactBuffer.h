#ifndef VXCONTACTBUFFER_H
#define VXCONTACTBUFFER_H

#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>

#include "vxObject.h"
#include "vxVector.h"
#include "vxPixel.h"

using std::vector;

namespace vxCore {

class Hit
{
public:
	Hit(const vxColor &px, const vxVector2d &coord)
		:m_color(px)
		,m_xyCoef(coord)
	{}
	
	Hit()
	{}
	
	vxColor m_color;
	vxVector2d m_xyCoef;
};

//static std::mutex m_mutex;

class vxContactBuffer
{
private:

	//pxs stores the hits,
	
	int m_scanX = {0};
	int m_scanY = {0};
	 
	unsigned long m_k{0u};
	
public:
	std::vector<Hit> m_pxs;

	vxContactBuffer(unsigned int hits)
		:m_pxs(hits)
	{
	}
	
	~vxContactBuffer()
	{
	}

	vxColor& pixel(unsigned int idx)
	{
		return m_pxs[idx].m_color;
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
		Hit t(px,coord);
		m_pxs[m_k]=t;
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

	void reserve(const unsigned int size)
	{
		m_pxs.resize(size);
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
