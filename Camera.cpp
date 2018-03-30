#include "Camera.h"

using namespace vxCore;


scalar Camera::horizontalAperture() const
{
	return m_horizontalAperture;
}

void Camera::setHorizontalAperture(scalar horizontalAperture)
{
	m_horizontalAperture = horizontalAperture;
}

scalar Camera::verticalAperture() const
{
	return m_verticalAperture;
}

void Camera::setVerticalAperture(scalar verticalAperture)
{
	m_verticalAperture = verticalAperture;
}
std::shared_ptr<const ImageProperties> Camera::properties() const
{
	return m_properties;
}

void Camera::setProperties(const std::shared_ptr<const ImageProperties> &properties)
{
	m_properties = properties;
	srand(time(NULL));
	m_rx = (scalar)m_properties->rx();
	m_ry = (scalar)m_properties->ry();
}

Matrix44 Camera::transform() const
{
	return m_transform;
}

void Camera::setTransform(const Matrix44 &transform)
{
	m_transform = transform;
}

scalar Camera::pixelRadius() const
{
	return m_pixelRadius;
}

void Camera::setPixelRadius(const scalar &pixelRadius)
{
	m_pixelRadius = pixelRadius;
}

Camera::Camera(std::shared_ptr<const ImageProperties> prop) 
	: m_properties{prop}
{
	srand(time(NULL));
	m_rx = (scalar)m_properties->rx();
	m_ry = (scalar)m_properties->ry();
}

Camera::Camera(const v3s &position, 
				   const v3s &orientation, 
				   scalar focusD, 
				   scalar apertureH, 
				   scalar apertureV)
{
	m_orientation = orientation;
	m_position = position;
	m_focusDistance = focusD;
	m_horizontalAperture = apertureH;
	m_verticalAperture = apertureV;
	
	m_hApTan = tan(-m_horizontalAperture/scalar(2.0));
	m_vApTan = tan(-m_verticalAperture/scalar(2.0));
	
	srand(time(NULL));
}

void Camera::set(const v3s& position, 
				   const v3s& orientation, 
				   scalar focusD, 
				   scalar apertureH, 
				   scalar apertureV) 
{
	m_orientation=orientation;
	m_position=position;
	m_focusDistance=focusD;
	
	if(std::max(apertureH+apertureV,(scalar)0.0)==(scalar)0.0)
	{
		m_horizontalAperture = 1.0;
		m_verticalAperture = m_properties->aspectRatio();
	}
	else
	{
		m_horizontalAperture = apertureH;
		m_verticalAperture = apertureV;
	}
	
	m_hApTan = tan(-m_horizontalAperture/scalar(2.0));
	m_vApTan = tan(-m_verticalAperture/scalar(2.0));
}

Ray Camera::ray(const v2s &coord, Sampler &sampler) const
{
	const auto xFactor = coord.x() * scalar(2.0) -1.0;
	const auto yFactor = coord.y() * scalar(2.0) -1.0;
	//std::cout << "Pixel factor " << xFactor << " " << yFactor << std::endl;
	
	const auto&& s = sampler.xy(m_pixelRadius);
	
	const auto compX = m_hApTan * xFactor - s.x()/(scalar(2.0) * m_rx);
	const auto compY = m_vApTan * yFactor - s.y()/(scalar(2.0) * m_ry);

	v3s pos = m_transform.origin();
	v3s ta =  v3s( 0,0,10000.0 );
	scalar roll = 0.0;

	const scalar FOCUSDISTANCE = 5.45;

	// camera tx
	v3s cw = (ta-pos).unit();
	v3s cp = v3s( sin(roll), cos(roll), 0.0 );
	v3s cu = cw.cross(cp).unit();
	v3s cv = cu.cross(cw).unit();

	v3s er = {compY, compX, m_focusDistance};

	v3s rd = cu * er.x() + cv * er.y() + cw * er.z();

	v3s go = v3s( s.x()/2.0,s.y()/2.0, 0.0 ) * 0.15;
	v3s gd = ( er*FOCUSDISTANCE - go ).unit();
	
	pos += cu*go.x() + cv*go.y();
	rd += cu*gd.x() + cv*gd.y();
	
	auto ret = Ray{{0,0,0}, rd.unit(), VisionType::kAll};
	
	//TODO: rotate origin and then place the origin of the ray in position
	//TODO:read from scene.
	ret.setOrigin(m_transform.origin());
	
	ret.direction().rotateX( scalar(2.0) * (MU::PI/8.0) );
	ret.direction() = ret.direction().rotate({0.990, 0.0, -1.1}, 0.05);
	
	ret.direction().setUnit();
	
	return ret;
}

Ray Camera::givemeRandRay(const v2s &coord)
{
	scalar compX = m_hApTan * (( coord.x() * scalar(2.0)) -1.0 ) 
			- 1.0/(scalar)(scalar(2.0) * m_properties->rx()) 
			+ ((rand()/(scalar)RAND_MAX))/(scalar)(m_properties->rx());
	
	scalar compY = m_vApTan * (( coord.y() * scalar(2.0)) -1.0 ) 
			- 1.0/(scalar)(scalar(2.0) * m_properties->ry()) 
			+ ((rand()/(scalar)RAND_MAX))/(scalar)(m_properties->ry());
	
	auto ret = Ray( compY, compX, m_focusDistance );
	ret.direction().rotateY(0.333);
	return ret;
}

Ray Camera::givemeNextRay(const ContactBuffer &imagen, scalar ang)
{
	Ray ret;
	auto&& direction = ret.direction();
	direction.set(tan(m_verticalAperture/scalar(2.0)) * ((imagen.getScanYd()*2)-1),
				  tan(m_horizontalAperture/scalar(2.0))*(( imagen.getScanXd() *2)-1),
				  m_focusDistance);
	direction.setUnit();
	direction=direction.rotateY(ang);
	return ret;
}

Ray Camera::givemeRandomRay(const v2s &coord)
{
	scalar yrv,xrv;
	xrv=((rand()/scalar(RAND_MAX)))/m_properties->rx();
	yrv=((rand()/scalar(RAND_MAX)))/m_properties->ry();
	return v3s{m_hApTan * (((coord.y()+yrv)*scalar(2.0))-(scalar)1.0) ,
				m_vApTan * (((coord.x()+xrv)*scalar(2.0))-(scalar)1.0), 
				m_focusDistance};
}

std::shared_ptr<const ImageProperties> Camera::prop() const
{
	return m_properties;
}

void Camera::setProp(std::shared_ptr<const ImageProperties> prop)
{
	m_properties = prop;
}
