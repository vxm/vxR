#include "vxLight.h"
namespace vxCore {

vxPointLight::vxPointLight()
	:vxLight() {}

vxPointLight::vxPointLight(double instensity, const vxColor &col)
	:vxLight(instensity, col)
{}

double vxPointLight::luminance(const vxCollision &collide) const
{
	double angl = collide.normal().angle(getLightRay(collide.position()));
	
	if(angl>1.57)
		return 0.0;
	
	return m_intensity * cos(angl);
}

vxVector3d vxPointLight::getLightRay(const vxVector3d &position) const
{
	return vxLight::getLightRay(position);
}

vxSpotLight::vxSpotLight()
	:vxLight()
{
}

vxSpotLight::vxSpotLight(const vxVector3d &position, const vxVector3d &orientation, double maxAngle, double minAngle)
	:vxLight() 
{
	m_position.set(position);
	m_orientation.set(orientation);
	m_maxAngle=maxAngle;
	m_minAngle=minAngle;
}

void vxSpotLight::set(const vxVector3d &position, const vxVector3d &orientation, double maxAngle, double minAngle) 
{
	m_position.set(position);
	m_orientation.set(orientation);
	m_maxAngle=maxAngle;
	m_minAngle=minAngle;
}

void vxSpotLight::setOrientation(const vxVector3d &orientation) {m_orientation.set(orientation);}

double vxSpotLight::luminance(const vxCollision &collide) const
{
	return intensity()*collide.normal().angle(getLightRay(collide.position()));
}

vxDirectLight::vxDirectLight() {}

vxDirectLight::vxDirectLight(const vxVector3d &orientation, bool bidirectional) 
{
	m_orientation.set(orientation);
	m_biDirectional=bidirectional;
}

void vxDirectLight::set(const vxVector3d &orientation, bool bidirectional) 
{
	m_orientation.set(orientation);
	m_biDirectional=bidirectional;
}

double vxDirectLight::luminance(const vxCollision &collide) const
{
	return intensity()*collide.normal().angle(getLightRay(collide.position()));
}




}
