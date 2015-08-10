#include "vxLight.h"
namespace vxCore 
{

vxLight::vxLight()
{
}

vxLight::vxLight(double intensity, const vxColor &color)
	:m_intensity(intensity)
	,m_color(color)
{
}

vxLight::vxLight(const vxVector3d &position) 
	:m_position(position)
{
}

vxLight::vxLight(double x, double y, double z)
	:m_position(x,y,z)
{
}

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

void vxLight::setScene(std::weak_ptr<vxScene> scene)
{
	m_scene = scene;
}

void vxLight::setPosition(double x, double y, double z) 
{
	m_position.set(x,y,z);
}

void vxLight::set(double intensity, const vxVector3d &color) 
{
	m_intensity = intensity;
	m_color = color;
}

void vxLight::setIntensity(double intensity) 
{
	m_intensity = intensity;
}

void vxLight::setColor(const vxColor &color) 
{
	m_color = color;
}

vxVector3d vxLight::getLightRay(const vxVector3d &position) const
{
	return m_position-position;
}

double vxLight::lightRatio(const vxVector3d &origin,
							const vxVector3d &direction,
							const vxVector3d &destiny) const
{
	double angl = (destiny-origin).angle(direction);
	
	//backlight support?
	if(angl>1.57)
	{
		return 0.0;
	}

	return MathUtils::clamp(cos(angl),0.0,1.0);
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

vxSpotLight::vxSpotLight(const vxVector3d &position, 
						 const vxVector3d &orientation, 
						 double maxAngle, 
						 double minAngle)
	: vxLight(position)
	, m_orientation(orientation)
	, m_maxAngle(maxAngle)
	, m_minAngle(minAngle)
{
}

void vxSpotLight::set(const vxVector3d &position, 
					  const vxVector3d &orientation, 
					  double maxAngle, 
					  double minAngle) 
{
	m_position.set(position);
	m_orientation.set(orientation);
	m_maxAngle=maxAngle;
	m_minAngle=minAngle;
}

void vxSpotLight::setOrientation(const vxVector3d &orientation) 
{
	m_orientation.set(orientation);
}

vxDirectLight::vxDirectLight()
	:vxLight()
{
}

vxDirectLight::vxDirectLight(double instensity, const vxColor &col)
	:vxLight(instensity, col)
{}


vxDirectLight::vxDirectLight(const vxVector3d &orientation, bool bidirectional) 
	: m_orientation(orientation)
	, m_biDirectional(bidirectional)
{
}

void vxDirectLight::set(const vxVector3d &orientation, bool bidirectional) 
{
	m_orientation.set(orientation.unit());
	m_biDirectional=bidirectional;
}


vxIBLight::vxIBLight()
	:m_map("")
{
}

vxIBLight::vxIBLight(double instensity, const vxColor &col)
	:vxLight(instensity, col)
	,m_map("")
{
}

vxIBLight::vxIBLight(double instensity, const std::string path)
	:vxLight(instensity, vxColor::white)
	,m_map(path)
{
}


vxVector3d vxIBLight::getLightRay(const vxVector3d &position) const
{
	return (m_position-position);
}

vxColor vxLight::acummulationLight(const vxCollision &collision) const
{
	vxColor acumColor;
	const auto& cPnt = collision.position();
	const auto& n = samples();
	const auto& scn = m_scene.lock();
	const auto colorRatio = 1.0/(double)n;
	// compute all sort of shadows.
	for(auto i=0; i<n; i++)
	{
		auto r = MathUtils::getHollowSphereRand(radius());

		const vxRay f(cPnt, position() + r);
		auto lumm = m_intensity * lightRatio(cPnt, 
											 collision.normal(), 
											 position() + r);
		
		if (lumm>0.001 && !scn->hasCollision(f))
		{
			//acumColor += fabs(lumm/n);
			acumColor.mixSumm(color().gained(lumm), colorRatio);
		}
	}

	return acumColor;
}


vxColor vxDirectLight::acummulationLight(const vxCollision &collision) const
{
	const auto& cPnt = collision.position();
	const auto &scn = m_scene.lock();

	// compute all sort of shadows.
	auto lumm = m_intensity * lightRatio(cPnt, 
										 collision.normal(), 
										cPnt + m_orientation.inverted());

	const vxRay f(cPnt, m_orientation.inverted());
	return scn->hasCollision(f) ? vxColor::black : color().gained(lumm);
}


vxColor vxIBLight::acummulationLight(const vxCollision &collision) const
{
	vxColor acumColor;
	// compute all sort of shadows.
	vxCollision environment;
	const auto&& n = samples();
	for(auto i=0; i<n; i++)
	{
		const auto&& cPnt = collision.position();
		const auto&& r = MathUtils::getHollowHemisphereRand(radius(),
													  collision.normal());
		const vxRay f(cPnt, collision.normal()+r);

		auto lumm = m_intensity * lightRatio(cPnt, 
											 collision.normal(), 
											cPnt + f.direction());

		const auto &scn = m_scene.lock();
		if (lumm>0.0001 && !scn->hasCollision(f))
		{
			double colorRatio = 1.0/(double)n;
			environment.setUV(MathUtils::normalToCartesian(f.direction()));
			auto environmentColor = m_map.compute(environment);
			acumColor.mixSumm(environmentColor.gained(lumm), colorRatio);
		}
	}

	return acumColor;
}

vxAmbientLight::vxAmbientLight()
{
	
}

vxAmbientLight::vxAmbientLight(double intensity, const vxColor &color)
	:vxLight(intensity, color)
{
}

vxVector3d vxAmbientLight::getLightRay(const vxVector3d &position) const
{
	return position.inverted();
}

vxColor vxAmbientLight::acummulationLight(const vxCollision &) const
{
	return m_color*m_intensity;
}


}
