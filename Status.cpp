#include "Status.h"
namespace vxCore {


Status::Status()
{
}

void Status::examine(std::function<bool ()> f, const std::string &msg)
{
	if(!f())
	{
		std::cout << msg << " failure." << std::endl;
		m_internalStatus = code::kError;
	}
}

bool Status::isSuccess()
{
	return m_internalStatus == code::kSuccess;
}


}
