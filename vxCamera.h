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


namespace vxCore {


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

	vxSampler m_sampler;
	std::shared_ptr<const ImageProperties> m_prop;

public:
	
	vxCamera(std::shared_ptr<const ImageProperties> prop);

	vxCamera(const vxVector3d &position,
				vxVector3d orientation,
				double focusD,
				double apertureH,
				double apertureV);

	void setPixelSamples(const unsigned int numSamples);

	unsigned int getPixelSamples() const;
	
	vxVector2d getCoords() const;
	
	double getXCoord() const;
	
	double getYCoord() const;

	void set(const vxVector3d &position, 
			 vxVector3d orientation, 
			 double focusD = 1.0, 
			 double apertureH = 0.0, 
			 double apertureV = 0.0);
	
	vxRayXYZ givemeRay(double x, double y) const;
	
	vxRayXYZ givemeRandRay(double x, double y);

	void resetRay();

	bool pixIsDone();

	void resetPixel();
	
	void next();
	
	vxRayXYZ nextRay();

	bool rayIsDone()
	{
		return m_iteratorPosY>=(m_prop->ry());
	}

	vxRayXYZ givemeNextRay(const vxContactBuffer &imagen, double ang);

	vxRayXYZ givemeRandomRay(double x, double y);
	
	std::shared_ptr<const ImageProperties> prop() const;
	
	void setProp(std::shared_ptr<const ImageProperties> prop);
};

}
#endif // VXPXCAMERA

