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

	// x[0:2]	theta_vel[3]
	// x[3:5]	theta_pos[3]

	x_dot.subvec(states::theta_vel_0,states::theta_vel_end)=mid.subvec(mids::angular_vec_0,mids::angular_vec_2);
	for(uint k=states::theta_vel_0;k<states::theta_vel_end;k++)
		if(std::abs(x(k))>=10.0 && x(k)*x_dot(k)>0)
			x_dot(k)=0.0; // integral limit

	x_dot.subvec(states::theta_pos_0,states::theta_pos_end)=x.subvec(states::theta_vel_0,states::theta_vel_end);
	for(uint k=states::theta_pos_0;k<states::theta_pos_end;k++)
		if(std::abs(x(k))>=10.0 && x(k)*x_dot(k)>0)
			x_dot(k)=0.0; // integral limit

}