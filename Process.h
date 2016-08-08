#ifndef VXPROCESS_H
#define VXPROCESS_H

#include <string>
#include "Status.h"
#include "Stopwatch.h"

namespace vxCore 
{

class Process
{
	Stopwatch m_wh;
	std::string m_name;
	
	public:
	Process();

	// performs any preprocess of this task
	virtual Status::code preProcess(Process* p=nullptr) = 0;
	// performs any postprocess of this task
	virtual Status::code postProcess(Process* p=nullptr) = 0;
	virtual Status::code execute() = 0;
	virtual Status::code preConditions() = 0;
	std::string name() const;
	void setName(const std::string &name);
};

}
#endif // VXPROCESS_H
