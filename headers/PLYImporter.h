#ifndef VXPLYIMPORTER_H
#define VXPLYIMPORTER_H

#include <fstream>

#include "GeometryImporter.h"
#include "TriangleMesh.h"
#include "FileUtils.h"
#include "StringUtils.h"

namespace vxCore {

///
/// \brief The vertexProperty struct
/// Defines a descriptor for vertex data.
using vertexProperty = std::pair<std::string, std::string>;

///
/// \brief The vxPLYImporter class
///This class reads a PLY file, builds and returns if queried 
/// a geometry which he owns if now passed in constructor.
class PLYImporter : public GeometryImporter
{
	vxTriangleMeshHandle m_geo;

public:
	///
	/// \brief vxPLYImporter
	/// \param geo
	///
	PLYImporter(vxTriangleMeshHandle geo);
	
	///
	/// \brief processPLYFile
	/// \param fileName
	///
	bool processPLYFile(const std::string &fileName);
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
