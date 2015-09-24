#pragma once

#include <vector>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp> // for using boost::split to explode a string
// #include <boost/algorithm/string/predicate.hpp>

namespace easytext
{
	std::string replace(std::string text,
						std::string tofind,
						std::string toreplace)
	{
		size_t found_pos = text.find(tofind);
		if(found_pos == std::string::npos)
			return(text);
		return text.replace(found_pos,tofind.length(),toreplace);
	}

	std::vector<std::string> explode(std::string const & text, std::string const delim)
	{
		std::vector<std::string> results;
		boost::split(results,text,boost::is_any_of(delim),boost::token_compress_off);
		return results;
	}

	std::string format_time(time_t time, std::string format)
	{
		tm *ltm = localtime(&time);
		boost::format formatted_time(format);
		formatted_time % (1900+ltm->tm_year) % (1+ltm->tm_mon) % (ltm->tm_mday);
		formatted_time % ltm->tm_hour % ltm->tm_min % ltm->tm_sec;
		return formatted_time.str();
	}

	// search for a string in a vector
	bool is_string_in(std::string text, std::vector<std::string> vec)
	{
		return (std::find(vec.begin(), vec.end(), text) != vec.end());
	}

	std::string get_indentation(std::string row)
	{
		const std::string delims(" \t");
		std::size_t pos = row.find_first_not_of(delims);
		return row.substr(0,pos);
	}

	std::string get_after_indentation(std::string row)
	{
		const std::string delims(" \t");
		std::size_t pos = row.find_first_not_of(delims);
		return row.substr(pos);
	}

	bool starts_with(std::string text, std::string item)
	{
		return boost::starts_with(text, item);
	}
}