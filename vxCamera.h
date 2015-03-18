#ifndef _VXPXBUFFERMC_
#define _VXPXBUFFERMC_


#include "vxObject.h"
#include "vxVector.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vxpxbuffer.h>
#include <imageproperties.h>

namespace vxStorage {

class vxSamples:public vxObject
{
	double *m_x = nullptr;
	double *m_y = nullptr;
	int m_f;
	int m_samples;
public:
	vxSamples() 
	{
		setSamples(1);
	};
	
	~vxSamples() 
	{
	}

	void next() {m_f>=m_samples-1 ? m_f=0 : m_f++;};

	double getX() {return m_x[m_f];}
	double getY() {return m_y[m_f];}

	void setSamples(int sampless)
	{
		m_samples=sampless;
		
		m_x=(double*)malloc(sizeof(double)*m_samples);
		m_y=(double*)malloc(sizeof(double)*m_samples);

		for(int i=0;i<m_samples;i++)
		{
			m_x[i]=(rand()/(double)RAND_MAX);
			m_y[i]=(rand()/(double)RAND_MAX);
		}
		m_f=0;
	}
};

class vxCamera:public vxObject
{

private:

	vxVector3d m_position{0.0, 0.0, 0.0};
	vxVector3d m_orientation{0.0, 0.0, 0.0};

	double m_focusDistance = {1.0};
	double m_horizontalAperture = {1.42};
	double m_verticalAperture = {1.42};
	unsigned int m_samples = {1u};
	unsigned int m_posX = {0u};
	unsigned int m_posY = {0u};
	bool m_chPix = {false};

	vxSamples m_sampler;
	ImageProperties m_prop;

public:
	
	vxCamera(const ImageProperties &prop) 
		: m_prop{prop}
		, m_orientation(0.0,0.0,1.0)
		, m_position(0.0,0.0,0.0)
	{
		m_horizontalAperture=1.42;
		m_verticalAperture=1.42;

		srand(time(NULL));
	}


	void camera(vxVector3d position, 
				vxVector3d orientation, 
				double focusD, 
				double apertureH, 
				double apertureV)
	{
		this->m_orientation=orientation;
		this->m_position=position;
		this->m_focusDistance=focusD;
		this->m_horizontalAperture=apertureH;
		this->m_verticalAperture=apertureV;
	
		srand(time(NULL));
		m_chPix=false;
	}

	void setPixelSamples(int samples)
	{
		this->m_samples=samples;
		m_sampler.setSamples(m_samples);
	}

	int getYCoord() const 
	{
		return m_posX < m_prop.rx() ? m_posX : m_prop.rx()-1;
	}
	
	int getXCoord() 
	{
		return m_posY < m_prop.ry() ? m_posY : m_prop.ry()-1;
	}

	void set(vxVector3d position, vxVector3d orientation, double focusD, double apertureH, double apertureV) 
	{
		this->m_orientation=orientation;
		this->m_position=position;
		this->m_focusDistance=focusD;
		this->m_horizontalAperture=apertureH;
		this->m_verticalAperture=apertureV;
		
		m_posX=0;
		m_posY=0;
	}
	
	/* 
	vxVector3d givemeRay(double x, double y)
	{
		return vxVector3d( tan(apertureV/2.0) * ((y*2)-1) , tan(apertureH/2.0)*((x*2)-1), focusD);
	}*/

	vxVector3d givemeRay(double x, double y)
	{
		m_sampler.next();
		double compX = tan(-m_horizontalAperture/2.0) * (( x * 2) -1 ) - 1 /(float)(2 * m_prop.rx()) + m_sampler.getX()/(float)(m_prop.rx());
		double compY = tan(-m_verticalAperture/2.0) * (( y * 2) -1 ) - 1 /(float)(2 * m_prop.ry()) + m_sampler.getY()/(float)(m_prop.ry());
		return vxVector3d( compY , compX , m_focusDistance );
	}
	
	vxVector3d givemeRandRay(double x, double y)
	{

		double compX = tan(-m_horizontalAperture/2.0) * (( x * 2) -1 ) - 1 /(float)(2 * m_prop.rx()) + ((rand()/(double)RAND_MAX))/(float)(m_prop.rx());
		double compY = tan(-m_verticalAperture/2.0) * (( y * 2) -1 ) - 1 /(float)(2 * m_prop.ry()) + ((rand()/(double)RAND_MAX))/(float)(m_prop.ry());
		return vxVector3d( compY , compX , m_focusDistance );
	}

	void resetRay() { m_posX=0; m_posY=0;m_samples=1;}

	bool pixIsDone()
	{// esto se puede abreviar.
		if (m_chPix) {m_chPix=false; return true;}
		else return m_chPix;
	}

	void resetPixel()
	{
		m_samples=1;
		m_chPix=false;
	}
	
	vxVector3d& nextRay()
	{
		double x = m_posX / float(m_prop.rx()) ;
		double y = m_posY / float(m_prop.ry()) ;
	
		if (m_samples>=m_samples)
		{
			m_posX++;
			if( m_posX >= m_prop.rx() ) 
			{
				m_posY++;
				m_posX = 0;
			}
			
			m_samples=1;
			m_chPix=true;
		}
		else
			m_samples++;

		vxVector3d ret = givemeRay( x, y );
		ret.rotateX(.5);
		return ret;
	}

	bool rayIsDone()
	{
		return m_posY>=m_prop.ry();
	}

	vxVector3d givemeNextRay(vxPxBuffer &imagen, double ang)
	{
		vxVector3d ret;
		ret.set(tan(m_verticalAperture/2.0) * ((imagen.getScanYd()*2)-1) , tan(m_horizontalAperture/2.0)*(( imagen.getScanXd() *2)-1), m_focusDistance);
		ret=ret.unit();
		ret=ret.rotateY(ang);
		return ret;
	}

	vxVector3d givemerRay(double x, double y)
	{
		double yrv,xrv;
		yrv=((rand()/double(RAND_MAX)))/700.0;
		xrv=((rand()/double(RAND_MAX)))/700.0;
		return vxVector3d(tan(m_horizontalAperture/2.0) * (((y+yrv)*2)-1) ,tan(m_verticalAperture/2)*(((x+xrv)*2.0)-1), m_focusDistance);

	}
	ImageProperties prop() const;
	void setProp(const ImageProperties &prop);
};

#endif // VXPXBUFFER
}
