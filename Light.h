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
	///
	/// \brief m_transform
	///
	Matrix44 m_transform;
	///
	/// \brief m_castShadows
	///
	bool m_castShadows = true;
	
	//not every light needs a position
	v3s m_position = {0.0,0.0,0.0};
	///
	/// \brief m_intensity
	///
	scalar m_intensity = 1.0;
	///
	/// \brief m_color
	///
	Color m_color = Color::white;
	///
	/// \brief m_radius
	///
	scalar m_radius = 1.0;
	///
	/// \brief m_samples
	///
	unsigned int m_samples = 1u;
	///
	/// \brief m_scene
	///
	std::weak_ptr<Scene> m_scene;
	
public:
	///
	/// \brief Light
	///
	Light();
	///
	/// \brief Light
	/// \param intensity
	/// \param color
	///
	Light(scalar intensity, const Color &color);
	///
	/// \brief Light
	/// \param position
	///
	Light(const v3s &position);
	///
	/// \brief Light
	/// \param x
	/// \param y
	/// \param z
	///
	Light(scalar x, scalar y, scalar z );
	///
	/// \brief ~Light
	///
	virtual ~Light(){}
	///
	/// \brief setScene
	/// \param scene
	///
	void setScene(std::weak_ptr<Scene> scene);
	///
	/// \brief setPosition
	/// \param position
	///
	void setPosition(const v3s &position);
	///
	/// \brief setPosition
	/// \param x
	/// \param y
	/// \param z
	///
	void setPosition(scalar x, scalar y, scalar z);
	///
	/// \brief set
	/// \param intensity
	/// \param color
	///
	void set(scalar intensity, const v3s &color);
	///
	/// \brief setIntensity
	/// \param intensity
	///
	void setIntensity(scalar intensity);
	///
	/// \brief setColor
	/// \param color
	///
	void setColor(const Color &color);
	///
	/// \brief position
	/// \return 
	///
	v3s position() const;
	///
	/// \brief intensity
	/// \return 
	///
	scalar intensity() const {return m_intensity;}
	///
	/// \brief color
	/// \return 
	///
	Color color() const {return m_color;}
	///
	/// \brief getLightRay
	/// \param position
	/// \return 
	///
	virtual v3s getLightRay(const v3s &position) const;
	///
	/// \brief lightRatio
	/// \param ray
	/// \param lightDirection
	/// \return 
	///
	virtual scalar lightRatio(const Ray &ray,
							  const v3s &lightDirection) const;
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return 
	///
	virtual Color acummulationLight(const Ray &, const Collision &collision) const;
	///
	/// \brief radius
	/// \return 
	///
	scalar radius() const;
	///
	/// \brief setRadius
	/// \param radius
	///
	void setRadius(scalar radius);
	///
	/// \brief samples
	/// \return 
	///
	unsigned int samples() const;
	///
	/// \brief setSamples
	/// \param samples
	///
	void setSamples(int samples);
	///
	/// \brief computeShadows
	/// \return 
	///
	bool computeShadows() const;
	///
	/// \brief setComputeShadows
	/// \param computeShadows
	///
	void setComputeShadows(bool computeShadows);
	///
	/// \brief getTransform
	/// \return 
	///
	Matrix44 getTransform() const;
	///
	/// \brief setTransform
	/// \param transform
	///
	void setTransform(const Matrix44 &transform);

	///
	/// \brief reachesLightSource
	/// \return 
	///
	bool reachesLightSource(const Ray &ray) const;

};



class SpotLight final : public Light
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


class PointLight final : public Light
{
protected:
	///
	/// \brief m_orientation
	///
	v3s m_orientation = {0.0,0.0,0.0};
	///
	/// \brief m_biDirectional
	/// 
	bool m_biDirectional;
	
public:
	///
	/// \brief PointLight
	///
	PointLight();
	///
	/// \brief PointLight
	/// \param instensity
	/// \param col
	///
	PointLight(scalar instensity, const Color &col);
	///
	/// \brief PointLight
	/// \param orientation
	/// \param bidirectional
	///
	PointLight(const v3s &orientation,
				 bool bidirectional);
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return 
	///
	Color acummulationLight(const Ray &, const Collision &collision) const override;
	///
	/// \brief set
	/// \param orientation
	/// \param bidirectional
	///
	void set(const v3s &orientation,bool bidirectional);
	///
	/// \brief setOrientation
	/// \param orientation
	///
	void setOrientation (const v3s &orientation) {m_orientation.set(orientation);}
	///
	/// \brief setBidirectional
	/// \param bidirectional
	///
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
};

///
/// \brief The SunLight class
/// simulates the light of the sun
class SunLight final : public Light
{
protected:
	///
	/// \brief m_orientation
	///
	v3s m_orientation = {0.0,0.0,0.0};
	///
	/// \brief m_radius
	///
	scalar m_radius = 695'700'000;
	///
	scalar m_distance = 149'600'000'000;
	///temperature = 5.777K

public:
	///
	/// \brief SunLight
	///
	SunLight();
	///
	/// \brief SunLight
	/// \param instensity
	/// \param col
	///
	SunLight(scalar instensity, const Color &col);
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return 
	///
	Color acummulationLight(const Ray &, const Collision &collision) const override;
	///
	/// \brief set
	/// \param orientation
	/// \param bidirectional
	///
	void set(const v3s &orientation);
	///
	/// \brief setOrientation
	/// \param orientation
	///
	void setOrientation (const v3s &orientation) {m_orientation.set(orientation);}
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

class IBLight final : public Light
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

class AmbientLight final : public Light
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
using SunLightHandle = std::shared_ptr<SunLight>;
using DirectLightHandle = std::shared_ptr<DirectLight>;
using AmbientLightHandle = std::shared_ptr<AmbientLight>;
}
#endif 
