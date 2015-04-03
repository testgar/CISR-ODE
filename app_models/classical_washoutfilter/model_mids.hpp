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
	_unused(last_observed_x);
	_unused(last_observed_t);

	// m0	surge_input_acc_ref
	// m1	surge_hp_out
	// m2	surge_kin_acc
	// m3	surge_kin_vel
	// m4	surge_kin_pos
	// m5	surge_lp_out
	// m6	tilt_coordination
	// m7	pitch_input_angle_ref
	// m8	pitch_hp_out
	// m9	pitch_angle
	// m10	tilt_force
	// m11	specific_force

	const double max_acc=platform::max_acc;
	const double g=platform::g;

	mid(mids::surge_input_acc_ref)=u(inputs::surge_acc);
	mid(mids::surge_hp_out)=hp_surge_out(mid,x);
	mid(mids::surge_kin_acc)=std::max(-max_acc,std::min(+max_acc,mid(mids::surge_hp_out)));
	mid(mids::surge_kin_vel)=x(states::surge_kin_vel);
	mid(mids::surge_kin_pos)=x(states::surge_kin_pos);
	mid(mids::surge_lp_out)=lp_surge_out(x);
	mid(mids::tilt_coordination)=mid(mids::surge_lp_out)/g;
	mid(mids::pitch_input_angle_ref)=u(inputs::pitch_vel);
	mid(mids::pitch_hp_out)=hp_pitch_out(mid,x);
	mid(mids::pitch_angle)=mid(mids::tilt_coordination)+mid(mids::pitch_hp_out);
	mid(mids::tilt_force)=mid(mids::pitch_angle)*g;
	mid(mids::specific_force)=mid(mids::tilt_force)+mid(mids::surge_kin_acc);

// macro based:
// #define M(a) mid(mids::a)
// #define X(a) x(states::a)
// 	M(surge_input_acc_ref)=u(inputs::surge_acc);
// 	M(surge_hp_out)=hp_surge_out(mid,x);
// 	M(surge_kin_acc)=std::max(-max_acc,std::min(+max_acc,M(surge_hp_out)));
// 	M(surge_kin_vel)=X(surge_kin_vel);
// 	M(surge_kin_pos)=X(surge_kin_pos);
// 	M(surge_lp_out)=lp_surge_out(x);
// 	M(tilt_coordination)=M(surge_lp_out)/g;
// 	M(pitch_input_angle_ref)=u(inputs::pitch_vel);
// 	M(pitch_hp_out)=hp_pitch_out(mid,x);
// 	M(pitch_angle)=M(tilt_coordination)+M(pitch_hp_out);
// 	M(tilt_force)=M(pitch_angle)*g;
// 	M(specific_force)=M(tilt_force)+M(surge_kin_acc);	
// #undef M
// #undef X
}
