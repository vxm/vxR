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
	double m_intensity		{1.0};
	vxColor m_color			{vxColor::white};
	double m_radius			{1.0};
	unsigned int m_samples	{1u};
	std::weak_ptr<vxScene>	m_scene;

public:

	vxLight();
	vxLight(double intensity, const vxColor &color);
	vxLight(const vxVector3d &position);
	vxLight(double x, double y, double z );

	void setScene(std::weak_ptr<vxScene> scene);
	void setPosition(const vxVector3d &position);
	void setPosition(double x, double y, double z);

	void set(double intensity, const vxVector3d &color);
	void setIntensity(double intensity);
	void setColor(const vxColor &color);

	vxVector3d position() const 
	{
		return m_position;
	}
	
	double intensity() const {return m_intensity;}
	vxColor color() const {return m_color;}

	virtual vxVector3d getLightRay(const vxVector3d &position) const;
	virtual double lightRatio(const vxRay &ray,
					const vxVector3d &lightDirection) const;

	virtual vxColor acummulationLight(const vxRay &, const vxCollision &collision) const;

	double radius() const;
	void setRadius(double radius);

	unsigned int samples() const;
	void setSamples(int samples);
};



class vxSpotLight:public vxLight
{
	private:
		vxVector3d m_orientation;

	double m_maxAngle=1.3;
	double m_minAngle=1;

 public:
	vxSpotLight();
	vxSpotLight(const vxVector3d &position,const vxVector3d &orientation,double maxAngle,double minAngle);

	void set(const vxVector3d &position,const vxVector3d &orientation,double maxAngle,double minAngle);

	void setOrientation(const vxVector3d &orientation);
	void setMin(double maxAngle) {m_maxAngle=maxAngle;}
	void setMax(double minAngle) {m_minAngle=minAngle;}

 };


 class vxPointLight:public vxLight
 {

 public:
	vxPointLight();
	vxPointLight(double instensity, const vxColor &col);

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
	vxDirectLight(double instensity, const vxColor &col);
	
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
	double m_gain{0.5};
	double m_gamma{2.0};
	double m_lowThreshold{0.1};
	
public:

	vxIBLight();
	vxIBLight(double instensity, const vxColor &col);
	vxIBLight(double instensity, const std::string path);
	
	// vxLight interface
	vxVector3d getLightRay(const vxVector3d &position) const override;
	vxColor acummulationLight(const vxRay &, 
							  const vxCollision &collision) const override;

	double gain() const;
	void setGain(double gain);
	double gamma() const;
	void setGamma(double gamma);
	double lowThreshold() const;
	void setLowThreshold(double lowThreshold);
};

class vxAmbientLight:public vxLight
{
public:
	vxAmbientLight();
	vxAmbientLight(double intensity, const vxColor &color);
	
	// vxLight interface
	public:
	vxVector3d getLightRay(const vxVector3d &position) const override;
	vxColor acummulationLight(const vxRay &, const vxCollision &) const override;
};
 
}
#endif 
