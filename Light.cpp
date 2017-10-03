#include "Light.h"
#include "Scene.h"
#include "LightShader.h"

using namespace vxCore;

Light::Light()
{
	m_shader = std::make_shared<LightShader>();
}

Light::Light(const scalar intensity)
	: m_intensity(intensity)
	, m_castShadows(true)
{
	m_shader = std::make_shared<LightShader>();
}

Light::Light(const v3s &position) 
	:m_position(position)
{
	m_shader = std::make_shared<LightShader>();
}

Light::Light(scalar x, scalar y, scalar z)
	:m_position(x,y,z)
{
	m_shader = std::make_shared<LightShader>();
}


bool Light::computeShadows() const
{
	return m_castShadows;
}

void Light::setComputeShadows(bool castShadows)
{
	m_castShadows = castShadows;
}

Matrix44 Light::getTransform() const
{
	return m_transform;
}

void Light::setTransform(const Matrix44 &transform)
{
	m_transform = transform;
}

bool Light::reachesLightSource(const Ray &ray) const
{
	Collision col;
	
	const auto hits  = m_scene.lock()->throwRay(ray,col);
	
	/// what about distance?.
	if(!hits)
	{
		return true;
	}
	
	const auto originToHit = ray.origin().distance(col.position());
	
	if(originToHit==scalar(0.0))
		return true;
	
	const auto originToSource = ray.origin().distance(m_transform.origin());
	
	return originToSource<originToHit;
}

scalar Light::radius() const
{
	return m_radius;
}

void Light::setRadius(scalar radius)
{
	m_radius = radius;
}

unsigned int Light::samples() const
{
	return m_samples;
}

void Light::setSamples(int samples)
{
	m_samples = samples;
}

void Light::setScene(std::weak_ptr<Scene> scene)
{
	m_scene = scene;
}

void Light::setPosition(scalar x, scalar y, scalar z) 
{
	m_position.set(x,y,z);
}

void Light::set(scalar intensity, const v3s &color) 
{
	m_intensity = intensity;
	m_color = color;
}

void Light::setIntensity(scalar intensity) 
{
	m_intensity = intensity;
}

v3s Light::position() const 
{
	return m_position;
}

v3s Light::getLightRay(const v3s &position) const
{
	return m_position-position;
}

scalar Light::lightRatio(const Ray &ray,
							const v3s &lightDirection) const
{
	return ray.incidence(lightDirection);
}

void Light::setPosition(const v3s &position)
{
	m_position.set(position);
}

DirectLight::DirectLight()
	:Light()
{
}

DirectLight::DirectLight(const v3s &orientation, bool bidirectional) 
	: m_orientation(orientation)
	, m_biDirectional(bidirectional)
{
}

void DirectLight::set(const v3s &orientation, bool bidirectional) 
{
	m_orientation.set(orientation.unit());
	m_biDirectional=bidirectional;
}

SpotLight::SpotLight()
	:Light()
{
}

SpotLight::SpotLight(const v3s &position, 
						 const v3s &orientation, 
						 scalar maxAngle, 
						 scalar minAngle)
	: Light(position)
	, m_orientation(orientation)
	, m_maxAngle(maxAngle)
	, m_minAngle(minAngle)
{
}

void SpotLight::set(const v3s &position, 
					  const v3s &orientation, 
					  scalar maxAngle, 
					  scalar minAngle) 
{
	m_position.set(position);
	m_orientation.set(orientation);
	m_maxAngle=maxAngle;
	m_minAngle=minAngle;
}

void SpotLight::setOrientation(const v3s &orientation) 
{
	m_orientation.set(orientation);
}

////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Point Light ////////////////////////
////////////////////////////////////////////////////////////////////////////

PointLight::PointLight()
	:Light()
{
}

PointLight::PointLight(const v3s &orientation, bool biPointional) 
	: m_orientation(orientation)
	, m_biDirectional(biPointional)
{
}

void PointLight::set(const v3s &orientation, bool biPointional) 
{
	m_orientation.set(orientation.unit());
	m_biDirectional=biPointional;
}

Color PointLight::acummulationLight(const Ray &, const Collision &collision) const
{
	const auto&& pp = collision.position();
	const auto&& p = pp - m_transform.origin();
	
	Ray f(p, collision.normal());
	// compute all sort of shadows.
	Color ret{Color::zero};
	
	if(collision.normal().follows(p))
	{
		auto ratio = lightRatio(f, p.inverted());
		auto lumm = m_intensity * ratio;
		
		const Ray ff(pp+collision.normal().small(), p.inverted());
		
		if (m_castShadows && reachesLightSource(ff))
		{
			ret = color().gained(lumm);
		}
	}
	
	return ret;
}

////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Sun Light ////////////////////////
////////////////////////////////////////////////////////////////////////////

SunLight::SunLight()
	:Light()
{
}

void SunLight::set(const v3s &orientation)
{
	m_orientation.set(orientation.unit());
}

Color SunLight::acummulationLight(const Ray &, const Collision &collision) const
{
	const auto&& pp = collision.position();
	auto position = v3s(0,0,1).rotate({1.0,0.0,0.0}, 4.0) * m_distance;
	position+=MU::getSolidSphereRand(m_radius);
	const auto&& p = pp - position;
	
	Ray f(p, collision.normal());
	// compute all sort of shadows.
	Color ret{Color::zero};
	
	if(collision.normal().follows(p))
	{
		auto ratio = lightRatio(f, p.inverted());
		auto lumm = m_intensity * ratio;
		
		const Ray ff(pp+collision.normal().small(), p.inverted());
		
		if (m_castShadows && reachesLightSource(ff))
		{
			ret = color().gained(lumm);
		}
	}
	
	return ret;
}

////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// IBL Light ////////////////////////
////////////////////////////////////////////////////////////////////////////

scalar IBLight::gain() const
{
	return m_gain;
}

void IBLight::setGain(scalar gain)
{
	m_gain = gain;
}

scalar IBLight::gamma() const
{
	return m_gamma;
}

void IBLight::setGamma(scalar gamma)
{
	m_gamma = gamma;
}

scalar IBLight::lowThreshold() const
{
	return m_lowThreshold;
}

void IBLight::setLowThreshold(scalar lowThreshold)
{
	m_lowThreshold = lowThreshold;
}
IBLight::IBLight()
	:m_map("")
{
}

IBLight::IBLight(scalar instensity, const std::string path)
	:Light(instensity)
	,m_map(path)
{
}


v3s IBLight::getLightRay(const v3s &position) const
{
	return (m_position-position);
}


////////////////////////////////////////////////////////////////////////////
//////////////////////////// Accumulation functions ////////////////////////
////////////////////////////////////////////////////////////////////////////

/* This is virtual now
Color Light::acummulationLight(const Ray &, const Collision &collision) const
{
	Color acumColor;
	const auto&& cPnt = collision.position();
	const auto& n = samples();
	const auto& scn = m_scene.lock();
	const auto colorRatio = 1.0/(scalar)n;
	// compute all sort of shadows.
	for(auto i=0u; i<n; i++)
	{
		auto r = MU::getHollowSphereRand(radius());

		const Ray f(cPnt, position() + r);
		auto lumm = m_intensity * lightRatio(f, position() + r);
		
		if (lumm>0.001 && !scn->hasCollision(f))
		{
			acumColor.mixSumm(color().gained(lumm), colorRatio);
		}
	}

	return acumColor;
}
*/

Color DirectLight::acummulationLight(const Ray &, 
										 const Collision &collision) const
{
	const auto cPnt = collision.position();

	Ray f(cPnt, collision.normal());
	// compute all sort of shadows.
	Color ret{Color::zero};
	
	if(collision.normal().follows(m_orientation))
	{
		auto ratio = lightRatio(f, m_orientation.inverted());
		auto lumm = m_intensity * ratio;

		v3s littleNormal = collision.normal()/scalar(10000.0);
		const Ray ff(cPnt+littleNormal, m_orientation.inverted());
		const auto&& scn = m_scene.lock();
		Collision col;
		if (!m_castShadows || !(scn->throwRay(ff,col)==1))
		{
			ret = color().gained(lumm);
		}
	}
	
	std::cout << "direct light acummul: " << ret << std::endl;
	
	return ret;
}


Color IBLight::acummulationLight(const Ray &, const Collision &collision) const
{
	Color acumColor;
	// compute all sort of shadows.
	Collision environment;
	const auto n = samples();
	const auto colorRatio = 1.0/(scalar)n;
	for(auto i=0u; i<n; i++)
	{
		const auto&& cPnt = collision.position();
		const auto&& r = MU::getHollowSphereRand(radius());
		const Ray f(cPnt, r);
		auto lumm = m_intensity * lightRatio(f, collision.normal());

		environment.setUV(MU::normalToCartesian(f.direction()));
		auto environmentColor = m_map.compute(environment);
		const auto &luma = environmentColor.lumma();
		
		if (luma>m_lowThreshold && lumm>0.0001)
		{
			const auto &scn = m_scene.lock();
			Collision col;
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

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Ambient Light ////////////////////////
////////////////////////////////////////////////////////////////////////////

AmbientLight::AmbientLight()
{
	
}

v3s AmbientLight::getLightRay(const v3s &position) const
{
	return position.inverted();
}

Color AmbientLight::acummulationLight(const Ray &, const Collision &) const
{
	return m_color*m_intensity;
}

scalar AreaLight::minY() const
{
	return m_minY;
}

void AreaLight::setMinY(const scalar &minY)
{
	m_minY = minY;
}

scalar AreaLight::maxX() const
{
	return m_maxX;
}

void AreaLight::setMaxX(const scalar &maxX)
{
	m_maxX = maxX;
}

scalar AreaLight::maxY() const
{
	return m_maxY;
}

void AreaLight::setMaxY(const scalar &maxY)
{
	m_maxY = maxY;
}

v3s AreaLight::normal() const
{
	return m_normal;
}

void AreaLight::setNormal(const v3s &normal)
{
	m_normal = normal;
}

AreaLight::AreaLight()
	: m_sampler(8)
{}

Color AreaLight::acummulationLight(const Ray &, const Collision &collision) const
{
	Color ret{Color::zero};
	Ray f(collision.position(), collision.normal());
	const auto&& cPnt = collision.position();

	const auto littleNormal = collision.normal().small();
		const auto finalIntensity = m_intensity / (scalar)m_samples;
		 
	for(auto x = 0u; x<m_samples; x++)
	{
		auto u = MU::getRand(m_maxX) + m_minX;
		auto v = MU::getRand(m_maxY) + m_minY;

		const auto&& orientation = m_transform.origin() - cPnt + v3s(u, 0.0 ,v);
		if(collision.normal().follows(orientation.inverted()))
		{
			const Ray ff(cPnt+littleNormal, orientation);
			const auto&& scn = m_scene.lock();
			Collision col;
			if (!(scn->throwRay(ff,col)==1))
			{
			   				auto ratio = lightRatio(f, orientation);
				ret += color().gained(finalIntensity * (ratio * ratio * ratio));
			}
		}
	}
	
	return ret;
	
}

scalar AreaLight::minX() const
{
	return m_minX;
}

void AreaLight::setMinX(const scalar &minX)
{
	m_minX = minX;
}

