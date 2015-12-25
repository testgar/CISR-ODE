// unit test

// conditions:
// compiler mode: release
// buffer size: 10000000
// time: 0 ~ 50000
// eps rel, abs: 1E-12
// current SHA1: 95b2d4c3ee74fd240ce9cd31ee0c0333560c9c9a time elapsed: 6.20748
// v1.0 SHA1: 95b2d4c3ee74fd240ce9cd31ee0c0333560c9c9a time elapsed: 6.1156
// passed: comparison between current and v1.0 results (DP5)

template <class T>
struct show_type;

#include "top_include.hpp"

const uint state_size=3;
const uint buffer_size=35000000;
const uint results_additions=2;
const uint buffer_headers=state_size+results_additions;

#include "../libs/solver/solver_dp5.hpp"
#include "libs/solver-v1/solver_dp5.hpp"
#include "libs/system_base.hpp"

class CSystem: public CSystem_UnitTest
{
public:
	const value_type eps_rel=1E-12;
	const value_type eps_abs=1E-12;
	const time_type start_time=0;
	const time_type stop_time=50000;
	const time_type max_dt=stop_time;
	const time_type min_dt=0.0;
	const time_type initial_dt=0.1;
	const state_type start_state={10.0,1.0,1.0};
	const time_type next_sudden_change_time=stop_time;
	std::string sha1;

	typedef arma::vec::fixed<state_size> state_type;
	typedef state_type deriv_type;

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

	void rhs(const state_type &x, state_type &x_dot, const double t)
	{
		rhs_generic(x,x_dot,t);
	}

	double timer(const state_type &x, const double t)
	{
		_unused(x);
		return t+stop_time;
	}

	void integrate_adaptive_current()
	{
		state_type X=start_state;
		ode::Solver<ode::Steppers::RKDP5,CSystem> solver_current(std::ref(*this));
		solver_current.integrate_adaptive(
			X,// is manipulated
			start_time ,
			stop_time,
			initial_dt);
		results_finalize();
		sha1=sha1sum(results).signature();
	}

	void integrate_adaptive_v1()
	{
		state_type X=start_state;
		odev1::Solver<odev1::Steppers::RKDP5,CSystem> solver_v1(std::ref(*this));
		solver_v1.integrate_adaptive(
			X,// is manipulated
			start_time ,
			stop_time ,
			initial_dt);
		results_finalize();
		sha1=sha1sum(results).signature();
	}

};

void test_item(CSystem &model,std::string name,void (CSystem::*custom_integrate)(void))
{
	cronometer t;
	//*** unit_test::cpu_temperature("v1.0 begin:");
	t.tic();
	(model.*custom_integrate)();
	t.toc();
	//*** unit_test::cpu_temperature("v1.0 end:");
	std::cout<<name<<" SHA1: "<<model.sha1<<" time elapsed: "<<t.last_toc()<<std::endl;
}

int main()
{
	const std::string title="comparison between current and v1.0 results (DP5)";
	std::cout<<"Running "<<title<<"..."<<std::endl;

	CSystem model1;
	CSystem model2;

	test_item(model2,"current",&CSystem::integrate_adaptive_current);
	test_item(model1,"v1.0",&CSystem::integrate_adaptive_v1);

	if(model1.sha1==model2.sha1)
		unit_test::passed(std::string("passed: ")+title);
	else
		unit_test::failed(std::string("rejected: ")+title);

	return 0;
}