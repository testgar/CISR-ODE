#pragma once

#include "solver.hpp"

NS_BNO_BEGIN

template<>
class runge_kutta<Steppers::RKCK45>
{

public :

    const unsigned short order_value = 5;
    const unsigned short stepper_order_value = 5;
    const unsigned short error_order_value = 4;

    runge_kutta(  )
  //      :  m_first_call( true )
    { }

    template< class System >
    void do_step( System system , const state_type &in , const deriv_type &dxdt_in , time_type t ,
            state_type &out , time_type dt , state_type &xerr )
    {
        //m_first_call = true;
        do_step_impl_v1( system , in , dxdt_in , t , out /*, dxdt_out*/ , dt , xerr );
    }

    template< class System  >
    void do_step_impl_v1( System system , const state_type &in , const deriv_type &dxdt_in , time_type t , state_type &out , time_type dt , state_type &xerr )
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

        // stepper_base_type::m_algebra.for_each6( xerr , dxdt , m_k3.m_v , m_k4.m_v , m_k5.m_v , m_k6.m_v ,
        //         typename operations_type::template scale_sum5< time_type , time_type , time_type , time_type , time_type >( dt*dc1 , dt*dc3 , dt*dc4 , dt*dc5 , dt*dc6 ));

    }

    template< class System , class StateIn , class DerivIn , class StateOut >
    void do_step_impl_v2( System system , const StateIn &in , const DerivIn &dxdt_in , time_type t , StateOut &out , time_type dt )
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

        sys.rhs( m_x_tmp , m_k6 , t + dt*a6 );
        out= in + dt*c1*dxdt_in + dt*c3*m_k3 + dt*c4*m_k4 + dt*c6*m_k6;
    }

private:
 //   bool m_first_call;

    state_type m_x_tmp;
    deriv_type m_k2 , m_k3 , m_k4 , m_k5 , m_k6 ;
    deriv_type m_dxdt_tmp;
};

NS_BNO_END
