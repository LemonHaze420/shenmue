#include "s2/camera_sequence_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/stack_spline.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/motion_yaw_dc.hpp"
#include "s2/field_query_dc.hpp"
#include <math.h>
#include "common/native_sqrt.hpp"
#endif
#include "s2/camera_spline_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/camera_dc.hpp"
#include <cstring>
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/frame_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include <float.h>
#endif
#include "s2/camera_search_dc.hpp"
#include "shenmue.h"
#include "s2/camera_state_query_dc.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/battle_qte_sprites.hpp"
#include <cmath>
#include "s2/character_control_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/hlib.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scenario_dc.hpp"
#include "common/matrix_stack.hpp"
#include <cstdio>
#include "s2/model_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/pause.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/camera_task_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/window.hpp"
#include "s2/fight.hpp"
#include "s2/math.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/conversation_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/conversation_fade_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include "s2/camera_roll_dc.hpp"
#include "s2/author_controller_dc.hpp"
#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
#include "s2/air_dc.hpp"
#include "s2/author_doors_dc.hpp"
#include "s2/author_table_dc.hpp"
#include "s2/author_cleanup_dc.hpp"
#include "s2/author_load_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/player_flags_dc.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "platform/dc/aica.hpp"
#include "s2/author_character_state_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/fog_dc.hpp"
#include "s2/lighting.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/screen_film_bars_dc.hpp"
#include "s2/weather.hpp"
#endif
#include "s2/author_control_state_dc.hpp"
#include "s2/scenario_subtitles_dc.hpp"
#include "s2/scenario_actions_dc.hpp"
#include "s2/scenario_talk_dc.hpp"
#include "s2/scnw_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/weather_wind_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "s2/font.hpp"
#include "s2/generated_surface_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/scene_assets_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/story_state.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#endif
#include "s2/map_region_dc.hpp"
#include "s2/scenario_scnt.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/map_region_data_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/maze_dc.hpp"
#include <cfloat>
#include "s2/ship_spray.hpp"
#include "s2/hiki.hpp"
#include "s2/event_character_state_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/flag_data.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/system_modules_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include <cstdarg>
#endif
#include "s2/scene_data.hpp"

namespace shenmue {
namespace s2 {

namespace {

typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);
typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);
typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);
typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);

} // namespace

static void camCtrlCreateSpline_0(CameraSplineState &spline);
std::int32_t camCtrlGetNextRoll();
static void spliEvaluate2_Wrapper(CameraSplineState &spline);
static std::uint16_t SplineModeFromFlags(std::uint32_t flags, std::uint32_t polar, std::uint32_t translate, std::uint32_t base, std::uint32_t heightOnly,
    std::uint32_t bone, std::uint32_t entity, std::uint32_t rotate, bool worldSpace);
static void BindCameraSplineTrack(CameraSplineTrack &track, std::uint32_t *&cursor);
static void GetPosRotUtil(
    Vector3 &position, Vector3i &rotation, std::uint32_t mode, const void *node, const Vector3 &fallbackPosition, const Vector3i &fallbackRotation);
static void spliEvaluate(Vector3 &value, std::uint32_t mode, const Vector3 &position, const Vector3i &rotation);
std::uint32_t AllocSpline(CameraSplineTrack &track);
void SeekAnimatedCameraSequence(std::uint32_t frame);
Character *ctrlGetScenarioChara(shen_name name);

Character *ctrlGetScenarioChara(shen_name name);
std::int32_t camCtrlGetIdealCameraPosition(CameraRuntime &camera);
std::int32_t camCtrlSetState(std::int32_t state);
std::int32_t camCtrlGetState();
void camCtrlToggleSplineRebuildFlag(std::int32_t count, const void *source);
void camCtrlSetNextPosition(const Vector3 &position);
void camCtrlSetTargetPosition(const Vector3 &position);
Vector3 *camCtrlGetNextCamPos(Vector3 *position);
Vector3 *camCtrlGetTargetPosition(Vector3 *position);
std::int32_t camCtrlSetPerspective(std::int16_t perspective);
std::int32_t camCtrlSetContextMode(std::int32_t mode);
std::int32_t camCtrlGetIdealCameraPosition(CameraRuntime &camera);
std::int32_t camCtrlSetState(std::int32_t state);
std::int32_t camCtrlGetState();
void camCtrlToggleSplineRebuildFlag(std::int32_t count, const void *source);
void camCtrlSetNextPosition(const Vector3 &position);
void camCtrlSetTargetPosition(const Vector3 &position);
Vector3 *camCtrlGetNextCamPos(Vector3 *position);
Vector3 *camCtrlGetTargetPosition(Vector3 *position);
std::int32_t camCtrlSetPerspective(std::int16_t perspective);
std::int32_t camCtrlSetContextMode(std::int32_t mode);
std::int32_t camCtrlGetIdealCameraPosition(CameraRuntime &camera);
void camCtrlToggleSplineRebuildFlag(std::int32_t count, const void *source);
std::int32_t camCtrlSetState(std::int32_t state);
void camCtrlSetNextPosition(const Vector3 &position);
void camCtrlSetTargetPosition(const Vector3 &position);
std::int32_t camCtrlSetPerspective(std::int16_t perspective);
std::int32_t camCtrlSetContextMode(std::int32_t mode);
Vector3 *camCtrlGetNextCamPos(Vector3 *position);
Vector3 *camCtrlGetTargetPosition(Vector3 *position);

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::uint32_t *pakFindEventCam(std::int32_t name);
#endif

CameraSplineState g_cameraSplineStorage;
CameraSplineState *g_currentCameraSplines = &g_cameraSplineStorage;

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t CubicHermiteEvaluate(float *out, float time, std::int32_t count, const float *times, const float *values, const float *coefficients) {
    if (time < times[0] || count < 2) {
        *out = values[0];
        return -1;
    }
    std::int32_t key = 1;
    while (key < count && !(time < times[key]))
        ++key;
    if (key == count) {
        *out = values[count - 1];
        return 0;
    }
    const float next = times[key] - time;
    const float elapsed = time - times[key - 1];
    const float nextSquared = next * next;
    const float elapsedSquared = elapsed * elapsed;
    const float nextCubed = nextSquared * next;
    const float elapsedCubed = elapsedSquared * elapsed;
    const float span = times[key] - times[key - 1];
    const float spanCubed = (span * span) * span;
    *out = (((nextCubed * values[key - 1] + elapsedCubed * values[key]) + (elapsed * nextSquared) * coefficients[key * 2]) +
               (elapsedSquared * next) * coefficients[key * 2 + 1]) /
        spanCubed;
    return key;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t GetSplineMaxDistinctKeyRun(const CameraSplineTrack &track) {
    std::int32_t longest;
    if (track.count <= 1) {
        longest = track.count;
    } else {
        std::int32_t key;
        std::int32_t run;
        longest = 1;
        run = longest;
        for (key = 0; key < track.count - 1; ++key) {
            if (track.times[key] == track.times[key + 1]) {
                if (run > longest)
                    longest = run;
                run = 1;
            } else {
                ++run;
            }
        }
        if (run > longest)
            longest = run;
    }
    return longest;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t GetSplineSegmentBufferSize(const CameraSplineTrack &track) {
    std::int32_t run = GetSplineMaxDistinctKeyRun(track);
    if (run <= 1)
        run = 2;
    return run * 8;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t InitializeAuthorSplineTrack(
    CameraSplineTrack &track, float *coefficients, std::int32_t count, const float *times, const float *values, const float *tangents, std::int32_t) {
    if (!coefficients) {
        coefficients = static_cast<float *>(HLib::AllocateTaskData(count * 8, shen_name(AUTH_SPLI)));
        if (!coefficients)
            return 0;
    }
    track.coefficients = coefficients;
    *coefficients = 0.0f;
    track.count = count;
    track.times = times;
    track.values = values;
    track.tangents = tangents;
    track.firstKey = 0;
    track.segmentCount = AllocSpline(track);
    return 1;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t DoKeyframeCubicHermiteInterpolation(CameraSplineTrack &track, float time, float *out) {
    for (;;) {
        const std::int32_t result = CubicHermiteEvaluate(
            out, time, static_cast<std::int32_t>(track.segmentCount), track.times + track.firstKey, track.values + track.firstKey, track.coefficients);
        if (result)
            return result;
        const std::int32_t next = track.firstKey + static_cast<std::int32_t>(track.segmentCount);
        if (next >= track.count)
            return 0;
        track.firstKey = next;
        track.segmentCount = AllocSpline(track);
    }
}
#endif

// @verified - d
void camCtrlInitSplines() {
    g_currentCameraSplines = &g_cameraSplineStorage;
    g_cameraSplineStorage.state = 0;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void camCtrlBuildScriptCamSpline(std::int32_t name, const void *eye, const void *target) {
    CameraSplineState *spline = g_currentCameraSplines;
    spline->state &= ~6U;
    spline->name = static_cast<std::uint32_t>(name);
    spline->eyeNode = const_cast<void *>(eye);
    spline->targetNode = const_cast<void *>(target);
    spline->record = pakFindEventCam(name);
    camCtrlCreateSpline_0(*spline);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void camCtrlCreateSpline(const std::uint32_t *record, const void *eye, const void *target) {
    CameraSplineState *spline = g_currentCameraSplines;
    spline->name = 0;
    spline->eyeNode = const_cast<void *>(eye);
    spline->targetNode = const_cast<void *>(target);
    spline->record = const_cast<std::uint32_t *>(record);
    camCtrlCreateSpline_0(*spline);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void camCtrlCreateSpline_0(CameraSplineState &spline) {
    Vector3 eye;
    Vector3 target;
    float value;
    std::int32_t index;
    CameraSplineTrack *track;
    std::int32_t count;
    spline.state |= 2U;
    spline.currentFrame = 0;
    spline.currentTime = 0.0f;
    if (spline.state & 8U)
        spline.state = (spline.state | 4U) & ~8U;
    std::uint32_t *cursor = spline.record;
    spline.authorFlags = *cursor++;
    const std::uint32_t flags = spline.authorFlags;
    std::uint16_t mode = 0;
    if (flags & 0x20000U)
        mode |= 0x10U;
    if (flags & 0x200U)
        mode |= 1U;
    if (flags & 0x1000U)
        mode |= 8U;
    if (flags & 0x2000000U)
        mode |= 0x40U;
    if (flags & 0x800U)
        mode |= 4U;
    if (flags & 0x80000U)
        mode |= 0x20U;
    if (flags & 0x400U)
        mode |= 2U;
    if (spline.state & 4U)
        mode |= 0x80U;
    spline.eyeMode = mode;
    mode = 0;
    if (flags & 0x40000U)
        mode |= 0x10U;
    if (flags & 0x2000U)
        mode |= 1U;
    if (flags & 0x10000U)
        mode |= 8U;
    if (flags & 0x4000000U)
        mode |= 0x40U;
    if (flags & 0x8000U)
        mode |= 4U;
    if (flags & 0x100000U)
        mode |= 0x20U;
    if (flags & 0x4000U)
        mode |= 2U;
    if (spline.state & 4U)
        mode |= 0x80U;
    spline.targetMode = mode;
    GetPosRotUtil(spline.eyeBasePosition, spline.eyeBaseRotation, spline.eyeMode, spline.eyeNode, spline.fallbackPosition, spline.fallbackRotation);
    GetPosRotUtil(spline.targetBasePosition, spline.targetBaseRotation, spline.targetMode, spline.targetNode, spline.fallbackPosition, spline.fallbackRotation);
    camCtrlGetNextCamPos(&eye);
    camCtrlGetTargetPosition(&target);
    spliEvaluate(eye, spline.eyeMode, spline.eyeBasePosition, spline.eyeBaseRotation);
    spliEvaluate(target, spline.targetMode, spline.targetBasePosition, spline.targetBaseRotation);
    for (index = 0; index < 3; ++index) {
        spline.eyeTracks[index].coefficients = spline.coefficientStorage[index];
        spline.targetTracks[index].coefficients = spline.coefficientStorage[index + 3];
    }
    spline.rollTrack.coefficients = spline.coefficientStorage[6];
    spline.fovTrack.coefficients = spline.coefficientStorage[7];
    for (index = 0; index < 3; ++index) {
        track = &spline.eyeTracks[index];
        count = static_cast<std::int32_t>(*cursor++);
        track->count = count;
        track->firstKey = 0;
        track->times = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->values = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->tangents = reinterpret_cast<float *>(cursor);
        cursor += count;
        if (flags & 0x80U)
            *track->coefficients = 1.0f;
        else
            *track->coefficients = 0.0f;
    }
    for (index = 0; index < 3; ++index) {
        track = &spline.targetTracks[index];
        count = static_cast<std::int32_t>(*cursor++);
        track->count = count;
        track->firstKey = 0;
        track->times = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->values = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->tangents = reinterpret_cast<float *>(cursor);
        cursor += count;
        if (flags & 0x100U)
            *track->coefficients = 1.0f;
        else
            *track->coefficients = 0.0f;
    }
    if (flags & CAMERA_SPLINE_DRIVE_ROLL) {
        track = &spline.rollTrack;
        count = static_cast<std::int32_t>(*cursor++);
        track->count = count;
        track->firstKey = 0;
        track->times = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->values = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->tangents = reinterpret_cast<float *>(cursor);
        cursor += count;
        *track->coefficients = 0.0f;
    }
    if (flags & CAMERA_SPLINE_DRIVE_FOV) {
        track = &spline.fovTrack;
        count = static_cast<std::int32_t>(*cursor++);
        track->count = count;
        track->firstKey = 0;
        track->times = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->values = reinterpret_cast<float *>(cursor);
        cursor += count;
        track->tangents = reinterpret_cast<float *>(cursor);
        *track->coefficients = 0.0f;
    }
    for (index = 0; index < 3; ++index) {
        track = &spline.eyeTracks[index];
        if ((flags & 0x200000U) || !track->count)
            const_cast<float *>(track->values)[0] = (&eye.x)[index];
        if (track->count > 0)
            track->segmentCount = AllocSpline(*track);
        else
            track->segmentCount = 1;
        CubicHermiteEvaluate(&eye.x + index, 0.0f, static_cast<std::int32_t>(track->segmentCount), track->times + track->firstKey,
            track->values + track->firstKey, track->coefficients);
        track = &spline.targetTracks[index];
        if ((flags & 0x400000U) || !track->count)
            const_cast<float *>(track->values)[0] = target.data()[index];
        if (track->count > 0)
            track->segmentCount = AllocSpline(*track);
        else
            track->segmentCount = 1;
        CubicHermiteEvaluate(&target.x + index, 0.0f, static_cast<std::int32_t>(track->segmentCount), track->times + track->firstKey,
            track->values + track->firstKey, track->coefficients);
    }
    if (flags & CAMERA_SPLINE_DRIVE_ROLL) {
        track = &spline.rollTrack;
        if ((flags & 0x800000U) || !track->count)
            const_cast<float *>(track->values)[0] = static_cast<float>(camCtrlGetNextRoll()) / 182.04444885253906f;
        if (track->count > 0)
            track->segmentCount = AllocSpline(*track);
        else
            track->segmentCount = 1;
        CubicHermiteEvaluate(
            &value, 0.0f, static_cast<std::int32_t>(track->segmentCount), track->times + track->firstKey, track->values + track->firstKey, track->coefficients);
        camCtrlSetNextRoll(static_cast<std::int16_t>(static_cast<std::int32_t>((value * 8192.0f) / 45.0f)));
    }
    if (flags & CAMERA_SPLINE_DRIVE_FOV) {
        track = &spline.fovTrack;
        if ((flags & 0x1000000U) || !track->count)
            const_cast<float *>(track->values)[0] = static_cast<float>(camCtrlGetPerspective()) / 182.04444885253906f;
        if (track->count > 0)
            track->segmentCount = AllocSpline(*track);
        else
            track->segmentCount = 1;
        CubicHermiteEvaluate(
            &value, 0.0f, static_cast<std::int32_t>(track->segmentCount), track->times + track->firstKey, track->values + track->firstKey, track->coefficients);
        camCtrlSetPerspective(static_cast<std::int16_t>(static_cast<std::int32_t>((value * 8192.0f) / 45.0f)));
    }
    spline.eye = eye;
    spline.target = target;
    spliEvaluate2_Wrapper(spline);
    camCtrlSetNextPosition(spline.eye);
    camCtrlSetTargetPosition(spline.target);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void DoCameraSplineInterpolation(CameraSplineState &spline) {
    const std::int32_t frame = spline.currentFrame;
    float time = static_cast<float>(frame);
    time *= CAMERA_SPLINE_FRAME_TIME;
    if (frame < 0x6fffffff)
        spline.currentFrame = frame + 2;
    spline.currentTime = time;
    Vector3 eye, target;
    if (spline.state & 2U) {
        spline.state &= ~2U;
        float value;
        if (spline.authorFlags & CAMERA_SPLINE_DRIVE_ROLL) {
            if (DoKeyframeCubicHermiteInterpolation(spline.rollTrack, time, &value) > 0)
                spline.state |= 2U;
            spline.roll = static_cast<std::uint16_t>((value * 8192.0f) / 45.0f);
            camCtrlSetNextRoll(static_cast<std::int16_t>(spline.roll));
        }
        if (spline.authorFlags & CAMERA_SPLINE_DRIVE_FOV) {
            if (DoKeyframeCubicHermiteInterpolation(spline.fovTrack, time, &value) > 0)
                spline.state |= 2U;
            spline.fov = static_cast<std::uint16_t>((value * 8192.0f) / 45.0f);
            if (value > 0.0f)
                camCtrlSetPerspective(static_cast<std::int16_t>(spline.fov));
        }
        for (std::int32_t axis = 0; axis < 3; ++axis) {
            if (DoKeyframeCubicHermiteInterpolation(spline.eyeTracks[axis], time, &eye.x + axis) > 0)
                spline.state |= 2U;
            if (DoKeyframeCubicHermiteInterpolation(spline.targetTracks[axis], time, &target.x + axis) > 0)
                spline.state |= 2U;
        }
        spline.cachedEye = eye;
        spline.cachedTarget = target;
    } else {
        if (spline.authorFlags & CAMERA_SPLINE_DRIVE_ROLL)
            camCtrlSetNextRoll(static_cast<std::int16_t>(spline.roll));
        if ((spline.authorFlags & CAMERA_SPLINE_DRIVE_FOV) && spline.fov > 0)
            camCtrlSetPerspective(static_cast<std::int16_t>(spline.fov));
        eye = spline.cachedEye;
        target = spline.cachedTarget;
    }
    spline.localEye = eye;
    spline.localTarget = target;
    spline.publishedRoll = static_cast<std::uint16_t>(spline.roll);
    spline.publishedFov = static_cast<std::uint16_t>(spline.fov);
    if ((spline.authorFlags & 0x8000000U) && !(spline.state & 2U)) {
        spline.state |= 2U;
        spline.currentFrame = 0;
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void camCtrlHandleQueuedSpline() {
    Vector3 eye, target, previousEye, intersection;
    CameraSplineState &spline = *g_currentCameraSplines;
    if (spline.state & 8U)
        spline.state = (spline.state | 4U) & ~8U;
    DoCameraSplineInterpolation(spline);
    eye = spline.localEye;
    target = spline.localTarget;
    spline.eye = spline.localEye;
    spline.target = spline.localTarget;
    spliEvaluate2_Wrapper(spline);
    eye = spline.eye;
    target = spline.target;
    if (spline.state & 0x10U) {
        camCtrlGetNextCamPos(&previousEye);
        if (FindNearestRayIntersection(&intersection, &eye, &previousEye)) {
            spline.state &= ~2U;
            return;
        }
    }
    camCtrlSetNextPosition(spline.eye);
    camCtrlSetTargetPosition(spline.target);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void spliEvaluate2(Vector3 &value, std::uint32_t mode, const void *node, const Vector3 &fallbackPosition, const Vector3i &fallbackRotation,
    const Vector3 &basePosition, const Vector3i &baseRotation) {
    if (mode & 0x10U) {
        const float angleValue = ((-value.y - 90.0f) * 8192.0f) / 45.0f;
        Vector2 sineCosine;
        CalculateFixedSinCos(static_cast<std::uint16_t>(static_cast<std::int32_t>(angleValue)), &sineCosine);
        const float radius = value.x;
        value.x = sineCosine.y * radius;
        value.y = value.z;
        value.z = sineCosine.x * radius;
    }
    if (mode & 1U) {
        Vector3 position;
        Vector3i rotation;
        GetPosRotUtil(position, rotation, mode, node, fallbackPosition, fallbackRotation);
        if (mode & 8U) {
            position.x = basePosition.x;
            position.y = basePosition.y;
            position.z = basePosition.z;
            rotation = baseRotation;
        } else if (mode & 0x40U) {
            position.y = basePosition.y;
        }
        if (mode & 0x80U)
            value.x = -value.x;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&position);
        if (mode & 2U) {
            Matrix4x4::RotateAroundX_Wrapper(rotation.x);
            Matrix4x4::RotateAroundY_Wrapper(rotation.y);
            Matrix4x4::RotateAroundZ_Wrapper(rotation.z);
        }
        Matrix4x4::TransformPoint(value, value);
        PopCurrentMatrix(1);
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void spliEvaluate2_Wrapper(CameraSplineState &spline) {
    spline.eyeMode &= 0xff7fU;
    spline.targetMode &= 0xff7fU;
    if (spline.state & 4U) {
        spline.eyeMode |= 0x80U;
        spline.targetMode |= 0x80U;
    }
    spliEvaluate2(spline.eye, spline.eyeMode, spline.eyeNode, spline.fallbackPosition, spline.fallbackRotation, spline.eyeBasePosition, spline.eyeBaseRotation);
    spliEvaluate2(spline.target, spline.targetMode, spline.targetNode, spline.fallbackPosition, spline.fallbackRotation, spline.targetBasePosition,
        spline.targetBaseRotation);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::uint16_t SplineModeFromFlags(std::uint32_t flags, std::uint32_t polar, std::uint32_t translate, std::uint32_t base, std::uint32_t heightOnly,
    std::uint32_t bone, std::uint32_t entity, std::uint32_t rotate, bool worldSpace) {
    std::uint16_t mode = flags & polar ? 0x10U : 0U;
    if (flags & translate)
        mode |= 1U;
    if (flags & base)
        mode |= 8U;
    if (flags & heightOnly)
        mode |= 0x40U;
    if (flags & bone)
        mode |= 4U;
    if (flags & entity)
        mode |= 0x20U;
    if (flags & rotate)
        mode |= 2U;
    if (worldSpace)
        mode |= 0x80U;
    return mode;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void BindCameraSplineTrack(CameraSplineTrack &track, std::uint32_t *&cursor) {
    const std::uint32_t count = *cursor++;
    track.count = static_cast<std::int32_t>(count);
    track.firstKey = 0;
    track.times = reinterpret_cast<float *>(cursor);
    cursor += count;
    track.values = reinterpret_cast<float *>(cursor);
    cursor += count;
    track.tangents = reinterpret_cast<float *>(cursor);
    cursor += count;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void GetPosRotUtil(
    Vector3 &position, Vector3i &rotation, std::uint32_t mode, const void *node, const Vector3 &fallbackPosition, const Vector3i &fallbackRotation) {
    if (mode & 1U) {
        if (!node) {
            position.x = fallbackPosition.x;
            position.y = fallbackPosition.y;
            position.z = fallbackPosition.z;
            if (mode & 2U) {
                rotation.x = -fallbackRotation.x;
                rotation.y = -fallbackRotation.y;
                rotation.z = -fallbackRotation.z;
            }
        } else {
            if (mode & 4U)
                charaGetMotWorldPos(static_cast<Character *>(const_cast<void *>(node)), 5, &position);
            else if (mode & 0x20U)
                GetCharacterWorldPosition(static_cast<Character *>(const_cast<void *>(node)), &position);
            else
                charaGetMotWorldPos(static_cast<Character *>(const_cast<void *>(node)), 0, &position);
            if (mode & 2U) {
                if (HasCharacterMotionWork(static_cast<Character *>(const_cast<void *>(node)))) {
                    rotation.x = 0;
                    rotation.z = 0;
                    if (mode & 4U)
                        rotation.y = mtwkGetBoneYaw(charFindMotionwork(static_cast<Character *>(const_cast<void *>(node))), 5);
                    else if (mode & 0x20U)
                        rotation.y = GetCharacterMotionWorldYaw(static_cast<Character *>(const_cast<void *>(node)));
                    else
                        rotation.y = mtwkGetBoneYaw(charFindMotionwork(static_cast<Character *>(const_cast<void *>(node))), 0);
                } else {
                    GetCharacterWorldRotation(static_cast<Character *>(const_cast<void *>(node)), &rotation);
                }
            }
        }
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
static void spliEvaluate(Vector3 &value, std::uint32_t mode, const Vector3 &position, const Vector3i &rotation) {
    if (mode & 1U) {
        matrix_stack::PushUnitMatrix();
        if (mode & 2U) {
            Matrix4x4::RotateAroundZ_Wrapper(-rotation.z);
            Matrix4x4::RotateAroundY_Wrapper(-rotation.y);
            Matrix4x4::RotateAroundX_Wrapper(-rotation.x);
        }
        Matrix4x4::preTranslate(-position.x, -position.y, -position.z);
        Matrix4x4::TransformPoint(value, value);
        PopCurrentMatrix(1);
    }
    if (mode & 0x10U) {
        const float radius = CalculatePlanarMagnitude(value.x, value.z);
        const float yaw =
            static_cast<float>(static_cast<std::int16_t>(-0x4000 - static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(value.z, value.x)))) / 182.04444885253906f;
        value.z = value.y;
        value.x = radius;
        value.y = yaw;
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t TestScriptCameraSplineCollision(std::int32_t name, const void *eye, const void *target, Character *obstacle) {
    Vector3 obstaclePosition, previousEye, intersection;
    std::int32_t frame;
    std::int32_t axis;
    CameraSplineState &spline = *g_currentCameraSplines;
    std::int32_t continuing = 1, result = 0;
    if (spline.state & 2U)
        return -1;
    camCtrlBuildScriptCamSpline(name, eye, target);
    if (obstacle)
        GetCharacterWorldPosition(obstacle, &obstaclePosition);
    float *eyeValues = spline.eye.data();
    float *targetValues = spline.target.data();
    SeekAnimatedCameraSequence(0);
    for (axis = 0; axis < 3; ++axis) {
        DoKeyframeCubicHermiteInterpolation(spline.eyeTracks[axis], 0.0f, eyeValues + axis);
        DoKeyframeCubicHermiteInterpolation(spline.targetTracks[axis], 0.0f, targetValues + axis);
    }
    spliEvaluate2_Wrapper(spline);
    previousEye = spline.eye;
    for (frame = 15; continuing; frame += 15) {
        continuing = 0;
        SeekAnimatedCameraSequence(static_cast<std::uint32_t>(frame));
        float time;
        time = static_cast<float>(frame) * CAMERA_SPLINE_FRAME_TIME;
        for (axis = 0; axis < 3; ++axis) {
            if (DoKeyframeCubicHermiteInterpolation(spline.eyeTracks[axis], time, eyeValues + axis) > 0)
                continuing = 1;
            if (DoKeyframeCubicHermiteInterpolation(spline.targetTracks[axis], time, targetValues + axis) > 0)
                continuing = 1;
        }
        spliEvaluate2_Wrapper(spline);
        if (FindNearestRayIntersection(&intersection, &spline.eye, &previousEye)) {
            result = 1;
            break;
        }
        if (obstacle && FindNearestRayIntersection(&intersection, &spline.eye, &obstaclePosition)) {
            result = 1;
            break;
        }
        previousEye = spline.eye;
    }
    spline.state &= ~2U;
    return result;
}
#endif

// @verified - d
void SeekCameraSplineTrack(std::int32_t frame, CameraSplineTrack *track) {
    float time = static_cast<float>(frame);
    time *= CAMERA_SPLINE_FRAME_TIME;
    track->firstKey = 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(track->count - 1); ++index) {
        if (track->times[index] > time)
            return;
        if (track->times[index] == track->times[index + 1])
            track->firstKey = index + 1;
    }
}

// @verified - d
std::uint32_t AllocSpline(CameraSplineTrack &track) {
    return static_cast<std::uint32_t>(dc::AllocSPLI(track.coefficients, static_cast<std::int32_t>(track.count - track.firstKey), track.times + track.firstKey,
        track.values + track.firstKey, track.tangents ? track.tangents + track.firstKey : 0));
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// Arc-length position spline.
struct PositionSpline {
    std::int32_t count;
    float *times;
    const float *values[3];
    const float *slopes[3];
    float *coefficients[3];
    float duration;
};

void SPLI_Alloc(PositionSpline *spline) {
    float unusedKeys[200];
    Vector3 previous, current;
    const std::int32_t count = spline->count;
    const float *x = spline->values[0], *y = spline->values[1], *z = spline->values[2];
    float *time = spline->times;
    const float *last = spline->times;
    float firstX = *x++;
    previous.x = firstX;
    float firstY = *y++;
    previous.y = firstY;
    float firstZ = *z++;
    previous.z = firstZ;
    *time++ = 0;
    for (std::int32_t remaining = count; --remaining;) {
        current.x = *x++;
        current.y = *y++;
        current.z = *z++;
        const float dx = current.x - previous.x, dy = current.y - previous.y, dz = current.z - previous.z;
        previous.x = current.x;
        previous.y = current.y;
        previous.z = current.z;
        *time++ = shenmue::NativeSqrt(dx * dx + dy * dy + dz * dz) + *last++;
    }
    for (std::int32_t axis = 0; axis < 3; ++axis)
        dc::AllocSPLI(spline->coefficients[axis], count, spline->times, spline->values[axis], spline->slopes[axis]);
}

std::int32_t HermiteSpline_GetInterpolated3D(const PositionSpline *spline, float time, Vector3 *output) {
    const std::int32_t count = spline->count;
    const float *const times = spline->times;
    const std::int32_t result = CubicHermiteEvaluate(&output->x, time, count, times, spline->values[0], spline->coefficients[0]);
    if (result <= 0)
        return result;
    CubicHermiteEvaluate(&output->y, time, count, times, spline->values[1], spline->coefficients[1]);
    CubicHermiteEvaluate(&output->z, time, count, times, spline->values[2], spline->coefficients[2]);
    return result;
}
#endif

// @verified - d
std::int32_t camCtrlCheckIsEventCam() {
    return (g_currentCameraSplines->state & 2U) && camCtrlGetState() == 6;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t *camCtrlFindEventCamera(std::int32_t index) {
    return pakFindEventCam(index);
}
#endif
#endif

// @verified - d
void SeekAnimatedCameraSequence(std::uint32_t frame) {
    const std::int32_t state = camCtrlGetState();
    CameraSplineState *const spline = g_currentCameraSplines;
    if (state != 6 && state != 8)
        return;
    spline->currentFrame = (static_cast<std::int32_t>(frame) / 2) * 2;
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        SeekCameraSplineTrack(spline->currentFrame, &spline->eyeTracks[axis]);
        SeekCameraSplineTrack(spline->currentFrame, &spline->targetTracks[axis]);
        spline->eyeTracks[axis].segmentCount = AllocSpline(spline->eyeTracks[axis]);
        spline->targetTracks[axis].segmentCount = AllocSpline(spline->targetTracks[axis]);
    }
    if (spline->authorFlags & CAMERA_SPLINE_DRIVE_ROLL) {
        SeekCameraSplineTrack(spline->currentFrame, &spline->rollTrack);
        spline->rollTrack.segmentCount = AllocSpline(spline->rollTrack);
    }
    if (spline->authorFlags & CAMERA_SPLINE_DRIVE_FOV) {
        SeekCameraSplineTrack(spline->currentFrame, &spline->fovTrack);
        spline->fovTrack.segmentCount = AllocSpline(spline->fovTrack);
    }
}

// @verified - d
std::int32_t spliGetCurrentFrmCnt() {
    return g_currentCameraSplines->currentFrame;
}

// @verified - d
void camCtrlSetSplineShouldRebuildRotTo(std::int32_t enabled) {
    if (enabled)
        g_currentCameraSplines->state |= 12U;
    else
        g_currentCameraSplines->state &= ~12U;
}

// @verified - d
std::int32_t EVS_LoopCamera(std::int32_t cameraId, shen_name eye, shen_name target) {
    struct CameraRequest {
        std::int32_t cameraId;
        Character *eye, *target;
    } request;
    request.cameraId = cameraId;
    request.eye = 0;
    request.target = 0;
    if (eye)
        request.eye = ctrlGetScenarioChara(eye);
    if (target)
        request.target = ctrlGetScenarioChara(target);
    camCtrlToggleSplineRebuildFlag(3, &request);
    return camCtrlSetState(6);
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void camCtrlSetSplineCheckEyeCollision(std::int32_t enabled) {
    if (enabled)
        g_currentCameraSplines->state |= 0x10U;
    else
        g_currentCameraSplines->state &= ~0x10U;
}

std::uint32_t camCtrlIsSplineCheckEyeCollisionSet() {
    return (g_currentCameraSplines->state >> 4) & 1U;
}

void camCtrlSetSplineFallback(const Vector3 *position, const Vector3i *rotation) {
    g_currentCameraSplines->fallbackPosition.x = position->x;
    g_currentCameraSplines->fallbackPosition.y = position->y;
    g_currentCameraSplines->fallbackPosition.z = position->z;
    g_currentCameraSplines->fallbackRotation.x = rotation->x;
    g_currentCameraSplines->fallbackRotation.y = rotation->y;
    g_currentCameraSplines->fallbackRotation.z = rotation->z;
}

void camCtrlGetSplineFallback(Vector3 *position, Vector3i *rotation) {
    position->x = g_currentCameraSplines->fallbackPosition.x;
    position->y = g_currentCameraSplines->fallbackPosition.y;
    position->z = g_currentCameraSplines->fallbackPosition.z;
    rotation->x = g_currentCameraSplines->fallbackRotation.x;
    rotation->y = g_currentCameraSplines->fallbackRotation.y;
    rotation->z = g_currentCameraSplines->fallbackRotation.z;
}
#endif

// @verified - d
std::int32_t camCtrlHandleContextSwitch(std::int32_t mode) {
    return camCtrlSetContextMode(mode);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t camCtrlIsLookAroundActive() {
    const CameraRuntime *const camera = g_cameraControl;
    if (camera->state == 0 && camera->collisionPhase == 1)
        return 1;
    return 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int16_t camCtrlGetBasePitchBias() {
    return g_cameraControl->basePitchBias;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlSetDefaultOrbitDistance(float distance) {
    g_cameraControl->defaultOrbitDistance = distance;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t camCtrlIsFlag21Set() {
    return (static_cast<std::int32_t>(g_cameraControl->flags) >> 21) & 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t camCtrlComputeOrbitCameraFromTarget(const Vector3 *target, std::int16_t yaw, Vector3 *position, Vector3 *lookAt) {
    CameraRuntime camera = *g_cameraControl;
    camera.targetPosition.x = target->x;
    camera.targetPosition.y = target->y + camera.targetHeightDistance;
    camera.targetPosition.z = target->z;
    camera.nextRotation.x = camera.basePitchBias;
    camera.nextRotation.y = yaw;
    camera.orbitDistance = camera.defaultOrbitDistance;
    CameraRuntime *const work = &camera;
    if (work->nextRotation.x < 0) {
        const float height = 2.65f - (work->targetPosition.y - target->y);
        float limit = height / dc::sdk::SinFixed(-work->nextRotation.x);
        if (limit < 0.2f)
            limit = 0.2f;
        if (limit < work->orbitDistance)
            work->orbitDistance = limit;
    }
    if (work->contextMode == 10)
        work->orbitDistance = 0.01f;
    work->targetOrbitDistance = work->orbitDistance;
    camCtrlGetIdealCameraPosition(*work);
    if (work->orbitDistance < 3.1f && work->contextMode != 10) {
        const float weight = (3.1f - work->orbitDistance) / 3.1f * 0.7f;
        const float lift = 0.7f > weight ? weight : 0.7f;
        work->targetPosition.y += lift;
    }
    const float vertical = CalculateFixedSine(-work->nextRotation.x) * work->orbitDistance;
    const float horizontal = CalculateFixedCosine(-work->nextRotation.x) * work->orbitDistance;
    position->x = CalculateFixedSine(work->nextRotation.y) * horizontal + work->targetPosition.x;
    position->y = vertical + work->targetPosition.y;
    position->z = CalculateFixedCosine(work->nextRotation.y) * horizontal + work->targetPosition.z;
    lookAt->x = work->targetPosition.x;
    lookAt->y = work->targetPosition.y;
    lookAt->z = work->targetPosition.z;
    return 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t camCtrlNullQueryA() {
    return 0;
}

std::int32_t camCtrlNullQueryB() {
    return 0;
}

std::int32_t camCtrlNullQueryC() {
    return 0;
}

void camCtrlNullsub() {
}
#endif

} // namespace s2
} // namespace shenmue

#endif
