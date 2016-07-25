#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include <memory>
#include "vxVector.h"
#include "vxCollision.h"
#include "MathUtils.h"
#include "vxImage.h"
#include "Matrix44.h"
#include "vxSampler.h"

namespace vxCore {

class vxScene;

class vxLight
{
protected:
	
	Matrix44 m_transform;
	bool m_castShadows{true};
	//not every light needs a position
	v3s m_position	{0.0,0.0,0.0};
	scalar m_intensity		{1.0};
	vxColor m_color			{vxColor::white};
	scalar m_radius			{1.0};
	unsigned int m_samples	{1u};
	std::weak_ptr<vxScene>	m_scene;
	
public:
	
	vxLight();
	vxLight(scalar intensity, const vxColor &color);
	vxLight(const v3s &position);
	vxLight(scalar x, scalar y, scalar z );
	
	void setScene(std::weak_ptr<vxScene> scene);
	void setPosition(const v3s &position);
	void setPosition(scalar x, scalar y, scalar z);
	
	void set(scalar intensity, const v3s &color);
	void setIntensity(scalar intensity);
	void setColor(const vxColor &color);
	///
	/// \brief position
	/// \return 
	///
	v3s position() const;

	scalar intensity() const {return m_intensity;}
	vxColor color() const {return m_color;}
	
	virtual v3s getLightRay(const v3s &position) const;
	virtual scalar lightRatio(const vxRay &ray,
							  const v3s &lightDirection) const;
	
	virtual vxColor acummulationLight(const vxRay &, const vxCollision &collision) const;
	
	scalar radius() const;
	void setRadius(scalar radius);
	
	unsigned int samples() const;
	void setSamples(int samples);
	bool computeShadows() const;
	void setComputeShadows(bool computeShadows);
	
	Matrix44 getTransform() const;
	void setTransform(const Matrix44 &transform);
};



class vxSpotLight:public vxLight
{
private:
	v3s m_orientation;
	
	scalar m_maxAngle=1.3;
	scalar m_minAngle=1;
	
public:
	vxSpotLight();
	vxSpotLight(const v3s &position,const v3s &orientation,scalar maxAngle,scalar minAngle);
	
	void set(const v3s &position,const v3s &orientation,scalar maxAngle,scalar minAngle);
	
	void setOrientation(const v3s &orientation);
	void setMin(scalar maxAngle) {m_maxAngle=maxAngle;}
	void setMax(scalar minAngle) {m_minAngle=minAngle;}
	
};


class vxPointLight:public vxLight
{
protected:
	v3s m_orientation;
	bool m_biDirectional;
	
public:
	vxPointLight();
	vxPointLight(scalar instensity, const vxColor &col);
	
	vxPointLight(const v3s &orientation,
				 bool bidirectional);
	
	vxColor acummulationLight(const vxRay &, const vxCollision &collision) const override;
	
	void set(const v3s &orientation,bool bidirectional);
	void setOrientation (const v3s &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
};


class vxDirectLight : public vxLight
{
protected:
	v3s m_orientation;
	bool m_biDirectional;
	
public:
	vxDirectLight();
	vxDirectLight(scalar instensity, const vxColor &col);
	
	vxDirectLight(const v3s &orientation,
				  bool bidirectional);
	
	vxColor acummulationLight(const vxRay &, const vxCollision &collision) const override;
	
	void set(const v3s &orientation,bool bidirectional);
	void setOrientation (const v3s &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
	
};

class vxIBLight:public vxLight
{
	vxImage m_map;
	scalar m_gain{0.5};
	scalar m_gamma{2.0};
	scalar m_lowThreshold{0.1};
	
public:
	
	vxIBLight();
	vxIBLight(scalar instensity, const vxColor &col);
	vxIBLight(scalar instensity, const std::string path);
	
	// vxLight interface
	v3s getLightRay(const v3s &position) const override;
	vxColor acummulationLight(const vxRay &, 
							  const vxCollision &collision) const override;
	
	scalar gain() const;
	void setGain(scalar gain);
	scalar gamma() const;
	void setGamma(scalar gamma);
	scalar lowThreshold() const;
	void setLowThreshold(scalar lowThreshold);
};

class vxAmbientLight:public vxLight
{
public:
	vxAmbientLight();
	vxAmbientLight(scalar intensity, const vxColor &color);
	
	// vxLight interface
public:
	v3s getLightRay(const v3s &position) const override;
	vxColor acummulationLight(const vxRay &, const vxCollision &) const override;
};


class vxAreaLight : public vxLight
{
protected:

	scalar m_minX{-1.25};
	scalar m_minY{-1.25};
	scalar m_maxX{ 1.25};
	scalar m_maxY{ 1.25};
	
	v3s m_normal{0.0, -1.0, 0.0};
	
	vxSampler m_sampler;
	
public:
	vxAreaLight();
	
	vxColor acummulationLight(const vxRay &, const vxCollision &collision) const override;
	
	
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

using vxAreaLightHandle = std::shared_ptr<vxAreaLight>;
using vxIBLightHandle = std::shared_ptr<vxIBLight>;
using vxPointLightHandle = std::shared_ptr<vxPointLight>;
using vxDirectLightHandle = std::shared_ptr<vxDirectLight>;
using vxAmbientLightHandle = std::shared_ptr<vxAmbientLight>;
}
#endif 
