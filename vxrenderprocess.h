#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <vxprocess.h>
#include <vxstatus.h>
#include <vxpxbuffer.h>
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

	std::unique_ptr <unsigned char[]> m_pc = nullptr;
	std::unique_ptr <double[]> m_pd = nullptr;
	std::unique_ptr <float[]> m_pf = nullptr;
	

	
public:

	// defines the size in bits for each channel.
	enum class ImgFormat {k8, k16, k32, k64};
	// enumerates the channels to store.
	enum class ImgChannels {kRGB, kRGBA, kRGBZ, kRGBAZ};
	
	vxRenderProcess();
	
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;
	
	const unsigned char *createPixelBuffer( int width,
										int heigth, 
										const ImgFormat f = ImgFormat::k8,
										const ImgChannels c = ImgChannels::kRGBA);
};

#endif // VXRENDERPROCESS_H

}
