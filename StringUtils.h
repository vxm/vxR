#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <iostream>

#define SPACES_ARE_A_TAB 4

class StringUtils
{
public:
	StringUtils();

	//Returns indentation of a line, or in other words the tabs or
	// int division of SPACES_ARE_A_TAB in spaces or a combination of both.
	static unsigned int indentation(const std::string &line);
};

#endif // STRINGUTILS_H
