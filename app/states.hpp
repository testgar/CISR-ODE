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
	// x6	surgepitch_TCRL
	// x7	pitch_hp_x1
	// x8	pitch_kin_pos

	const int surge_hp_x1=0;
	const int surge_hp_x2=1;
	const int surge_proposed_vel=2;
	const int surge_kin_pos=3;
	const int surge_lp_x1=4;
	const int surge_lp_x2=5;
	const int surgepitch_TCRL=6;
	const int pitch_hp_x1=7;
	const int pitch_kin_pos=8;

	// total size of state vector
	const int state_size=9;
}
