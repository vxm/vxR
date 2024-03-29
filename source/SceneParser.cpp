#include "SceneParser.h"

using namespace vxCore;

#define COUT_PARSING 0

SceneParser::SceneParser(const std::string &fileName) : m_fileName(fileName) {}

decltype(auto) SceneParser::getLine(std::ifstream &f, std::string &line) const
{
	decltype(auto) ln = std::getline(f, line);

	if (f.fail() || (ln && line[0] == '/' && line[1] == '/'))
	{
		line.clear();
	}
	else
	{
		if (!line.empty() && *line.rbegin() == '\r')
		{
			line.erase(line.length() - 1, 1);
		}
	}
	return ln;
}

Attribute SceneParser::parseAttribute(const std::string &txt)
{
// TODO: "attributeName1" won't be caputred due to the number 1. fix it.
#if COUT_PARSING
	std::cout << "\t\tParsing attribute: " << txt << std::endl;
#endif
	std::string line;
	std::smatch base_match;

	Attribute ret;
	if (std::regex_match(txt, base_match, var_string))
	{
		ret.first = base_match[1].str();
		auto strLiteral = base_match[5].str();
		auto convert = strLiteral.substr(1, strLiteral.size() - 2);
		ret.second = Value(convert);
#if COUT_PARSING
		std::cout << " string (" << ret.first << ")="
				  << "(" << ret.second.asString() << ")" << std::endl;
#endif
	}

	if (std::regex_match(txt, base_match, var_int))
	{
		ret.first = base_match[1].str();
		auto intValue = std::stoi(base_match[5].str());
		ret.second.setInt(intValue);
#if COUT_PARSING
		std::cout << " int (" << ret.first << ")="
				  << "(" << ret.second.asInt() << ")" << std::endl;
#endif
	}

	if (std::regex_match(txt, base_match, var_float))
	{
		ret.first = base_match[1].str();
		auto floatValue = std::stof(base_match[5].str());
		ret.second.setFloat(floatValue);
#if COUT_PARSING
		std::cout << " float (" << ret.first << ")="
				  << "(" << ret.second.asFloat() << ")" << std::endl;
#endif
	}

	if (std::regex_match(txt, base_match, var_int_int))
	{
		ret.first = base_match[1].str();
		int intValue1, intValue2;
		std::stringstream strValue;
		strValue << base_match[5] << ' ' << base_match[7];

		strValue >> intValue1 >> intValue2;
		ret.second.setVector2d(v2s(intValue1, intValue2));
#if COUT_PARSING
		auto capt = ret.second.asVector2d();
		std::cout << " v2 (" << ret.first << ")="
				  << "(" << capt->x() << ", " << capt->y() << ")" << std::endl;
#endif
	}

	if (std::regex_match(txt, base_match, var_int_int_int))
	{
		ret.first = base_match[1].str();
		int intValue1, intValue2, intValue3;
		std::stringstream strValue;
		strValue << base_match[5] << ' ' << base_match[7] << ' ' << base_match[9];

		strValue >> intValue1 >> intValue2 >> intValue3;
		ret.second.setVector3d(v3s(intValue1, intValue2, intValue3));
#if COUT_PARSING
		auto capt = ret.second.asVector3d();
		std::cout << " v3 (" << ret.first << ")="
				  << "(" << capt->x() << ", " << capt->y() << ", " << capt->z()
				  << ")" << std::endl;
#endif
	}

	if (std::regex_match(txt, base_match, var_float_float))
	{
		ret.first = base_match[1].str();
		auto floatValue1 = std::stod(base_match[5]);
		auto floatValue2 = std::stod(base_match[7]);

		ret.second.setVector2d(v2s(floatValue1, floatValue2));
#if COUT_PARSING
		auto capt = ret.second.asVector3d();
		std::cout << " v2 (" << ret.first << ")="
				  << "(" << capt->x() << ", " << capt->y() << ")" << std::endl;
#endif
	}

	if (std::regex_match(txt, base_match, var_float_float_float))
	{
		ret.first = base_match[1].str();
		auto floatValue1 = std::stod(base_match[5]);
		auto floatValue2 = std::stod(base_match[7]);
		auto floatValue3 = std::stod(base_match[9]);

		ret.second.setVector3d(v3s(floatValue1, floatValue2, floatValue3));

#if COUT_PARSING
		auto capt = ret.second.asVector3d();
		std::cout << " v3 (" << ret.first << ")="
				  << "--------(" << capt->x() << ", " << capt->y() << ", "
				  << capt->z() << ")" << std::endl;
#endif
	}

	if (std::regex_match(txt, base_match, var_brakets))
	{
		ret.first = base_match[1].str();

		scalar m[16];
		auto matValues =
			base_match[5].str().substr(1, base_match[5].str().size() - 2);
		std::stringstream strValue(matValues);
		for (auto &v : m)
		{
			strValue >> v;
		}

		ret.second.setMatrix44(Matrix44(m));

#if COUT_PARSING
		std::cout << " matrix (" << ret.first << ")=";
		for (unsigned int i = 0; i < 4; i++)
			for (unsigned int j = 0; j < 4; j++)
			{
				std::cout << "[" << (ret.second.asMatrix44()->operator()(i, j)) << "]";
			}
		std::cout << ")" << std::endl;
#endif
	}

	return ret;
}

Status SceneParser::parseNodeBody(std::ifstream &inFile,
								  const vxNodeHandle &node)
{
	// Condition to finish node reading.
	const std::regex rel("(\\})");
	std::smatch base_match;

	Status status;
	std::string line;
	bool found{false};
	// Not optional lines.
	do
	{
		getLine(inFile, line);

		// continue till body is open.
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
			unsigned int ind = StringUtils::indentation(line);
			if (ind >= line.size())
			{
				continue;
			}

			// then we are reading an attribute
			const auto &&attr = parseAttribute(line.substr(ind));
			if (!attr.first.empty())
			{
				node->addAttribute(attr);
			}
		}
	} while (!found);

	return status;
}

VS SceneParser::procesScene()
{
	if (!FileUtils::fileExists(m_fileName))
	{
		std::cerr << "Scene file " << m_fileName << " doesn't exist" << std::endl;
		return VS::kError;
	}

	std::ifstream iFile(m_fileName);
	std::string line;

	// Not optional lines.
	getLine(iFile, line);
	std::cout << "line: '" << line << "'" << std::endl;

	// checking this contains a vx scene description
	if (line != "#vxR scene")
	{
		std::cerr << "scene parse: '" << m_fileName
				  << "' doesn't contain vxR scene information." << std::endl;
		return VS::kError;
	}

	// checking if ASCII
	getLine(iFile, line);
	std::cout << "line: '" << line << "'" << std::endl;
	if (line != "#format ascii 1.0"s)
	{
		std::cerr << "scene parse: File " << m_fileName << " is not ASCII"
				  << std::endl;
		return VS::kError;
	}

	// capturing nodes.
	while (getLine(iFile, line))
	{
		// Extraction of a sub-match
		std::smatch base_match;
		// looking for nodes to start
		if (std::regex_match(line, base_match, std::regex("(node)")))
		{
			auto newNode = std::make_shared<Node>();
			parseNodeBody(iFile, newNode);
			const auto nodeName{newNode->name()};

			if (m_nodes.find(nodeName) != m_nodes.end())
			{
				std::cerr << "Node '" << nodeName
						  << "' not added. There is already a node"
						  << " with that name in scene" << std::endl;
			}
			else if (newNode->active())
			{
				m_nodes[nodeName] = newNode;
			}
		}
	}

	iFile.close();

	std::cout << "scene parse: " << m_fileName << " finished:: '"
			  << m_nodes.size() << "' nodes taken" << std::endl;
	return VS::kSuccess;
}

void SceneParser::printDatabaseInfo()
{
	for (const auto &node : m_nodes)
	{
		std::cout << (*node.second) << std::endl;
	}
}

void SceneParser::clear() { m_nodes.clear(); }

std::vector<vxNodeHandle> SceneParser::getNodesByType(const std::string &type)
{
	std::vector<vxNodeHandle> ret;
	for (const auto &nodePair : m_nodes)
	{
		const auto &node = nodePair.second;
		if (node->type() == type)
		{
			ret.emplace_back(nodePair.second);
		}
	}

	return ret;
}

vxNodeHandle SceneParser::getNodeByName(const std::string &name)
{
	vxNodeHandle ret;

	auto found = m_nodes.find(name);

	if (found != m_nodes.end())
	{
		ret = found->second;
	}

	return ret;
}
