#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <vxmanager.h>

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
		if(!fileExists(unique))
		{
			//was unique
			return unique;
		}
		
		std::vector<std::string> toks;
		boost::char_separator<char> sep(".");
		boost::tokenizer<boost::char_separator<char>> tokens(fileName, sep);
		for (const auto& t : tokens) 
		{
			toks.push_back(t);
		}
		
		if(toks.size()==3)
		{
			auto number = boost::lexical_cast<int>(toks[1]);
			do
			{
				number++;
				std::stringstream uppedName;
				uppedName << toks[0] << ".";
				uppedName << std::setw( 7 ) << std::setfill( '0' ) << number << ".";
				uppedName << toks[2];
				unique = uppedName.str();
			}
			while(fileExists(unique));
		}
		
		std::cout << "Made unique: " << unique << std::endl;
		
		return unique;
	}
	
	
};

#endif // FileUtils_H
