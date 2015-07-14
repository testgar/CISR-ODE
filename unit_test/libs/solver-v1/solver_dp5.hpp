#pragma once

#include "solver.hpp"

NS_ODEV1_BEGIN

template<class ErrorStepper>
class controlled_runge_kutta<Steppers::RKDP5,ErrorStepper>
{

public:

	typedef ErrorStepper stepper_type;
	typedef default_error_checker error_checker_type;

	controlled_runge_kutta(const value_type eps_abs,const value_type eps_rel,const time_type max_dt)
	: m_stepper() , m_error_checker( eps_abs, eps_rel) ,
	  m_first_call( true ), max_dt(max_dt)
	{ }

	template< class System  >
	controlled_step_result try_step( System &system , state_type &x , time_type &t , time_type &dt )
	{
		if( m_first_call )
		{
			system.rhs( x , m_dxdt , t );
			m_first_call = false;
		}

		controlled_step_result res = try_step( system , x , m_dxdt , t , m_xnew , m_dxdtnew , dt );

		if( res == success )
		{
			x=m_xnew;
			m_dxdt=m_dxdtnew;
		}
		return res;
	}

	//call 4
	template<class System >
	controlled_step_result try_step( System &system , const state_type &in , const deriv_type &dxdt_in , time_type &t ,
			state_type &out , deriv_type &dxdt_out , time_type &dt)
	{
		m_stepper.do_step( system , in , dxdt_in , t , out , dxdt_out , dt , m_xerr );

		// this potentially overwrites m_x_err! (standard_error_checker does, at least)
		value_type max_rel_err = m_error_checker.error(  in , dxdt_in , m_xerr , dt );

		if( max_rel_err > 1.0 )
		{
			// error too large - decrease dt ,limit scaling factor to 0.2 and reset state
			// simplified: max( 9/10*pow( max_rel_err , -1/( m_stepper.error_order_value - 1) ) ,1/5 );
			dt *= std::max( value_type( value_type(9)/value_type(10) *
																 pow( max_rel_err , value_type(-1) / ( m_stepper.error_order_value - 1 ) ) ) ,
														 value_type( value_type(1)/value_type(5)) );
			return fail;
		}
		else
		{
			if( max_rel_err < 0.5 )
			{   //error too small - increase dt and keep the evolution and limit scaling factor to 5.0
				// error should be > 0
				max_rel_err = std::max( value_type( pow( value_type(5.0) , -value_type(m_stepper.stepper_order_value) ) ) ,
																	 max_rel_err );
				t += dt;
				// simplified: 9/10 * pow( max_rel_err , -1 / m_stepper.stepper_order_value )
				dt *= value_type( value_type(9)/value_type(10) * pow( max_rel_err , value_type(-1) / m_stepper.stepper_order_value ) );
				dt=std::min(dt,max_dt); // added for maximum step size
				return success;
			}
			else
			{
				t += dt;
				return success;
			}
		}
	}

private:

	stepper_type m_stepper;
	error_checker_type m_error_checker;

	deriv_type m_dxdt;
	state_type m_xerr;
	state_type m_xnew;
	deriv_type m_dxdtnew;
	bool m_first_call;
	const time_type max_dt;
};


template<>
class runge_kutta<Steppers::RKDP5>
{

public:

	const unsigned short order_value = 5;
	const unsigned short stepper_order_value = 5;
	const unsigned short error_order_value = 4;

	runge_kutta(  )
		:  m_first_call( true )
	{ }

	template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut , class Err >
	void do_step(System &system , const StateIn &in , const DerivIn &dxdt_in , time_type t ,
			StateOut &out , DerivOut &dxdt_out , time_type dt , Err &xerr )
	{
		m_first_call = true;
		do_step_impl_v1( system , in , dxdt_in , t , out , dxdt_out , dt , xerr );
	}

	template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut , class Err >
	void do_step_impl_v1( System &system , const StateIn &in , const DerivIn &dxdt_in , time_type t ,
			StateOut &out , DerivOut &dxdt_out , time_type dt , Err &xerr )
	{
		const value_type c1 = value_type( 35 ) / value_type( 384 );
		const value_type c3 = value_type( 500 ) / value_type( 1113 );
		const value_type c4 = value_type( 125 ) / value_type( 192 );
		const value_type c5 = value_type( -2187 ) / value_type( 6784 );
		const value_type c6 = value_type( 11 ) / value_type( 84 );

		const value_type dc1 = c1 - value_type( 5179 ) / value_type( 57600 );
		const value_type dc3 = c3 - value_type( 7571 ) / value_type( 16695 );
		const value_type dc4 = c4 - value_type( 393 ) / value_type( 640 );
		const value_type dc5 = c5 - value_type( -92097 ) / value_type( 339200 );
		const value_type dc6 = c6 - value_type( 187 ) / value_type( 2100 );
		const value_type dc7 = value_type( -1 ) / value_type( 40 );

		do_step_impl_v2( system , in , dxdt_in , t , out , dxdt_out , dt );
		//error estimate

		xerr=dt*dc1*dxdt_in + dt*dc3*m_k3 + dt*dc4*m_k4 + dt*dc5*m_k5 + dt*dc6*m_k6 + dt*dc7*dxdt_out;
	}

	template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut >
	void do_step_impl_v2( System &system , const StateIn &in , const DerivIn &dxdt_in , time_type t ,
			StateOut &out , DerivOut &dxdt_out , time_type dt )
	{
		const value_type a2 = value_type( 1 ) / value_type( 5 );
		const value_type a3 = value_type( 3 ) / value_type( 10 );
		const value_type a4 = value_type( 4 ) / value_type( 5 );
		const value_type a5 = value_type( 8 )/value_type( 9 );

		const value_type b21 = value_type( 1 ) / value_type( 5 );

		const value_type b31 = value_type( 3 ) / value_type( 40 );
		const value_type b32 = value_type( 9 ) / value_type( 40 );

		const value_type b41 = value_type( 44 ) / value_type( 45 );
		const value_type b42 = value_type( -56 ) / value_type( 15 );
		const value_type b43 = value_type( 32 ) / value_type( 9 );

		const value_type b51 = value_type( 19372 ) / value_type( 6561 );
		const value_type b52 = value_type( -25360 ) / value_type( 2187 );
		const value_type b53 = value_type( 64448 ) / value_type( 6561 );
		const value_type b54 = value_type( -212 ) / value_type( 729 );

		const value_type b61 = value_type( 9017 ) / value_type( 3168 );
		const value_type b62 = value_type( -355 ) / value_type( 33 );
		const value_type b63 = value_type( 46732 ) / value_type( 5247 );
		const value_type b64 = value_type( 49 ) / value_type( 176 );
		const value_type b65 = value_type( -5103 ) / value_type( 18656 );

		const value_type c1 = value_type( 35 ) / value_type( 384 );
		const value_type c3 = value_type( 500 ) / value_type( 1113 );
		const value_type c4 = value_type( 125 ) / value_type( 192 );
		const value_type c5 = value_type( -2187 ) / value_type( 6784 );
		const value_type c6 = value_type( 11 ) / value_type( 84 );

		typename std::remove_reference< System >::type &sys = system;
	

		m_x_tmp= in + dt*b21 * dxdt_in;

		sys.rhs( m_x_tmp , m_k2 , t + dt*a2 );
		m_x_tmp= in + dt*b31*dxdt_in + dt*b32*m_k2; // why cannot factor dt????

		sys.rhs( m_x_tmp , m_k3 , t + dt*a3 );
		m_x_tmp=in+dt*b41*dxdt_in+dt*b42*m_k2+dt*b43*m_k3;

		sys.rhs( m_x_tmp, m_k4 , t + dt*a4 );
		m_x_tmp=in+dt*b51*dxdt_in+dt*b52*m_k2+ dt*b53*m_k3+dt*b54*m_k4;

		sys.rhs( m_x_tmp , m_k5 , t + dt*a5 );
		m_x_tmp=in + dt*b61*dxdt_in + dt*b62*m_k2 + dt*b63*m_k3 + dt*b64*m_k4 + dt*b65*m_k5 ;

		sys.rhs( m_x_tmp , m_k6 , t + dt );
		out= in + dt*c1*dxdt_in + dt*c3*m_k3 + dt*c4*m_k4 + dt*c5*m_k5 + dt*c6*m_k6;

		// the new derivative
		sys.rhs( out , dxdt_out , t + dt );
	}


private:
	bool m_first_call;

	state_type m_x_tmp;
	deriv_type m_k2 , m_k3 , m_k4 , m_k5 , m_k6 ;
	deriv_type m_dxdt_tmp;
};

NS_ODEV1_END
