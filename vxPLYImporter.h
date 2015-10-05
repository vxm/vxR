#ifndef VXPLYIMPORTER_H
#define VXPLYIMPORTER_H

#include <fstream>

#include "vxGeometryImporter.h"
#include "vxGeometry.h"
#include "FileUtils.h"
#include "StringUtils.h"

namespace vxCore {


class vxPLYImporter : public vxGeometryImporter
{
	std::shared_ptr<vxGeometry> m_geo;

public:

	vxPLYImporter(std::shared_ptr<vxGeometry> geo);

	void processPLYFile(const std::string &fileName);
	
	std::shared_ptr<vxGeometry> getGeometry() const;
	void setGeo(const std::shared_ptr<vxGeometry> &geo);
};

}
#endif // VXPLYIMPORTER_H
