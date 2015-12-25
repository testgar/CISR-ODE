#pragma once

#include "solver.hpp"

NS_ODEV1_BEGIN

template<class ErrorStepper>
class controlled_runge_kutta<Steppers::RKCK54,ErrorStepper>
{

public:

	typedef ErrorStepper stepper_type;
	typedef default_error_checker error_checker_type;

	controlled_runge_kutta(const value_type eps_abs,const value_type eps_rel,const time_type max_dt)
	: m_stepper() , m_error_checker( eps_abs, eps_rel), max_dt(max_dt)
	{ }

	template< class System  >
	controlled_step_result try_step( System &system , state_type &x , time_type &t , time_type &dt )
	{
		system.rhs( x , m_dxdt , t );
		controlled_step_result res = try_step( system , x , m_dxdt , t , m_xnew , dt );
		if( res == success )
		{
			x=m_xnew;
		}
		return res;
	}

	template< class System >
	controlled_step_result try_step( System &system , const state_type &in , const deriv_type &dxdt_in , time_type &t , state_type &out , time_type &dt )
	{
		// do one step with error calculationm_stepper
		m_stepper.do_step( system , in , dxdt_in , t , out , dt , m_xerr );

		m_max_rel_error = m_error_checker.error( in , dxdt_in , m_xerr , dt );

		if( m_max_rel_error > 1.0 )
		{
			// error too large - decrease dt ,limit scaling factor to 0.2 and reset state
			dt *= std::max ( value_type( value_type(9)/value_type(10) *
														 pow( m_max_rel_error , value_type(-1) / ( m_stepper.error_order_value - 1 ) ) ) ,
														 value_type( value_type(1)/value_type(5) ) );
			return fail;
		}
		else
		{
			if( m_max_rel_error < 0.5 )
			{
				// error should be > 0
				m_max_rel_error = std::max ( value_type( pow( value_type(5.0) , -value_type(m_stepper.stepper_order_value) ) ) ,
							m_max_rel_error );
				//error too small - increase dt and keep the evolution and limit scaling factor to 5.0
				t += dt;
				dt *= value_type(9)/value_type(10) * pow( m_max_rel_error, value_type(-1)/m_stepper.stepper_order_value );
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

	// value_type last_error( void ) const
	// {
	//	 return m_max_rel_error;
	// }

private:

	stepper_type m_stepper;
	error_checker_type m_error_checker;

	deriv_type m_dxdt;
	state_type m_xerr;
	state_type m_xnew;
	value_type m_max_rel_error;
	const time_type max_dt;
};


template<>
class runge_kutta<Steppers::RKCK54>
{

public:

	const unsigned short order_value = 5;
	const unsigned short stepper_order_value = 5;
	const unsigned short error_order_value = 4;

	runge_kutta(  )
	{ }

	template< class System >
	void do_step( System &system , const state_type &in , const deriv_type &dxdt_in , time_type t ,
			state_type &out , time_type dt , state_type &xerr )
	{
		do_step_impl_v1( system , in , dxdt_in , t , out , dt , xerr );
	}

	template< class System  >
	void do_step_impl_v1( System &system , const state_type &in , const deriv_type &dxdt_in , time_type t , state_type &out , time_type dt , state_type &xerr )
	{
		const value_type c1 = value_type( 37 ) / value_type( 378 );
		const value_type c3 = value_type( 250 ) / value_type( 621 );
		const value_type c4 = value_type( 125 ) / value_type( 594 );
		const value_type c6 = value_type( 512 ) / value_type( 1771 );

		const value_type dc1 = c1 - value_type( 2825 ) / value_type( 27648 );
		const value_type dc3 = c3 - value_type( 18575 ) / value_type( 48384 );
		const value_type dc4 = c4 - value_type( 13525 ) / value_type( 55296 );
		const value_type dc5 = value_type( -277 ) / value_type( 14336 );
		const value_type dc6 = c6 - value_type( 1 ) / value_type( 4 );

		do_step_impl_v2( system , in , dxdt_in , t , out , dt );

		//error estimate
		xerr=dt*dc1*dxdt_in + dt*dc3*m_k3 + dt*dc4*m_k4 + dt*dc5*m_k5 + dt*dc6*m_k6;

	}

	template< class System , class StateIn , class DerivIn , class StateOut >
	void do_step_impl_v2( System &system , const StateIn &in , const DerivIn &dxdt_in , time_type t , StateOut &out , time_type dt )
	{
		const value_type a2 = value_type( 1 ) / value_type( 5 );
		const value_type a3 = value_type( 3 ) / value_type( 10 );
		const value_type a4 = value_type( 3 ) / value_type( 5 );
		const value_type a6 = value_type( 7 ) / value_type( 8 );

		const value_type b21 = value_type( 1 ) / value_type( 5 );
		const value_type b31 = value_type( 3 ) / value_type( 40 );
		const value_type b32 = value_type( 9 ) / value_type( 40 );
		const value_type b41 = value_type( 3 ) / value_type( 10 );
		const value_type b42 = value_type( -9 ) / value_type( 10 );
		const value_type b43 = value_type( 6 ) / value_type( 5 );
		const value_type b51 = value_type( -11 ) / value_type( 54 );
		const value_type b52 = value_type( 5 ) / value_type( 2 );
		const value_type b53 = value_type( -70 ) / value_type( 27 );
		const value_type b54 = value_type( 35 ) / value_type( 27 );
		const value_type b61 = value_type( 1631 ) / value_type( 55296 );
		const value_type b62 = value_type( 175 ) / value_type( 512 );
		const value_type b63 = value_type( 575 ) / value_type( 13824 );
		const value_type b64 = value_type( 44275 ) / value_type( 110592 );
		const value_type b65 = value_type( 253 ) / value_type( 4096 );

		const value_type c1 = value_type( 37 ) / value_type( 378 );
		const value_type c3 = value_type( 250 ) / value_type( 621 );
		const value_type c4 = value_type( 125 ) / value_type( 594 );
		const value_type c6 = value_type( 512 ) / value_type( 1771 );

		typename std::remove_reference< System >::type &sys = system;

		m_x_tmp= in + dt*b21 * dxdt_in;

		sys.rhs( m_x_tmp , m_k2 , t + dt*a2 );
		m_x_tmp= in + dt*b31*dxdt_in + dt*b32*m_k2; // why cannot factor dt????

		sys.rhs( m_x_tmp , m_k3 , t + dt*a3 );
		m_x_tmp=in+dt*b41*dxdt_in+dt*b42*m_k2+dt*b43*m_k3;

		sys.rhs( m_x_tmp, m_k4 , t + dt*a4 );
		m_x_tmp=in+dt*b51*dxdt_in+dt*b52*m_k2+ dt*b53*m_k3+dt*b54*m_k4;

		sys.rhs( m_x_tmp , m_k5 , t + dt );
		m_x_tmp=in + dt*b61*dxdt_in + dt*b62*m_k2 + dt*b63*m_k3 + dt*b64*m_k4 + dt*b65*m_k5 ;

		sys.rhs( m_x_tmp , m_k6 , t + dt*a6 );
		out= in + dt*c1*dxdt_in + dt*c3*m_k3 + dt*c4*m_k4 + dt*c6*m_k6;
	}

private:
 //   bool m_first_call;

	state_type m_x_tmp;
	deriv_type m_k2 , m_k3 , m_k4 , m_k5 , m_k6 ;
	deriv_type m_dxdt_tmp;
};

NS_ODEV1_END
