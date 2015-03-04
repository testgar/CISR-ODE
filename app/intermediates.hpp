#pragma once

namespace mids
{
	const int surge_hp_out=0;
	const int ref_surge_acc=1;
	const int ref_surge_vel=2;
	const int kin_surge_pos=3;
	const int kin_surge_vel=4;
	const int kin_surge_acc=5;

	const int lp_surge_out=6;
	const int tc=7;  // titl coordination
	const int tc_rate_limited=8;
	const int tc_rate_limited_derived=9;
	const int hp_pitch_out=10;
	const int kin_pitch_vel=11;
	const int kin_pitch_angle=12;
	const int tc_surge_sp_force=13;
	const int surge_sp_force=14;

	// total size of intermediate vector
	const int intermediate_size=10;
}