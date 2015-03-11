#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <vxprocess.h>
#include <vxstatus.h>
#include <vximage.h>
#include <memory>

using namespace vxStorage;
namespace vxCompute {


/* This class performs the render.
 * it takes a scene, with at least one light, one camera
 * a geometry and performs the render, storing the results
 * in a vxPxBuffer
 */

class vxRenderProcess : public vxProcess
{
private:

	std::unique_ptr <vxPxBuffer> m_pb;

public:
	vxRenderProcess();
	
	// vxProcess interface
	vxStatus::code preProcess(vxProcess* p=nullptr) override;
	vxStatus::code postProcess(vxProcess* p=nullptr) override;
	vxStatus::code execute() override;
	vxStatus::code preConditions() override;
};

#endif // VXRENDERPROCESS_H

}
