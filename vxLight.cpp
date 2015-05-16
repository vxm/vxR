#include "vxLight.h"
namespace vxCore {



double vxLight::radius() const
{
	return m_radius;
}

void vxLight::setRadius(double radius)
{
	m_radius = radius;
}

int vxLight::samples() const
{
	return m_samples;
}

void vxLight::setSamples(int samples)
{
	m_samples = samples;
}
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

void vxLight::setScene(std::weak_ptr<vxScene> scene)
{
	m_scene = scene;
}

void vxLight::setPosition(double x, double y, double z) 
{
	m_position.set(x,y,z);
}

void vxLight::set(double intensity, vxVector3d color) {m_intensity=intensity;m_color=color;}

void vxLight::setIntensity(double intensity) {m_intensity=intensity;}

void vxLight::setColor(vxColor color) {m_color=color;}

vxVector3d vxLight::getLightRay(const vxVector3d &position) const
{
	return m_position-position;
}

double vxLight::lightRatio(const vxVector3d &position,
						   const vxVector3d &normal) const
{
	double angl = normal.angle(getLightRay(position));
	
	if(angl>1.57)
		return 0.0;
	
	return angl;
}

double vxLight::acumLight(const vxCollision &collision) const
{
	double acumLumm{0.0};
	const auto &cPnt = collision.position();
	const auto& n = samples();
	//m_scene.lock();
	// compute shadows.
	auto sm = m_scene.lock();
	for(auto i=0; i<n; i++)
	{
		auto r = MathUtils::getHollowSphereRand(radius());
		const vxRayXYZ f = (position() + r) - cPnt;
		auto lumm = m_intensity * lightRatio(cPnt-r, collision.normal());

		if (lumm>0.001 && !sm->hasCollision(cPnt, f))
		{
			acumLumm += fabs(lumm/n);
		}
	}

	return acumLumm;
}

void vxLight::setPosition(const vxVector3d &position)
{
	m_position.set(position);
}

vxPointLight::vxPointLight()
	:vxLight()
{
}

vxPointLight::vxPointLight(double instensity, const vxColor &col)
	:vxLight(instensity, col)
{

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


vxIBLight::vxIBLight()
{
	
}

vxIBLight::vxIBLight(double instensity, const vxColor &col)
	:vxLight(instensity, col)
{
	
}


vxVector3d vxIBLight::getLightRay(const vxVector3d &position) const
{
	return (m_position-position);
}




}
