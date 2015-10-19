#ifndef VXNODE_H
#define VXNODE_H

#include <map>
#include <vector>
#include "vxVector.h"
#include "vxColor.h"
#include "vxValue.h"

static unsigned long nNodes{0};

namespace vxCore
{

using namespace std::string_literals;
//Attribute should have default values.
using Attribute = std::pair<std::string, vxValue>;

class vxNode
{
	bool m_active{true};
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
		: m_name{std::move(other.m_name)}
		, m_type{std::move(other.m_type)}
		, m_attributes{std::move(other.m_attributes)}
	{
		id = nNodes++;
	}
	
	//Adds the attribute identifying it first.
	void addAttribute(const Attribute &attr);

	friend std::ostream& operator<<(std::ostream &os, const vxNode& v)
	{
		os << "Node ::" 
			<< v.id 
			<< ":: " 
			<< v.m_name 
			<< std::endl
			<< "Type: " 
			<< v.m_type
			<< std::endl;

		for(const auto nodeAttr: v.m_attributes)
		{
			os << "\t" << nodeAttr.first << std::endl;
		}
		
		return os;
	}

	vxColor getColorAttribute(const std::string attrName);
	float getFloatAttribute(const std::__cxx11::string attrName);
	vxVector3d getVector3dAttribute(const std::__cxx11::string attrName);
	vxVector2d getVector2dAttribute(const std::__cxx11::string attrName);
	int getIntAttribute(const std::__cxx11::string attrName);
	std::string getStringAttribute(const std::__cxx11::string attrName);
	bool getBoolAttribute(const std::__cxx11::string attrName);

	std::string name() const;
	void setName(const std::string &name);
	std::string type() const;
	void setType(const std::string &type);
	std::vector<Attribute> attributes() const;
	void setAttributes(const std::vector<Attribute> &attributes);
	bool active() const;
	void setActive(bool active);
};

class nodeMap:public std::map<std::string, std::shared_ptr<vxNode>>
{
public:
	nodeMap()
	{}
	
};

}
#endif // VXNODE_H
