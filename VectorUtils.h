#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#define SPACES_ARE_A_TAB 4

#include <vector>
#include <iostream>
#include <regex>

#include "vxVector.h"

using namespace std::string_literals;
using std::regex;
using std::sregex_token_iterator;

namespace vxCore
{

class VectorUtils
{
public:
	VectorUtils();

	//TODO::template the vector element type.
	static unsigned int sortAndUnique(std::vector<scalar> &v)
	{
		std::sort(v.begin(), v.end());
		auto last = std::unique(v.begin(), v.end());
		v.erase(last, v.end());
		
		return v.size();
	}
	
};

using VU = VectorUtils;

}
#endif // VECTORUTILS_H
