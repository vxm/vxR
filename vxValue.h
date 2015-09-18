#ifndef VXVALUE_H
#define VXVALUE_H

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
	
};

#endif // VXVALUE_H
