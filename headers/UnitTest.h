#ifndef VXUNITTEST_H
#define VXUNITTEST_H

#include "Status.h"

//TODO: should I use another namespace?
namespace vxCore {

class UnitTest
{
	bool m_verbose{false};
public:
	UnitTest() = default;
	bool verbose();
	void setVerbose(bool verbose);
};

}
#endif // VXUNITTEST_H
