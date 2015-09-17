#include "StringUtils.h"

StringUtils::StringUtils()
{
}

unsigned int StringUtils::indentation(const std::string &line)
{
	auto n = 0u;

	for(auto c:line)
	{
		auto notFound{true};
		if(c=='\t')
		{
			n+=SPACES_ARE_A_TAB;
			notFound = false;
		}
		
		if(std::isspace(c))
		{
			n++;
			notFound = false;
		}
		
		if(notFound)
		{
			break;
		}
	}

	return n;
}
