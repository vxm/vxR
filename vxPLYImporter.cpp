#include "vxPLYImporter.h"

using namespace vxCore;

vxPLYImporter::vxPLYImporter(std::shared_ptr<vxGeometry> geo)
	:m_geo(geo)
{
}

std::shared_ptr<vxGeometry> vxPLYImporter::getGeometry() const
{
	return m_geo;
}

void vxPLYImporter::setGeo(const std::shared_ptr<vxGeometry> &geo)
{
	m_geo = geo;
}

void vxPLYImporter::processPLYFile(const std::string &fileName)
{
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
	std::cout << "PLY: File Comments:: '" << line.substr(8) << "'" << std::endl;
	
	// element vertex.
	std::getline(iFile, line);
	std::cout << "PLY: Num vertex: " << line.substr(15) << std::endl;
	unsigned int numVertex = strtol(line.substr(15).c_str(), 
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
			m_geo->addVertex(vxVector3d{x,y,z});
		}
		
		k++;
	}
	
	
	// reading properties.
	while(std::getline(iFile, line))
	{
		auto lineTokens = StringUtils::tokenizeSpace(line);
		if(lineTokens.size()==4	&& lineTokens[0]=="3")
		{
			int a = std::stoi(lineTokens[1]);
			int b = std::stoi(lineTokens[2]);
			int c = std::stoi(lineTokens[3]);
			m_geo->addTriangle(a,b,c);
		}
	}
	
	std::cout << "PLY: file " 
			  << fileName 
			  << " :: '" 
			  << m_geo->vertexCount() 
			  << "' vertices " 
			  << " :: '" 
			  << m_geo->triangleCount() 
			  << "' triangles taken" 
			  << std::endl;
	
	iFile.close();
}

