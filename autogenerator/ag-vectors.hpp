#pragma once

#include "ag-item.hpp"

class AG_Vector
{
public:

	std::vector<AG_Item> aglist;

	AG_Vector(boost::filesystem::path filepath)
	{
		std::string textcontent(filesystem::read_file(filepath));
		textcontent=easytext::strip_comments(textcontent);
		std::vector<std::string> contentvector=easytext::explode(textcontent,"\n");
		for(auto row : contentvector)
		{
			// std::string row_pure=easytext::remove_comments(row);
			AG_Item item=string_to_item(row);
			if(!item.empty)
				aglist.push_back(item);
		}
	}

	AG_Item string_to_item(std::string row)
	{
		AG_Item item;
		
		boost::algorithm::trim(row);

		std::vector<std::string> tokens=easytext::explode_shallow(row,',');
		if(row.empty()||tokens.size()==0||easytext::starts_with(row,"//"))
			return AG_Item(nullptr);

		for(auto token:tokens)
			boost::trim(token);

		bool item_is_array=(tokens[0].find('[')!=std::string::npos);

		// check for item name
		if(item_is_array)
		{
			const boost::regex pattern_variable("^([A-Za-z0-9_]+)\\[(.*?)\\]"); 
			boost::match_results<std::string::const_iterator> regex_results;
			if (boost::regex_match(tokens[0], regex_results, pattern_variable))
			{
				item.name=regex_results[1];
				try {
					item.array_size = boost::lexical_cast<int>( regex_results[2] );
				} catch( boost::bad_lexical_cast const& ) {
					throw std::runtime_error("Array \""+item.name+"\" with invalid size has been requested in `"+row+"`");
				}
			}
		}
		else
		{
			item.name=tokens[0];
		}

		for(uint p_index=1;p_index<tokens.size();p_index++)
		{
			std::string token=tokens[p_index];
			token=easytext::replace_all(token," ","");
			token=easytext::replace_all(token,"\t","");
			std::string property_name;
			std::vector<uint> property_range;

			const boost::regex name_range("^([A-Za-z0-9_]+)\\[(.*?)\\]"); 
			boost::match_results<std::string::const_iterator> regex_results;
			if (boost::regex_match(token, regex_results, name_range))
			{
				property_name=regex_results[1];
				property_range=parse_range(regex_results[2],item.array_size);
			}
			else
			{
				property_name=token;
				// if(item_is_array)
				for(uint i=0;i<item.array_size;i++)
					property_range.push_back(i);
			}

			if(property_name=="plot")
			{
				if(item.plot)
					throw std::runtime_error("Property `plot` duplication detected in `"+row+"`");
				item.plot=true;
				item.plot_indices=property_range;
			}
			else if(property_name=="out")
			{
				if(item.out)
					throw std::runtime_error("Property `out` duplication detected in `"+row+"`");
				item.out=true;
				item.out_indices=property_range;
			}
			else if(property_name=="explicit")
			{
				item.explicit_indices=property_range;
			}
			else
				throw std::runtime_error("Unknown property `"+property_name+"` in `"+row+"`");
		}

		for(auto x:item.plot_indices)
			if ( std::find(item.out_indices.begin(), item.out_indices.end(), x)==item.out_indices.end() )
				throw std::runtime_error(item.name+"["+easytext::to_string(x)+"] is requested to be plotted but is not in output list in `"+row+"`");

		return item;
	}

	// convert "3,4,5,9:12,16" into vector
	std::vector<uint> parse_range(std::string range_text,uint last_index)
	{
		std::vector<uint> result;
		std::vector<std::string> range_items=easytext::explode(range_text,",");
		for(std::string range_item_text:range_items)
		{
			int start,stop,step;
			try
			{
				std::vector<std::string> range_item_parts=easytext::explode(range_item_text,":");
				if(range_item_parts.size()<1)
					throw std::runtime_error("Empty number/range detected in `"+range_text+"`");
				std::size_t colon_occurance=range_item_parts.size()-1;

				switch(colon_occurance)
				{
					case 0:
						// number
						start=boost::lexical_cast<int>(range_item_text);
						step=1;
						stop=start;
						break;
					case 1:
						// start:stop
						start=boost::lexical_cast<int>(range_item_parts[0]);
						step=1;
						stop=(range_item_parts[1]=="end"? last_index-1 : boost::lexical_cast<int>(range_item_parts[1]));
						break;
					case 2:
						// start:step:stop
						start=boost::lexical_cast<int>(range_item_parts[0]);
						step=boost::lexical_cast<int>(range_item_parts[1]);
						stop=(range_item_parts[2]=="end"? last_index-1 : boost::lexical_cast<int>(range_item_parts[2]));
						// stop=boost::lexical_cast<int>(range_item_parts[2]);
						break;
					default:
						throw std::runtime_error("Invalid number of range partitions `"+range_item_text+"` in `"+range_text+"`");
				}
			} catch( boost::bad_lexical_cast const& ) {
				throw std::runtime_error("Invalid number/range `"+range_item_text+"` in `"+range_text+"`");
			}

			if(step<1)
				throw std::runtime_error("Invalid step `"+easytext::to_string(step)+"` in `"+range_text+"`");
			if(start<0)
				throw std::runtime_error("Invalid index `"+easytext::to_string(start)+"` in `"+range_text+"`");

			for(int i=start;i<=stop;i+=step)
				result.push_back(i);
		}
		easyalgorithm::removeDuplicates(result);
		return result;
	}

	uint size()
	{
		uint sz=0;
		for(auto al:aglist)
			sz+=al.array_size;
		return sz;
	}

	uint output_size()
	{
		uint sz=0;
		for(auto al:aglist)
			sz+=al.out_indices.size();
		return sz;
	}
	
	// sample:
	// x0	surge_hp_x1
	// x1	surge_hp_x2
	// x2	surge_kin_vel
	std::string index_explanations(std::string variable_ns)
	{
		std::string ret;
		uint start_index=0;
		for(auto item : aglist)
		{
			ret+=(start_index?easytext::newline:"");
			ret+=item.to_index_explanations(start_index,variable_ns);
		}
		return ret;
	}

	// sample:
	// y0 surge_input_acc_ref;
	// y1 surge_hp_out;
	// y2 surge_kin_acc;
	std::string output_index_explanations()
	{
		std::string ret;
		uint start_index=0;
		for(auto item : aglist)
			if(item.out)
				ret+=item.to_output_index_explanations(start_index);
		return ret;
	}

	// sample:
	// const int surge_hp_x1=0;
	// const int surge_hp_x2=1;
	// const int surge_kin_vel=2;
	std::string index_constants_definitions()
	{
		std::string ret;
		uint start_index=0;
		for(auto item : aglist)
		{
			ret+=(start_index?easytext::newline:"");
			ret+=item.to_index_constants_definitions(start_index);
		}
		return ret;		
	}

	// sample:
	// const int surge_input_acc_ref=0;
	// const int surge_hp_out=1;
	// const int surge_kin_acc=2;
	std::string output_index_constants_definitions()
	{
		std::string ret;
		uint start_index=0;
		for(auto item : aglist)
			if(item.out)
				ret+=item.to_output_index_constants_definitions(start_index);
		return ret;	
	}

	// sample:
	// {"surge_input_acc_ref",outputs::surge_input_acc_ref},
	// {"specific_force",outputs::specific_force},
	std::string export_figures_list()
	{
		std::string ret;
		uint line_number=0;
		for(auto item : aglist)
			if(item.plot)
				ret+=item.to_export_figures_list(line_number);
		return ret;
	}

	// sample:
	// "surge_input_acc_ref",
	// "surge_hp_out",
	// "surge_kin_acc",
	std::string output_header()
	{
		std::string ret;
		uint index=0;
		for(auto item : aglist)
			if(item.out)
			{
				ret+=(index?std::string(",")+easytext::newline:"");
				ret+=item.to_output_header();
				index+=item.array_size;
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
		std::string ret;
		// uint current_out_index=0;
		uint line_number=0;
		for(auto item : aglist)
			if(item.out)
				ret+=item.to_output_matrix(line_number);
		if(line_number==0)
			ret="_unused(ymat);";
		return ret;		
	}

	// sample:
	// x_dot(states::surge_hp_x1)=...
	// x_dot(states::surge_hp_x2)=...
	// x_dot(states::surge_kin_vel)=...
	std::string indices_eq_dots(const std::string& var_name, const std::string& vec_ns)
	{
		std::string ret;
		uint index=0;
		for(auto item : aglist)
		{
			ret+=(index?easytext::newline:"");
			ret+=item.to_indices_eq_dots(var_name,vec_ns);
			index+=item.array_size;
		}
		return ret;
	}

};