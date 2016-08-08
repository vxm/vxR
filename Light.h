#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include <memory>
#include "Vector.h"
#include "Collision.h"
#include "MathUtils.h"
#include "Image.h"
#include "Matrix44.h"
#include "Sampler.h"

namespace vxCore {

class Scene;

class Light
{
protected:
	
	Matrix44 m_transform;
	bool m_castShadows{true};
	//not every light needs a position
	v3s m_position	{0.0,0.0,0.0};
	scalar m_intensity		{1.0};
	Color m_color			{Color::white};
	scalar m_radius			{1.0};
	unsigned int m_samples	{1u};
	std::weak_ptr<Scene>	m_scene;
	
public:
	
	Light();
	Light(scalar intensity, const Color &color);
	Light(const v3s &position);
	Light(scalar x, scalar y, scalar z );
	
	virtual ~Light(){}
	
	void setScene(std::weak_ptr<Scene> scene);
	void setPosition(const v3s &position);
	void setPosition(scalar x, scalar y, scalar z);
	
	void set(scalar intensity, const v3s &color);
	void setIntensity(scalar intensity);
	void setColor(const Color &color);
	///
	/// \brief position
	/// \return 
	///
	v3s position() const;

	scalar intensity() const {return m_intensity;}
	Color color() const {return m_color;}
	
	virtual v3s getLightRay(const v3s &position) const;
	virtual scalar lightRatio(const Ray &ray,
							  const v3s &lightDirection) const;
	
	virtual Color acummulationLight(const Ray &, const Collision &collision) const;
	
	scalar radius() const;
	void setRadius(scalar radius);
	
	unsigned int samples() const;
	void setSamples(int samples);
	bool computeShadows() const;
	void setComputeShadows(bool computeShadows);
	
	Matrix44 getTransform() const;
	void setTransform(const Matrix44 &transform);
};



class SpotLight:public Light
{
private:
	v3s m_orientation;
	
	scalar m_maxAngle=1.3;
	scalar m_minAngle=1;
	
public:
	SpotLight();
	SpotLight(const v3s &position,const v3s &orientation,scalar maxAngle,scalar minAngle);
	
	void set(const v3s &position,const v3s &orientation,scalar maxAngle,scalar minAngle);
	
	void setOrientation(const v3s &orientation);
	void setMin(scalar maxAngle) {m_maxAngle=maxAngle;}
	void setMax(scalar minAngle) {m_minAngle=minAngle;}
	
};


class PointLight final:public Light
{
protected:
	v3s m_orientation;
	bool m_biDirectional;
	
public:
	PointLight();
	PointLight(scalar instensity, const Color &col);
	
	PointLight(const v3s &orientation,
				 bool bidirectional);
	
	Color acummulationLight(const Ray &, const Collision &collision) const override;
	
	void set(const v3s &orientation,bool bidirectional);
	void setOrientation (const v3s &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
};


class DirectLight final: public Light
{
protected:
	v3s m_orientation;
	bool m_biDirectional;
	
public:
	DirectLight();
	DirectLight(scalar instensity, const Color &col);
	
	DirectLight(const v3s &orientation,
				  bool bidirectional);
	
	Color acummulationLight(const Ray &, const Collision &collision) const override;
	
	void set(const v3s &orientation,bool bidirectional);
	void setOrientation (const v3s &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
	
};

class IBLight final:public Light
{
	Image m_map;
	scalar m_gain{0.5};
	scalar m_gamma{2.0};
	scalar m_lowThreshold{0.1};
	
public:
	
	IBLight();
	IBLight(scalar instensity, const Color &col);
	IBLight(scalar instensity, const std::string path);
	
	// vxLight interface
	v3s getLightRay(const v3s &position) const override;
	Color acummulationLight(const Ray &, 
							  const Collision &collision) const override;
	
	scalar gain() const;
	void setGain(scalar gain);
	scalar gamma() const;
	void setGamma(scalar gamma);
	scalar lowThreshold() const;
	void setLowThreshold(scalar lowThreshold);
};

class AmbientLight final:public Light
{
public:
	AmbientLight();
	AmbientLight(scalar intensity, const Color &color);
	
	// vxLight interface
public:
	v3s getLightRay(const v3s &position) const override;
	Color acummulationLight(const Ray &, const Collision &) const override;
};


class AreaLight final: public Light
{
protected:

	scalar m_minX{-1.25};
	scalar m_minY{-1.25};
	scalar m_maxX{ 1.25};
	scalar m_maxY{ 1.25};
	
	v3s m_normal{0.0, -1.0, 0.0};
	
	Sampler m_sampler;
	
public:
	AreaLight();
	
	Color acummulationLight(const Ray &, const Collision &collision) const override;
	
	
	scalar minX() const;
	void setMinX(const scalar &minX);

	scalar minY() const;
	void setMinY(const scalar &minY);

	scalar maxX() const;
	void setMaxX(const scalar &maxX);

	scalar maxY() const;
	void setMaxY(const scalar &maxY);
	v3s normal() const;
	void setNormal(const v3s &normal);
};

using AreaLightHandle = std::shared_ptr<AreaLight>;
using IBLightHandle = std::shared_ptr<IBLight>;
using PointLightHandle = std::shared_ptr<PointLight>;
using DirectLightHandle = std::shared_ptr<DirectLight>;
using AmbientLightHandle = std::shared_ptr<AmbientLight>;
}
#endif 
