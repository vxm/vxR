#ifndef VXPROCESS_H
#define VXPROCESS_H

#include <vxstatus.h>

class vxProcess
{
	public:
		vxProcess();

	virtual vxStatus::code execute() = 0;
	virtual vxStatus::code preConditions() = 0;
};

#endif // VXPROCESS_H
