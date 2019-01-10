#ifndef VXCAMERAMC
#define VXCAMERAMC

#include <iostream>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ContactBuffer.h"
#include "ImageProperties.h"
#include "Matrix44.h"
#include "Ray.h"
#include "Sampler.h"
#include "Vector.h"

namespace vxCore
{

class Camera
{

private:
	v3s m_position{0.0, 0.0, 0.0};
	v3s m_orientation{0.0, 0.0, 1.0};

	scalar m_focusDistance = {1.0};

	scalar m_focalLength = {1.0};
	scalar m_lensSize = {0.02};
	scalar m_horizontalAperture = {1.42};
	scalar m_verticalAperture = {1.42};

	scalar m_hApTan = tan(-m_horizontalAperture / scalar(2.0));
	scalar m_vApTan = tan(-m_verticalAperture / scalar(2.0));
	scalar m_pixelRadius{1.0};

	scalar m_rx = 0;
	scalar m_ry = 0;

	std::shared_ptr<const ImageProperties> m_properties;

	Matrix44 m_transform;

public:
	///
	/// \brief vxCamera
	/// \param prop
	///
	Camera(const std::shared_ptr<const ImageProperties> &&prop);
	///
	/// \brief vxCamera
	/// \param position
	/// \param orientation
	/// \param focusD
	/// \param apertureH
	/// \param apertureV
	///
	Camera(const v3s &position, const v3s &orientation, scalar focusD,
		   scalar apertureH, scalar apertureV);
	///
	/// \brief set
	/// \param position
	/// \param orientation
	/// \param focusD
	/// \param apertureH
	/// \param apertureV
	///
	void set(const v3s &position, const v3s &orientation, scalar focusD = 1.0,
			 scalar apertureH = 0.0, scalar apertureV = 0.0);
	///
	/// \brief ray
	/// \param coord
	/// \param sampler
	/// \return
	///
	Ray ray(const v2s &coord, Sampler &sampler) const;
	///
	/// \brief givemeRandRay
	/// \param coord
	/// \return
	///
	Ray givemeRandRay(const v2s &coord);
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
	Ray givemeNextRay(const ContactBuffer &imagen, scalar ang);
	///
	/// \brief givemeRandomRay
	/// \param coord
	/// \return
	///
	Ray givemeRandomRay(const v2s &coord);
	///
	/// \brief prop
	/// \return
	///
	std::shared_ptr<const ImageProperties> prop() const;
	///
	/// \brief setProp
	/// \param prop
	///
	void setProp(const std::shared_ptr<const ImageProperties> &prop);
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
	scalar lensSize() const;
	void setLensSize(const scalar &lensSize);
	scalar focalLength() const;
	void setFocalLength(const scalar &focalLength);
};

} // namespace vxCore
#endif // VXPXCAMERA
