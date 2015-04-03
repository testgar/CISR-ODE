#pragma once

#include "solver.hpp"

NS_ODE_BEGIN

template<class ErrorStepper>
class controlled_runge_kutta<Steppers::RKF78,ErrorStepper>
{

public:

	typedef ErrorStepper stepper_type;
	typedef default_error_checker error_checker_type;

	controlled_runge_kutta(const value_type eps_abs,const value_type eps_rel)
			: m_stepper() , m_error_checker( eps_abs, eps_rel)
	{ }

	template< class System  >
	controlled_step_result try_step( System system , state_type &x , time_type &t , time_type &dt )
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
	controlled_step_result try_step( System system , const state_type &in , const deriv_type &dxdt_in , time_type &t , state_type &out , time_type &dt )
	{
 
		// do one step with error calculationm_stepper
		m_stepper.do_step( system , in , dxdt_in , t , out , dt , m_xerr );

		m_max_rel_error = m_error_checker.error( in , dxdt_in , m_xerr , dt );

		if( m_max_rel_error > 1.0 )
		{
			// error too large - decrease dt ,limit scaling factor to 0.2 and reset state
			dt *= std::max( value_type( value_type(9)/value_type(10) *
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
				dt *= value_type(9)/value_type(10) * pow( m_max_rel_error ,
						value_type(-1) / m_stepper.stepper_order_value );
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
	value_type m_max_rel_error;
};

NS_ODE_END
