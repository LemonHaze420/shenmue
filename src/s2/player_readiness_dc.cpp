#include "s2/player_readiness_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/screen_film_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/player_model_dc.hpp"
#include "s2/motion_work_dc.hpp"
#endif
#include "shenmue.h"
#include "s2/shadow_dc.hpp"
#include "s2/player_stats_dc.hpp"
#define UI_ZERO
#define UI_ICON(name) name
#include "s2/input_dc.hpp"
#include <sg_pad.h>
#include "s2/cold_archive_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/hlib.hpp"
#include "platform/dc/ninja.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/modifier_volume_dc.hpp"
#endif
#include <cstdio>
#include <cstring>
#include "s2/prop_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/object_draw_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/world_render.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/math.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/shadow_render_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/character_transparency_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "platform/dc/particle_effects.hpp"
#endif
#include "s2/character_components_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/effect_fft.hpp"
#include "common/math.hpp"
#include <cmath>
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/maze_dc.hpp"
#include "s2/motion_rig.hpp"
#endif
#include "s2/map_info.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/object_light_dc.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
void SetMirroredModelDrawMode(std::int32_t enabled);
}
} // namespace dc
} // namespace shenmue
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/hand_dc.hpp"
#include "s2/pause.hpp"
#endif
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/event_character_control_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/map_event_task_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "common/matrix_stack.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/character_table_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/font.hpp"
#include "s2/screen_film.hpp"
#include "s2/title_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/window.hpp"
#include "s2/area_event.hpp"
#include "s2/game_save_area.hpp"
#include "s2/system_modules_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#endif
#include "s2/scenario_character_query_dc.hpp"

namespace shenmue {
namespace s2 {

std::uint8_t GetParentMenu();
std::uint8_t UpdateSelectorMemo();

bool GetShouldRenderIcons(RenderIconSlot slot);
std::int32_t IsItemSelectable();
std::int32_t PlayerBusyCheckNullsub();

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool shenGetIsPlayerInMenu() {
    if (plymIsSearchBlocked() || GetParentMenu() == 0 || UpdateSelectorMemo() || !IsCtrlSpeakIconInactive() || IsSaveAllowed())
        return false;
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const bool filmClear = IsScreenFilmNotShowing(control->filmTask);
    if (filmClear) {
        if (IsScenarioBusy() == 0)
            return true;
        return false;
    }
    return filmClear;
}
#endif

bool shenCheckIsBusy() {
    if (plymIsSearchBlocked() || srchCheckFlags(3) >= 0 || srchCheckFlags(6) != 0 || PlayerBusyCheckNullsub() > 0 || GetParentMenu() == 0 ||
        UpdateSelectorMemo() || !IsCtrlSpeakIconInactive() || IsSaveAllowed() != 0)
        return false;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const bool filmClear = IsScreenFilmNotShowing(control->filmTask);
    if (filmClear) {
        if (IsScenarioBusy() == 0)
            return true;
        return false;
    }
    return filmClear;
#else
    if (IsScreenFilmNotShowing(control->filmTask) && !IsScenarioBusy())
        return true;
    return false;
#endif
}

bool ryoNotBusy() {
    if (plymIsSearchBlocked() || srchCheckFlags(3) >= 0 || PlayerBusyCheckNullsub() > 0 || GetParentMenu() == 0 || UpdateSelectorMemo() ||
        !IsCtrlSpeakIconInactive() || IsSaveAllowed() != 0)
        return false;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const bool filmClear = IsScreenFilmNotShowing(control->filmTask);
    if (filmClear) {
        if (HasScenarioCharacterControl() == 0)
            return true;
        return false;
    }
    return filmClear;
#else
    if (IsScreenFilmNotShowing(control->filmTask) && !HasScenarioCharacterControl())
        return true;
    return false;
#endif
}

// @verified - d
void ProcessDebugInputNullsub() {
}

} // namespace s2
} // namespace shenmue

#endif
