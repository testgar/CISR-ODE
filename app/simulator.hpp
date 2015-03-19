#pragma once

using namespace arma;

class CSimulator
{
public:

	static void rhs(
		const state_type &x,
		state_type &x_dot,
		const time_type &t,
		const state_type &last_observed_x,
		const time_type &last_observed_t
		)
	{
		// ******* system implementation *******
		input_type u;
		input(t,u);
		intermediate_type mid;
		intermediate_states(u,x,mid,t,last_observed_x,last_observed_t);

		// x0	surge_hp_x1
		// x1	surge_hp_x2
		// x2	surge_proposed_vel
		// x3	surge_kin_pos
		// x4	surge_lp_x1
		// x5	surge_lp_x2
		// x6	surgepitch_TCRL
		// x7	pitch_hp_x1
		// x8	pitch_pos

		// washouts dynamics
		// x0, x1
		hp_surge(mid,x,x_dot);// influence hp surge x 1,2
		// x4, x5
		lp_surge(mid,x,x_dot);// influence lp surge x 1,2
		// x7
		hp_pitch(mid,x,x_dot);// influence hp pitch x 1

		// kinematics
		const double surge_vmax=platform::max_vel_surge;
		const double surge_xmax=platform::max_pos_surge;
		// x2, x3
		linear_dynamic_proposed(t,mid(mids::surge_proposed_acc),x,x_dot,surge_vmax,surge_xmax);

		// tilt rate limit
		// x6
		x_dot(states::surgepitch_TCRL)=mid(mids::surgepitch_TCRL_deriv);

		// x8
		x_dot(states::pitch_kin_pos)=mid(mids::pitch_kin_vel);
	}

	static void observer(
		const state_type &x ,
		const double &t,
		observer_type &ymat,
		const state_type &last_observed_x,
		const time_type &last_observed_t
		)
	{
		input_type u;
		intermediate_type mid;
		input(t,u);
		intermediate_states(u,x,mid,t,last_observed_x,last_observed_t);

		ymat(outputs::surge_kin_acc)=mid(mids::surge_kin_acc);
		ymat(outputs::surge_kin_vel)=mid(mids::surge_kin_vel);
		ymat(outputs::surge_kin_pos)=mid(mids::surge_kin_pos);
		ymat(outputs::surge_hp_out)=mid(mids::surge_hp_out);
		ymat(outputs::surge_input_acc)=mid(mids::surge_input_acc_ref);
		ymat(outputs::surgepitch_sp_force)=mid(mids::surgepitch_sp_force);

		const int x1=states::surge_proposed_vel; //test
		const int x2=states::surge_kin_pos; //test

		ymat(outputs::x_proposed_vel)=x(x1); //test
		ymat(outputs::x_kin_pos)=x(x2); //test
	}

	static void intermediate_states(
		const input_type &u,
		const state_type &x,
		intermediate_type &mid,
		const time_type &t,
		const state_type &last_observed_x,
		const time_type &last_observed_t
		)
	{

		// m0	surge_input_acc_ref
		// m1	surge_hp_out
		// m2	surge_proposed_acc
		// m3	surge_proposed_vel
		// m4	surge_kin_pos
		// m5	surge_kin_vel
		// m6	surge_kin_acc
		// m7	surge_lp_out
		// m8	surgepitch_TC
		// m9	surgepitch_TCRL
		// m10	surgepitch_TCRL_deriv
		// m11	pitch_input_vel_ref
		// m12	pitch_hp_out
		// m13	pitch_kin_vel
		// m14	pitch_kin_pos
		// m15	pitch_tilt_force
		// m16	surgepitch_sp_force

		_unused(t);
		_unused(last_observed_x);

		const double surge_vmax=platform::max_vel_surge;
		const double surge_xmax=platform::max_pos_surge;
		const double surge_amax=platform::max_acc_surge;

		// m0, m11
		mid(mids::surge_input_acc_ref)=u(inputs::surge_acc);
		mid(mids::pitch_input_vel_ref)=u(inputs::pitch_vel);

		// m1
		mid(mids::surge_hp_out)=hp_surge_out(mid,x);

		// m2
		mid(mids::surge_proposed_acc)=bound(mid(mids::surge_hp_out),-surge_amax,+surge_amax); // dependant

		// m3
		mid(mids::surge_proposed_vel)=x(states::surge_proposed_vel);

		// m4
		mid(mids::surge_kin_pos)=x(states::surge_kin_pos);

		// m5, m6
		mid(mids::surge_kin_vel)=get_kinematic_surge_vel(x,surge_xmax);
		
		mid(mids::surge_kin_acc)=get_kinematic_surge_acc(x,mid(mids::surge_proposed_acc),surge_vmax,surge_xmax);

		// m7
		mid(mids::surge_lp_out)=lp_surge_out(x);

		// m8
		mid(mids::surgepitch_TC)=mid(mids::surge_lp_out)/constants::g; // dependant

		// m9
		mid(mids::surgepitch_TCRL)=x(states::surgepitch_TCRL);

		// m10
		const double surge_max_tilt_rate=platform::surge_tilt_threshold;
		mid(mids::surgepitch_TCRL_deriv)=// next line
			rate_limit_deriv(
				x(states::surgepitch_TCRL),
				mid(mids::surgepitch_TC),
				t,last_observed_t,
				surge_max_tilt_rate
				);

// x(states::surgepitch_TCRL_deriv)

		// m12
		mid(mids::pitch_hp_out)=hp_pitch_out(mid,x);

		// m13
		mid(mids::pitch_kin_vel)=mid(mids::pitch_hp_out)+mid(mids::surgepitch_TCRL); // dependant

		// m14
		mid(mids::pitch_kin_pos)=x(states::pitch_kin_pos);

		// m15, m16
		mid(mids::pitch_tilt_force)=mid(mids::pitch_kin_pos)*constants::g; // dependant
		mid(mids::surgepitch_sp_force)=mid(mids::pitch_tilt_force)+mid(mids::surge_kin_acc); // dependant
	}

	static void input(const time_type t,input_type &u)
	{
		u(inputs::surge_acc)=(t>1?(t>7?0:1):0);
		u(inputs::pitch_vel)=0;
	}

// protected:

	static double rate_limit_deriv(double limited,double ref,double t, double last_t,double rate_min,double rate_max)
	{
				// x(states::surgepitch_TCRL),
				// mid(mids::surgepitch_TC),
				// t,last_observed_t,
				// surge_max_tilt_rate
_unused(t);
_unused(last_t);
_unused(rate_min);
_unused(rate_max);
_unused(limited);
		return ref;
		// if(rate_min>=rate_max)
		// 	throw std::runtime_error("wrong rate limiter range!");

		// double rate=(ref-limited)/(t-last_t);
		// if(rate>rate_max)
		// 	return rate_max;
		// else if(rate<rate_min)
		// 	return rate_min;
		// else
		// 	return rate;
	}

	static double rate_limit_deriv(double limited,double ref,double t, double last_t,double rate)
	{
		return rate_limit_deriv(limited,ref,t,last_t,-rate,+rate);
	}


	static double bound(double val,double min,double max)
	{
		return std::min(std::max(val,min),max);
	}

	static void hp_surge(const intermediate_type &mid,const state_type &x,state_type &x_dot)
	{
		// observable form
		const double w=platform::omega_hp_x;
		const double z=platform::zeta_hp_x;
		const int x1=states::surge_hp_x1;
		const int x2=states::surge_hp_x2;
		x_dot(x1)=-w*w*x(x2)-w*w*mid(mids::surge_input_acc_ref);
		x_dot(x2)=x(x1)-2*z*w*x(x2)-2*z*w*mid(mids::surge_input_acc_ref);
	}

	static double hp_surge_out(const intermediate_type &mid,const state_type &x)
	{
		return x(states::surge_hp_x2)+mid(mids::surge_input_acc_ref);
	}

	static void lp_surge(const intermediate_type &mid,const state_type &x, state_type &x_dot)
	{
		// observable form
		const double w=platform::omega_lp_x;
		const double z=platform::zeta_lp_x;
		const int x1=states::surge_lp_x1;
		const int x2=states::surge_lp_x2;
		x_dot(x1)=-w*w*x(x2)+w*w*mid(mids::surge_input_acc_ref);
		x_dot(x2)=x(x1)-2*z*w*x(x2);
	}

	static double lp_surge_out(const state_type &x)
	{
		return x(states::surge_lp_x2);
	}

	static void hp_pitch(const intermediate_type &mid,const state_type &x, state_type &x_dot)
	{
		// observable form
		const double w=platform::omega_hp_pitch;
		const int x1=states::pitch_hp_x1;
		x_dot(x1)=-w*x(x1)-w*mid(mids::pitch_input_vel_ref);
	}

	static double hp_pitch_out(const intermediate_type &mid,const state_type &x)
	{
		return x(states::pitch_hp_x1)+mid(mids::pitch_input_vel_ref);
	}

	static void linear_dynamic_proposed(const time_type t,const double u,const state_type &x, state_type &x_dot,const double vmax,const double xmax)
	{
		// f(x) = d saturation(x) / dx
		// x1'=f(x1/amax)a
		// x2'=f(x2/vmax)x1
		_unused(t);
		const int x1=states::surge_proposed_vel;
		const int x2=states::surge_kin_pos;
		x_dot(x1)=proposed_velocity_derivative(u,x,vmax,xmax);
		x_dot(x2)=proposed_position_derivative(x,vmax,xmax);
	}

	static double proposed_position_derivative(
		const state_type &x,
		const double vmax,
		const double xmax)
	{
		_unused(vmax);
		const int x1=states::surge_proposed_vel;
		const int x2=states::surge_kin_pos;
		if(x(x1)*x(x2)<=0.0)
			return x(x1);
		else
			return sat_logistic(x(x2)/xmax)*x(x1);
	}

	static double proposed_velocity_derivative(
		const double u,
		const state_type &x,
		const double vmax,
		const double xmax)
	{
		const int x1=states::surge_proposed_vel;
		const int x2=states::surge_kin_pos;
		double pvd=sat_logistic(x(x1)/vmax)*u;
		if(x(x1)*x(x2)>0)
		{
			double slowerer=x(x2)*std::abs(x(x1));
			slowerer*=slowerer*slowerer;
			pvd-=(1.0/sat_logistic(x(x2)/xmax)-1.0)*slowerer;
		}
		if(u*x(x1)<=0.0)
		{
			if(x(x1)>=0.0)
				return std::min(u,pvd);
			else
				return std::max(u,pvd);
		}
		else
		{
			return pvd;
		}
	}

	static double get_kinematic_surge_acc(const state_type &x,const double u,const double vmax,const double xmax)
	{
		// x proposed velocity
		const auto xpv= x(states::surge_proposed_vel);
		// x kinematic position
		const auto xkp= x(states::surge_kin_pos);

		if(xkp*xpv>=0)
		{
			double xx_dot=proposed_position_derivative(x,vmax,xmax);
			double xv_dot=proposed_velocity_derivative(u,x,vmax,xmax);
			double result=xv_dot*sat_logistic(xkp/xmax)+sat_logistic_D1(std::abs(xkp/xmax))*xpv/xmax*xx_dot*std::abs(xpv);
			return result;
		}
		else
		{
			return proposed_velocity_derivative(u,x,vmax,xmax);
		}
	}

	static double get_kinematic_surge_vel(const state_type &x,const double xmax)
	{
		const int x1=states::surge_proposed_vel;
		const int x2=states::surge_kin_pos;
		return x(x1)*x(x2)<=0 ? x(x1) : sat_logistic(x(x2)/xmax)*x(x1);
	}

	static double sat_logistic_numinator()
	{
		static double numinator=(1+exp(platform::saturation_b));
		return numinator;
	}

	static double sat_logistic_denuminator(double x)
	{
		double denuminator=(1+exp(platform::saturation_a*x+platform::saturation_b));
		return denuminator;
	}

	// saturation logistic funciton
	static double sat_logistic(double x)
	{	// derivative of saturation function
		return sat_logistic_numinator()/sat_logistic_denuminator(std::abs(x));
	}

	static double sat_logistic_D1(double x)
	{	// second derivative of saturation function
		const double n=sat_logistic_numinator();
		const double d=sat_logistic_denuminator(std::abs(x));
		const double a=platform::saturation_a;
		return -(n*a*(d-1))/(d*d)*(x>=0?1:-1);
	}

};