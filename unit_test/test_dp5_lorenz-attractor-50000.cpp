// unit test
// test_dp5_lorenz-attractor-50000.cpp

// conditions:
// compiler mode: release
// buffer size: 10000000
// time: 0 ~ 50000
// eps rel, abs: 1E-12
// ODEINT SHA1: 95b2d4c3ee74fd240ce9cd31ee0c0333560c9c9a   time elapsed: 7.76337
// CISR   SHA1: 95b2d4c3ee74fd240ce9cd31ee0c0333560c9c9a   time elapsed: 6.9538
// passed: comparison between CISR and ODEINT results (DP5)

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
#include "libs/system_base.hpp"

class CSystem_ODEINT: public CSystem_UnitTest
{
public:
	const value_type eps_rel=1E-12;
	const value_type eps_abs=1E-12;
	const time_type start_time=0;
	const time_type stop_time=50000;
	const time_type max_dt=stop_time;
	const time_type initial_dt=0.1;
	const state_type start_state={10.0,1.0,1.0};
	const time_type next_sudden_change_time=stop_time;

	CSystem_ODEINT(): CSystem_UnitTest()
	{
		// constructor
	}

	void observer(const state_type &x, const double &t,const double &dummy_next_dt)
	{
		_unused(dummy_next_dt);
		observer_type ymat;
		ymat=x;
		results_push(t,0,ymat);
	}

	void observer(const state_type &x, const double &t)
	{
		observer(x,t,0);
	}

	void rhs(const state_type &x, state_type &x_dot, const double t)
	{
		_unused(t);
		const double sigma = 10.0;
		const double R = 28.0;
		const double b = 8.0 / 3.0;
		x_dot(0) = sigma * ( x(1) - x(0) );
		x_dot(1) = R * x(0) - x(1) - x(0) * x(2);
		x_dot(2) = -b * x(2) + x(0) * x(1);
	}

	double timer(const state_type &x, const double t)
	{
		_unused(x);
		return t+stop_time;
	}

	std::string integrate_adaptive_cisr()
	{
		state_type X=start_state;
		ode::Solver<ode::Steppers::RKDP5,CSystem_ODEINT> solver_cisr(std::ref(*this));
		solver_cisr.integrate_adaptive(
			X,// is manipulated
			start_time ,
			stop_time ,
			initial_dt);
		results_finalize();
		return sha1sum(results).signature();
	}

	std::string integrate_adaptive_odeint()
	{
		using namespace boost::numeric::odeint;
		state_type X=start_state;
		typedef runge_kutta_dopri5<state_type> stepper_type;
		integrate_adaptive( make_controlled( eps_abs , eps_rel , stepper_type() ) ,
			[this](const state_type &x, state_type &x_dot, const double t){ this->rhs(x,x_dot,t); },
			X , start_time , stop_time , initial_dt ,
			[this](const state_type &x, const double &t){ this->observer(x,t); }
			);
		results_finalize();
		return sha1sum(results).signature();
	}

};

int main()
{
	const std::string title="comparison between CISR and ODEINT results (DP5)";
	std::cout<<"Running "<<title<<"..."<<std::endl;
	std::string sha1_cisr,sha1_odeint;
	CSystem_ODEINT lerenz_model;

	// test ODEINT
	cronometer t_odeint;
	//*** unit_test::cpu_temperature("ODEINT begin:");
	t_odeint.tic();
	sha1_odeint=lerenz_model.integrate_adaptive_odeint();
	t_odeint.toc();
	//*** unit_test::cpu_temperature("ODEINT end:");

	lerenz_model.Reset();

	// test ODE CISR
	cronometer t_cisr;
	//*** unit_test::cpu_temperature("CISR begin:");
	t_cisr.tic();
	sha1_cisr=lerenz_model.integrate_adaptive_cisr();
	t_cisr.toc();
	//*** unit_test::cpu_temperature("CISR end:");

	std::cout<<"ODEINT SHA1: "<<sha1_odeint<<"   time elapsed: "<<t_odeint.last_toc()<<std::endl;
	std::cout<<"CISR   SHA1: "<<sha1_cisr<<"   time elapsed: "<<t_cisr.last_toc()<<std::endl;

	if(sha1_odeint==sha1_cisr)
		unit_test::passed(std::string("passed: ")+title);
	else
		unit_test::failed(std::string("rejected: ")+title);

	return 0;
}