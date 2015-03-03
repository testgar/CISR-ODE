#pragma once

using namespace arma;

class CSimulator
{
public:

	static void rhs(
		const state_type &x,
		state_type &dxdt,
		const time_type t,
		const time_type previous_observed_t)
	{
		// ******* system implementation *******
		input_type u;
		input(t,u);
		intermediate_type mid;
		intermediate_states(u,x,mid,t);

		// washouts dynamics
		hp_surge(u,x,dxdt);// influence hp surge x 1,2
		lp_surge(u,x,dxdt);// influence lp surge x 1,2
		hp_pitch(u,x,dxdt);
		
		// kinematics
		const double surge_vmax=platform::max_vel_surge;
		const double surge_xmax=platform::max_pos_surge;
		linear_kin_limits(mid(mids::ref_surge_acc),x,dxdt,surge_vmax,surge_xmax);
		linear_kinematics(mid(mids::ref_surge_acc),x,dxdt);

		// tilt rate limit
		const double surge_max_tilt_rate=platform::surge_tilt_threshold;
		dxdt(states::tilt_coordination_rate_limited)= // next line
			rate_limit(
				x(states::tilt_coordination_rate_limited),
				mid(mids::tilt_coordination),
				t,previous_observed_t,
				surge_max_tilt_rate
				);

		dxdt(states::position_pitch_theta)=mid(mids::scc_pitch_input);

	}

	static void observer(
		const state_type &x ,
		const double t,
		observer_type &ymat)
	{
		input_type u;
		intermediate_type mid;
		input(t,u);
		intermediate_states(u,x,mid,t);

		ymat(outputs::kin_surge_acc)=mid(mids::kin_surge_acc);
		ymat(outputs::kin_surge_vel)=mid(mids::kin_surge_vel);
		ymat(outputs::kin_surge_pos)=mid(mids::kin_surge_pos);
		ymat(outputs::hp_surge_out)=mid(mids::hp_surge_out);
		ymat(outputs::input_surge_acc)=u(inputs::surge_acc);
		ymat(outputs::specific_force_surge)=mid(mids::otolith_surge_input);

	}

	static void intermediate_states(const input_type &u,const state_type &x,intermediate_type &mid,const time_type t)
	{
		_unused(t);
		const double surge_vmax=platform::max_vel_surge;
		const double surge_xmax=platform::max_pos_surge;

		mid(mids::hp_surge_out)=hp_surge_out(u,x);
		mid(mids::lp_surge_out)=lp_surge_out(x);
		mid(mids::hp_pitch_out)=hp_pitch_out(u,x);

		const double surge_amax=platform::max_acc_surge;
		double ref_surge_acc=bound(mid(mids::hp_surge_out),-surge_amax,surge_amax);
		double kin_surge_acc=limited_surge_acc(x,ref_surge_acc,surge_vmax,surge_xmax);
		double kin_surge_vel=x(states::kin_surge_vel);//limited_surge_vel(x,surge_vmax,surge_xmax);
		double kin_surge_pos=x(states::kin_surge_pos);//limited_surge_pos(x);

		mid(mids::kin_surge_acc)=kin_surge_acc;
		mid(mids::kin_surge_vel)=kin_surge_vel;
		mid(mids::kin_surge_pos)=kin_surge_pos;
		mid(mids::ref_surge_acc)=ref_surge_acc;

		mid(mids::tilt_coordination)=mid(mids::lp_surge_out)/constants::g;
		mid(mids::scc_pitch_input)=mid(mids::hp_pitch_out)+x(states::tilt_coordination_rate_limited);
		mid(mids::otolith_surge_input)=mid(mids::hp_pitch_out)+constants::g*x(mids::scc_pitch_input);
	}

	static void input(const time_type t,input_type &u)
	{
		u(inputs::surge_acc)=(t>1?(t>7?0:1):0);
		u(inputs::pitch_angvel)=0;
	_(t);
	}

// protected:

	static double rate_limit(double limited,double ref,double t, double last_t,double rate_min,double rate_max)
	{
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

	static double rate_limit(double limited,double ref,double t, double last_t,double rate)
	{
		return rate_limit(limited,ref,t,last_t,-rate,+rate);
	}


	static double bound(double val,double min,double max)
	{
		return std::min(std::max(val,min),max);
	}

	static void hp_surge(const input_type &u,const state_type &x,state_type &dxdt)
	{
		// observable form
		const double w=platform::omega_hp_x;
		const double z=platform::zeta_hp_x;
		const int x1=states::hp_surge_x1;
		const int x2=states::hp_surge_x2;
		dxdt(x1)=-w*w*x(x2)-w*w*u(inputs::surge_acc);
		dxdt(x2)=x(x1)-2*z*w*x(x2)-2*z*w*u(inputs::surge_acc);
	}

	static double hp_surge_out(const input_type &u,const state_type &x)
	{
		return x(states::hp_surge_x2)+u(inputs::surge_acc);
	}

	static void lp_surge(const input_type &u,const state_type &x, state_type &dxdt)
	{
		// observable form
		const double w=platform::omega_lp_x;
		const double z=platform::zeta_lp_x;
		const int x1=states::lp_surge_x1;
		const int x2=states::lp_surge_x2;
		dxdt(x1)=-w*w*x(x2)+w*w*u(inputs::surge_acc);
		dxdt(x2)=x(x1)-2*z*w*x(x2);
	}

	static double lp_surge_out(const state_type &x)
	{
		return x(states::lp_surge_x2);
	}

	static void hp_pitch(const input_type &u,const state_type &x, state_type &dxdt)
	{
		// observable form
		const double w=platform::omega_hp_pitch;
		const int x1=states::hp_pitch_x1;
		dxdt(x1)=-w*x(x1)-w*u(inputs::surge_acc);
	}

	static double hp_pitch_out(const input_type &u,const state_type &x)
	{
		return x(states::hp_pitch_x1)+u(inputs::surge_acc);
	}

	static void linear_kin_limits(const double u,const state_type &x, state_type &dxdt,const double vmax,const double xmax)
	{
		// f(x) = d saturation(x) / dx
		// x0'=f(x0/amax)a
		// x1'=f(x1/vmax)x0
		const int x1=states::kin_limits_surge_vel;
		const int x2=states::kin_limits_surge_pos;
		dxdt(x1)=saturation_D1(x(x1)/vmax)*u;
		dxdt(x2)=saturation_D1(x(x2)/xmax)*x(x1);
	}


	static void linear_kinematics(const double acc_ref,const state_type &x, state_type &dxdt)
	{
		const int x1=states::kin_surge_vel;
		const int x2=states::kin_surge_pos;
		dxdt(x1)=acc_ref;
		dxdt(x2)=x(x1);
	}

	static double saturationD1_numinator()
	{
		static double numinator=(1+exp(platform::saturation_b));
		return numinator;
	}

	static double saturationD1_denuminator(double x)
	{
		double denuminator=(1+exp(platform::saturation_a*x+platform::saturation_b));
		return denuminator;
	}

	static double saturation_D1(double x)
	{  // derivative of saturation function
		return saturationD1_numinator()/saturationD1_denuminator(abs(x));
	}

	static double saturation_D2(double x)
	{	// second derivative of saturation function
		double n=saturationD1_numinator();
		double d=saturationD1_denuminator(abs(x));
		return -(n*platform::saturation_a*(d-1))/(d*d)*(x>=0?1:-1);
	}

	static double limited_surge_acc(const state_type &x,const double u,const double vmax,const double xmax)
	{
		const int x1=states::kin_limits_surge_vel;
		const int x2=states::kin_limits_surge_pos;
		return saturation_D1(x(x2)/xmax)*(saturation_D2(x(x2)/xmax)*x(x1)*x(x1)/xmax+saturation_D1(x(x1)/vmax)*u);
	}

	static double limited_surge_vel(const state_type &x,const double vmax,const double xmax)
	{
		const int x1=states::kin_limits_surge_vel;
		const int x2=states::kin_limits_surge_pos;
		return saturation_D1(x(x2)/xmax)*x(x1);
	}

	static double limited_surge_pos(const state_type &x)
	{
		const int x2=states::kin_limits_surge_pos;
		return x(x2);
	}

};