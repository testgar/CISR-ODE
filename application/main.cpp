#include "../includes.hpp"
#include "system.hpp"

void main_normal()
{
	CSystem my_system;
	Model::state_type x_iv;
	x_iv.zeros();
	my_system.integrate_adaptive(
		x_iv ,// is manipulated
		config::start_time ,
		config::stop_time ,
		0.1);
}

int main()
{
	main_normal();
	return 0;
}