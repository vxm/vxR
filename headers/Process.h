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
	///
	/// \brief Process
	///
	Process();
	///
	/// \brief ~Process
	///
	virtual ~Process() {}
	
	std::string name() const;
	void setName(const std::string &name);
};

}
#endif // VXPROCESS_H
