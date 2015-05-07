#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <memory>

#include "vxProcess.h"
#include "vxStatus.h"
#include "vxBucketList.h"
#include "ImageProperties.h"
#include "vxScene.h"

namespace vxCompute {

using namespace vxStorage;

/* This class performs the render.
 * it takes a scene, with at least one light, one camera
 * a geometry and performs the render, storing the results
 * in a vxContactBuffer
 */


class vxRenderProcess : public vxProcess
{
private:

	std::unique_ptr <unsigned char[]>	m_pc = nullptr;
	std::unique_ptr <double[]>			m_pd = nullptr;
	std::unique_ptr <float[]>			m_pf = nullptr;
	vxBucketList						m_bucketList;

	std::shared_ptr<const ImageProperties> m_imageProperties;
	std::shared_ptr<vxScene> m_scene;

public:

	// constructor with imageproperties propagation
	vxRenderProcess(std::shared_ptr<ImageProperties> &prop)
		:	m_bucketList(prop, 10)
		,	m_imageProperties(prop)
	{
		m_scene.reset(new vxScene(prop));
	}
	
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;

	void createBucketList();
	const unsigned char *generateImage();

	std::shared_ptr<const ImageProperties> imageProperties() const
	{
		return m_imageProperties;
	}

	void setImageProperties(std::shared_ptr<const ImageProperties> imageProperties)
	{
		m_imageProperties = imageProperties;
	}
	std::shared_ptr<vxScene> scene() const
	{
		return m_scene;
	}
	void setScene(const std::shared_ptr<vxScene> &scene)
	{
		m_scene = scene;
	}
};

#endif // VXRENDERPROCESS_H

}
