
// *** Warning:
// *** auto-generated file and subjected to be overwritten

// path: ./app/outputs.hpp

#pragma once

namespace outputs
{
	const std::string output_header[]={
		"surgepitch_TC",
		"surgepitch_TCRL",
		"surgepitch_TCRL_deriv",
		"pitch_tilt_force",
		"surgepitch_sp_force",
		"pitch_kin_pos",
		"pitch_kin_vel",
	};

	const int surgepitch_TC=0;
	const int surgepitch_TCRL=1;
	const int surgepitch_TCRL_deriv=2;
	const int pitch_tilt_force=3;
	const int surgepitch_sp_force=4;
	const int pitch_kin_pos=5;
	const int pitch_kin_vel=6;

	// total size of output vector
	const int output_size=7;

	//Export list
	struct ExpList
	{
		std::string file_suffix; // no space is accepted in file suffix name
		int output_index;
	} figure_list[]={
		{"surgepitch_sp_force",outputs::surgepitch_sp_force},
	};
}
