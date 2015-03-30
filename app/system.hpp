class CSystem: public CSystem_Base
{
protected:
	time_type last_observed_time;
	state_type last_observed_states;

public:

	time_type next_sudden_change_time;

	const value_type eps_rel=1E-10;
	const value_type eps_abs=1E-10;
	const time_type max_dt=10000;//0.1;

	CSystem(): CSystem_Base()
	{
		// constructor
	}

	void observer(const state_type &x, const double &t,const double &next_dt)
	{
		input_type u;
		next_sudden_change_time=CSimulator::input(t,u);
		observer_type ymat;
		CSimulator::observer(x,t,ymat,last_observed_states,last_observed_time,u);
		results_push(t,next_dt,ymat);
		last_observed_time=t;
		last_observed_states=x;
	}

	void rhs(const state_type &x, state_type &dxdt, const double t)
	{
		input_type u;
		next_sudden_change_time=CSimulator::input(t,u);
		CSimulator::rhs(x,dxdt,t,last_observed_states,last_observed_time,u);
	}

	double timer(const state_type &x, const double t)
	{
		_unused(x);
		return t+10000;
	}

	size_t integrate_adaptive(
			state_type &start_state,
			const time_type start_time,
			const time_type end_time,
			const time_type dt )
	{
		ode::Solver<ode::Steppers::RKCK45,CSystem> solver(std::ref(*this));
		last_observed_time=start_time;
		last_observed_states=start_state;
		next_sudden_change_time=end_time;
		size_t return_val=
			solver.integrate_adaptive(
				start_state,// is manipulated
				start_time ,
				end_time ,
				dt);
		results_finalize();
		post_solve();
		std::cout<<"Results("<<files::now_print<<") SHA1: ";
		std::cout<<sha1sum(results).signature()<<std::endl;
		return return_val;
	}

};
