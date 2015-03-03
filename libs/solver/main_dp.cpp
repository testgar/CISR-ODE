// g++ -std=c++11 minimal.cpp  -larmadillo -Wfatal-errors

template <class T>
struct show_type;
//#define ARMA_NO_DEBUG

#include <armadillo>
#include <iostream>
using namespace std;
#include <string>

using namespace arma;

const int state_size=3;
typedef double my_value_type;
typedef double my_time_type;

typedef arma::vec::fixed<state_size> my_state_type;
typedef arma::vec::fixed<state_size> my_deriv_type;
// #define MY_VECTOR_RANGE_COPY 0

#include "solver_dp45.hpp"
#include "solver_ck45.hpp"
//#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;


class CSystem
{
private:
    const double sigma = 10.0;
    const double R = 28.0;
    const double b = 8.0 / 3.0;
public:

    const my_value_type eps_rel=1E-10;
    const my_value_type eps_abs=1E-10;

	void operator() ( const my_state_type &x , my_state_type &dxdt , const double  )
	{
        dxdt(0) = sigma * ( x(1) - x(0) );
        dxdt(1) = R * x(0) - x(1) - x(0) * x(2);
        dxdt(2) = -b * x(2) + x(0) * x(1);
	}

    void observer( const my_state_type &x , const double t )
    {
        cout << t << '\t' << x(0) << '\t' << x(1) << '\t' << x(2) << endl;
    }

};

void my_observer( const my_state_type &x , const double t )
{
    cout << t << '\t' << x(0) << '\t' << x(1) << '\t' << x(2) << endl;
}

const double stop_time=50.0;

int main()
{
	CSystem my_system;
	my_state_type x;
	x(0) = 10;
	x(1) = 1;
	x(2) = 1;

    // original
   //integrate_adaptive( make_controlled( 1E-10 , 1E-10 , runge_kutta_dopri5<my_state_type>() ) ,
   //                  my_system , x , 0.0 , stop_time , 0.1 , my_observer );
    // improved
    Solver<Steppers::RKDP5,CSystem> solver(my_system);
    solver.integrate_adaptive(x , 0.0 , stop_time , 0.1);

    cout<<"ok"<<endl;

    return 0;
}
