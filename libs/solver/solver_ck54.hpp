#pragma once

#include "solver.hpp"

NS_ODE_BEGIN

template<class System>
class controlled_runge_kutta<Steppers::RKCK54,System>
{
protected:
	typedef typename System::value_type value_type;
	typedef typename System::state_type state_type;
	typedef typename System::deriv_type deriv_type;
	typedef typename System::time_type time_type;

public:

	controlled_runge_kutta(const value_type eps_abs,const value_type eps_rel)
	: m_error_checker( eps_abs, eps_rel)
	{ }

	controlled_step_result try_step( System &system , state_type &x , time_type &t , time_type &dt )
	{
		system.rhs( x , m_dxdt , t );
		controlled_step_result res = try_step2( system , x , m_dxdt , t , m_xnew , dt );
		if( res == success )
		{
			x=m_xnew;
		}
		return res;
	}

	controlled_step_result try_step2( System &system , const state_type &x_in , const deriv_type &dxdt_in , time_type &t , state_type &x_out , time_type &dt )
	{
		bool dt_overwritten=false;
		if(system.min_dt>0 && dt<system.min_dt) // Help C++ optimizer
		{
			dt=system.min_dt; // added for minimum step size
			dt_overwritten=true;
		}
		if(system.max_dt>0 && dt>system.max_dt) // Help C++ optimizer
		{
			dt=system.max_dt; // added for maximum step size
			dt_overwritten=true;
		}

		// do one step with error calculationm_stepper
		do_step( system , x_in , dxdt_in , t , x_out , dt , m_xerr );

		m_max_rel_error = m_error_checker.error( x_in , dxdt_in , m_xerr , dt );

		if( m_max_rel_error > 1.0 && !dt_overwritten)
		{
			// error too large - decrease dt ,limit scaling factor to 0.2 and reset state
			dt *= std::max ( value_type( value_type(9)/value_type(10) *
						pow( m_max_rel_error , value_type(-1) / ( error_order_value - 1 ) ) ) ,
						value_type( value_type(1)/value_type(5) ) );
			return fail;
		}
		else
		{
			t += dt;
			if( m_max_rel_error < 0.5 )
			{
				// error should be > 0
				m_max_rel_error = std::max ( value_type( pow( value_type(5.0) , -value_type(stepper_order_value) ) ) , m_max_rel_error );
				//error too small - increase dt and keep the evolution and limit scaling factor to 5.0
				dt *= value_type(9)/value_type(10) * pow( m_max_rel_error, value_type(-1)/stepper_order_value );
			}
			return success;
		}
	}

private:

	default_error_checker<System> m_error_checker;

	deriv_type m_dxdt;
	state_type m_xerr;
	state_type m_xnew;
	value_type m_max_rel_error;

public:

	const unsigned short order_value = 5;
	const unsigned short stepper_order_value = 5;
	const unsigned short error_order_value = 4;

	void do_step(System &system , const state_type &x_in , const deriv_type &dxdt_in , time_type t ,
			state_type &x_out , time_type dt , state_type &xerr )
	{
		const value_type a2 = value_type( 1 ) / value_type( 5 );
		const value_type a3 = value_type( 3 ) / value_type( 10 );
		const value_type a4 = value_type( 3 ) / value_type( 5 );
		const value_type a5 = value_type( 1 );
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

		const value_type dc1 = c1 - value_type( 2825 ) / value_type( 27648 );
		const value_type dc3 = c3 - value_type( 18575 ) / value_type( 48384 );
		const value_type dc4 = c4 - value_type( 13525 ) / value_type( 55296 );
		const value_type dc5 = value_type( -277 ) / value_type( 14336 );
		const value_type dc6 = c6 - value_type( 1 ) / value_type( 4 );

		m_x_tmp= x_in + dt*b21 * dxdt_in;

		system.rhs( m_x_tmp , m_k2 , t + dt*a2 );
		m_x_tmp= x_in + dt*b31*dxdt_in + dt*b32*m_k2; // why cannot factor dt????

		system.rhs( m_x_tmp , m_k3 , t + dt*a3 );
		m_x_tmp=x_in+dt*b41*dxdt_in+dt*b42*m_k2+dt*b43*m_k3;

		system.rhs( m_x_tmp, m_k4 , t + dt*a4 );
		m_x_tmp=x_in+dt*b51*dxdt_in+dt*b52*m_k2+ dt*b53*m_k3+dt*b54*m_k4;

		system.rhs( m_x_tmp , m_k5 , t + dt*a5 );
		m_x_tmp=x_in + dt*b61*dxdt_in + dt*b62*m_k2 + dt*b63*m_k3 + dt*b64*m_k4 + dt*b65*m_k5 ;

		system.rhs( m_x_tmp , m_k6 , t + dt*a6 );
		x_out= x_in + dt*c1*dxdt_in + dt*c3*m_k3 + dt*c4*m_k4 + dt*c6*m_k6;

		//error estimate
		xerr=dt*dc1*dxdt_in + dt*dc3*m_k3 + dt*dc4*m_k4 + dt*dc5*m_k5 + dt*dc6*m_k6;
	}

private:
	state_type m_x_tmp;
	deriv_type m_k2 , m_k3 , m_k4 , m_k5 , m_k6 ;
};

NS_ODE_END
