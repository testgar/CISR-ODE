#pragma once

#include <iostream>
#include <string>
#define _unused(x) ((void)x)

class unit_test
{
public:

	static void print_red(std::string text)
	{
		std::cout<< "\033[1;31m"<<text<<"\033[0m"<<std::endl;
	}

	static void print_green(std::string text)
	{
		std::cout<< "\033[0;32m"<<text<<"\033[0m"<<std::endl;
	}

	static void passed(std::string text)
	{
		print_green(text);
	}

	static void failed(std::string text)
	{
		print_red(text);
	}

	static void cpu_temperature(std::string message)
	{
		if(system((std::string("")+"echo \""+message+"\" $(acpi -t)").c_str()))
		{
			// do nothing, suppress warning...
		}
	}

};
