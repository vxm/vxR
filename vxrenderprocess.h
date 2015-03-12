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

	std::unique_ptr <vxPxBuffer> m_pb;

public:
	vxRenderProcess();
	
	// performs any preprocess of this task
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) override;
	// performs any postprocess of this task
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) override;
	virtual vxStatus::code execute() override;
	virtual vxStatus::code preConditions() override;
	
	

};

#endif // VXRENDERPROCESS_H

}
