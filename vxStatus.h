#ifndef VXSTATUS_H
#define VXSTATUS_H

namespace vxStorage {


class vxStatus
{
	public:
		vxStatus();

	enum class code{ kError, kUnknown, kSuccess};
};

}
#endif // VXSTATUS_H