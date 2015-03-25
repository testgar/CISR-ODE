#include <iostream>
#include <cmath>

// #include <boost/array.hpp>

// #include <boost/numeric/odeint.hpp>

// using namespace std;
// using namespace boost::numeric::odeint;

// typedef boost::array< double , 3 > state_type;

// double sat_d(double x)
// {
// 	if(abs(x)<1)
// 		return 1;
// 	else
// 		return 0;
// }

// double input(double t)
// {
// 	if(t<1)
// 		return 0;
// 	if(t<2)
// 		return 1;
// 	if(t<4)
// 		return 0;
// 	if(t<10)
// 		return -1;
// 	return 0;
// }

// void lorenz( const state_type &x , state_type &dxdt , double t )
// {
//     dxdt[0] = x[0]*x[1]<=0 ? x[1] : sat_d(x[0])*x[1];
//     dxdt[1] = x[1]*input(t)<=0 ? input(t) : sat_d(x[1])*input(t);
// }

// void write_lorenz( const state_type &x , const double t )
// {
//     cout << t <<'\t'<<input(t)<< '\t' << x[0] << '\t' << x[1] << '\t' << endl;
// }

int main()
{
    // state_type x = { 0, 0 }; // initial conditions
    // integrate( lorenz , x , 0.0 , 25.0 , 0.1 , write_lorenz );
    std::cout<<"aaaaaaaa"<<std::endl;
    return 0;
}
