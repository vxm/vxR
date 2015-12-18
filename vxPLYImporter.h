#ifndef VXPLYIMPORTER_H
#define VXPLYIMPORTER_H

#include <fstream>

#include "vxGeometryImporter.h"
#include "vxTriangleMesh.h"
#include "FileUtils.h"
#include "StringUtils.h"

namespace vxCore {


class vxPLYImporter : public vxGeometryImporter
{
	vxTriangleMeshHandle m_geo;

public:

	vxPLYImporter(vxTriangleMeshHandle geo);

	void processPLYFile(const std::string &fileName);
	
	vxTriangleMeshHandle getGeometry() const;
	void setGeo(const vxTriangleMeshHandle &geo);
};

}
#endif // VXPLYIMPORTER_H
