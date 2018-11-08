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

#include "Object.h"
#include "Vector.h"
#include "Pixel.h"

using std::vector;

namespace vxCore {

class Hit
{
public:
	Hit(const Color &px, const v2s &coord)
		:m_color(px)
		,m_xyCoef(coord)
	{}
	
	Hit()
		:m_color(0.0,0.0,0.0,0.0)
	{}
	
	Color m_color;
	v2s m_xyCoef;
};

//static std::mutex m_mutex;

class ContactBuffer
{
private:

	//pxs stores the hits,
	
	int m_scanX = {0};
	int m_scanY = {0};
	 
	unsigned long m_k{0u};
	
public:
	std::vector<Hit> m_pxs;

	ContactBuffer(unsigned int hits)
		:m_pxs(hits)
	{
	}
	
	~ContactBuffer()
	{
	}

	Color& pixel(unsigned int idx)
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

	void append(const Color &px, const v2s &coord)
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

	scalar getScanXd() const
	{
		return 0;
	}

	scalar getScanYd() const
	{
		return 0;
	}

};

}
#endif // VXIMAGE_H
