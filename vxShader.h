#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "vxVector.h"
#include "vxObject.h"
#include "vxScene.h"
#include "stdlib.h"
#include "time.h"

namespace vxStorage {

class vxShader:public vxObject
{
protected:

	vxScene *m_scene;

public:

	vxShader()
	{
		m_scene=NULL;
		srand(time(NULL));
	}
	
	virtual vxColor getColor(vxCollision &collide)
	{
		return vxColor(0, 0, 11);
	}

	vxScene* getScene()
	{
		return m_scene;
	}

	void setScene(vxScene *scene)
	{
		m_scene=scene;
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

static class vxLambert:public vxShader
{
private:


public:
	vxLambert():vxShader() 
	{
	}

	vxColor getColor(vxCollision &collide);

}lambert;

}
#endif