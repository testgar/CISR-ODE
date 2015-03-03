namespace application
{
	std::string serialized_time()
	{
		static std::string m_serialized_time="";
		if(m_serialized_time=="")
		{
			time_t now = time(0);
			tm *ltm = localtime(&now);
			boost::format formatted_time("%04d%02d%02d_%02d%02d%02d");
			formatted_time % (1900+ltm->tm_year) % (1+ltm->tm_mon) % (ltm->tm_mday);
			formatted_time % ltm->tm_hour % ltm->tm_min % ltm->tm_sec;
			m_serialized_time=formatted_time.str();
		}
		return m_serialized_time;
	}

	std::string printable_time()
	{
		static std::string m_printable_time="";
		if(m_printable_time=="")
		{
			time_t now = time(0);
			tm *ltm = localtime(&now);
			boost::format formatted_time("%04d/%02d/%02d %02d:%02d:%02d");
			formatted_time % (1900+ltm->tm_year) % (1+ltm->tm_mon) % (ltm->tm_mday);
			formatted_time % ltm->tm_hour % ltm->tm_min % ltm->tm_sec;
			m_printable_time=formatted_time.str();
		}
		return m_printable_time;
	}
}

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
		boost::split(results,text,boost::is_any_of(delim),boost::token_compress_on);
		return results;
	}
}