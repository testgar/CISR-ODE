#pragma once

#include <vector>
#include <algorithm>
#include <functional>

namespace easyalgorithm
{
	template<typename T>
	void removeDuplicates(std::vector<T>& v)
	{
		std::sort(v.begin(), v.end());
		v.erase(std::unique(v.begin(), v.end()), v.end());
	}


	// sample:
	// myvector=easyalgorithm::filter_vector(the vector,
	// 	[](const myType& item)-> bool { return item.out==true; });
	template<typename T, typename U>
	std::vector<T> filter_vector(const std::vector<T> &list_source, U function_keep)
	{
		std::vector<T> list_result;
		list_result.reserve(list_source.size());

		std::copy_if(begin(list_source), end(list_source),std::back_inserter(list_result),function_keep);

		return list_result;
	}

	template<typename T>
	bool is_subset(std::vector<T> v1,std::vector<T> v2)
	{
		for(auto x:v1)
			if ( std::find(v2.begin(), v2.end(), x)!=v2.end() )
				return false;
		return true;
	}

}
