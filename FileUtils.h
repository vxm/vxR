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

#include "vxmanager.h"

class FileUtils : public vxManager
{
	public:
		FileUtils();
	
	struct fileDesc
	{
		std::string name;
		std::string ext;
		unsigned int number;
	};

	static fileDesc splitFileName(const std::string &filePath);
	
	static bool fileExists(const std::string &filePath);
	
	static std::string makeUnique(const std::string &fileName);
	
	
};

#endif // FileUtils_H
