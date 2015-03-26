#pragma once

using namespace arma;

class CSimulator
{
public:

	static void rhs(
		const state_type 			&x,
		state_type 					&x_dot,
		const time_type				&t,
		const state_type 			&last_observed_x,
		const time_type 			&last_observed_t,
		input_type					&u
		);

	static void observer(
		const state_type 			&x ,
		const double 				&t,
		observer_type 				&ymat,
		const state_type 			&last_observed_x,
		const time_type 			&last_observed_t,
		input_type					&u
		);

	static void intermediate_states(
		const input_type &u,
		const state_type &x,
		intermediate_type &mid,
		const time_type &t,
		const state_type &last_observed_x,
		const time_type &last_observed_t
		);

	static double input(const time_type t,input_type &u)
	{
		// surge_acc
		// pitch_vel
		u(inputs::surge_acc)=(t>1?(t>7?0:1):0);
		u(inputs::pitch_vel)=0;
		
		double next_sudden_change_time;
		if(t<1)
			next_sudden_change_time=1.0+0.000001;
		else if(t<7)
			next_sudden_change_time=7.0+0.000001;
		else
			next_sudden_change_time=0.0;
		return next_sudden_change_time;
	}

protected:

 	static double rate_limit_deriv(
		const double &signal,
		const double &signal_last_observed,
		const double &rate_limited_signal,
		const double &dt,
		const double &rate
		)
	{
		if(dt==0)
			return 0;
		const double error_threshold=0.002;
		double current_rate=(signal-signal_last_observed)/dt;
		// is is correct to use last observed signal instead of current signal?
		double error=(signal_last_observed-rate_limited_signal);
		double proposed_derivative=current_rate+error*rate/error_threshold;
		return bound(proposed_derivative,-rate,+rate);
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

		double pxd=sat_logistic(x(x2)/xmax)*x(x1);
		if(x(x1)*x(x2)<=0.0)
		{
			if(x(x2)>=0.0)
				return std::min(x(x1),pxd);
			else
				return std::max(x(x1),pxd);
		}
		else
		{
			return pxd;
		}
	}

	static double proposed_velocity_derivative(
		const double u,
		const state_type &x,
		const double vmax,
		const double xmax)
	{
		const int x1=states::surge_proposed_vel;
		// const int x2=states::surge_kin_pos;
		_unused(xmax);
		double pvd=sat_logistic(x(x1)/vmax)*u;

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
			double result=xv_dot*sat_logistic(xkp/xmax)+sat_logistic_D1(xkp/xmax)*xpv/xmax*xx_dot;
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

	// saturation logistic funciton
	static double sat_logistic(double x)
	{	// derivative of saturation function
		double xabs=std::abs(x);
		double threshold=0.95;
		if(xabs<threshold)
			return 1.0;
		if(xabs>=1)
			return 0.0;
		return (1.0-xabs)/(1.0-threshold);
	}

	static double sat_logistic_D1(double x)
	{	// second derivative of saturation function
		double xabs=std::abs(x);
		double threshold=0.95;
		if(xabs<threshold||xabs>=1)
			return 0.0;
		return -1.0/(1.0-threshold)*(x>0?1:-1);
	}
};