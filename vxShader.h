#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "vxVector.h"
#include "vxObject.h"
#include "stdlib.h"
#include "time.h"

namespace vxStorage {

class vxShader:public vxObject
{
protected:


public:

	vxShader()
	{
		srand(time(NULL));
	}
	
	virtual vxColor getColor(vxCollision &, double lumm)
	{
		return vxColor(lumm, lumm, lumm);
	}

	double getRand()
	{
		return (rand()/(double)RAND_MAX);
	}

	double getBoolRand()
	{
		return getRand()<.5;
	}
};

class vxLambert:public vxShader
{
private:


public:
	vxLambert()
	:vxShader() 
	{
	}

	virtual vxColor getColor( vxCollision &collide,
							  double lumm) override;

};

}
#endif