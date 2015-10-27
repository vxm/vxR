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

unsigned int vxLight::samples() const
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

double vxLight::lightRatio(const vxRay &ray,
							const vxVector3d &lightDirection) const
{
	double angl = ray.incidence(lightDirection);
	return angl;
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



double vxIBLight::gain() const
{
	return m_gain;
}

void vxIBLight::setGain(double gain)
{
	m_gain = gain;
}

double vxIBLight::gamma() const
{
	return m_gamma;
}

void vxIBLight::setGamma(double gamma)
{
	m_gamma = gamma;
}

double vxIBLight::lowThreshold() const
{
	return m_lowThreshold;
}

void vxIBLight::setLowThreshold(double lowThreshold)
{
	m_lowThreshold = lowThreshold;
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

vxColor vxLight::acummulationLight(const vxRay &, const vxCollision &collision) const
{
	vxColor acumColor;
	const auto&& cPnt = collision.position();
	const auto& n = samples();
	const auto& scn = m_scene.lock();
	const auto colorRatio = 1.0/(double)n;
	// compute all sort of shadows.
	for(auto i=0u; i<n; i++)
	{
		auto r = MU::getHollowSphereRand(radius());

		const vxRay f(cPnt, position() + r);
		auto lumm = m_intensity * lightRatio(f, position() + r);
		
		if (lumm>0.001 && !scn->hasCollision(f))
		{
			acumColor.mixSumm(color().gained(lumm), colorRatio);
		}
	}

	return acumColor;
}


vxColor vxDirectLight::acummulationLight(const vxRay &, const vxCollision &collision) const
{
	const auto&& cPnt = collision.position();

	vxRay f(cPnt, collision.normal());
	// compute all sort of shadows.
	vxColor ret{vxColor::black};
	
	
	if(f.direction().follows(m_orientation))
	{
		const auto&& ratio = lightRatio(f, m_orientation.inverted());
		auto lumm = m_intensity * ratio;
		//auto org = vxVector3d(0, 0, 0);
		const vxRay ff(cPnt, m_orientation.inverted());
		const auto&& scn = m_scene.lock();
		if (!scn->throwRay(ff))
		{
			ret = color().gained(lumm);
		}
	}
		
	return ret;
}


vxColor vxIBLight::acummulationLight(const vxRay &, const vxCollision &collision) const
{
	vxColor acumColor;
	// compute all sort of shadows.
	vxCollision environment;
	const auto n = samples();
	const auto colorRatio = 1.0/(double)n;
	for(auto i=0u; i<n; i++)
	{
		const auto&& cPnt = collision.position();
		const auto&& r = MU::getHollowSphereRand(radius());
		const vxRay f(cPnt, collision.normal()+r);
		auto lumm = m_intensity * lightRatio(f, collision.normal());

		environment.setUV(MU::normalToCartesian(f.direction()));
		auto environmentColor = m_map.compute(environment);
		const auto &luma = environmentColor.lumma();
		
		if (luma>m_lowThreshold && lumm>0.001)
		{
			const auto &scn = m_scene.lock();
			if(!scn->hasCollision(f))
			{
				environmentColor=environmentColor*(m_gain + pow(luma,m_gamma));
				acumColor.mixSumm(environmentColor.gained(lumm), colorRatio);
			}
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

vxColor vxAmbientLight::acummulationLight(const vxRay &, const vxCollision &) const
{
	return m_color*m_intensity;
}


}
