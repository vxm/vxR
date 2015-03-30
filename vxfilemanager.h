#ifndef VXFILEMANAGER_H
#define VXFILEMANAGER_H

#include <vxmanager.h>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

class vxFileManager : public vxManager
{
	public:
		vxFileManager();

	static bool fileExists(std::string &filePath)
	{
		return std::ifstream(filePath).good();
	}
	
	static std::string makeUnique(const std::string &fileName)
	{
		std::string ret = fileName;
		if(!fileExists(ret))
		{
			//was unique
			return ret;
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
			std::stringstream uppedName;
			auto number = boost::lexical_cast<int>(toks[1]);
			std::cout << "number is " << number << std::endl;
			number++;
			uppedName << toks[0] << ".";
			uppedName << number << ".";
			uppedName << toks[2];
			ret = uppedName.str();
		}
		std::cout << "returning " << ret << std::endl;
		
		return ret;
	}
	
	
};

#endif // VXFILEMANAGER_H
