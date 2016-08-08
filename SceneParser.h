#ifndef VXSCENEPARSER_H
#define VXSCENEPARSER_H

#include <functional>

#include "FileUtils.h"
#include "StringUtils.h"
#include "Matrix44.h"
#include "Status.h"
#include "Value.h"
#include "Node.h"

using namespace std::string_literals;

const std::string attributeNameEqSp = "((?:[a-z][A-z0-9]*))(\\s+)(=)(\\s+)"s;
//rule descriptions
const std::string floatReg{"([+-]?\\d*\\.\\d+)(?![-+0-9\\.])"s};
const std::string spaceReg{"(\\s+)"s};
const std::string intReg{"(\\d+)"s};

//simple
const std::regex var_string{attributeNameEqSp
									+"(\".*?\")"s};
const std::regex var_int{attributeNameEqSp
									+"(\\d+)"s};
const std::regex var_float{attributeNameEqSp 
									+ floatReg};
const std::regex var_brakets{attributeNameEqSp
									+ "(\\[.*?\\])"s};

// complex numbers
const std::regex var_float_float{attributeNameEqSp 
									+ floatReg 
									+ spaceReg
									+ floatReg};

const std::regex var_float_float_float{attributeNameEqSp 
										+ floatReg 
										+ spaceReg
										+ floatReg
										+ spaceReg
										+ floatReg};

const std::regex var_int_int{attributeNameEqSp 
								+ intReg
								+ spaceReg
								+ intReg};

const std::regex var_int_int_int{attributeNameEqSp 
										+ intReg
										+ spaceReg
										+ intReg
										+ spaceReg
										+ intReg};



namespace vxCore {

using Attribute = std::pair<std::string, Value>;
using VS = Status::code;

class SceneParser
{
	std::string m_fileName;
	nodeMap m_nodes;
	
public:

	SceneParser(const std::string &fileName);

	decltype(auto) getLine(std::ifstream &f, std::string &line) const;

	std::pair<std::string, Value> parseAttribute(const std::string &txt);

	Status parseNodeBody(std::ifstream &inFile, 
							  vxNodeHandle node);

	VS procesScene();

	void printDatabaseInfo();

	void clear();
	
	std::vector<vxNodeHandle> getNodesByType(const std::string &type);
	///
	/// \brief getNodeByName
	/// \param name
	/// \return 
	///gets a node of any type with that name
	vxNodeHandle getNodeByName(const std::string &name);
};

}
#endif // VXSCENEPARSER_H
