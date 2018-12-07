#ifndef VXSHADERMC
#define VXSHADERMC

#include "stdlib.h"
#include "time.h"
#include <memory>
#include <vector>

#include "CirclesMap.h"
#include "Collision.h"
#include "Light.h"
#include "Vector.h"

namespace vxCore
{

class Light;
class Scene;

using lightsRef = std::shared_ptr<std::vector<Light>>;

class Shader
{

protected:
	///
	/// \brief m_lights
	///
	std::vector<std::shared_ptr<Light>> *m_lights = nullptr;
	///
	/// \brief m_scene
	///
	std::weak_ptr<Scene> m_scene;

	Color m_diffuseColor{1.0, 1.0, 1.0, 1.0};
	scalar m_diffuseCoeficent = 1.0;

	scalar m_giCoeficent = 1.0;
	Color m_giColorMultiplier{1.0, 1.0, 1.0, 1.0};

	unsigned int m_rayDepth = 0;
	scalar m_reflectionRadius = 1.0;
	scalar m_reflectionCoefficent = 0.0;
	Color m_reflectionColorMultiplier{1.0, 1.0, 1.0, 1.0};

	unsigned int m_refractionSamples = 0;
	scalar m_refractionRadius = 1.0;
	scalar m_refractionCoefficent = 1.0;
	Color m_refractionColorMultiplier{1.0, 1.0, 1.0, 1.0};

	unsigned int m_sscSamples = 0;
	scalar m_sscRadius = 1.0;
	scalar m_sscCoefficent = 0.5;
	Color m_sscColorMultiplier{1.0, 1.0, 1.0, 1.0};

	bool m_hasGI = true;
	bool m_hasReflection = true;
	///
	/// \brief m_diffuse
	///
	CirclesMap m_diffuse;

public:
	///
	/// \brief vxShader
	///
	Shader();
	virtual Color getColor(const Ray &ray, const Collision &collide) const = 0;
	///
	/// \brief getIlluminatedColor
	/// \param ray
	/// \param collide
	/// \return
	///
	virtual Color getIlluminatedColor(const Ray &ray,
	                                  const Collision &collide) const = 0;
	///
	/// \brief getVector
	/// \param collide
	/// \return
	///
	virtual v3s getVector(const Collision &collide) const = 0;
	///
	/// \brief getLightLoop
	/// \param ray
	/// \param collision
	/// \return
	///
	virtual Color getLightLoop(const Ray &ray, const Collision &collision) const;
	///
	/// \brief setScene
	/// \param scene
	///
	void setScene(std::weak_ptr<Scene> scene);
	///
	/// \brief light
	/// \return
	///
	std::shared_ptr<Light> light() const;
	///
	/// \brief setLights
	/// \param lights
	///
	void setLights(std::vector<std::shared_ptr<Light>> *lights);
	///
	/// \brief getDiffuseColor
	/// \return
	///
	Color getDiffuseColor() const;
	///
	/// \brief setDiffuseColor
	/// \param diffuseColor
	///
	void setDiffuseColor(const Color &diffuseColor);
	///
	/// \brief getDiffuseCoeficent
	/// \return
	///
	scalar getDiffuseCoeficent() const;
	///
	/// \brief setDiffuseCoeficent
	/// \param diffuseCoeficent
	///
	void setDiffuseCoeficent(const scalar &diffuseCoeficent);
	///
	/// \brief getGiCoeficent
	/// \return
	///
	scalar getGiCoeficent() const;
	///
	/// \brief setGiCoeficent
	/// \param giCoeficent
	///
	void setGiCoeficent(const scalar &giCoeficent);
	///
	/// \brief getGiColorMultiplier
	/// \return
	///
	Color getGiColorMultiplier() const;
	///
	/// \brief setGiColorMultiplier
	/// \param giColorMultiplier
	///
	void setGiColorMultiplier(const Color &giColorMultiplier);
	///
	/// \brief getrayDepth
	/// \return
	///
	unsigned int getRayDepth() const;
	///
	/// \brief setRayDepth
	/// \param rayDepth
	///
	void setRayDepth(unsigned int rayDepth);
	///
	/// \brief getReflectionRadius
	/// \return
	///
	scalar getReflectionRadius() const;
	///
	/// \brief setReflectionRadius
	/// \param reflectionRadius
	///
	void setReflectionRadius(const scalar &reflectionRadius);
	///
	/// \brief getReflectionCoefficent
	/// \return
	///
	scalar getReflectionCoefficent() const;
	///
	/// \brief setReflectionCoefficent
	/// \param reflectionCoefficent
	///
	void setReflectionCoefficent(const scalar &reflectionCoefficent);
	///
	/// \brief getReflectionColorMultiplier
	/// \return
	///
	Color getReflectionColorMultiplier() const;
	///
	/// \brief setReflectionColorMultiplier
	/// \param reflectionColorMultiplier
	///
	void setReflectionColorMultiplier(const Color &reflectionColorMultiplier);
	///
	/// \brief getRefractionSamples
	/// \return
	///
	unsigned int getRefractionSamples() const;
	///
	/// \brief setRefractionSamples
	/// \param refractionSamples
	///
	void setRefractionSamples(unsigned int refractionSamples);
	///
	/// \brief getRefractionRadius
	/// \return
	///
	scalar getRefractionRadius() const;
	///
	/// \brief setRefractionRadius
	/// \param refractionRadius
	///
	void setRefractionRadius(const scalar &refractionRadius);
	///
	/// \brief getRefractionCoefficent
	/// \return
	///
	scalar getRefractionCoefficent() const;
	///
	/// \brief setRefractionCoefficent
	/// \param refractionCoefficent
	///
	void setRefractionCoefficent(const scalar &refractionCoefficent);
	///
	/// \brief getRefractionColorMultiplier
	/// \return
	///
	Color getRefractionColorMultiplier() const;
	///
	/// \brief setRefractionColorMultiplier
	/// \param refractionColorMultiplier
	///
	void setRefractionColorMultiplier(const Color &refractionColorMultiplier);
	///
	/// \brief getSscSamples
	/// \return
	///
	unsigned int getSscSamples() const;
	///
	/// \brief setSscSamples
	/// \param sscSamples
	///
	void setSscSamples(unsigned int sscSamples);
	///
	/// \brief getSscRadius
	/// \return
	///
	scalar getSscRadius() const;
	///
	/// \brief setSscRadius
	/// \param sscRadius
	///
	void setSscRadius(const scalar &sscRadius);
	///
	/// \brief getSscCoefficent
	/// \return
	///
	scalar getSscCoefficent() const;
	///
	/// \brief setSscCoefficent
	/// \param sscCoefficent
	///
	void setSscCoefficent(const scalar &sscCoefficent);
	///
	/// \brief getSscColorMultiplier
	/// \return
	///
	Color getSscColorMultiplier() const;
	///
	/// \brief setSscColorMultiplier
	/// \param sscColorMultiplier
	///
	void setSscColorMultiplier(const Color &sscColorMultiplier);
	///
	/// \brief hasReflection
	/// \return
	///
	bool hasReflection() const;
	///
	/// \brief setHasReflection
	/// \param hasReflection
	///
	void setHasReflection(bool hasReflection);
	///
	/// \brief hasGI
	/// \return
	///
	bool hasGI() const;
	///
	/// \brief setHasGI
	/// \param hasGI
	///
	void setHasGI(bool hasGI);
};

class Lambert final : public Shader
{
protected:
public:
	Lambert();

	///
	/// \brief getColor
	/// \param collide
	/// \return
	///
	virtual Color getColor(const Ray &, const Collision &collide) const override;
	///
	/// \brief getVector
	/// \param collide
	/// \return
	///
	virtual v3s getVector(const Collision &collide) const override;
	///
	/// \brief getIlluminatedColor
	/// \param ray
	/// \param collide
	/// \return
	///
	virtual Color getIlluminatedColor(const Ray &ray,
	                                  const Collision &collide) const override;
};

using vxShaderHandle = std::shared_ptr<Shader>;
}
#endif
