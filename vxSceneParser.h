#ifndef VXSCENEPARSER_H
#define VXSCENEPARSER_H

#include <functional>
#include "FileUtils.h"
#include "vxMatrix.h"

namespace vxCore {

static long nNodes{0};

struct vxNode
{
	vxNode()
	{
		id=nNodes++;
	}
	
	std::string name;
	std::string type;
	vxMatrix transform{vxMatrix::identity};
	
	int id{0};

	friend std::ostream& operator<<(std::ostream &os, const vxNode& v)
	{
		return os << "Node ::" << v.id << ":: " << v.name << std::endl
					 << "Type: " << v.type << std::endl;
	}

};

class nodeMap:public std::map<std::string, vxNode*>
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
	
	void procesScene();
	
	decltype(auto) getLine(std::ifstream &f, std::string &line) const;

	void printSceneInfo()
	{
		for(const auto& node: m_nodes)
		{
			std::cout << (*node.second) << std::endl;
		}
	}
	
	void clear()
	{
		m_nodes.clear();
	}
};

}
#endif // VXSCENEPARSER_H
