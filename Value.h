#ifndef VXVALUE_H
#define VXVALUE_H

#include <memory>
#include <string>
#include <variant>

#include "Matrix44.h"
#include "Vector.h"

namespace vxCore
{

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

	std::variant<bool, char, unsigned char, int, unsigned int, long,
	             unsigned long, long long, float, double, long double,
	             std::string, Matrix44, v3s, v2s>
	    value;

public:
	Value() : valueType{ValueType::kInvalid} {}

	~Value() {}

	Value(bool b) : valueType{ValueType::kBool} { value = b; }

	Value(int i) : valueType{ValueType::kInt} { value = i; }

	Value(char c) : valueType{ValueType::kChar} { value = c; }

	Value(unsigned char uc) : valueType{ValueType::kUChar} { value = uc; }

	Value(unsigned int ui) : valueType{ValueType::kUInt} { value = ui; }

	Value(long l) : valueType{ValueType::kLong} { value = l; }

	Value(unsigned long ul) : valueType{ValueType::kULong} { value = ul; }

	Value(long long ll) : valueType{ValueType::kLongLong} { value = ll; }

	Value(float f) : valueType{ValueType::kFloat} { value = f; }

	Value(double d) : valueType{ValueType::kDouble} { value = d; }

	Value(long double ld) : valueType{ValueType::kLongDouble} { value = ld; }

	Value(std::string str) : valueType{ValueType::kString}, value(str) {}

	Value(const v2s &v2d) : valueType{ValueType::kVector2d} { value = v2d; }

	Value(const v3s &v3d) : valueType{ValueType::kVector3d} { value = v3d; }

	Value(const Matrix44 &m44) : valueType{ValueType::kMatrix44} { value = m44; }

	void setBool(bool other)
	{
		valueType = ValueType::kBool;
		value = other;
	}

	void setChar(char other)
	{
		valueType = ValueType::kChar;
		value = other;
	}

	void setUChar(unsigned char other)
	{
		valueType = ValueType::kBool;
		value = other;
	}

	void setInt(int other)
	{
		valueType = ValueType::kInt;
		value = other;
	}

	void setUInt(unsigned int other)
	{
		valueType = ValueType::kUInt;
		value = other;
	}

	void setLong(long other)
	{
		valueType = ValueType::kLong;
		value = other;
	}

	void setULong(unsigned long other)
	{
		valueType = ValueType::kULong;
		value = other;
	}

	void setLongLong(long long other)
	{
		valueType = ValueType::kLongLong;
		value = other;
	}

	void setFloat(float other)
	{
		valueType = ValueType::kFloat;
		value = other;
	}

	void setDouble(double other)
	{
		valueType = ValueType::kDouble;
		value = other;
	}

	void setLongDouble(long double other)
	{
		valueType = ValueType::kLongDouble;
		value = other;
	}

	void setVector2d(const v2s &other)
	{
		valueType = ValueType::kVector2d;
		value = other;
	}

	void setVector3d(const v3s &other)
	{
		valueType = ValueType::kVector3d;
		value = other;
	}

	void setString(const std::string &other)
	{
		valueType = ValueType::kString;
		value = other;
	}

	void setMatrix44(const Matrix44 &other)
	{
		valueType = ValueType::kMatrix44;
		value = other;
	}

	bool asBool() const { return std::get<bool>(value); }

	char asChar() const { return std::get<char>(value); }

	unsigned char asUChar() const { return std::get<unsigned char>(value); }

	int asInt() const { return std::get<int>(value); }

	unsigned int asUInt() const { return std::get<unsigned int>(value); }

	long asLong() const { return std::get<long>(value); }

	unsigned long asULong() const { return std::get<unsigned long>(value); }

	long long asLongLong() const { return std::get<long long>(value); }

	float asFloat() const { return std::get<float>(value); }

	double asDouble() const { return std::get<double>(value); }

	long double asLongDouble() const { return std::get<long double>(value); }

	v2s asVector2d() const { return std::get<v2s>(value); }

	v3s asVector3d() const { return std::get<v3s>(value); }

	std::string asString() const { return std::get<std::string>(value); }

	Matrix44 asMatrix44() const { return std::get<Matrix44>(value); }

	friend std::ostream &operator<<(std::ostream &os, const Value &v)
	{
		switch (v.valueType)
		{
		case ValueType::kBool:
		{
			os << (v.asBool() ? "True" : "False");
			break;
		}
		case ValueType::kInt:
		{
			os << v.asInt();
			break;
		}
		case ValueType::kChar:
		{
			os << v.asChar();
			break;
		}
		case ValueType::kUChar:
		{
			os << v.asUChar();
			break;
		}
		case ValueType::kUInt:
		{
			os << v.asUInt();
			break;
		}
		case ValueType::kLong:
		{
			os << v.asLong();
			break;
		}
		case ValueType::kULong:
		{
			os << v.asULong();
			break;
		}
		case ValueType::kLongLong:
		{
			os << v.asLongLong();
			break;
		}
		case ValueType::kFloat:
		{
			os << v.asFloat();
			break;
		}
		case ValueType::kDouble:
		{
			os << v.asDouble();
			break;
		}
		case ValueType::kLongDouble:
		{
			os << v.asLongDouble();
			break;
		}
		default:
		{
			os << v.asInt();
			break;
		}
		}

		return os;
	}

	bool isInvalid() const { return valueType == ValueType::kInvalid; }
};

} // Namespace
#endif // VXVALUE_H
