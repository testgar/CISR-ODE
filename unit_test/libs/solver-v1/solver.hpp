#pragma once

#include <boost/numeric/ublas/vector.hpp>

#ifndef __CUDACC__
#include <boost/units/quantity.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/units/get_system.hpp>
#endif

#define NS_ODEV1_BEGIN namespace odev1 {
#define NS_ODEV1_END }

NS_ODEV1_BEGIN

// types begin
typedef double value_type;
typedef double time_type;
typedef arma::vec::fixed<state_size> state_type;
typedef state_type deriv_type;
// typedef arma::vec::fixed<state_size> observer_type;
// typedef arma::mat::fixed<buffer_headers,buffer_size> buffer_type;
// types end

enum class Steppers { RKDP5, RKCK54, RKF78 };

class default_error_checker
{
public:

	default_error_checker(
			value_type eps_abs = static_cast<value_type>( 1.0e-6 ) ,
			value_type eps_rel = static_cast<value_type>( 1.0e-6 ) ,
			value_type a_x = static_cast< value_type >( 1 ) ,
			value_type a_dxdt = static_cast< value_type >( 1 ) )
	: m_eps_abs( eps_abs ) , m_eps_rel( eps_rel ) , m_a_x( a_x ) , m_a_dxdt( a_dxdt )
	{ }

	value_type error(const state_type &x_old , const deriv_type &dxdt_old , state_type &x_err , time_type dt ) const
	{
		// this overwrites x_err !
		x_err=arma::abs( x_err ) / ( m_eps_abs + m_eps_rel * ( m_a_x * arma::abs( x_old ) + m_a_dxdt * dt * arma::abs( dxdt_old ) ) );
		return arma::max( x_err );
	}

private:

	value_type m_eps_abs;
	value_type m_eps_rel;
	value_type m_a_x;
	value_type m_a_dxdt;

};

typedef enum
{
	success , // < The trial step was successful, hence the state and the time have been advanced.
	fail	  // < The step was not successful and might possibly be repeated with a small step size.
} controlled_step_result;

template<enum Steppers>
class runge_kutta;

template<enum Steppers,class ErrorStepper>
class controlled_runge_kutta;

#define EPSILON (std::numeric_limits<time_type>::epsilon())

template<Steppers SolverMethod, class System>
class Solver
{

	typedef typename std::remove_reference<System>::type system_type;
	typedef controlled_runge_kutta<SolverMethod,runge_kutta<SolverMethod>> stepper_type;

	system_type &system;
	stepper_type stepper;

public:

	Solver(system_type &_system):
		system(_system),
		stepper(system.eps_abs,system.eps_rel,system.max_dt)
	{
		// constructor
	}

	size_t integrate_adaptive(
			state_type &start_state ,
			time_type start_time ,
			time_type end_time ,
			time_type dt )
	{
		const size_t max_attempts = 1000;
		const char *error_string = "Integrate adaptive : Maximal number of iterations reached. A step size could not be found.";
		size_t count = 0;
		time_type next_interrupt_time=system.timer(start_state,start_time);
		time_type next_sudden_change_time=system.next_sudden_change_time;
		dt=std::min(dt,system.max_dt); // what if system.max_dt is mismatches with stepper.max_dt
		time_type force_point;
		while(less_with_sign(start_time,end_time,dt))
		{
			if(std::abs(start_time-next_interrupt_time)<EPSILON)
				next_interrupt_time=system.timer(start_state,start_time);
			system.observer(start_state,start_time,dt);
			force_point=std::min(end_time,next_interrupt_time); // minimum
			if(next_sudden_change_time-start_time>EPSILON)
				force_point=std::min(force_point,next_sudden_change_time);
			if(less_with_sign(force_point,static_cast<time_type>(start_time + dt) , dt ) )
			{
				dt= force_point - start_time;
				if(std::abs(dt)<EPSILON)
					throw std::runtime_error("dt was proposed to be zero!");
				// dt= std::max(dt,system.min_dt);// what if system.min_dt is mismatches with stepper.min_dt
			}
			size_t trials = 0;
			controlled_step_result res = success;
			do
			{
				res = stepper.try_step( system , start_state , start_time , dt );
				++trials;
			}
			while( ( res == fail ) && ( trials < max_attempts ) );
			if( trials == max_attempts )
				throw std::overflow_error(error_string);
			++count;
		}
		system.observer( start_state , start_time, dt );
		return count;
	}

private:
	bool less_with_sign( time_type t1 , time_type t2 , time_type dt )
	{
		if( dt > 0 )
			//return t1 < t2;
			return t2-t1 > EPSILON;
		else
			//return t1 > t2;
			return t1-t2 > EPSILON;
	}
};

#undef EPSILON 

NS_ODEV1_END
