#pragma once

#include "solver.hpp"

NS_ODE_BEGIN

template<class System>
class controlled_runge_kutta<Steppers::RKF78,System>
{
protected:
	typedef typename System::value_type value_type;
	typedef typename System::state_type state_type;
	typedef typename System::deriv_type deriv_type;
	typedef typename System::time_type time_type;

public:

	controlled_runge_kutta(const value_type eps_abs,const value_type eps_rel)
	: m_error_checker( eps_abs, eps_rel) ,
	  m_first_call( true )
	{ }

	controlled_step_result try_step( System &system , state_type &x , time_type &t , time_type &dt )
	{
		system.rhs( x , m_dxdt , t );
		controlled_step_result res = try_step2( system , x , m_dxdt , t , m_xnew , m_dxdtnew , dt );
		dt=std::min(dt,system.max_dt); // added for maximum step size
		if( res == success )
		{
			x=m_xnew;
			// m_dxdt=m_dxdtnew;
		}
		return res;
	}

	//call 4
	controlled_step_result try_step2( System &system , const state_type &x_in , const deriv_type &dxdt_in , time_type &t ,
			state_type &x_out , deriv_type &dxdt_out , time_type &dt)
	{
		do_step( system , x_in , dxdt_in , t , x_out , dxdt_out , dt , m_xerr );

		// this potentially overwrites m_x_err! (standard_error_checker does, at least)
		value_type max_rel_err = m_error_checker.error(  x_in , dxdt_in , m_xerr , dt );

		if( max_rel_err > 1.0 )
		{
			// error too large - decrease dt ,limit scaling factor to 0.2 and reset state
			// simplified: max( 9/10*pow( max_rel_err , -1/( m_stepper.error_order_value - 1) ) ,1/5 );
			dt *= std::max( value_type( value_type(9)/value_type(10) *
						pow( max_rel_err , value_type(-1) / ( error_order_value - 1 ) ) ) ,
						value_type( value_type(1)/value_type(5)) );
			return fail;
		}
		else
		{
			t += dt;
			if( max_rel_err < 0.5 )
			{   //error too small - increase dt and keep the evolution and limit scaling factor to 5.0
				// error should be > 0
				max_rel_err = std::max( value_type( pow( value_type(5.0) , -value_type(stepper_order_value) ) ) , max_rel_err );
				// simplified: 9/10 * pow( max_rel_err , -1 / m_stepper.stepper_order_value )
				dt *= value_type( value_type(9)/value_type(10) * pow( max_rel_err , value_type(-1) / stepper_order_value ) );
			}
			return success;
		}
	}

private:

	default_error_checker<System> m_error_checker;

	deriv_type m_dxdt;
	state_type m_xerr;
	state_type m_xnew;
	deriv_type m_dxdtnew;
	bool m_first_call;

public:
	const unsigned short stage_count_value = 13; // where is it used????
	const unsigned short order_value = 8; // where is it used????
	const unsigned short stepper_order_value = 8;
	const unsigned short error_order_value = 7;

	void do_step(System &system , const state_type &x_in , const deriv_type &dxdt_in , time_type t ,
			state_type &x_out , deriv_type &dxdt_out , time_type dt , state_type &xerr )
	{
		const value_type a1 = value_type( 2 ) / value_type( 27 );

		const value_type a21 = value_type( 1 ) / value_type( 36 );
		const value_type a22 = value_type( 1 ) / value_type( 12 );

		const value_type a31 = value_type( 1 ) / value_type( 24 );
		const value_type a32 = value_type( 0 );
		const value_type a33 = value_type( 1 ) / value_type( 8 );

		const value_type a41 = value_type( 5 ) / value_type( 12 );
		const value_type a42 = value_type( 0 );
		const value_type a43 = value_type( -25 ) / value_type( 16 );
		const value_type a44 = value_type( 25 ) / value_type( 16 );

		const value_type a51 = value_type( 1 ) / value_type( 20 );
		const value_type a52 = value_type( 0 );
		const value_type a53 = value_type( 0 );
		const value_type a54 = value_type( 1 ) / value_type( 4 );
		const value_type a55 = value_type( 1 ) / value_type( 5 );

		const value_type a61 = value_type( -25 )/static_cast< Value >( 108 );
		const value_type a62 = value_type( 0 );
		const value_type a63 = value_type( 0 );
		const value_type a64 = value_type( 125 ) / value_type( 108 );
		const value_type a65 = value_type( -65 ) / value_type( 27 );
		const value_type a66 = value_type( 125 ) / value_type( 54 );

		const value_type a71 = value_type( 31 ) / value_type( 300 );
		const value_type a72 = value_type( 0 );
		const value_type a73 = value_type( 0 );
		const value_type a74 = value_type(0 );
		const value_type a75 = value_type( 61 ) / value_type( 225 );
		const value_type a76 = value_type( -2 ) / value_type( 9 );
		const value_type a77 = value_type( 13 ) / value_type( 900 );

		const value_type a81 = value_type( 2 );
		const value_type a82 = value_type( 0 );
		const value_type a83 = value_type( 0 );
		const value_type a84 = value_type( -53 ) / value_type( 6 );
		const value_type a85 = value_type(704 ) / value_type( 45 );
		const value_type a86 = value_type( -107 ) / value_type( 9 );
		const value_type a87 = value_type( 67 ) / value_type( 90 );
		const value_type a88 = value_type( 3 );

		const value_type a91 = value_type( Value >( -91 ) / value_type( 108 );
		const value_type a92 = value_type( 0 );
		const value_type a93 = value_type( 0 );
		const value_type a94 = value_type( 23 ) / value_type( 108 );
		const value_type a95 = value_type( -976 ) / value_type( 135 );
		const value_type a96 = value_type( 311 ) / value_type( 54 );
		const value_type a97 = value_type( -19 ) / value_type( 60 );
		const value_type a98 = value_type( 17 ) / value_type( 6 );
		const value_type a99 = value_type( -1 ) / value_type( 12 );

		const value_type a10_1 = value_type( 2383 ) / value_type( 4100 );
		const value_type a10_2 = value_type( 0 );
		const value_type a10_3 = value_type( 0 );
		const value_type a10_4 = value_type( -341 ) / value_type( 164 );
		const value_type a10_5 = value_type( 4496 ) / value_type( 1025 );
		const value_type a10_6 = value_type( -301 ) / value_type( 82 );
		const value_type a10_7 = value_type( 2133 ) / value_type( 4100 );
		const value_type a10_8 = value_type( 45 ) / value_type( 82 );
		const value_type a10_9 = value_type( 45 ) / value_type( 164 );
		const value_type a10_10 = value_type( 18 ) / value_type( 41 );

		const value_type a11_1 = value_type( 3 ) / value_type( 205 );
		const value_type a11_2 = value_type( 0 );
		const value_type a11_3 = value_type( 0 );
		const value_type a11_4 = value_type( 0 );
		const value_type a11_5 = value_type( 0 );
		const value_type a11_6 = value_type( -6 ) / value_type( 41 );
		const value_type a11_7 = value_type( -3 ) / value_type( 205 );
		const value_type a11_8 = value_type( -3 ) / value_type( 41 );
		const value_type a11_9 = value_type( 3 ) / value_type( 41 );
		const value_type a11_10 = value_type( 6 ) / value_type( 41 );
		const value_type a11_11 = value_type( 0 );

		const value_type a12_1 = value_type( -1777 ) / value_type( 4100 );
		const value_type a12_2 = value_type( 0 );
		const value_type a12_3 = value_type( 0 );
		const value_type a12_4 = value_type( -341 ) / value_type( 164 );
		const value_type a12_5 = value_type( 4496 ) / value_type( 1025 );
		const value_type a12_6 = value_type( -289 ) / value_type( 82 );
		const value_type a12_7 = value_type( 2193 ) / value_type( 4100 );
		const value_type a12_8 = value_type( 51 ) / value_type( 82 );
		const value_type a12_9 = value_type( 33 ) / value_type( 164 );
		const value_type a12_10 = value_type( 12 ) / value_type( 41 );
		const value_type a12_11 = value_type( 0 );
		const value_type a12_12 = value_type( 1 );

		const value_type b_1 = value_type( 0 );
		const value_type b_2 = value_type( 0 );
		const value_type b_3 = value_type( 0 );
		const value_type b_4 = value_type( 0 );
		const value_type b_5 = value_type( 0 );
		const value_type b_6 = value_type( 34 ) / value_type( 105 );
		const value_type b_7 = value_type( 9 ) / value_type( 35 );
		const value_type b_8 = value_type( 9 ) / value_type( 35 );
		const value_type b_9 = value_type( 9 ) / value_type( 280 );
		const value_type b_10 = value_type( 9 ) / value_type( 280 );
		const value_type b_11 = value_type( 0 );
		const value_type b_12 = value_type( 41 ) / value_type( 840 );
		const value_type b_13 = value_type( 41 ) / value_type( 840 );

		const value_type db_1 = value_type( 0 ) - value_type( 41 ) / value_type( 840 );
		const value_type db_2 = value_type( 0 );
		const value_type db_3 = value_type( 0 );
		const value_type db_4 = value_type( 0 );
		const value_type db_5 = value_type( 0 );
		const value_type db_6 = value_type( 0 );
		const value_type db_7 = value_type( 0 );
		const value_type db_8 = value_type( 0 );
		const value_type db_9 = value_type( 0 );
		const value_type db_10 = value_type( 0 );
		const value_type db_11 = value_type( 0 ) - value_type( 41 ) / value_type( 840 );
		const value_type db_12 = value_type( 41 ) / value_type( 840 );
		const value_type db_13 = value_type( 41 ) / value_type( 840 );

		const value_type c_1 = value_type( 0 );
		const value_type c_2 = value_type( 2 ) / value_type( 27 );
		const value_type c_3 = value_type( 1 ) / value_type( 9 );
		const value_type c_4 = value_type( 1 ) / value_type( 6 );
		const value_type c_5 = value_type( 5 ) / value_type( 12 );
		const value_type c_6 = value_type( 1 ) / value_type( 2 );
		const value_type c_7 = value_type( 5 ) / value_type( 6 );
		const value_type c_8 = value_type( 1 ) / value_type( 6 );
		const value_type c_9 = value_type( 2 ) / value_type( 3 );
		const value_type c_10 = value_type( 1 ) / value_type( 3 );
		const value_type c_11 = value_type( 1 );
		const value_type c_12 = value_type( 0 );
		const value_type c_13 = value_type( 1 );

??????????????????????? afterward

		m_x_tmp= x_in + dt*b21 * dxdt_in;

		system.rhs( m_x_tmp , m_k2 , t + dt*a2 );
		m_x_tmp= x_in + dt*b31*dxdt_in + dt*b32*m_k2; // why cannot factor dt????

		system.rhs( m_x_tmp , m_k3 , t + dt*a3 );
		m_x_tmp=x_in+dt*b41*dxdt_in+dt*b42*m_k2+dt*b43*m_k3;

		system.rhs( m_x_tmp, m_k4 , t + dt*a4 );
		m_x_tmp=x_in+dt*b51*dxdt_in+dt*b52*m_k2+ dt*b53*m_k3+dt*b54*m_k4;

		system.rhs( m_x_tmp , m_k5 , t + dt*a5 );
		m_x_tmp=x_in + dt*b61*dxdt_in + dt*b62*m_k2 + dt*b63*m_k3 + dt*b64*m_k4 + dt*b65*m_k5 ;

		system.rhs( m_x_tmp , m_k6 , t + dt );
		x_out= x_in + dt*c1*dxdt_in + dt*c3*m_k3 + dt*c4*m_k4 + dt*c5*m_k5 + dt*c6*m_k6;

		// the new derivative
		system.rhs( x_out , dxdt_out , t + dt );

		//error estimate
		xerr=dt*dc1*dxdt_in + dt*dc3*m_k3 + dt*dc4*m_k4 + dt*dc5*m_k5 + dt*dc6*m_k6 + dt*dc7*dxdt_out;
	}

private:
	state_type m_x_tmp;
	deriv_type m_k2 , m_k3 , m_k4 , m_k5 , m_k6 ;
};

NS_ODE_END
