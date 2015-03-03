#pragma once

void call_loger(std::string a);

namespace filesystem
{

	void ensure_folder(std::string filename)
	{
		if(!boost::filesystem::exists(filename))
			boost::filesystem::create_directory(filename);	
	}

	// std::string base_filename()
	// {
	// 	char separator=boost::filesystem::path::preferred_separator;
	// 	std::string output_file=files::output_common;
	// 	return (files::output_folder+separator+output_file);
	// }

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
