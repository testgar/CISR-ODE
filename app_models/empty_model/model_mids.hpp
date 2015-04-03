#include <armadillo>
#include "model.hpp"

using namespace arma;

void Model::intermediates(
	const input_type &u,
	const state_type &x,
	intermediate_type &mid,
	const time_type &t,
	const state_type &last_observed_x,
	const time_type &last_observed_t
	)
{
	_unused(t);
	_unused(x);
	_unused(u);
	_unused(mid);
	_unused(last_observed_t);
	_unused(last_observed_x);

	// list of intermediates goes here

	// assigning intermediates

}