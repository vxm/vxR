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

namespace vxStorage {

class vxSamples:public vxObject
{
	double *x = nullptr;
	double *y = nullptr;
	int k;
	int samples;
public:
	vxSamples() 
	{
		setSamples(1);
	};
	
	~vxSamples() 
	{
		free(x);
		free(y);
	}

	void next() {k>=samples-1 ? k=0 : k++;};
	double getX() {return x[k];}
	double getY() {return y[k];}

	void setSamples(int sampless)
	{
		samples=sampless;
		x=(double*)malloc(sizeof(double)*samples);
		y=(double*)malloc(sizeof(double)*samples);
		for(int i=0;i<samples;i++)
		{
			x[i]=(rand()/(double)RAND_MAX);
			y[i]=(rand()/(double)RAND_MAX);
		}
		k=0;
	}
};

class vxCamera:public vxObject
{
private:
	vxVector3d position;
	vxVector3d orientation;
	double focusD;
	double apertureH;
	double apertureV;

	int samples;

	int xres;
	int yres;

	int posX;
	int posY;

	int k_samples;
	bool chPix;

	vxSamples sam;

public:
	void camera() 
	{
		orientation.set(0,0,1);
		position.set(0,0,0);
		focusD=1;
		apertureH=1.42;
		apertureV=1.42;

		posX=0;
		posY=0;
		samples=1;
		k_samples=1;
		srand(time(NULL));

		chPix=false;
	}


	void camera(vxVector3d position, vxVector3d orientation, double focusD, double apertureH, double apertureV)
	{
		this->orientation=orientation;
		this->position=position;
		this->focusD=focusD;
		this->apertureH=apertureH;
		this->apertureV=apertureV;
		
		posX=0;
		posY=0;
		samples=1;
		k_samples=1;
		srand(time(NULL));

		chPix=false;
	}

	void setSamples(int sampless)
	{
		this->samples=sampless;
		sam.setSamples(samples);
	}

	void setResolution(int xr, int yr) {xres=yr;yres=xr;}

	int getYCoord() {return posX < xres ? posX : xres-1;}
	int getXCoord() {return posY < yres ? posY : yres-1;}

	void set(vxVector3d position, vxVector3d orientation, double focusD, double apertureH, double apertureV) 
	{
		this->orientation=orientation;
		this->position=position;
		this->focusD=focusD;
		this->apertureH=apertureH;
		this->apertureV=apertureV;
		
		posX=0;
		posY=0;
	}
	
	/* 
	vxVector3d givemeRay(double x, double y)
	{
		return vxVector3d( tan(apertureV/2.0) * ((y*2)-1) , tan(apertureH/2.0)*((x*2)-1), focusD);
	}*/

	vxVector3d givemeRay(double x, double y)
	{
		sam.next();
		double compX = tan(-apertureH/2.0) * (( x * 2) -1 ) - 1 /(float)(2 * xres) + sam.getX()/(float)(xres);
		double compY = tan(-apertureV/2.0) * (( y * 2) -1 ) - 1 /(float)(2 * yres) + sam.getY()/(float)(yres);
		return vxVector3d( compY , compX , focusD );
	}
	
	vxVector3d givemeRandRay(double x, double y)
	{

		double compX = tan(-apertureH/2.0) * (( x * 2) -1 ) - 1 /(float)(2 * xres) + ((rand()/(double)RAND_MAX))/(float)(xres);
		double compY = tan(-apertureV/2.0) * (( y * 2) -1 ) - 1 /(float)(2 * yres) + ((rand()/(double)RAND_MAX))/(float)(yres);
		return vxVector3d( compY , compX , focusD );
	}

	void resetRay() { posX=0; posY=0;k_samples=1;}

	bool pixIsDone()
	{// esto se puede abreviar.
		if (chPix) {chPix=false; return true;}
		else return chPix;
	}

	void resetPixel()
	{
		k_samples=1;
		chPix=false;
	}
	
	vxVector3d& nextRay()
	{
		double x = posX / float(xres) ;
		double y = posY / float(yres) ;
	
		if (k_samples>=samples)
		{
			posX++;
			if( posX >= xres ) 
			{
				posY++;
				posX = 0;
			}
			k_samples=1;
			chPix=true;
		}
		else
			k_samples++;

		vxVector3d ret;

		ret = givemeRay( x, y );
		ret.rotateX(.5);
		return ret;
	}

	bool rayIsDone()
	{
		return posY>=yres;
	}

	vxVector3d givemeNextRay(vxPxBuffer &imagen, double ang)
	{
		vxVector3d ret;
		ret.set(tan(apertureV/2.0) * ((imagen.getScanYd()*2)-1) , tan(apertureH/2.0)*(( imagen.getScanXd() *2)-1), focusD);
		ret=ret.unit();
		ret=ret.rotateY(ang);
		return ret;
	}

	vxVector3d givemerRay(double x, double y)
	{
		double yrv,xrv;
		yrv=((rand()/double(RAND_MAX)))/700.0;
		xrv=((rand()/double(RAND_MAX)))/700.0;
		return vxVector3d(tan(apertureH/2.0) * (((y+yrv)*2)-1) ,tan(apertureV/2)*(((x+xrv)*2.0)-1), focusD);

	}
};

#endif // VXPXBUFFER
}
