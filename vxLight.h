#ifndef _VXLIGHTSMC_
#define _VXLIGHTSMC_

#include "vxObject.h"
#include "vxVector.h"
#include<vxCollision.h>

namespace vxCore {

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

	void setPosition(const vxVector3d &position) {m_position.set(position);};
	void setPosition(double x, double y, double z ) {m_position.set(x,y,z);};

	void set(double intensity, vxVector3d color) {m_intensity=intensity;m_color=color;}
	void setIntensity(double intensity) {m_intensity=intensity;}
	void setColor(vxColor color) {m_color=color;}

	vxVector3d position() const {return m_position;}
	double intensity() const {return m_intensity;}
	vxColor color() const {return m_color;}

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


 class vxDirectLight:public vxLight
 {
 private:
	 vxVector3d m_orientation;
	 bool m_biDirectional;

 public:
	vxDirectLight();
	vxDirectLight (const vxVector3d &orientation,bool bidirectional);

	void set(const vxVector3d &orientation,bool bidirectional);

	void setOrientation (const vxVector3d &orientation) {m_orientation.set(orientation);}
	void setBidirectional (bool bidirectional) {m_biDirectional=bidirectional;}

	double luminance(const vxCollision &collide) const override;
 };

 }
#endif 