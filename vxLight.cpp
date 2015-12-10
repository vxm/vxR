#include "vxLight.h"
namespace vxCore 
{

bool vxLight::computeShadows() const
{
	return m_castShadows;
}

void vxLight::setComputeShadows(bool castShadows)
{
	m_castShadows = castShadows;
}

vxMatrix vxLight::getTransform() const
{
	return m_transform;
}

void vxLight::setTransform(const vxMatrix &transform)
{
	m_transform = transform;
}

vxLight::vxLight()
{
}

vxLight::vxLight(scalar intensity, const vxColor &color)
	:m_intensity(intensity)
	,m_color(color)
{
}

vxLight::vxLight(const v3 &position) 
	:m_position(position)
{
}

vxLight::vxLight(scalar x, scalar y, scalar z)
	:m_position(x,y,z)
{
}

scalar vxLight::radius() const
{
	return m_radius;
}

void vxLight::setRadius(scalar radius)
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

void vxLight::setPosition(scalar x, scalar y, scalar z) 
{
	m_position.set(x,y,z);
}

void vxLight::set(scalar intensity, const v3 &color) 
{
	m_intensity = intensity;
	m_color = color;
}

void vxLight::setIntensity(scalar intensity) 
{
	m_intensity = intensity;
}

void vxLight::setColor(const vxColor &color) 
{
	m_color = color;
}

v3 vxLight::getLightRay(const v3 &position) const
{
	return m_position-position;
}

scalar vxLight::lightRatio(const vxRay &ray,
							const v3 &lightDirection) const
{
	return ray.incidence(lightDirection);
}

void vxLight::setPosition(const v3 &position)
{
	m_position.set(position);
}

vxDirectLight::vxDirectLight()
	:vxLight()
{
}

vxDirectLight::vxDirectLight(scalar instensity, const vxColor &col)
	:vxLight(instensity, col)
{}


vxDirectLight::vxDirectLight(const v3 &orientation, bool bidirectional) 
	: m_orientation(orientation)
	, m_biDirectional(bidirectional)
{
}

void vxDirectLight::set(const v3 &orientation, bool bidirectional) 
{
	m_orientation.set(orientation.unit());
	m_biDirectional=bidirectional;
}



vxSpotLight::vxSpotLight()
	:vxLight()
{
}

vxSpotLight::vxSpotLight(const v3 &position, 
						 const v3 &orientation, 
						 scalar maxAngle, 
						 scalar minAngle)
	: vxLight(position)
	, m_orientation(orientation)
	, m_maxAngle(maxAngle)
	, m_minAngle(minAngle)
{
}

void vxSpotLight::set(const v3 &position, 
					  const v3 &orientation, 
					  scalar maxAngle, 
					  scalar minAngle) 
{
	m_position.set(position);
	m_orientation.set(orientation);
	m_maxAngle=maxAngle;
	m_minAngle=minAngle;
}

void vxSpotLight::setOrientation(const v3 &orientation) 
{
	m_orientation.set(orientation);
}

vxPointLight::vxPointLight()
	:vxLight()
{
}

vxPointLight::vxPointLight(scalar instensity, const vxColor &col)
	:vxLight(instensity, col)
{}


vxPointLight::vxPointLight(const v3 &orientation, bool biPointional) 
	: m_orientation(orientation)
	, m_biDirectional(biPointional)
{
}

void vxPointLight::set(const v3 &orientation, bool biPointional) 
{
	m_orientation.set(orientation.unit());
	m_biDirectional=biPointional;
}
vxColor vxPointLight::acummulationLight(const vxRay &, const vxCollision &collision) const
{
	const auto pp = collision.position();
	const auto p = pp - m_transform.getOrigin();

	vxRay f(p, collision.normal());
	// compute all sort of shadows.
	vxColor ret{vxColor::black};
	
	if(collision.normal().follows(p))
	{
		auto ratio = lightRatio(f, p.inverted());
		auto lumm = m_intensity * ratio;

		const vxRay ff(pp+collision.normal()/10000.0, p.inverted());
		const auto&& scn = m_scene.lock();
		//if (!m_castShadows || !scn->throwRay(ff))
		{
			ret = color().gained(lumm);
		}
	}
	
	return ret;
}



scalar vxIBLight::gain() const
{
	return m_gain;
}

void vxIBLight::setGain(scalar gain)
{
	m_gain = gain;
}

scalar vxIBLight::gamma() const
{
	return m_gamma;
}

void vxIBLight::setGamma(scalar gamma)
{
	m_gamma = gamma;
}

scalar vxIBLight::lowThreshold() const
{
	return m_lowThreshold;
}

void vxIBLight::setLowThreshold(scalar lowThreshold)
{
	m_lowThreshold = lowThreshold;
}
vxIBLight::vxIBLight()
	:m_map("")
{
}

vxIBLight::vxIBLight(scalar instensity, const vxColor &col)
	:vxLight(instensity, col)
	,m_map("")
{
}

vxIBLight::vxIBLight(scalar instensity, const std::string path)
	:vxLight(instensity, vxColor::white)
	,m_map(path)
{
}


v3 vxIBLight::getLightRay(const v3 &position) const
{
	return (m_position-position);
}

vxColor vxLight::acummulationLight(const vxRay &, const vxCollision &collision) const
{
	vxColor acumColor;
	const auto&& cPnt = collision.position();
	const auto& n = samples();
	const auto& scn = m_scene.lock();
	const auto colorRatio = 1.0/(scalar)n;
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
	
	if(collision.normal().follows(m_orientation))
	{
		auto ratio = lightRatio(f, m_orientation.inverted());
		auto lumm = m_intensity * ratio;

		const vxRay ff(cPnt+collision.normal()/100000.0, m_orientation.inverted());
		const auto&& scn = m_scene.lock();
		if (!m_castShadows || !scn->throwRay(ff))
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
	const auto colorRatio = 1.0/(scalar)n;
	for(auto i=0u; i<n; i++)
	{
		const auto&& cPnt = collision.position();
		const auto&& r = MU::getHollowSphereRand(radius());
		const vxRay f(cPnt, r);
		auto lumm = m_intensity * lightRatio(f, collision.normal());

		environment.setUV(MU::normalToCartesian(f.direction()));
		auto environmentColor = m_map.compute(environment);
		const auto &luma = environmentColor.lumma();
		
		if (luma>m_lowThreshold && lumm>0.0001)
		{
			const auto &scn = m_scene.lock();
			vxCollision col;
			scn->throwRay(f,col);
			if(col.isValid())
			{
				acumColor.mixSumm(col.color().gained(.01), colorRatio);
			}
			else
			{
				environmentColor=environmentColor*(scalar)(m_gain + pow(luma,m_gamma));
				acumColor.mixSumm(environmentColor.gained(lumm), colorRatio);
			}
		}
	}

	return acumColor;
}

vxAmbientLight::vxAmbientLight()
{
	
}

vxAmbientLight::vxAmbientLight(scalar intensity, const vxColor &color)
	:vxLight(intensity, color)
{
}

v3 vxAmbientLight::getLightRay(const v3 &position) const
{
	return position.inverted();
}

vxColor vxAmbientLight::acummulationLight(const vxRay &, const vxCollision &) const
{
	return m_color*m_intensity;
}


}
