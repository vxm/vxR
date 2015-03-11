#include "vxLight.h"
namespace vxStorage {


double vxPointLight::luminance(vxCollision &collide)	
{
	//vxVector3d lightRay = getLightRay(collide.getPosition());
	
	double angl = collide.getNormal().angle(getLightRay(collide.getPosition()));

	return m_intensity * (cos(angl));
}

}
