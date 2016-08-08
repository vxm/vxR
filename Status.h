#ifndef VXSTATUS_H
#define VXSTATUS_H
#include <functional>
#include <iostream>

namespace vxCore {

class Status
{
	public:
		Status();

	enum class code{ kError, kUnknown, kSuccess};

	code m_internalStatus{code::kSuccess};
	
	void examine(std::function<bool()> f, const std::string &msg);
	
	bool isSuccess();
};

}
#endif // VXSTATUS_H
