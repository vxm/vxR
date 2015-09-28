#ifndef VXSCENEPARSER_H
#define VXSCENEPARSER_H

#include <functional>

#include "FileUtils.h"
#include "StringUtils.h"
#include "vxMatrix.h"
#include "vxStatus.h"
#include "vxValue.h"
#include "vxNode.h"

using namespace std::string_literals;

//rule descriptions
const std::string floatReg{"([+-]?\\d*\\.\\d+)(?![-+0-9\\.])"s};
const std::string spaceReg{"(\\s+)"s};
const std::string intReg{"(\\d+)"s};

//simple
const std::regex var_string{"((?:[a-z][A-z0-9]*))(\\s+)(=)(\\s+)(\".*?\")"s};
const std::regex var_int{"((?:[a-z][A-z0-9]*))(\\s+)(=)(\\s+)(\\d+)"s};
const std::regex var_float{"((?:[a-z][A-z0-9]*))(\\s+)(=)(\\s+)"s 
									+ floatReg};

const std::regex var_brakets{"((?:[a-z][A-z0-9]*))(\\s+)(=)(\\s+)"s 
									+ "(\\[.*?\\])"s};

// complex numbers
const std::regex var_float_float{"((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
									+ floatReg 
									+ spaceReg
									+ floatReg};

const std::regex var_float_float_float{"((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
										+ floatReg 
										+ spaceReg
										+ floatReg
										+ spaceReg
										+ floatReg};

const std::regex var_int_int{"((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
								+ intReg
								+ spaceReg
								+ intReg};

const std::regex var_int_int_int{"((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
										+ intReg
										+ spaceReg
										+ intReg
										+ spaceReg
										+ intReg};



namespace vxCore {

using Attribute = std::pair<std::string, vxValue>;
using VS = vxStatus::code;

class vxSceneParser
{
	std::string m_fileName;
	nodeMap m_nodes;
	
public:

	vxSceneParser(const std::string &fileName);

	decltype(auto) getLine(std::ifstream &f, std::string &line) const;

	std::pair<std::__cxx11::string, vxValue> parseAttribute(const std::string &txt);

	vxStatus parseNodeBody(std::ifstream &inFile, 
							  std::shared_ptr<vxNode> node);

	VS procesScene();

	void printSceneInfo();

	void clear();
	
	std::vector<std::shared_ptr<vxNode>> getNodesByType(const std::string &type);
};

}
#endif // VXSCENEPARSER_H
