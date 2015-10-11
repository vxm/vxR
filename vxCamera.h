#ifndef _VXCAMERAMC_
#define _VXCAMERAMC_

#include <iostream>
#include <math.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "vxVector.h"
#include "vxRay.h"
#include "ImageProperties.h"
#include "vxContactBuffer.h"
#include "vxSampler.h"


namespace vxCore {


class vxCamera
{

private:

	vxVector3d m_position{0.0, 0.0, 0.0};
	vxVector3d m_orientation{0.0, 0.0, 1.0};

	double m_focusDistance = {1.0};
	double m_horizontalAperture = {1.42};
	double m_verticalAperture = {1.42};
	
	double m_hApTan = tan(-m_horizontalAperture/2.0);
	double m_vApTan = tan(-m_verticalAperture/2.0);

	double m_rx;
	double m_ry;

	std::shared_ptr<const ImageProperties> m_prop;

public:
	
	vxCamera(std::shared_ptr<const ImageProperties> prop);
	vxCamera(const vxVector3d &position,
				const vxVector3d &orientation,
				double focusD,
				double apertureH,
				double apertureV);

	void set(const vxVector3d &position, 
			 const vxVector3d &orientation, 
			 double focusD = 1.0, 
			 double apertureH = 0.0, 
			 double apertureV = 0.0);
	
	vxRay ray(const vxVector2d &coord, vxSampler &sampler) const;
	
	vxRay givemeRandRay(const vxVector2d &coord);

	void next(unsigned int skip = 1u);

	vxRay givemeNextRay(const vxContactBuffer &imagen, double ang);

	vxRay givemeRandomRay(const vxVector2d &coord);
	
	std::shared_ptr<const ImageProperties> prop() const;
	
	void setProp(std::shared_ptr<const ImageProperties> prop);
	double horizontalAperture() const;
	void setHorizontalAperture(double horizontalAperture);
	double verticalAperture() const;
	void setVerticalAperture(double verticalAperture);
};

}
#endif // VXPXCAMERA

