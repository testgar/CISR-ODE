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

	// x0	surge_hp_x1
	// x1	surge_hp_x2
	// x2	surge_kin_vel
	// x3	surge_kin_pos
	// x4	surge_lp_x1
	// x5	surge_lp_x2
	// x6	pitch_hp_x1

	// x_dot(states::surge_hp_x1)=...
	// x_dot(states::surge_hp_x2)=...
	hp_surge(mid,x,x_dot);

	x_dot(states::surge_kin_vel)=mid(mids::surge_kin_acc);
	
	x_dot(states::surge_kin_pos)=mid(mids::surge_kin_vel);

	// x_dot(states::surge_lp_x1)=...
	// x_dot(states::surge_lp_x2)=...
	lp_surge(mid,x,x_dot);

	// x_dot(states::pitch_hp_x1)=...
	hp_pitch(mid,x,x_dot);

}