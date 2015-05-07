#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include "vxObject.h"
#include "vxVector.h"
#include<vxCollision.h>

namespace vxStorage {

class vxLight:public vxObject
{
protected:

	 vxVector3d m_position;
	 double m_intensity{1.0};
	 vxColor m_color{1.0,1.0,1.0};

 public:

	vxLight()
	{
	}
	
	vxLight(double intensity, const vxColor &color) {m_intensity=intensity;m_color=color;}
	vxLight(const vxVector3d &position) {m_position.set(position);};
	vxLight(double x, double y, double z ) {m_position.set(x,y,z);};

	void setPosition(vxVector3d position) {m_position.set(position);};
	void setPosition(double x, double y, double z ) {m_position.set(x,y,z);};

	void set(double intensity, vxVector3d color) {m_intensity=intensity;m_color=color;}
	void setIntensity(double intensity) {m_intensity=intensity;}
	void setColor(vxColor color) {m_color=color;}

	double intensity() const {return m_intensity;}
	vxColor getColor() const {return m_color;}

	virtual vxVector3d getLightRay(const vxVector3d &position) const
	{
		return m_position-position;
	}
	
	virtual double luminance(const vxCollision &collide) const = 0; 
 };



 class vxSpotLight:public vxLight
 {
 private:

	 vxVector3d m_orientation;

	 double m_maxAngle;
	 double m_minAngle;


 public:
	 vxSpotLight()
		 :vxLight()
	{
		m_maxAngle=1.3;
		m_minAngle=1;
	};
	vxSpotLight(vxVector3d position,vxVector3d orientation,double maxAngle,double minAngle)
		:vxLight() 
	{
		m_position.set(position);
		m_orientation.set(orientation);
		m_maxAngle=maxAngle;
		m_minAngle=minAngle;
	};

	void set(vxVector3d position,vxVector3d orientation,double maxAngle,double minAngle) 
	{
		m_position.set(position);
		m_orientation.set(orientation);
		m_maxAngle=maxAngle;
		m_minAngle=minAngle;
	};

	void setOrientation(vxVector3d orientation) {m_orientation.set(orientation);}
	void setMin(double maxAngle) {m_maxAngle=maxAngle;}
	void setMax(double minAngle) {m_minAngle=minAngle;}

	double luminance(const vxCollision &collide) const override
	{
		return intensity()*collide.getNormal().angle(getLightRay(collide.getPosition()));
	}
 };


 class vxPointLight:public vxLight
 {

 public:
	 vxPointLight()
		 :vxLight() {};

	 vxPointLight(double instensity, const vxColor &col)
		 :vxLight(instensity, col)
	 {};

	 double luminance(const vxCollision &collide) const override
	 {
		double angl = collide.getNormal().angle(getLightRay(collide.getPosition()));
		
		if(angl>1.57)
			return 0.0;
		
		return m_intensity * (pow(cos(angl),2.0));
	 }
	 
	 // vxLight interface
	 public:
	 vxVector3d getLightRay(const vxVector3d &position) const override;
 };


 class vxDirectLight:public vxLight
 {
 private:
	 vxVector3d m_orientation;
	 bool m_biDirectional;

 public:
	vxDirectLight() {};
	vxDirectLight (vxVector3d orientation,bool bidirectional) 
	{
		m_orientation.set(orientation);
		m_biDirectional=bidirectional;
	};

	void set (vxVector3d orientation,bool bidirectional) 
	{
		m_orientation.set(orientation);
		m_biDirectional=bidirectional;
	};

	void setOrientation (vxVector3d orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}

	double luminance(const vxCollision &collide) const override
	{
		return intensity()*collide.getNormal().angle(getLightRay(collide.getPosition()));
	}
 };

 }
#endif 