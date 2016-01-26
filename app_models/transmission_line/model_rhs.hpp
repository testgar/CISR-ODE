#include <armadillo>
#include "model.hpp"

void Model::rhs(
	const state_type 			&x,
	state_type 					&x_dot,
	const time_type				&t,
	const intermediate_type 	&last_observed_mids,
	const time_type 			&last_observed_t,
	input_type					&u
	)
{
	// ******* system implementation *******
	intermediate_type mid;
	intermediates(u,x,mid,t,last_observed_mids,last_observed_t);

	_unused(t);
	_unused(x);
	_unused(last_observed_mids);
	_unused(last_observed_t);
	_unused(u);

	static arma::mat::fixed<2*N,2*N> A_mat=get_A();

	x_dot=A_mat*x;
	x_dot(states::phi_0)+=mid(mids::v_in);
}