#include "app/includes.hpp"
#include "app/declares.hpp"
#include "app/inputs.hpp"
#include "app/intermediates.hpp"
#include "app/outputs.hpp"
#include "app/constants.hpp"
#include "app/config.hpp"
#include "app/states.hpp"
#include "app/types.hpp"
#include "libs/application.hpp"
#include "libs/filesystem.hpp"
#include "libs/html.hpp"
#include "libs/log.hpp"
#include "app/simulator.hpp"
#include "libs/tf.hpp"
#include "libs/svg.hpp"
#include "libs/solver/solver_dp45.hpp"
#include "libs/solver/solver_ck45.hpp"
#include "app/system.hpp"

void main_normal()
{
	CSysHandler my_system;
	state_type x_iv;
	x_iv.zeros();
    ode::Solver<ode::Steppers::RKDP5,CSysHandler> solver(my_system);
    solver.integrate_adaptive(
    	x_iv ,// is manipulated
    	config::start_time ,
    	config::stop_time ,
    	0.1);
    std::string command="md5sum ";
    command+=files::output_results;
    system(command.c_str());
}

void main_test_sat()
{

	const double vmax=platform::max_vel_surge;
	const double xmax=platform::max_pos_surge;
	const double amax=platform::max_acc_surge;

	double x;
	std::cout<<"enter x: ";
	cin>>x;
	std::cout<<"vmax: "<<vmax<<std::endl;
	std::cout<<"xmax: "<<xmax<<std::endl;
	std::cout<<"amax: "<<amax<<std::endl;

	std::cout<<"x/amax: "<<x/amax<<std::endl;
	std::cout<<"x/vmax: "<<x/vmax<<std::endl;
	std::cout<<"x/xmax: "<<x/xmax<<std::endl;


	std::cout<<"sat'(x/amax): "<<CSimulator::sat_logistic(x/amax)<<std::endl;
	std::cout<<"sat'(x/vmax): "<<CSimulator::sat_logistic(x/vmax)<<std::endl;
	std::cout<<"sat'(x/xmax): "<<CSimulator::sat_logistic(x/xmax)<<std::endl;

	// std::cout<<"sat'_den(|a/vmax|): "<<CSimulator::sat_logistic_denuminator(abs(x/amax))<<std::endl;
	// std::cout<<"sat'_den(|x/vmax|): "<<CSimulator::sat_logistic_denuminator(abs(x/vmax))<<std::endl;
	// std::cout<<"sat'_den(|x/xmax|): "<<CSimulator::sat_logistic_denuminator(abs(x/xmax))<<std::endl;
}

int main()
{
	main_normal();
	// main_test_sat();
	return 0;
}