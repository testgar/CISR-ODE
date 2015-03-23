#pragma once

namespace outputs
{
	const std::string output_header[]={"acc","vel","pos","hp","in","spforce","x_proposed_vel","x_kin_pos","pitch_tilt_force"};

	const int surge_kin_acc=0;
	const int surge_kin_vel=1;
	const int surge_kin_pos=2;
	const int surge_hp_out=3;
	const int surge_input_acc=4;
	const int surgepitch_sp_force=5;
	const int x_proposed_vel=6; //test
	const int x_kin_pos=7; //test
	const int pitch_tilt_force=8;

	// total size of output vector
	const int output_size=9;
}