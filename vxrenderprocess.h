#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <vxprocess.h>
#include <vxstatus.h>

class vxRenderProcess : public vxProcess
{
	public:
		vxRenderProcess();
		
		
	vxStatus::code execute() override;
	
	vxStatus::code preConditions() override
	{
		
		return vxStatus::code::success;
	}
	
};

#endif // VXRENDERPROCESS_H
