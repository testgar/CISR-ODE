#include "includes.hpp"

void main_normal()
{
	CSystem my_system;
	state_type x_iv;
	x_iv.zeros();
	my_system.integrate_adaptive(
		x_iv ,// is manipulated
		config::start_time ,
		config::stop_time ,
		0.1);
}

void main_test_sat()
{
}

int main()
{
	main_normal();
	// main_test_sat();
	return 0;
}