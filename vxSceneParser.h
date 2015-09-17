#ifndef VXSCENEPARSER_H
#define VXSCENEPARSER_H

#include <functional>
#include "FileUtils.h"
#include "StringUtils.h"
#include "vxMatrix.h"
#include "vxStatus.h"`

namespace vxCore {

using namespace std::string_literals;
using VS = vxStatus::code;

static unsigned long nNodes{0};

struct vxNode
{
	int id{0};
	vxNode()
	{
		id=nNodes++;
	}
	
	std::string name;
	std::string type;
	vxMatrix transform{vxMatrix::identity};

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

	std::pair<std::string, std::string> parseAttribute(const std::string &txt);

	std::string parseNodeBody(std::ifstream &inFile, 
							  std::shared_ptr<vxNode> node);

	VS procesScene();

	void printSceneInfo();

	void clear();
};

}
#endif // VXSCENEPARSER_H
