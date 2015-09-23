#ifndef VXSCENEPARSER_H
#define VXSCENEPARSER_H

#include <functional>
#include "FileUtils.h"
#include "StringUtils.h"
#include "vxMatrix.h"
#include "vxStatus.h"
#include "vxValue.h"

using namespace std::string_literals;
//simple
const std::regex var_string("((?:[a-z][a-z]+))(\\s+)(=)(\\s+)(\".*?\")"s);
const std::regex var_int("((?:[a-z][a-z]+))(\\s+)(=)(\\s+)(\\d+)"s);
const std::string floatReg = "([+-]?\\d*\\.\\d+)(?![-+0-9\\.])"s;
const std::regex var_float("((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s + floatReg);

// complex
const std::regex var_float_float("((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
									+ floatReg 
									+ "(\\s+)"s
									+ floatReg);

const std::regex var_float_float_float("((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
										+ floatReg 
										+ "(\\s+)"s
										+ floatReg
										+ "(\\s+)"s
										+ floatReg);

const std::regex var_int_int("((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
								+ "(\\d+)"s
								+ "(\\s+)"s
								+ "(\\d+)"s);

const std::regex var_int_int_int("((?:[a-z][a-z]+))(\\s+)(=)(\\s+)"s 
										+ "(\\d+)"s
										+ "(\\s+)"s
										+ "(\\d+)"s
										+ "(\\s+)"s
										+ "(\\d+)"s);



namespace vxCore {

using Attribute = std::pair<std::string, vxValue>;
using VS = vxStatus::code;

static unsigned long nNodes{0};

struct vxNode
{
	int id{0};
	vxNode()
	{
		id=nNodes++;
	}
	
	~vxNode()
	{
		nNodes--;
	}
	
	vxNode(vxNode &&other)
	{
		id=nNodes++;
	}

	vxNode(const vxNode &other)
	{
		id=nNodes++;
	}
	
	vxNode(const vxNode &&other)
	{
		id=nNodes++;
	}
	
	std::string name;
	std::string type;
	vxMatrix transform{vxMatrix::identity};

	//Adds the attribute identifying it first.
	void addAttribute(const Attribute &attr);

	friend std::ostream& operator<<(std::ostream &os, const vxNode& v)
	{
		return os << "Node ::" << v.id << ":: " << v.name << std::endl
					 << "Type: " << v.type << std::endl;
	}

};

class nodeMap:public std::map<std::string, std::shared_ptr<vxNode>>
{
public:
	nodeMap()
	{}
	
};

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
};

}
#endif // VXSCENEPARSER_H
