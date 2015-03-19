#pragma once

//	state space

namespace states
{

	// x0	surge_hp_x1
	// x1	surge_hp_x2
	// x2	surge_proposed_vel
	// x3	surge_kin_pos
	// x4	surge_lp_x1
	// x5	surge_lp_x2
	// x6	pitch_hp_x1
	// x7	surgepitch_TCRL
	// x8	surgepitch_TCRL_deriv
	// x9	pitch_pos


	const int surge_hp_x1=0;
	const int surge_hp_x2=1;
	const int surge_proposed_vel=2;
	const int surge_kin_pos=3;
	const int surge_lp_x1=4;
	const int surge_lp_x2=5;
	const int pitch_hp_x1=6;
	const int surgepitch_TCRL=7;
	const int surgepitch_TCRL_deriv=8;
	const int pitch_kin_pos=9;

	// total size of state vector
	const int state_size=10;
}