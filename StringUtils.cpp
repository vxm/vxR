#include "StringUtils.h"
using namespace vxCore;

StringUtils::StringUtils() {}
#include <locale>         // std::locale, std::isspace

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

        if (std::isspace(c,loc))
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
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, ' '))
    {
       ret.emplace_back(token);
    }
    return ret;
}
