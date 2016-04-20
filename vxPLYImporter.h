#ifndef VXPLYIMPORTER_H
#define VXPLYIMPORTER_H

#include <fstream>

#include "vxGeometryImporter.h"
#include "vxTriangleMesh.h"
#include "FileUtils.h"
#include "StringUtils.h"

namespace vxCore {

///
/// \brief The vxPLYImporter class
///This class reads a PLY file, builds and returns if queried 
/// a geometry which he owns if now passed in constructor.
class vxPLYImporter : public vxGeometryImporter
{
	vxTriangleMeshHandle m_geo;

public:
	///
	/// \brief vxPLYImporter
	/// \param geo
	///
	vxPLYImporter(vxTriangleMeshHandle geo);
	
	///
	/// \brief processPLYFile
	/// \param fileName
	///
	void processPLYFile(const std::string &fileName);
	///
	/// \brief getGeometry
	/// \return 
	///
	vxTriangleMeshHandle getGeometry() const;
	///
	/// \brief setGeometry
	/// \param geo
	///
	void setGeometry(const vxTriangleMeshHandle &geo);
};

}
#endif // VXPLYIMPORTER_H
