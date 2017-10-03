#ifndef VXNODE_H
#define VXNODE_H

#include <map>
#include <vector>
#include "Vector.h"
#include "Color.h"
#include "Value.h"

namespace vxCore
{

static unsigned long nNodes{0};

using namespace std::string_literals;
// Attribute should have default values.
using Attribute = std::pair<std::string, Value>;

class Node
{

	bool m_active{true};
	std::string m_name;
	std::string m_type;
	Matrix44 transform{Matrix44::identity44};

	std::vector<Attribute> m_attributes;
	int id{0};

public:

	std::shared_ptr<void> m_sharedObj;

	Node();
	~Node() { nNodes--; }

	Node(Node &&other)
	    : m_name{std::move(other.m_name)}, m_type{std::move(other.m_type)},
	      m_attributes{std::move(other.m_attributes)}
	{
		id = nNodes++;
	}

	Node(const Node &other)
	    : m_name{other.m_name}, m_type{other.m_type},
	      m_attributes{other.m_attributes}
	{
		id = nNodes++;
	}

	Node(const Node &&other)
	    : m_name{std::move(other.m_name)}, m_type{std::move(other.m_type)},
	      m_attributes{std::move(other.m_attributes)}
	{
		id = nNodes++;
	}

	// Adds the attribute identifying it first.
	void addAttribute(const Attribute &attr);

	friend std::ostream &operator<<(std::ostream &os, const Node &v)
	{
		os << "Node ::" << v.id << ":: " << v.m_name << std::endl
		   << "Type: " << v.m_type << std::endl;

		for (const auto nodeAttr : v.m_attributes)
		{
			os << "\t" << nodeAttr.first << std::endl;
		}

		return os;
	}

	Color getColor(const std::string attrName);
	float getFloat(const std::string attrName);
	v3s getVector3d(const std::string attrName);
	v2s getVector2d(const std::string attrName);
	int getInt(const std::string attrName);
	std::string getString(const std::string attrName);
	bool getBool(const std::string attrName);
	Matrix44 getMatrix(const std::string attrName);

	std::string name() const;
	void setName(const std::string &name);
	std::string type() const;
	void setType(const std::string &type);
	std::vector<Attribute> attributes() const;
	void setAttributes(const std::vector<Attribute> &attributes);
	bool active() const;
	void setActive(bool active);
};

using vxNodeHandle = std::shared_ptr<Node>;

using nodeMap = std::map<std::string, vxNodeHandle>;
}
#endif // VXNODE_H
