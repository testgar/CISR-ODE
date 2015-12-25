#pragma once

#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include "../libs/filesystem.hpp"
#include "../libs/easytext.hpp"
#include "../libs/easyalgorithm.hpp"

class AG_Item
{
public:

	// struct Range_specification
	// {
	// 	uint start;
	// 	uint step;
	// 	uint stop;
	// };

	std::string name;
	bool plot;
	bool out;
	bool empty;
	uint array_size;
	std::vector<uint> out_indices;
	std::vector<uint> plot_indices;
	std::vector<uint> explicit_indices;

	AG_Item() :
		name(""),
		plot(false),
		out(false),
		empty(false),
		array_size(1)
	{
	}

	// This constructor is designed for generation of inedx properties for being ignored
	AG_Item(AG_Item *p) : empty(true)
	{
		if(p!=nullptr)
			throw std::runtime_error("This constructor is designed for null pointers only!");
	}

	// sample:
	// "surge_input_acc_ref"
	std::string to_output_header()
	{
		std::string ret;
		if(out)
		{
			if(array_size==1)
				ret+="\""+name+"\"";
			else
			{
				for(uint k=0;k<out_indices.size();k++)
				{
					ret+=(k>0?std::string(",")+easytext::newline:"");
					ret+="\""+name+"["+std::to_string(out_indices[k])+"]\"";
				}
			}
		}
		return ret;
	}

	// sample:
	// ymat(outputs::surge_input_acc_ref)=mid(mids::surge_input_acc_ref);
	std::string to_output_matrix(uint &line_number)
	{
		std::string ret;
		if(out)
		{
			if(array_size==1)
			{
				ret+=(line_number?easytext::newline:"");
				ret+="ymat(outputs::"+name+")=mid(mids::"+name+");";
				line_number++;
			}
			else
			{
				bool cover_all_range=true;
				if(out_indices.size()!=array_size)
					cover_all_range=false;
				else
				{
					for(uint cov=0;cov<array_size;cov++)
						cover_all_range = cover_all_range && ( std::find(out_indices.begin(), out_indices.end(), cov)!=out_indices.end() );
				}

				if(cover_all_range)
				{
					ret+=(line_number?easytext::newline:"");
					ret+="ymat.subvec(outputs::"+name+"_0,outputs::"+name+"_"+std::to_string(array_size-1)+")=mid.subvec(mids::"+name+"_0,mids::"+name+"_end);";
					line_number++;
				}
				else
				{
					for(auto k:out_indices)
					{
						ret+=(line_number?easytext::newline:"");
						bool k_is_explicit= (k==0) || ( std::find(explicit_indices.begin(), explicit_indices.end(), k)!=explicit_indices.end() );
						if(k_is_explicit)
							ret+="ymat(outputs::"+name+"_"+std::to_string(k)+")=mid(mids::"+name+"_"+std::to_string(k)+");";
						else
							ret+="ymat(outputs::"+name+"_"+std::to_string(k)+")=mid(mids::"+name+"_0+"+std::to_string(k)+");";
						line_number++;
					}
				}
			}
		}
		return ret;
	}


	// sample:
	// x_dot(states::surge_hp_x1)=...
	std::string to_indices_eq_dots(const std::string& var_name, const std::string& vec_ns)
	{
		std::string ret;
		if(array_size==1)
			ret+=var_name+"("+vec_ns+"::"+name+")=...";
		else
		{
			ret+=var_name+".subvec("+vec_ns+"::"+name+"_0,"+vec_ns+"::"+name+"_end)=...";
			for(auto explicit_index:explicit_indices)
				if(explicit_index!=0)
				{
					ret+=easytext::newline+"//* Also explicitly used: "+var_name+"("+vec_ns+"::"+name+"_"+std::to_string(explicit_index)+")=...";
				}
		}
		return ret;		
	}

	// sample:
	// {"surge_input_acc_ref",outputs::surge_input_acc_ref},
	std::string to_export_figures_list(uint &line_number)
	{
		std::string ret;
		if(plot)
		{
			if(array_size==1)
			{
				ret+=(line_number?std::string(",")+easytext::newline:"");
				ret+="{\""+name+"\",outputs::"+name+"}";
				line_number++;
			}
			else
			{
				for(auto plot_index:plot_indices)
				{
					ret+=(line_number?std::string(",")+easytext::newline:"");
					ret+="{\""+name+"["+std::to_string(plot_index)+"]"+"\",outputs::"+name+"_"+std::to_string(plot_index)+"}";
					line_number++;
				}
			}
		}
		return ret;
	}

	std::string to_index_explanations(uint &start_index,std::string variable_ns)
	{
		std::string ret;
		std::string index_start_tx=std::to_string(start_index);
		std::string index_stop_tx=std::to_string(start_index+array_size-1);
		std::string variable_size=std::to_string(array_size);

		std::string range=(array_size==1?index_start_tx:index_start_tx+":"+index_stop_tx);
		ret="// "+variable_ns+"["+range+"]"+"\t"+name+(array_size==1?"":"["+variable_size+"]");
		start_index+=array_size;
		return ret;
	}

	std::string to_index_constants_definitions(uint &start_index)
	{
		std::string ret;
		const std::string type_text="const uint ";
		if(array_size==1)
		{
			ret+=type_text+name+"="+std::to_string(start_index)+";";
			start_index++;
		}
		else
		{
			ret+=type_text+name+"_0="+std::to_string(start_index)+";"+easytext::newline;
			for(auto explicit_index:explicit_indices)
				if(explicit_index!=0)
				{
					ret+=type_text+name+"_"+std::to_string(explicit_index)+"="+std::to_string(start_index+explicit_index)+";"+easytext::newline;
				}
			ret+=type_text+name+"_end="+std::to_string(start_index+array_size-1)+";";
			start_index+=array_size;
		}
		return ret;
	}

	std::string to_output_index_constants_definitions(uint &start_index)
	{
		std::string ret;
		const std::string type_text="const uint ";
		if(out)
		{
			if(array_size==1)
			{
				ret+=(start_index?easytext::newline:"");
				ret+=type_text+name+"="+std::to_string(start_index)+";";
				start_index++;
			}
			else
			{
				for(auto out_index:out_indices)
				{
					ret+=(start_index?easytext::newline:"");
					ret+=type_text+name+"_"+std::to_string(out_index)+"="+std::to_string(start_index)+";";
					start_index++;
				}
			}
		}
		return ret;
	}

	std::string to_output_index_explanations(uint &start_index)
	{
		std::string ret;
		if(out)
		{
			if(array_size==1)
			{
				ret+=(start_index?easytext::newline:"");
				ret+="// y"+std::to_string(start_index)+"\t"+name+";";
				start_index++;
			}
			else
			{
				for(auto out_index:out_indices)
				{
					ret+=(start_index?easytext::newline:"");
					ret+="// y"+std::to_string(start_index)+"\t"+name+"["+std::to_string(out_index)+"];";
					start_index++;
				}
			}
		}
		return ret;
	}
	

}; // end of class AG_Item
