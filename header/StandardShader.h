#ifndef VXSTANDARDSHADER_H
#define VXSTANDARDSHADER_H
#include "Shader.h"
#include "Map.h"

namespace vxCore
{

class StandardShader : public Shader
{
	public:
		StandardShader() = default;
		
		std::shared_ptr<Map> m_diffueseTexture;
		// vxShader interface
	public:

		virtual Color getColor(const Ray &, const Collision &collide) const override;
		virtual v3s getVector(const Collision &collide) const override;
		virtual Color getLightLoop(const Ray &, const Collision &collision) const override;

		bool hasDiffuseMap() const;
};

}
#endif // VXSTANDARDSHADER_H
