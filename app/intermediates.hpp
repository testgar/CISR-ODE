#pragma once

namespace mids
{

	// m0	surge_input_acc_ref
	// m1	surge_hp_out
	// m2	surge_proposed_acc
	// m3	surge_proposed_vel
	// m4	surge_kin_pos
	// m5	surge_kin_vel
	// m6	surge_kin_acc
	// m7	surge_lp_out
	// m8	surgepitch_TC
	// m9	surgepitch_TCRL
	// m10	surgepitch_TCRL_deriv
	// m11	pitch_input_vel_ref
	// m12	pitch_hp_out
	// m13	pitch_kin_vel
	// m14	pitch_kin_pos
	// m15	pitch_tilt_force
	// m16	surgepitch_sp_force

	const int surge_input_acc_ref=0;// add
	const int surge_hp_out=1;
	// const int ref_surge_acc=1;
	const int surge_proposed_acc=2;// add
	const int surge_proposed_vel=3;//ref_surge_vel=2;
	const int surge_kin_pos=4;//kin_surge_pos
	const int surge_kin_vel=5;//kin_surge_vel=4;
	const int surge_kin_acc=6;// kin_surge_acc=5;

	const int surge_lp_out=7;//lp_surge_out=6;
	const int surgepitch_TC=8;//tc=7;  // titl coordination
	const int surgepitch_TCRL=9;//tc_rate_limited=8; // titl coordination rate limit
	const int surgepitch_TCRL_deriv=10;//tc_rate_limited_derived=9;
	const int pitch_input_vel_ref=11;
	const int pitch_hp_out=12;//hp_pitch_out=10;
	const int pitch_kin_vel=13;//kin_pitch_vel=11;
	const int pitch_kin_pos=14;//kin_pitch_angle=12;
	const int pitch_tilt_force=15;//tc_surge_sp_force=13;
	const int surgepitch_sp_force=16;//surge_sp_force=14;

	// total size of intermediate vector
	const int intermediate_size=17;
}