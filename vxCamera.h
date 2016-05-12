#ifndef _VXCAMERAMC_
#define _VXCAMERAMC_

#include <iostream>
#include <math.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "Matrix44.h"
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
	scalar m_pixelRadius{1.0};

	scalar m_rx;
	scalar m_ry;

	std::shared_ptr<const ImageProperties> m_properties;

	Matrix44 m_transform;
public:
	///
	/// \brief vxCamera
	/// \param prop
	///
	vxCamera(std::shared_ptr<const ImageProperties> prop);
	///
	/// \brief vxCamera
	/// \param position
	/// \param orientation
	/// \param focusD
	/// \param apertureH
	/// \param apertureV
	///
	vxCamera(const v3s &position,
				const v3s &orientation,
				scalar focusD,
				scalar apertureH,
				scalar apertureV);
	///
	/// \brief set
	/// \param position
	/// \param orientation
	/// \param focusD
	/// \param apertureH
	/// \param apertureV
	///
	void set(const v3s &position, 
			 const v3s &orientation, 
			 scalar focusD = 1.0, 
			 scalar apertureH = 0.0, 
			 scalar apertureV = 0.0);
	///
	/// \brief ray
	/// \param coord
	/// \param sampler
	/// \return 
	///
	vxRay ray(const v2s &coord, vxSampler &sampler) const;
	///
	/// \brief givemeRandRay
	/// \param coord
	/// \return 
	///
	vxRay givemeRandRay(const v2s &coord);
	///
	/// \brief next
	/// \param skip
	///
	void next(unsigned int skip = 1u);
	///
	/// \brief givemeNextRay
	/// \param imagen
	/// \param ang
	/// \return 
	///
	vxRay givemeNextRay(const vxContactBuffer &imagen, scalar ang);
	///
	/// \brief givemeRandomRay
	/// \param coord
	/// \return 
	///
	vxRay givemeRandomRay(const v2s &coord);
	///
	/// \brief prop
	/// \return 
	///
	std::shared_ptr<const ImageProperties> prop() const;
	///
	/// \brief setProp
	/// \param prop
	///
	void setProp(std::shared_ptr<const ImageProperties> prop);
	///
	/// \brief horizontalAperture
	/// \return 
	///
	scalar horizontalAperture() const;
	///
	/// \brief setHorizontalAperture
	/// \param horizontalAperture
	///
	void setHorizontalAperture(scalar horizontalAperture);
	///
	/// \brief verticalAperture
	/// \return 
	///
	scalar verticalAperture() const;
	///
	/// \brief setVerticalAperture
	/// \param verticalAperture
	///
	void setVerticalAperture(scalar verticalAperture);
	///
	/// \brief properties
	/// \return 
	///
	std::shared_ptr<const ImageProperties> properties() const;
	///
	/// \brief setProperties
	/// \param properties
	///
	void setProperties(const std::shared_ptr<const ImageProperties> &properties);
	///
	/// \brief transform
	/// \return 
	///
	Matrix44 transform() const;
	///
	/// \brief setTransform
	/// \param transform
	///
	void setTransform(const Matrix44 &transform);
	///
	/// \brief pixelRadius
	/// \return 
	/// 
	scalar pixelRadius() const;
	///
	/// \brief setPixelRadius
	/// \param pixelRadius
	///
	void setPixelRadius(const scalar &pixelRadius);
};

}
#endif // VXPXCAMERA

