#ifndef VXFILEMANAGER_H
#define VXFILEMANAGER_H

#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <vxmanager.h>

class vxFileManager : public vxManager
{
	public:
		vxFileManager();

	static bool fileExists(const std::string &filePath)
	{
		std::cout << "Request existence " << filePath << std::endl;
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
			auto number = boost::lexical_cast<int>(toks[1]);
			do
			{
				number++;
				std::stringstream uppedName;
				uppedName << toks[0] << ".";
				uppedName << std::setw( 7 ) << std::setfill( '0' ) << number << ".";
				uppedName << toks[2];
				ret = uppedName.str();
			}
			while(fileExists(ret));
		}
		
		return ret;
	}
	
	
};

#endif // VXFILEMANAGER_H
