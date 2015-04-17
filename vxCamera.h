#ifndef _VXCAMERAMC_
#define _VXCAMERAMC_

#include <iostream>
#include <math.h>

#include "vxObject.h"
#include "vxVector.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ImageProperties.h>
#include <vxSampler.h>
#include <vxContactBuffer.h>


namespace vxStorage {



class vxCamera:public vxObject
{

private:

	vxVector3d m_position{0.0, 0.0, 0.0};
	vxVector3d m_orientation{0.0, 0.0, 1.0};

	double m_focusDistance = {1.0};
	double m_horizontalAperture = {1.42};
	double m_verticalAperture = {1.42};
	unsigned int m_nSamples = {1u};
	unsigned int m_iteratorPosX = {0u};
	unsigned int m_iteratorPosY = {0u};
	bool m_chPix = {false};

	vxSampler m_sampler;
	std::shared_ptr<const ImageProperties> m_prop;

public:
	
	vxCamera(std::shared_ptr<const ImageProperties> prop) 
		: m_prop{prop}
	{
		srand(time(NULL));
	}

	vxCamera(vxVector3d position,
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

	void setPixelSamples(const int numSamples)
	{
		m_nSamples=numSamples;
		m_sampler.setSamples(m_nSamples);
	}

	vxVector2d getCoords() const
	{
		return vxVector2d( (m_sampler.x() + m_iteratorPosX) / (double)m_prop->rx()
							,(m_sampler.y() + m_iteratorPosY) / (double)m_prop->ry());
	}
	
	double getXCoord() const
	{
		return (m_sampler.x() + m_iteratorPosX) / (double)m_prop->rx();
	}
	
	double getYCoord() const
	{
		return (m_sampler.y() + m_iteratorPosY) / (double)m_prop->ry();
	}

	void set(vxVector3d position, 
			 vxVector3d orientation, 
			 double focusD = 1.0, 
			 double apertureH = 0.0, 
			 double apertureV = 0.0) 
	{
		this->m_orientation=orientation;
		this->m_position=position;
		this->m_focusDistance=focusD;
		
		if(std::max(apertureH+apertureV,0.0)==0.0)
		{
			this->m_horizontalAperture = 1.0;
			this->m_verticalAperture = m_prop->aspectRatio();
		}
		else
		{	
			this->m_horizontalAperture = apertureH;
			this->m_verticalAperture = apertureV;
		}
			
		m_iteratorPosX=0;
		m_iteratorPosY=0;
	}
	
	vxVector3d givemeRay(double x, double y)
	{
		m_sampler.next();
		double compX = tan(-m_horizontalAperture/2.0) * (( x * 2) -1 ) - 1 /(double)(2 * m_prop->rx()) + m_sampler.x()/(double)(m_prop->rx());
		double compY = tan(-m_verticalAperture/2.0) * (( y * 2) -1 ) - 1 /(double)(2 * m_prop->ry()) + m_sampler.y()/(double)(m_prop->ry());
		return vxVector3d( compY , compX , m_focusDistance );
	}
	
	vxVector3d givemeRandRay(double x, double y)
	{

		double compX = tan(-m_horizontalAperture/2.0) * (( x * 2) -1 ) - 1 /(double)(2 * m_prop->rx()) + ((rand()/(double)RAND_MAX))/(double)(m_prop->rx());
		double compY = tan(-m_verticalAperture/2.0) * (( y * 2) -1 ) - 1 /(double)(2 * m_prop->ry()) + ((rand()/(double)RAND_MAX))/(double)(m_prop->ry());
		return vxVector3d( compY , compX , m_focusDistance );
	}

	void resetRay()
	{ 
		m_iteratorPosX=0; 
		m_iteratorPosY=0;
		m_nSamples=1;
	}

	bool pixIsDone()
	{
		// TODO: might require rework
		if (m_chPix) 
		{
			m_chPix=false; 
			return true;
		}
		else 
		{
			return m_chPix;
		}
	}

	void resetPixel()
	{
		m_nSamples=1;
		m_chPix=false;
	}
	
	vxVector3d nextRay()
	{
		m_iteratorPosX++;
		
		if( m_iteratorPosX > (m_prop->rx()-1))
		{
			m_iteratorPosY++;
			m_iteratorPosX = 0;
		}

		m_chPix=true;

		vxVector3d ret = givemeRay( getXCoord(), getYCoord() );
		ret.rotateX(.291);
		return ret;
	}

	bool rayIsDone()
	{
		return m_iteratorPosY>=(m_prop->ry());
	}

	vxVector3d givemeNextRay(vxContactBuffer &imagen, double ang)
	{
		vxVector3d ret;
		ret.set(tan(m_verticalAperture/2.0) * ((imagen.getScanYd()*2)-1) , tan(m_horizontalAperture/2.0)*(( imagen.getScanXd() *2)-1), m_focusDistance);
		ret=ret.unit();
		ret=ret.rotateY(ang);
		return ret;
	}

	vxVector3d givemeRandomRay(double x, double y)
	{
		double yrv,xrv;
		//TODO:Remove this 700
		yrv=((rand()/double(RAND_MAX)))/700.0;
		xrv=((rand()/double(RAND_MAX)))/700.0;
		return vxVector3d(tan(m_horizontalAperture/2.0) * (((y+yrv)*2)-1) ,tan(m_verticalAperture/2)*(((x+xrv)*2.0)-1), m_focusDistance);
	}
	
	std::shared_ptr<const ImageProperties> prop() const
	{
		return m_prop;
	}
	
	void setProp(std::shared_ptr<const ImageProperties> prop)
	{
		m_prop = prop;
	}
};

}
#endif // VXPXCAMERA

