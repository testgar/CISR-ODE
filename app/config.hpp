#pragma once

namespace config
{
	const int debugmod=1;

	const uint buffer_size=1000;
	const uint results_additions=2; // t, dt
	const uint buffer_headers=outputs::output_size+config::results_additions;

	const double start_time=0;
	const double stop_time=15;

	// struct ExpList
	// {
	// 	std::string file_suffix; // no space is accepted in file suffix name
	// 	int output_index;
	// } figure_list[]={ 
	// 	{"input-surge-acc",outputs::surge_input_acc_ref},
	// 	{"surge-high-pass",outputs::surge_hp_out},
	// 	{"kin-acc-surge",outputs::surge_kin_acc},
	// 	{"kin-vel-surge",outputs::surge_kin_vel},
	// 	{"kin-pos-surge",outputs::surge_kin_pos},
	// 	{"specific-force-surge",outputs::surgepitch_sp_force},
	// 	{"pitch-tilt-force",outputs::pitch_tilt_force},
	// };

	const std::vector<std::string> log_columns=
			{"File","Line","Function","Value","Message"};
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

namespace files
{
	const std::string html_open="web/html_open.html"; 
	const std::string html_close="web/html_close.html";
	const std::string html_head_open="web/head_open.html"; 
	const std::string html_head_close="web/head_close.html";
	const std::string html_body_open="web/body_open.html"; 
	const std::string html_body_close="web/body_close.html";
	const std::string fhs_table_open="web/fixed_header_scrolling/table_open.html";
	const std::string fhs_table_close="web/fixed_header_scrolling/table_close.html";
	const std::string fhs_thead_open="web/fixed_header_scrolling/thead_open.html";
	const std::string fhs_thead_close="web/fixed_header_scrolling/thead_close.html";
	const std::string fhs_thead_column="web/fixed_header_scrolling/thead_column.html";
	const std::string fhs_tbody_open="web/fixed_header_scrolling/tbody_open.html";
	const std::string fhs_tbody_close="web/fixed_header_scrolling/tbody_close.html";
	const std::string fhs_tbody_tr_open="web/fixed_header_scrolling/tbody_tr_open.html";
	const std::string fhs_tbody_tr_close="web/fixed_header_scrolling/tbody_tr_close.html";
	const std::string fhs_tbody_td="web/fixed_header_scrolling/tbody_td.html";
	const std::string fhs_table_style="web/fixed_header_scrolling/table2.css"; 
	const std::string fhs_table_js="web/fixed_header_scrolling/fhs_adjust.js";
	const std::string jquery="jquery-1.11.2.min.js";
	const std::string js_src="web/js_src.html";
	const std::string html_fixation_list_open="web/templates/fixation_list/select_open.html";
	const std::string html_fixation_list_close="web/templates/fixation_list/select_close.html";
	const std::string html_fixation_option="web/templates/fixation_list/option.html";
	const std::string fixation_list="fixationlist";
	const std::string html_h1="web/templates/h1.html";
	const std::string html_h2="web/templates/h2.html";

	const std::string output_folder="out"; 
	const std::string output_prefix="data_";
	const char sep=boost::filesystem::path::preferred_separator;
	const std::string output_common_base=output_prefix+application::serialized_time()+'_';
	const std::string output_common=output_folder+sep+output_common_base;
	const std::string output_results=output_common+"_results";
	const std::string output_log=output_common+"_log";
}