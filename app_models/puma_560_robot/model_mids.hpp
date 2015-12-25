#include <armadillo>
#include "model.hpp"

using namespace arma;

void Model::intermediates(
	const input_type			&u,
	const state_type			&x,
	intermediate_type			&mid,
	const time_type 			&t,
	const intermediate_type		&last_observed_mids,
	const time_type				&last_observed_t
	)
{
	// m[0:2]	theta_desired[3]
	// m[3:5]	D_theta_desired[3]
	// m[6:8]	DD_theta_desired[3]
	// m[9:11]	theta[3]
	// m[12:14]	D_theta[3]
	// m[15:17]	theta_error[3]
	// m[18:20]	D_theta_error[3]
	// m[21:26]	DD_theta_controller[6]
	// m[27:32]	T[6]
	// m[33:38]	angular_vec[6]
	// m[39:41]	position[3]
	// m[42:44]	position_desired[3]
	// m[45]	error

	mid.subvec(mids::theta_desired_0,mids::theta_desired_end)=u;
	mid.subvec(mids::D_theta_desired_0,mids::D_theta_desired_end)=Model::derivative_range3(mid,last_observed_mids,mids::theta_desired_0,mids::theta_desired_end,t,last_observed_t);
	mid.subvec(mids::DD_theta_desired_0,mids::DD_theta_desired_end)=Model::derivative_range3(mid,last_observed_mids,mids::D_theta_desired_0,mids::D_theta_desired_end,t,last_observed_t);
	mid.subvec(mids::theta_0,mids::theta_end)=x.subvec(states::theta_pos_0,states::theta_pos_end);
	mid.subvec(mids::D_theta_0,mids::D_theta_end)=x.subvec(states::theta_vel_0,states::theta_vel_end);

	// ********** CTC Control law **********
	mid.subvec(mids::theta_error_0,mids::theta_error_end)=mid.subvec(mids::theta_desired_0,mids::theta_desired_end)-mid.subvec(mids::theta_0,mids::theta_end);
	// mid.subvec(mids::D_theta_error_0,mids::D_theta_error_end)=(mid.subvec(mids::theta_error_0,mids::theta_error_end)-last_observed_mids.subvec(mids::theta_error_0,mids::theta_error_end))/(t-last_observed_t);
	mid.subvec(mids::D_theta_error_0,mids::D_theta_error_end)=Model::derivative_range3(mid,last_observed_mids,mids::theta_error_0,mids::theta_error_end,t,last_observed_t);

	mid.subvec(mids::DD_theta_controller_0,mids::DD_theta_controller_2)=mid.subvec(mids::DD_theta_desired_0,mids::DD_theta_desired_end)+15.0*mid.subvec(mids::theta_error_0,mids::theta_error_end)+10.0*mid.subvec(mids::D_theta_error_0,mids::D_theta_error_end);
	mid.subvec(mids::DD_theta_controller_3,mids::DD_theta_controller_end).zeros();//={0,0,0};

	const double theta2=mid(mids::theta_1);
	const double theta3=mid(mids::theta_2);
	const arma::mat Inertia_mat=Model::kinetic_energy_mat(theta2,theta3);

	Model::coriolis_type B=Model::coriolis_mat(theta2,theta3);
	arma::mat C=Model::centrifugal_mat(theta2,theta3);
	arma::mat G=Model::gravity_mat(theta2,theta3);

	mid.subvec(mids::T_0,mids::T_end)=Model::to_T_vec(Inertia_mat,mid.subvec(mids::DD_theta_controller_0,mids::DD_theta_controller_end),B,C,G,mid.subvec(mids::D_theta_0,mids::D_theta_end));

	// // ddtheta=....

	// // ********** 3-link Puma robot **********
	mid.subvec(mids::angular_vec_0,mids::angular_vec_end)=arma::solve(Inertia_mat, Model::to_angular_vec(mid.subvec(mids::T_0,mids::T_end),B,C,G,mid.subvec(mids::D_theta_0,mids::D_theta_end)));
	mid.subvec(mids::position_0,mids::position_end)=Model::angle_to_position(mid.subvec(mids::theta_0,mids::theta_end));
	mid.subvec(mids::position_desired_0,mids::position_desired_end)=Model::angle_to_position(mid.subvec(mids::theta_desired_0,mids::theta_desired_end));
// _(mid(mids::position_0));
// _(mid(mids::position_end));
// _(mid(mids::position_desired_0));
// _(mid(mids::position_desired_end));
	mid(mids::error)=arma::norm(mid.subvec(mids::position_0,mids::position_end)-mid.subvec(mids::position_desired_0,mids::position_desired_end));

}