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

	void processFile(const std::string &fileName)
	{
		if (!FileUtils::fileExists(fileName))
		{
			std::cout << "PLY: File " << fileName << " doesn't exist" << std::endl;
			return;
		}
		m_pointCloud.clear();
		
		std::ifstream iFile(fileName);
		std::string line;
		unsigned int k {0};

		std::getline(iFile, line);
		if(line!="ply")
		{
			std::cout << "PLY: File " << fileName << " doesn't contain ply info" << std::endl;
			return;
		}
		
		// checking if ASCII
		std::getline(iFile, line);
		if(line!="format ascii 1.0")
		{
			std::cout << "PLY: File " << fileName << " is not ASCII" << std::endl;
			return;
		}
		
		// comments.
		std::getline(iFile, line);
		std::cout << "PLY File Comments:: '" << line.substr(8) << "'" << std::endl;
		
		// element vertex.
		std::getline(iFile, line);
		std::cout << "PLY: Num vertex " << line.substr(15) << std::endl;
		int numVertex = strtol(line.substr(15).c_str(), 
								 (char **)NULL, 
								 10);

		//TODO: variable parameters
		std::getline(iFile, line);
		std::getline(iFile, line);
		std::getline(iFile, line);
		std::getline(iFile, line);
		std::getline(iFile, line);
		std::getline(iFile, line);
		std::getline(iFile, line);
		std::getline(iFile, line);
		
		double x, y, z;
		while(std::getline(iFile, line) && k<numVertex)
		{
			std::istringstream is(line);
			
			if(!(is >> x >> y >> z))
			{
				std::cout << "PLY: Line " << k << " could not be parsed as xyx double" << std::endl;
			}
			else
			{
				//std::cout << "PLY: X " << x << "  Y "  << y << "  Z "  << z << " found on line: " << k << std::endl;
				m_pointCloud.push_back(vxVector3d(x,y,z));
			}
		
			k++;
		}
	}
	
	virtual std::vector<vxVector3d>& getPointCloud() override
	{
		return m_pointCloud;
	}
	
};

}
#endif // VXPLYIMPORTER_H
