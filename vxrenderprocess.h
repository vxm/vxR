#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#import <vxprocess.h>
 
class vxRenderProcess : public vxProcess
{
	public:
		vxRenderProcess();
		
		
	vxStatus::code execute() override
	{
		
		return vxStatus::code::success;
	}
	
	vxStatus::code preConditions() override
	{
		
		return vxStatus::code::success;
	}
};

#endif // VXRENDERPROCESS_H
