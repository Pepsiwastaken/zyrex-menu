#include <config/config.h>
#include <settings.h>
#include <json/json.hpp>
#include <fstream>
#include <windows.h>

using json = nlohmann::json;

static std::string get_config_path()
{
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	std::string p(path);
	auto pos = p.find_last_of("\\/");
	if (pos != std::string::npos)
		p = p.substr(0, pos);
	return p + "\\autosave.json";
}

// Helper to serialize float[4] arrays
static json to_json_array(const float* arr, int n = 4)
{
	json j = json::array();
	for (int i = 0; i < n; i++)
		j.push_back(arr[i]);
	return j;
}

// Helper to deserialize float[4] arrays
static void from_json_array(const json& j, float* arr, int n = 4)
{
	if (!j.is_array()) return;
	for (int i = 0; i < n && i < (int)j.size(); i++)
		arr[i] = j[i].get<float>();
}

void config::save()
{
	json j;

	// -- menu --
	j["menu"]["menu_keybind"] = settings::menu::menu_keybind;
	j["menu"]["watermark"] = settings::menu::watermark;
	j["menu"]["streamproof"] = settings::menu::streamproof;
	j["menu"]["cursor_streamproof"] = settings::menu::cursor_streamproof;
	j["menu"]["auto_inject"] = settings::menu::auto_inject;
	j["menu"]["unload"] = settings::menu::unload;
	j["menu"]["vsync"] = settings::menu::vsync;
	j["menu"]["frame_limiter_ms"] = settings::menu::frame_limiter_ms;
	j["menu"]["performance_mode"] = settings::menu::performance_mode;

	// -- aimbot --
	j["aimbot"]["enabled"] = settings::aimbot::enabled;
	j["aimbot"]["keybind"] = settings::aimbot::keybind;
	j["aimbot"]["activation_mode"] = settings::aimbot::activation_mode;
	j["aimbot"]["mode"] = settings::aimbot::mode;
	j["aimbot"]["aim_type"] = settings::aimbot::aim_type;
	j["aimbot"]["target_part"] = settings::aimbot::target_part;
	j["aimbot"]["air_part_enabled"] = settings::aimbot::air_part_enabled;
	j["aimbot"]["air_part"] = settings::aimbot::air_part;
	j["aimbot"]["fov"] = settings::aimbot::fov;
	j["aimbot"]["use_fov"] = settings::aimbot::use_fov;
	j["aimbot"]["draw_fov"] = settings::aimbot::draw_fov;
	j["aimbot"]["fov_circle_colour"] = to_json_array(settings::aimbot::fov_circle_colour);
	j["aimbot"]["fov_outline_colour"] = to_json_array(settings::aimbot::fov_outline_colour);
	j["aimbot"]["smoothing"] = settings::aimbot::smoothing;
	j["aimbot"]["smoothingx"] = settings::aimbot::smoothingx;
	j["aimbot"]["smoothingy"] = settings::aimbot::smoothingy;
	j["aimbot"]["enable_prediction"] = settings::aimbot::enable_prediction;
	j["aimbot"]["prediction_x"] = settings::aimbot::prediction_x;
	j["aimbot"]["prediction_y"] = settings::aimbot::prediction_y;
	j["aimbot"]["air_prediction_enabled"] = settings::aimbot::air_prediction_enabled;
	j["aimbot"]["air_prediction_x"] = settings::aimbot::air_prediction_x;
	j["aimbot"]["air_prediction_y"] = settings::aimbot::air_prediction_y;
	j["aimbot"]["smoothing_style"] = settings::aimbot::smoothing_style;
	j["aimbot"]["teamcheck"] = settings::aimbot::teamcheck;
	j["aimbot"]["knock_check"] = settings::aimbot::knock_check;
	j["aimbot"]["wallcheck"] = settings::aimbot::wallcheck;
	j["aimbot"]["grabbed_check"] = settings::aimbot::grabbed_check;
	j["aimbot"]["forcefield_check"] = settings::aimbot::forcefield_check;
	j["aimbot"]["npc_check"] = settings::aimbot::npc_check;
	j["aimbot"]["reloading_check"] = settings::aimbot::reloading_check;
	j["aimbot"]["no_ammo_check"] = settings::aimbot::no_ammo_check;
	j["aimbot"]["sticky_aim"] = settings::aimbot::sticky_aim;
	j["aimbot"]["anti_shake"] = settings::aimbot::anti_shake;
	j["aimbot"]["health_check_enabled"] = settings::aimbot::health_check_enabled;
	j["aimbot"]["min_health"] = settings::aimbot::min_health;
	j["aimbot"]["fov_shape"] = settings::aimbot::fov_shape;
	j["aimbot"]["offset_enabled"] = settings::aimbot::offset_enabled;
	j["aimbot"]["offset_x"] = settings::aimbot::offset_x;
	j["aimbot"]["offset_y"] = settings::aimbot::offset_y;
	j["aimbot"]["ai_silent_y_offset"] = settings::aimbot::ai_silent_y_offset;

	// -- aimbot triggerbot --
	j["aimbot"]["triggerbot_enabled"] = settings::aimbot::triggerbot::enabled;
	j["aimbot"]["triggerbot_keybind"] = settings::aimbot::triggerbot::keybind;
	j["aimbot"]["triggerbot_activation_mode"] = settings::aimbot::triggerbot::activation_mode;
	j["aimbot"]["triggerbot_fire_mode"] = settings::aimbot::triggerbot::fire_mode;
	j["aimbot"]["triggerbot_clicks_per_second"] = settings::aimbot::triggerbot::clicks_per_second;
	j["aimbot"]["triggerbot_hold_duration"] = settings::aimbot::triggerbot::hold_duration;
	j["aimbot"]["triggerbot_reaction_ms"] = settings::aimbot::triggerbot::reaction_ms;
	j["aimbot"]["triggerbot_max_distance"] = settings::aimbot::triggerbot::max_distance;
	j["aimbot"]["triggerbot_max_distance_enabled"] = settings::aimbot::triggerbot::max_distance_enabled;
	j["aimbot"]["triggerbot_wallcheck"] = settings::aimbot::triggerbot::wallcheck;
	j["aimbot"]["triggerbot_headshot_only"] = settings::aimbot::triggerbot::headshot_only;
	j["aimbot"]["triggerbot_knife_check"] = settings::aimbot::triggerbot::knife_check;
	j["aimbot"]["triggerbot_reloading_check"] = settings::aimbot::triggerbot::reloading_check;
	j["aimbot"]["triggerbot_no_ammo_check"] = settings::aimbot::triggerbot::no_ammo_check;
	j["aimbot"]["triggerbot_crew_check"] = settings::aimbot::triggerbot::crew_check;
	j["aimbot"]["triggerbot_gun_switch_delay"] = settings::aimbot::triggerbot::gun_switch_delay;
	j["aimbot"]["triggerbot_hitbox_allowance"] = settings::aimbot::triggerbot::hitbox_allowance;
	j["aimbot"]["triggerbot_trigger_when_host_shoots"] = settings::aimbot::triggerbot::trigger_when_host_shoots;
	j["aimbot"]["triggerbot_host_delay"] = settings::aimbot::triggerbot::host_delay;
	j["aimbot"]["triggerbot_host_duration"] = settings::aimbot::triggerbot::host_duration;
	j["aimbot"]["triggerbot_host_player_index"] = settings::aimbot::triggerbot::host_player_index;
	j["aimbot"]["triggerbot_fov_perspective"] = settings::aimbot::triggerbot::fov_perspective;

	// -- silentaim --
	j["silentaim"]["enabled"] = settings::silentaim::enabled;
	j["silentaim"]["keybind"] = settings::silentaim::keybind;
	j["silentaim"]["activation_mode"] = settings::silentaim::activation_mode;
	j["silentaim"]["target_part"] = settings::silentaim::target_part;
	j["silentaim"]["closest_point_scale"] = settings::silentaim::closest_point_scale;
	j["silentaim"]["fov"] = settings::silentaim::fov;
	j["silentaim"]["use_fov"] = settings::silentaim::use_fov;
	j["silentaim"]["draw_fov"] = settings::silentaim::draw_fov;
	j["silentaim"]["attach_fov_to_target"] = settings::silentaim::attach_fov_to_target;
	j["silentaim"]["fov_circle_colour"] = to_json_array(settings::silentaim::fov_circle_colour);
	j["silentaim"]["fov_outline_colour"] = to_json_array(settings::silentaim::fov_outline_colour);
	j["silentaim"]["fov_shape"] = settings::silentaim::fov_shape;
	j["silentaim"]["enable_prediction"] = settings::silentaim::enable_prediction;
	j["silentaim"]["prediction_x"] = settings::silentaim::prediction_x;
	j["silentaim"]["prediction_y"] = settings::silentaim::prediction_y;
	j["silentaim"]["sticky_aim"] = settings::silentaim::sticky_aim;
	j["silentaim"]["auto_switch"] = settings::silentaim::auto_switch;
	j["silentaim"]["spoof_mouse"] = settings::silentaim::spoof_mouse;
	j["silentaim"]["silent_mode"] = settings::silentaim::silent_mode;
	j["silentaim"]["use_aimbot_target"] = settings::silentaim::use_aimbot_target;
	j["silentaim"]["teamcheck"] = settings::silentaim::teamcheck;
	j["silentaim"]["wallcheck"] = settings::silentaim::wallcheck;
	j["silentaim"]["grabbed_check"] = settings::silentaim::grabbed_check;
	j["silentaim"]["forcefield_check"] = settings::silentaim::forcefield_check;
	j["silentaim"]["guncheck"] = settings::silentaim::guncheck;
	j["silentaim"]["knock_check"] = settings::silentaim::knock_check;
	j["silentaim"]["npc_check"] = settings::silentaim::npc_check;
	j["silentaim"]["priorities"] = settings::silentaim::priorities;
	j["silentaim"]["health_check_enabled"] = settings::silentaim::health_check_enabled;
	j["silentaim"]["min_health"] = settings::silentaim::min_health;
	j["silentaim"]["hitchance_enabled"] = settings::silentaim::hitchance_enabled;
	j["silentaim"]["hitchance"] = settings::silentaim::hitchance;
	j["silentaim"]["draw_target_dot"] = settings::silentaim::draw_target_dot;
	j["silentaim"]["target_dot_color"] = to_json_array(settings::silentaim::target_dot_color);
	j["silentaim"]["target_dot_size"] = settings::silentaim::target_dot_size;
	j["silentaim"]["draw_snap_line"] = settings::silentaim::draw_snap_line;
	j["silentaim"]["snap_line_color"] = to_json_array(settings::silentaim::snap_line_color);

	// -- silentaim triggerbot --
	j["silentaim"]["triggerbot_enabled"] = settings::silentaim::triggerbot::enabled;
	j["silentaim"]["triggerbot_keybind"] = settings::silentaim::triggerbot::keybind;
	j["silentaim"]["triggerbot_activation_mode"] = settings::silentaim::triggerbot::activation_mode;
	j["silentaim"]["triggerbot_fire_mode"] = settings::silentaim::triggerbot::fire_mode;
	j["silentaim"]["triggerbot_clicks_per_second"] = settings::silentaim::triggerbot::clicks_per_second;
	j["silentaim"]["triggerbot_hold_duration"] = settings::silentaim::triggerbot::hold_duration;
	j["silentaim"]["triggerbot_reaction_ms"] = settings::silentaim::triggerbot::reaction_ms;
	j["silentaim"]["triggerbot_max_distance"] = settings::silentaim::triggerbot::max_distance;
	j["silentaim"]["triggerbot_max_distance_enabled"] = settings::silentaim::triggerbot::max_distance_enabled;
	j["silentaim"]["triggerbot_wallcheck"] = settings::silentaim::triggerbot::wallcheck;
	j["silentaim"]["triggerbot_headshot_only"] = settings::silentaim::triggerbot::headshot_only;

	// -- rage --
	j["rage"]["hitsounds"] = settings::rage::hitsounds;
	j["rage"]["hitsound_type"] = settings::rage::hitsound_type;
	j["rage"]["hitsound_method"] = settings::rage::hitsound_method;
	j["rage"]["rapidfire"] = settings::rage::rapidfire;
	j["rage"]["noclip"] = settings::rage::noclip;
	j["rage"]["noclip_keybind"] = settings::rage::noclip_keybind;
	j["rage"]["noclip_activation_mode"] = settings::rage::noclip_activation_mode;
	j["rage"]["hit_tracers"] = settings::rage::hit_tracers;
	j["rage"]["hit_tracers_color"] = to_json_array(settings::rage::hit_tracers_color);
	j["rage"]["hit_tracers_duration"] = settings::rage::hit_tracers_duration;

	// -- rage hipheight --
	j["rage"]["hipheight_enabled"] = settings::rage::hipheight::enabled;
	j["rage"]["hipheight_height"] = settings::rage::hipheight::height;

	// -- rage hitbox_expander --
	j["rage"]["hitbox_expander_enabled"] = settings::rage::hitbox_expander::enabled;
	j["rage"]["hitbox_expander_expand_all_parts"] = settings::rage::hitbox_expander::expand_all_parts;
	j["rage"]["hitbox_expander_size_x"] = settings::rage::hitbox_expander::size_x;
	j["rage"]["hitbox_expander_size_y"] = settings::rage::hitbox_expander::size_y;
	j["rage"]["hitbox_expander_size_z"] = settings::rage::hitbox_expander::size_z;
	j["rage"]["hitbox_expander_knock_check"] = settings::rage::hitbox_expander::knock_check;
	j["rage"]["hitbox_expander_anti_collision"] = settings::rage::hitbox_expander::anti_collision;
	j["rage"]["hitbox_expander_head_size_x"] = settings::rage::hitbox_expander::head_size_x;
	j["rage"]["hitbox_expander_head_size_y"] = settings::rage::hitbox_expander::head_size_y;
	j["rage"]["hitbox_expander_head_size_z"] = settings::rage::hitbox_expander::head_size_z;
	j["rage"]["hitbox_expander_torso_size_x"] = settings::rage::hitbox_expander::torso_size_x;
	j["rage"]["hitbox_expander_torso_size_y"] = settings::rage::hitbox_expander::torso_size_y;
	j["rage"]["hitbox_expander_torso_size_z"] = settings::rage::hitbox_expander::torso_size_z;
	j["rage"]["hitbox_expander_limb_size_x"] = settings::rage::hitbox_expander::limb_size_x;
	j["rage"]["hitbox_expander_limb_size_y"] = settings::rage::hitbox_expander::limb_size_y;
	j["rage"]["hitbox_expander_limb_size_z"] = settings::rage::hitbox_expander::limb_size_z;

	// -- rage spin360 --
	j["rage"]["spin360_enabled"] = settings::rage::spin360::enabled;
	j["rage"]["spin360_speed"] = settings::rage::spin360::speed;
	j["rage"]["spin360_keybind"] = settings::rage::spin360::keybind;
	j["rage"]["spin360_activation_mode"] = settings::rage::spin360::activation_mode;

	// -- desync --
	j["desync"]["enabled"] = settings::desync::enabled;
	j["desync"]["keybind"] = settings::desync::keybind;
	j["desync"]["keybind_mode"] = settings::desync::keybind_mode;
	j["desync"]["visualizer_enabled"] = settings::desync::visualizer::enabled;
	j["desync"]["visualizer_color"] = to_json_array(settings::desync::visualizer::color);
	j["desync"]["visualizer_thickness"] = settings::desync::visualizer::thickness;
	j["desync"]["visualizer_pulse"] = settings::desync::visualizer::pulse;

	// -- magicbullet --
	j["magicbullet"]["enabled"] = settings::magicbullet::enabled;
	j["magicbullet"]["keybind"] = settings::magicbullet::keybind;
	j["magicbullet"]["activation_mode"] = settings::magicbullet::activation_mode;
	j["magicbullet"]["target_source"] = settings::magicbullet::target_source;
	j["magicbullet"]["offset_distance"] = settings::magicbullet::offset_distance;
	j["magicbullet"]["hold_ms"] = settings::magicbullet::hold_ms;
	j["magicbullet"]["tp_iterations"] = settings::magicbullet::tp_iterations;

	// -- infinite_ammo --
	j["infinite_ammo"]["enabled"] = settings::infinite_ammo::enabled;
	j["infinite_ammo"]["universal_mode"] = settings::infinite_ammo::universal_mode;
	j["infinite_ammo"]["ammo_value"] = settings::infinite_ammo::ammo_value;
	j["infinite_ammo"]["preserve_mag"] = settings::infinite_ammo::preserve_mag;
	j["infinite_ammo"]["no_reload"] = settings::infinite_ammo::no_reload;
	j["infinite_ammo"]["detection_method"] = settings::infinite_ammo::detection_method;

	// -- lua_executor --
	j["lua_executor"]["enabled"] = settings::lua_executor::enabled;
	j["lua_executor"]["auto_inject"] = settings::lua_executor::auto_inject;
	j["lua_executor"]["execution_mode"] = settings::lua_executor::execution_mode;
	j["lua_executor"]["bypass_detection"] = settings::lua_executor::bypass_detection;
	j["lua_executor"]["inject_delay_ms"] = settings::lua_executor::inject_delay_ms;
	j["lua_executor"]["clear_on_execute"] = settings::lua_executor::clear_on_execute;

	// -- visuals --
	j["visuals"]["enable_enemies"] = settings::visuals::enable_enemies;
	j["visuals"]["enable_client"] = settings::visuals::enable_client;
	j["visuals"]["box"] = settings::visuals::box;
	j["visuals"]["box_type"] = settings::visuals::box_type;
	j["visuals"]["box_color"] = to_json_array(settings::visuals::box_color);
	j["visuals"]["box_fill"] = settings::visuals::box_fill;
	j["visuals"]["box_fill_color"] = to_json_array(settings::visuals::box_fill_color);
	j["visuals"]["name"] = settings::visuals::name;
	j["visuals"]["name_type"] = settings::visuals::name_type;
	j["visuals"]["name_display_type"] = settings::visuals::name_display_type;
	j["visuals"]["name_color"] = to_json_array(settings::visuals::name_color);
	j["visuals"]["name_color_blend_start"] = to_json_array(settings::visuals::name_color_blend_start);
	j["visuals"]["name_color_blend_end"] = to_json_array(settings::visuals::name_color_blend_end);
	j["visuals"]["blend"] = settings::visuals::blend;
	j["visuals"]["avatar"] = settings::visuals::avatar;
	j["visuals"]["healthbar"] = settings::visuals::healthbar;
	j["visuals"]["healthbar_color"] = to_json_array(settings::visuals::healthbar_color);
	j["visuals"]["health_based_healthbar"] = settings::visuals::health_based_healthbar;
	j["visuals"]["gradient_healthbar"] = settings::visuals::gradient_healthbar;
	j["visuals"]["gradient_healthbar_color_start"] = to_json_array(settings::visuals::gradient_healthbar_color_start);
	j["visuals"]["gradient_healthbar_color_end"] = to_json_array(settings::visuals::gradient_healthbar_color_end);
	j["visuals"]["health_percent"] = settings::visuals::health_percent;
	j["visuals"]["health_percent_color"] = to_json_array(settings::visuals::health_percent_color);
	j["visuals"]["armorbar"] = settings::visuals::armorbar;
	j["visuals"]["armorbar_color"] = to_json_array(settings::visuals::armorbar_color);
	j["visuals"]["distance"] = settings::visuals::distance;
	j["visuals"]["distance_measurement"] = settings::visuals::distance_measurement;
	j["visuals"]["distance_color"] = to_json_array(settings::visuals::distance_color);
	j["visuals"]["tool"] = settings::visuals::tool;
	j["visuals"]["tool_color"] = to_json_array(settings::visuals::tool_color);
	j["visuals"]["esp_font"] = settings::visuals::esp_font;
	j["visuals"]["local_player"] = settings::visuals::local_player;
	j["visuals"]["chams"] = settings::visuals::chams;
	j["visuals"]["chams_type"] = settings::visuals::chams_type;
	j["visuals"]["chams_fill_color"] = to_json_array(settings::visuals::chams_fill_color);
	j["visuals"]["chams_outline_color"] = to_json_array(settings::visuals::chams_outline_color);
	j["visuals"]["chams_fill_enabled"] = settings::visuals::chams_fill_enabled;
	j["visuals"]["chams_outline_enabled"] = settings::visuals::chams_outline_enabled;
	j["visuals"]["target_warning_icon"] = settings::visuals::target_warning_icon;
	j["visuals"]["target_warning_icon_size"] = settings::visuals::target_warning_icon_size;
	j["visuals"]["flags"] = settings::visuals::flags;
	j["visuals"]["flags_state_colour"] = to_json_array(settings::visuals::flags_state_colour);
	j["visuals"]["client_box"] = settings::visuals::client_box;
	j["visuals"]["client_box_color"] = to_json_array(settings::visuals::client_box_color);
	j["visuals"]["client_box_fill"] = settings::visuals::client_box_fill;
	j["visuals"]["client_box_fill_color"] = to_json_array(settings::visuals::client_box_fill_color);
	j["visuals"]["client_name"] = settings::visuals::client_name;
	j["visuals"]["client_name_color"] = to_json_array(settings::visuals::client_name_color);
	j["visuals"]["client_avatar"] = settings::visuals::client_avatar;
	j["visuals"]["client_healthbar"] = settings::visuals::client_healthbar;
	j["visuals"]["client_healthbar_color"] = to_json_array(settings::visuals::client_healthbar_color);
	j["visuals"]["client_health_percent"] = settings::visuals::client_health_percent;
	j["visuals"]["client_health_percent_color"] = to_json_array(settings::visuals::client_health_percent_color);
	j["visuals"]["client_armorbar"] = settings::visuals::client_armorbar;
	j["visuals"]["client_armorbar_color"] = to_json_array(settings::visuals::client_armorbar_color);
	j["visuals"]["client_distance"] = settings::visuals::client_distance;
	j["visuals"]["client_distance_color"] = to_json_array(settings::visuals::client_distance_color);
	j["visuals"]["client_tool"] = settings::visuals::client_tool;
	j["visuals"]["client_tool_color"] = to_json_array(settings::visuals::client_tool_color);
	j["visuals"]["client_chams"] = settings::visuals::client_chams;
	j["visuals"]["client_chams_fill_color"] = to_json_array(settings::visuals::client_chams_fill_color);
	j["visuals"]["client_chams_outline_color"] = to_json_array(settings::visuals::client_chams_outline_color);
	j["visuals"]["client_flags"] = settings::visuals::client_flags;
	j["visuals"]["client_flags_state_colour"] = to_json_array(settings::visuals::client_flags_state_colour);
	j["visuals"]["client_headless"] = settings::visuals::client_headless;
	j["visuals"]["client_korblox"] = settings::visuals::client_korblox;
	j["visuals"]["debug_wallcheck"] = settings::visuals::debug_wallcheck;
	j["visuals"]["view_hitbox"] = settings::visuals::view_hitbox;
	j["visuals"]["view_hitbox_color"] = to_json_array(settings::visuals::view_hitbox_color);
	j["visuals"]["fade_in_speed"] = settings::visuals::fade_in_speed;
	j["visuals"]["fade_out_speed"] = settings::visuals::fade_out_speed;
	j["visuals"]["knock_check"] = settings::visuals::knock_check;
	j["visuals"]["teamcheck"] = settings::visuals::teamcheck;
	j["visuals"]["use_team_color"] = settings::visuals::use_team_color;
	j["visuals"]["ignore_whitelisted"] = settings::visuals::ignore_whitelisted;
	j["visuals"]["max_distance_enabled"] = settings::visuals::max_distance_enabled;
	j["visuals"]["max_distance"] = settings::visuals::max_distance;
	j["visuals"]["esp_keybind"] = settings::visuals::esp_keybind;
	j["visuals"]["esp_keybind_mode"] = settings::visuals::esp_keybind_mode;
	j["visuals"]["mm2_esp"] = settings::visuals::mm2_esp;
	j["visuals"]["hit_tracers_enabled"] = settings::visuals::hit_tracers_enabled;
	j["visuals"]["hit_tracers_method"] = settings::visuals::hit_tracers_method;
	j["visuals"]["hit_tracers_color"] = to_json_array(settings::visuals::hit_tracers_color);
	j["visuals"]["hit_tracers_duration"] = settings::visuals::hit_tracers_duration;
	j["visuals"]["skeleton"] = settings::visuals::skeleton;
	j["visuals"]["skeleton_color"] = to_json_array(settings::visuals::skeleton_color);
	j["visuals"]["custom_preview_show_esp"] = settings::visuals::custom_preview_show_esp;
	j["visuals"]["custom_preview_rotate"] = settings::visuals::custom_preview_rotate;
	j["visuals"]["custom_preview_rotation_speed"] = settings::visuals::custom_preview_rotation_speed;
	j["visuals"]["character_preview"] = settings::visuals::character_preview;
	j["visuals"]["preview_size"] = settings::visuals::preview_size;

	// -- movement --
	j["movement"]["speedhack_enabled"] = settings::movement::speedhack::enabled;
	j["movement"]["speedhack_mode"] = settings::movement::speedhack::mode;
	j["movement"]["speedhack_speed"] = settings::movement::speedhack::speed;
	j["movement"]["speedhack_keybind"] = settings::movement::speedhack::keybind;
	j["movement"]["speedhack_activation_mode"] = settings::movement::speedhack::activation_mode;
	j["movement"]["jumphack_enabled"] = settings::movement::jumphack::enabled;
	j["movement"]["jumphack_value"] = settings::movement::jumphack::value;
	j["movement"]["jumphack_keybind"] = settings::movement::jumphack::keybind;
	j["movement"]["jumphack_activation_mode"] = settings::movement::jumphack::activation_mode;
	j["movement"]["nojumpcooldown_enabled"] = settings::movement::nojumpcooldown::enabled;
	j["movement"]["jump_power_enabled"] = settings::movement::jump_power::enabled;
	j["movement"]["jump_power_value"] = settings::movement::jump_power::value;
	j["movement"]["jump_power_jump_cd"] = settings::movement::jump_power::jump_cd;
	j["movement"]["jump_power_keybind"] = settings::movement::jump_power::keybind;
	j["movement"]["jump_power_activation_mode"] = settings::movement::jump_power::activation_mode;
	j["movement"]["flyhack_enabled"] = settings::movement::flyhack::enabled;
	j["movement"]["flyhack_mode"] = settings::movement::flyhack::mode;
	j["movement"]["flyhack_speed"] = settings::movement::flyhack::speed;
	j["movement"]["flyhack_keybind"] = settings::movement::flyhack::keybind;
	j["movement"]["flyhack_activation_mode"] = settings::movement::flyhack::activation_mode;
	j["movement"]["tickrate_enabled"] = settings::movement::tickrate::enabled;
	j["movement"]["tickrate_value"] = settings::movement::tickrate::value;
	j["movement"]["orbit_enabled"] = settings::movement::orbit::enabled;
	j["movement"]["orbit_type"] = settings::movement::orbit::orbit_type;
	j["movement"]["orbit_speed"] = settings::movement::orbit::speed;
	j["movement"]["orbit_radius"] = settings::movement::orbit::radius;
	j["movement"]["orbit_height_offset"] = settings::movement::orbit::height_offset;
	j["movement"]["orbit_spectate_target"] = settings::movement::orbit::spectate_target;
	j["movement"]["orbit_randomize"] = settings::movement::orbit::randomize;
	j["movement"]["orbit_randomize_x"] = settings::movement::orbit::randomize_x;
	j["movement"]["orbit_randomize_y"] = settings::movement::orbit::randomize_y;
	j["movement"]["gravity_enabled"] = settings::movement::gravity::enabled;
	j["movement"]["gravity_value"] = settings::movement::gravity::value;

	// -- exploits --
	j["exploits"]["antiafk_enabled"] = settings::exploits::antiafk::enabled;
	j["exploits"]["freezeplayer_enabled"] = settings::exploits::freezeplayer::enabled;
	j["exploits"]["freezeplayer_keybind"] = settings::exploits::freezeplayer::keybind;
	j["exploits"]["freezeplayer_activation_mode"] = settings::exploits::freezeplayer::activation_mode;

	// -- cilent fpscaps --
	j["cilent"]["fpscaps_enabled"] = settings::cilent::fpscaps::enabled;

	// -- extras --
	j["extras"]["crosshair"] = settings::extras::crosshair;
	j["extras"]["crosshair_size"] = settings::extras::crosshair_size;
	j["extras"]["crosshair_gap"] = settings::extras::crosshair_gap;
	j["extras"]["crosshair_thickness"] = settings::extras::crosshair_thickness;
	j["extras"]["crosshair_color"] = to_json_array(settings::extras::crosshair_color);
	j["extras"]["bhop"] = settings::extras::bhop;
	j["extras"]["esp_tracers"] = settings::extras::esp_tracers;
	j["extras"]["esp_tracers_color"] = to_json_array(settings::extras::esp_tracers_color);
	j["extras"]["notifications"] = settings::extras::notifications;
	j["extras"]["keybind_list"] = settings::extras::keybind_list;
	j["extras"]["thirdperson"] = settings::extras::thirdperson;
	j["extras"]["custom_fov"] = settings::extras::custom_fov;
	j["extras"]["custom_fov_enabled"] = settings::extras::custom_fov_enabled;
	j["extras"]["instant_respawn"] = settings::extras::instant_respawn;
	j["extras"]["ragetp"] = settings::extras::ragetp;
	j["extras"]["ragetp_key"] = settings::extras::ragetp_key;
	j["extras"]["ragetp_range"] = settings::extras::ragetp_range;
	j["extras"]["ragetp_autoshoot"] = settings::extras::ragetp_autoshoot;
	j["extras"]["ragetp_magicbullet"] = settings::extras::ragetp_magicbullet;
	j["extras"]["autofarm"] = settings::extras::autofarm;
	j["extras"]["autofarm_range"] = settings::extras::autofarm_range;
	j["extras"]["anti_lock"] = settings::extras::anti_lock;
	j["extras"]["no_register"] = settings::extras::no_register;
	j["extras"]["macro_glitch"] = settings::extras::macro_glitch;
	j["extras"]["macro_glitch_key"] = settings::extras::macro_glitch_key;
	j["extras"]["macro_normal_speed"] = settings::extras::macro_normal_speed;
	j["extras"]["macro_boost_speed"] = settings::extras::macro_boost_speed;
	j["extras"]["macro_interval"] = settings::extras::macro_interval;

	// -- ui --
	j["ui"]["watermark"] = settings::ui::watermark;
	j["ui"]["keybinds"] = settings::ui::keybinds;
	j["ui"]["accent_color"] = settings::ui::accent_color;
	j["ui"]["custom_accent_color"] = to_json_array(settings::ui::custom_accent_color);
	j["ui"]["menu_opacity"] = settings::ui::menu_opacity;
	j["ui"]["rounded_corners"] = settings::ui::rounded_corners;
	j["ui"]["compact_mode"] = settings::ui::compact_mode;
	j["ui"]["show_icons"] = settings::ui::show_icons;
	j["ui"]["anim_transitions"] = settings::ui::anim_transitions;
	j["ui"]["animation_speed"] = settings::ui::animation_speed;

	// -- watermark --
	j["watermark"]["show_cheat_name"] = settings::watermark::show_cheat_name;
	j["watermark"]["show_display_name"] = settings::watermark::show_display_name;
	j["watermark"]["show_username"] = settings::watermark::show_username;
	j["watermark"]["show_fps"] = settings::watermark::show_fps;
	j["watermark"]["show_server_ip"] = settings::watermark::show_server_ip;
	j["watermark"]["show_ping"] = settings::watermark::show_ping;
	j["watermark"]["separator_type"] = settings::watermark::separator_type;
	j["watermark"]["text_color"] = to_json_array(settings::watermark::text_color);
	j["watermark"]["rainbow"] = settings::watermark::rainbow;
	j["watermark"]["rainbow_speed"] = settings::watermark::rainbow_speed;
	j["watermark"]["pos_x"] = settings::watermark::pos_x;
	j["watermark"]["pos_y"] = settings::watermark::pos_y;

	// -- lighting --
	j["lighting"]["fog_enabled"] = settings::lighting::fog::enabled;
	j["lighting"]["fog_start"] = settings::lighting::fog::fog_start;
	j["lighting"]["fog_end"] = settings::lighting::fog::fog_end;
	j["lighting"]["fog_r"] = settings::lighting::fog::fog_r;
	j["lighting"]["fog_g"] = settings::lighting::fog::fog_g;
	j["lighting"]["fog_b"] = settings::lighting::fog::fog_b;
	j["lighting"]["shadows_disable"] = settings::lighting::shadows::disable;
	j["lighting"]["clocktime_enabled"] = settings::lighting::clocktime::enabled;
	j["lighting"]["clocktime_value"] = settings::lighting::clocktime::clock_time;
	j["lighting"]["skybox_enabled"] = settings::lighting::skybox::enabled;
	j["lighting"]["skybox_preset_index"] = settings::lighting::skybox::preset_index;
	j["lighting"]["exposure_enabled"] = settings::lighting::exposure::enabled;
	j["lighting"]["exposure_value"] = settings::lighting::exposure::exposure;

	// -- football --
	j["football"]["autodive_enabled"] = settings::football::autodive_enabled;
	j["football"]["auto_m2"] = settings::football::auto_m2;
	j["football"]["auto_m2_from_key"] = settings::football::auto_m2_from_key;
	j["football"]["show_prediction"] = settings::football::show_prediction;
	j["football"]["show_zones"] = settings::football::show_zones;
	j["football"]["mode_a_dive_offset"] = settings::football::mode_a_dive_offset;
	j["football"]["mode_a_top_threshold"] = settings::football::mode_a_top_threshold;
	j["football"]["mode_b_enabled"] = settings::football::mode_b_enabled;
	j["football"]["mode_b_dive_offset"] = settings::football::mode_b_dive_offset;
	j["football"]["mode_b_top_threshold"] = settings::football::mode_b_top_threshold;
	j["football"]["mode_b_duration"] = settings::football::mode_b_duration;
	j["football"]["mode_b_effect_enabled"] = settings::football::mode_b_effect_enabled;
	j["football"]["mid_top_threshold"] = settings::football::mid_top_threshold;
	j["football"]["mid_iframe_top_threshold"] = settings::football::mid_iframe_top_threshold;
	j["football"]["zone_scale_x"] = settings::football::zone_scale_x;
	j["football"]["zone_scale_y"] = settings::football::zone_scale_y;
	j["football"]["panel_behind_dist"] = settings::football::panel_behind_dist;
	j["football"]["panel_height_adj"] = settings::football::panel_height_adj;
	j["football"]["dive_key_space"] = settings::football::dive_key_space;
	j["football"]["dive_key_left"] = settings::football::dive_key_left;
	j["football"]["dive_key_right"] = settings::football::dive_key_right;
	j["football"]["dive_key_middle"] = settings::football::dive_key_middle;
	j["football"]["dive_key_m2"] = settings::football::dive_key_m2;
	j["football"]["random_dive_enabled"] = settings::football::random_dive_enabled;
	j["football"]["random_dive_offset_reduction"] = settings::football::random_dive_offset_reduction;
	j["football"]["dive_cooldown"] = settings::football::dive_cooldown;
	j["football"]["dive_jump_delay"] = settings::football::dive_jump_delay;
	j["football"]["autodive_key"] = settings::football::autodive_key;
	j["football"]["autodive_key_mode"] = settings::football::autodive_key_mode;
	j["football"]["auto_m2_key"] = settings::football::auto_m2_key;
	j["football"]["auto_m2_key_mode"] = settings::football::auto_m2_key_mode;
	j["football"]["panel_vis_key"] = settings::football::panel_vis_key;
	j["football"]["auto_m2_dive_offset"] = settings::football::auto_m2_dive_offset;
	j["football"]["auto_m2_mode_b_dive_offset"] = settings::football::auto_m2_mode_b_dive_offset;
	j["football"]["auto_m2_cooldown"] = settings::football::auto_m2_cooldown;
	j["football"]["m2_jump_delay"] = settings::football::m2_jump_delay;
	j["football"]["post_guard_enabled"] = settings::football::post_guard_enabled;
	j["football"]["post_guard_distance"] = settings::football::post_guard_distance;
	j["football"]["post_guard_offset_bonus"] = settings::football::post_guard_offset_bonus;
	j["football"]["gravity_mult"] = settings::football::gravity_mult;
	j["football"]["ground_y"] = settings::football::ground_y;
	j["football"]["bounce_vel_y"] = settings::football::bounce_vel_y;
	j["football"]["bounce_friction"] = settings::football::bounce_friction;
	j["football"]["rolling_friction"] = settings::football::rolling_friction;
	j["football"]["stop_velocity"] = settings::football::stop_velocity;
	j["football"]["min_bounce_velocity"] = settings::football::min_bounce_velocity;
	j["football"]["mid_fraction"] = settings::football::mid_fraction;
	j["football"]["mid_side_fraction"] = settings::football::mid_side_fraction;
	j["football"]["ball_name"] = settings::football::ball_name;
	j["football"]["show_path"] = settings::football::show_path;
	j["football"]["show_panel"] = settings::football::show_panel;

	j["football"]["rotdive_enabled"] = settings::football::rotdive_enabled;
	j["football"]["rotdive_key"] = settings::football::rotdive_key;
	j["football"]["rotdive_key_mode"] = settings::football::rotdive_key_mode;
	j["football"]["rotdive_leap_key"] = settings::football::rotdive_leap_key;
	j["football"]["rotdive_delay"] = settings::football::rotdive_delay;
	j["football"]["rotdive_hold"] = settings::football::rotdive_hold;
	j["football"]["rotdive_cooldown"] = settings::football::rotdive_cooldown;

	// -- playerlist whitelist --
	{
		json arr = json::array();
		for (auto& name : settings::rage::playerlist::whitelist)
			arr.push_back(name);
		j["playerlist_whitelist"] = arr;
	}

	// -- custom_entities --
	j["custom_entities"]["current_input"] = settings::custom_entities::current_input;
	j["custom_entities"]["auto_refresh"] = settings::custom_entities::auto_refresh;
	j["custom_entities"]["refresh_rate"] = settings::custom_entities::refresh_rate;

	std::ofstream f(get_config_path());
	if (f.is_open())
		f << j.dump(2);
}

void config::load()
{
	std::ifstream f(get_config_path());
	if (!f.is_open()) return;
	json j;
	try { j = json::parse(f); }
	catch (...) { return; }

	// -- menu --
	if (j.contains("menu")) {
		auto& m = j["menu"];
		settings::menu::menu_keybind = m.value("menu_keybind", VK_INSERT);
		settings::menu::watermark = m.value("watermark", false);
		settings::menu::streamproof = m.value("streamproof", false);
		settings::menu::cursor_streamproof = m.value("cursor_streamproof", false);
		settings::menu::auto_inject = m.value("auto_inject", false);
		settings::menu::unload = m.value("unload", false);
		settings::menu::vsync = m.value("vsync", false);
		settings::menu::frame_limiter_ms = m.value("frame_limiter_ms", 8);
		settings::menu::performance_mode = m.value("performance_mode", false);
	}

	// -- aimbot --
	if (j.contains("aimbot")) {
		auto& a = j["aimbot"];
		settings::aimbot::enabled = a.value("enabled", false);
		settings::aimbot::keybind = a.value("keybind", 0);
		settings::aimbot::activation_mode = a.value("activation_mode", 1);
		settings::aimbot::mode = a.value("mode", 1);
		settings::aimbot::aim_type = a.value("aim_type", 0);
		settings::aimbot::target_part = a.value("target_part", 1);
		settings::aimbot::air_part_enabled = a.value("air_part_enabled", false);
		settings::aimbot::air_part = a.value("air_part", 1);
		settings::aimbot::fov = a.value("fov", 100.f);
		settings::aimbot::use_fov = a.value("use_fov", false);
		settings::aimbot::draw_fov = a.value("draw_fov", false);
		from_json_array(a.value("fov_circle_colour", json::array()), settings::aimbot::fov_circle_colour);
		from_json_array(a.value("fov_outline_colour", json::array()), settings::aimbot::fov_outline_colour);
		settings::aimbot::smoothing = a.value("smoothing", false);
		settings::aimbot::smoothingx = a.value("smoothingx", 10.f);
		settings::aimbot::smoothingy = a.value("smoothingy", 10.f);
		settings::aimbot::enable_prediction = a.value("enable_prediction", false);
		settings::aimbot::prediction_x = a.value("prediction_x", 10.f);
		settings::aimbot::prediction_y = a.value("prediction_y", 10.f);
		settings::aimbot::air_prediction_enabled = a.value("air_prediction_enabled", false);
		settings::aimbot::air_prediction_x = a.value("air_prediction_x", 10.f);
		settings::aimbot::air_prediction_y = a.value("air_prediction_y", 10.f);
		settings::aimbot::smoothing_style = a.value("smoothing_style", 0);
		settings::aimbot::teamcheck = a.value("teamcheck", false);
		settings::aimbot::knock_check = a.value("knock_check", false);
		settings::aimbot::wallcheck = a.value("wallcheck", false);
		settings::aimbot::grabbed_check = a.value("grabbed_check", false);
		settings::aimbot::forcefield_check = a.value("forcefield_check", false);
		settings::aimbot::npc_check = a.value("npc_check", false);
		settings::aimbot::reloading_check = a.value("reloading_check", false);
		settings::aimbot::no_ammo_check = a.value("no_ammo_check", false);
		settings::aimbot::sticky_aim = a.value("sticky_aim", false);
		settings::aimbot::anti_shake = a.value("anti_shake", false);
		settings::aimbot::health_check_enabled = a.value("health_check_enabled", false);
		settings::aimbot::min_health = a.value("min_health", 0.f);
		settings::aimbot::fov_shape = a.value("fov_shape", 0);
		settings::aimbot::offset_enabled = a.value("offset_enabled", false);
		settings::aimbot::offset_x = a.value("offset_x", 0.f);
		settings::aimbot::offset_y = a.value("offset_y", 0.f);
		settings::aimbot::ai_silent_y_offset = a.value("ai_silent_y_offset", 0.5f);

		settings::aimbot::triggerbot::enabled = a.value("triggerbot_enabled", false);
		settings::aimbot::triggerbot::keybind = a.value("triggerbot_keybind", 0);
		settings::aimbot::triggerbot::activation_mode = a.value("triggerbot_activation_mode", 1);
		settings::aimbot::triggerbot::fire_mode = a.value("triggerbot_fire_mode", 0);
		settings::aimbot::triggerbot::clicks_per_second = a.value("triggerbot_clicks_per_second", 10.f);
		settings::aimbot::triggerbot::hold_duration = a.value("triggerbot_hold_duration", 0.15f);
		settings::aimbot::triggerbot::reaction_ms = a.value("triggerbot_reaction_ms", 80.f);
		settings::aimbot::triggerbot::max_distance = a.value("triggerbot_max_distance", 300.f);
		settings::aimbot::triggerbot::max_distance_enabled = a.value("triggerbot_max_distance_enabled", false);
		settings::aimbot::triggerbot::wallcheck = a.value("triggerbot_wallcheck", false);
		settings::aimbot::triggerbot::headshot_only = a.value("triggerbot_headshot_only", false);
		settings::aimbot::triggerbot::knife_check = a.value("triggerbot_knife_check", false);
		settings::aimbot::triggerbot::reloading_check = a.value("triggerbot_reloading_check", false);
		settings::aimbot::triggerbot::no_ammo_check = a.value("triggerbot_no_ammo_check", false);
		settings::aimbot::triggerbot::crew_check = a.value("triggerbot_crew_check", false);
		settings::aimbot::triggerbot::gun_switch_delay = a.value("triggerbot_gun_switch_delay", 0.f);
		settings::aimbot::triggerbot::hitbox_allowance = a.value("triggerbot_hitbox_allowance", 0.f);
		settings::aimbot::triggerbot::trigger_when_host_shoots = a.value("triggerbot_trigger_when_host_shoots", false);
		settings::aimbot::triggerbot::host_delay = a.value("triggerbot_host_delay", 0.f);
		settings::aimbot::triggerbot::host_duration = a.value("triggerbot_host_duration", 0.f);
		settings::aimbot::triggerbot::host_player_index = a.value("triggerbot_host_player_index", -1);
		settings::aimbot::triggerbot::fov_perspective = a.value("triggerbot_fov_perspective", 0);
	}

	// -- silentaim --
	if (j.contains("silentaim")) {
		auto& s = j["silentaim"];
		settings::silentaim::enabled = s.value("enabled", false);
		settings::silentaim::keybind = s.value("keybind", 0);
		settings::silentaim::activation_mode = s.value("activation_mode", 1);
		settings::silentaim::target_part = s.value("target_part", 1);
		settings::silentaim::closest_point_scale = s.value("closest_point_scale", 1.0f);
		settings::silentaim::fov = s.value("fov", 100.f);
		settings::silentaim::use_fov = s.value("use_fov", false);
		settings::silentaim::draw_fov = s.value("draw_fov", false);
		settings::silentaim::attach_fov_to_target = s.value("attach_fov_to_target", false);
		from_json_array(s.value("fov_circle_colour", json::array()), settings::silentaim::fov_circle_colour);
		from_json_array(s.value("fov_outline_colour", json::array()), settings::silentaim::fov_outline_colour);
		settings::silentaim::fov_shape = s.value("fov_shape", 0);
		settings::silentaim::enable_prediction = s.value("enable_prediction", false);
		settings::silentaim::prediction_x = s.value("prediction_x", 10.f);
		settings::silentaim::prediction_y = s.value("prediction_y", 10.f);
		settings::silentaim::sticky_aim = s.value("sticky_aim", false);
		settings::silentaim::auto_switch = s.value("auto_switch", false);
		settings::silentaim::spoof_mouse = s.value("spoof_mouse", true);
		settings::silentaim::silent_mode = s.value("silent_mode", 0);
		settings::silentaim::use_aimbot_target = s.value("use_aimbot_target", false);
		settings::silentaim::teamcheck = s.value("teamcheck", false);
		settings::silentaim::wallcheck = s.value("wallcheck", false);
		settings::silentaim::grabbed_check = s.value("grabbed_check", false);
		settings::silentaim::forcefield_check = s.value("forcefield_check", false);
		settings::silentaim::guncheck = s.value("guncheck", false);
		settings::silentaim::knock_check = s.value("knock_check", false);
		settings::silentaim::npc_check = s.value("npc_check", false);
		settings::silentaim::priorities = s.value("priorities", 0);
		settings::silentaim::health_check_enabled = s.value("health_check_enabled", false);
		settings::silentaim::min_health = s.value("min_health", 0.f);
		settings::silentaim::hitchance_enabled = s.value("hitchance_enabled", false);
		settings::silentaim::hitchance = s.value("hitchance", 100.f);
		settings::silentaim::draw_target_dot = s.value("draw_target_dot", false);
		from_json_array(s.value("target_dot_color", json::array()), settings::silentaim::target_dot_color);
		settings::silentaim::target_dot_size = s.value("target_dot_size", 4.f);
		settings::silentaim::draw_snap_line = s.value("draw_snap_line", false);
		from_json_array(s.value("snap_line_color", json::array()), settings::silentaim::snap_line_color);

		settings::silentaim::triggerbot::enabled = s.value("triggerbot_enabled", false);
		settings::silentaim::triggerbot::keybind = s.value("triggerbot_keybind", 0);
		settings::silentaim::triggerbot::activation_mode = s.value("triggerbot_activation_mode", 1);
		settings::silentaim::triggerbot::fire_mode = s.value("triggerbot_fire_mode", 0);
		settings::silentaim::triggerbot::clicks_per_second = s.value("triggerbot_clicks_per_second", 10.f);
		settings::silentaim::triggerbot::hold_duration = s.value("triggerbot_hold_duration", 0.15f);
		settings::silentaim::triggerbot::reaction_ms = s.value("triggerbot_reaction_ms", 80.f);
		settings::silentaim::triggerbot::max_distance = s.value("triggerbot_max_distance", 300.f);
		settings::silentaim::triggerbot::max_distance_enabled = s.value("triggerbot_max_distance_enabled", false);
		settings::silentaim::triggerbot::wallcheck = s.value("triggerbot_wallcheck", false);
		settings::silentaim::triggerbot::headshot_only = s.value("triggerbot_headshot_only", false);
	}

	// -- rage --
	if (j.contains("rage")) {
		auto& r = j["rage"];
		settings::rage::hitsounds = r.value("hitsounds", false);
		settings::rage::hitsound_type = r.value("hitsound_type", 0);
		settings::rage::hitsound_method = r.value("hitsound_method", 0);
		settings::rage::rapidfire = r.value("rapidfire", false);
		settings::rage::noclip = r.value("noclip", false);
		settings::rage::noclip_keybind = r.value("noclip_keybind", 0);
		settings::rage::noclip_activation_mode = r.value("noclip_activation_mode", 1);
		settings::rage::hit_tracers = r.value("hit_tracers", false);
		from_json_array(r.value("hit_tracers_color", json::array()), settings::rage::hit_tracers_color);
		settings::rage::hit_tracers_duration = r.value("hit_tracers_duration", 1.0f);
		settings::rage::hipheight::enabled = r.value("hipheight_enabled", false);
		settings::rage::hipheight::height = r.value("hipheight_height", 2.0f);
		settings::rage::hitbox_expander::enabled = r.value("hitbox_expander_enabled", false);
		settings::rage::hitbox_expander::expand_all_parts = r.value("hitbox_expander_expand_all_parts", false);
		settings::rage::hitbox_expander::size_x = r.value("hitbox_expander_size_x", 2.2f);
		settings::rage::hitbox_expander::size_y = r.value("hitbox_expander_size_y", 2.2f);
		settings::rage::hitbox_expander::size_z = r.value("hitbox_expander_size_z", 1.2f);
		settings::rage::hitbox_expander::knock_check = r.value("hitbox_expander_knock_check", false);
		settings::rage::hitbox_expander::anti_collision = r.value("hitbox_expander_anti_collision", true);
		settings::rage::hitbox_expander::head_size_x = r.value("hitbox_expander_head_size_x", 2.5f);
		settings::rage::hitbox_expander::head_size_y = r.value("hitbox_expander_head_size_y", 2.5f);
		settings::rage::hitbox_expander::head_size_z = r.value("hitbox_expander_head_size_z", 2.0f);
		settings::rage::hitbox_expander::torso_size_x = r.value("hitbox_expander_torso_size_x", 2.2f);
		settings::rage::hitbox_expander::torso_size_y = r.value("hitbox_expander_torso_size_y", 2.2f);
		settings::rage::hitbox_expander::torso_size_z = r.value("hitbox_expander_torso_size_z", 2.2f);
		settings::rage::hitbox_expander::limb_size_x = r.value("hitbox_expander_limb_size_x", 1.8f);
		settings::rage::hitbox_expander::limb_size_y = r.value("hitbox_expander_limb_size_y", 1.8f);
		settings::rage::hitbox_expander::limb_size_z = r.value("hitbox_expander_limb_size_z", 1.0f);
		settings::rage::spin360::enabled = r.value("spin360_enabled", false);
		settings::rage::spin360::speed = r.value("spin360_speed", 5.f);
		settings::rage::spin360::keybind = r.value("spin360_keybind", 0);
		settings::rage::spin360::activation_mode = r.value("spin360_activation_mode", 1);
	}

	// -- desync --
	if (j.contains("desync")) {
		auto& d = j["desync"];
		settings::desync::enabled = d.value("enabled", false);
		settings::desync::keybind = d.value("keybind", 0);
		settings::desync::keybind_mode = d.value("keybind_mode", 1);
		settings::desync::visualizer::enabled = d.value("visualizer_enabled", false);
		from_json_array(d.value("visualizer_color", json::array()), settings::desync::visualizer::color);
		settings::desync::visualizer::thickness = d.value("visualizer_thickness", 2.f);
		settings::desync::visualizer::pulse = d.value("visualizer_pulse", true);
	}

	// -- magicbullet --
	if (j.contains("magicbullet")) {
		auto& m = j["magicbullet"];
		settings::magicbullet::enabled = m.value("enabled", false);
		settings::magicbullet::keybind = m.value("keybind", 0);
		settings::magicbullet::activation_mode = m.value("activation_mode", 1);
		settings::magicbullet::target_source = m.value("target_source", 2);
		settings::magicbullet::offset_distance = m.value("offset_distance", 5.f);
		settings::magicbullet::hold_ms = m.value("hold_ms", 50);
		settings::magicbullet::tp_iterations = m.value("tp_iterations", 8000);
	}

	// -- infinite_ammo --
	if (j.contains("infinite_ammo")) {
		auto& ia = j["infinite_ammo"];
		settings::infinite_ammo::enabled = ia.value("enabled", false);
		settings::infinite_ammo::universal_mode = ia.value("universal_mode", true);
		settings::infinite_ammo::ammo_value = ia.value("ammo_value", 30);
		settings::infinite_ammo::preserve_mag = ia.value("preserve_mag", true);
		settings::infinite_ammo::no_reload = ia.value("no_reload", false);
		settings::infinite_ammo::detection_method = ia.value("detection_method", 0);
	}

	// -- lua_executor --
	if (j.contains("lua_executor")) {
		auto& le = j["lua_executor"];
		settings::lua_executor::enabled = le.value("enabled", false);
		settings::lua_executor::auto_inject = le.value("auto_inject", false);
		settings::lua_executor::execution_mode = le.value("execution_mode", 0);
		settings::lua_executor::bypass_detection = le.value("bypass_detection", true);
		settings::lua_executor::inject_delay_ms = le.value("inject_delay_ms", 2000);
		settings::lua_executor::clear_on_execute = le.value("clear_on_execute", false);
	}

	// -- visuals --
	if (j.contains("visuals")) {
		auto& v = j["visuals"];
		settings::visuals::enable_enemies = v.value("enable_enemies", false);
		settings::visuals::enable_client = v.value("enable_client", false);
		settings::visuals::box = v.value("box", false);
		settings::visuals::box_type = v.value("box_type", 0);
		from_json_array(v.value("box_color", json::array()), settings::visuals::box_color);
		settings::visuals::box_fill = v.value("box_fill", false);
		from_json_array(v.value("box_fill_color", json::array()), settings::visuals::box_fill_color);
		settings::visuals::name = v.value("name", false);
		settings::visuals::name_type = v.value("name_type", 0);
		settings::visuals::name_display_type = v.value("name_display_type", 0);
		from_json_array(v.value("name_color", json::array()), settings::visuals::name_color);
		from_json_array(v.value("name_color_blend_start", json::array()), settings::visuals::name_color_blend_start);
		from_json_array(v.value("name_color_blend_end", json::array()), settings::visuals::name_color_blend_end);
		settings::visuals::blend = v.value("blend", false);
		settings::visuals::avatar = v.value("avatar", false);
		settings::visuals::healthbar = v.value("healthbar", false);
		from_json_array(v.value("healthbar_color", json::array()), settings::visuals::healthbar_color);
		settings::visuals::health_based_healthbar = v.value("health_based_healthbar", false);
		settings::visuals::gradient_healthbar = v.value("gradient_healthbar", false);
		from_json_array(v.value("gradient_healthbar_color_start", json::array()), settings::visuals::gradient_healthbar_color_start);
		from_json_array(v.value("gradient_healthbar_color_end", json::array()), settings::visuals::gradient_healthbar_color_end);
		settings::visuals::health_percent = v.value("health_percent", false);
		from_json_array(v.value("health_percent_color", json::array()), settings::visuals::health_percent_color);
		settings::visuals::armorbar = v.value("armorbar", false);
		from_json_array(v.value("armorbar_color", json::array()), settings::visuals::armorbar_color);
		settings::visuals::distance = v.value("distance", false);
		settings::visuals::distance_measurement = v.value("distance_measurement", 0);
		from_json_array(v.value("distance_color", json::array()), settings::visuals::distance_color);
		settings::visuals::tool = v.value("tool", false);
		from_json_array(v.value("tool_color", json::array()), settings::visuals::tool_color);
		settings::visuals::esp_font = v.value("esp_font", 0);
		settings::visuals::local_player = v.value("local_player", false);
		settings::visuals::chams = v.value("chams", false);
		settings::visuals::chams_type = v.value("chams_type", 1);
		from_json_array(v.value("chams_fill_color", json::array()), settings::visuals::chams_fill_color);
		from_json_array(v.value("chams_outline_color", json::array()), settings::visuals::chams_outline_color);
		settings::visuals::chams_fill_enabled = v.value("chams_fill_enabled", true);
		settings::visuals::chams_outline_enabled = v.value("chams_outline_enabled", true);
		settings::visuals::target_warning_icon = v.value("target_warning_icon", false);
		settings::visuals::target_warning_icon_size = v.value("target_warning_icon_size", 24.f);
		settings::visuals::flags = v.value("flags", false);
		from_json_array(v.value("flags_state_colour", json::array()), settings::visuals::flags_state_colour);
		settings::visuals::client_box = v.value("client_box", false);
		from_json_array(v.value("client_box_color", json::array()), settings::visuals::client_box_color);
		settings::visuals::client_box_fill = v.value("client_box_fill", false);
		from_json_array(v.value("client_box_fill_color", json::array()), settings::visuals::client_box_fill_color);
		settings::visuals::client_name = v.value("client_name", false);
		from_json_array(v.value("client_name_color", json::array()), settings::visuals::client_name_color);
		settings::visuals::client_avatar = v.value("client_avatar", false);
		settings::visuals::client_healthbar = v.value("client_healthbar", false);
		from_json_array(v.value("client_healthbar_color", json::array()), settings::visuals::client_healthbar_color);
		settings::visuals::client_health_percent = v.value("client_health_percent", false);
		from_json_array(v.value("client_health_percent_color", json::array()), settings::visuals::client_health_percent_color);
		settings::visuals::client_armorbar = v.value("client_armorbar", false);
		from_json_array(v.value("client_armorbar_color", json::array()), settings::visuals::client_armorbar_color);
		settings::visuals::client_distance = v.value("client_distance", false);
		from_json_array(v.value("client_distance_color", json::array()), settings::visuals::client_distance_color);
		settings::visuals::client_tool = v.value("client_tool", false);
		from_json_array(v.value("client_tool_color", json::array()), settings::visuals::client_tool_color);
		settings::visuals::client_chams = v.value("client_chams", false);
		from_json_array(v.value("client_chams_fill_color", json::array()), settings::visuals::client_chams_fill_color);
		from_json_array(v.value("client_chams_outline_color", json::array()), settings::visuals::client_chams_outline_color);
		settings::visuals::client_flags = v.value("client_flags", false);
		from_json_array(v.value("client_flags_state_colour", json::array()), settings::visuals::client_flags_state_colour);
		settings::visuals::client_headless = v.value("client_headless", false);
		settings::visuals::client_korblox = v.value("client_korblox", false);
		settings::visuals::debug_wallcheck = v.value("debug_wallcheck", false);
		settings::visuals::view_hitbox = v.value("view_hitbox", false);
		from_json_array(v.value("view_hitbox_color", json::array()), settings::visuals::view_hitbox_color);
		settings::visuals::fade_in_speed = v.value("fade_in_speed", 5.f);
		settings::visuals::fade_out_speed = v.value("fade_out_speed", 5.f);
		settings::visuals::knock_check = v.value("knock_check", false);
		settings::visuals::teamcheck = v.value("teamcheck", false);
		settings::visuals::use_team_color = v.value("use_team_color", false);
		settings::visuals::ignore_whitelisted = v.value("ignore_whitelisted", false);
		settings::visuals::max_distance_enabled = v.value("max_distance_enabled", false);
		settings::visuals::max_distance = v.value("max_distance", 500.f);
		settings::visuals::esp_keybind = v.value("esp_keybind", 0);
		settings::visuals::esp_keybind_mode = v.value("esp_keybind_mode", 1);
		settings::visuals::mm2_esp = v.value("mm2_esp", false);
		settings::visuals::hit_tracers_enabled = v.value("hit_tracers_enabled", false);
		settings::visuals::hit_tracers_method = v.value("hit_tracers_method", 0);
		from_json_array(v.value("hit_tracers_color", json::array()), settings::visuals::hit_tracers_color);
		settings::visuals::hit_tracers_duration = v.value("hit_tracers_duration", 1.f);
		settings::visuals::skeleton = v.value("skeleton", false);
		from_json_array(v.value("skeleton_color", json::array()), settings::visuals::skeleton_color);
		settings::visuals::custom_preview_show_esp = v.value("custom_preview_show_esp", true);
		settings::visuals::custom_preview_rotate = v.value("custom_preview_rotate", true);
		settings::visuals::custom_preview_rotation_speed = v.value("custom_preview_rotation_speed", 1.f);
		settings::visuals::character_preview = v.value("character_preview", true);
		settings::visuals::preview_size = v.value("preview_size", 200.f);
	}

	// -- movement --
	if (j.contains("movement")) {
		auto& m = j["movement"];
		settings::movement::speedhack::enabled = m.value("speedhack_enabled", false);
		settings::movement::speedhack::mode = m.value("speedhack_mode", 0);
		settings::movement::speedhack::speed = m.value("speedhack_speed", 50.f);
		settings::movement::speedhack::keybind = m.value("speedhack_keybind", 0);
		settings::movement::speedhack::activation_mode = m.value("speedhack_activation_mode", 1);
		settings::movement::jumphack::enabled = m.value("jumphack_enabled", false);
		settings::movement::jumphack::value = m.value("jumphack_value", 50.f);
		settings::movement::jumphack::keybind = m.value("jumphack_keybind", 0);
		settings::movement::jumphack::activation_mode = m.value("jumphack_activation_mode", 1);
		settings::movement::nojumpcooldown::enabled = m.value("nojumpcooldown_enabled", false);
		settings::movement::jump_power::enabled = m.value("jump_power_enabled", false);
		settings::movement::jump_power::value = m.value("jump_power_value", 109.8f);
		settings::movement::jump_power::jump_cd = m.value("jump_power_jump_cd", 1.2f);
		settings::movement::jump_power::keybind = m.value("jump_power_keybind", 0);
		settings::movement::jump_power::activation_mode = m.value("jump_power_activation_mode", 1);
		settings::movement::flyhack::enabled = m.value("flyhack_enabled", false);
		settings::movement::flyhack::mode = m.value("flyhack_mode", 0);
		settings::movement::flyhack::speed = m.value("flyhack_speed", 50.f);
		settings::movement::flyhack::keybind = m.value("flyhack_keybind", 0);
		settings::movement::flyhack::activation_mode = m.value("flyhack_activation_mode", 1);
		settings::movement::tickrate::enabled = m.value("tickrate_enabled", false);
		settings::movement::tickrate::value = m.value("tickrate_value", 240.f);
		settings::movement::orbit::enabled = m.value("orbit_enabled", false);
		settings::movement::orbit::orbit_type = m.value("orbit_type", 0);
		settings::movement::orbit::speed = m.value("orbit_speed", 30.f);
		settings::movement::orbit::radius = m.value("orbit_radius", 10.f);
		settings::movement::orbit::height_offset = m.value("orbit_height_offset", 10.f);
		settings::movement::orbit::spectate_target = m.value("orbit_spectate_target", false);
		settings::movement::orbit::randomize = m.value("orbit_randomize", false);
		settings::movement::orbit::randomize_x = m.value("orbit_randomize_x", 5.f);
		settings::movement::orbit::randomize_y = m.value("orbit_randomize_y", 5.f);
		settings::movement::gravity::enabled = m.value("gravity_enabled", false);
		settings::movement::gravity::value = m.value("gravity_value", 196.2f);
	}

	// -- exploits --
	if (j.contains("exploits")) {
		auto& e = j["exploits"];
		settings::exploits::antiafk::enabled = e.value("antiafk_enabled", false);
		settings::exploits::freezeplayer::enabled = e.value("freezeplayer_enabled", false);
		settings::exploits::freezeplayer::keybind = e.value("freezeplayer_keybind", 0);
		settings::exploits::freezeplayer::activation_mode = e.value("freezeplayer_activation_mode", 1);
	}

	// -- cilent fpscaps --
	if (j.contains("cilent")) {
		auto& c = j["cilent"];
		settings::cilent::fpscaps::enabled = c.value("fpscaps_enabled", false);
	}

	// -- extras --
	if (j.contains("extras")) {
		auto& e = j["extras"];
		settings::extras::crosshair = e.value("crosshair", false);
		settings::extras::crosshair_size = e.value("crosshair_size", 8.f);
		settings::extras::crosshair_gap = e.value("crosshair_gap", 4.f);
		settings::extras::crosshair_thickness = e.value("crosshair_thickness", 2.f);
		from_json_array(e.value("crosshair_color", json::array()), settings::extras::crosshair_color);
		settings::extras::bhop = e.value("bhop", false);
		settings::extras::esp_tracers = e.value("esp_tracers", false);
		from_json_array(e.value("esp_tracers_color", json::array()), settings::extras::esp_tracers_color);
		settings::extras::notifications = e.value("notifications", true);
		settings::extras::keybind_list = e.value("keybind_list", true);
		settings::extras::thirdperson = e.value("thirdperson", false);
		settings::extras::custom_fov = e.value("custom_fov", 70.f);
		settings::extras::custom_fov_enabled = e.value("custom_fov_enabled", false);
		settings::extras::instant_respawn = e.value("instant_respawn", false);
		settings::extras::ragetp = e.value("ragetp", false);
		settings::extras::ragetp_key = e.value("ragetp_key", 0);
		settings::extras::ragetp_range = e.value("ragetp_range", 200.f);
		settings::extras::ragetp_autoshoot = e.value("ragetp_autoshoot", false);
		settings::extras::ragetp_magicbullet = e.value("ragetp_magicbullet", true);
		settings::extras::autofarm = e.value("autofarm", false);
		settings::extras::autofarm_range = e.value("autofarm_range", 30.f);
		settings::extras::anti_lock = e.value("anti_lock", false);
		settings::extras::no_register = e.value("no_register", false);
		settings::extras::macro_glitch = e.value("macro_glitch", false);
		settings::extras::macro_glitch_key = e.value("macro_glitch_key", 0);
		settings::extras::macro_normal_speed = e.value("macro_normal_speed", 16.f);
		settings::extras::macro_boost_speed = e.value("macro_boost_speed", 120.f);
		settings::extras::macro_interval = e.value("macro_interval", 0.05f);
	}

	// -- ui --
	if (j.contains("ui")) {
		auto& u = j["ui"];
		settings::ui::watermark = u.value("watermark", true);
		settings::ui::keybinds = u.value("keybinds", true);
		settings::ui::accent_color = u.value("accent_color", 0);
		from_json_array(u.value("custom_accent_color", json::array()), settings::ui::custom_accent_color);
		settings::ui::menu_opacity = u.value("menu_opacity", 1.f);
		settings::ui::rounded_corners = u.value("rounded_corners", true);
		settings::ui::compact_mode = u.value("compact_mode", false);
		settings::ui::show_icons = u.value("show_icons", true);
		settings::ui::anim_transitions = u.value("anim_transitions", true);
		settings::ui::animation_speed = u.value("animation_speed", 1.f);
	}

	// -- watermark --
	if (j.contains("watermark")) {
		auto& w = j["watermark"];
		settings::watermark::show_cheat_name = w.value("show_cheat_name", true);
		settings::watermark::show_display_name = w.value("show_display_name", false);
		settings::watermark::show_username = w.value("show_username", false);
		settings::watermark::show_fps = w.value("show_fps", true);
		settings::watermark::show_server_ip = w.value("show_server_ip", false);
		settings::watermark::show_ping = w.value("show_ping", false);
		settings::watermark::separator_type = w.value("separator_type", 0);
		from_json_array(w.value("text_color", json::array()), settings::watermark::text_color);
		settings::watermark::rainbow = w.value("rainbow", false);
		settings::watermark::rainbow_speed = w.value("rainbow_speed", 1.f);
		settings::watermark::pos_x = w.value("pos_x", 20.f);
		settings::watermark::pos_y = w.value("pos_y", 100.f);
	}

	// -- lighting --
	if (j.contains("lighting")) {
		auto& l = j["lighting"];
		settings::lighting::fog::enabled = l.value("fog_enabled", false);
		settings::lighting::fog::fog_start = l.value("fog_start", 0.f);
		settings::lighting::fog::fog_end = l.value("fog_end", 500.f);
		settings::lighting::fog::fog_r = l.value("fog_r", 0.75f);
		settings::lighting::fog::fog_g = l.value("fog_g", 0.75f);
		settings::lighting::fog::fog_b = l.value("fog_b", 0.75f);
		settings::lighting::shadows::disable = l.value("shadows_disable", false);
		settings::lighting::clocktime::enabled = l.value("clocktime_enabled", false);
		settings::lighting::clocktime::clock_time = l.value("clocktime_value", 12.f);
		settings::lighting::skybox::enabled = l.value("skybox_enabled", false);
		settings::lighting::skybox::preset_index = l.value("skybox_preset_index", 0);
		settings::lighting::exposure::enabled = l.value("exposure_enabled", false);
		settings::lighting::exposure::exposure = l.value("exposure_value", 0.f);
	}

	// -- football --
	if (j.contains("football")) {
		auto& f = j["football"];
		settings::football::autodive_enabled = f.value("autodive_enabled", false);
		settings::football::auto_m2 = f.value("auto_m2", false);
		settings::football::auto_m2_from_key = f.value("auto_m2_from_key", false);
		settings::football::show_prediction = f.value("show_prediction", true);
		settings::football::show_zones = f.value("show_zones", true);
		settings::football::mode_a_dive_offset = f.value("mode_a_dive_offset", 0.45f);
		settings::football::mode_a_top_threshold = f.value("mode_a_top_threshold", 4.8f);
		settings::football::mode_b_enabled = f.value("mode_b_enabled", false);
		settings::football::mode_b_dive_offset = f.value("mode_b_dive_offset", 0.60f);
		settings::football::mode_b_top_threshold = f.value("mode_b_top_threshold", 4.8f);
		settings::football::mode_b_duration = f.value("mode_b_duration", 2.f);
		settings::football::mode_b_effect_enabled = f.value("mode_b_effect_enabled", true);
		settings::football::mid_top_threshold = f.value("mid_top_threshold", 4.8f);
		settings::football::mid_iframe_top_threshold = f.value("mid_iframe_top_threshold", 4.8f);
		settings::football::zone_scale_x = f.value("zone_scale_x", 1.5f);
		settings::football::zone_scale_y = f.value("zone_scale_y", 3.0f);
		settings::football::panel_behind_dist = f.value("panel_behind_dist", -0.6f);
		settings::football::panel_height_adj = f.value("panel_height_adj", 0.f);
		settings::football::dive_key_space = f.value("dive_key_space", VK_SPACE);
		settings::football::dive_key_left = f.value("dive_key_left", 0x43);
		settings::football::dive_key_right = f.value("dive_key_right", 0x5A);
		settings::football::dive_key_middle = f.value("dive_key_middle", 0);
		settings::football::dive_key_m2 = f.value("dive_key_m2", 0);
		settings::football::random_dive_enabled = f.value("random_dive_enabled", false);
		settings::football::random_dive_offset_reduction = f.value("random_dive_offset_reduction", 0.50f);
		settings::football::dive_cooldown = f.value("dive_cooldown", 0.75f);
		settings::football::dive_jump_delay = f.value("dive_jump_delay", 0.0f);
		settings::football::autodive_key = f.value("autodive_key", 0);
		settings::football::autodive_key_mode = f.value("autodive_key_mode", 0);
		settings::football::auto_m2_key = f.value("auto_m2_key", 0);
		settings::football::auto_m2_key_mode = f.value("auto_m2_key_mode", 0);
		settings::football::panel_vis_key = f.value("panel_vis_key", 0x4B);
		settings::football::auto_m2_dive_offset = f.value("auto_m2_dive_offset", 0.45f);
		settings::football::auto_m2_mode_b_dive_offset = f.value("auto_m2_mode_b_dive_offset", 0.60f);
		settings::football::auto_m2_cooldown = f.value("auto_m2_cooldown", 0.75f);
		settings::football::m2_jump_delay = f.value("m2_jump_delay", 0.f);
		settings::football::post_guard_enabled = f.value("post_guard_enabled", true);
		settings::football::post_guard_distance = f.value("post_guard_distance", 18.f);
		settings::football::post_guard_offset_bonus = f.value("post_guard_offset_bonus", 0.05f);
		settings::football::gravity_mult = f.value("gravity_mult", 6.f);
		settings::football::ground_y = f.value("ground_y", 0.758f);
		settings::football::bounce_vel_y = f.value("bounce_vel_y", 0.4f);
		settings::football::bounce_friction = f.value("bounce_friction", 0.70f);
		settings::football::rolling_friction = f.value("rolling_friction", 0.995f);
		settings::football::stop_velocity = f.value("stop_velocity", 0.1f);
		settings::football::min_bounce_velocity = f.value("min_bounce_velocity", 1.f);
		settings::football::mid_fraction = f.value("mid_fraction", 0.32f);
		settings::football::mid_side_fraction = f.value("mid_side_fraction", 0.27f);
		settings::football::ball_name = f.value("ball_name", std::string("Ball"));
		settings::football::show_path = f.value("show_path", false);
		settings::football::show_panel = f.value("show_panel", false);

		settings::football::rotdive_enabled = f.value("rotdive_enabled", false);
		settings::football::rotdive_key = f.value("rotdive_key", 0);
		settings::football::rotdive_key_mode = f.value("rotdive_key_mode", 1);
		settings::football::rotdive_leap_key = f.value("rotdive_leap_key", 0x43);
		settings::football::rotdive_delay = f.value("rotdive_delay", 0.0f);
		settings::football::rotdive_hold = f.value("rotdive_hold", 0.05f);
		settings::football::rotdive_cooldown = f.value("rotdive_cooldown", 0.3f);
	}

	// -- playerlist whitelist --
	if (j.contains("playerlist_whitelist")) {
		settings::rage::playerlist::whitelist.clear();
		for (auto& name : j["playerlist_whitelist"])
			settings::rage::playerlist::whitelist.insert(name.get<std::string>());
	}

	// -- custom_entities --
	if (j.contains("custom_entities")) {
		auto& ce = j["custom_entities"];
		settings::custom_entities::current_input = ce.value("current_input", std::string("Workspace.Bots"));
		settings::custom_entities::auto_refresh = ce.value("auto_refresh", false);
		settings::custom_entities::refresh_rate = ce.value("refresh_rate", 0.005f);
	}
}
