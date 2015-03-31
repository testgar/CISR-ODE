#pragma once

namespace config
{

	const uint buffer_size=1000;
	const uint results_additions=2; // t, dt
	const uint buffer_headers=outputs::output_size+config::results_additions;

	const double start_time=0;
	const double stop_time=15;
	const std::string web_dir="web";
}

namespace config
{
#ifdef NDEBUG
	const int debugmod=0;
#else
	const int debugmod=1;
#endif
}

namespace platform
{
	//	washout filters
	const double omega_hp_x=2.5;
	const double zeta_hp_x=1;
	const double omega_hp_y=4;
	const double zeta_hp_y=1;
	const double omega_hp_z=4;

	const double omega_lp_x=5;
	const double zeta_lp_x=1;
	const double omega_lp_y=8;
	const double zeta_lp_y=1;

	const double omega_hp_pitch=1;
	const double omega_hp_roll=1;
	const double omega_hp_yaw=1;

	// saturation
	const double saturation_a=200;
	const double saturation_b=-0.95*saturation_a;

	// limits
	const double max_acc_surge=3;
	const double max_vel_surge=4;
	const double max_pos_surge=1.2;
	// tilt thresholds
	const double surge_tilt_threshold=((double) 5.0)*constants::pi/180;
};

