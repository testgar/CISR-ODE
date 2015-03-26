#pragma once

#include <boost/numeric/ublas/vector.hpp>

#ifndef __CUDACC__
#include <boost/units/quantity.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/units/get_system.hpp>
#endif

// #define NS_BNO_BEGIN namespace boost {namespace numeric {namespace odeint {
// #define NS_BNO_END }}}

#define NS_BNO_BEGIN namespace ode {
#define NS_BNO_END }

NS_BNO_BEGIN

enum class Steppers { RKDP5, RKCK45, RKF78 };

class default_error_checker
{
public:


    default_error_checker(
            value_type eps_abs = static_cast< value_type >( 1.0e-6 ) ,
            value_type eps_rel = static_cast< value_type >( 1.0e-6 ) ,
            value_type a_x = static_cast< value_type >( 1 ) ,
            value_type a_dxdt = static_cast< value_type >( 1 ) )
    : m_eps_abs( eps_abs ) , m_eps_rel( eps_rel ) , m_a_x( a_x ) , m_a_dxdt( a_dxdt )
    { }


    value_type error(  const state_type &x_old , const deriv_type &dxdt_old , state_type &x_err , time_type dt ) const
    {
        // this overwrites x_err !
        x_err=arma::abs( x_err ) / ( m_eps_abs + m_eps_rel * ( m_a_x * arma::abs( x_old ) + m_a_dxdt * dt * abs( dxdt_old ) ) );

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
    fail      // < The step was not successful and might possibly be repeated with a small step size.
} controlled_step_result;


//template< enum Steppers >
//class controlled_runge_kutta;


template<enum Steppers>
class runge_kutta;

template<enum Steppers,class ErrorStepper>
class controlled_runge_kutta;

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
    controlled_step_result try_step( System system , state_type &x , time_type &t , time_type &dt )
    {
        if( m_first_call )
        {
            // initialize
             // typename std::remove_reference< System >::type &sys = system;
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
    controlled_step_result try_step( System system , const state_type &in , const deriv_type &dxdt_in , time_type &t ,
            state_type &out , deriv_type &dxdt_out , time_type &dt)
    {
        using namespace std;

        m_stepper.do_step( system , in , dxdt_in , t , out , dxdt_out , dt , m_xerr );

        // this potentially overwrites m_x_err! (standard_error_checker does, at least)
        value_type max_rel_err = m_error_checker.error(  in , dxdt_in , m_xerr , dt );

        if( max_rel_err > 1.0 )
        {
            // error too large - decrease dt ,limit scaling factor to 0.2 and reset state
            // simplified: max( 9/10*pow( max_rel_err , -1/( m_stepper.error_order_value - 1) ) ,1/5 );
            dt *= max( static_cast<value_type>( static_cast<value_type>(9)/static_cast<value_type>(10) *
                                                                 pow( max_rel_err , static_cast<value_type>(-1) / ( m_stepper.error_order_value - 1 ) ) ) ,
                                                         static_cast<value_type>( static_cast<value_type>(1)/static_cast<value_type> (5)) );
            return fail;
        }
        else
        {
            if( max_rel_err < 0.5 )
            {   //error too small - increase dt and keep the evolution and limit scaling factor to 5.0
                // error should be > 0
                max_rel_err = max( static_cast<value_type>( pow( static_cast<value_type>(5.0) , -static_cast<value_type>(m_stepper.stepper_order_value) ) ) ,
                                                                     max_rel_err );
                t += dt;
                // simplified: 9/10 * pow( max_rel_err , -1 / m_stepper.stepper_order_value )
                dt *= static_cast<value_type>( static_cast<value_type>(9)/static_cast<value_type>(10) * pow( max_rel_err , static_cast<value_type>(-1) / m_stepper.stepper_order_value ) );
                dt=min(dt,max_dt); // added for maximum step size
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


template<class ErrorStepper>
class controlled_runge_kutta<Steppers::RKCK45,ErrorStepper>
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
        using namespace std;
        // do one step with error calculationm_stepper
        m_stepper.do_step( system , in , dxdt_in , t , out , dt , m_xerr );

        m_max_rel_error = m_error_checker.error( in , dxdt_in , m_xerr , dt );

        if( m_max_rel_error > 1.0 )
        {
            // error too large - decrease dt ,limit scaling factor to 0.2 and reset state
            dt *= max ( static_cast<value_type>( static_cast<value_type>(9)/static_cast<value_type>(10) *
                                                         pow( m_max_rel_error , static_cast<value_type>(-1) / ( m_stepper.error_order_value - 1 ) ) ) ,
                                                         static_cast<value_type>( static_cast<value_type>(1)/static_cast<value_type> (5) ) );
            return fail;
        }
        else
        {
            if( m_max_rel_error < 0.5 )
            {
                // error should be > 0
                m_max_rel_error = max ( static_cast<value_type>( pow( static_cast<value_type>(5.0) , -static_cast<value_type>(m_stepper.stepper_order_value) ) ) ,
                            m_max_rel_error );
                //error too small - increase dt and keep the evolution and limit scaling factor to 5.0
                t += dt;
                dt *= static_cast<value_type>(9)/static_cast<value_type>(10) * pow( m_max_rel_error ,
                                                                                    static_cast<value_type>(-1) / m_stepper.stepper_order_value );
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
    //     return m_max_rel_error;
    // }

private:

    stepper_type m_stepper;
    error_checker_type m_error_checker;

    deriv_type m_dxdt;
    state_type m_xerr;
    state_type m_xnew;
    value_type m_max_rel_error;
};

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
        using namespace std;
 
        // do one step with error calculationm_stepper
        m_stepper.do_step( system , in , dxdt_in , t , out , dt , m_xerr );

        m_max_rel_error = m_error_checker.error( in , dxdt_in , m_xerr , dt );

        if( m_max_rel_error > 1.0 )
        {
            // error too large - decrease dt ,limit scaling factor to 0.2 and reset state
            dt *= max ( static_cast<value_type>( static_cast<value_type>(9)/static_cast<value_type>(10) *
                            pow( m_max_rel_error , static_cast<value_type>(-1) / ( m_stepper.error_order_value - 1 ) ) ) ,
                    static_cast<value_type>( static_cast<value_type>(1)/static_cast<value_type> (5) ) );
            return fail;
        }
        else
        {
            if( m_max_rel_error < 0.5 )
            {
                // error should be > 0
                m_max_rel_error = max ( static_cast<value_type>( pow( static_cast<value_type>(5.0) , -static_cast<value_type>(m_stepper.stepper_order_value) ) ) ,
                        m_max_rel_error );
                //error too small - increase dt and keep the evolution and limit scaling factor to 5.0
                t += dt;
                dt *= static_cast<value_type>(9)/static_cast<value_type>(10) * pow( m_max_rel_error ,
                        static_cast<value_type>(-1) / m_stepper.stepper_order_value );
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
    //     return m_max_rel_error;
    // }

private:

    stepper_type m_stepper;
    error_checker_type m_error_checker;

    deriv_type m_dxdt;
    state_type m_xerr;
    state_type m_xnew;
    value_type m_max_rel_error;
};

#define EPSILON std::numeric_limits<time_type>::epsilon()

template<Steppers SolverMethod, class System>
class Solver //<SolverMethod::RKDP5,System>
{

    typedef typename std::remove_reference<System>::type system_type;
    typedef controlled_runge_kutta<SolverMethod,runge_kutta<SolverMethod>> stepper_type;

    system_type &system;
    stepper_type stepper;

public:

    Solver(system_type &_system): system(_system), stepper(system.eps_abs,system.eps_rel,system.max_dt)
    {
        // constructor
    }

    size_t integrate_adaptive(
            state_type &start_state ,
            time_type start_time ,
            time_type end_time ,
            time_type dt )
    {
        using namespace std;

        //typename std::remove_reference< Observer >::type &obs = system.observer;
        const size_t max_attempts = 1000;
        const char *error_string = "Integrate adaptive : Maximal number of iterations reached. A step size could not be found.";
        size_t count = 0;
        time_type next_interrupt_time=system.timer(start_state,start_time);
        time_type next_urgent_time=system.next_urgent_time;
        dt=min(dt,system.max_dt); // what if system.max_dt is mismatches with stepper.max_dt
        time_type force_point;
        while(less_with_sign(start_time,end_time,dt))
        {
            if(abs(start_time-next_interrupt_time)<EPSILON)
                next_interrupt_time=system.timer(start_state,start_time);
            system.observer(start_state,start_time,dt);
            force_point=min(end_time,next_interrupt_time); // minimum
            if(next_urgent_time-start_time>EPSILON)
                force_point=min(force_point,next_urgent_time);
            if(less_with_sign(force_point,static_cast<time_type>(start_time + dt) , dt ) )
            {
                dt= force_point - start_time;
                if(abs(dt)<EPSILON)
                    throw std::runtime_error("dt was proposed to be zero!");
                dt= min(dt,system.max_dt);// what if system.max_dt is mismatches with stepper.max_dt
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

NS_BNO_END
