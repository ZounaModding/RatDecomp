#ifndef _ABC_MESSAGE_GAME_H_
#define _ABC_MESSAGE_GAME_H_

#include "ABC_Message_Engine.h"

// Game Messages
enum {
    msg_game_start = msg_engine_end,
    msg_collision_player_in = 105,             // IT_TIMED_PLATFORM_G::CheckPlayer, Player_G::GetCollisionMessageIn
    msg_collision_player_out = 106,            // Player_G::GetCollisionMessageOut
    msg_end_check_player = 107,                // GangAgent_G::CheckPlayerDist, GuardAgent_G::CheckSight, SwarmGroupTriggerZone_G::CheckPlayerDist
    msg_start_check_respawn = 108,             // IT_TIMED_PLATFORM_G::CheckRespawn
    msg_check_player = 109,                    // IT_TIMED_PLATFORM_G::CheckPlayer
    msg_respawn = 110,                         // IT_BallWalk_G::CheckRespawn
    msg_reset_base_agent = 111,                // BaseAgent_G::CheckReset
    msg_rtc_script_interp_command = 112,       // Creatures_G::CheckRtcScriptMessage, RtcScriptAgent_G::CheckNextCommand
    msg_rtc_script_check_skip_rtc = 113,       // RtcScriptAgent_G::CheckSkipRtc
    msg_rtc_script_check_skip_text = 114,      // RtcScriptAgent_G::CheckSkipText
    msg_menu_activate_game = 115,              // MenuLogicAgent_G::ActivateGame
    msg_rtc_script_check_end_goto = 117,       // RtcScriptAgent_G::CheckEndOfGoto
    msg_note_track_sample = 119,               // Creatures_G::NoteTrackSample
    msg_to_level = 120,                        // LevelAgent_G::BhvToLevel
    msg_change_level = 121,                    // LevelAgent_G::BhvChangeLevel
    msg_to_menu = 122,                         // GameLogicAgent_G::StopPauseGameBhv, LevelAgent_G::BhvToMenu
    msg_start_menu = 125,                      // MenuLogicAgent_G::ReallyStartMenu
    msg_rtc_retrieve_node = 126,               // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_orient_nodes = 127,                // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_hide_node = 128,                   // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_unhide_node = 129,                 // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_control_strip = 130,               // RtcScriptAgent_G::GlobalTrackMessage, black bars
    msg_rtc_set_camera_game = 131,             // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_camera_chase = 132,            // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_camera_travelling = 133,       // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_camera_force = 134,            // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_camera_force_travelling = 135, // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_camera_focus = 136,            // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_camera_dialog = 137,           // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_camera_cut = 138,                  // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_camera_smooth = 139,               // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_play_anim = 140,                   // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_goto = 141,                        // RtcScriptAgent_G::GlobalTrackMessages
    msg_rtc_skip = 142,                        // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_clear_text = 143,                  // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_start_countdown = 144,             // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_dof = 145,                     // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_end_of_rtc = 146,                  // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_disable_skip = 147,                // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_do_fade = 148,                     // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_wait = 149,                        // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_set_text = 150,                    // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_wait_anim = 153,                   // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_play_dialog = 154,                 // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_pos_perso = 155,                   // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_pos_vehicle = 156,                 // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_wait_goto = 157,                   // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_start_animframe = 158,             // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_start_rtc_fx = 159,                // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_change_smooth_anim = 160,          // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_play_sound = 161,                  // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_unhide_all_creatures = 163,        // RtcScriptAgent_G::GlobalTrackMessage
    msg_rtc_unk_sl = 164,                      // RtcScriptAgent_G::GlobalTrackMessage
    msg_enter_teleport = 165,                  // Creatures_G::EnterTeleportZone
    msg_enter_teleport_alternative = 166,      // Creatures_G::EnterTeleportZone
    msg_execute_teleport = 167,                // Creatures_G::ExecuteTeleportZone
    msg_warp_color_check = 169,                // Player_G::CheckWarpColor
    msg_warp_color_check2 = 170,               // Player_G::CheckWarpColor
    msg_warp_color_dont_check = 171,           // Player_G::CheckWarpColor
    msg_block_fader = 174,                     // LogicAgent_G::BlockedFaderBhv
    msg_fx_trace_b_start = 182,                // Creatures_G::FX_WeaponBlur
    msg_fx_trace_b_end = 183,                  // Creatures_G::FX_WeaponBlur
    msg_fx_trace_e_start = 184,                // Creatures_G::FX_WeaponBlur
    msg_fx_trace_e_end = 185,                  // Creatures_G::FX_WeaponBlur
    msg_break = 191,                           // IT_BREAKBASE_G::FireInside
    msg_generate_object = 192,                 // IT_BREAKBASE_G::GenerateObject
    msg_execute_teleport_end = 193,            // Creatures_G::ExecuteTeleportZone
    msg_foot_on_ground_bone = 196,             // Creatures_G::CheckFootOnGround
    msg_footprint_on_ground = 197,             // Creatures_G::CheckFootPrintOnGround
    msg_anim_change = 199,                     // Creatures_G::AnimChange, SASVolumeAgent_G::CheckSwitch
    msg_deactivate_scene_omnis = 201,          // RtcScriptAgent_G::GlobalTrackMessage
    msg_activate_scene_omnis = 202,            // RtcScriptAgent_G::GlobalTrackMessage
    msg_action_in_event_volume = 204,          // EvenementVolAgent_G::BhvCheckActionDansVolume
    msg_explosion_event_volume = 206,          // EvenementVolAgent_G::BhvWaitExplosion
    msg_impactable = 207,                      // Proto ScriptManager_G::NoteTrackInterpMessage
    msg_lock_pos = 208,                        // Proto ScriptManager_G::NoteTrackInterpMessage
    msg_unlock_pos = 209,                      // Proto ScriptManager_G::NoteTrackInterpMessage
    msg_contextual_sound_1 = 210,              // Friends_G::CheckContextualSound
    msg_contextual_sound_2 = 211,              // Friends_G::CheckContextualSound
    msg_sound_clothe = 212,                    // Player_G::CheckSoundClothe
    msg_hit = 213,                             // Proto ScriptManager_G::NoteTrackInterpMessage
    msg_unhit = 214,                           // Proto ScriptManager_G::NoteTrackInterpMessage
    msg_event = 215,                           // Enemies_G::CheckEvent, Player_G::CheckAnimEvent
    msg_start_mission = 228,                   // Creatures_G::CheckStartMission
    msg_check_player_dist_mission = 230,       // GangAgent_G::CheckPlayerDist
    msg_check_respawn_specific = 232,          // IT_Carrying::CheckRespawn
    msg_start_blind = 233,                     // GuardAgent_G::CheckBlind
    msg_finish_blind = 234,                    // GuardAgent_G::CheckBlind
    msg_on_ground = 235,                       // Creatures_G::CheckFootOnGround, IT_Carrying::CheckRespawn
    msg_it_carrying_respawn_teleport = 236     // IT_Carrying::CheckRespawn
};

#endif
