#pragma once

//	state space

namespace states
{

	// x0	surge_hp_x1
	// x1	surge_hp_x2
	// x2	surge_proposed_vel
	// x3	surge_kin_pos
	// x4	surge_kin_vel
	// x5	surge_kin_acc
	// x6	surge_lp_x1
	// x7	surge_lp_x2
	// x8	pitch_hp_x1
	// x9	surgepitch_TCRL
	// x10	surgepitch_TCRL_deriv
	// x11	pitch_pos

	const int surge_hp_x1=0;
	const int surge_hp_x2=1;
	const int surge_proposed_vel=2;
	const int surge_kin_pos=3;
	const int surge_kin_vel=4;
	const int surge_kin_acc=5;
	const int surge_lp_x1=6;
	const int surge_lp_x2=7;
	const int pitch_hp_x1=8;
	const int surgepitch_TCRL=9;
	const int surgepitch_TCRL_deriv=10;
	const int pitch_pos=11;

	// total size of state vector
	const int state_size=12;
}