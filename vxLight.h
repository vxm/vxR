#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include <memory>
#include "vxVector.h"
#include "vxCollision.h"
#include "MathUtils.h"
#include "vxScene.h"
#include "vxBitMap2d.h"

namespace vxCore {

class vxScene;

class vxLight
{
protected:

	//not every light needs a position
	vxVector3d m_position	{0.0,0.0,0.0};
	scalar m_intensity		{1.0};
	vxColor m_color			{vxColor::white};
	scalar m_radius			{1.0};
	unsigned int m_samples	{1u};
	std::weak_ptr<vxScene>	m_scene;

public:

	vxLight();
	vxLight(scalar intensity, const vxColor &color);
	vxLight(const vxVector3d &position);
	vxLight(scalar x, scalar y, scalar z );

	void setScene(std::weak_ptr<vxScene> scene);
	void setPosition(const vxVector3d &position);
	void setPosition(scalar x, scalar y, scalar z);

	void set(scalar intensity, const vxVector3d &color);
	void setIntensity(scalar intensity);
	void setColor(const vxColor &color);

	vxVector3d position() const 
	{
		return m_position;
	}
	
	scalar intensity() const {return m_intensity;}
	vxColor color() const {return m_color;}

	virtual vxVector3d getLightRay(const vxVector3d &position) const;
	virtual scalar lightRatio(const vxRay &ray,
					const vxVector3d &lightDirection) const;

	virtual vxColor acummulationLight(const vxRay &, const vxCollision &collision) const;

	scalar radius() const;
	void setRadius(scalar radius);

	unsigned int samples() const;
	void setSamples(int samples);
};



class vxSpotLight:public vxLight
{
	private:
		vxVector3d m_orientation;

	scalar m_maxAngle=1.3;
	scalar m_minAngle=1;

 public:
	vxSpotLight();
	vxSpotLight(const vxVector3d &position,const vxVector3d &orientation,scalar maxAngle,scalar minAngle);

	void set(const vxVector3d &position,const vxVector3d &orientation,scalar maxAngle,scalar minAngle);

	void setOrientation(const vxVector3d &orientation);
	void setMin(scalar maxAngle) {m_maxAngle=maxAngle;}
	void setMax(scalar minAngle) {m_minAngle=minAngle;}

 };


 class vxPointLight:public vxLight
 {

 public:
	vxPointLight();
	vxPointLight(scalar instensity, const vxColor &col);

	// vxLight interface
	public:
	vxVector3d getLightRay(const vxVector3d &position) const override;
 };


class vxDirectLight : public vxLight
{
protected:
	vxVector3d m_orientation;
	bool m_biDirectional;

public:
	vxDirectLight();
	vxDirectLight(scalar instensity, const vxColor &col);
	
	vxDirectLight(const vxVector3d &orientation,
				  bool bidirectional);
	
	vxColor acummulationLight(const vxRay &, const vxCollision &collision) const override;
	
	void set(const vxVector3d &orientation,bool bidirectional);
	void setOrientation (const vxVector3d &orientation) {m_orientation.set(orientation);}
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
	vxVector3d getLightRay(const vxVector3d &position) const override;
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
	vxVector3d getLightRay(const vxVector3d &position) const override;
	vxColor acummulationLight(const vxRay &, const vxCollision &) const override;
};
 
}
#endif 
