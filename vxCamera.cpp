#include "vxCamera.h"

using namespace vxCore;


scalar vxCamera::horizontalAperture() const
{
	return m_horizontalAperture;
}

void vxCamera::setHorizontalAperture(scalar horizontalAperture)
{
	m_horizontalAperture = horizontalAperture;
}

scalar vxCamera::verticalAperture() const
{
	return m_verticalAperture;
}

void vxCamera::setVerticalAperture(scalar verticalAperture)
{
	m_verticalAperture = verticalAperture;
}
std::shared_ptr<const ImageProperties> vxCamera::properties() const
{
	return m_properties;
}

void vxCamera::setProperties(const std::shared_ptr<const ImageProperties> &properties)
{
	m_properties = properties;
	srand(time(NULL));
	m_rx = (scalar)m_properties->rx();
	m_ry = (scalar)m_properties->ry();
}

vxCamera::vxCamera(std::shared_ptr<const ImageProperties> prop) 
	: m_properties{prop}
{
	srand(time(NULL));
	m_rx = (scalar)m_properties->rx();
	m_ry = (scalar)m_properties->ry();
}

vxCamera::vxCamera(const v3 &position, 
					const v3 &orientation, 
					scalar focusD, 
					scalar apertureH, 
					scalar apertureV)
{
	m_orientation = orientation;
	m_position = position;
	m_focusDistance = focusD;
	m_horizontalAperture = apertureH;
	m_verticalAperture = apertureV;
	
	m_hApTan = tan(-m_horizontalAperture/2.0);
	m_vApTan = tan(-m_verticalAperture/2.0);

	srand(time(NULL));
}

void vxCamera::set(const v3& position, 
					const v3& orientation, 
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
	
	m_hApTan = tan(-m_horizontalAperture/2.0);
	m_vApTan = tan(-m_verticalAperture/2.0);
}

vxRay vxCamera::ray(const v2 &coord, vxSampler &sampler) const
{
	const auto& s = sampler.xy();
	scalar compX = m_hApTan * (( coord[0] * 2.0)-1.0)
					- 1.0/(scalar)(2.0 * m_rx)
					+ (s.x()+MU::getRand(0.05))/m_rx;

	scalar compY = m_vApTan * (( coord[1] * 2.0)-1.0)
					- 1.0/(scalar)(2.0 * m_ry)
					+ (s.y()+MU::getRand(0.05))/m_ry;

	auto&& ret = vxRay{compY, compX, m_focusDistance};

	//TODO:read from scene
	ret.setOrigin(v3(-12.0, 3.0, -12.0));
	ret.direction().rotateX( 2.0 * (MU::PI/8.0) );
	ret.direction().setUnit();
	return ret;
}

vxRay vxCamera::givemeRandRay(const v2 &coord)
{
	scalar compX = m_hApTan * (( coord.x() * 2.0) -1.0 ) 
			- 1.0/(scalar)(2.0 * m_properties->rx()) 
			+ ((rand()/(scalar)RAND_MAX))/(scalar)(m_properties->rx());

	scalar compY = m_vApTan * (( coord.y() * 2.0) -1.0 ) 
			- 1.0/(scalar)(2.0 * m_properties->ry()) 
			+ ((rand()/(scalar)RAND_MAX))/(scalar)(m_properties->ry());

	auto ret = vxRay( compY, compX, m_focusDistance );
	ret.direction().rotateY(0.333);
	return ret;
}

vxRay vxCamera::givemeNextRay(const vxContactBuffer &imagen, scalar ang)
{
	vxRay ret;
	auto&& direction = ret.direction();
	direction.set(tan(m_verticalAperture/2.0) * ((imagen.getScanYd()*2)-1),
				  tan(m_horizontalAperture/2.0)*(( imagen.getScanXd() *2)-1),
				  m_focusDistance);
	direction.setUnit();
	direction=direction.rotateY(ang);
	return ret;
}

vxRay vxCamera::givemeRandomRay(const v2 &coord)
{
	scalar yrv,xrv;
	xrv=((rand()/scalar(RAND_MAX)))/m_properties->rx();
	yrv=((rand()/scalar(RAND_MAX)))/m_properties->ry();
	return v3{m_hApTan * (((coord.y()+yrv)*2.0)-1.0) ,
						m_vApTan * (((coord.x()+xrv)*2.0)-1.0), 
						m_focusDistance};
}

std::shared_ptr<const ImageProperties> vxCamera::prop() const
{
	return m_properties;
}

void vxCamera::setProp(std::shared_ptr<const ImageProperties> prop)
{
	m_properties = prop;
}
