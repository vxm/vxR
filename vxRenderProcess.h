#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <memory>
#include <atomic>

#include "vxProcess.h"
#include "vxStatus.h"
#include "vxBucketList.h"
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

	vxBucketList						m_bucketList;

	std::shared_ptr<const ImageProperties> m_prop;
	std::shared_ptr<vxScene> m_scene;
	std::atomic_bool m_finished;
	unsigned int m_nThreads;
	
	std::atomic<double> m_progress;
	ImageData m_imageData;

public:

	// constructor with imageproperties propagation
	vxRenderProcess(std::shared_ptr<ImageProperties> &prop);
	
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;

	vxStatus::code render(unsigned int by = 1u, unsigned int offset = 0u);

	void createBucketList();
	const unsigned char *generateImage();

	vxStatus setDatabase(std::shared_ptr<vxSceneParser> scn);
	void setImageProperties(std::shared_ptr<const ImageProperties> imageProperties);
	std::shared_ptr<const ImageProperties> imageProperties() const;

	void setScene(const std::shared_ptr<vxScene> &scene);
	std::shared_ptr<vxScene> scene() const;
	
	void setNMaxThreads(unsigned int nMaxThreads);
	unsigned int nMaxThreads() const;

	double progress() const;
};

#endif // VXRENDERPROCESS_H

}
