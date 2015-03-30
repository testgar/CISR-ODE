#pragma once

#define _unused(x) ((void)x)

typedef arma::vec::fixed<states::state_size> state_type;
typedef state_type deriv_type;

typedef arma::vec::fixed<outputs::output_size> observer_type;
typedef arma::vec::fixed<inputs::input_size> input_type;
typedef arma::vec::fixed<mids::intermediate_size> intermediate_type;
typedef arma::mat::fixed<config::buffer_headers,config::buffer_size> buffer_type;

typedef double time_type;
typedef double value_type;

struct figure_frame
{
public:
	time_type min_time;
	time_type max_time;
	double min_y;
	double max_y;
};

