#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/fight.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/hlib.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/fight_stage.hpp"
#include "s2/character_placement_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/character.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/battle_qte.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/battle_qte_sprites.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/camera_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/camera.hpp"
#include "s2/fight_stage.hpp"
#include "s2/ui.hpp"
#include "s2/event_runtime.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/particle_effects.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/texture_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/pak_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/moves_scroll.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/window.hpp"
#include "s2/conversation_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "common/native_sqrt.hpp"
#include <cmath>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/pak.hpp"
#include "s2/save.hpp"

#include <algorithm>
#include <bit>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cstring>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_control_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/math.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/matrix.hpp"
#include <cstdio>
#include "s2/motion_registry_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/pause.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/conversation_fade_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include "s2/model_dc.hpp"
#include "s2/camera_task_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/camera_search_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include <math.h>
#include <float.h>
#include "s2/camera_conversation_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/npc_data_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/scene_assets_dc.hpp"
#include "s2/area_jump_data_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/ui_state_dc.hpp"
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/system_modules_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/party_companion.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/maze_dc.hpp"
#include <cfloat>
#include "s2/scenario_talk_dc.hpp"
#include "s2/scenario_actions_dc.hpp"
#include "s2/scenario_scnt.hpp"
#include "s2/ship_spray.hpp"
#include "s2/hiki.hpp"
#include "s2/event_character_state_dc.hpp"
#include "s2/event_vector_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/author_control_state_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/map_region_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/object_xforms.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/map_region_data_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "s2/flag_data.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cstdint>
#endif


namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocateFBIT();
void FB_CallBack_InitTask_Callback(void *parameter);
std::uint8_t PublishFightFormation(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style,
    std::uint16_t spawnWord0, std::int32_t spawnWord1, float x, float y, float z);
std::int32_t FindBattleStatusEntry(Character *character);
std::int32_t ReadFightCharacterValue(std::uint8_t operation, Character *character);
void fbqtResetStates(std::uint8_t mode);
void EnsureLoadedFightStyle(std::uint8_t style, std::uint8_t slot);
std::uint32_t EV_vfBattleUtil(std::uint8_t operation, shen_name name, std::int32_t value);
FightActorRuntimePrefix *btlGetActorByScenarioName(FightRuntimePrefix *game, shen_name name);
void AllocateFGST();
void AllocateFCMG(std::uint8_t count);
void AllocateFRMG(std::uint8_t count);
void AllocateFGEN(std::uint8_t count);
void FreeFightStyles();
FightRuntimePrefix *vfmaGetTaskParam();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t vfFindCharaIndex(const FightRuntimePrefix *game, const Character *character);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t vfclControlRuntimeFlags(FightCollisionRuntimePrefix *collision, std::uint32_t mask, std::int32_t action);

bool vfCheckSkillFlag(shen_name flag);
void vfInitStageObjects();
void vfFreeStageObjects();
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot);
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor);
void vfResetActorActionState(FightActorRuntimePrefix *actor);
void FightAreaMaskNullsub(std::uint8_t count);
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index);
void mtwkRemoveCallback(MotionWork *motion);
void vfInitStageObjects();
void vfFreeStageObjects();
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot);
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor);
void vfResetActorActionState(FightActorRuntimePrefix *actor);
void FightAreaMaskNullsub(std::uint8_t count);
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index);
void mtwkRemoveCallback(MotionWork *motion);
void vfInitStageObjects();
void vfFreeStageObjects();
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot);
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor);
void vfResetActorActionState(FightActorRuntimePrefix *actor);
void FightAreaMaskNullsub(std::uint8_t count);
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index);
void mtwkRemoveCallback(MotionWork *motion);
void vfInitStageObjects();
void vfFreeStageObjects();
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot);
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor);
void vfResetActorActionState(FightActorRuntimePrefix *actor);
void FightAreaMaskNullsub(std::uint8_t count);
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index);
void mtwkRemoveCallback(MotionWork *motion);
void vfInitStageObjects();
void vfFreeStageObjects();
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot);
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor);
void vfResetActorActionState(FightActorRuntimePrefix *actor);
void FightAreaMaskNullsub(std::uint8_t count);
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index);
void mtwkRemoveCallback(MotionWork *motion);
std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);
void FightAreaMaskNullsub(std::uint8_t count);
void FightAreaMaskNullsub(std::uint8_t count);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t cvsHasValidConverseData(const ConversationBuffer *buffer);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void StopConversationBuffers(ConversationBuffer *buffer);
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot);
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor);
void vfResetActorActionState(FightActorRuntimePrefix *actor);
void mtwkRemoveCallback(MotionWork *motion);
void vfInitStageObjects();
void vfFreeStageObjects();
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index);
std::uint16_t g_fightModeFlags = 0;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint8_t g_fightActive{};
std::uint8_t g_fightUiActive{};
std::uint16_t g_fightModeFlags{};
std::int32_t g_forcePlayerGodmode{};
std::int32_t g_hasColouredText{};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t g_is_vfm_active;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t streetFightProgress = 0;

FGST_t *g_pFightStage = 0;
std::uint8_t g_vfFormationGroupIndex, g_FGEN_count = 0, g_battleQteCommandSelection = 0;
std::int32_t g_hasColouredText = 0;
std::uint16_t g_fightLoadWidth = 0, g_fightLoadHeight = 0;
std::uint8_t g_vfRoundWins, g_vfRoundLosses, g_vfRoundWinLimit, g_vfRoundLossLimit, g_vfStreetFightLevel = 0;
std::int32_t g_vfStrikeLabelSprite, g_vfStrikesLeftLabelSprite;
std::uint8_t blurModelPreviousFlag;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeFightSmokeEffects() {
    const Vector3 zero = {0, 0, 0};
    FBSM_t *const smoke = g_pFightStage->smoke;
    smoke->activeCount = 0;
    for (std::uint8_t index = 0; index < 9; ++index)
        smoke->effects[index] = CreateSmokeEffect(&zero, &zero, 40, 0.0f, 0, 4);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateFightSmokeEffects() {
    if (g_pFightStage->smokeInitTask)
        return;
    FBSM_t *const smoke = g_pFightStage->smoke;
    FightSmokeSlot *slot = smoke->slots;
    for (std::uint8_t index = 0; index < 9; ++index, ++slot) {
        if (slot->effect == -1)
            continue;
        slot->birthRate -= 0.025f;
        if (slot->birthRate < 0)
            slot->birthRate = 0;
        --slot->remainingTicks;
        if (!slot->remainingTicks) {
            slot->effect = -1;
            --smoke->activeCount;
        } else
            SetParticleEffectBirthRate(0, static_cast<std::uint8_t>(slot->effect), slot->birthRate);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeFightSmokeEffects() {
    FBSM_t *const smoke = g_pFightStage->smoke;
    for (std::uint8_t index = 0; index < 9; ++index) {
        SuspendParticleEffect(0, static_cast<std::uint8_t>(smoke->effects[index]));
        FreeParticleEffect(0, static_cast<std::uint8_t>(smoke->effects[index]));
    }
    HLib::ClearTaskDataMark(smoke);
    HLib::FreeTaskData(smoke);
    g_pFightStage->smoke = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetFightLoadSprite(std::int32_t index, const char *text, std::int32_t width, std::int32_t height) {
    g_fightLoadSprites[index].width = width;
    g_fightLoadSprites[index].height = height;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_fightLoadSprites[index].textureIndex = CreateCenteredWindowTextSurface(control->retainedTaskD, text, width, 0xffff, 0xf000, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeFightAreaMaskNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void shenStartFGST(std::uint8_t enemies, std::uint8_t groups, std::uint8_t areaMasks, std::uint8_t generators) {
    AllocateFGST();
    AllocateFCMG(enemies);
    AllocateFRMG(groups);
    FightAreaMaskNullsub(areaMasks);
    AllocateFGEN(generators);
    for (std::uint16_t index = 0; index < 16; ++index)
        g_activeFightData[index].character = 0;
    AllocateFBIT();
    g_hasColouredText = 0;
    vfInitStageObjects();
    if (!g_is_vfm_active) {
        fbLoadGauge();
        SetupLoadSprite();
    }
    g_fightLoadWidth = 64;
    g_fightLoadHeight = 128;
    EV_vfBattleUtil(1, shen_name(0xffffffffU), 0);
    fbqtResetStates(0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AllocateFBIT() {
    g_pFightStage->smokeInitTask = HLib::CreateNewCurrentTaskWithoutParameter(FB_CallBack_InitTask_Callback, 8, 2, TASK_FBIT);
    FBSM_t *const smoke = static_cast<FBSM_t *>(HLib::AllocateTaskData(sizeof(FBSM_t), TASK_FBSM));
    HLib::AssignDynamicAlignmentForTaskData(smoke);
    g_pFightStage->smoke = smoke;
    smoke->activeCount = 0;
    for (std::int32_t index = 0; index < 9; ++index) {
        smoke->effects[index] = 0xffffffffU;
        smoke->slots[index].effect = -1;
        smoke->slots[index].remainingTicks = 0;
        smoke->slots[index].birthRate = 0.0f;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FB_CallBack_InitTask_Callback(void *parameter) {
    KMUINT32 totalFree, largestFree;
    QueryFreeTextureMemory(&totalFree, &largestFree);
    if (largestFree > 0x1200U) {
        InitializeFightSmokeEffects();
        HLib::DestroyCurrentEventTask();
        g_pFightStage->smokeInitTask = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetStreetFightLevel(std::uint8_t level) {
    g_pFightStage->battleFlags |= 0x10000U;
    g_vfStreetFightLevel = level;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CreateFightStrikeLabels() {
    g_pFightStage->battleFlags |= 0x20000U;
    char strikes[] = "\xbb\xc4\xa4\xea";
    char left[] = "\xc8\xaf";
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_vfStrikeLabelSprite = SetupUIElement(control->retainedTaskD, strikes, 64, 65535, 0xf000, 1);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_vfStrikesLeftLabelSprite = SetupUIElement(control->retainedTaskD, left, 64, 65535, 0xf000, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void FreeFightLoadSprites() {
    FreeLoadSprites();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetFightRoundCounts(std::uint8_t first, std::uint8_t second) {
    g_pFightStage->battleFlags |= 0x8000U;
    if (first > 2)
        first = 2;
    g_vfRoundWins = first;
    g_vfRoundWinLimit = first + 1;
    if (g_vfRoundWinLimit > 2)
        g_vfRoundWinLimit = 2;
    if (second > 2)
        second = 2;
    g_vfRoundLosses = second;
    g_vfRoundLossLimit = second + 1;
    if (g_vfRoundLossLimit > 2)
        g_vfRoundLossLimit = 2;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void vfAddRoundResults(std::uint8_t first, std::uint8_t second) {
    g_vfRoundWins += first;
    if (g_vfRoundWins > g_vfRoundWinLimit)
        g_vfRoundWins = g_vfRoundWinLimit;
    g_vfRoundLosses += second;
    if (g_vfRoundLosses > g_vfRoundLossLimit)
        g_vfRoundLosses = g_vfRoundLossLimit;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ResetFightHudGlobals() {
    g_nativePlayerStats.actionState = blurModelPreviousFlag = blurModelFlag = 0;
    accumulatedFightBlur = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t g_vfBattleBestTimes[32];
std::uint8_t g_vfBattleResults[32];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void vfRecordBattleResult(std::uint8_t battle) {
    g_vfBattleResults[battle] = g_nativePlayerStats.actionState;
    if (!*(g_vfBattleBestTimes + battle) || static_cast<std::uint32_t>(accumulatedFightBlur) < g_vfBattleBestTimes[battle])
        *(g_vfBattleBestTimes + battle) = accumulatedFightBlur;
    ApplyMoveProficiencyUsage();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetFightHudGlobals(std::uint8_t value) {
    g_nativePlayerStats.actionState = blurModelPreviousFlag = blurModelFlag = value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t GetFightHudResetFlag() {
    return blurModelPreviousFlag;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t vfMotnNibbleGet(std::uint8_t code) {
    const std::uint32_t index = code & 0x3fU;
    if (index > 15)
        return 0;
    const std::uint8_t value = g_vfMotnNibbles[index];
    if (code & 0x40U)
        return value & 15U;
    if (code & 0x80U)
        return value >> 4;
    return value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t shenInitFightData(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style) {
    return PublishFightFormation(character, blocker, poolMode, team, hp, style, 0, 0, 0.0f, 0.0f, 0.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void PublishFightRespawn(std::uint16_t hp, std::uint8_t style, std::uint16_t spawnWord0, float x, float y, float z, std::int32_t spawnWord1) {
    PublishFightFormation(reinterpret_cast<Character *>(FIGHT_DEAD), 0, 1, 1, hp, style, spawnWord0, spawnWord1, x, y, z);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t PublishFightFormation(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style,
    std::uint16_t spawnWord0, std::int32_t spawnWord1, float x, float y, float z) {
    const std::uint8_t enemyIndex = g_pFightStage->enemySlotCount;
    if (enemyIndex >= g_pFightStage->enemyCount)
        return 0;
    const std::uint8_t groupIndex = static_cast<std::uint8_t>(g_pFightStage->formationCount[0] + g_pFightStage->formationCount[1]);
    if (groupIndex >= g_pFightStage->groupCount)
        return 0;
    FRMG_t &group = g_pFightStage->groups[groupIndex];
    group.character = character;
    group.blocker = blocker;
    group.totalHp = group.hp = hp;
    group.style = style;
    group.poolMode = poolMode;
    group.team = team;
    group.state = 0;
    group.formationFlags = 0;
    group.waveFlags = 0;
    group.selectFlags = 0;
    if (team == 1)
        g_pFightStage->accumulatedHp += hp;
    if (reinterpret_cast<std::uintptr_t>(character) == FIGHT_DEAD) {
        g_vfFormationGroupIndex = groupIndex;
        group.slot = 0;
        group.spawnWords[0] = spawnWord0;
        group.spawnWords[1] = spawnWord1;
        group.spawnPosition.x = x;
        group.spawnPosition.y = y;
        group.spawnPosition.z = z;
    } else {
        bool found = false;
        for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
            if (g_pFightStage->enemySlots[index].character == character) {
                group.slot = &g_pFightStage->enemySlots[index];
                found = true;
                break;
            }
        }
        if (!found) {
            FCMG_t &slot = g_pFightStage->enemySlots[enemyIndex];
            group.slot = &slot;
            slot.character = character;
            slot.group = 0;
            slot.fighter = 0;
            slot.actorIndex = 0;
            slot.state = 0;
            slot.flags = 0;
            slot.retained21 = 0;
            slot.counterGroup = team;
            slot.fadeAlpha = 1.0f;
            slot.savedPositionState = -1;
            slot.positionState = 0;
            slot.fightEntryIndex = enemyIndex;
            slot.quadrant = -1;
            slot.reflectorColour.x = 0.0f;
            slot.reflectorColour.y = 0.0f;
            slot.reflectorColour.z = 0.0f;
            slot.reflectorColour.w = 0.0f;
            g_activeFightData[enemyIndex].character = character;
            g_activeFightData[enemyIndex].status = 0;
            g_activeFightData[enemyIndex].initialHp = 100;
            g_activeFightData[enemyIndex].hp = hp;
            ++g_pFightStage->enemySlotCount;
        }
    }
    EnsureLoadedFightStyle(style, enemyIndex);
    ++g_pFightStage->formationCount[team];
    ++g_pFightStage->secondaryGroupCounters[team];
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void __attribute__((renesas)) shenmue_s2_event_j_vfUpdateFlagsFor(Character *character, std::uint8_t mode, std::uint32_t flag, std::int32_t value) {
    vfUpdateFlagsFor(character, mode, flag, value);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t vfUpdateFlagsFor(Character *character, std::uint8_t mode, std::uint32_t flag, std::int32_t value) {
    const std::uint8_t operation = mode;
    std::uint32_t index = 99;
    std::uint32_t current;
    if (!character && (current = g_vfFormationGroupIndex) != 99)
        index = current;
    else {
        for (std::int8_t candidate = g_pFightStage->groupCount - 1; candidate >= 0; --candidate)
            if (g_pFightStage->groups[candidate].character == character) {
                index = static_cast<std::uint8_t>(candidate);
                break;
            }
        if (index == 99)
            return 0;
    }
    FRMG_t &group = g_pFightStage->groups[index];
    if (operation == 1) {
        group.formationFlags &= ~flag;
        if (flag & 0x30001U) {
            FightActorRuntimePrefix *const actor = group.slot->fighter;
            if (actor)
                actor->formationFlags &= 0xfffcfffeU;
        }
        return 1;
    }
    group.formationFlags |= flag;
    switch (flag) {
    case 8:
    case 0x10U:
    case 0x20U:
    case 0x40U:
    case 0x80U:
    case 0x10000U:
    case 0x100000U:
        break;
    case 1:
        group.range = value;
        break;
    case 0x20000U:
        group.height = value;
        break;
    case 2:
    case 4:
    case 0x80000U:
        group.direction = value;
        break;
    case 0x400U:
        if (static_cast<std::int16_t>(group.animation) != -1)
            group.spawnYaw = value;
        else
            group.animation = value;
        break;
    case 0x40000U:
        group.retained24 = value;
        break;
    case 0x100U:
        group.actionParameters[0] = value;
        break;
    case 0x200U:
        group.actionParameters[1] = value;
        break;
    case 0x8000U:
        group.activationFlags = value;
        break;
    case 0x200000U:
        group.waveFlags |= value;
        break;
    case 0x400000U:
        if (value & 0x80)
            value &= ~0x80;
        group.selectFlags |= static_cast<std::uint8_t>(value);
        break;
    case 0x800000U:
        group.activationState = 2;
        group.activationIndex = value;
        break;
    case 0x1000000U:
        group.activationState = 1;
        group.activationIndex = value;
        group.activationParameter = 2;
        group.spawnWords[0] = 470;
        break;
    case 0x2000000U:
        group.activationState = 3;
        break;
    case 0x20000000U:
        group.activationState = 5;
        break;
    case 0x8000000U:
        group.assignment = value;
        break;
    case 0x10000000U:
        group.secondaryAssignment = value;
        break;
    case 0x80000000U:
        fbqtResetStates(value);
        break;
    }
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void vfCopyPosFromEntity(std::uint8_t mode, Character *character, float x, float z, float parameter) {
    Vector3 position;
    switch (mode) {
    case 0:
        position.x = x;
        position.z = z;
        break;
    case 1:
        GetCharacterWorldPosition(character, &position);
        break;
    }
    position.y = 0.0f;
    const std::uint8_t index = g_pFightStage->spawnPointCount;
    g_pFightStage->spawnPoints[index].x = position.x;
    g_pFightStage->spawnPoints[index].z = position.z;
    g_pFightStage->spawnPoints[index].parameter = parameter;
    ++g_pFightStage->spawnPointCount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetBtlStatus(Character *character) {
    if (!character)
        return -1;
    return FindBattleStatusEntry(character);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t FindBattleStatusEntry(Character *character) {
    for (std::uint8_t index = 0; index < 16; ++index)
        if (g_activeFightData[index].character == character)
            return g_activeFightData[index].status;
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t btlGetCharaFlag(std::uint8_t operation, Character *character, float hp) {
    if (operation != 2)
        return ReadFightCharacterValue(operation, character);
    FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    const std::int8_t index = static_cast<std::int8_t>(vfFindCharaIndex(game, character));
    if (index == -1)
        return 0;
    FightActorRuntimePrefix *const actor = game->actors[index];
    if (!(hp > 0.1f))
        hp = 0.1f;
    if (static_cast<float>(actor->initialHp) < hp)
        hp = static_cast<float>(actor->initialHp);
    actor->hp = hp;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetRetainedBattleState() {
    streetFightProgress = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t ConversationResultNullsub() {
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ConversationNullsubA() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ConversationNullsubB() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t ConversationStateNullsub() {
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetStreetFightProgress() {
    return streetFightProgress;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t SetStreetFightProgress(std::uint32_t value) {
    ConversationStateNullsub();
    if (value > 255U)
        value = 255U;
    streetFightProgress = static_cast<std::uint8_t>(value);
    return value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AddStreetFightProgress(std::int32_t delta) {
    ConversationStateNullsub();
    delta += streetFightProgress;
    if (delta > 255)
        delta = 255;
    if (delta < 0)
        delta = 0;
    streetFightProgress = static_cast<std::uint8_t>(delta);
    return static_cast<std::uint32_t>(delta);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t ReadFightCharacterValue(std::uint8_t operation, Character *character) {
    for (std::uint8_t index = 0; index < 16; ++index) {
        if (g_activeFightData[index].character != character)
            continue;
        if (!operation)
            return g_activeFightData[index].hp;
        if (operation == 1)
            return g_activeFightData[index].initialHp;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetSetFightActorByte(std::uint8_t operation, Character *character, std::int32_t value) {
    const FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    const std::int8_t index = static_cast<std::int8_t>(vfFindCharaIndex(game, character));
    if (index == -1)
        return 0;
    FightActorRuntimePrefix *const actor = game->actors[index];
    if (!operation)
        return actor->retainedMode;
    actor->retainedMode = value;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
float GetSetFightActorFloat(std::uint8_t operation, Character *character, float value) {
    const FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game)
        return -1.0f;
    const std::int8_t index = static_cast<std::int8_t>(vfFindCharaIndex(game, character));
    if (index == -1)
        return -1.0f;
    FightActorRuntimePrefix *const actor = game->actors[index];
    switch (operation) {
    case 0:
        return actor->retainedMotionValues[2];
    case 1:
        actor->retainedMotionValues[2] = value;
        break;
    case 2:
        return actor->retainedMotionValues[0];
    case 4:
        return actor->retainedMotionValues[1];
    case 3:
    case 5:
        actor->retainedMotionValues[0] = value;
        break;
    }
    return 0.0f;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t IsFightActive() {
    if (!g_fightActive)
        return 0;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetFightModeFlags() {
    return g_fightModeFlags;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t SetFightModeFlags(std::uint16_t flags) {
    const std::uint16_t previous = g_fightModeFlags;
    if (flags & 0x80U)
        flags = static_cast<std::uint16_t>((flags & 0xff7fU) | 8U);
    g_fightModeFlags = flags;
    return previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void fightInit(std::int32_t mode) {
    std::uint16_t flags = static_cast<std::uint16_t>(mode);
    flags |= 0x80;
    SetFightModeFlags(flags);
    EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, 1233, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
VCDW_t *GetFightCameraDialogue(FightRuntimePrefix *game) {
    return game->cameraDialogue;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static FightRuntimePrefix *GetFightDialogueRuntime() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (task) {
        FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
        if (game)
            return game;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeFightCameraDialogue(FightRuntimePrefix *game) {
    VCDW_t *const owner = static_cast<VCDW_t *>(HLib::AllocateTaskData(sizeof(VCDW_t), TASK_VCDW));
    HLib::AssignDynamicAlignmentForTaskData(owner);
    game->cameraDialogue = owner;
    owner->records = 0;
    owner->state = 0;
    owner->currentPriority = 0;
    owner->nextId = 1;
    owner->counter = 0;
    owner->retained14 = 0;
    owner->flags = 0;
    owner->selected = -1;
    owner->parameters = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
FightActorRuntimePrefix *FindFightActorByName(FightRuntimePrefix *game, shen_name name) {
    for (std::int32_t index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[index];
        const std::uint32_t *const slot = reinterpret_cast<const std::uint32_t *>(actor->slot);
        if (slot && *slot == name)
            return actor;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t CanQueueBattleDialogue(const VCDW_t *owner, std::int16_t priority) {
    if (!owner->state)
        return 1;
    if (static_cast<std::int32_t>(owner->currentPriority) > priority)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
VCDS_t *FindCameraDialogueTail(FightRuntimePrefix *game, VCDS_t *record) {
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner || !owner->records)
        return 0;
    if (record->next)
        record = FindCameraDialogueTail(game, record->next);
    return record;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
VCDS_t *FindCameraDialogue(FightRuntimePrefix *game, std::uint32_t id) {
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner)
        return 0;
    for (VCDS_t *record = owner->records; record; record = record->next)
        if (record->id == id)
            return record;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
VCDS_t *AllocateBattleDialogueRecord() {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    return record;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeBattleDialogueRecord(VCDS_t *record) {
    HLib::FreeHeapSpace(record, static_cast<std::int32_t>(TASK_VCDS));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AssignFightDialogueCallback(std::uint32_t id, std::int32_t (*callback)(void *)) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (game && GetFightCameraDialogue(game)) {
        VCDS_t *const record = FindCameraDialogue(game, id);
        if (record && id)
            record->callback = callback;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t vfRegisterCameraDialogue(VCDS_t *record) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (!game)
        return 0;
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner)
        return 0;
    record->state = 1;
    record->completions = 0;
    record->occurrences = 0;
    record->idleFrames = 0;
    record->lastCharacter = 0;
    record->callback = 0;
    record->previousDialogue = -1;
    record->callbackArgument = 0;
    if (record->probability != 0.0f)
        record->probability = 0.1f + record->probability;
    if (!owner->records) {
        owner->records = record;
        record->previous = 0;
    } else {
        VCDS_t *const tail = FindCameraDialogueTail(game, owner->records);
        record->previous = tail;
        tail->next = record;
    }
    record->next = 0;
    record->id = owner->nextId;
    owner->nextId = owner->nextId + 1U;
    return record->id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocatePeriodicBattleDialogue(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 1;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->period = period;
    record->repeatEvery = 1;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition15(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 15;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->period = period;
    record->repeatEvery = 1;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition2(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float healthPercent, float probability) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 2;
    record->flags = 0;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->healthPercent = healthPercent;
    record->repeatEvery = 1;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition3(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t health, float probability) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 3;
    record->flags = 0;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->health = health;
    record->repeatEvery = 1;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::int16_t repeatEvery,
    float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 4;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition5(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 5;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition6(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 6;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition7(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 7;
    record->priority = static_cast<std::uint16_t>(priority);
    record->flags = flags;
    record->probability = probability;
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition13(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 13;
    record->priority = static_cast<std::uint16_t>(priority);
    record->flags = flags;
    record->probability = probability;
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition16(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 16;
    record->priority = static_cast<std::uint16_t>(priority);
    record->flags = flags;
    record->probability = probability;
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition17(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 17;
    record->priority = static_cast<std::uint16_t>(priority);
    record->flags = flags;
    record->probability = probability;
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition10(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 10;
    record->priority = static_cast<std::uint16_t>(priority);
    record->flags = flags;
    record->probability = probability;
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition11(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 11;
    record->priority = static_cast<std::uint16_t>(priority);
    record->flags = flags;
    record->probability = probability;
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->previousAnimation = 0;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition12(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 12;
    record->priority = static_cast<std::uint16_t>(priority);
    record->flags = flags;
    record->probability = probability;
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateMotionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t animation,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 8;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->animation = animation;
    record->previousAnimation = 0;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateReactionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::uint32_t reactionMask, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 14;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->reactionMask = reactionMask;
    record->repeatEvery = repeatEvery;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AllocateBattleDialogueCondition9(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float distance, float probability, std::uint16_t flags) {
    VCDS_t *const record = AllocateBattleDialogueRecord();
    record->condition = 9;
    record->flags = flags;
    record->probability = probability;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->distance = distance;
    record->repeatEvery = -1;
    vfRegisterCameraDialogue(record);
    std::uint32_t id;
    std::memcpy(&id, &record->id, sizeof(id));
    return id;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t QueueBattleDialogue(std::int16_t priority, std::int16_t dialogue) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (!game)
        return 0;
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner || (owner->state && !CanQueueBattleDialogue(owner, owner->parameters)))
        return 0;
    owner->selected = dialogue;
    owner->parameters = priority;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetBattleDialogueFlags(std::uint32_t id, std::uint16_t flags) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (game) {
        VCDS_t *const record = FindCameraDialogue(game, id);
        if (record && id)
            record->flags = flags;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeBattleDialogue(std::uint32_t id) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (!game)
        return;
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner)
        return;
    VCDS_t *const record = FindCameraDialogue(game, id);
    if (record && id) {
        if (record->previous)
            record->previous->next = record->next;
        else
            owner->records = record->next;
        if (record->next)
            record->next->previous = record->previous;
        FreeBattleDialogueRecord(record);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RetireCameraDialogueRecords() {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (!game)
        return;
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner)
        return;
    VCDS_t *record = FindCameraDialogueTail(game, owner->records);
    while (record) {
        VCDS_t *const previous = record->previous;
        FreeBattleDialogue(record->id);
        record = previous;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BeginFightDialogueRetirement() {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (game) {
        VCDW_t *const owner = GetFightCameraDialogue(game);
        if (owner)
            owner->flags = 1;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeFightCameraDialogue(FightRuntimePrefix *game) {
    RetireCameraDialogueRecords();
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (owner) {
        HLib::FreeHeapSpace(owner, static_cast<std::int32_t>(TASK_VCDW));
        game->cameraDialogue = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SuspendBattleDialogue(std::uint32_t id) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (game) {
        VCDS_t *const record = FindCameraDialogue(game, id);
        if (record && id)
            record->state = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResumeBattleDialogue(std::uint32_t id) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (game) {
        VCDS_t *const record = FindCameraDialogue(game, id);
        if (record && id) {
            record->previousAnimation = 0;
            record->state = 1;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetBattleDialogueOccurrences(std::uint32_t id) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (game) {
        const VCDS_t *const record = FindCameraDialogue(game, id);
        if (record && id)
            return record->occurrences;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetBattleDialogueCompletions(std::uint32_t id) {
    FightRuntimePrefix *const game = GetFightDialogueRuntime();
    if (game) {
        const VCDS_t *const record = FindCameraDialogue(game, id);
        if (record && id)
            return record->completions;
    }
    return 0xffffffffU;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BeginFightCameraDialogue(FightRuntimePrefix *game, VCDW_t *owner, std::uint32_t id, std::int16_t dialogue, std::int16_t priority) {
    owner->currentPriority = static_cast<std::uint8_t>(priority);
    owner->state = static_cast<std::uint8_t>(id);
    owner->retained14 = 60;
    *reinterpret_cast<std::int16_t *>(reinterpret_cast<std::byte *>(game) + 0x22e) = -1;
    StopConversationBuffers(0);
    vfSetFightDialogue(game, dialogue);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void StartFightCameraDialogue(FightRuntimePrefix *game, std::uint32_t id) {
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner || !id)
        return;
    VCDS_t *record;
    if (owner->state) {
        record = FindCameraDialogue(game, owner->state);
        if (record) {
            record->state = 1;
            if ((record->flags & 1) == 0)
                SuspendBattleDialogue(record->id);
        }
    }
    record = FindCameraDialogue(game, id);
    record->state = 2;
    if (record->callback) {
        const std::int16_t dialogue = static_cast<std::int16_t>(record->callback(&record->callbackArgument));
        if (static_cast<std::uint16_t>(dialogue + 2) < 2) {
            record->state = 1;
            return;
        }
        record->dialogue = dialogue;
    }
    BeginFightCameraDialogue(game, owner, id, record->dialogue, static_cast<std::int16_t>(record->priority));
    record->previousDialogue = record->dialogue;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t IsFightCameraDialogueActive(FightRuntimePrefix *game) {
    if (*reinterpret_cast<std::int16_t *>(reinterpret_cast<std::byte *>(game) + 0x22e) != -1)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void TriggerCameraDialogue(FightRuntimePrefix *game, VCDS_t *record) {
    if (record->dialogue != -2) {
        StartFightCameraDialogue(game, record->id);
    } else if (record->callback) {
        record->callback(&record->callbackArgument);
    }
    if ((record->flags & 1) == 0)
        SuspendBattleDialogue(record->id);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static inline float FightDialogueSqrt(float value) {
    return sqrtf(value);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define FIGHT_DIALOGUE_NAME(actor) (*reinterpret_cast<const shen_name *>((actor)->slot))
#define FIGHT_DIALOGUE_ADMIT()                                                                                                                                 \
    if (CanQueueBattleDialogue(owner, record->priority)) {                                                                                                     \
        if (dc::sdk::RandomFloat() * 100.0f < record->probability)                                                                                             \
            TriggerCameraDialogue(game, record);                                                                                                               \
    }
#define FIGHT_DIALOGUE_REPEAT_ADMIT()                                                                                                                          \
    if (record->repeatEvery == -1 || record->occurrences % record->repeatEvery == 0) {                                                                         \
        FIGHT_DIALOGUE_ADMIT()                                                                                                                                 \
    }
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateFightCameraDialogue(FightRuntimePrefix *game) {
    VCDW_t *const owner = GetFightCameraDialogue(game);
    if (!owner)
        return;
    VCDS_t *record = owner->records;
    if (owner->selected != -1) {
        if (owner->state) {
            record = FindCameraDialogue(game, owner->state);
            if (record) {
                if (record->state == 2)
                    record->state = 1;
                if ((record->flags & 1) == 0)
                    SuspendBattleDialogue(record->id);
            }
        }
        BeginFightCameraDialogue(game, owner, 0xffffffffU, owner->selected, owner->parameters);
        owner->selected = -1;
    }
    if (owner->flags && !--owner->flags) {
        RetireCameraDialogueRecords();
        return;
    }
    for (; record; record = record->next) {
        if (record->state != 1 || record->id == owner->state)
            continue;
        FightActorRuntimePrefix *self, *other;
        switch (record->condition) {
        case 1:
            if (owner->counter && owner->counter % record->period == 0) {
                ++record->occurrences;
                FIGHT_DIALOGUE_ADMIT()
            }
            break;
        case 15: {
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (record->idleFrames && record->idleFrames % record->period == 0) {
                ++record->occurrences;
                FIGHT_DIALOGUE_ADMIT()
            }
            if (!(self->inputFlags & 15))
                ++record->idleFrames;
            else
                record->idleFrames = 0;
            break;
        }
        case 2: {
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (self->hp / self->initialHp * 100.0f < record->healthPercent) {
                ++record->occurrences;
                FIGHT_DIALOGUE_ADMIT()
            }
            break;
        }
        case 3: {
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (self->hp < record->health) {
                ++record->occurrences;
                FIGHT_DIALOGUE_ADMIT()
            }
            break;
        }
        case 4: {
            std::int32_t found = 0;
            for (std::int32_t index = 0; index < 6; ++index) {
                if (game->characters[index]) {
                    other = game->actors[index];
                    if ((other->hitStateFlags & 0x4000) && record->character == FIGHT_DIALOGUE_NAME(other->actor)) {
                        if (!record->otherCharacter) {
                            found = 1;
                            break;
                        }
                        if (record->otherCharacter == FIGHT_DIALOGUE_NAME(other)) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 5: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            for (std::int32_t index = 0; index < 6; ++index) {
                if (game->characters[index]) {
                    other = game->actors[index];
                    if ((other->hitStateFlags & 0x84000) && (other->hitStateFlags & 0x100) && record->character == FIGHT_DIALOGUE_NAME(other->actor)) {
                        if (!record->otherCharacter) {
                            found = 1;
                            break;
                        }
                        if (record->otherCharacter == FIGHT_DIALOGUE_NAME(other)) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 6: {
            other = 0;
            std::int32_t found = 0;
            if (record->otherCharacter) {
                other = FindFightActorByName(game, record->otherCharacter);
                if (!other)
                    continue;
                if (fabsf(other->hp) < 0.00000011920928955078125f && record->otherCharacter == FIGHT_DIALOGUE_NAME(other))
                    found = record->character == FIGHT_DIALOGUE_NAME(other->actor);
            } else {
                for (std::int32_t index = 0; index < 6; ++index) {
                    if (game->characters[index]) {
                        other = game->actors[index];
                        if (fabsf(other->hp) < 0.00000011920928955078125f && record->character == FIGHT_DIALOGUE_NAME(other->actor))
                            found = 1;
                    }
                }
            }
            if (!found)
                break;
            if (record->lastCharacter == FIGHT_DIALOGUE_NAME(other))
                continue;
            ++record->occurrences;
            if (record->repeatEvery == -1 || record->occurrences % record->repeatEvery == 0) {
                if (CanQueueBattleDialogue(owner, record->priority)) {
                    if (dc::sdk::RandomFloat() * 100.0f < record->probability) {
                        record->lastCharacter = FIGHT_DIALOGUE_NAME(other);
                        TriggerCameraDialogue(game, record);
                    }
                }
            }
            break;
        }
        case 7: {
            std::int32_t found = 0;
            for (std::int32_t index = 0; index < 6; ++index) {
                if (game->characters[index]) {
                    other = game->actors[index];
                    if (other->queuedReactionEvent == 30 && record->character == FIGHT_DIALOGUE_NAME(other->actor)) {
                        if (!record->otherCharacter) {
                            found = 1;
                            break;
                        }
                        if (record->otherCharacter == FIGHT_DIALOGUE_NAME(other)) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 13: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (self->activeMotionFlags & 0x200000) {
                if (!record->otherCharacter)
                    found = 1;
                else if (record->otherCharacter == FIGHT_DIALOGUE_NAME(self))
                    found = 1;
            }
            if (!found && !(self->previousHitStateFlags & 0x10000000) && (self->hitStateFlags & 0x10000000)) {
                if (!record->otherCharacter)
                    found = 1;
                else if (self->actor && record->otherCharacter == FIGHT_DIALOGUE_NAME(self->actor)) {
                    other = FindFightActorByName(game, record->otherCharacter);
                    if (!other)
                        continue;
                    if (other->hitStateFlags & 0x100)
                        found = 1;
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 16: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (!(self->previousHitStateFlags & 0x10000000) && (self->hitStateFlags & 0x10000000)) {
                if (!record->otherCharacter)
                    found = 1;
                else if (self->actor && record->otherCharacter == FIGHT_DIALOGUE_NAME(self->actor)) {
                    other = FindFightActorByName(game, record->otherCharacter);
                    if (!other)
                        continue;
                    if (other->hitStateFlags & 0x100)
                        found = 1;
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 17: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (self->hitResolutionFlags & 1) {
                if (!record->otherCharacter)
                    found = 1;
                else if (self->actor && record->otherCharacter == FIGHT_DIALOGUE_NAME(self->actor))
                    found = 1;
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 10: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            for (std::int32_t index = 0; index < 6; ++index) {
                if (game->characters[index]) {
                    other = game->actors[index];
                    FightActorRuntimePrefix *paired;
                    if ((self->hitStateFlags & 0x40000) && (other->hitStateFlags & 0x80000) && !(self->hitStateFlags & 0x100000) &&
                        (paired = other->secondActor) != 0) {
                        if (static_cast<std::uint16_t>(static_cast<std::int32_t>(paired->animationTime)) == paired->animationEndFrame + 1 &&
                            record->character == FIGHT_DIALOGUE_NAME(paired)) {
                            if (!record->otherCharacter) {
                                found = 1;
                                break;
                            }
                            if (record->otherCharacter == FIGHT_DIALOGUE_NAME(other)) {
                                found = 1;
                                break;
                            }
                        }
                    }
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 11: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            other = self->secondActor;
            if (!(self->previousHitStateFlags & 0x100000) && (self->hitStateFlags & 0x100000) && other) {
                if (!record->otherCharacter) {
                    found = 1;
                } else if (record->otherCharacter == FIGHT_DIALOGUE_NAME(self->secondActor)) {
                    other = FindFightActorByName(game, record->otherCharacter);
                    if (!other)
                        continue;
                    found = 1;
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 12: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if ((self->hitStateFlags & 0x80100) == 0x100 && self->currentAnimation != self->previousAnimation) {
                if (!record->otherCharacter)
                    found = 1;
                else if (self->actor && record->otherCharacter == FIGHT_DIALOGUE_NAME(self->actor))
                    found = 1;
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 14: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (vfSelectHitReactionSequence(self) & record->reactionMask) {
                if (!record->otherCharacter)
                    found = 1;
                else if (self->actor && record->otherCharacter == FIGHT_DIALOGUE_NAME(self->actor))
                    found = 1;
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 8: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (record->animation == record->previousAnimation) {
                if (self->currentAnimation != record->animation)
                    record->previousAnimation = 0;
            } else if (self->currentAnimation == record->animation) {
                if (record->otherCharacter) {
                    other = FindFightActorByName(game, record->otherCharacter);
                    if (!other)
                        continue;
                    if (record->otherCharacter == FIGHT_DIALOGUE_NAME(other))
                        found = 1;
                } else {
                    found = 1;
                }
            }
            if (found) {
                ++record->occurrences;
                record->previousAnimation = record->animation;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        case 9: {
            std::int32_t found = 0;
            self = FindFightActorByName(game, record->character);
            if (!self)
                continue;
            if (record->otherCharacter) {
                other = FindFightActorByName(game, record->otherCharacter);
                if (!other)
                    continue;
                const float x = self->contactPosition.x - other->contactPosition.x;
                const float z = self->contactPosition.z - other->contactPosition.z;
                if (shenmue::NativeSqrt(x * x + z * z) > record->distance)
                    found = 1;
            } else {
                found = 1;
                for (std::int32_t index = 0; index < 6; ++index) {
                    if (game->characters[index] && index != self->groupIndex && self->contactDistances[index] < record->distance) {
                        found = 0;
                        break;
                    }
                }
            }
            if (found) {
                ++record->occurrences;
                FIGHT_DIALOGUE_REPEAT_ADMIT()
            }
            break;
        }
        }
        if (record->flags & 2)
            SuspendBattleDialogue(record->id);
    }
    if (owner->retained14) {
        --owner->retained14;
    } else if (!IsFightCameraDialogueActive(game) && !cvsHasValidConverseData(0) && owner->state) {
        record = FindCameraDialogue(game, owner->state);
        if (record) {
            if (record->state == 2)
                record->state = 1;
            ++record->completions;
        }
        owner->currentPriority = 0xff;
        owner->state = 0;
    }
    ++owner->counter;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#undef FIGHT_DIALOGUE_REPEAT_ADMIT
#undef FIGHT_DIALOGUE_ADMIT
#undef FIGHT_DIALOGUE_NAME
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void check_dead_status(std::uint8_t mode) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return;
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
    if (!game || game->state != 3)
        return;
    for (std::int32_t index = 0; index < g_pFightStage->groupCount; ++index) {
        FRMG_t &group = g_pFightStage->groups[index];
        if (!group.character)
            continue;
        group.state = 10;
        if (reinterpret_cast<std::uintptr_t>(group.character) != FIGHT_DEAD)
            g_activeFightData[group.slot->fightEntryIndex].status = 10;
    }
    if (!mode)
        game->state = 6;
    else if (mode == 1)
        game->state = 9;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void fbqtResetStates(std::uint8_t mode) {
    if (!mode)
        g_battleQteCommandSelection = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t vfGetElapsedSeconds() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (task) {
        const FightRuntimePrefix *const work = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
        if (work->elapsedFrames != 0xffffffffU)
            return work->elapsedFrames / 30U;
    }
    return 0xffffffffU;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetFightRuntime(Character *character) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (task) {
        const FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
        if (game) {
            const std::uint32_t index = vfFindCharaIndex(game, character);
            if (index != 99 && game->characters[index])
                return game->actors[index]->runtimeFlags;
        }
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RemoveFightGroundSlotReference(std::uint32_t index);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void vfReleaseEnemySlotForCharacter(Character *character) {
    if (character) {
        HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
        if (task) {
            FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
            if (game) {
                for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
                    FCMG_t &slot = g_pFightStage->enemySlots[index];
                    if (slot.character == character) {
                        if (slot.state == 3) {
                            vfTransitionEnemySlotToInactive(game, &slot);
                            RemoveFightGroundSlotReference(index);
                        }
                        break;
                    }
                }
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetFightOutcomeMode(std::int16_t outcome) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (task) {
        FightRuntimePrefix *const work = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
        if (work)
            work->outcome = outcome;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetFightResult() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (task) {
        const FightRuntimePrefix *const work = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
        if (work)
            return work->outcome;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t vfSetFightControlFlag(std::int32_t enabled) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    FightRuntimePrefix *work;
    if (!task || !(work = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task))))
        return 0;
    const std::uint32_t previous = work->controlFlags;
    if (enabled) {
        work->controlFlags |= 0x400000U;
        work->stateFlags |= 0x200U;
    } else {
        work->controlFlags &= ~0x400000U;
    }
    return previous & 0x400000U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SuspendFight() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    FightRuntimePrefix *game;
    if (!task || !(game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task))))
        return;
    for (std::int32_t index = 0; index < 6; ++index)
        if (game->characters[index] && game->actors[index])
            mtwkRemoveCallback(game->actors[index]->motion);
    for (std::int32_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (slot.character && slot.group && slot.state == 3)
            mtwkRemoveCallback(slot.motion);
    }
    HLib::SetupTimedTask(game->task, -1);
    HLib::SetupTimedTask(game->collisionTask, -1);
    HLib::SetupTimedTask(game->gameInfoTask, -1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetFightFrame(std::int32_t restartActors) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    FightRuntimePrefix *game;
    if (!task || !(game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task))))
        return;
    HLib::ResolveDeferredCallback(game->task);
    HLib::ResolveDeferredCallback(game->collisionTask);
    HLib::ResolveDeferredCallback(game->gameInfoTask);
    for (std::int32_t index = 0; index < 6; ++index)
        if (game->characters[index] && game->actors[index])
            mtwkRegisterGroundTargetCallbacks(game->actors[index]->motion, game->actors[index]);
    for (std::int32_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (slot.character && slot.group && slot.state == 3)
            mtwkRegisterCooldownTargetCallbacks(slot.motion, &slot);
    }
    if (!restartActors)
        return;
    g_pFightStage->battleFlags |= 0x10U;
    for (std::int32_t index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[index];
        MotionWork *const motion = actor->motion;
        vfSynchronizeActorPosition(actor);
        vfResetReactionProgram(actor);
        actor->queuedAction = 0;
        actor->facingYaw = actor->facingAngle;
        actor->contactPosition.x = actor->position.x;
        actor->contactPosition.y = actor->position.y;
        actor->contactPosition.z = actor->position.z;
        actor->facingAngleDelta = 0;
        actor->hitStateFlags = actor->activeMotionFlags = actor->retained350 = 0;
        actor->motionFlags = actor->secondaryMotionFlags = actor->markerMask = 0;
        actor->moveFlags &= 0xfffffbefU;
        if (mtwkSetRegionFlag(motion, 0x46, 1, static_cast<MotionRegion>(0)))
            actor->moveFlags |= 0x40U;
        else
            actor->moveFlags &= ~0x40U;
        const float frame = mtwkGetFrame(motion, static_cast<MotionRegion>(0));
        if (*reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(actor) + 0x42c) !=
            mtwkGetAnimationId(motion, static_cast<MotionRegion>(0)))
            vfSelectDefaultMotion(actor, 0);
        mtwkSetExtraFlag(actor->motion, 1, 2);
        actor->animationTime = frame;
        actor->stateFlags |= 0x80000U;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void EnsureLoadedFightStyle(std::uint8_t style, std::uint8_t slot) {
    if (g_pFightStage->styles[slot].file)
        return;
    const char *const name = fightStyleNames[style];
    void *file = 0;
    PakGlobalData *package = static_cast<PakGlobalData *>(g_pFightStage->packData);
    if (!package)
        package = GetPakLoadTaskDataPointer();
    char directory[32], filename[32];
    std::sprintf(filename, "EN_%s", name);
    if (package)
        file = FileLoader(package, filename, shen_name(0x204e4942U));
    if (!file) {
        const std::int32_t map = static_cast<std::int32_t>(g_currentMap);
        std::sprintf(filename, "EN_%s.BIN", name);
        std::sprintf(directory, "scene/%02d/%c%c%c%c", g_actNumber, static_cast<char>(map), static_cast<char>(static_cast<std::uint32_t>(map) >> 8),
            static_cast<char>((map << 8) >> 24), static_cast<char>(map >> 24));
        file = shenLoadFile(directory, filename, shen_name(SHEN_NAME4('F', 'I', 'G', 'H')));
    }
    g_pFightStage->styles[slot].file = file;
    g_pFightStage->styles[slot].style = style;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void vfAddGeneratorPoint(float x, float y, float z) {
    FGEN_t *const generator = &g_pFightStage->generators[g_FGEN_count];
    generator->enabled = 1;
    generator->other = 0;
    generator->position.x = x;
    generator->position.y = y;
    generator->position.z = z;
    ++g_FGEN_count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void vfSetBattleCenter(float x, float z) {
    g_pFightStage->useCenterBias = 1;
    g_pFightStage->battleCenter.x = x;
    g_pFightStage->battleCenter.y = 0.0f;
    g_pFightStage->battleCenter.z = z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetFightStateFlags() {
    const FightRuntimePrefix *const work = vfmaGetTaskParam();
    if (work)
        return work->stateFlags;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetActiveFightModeFlag() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (task) {
        FightRuntimePrefix *const work = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
        if (work->state == 3)
            work->stateFlags |= 0x80U;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetFightFrame() {
    const FightRuntimePrefix *const work = vfmaGetTaskParam();
    if (work)
        return work->frame;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t EV_vfBattleUtil(std::uint8_t operation, shen_name name, std::int32_t value) {
    std::uint32_t result = 0xffffffffU;
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    FightRuntimePrefix *game = task ? static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task)) : 0;
    FightActorRuntimePrefix *actor;
    switch (operation) {
    case 0:
        if (game)
            result = vfclControlRuntimeFlags(game->collision, name, value);
        break;
    case 1:
        result = EV_camCtrlToggleCombatCamera(name, value);
        break;
    case 2:
        actor = btlGetActorByScenarioName(game, name);
        if (!actor)
            break;
        switch (value) {
        case 0:
            result = actor->hitStateFlags;
            break;
        case 1:
            result = actor->activeMotionFlags;
            break;
        case 2:
            result = actor->savedHitStateFlags;
            break;
        case 3:
            result = actor->previousHitStateFlags;
            break;
        case 4:
            result = actor->previousActiveMotionFlags;
            break;
        case 5:
            result = actor->motionFlags;
            break;
        case 6:
            result = actor->secondaryMotionFlags;
            break;
        case 7:
            result = actor->savedMotionFlags;
            break;
        case 8:
            result = actor->savedSecondaryMotionFlags;
            break;
        case 9:
            result = actor->hitResolutionFlags;
            break;
        case 10:
            result = actor->moveFlags;
            break;
        case 12:
            result = actor->groundContactFlags;
            break;
        case 13:
            result = actor->reactionFlags;
            break;
        case 14:
            result = actor->hitFlags;
            break;
        }
        break;
    case 3:
        result = name & g_pFightStage->battleFlags;
        if (value > 0)
            g_pFightStage->battleFlags |= name;
        else if (!value)
            g_pFightStage->battleFlags &= ~name;
        if (game && name == 4) {
            if (value > 0)
                game->stateFlags |= 0x4000U;
            else if (!value)
                game->stateFlags &= ~0x4000U;
        }
        break;
    case 4:
        if (game) {
            result = game->stateFlags;
            if (value > 0)
                game->stateFlags |= name;
            else if (!value)
                game->stateFlags &= ~name;
        }
        break;
    case 5:
        actor = btlGetActorByScenarioName(game, name);
        if (actor) {
            actor->hp = 0.0f;
            actor->moveFlags |= 0x20U;
        }
        break;
    case 6:
        actor = btlGetActorByScenarioName(game, name);
        if (actor) {
            result = actor->moveFlags & 0x400000U;
            if (value > 0)
                actor->moveFlags |= 0x400000U;
            else if (!value)
                actor->moveFlags &= ~0x400000U;
        }
        break;
    case 7:
        result = vfCheckSkillFlag(name);
        break;
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t vfmDispatchControl(std::uint8_t operation, shen_name name, std::uintptr_t payload, std::int32_t action) {
    std::int32_t result = -1;
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    FightRuntimePrefix *game;
    if (task)
        game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
    else
        game = 0;
    switch (operation) {
    case 1: {
        FightActorRuntimePrefix *const actor = btlGetActorByScenarioName(game, name);
        if (actor) {
            std::int8_t *const rank = reinterpret_cast<std::int8_t *>(actor) + 0x780;
            result = *rank;
            if (action > 0)
                *rank = static_cast<std::int8_t>(payload);
        }
        break;
    }
    case 2:
        result = reinterpret_cast<std::intptr_t>(g_pFightStage->packData);
        if (action > 0)
            g_pFightStage->packData = FindPakByEntryName(reinterpret_cast<const char *>(payload));
        break;
    case 4:
        if (g_pFightStage) {
            result = static_cast<std::int16_t>(g_pFightStage->teamRespawnTimers[g_pFightStage->spawnPointCount]);
            g_pFightStage->teamRespawnTimers[g_pFightStage->spawnPointCount] = static_cast<std::uint16_t>(payload);
        }
        break;
    case 5:
        if (game) {
            Character *const character = ctrlGetScenarioChara(name);
            if (character && !EV_camCtrlToggleCombatCamera(4, 1))
                BeginMotionUpdateCamera(character);
        }
        break;
    case 6:
        result = camCtrlGetInterpMode();
        if (action > 0)
            camCtrlSetInterpModeTo(static_cast<std::uint8_t>(payload));
        break;
    case 7: {
        FightActorRuntimePrefix *const actor = btlGetActorByScenarioName(game, name);
        if (actor) {
            std::uint32_t *const values = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x2fc);
            result = static_cast<std::int32_t>(values[7]);
            if (action > 0) {
                values[7] = payload;
                values[8] = payload;
                values[10] = 0;
            } else if (!action) {
                std::uint32_t *const cleared = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x2fc);
                cleared[7] = 0;
                cleared[8] = 0;
            }
        }
        break;
    }
    case 8:
        if (game) {
            result = game->cameraParameter;
            if (action > 0)
                game->cameraParameter = static_cast<std::uint8_t>(payload);
            else if (!action)
                game->cameraParameter = 0;
            else
                break;
            g_pFightStage->battleFlags |= 0x40U;
        }
        break;
    case 9:
        if (game)
            result = g_pFightStage->groupCounters[payload & 1U];
        break;
    case 10:
        if (game) {
            result = game->spriteDepth;
            if (action > 0)
                game->spriteDepth = static_cast<std::uint8_t>(payload);
            else if (!action)
                game->spriteDepth = 0;
        }
        break;
    case 11:
        ResetMotionCameraWaypoints();
        break;
    case 12:
        AppendMotionCameraWaypoint(*reinterpret_cast<const Vector3 *>(payload));
        break;
    case 13:
        BeginMotionInterpolationCamera(ctrlGetScenarioChara(name));
        break;
    case 14:
        result = vfMotnNibbleGet(static_cast<std::uint8_t>(name));
        if (action > 0)
            vfMotnNibbleSet(static_cast<std::uint8_t>(name), static_cast<std::int32_t>(payload));
        break;
    case 15:
        SetFightRoundCounts(static_cast<std::uint8_t>(name), static_cast<std::uint8_t>(payload));
        break;
    case 16:
        SetStreetFightLevel(static_cast<std::uint8_t>(payload));
        break;
    case 17:
        CreateFightStrikeLabels();
        break;
    case 18:
        result = *(reinterpret_cast<std::int8_t *>(game) + 0x1de);
        break;
    case 19:
        result = *(reinterpret_cast<std::int8_t *>(game) + 0x23a);
        break;
    case 20:
        if (action > 0) {
            fbLoadGauge();
            SetupLoadSprite();
            g_is_vfm_active = 1;
            result = 1;
        } else if (!action) {
            result = 0;
            if (g_is_vfm_active) {
                result = 1;
                FreeBattleGaugeSprites();
                FreeLoadSprites();
                g_is_vfm_active = 0;
            }
        } else
            result = g_is_vfm_active;
        break;
    case 21:
        if (action > 0) {
            QueueWazamakiTask();
            result = 1;
        } else
            result = IsWazamakiInactive();
        break;
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
FightActorRuntimePrefix *btlGetActorByScenarioName(FightRuntimePrefix *game, shen_name name) {
    if (!game)
        return 0;
    Character *const character = ctrlGetScenarioChara(name);
    if (!character)
        return 0;
    std::int32_t index = 0;
    FightActorRuntimePrefix *actor = 0;
    for (; index < 6; ++index) {
        if (game->characters[index] && game->characters[index] == character) {
            actor = game->actors[index];
            break;
        }
    }
    return actor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocateFGST() {
    std::uint8_t index;
    g_pFightStage = static_cast<FGST_t *>(HLib::AllocateTaskData(sizeof(FGST_t), TASK_FGST));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage);
    for (index = 0; index < 2; ++index) {
        FightSpawnPoint *const point = &g_pFightStage->spawnPoints[index];
        point->x = 0.0f;
        point->z = 0.0f;
        point->parameter = 0.0f;
        g_pFightStage->teamRespawnTimers[index] = 0;
    }
    g_pFightStage->spawnPointCount = 0;
    g_pFightStage->enemySlotCount = 0;
    g_pFightStage->formationCount[0] = g_pFightStage->formationCount[1] = 0;
    g_pFightStage->readyEnemySlots[0] = g_pFightStage->readyEnemySlots[1] = 0;
    g_pFightStage->activeFightersByTeam[0] = g_pFightStage->activeFightersByTeam[1] = 0;
    g_pFightStage->groupCounters[0] = g_pFightStage->groupCounters[1] = 0;
    g_pFightStage->groupCounters[2] = g_pFightStage->groupCounters[3] = 0;
    g_pFightStage->secondaryGroupCounters[0] = g_pFightStage->secondaryGroupCounters[1] = 0;
    g_pFightStage->secondaryGroupCounters[2] = g_pFightStage->secondaryGroupCounters[3] = 0;
    g_pFightStage->useCenterBias = 0;
    g_pFightStage->accumulatedHp = 0;
    g_pFightStage->battleFlags = 0;
    g_pFightStage->packData = 0;
    g_pFightStage->smokeInitTask = 0;
    for (index = 0; index < 8; ++index) {
        g_pFightStage->styles[index].style = 0;
        g_pFightStage->styles[index].file = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeFightStage() {
    HLib::ClearTaskDataMark(g_pFightStage);
    HLib::FreeTaskData(g_pFightStage);
    g_pFightStage = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AllocateFCMG(std::uint8_t count) {
    std::uint32_t size = sizeof(FCMG_t);
    count = count > 16 ? 16 : count;
    count = count ? count : 1;
    g_pFightStage->enemySlots = static_cast<FCMG_t *>(HLib::AllocateTaskData(size * count, TASK_FCMG));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemyCount = count;
    for (std::uint8_t index = 0; index < count; ++index)
        g_pFightStage->enemySlots[index].character = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AllocateFRMG(std::uint8_t count) {
    std::uint32_t size = sizeof(FRMG_t);
    count = static_cast<std::int8_t>(count) < 0 ? 127 : count;
    count = count ? count : 1;
    g_pFightStage->groups = static_cast<FRMG_t *>(HLib::AllocateTaskData(size * count, TASK_FRMG));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->groups);
    g_pFightStage->groupCount = count;
    for (std::uint8_t index = 0; index < count; ++index) {
        FRMG_t &group = g_pFightStage->groups[index];
        group.character = 0;
        group.activationState = 0;
        group.activationIndex = 255;
        group.activationParameter = 0;
        group.animation = group.spawnYaw = -1;
        group.assignment = -1;
        group.secondaryAssignment = -1;
        group.activationFlags = 0;
    }
    g_vfFormationGroupIndex = 99;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeFightFormations() {
    HLib::ClearTaskDataMark(g_pFightStage->groups);
    HLib::FreeTaskData(g_pFightStage->groups);
    g_pFightStage->groups = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AllocateFGEN(std::uint8_t count) {
    count = static_cast<std::int8_t>(count) < 0 ? 127 : count;
    count = count ? count : 1;
    g_pFightStage->generators = static_cast<FGEN_t *>(HLib::AllocateTaskData(sizeof(FGEN_t) * count, TASK_FGEN));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->generators);
    g_pFightStage->generatorCount = count;
    for (std::uint8_t index = 0; index < count; ++index)
        g_pFightStage->generators[index].enabled = 0;
    g_FGEN_count = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FreeFightGenerators() {
    HLib::ClearTaskDataMark(g_pFightStage->generators);
    HLib::FreeTaskData(g_pFightStage->generators);
    g_pFightStage->generators = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void UpdateEnemySlotCooldownsForDefeat(FightRuntimePrefix *game) {
    if (static_cast<std::uint8_t>(g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1]) > game->maximumActiveEnemies)
        vfUpdateEnemySlotCooldowns(game);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void vfTransitionAllEnemySlotsToInactive(FightRuntimePrefix *game) {
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (!slot.character || slot.state == 4)
            continue;
        figpAssignModeForCharacter(slot.character, 0);
        SetCharacterAlpha(slot.character, 1);
        vfTransitionEnemySlotToInactive(game, &slot);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
FightRuntimePrefix *vfmaGetTaskParam() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    return task ? static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task)) : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void *GetFightTaskParameter() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(10, 1);
    if (!task)
        return 0;
    return HLib::GetTaskParameter(task);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t IsFightStageFlag2Set() {
    return g_pFightStage->battleFlags & 2U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float vfIntBitsToFloat(std::int32_t bits) {
    return *reinterpret_cast<float *>(&bits);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t vfFindCharaIndex(const FightRuntimePrefix *game, const Character *character) {
#else
std::uint32_t vfFindCharaIndex(const FightRuntimePrefix *game, const Character *character) {
#endif
    for (std::uint8_t index = 0; index < 6; ++index)
        if (game->characters[index] == character)
            return index;
    return 99;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t vfIsLastEnemyForCamera(FightActorRuntimePrefix *actor) {
    if (actor->game->slotTeams[actor->groupIndex] == 1 && !g_pFightStage->secondaryGroupCounters[1] && !g_pFightStage->readyEnemySlots[1]) {
        if (static_cast<std::int8_t>(g_pFightStage->unknownGroupCounts[1]) == 1 && !(actor->moveFlags & 0x20U))
            return 1;
        if (!g_pFightStage->unknownGroupCounts[1] && (actor->moveFlags & 0x20U))
            return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void vfSetAllEnemySlotsUnknownFlag(std::uint32_t enabled) {
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t *const slot = &g_pFightStage->enemySlots[index];
        if (slot && slot->character)
            SetCharacterUnknownFlag(slot->character, enabled);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ToggleStreetFighterUI() {
    if (g_fightActive) {
        check_dead_status(1);
        return;
    }
    g_hasColouredText = 0;
    vfFreeStageObjects();
    FreeFightStyles();
    if (!g_is_vfm_active) {
        FreeBattleGaugeSprites();
        FreeLoadSprites();
    }
    g_fightUiActive = g_fightActive = 0;
    if (!g_pFightStage->smokeInitTask)
        FreeFightSmokeEffects();
    else
        HLib::DestroyTaskInternal(g_pFightStage->smokeInitTask);
    HLib::ClearTaskDataMark(g_pFightStage->enemySlots);
    HLib::FreeTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemySlots = 0;
    FreeFightFormations();
    FreeFightAreaMaskNullsub();
    FreeFightGenerators();
    FreeFightStage();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetFightActorParameterBytes(shen_name character, const std::uint8_t *values) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return;
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
    std::byte *const parameters = btlGetActorByScenarioName(game, character)->motionParameters;
    parameters[0x1a] = static_cast<std::byte>(values[0]);
    parameters[0x1b] = static_cast<std::byte>(values[1]);
    parameters[0x1c] = static_cast<std::byte>(values[2]);
    parameters[0x1d] = static_cast<std::byte>(values[3]);
    parameters[0x1e] = static_cast<std::byte>(values[4]);
    parameters[0x1f] = static_cast<std::byte>(values[5]);
    parameters[0x20] = static_cast<std::byte>(values[6]);
    parameters[0x21] = static_cast<std::byte>(values[7]);
    parameters[0x22] = static_cast<std::byte>(values[8]);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t vfclControlRuntimeFlags(FightCollisionRuntimePrefix *collision, std::uint32_t mask, std::int32_t action) {
    const std::uint32_t result = mask & collision->runtimeFlags;
    if (action > 0)
        collision->runtimeFlags |= mask;
    else if (!action)
        collision->runtimeFlags &= ~mask;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// CTRL construction.

// @verified - d
void InitVFM() {
    g_is_vfm_active = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint8_t g_battleQteCommandSelection;
std::uint16_t g_fightLoadWidth{}, g_fightLoadHeight{};
FGST_t *g_pFightStage{};
std::uint8_t g_FGEN_count{};
std::uint8_t g_vfFormationGroupIndex;
std::uint8_t g_vfStreetFightLevel;
std::int32_t g_vfStrikeLabelSprites[2];
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t IsFightActive() {
    return g_fightActive != 0;
}
#endif

namespace {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint8_t streetFightProgress{};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native PC's area-mask allocation and disposal calls both target the proven nullsub.
void FightAreaMaskNullsub(std::uintptr_t) noexcept {
}
#endif

} // namespace
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// The reset leaf does not require FBQT admission.
// @verified - p
std::uint8_t fbqtResetStates(std::uint8_t mode) noexcept {
    if (mode == 0)
        g_battleQteCommandSelection = 0;
    return mode;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint8_t shenStartFGST(std::uint8_t enemies, std::uint8_t groups, std::uint8_t areaMasks, std::uint8_t generators) {
    ShowElementByIndex(UIElement::StreetFighterHud);
    AllocateFGST();
    AllocateFCMG(enemies);
    AllocateFRMG(groups);
    FightAreaMaskNullsub(areaMasks);
    AllocateFGEN(generators);
    for (FightEntry &entry : g_activeFightData)
        entry.character = nullptr;
    AllocateFBIT();
    g_hasColouredText = 0;
    vfInitStageObjects();
    if (!g_is_vfm_active) {
        fbLoadGauge();
        SetupLoadSprite();
    }
    g_fightLoadWidth = 64;
    g_fightLoadHeight = 128;
    (void)vfmaGetTaskParam();
    EV_camCtrlToggleCombatCamera(UINT32_MAX, 0);
    return fbqtResetStates(0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t shenInitFightData(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style) {
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int8_t>(PublishFightFormation(character, blocker, poolMode, team, hp, style, 0, 0, 0.0f, 0.0f, 0.0f))));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Unlike the FENE-name lookup, this resolves the scenario character first.
// @verified - p
VFActor *btlGetActorByScenarioName(VFMA_t *game, shen_name name) {
    if (!game)
        return nullptr;
    Character *const character = ctrlGetScenarioChara(name);
    if (!character)
        return nullptr;
    for (std::uint32_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->characters[i] == character)
            return game->actors[i];
    return nullptr;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t vfclControlRuntimeFlags(VFCL_t *collision, std::uint32_t mask, std::int32_t action) {
    const std::uint32_t previous = collision->runtimeFlags;
    if (action > 0)
        collision->runtimeFlags = previous | mask;
    else if (action == 0)
        collision->runtimeFlags = previous & ~mask;
    return previous & mask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint8_t vfUpdateFlagsFor(Character *character, std::int8_t mode, std::uint32_t flag, std::int32_t value) {
    std::uint8_t index = g_vfFormationGroupIndex;
    if (character || index == 99) {
        std::int8_t candidate = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(g_pFightStage->groupCount - 1U));
        while (candidate >= 0 && g_pFightStage->groups[candidate].character != character)
            --candidate;
        if (candidate < 0 || candidate == 99)
            return 0;
        index = static_cast<std::uint8_t>(candidate);
    }
    FRMG_t &group = g_pFightStage->groups[index];
    if (mode == 1) {
        group.formationFlags &= ~flag;
        if (flag & 0x30001U) {
            VFActor *const actor = group.slot->fighter;
            if (actor)
                actor->formationFlags &= static_cast<std::int32_t>(0xfffcfffeU);
        }
        return 1;
    }
    group.formationFlags |= flag;
    const std::uint8_t byte = static_cast<std::uint8_t>(value);
    const std::uint16_t word = static_cast<std::uint16_t>(value);
    switch (flag) {
    case 1:
        group.gameplayParameters[0] = (group.gameplayParameters[0] & 0x0000ffffU) | (static_cast<std::uint32_t>(word) << 16U);
        break;
    case 2:
    case 4:
    case 0x80000U:
        group.gameplayParameters[1] = (group.gameplayParameters[1] & 0x0000ffffU) | (static_cast<std::uint32_t>(word) << 16U);
        break;
    case 0x100U:
        group.actionParameters[0] = word;
        break;
    case 0x200U:
        group.actionParameters[1] = word;
        break;
    case 0x400U:
        if (group.animation == 0xffffU)
            group.animation = word;
        else
            group.spawnYaw = word;
        break;
    case 0x8000U:
        group.activationFlags = static_cast<std::uint32_t>(value);
        break;
    case 0x20000U:
        group.gameplayParameters[1] = (group.gameplayParameters[1] & 0xffff0000U) | word;
        break;
    case 0x40000U:
        group.gameplayParameters[0] = (group.gameplayParameters[0] & 0xffffff00U) | byte;
        break;
    case 0x200000U:
        group.waveFlags |= static_cast<std::uint32_t>(value);
        break;
    case 0x400000U:
        group.selectFlags |= byte;
        break;
    case 0x800000U:
        group.activationState = (group.activationState & 0xff00ff00U) | 2U | (static_cast<std::uint32_t>(byte) << 16U);
        break;
    case 0x1000000U:
        group.activationState = (group.activationState & 0xff000000U) | 0x201U | (static_cast<std::uint32_t>(byte) << 16U);
        group.spawnWords[0] = 470;
        break;
    case 0x2000000U:
        group.activationState = (group.activationState & 0xffffff00U) | 3U;
        break;
    case 0x8000000U:
        group.activationState = (group.activationState & 0x00ffffffU) | (static_cast<std::uint32_t>(byte) << 24U);
        break;
    case 0x10000000U:
        group.assignment = byte;
        break;
    case 0x20000000U:
        group.activationState = (group.activationState & 0xffffff00U) | 5U;
        break;
    case 0x80000000U:
        (void)fbqtResetStates(byte);
        break;
    default:
        break;
    }
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ToggleFightMotionCallbackFlag(Character *character, std::int32_t enabled) {
    MotionWork *const motion = FindMotionWork(character);
    if (enabled)
        motion->runtime.callbackFlags |= 8U;
    else
        motion->runtime.callbackFlags &= ~8U;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t SetFightRoundCounts(std::uint8_t first, std::uint8_t second) {
    g_pFightStage->battleFlags |= 0x40000U;
    SetFightRoundPrompt(static_cast<std::int32_t>(first) + second);
    g_vfRoundResultCounters[0] = std::min<std::uint8_t>(first, 2);
    g_vfRoundResultCounters[2] = std::min<std::uint8_t>(static_cast<std::uint8_t>(g_vfRoundResultCounters[0] + 1), 2);
    g_vfRoundResultCounters[1] = std::min<std::uint8_t>(second, 2);
    g_vfRoundResultCounters[3] = std::min<std::uint8_t>(static_cast<std::uint8_t>(g_vfRoundResultCounters[1] + 1), 2);
    return g_vfRoundResultCounters[3];
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// The native caller discards GetSFLevel's text pointer.
void SetStreetFightLevel(std::uint8_t level) {
    g_vfStreetFightLevel = level;
    g_pFightStage->battleFlags |= 0x10000U;
    GetSFLevel(level);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t GetStreetFightProgress() noexcept {
    return streetFightProgress;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t SetStreetFightProgress(std::uint32_t value) noexcept {
    if (value > 255U)
        value = 255U;
    streetFightProgress = static_cast<std::uint8_t>(value);
    return value;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t AddStreetFightProgress(std::int32_t delta) noexcept {
    std::int32_t value = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(delta) + streetFightProgress);
    if (value > 255)
        value = 255;
    else if (value < 0)
        value = 0;
    streetFightProgress = static_cast<std::uint8_t>(value);
    return static_cast<std::uint32_t>(value);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ResetStreetFightProgress() noexcept {
    streetFightProgress = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// These separate legacy handles are not LOADNUM's six records.
// @verified - p
std::uint64_t CreateFightStrikeLabels() {
    g_pFightStage->battleFlags |= 0x20000U;
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_vfStrikeLabelSprites[0] = static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(control->windowTask, "strike(s)", 256, -1, -1, 0));
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::uint64_t result = CreateLeftAlignedWindowTextSurface(control->windowTask, "left", 128, -1, -1, 0);
    g_vfStrikeLabelSprites[1] = static_cast<std::int32_t>(result);
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// EAX zero-extends every result, including the default UINT32_MAX.
// @verified - p
std::uint32_t EV_vfBattleUtil(FightBattleUtilOp operation, shen_name name, std::int32_t value) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    VFMA_t *const game = task ? static_cast<VFMA_t *>(HLib::GetTaskParameter(task)) : nullptr;
    switch (operation) {
    case FightBattleUtilOp::CollisionFlags:
        if (game)
            return vfclControlRuntimeFlags(game->collision, name.name, value);
        break;
    case FightBattleUtilOp::CombatCameraFlags:
        return EV_camCtrlToggleCombatCamera(name.name, value);
    case FightBattleUtilOp::ActorField:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            switch (value) {
            case 0:
                return actor->hitStateFlags;
            case 1:
                return actor->activeMotionFlags;
            case 2:
                return actor->savedHitStateFlags;
            case 3:
                return actor->previousHitStateFlags;
            case 4:
                return actor->previousActiveMotionFlags;
            case 5:
                return actor->motionFlags;
            case 6:
                return actor->secondaryMotionFlags;
            case 7:
                return actor->savedMotionFlags;
            case 8:
                return actor->savedSecondaryMotionFlags;
            case 9:
                return actor->hitResolutionFlags;
            case 10:
                return actor->moveFlags;
            case 11:
                return actor->stateFlags;
            case 12:
                return actor->groundContactFlags;
            case 13:
                return actor->reactionFlags;
            case 14:
                return actor->hitFlags;
            default:
                break;
            }
        }
        break;
    case FightBattleUtilOp::StageBattleFlags: {
        const std::uint32_t previous = g_pFightStage->battleFlags;
        if (value > 0)
            g_pFightStage->battleFlags = previous | name.name;
        else if (value == 0)
            g_pFightStage->battleFlags = previous & ~name.name;
        if (game && name.name == 4) {
            if (value > 0)
                game->mode |= 0x4000U;
            else if (value == 0)
                game->mode &= ~0x4000U;
        }
        return previous & name.name;
    }
    case FightBattleUtilOp::FightModeFlags:
        if (game) {
            const std::uint32_t previous = game->mode;
            if (value > 0)
                game->mode = previous | name.name;
            else if (value == 0)
                game->mode = previous & ~name.name;
            return previous;
        }
        break;
    case FightBattleUtilOp::ForceActorDefeat:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            actor->moveFlags |= VFMF_ForceDefeat;
            actor->hp = 0.0f;
        }
        break;
    case FightBattleUtilOp::MoveFlag400000:
    case FightBattleUtilOp::MoveFlag40000:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            const std::uint32_t mask = operation == FightBattleUtilOp::MoveFlag400000 ? 0x400000U : 0x40000U;
            const std::uint32_t previous = actor->moveFlags;
            if (value > 0)
                actor->moveFlags = previous | mask;
            else if (value == 0)
                actor->moveFlags = previous & ~mask;
            return previous & mask;
        }
        break;
    case FightBattleUtilOp::SkillMask:
        return static_cast<std::uint8_t>(vfCheckSkillFlag(name));
    case FightBattleUtilOp::MotionOrSlotFlag:
        if (Character *const character = ctrlGetScenarioChara(name)) {
            if (value == 0)
                ToggleFightMotionCallbackFlag(character, 0);
            else if (value > 0) {
                for (std::uint32_t i = 0; i < g_pFightStage->enemyCount; ++i) {
                    if (g_pFightStage->enemySlots[i].character != character)
                        continue;
                    g_pFightStage->enemySlots[i].flags |= 2U;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
    return UINT32_MAX;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// The default and side-effect-only branches retain signed RAX -1.
// @verified - p
std::int64_t vfmDispatchControl(FightControlOp operation, shen_name name, std::uintptr_t payload, std::int32_t action) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    VFMA_t *const game = task ? static_cast<VFMA_t *>(HLib::GetTaskParameter(task)) : nullptr;
    std::int64_t result = -1;
    switch (operation) {
    case FightControlOp::ReactionRank:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            result = static_cast<std::int8_t>(actor->reactionRank);
            if (action > 0)
                actor->reactionRank = static_cast<std::uint8_t>(payload);
        }
        break;
    case FightControlOp::Package:
        result = std::bit_cast<std::int64_t>(reinterpret_cast<std::uintptr_t>(g_pFightStage->packData));
        if (action > 0)
            g_pFightStage->packData = FindPakByEntryName(reinterpret_cast<const char *>(payload));
        break;
    case FightControlOp::SpawnTimer:
        if (g_pFightStage) {
            // Native C0 + 2*C4 deliberately uses the current authored spawn-point count.
            std::byte *const timer = reinterpret_cast<std::byte *>(g_pFightStage) + offsetof(FGST_t, teamRespawnTimers) + 2U * g_pFightStage->spawnPointCount;
            std::int16_t previous;
            std::memcpy(&previous, timer, sizeof(previous));
            const std::uint16_t replacement = static_cast<std::uint16_t>(payload);
            std::memcpy(timer, &replacement, sizeof(replacement));
            result = previous;
        }
        break;
    case FightControlOp::MotionUpdateCamera:
        if (game) {
            Character *const character = ctrlGetScenarioChara(name);
            if (character && EV_camCtrlToggleCombatCamera(4, 1) == 0)
                BeginMotionUpdateCamera(character);
        }
        break;
    case FightControlOp::CameraMode:
        result = static_cast<std::uint8_t>(camCtrlGetInterpMode());
        if (action > 0)
            camCtrlSetInterpModeTo(static_cast<CameraMotionMode>(static_cast<std::uint8_t>(payload)));
        break;
    case FightControlOp::QueuedAction:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            result = std::bit_cast<std::int64_t>(actor->parameter370);
            if (action > 0) {
                vfResetReactionProgram(actor);
                actor->parameter370 = payload;
                actor->parameter378 = payload;
                actor->deferredReaction = 0;
                if (((payload >> 24U) & 0xffU) == 8 && actor->pairedActor) {
                    VFActor *const paired = actor->pairedActor;
                    vfResetReactionProgram(paired);
                    paired->action = 0x1000000U;
                    paired->parameter370 = 0;
                    paired->parameter378 = 0;
                    paired->deferredReaction = 0;
                }
            } else if (action == 0) {
                actor->parameter370 = 0;
                actor->parameter378 = 0;
            }
        }
        break;
    case FightControlOp::CameraParameter:
        if (game) {
            result = static_cast<std::uint8_t>(game->cameraParameter);
            if (action >= 0) {
                game->cameraParameter = action > 0 ? static_cast<std::int8_t>(payload) : 0;
                g_pFightStage->battleFlags |= 0x40U;
            }
        }
        break;
    case FightControlOp::GroupCounter:
        if (game)
            result = g_pFightStage->groupCounters[payload & 1U];
        break;
    case FightControlOp::SpriteDepth:
        if (game) {
            result = game->spriteDepth;
            if (action > 0)
                game->spriteDepth = static_cast<std::uint8_t>(payload);
            else if (action == 0)
                game->spriteDepth = 0;
        }
        break;
    case FightControlOp::ResetCameraWaypoints:
        ResetMotionCameraWaypoints();
        break;
    case FightControlOp::AppendCameraWaypoint:
        AppendMotionCameraWaypoint(*reinterpret_cast<const Vector3 *>(payload));
        break;
    case FightControlOp::MotionInterpolationCamera:
        BeginMotionInterpolationCamera(ctrlGetScenarioChara(name));
        break;
    case FightControlOp::MotionNibble: {
        const std::int8_t code = static_cast<std::int8_t>(name.name);
        result = vfMotnNibbleGet(code);
        if (action > 0)
            vfMotnNibbleSet(code, static_cast<std::int32_t>(payload));
        break;
    }
    case FightControlOp::RoundCounts:
        SetFightRoundCounts(static_cast<std::uint8_t>(name.name), static_cast<std::uint8_t>(payload));
        break;
    case FightControlOp::StreetFightLevel:
        SetStreetFightLevel(static_cast<std::uint8_t>(payload));
        break;
    case FightControlOp::StrikeLabels:
        CreateFightStrikeLabels();
        break;
    case FightControlOp::StrikesLeft:
        result = static_cast<std::int8_t>(game->strikesLeft);
        break;
    case FightControlOp::BattleCounter:
        result = static_cast<std::int8_t>(game->counter32c);
        break;
    case FightControlOp::GaugeResources:
        if (action > 0) {
            fbLoadGauge();
            SetupLoadSprite();
            g_is_vfm_active = 1;
            result = 1;
        } else if (action == 0) {
            if (g_is_vfm_active) {
                FreeBattleGaugeSprites();
                FreeLoadSprites();
                g_is_vfm_active = 0;
                result = 1;
            } else
                result = 0;
        } else
            result = g_is_vfm_active;
        break;
    case FightControlOp::MovesScroll:
        if (action > 0) {
            QueueWazamakiTask();
            result = 1;
        } else
            result = static_cast<std::int32_t>(IsWazamakiInactive());
        break;
    case FightControlOp::BattleType:
        result = game->type;
        break;
    default:
        break;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
VFMA_t *check_dead_status(std::uint8_t mode) {
    VFMA_t *const game = vfmaGetTaskParam();
    VFMA_t *result = game;
    if (game && game->state == 3) {
        for (std::uint32_t index = 0; index < g_pFightStage->groupCount; ++index) {
            FRMG_t &group = g_pFightStage->groups[index];
            if (group.character) {
                group.state = 10;
                if (reinterpret_cast<std::uintptr_t>(group.character) != FIGHT_DEAD.name)
                    g_activeFightData[group.slot->fightEntryIndex].status = 10;
            }
            // The native leaf returns the last loop bound in RAX, not necessarily the VFMA pointer.
            result = reinterpret_cast<VFMA_t *>(static_cast<std::uintptr_t>(g_pFightStage->groupCount));
        }
        if (mode == 0)
            game->state = 6;
        else if (mode == 1)
            game->state = 9;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::intptr_t ToggleStreetFighterUI() {
    if (g_fightActive)
        return reinterpret_cast<std::intptr_t>(check_dead_status(1));
    g_hasColouredText = 0;
    vfFreeStageObjects();
    for (FightStyleSlot &style : g_pFightStage->styles) {
        if (!style.file)
            continue;
        HLib::FreeTaskData(style.file);
        style.style = 0;
    }
    if (!g_is_vfm_active) {
        FreeBattleGaugeSprites();
        FreeLoadSprites();
    }
    g_fightActive = 0;
    g_fightUiActive = 0;
    if (g_pFightStage->smokeInitTask)
        HLib::DestroyTask(g_pFightStage->smokeInitTask);
    else
        FreeFightSmokeEffects();
    HLib::ClearTaskDataMark(g_pFightStage->enemySlots);
    HLib::FreeTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemySlots = nullptr;
    HLib::ClearTaskDataMark(g_pFightStage->groups);
    HLib::FreeTaskData(g_pFightStage->groups);
    g_pFightStage->groups = nullptr;
    FightAreaMaskNullsub(0);
    HLib::ClearTaskDataMark(g_pFightStage->generators);
    HLib::FreeTaskData(g_pFightStage->generators);
    g_pFightStage->generators = nullptr;
    DestroyElementByIndex(UIElement::StreetFighterHud);
    HLib::ClearTaskDataMark(g_pFightStage);
    const std::intptr_t result = HLib::FreeTaskData(g_pFightStage);
    g_pFightStage = nullptr;
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void fightInit(std::uint16_t mode) {
    g_fightActive = 0;
    g_fightUiActive = 0;
    const std::uint16_t requested = static_cast<std::uint16_t>(mode | 0x80U);
    SetLoadingPhaseFlags(GetLoadingPhaseFlags() & ~LoadingPhaseFightInitMask);
    g_fightModeFlags = static_cast<std::uint16_t>((requested & 0xff7fU) | 8U);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t GetFightModeFlags() {
    const std::uint32_t loading = GetLoadingPhaseFlags();
    std::uint16_t flags = g_fightModeFlags;
    if (loading & 0x20U)
        flags |= 1U;
    if (loading & 0x40U)
        flags |= 2U;
    if (loading & 0x80U)
        flags |= 4U;
    return flags;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Clear the loading mask conditionally; retain the WORD transform on every call.
std::uint64_t SetFightModeFlags(std::uint16_t flags) {
    const std::uint16_t previous = g_fightModeFlags;
    if (flags & 0x80U)
        SetLoadingPhaseFlags(GetLoadingPhaseFlags() & ~LoadingPhaseFightInitMask);
    g_fightModeFlags = static_cast<std::uint16_t>((flags & 0xff77U) | 8U);
    return previous;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
FGST_t *AllocateFGST() {
    g_pFightStage = static_cast<FGST_t *>(HLib::AllocateTaskData(sizeof(FGST_t), TASK_FGST));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage);
    for (std::size_t i = 0; i < 2; ++i) {
        g_pFightStage->spawnPoints[i] = {.parameter = 0.0f, .x = 0.0f, .z = 0.0f};
        g_pFightStage->teamRespawnTimers[i] = 0;
    }
    g_pFightStage->spawnPointCount = 0;
    g_pFightStage->enemySlotCount = 0;
    g_pFightStage->formationCount[1] = 0;
    g_pFightStage->formationCount[0] = 0;
    g_pFightStage->readyEnemySlots[1] = 0;
    g_pFightStage->readyEnemySlots[0] = 0;
    g_pFightStage->activeFightersByTeam[1] = 0;
    g_pFightStage->activeFightersByTeam[0] = 0;
    g_pFightStage->groupCounters[1] = 0;
    g_pFightStage->groupCounters[0] = 0;
    g_pFightStage->groupCounters[3] = 0;
    g_pFightStage->groupCounters[2] = 0;
    g_pFightStage->secondaryGroupCounters[1] = 0;
    g_pFightStage->secondaryGroupCounters[0] = 0;
    g_pFightStage->secondaryGroupCounters[3] = 0;
    g_pFightStage->secondaryGroupCounters[2] = 0;
    g_pFightStage->useCenterBias = 0;
    g_pFightStage->accumulatedHp = 0;
    g_pFightStage->battleFlags = 0;
    g_pFightStage->packData = nullptr;
    g_pFightStage->smokeInitTask = nullptr;
    for (std::size_t i = 0; i < 8; ++i) {
        g_pFightStage->styles[i].style = 0;
        g_pFightStage->styles[i].file = nullptr;
    }
    return g_pFightStage;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void AllocateFCMG(std::uint8_t count) {
    if (count > 16)
        count = 16;
    if (count == 0)
        count = 1;
    g_pFightStage->enemySlots = static_cast<FCMG_t *>(HLib::AllocateTaskData(sizeof(FCMG_t) * count, TASK_FCMG));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemyCount = count;
    for (std::uint8_t i = 0; i < count; ++i)
        g_pFightStage->enemySlots[i].character = nullptr;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void AllocateFRMG(std::uint8_t count) {
    if (count > 127)
        count = 127;
    if (count == 0)
        count = 1;
    g_pFightStage->groups = static_cast<FRMG_t *>(HLib::AllocateTaskData(sizeof(FRMG_t) * count, TASK_FRMG));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->groups);
    g_pFightStage->groupCount = count;
    for (std::uint8_t i = 0; i < count; ++i) {
        FRMG_t &group = g_pFightStage->groups[i];
        group.character = nullptr;
        group.activationState = 0xffff0000U;
        group.spawnYaw = 0xffffU;
        group.animation = 0xffffU;
        group.assignment = 0xffU;
        group.activationFlags = 0;
    }
    g_vfFormationGroupIndex = 99;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void AllocateFGEN(std::uint8_t count) {
    if (count > 127)
        count = 127;
    if (count == 0)
        count = 1;
    g_pFightStage->generators = static_cast<FGEN_t *>(HLib::AllocateTaskData(sizeof(FGEN_t) * count, TASK_FGEN));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->generators);
    g_pFightStage->generatorCount = count;
    for (std::uint8_t i = 0; i < count; ++i)
        g_pFightStage->generators[i].enabled = 0;
    g_FGEN_count = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
FGST_t *vfAddGeneratorPoint(float x, float y, float z) {
    FGEN_t &generator = g_pFightStage->generators[g_FGEN_count];
    generator.position = {.x = x, .y = y, .z = z};
    generator.enabled = 1;
    generator.other = 0;
    ++g_FGEN_count;
    return g_pFightStage;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
FGST_t *vfCopyPosFromEntity(std::uint8_t mode, Character *character, float x, float z, float parameter) {
    Vector3 position{};
    if (mode == 0)
        position = {.x = x, .y = 0.0f, .z = z};
    else if (mode == 1)
        position = GetCharacterWorldPosition(character);
    FightSpawnPoint &point = g_pFightStage->spawnPoints[g_pFightStage->spawnPointCount];
    point.x = position.x;
    point.z = position.z;
    point.parameter = parameter;
    ++g_pFightStage->spawnPointCount;
    return g_pFightStage;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void AllocateFBIT() {
    g_pFightStage->smokeInitTask = HLib::CreateNewCurrentTaskWithoutParameter(FB_CallBack_InitTask_Callback, 8, 2, TASK_FBIT.name);
    FBSM_t *const smoke = static_cast<FBSM_t *>(HLib::AllocateTaskData(sizeof(FBSM_t), TASK_FBSM));
    HLib::AssignDynamicAlignmentForTaskData(smoke);
    g_pFightStage->smoke = smoke;
    smoke->activeCount = 0;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i) {
        smoke->effects[i] = invalidEffect;
        smoke->slots[i].effect = 0xffffU;
        smoke->slots[i].remainingTicks = 0.0f;
        smoke->slots[i].birthRate = 0.0f;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void FB_CallBack_InitTask_Callback(void *) {
    std::uint64_t totalFree;
    std::uint64_t largestFree;
    HLib::TryGetFreeHeapStats(&totalFree, &largestFree);
    if (largestFree > 0x1200U) {
        InitializeFightSmokeEffects();
        HLib::DestroyCurrentTask();
        g_pFightStage->smokeInitTask = nullptr;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void InitializeFightSmokeEffects() {
    FBSM_t *const smoke = g_pFightStage->smoke;
    const Vector3 zero{};
    smoke->activeCount = 0;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i)
        smoke->effects[i] = static_cast<std::uint32_t>(CreateSmokeEffect(zero, zero, 40, 0.0f, shen_name{}, 4));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void UpdateFightSmokeEffects() {
    if (g_pFightStage->smokeInitTask != nullptr)
        return;
    FBSM_t *const smoke = g_pFightStage->smoke;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i) {
        FightSmokeSlot &slot = smoke->slots[i];
        if (slot.effect == 0xffffU)
            continue;
        slot.birthRate -= 0.025f;
        if (slot.birthRate < 0.0f)
            slot.birthRate = 0.0f;
        slot.remainingTicks -= shenGetScaledDeltaTickTime(1.0f);
        if (slot.remainingTicks > 0.0f) {
            SetParticleEffectBirthRate(0, static_cast<std::uint8_t>(slot.effect), slot.birthRate);
        } else {
            slot.remainingTicks = 0.0f;
            slot.effect = 0xffffU;
            --smoke->activeCount;
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void FreeFightSmokeEffects() {
    FBSM_t *const smoke = g_pFightStage->smoke;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i) {
        if (smoke->effects[i] == invalidEffect)
            continue;
        const std::uint8_t effect = static_cast<std::uint8_t>(smoke->effects[i]);
        SuspendParticleEffect(0, effect);
        FreeParticleEffect(0, effect);
    }
    HLib::ClearTaskDataMark(smoke);
    HLib::FreeTaskData(smoke);
    g_pFightStage->smoke = nullptr;
}
#endif


} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetFightStateFlags() {
    return shenmue::s2::GetFightStateFlags();
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetFightFrame() {
    return shenmue::s2::GetFightFrame();
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetStreetFightProgress() {
    return shenmue::s2::GetStreetFightProgress();
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetStreetFightProgress(std::uint32_t value) {
    return shenmue::s2::SetStreetFightProgress(value);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AddStreetFightProgress(std::int32_t delta) {
    return shenmue::s2::AddStreetFightProgress(delta);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_6(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::uint32_t period, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocatePeriodicBattleDialogue(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), period, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_13(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::uint32_t period, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition15(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), period, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_9(
    std::int16_t priority, std::int16_t dialogue, std::uint32_t character, std::uint32_t otherCharacter, float healthPercent, float probability) {
    return shenmue::s2::AllocateBattleDialogueCondition2(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), healthPercent, probability);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_8(
    std::int16_t priority, std::int16_t dialogue, std::uint32_t character, std::uint32_t otherCharacter, std::uint16_t health, float probability) {
    return shenmue::s2::AllocateBattleDialogueCondition3(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), health, probability);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_4(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogue(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_5(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition5(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_10(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition6(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_14(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition7(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_15(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition13(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_17(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition16(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_7(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition17(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_11(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition10(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_12(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition11(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_2(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition12(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_16(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::uint16_t animation, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateMotionBattleDialogue(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), animation, repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, std::uint32_t reactionMask, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateReactionBattleDialogue(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), reactionMask, repeatEvery, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_AllocateVCDS_3(std::int16_t priority, std::int16_t dialogue, std::uint32_t character,
    std::uint32_t otherCharacter, float distance, float probability, std::uint16_t flags) {
    return shenmue::s2::AllocateBattleDialogueCondition9(
        priority, dialogue, shenmue::shen_name(character), shenmue::shen_name(otherCharacter), distance, probability, flags);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetCameraDialogueTransition(std::int16_t priority, std::int16_t dialogue) {
    return shenmue::s2::QueueBattleDialogue(priority, dialogue);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_SetCameraDialogueNodeType(std::uint32_t id, std::uint16_t flags) {
    shenmue::s2::SetBattleDialogueFlags(id, flags);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_FreeCameraDialogueNode(std::uint32_t id) {
    shenmue::s2::FreeBattleDialogue(id);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_SetFlagForTaskData() {
    shenmue::s2::BeginFightDialogueRetirement();
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfDisableCameraDialogue(std::uint32_t id) {
    shenmue::s2::SuspendBattleDialogue(id);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_ResetCameraDialogueNode(std::uint32_t id) {
    shenmue::s2::ResumeBattleDialogue(id);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_GetCameraDialogueState(std::uint32_t id) {
    return shenmue::s2::GetBattleDialogueOccurrences(id);
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetCameraDialogueFlags(std::uint32_t id) {
    return shenmue::s2::GetBattleDialogueCompletions(id);
}
#endif
