#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <memory>

#include <vxProcess.h>
#include <vxStatus.h>
#include <vxBucketList.h>
#include <ImageProperties.h>

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

	std::unique_ptr <unsigned char[]> m_pc = nullptr;
	std::unique_ptr <double[]> m_pd = nullptr;
	std::unique_ptr <float[]> m_pf = nullptr;
	std::unique_ptr <vxBucketList> m_bList = nullptr;
	
	std::shared_ptr<const ImageProperties> m_imageProperties;

public:

	vxRenderProcess();
	vxRenderProcess(std::shared_ptr<ImageProperties> &prop)
		:	m_imageProperties(prop)
	{}
	
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;
	
	void createBucketList();
	const unsigned char *generateImage();
	
	std::shared_ptr<const ImageProperties> imageProperties() const;
	void setImageProperties(std::shared_ptr<const ImageProperties> imageProperties);
};

#endif // VXRENDERPROCESS_H

}
