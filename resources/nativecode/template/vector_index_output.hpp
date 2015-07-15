
#WARNING_COMMENTS#

// path: ./#TRACING_PATH#

#pragma once

namespace #TITLE#
{
	const std::string output_header[]={
		#OUTPUTHEADER#
	};

	#INDEX_NUMBERS_COMMENTS#

	#INDEX_CONSTANTS#

	// total size of #TITLE# vector
	const int #TOTAL_SIZE#;
	
	//Export list
	struct ExpList
	{
		std::string file_suffix; // no space is accepted in file suffix name
		int output_index;
	} figure_list[]={
		#EXPORT_LIST#
	};
}
