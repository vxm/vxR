#ifndef VXVALUE_H
#define VXVALUE_H

#include <string>
#include <memory>

#include "Vector.h"
#include "Matrix44.h"

namespace vxCore {

enum class ValueType
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

class Value
{

public:
	
	static Value invalid;
	ValueType valueType;
	
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
		std::shared_ptr<Matrix44> mat;
		std::shared_ptr<v3s> v3d;
		std::shared_ptr<v2s> v2d;
	
	}value;


public:

	Value()
		:valueType{ValueType::kInvalid}
	{
	}
	
	~Value()
	{
		switch(valueType)
		{
			case ValueType::kVector2d:
			{
				value.v2d.reset();
				break;
			}
			case ValueType::kVector3d:
			{
				value.v3d.reset();
				break;
			}
			case ValueType::kMatrix44:
			{
				value.mat.reset();
				break;
			}
			case ValueType::kString:
			{
				value.str.clear();
				break;
			}
		default:
			value.str.clear();
		}
	}
	
	Value(const Value &val)
	{
		valueType = val.valueType;
		switch(valueType)
		{
			case ValueType::kBool:
			{
				value.b = (val.value.b);
				break;
			}
			case ValueType::kInt:
			{
				value.i = (val.value.i);
				break;
			}
			case ValueType::kChar:
			{
				value.c = (val.value.c);
				break;
			}
			case ValueType::kUChar:
			{
				value.uc = (val.value.uc);
				break;
			}
			case ValueType::kUInt:
			{
				value.ui = (val.value.ui);
				break;
			}
			case ValueType::kLong:
			{
				value.l = (val.value.l);
				break;
			}
			case ValueType::kULong:
			{
				value.ul = (val.value.ul);
				break;
			}
			case ValueType::kLongLong:
			{
				value.ll = (val.value.ll);
				break;
			}
			case ValueType::kFloat:
			{
				value.f = (val.value.f);
				break;
			}
			case ValueType::kDouble:
			{
				value.d = (val.value.d);
				break;
			}
			case ValueType::kLongDouble:
			{
				value.ld = (val.value.ld);
				break;
			}
			case ValueType::kVector2d:
			{
				value.v2d = (val.value.v2d);
				break;
			}
			case ValueType::kVector3d:
			{
				value.v3d = (val.value.v3d);
				break;
			}
			case ValueType::kMatrix44:
			{
				value.mat = (val.value.mat);
				break;
			}
			case ValueType::kString:
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
	
	Value& operator=(const Value &val)
	{
		valueType = val.valueType;
		switch(valueType)
		{
			case ValueType::kBool:
			{
				value.b = (val.value.b);
				break;
			}
			case ValueType::kInt:
			{
				value.i = (val.value.i);
				break;
			}
			case ValueType::kChar:
			{
				value.c = (val.value.c);
				break;
			}
			case ValueType::kUChar:
			{
				value.uc = (val.value.uc);
				break;
			}
			case ValueType::kUInt:
			{
				value.ui = (val.value.ui);
				break;
			}
			case ValueType::kLong:
			{
				value.l = (val.value.l);
				break;
			}
			case ValueType::kULong:
			{
				value.ul = (val.value.ul);
				break;
			}
			case ValueType::kLongLong:
			{
				value.ll = (val.value.ll);
				break;
			}
			case ValueType::kFloat:
			{
				value.f = (val.value.f);
				break;
			}
			case ValueType::kDouble:
			{
				value.d = (val.value.d);
				break;
			}
			case ValueType::kLongDouble:
			{
				value.ld = (val.value.ld);
				break;
			}
			case ValueType::kVector2d:
			{
				value.v2d = (val.value.v2d);
				break;
			}
			case ValueType::kVector3d:
			{
				value.v3d = (val.value.v3d);
				break;
			}
			case ValueType::kMatrix44:
			{
				value.mat = (val.value.mat);
				break;
			}
			case ValueType::kString:
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
	
	Value(Value &&val)
		: valueType{std::move(val.valueType)}
	{
		switch(valueType)
		{
			case ValueType::kBool:
			{
				value.b = std::move(val.value.b);
				break;
			}
			case ValueType::kInt:
			{
				value.i = std::move(val.value.i);
				break;
			}
			case ValueType::kChar:
			{
				value.c = std::move(val.value.c);
				break;
			}
			case ValueType::kUChar:
			{
				value.uc = std::move(val.value.uc);
				break;
			}
			case ValueType::kUInt:
			{
				value.ui = std::move(val.value.ui);
				break;
			}
			case ValueType::kLong:
			{
				value.l = std::move(val.value.l);
				break;
			}
			case ValueType::kULong:
			{
				value.ul = std::move(val.value.ul);
				break;
			}
			case ValueType::kLongLong:
			{
				value.ll = std::move(val.value.ll);
				break;
			}
			case ValueType::kFloat:
			{
				value.f = std::move(val.value.f);
				break;
			}
			case ValueType::kDouble:
			{
				value.d = std::move(val.value.d);
				break;
			}
			case ValueType::kLongDouble:
			{
				value.ld = std::move(val.value.ld);
				break;
			}
			case ValueType::kVector2d:
			{
				value.v2d = std::move(val.value.v2d);
				break;
			}
			case ValueType::kVector3d:
			{
				value.v3d = std::move(val.value.v3d);
				break;
			}
			case ValueType::kMatrix44:
			{
				value.mat = std::move(val.value.mat);
				break;
			}
			case ValueType::kString:
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
	
	Value(bool b)
		: valueType{ValueType::kBool}
	{
		value.b = b;
	}

	Value(int i)
		: valueType{ValueType::kInt}
	{
		value.i = i;
	}

	Value(char c)
		: valueType{ValueType::kChar}
	{
		value.c = c;
	}

	Value(unsigned char uc)
		: valueType{ValueType::kUChar}
	{
		value.uc = uc;
	}

	Value(unsigned int ui)
		: valueType{ValueType::kUInt}
	{
		value.ui = ui;
	}

	Value(long l)
		: valueType{ValueType::kLong} 
	{
		value.l = l;
	}

	Value(unsigned long ul)
		: valueType{ValueType::kULong} 
	{
		value.ul = ul;
	}

	Value(long long ll)
		: valueType{ValueType::kLongLong} 
	{
		value.ll = ll;
	}

	Value(float f)
		: valueType{ValueType::kFloat} 
	{
		value.f = f;
	}

	Value(double d)
		: valueType{ValueType::kDouble} 
	{
		value.d = d;
	}

	Value(long double ld)
		: valueType{ValueType::kLongDouble} 
	{
		value.ld = ld;
	}

	Value(std::string str)
		: valueType{ValueType::kString}
		, value(str)
	{
	}

	Value(std::shared_ptr<v2s> v2d)
		: valueType{ValueType::kVector2d} 
	{
		value.v2d = v2d;
	}

	Value(std::shared_ptr<v3s> v3d)
		: valueType{ValueType::kVector3d} 
	{
		value.v3d = v3d;
	}
	
	Value(std::shared_ptr<Matrix44> m44)
		: valueType{ValueType::kMatrix44} 
	{
		value.mat = m44 ;
	}
	
	void setBool(bool other)
	{
		valueType = ValueType::kBool;
		value.b = other;
	}
	
	void setChar(char other)
	{
		valueType = ValueType::kChar;
		value.c = other;
	}
	
	void setUChar(unsigned char other)
	{
		valueType = ValueType::kBool;
		value.uc = other;
	}
	
	void setInt(int other)
	{
		valueType = ValueType::kInt;
		value.i = other;
	}
	
	void setUInt(unsigned int other)
	{
		valueType = ValueType::kUInt;
		value.ui = other;
	}
	
	void setLong(long other)
	{
		valueType = ValueType::kLong;
		value.l = other;
	}
	
	void setULong(unsigned long other)
	{
		valueType = ValueType::kULong;
		value.ul = other;
	}
	
	void setLongLong(long long other)
	{
		valueType = ValueType::kLongLong;
		value.ll = other;
	}
	
	void setFloat(float other)
	{
		valueType = ValueType::kFloat;
		value.f = other;
	}
	
	void setDouble(double other)
	{
		valueType = ValueType::kDouble;
		value.d = other;
	}
	
	void setLongDouble(long double other)
	{
		valueType = ValueType::kLongDouble;
		value.ld = other;
	}
	
	void setVector2d(std::shared_ptr<v2s> other)
	{
		valueType = ValueType::kVector2d;
		value.v2d = other;
	}
	
	void setVector3d(std::shared_ptr<v3s> other)
	{
		valueType = ValueType::kVector3d;
		value.v3d = other;
	}
	
	void setString(std::string other)
	{
		valueType = ValueType::kString;
		value.str = other;
	}
	
	void setMatrix44(std::shared_ptr<Matrix44> other)
	{
		valueType = ValueType::kMatrix44;
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
	
	std::shared_ptr<v2s> asVector2d() const
	{
		return value.v2d;
	}
	
	std::shared_ptr<v3s> asVector3d() const
	{
		return value.v3d;
	}
	
	std::string asString() const
	{
		return value.str;
	}
	
	std::shared_ptr<Matrix44> asMatrix44() const
	{
		return value.mat;
	}
	
	friend std::ostream& operator<<(std::ostream &os, const Value& v)
	{
		switch(v.valueType)
		{
			case ValueType::kBool:
			{
				os << (v.value.b ? "True" : "False");
				break;
			}
			case ValueType::kInt:
			{
				os << v.value.i;
				break;
			}
			case ValueType::kChar:
			{
				os << v.value.c;
				break;
			}
			case ValueType::kUChar:
			{
				os << v.value.uc;
				break;
			}
			case ValueType::kUInt:
			{
				os << v.value.ui;
				break;
			}
			case ValueType::kLong:
			{
				os << v.value.l;
				break;
			}
			case ValueType::kULong:
			{
				os << v.value.ul;
				break;
			}
			case ValueType::kLongLong:
			{
				os << v.value.ll;
				break;
			}
			case ValueType::kFloat:
			{
				os << v.value.f;
				break;
			}
			case ValueType::kDouble:
			{
				os << v.value.d;
				break;
			}
			case ValueType::kLongDouble:
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
		return valueType == ValueType::kInvalid;
	}
};

} // Namespace 
#endif // VXVALUE_H
