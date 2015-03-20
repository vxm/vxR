#ifndef VXPROCESS_H
#define VXPROCESS_H

#include <vxStatus.h>

namespace vxStorage 
{

class vxProcess
{
	public:
		vxProcess();

	// performs any preprocess of this task
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) = 0;
	// performs any postprocess of this task
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) = 0;
	virtual vxStatus::code execute() = 0;
	virtual vxStatus::code preConditions() = 0;
};

}
#endif // VXPROCESS_H
