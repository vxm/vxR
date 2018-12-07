#ifndef VXLIGHTSMC
#define VXLIGHTSMC

#include "Collision.h"
#include "Image.h"
#include "MathUtils.h"
#include "Matrix44.h"
#include "Sampler.h"
#include "Vector.h"
#include <memory>
#include <visible.h>

namespace vxCore
{

class Scene;

class Light : public Visible
{

protected:
	///
	/// \brief m_intensity
	///
	scalar m_intensity = 1.0;
	///
	/// \brief m_castShadows
	///
	bool m_castShadows = true;

	// not every light needs a position
	v3s m_position = {0.0, 0.0, 0.0};
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
	///
	Light(const scalar intensity);
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
	Light(scalar x, scalar y, scalar z);
	///
	/// \brief ~Light
	///
	virtual ~Light() {}
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
	/// \brief position
	/// \return
	///
	v3s position() const;
	///
	/// \brief intensity
	/// \return
	///
	scalar intensity() const { return m_intensity; }
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
	virtual scalar lightRatio(const Ray &ray, const v3s &lightDirection) const;
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return
	///
	virtual Color acummulationLight(const Ray &,
	                                const Collision &collision) const = 0;
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

	// renderable interface
	virtual bool throwRay(const Ray &) const override { return false; }

	virtual int throwRay(const Ray &, Collision &) const override { return 0; }

	virtual bool hasCollision(const Ray &) const override { return false; }

	// Visible interface
	virtual void updateBoundingBox() override { 	m_bb->clear();
													m_bb->extend( m_transform.origin() + m_position + v3s(1,1,1));
													m_bb->extend( m_transform.origin() + m_position + v3s(-1,-1,-1));
											  }
};

class SpotLight final : public Light
{
private:
	v3s m_orientation;

	scalar m_maxAngle = 1.3;
	scalar m_minAngle = 1;

public:
	///
	/// \brief SpotLight
	///
	SpotLight();
	///
	/// \brief SpotLight
	/// \param position
	/// \param orientation
	/// \param maxAngle
	/// \param minAngle
	///
	SpotLight(const v3s &position, const v3s &orientation, scalar maxAngle,
	          scalar minAngle);
	///
	/// \brief set
	/// \param position
	/// \param orientation
	/// \param maxAngle
	/// \param minAngle
	///
	void set(const v3s &position, const v3s &orientation, scalar maxAngle,
	         scalar minAngle);
	///
	/// \brief setOrientation
	/// \param orientation
	///
	void setOrientation(const v3s &orientation);
	///
	/// \brief setMin
	/// \param maxAngle
	///
	void setMin(scalar maxAngle) { m_maxAngle = maxAngle; }
	///
	/// \brief setMax
	/// \param minAngle
	///
	void setMax(scalar minAngle) { m_minAngle = minAngle; }

	// renderable interface
	virtual bool throwRay(const Ray &) const override { return false; }

	virtual int throwRay(const Ray &, Collision &) const override { return 0; }

	virtual bool hasCollision(const Ray &) const override { return false; }


	// Light interface
public:
	virtual v3s getLightRay(const v3s &) const override;
	virtual scalar lightRatio(const Ray &, const v3s &) const override;
	virtual Color acummulationLight(const Ray &, const Collision &) const override;
};

class PointLight final : public Light
{
protected:
	///
	/// \brief m_orientation
	///
	v3s m_orientation = {0.0, 0.0, 0.0};
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
	/// \param orientation
	/// \param bidirectional
	///
	PointLight(const v3s &orientation, bool bidirectional);
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return
	///
	Color acummulationLight(const Ray &,
	                        const Collision &collision) const override;
	///
	/// \brief set
	/// \param orientation
	/// \param bidirectional
	///
	void set(const v3s &orientation, bool bidirectional);
	///
	/// \brief setOrientation
	/// \param orientation
	///
	void setOrientation(const v3s &orientation)
	{
		m_orientation.set(orientation);
	}
	///
	/// \brief setBidirectional
	/// \param bidirectional
	///
	void setBidirectional(bool bidirectional) { m_biDirectional = bidirectional; }

	// renderable interface
	virtual bool throwRay(const Ray &ray) const override;

	virtual int throwRay(const Ray &, Collision &col) const override;

	virtual bool hasCollision(const Ray &ray) const override;

	// Visible interface
};

class SphereLight final : public Light
{
protected:
	///
	/// \brief m_orientation
	///
	v3s m_orientation = {0.0, 0.0, 0.0};
	///
	/// \brief m_biDirectional
	///
	bool m_biDirectional = false;

public:
	///
	/// \brief SphereLight
	///
	SphereLight();
	///
	/// \brief SphereLight
	/// \param orientation
	/// \param bidirectional
	///
	SphereLight(const v3s &orientation, bool bidirectional);
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return
	///
	Color acummulationLight(const Ray &,
	                        const Collision &collision) const override;
	///
	/// \brief set
	/// \param orientation
	/// \param bidirectional
	///
	void set(const v3s &orientation, bool bidirectional);
	///
	/// \brief setOrientation
	/// \param orientation
	///
	void setOrientation(const v3s &orientation)
	{
		m_orientation.set(orientation);
	}
	///
	/// \brief setBidirectional
	/// \param bidirectional
	///
	void setBidirectional(bool bidirectional) { m_biDirectional = bidirectional; }

	// renderable interface
	virtual bool throwRay(const Ray &ray) const override;

	virtual int throwRay(const Ray &ray, Collision &col) const override;

	virtual bool hasCollision(const Ray &ray) const override;

	// Visible interface
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
	v3s m_orientation = {-0.5, 1.0, -0.6};
	///
	/// \brief m_radius
	///
	scalar m_sunRadius = 695'700'000; //(695.700.000)
	///
	scalar m_distance = 149'600'000'000;
	/// temperature = 5.777K

public:
	///
	/// \brief SunLight
	///
	SunLight();
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return
	///
	Color acummulationLight(const Ray &,
	                        const Collision &collision) const override;
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
	void setOrientation(const v3s &orientation);

	// Visible interface

	// renderable interface
	virtual bool throwRay(const Ray &) const override { return false; }

	virtual int throwRay(const Ray &, Collision &) const override;

	virtual bool hasCollision(const Ray &) const override { return false; }

	// Visible interface
	v3s orientation() const;
};

class DirectLight final : public Light
{
protected:
	v3s m_orientation;
	bool m_biDirectional;

public:
	///
	/// \brief DirectLight
	///
	DirectLight();
	///
	/// \brief DirectLight
	/// \param orientation
	/// \param bidirectional
	///
	DirectLight(const v3s &orientation, bool bidirectional);
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return
	///
	Color acummulationLight(const Ray &,
	                        const Collision &collision) const override;
	///
	/// \brief set
	/// \param orientation
	/// \param bidirectional
	///
	void set(const v3s &orientation, bool bidirectional);
	///
	/// \brief setOrientation
	/// \param orientation
	///
	void setOrientation(const v3s &orientation)
	{
		m_orientation.set(orientation);
	}
	///
	/// \brief setBidirectional
	/// \param bidirectional
	///
	void setBidirectional(bool bidirectional) { m_biDirectional = bidirectional; }

	// Visible interface

	// renderable interface
	virtual bool throwRay(const Ray &) const override { return false; }

	virtual int throwRay(const Ray &, Collision &) const override { return 0; }

	virtual bool hasCollision(const Ray &) const override { return false; }

	// Visible interface
};

class IBLight final : public Light
{
	///
	/// \brief m_map
	///
	Image m_map;
	///
	/// \brief m_gain
	///
	scalar m_gain = 0.5;
	///
	/// \brief m_gamma
	///
	scalar m_gamma = 2.0;
	///
	/// \brief m_lowThreshold
	///
	scalar m_lowThreshold = 0.1;

public:
	///
	/// \brief IBLight
	///
	IBLight();
	///
	/// \brief IBLight
	/// \param instensity
	/// \param path
	///
	IBLight(scalar instensity, const std::string &path);

	// vxLight interface
	v3s getLightRay(const v3s &position) const override;
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return
	///
	Color acummulationLight(const Ray &,
	                        const Collision &collision) const override;
	///
	/// \brief gain
	/// \return
	///
	scalar gain() const;
	///
	/// \brief setGain
	/// \param gain
	///
	void setGain(scalar gain);
	///
	/// \brief gamma
	/// \return
	///
	scalar gamma() const;
	///
	/// \brief setGamma
	/// \param gamma
	///
	void setGamma(scalar gamma);
	///
	/// \brief lowThreshold
	/// \return
	///
	scalar lowThreshold() const;
	///
	/// \brief setLowThreshold
	/// \param lowThreshold
	///
	void setLowThreshold(scalar lowThreshold);

	// Visible interface

	// renderable interface
	virtual bool throwRay(const Ray &) const override { return false; }

	virtual int throwRay(const Ray &, Collision &) const override { return 0; }

	virtual bool hasCollision(const Ray &) const override { return false; }

	// Visible interface
};

class AmbientLight final : public Light
{
public:
	///
	/// \brief AmbientLight
	///
	AmbientLight() = default;
	///
	/// \brief getLightRay
	/// \param position
	/// \return
	///
	v3s getLightRay(const v3s &position) const override;
	///
	/// \brief acummulationLight
	/// \return
	///
	Color acummulationLight(const Ray &, const Collision &) const override;

	// renderable interface
	virtual bool throwRay(const Ray &) const override { return false; }

	virtual int throwRay(const Ray &, Collision &) const override { return 0; }

	virtual bool hasCollision(const Ray &) const override { return false; }

	// Visible interface
};

class AreaLight final : public Light
{
protected:
	scalar m_minX = -1.25;
	scalar m_minY = -1.25;
	scalar m_maxX = 1.25;
	scalar m_maxY = 1.25;

	v3s m_normal = {0.0, -1.0, 0.0};

	Sampler m_sampler;

public:
	///
	/// \brief AreaLight
	///
	AreaLight();
	///
	/// \brief acummulationLight
	/// \param collision
	/// \return
	///
	Color acummulationLight(const Ray &,
	                        const Collision &collision) const override;
	///
	/// \brief minX
	/// \return
	///
	scalar minX() const;
	///
	/// \brief setMinX
	/// \param minX
	///
	void setMinX(const scalar &minX);
	///
	/// \brief minY
	/// \return
	///
	scalar minY() const;
	///
	/// \brief setMinY
	/// \param minY
	///
	void setMinY(const scalar &minY);
	///
	/// \brief maxX
	/// \return
	///
	scalar maxX() const;
	///
	/// \brief setMaxX
	/// \param maxX
	///
	void setMaxX(const scalar &maxX);
	///
	/// \brief maxY
	/// \return
	///
	scalar maxY() const;
	///
	/// \brief setMaxY
	/// \param maxY
	///
	void setMaxY(const scalar &maxY);
	///
	/// \brief normal
	/// \return
	///
	v3s normal() const;
	///
	/// \brief setNormal
	/// \param normal
	///
	void setNormal(const v3s &normal);
};

using AreaLightHandle = std::shared_ptr<AreaLight>;
using IBLightHandle = std::shared_ptr<IBLight>;
using PointLightHandle = std::shared_ptr<PointLight>;
using SphereLightHandle = std::shared_ptr<SphereLight>;
using SunLightHandle = std::shared_ptr<SunLight>;
using DirectLightHandle = std::shared_ptr<DirectLight>;
using AmbientLightHandle = std::shared_ptr<AmbientLight>;

using LightHandle = std::shared_ptr<Light>;
}
#endif
