// unit test
// test_dp5_compare_v1.cpp

// conditions:
// compiler mode: release
// buffer size: 10000000
// time: 0 ~ 50000
// eps rel, abs: 1E-12

template <class T>
struct show_type;

#include "top_include.hpp"

const uint state_size=3;
const uint results_additions=2;
const uint buffer_size=10000000;
const uint buffer_headers=state_size+results_additions;
typedef arma::vec::fixed<state_size> state_type;
typedef state_type deriv_type;
typedef arma::vec::fixed<state_size> observer_type;
typedef arma::mat::fixed<buffer_headers,buffer_size> buffer_type;
typedef double time_type;
typedef double value_type;

#include "../libs/solver/solver_dp5.hpp"
#include "libs/solver-v1/solver_dp5.hpp"
#include "libs/system_base.hpp"

class CSystem: public CSystem_UnitTest
{
public:
	const value_type eps_rel=1E-12;
	const value_type eps_abs=1E-12;
	const time_type start_time=0;
	const time_type stop_time=500;
	const time_type max_dt=stop_time;
	const time_type initial_dt=0.1;
	const state_type start_state={10.0,1.0,1.0};
	const time_type next_sudden_change_time=stop_time;

	CSystem(): CSystem_UnitTest()
	{
		// constructor
	}

	void observer_generic(const state_type &x, const double &t,const double &dummy_next_dt)
	{
		_unused(dummy_next_dt);
		observer_type ymat;
		ymat=x;
		results_push(t,0,ymat);
	}

	void rhs_generic(const state_type &x, state_type &x_dot, const double t)
	{
		_unused(t);
		const double sigma = 10.0;
		const double R = 28.0;
		const double b = 8.0 / 3.0;
		x_dot(0) = sigma * ( x(1) - x(0) );
		x_dot(1) = R * x(0) - x(1) - x(0) * x(2);
		x_dot(2) = -b * x(2) + x(0) * x(1);
	}

	void observer(const state_type &x, const double &t,const double &dummy_next_dt)
	{
		observer_generic(x,t,dummy_next_dt);
	}

	void operator()(const state_type &x, const double &t)
	{
		observer_generic(x,t,0);
	}


	void rhs(const state_type &x, state_type &x_dot, const double t)
	{
		rhs_generic(x,x_dot,t);
	}

	void operator()(const state_type &x, state_type &x_dot, const double t)
	{
		rhs_generic(x,x_dot,t);
	}

	double timer(const state_type &x, const double t)
	{
		_unused(x);
		return t+stop_time;
	}

	std::string integrate_adaptive_current()
	{
		state_type X=start_state;
		ode::Solver<ode::Steppers::RKDP5,CSystem> solver_current(std::ref(*this));
		solver_current.integrate_adaptive(
			X,// is manipulated
			start_time ,
			stop_time,
			initial_dt);
		results_finalize();
		return sha1sum(results).signature();
	}

	std::string integrate_adaptive_v1()
	{
		state_type X=start_state;
		odev1::Solver<odev1::Steppers::RKDP5,CSystem> solver_v1(std::ref(*this));
		solver_v1.integrate_adaptive(
			X,// is manipulated
			start_time ,
			stop_time ,
			initial_dt);
		results_finalize();
		return sha1sum(results).signature();
	}

};

int main()
{
	const std::string title="comparison between current and v1.0 results (DP5)";
	std::cout<<"Running "<<title<<"..."<<std::endl;
	std::string sha1_current,sha1_v1;
	CSystem lerenz_model;

	// test v1
	cronometer t_v1;
	//*** unit_test::cpu_temperature("v1.0 begin:");
	t_v1.tic();
	sha1_v1=lerenz_model.integrate_adaptive_v1();
	t_v1.toc();
	//*** unit_test::cpu_temperature("v1.0 end:");

	lerenz_model.Reset();

	// test ODE current
	cronometer t_current;
	//*** unit_test::cpu_temperature("current begin:");
	t_current.tic();
	sha1_current=lerenz_model.integrate_adaptive_current();
	t_current.toc();
	//*** unit_test::cpu_temperature("current end:");

	std::cout<<"v1.0 SHA1: "<<sha1_v1<<"   time elapsed: "<<t_v1.last_toc()<<std::endl;
	std::cout<<"current   SHA1: "<<sha1_current<<"   time elapsed: "<<t_current.last_toc()<<std::endl;

	if(sha1_v1==sha1_current)
		unit_test::passed(std::string("passed: ")+title);
	else
		unit_test::failed(std::string("rejected: ")+title);

	return 0;
}