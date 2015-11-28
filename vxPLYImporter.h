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
	vxGeometryHandle m_geo;

public:

	vxPLYImporter(vxGeometryHandle geo);

	void processPLYFile(const std::string &fileName);
	
	vxGeometryHandle getGeometry() const;
	void setGeo(const vxGeometryHandle &geo);
};

}
#endif // VXPLYIMPORTER_H
