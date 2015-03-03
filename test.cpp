#include <iostream>
#include <string>
#include <vector>


// std::string replace(const std::string text,
//                   std::string tofind,
//                   std::string toreplace)
// {
// 	int found = text.find(tofind);
// 	if (found != std::string::npos)
// 	    return(text.replace(text.find(tofind), tofind.length(), toreplace));
// 	return text;
// }

std::string replace(std::string text,
					std::string find,
					std::string replace)
{
	size_t found_pos = text.find(find);
	if(found_pos == std::string::npos)
		return(text);
	return text.replace(found_pos,find.length(),replace);
}

int main()
{
	std::vector<std::string> mylist={"col1","cell2","col3","col4","col5"};
	
	for(const std::string item: mylist)
	{
		std::cout<<replace(item,"cell","item")<<std::endl;
	}

	for(const std::string item: mylist)
	{
		std::cout<<item<<std::endl;
	}

	return 0;
}
ddd2
