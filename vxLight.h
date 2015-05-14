#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include "vxObject.h"
#include "vxVector.h"
#include "vxCollision.h"
#include "MathUtils.h"

namespace vxCore {


class vxLight:public vxObject
{
protected:

		//not every light needs position
	vxVector3d m_position;
	double m_intensity	{1.0};
	vxColor m_color	{1.0,1.0,1.0};
	double m_radius		{0.0};
	int m_samples		{1};
	 
public:

	vxLight();
	
	vxLight(double intensity, const vxColor &color);
	
	vxLight(const vxVector3d &position);
	vxLight(double x, double y, double z );

	void setPosition(const vxVector3d &position);
	void setPosition(double x, double y, double z);

	void set(double intensity, vxVector3d color);
	void setIntensity(double intensity);
	void setColor(vxColor color);

	vxVector3d position() const {return m_position;}
	double intensity() const {return m_intensity;}
	vxColor color() const {return m_color;}

	virtual vxVector3d getLightRay(const vxVector3d &position) const;
	virtual double luminance(const vxCollision &collide) const = 0; 
	virtual double ratio(const vxCollision &collide) const;

	virtual double acumLight(const vxCollision &collide) const
	{	
		const auto& n = samples();
		// compute shadows.
		for(auto i=0; i<n; i++)
		{
			auto r = MathUtils::getHollowSphereRand(2);
			const vxRayXYZ f = (position() + r) - cPnt;
			auto lumm = l->luminance(collision);
			if (lumm>0.001 && !m_scene->hasCollision(cPnt, f))
			{
				acumLumm += fabs(lumm/n);
			}
		}
	}
	
	double radius() const;
	void setRadius(double radius);
	
	int samples() const;
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

	double luminance(const vxCollision &collide) const override;
 };


 class vxPointLight:public vxLight
 {

 public:
	vxPointLight();
	vxPointLight(double instensity, const vxColor &col);

	double luminance(const vxCollision &collide) const override;

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
	
	void set(const vxVector3d &orientation,bool bidirectional);
	
	void setOrientation (const vxVector3d &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}
	
	double luminance(const vxCollision &collide) const override;
};

class vxIBLight:public vxLight
{

public:
	vxIBLight();
	vxIBLight(double instensity, const vxColor &col);
	
	double luminance(const vxCollision &collide) const override;
	
	// vxLight interface
	public:
	vxVector3d getLightRay(const vxVector3d &position) const override;

	
};

 
}
#endif 