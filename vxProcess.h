#ifndef VXPROCESS_H
#define VXPROCESS_H

#include <string>
#include "vxStatus.h"
#include "vxStopwatch.h"

namespace vxCore 
{

class vxProcess
{
	vxStopwatch m_wh;
	std::string m_name;
	
	public:
	vxProcess()
	{
	}

	// performs any preprocess of this task
	virtual vxStatus::code preProcess(vxProcess* p=nullptr) = 0;
	// performs any postprocess of this task
	virtual vxStatus::code postProcess(vxProcess* p=nullptr) = 0;
	virtual vxStatus::code execute() = 0;
	virtual vxStatus::code preConditions() = 0;
	std::string name() const;
	void setName(const std::string &name);
};

}
#endif // VXPROCESS_H
