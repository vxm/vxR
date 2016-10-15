#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <memory>
#include <atomic>

#include "Process.h"
#include "Status.h"
#include "ImageProperties.h"
#include "SceneParser.h"
#include "Scene.h"
#include "imageData.h"
namespace vxCompute {

using namespace vxCore;

/* This class performs the render.
 * it takes a scene, with at least one light, one camera
 * a geometry and performs the render, storing the results
 * in a vxContactBuffer
 */

class vxRenderProcess : public Process
{
private:
	std::shared_ptr<const ImageProperties> m_properties;
	std::shared_ptr<Scene> m_scene;
	std::atomic_bool m_finished;
	unsigned int m_nThreads;
	
	scalar m_giMultiplier{1.0};
	std::atomic<scalar> m_progress{0.0};
	ImageData m_imageData;
	ContactBuffer m_contactBuffer;
	
	unsigned int m_samples{1};
	scalar m_c_invSamples{1/(scalar)m_samples};
	unsigned int m_reflectionSamples{0};
	unsigned int m_giSamples{0};
public:
	
	// constructor with imageproperties propagation
	vxRenderProcess(ImagePropertiesHandle &prop,
					unsigned int samples);
	
	virtual Status::code preProcess(Process* p=nullptr) override;
	virtual Status::code postProcess(Process* p=nullptr) override;
	virtual Status::code execute() override;
	virtual Status::code preConditions() override;
	
	///
	/// \brief render
	/// \param by
	/// \param offset
	/// \return 
	///
	Status::code render(unsigned int by = 1u, unsigned int offset = 0u);
	
	//void createBucketList(unsigned int samples);
	const unsigned char *generateImage();
	///
	/// \brief setDatabase
	/// \return 
	///
	Status setDatabase(std::shared_ptr<SceneParser>);
	///
	/// \brief setImageProperties
	/// \param imageProperties
	///
	void setImageProperties(std::shared_ptr<const ImageProperties> imageProperties);
	///
	/// \brief imageProperties
	/// \return 
	///
	std::shared_ptr<const ImageProperties> imageProperties() const;
	///
	/// \brief setScene
	/// \param scene
	///
	void setScene(const std::shared_ptr<Scene> &scene);
	///
	/// \brief scene
	/// \return 
	///
	std::shared_ptr<Scene> scene() const;
	///
	/// \brief setNMaxThreads
	/// \param nMaxThreads
	///
	void setNMaxThreads(unsigned int nMaxThreads);
	///
	/// \brief nMaxThreads
	/// \return 
	///
	unsigned int nMaxThreads() const;
	///
	/// \brief progress
	/// \return 
	///
	scalar progress() const;
	///
	/// \brief visSamples
	/// \return 
	///
	unsigned int visSamples() const;
	///
	/// \brief setVisSamples
	/// \param visSamples
	///
	void setVisSamples(unsigned int visSamples);
	///
	/// \brief reflectionSamples
	/// \return 
	///
	unsigned int reflectionSamples() const;
	///
	/// \brief setReflectionSamples
	/// \param reflectionSamples
	///
	void setReflectionSamples(unsigned int reflectionSamples);
	///
	/// \brief giSamples
	/// \return 
	///
	unsigned int giSamples() const;
	///
	/// \brief setGISamples
	/// \param giSamples
	///
	void setGISamples(unsigned int giSamples);
	///
	/// \brief giMultiplier
	/// \return 
	///
	scalar giMultiplier() const;
	///
	/// \brief setGIMultiplier
	/// \param giMultiplier
	///
	void setGIMultiplier(const scalar &giMultiplier);
	///
	/// \brief samples
	/// \return 
	///
	unsigned int samples() const;
	///
	/// \brief setSamples
	/// \param samples
	///
	void setSamples(unsigned int samples);
	///
	/// \brief properties
	/// \return 
	///
	std::shared_ptr<const ImageProperties> properties() const;
	///
	/// \brief setProperties
	/// \param properties
	///
	void setProperties(const std::shared_ptr<const ImageProperties> &properties);
	///
	/// \brief computeLight
	/// \param ray
	/// \param col
	/// \return 
	///
	Color computeLight(const Ray &ray, Collision &col);
	///
	/// \brief computeEnergyAndColor
	/// \param ray
	/// \param col
	/// \return 
	///
	Color computeEnergyAndColor(unsigned int iter, const Ray &ray, Collision &col);
};

#endif // VXRENDERPROCESS_H

}
