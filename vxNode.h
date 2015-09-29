#ifndef VXNODE_H
#define VXNODE_H

#include <map>
#include <vector>
#include "vxValue.h"

static unsigned long nNodes{0};

namespace vxCore
{

using namespace std::string_literals;
using Attribute = std::pair<std::string, vxValue>;

class vxNode
{
	std::string m_name;
	std::string m_type;
	vxMatrix transform{vxMatrix::identity};
	
	std::vector<Attribute> m_attributes;
	int id{0};

public:

	vxNode();
	~vxNode()
	{
		nNodes--;
	}
	
	vxNode(vxNode &&other)
		: m_name{std::move(other.m_name)}
		, m_type{std::move(other.m_type)}
		, m_attributes{std::move(other.m_attributes)}
	{
		id = nNodes++;
	}

	vxNode(const vxNode &other)
		: m_name{other.m_name}
		, m_type{other.m_type}
		, m_attributes{other.m_attributes}
	{
		id = nNodes++;
	}
	
	vxNode(const vxNode &&other)
	{
		id = nNodes++;
	}

	
	//Adds the attribute identifying it first.
	void addAttribute(const Attribute &attr);

	friend std::ostream& operator<<(std::ostream &os, const vxNode& v)
	{
		return os << "Node ::" << v.id << ":: " << v.m_name << std::endl
					 << "Type: " << v.m_type << std::endl;
	}

	vxValue getColorAttribute(const std::string attrName);
	std::string name() const;
	void setName(const std::string &name);
	std::string type() const;
	void setType(const std::string &type);
	std::vector<Attribute> attributes() const;
	void setAttributes(const std::vector<Attribute> &attributes);
};

class nodeMap:public std::map<std::string, std::shared_ptr<vxNode>>
{
public:
	nodeMap()
	{}
	
};

}
#endif // VXNODE_H
