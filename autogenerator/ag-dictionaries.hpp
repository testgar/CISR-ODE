#pragma once

#include <boost/filesystem.hpp>
#include <map>
#include "ag-vectors.hpp"

class AG_Dictionary
{
public:

	std::map<std::string,std::string> dictionary;
	AG_Dictionary(boost::filesystem::path filepath) : dictionary()
	{
		dictionary["#WARNING_COMMENTS#"]=line_prefix("// *** ","Warning:"+easytext::newline+"auto-generated file and subjected to be overwritten");
		dictionary["#TRACING_PATH#"]=filepath.string();
	}

	std::string line_prefix(std::string prefix,std::string text)
	{
		std::string indentation=easytext::get_indentation(text);
		std::vector<std::string> rows=easytext::explode(text,"\n");
		std::string ret("");
		int index=0;
		for(auto row : rows)
		{
			std::string trimed_row(row);
			boost::algorithm::trim(trimed_row);
			ret+=indentation+(index++?easytext::newline:"")+prefix+trimed_row;
		}
		return ret;
	}

	std::string commented(std::string text)
	{
		return line_prefix("// ",text);
	}

	std::string replace_dictionary(std::string text)
	{
		std::vector<std::string> rows=easytext::explode(text,"\n");
		std::string ret("");
		int original_row_index=0;
		for(auto row : rows)
		{
			std::string row_indentation=easytext::get_indentation(row);
			std::string trimed_row(row);
			boost::algorithm::trim(trimed_row);
			for(auto map_item : dictionary)
				trimed_row=easytext::replace_once(trimed_row,map_item.first,map_item.second);
			trimed_row=line_prefix(row_indentation,trimed_row);
			ret+=(original_row_index++?easytext::newline:"")+trimed_row;
		}
		return ret;
	}

};

class AG_Dictionary_input : public AG_Dictionary
{
public:
	AG_Dictionary_input() : AG_Dictionary(ag_config.autogen_inputs)
	{
		dictionary["#TITLE#"]="inputs";
		AG_Vector vec_inputs(ag_config.list_inputs);
		dictionary["#INDEX_NUMBERS_COMMENTS#"]=vec_inputs.index_explanations("u");
		dictionary["#INDEX_CONSTANTS#"]=vec_inputs.index_constants_definitions();
		dictionary["#TOTAL_SIZE#"]=std::string("input_size=")+easytext::to_string(vec_inputs.size());
	}
};

class AG_Dictionary_mid : public AG_Dictionary
{
public:
	AG_Dictionary_mid() : AG_Dictionary(ag_config.autogen_intermediates)
	{
		dictionary["#TITLE#"]="mids";
		AG_Vector vec_mids(ag_config.list_intermediates);
		dictionary["#INDEX_NUMBERS_COMMENTS#"]=vec_mids.index_explanations("m");
		dictionary["#INDEX_CONSTANTS#"]=vec_mids.index_constants_definitions();
		dictionary["#TOTAL_SIZE#"]=std::string("intermediate_size=")+easytext::to_string(vec_mids.size());
	}
};

class AG_Dictionary_state : public AG_Dictionary
{
public:
	AG_Dictionary_state() : AG_Dictionary(ag_config.autogen_states)
	{
		dictionary["#TITLE#"]="states";
		AG_Vector vec_states(ag_config.list_states);
		dictionary["#INDEX_NUMBERS_COMMENTS#"]=vec_states.index_explanations("x");
		dictionary["#INDEX_CONSTANTS#"]=vec_states.index_constants_definitions();
		dictionary["#TOTAL_SIZE#"]=std::string("state_size=")+easytext::to_string(vec_states.size());
	}
};

class AG_Dictionary_output : public AG_Dictionary
{
public:
	AG_Dictionary_output() : AG_Dictionary(ag_config.autogen_outputs)
	{
		dictionary["#TITLE#"]="outputs";

		AG_Vector vec_mids(ag_config.list_intermediates);
		dictionary["#INDEX_NUMBERS_COMMENTS#"]=vec_mids.output_index_explanations();
		dictionary["#INDEX_CONSTANTS#"]=vec_mids.output_index_constants_definitions();
		dictionary["#TOTAL_SIZE#"]=std::string("output_size=")+easytext::to_string(vec_mids.output_size());
		dictionary["#FIGURE_LIST#"]=vec_mids.export_figures_list();
		dictionary["#OUTPUTHEADER#"]=vec_mids.output_header();
	}
};

class AG_Dictionary_observer : public AG_Dictionary
{
public:
	AG_Dictionary_observer() : AG_Dictionary(ag_config.autogen_observer)
	{
		AG_Vector vec_mids(ag_config.list_intermediates);
		dictionary["#INDEX_NUMBERS_COMMENTS#"]=vec_mids.output_index_explanations();
		dictionary["#OUTPUT_MATRIX#"]=vec_mids.output_matrix();
	}
};

class AG_Dictionary_usertemplate : public AG_Dictionary
{
public:
	AG_Dictionary_usertemplate() : AG_Dictionary(ag_config.autogen_usertemplate)
	{
		AG_Vector vec_states(ag_config.list_states);
		AG_Vector vec_mids(ag_config.list_intermediates);
		AG_Vector vec_inputs(ag_config.list_inputs);

		dictionary["#INDEX_NUMBERS_COMMENTS_INPUTS#"]=vec_inputs.index_explanations("u");
		dictionary["#INDEX_NUMBERS_COMMENTS_STATES#"]=vec_states.index_explanations("x");
		dictionary["#INDEX_NUMBERS_COMMENTS_MIDS#"]=vec_mids.index_explanations("m");

		dictionary["#INPUTS_EQ_DOTS#"]=vec_inputs.indices_eq_dots("u","inputs");
		dictionary["#MIDS_EQ_DOTS#"]=vec_mids.indices_eq_dots("mid","mids");
		dictionary["#STATES_EQ_DOTS#"]=vec_states.indices_eq_dots("x_dot","states");

	}
};

class AG_Dictionary_embed : public AG_Dictionary
{
public:
	AG_Dictionary_embed(std::string modelname) : AG_Dictionary(ag_config.autogen_embed)
	{
		boost::filesystem::path model_root("");
		model_root=model_root/".."/".."/"app_models"/modelname/"model.hpp";
		dictionary["#MODEL_ENTRANCE_PATH#"]="\""+model_root.string()+"\"";
	}
};
