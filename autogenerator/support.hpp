#include "../libs/filesystem.hpp"
#include "../libs/config_files.hpp"

void support_javascript_files()
{
	typedef boost::filesystem::path path;

	const path jquery_dest=files::output_folder/files::jquery;
	const path jquery_source=files::js_src;

	const path fixedHeaderScrolling_dest=files::output_folder/files::fhs_table_js_basename;
	const path fixedHeaderScrolling_source=files::fhs_table_js;

	if( ! filesystem::exists(jquery_dest))
		boost::filesystem::copy_file(jquery_source,jquery_dest);

	if( ! filesystem::exists(fixedHeaderScrolling_dest))
		boost::filesystem::copy_file(fixedHeaderScrolling_source,fixedHeaderScrolling_dest);
}