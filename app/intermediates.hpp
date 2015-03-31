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
	// m9	surgepitch_TCRL_deriv
	// m10	surgepitch_TCRL
	// m11	pitch_input_vel_ref
	// m12	pitch_hp_out
	// m13	pitch_kin_vel
	// m14	pitch_kin_pos
	// m15	pitch_tilt_force
	// m16	surgepitch_sp_force

	const int surge_input_acc_ref=0;
	const int surge_hp_out=1;
	const int surge_proposed_acc=2;
	const int surge_proposed_vel=3;
	const int surge_kin_pos=4;
	const int surge_kin_vel=5;
	const int surge_kin_acc=6;
	const int surge_lp_out=7;
	const int surgepitch_TC=8;
	const int surgepitch_TCRL_deriv=9;
	const int surgepitch_TCRL=10;
	const int pitch_input_vel_ref=11;
	const int pitch_hp_out=12;
	const int pitch_kin_vel=13;
	const int pitch_kin_pos=14;
	const int pitch_tilt_force=15;
	const int surgepitch_sp_force=16;

	// total size of intermediate vector
	const int intermediate_size=17;
}