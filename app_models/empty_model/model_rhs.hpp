#include <armadillo>
#include "model.hpp"

using namespace arma;

void Model::rhs(
	const state_type 			&x,
	state_type 					&x_dot,
	const time_type				&t,
	const state_type 			&last_observed_x,
	const time_type 			&last_observed_t,
	input_type					&u
	)
{
	// ******* system implementation *******
	intermediate_type mid;
	intermediates(u,x,mid,t,last_observed_x,last_observed_t);

	_unused(t);
	_unused(x);
	_unused(x_dot);
	_unused(last_observed_x);
	_unused(last_observed_t);
	_unused(u);

}