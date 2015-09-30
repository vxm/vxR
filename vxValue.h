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
	kInvalid
};

class vxValue
{

public:
	
	static vxValue invalid;
	
	//TODO: move back valueTypes to union
	// it was very advanced already.
	//TODO: for performance, this is really bad. bad!
	struct valueTypes
	{
		valueTypes()
		{}

		valueTypes(std::string instr)
			:str(instr)
		{}
		
		~valueTypes()
		{}

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
		std::string str;
		std::shared_ptr<vxMatrix> mat;
		std::shared_ptr<vxVector3d> v3d;
		std::shared_ptr<vxVector2d> v2d;
	
	}value;

	vxValueType valueType;

public:

	vxValue()
		:valueType{vxValueType::kInvalid}
	{
	}
	
	~vxValue()
	{
		switch(valueType)
		{
			case vxValueType::kVector2d:
			{
				value.v2d.reset();
				break;
			}
			case vxValueType::kVector3d:
			{
				value.v3d.reset();
				break;
			}
			case vxValueType::kMatrix44:
			{
				value.mat.reset();
				break;
			}
			case vxValueType::kString:
			{
				value.str.clear();
				break;
			}
		default:
			value.str.clear();
		}
	}
	
	vxValue(const vxValue &val)
	{
		valueType = val.valueType;
		switch(valueType)
		{
			case vxValueType::kBool:
			{
				value.b = (val.value.b);
				break;
			}
			case vxValueType::kInt:
			{
				value.i = (val.value.i);
				break;
			}
			case vxValueType::kChar:
			{
				value.c = (val.value.c);
				break;
			}
			case vxValueType::kUChar:
			{
				value.uc = (val.value.uc);
				break;
			}
			case vxValueType::kUInt:
			{
				value.ui = (val.value.ui);
				break;
			}
			case vxValueType::kLong:
			{
				value.l = (val.value.l);
				break;
			}
			case vxValueType::kULong:
			{
				value.ul = (val.value.ul);
				break;
			}
			case vxValueType::kLongLong:
			{
				value.ll = (val.value.ll);
				break;
			}
			case vxValueType::kFloat:
			{
				value.f = (val.value.f);
				break;
			}
			case vxValueType::kDouble:
			{
				value.d = (val.value.d);
				break;
			}
			case vxValueType::kLongDouble:
			{
				value.ld = (val.value.ld);
				break;
			}
			case vxValueType::kVector2d:
			{
				value.v2d = (val.value.v2d);
				break;
			}
			case vxValueType::kVector3d:
			{
				value.v3d = (val.value.v3d);
				break;
			}
			case vxValueType::kMatrix44:
			{
				value.mat = (val.value.mat);
				break;
			}
			case vxValueType::kString:
			{
				value.str = val.value.str;
				break;
			}
			default:
			{
				value.i = (val.value.i);
				break;
			}
		}
	}
	
	vxValue& operator=(const vxValue &val)
	{
		valueType = val.valueType;
		switch(valueType)
		{
			case vxValueType::kBool:
			{
				value.b = (val.value.b);
				break;
			}
			case vxValueType::kInt:
			{
				value.i = (val.value.i);
				break;
			}
			case vxValueType::kChar:
			{
				value.c = (val.value.c);
				break;
			}
			case vxValueType::kUChar:
			{
				value.uc = (val.value.uc);
				break;
			}
			case vxValueType::kUInt:
			{
				value.ui = (val.value.ui);
				break;
			}
			case vxValueType::kLong:
			{
				value.l = (val.value.l);
				break;
			}
			case vxValueType::kULong:
			{
				value.ul = (val.value.ul);
				break;
			}
			case vxValueType::kLongLong:
			{
				value.ll = (val.value.ll);
				break;
			}
			case vxValueType::kFloat:
			{
				value.f = (val.value.f);
				break;
			}
			case vxValueType::kDouble:
			{
				value.d = (val.value.d);
				break;
			}
			case vxValueType::kLongDouble:
			{
				value.ld = (val.value.ld);
				break;
			}
			case vxValueType::kVector2d:
			{
				value.v2d = (val.value.v2d);
				break;
			}
			case vxValueType::kVector3d:
			{
				value.v3d = (val.value.v3d);
				break;
			}
			case vxValueType::kMatrix44:
			{
				value.mat = (val.value.mat);
				break;
			}
			case vxValueType::kString:
			{
				value.str = val.value.str;
				break;
			}
			default:
			{
				value.i = (val.value.i);
				break;
			}
		}
		
		return *this;
	}
	
	vxValue(vxValue &&val)
		: valueType{std::move(val.valueType)}
	{
		switch(valueType)
		{
			case vxValueType::kBool:
			{
				value.b = std::move(val.value.b);
				break;
			}
			case vxValueType::kInt:
			{
				value.i = std::move(val.value.i);
				break;
			}
			case vxValueType::kChar:
			{
				value.c = std::move(val.value.c);
				break;
			}
			case vxValueType::kUChar:
			{
				value.uc = std::move(val.value.uc);
				break;
			}
			case vxValueType::kUInt:
			{
				value.ui = std::move(val.value.ui);
				break;
			}
			case vxValueType::kLong:
			{
				value.l = std::move(val.value.l);
				break;
			}
			case vxValueType::kULong:
			{
				value.ul = std::move(val.value.ul);
				break;
			}
			case vxValueType::kLongLong:
			{
				value.ll = std::move(val.value.ll);
				break;
			}
			case vxValueType::kFloat:
			{
				value.f = std::move(val.value.f);
				break;
			}
			case vxValueType::kDouble:
			{
				value.d = std::move(val.value.d);
				break;
			}
			case vxValueType::kLongDouble:
			{
				value.ld = std::move(val.value.ld);
				break;
			}
			case vxValueType::kVector2d:
			{
				value.v2d = std::move(val.value.v2d);
				break;
			}
			case vxValueType::kVector3d:
			{
				value.v3d = std::move(val.value.v3d);
				break;
			}
			case vxValueType::kMatrix44:
			{
				value.mat = std::move(val.value.mat);
				break;
			}
			case vxValueType::kString:
			{
				value.str = std::move(val.value.str);
				break;
			}
			default:
			{
				value.i = std::move(val.value.i);
				break;
			}
		}
	}
	
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
		, value(str)
	{
	}

	vxValue(std::shared_ptr<vxVector2d> v2d)
		: valueType{vxValueType::kVector2d} 
	{
		value.v2d = v2d;
	}

	vxValue(std::shared_ptr<vxVector3d> v3d)
		: valueType{vxValueType::kVector3d} 
	{
		value.v3d = v3d;
	}
	
	vxValue(std::shared_ptr<vxMatrix> m44)
		: valueType{vxValueType::kMatrix44} 
	{
		value.mat = m44 ;
	}
	
	void setBool(bool other)
	{
		valueType = vxValueType::kBool;
		value.b = other;
	}
	
	void setChar(char other)
	{
		valueType = vxValueType::kChar;
		value.c = other;
	}
	
	void setUChar(unsigned char other)
	{
		valueType = vxValueType::kBool;
		value.uc = other;
	}
	
	void setInt(int other)
	{
		valueType = vxValueType::kInt;
		value.i = other;
	}
	
	void setUInt(unsigned int other)
	{
		valueType = vxValueType::kUInt;
		value.ui = other;
	}
	
	void setLong(long other)
	{
		valueType = vxValueType::kLong;
		value.l = other;
	}
	
	void setULong(unsigned long other)
	{
		valueType = vxValueType::kULong;
		value.ul = other;
	}
	
	void setLongLong(long long other)
	{
		valueType = vxValueType::kLongLong;
		value.ll = other;
	}
	
	void setFloat(float other)
	{
		valueType = vxValueType::kFloat;
		value.f = other;
	}
	
	void setDouble(double other)
	{
		valueType = vxValueType::kDouble;
		value.d = other;
	}
	
	void setLongDouble(long double other)
	{
		valueType = vxValueType::kLongDouble;
		value.ld = other;
	}
	
	void setVector2d(std::shared_ptr<vxVector2d> other)
	{
		valueType = vxValueType::kVector2d;
		value.v2d = other;
	}
	
	void setVector3d(std::shared_ptr<vxVector3d> other)
	{
		valueType = vxValueType::kVector3d;
		value.v3d = other;
	}
	
	void setString(std::string other)
	{
		valueType = vxValueType::kString;
		value.str = other;
	}
	
	void setMatrix44(std::shared_ptr<vxMatrix> other)
	{
		valueType = vxValueType::kMatrix44;
		value.mat = other;
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
	
	std::shared_ptr<vxVector2d> asVector2d() const
	{
		return value.v2d;
	}
	
	std::shared_ptr<vxVector3d> asVector3d() const
	{
		return value.v3d;
	}
	
	std::string asString() const
	{
		return value.str;
	}
	
	std::shared_ptr<vxMatrix> asMatrix44() const
	{
		return value.mat;
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
	
	bool isInvalid() const
	{
		return valueType == vxValueType::kInvalid;
	}
};

} // Namespace 
#endif // VXVALUE_H
