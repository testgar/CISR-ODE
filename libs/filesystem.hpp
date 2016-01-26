#pragma once

// bug problem of BOOST: http://stackoverflow.com/questions/15634114/c#17988317
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include <string>
#include <fstream>

void call_loger(std::string a);

namespace filesystem
{

	void ensure_folder(const boost::filesystem::path &filepath)
	{
		if(!boost::filesystem::exists(filepath))
			boost::filesystem::create_directory(filepath);	
	}

	bool exists(const boost::filesystem::path &filepath)
	{
		return boost::filesystem::exists(filepath);
	}

	std::string read_file(const boost::filesystem::path &filepath)
	{
		if(!boost::filesystem::exists(filepath))
			throw std::runtime_error(std::string("file \"")+filepath.string()+"\" does not exist!");
		std::string data;
		std::ifstream in(filepath.string().c_str());
		std::getline(in, data, std::string::traits_type::to_char_type(
						  std::string::traits_type::eof()));
		return data;
	}

	void write_file(const boost::filesystem::path &filepath, const std::string &text)
	{
		std::ofstream out(filepath.string().c_str());
		out << text;
		out.close();
	}

};
