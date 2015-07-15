#pragma once

#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include "../libs/filesystem.hpp"
#include "../libs/easytext.hpp"

class AG_Index_Properties
{
public:

	std::string name;
	bool plot;
	bool empty;

	AG_Index_Properties() :
		name(""),
		plot(false),
		empty(false)
	{
	}

	// This constructor is designed for generation of inedx properties for being ignored
	AG_Index_Properties(AG_Index_Properties *p) : empty(true)
	{
		if(p!=nullptr)
			throw std::runtime_error("This constructor is designed for null pointers only!");
	}

};

class AG_Vector
{
public:

	std::vector<AG_Index_Properties> aglist;

	AG_Vector(boost::filesystem::path filepath)
	{
		std::string textcontent(filesystem::read_file(filepath));
		std::vector<std::string> contentvector=easytext::explode(textcontent,"\n");
		for(auto row : contentvector)
		{
			AG_Index_Properties idx_prop=convert_to_index(row);
			if(!idx_prop.empty)
				aglist.push_back(idx_prop);
		}
	}

	AG_Index_Properties convert_to_index(std::string row)
	{
		AG_Index_Properties idx_prop;
		boost::algorithm::trim(row);
		if(row.empty()||easytext::starts_with(row,"//"))
			return AG_Index_Properties(nullptr);
		std::vector<std::string> tokens=easytext::explode(row,",");
		 std::vector<std::string> tokens_prop(tokens.begin()+1,tokens.end());

		idx_prop.name=tokens[0];
		idx_prop.plot=easytext::is_string_in("plot",tokens_prop);
		return idx_prop;
	}

	// sample:
	// x0	surge_hp_x1
	// x1	surge_hp_x2
	// x2	surge_kin_vel
	std::string index_explanations(std::string variable)
	{
		std::string ret("");
		int index=0;
		for(auto idx_prop : aglist)
		{
			ret+=(index?newline:"");
			ret+="// "+variable+std::to_string(index)+"\t"+idx_prop.name;
			index++;
		}
		return ret;
	}

	// sample:
	// const int surge_hp_x1=0;
	// const int surge_hp_x2=1;
	// const int surge_kin_vel=2;
	std::string index_constants_definitions()
	{
		std::string ret("");
		int index=0;
		for(auto idx_prop : aglist)
		{
			ret+=(index?newline:"");
			ret+="const int "+idx_prop.name+"="+std::to_string(index)+";";
			index++;
		}
		return ret;		
	}

	// sample:
	// {"surge_input_acc_ref",outputs::surge_input_acc_ref},
	// {"specific_force",outputs::specific_force},
	std::string export_list()
	{
		std::string ret("");
		int index=0;
		for(auto idx_prop : aglist)
			if(idx_prop.plot)
			{
				ret+=(index?std::string(",")+newline:"");
				ret+="{\""+idx_prop.name+"\",outputs::"+idx_prop.name+"}";
				index++;
			}
		return ret;	
	}

	// sample:
	// "surge_input_acc_ref",
	// "surge_hp_out",
	// "surge_kin_acc",
	std::string output_header()
	{
		std::string ret("");
		int index=0;
		for(auto idx_prop : aglist)
		{
			ret+=(index?std::string(",")+newline:"");
			ret+="\""+idx_prop.name+"\"";
			index++;
		}
		return ret;		
	}

	// sample:
	// ymat(outputs::surge_input_acc_ref)=mid(mids::surge_input_acc_ref);
	// ymat(outputs::surge_hp_out)=mid(mids::surge_hp_out);
	// ymat(outputs::surge_kin_acc)=mid(mids::surge_kin_acc);
	// or
	// _unused(ymat);
	std::string output_matrix()
	{
		std::string ret("");
		int index=0;
		if(aglist.size()>0)
			for(auto idx_prop : aglist)
			{
				ret+=(index?newline:"");
				ret+="ymat(outputs::"+idx_prop.name+")=mid(mids::"+idx_prop.name+");";
				index++;
			}
		else
			ret="unused(ymat);";
		return ret;		
	}

	// sample:
	// x_dot(states::surge_hp_x1)=...
	// x_dot(states::surge_hp_x2)=...
	// x_dot(states::surge_kin_vel)=...
	std::string indices_eq_dots(std::string ns, std::string vec)
	{
		std::string ret("");
		int index=0;
		for(auto idx_prop : aglist)
		{
			ret+=(index?newline:"");
			ret+=ns+"("+vec+"::"+idx_prop.name+")=...";
			index++;
		}
		return ret;
	}	

};