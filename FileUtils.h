#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "vxmanager.h"

class FileUtils : public vxManager
{
	public:
		FileUtils();

	static bool fileExists(const std::string &filePath)
	{
		return std::ifstream(filePath).good();
	}
	
	static std::string makeUnique(const std::string &fileName)
	{
		std::string unique = fileName;
		
		return unique;
	}
	
	
};

#endif // FileUtils_H
