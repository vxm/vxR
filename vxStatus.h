#ifndef VXSTATUS_H
#define VXSTATUS_H
#include <functional>
#include <iostream>

namespace vxCore {

class vxStatus
{
	public:
		vxStatus();

	enum class code{ kError, kUnknown, kSuccess};

	code m_internalStatus{code::kSuccess};
	
	void examine(std::function<bool()> f, const std::string &msg)
	{
		if(!f())
		{
			std::cout << msg << " failure." << std::endl;
			m_internalStatus = code::kError;
		}
	}
	
	bool isSuccess()
	{
		return m_internalStatus == code::kSuccess;
	}
};

}
#endif // VXSTATUS_H
