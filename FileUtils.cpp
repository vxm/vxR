#include "FileUtils.h"

using namespace vxCore;

FileUtils::FileUtils()
{
}


FileUtils::fileDesc FileUtils::splitFileName(const std::string &filePath)
{
	fileDesc desc;
	std::regex rel("((?:[a-z][a-z]+)).*?(\\d+).*?((?:[a-z][a-z]+))");
	
	// Extraction of a sub-match
	std::smatch base_match;
	
	if (std::regex_match(filePath, base_match, rel))
	{
		// The first sub_match is the whole string; the next
		// sub_match is the first parenthesized expression.
		if (base_match.size() == 4)
		{
			std::ssub_match base_sub_match = base_match[1];
			std::string base = base_sub_match.str();
			desc.name = base;
			
			base_sub_match = base_match[2];
			desc.number = strtol(base_sub_match.str().c_str(), 
								 (char **)NULL, 
								 10);
			
			base_sub_match = base_match[3];
			desc.ext = base_sub_match.str();
		}
	}
	
	return desc;
}

bool FileUtils::fileExists(const std::string &filePath)
{
	return std::ifstream(filePath).good();
}

std::string FileUtils::makeUnique(const std::string &fileName, scalar suffix)
{
	std::string unique = fileName;
	auto desc = FileUtils::splitFileName(fileName);
	while(fileExists(unique))
	{
		std::stringstream ss;
		desc.number++;
		ss << desc.name << "."
		 << std::setfill('0') 
		 << std::setw(7) 
		 << desc.number
		 << "."
		 << std::setfill('0') 
		 << std::setw(4) 
		 << (unsigned int)suffix
		 << "."
		 << desc.ext;
		unique = ss.str();
	}
	
	return unique;
}
