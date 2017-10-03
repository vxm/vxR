#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "stdlib.h"
#include "time.h"
#include <memory>
#include <vector>

#include "Vector.h"
#include "Shader.h"
#include "Light.h"
#include "CirclesMap.h"

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
	virtual Color getColor(const Ray &ray,
	                       const Collision &) const override;
	virtual Color getIlluminatedColor(const Ray &,
	                                  const Collision &) const override;
	virtual v3s getVector(const Collision &) const override;
};
}
#endif // LIGHTSHADER_H
