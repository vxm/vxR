#include "vxPLYImporter.h"

using namespace vxCore;

vxPLYImporter::vxPLYImporter()
{
}

void vxPLYImporter::processPLYFile(const std::string &fileName)
{
	m_pointCloud.clear();
	if (!FileUtils::fileExists(fileName))
	{
		std::cout << "PLY: File " << fileName << " doesn't exist" << std::endl;
		return;
	}

	std::ifstream iFile(fileName);
	std::string line;
	unsigned int k {0};

	// Not optional lines.
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
		std::cerr << "PLY: File " << fileName << " is not ASCII" << std::endl;
		return;
	}
	
	//optional lines
	
	// comments.
	std::getline(iFile, line);
	std::cout << "PLY File Comments:: '" << line.substr(8) << "'" << std::endl;
	
	// element vertex.
	std::getline(iFile, line);
	std::cout << "PLY: Num vertex: " << line.substr(15) << std::endl;
	int numVertex = strtol(line.substr(15).c_str(), 
						   (char **)NULL, 
						   10/*Base 10*/);
	
	unsigned int nFaces{0u};
	
	// reading properties.
	do
	{
		std::getline(iFile, line);
	
		auto lineTokens = StringUtils::tokenizeSpace(line);
		std::cout << "PLY: paramter tokens: ";
		for(auto t:lineTokens)
		{
			std::cout << " [" << t << "]";
		}
		std::cout << std::endl;
		
		if(lineTokens.size()==3
				&& lineTokens[0]=="element"
				&& lineTokens[1]=="face")
		{
			nFaces = std::stoi(lineTokens[2]);
			std::cout << "PLY: num faces: " << nFaces << std::endl;
		}
	}
	while(line!="end_header");
	
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
			m_pointCloud.push_back(vxVector3d{x,y,z});
		}
		
		k++;
	}
	
	std::cout << "PLY file " << fileName << " Ended:: '" << m_pointCloud.size() << "' points taken" << std::endl;
	iFile.close();
}

std::vector<vxVector3d> &vxPLYImporter::getPointCloud()
{
	return m_pointCloud;
}
