#include "vxSceneParser.h"

using namespace vxCore;

vxSceneParser::vxSceneParser(const std::string &fileName)
	:m_fileName(fileName)
{
}

decltype(auto) vxSceneParser::getLine(std::ifstream &f, std::string &line) const
{
	return std::getline(f, line);
}

std::pair<std::string, std::string> 
vxSceneParser::parseAttribute(const std::string &txt)
{
	std::cout << "\t\tParsing attribute: " << txt << std::endl;
	std::pair<std::string, std::string> ret;
	std::string line;
	std::smatch base_match;

	const std::string sp{"((?:[a-z][a-z]+))(\\s+)(=)(\\s+)(\".*?\")"s};	// Double Quote String 1
	
	const std::regex rel(sp);
	
	if (std::regex_match(txt, base_match, rel))
	{
		ret.first = base_match[1].str();
		auto strLiteral = base_match[5].str();
		ret.second = strLiteral.substr(1,strLiteral.size()-2);
		std::cout << " * ("<<ret.first<<")="<<"("<<ret.second<<")"<< std::endl;
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
			auto firstNonSpace = line.find_first_not_of('\t');
			if(firstNonSpace>=line.size())
			{
				continue;
			}
			
			auto ind = StringUtils::indentation(line);
			
			// then we are reading an attribute
			auto attr = parseAttribute(line.substr(ind));
			
			//Looking for node name
			if(attr.first=="name"s)
			{
				name = attr.second;
			}
			
			//Looking for node type
			if(attr.first=="type"s)
			{
				node->type = attr.second;
			}
			
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
		// Extraction of a sub-match
		std::smatch base_match;
		//looking for nodes to start
		if (std::regex_match(line, base_match, std::regex("(node)")))
		{
			auto newNode = std::make_shared<vxNode>();
			const auto nodeName = parseNodeBody(iFile, newNode);
			
			if (m_nodes.find(nodeName)!=m_nodes.end())
			{
				std::cout << "Node '" << nodeName << "' already existed in scene" << std::endl;
			}
			else
			{
				m_nodes[nodeName] = newNode;
			}
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
