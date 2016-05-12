#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "stdlib.h"
#include "time.h"
#include <memory>
#include <vector>

#include "vxVector.h"
#include "vxCollision.h"
#include "vxLight.h"
#include "vxCirclesMap.h"
#include "vxScene.h"

namespace vxCore {

class vxLight;
class vxScene;

class vxShader
{
	using lightsRef = std::shared_ptr<std::vector<vxLight>>;
	
protected:
	///
	/// \brief m_lights
	///
	std::vector<std::shared_ptr<vxLight>> *m_lights = nullptr;
	///
	/// \brief m_scene
	///
	std::weak_ptr<vxScene> m_scene;
	
	vxColor m_diffuseColor{1.0, 1.0, 1.0, 1.0};
	scalar m_diffuseCoeficent = 1.0;
	
	unsigned int m_giSamples = 1;
	scalar m_giCoeficent = 1.0;
	vxColor m_giColorMultiplier{1.0, 1.0, 1.0, 1.0};
	
	unsigned int m_reflectionSamples = 0;
	scalar m_reflectionRadius = 1.0;
	scalar m_reflectionCoefficent = 1.0;
	vxColor m_reflectionColorMultiplier{1.0, 1.0, 1.0, 1.0};
	
	unsigned int m_refractionSamples = 0;
	scalar m_refractionRadius = 1.0;
	scalar m_refractionCoefficent = 1.0;
	vxColor m_refractionColorMultiplier{1.0, 1.0, 1.0, 1.0};
	
	unsigned int m_sscSamples = 0;
	scalar m_sscRadius = 1.0;
	scalar m_sscCoefficent = 0.5;
	vxColor m_sscColorMultiplier{1.0, 1.0, 1.0, 1.0};
	
	///
	/// \brief m_diffuse
	///
	vxCirclesMap m_diffuse;
	
public:
	
	///
	/// \brief vxShader
	///
	vxShader();
	virtual vxColor getColor(const vxRay &ray, const vxCollision &collide) const = 0;
	///
	/// \brief getIlluminatedColor
	/// \param ray
	/// \param collide
	/// \return 
	///
	virtual vxColor getIlluminatedColor(const vxRay &ray, const vxCollision &collide) const = 0;
	///
	/// \brief getVector
	/// \param collide
	/// \return 
	///
	virtual v3s getVector(const vxCollision &collide) const = 0;
	///
	/// \brief getLightLoop
	/// \param ray
	/// \param collision
	/// \return 
	///
	virtual vxColor getLightLoop(const vxRay &ray, const vxCollision &collision) const;
	///
	/// \brief setScene
	/// \param scene
	///
	void setScene(std::weak_ptr<vxScene> scene);
	///
	/// \brief light
	/// \return 
	///
	std::shared_ptr<vxLight> light() const;
	///
	/// \brief setLights
	/// \param lights
	///
	void setLights(std::vector<std::shared_ptr<vxLight>> *lights);
	///
	/// \brief getDiffuseColor
	/// \return 
	///
	vxColor getDiffuseColor() const;
	///
	/// \brief setDiffuseColor
	/// \param diffuseColor
	///
	void setDiffuseColor(const vxColor &diffuseColor);
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
	/// \brief getGiSamples
	/// \return 
	///
	unsigned int getGiSamples() const;
	///
	/// \brief setGiSamples
	/// \param giSamples
	///
	void setGiSamples(unsigned int giSamples);
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
	vxColor getGiColorMultiplier() const;
	///
	/// \brief setGiColorMultiplier
	/// \param giColorMultiplier
	///
	void setGiColorMultiplier(const vxColor &giColorMultiplier);
	///
	/// \brief getReflectionSamples
	/// \return 
	///
	unsigned int getReflectionSamples() const;
	///
	/// \brief setReflectionSamples
	/// \param reflectionSamples
	///
	void setReflectionSamples(unsigned int reflectionSamples);
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
	vxColor getReflectionColorMultiplier() const;
	///
	/// \brief setReflectionColorMultiplier
	/// \param reflectionColorMultiplier
	///
	void setReflectionColorMultiplier(const vxColor &reflectionColorMultiplier);
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
	vxColor getRefractionColorMultiplier() const;
	///
	/// \brief setRefractionColorMultiplier
	/// \param refractionColorMultiplier
	///
	void setRefractionColorMultiplier(const vxColor &refractionColorMultiplier);
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
	vxColor getSscColorMultiplier() const;
	///
	/// \brief setSscColorMultiplier
	/// \param sscColorMultiplier
	///
	void setSscColorMultiplier(const vxColor &sscColorMultiplier);
};

class vxLambert final : public vxShader
{
protected:
	
	
public:
	vxLambert();
	
	///
	/// \brief getColor
	/// \param collide
	/// \return 
	///
	virtual vxColor getColor(const vxRay &, const vxCollision &collide) const override;
	///
	/// \brief getVector
	/// \param collide
	/// \return 
	///
	virtual v3s getVector(const vxCollision &collide) const override;
	///
	/// \brief getIlluminatedColor
	/// \param ray
	/// \param collide
	/// \return 
	///
	virtual vxColor getIlluminatedColor(const vxRay &ray, const vxCollision &collide) const override;
};

}
#endif
