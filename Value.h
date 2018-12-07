#ifndef VXVALUE_H
#define VXVALUE_H

#include <memory>
#include <string>
#include <variant>

#include "Matrix44.h"
#include "Vector.h"

namespace vxCore {

enum class ValueType {
  kBool,
  kChar,
  kUChar,
  kInt,
  kUInt,
  kLong,
  kULong,
  kLongLong,
  kScalar,
  kVector2d,
  kVector3d,
  kString,
  kMatrix44,
  kInvalid
};

class Value {

public:
  static Value invalid;
  ValueType valueType;

  bool bvalue;

  char cvalue;

  unsigned char ucvalue;

  int ivalue;

  unsigned int uivalue;

  long lvalue;

  unsigned long ulvalue;

  long long llvalue;

  scalar scalarValue;

  v2s v2svalue;

  v3s v3svalue;

  std::string svalue;

  Matrix44 Matrix44Value;

public:
  Value() : valueType{ValueType::kInvalid} {}

  ~Value() {}

  Value(bool b) : valueType{ValueType::kBool} { bvalue = b; }

  Value(int i) : valueType{ValueType::kInt} { ivalue = i; }

  Value(char c) : valueType{ValueType::kChar} { cvalue = c; }

  Value(unsigned char uc) : valueType{ValueType::kUChar} { ucvalue = uc; }

  Value(unsigned int ui) : valueType{ValueType::kUInt} { uivalue = ui; }

  Value(long l) : valueType{ValueType::kLong} { lvalue = l; }

  Value(unsigned long ul) : valueType{ValueType::kULong} { ulvalue = ul; }

  Value(long long ll) : valueType{ValueType::kLongLong} { llvalue = ll; }

  Value(float f) : valueType{ValueType::kScalar} { scalarValue = f; }

  Value(double d) : valueType{ValueType::kScalar} { scalarValue = d; }

  Value(long double ld) : valueType{ValueType::kScalar} { scalarValue = ld; }

  Value(std::string str) : valueType{ValueType::kString}, svalue(str) {}

  Value(const v2s &v2d) : valueType{ValueType::kVector2d} { v2svalue = v2d; }

  Value(const v3s &v3d) : valueType{ValueType::kVector3d} { v3svalue = v3d; }

  Value(const Matrix44 &m44) : valueType{ValueType::kMatrix44} {
	Matrix44Value = m44;
  }

  void setBool(bool other) {
	valueType = ValueType::kBool;
	bvalue = other;
  }

  void setChar(char other) {
	valueType = ValueType::kChar;
	cvalue = other;
  }

  void setUChar(unsigned char other) {
	valueType = ValueType::kBool;
	ucvalue = other;
  }

  void setInt(int other) {
	valueType = ValueType::kInt;
	ivalue = other;
  }

  void setUInt(unsigned int other) {
	valueType = ValueType::kUInt;
	uivalue = other;
  }

  void setLong(long other) {
	valueType = ValueType::kLong;
	lvalue = other;
  }

  void setULong(unsigned long other) {
	valueType = ValueType::kULong;
	ulvalue = other;
  }

  void setLongLong(long long other) {
	valueType = ValueType::kLongLong;
	llvalue = other;
  }

  void setScalar(scalar other) {
	valueType = ValueType::kScalar;
	scalarValue = other;
  }

  void setFloat(float other) {
	valueType = ValueType::kScalar;
	scalarValue = other;
  }

  void setDouble(double other) {
	valueType = ValueType::kScalar;
	scalarValue = other;
  }

  void setLongDouble(long double other) {
	valueType = ValueType::kScalar;
	scalarValue = other;
  }

  void setVector2d(const v2s &other) {
	valueType = ValueType::kVector2d;
	v2svalue = other;
  }

  void setVector3d(const v3s &other) {
	valueType = ValueType::kVector3d;
	v3svalue = other;
  }

  void setString(const std::string &other) {
	valueType = ValueType::kString;
	svalue = other;
  }

  void setMatrix44(const Matrix44 &other) {
	valueType = ValueType::kMatrix44;
	Matrix44Value = other;
  }

  bool asBool() const {
	assert(valueType == ValueType::kBool);
	return bvalue;
  }

  char asChar() const {
	assert(valueType == ValueType::kChar);
	return cvalue;
  }

  unsigned char asUChar() const {
	assert(valueType == ValueType::kUChar);
	return ucvalue;
  }

  int asInt() const {
	assert(valueType == ValueType::kInt);
	return ivalue;
  }

  unsigned int asUInt() const {
	assert(valueType == ValueType::kUInt);
	return uivalue;
  }

  long asLong() const {
	assert(valueType == ValueType::kLong);
	return lvalue;
  }

  unsigned long asULong() const {
	assert(valueType == ValueType::kULong);
	return ulvalue;
  }

  long long asLongLong() const {
	assert(valueType == ValueType::kLongLong);
	return llvalue;
  }

  float asFloat() const {
	assert(valueType == ValueType::kScalar);
	return scalarValue;
  }

  scalar asScalar() const {
	assert(valueType == ValueType::kScalar);
	return scalarValue;
  }

  double asDouble() const {
	assert(valueType == ValueType::kScalar);
	return scalarValue;
  }

  long double asLongDouble() const {
	assert(valueType == ValueType::kScalar);
	return scalarValue;
  }

  v2s asVector2d() const {
	assert(valueType == ValueType::kVector2d);
	return v2svalue;
  }

  v3s asVector3d() const {
	assert(valueType == ValueType::kVector3d);
	return v3svalue;
  }

  std::string asString() const {
	assert(valueType == ValueType::kString);
	return svalue;
  }

  Matrix44 asMatrix44() const {
	assert(valueType == ValueType::kMatrix44);
	return Matrix44Value;
  }

  friend std::ostream &operator<<(std::ostream &os, const Value &v) {
	switch (v.valueType) {
	case ValueType::kBool: {
	  os << (v.asBool() ? "True" : "False");
	  break;
	}
	case ValueType::kInt: {
	  os << v.asInt();
	  break;
	}
	case ValueType::kChar: {
	  os << v.asChar();
	  break;
	}
	case ValueType::kUChar: {
	  os << v.asUChar();
	  break;
	}
	case ValueType::kUInt: {
	  os << v.asUInt();
	  break;
	}
	case ValueType::kLong: {
	  os << v.asLong();
	  break;
	}
	case ValueType::kULong: {
	  os << v.asULong();
	  break;
	}
	case ValueType::kLongLong: {
	  os << v.asLongLong();
	  break;
	}
	case ValueType::kScalar: {
	  os << v.asScalar();
	  break;
	}
	default: {
	  os << v.asInt();
	  break;
	}
	}

	return os;
  }

  bool isInvalid() const { return valueType == ValueType::kInvalid; }
};

} // namespace vxCore
#endif // VXVALUE_H
