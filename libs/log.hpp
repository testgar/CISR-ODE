#pragma once

#include "libs_config.hpp"
#include "html.hpp"
#include <boost/lexical_cast.hpp>

#define LOG2(Message_,VALUE_)					\
  logger(										\
	Message_,									\
	VALUE_,										\
	__LINE__,									\
	__FUNCTION__,								\
	__FILE__									\
  )

#define LOG1(Message_)							\
  logger(										\
	Message_,									\
	__LINE__,									\
	__FUNCTION__,								\
	__FILE__									\
  )

#define GET_LOG_MACRO(_1,_2,NAME,...) NAME
#define LOG(...) GET_LOG_MACRO(__VA_ARGS__, LOG2, LOG1)(__VA_ARGS__)

#define _(x) LOG(#x,x)

#define _PROFILE_ONCE_START 						\
				{									\
					static cronometer clock_tmp; 	\
					clock_tmp.tic_once();

#define _PROFILE_ONCE_END(STRING) 						\
					clock_tmp.print_toc_once(STRING); 	\
				}

#define _COUNT_ME(TEXT) \
				class COUNTER_TMP{int _count;std::string text; public: COUNTER_TMP(std::string text):_count(0),text(text){} ~COUNTER_TMP(){std::cout<<TEXT<<" counted "<<_count<<" times."<<std::endl;} void count(){_count++;}}; static COUNTER_TMP counted_tmp(TEXT); counted_tmp.count();

class CLog
{
private:
	CHTML html;
	int logger_file_initialized=false;
	int logger_file_finalized=false;
	const std::vector<std::string> log_columns=
			{"File","Line","Function","Value","Message"};

public:

	CLog(const boost::filesystem::path log_path) 
		: html((filesystem::ensure_folder(files::log_folder),log_path))
	{
		html.head_open();
		// html.js_src(files::jquery_basename);
		html.js_inline(files::jquery_src_path);
		html.js_inline(files::fhs_table_js);
		html.style(files::fhs_table_style);
		html.head_close();
		html.body_open();
		html.h1("Log file");
		html.h2(files::now_print);
		html.fhs_table_open(log_columns);
		logger_file_initialized=true;
	}

	~CLog()
	{
		logger_file_finalized=true;
		html.fhs_table_close();
		html.h2("Fixation list");
		if(filesystem::exists(files::fixation_list))
		{
			html.insert_template(files::html_fixation_list_open);
			std::string fix_content=filesystem::read_file(files::fixation_list);
			const std::string option=filesystem::read_file(files::html_fixation_option);
			auto fix_vec=easytext::explode(fix_content,"\n*");
			for(const std::string fix:fix_vec)
				if(fix!="")
					html.insert_text(easytext::replace_once(option,"{option}",fix));
			html.insert_template(files::html_fixation_list_close);
		}
		else
		{
			html.insert_text(std::string("(fixation file does not exist: ")+files::fixation_list.string()+")");
		}
		html.body_close();
	}

	template<class valuetype>
	void operator()(
		std::string message,
		valuetype value,
		int line,
		std::string function,
		std::string filename
		)
	{
		if(!config::debugmod)
			throw std::runtime_error("calling log function is possible only in debug mod!");
		if(!logger_file_initialized)
		{
			std::cout<<"logger file not initialized yet! message="<<message<<std::endl;
			return ;
		}
		if(logger_file_finalized)
		{
			std::cout<<"logger file already closed! message="<<message<<std::endl;
			return ;
		}
		std::vector<std::string> log_row={filename,easytext::to_string(line),function,boost::lexical_cast<std::string>(value),message};
		html.fhs_table_row(log_row);
	}

}logger(files::output_log);

void call_loger(std::string msg)
{	// this function is a forward declared version of logger
	logger(msg,0,0,"","");
}

