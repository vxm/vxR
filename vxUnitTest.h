#ifndef VXUNITTEST_H
#define VXUNITTEST_H

#include "vxStatus.h"

//TODO: should I use another namespace?
namespace vxCore {

class vxUnitTest
{
	bool m_verbose{false};
public:
	vxUnitTest();
	bool verbose();
	void setVerbose(bool verbose);
};

}
#endif // VXUNITTEST_H
