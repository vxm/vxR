#include "vxNode.h"
using namespace vxCore;


bool vxNode::active() const
{
	return m_active;
}

void vxNode::setActive(bool active)
{
	m_active = active;
}

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
		return;
	}
	
	//Looking for node type
	if(attr.first=="type"s)
	{
		m_type = attr.second.asString();
		return;
	}
	
	//Looking for node active state
	if(attr.first=="active"s)
	{
		if(attr.second.asString()=="false"s)
		{
			setActive(false);
		}
	}
	
	m_attributes.emplace_back(attr);
}

// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
vxColor vxNode::getColor(const std::string attrName)
{
	for(const auto nodeAttr: m_attributes)
	{
		if(nodeAttr.first == attrName)
		{
			return vxColor(*nodeAttr.second.asVector3d());
		}
	}
	
	std::cerr << "Node: Color attribute " << attrName << " not found in node" << std::endl;
	return vxColor::black;
}

// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
v3s vxNode::getVector3d(const std::string attrName)
{
	for(const auto nodeAttr: m_attributes)
	{
		if(nodeAttr.first == attrName)
		{
			return *nodeAttr.second.asVector3d();
		}
	}

	std::cerr << "Node: Vector3d attribute " << attrName << " not found in node" << std::endl;
	return v3s::zero;
}

// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
v2s vxNode::getVector2d(const std::string attrName)
{
	for(const auto nodeAttr: m_attributes)
	{
		if(nodeAttr.first == attrName)
		{
			return *nodeAttr.second.asVector2d();
		}
	}
	
	std::cerr << "Node: Vector2d attribute " << attrName << " not found in node" << std::endl;
	return v2s::zero;
}

// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
float vxNode::getFloat(const std::string attrName)
{
	for(const auto nodeAttr: m_attributes)
	{
		if(nodeAttr.first == attrName)
		{
			return nodeAttr.second.asFloat();
		}
	}

	
	std::cerr << "Node: Float attribute " << attrName << " not found in node" << std::endl;
	return 0.f;
}

// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
int vxNode::getInt(const std::string attrName)
{
	for(const auto nodeAttr: m_attributes)
	{
		if(nodeAttr.first == attrName)
		{
			return nodeAttr.second.asInt();
		}
	}

	
	std::cerr << "Node: Int attribute " << attrName << " not found in node" << std::endl;
	return 0;
}

// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
std::string vxNode::getString(const std::string attrName)
{
	for(const auto nodeAttr: m_attributes)
	{
		if(nodeAttr.first == attrName)
		{
			return nodeAttr.second.asString();
		}
	}

	
	std::cerr << "Node: String attribute " << attrName << " not found in node" << std::endl;
	return ""s;
}


// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
bool vxNode::getBool(const std::string attrName)
{
	for(const auto nodeAttr: m_attributes)
	{
		if(nodeAttr.first == attrName)
		{
			return nodeAttr.second.asBool();
		}
	}
	
	std::cerr << "Node: Bool attribute " << attrName << " not found in node" << std::endl;
	return false;
}


// It will retrieve the matching vector3d attribute and will cast
// it's values to a color to be returned.
Matrix44 vxNode::getMatrix(const std::string attrName)
{
	//TODO: I see lambdas to specify the function to convert the data type 
	// and return the default of the value in case it didnt find the attr.
	for(const auto nodeAttr: m_attributes)
	{

		if(nodeAttr.first == attrName)
		{
			return *nodeAttr.second.asMatrix44();
		}
	}

	std::cerr << "Node: Matrix attribute " << attrName << " not found in node" << std::endl;
	return Matrix44::identity44;
}

//----------------------------------

