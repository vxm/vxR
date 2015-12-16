#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include <memory>
#include "vxVector.h"
#include "vxCollision.h"
#include "MathUtils.h"
#include "vxScene.h"
#include "vxBitMap2d.h"
#include "vxMatrix.h"

namespace vxCore {

class vxScene;

class vxLight
{
protected:
	
	vxMatrix m_transform;
	bool m_castShadows{true};
	//not every light needs a position
	v3 m_position	{0.0,0.0,0.0};
	scalar m_intensity		{1.0};
	vxColor m_color			{vxColor::white};
	scalar m_radius			{1.0};
	unsigned int m_samples	{1u};
	std::weak_ptr<vxScene>	m_scene;
	
public:
	
	vxLight();
	vxLight(scalar intensity, const vxColor &color);
	vxLight(const v3 &position);
	vxLight(scalar x, scalar y, scalar z );
	
	void setScene(std::weak_ptr<vxScene> scene);
	void setPosition(const v3 &position);
	void setPosition(scalar x, scalar y, scalar z);
	
	void set(scalar intensity, const v3 &color);
	void setIntensity(scalar intensity);
	void setColor(const vxColor &color);
	
	v3 position() const 
	{
		return m_position;
	}
	
	scalar intensity() const {return m_intensity;}
	vxColor color() const {return m_color;}
	
	virtual v3 getLightRay(const v3 &position) const;
	virtual scalar lightRatio(const vxRay &ray,
							  const v3 &lightDirection) const;
	
	virtual vxColor acummulationLight(const vxRay &, const vxCollision &collision) const;
	
	scalar radius() const;
	void setRadius(scalar radius);
	
	unsigned int samples() const;
	void setSamples(int samples);
	bool computeShadows() const;
	void setComputeShadows(bool computeShadows);
	
	vxMatrix getTransform() const;
	void setTransform(const vxMatrix &transform);
};



class vxSpotLight:public vxLight
{
private:
	v3 m_orientation;
	
	scalar m_maxAngle=1.3;
	scalar m_minAngle=1;
	
public:
	vxSpotLight();
	vxSpotLight(const v3 &position,const v3 &orientation,scalar maxAngle,scalar minAngle);
	
	void set(const v3 &position,const v3 &orientation,scalar maxAngle,scalar minAngle);
	
	void setOrientation(const v3 &orientation);
	void setMin(scalar maxAngle) {m_maxAngle=maxAngle;}
	void setMax(scalar minAngle) {m_minAngle=minAngle;}
	
};


class vxPointLight:public vxLight
{
protected:
	v3 m_orientation;
	bool m_biDirectional;
	
public:
	vxPointLight();
	vxPointLight(scalar instensity, const vxColor &col);
	
	vxPointLight(const v3 &orientation,
				 bool bidirectional);
	
	vxColor acummulationLight(const vxRay &, const vxCollision &collision) const override;
	
	void set(const v3 &orientation,bool bidirectional);
	void setOrientation (const v3 &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
};


class vxDirectLight : public vxLight
{
protected:
	v3 m_orientation;
	bool m_biDirectional;
	
public:
	vxDirectLight();
	vxDirectLight(scalar instensity, const vxColor &col);
	
	vxDirectLight(const v3 &orientation,
				  bool bidirectional);
	
	vxColor acummulationLight(const vxRay &, const vxCollision &collision) const override;
	
	void set(const v3 &orientation,bool bidirectional);
	void setOrientation (const v3 &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
	
};

class vxIBLight:public vxLight
{
	vxBitMap2d m_map;
	scalar m_gain{0.5};
	scalar m_gamma{2.0};
	scalar m_lowThreshold{0.1};
	
public:
	
	vxIBLight();
	vxIBLight(scalar instensity, const vxColor &col);
	vxIBLight(scalar instensity, const std::string path);
	
	// vxLight interface
	v3 getLightRay(const v3 &position) const override;
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
	v3 getLightRay(const v3 &position) const override;
	vxColor acummulationLight(const vxRay &, const vxCollision &) const override;
};


class vxAreaLight : public vxLight
{
protected:

	scalar m_minX{-1.25};
	scalar m_minY{-1.25};
	scalar m_maxX{ 1.25};
	scalar m_maxY{ 1.25};
	
	v3 m_normal;
	
public:
	vxAreaLight()
	{}
	
	vxColor acummulationLight(const vxRay &, const vxCollision &collision) const override;
	
	
	scalar minX() const;
	void setMinX(const scalar &minX);

	scalar minY() const;
	void setMinY(const scalar &minY);

	scalar maxX() const;
	void setMaxX(const scalar &maxX);

	scalar maxY() const;
	void setMaxY(const scalar &maxY);
};


}
#endif 
