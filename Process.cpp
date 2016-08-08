#include "Process.h"
namespace vxCore {
Process::Process()
{
}

std::string Process::name() const
{
	return m_name;
}

void Process::setName(const std::string &name)
{
	m_name = name;
}






}
