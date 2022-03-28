#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#define SPACES_ARE_A_TAB 4

#include <vector>
#include <iostream>
#include <regex>

#include "Vector.h"

using namespace std::string_literals;
using std::regex;
using std::sregex_token_iterator;

namespace vxCore
{

class VectorUtils
{
public:
	VectorUtils() = default;

	//TODO::template the vector element type.
	static unsigned int sortAndUnique(std::vector<scalar> &vec)
	{
		std::sort(vec.begin(), vec.end());
		auto last = std::unique(vec.begin(), vec.end());
		vec.erase(last, vec.end());
		return vec.size();
	}
	
};

using VU = VectorUtils;

}
#endif // VECTORUTILS_H
