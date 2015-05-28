#ifndef VXSTANDARDSHADER_H
#define VXSTANDARDSHADER_H
#include "vxShader.h"
#include "vxMap.h"

using namespace vxCore;

class vxStandardShader : public vxShader
{
	public:
		vxStandardShader();
		
		std::shared_ptr<vxMap> m_diffueseTexture;
		// vxShader interface
	public:

		virtual vxColor getColor(const vxCollision &collide) const override;

		virtual vxColor getLightLoop(const vxCollision &col) const override;

		bool hasDiffuseMap() const;
};

#endif // VXSTANDARDSHADER_H
