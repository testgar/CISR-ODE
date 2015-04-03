#pragma once

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

void call_loger(std::string a);

namespace filesystem
{

	void ensure_folder(std::string filename)
	{
		if(!boost::filesystem::exists(filename))
			boost::filesystem::create_directory(filename);	
	}

	bool exists(std::string filename)
	{
		return boost::filesystem::exists(filename);
	}

	std::string read_file(const std::string &filename)
	{
		if(!boost::filesystem::exists(filename))
			throw std::runtime_error(std::string("file \"")+filename+"\" does not exist!");
		std::string data;
		std::ifstream in(filename.c_str());
		std::getline(in, data, std::string::traits_type::to_char_type(
						  std::string::traits_type::eof()));
		return data;
	}
};
