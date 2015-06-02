#ifndef VXPLYIMPORTER_H
#define VXPLYIMPORTER_H

#include <fstream>

#include "vximporter.h"
#include "FileUtils.h"

namespace vxCore {


class vxPLYImporter : public vxImporter
{
public:
	vxPLYImporter();
	
	std::vector<vxVector3d> m_pointCloud;

	void processPLYFile(const std::string &fileName);
	
	virtual std::vector<vxVector3d>& getPointCloud() override;
};

}
#endif // VXPLYIMPORTER_H
