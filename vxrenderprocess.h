#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <vxprocess.h>
#include <vxstatus.h>
#include <vximage.h>
#include <memory>

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

	std::unique_ptr <vxPxBuffer> m_pb = nullptr;
	std::unique_ptr <const double*> m_pd = nullptr;
	std::unique_ptr <const double*> m_pf = nullptr;
	
public:

	// defines the size in bits for each channel.
	enum class ImgFormat {k8, k16, k32, k64};
	enum class ImgChannels {kRGB, kRGBA, kRGBZ, kRGBAZ};
	
	vxRenderProcess();
	
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;
	
	const char *getPixelBuffer( int width,
								int heigth, 
								const ImgFormat f = ImgFormat::k8,
								const ImgChannels c = ImgChannels::kRGBA);
};

#endif // VXRENDERPROCESS_H

}
