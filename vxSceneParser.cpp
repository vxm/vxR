#include "vxSceneParser.h"

using namespace vxCore;

vxSceneParser::vxSceneParser(const std::string &fileName)
	:m_fileName(fileName)
{
}

decltype(auto) vxSceneParser::getLine(std::ifstream &f, std::string &line) const
{
	decltype(auto) a = std::getline(f, line);

	return a;
}

std::pair<std::string, std::string> 
vxSceneParser::parseAttribute(const std::string &txt)
{
	std::cout << "\t\tParsing attribute" << txt << std::endl;
	std::pair<std::string, std::string> ret;
	std::string line;
	std::smatch base_match;

	const std::string re1="((?:[a-z][a-z]+))";	// Word 1
	const std::string re2="(\\s+)";	// White Space 1
	const std::string re3="(=)";	// Any Single Character 1
	const std::string re4="(\\s+)";	// White Space 2
	const std::string re5="(\".*?\")";	// Double Quote String 1
	
	const std::regex rel(re1+re2+re3+re4+re5);
	if (std::regex_match(line, base_match, rel))
	{
		std::string word1 = base_match[1].str();
		std::string ws1 = base_match[2].str();
		std::string c1 = base_match[3].str();
		std::string ws2 = base_match[4].str();
		std::string string1 = base_match[5].str();
		std::cout << "("<<word1<<")"<<"("<<ws1<<")"<<"("<<c1<<")"<<"("<<ws2<<")"<<"("<<string1<<")"<< std::endl;
	}

	return ret;
}

std::string vxSceneParser::parseNodeBody(std::ifstream &inFile, 
										 std::shared_ptr<vxNode> node)
{
	// Condition to finish node reading.
	const std::regex rel("(\\})");
	std::smatch base_match;

	std::string name;
	std::string line;
	bool found{false};
	// Not optional lines.
	do
	{
		getLine(inFile, line);

		//continue till body is open.
		if (std::regex_match(line, base_match, std::regex("(\\{)")))
		{
			continue;
		}
		
		// Check the body is not finished
		if (std::regex_match(line, base_match, rel))
		{
			found = true;
		}
		else
		{
			// then we are reading an attribute
			parseAttribute(line);
		}
	}
	while(!found);

	return name;
}

VS vxSceneParser::procesScene()
{
	if (!FileUtils::fileExists(m_fileName))
	{
		std::cout << "Scene file " << m_fileName << " doesn't exist" << std::endl;
		return VS::kError;
	}

	std::ifstream iFile(m_fileName);
	std::string line;
	auto k {0u};

	// Not optional lines.
	getLine(iFile, line);
	std::cout << "line: '" << line << "'" << std::endl;
	
	// checking this contains a scene
	if(line!="#vxR scene")
	{
		std::cout << "scene parse: '" << m_fileName << "' doesn't contain vxR scene information." << std::endl;
		return VS::kError;
	}
	
	// checking if ASCII
	std::getline(iFile, line);
	std::cout << "line: '" << line << "'" << std::endl;
	if(line!="#format ascii 1.0"s)
	{
		std::cout << "scene parse: File " << m_fileName << " is not ASCII" << std::endl;
		return VS::kError;
	}
	
	// capturing nodes.
	while(getLine(iFile, line))
	{	
		//std::cout << "line: '" << line << "'" << std::endl;
		std::regex rel("(node)");
		// Extraction of a sub-match
		std::smatch base_match;
		if (std::regex_match(line, base_match, rel))
		{
			auto newNode = std::make_shared<vxNode>();
			const auto nodeName = parseNodeBody(iFile, newNode);
			
			if (m_nodes.find(nodeName)==std::cend(m_nodes))
			{
				std::cout << "Node " << nodeName << "already existed in scene" << std::endl;
			}
			
			std::cout << "found a node" << std::endl;
		}
	}

	std::cout << "scene parse: " << m_fileName << " finished:: '" << m_nodes.size() << "' nodes taken" << std::endl;
	return VS::kSuccess;
}

void vxSceneParser::printSceneInfo()
{
	for(const auto& node: m_nodes)
	{
		std::cout << (*node.second) << std::endl;
	}
}

void vxSceneParser::clear()
{
	m_nodes.clear();
}
