#include "StringUtils.h"
using namespace vxCore;

StringUtils::StringUtils() {}

unsigned int StringUtils::indentation(const std::string &line)
{
	auto n = 0u;
    std::locale loc;

	for (const auto c : line)
	{
		auto notFound{true};
		if (c == '\t')
		{
			n += SPACES_ARE_A_TAB;
			notFound = false;
		}

        if (std::isspace(c, loc))
		{
			n++;
			notFound = false;
		}

		if (notFound)
		{
			break;
		}
	}

	return n;
}

std::vector<std::string> StringUtils::tokenizeSpace(const std::string &str)
{
    std::vector<std::string> ret;
	sregex_token_iterator it(str.begin(), str.end(), spaceDelimiter, -1);

	sregex_token_iterator reg_end;
	for (; it != reg_end; ++it)
	{
		ret.emplace_back(it->str());
	}

    return ret;
}
