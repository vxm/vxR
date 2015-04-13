#include "vxProcess.h"
namespace vxStorage {
std::string vxProcess::name() const
{
	return m_name;
}

void vxProcess::setName(const std::string &name)
{
	m_name = name;
}






}