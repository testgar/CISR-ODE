#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "config_files.hpp"
#include "filesystem.hpp"

class CHTML
{
private:
	typedef boost::filesystem::path path;
	path html_file_path;
	std::ofstream html_file;

	bool file_opened=false;
	bool file_closed=false;

	bool html_opened=false;
	bool html_closed=false;
	bool head_opened=false;
	bool head_closed=false;
	bool body_opened=false;
	bool body_closed=false;
	bool in_table=false;
	bool in_fhs_table=false;


	enum class html_locations{
			empty,
			body_ended,
			in_html,
			html_ended,
			before_head,
			in_head,
			head_ended,
			before_body,
			in_body,
			out_table,
			out_fhs_table,
			in_table,
			in_fhs_table
		};

public:

	CHTML(const path &filepath)
	{
		html_file_path=filepath;
		open(filepath);
		html_open();
	}

	~CHTML()
	{
		html_close();
		close();
	}

	void head_open()
	{
		ensure(html_locations::in_html,"Header must be inside HTML tag!");
		ensure(html_locations::before_head,"Cannot reinitiate head tag!");
		head_opened=true;
		insert_template(files::html_head_open);
	}

	void head_close()
	{
		ensure(html_locations::in_head,"Head tag close is not valid!");
		head_closed=true;
		insert_template(files::html_head_close);
	}

	void body_open()
	{
		ensure(html_locations::head_ended,"Body must be placed after head!");
		ensure(html_locations::before_body,"Cannot reinitiate body tag!");
		body_opened=true;
		insert_template(files::html_body_open);
	}

	void body_close()
	{
		ensure(html_locations::in_body,"Body tag close is not valid!");
		ensure(html_locations::out_table,"Cannot close body before closing the last table!");
		body_closed=true;
		insert_template(files::html_body_close);
	}

	void fhs_table_open(std::vector<std::string> columns)
	{	//	fixed header strolling table
		ensure(html_locations::in_body,"Table must be inside body!");
		ensure(html_locations::out_table,"The previous table is still open!");
		ensure(html_locations::out_fhs_table,"Table type mismatch for table open tag!");
		in_table=true;
		in_fhs_table=true;

		insert_template(files::fhs_table_open);
		insert_template(files::fhs_thead_open);
		// insert each header column
		for(const std::string citem : columns)
		    fhs_table_column(citem);

		insert_template(files::fhs_thead_close);
		insert_template(files::fhs_tbody_open);

	}

	void fhs_table_close()
	{
		ensure(html_locations::in_table,"Table close tag before table open!");
		ensure(html_locations::in_fhs_table,"Table type mismatch for table close tag!");
		in_table=false;
		in_fhs_table=false;
		insert_template(files::fhs_tbody_close);
		insert_template(files::fhs_table_close);
	}

	void fhs_table_row(const std::vector<std::string> cells)
	{
		ensure(html_locations::in_table,"Table row must be placed inside table body!");
		const static std::string row_open_text=filesystem::read_file(files::fhs_tbody_tr_open);
		const static std::string row_close_text=filesystem::read_file(files::fhs_tbody_tr_close);
		const static std::string cell_text=filesystem::read_file(files::fhs_tbody_td);

		insert_text(row_open_text);
		for(const std::string cell: cells)
			insert_text(easytext::replace_once(cell_text,"{cell}",cell));
		insert_text(row_close_text);
	}

	void style(boost::filesystem::path filepath)
	{
		open_tag("style");
		insert_text(filesystem::read_file(filepath));
		next_line();
		close_tag("style");
	}

	void js_src(std::string js_url)
	{
		ensure(html_locations::in_head,"Cannot link to script out of head!");
		static std::string sc_hmtl=filesystem::read_file(files::js_src_template);
		insert_text(easytext::replace_once(sc_hmtl,"{src}",js_url));
	}

	void js_inline(boost::filesystem::path filepath)
	{
		ensure(html_locations::in_head,"Cannot link to script out of head!");
		open_tag("script");
		insert_text(filesystem::read_file(filepath));
		next_line();
		close_tag("script");
	}

	void h1(std::string title)
	{
		ensure(html_locations::in_body,"Large pen is not valid out of body!");
		ensure(html_locations::out_table,"Large pen cannot be used inside table!");
		static std::string h1_t=filesystem::read_file(files::html_h1);
		insert_text(easytext::replace_once(h1_t,"{title}",title));
	}

	void h2(std::string title)
	{
		ensure(html_locations::in_body,"Large pen is not valid out of body!");
		ensure(html_locations::out_table,"Large pen cannot be used inside table!");
		static std::string h2_t=filesystem::read_file(files::html_h2);
		insert_text(easytext::replace_once(h2_t,"{title}",title));
	}

	void insert_text(std::string text)
	{
		html_file<<text;
	}

	void insert_template(boost::filesystem::path filepath)
	{
		std::string text=filesystem::read_file(filepath);
		insert_text(text);
	}

private:

	void open_tag(std::string tagname)
	{
		insert_text(std::string("<")+tagname+">\n");
	}

	void close_tag(std::string tagname)
	{
		insert_text(std::string("</")+tagname+">\n");
	}

	void next_line()
	{
		html_file<<"\n";
	}

	void fhs_table_column(std::string column)
	{
		const static std::string ct=filesystem::read_file(files::fhs_thead_column);
		insert_text(easytext::replace_once(ct,"{column_name}",column));
	}

	void html_open()
	{
		ensure_open();
		ensure(html_locations::empty,"Cannot reinitiate html tag!");
		html_opened=true;
		insert_template(files::html_open);
	}

	void html_close()
	{
		ensure(html_locations::body_ended,"Cannot close html tag before body!");
		html_closed=true;
		insert_template(files::html_close);
	}

	void open(const path filepath)
	{
		html_file.open(filepath.string());
		if(!html_file.is_open())
			throw std::runtime_error(
				std::string("cannot open file \"")+filepath.string()+"\" !");
		file_opened=true;
	}

	void close()
	{
		ensure_open();
		ensure(html_locations::html_ended,"Cannot close html file before closing html tag!");
		html_file.close();
		file_closed=true;
	}

	void ensure_open()
	{
		if(!file_opened)
			throw std::runtime_error(
				std::string("file ")+html_file_path.string()+" is not open yet!");
		if(file_closed)
			throw std::runtime_error(
				std::string("file ")+html_file_path.string()+" is already closed!");
	}

	void ensure(html_locations location,std::string message)
	{
		ensure_open();
		bool terminate;
		switch(location)
		{
			case html_locations::empty:
				terminate= html_opened;
				break;
			case html_locations::body_ended:
				terminate= !body_closed;
				break;
			case html_locations::in_html:
				terminate= !html_opened||html_closed;
				break;
			case html_locations::html_ended:
				terminate= !html_closed;
				break;
			case html_locations::before_head:
				terminate= head_opened;
				break;
			case html_locations::in_head:
				terminate= !head_opened||head_closed;
				break;
			case html_locations::head_ended:
				terminate= !head_closed;
				break;
			case html_locations::before_body:
				terminate= body_opened;
				break;
			case html_locations::in_body:
				terminate= !body_opened||body_closed;
				break;
			case html_locations::in_table:
				terminate= !in_table;
				break;
			case html_locations::out_table:
				terminate= in_table;
				break;
			case html_locations::in_fhs_table:
				terminate= !in_fhs_table;
				break;
			case html_locations::out_fhs_table:
				terminate= in_fhs_table;
				break;
			default:
				throw std::runtime_error("Not implemented!");
		}// end switch
		if(terminate)
			throw std::runtime_error(message);
	}

};
