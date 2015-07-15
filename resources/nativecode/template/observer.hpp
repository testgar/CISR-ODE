
#WARNING_COMMENTS#

// path: ./#TRACING_PATH#

#pragma once

using namespace arma;

void Model::observer(
	const state_type 			&x ,
	const double 				&t,
	observer_type 				&ymat,
	const state_type 			&last_observed_x,
	const time_type 			&last_observed_t,
	input_type					&u
	)
{
	intermediate_type mid;
	intermediates(u,x,mid,t,last_observed_x,last_observed_t);

	#INDEX_NUMBERS_COMMENTS#

	#OUTPUT_MATRIX#

}
