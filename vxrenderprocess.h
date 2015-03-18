#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <memory>

#include <vxprocess.h>
#include <vxstatus.h>
#include <vxbucketlist.h>
#include <imageproperties.h>

namespace vxCompute {

using namespace vxStorage;

/* This class performs the render.
 * it takes a scene, with at least one light, one camera
 * a geometry and performs the render, storing the results
 * in a vxPxBuffer
 */


class vxRenderProcess : public vxProcess
{
private:

	std::unique_ptr <unsigned char[]> m_pc = nullptr;
	std::unique_ptr <double[]> m_pd = nullptr;
	std::unique_ptr <float[]> m_pf = nullptr;
	std::unique_ptr <vxBucketList> m_bList = nullptr;
	
	ImageProperties m_imageProperties;

public:
	
	vxRenderProcess();
	
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;
	
	const unsigned char *createBucketList();

	ImageProperties imageProperties() const;
	void setImageProperties(const ImageProperties &imageProperties);
};

#endif // VXRENDERPROCESS_H

}
