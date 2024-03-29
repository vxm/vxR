#include "Camera.h"
#include <utility>
using namespace vxCore;

scalar Camera::horizontalAperture() const { return m_horizontalAperture; }

void Camera::setHorizontalAperture(scalar horizontalAperture)
{
	m_horizontalAperture = horizontalAperture;
}

scalar Camera::verticalAperture() const { return m_verticalAperture; }

void Camera::setVerticalAperture(scalar verticalAperture)
{
	m_verticalAperture = verticalAperture;
}
std::shared_ptr<const ImageProperties> Camera::properties() const
{
	return m_properties;
}

void Camera::setProperties(

	const std::shared_ptr<const ImageProperties> &properties)
{
	m_properties = properties;
	srand(time(nullptr));
	m_rx = (scalar)m_properties->rx();
	m_ry = (scalar)m_properties->ry();
}

Matrix44 Camera::transform() const { return m_transform; }

void Camera::setTransform(const Matrix44 &transform)
{
	m_transform = transform;
}

scalar Camera::pixelRadius() const { return m_pixelRadius; }

void Camera::setPixelRadius(const scalar &pixelRadius)
{
	m_pixelRadius = pixelRadius;
}

scalar Camera::lensSize() const { return m_lensSize; }

void Camera::setLensSize(const scalar &lensSize) { m_lensSize = lensSize; }

scalar Camera::focalLength() const { return m_focalLength; }

void Camera::setFocalLength(const scalar &focalLength)
{
	m_focalLength = focalLength;
}

Camera::Camera(const std::shared_ptr<const ImageProperties> &&prop)
	: m_properties{prop}
{
	srand(time(nullptr));
	m_rx = (scalar)m_properties->rx();
	m_ry = (scalar)m_properties->ry();
}

Camera::Camera(const v3s &position, const v3s &orientation, scalar focusD,
			   scalar apertureH, scalar apertureV)
{
	m_orientation = orientation;
	m_position = position;
	m_focusDistance = focusD;
	m_horizontalAperture = apertureH;
	m_verticalAperture = apertureV;

	m_hApTan = tan(-m_horizontalAperture / scalar(2.0));
	m_vApTan = tan(-m_verticalAperture / scalar(2.0));

	srand(time(nullptr));
}

void Camera::set(const v3s &position, const v3s &orientation, scalar focusD,
				 scalar apertureH, scalar apertureV)
{
	m_orientation = orientation;
	m_position = position;
	m_focusDistance = focusD;

	if (std::max(apertureH + apertureV, (scalar)0.0) == (scalar)0.0)
	{
		m_horizontalAperture = 1.0;
		m_verticalAperture = m_properties->aspectRatio();
	}
	else
	{
		m_horizontalAperture = apertureH;
		m_verticalAperture = apertureV;
	}

	m_hApTan = tan(-m_horizontalAperture / scalar(2.0));
	m_vApTan = tan(-m_verticalAperture / scalar(2.0));
}

Ray Camera::ray(const v2s &coord, Sampler &sampler) const
{
	const auto xFactor = coord.x() * scalar(2.0) - 1.0;
	const auto yFactor = coord.y() * scalar(2.0) - 1.0;
	// std::cout << "Pixel factor " << xFactor << " " << yFactor << std::endl;

	const auto &&s = sampler.xy(m_pixelRadius);

	const auto compX = m_hApTan * xFactor - s.x() / (scalar(2.0) * m_rx);
	const auto compY = m_vApTan * yFactor - s.y() / (scalar(2.0) * m_ry);

	auto &&ret = Ray{{0, 0, 0},
					 {(scalar)compY, (scalar)compX, (scalar)m_focalLength},
					 VisionType::kAll};

	// TODO: rotate origin and then place the origin of the ray in position
	// TODO:read from scene.
	ret.setOrigin(m_transform.origin());

	ret.direction().rotateX(scalar(2.0) * (MU::PI / 8.0));
	// ret.direction() = ret.direction().rotate({0.990, 0.0, -1.1}, 0.510);

	ret.direction().setUnit();

	auto d = ret.direction().unit();
	auto orig = ret.origin();

	auto np = orig + (d * m_focusDistance);
	auto no = orig + (MU::getSolidSphereRand(m_lensSize));

	ret.setOrigin(no);

	ret.setDirection(np - no);

	return ret;
}

Ray Camera::givemeRandRay(const v2s &coord)
{
	scalar compX = m_hApTan * ((coord.x() * scalar(2.0)) - 1.0) -
				   1.0 / (scalar)(scalar(2.0) * m_properties->rx()) +
				   ((rand() / (scalar)RAND_MAX)) / (scalar)(m_properties->rx());

	scalar compY = m_vApTan * ((coord.y() * scalar(2.0)) - 1.0) -
				   1.0 / (scalar)(scalar(2.0) * m_properties->ry()) +
				   ((rand() / (scalar)RAND_MAX)) / (scalar)(m_properties->ry());

	auto ret = Ray(compY, compX, m_focusDistance);
	ret.direction().rotateY(0.333);
	return ret;
}

Ray Camera::givemeNextRay(const ContactBuffer &imagen, scalar ang)
{
	Ray ret;
	auto &&direction = ret.direction();
	direction.set(
		tan(m_verticalAperture / scalar(2.0)) * ((imagen.getScanYd() * 2) - 1),
		tan(m_horizontalAperture / scalar(2.0)) * ((imagen.getScanXd() * 2) - 1),
		m_focusDistance);
	direction.setUnit();
	direction = direction.rotateY(ang);
	return ret;
}

Ray Camera::givemeRandomRay(const v2s &coord)
{
	scalar yrv, xrv;
	xrv = ((rand() / scalar(RAND_MAX))) / m_properties->rx();
	yrv = ((rand() / scalar(RAND_MAX))) / m_properties->ry();
	return v3s{m_hApTan * (((coord.y() + yrv) * scalar(2.0)) - (scalar)1.0),
			   m_vApTan * (((coord.x() + xrv) * scalar(2.0)) - (scalar)1.0),
			   m_focusDistance};
}

std::shared_ptr<const ImageProperties> Camera::prop() const
{
	return m_properties;
}

void Camera::setProp(const std::shared_ptr<const ImageProperties> &prop)
{
	m_properties = prop;
}
