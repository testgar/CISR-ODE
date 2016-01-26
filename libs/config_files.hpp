#pragma once

#include <boost/filesystem.hpp>
#include "easytext.hpp"

#ifndef WEB_DIR
	#define WEB_DIR (boost::filesystem::path("resources")/"html")
#endif

namespace files
{
	typedef boost::filesystem::path path;
	const time_t now= time(0);
	const std::string now_print=easytext::format_time(now,"%04d/%02d/%02d %02d:%02d:%02d");
	const std::string now_file=easytext::format_time(now,"%04d%02d%02d_%02d%02d%02d");
	const char sep=boost::filesystem::path::preferred_separator;

	const path web_dir=WEB_DIR;
	const path html_open=web_dir/"html_open.html"; 
	const path html_close=web_dir/"html_close.html";
	const path html_head_open=web_dir/"head_open.html"; 
	const path html_head_close=web_dir/"head_close.html";
	const path html_body_open=web_dir/"body_open.html"; 
	const path html_body_close=web_dir/"body_close.html";
	const path fhs_table_open=web_dir/"fixed_header_scrolling"/"table_open.html";
	const path fhs_table_close=web_dir/"fixed_header_scrolling"/"table_close.html";
	const path fhs_thead_open=web_dir/"fixed_header_scrolling"/"thead_open.html";
	const path fhs_thead_close=web_dir/"fixed_header_scrolling"/"thead_close.html";
	const path fhs_thead_column=web_dir/"fixed_header_scrolling"/"thead_column.html";
	const path fhs_tbody_open=web_dir/"fixed_header_scrolling"/"tbody_open.html";
	const path fhs_tbody_close=web_dir/"fixed_header_scrolling"/"tbody_close.html";
	const path fhs_tbody_tr_open=web_dir/"fixed_header_scrolling"/"tbody_tr_open.html";
	const path fhs_tbody_tr_close=web_dir/"fixed_header_scrolling"/"tbody_tr_close.html";
	const path fhs_tbody_td=web_dir/"fixed_header_scrolling"/"tbody_td.html";
	const path fhs_table_style=web_dir/"fixed_header_scrolling"/"table2.css"; 
	const std::string fhs_table_js_basename("fhs_adjust.js");
	const std::string jquery_basename("jquery-1.11.2.min.js");
	const path jquery_src_path=web_dir/jquery_basename;
	const path fhs_table_js=web_dir/"fixed_header_scrolling"/fhs_table_js_basename;
	const path js_src_template=web_dir/"js_src.html";
	const path html_fixation_list_open=web_dir/"templates"/"fixation_list"/"select_open.html";
	const path html_fixation_list_close=web_dir/"templates"/"fixation_list"/"select_close.html";
	const path html_fixation_option=web_dir/"templates"/"fixation_list"/"option.html";
	const path fixation_list="fixationlist";
	const path html_h1=web_dir/"templates"/"h1.html";
	const path html_h2=web_dir/"templates"/"h2.html";

	const path output_folder("out"); 
	const path log_folder=output_folder; 
	const std::string output_prefix="data_";
	const std::string output_common_base=output_prefix+now_file+'_';
	// const path output_common=output_folder/output_common_base;
	// const path log_common=log_folder/output_common_base;
	const path output_results=output_folder/(output_common_base+"_results");
	const path output_log=output_folder/(output_common_base+"_log");
}