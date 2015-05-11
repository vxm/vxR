#include "vxLight.h"
namespace vxCore {


vxLight::vxLight()
{
}

vxLight::vxLight(const vxVector3d &position) {m_position.set(position);}

vxLight::vxLight(double intensity, const vxColor &color)
	:m_intensity(intensity)
	,m_color(color)
{
}

vxLight::vxLight(double x, double y, double z) {m_position.set(x,y,z);}

void vxLight::setPosition(double x, double y, double z) {m_position.set(x,y,z);}

void vxLight::set(double intensity, vxVector3d color) {m_intensity=intensity;m_color=color;}

void vxLight::setIntensity(double intensity) {m_intensity=intensity;}

void vxLight::setColor(vxColor color) {m_color=color;}

vxVector3d vxLight::getLightRay(const vxVector3d &position) const
{
	return m_position-position;
}

void vxLight::setPosition(const vxVector3d &position) {m_position.set(position);}

vxPointLight::vxPointLight()
	:vxLight() 
{
}

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

vxDirectLight::vxDirectLight()
	:vxLight()
{
}

vxDirectLight::vxDirectLight(double instensity, const vxColor &col)
	:vxLight(instensity, col)
{}


vxDirectLight::vxDirectLight(const vxVector3d &orientation, bool bidirectional) 
{
	m_orientation.set(orientation);
	m_biDirectional=bidirectional;
}

void vxDirectLight::set(const vxVector3d &orientation, bool bidirectional) 
{
	m_orientation.set(orientation.unit());
	m_biDirectional=bidirectional;
}

double vxDirectLight::luminance(const vxCollision &collide) const
{
	double angl = collide.normal().angle(m_orientation.inverted());
	if(angl>1.57)
	{
		return 0.0;
	}
	return m_intensity * cos(angl);
}




}
