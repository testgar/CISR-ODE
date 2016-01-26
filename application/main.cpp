#include "../includes.hpp"
#include "system.hpp"
#include "profiler.hpp"

void main_normal()
{
	cronometer timer;
	timer.tic();

	CSystem my_system;
	Model::state_type x_iv;
	x_iv.zeros();
	my_system.integrate_adaptive(
		x_iv ,// is manipulated
		config::start_time ,
		config::stop_time ,
		0.1);

	timer.toc();
	std::cout<<"Results("<<files::now_print<<") ";
	std::cout<<"SHA1: "<<sha1sum(my_system.get_results()).signature()<<" time elapsed: "<<timer.last_toc()<<std::endl;
}

int main()
{
	main_normal();
	return 0;
}