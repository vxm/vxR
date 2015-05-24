#ifndef VXIMPORTER_H
#define VXIMPORTER_H

#include <vector>
#include "vxVector.h"

namespace vxCore {


class vxImporter
{
public:
	vxImporter();
	
	virtual std::vector<vxVector3d>& getPointCloud() = 0;
};

#endif // VXIMPORTER_H
}
