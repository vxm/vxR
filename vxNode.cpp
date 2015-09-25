#include "vxNode.h"
using namespace vxCore;

vxNode::vxNode()
{
	id = nNodes++;
}

std::string vxNode::name() const
{
	return m_name;
}

void vxNode::setName(const std::string &name)
{
	m_name = name;
}

std::string vxNode::type() const
{
	return m_type;
}

void vxNode::setType(const std::string &type)
{
	m_type = type;
}

std::vector<Attribute> vxNode::attributes() const
{
	return m_attributes;
}

void vxNode::setAttributes(const std::vector<Attribute> &attributes)
{
	m_attributes = attributes;
}
void vxNode::addAttribute(const Attribute &attr)
{
	//Looking for node name
	if(attr.first=="name"s)
	{
		m_name = attr.second.asString();
	}
	
	//Looking for node type
	if(attr.first=="type"s)
	{
		m_type = attr.second.asString();
	}	
}

//----------------------------------

