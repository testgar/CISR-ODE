#pragma once

#include <boost/filesystem.hpp>
#include "easytext.hpp"

#ifndef WEB_DIR
	#define WEB_DIR "resources/html"
#endif

namespace files
{
	const time_t now= time(0);
	const std::string now_print=easytext::format_time(now,"%04d/%02d/%02d %02d:%02d:%02d");
	const std::string now_file=easytext::format_time(now,"%04d%02d%02d_%02d%02d%02d");

	const std::string web_dir=std::string(WEB_DIR)+"/";
	const std::string html_open=web_dir+"html_open.html"; 
	const std::string html_close=web_dir+"html_close.html";
	const std::string html_head_open=web_dir+"head_open.html"; 
	const std::string html_head_close=web_dir+"head_close.html";
	const std::string html_body_open=web_dir+"body_open.html"; 
	const std::string html_body_close=web_dir+"body_close.html";
	const std::string fhs_table_open=web_dir+"fixed_header_scrolling/table_open.html";
	const std::string fhs_table_close=web_dir+"fixed_header_scrolling/table_close.html";
	const std::string fhs_thead_open=web_dir+"fixed_header_scrolling/thead_open.html";
	const std::string fhs_thead_close=web_dir+"fixed_header_scrolling/thead_close.html";
	const std::string fhs_thead_column=web_dir+"fixed_header_scrolling/thead_column.html";
	const std::string fhs_tbody_open=web_dir+"fixed_header_scrolling/tbody_open.html";
	const std::string fhs_tbody_close=web_dir+"fixed_header_scrolling/tbody_close.html";
	const std::string fhs_tbody_tr_open=web_dir+"fixed_header_scrolling/tbody_tr_open.html";
	const std::string fhs_tbody_tr_close=web_dir+"fixed_header_scrolling/tbody_tr_close.html";
	const std::string fhs_tbody_td=web_dir+"fixed_header_scrolling/tbody_td.html";
	const std::string fhs_table_style=web_dir+"fixed_header_scrolling/table2.css"; 
	const std::string fhs_table_js=web_dir+"fixed_header_scrolling/fhs_adjust.js";
	const std::string jquery="jquery-1.11.2.min.js";
	const std::string js_src=web_dir+"js_src.html";
	const std::string html_fixation_list_open=web_dir+"templates/fixation_list/select_open.html";
	const std::string html_fixation_list_close=web_dir+"templates/fixation_list/select_close.html";
	const std::string html_fixation_option=web_dir+"templates/fixation_list/option.html";
	const std::string fixation_list="fixationlist";
	const std::string html_h1=web_dir+"templates/h1.html";
	const std::string html_h2=web_dir+"templates/h2.html";

	const std::string output_folder="out"; 
	const std::string log_folder=output_folder; 
	const std::string output_prefix="data_";
	const char sep=boost::filesystem::path::preferred_separator;
	// const std::string serialized_time=serialized_time_filetype_func();
	const std::string output_common_base=output_prefix+now_file+'_';
	const std::string output_common=output_folder+sep+output_common_base;
	const std::string log_common=log_folder+sep+output_common_base;
	const std::string output_results=output_common+"_results";
	const std::string output_log=output_common+"_log";
}