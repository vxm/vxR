#include "vxLight.h"
namespace vxStorage {

vxVector3d vxPointLight::getLightRay(const vxVector3d &position) const
{
	return vxLight::getLightRay(position);
}




}
