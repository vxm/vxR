#ifndef VXVALUE_H
#define VXVALUE_H

#include <string>
#include <memory>

#include "vxVector.h"
#include "vxMatrix.h"

namespace vxCore {

enum class vxValueType
{
	kBool,
	kChar,
	kUChar,
	kInt,
	kUInt,
	kLong,
	kULong,
	kLongLong,
	kFloat,
	kDouble,
	kLongDouble,
	kVector2d,
	kVector3d,
	kString,
	kMatrix44,
};

class vxValue
{
	union valueTypes
	{
		bool b;
		char c;
		unsigned char uc;
		int i;
		unsigned int ui;
		long l;
		unsigned long ul;
		long long ll;
		float f;
		double d;
		long double ld;
		//std::shared_ptr<std::string> str;
		//TODO: could this be a shared ptr to void?
		void * vp;
	} 
	value;

	vxValueType valueType;

public:
	vxValue();
	
	vxValue(bool b)
		: valueType{vxValueType::kBool}
	{
		value.b = b;
	}

	vxValue(int i)
		: valueType{vxValueType::kInt}
	{
		value.i = i;
	}

	vxValue(char c)
		: valueType{vxValueType::kChar}
	{
		value.c = c;
	}

	vxValue(unsigned char uc)
		: valueType{vxValueType::kUChar}
	{
		value.uc = uc;
	}

	vxValue(unsigned int ui)
		: valueType{vxValueType::kUInt}
	{
		value.ui = ui;
	}

	vxValue(long l)
		: valueType{vxValueType::kLong} 
	{
		value.l = l;
	}

	vxValue(unsigned long ul)
		: valueType{vxValueType::kULong} 
	{
		value.ul = ul;
	}

	vxValue(long long ll)
		: valueType{vxValueType::kLongLong} 
	{
		value.ll = ll;
	}

	vxValue(float f)
		: valueType{vxValueType::kFloat} 
	{
		value.f = f;
	}

	vxValue(double d)
		: valueType{vxValueType::kDouble} 
	{
		value.d = d;
	}

	vxValue(long double ld)
		: valueType{vxValueType::kLongDouble} 
	{
		value.ld = ld;
	}

	vxValue(std::string str)
		: valueType{vxValueType::kString} 
	{
		value.vp = (void*)str.c_str();
	}

	bool asBool() const
	{
		return value.b;
	}
	
	char asChar() const
	{
		return value.c;
	}
	
	unsigned char asUChar() const
	{
		return value.uc;
	}
	
	int asInt() const
	{
		return value.i;
	}
	
	unsigned int asUInt() const
	{
		return value.ui;
	}
	
	long asLong() const
	{
		return value.l;
	}
	
	unsigned long asULong() const
	{
		return value.ul;
	}
	
	long long asLongLong() const
	{
		return value.ll;
	}
	
	float asFloat() const
	{
		return value.f;
	}
	
	double asDouble() const
	{
		return value.d;
	}
	
	long double asLongDouble() const
	{
		return value.ld;
	}
	
	vxVector2d asVector2d() const
	{
		return *(vxVector2d*)value.vp;
	}
	
	vxVector3d asVector3d() const
	{
		return *(vxVector3d*)value.vp;
	}
	
	std::string asString() const
	{
		return std::string((const char*)value.vp);
	}
	
	vxMatrix asMatrix44() const
	{
		return *(vxMatrix*)value.vp;
	}
	
	
	friend std::ostream& operator<<(std::ostream &os, const vxValue& v)
	{
		switch(v.valueType)
		{
			case vxValueType::kBool:
			{
				os << (v.value.b ? "True" : "False");
				break;
			}
			case vxValueType::kInt:
			{
				os << v.value.i;
				break;
			}
			case vxValueType::kChar:
			{
				os << v.value.c;
				break;
			}
			case vxValueType::kUChar:
			{
				os << v.value.uc;
				break;
			}
			case vxValueType::kUInt:
			{
				os << v.value.ui;
				break;
			}
			case vxValueType::kLong:
			{
				os << v.value.l;
				break;
			}
			case vxValueType::kULong:
			{
				os << v.value.ul;
				break;
			}
			case vxValueType::kLongLong:
			{
				os << v.value.ll;
				break;
			}
			case vxValueType::kFloat:
			{
				os << v.value.f;
				break;
			}
			case vxValueType::kDouble:
			{
				os << v.value.d;
				break;
			}
			case vxValueType::kLongDouble:
			{
				os << v.value.ld;
				break;
			}
			default:
			{
				os << v.value.i;
				break;
			}
		}
		
		return os;
	}
	
};

} // Namespace 
#endif // VXVALUE_H
