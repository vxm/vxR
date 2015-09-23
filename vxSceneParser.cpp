#include "vxSceneParser.h"

using namespace vxCore;


void vxNode::addAttribute(const Attribute &attr)
{
	//Looking for node name
	if(attr.first=="name"s)
	{
		name = attr.second.asString();
	}
	
	//Looking for node type
	if(attr.first=="type"s)
	{
		type = attr.second.asString();
	}	
}

vxSceneParser::vxSceneParser(const std::string &fileName)
	:m_fileName(fileName)
{
}

decltype(auto) vxSceneParser::getLine(std::ifstream &f, std::string &line) const
{
	return std::getline(f, line);
}

Attribute vxSceneParser::parseAttribute(const std::string &txt)
{
	std::cout << "\t\tParsing attribute: " << txt << std::endl;
	std::string line;
	std::smatch base_match;

	Attribute ret;
	if (std::regex_match(txt, base_match, var_string))
	{
		ret.first = base_match[1].str();
		auto strLiteral = base_match[5].str();
		auto convert = strLiteral.substr(1,strLiteral.size()-2);

		ret.second = vxValue(convert);
		std::cout << " string ("
				  << ret.first
				  << ")="
				  << "("
				  << ret.second.asString()
				  << ")"
				  << std::endl;

	}

	if (std::regex_match(txt, base_match, var_int))
	{
		ret.first = base_match[1].str();
		auto strValue = std::stringstream(base_match[5].str());
		int intValue;
		strValue >> intValue;
		ret.second.setInt(intValue);
		std::cout << " int ("
				  <<ret.first
				 <<")="
				<<"("<<ret.second.asInt()<<")"<< std::endl;
	}

	if (std::regex_match(txt, base_match, var_float))
	{
		ret.first = base_match[1].str();
		auto strValue = std::stringstream(base_match[5].str());
		float floatValue;
		strValue >> floatValue;
		ret.second.setFloat(floatValue);
		std::cout << " float ("
				  << ret.first
				  << ")="
				  << "("
				  << ret.second.asFloat()
				  << ")"<< std::endl;
	}

	if (std::regex_match(txt, base_match, var_int_int))
	{
		ret.first = base_match[1].str();
		auto strValue = std::stringstream(base_match[5].str());
		int intValue1, intValue2;
		strValue >> intValue1;
		strValue >> intValue2;
		ret.second.setVector2d(std::make_shared<vxVector2d>(intValue1, intValue2));
		auto capt = ret.second.asVector2d();
		std::cout << " vxVector2d (" << ret.first 
				  << ")="
				  << "(" << capt->x()
				  << ", " << capt->y()
				  << ")" << std::endl;
	}
	
	if (std::regex_match(txt, base_match, var_int_int_int))
	{
		ret.first = base_match[1].str();
		auto strValue = std::stringstream(base_match[5].str());
		int intValue1, intValue2, intValue3;
		strValue >> intValue1;
		strValue >> intValue2;
		strValue >> intValue3;
		ret.second.setVector3d(std::make_shared<vxVector3d>(intValue1, intValue2, intValue3));
		auto capt = ret.second.asVector3d();
		std::cout << " vxVector3d (" << ret.first 
				  << ")="
				  << "(" << capt->x()
				  << ", " << capt->y()
				  << ", " << capt->z()
				  << ")" << std::endl;
	}
	
	
	
	
	
	
	
	
	
	
	
	if (std::regex_match(txt, base_match, var_float_float))
	{
		ret.first = base_match[1].str();
		auto strValue = std::stringstream(base_match[5].str());
		float floatValue1, floatValue2;
		strValue >> floatValue1;
		strValue >> floatValue2;
		ret.second.setVector2d(std::make_shared<vxVector2d>(floatValue1, floatValue2));
		auto capt = ret.second.asVector2d();
		std::cout << " vxVector2d (" << ret.first 
				  << ")="
				  << "(" << capt->x()
				  << ", " << capt->y()
				  << ")" << std::endl;
	}
	
	
	if (std::regex_match(txt, base_match, var_float_float_float))
	{
		ret.first = base_match[1].str();
		auto strValue = std::stringstream(base_match[5].str());
		float floatValue1, floatValue2, floatValue3;
		strValue >> floatValue1;
		strValue >> floatValue2;
		strValue >> floatValue3;
		ret.second.setVector3d(std::make_shared<vxVector3d>(floatValue1, 
															floatValue2,
															floatValue3));
		auto capt = ret.second.asVector2d();
		std::cout << " vxVector3d (" << ret.first 
				  << ")="
				  << "(" << capt->x()
				  << ", " << capt->y()
				  << ")" << std::endl;
	}
	
	return ret;
}


vxStatus vxSceneParser::parseNodeBody(std::ifstream &inFile, 
										 std::shared_ptr<vxNode> node)
{
	// Condition to finish node reading.
	const std::regex rel("(\\})");
	std::smatch base_match;

	vxStatus status;
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
			int ind = StringUtils::indentation(line);
			if(ind>=line.size())
			{
				continue;
			}
			
			// then we are reading an attribute
			const auto&& attr = parseAttribute(line.substr(ind));
			if(attr.first.size())
			{
				node->addAttribute(attr);
			}
		}
	}
	while(!found);

	return status;
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

	// Not optional lines.
	getLine(iFile, line);
	std::cout << "line: '" << line << "'" << std::endl;
	
	// checking this contains a vx scene description
	if(line!="#vxR scene")
	{
		std::cout << "scene parse: '" << m_fileName << "' doesn't contain vxR scene information." << std::endl;
		return VS::kError;
	}
	
	// checking if ASCII
	getline(iFile, line);
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
			parseNodeBody(iFile, newNode);
			const auto nodeName{newNode->name};
			
			if (m_nodes.find(nodeName)!=m_nodes.end())
			{
				std::cout << "Node '" 
						<< nodeName 
						<< "' not added. There is already a node"
						<< " with that name in scene" << std::endl;
			}
			else
			{
				m_nodes[nodeName] = newNode;
			}
		}
	}
	
	iFile.close();

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
