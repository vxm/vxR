#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <memory>
#include <atomic>

#include "vxProcess.h"
#include "vxStatus.h"
#include "ImageProperties.h"
#include "vxSceneParser.h"
#include "vxScene.h"
#include "imageData.h"
namespace vxCompute {

using namespace vxCore;

/* This class performs the render.
 * it takes a scene, with at least one light, one camera
 * a geometry and performs the render, storing the results
 * in a vxContactBuffer
 */

class vxRenderProcess : public vxProcess
{
private:
	std::shared_ptr<const ImageProperties> m_prop;
	std::shared_ptr<vxScene> m_scene;
	std::atomic_bool m_finished;
	unsigned int m_nThreads;
	
	scalar m_giMultiplier{1.0};
	std::atomic<scalar> m_progress{0.0};
	ImageData m_imageData;
	vxContactBuffer m_contactBuffer;
	
	unsigned int m_visSamples{1};
	scalar m_c_invSamples{1/(scalar)m_visSamples};
	unsigned int m_reflectionSamples{0};
	unsigned int m_giSamples{0};
public:

	// constructor with imageproperties propagation
	vxRenderProcess(std::shared_ptr<ImageProperties> &prop,
					unsigned int samples);
	
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;

	vxStatus::code render(unsigned int by = 1u, unsigned int offset = 0u);

	//void createBucketList(unsigned int samples);
	const unsigned char *generateImage();

	vxStatus setDatabase(std::shared_ptr<vxSceneParser> scn);
	void setImageProperties(std::shared_ptr<const ImageProperties> imageProperties);
	std::shared_ptr<const ImageProperties> imageProperties() const;

	void setScene(const std::shared_ptr<vxScene> &scene);
	std::shared_ptr<vxScene> scene() const;
	
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
};

#endif // VXRENDERPROCESS_H

}
