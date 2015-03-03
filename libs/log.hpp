#pragma once

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

class CLog
{
private:
	CHTML html;
	int logger_file_initialized=false;
	int logger_file_finalized=false;
public:

	CLog(const std::string log_filename) : html(log_filename)
	{
		html.head_open();
		html.js_src(files::jquery);
		html.js_inline(files::fhs_table_js);
		html.style(files::fhs_table_style);
		html.head_close();
		html.body_open();
		html.h1("Log file");
		html.h2(application::printable_time());
		html.fhs_table_open(config::log_columns);
		logger_file_initialized=true;
	}

	~CLog()
	{
		logger_file_finalized=true;
		html.fhs_table_close();
		html.h2("Fixation list");
		html.insert_template(files::html_fixation_list_open);
		std::string fix_content=filesystem::read_file(files::fixation_list);
		const std::string option=filesystem::read_file(files::html_fixation_option);
		auto fix_vec=easytext::explode(fix_content,"\n*");
		for(const std::string fix:fix_vec)
			if(fix!="")
				html.insert_text(easytext::replace(option,"{option}",fix));
		html.insert_template(files::html_fixation_list_close);
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
		std::vector<std::string> log_row={filename,std::to_string(line),function,std::to_string(value),message};
		html.fhs_table_row(log_row);
	}

}logger(files::output_log);

void call_loger(std::string msg)
{	// this function is a forward declared version of logger
	logger(msg,0,0,"","");
}