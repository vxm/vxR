#include "vxCamera.h"

using namespace vxCore;

vxCamera::vxCamera(std::shared_ptr<const ImageProperties> prop) 
	: m_prop{prop}
{
	srand(time(NULL));
	m_rx = (double)m_prop->rx();
	m_ry = (double)m_prop->ry();
}

vxCamera::vxCamera(const vxVector3d &position, 
					const vxVector3d &orientation, 
					double focusD, 
					double apertureH, 
					double apertureV)
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

void vxCamera::set(const vxVector3d& position, 
					const vxVector3d& orientation, 
					double focusD, 
					double apertureH, 
					double apertureV) 
{
	m_orientation=orientation;
	m_position=position;
	m_focusDistance=focusD;
	
	if(std::max(apertureH+apertureV,0.0)==0.0)
	{
		m_horizontalAperture = 1.0;
		m_verticalAperture = m_prop->aspectRatio();
	}
	else
	{	
		m_horizontalAperture = apertureH;
		m_verticalAperture = apertureV;
	}
	
	m_hApTan = tan(-m_horizontalAperture/2.0);
	m_vApTan = tan(-m_verticalAperture/2.0);
}

vxRay vxCamera::ray(const vxVector2d &coord, const vxSampler &sampler) const
{
	double compX = m_hApTan * (( coord[0] * 2.0)-1.0)
					- 1.0/(double)(2.0 * m_rx)
					+ sampler.x()/m_rx;

	double compY = m_vApTan * (( coord[1] * 2.0)-1.0)
					- 1.0/(double)(2.0 * m_ry)
					+ sampler.y()/m_ry;

	auto&& ret = vxRay{compY, compX, m_focusDistance};
	ret.direction().rotateX( -2 * (MathUtils::PI/8.0) );
	return ret;
}

vxRay vxCamera::givemeRandRay(const vxVector2d &coord)
{
	double compX = m_hApTan * (( coord.x() * 2.0) -1.0 ) 
			- 1.0/(double)(2.0 * m_prop->rx()) 
			+ ((rand()/(double)RAND_MAX))/(double)(m_prop->rx());

	double compY = m_vApTan * (( coord.y() * 2.0) -1.0 ) 
			- 1.0/(double)(2.0 * m_prop->ry()) 
			+ ((rand()/(double)RAND_MAX))/(double)(m_prop->ry());

	auto ret = vxRay( compY, compX, m_focusDistance );
	ret.direction().rotateY(0.333);
	return ret;
}

vxRay vxCamera::givemeNextRay(const vxContactBuffer &imagen, double ang)
{
	vxRay ret;
	auto&& direction = ret.direction();
	direction.set(tan(m_verticalAperture/2.0) * ((imagen.getScanYd()*2)-1) , tan(m_horizontalAperture/2.0)*(( imagen.getScanXd() *2)-1), m_focusDistance);
	direction.setUnit();
	direction=direction.rotateY(ang);
	return ret;
}

vxRay vxCamera::givemeRandomRay(const vxVector2d &coord)
{
	double yrv,xrv;
	xrv=((rand()/double(RAND_MAX)))/m_prop->rx();
	yrv=((rand()/double(RAND_MAX)))/m_prop->ry();
	return vxVector3d(m_hApTan * (((coord.y()+yrv)*2.0)-1.0) ,
						m_vApTan * (((coord.x()+xrv)*2.0)-1.0), 
						m_focusDistance);
}

std::shared_ptr<const ImageProperties> vxCamera::prop() const
{
	return m_prop;
}

void vxCamera::setProp(std::shared_ptr<const ImageProperties> prop)
{
	m_prop = prop;
}
