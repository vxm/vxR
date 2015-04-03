#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include "vxObject.h"
#include "vxVector.h"

namespace vxStorage {

class vxLight:public vxObject
{
protected:

	 vxVector3d m_position;
	 double m_intensity;
	 vxColor m_color;

 public:

	vxLight() 
	{
		m_intensity=1;
		m_color.set(255,255,255);
	}

	vxLight(double intensity, vxVector3d color) {this->m_intensity=intensity;this->m_color=color;}
	vxLight(vxVector3d position) {this->m_position.set(position);};
	vxLight(double x, double y, double z ) {this->m_position.set(x,y,z);};

	void setPosition(vxVector3d position) {this->m_position.set(position);};
	void setPosition(double x, double y, double z ) {m_position.set(x,y,z);};

	void set(double intensity, vxVector3d color) {this->m_intensity=intensity;this->m_color=color;}
	void setIntensity(double intensity) {this->m_intensity=intensity;}
	void setColor(vxColor color) {this->m_color=color;}

	double getIntensity() {return this->m_intensity;}
	vxColor getColor() {return this->m_color;}

	virtual vxVector3d getLightRay(vxVector3d position)
	{
		return this->m_position-position;
	}
	
	virtual double luminance(vxCollision &collide) = 0; 
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
		this->m_position.set(position);
		this->m_orientation.set(orientation);
		this->m_maxAngle=maxAngle;
		this->m_minAngle=minAngle;
	};

	void set(vxVector3d position,vxVector3d orientation,double maxAngle,double minAngle) 
	{
		this->m_position.set(position);
		this->m_orientation.set(orientation);
		this->m_maxAngle=maxAngle;
		this->m_minAngle=minAngle;
	};

	void setOrientation(vxVector3d orientation) {this->m_orientation.set(orientation);}
	void setMin(double maxAngle) {this->m_maxAngle=maxAngle;}
	void setMax(double minAngle) {this->m_minAngle=minAngle;}

	double luminance(vxCollision &collide) 
	{
		return this->getIntensity()*collide.getNormal().angle(getLightRay(collide.getPosition()));
	}
 };


 class vxPointLight:public vxLight
 {

 public:
	 vxPointLight()
		 :vxLight() {};

 
	 double luminance(vxCollision &collide);
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
		this->m_orientation.set(orientation);
		this->m_biDirectional=bidirectional;
	};

	void set (vxVector3d orientation,bool bidirectional) 
	{
		this->m_orientation.set(orientation);
		this->m_biDirectional=bidirectional;
	};

	void setOrientation (vxVector3d orientation) {this->m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {this->m_biDirectional=bidirectional;}

	double luminance(vxCollision &collide) 
	{
		return this->getIntensity()*collide.getNormal().angle(getLightRay(collide.getPosition()));
	}
 };

 }
#endif 