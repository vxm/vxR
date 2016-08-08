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
	
	Status::code render(unsigned int by = 1u, unsigned int offset = 0u);
	
	//void createBucketList(unsigned int samples);
	const unsigned char *generateImage();
	
	Status setDatabase(std::shared_ptr<SceneParser>);
	void setImageProperties(std::shared_ptr<const ImageProperties> imageProperties);
	std::shared_ptr<const ImageProperties> imageProperties() const;
	
	void setScene(const std::shared_ptr<Scene> &scene);
	std::shared_ptr<Scene> scene() const;
	
	void setNMaxThreads(unsigned int nMaxThreads);
	unsigned int nMaxThreads() const;
	
	scalar progress() const;
	unsigned int visSamples() const;
	void setVisSamples(unsigned int visSamples);
	unsigned int reflectionSamples() const;
	void setReflectionSamples(unsigned int reflectionSamples);
	unsigned int giSamples() const;
	void setGISamples(unsigned int giSamples);
	scalar giMultiplier() const;
	void setGIMultiplier(const scalar &giMultiplier);
	unsigned int samples() const;
	void setSamples(unsigned int samples);
	std::shared_ptr<const ImageProperties> properties() const;
	void setProperties(const std::shared_ptr<const ImageProperties> &properties);
	Color computeLight(const Ray &ray, Collision &col);
};

#endif // VXRENDERPROCESS_H

}
