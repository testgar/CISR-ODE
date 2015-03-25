#pragma once

#include "solver.hpp"

NS_BNO_BEGIN

template<>
class runge_kutta<Steppers::RKDP5>
{

public :

    const unsigned short order_value = 5;
    const unsigned short stepper_order_value = 5;
    const unsigned short error_order_value = 4;

    runge_kutta(  )
        :  m_first_call( true )
    { }

    template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut , class Err >
    void do_step( System system , const StateIn &in , const DerivIn &dxdt_in , time_type t ,
            StateOut &out , DerivOut &dxdt_out , time_type dt , Err &xerr )
    {
        m_first_call = true;
        do_step_impl_v1( system , in , dxdt_in , t , out , dxdt_out , dt , xerr );
    }

    template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut , class Err >
    void do_step_impl_v1( System system , const StateIn &in , const DerivIn &dxdt_in , time_type t ,
            StateOut &out , DerivOut &dxdt_out , time_type dt , Err &xerr )
    {
        const value_type c1 = static_cast<value_type> ( 35 ) / static_cast<value_type>( 384 );
        const value_type c3 = static_cast<value_type> ( 500 ) / static_cast<value_type>( 1113 );
        const value_type c4 = static_cast<value_type> ( 125 ) / static_cast<value_type>( 192 );
        const value_type c5 = static_cast<value_type> ( -2187 ) / static_cast<value_type>( 6784 );
        const value_type c6 = static_cast<value_type> ( 11 ) / static_cast<value_type>( 84 );

        const value_type dc1 = c1 - static_cast<value_type> ( 5179 ) / static_cast<value_type>( 57600 );
        const value_type dc3 = c3 - static_cast<value_type> ( 7571 ) / static_cast<value_type>( 16695 );
        const value_type dc4 = c4 - static_cast<value_type> ( 393 ) / static_cast<value_type>( 640 );
        const value_type dc5 = c5 - static_cast<value_type> ( -92097 ) / static_cast<value_type>( 339200 );
        const value_type dc6 = c6 - static_cast<value_type> ( 187 ) / static_cast<value_type>( 2100 );
        const value_type dc7 = static_cast<value_type>( -1 ) / static_cast<value_type> ( 40 );

        do_step_impl_v2( system , in , dxdt_in , t , out , dxdt_out , dt );
        //error estimate

        xerr=dt*dc1*dxdt_in + dt*dc3*m_k3 + dt*dc4*m_k4 + dt*dc5*m_k5 + dt*dc6*m_k6 + dt*dc7*dxdt_out;

    }

    template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut >
    void do_step_impl_v2( System system , const StateIn &in , const DerivIn &dxdt_in , time_type t ,
            StateOut &out , DerivOut &dxdt_out , time_type dt )
    {
        const value_type a2 = static_cast<value_type> ( 1 ) / static_cast<value_type>( 5 );
        const value_type a3 = static_cast<value_type> ( 3 ) / static_cast<value_type> ( 10 );
        const value_type a4 = static_cast<value_type> ( 4 ) / static_cast<value_type> ( 5 );
        const value_type a5 = static_cast<value_type> ( 8 )/static_cast<value_type> ( 9 );

        const value_type b21 = static_cast<value_type> ( 1 ) / static_cast<value_type> ( 5 );

        const value_type b31 = static_cast<value_type> ( 3 ) / static_cast<value_type>( 40 );
        const value_type b32 = static_cast<value_type> ( 9 ) / static_cast<value_type>( 40 );

        const value_type b41 = static_cast<value_type> ( 44 ) / static_cast<value_type> ( 45 );
        const value_type b42 = static_cast<value_type> ( -56 ) / static_cast<value_type> ( 15 );
        const value_type b43 = static_cast<value_type> ( 32 ) / static_cast<value_type> ( 9 );

        const value_type b51 = static_cast<value_type> ( 19372 ) / static_cast<value_type>( 6561 );
        const value_type b52 = static_cast<value_type> ( -25360 ) / static_cast<value_type> ( 2187 );
        const value_type b53 = static_cast<value_type> ( 64448 ) / static_cast<value_type>( 6561 );
        const value_type b54 = static_cast<value_type> ( -212 ) / static_cast<value_type>( 729 );

        const value_type b61 = static_cast<value_type> ( 9017 ) / static_cast<value_type>( 3168 );
        const value_type b62 = static_cast<value_type> ( -355 ) / static_cast<value_type>( 33 );
        const value_type b63 = static_cast<value_type> ( 46732 ) / static_cast<value_type>( 5247 );
        const value_type b64 = static_cast<value_type> ( 49 ) / static_cast<value_type>( 176 );
        const value_type b65 = static_cast<value_type> ( -5103 ) / static_cast<value_type>( 18656 );

        const value_type c1 = static_cast<value_type> ( 35 ) / static_cast<value_type>( 384 );
        const value_type c3 = static_cast<value_type> ( 500 ) / static_cast<value_type>( 1113 );
        const value_type c4 = static_cast<value_type> ( 125 ) / static_cast<value_type>( 192 );
        const value_type c5 = static_cast<value_type> ( -2187 ) / static_cast<value_type>( 6784 );
        const value_type c6 = static_cast<value_type> ( 11 ) / static_cast<value_type>( 84 );

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


NS_BNO_END
