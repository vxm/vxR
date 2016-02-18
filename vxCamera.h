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

	v3s m_position{0.0, 0.0, 0.0};
	v3s m_orientation{0.0, 0.0, 1.0};

	scalar m_focusDistance = {1.0};
	scalar m_horizontalAperture = {1.42};
	scalar m_verticalAperture = {1.42};
	
	scalar m_hApTan = tan(-m_horizontalAperture/2.0);
	scalar m_vApTan = tan(-m_verticalAperture/2.0);

	scalar m_rx;
	scalar m_ry;

	std::shared_ptr<const ImageProperties> m_properties;

public:
	
	vxCamera(std::shared_ptr<const ImageProperties> prop);
	vxCamera(const v3s &position,
				const v3s &orientation,
				scalar focusD,
				scalar apertureH,
				scalar apertureV);

	void set(const v3s &position, 
			 const v3s &orientation, 
			 scalar focusD = 1.0, 
			 scalar apertureH = 0.0, 
			 scalar apertureV = 0.0);
	
	vxRay ray(const v2s &coord, vxSampler &sampler) const;
	
	vxRay givemeRandRay(const v2s &coord);

	void next(unsigned int skip = 1u);

	vxRay givemeNextRay(const vxContactBuffer &imagen, scalar ang);

	vxRay givemeRandomRay(const v2s &coord);
	
	std::shared_ptr<const ImageProperties> prop() const;
	
	void setProp(std::shared_ptr<const ImageProperties> prop);
	scalar horizontalAperture() const;
	void setHorizontalAperture(scalar horizontalAperture);
	scalar verticalAperture() const;
	void setVerticalAperture(scalar verticalAperture);
	std::shared_ptr<const ImageProperties> properties() const;
	void setProperties(const std::shared_ptr<const ImageProperties> &properties);
};

}
#endif // VXPXCAMERA

