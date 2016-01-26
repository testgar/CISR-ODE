#include <armadillo>
#include "model.hpp"

void Model::intermediates(
	const input_type			&u,
	const state_type			&x,
	intermediate_type			&mid,
	const time_type 			&t,
	const intermediate_type		&last_observed_mids,
	const time_type				&last_observed_t
	)
{
	_unused(t);
	_unused(u);
	_unused(last_observed_t);
	_unused(last_observed_mids);

	// m[0]	v_in
	// m[1]	v_end

	mid(mids::v_in)=u(inputs::v_in);
	mid(mids::v_end)=x(states::q_end)/physics::C;
}