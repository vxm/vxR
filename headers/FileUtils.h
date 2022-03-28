#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <regex>

#include "Vector.h"
#include "Manager.h"

namespace vxCore {


class FileUtils : public vxManager
{
	public:
		FileUtils() = default;
	
	struct fileDesc
	{
		std::string name;
		std::string ext;
		unsigned int number;
	};

	static fileDesc splitFileName(const std::string &filePath);
	
	static bool fileExists(const std::string &filePath);
	
	static std::string makeUnique(const std::string &fileName, scalar suffix=0.0);

	static std::string removeLastFileOnRoute(std::string &str);

	static std::string path;
};

}
#endif // FileUtils_H
