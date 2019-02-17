#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "stdlib.h"
#include "time.h"
#include <memory>
#include <vector>

#include "CirclesMap.h"
#include "Light.h"
#include "Shader.h"
#include "Vector.h"

namespace vxCore
{

class Light;
class Scene;

class LightShader : public Shader
{
public:
	LightShader();

	// Shader interface
public:
	virtual Color getColor(const Ray &ray, const Collision &col) const override;
	virtual Color getLumminance(const Ray &, const Collision &col) const override;
	virtual v3s getVector(const Collision &) const override;
};
} // namespace vxCore
#endif // LIGHTSHADER_H
