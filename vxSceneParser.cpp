#include "vxSceneParser.h"

using namespace vxCore;

vxSceneParser::vxSceneParser(const std::string &fileName)
	:m_fileName(fileName)
{
}

decltype(auto) vxSceneParser::getLine(std::ifstream &f, std::string &line) const
{
	decltype(auto) a = std::getline(f, line);
	
//	while(line[0])
	{
		std::getline(f, line);
	}
	
	return a;
}

void vxSceneParser::procesScene()
{
	if (!FileUtils::fileExists(m_fileName))
	{
		std::cout << "Scene file " << m_fileName << " doesn't exist" << std::endl;
		return;
	}

	std::ifstream iFile(m_fileName);
	std::string line;
	unsigned int k {0};

	// Not optional lines.
	getLine(iFile, line);
	
	// checking this contains a scene
	if(line!="#vxR scene")
	{
		std::cout << "scene parse: '" << m_fileName << "' doesn't contain vxR scene information." << std::endl;
		return;
	}
	
	// checking if ASCII
	std::getline(iFile, line);
	if(line!="#format ascii 1.0")
	{
		std::cout << "scene parse: File " << m_fileName << " is not ASCII" << std::endl;
		return;
	}
	
	// capturing nodes.
	while(getLine(iFile, line))
	{	
		std::cout << "line: '" << line << "'" << std::endl;
	}

	std::cout << "scene parse: " << m_fileName << " finished:: '" << m_nodes.size() << "' nodes taken" << std::endl;
}
