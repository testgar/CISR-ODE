#pragma once

//	state space

namespace states
{
	const int hp_surge_x1=0;
	const int hp_surge_x2=1;

	const int kin_limits_surge_vel=2;
	const int kin_limits_surge_pos=3;

	const int kin_surge_vel=4;
	const int kin_surge_acc=5;

	const int lp_surge_x1=6;
	const int lp_surge_x2=7;

	const int hp_pitch_x1=8;

	const int tilt_coordination_rate_limited=9;
	const int tilt_coordination_rate_limited_derivative=10;

	const int position_pitch_theta=11;

	// total size of state vector
	const int state_size=12;
}