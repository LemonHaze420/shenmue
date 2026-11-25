#include "s2/flag_wind_dc.hpp"
#include "s2/navigation.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/telephone_cord_dc.hpp"
#include "s2/event_overlay.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/maze_dc.hpp"
#include "s2/effect_fft.hpp"
#include "s2/hlib.hpp"
#include "s2/screen_film.hpp"
#include "s2/input_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/weather.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/texture_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/minigame_random.hpp"
#include "s2/date_time.hpp"
#include "s2/pause.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/load_title.hpp"

#include "s2/lcdm_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/event_character_state_dc.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/battle_qte.hpp"
#include "s2/extra_telop.hpp"
#include "s2/gauge.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/osage_dc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/arcade_modules_dc.hpp"
#include "s2/magic_room_dc.hpp"
#include "s2/handcuffs_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/author_controller_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/memo_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/scenario_control_dc.hpp"

namespace shenmue {
namespace s2 {
std::int32_t EV_authSceneCtrl(std::int32_t operation);
}
} // namespace shenmue
#include "s2/author_load_dc.hpp"
#include "s2/area_event.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/button_mediation_runtime_dc.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/debug.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/event_character_transform_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/flag_startup_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/fog_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/legacy_hud_text_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/lens_flare_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/title_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/math.hpp"
#include "s2/model_dc.hpp"
#include "s2/navi.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/window.hpp"
#include "s2/world_render.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/sound_table_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/weather_data_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/weather_particles_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace shenmue {
namespace dc {
namespace sdk {
void SetMT7VertexColourType(std::int32_t type);
}
} // namespace dc
} // namespace shenmue
namespace shenmue {
namespace s2 {
void nlSetTexanimUV(float u, float v);
void ApplyTexAnimUVs(void *mesh);
void RecursiveApplyTexAnimUVs(MT7Node *node);
void UpdateModelTextureCoordinates(MT7ModelChunk *model);
} // namespace s2
} // namespace shenmue
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
namespace shenmue { namespace s2 {
void SetOmakeVideoCount(std::uint32_t count);
std::uint32_t GetOmakeVideoCount();
void AddLightPriority(std::int32_t light);
void InitializeMazeNavigationTrail();
void InitializeShipSpray();
void AssignShipSprayPosition(const Vector3 &position);
void AssignShipSprayYaw(std::int32_t yaw);
void HIKI_Enqueue();
void AssignHikiYaw(std::int32_t yaw, std::int32_t reserved);
void mazeSetCurrentRefCount(std::uint32_t count);
std::uint32_t mazeGetCurrentRefCount();
void Queue_OP_StaffRoll();
std::uint32_t SetNpcColdResetMode(std::uint32_t mode);
std::uint32_t GetLoadingPhaseFlags();
void SetLoadingPhaseFlags(std::uint32_t flags);
void AssignShipSprayLocalPosition(const Vector3 *position);
void AssignHikiPosition(const Vector3 *position);
void AssignHikiLocalPosition(const Vector3 *position);
void AssignHikiRotation(std::int32_t yaw);
std::int32_t ResetAllNpcRouteStates();
void ReleaseNpcScriptRouteOwner();
std::int32_t npcStartPlayEntityAtPos(shen_name name, shen_name point, const Vector3 *position, std::int32_t yaw);
void SetPlayerCashOrMoney(std::int32_t amount, std::int32_t mode, std::int32_t step);
void SetChildObjectFlagByIndex(Character *character, std::uint8_t mode);
void AssignMapFacialClip(shen_name speaker, const std::int16_t *clip);
std::int32_t GetMainIconInputAdmitted();
void ReleaseLoadTitleFade();
void StopNpcFollowingMotionForEvent(shen_name name);
void SetFireCameraOffset(std::uint32_t index, float offset);
std::int32_t GetMagicRoomState3();
void SetNewNpcBatchCountOverride(std::int32_t mode, std::int32_t count);
void ChangeToMovieTask(std::int32_t movie);
void SetShipSprayFadeRate(float rate);
void InitializeDakuSpray();
void SetDakuSprayPosition(std::uint32_t index, const Vector3 &position);
void SetDakuSpraySize(std::uint32_t index, float width, float height);
void SetDakuSprayColour(std::uint32_t index, std::uint32_t colour);
std::int32_t RegisterNpcFlags(shen_name name, std::uint32_t flags, std::int32_t operation);
#if defined(SHENMUE_TARGET_S2_DC_EU)
std::int32_t GetGameLanguage();
std::int32_t GetRenderCallbackState();
#endif
} }
#endif

extern "C" {
void Event_GetNpcNewLoadState() asm("_shenmue_s2_event_GetNpcNewLoadState");
void Event_EV_NPC_NewLoadSw_0() asm("_shenmue_s2_event_EV_NPC_NewLoadSw_0");
void Event_EV_ResetFreeTalkFlags() asm("_shenmue_s2_event_EV_ResetFreeTalkFlags");
void Event_CheckWarmStartNpcRoute() asm("_shenmue_s2_event_CheckWarmStartNpcRoute");
std::int32_t __attribute__((renesas)) Event_npcLockUnlock(std::uint32_t name) asm("_shenmue_s2_event_npcLockUnlock");
void Event_IsNpcRouteLoadedOrVisible() asm("_shenmue_s2_event_IsNpcRouteLoadedOrVisible");
void Event_ForceLoadNpc() asm("_shenmue_s2_event_ForceLoadNpc");
void Event_npcFree() asm("_shenmue_s2_event_npcFree");
void Event_SetMoneyRelatedUIElement() asm("_shenmue_s2_event_SetMoneyRelatedUIElement");
void Event_SetPersistentHelpIconState() asm("_shenmue_s2_event_SetPersistentHelpIconState");
void Event_dbgPrintEventMessage() asm("_shenmue_s2_event_dbgPrintEventMessage");
void Event_dynaReadEvaluate() asm("_shenmue_s2_event_dynaReadEvaluate");
void Event_EV_ApplyTranslationClockVisibility() asm("_shenmue_s2_event_EV_ApplyTranslationClockVisibility");
void Event_SetAreaJumpPlacementFloat() asm("_shenmue_s2_event_SetAreaJumpPlacementFloat");
void Event_DebugLogToScreen() asm("_shenmue_s2_event_DebugLogToScreen");
void Event_BeginPrimarySampleExecution() asm("_shenmue_s2_event_BeginPrimarySampleExecution");
void Event_scenCopyCharaToGame() asm("_shenmue_s2_event_scenCopyCharaToGame");
void Event_scenGetCharaGameNode() asm("_shenmue_s2_event_scenGetCharaGameNode");
void Event_SetPosRotForGame() asm("_shenmue_s2_event_SetPosRotForGame");
void Event_UpdateGameNodeFlags() asm("_shenmue_s2_event_UpdateGameNodeFlags");
void Event_SetMapModelSlotTransform() asm("_shenmue_s2_event_SetMapModelSlotTransform");
void Event_GetCurrentControlInput() asm("_shenmue_s2_event_GetCurrentControlInput");
void Event_camCtrlSetRotationXYZ() asm("_shenmue_s2_event_camCtrlSetRotationXYZ");
void Event_camCtrlSetFarDistance() asm("_shenmue_s2_event_camCtrlSetFarDistance");
void Event_camCtrlSetNearDistance() asm("_shenmue_s2_event_camCtrlSetNearDistance");
void Event_camCtrlSetNpcRadius() asm("_shenmue_s2_event_camCtrlSetNpcRadius");
void Event_EVS_LoopCamera() asm("_shenmue_s2_event_EVS_LoopCamera");
void Event_sctlAuthSeekSplines() asm("_shenmue_s2_event_sctlAuthSeekSplines");
void Event_EV_SetCharaControllerFlags() asm("_shenmue_s2_event_EV_SetCharaControllerFlags");
void Event_shenFindIntersectionWithScene() asm("_shenmue_s2_event_shenFindIntersectionWithScene");
void Event_SetCharacterMapEventRegionEnabled() asm("_shenmue_s2_event_SetCharacterMapEventRegionEnabled");
void Event_IsCharacterMapEventRegionEnabled() asm("_shenmue_s2_event_IsCharacterMapEventRegionEnabled");
void Event_GetCharacterMapEventRegion() asm("_shenmue_s2_event_GetCharacterMapEventRegion");
void Event_SetMapEventCallbacks() asm("_shenmue_s2_event_SetMapEventCallbacks");
void Event_SetScenarioCharacterRadius() asm("_shenmue_s2_event_SetScenarioCharacterRadius");
void Event_SetScenarioCharacterFlag40() asm("_shenmue_s2_event_SetScenarioCharacterFlag40");
void Event_GetNpcBatchCountOverride() asm("_shenmue_s2_event_GetNpcBatchCountOverride");
void Event_ResumeParticleEffect() asm("_shenmue_s2_event_ResumeParticleEffect");
void Event_SuspendParticleEffect() asm("_shenmue_s2_event_SuspendParticleEffect");
void Event_FreeParticleEffect() asm("_shenmue_s2_event_FreeParticleEffect");
void Event_GetParticleEffectVisibility() asm("_shenmue_s2_event_GetParticleEffectVisibility");
void Event_SetParticleEffectPosition() asm("_shenmue_s2_event_SetParticleEffectPosition");
void Event_SetParticleEffectVelocity() asm("_shenmue_s2_event_SetParticleEffectVelocity");
void Event_SetParticleEffectBirthRate() asm("_shenmue_s2_event_SetParticleEffectBirthRate");
void Event_SetParticleEffectSize() asm("_shenmue_s2_event_SetParticleEffectSize");
void Event_SetParticleEffectInitialScale() asm("_shenmue_s2_event_SetParticleEffectInitialScale");
void Event_TranslateParticleEffectParticles() asm("_shenmue_s2_event_TranslateParticleEffectParticles");
void Event_SetParticleEffectColour() asm("_shenmue_s2_event_SetParticleEffectColour");
void Event_CreateFallingParticleEffect() asm("_shenmue_s2_event_CreateFallingParticleEffect");
void Event_CreateSmokeEffect() asm("_shenmue_s2_event_CreateSmokeEffect");
void Event_SetSmokeEffectFalloff() asm("_shenmue_s2_event_SetSmokeEffectFalloff");
void Event_SetSmokeEffectFadeFactor() asm("_shenmue_s2_event_SetSmokeEffectFadeFactor");
void Event_SetStationarySmokeAlpha() asm("_shenmue_s2_event_SetStationarySmokeAlpha");
void Event_SetSmokeEffectRadialMotion() asm("_shenmue_s2_event_SetSmokeEffectRadialMotion");
void Event_SetSmokeEffectSpread() asm("_shenmue_s2_event_SetSmokeEffectSpread");
void Event_SetSmokeEffectPhaseRate() asm("_shenmue_s2_event_SetSmokeEffectPhaseRate");
void Event_SetSmokeEffectFalloffCurve() asm("_shenmue_s2_event_SetSmokeEffectFalloffCurve");
void Event_SetLightSceneIntensity() asm("_shenmue_s2_event_SetLightSceneIntensity");
void Event_SetLightRange() asm("_shenmue_s2_event_SetLightRange");
void Event_SetScenarioCharacterDrawControl() asm("_shenmue_s2_event_SetScenarioCharacterDrawControl");
void Event_CheckIsArgEquZero() asm("_shenmue_s2_event_CheckIsArgEquZero");
void Event_PushGlobalLightingProgram() asm("_shenmue_s2_event_PushGlobalLightingProgram");
void Event_SetMapModelDataDefaultVertexShader() asm("_shenmue_s2_event_SetMapModelDataDefaultVertexShader");
void Event_AssignLightProgram() asm("_shenmue_s2_event_AssignLightProgram");
void Event_SetDefaultVSByIndex() asm("_shenmue_s2_event_SetDefaultVSByIndex");
void Event_SetScenarioReflectorBoardValues() asm("_shenmue_s2_event_SetScenarioReflectorBoardValues");
void Event_QueueImmediateNextFadeTask() asm("_shenmue_s2_event_QueueImmediateNextFadeTask");
void Event_EV_AT_SetPos() asm("_shenmue_s2_event_EV_AT_SetPos");
void Event_GetMTWK_NodeParent_0() asm("_shenmue_s2_event_GetMTWK_NodeParent_0");
void Event_GetCharaPosition() asm("_shenmue_s2_event_GetCharaPosition");
void Event_AttachScenarioCharacterWithTransform() asm("_shenmue_s2_event_AttachScenarioCharacterWithTransform");
void Event_AttachScenarioCharacter() asm("_shenmue_s2_event_AttachScenarioCharacter");
void Event_ResetScenarioCharacterAttachment() asm("_shenmue_s2_event_ResetScenarioCharacterAttachment");
void Event_scenSetCharaPosRotById() asm("_shenmue_s2_event_scenSetCharaPosRotById");
void Event_SetScenarioCharacterPositionAndYaw() asm("_shenmue_s2_event_SetScenarioCharacterPositionAndYaw");
void Event_EV_AT_SetAngY_0() asm("_shenmue_s2_event_EV_AT_SetAngY_0");
void Event_GetScenarioCharacterEulerAngles() asm("_shenmue_s2_event_GetScenarioCharacterEulerAngles");
void Event_AT_SetLocalBaseChara() asm("_shenmue_s2_event_AT_SetLocalBaseChara");
void Event_SCEN_FindChar_Wrapper() asm("_shenmue_s2_event_SCEN_FindChar_Wrapper");
void Event_scenApplyPosOrRotToChildId() asm("_shenmue_s2_event_scenApplyPosOrRotToChildId");
std::int32_t __attribute__((renesas)) Event_EV_authSceneCtrl(std::int32_t operation) asm("_shenmue_s2_event_EV_authSceneCtrl");
void Event_AuthorPrintMessage() asm("_shenmue_s2_event_AuthorPrintMessage");
void Event_ST_FadeInSubTitle() asm("_shenmue_s2_event_ST_FadeInSubTitle");
void Event_EV_LoadAudio() asm("_shenmue_s2_event_EV_LoadAudio");
void Event_Loop3DSoundEffect() asm("_shenmue_s2_event_Loop3DSoundEffect");
void Event_Queue_ST_LoadTitleFadeIn() asm("_shenmue_s2_event_Queue_ST_LoadTitleFadeIn");
void Event_SetupControlSpeakAndUI() asm("_shenmue_s2_event_SetupControlSpeakAndUI");
void Event_GetNumBet() asm("_shenmue_s2_event_GetNumBet");
void Event_SetMoneyRelatedUIElementAlternative() asm("_shenmue_s2_event_SetMoneyRelatedUIElementAlternative");
void Event_SetRenderClockAndCompassTo() asm("_shenmue_s2_event_SetRenderClockAndCompassTo");
void Event_EV_EventSetUpHelpIcon_0() asm("_shenmue_s2_event_EV_EventSetUpHelpIcon_0");
void Event_vfSetMoveInstructionConfiguration() asm("_shenmue_s2_event_vfSetMoveInstructionConfiguration");
void Event_EV_stepSetFlags() asm("_shenmue_s2_event_EV_stepSetFlags");
void Event_CreateEventCharacter() asm("_shenmue_s2_event_CreateEventCharacter");
void Event_IsScenarioCharacterDisplayEnabled() asm("_shenmue_s2_event_IsScenarioCharacterDisplayEnabled");
void Event_EV_EnableCharacterDisplay() asm("_shenmue_s2_event_EV_EnableCharacterDisplay");
void Event_EV_DisableCharacterDisplay() asm("_shenmue_s2_event_EV_DisableCharacterDisplay");
void Event_SetScenarioCharacterKillMode() asm("_shenmue_s2_event_SetScenarioCharacterKillMode");
void Event_EV_ToggleCharacterFlag() asm("_shenmue_s2_event_EV_ToggleCharacterFlag");
void Event_EV_EnsureCharacterDraw() asm("_shenmue_s2_event_EV_EnsureCharacterDraw");
void Event_GetScenarioCharacter() asm("_shenmue_s2_event_GetScenarioCharacter");
void Event_DoesScenarioCharacterExist() asm("_shenmue_s2_event_DoesScenarioCharacterExist");
void Event_EV_SetCharacterAlphaEnabled() asm("_shenmue_s2_event_EV_SetCharacterAlphaEnabled");
void Event_SetScenarioCharacterTransparency() asm("_shenmue_s2_event_SetScenarioCharacterTransparency");
void Event_mtwkSetMotionworkScale3() asm("_shenmue_s2_event_mtwkSetMotionworkScale3");
void Event_EV_SetCharacterImageSelection() asm("_shenmue_s2_event_EV_SetCharacterImageSelection");
void Event_shenStartFGST() asm("_shenmue_s2_event_shenStartFGST");
void Event_shenInitFightData() asm("_shenmue_s2_event_shenInitFightData");
void Event_StartFightDefeat() asm("_shenmue_s2_event_StartFightDefeat");
void Event_vfmQueueFightStart() asm("_shenmue_s2_event_vfmQueueFightStart");
void Event_IsFightActive() asm("_shenmue_s2_event_IsFightActive");
void Event_GetBtlStatus() asm("_shenmue_s2_event_GetBtlStatus");
void Event_vfCopyPosFromEntity() asm("_shenmue_s2_event_vfCopyPosFromEntity");
void Event_j_vfUpdateFlagsFor() asm("_shenmue_s2_event_j_vfUpdateFlagsFor");
void Event_GetFightRuntime() asm("_shenmue_s2_event_GetFightRuntime");
void Event_btlGetCharaFlag() asm("_shenmue_s2_event_btlGetCharaFlag");
void Event_GetFightFrame() asm("_shenmue_s2_event_GetFightFrame");
void Event_GetFightStateFlags() asm("_shenmue_s2_event_GetFightStateFlags");
void Event_check_dead_status() asm("_shenmue_s2_event_check_dead_status");
void Event_ctrlGetCharaByName() asm("_shenmue_s2_event_ctrlGetCharaByName");
void Event_TurnScenarioCharacterTowardPosition() asm("_shenmue_s2_event_TurnScenarioCharacterTowardPosition");
void Event_scenMotionworkUtil() asm("_shenmue_s2_event_scenMotionworkUtil");
void Event_scenFindMotionwork() asm("_shenmue_s2_event_scenFindMotionwork");
void Event_SetCharaTargetTransformOffset() asm("_shenmue_s2_event_SetCharaTargetTransformOffset");
void Event_ReadMotiFromDirectory() asm("_shenmue_s2_event_ReadMotiFromDirectory");
void Event_DeleteMotion() asm("_shenmue_s2_event_DeleteMotion");
void Event_SetMotionLayerFrameRange() asm("_shenmue_s2_event_SetMotionLayerFrameRange");
void Event_SetMotionLayerLoopStartTime() asm("_shenmue_s2_event_SetMotionLayerLoopStartTime");
void Event_SetMotionRegionEvalTime() asm("_shenmue_s2_event_SetMotionRegionEvalTime");
void Event_SetMotionLayerEndFrame() asm("_shenmue_s2_event_SetMotionLayerEndFrame");
void Event_mtwkSetSpeed() asm("_shenmue_s2_event_mtwkSetSpeed");
void Event_CopyMotionTransition() asm("_shenmue_s2_event_CopyMotionTransition");
void Event_RequestMotionTargetFacing() asm("_shenmue_s2_event_RequestMotionTargetFacing");
void Event_ConfigureScenarioNodeMotion() asm("_shenmue_s2_event_ConfigureScenarioNodeMotion");
void Event_figpAssignModeFor() asm("_shenmue_s2_event_figpAssignModeFor");
void Event_figpSetFlagsBlendForBlur() asm("_shenmue_s2_event_figpSetFlagsBlendForBlur");
void Event_FindScenarioModelNode() asm("_shenmue_s2_event_FindScenarioModelNode");
void Event_figpSetFlagModesFor() asm("_shenmue_s2_event_figpSetFlagModesFor");
void Event_AllocateVOBJ() asm("_shenmue_s2_event_AllocateVOBJ");
void Event_vfStageDefinitionSetMotion() asm("_shenmue_s2_event_vfStageDefinitionSetMotion");
void Event_vfStageDefinitionAddHitbox() asm("_shenmue_s2_event_vfStageDefinitionAddHitbox");
void Event_ToggleVOBJArrayIndex() asm("_shenmue_s2_event_ToggleVOBJArrayIndex");
void Event_vfStageDefinitionSetHitResponse() asm("_shenmue_s2_event_vfStageDefinitionSetHitResponse");
void Event_vfStageDefinitionSetAttachment() asm("_shenmue_s2_event_vfStageDefinitionSetAttachment");
void Event_vfStageDefinitionAddContactMask() asm("_shenmue_s2_event_vfStageDefinitionAddContactMask");
void Event_vfAllocateVOBJ() asm("_shenmue_s2_event_vfAllocateVOBJ");
void Event_vfStageObjectSetBrokenModel() asm("_shenmue_s2_event_vfStageObjectSetBrokenModel");
void Event_vfStageObjectAssignPosXYZ() asm("_shenmue_s2_event_vfStageObjectAssignPosXYZ");
void Event_vfStageObjectAssignRotXYZ() asm("_shenmue_s2_event_vfStageObjectAssignRotXYZ");
void Event_vfStageObjectAssignMotFlags() asm("_shenmue_s2_event_vfStageObjectAssignMotFlags");
void Event_vfStageObjectAssignMotFlagsAlternative() asm("_shenmue_s2_event_vfStageObjectAssignMotFlagsAlternative");
void Event_vfInitializeStageObjectHitboxes() asm("_shenmue_s2_event_vfInitializeStageObjectHitboxes");
void Event_vfStageObjectReset() asm("_shenmue_s2_event_vfStageObjectReset");
void Event_vfStageObjectAssignVelocityXYZ() asm("_shenmue_s2_event_vfStageObjectAssignVelocityXYZ");
void Event_SetShowQteFlagForButtonType() asm("_shenmue_s2_event_SetShowQteFlagForButtonType");
void Event_GetCurrentQteStatus() asm("_shenmue_s2_event_GetCurrentQteStatus");
void Event_EV_PlayDialogue() asm("_shenmue_s2_event_EV_PlayDialogue");
void Event_EV_ControlPackageResources() asm("_shenmue_s2_event_EV_ControlPackageResources");
void Event_BeginSecondarySampleExecution() asm("_shenmue_s2_event_BeginSecondarySampleExecution");
void Event_ExecuteCurrAreaMainFunc() asm("_shenmue_s2_event_ExecuteCurrAreaMainFunc");
void Event_ExecuteCurrAreaMainFunc_Wrapper() asm("_shenmue_s2_event_ExecuteCurrAreaMainFunc_Wrapper");
void Event_GetRandomNumberInRange() asm("_shenmue_s2_event_GetRandomNumberInRange");
void Event_InverseFixedAtan2() asm("_shenmue_s2_event_InverseFixedAtan2");
void Event_RemoveInventoryItem() asm("_shenmue_s2_event_RemoveInventoryItem");
void Event_GiveItemOf() asm("_shenmue_s2_event_GiveItemOf");
void Event_EV_HLKillOtherHierarchyTask() asm("_shenmue_s2_event_EV_HLKillOtherHierarchyTask");
void Event_ComputeAngle() asm("_shenmue_s2_event_ComputeAngle");
void Event_vfmDispatchControl() asm("_shenmue_s2_event_vfmDispatchControl");
void Event_GetCurrentBgm() asm("_shenmue_s2_event_GetCurrentBgm");
void Event_RestoreFightStageObject() asm("_shenmue_s2_event_RestoreFightStageObject");
void Event_shenGetCurrentSbt() asm("_shenmue_s2_event_shenGetCurrentSbt");
void Event_GetSelectedSoundSceneIndex() asm("_shenmue_s2_event_GetSelectedSoundSceneIndex");
void Event_SetFightStateFlags() asm("_shenmue_s2_event_SetFightStateFlags");
void Event_SetFightFrame() asm("_shenmue_s2_event_SetFightFrame");
void Event_FreeWAIR() asm("_shenmue_s2_event_FreeWAIR");
void Event_RefreshWeatherForCurrentTime() asm("_shenmue_s2_event_RefreshWeatherForCurrentTime");
void Event_GetFightMotionRuntime() asm("_shenmue_s2_event_GetFightMotionRuntime");
void Event_SetOccupancyForThisFieldSegment() asm("_shenmue_s2_event_SetOccupancyForThisFieldSegment");
void Event_SetMotionBoneTarget() asm("_shenmue_s2_event_SetMotionBoneTarget");
void Event_LatchMotionBoneTarget() asm("_shenmue_s2_event_LatchMotionBoneTarget");
void Event_SetSleepActionIcon() asm("_shenmue_s2_event_SetSleepActionIcon");
void Event_GetSleepActionIconResult() asm("_shenmue_s2_event_GetSleepActionIconResult");
void Event_ApplyCharacterHandPoseCommand() asm("_shenmue_s2_event_ApplyCharacterHandPoseCommand");
void Event_SetHighModelXYZ() asm("_shenmue_s2_event_SetHighModelXYZ");
void Event_imgmGetPosXYZ() asm("_shenmue_s2_event_imgmGetPosXYZ");
void Event_imgmSetHandPos() asm("_shenmue_s2_event_imgmSetHandPos");
void Event_ResetScenarioRegisteredCallbacks() asm("_shenmue_s2_event_ResetScenarioRegisteredCallbacks");
void Event_hndlSetFlags() asm("_shenmue_s2_event_hndlSetFlags");
void Event_getSpecialBonePos() asm("_shenmue_s2_event_getSpecialBonePos");
void Event_SetFogRed() asm("_shenmue_s2_event_SetFogRed");
void Event_GetFogRed() asm("_shenmue_s2_event_GetFogRed");
void Event_SetFogGreen() asm("_shenmue_s2_event_SetFogGreen");
void Event_GetFogGreen() asm("_shenmue_s2_event_GetFogGreen");
void Event_SetFogBlue() asm("_shenmue_s2_event_SetFogBlue");
void Event_GetFogBlue() asm("_shenmue_s2_event_GetFogBlue");
void Event_GetFogColour() asm("_shenmue_s2_event_GetFogColour");
void Event_GetFogEnabled() asm("_shenmue_s2_event_GetFogEnabled");
void Event_LoadFogDefinition() asm("_shenmue_s2_event_LoadFogDefinition");
void Event_DrawPIN() asm("_shenmue_s2_event_DrawPIN");
void Event_Matrix4x4_ApplyRotationAroundTransform_0() asm("_shenmue_s2_event_Matrix4x4_ApplyRotationAroundTransform_0");
void Event_FindChunkInCurrentMapInfo() asm("_shenmue_s2_event_FindChunkInCurrentMapInfo");
void Event_EV_wthrApplyRainGroups() asm("_shenmue_s2_event_EV_wthrApplyRainGroups");
void Event_LoadGauge() asm("_shenmue_s2_event_LoadGauge");
void Event_SetFightGaugeVisible() asm("_shenmue_s2_event_SetFightGaugeVisible");
void Event_UpdateDualFightGauge() asm("_shenmue_s2_event_UpdateDualFightGauge");
void Event_SetWeatherInstruction() asm("_shenmue_s2_event_SetWeatherInstruction");
void Event_AllocateVCDS_6() asm("_shenmue_s2_event_AllocateVCDS_6");
void Event_AllocateVCDS_9() asm("_shenmue_s2_event_AllocateVCDS_9");
void Event_AllocateVCDS_8() asm("_shenmue_s2_event_AllocateVCDS_8");
void Event_AllocateVCDS_4() asm("_shenmue_s2_event_AllocateVCDS_4");
void Event_AllocateVCDS_5() asm("_shenmue_s2_event_AllocateVCDS_5");
void Event_AllocateVCDS_10() asm("_shenmue_s2_event_AllocateVCDS_10");
void Event_AllocateVCDS_14() asm("_shenmue_s2_event_AllocateVCDS_14");
void Event_AllocateVCDS_16() asm("_shenmue_s2_event_AllocateVCDS_16");
void Event_AllocateVCDS_3() asm("_shenmue_s2_event_AllocateVCDS_3");
void Event_FreeCameraDialogueNode() asm("_shenmue_s2_event_FreeCameraDialogueNode");
void Event_SetFlagForTaskData() asm("_shenmue_s2_event_SetFlagForTaskData");
void Event_vfDisableCameraDialogue() asm("_shenmue_s2_event_vfDisableCameraDialogue");
void Event_ResetCameraDialogueNode() asm("_shenmue_s2_event_ResetCameraDialogueNode");
void Event_shenUpdatePlayerDataFlags() asm("_shenmue_s2_event_shenUpdatePlayerDataFlags");
void Event_AdvanceScenarioExecution() asm("_shenmue_s2_event_AdvanceScenarioExecution");
void Event_SetCameraDialogueTransition() asm("_shenmue_s2_event_SetCameraDialogueTransition");
void Event_AllocateVCDS_11() asm("_shenmue_s2_event_AllocateVCDS_11");
void Event_AllocateVCDS_12() asm("_shenmue_s2_event_AllocateVCDS_12");
void Event_AllocateVCDS_2() asm("_shenmue_s2_event_AllocateVCDS_2");
void Event_GetCameraDialogueState() asm("_shenmue_s2_event_GetCameraDialogueState");
void Event_GetCameraDialogueFlags() asm("_shenmue_s2_event_GetCameraDialogueFlags");
void Event_SetQteButtonState() asm("_shenmue_s2_event_SetQteButtonState");
void Event_SampleNativeMotionRoot() asm("_shenmue_s2_event_SampleNativeMotionRoot");
void Event_camCtrlSetMotionScaleFactor() asm("_shenmue_s2_event_camCtrlSetMotionScaleFactor");
void Event_PrepareNpcAllFromArray() asm("_shenmue_s2_event_PrepareNpcAllFromArray");
void Event_FindMapInfoChunkSize() asm("_shenmue_s2_event_FindMapInfoChunkSize");
void Event_LoadCurrentPakBinary() asm("_shenmue_s2_event_LoadCurrentPakBinary");
void Event_GetCurrentPakBinarySize() asm("_shenmue_s2_event_GetCurrentPakBinarySize");
void Event_EV_SetupImageModule() asm("_shenmue_s2_event_EV_SetupImageModule");
void Event_SetHighModelAndFaceFlagsForChara() asm("_shenmue_s2_event_SetHighModelAndFaceFlagsForChara");
void Event_SetFacialMouthExpression() asm("_shenmue_s2_event_SetFacialMouthExpression");
void Event_IsFacialReplacementSelected() asm("_shenmue_s2_event_IsFacialReplacementSelected");
void Event_SelectCharacterFacialNode() asm("_shenmue_s2_event_SelectCharacterFacialNode");
void Event_FindScenarioCharacterType() asm("_shenmue_s2_event_FindScenarioCharacterType");
void Event_SetScenarioCharacterType() asm("_shenmue_s2_event_SetScenarioCharacterType");
void Event_LoadBlurModelForCharacter_0() asm("_shenmue_s2_event_LoadBlurModelForCharacter_0");
void Event_OpenPAWNShop() asm("_shenmue_s2_event_OpenPAWNShop");
void Event_IsPAWNShopOpen() asm("_shenmue_s2_event_IsPAWNShopOpen");
void Event_ClosePAWNShop() asm("_shenmue_s2_event_ClosePAWNShop");
void Event_EV_vfBattleUtil() asm("_shenmue_s2_event_EV_vfBattleUtil");
void Event_MoveObjectToPosFromThisScene() asm("_shenmue_s2_event_MoveObjectToPosFromThisScene");
void Event_AllocateMaterialSoundEffect() asm("_shenmue_s2_event_AllocateMaterialSoundEffect");
void Event_ExecuteAicaCommand_Wrapper() asm("_shenmue_s2_event_ExecuteAicaCommand_Wrapper");
void Event_StopMaterialSoundEffect() asm("_shenmue_s2_event_StopMaterialSoundEffect");
void Event_UpdateMaterialSoundEffectPosition() asm("_shenmue_s2_event_UpdateMaterialSoundEffectPosition");
void Event_GenerateFFT() asm("_shenmue_s2_event_GenerateFFT");
void Event_fft_fill_dist() asm("_shenmue_s2_event_fft_fill_dist");
void Event_fft_bins_load_pairwise_tables() asm("_shenmue_s2_event_fft_bins_load_pairwise_tables");
void Event_fft_fill_krn() asm("_shenmue_s2_event_fft_fill_krn");
void Event_FFTUtil() asm("_shenmue_s2_event_FFTUtil");
void Event_set_lighting_flags() asm("_shenmue_s2_event_set_lighting_flags");
void Event_BuildLightingTableFrom() asm("_shenmue_s2_event_BuildLightingTableFrom");
void Event_CalculateFieldCollision() asm("_shenmue_s2_event_CalculateFieldCollision");
void Event_ApplyFFTWaveformToStripTree() asm("_shenmue_s2_event_ApplyFFTWaveformToStripTree");
void Event_ApplyFFTWaveformForModelChunk() asm("_shenmue_s2_event_ApplyFFTWaveformForModelChunk");
void Event_GetInterpolatedWeatherValues() asm("_shenmue_s2_event_GetInterpolatedWeatherValues");
void Event_AllocateVCDS_13() asm("_shenmue_s2_event_AllocateVCDS_13");
void Event_AllocateVCDS_15() asm("_shenmue_s2_event_AllocateVCDS_15");
void Event_AssignNextTaskInQueueByDepth() asm("_shenmue_s2_event_AssignNextTaskInQueueByDepth");
void Event_SetMotionBoneTargetStepLimit() asm("_shenmue_s2_event_SetMotionBoneTargetStepLimit");
void Event_qteBattleUtil() asm("_shenmue_s2_event_qteBattleUtil");
void Event_Shenmue_Execute3DSoundCommand() asm("_shenmue_s2_event_Shenmue_Execute3DSoundCommand");
void Event_PlayFootstepForNode() asm("_shenmue_s2_event_PlayFootstepForNode");
void Event_FindMaterialSoundBank() asm("_shenmue_s2_event_FindMaterialSoundBank");
void Event_ExecuteAicaBankCommand() asm("_shenmue_s2_event_ExecuteAicaBankCommand");
void Event_qronSetFloorDescriptor() asm("_shenmue_s2_event_qronSetFloorDescriptor");
void Event_UpdateWeatherForMinute() asm("_shenmue_s2_event_UpdateWeatherForMinute");
void Event_AT_SetEyeCtrl() asm("_shenmue_s2_event_AT_SetEyeCtrl");
void Event_EV_WeatherEffectUtil_Impl() asm("_shenmue_s2_event_EV_WeatherEffectUtil_Impl");
void Event_Weather_FreeActiveFIXC() asm("_shenmue_s2_event_Weather_FreeActiveFIXC");
void Event_SetIsSkySetup() asm("_shenmue_s2_event_SetIsSkySetup");
void Event_ReadSkyboxTexture() asm("_shenmue_s2_event_ReadSkyboxTexture");
void Event_AllocateVCDS() asm("_shenmue_s2_event_AllocateVCDS");
void Event_AllocateVCDS_17() asm("_shenmue_s2_event_AllocateVCDS_17");
void Event_AllocateVCDS_7() asm("_shenmue_s2_event_AllocateVCDS_7");
void Event_ConfigureFieldCollisionForModel() asm("_shenmue_s2_event_ConfigureFieldCollisionForModel");
void Event_TransformFieldCollisionForModel() asm("_shenmue_s2_event_TransformFieldCollisionForModel");
void Event_SetCameraDialogueNodeType() asm("_shenmue_s2_event_SetCameraDialogueNodeType");
void Event_EV_SetMapLightingPlacementEnabled() asm("_shenmue_s2_event_EV_SetMapLightingPlacementEnabled");
void Event_SetPropSlotTransform() asm("_shenmue_s2_event_SetPropSlotTransform");
void Event_RaycastInWorldGrid() asm("_shenmue_s2_event_RaycastInWorldGrid");
void Event_ComputeNormalizedAngleRatio() asm("_shenmue_s2_event_ComputeNormalizedAngleRatio");
void Event_FindNearestRayIntersection() asm("_shenmue_s2_event_FindNearestRayIntersection");
void Event_npcGetFlagFor() asm("_shenmue_s2_event_npcGetFlagFor");
void Event_SetNpcFlag() asm("_shenmue_s2_event_SetNpcFlag");
void Event_StartNpcScriptAreaRoute() asm("_shenmue_s2_event_StartNpcScriptAreaRoute");
void Event_SetNpcMotionScale() asm("_shenmue_s2_event_SetNpcMotionScale");
void Event_ClaimDoor() asm("_shenmue_s2_event_ClaimDoor");
void Event_ReleaseDoor() asm("_shenmue_s2_event_ReleaseDoor");
void Event_GetNpcDoorAnimation() asm("_shenmue_s2_event_GetNpcDoorAnimation");
void Event_ComputeDoorIngress() asm("_shenmue_s2_event_ComputeDoorIngress");
void Event_ComputeDoorIngressAlternative() asm("_shenmue_s2_event_ComputeDoorIngressAlternative");
void Event_SetDoorCallback() asm("_shenmue_s2_event_SetDoorCallback");
void Event_GetDoorCallback() asm("_shenmue_s2_event_GetDoorCallback");
void Event_AssignAnimatedFlag() asm("_shenmue_s2_event_AssignAnimatedFlag");
void Event_UpdateTempHourMinTimeScale() asm("_shenmue_s2_event_UpdateTempHourMinTimeScale");
void Event_AssignLightParams() asm("_shenmue_s2_event_AssignLightParams");
void Event_SetNpcMotionTransition() asm("_shenmue_s2_event_SetNpcMotionTransition");
void Event_npcSampleRouteNodeWindow() asm("_shenmue_s2_event_npcSampleRouteNodeWindow");
void Event_GetControllerEventWork() asm("_shenmue_s2_event_GetControllerEventWork");
void Event_FindFieldGroundHeight() asm("_shenmue_s2_event_FindFieldGroundHeight");
void Event_GetCurrentWeather() asm("_shenmue_s2_event_GetCurrentWeather");
void Event_InitializeNpcMovement() asm("_shenmue_s2_event_InitializeNpcMovement");
void Event_LoadLegacyFiles() asm("_shenmue_s2_event_LoadLegacyFiles");
void Event_LoadLegacySceneFileWithoutSize() asm("_shenmue_s2_event_LoadLegacySceneFileWithoutSize");
void Event_NpcWait() asm("_shenmue_s2_event_NpcWait");
void Event_npcResetDateTimeIfNeeded() asm("_shenmue_s2_event_npcResetDateTimeIfNeeded");
void Event_npcAdvanceTime() asm("_shenmue_s2_event_npcAdvanceTime");
void Event_ActivatePartyCompanion() asm("_shenmue_s2_event_ActivatePartyCompanion");
std::int32_t __attribute__((renesas)) Event_ptycCharaUnlock(std::uint32_t name) asm("_shenmue_s2_event_ptycCharaUnlock");
void Event_DeactivatePartyCompanion() asm("_shenmue_s2_event_DeactivatePartyCompanion");
void Event_PTYCUtils() asm("_shenmue_s2_event_PTYCUtils");
void Event_EV_spmBeginSampleExecute() asm("_shenmue_s2_event_EV_spmBeginSampleExecute");
void Event_spmFadeOut() asm("_shenmue_s2_event_spmFadeOut");
void Event_GetQronNextStream() asm("_shenmue_s2_event_GetQronNextStream");
void Event_GetQronTemporaryFloorHeight() asm("_shenmue_s2_event_GetQronTemporaryFloorHeight");
void Event_qronSetFloor() asm("_shenmue_s2_event_qronSetFloor");
void Event_qronRepositionPlayerForFloor() asm("_shenmue_s2_event_qronRepositionPlayerForFloor");
void Event_npcUpdateFlags() asm("_shenmue_s2_event_npcUpdateFlags");
void Event_UnknownCharaFunc() asm("_shenmue_s2_event_UnknownCharaFunc");
void Event_IsNpcAnimationActive() asm("_shenmue_s2_event_IsNpcAnimationActive");
void Event_npcStreamOrLoadHuman() asm("_shenmue_s2_event_npcStreamOrLoadHuman");
void Event_CloseMoveLearning() asm("_shenmue_s2_event_CloseMoveLearning");
void Event_vfAddGeneratorPoint() asm("_shenmue_s2_event_vfAddGeneratorPoint");
void Event_CreateSplashEffect() asm("_shenmue_s2_event_CreateSplashEffect");
void Event_ToggleFlagOnFieldEntity() asm("_shenmue_s2_event_ToggleFlagOnFieldEntity");
void Event_SetDisplayFlagForProp() asm("_shenmue_s2_event_SetDisplayFlagForProp");
void Event_HasPropCharacterZeroUpperSignature() asm("_shenmue_s2_event_HasPropCharacterZeroUpperSignature");
void Event_TestScriptCameraSplineCollision() asm("_shenmue_s2_event_TestScriptCameraSplineCollision");
void Event_ToggleStreetFighterUI() asm("_shenmue_s2_event_ToggleStreetFighterUI");
void Event_GetCharaUnknownStatus() asm("_shenmue_s2_event_GetCharaUnknownStatus");
void Event_camCtrlSetTargetChara() asm("_shenmue_s2_event_camCtrlSetTargetChara");
void Event_doorPlayRigidMotion() asm("_shenmue_s2_event_doorPlayRigidMotion");
void Event_SetFireFalloffScalar() asm("_shenmue_s2_event_SetFireFalloffScalar");
void Event_SetFirePalette() asm("_shenmue_s2_event_SetFirePalette");
void Event_SetFireFalloff() asm("_shenmue_s2_event_SetFireFalloff");
void Event_SetMotionIkTargetPosition() asm("_shenmue_s2_event_SetMotionIkTargetPosition");
void Event_SetNewWindPhaseAndSpeed() asm("_shenmue_s2_event_SetNewWindPhaseAndSpeed");
void Event_SyncWindWithWDATWeatherState() asm("_shenmue_s2_event_SyncWindWithWDATWeatherState");
void Event_Matrix4x4_ScaleXYZ() asm("_shenmue_s2_event_Matrix4x4_ScaleXYZ");
void Event_GetCurrentElevatorFloor() asm("_shenmue_s2_event_GetCurrentElevatorFloor");
void Event_GetAnimatedObjectState() asm("_shenmue_s2_event_GetAnimatedObjectState");
void Event_GetElevatorTransform() asm("_shenmue_s2_event_GetElevatorTransform");
void Event_SetAnimatedObjectDestination() asm("_shenmue_s2_event_SetAnimatedObjectDestination");
void Event_SetAnimatedObjectMotionRange() asm("_shenmue_s2_event_SetAnimatedObjectMotionRange");
void Event_FinishAnimatedObjectTransition() asm("_shenmue_s2_event_FinishAnimatedObjectTransition");
void Event_CancelAnimatedObjectTransition() asm("_shenmue_s2_event_CancelAnimatedObjectTransition");
void Event_SetScenarioCharacterAnimation() asm("_shenmue_s2_event_SetScenarioCharacterAnimation");
void Event_IsScenarioCharacterAnimationPaused() asm("_shenmue_s2_event_IsScenarioCharacterAnimationPaused");
void Event_SetScenarioCharacterAnimationFlags() asm("_shenmue_s2_event_SetScenarioCharacterAnimationFlags");
void Event_SetScenarioCharacterAnimationSpeed() asm("_shenmue_s2_event_SetScenarioCharacterAnimationSpeed");
void Event_GetScenarioCharacterAnimationFrame() asm("_shenmue_s2_event_GetScenarioCharacterAnimationFrame");
void Event_mazeInit() asm("_shenmue_s2_event_mazeInit");
void Event_ApplyMapModelTransforms() asm("_shenmue_s2_event_ApplyMapModelTransforms");
void Event_SetScenarioCharacterAnimationFrame() asm("_shenmue_s2_event_SetScenarioCharacterAnimationFrame");
void Event_GetLeftOrRight() asm("_shenmue_s2_event_GetLeftOrRight");
void Event_GetConversationPaused() asm("_shenmue_s2_event_GetConversationPaused");
void Event_npcSetAreaName() asm("_shenmue_s2_event_npcSetAreaName");
void Event_SetAreaCleanupCallback() asm("_shenmue_s2_event_SetAreaCleanupCallback");
void Event_drGetCurrentAuthorId() asm("_shenmue_s2_event_drGetCurrentAuthorId");
void Event_SetDoorSubjectEnabled() asm("_shenmue_s2_event_SetDoorSubjectEnabled");
void Event_MapModelSlotCacheUtil() asm("_shenmue_s2_event_MapModelSlotCacheUtil");
void Event_ExchangeLegacyModelColour() asm("_shenmue_s2_event_ExchangeLegacyModelColour");
void Event_GetStreetFightProgress() asm("_shenmue_s2_event_GetStreetFightProgress");
void Event_SetStreetFightProgress() asm("_shenmue_s2_event_SetStreetFightProgress");
void Event_AddStreetFightProgress() asm("_shenmue_s2_event_AddStreetFightProgress");
void Event_GetSetFightActorByte() asm("_shenmue_s2_event_GetSetFightActorByte");
void Event_GetSetFightActorFloat() asm("_shenmue_s2_event_GetSetFightActorFloat");
void Event_hsFadeVoice() asm("_shenmue_s2_event_hsFadeVoice");
void Event_WSCN_LoadWeatherData() asm("_shenmue_s2_event_WSCN_LoadWeatherData");
void Event_WSCN_DeleteWeatherData() asm("_shenmue_s2_event_WSCN_DeleteWeatherData");
void Event_SetMoneyRelatedUIElement_0() asm("_shenmue_s2_event_SetMoneyRelatedUIElement_0");
void Event_adfsSetCameraIdxForChara() asm("_shenmue_s2_event_adfsSetCameraIdxForChara");
void Event_ClearConversationTaskRegistration() asm("_shenmue_s2_event_ClearConversationTaskRegistration");
void Event_GetPlanetLayerColour() asm("_shenmue_s2_event_GetPlanetLayerColour");
void Event_srchSetTargetEnabled() asm("_shenmue_s2_event_srchSetTargetEnabled");
void Event_UpdateSingleFightGauge() asm("_shenmue_s2_event_UpdateSingleFightGauge");
void Event_RequestMotionPositionTransition() asm("_shenmue_s2_event_RequestMotionPositionTransition");
void Event_Queue_task_scnchg_1() asm("_shenmue_s2_event_Queue_task_scnchg_1");
void Event_QueueExtraTelop() asm("_shenmue_s2_event_QueueExtraTelop");
void Event_SetMapModelVertexShaderType() asm("_shenmue_s2_event_SetMapModelVertexShaderType");
void Event_LoadSoundProgramWithoutSelectedRecord() asm("_shenmue_s2_event_LoadSoundProgramWithoutSelectedRecord");
void Event_FindAuthor() asm("_shenmue_s2_event_FindAuthor");
void Event_plymAdvanceTime() asm("_shenmue_s2_event_plymAdvanceTime");
void Event_SetAreaFogParams() asm("_shenmue_s2_event_SetAreaFogParams");
void Event_SetSceneInfos() asm("_shenmue_s2_event_SetSceneInfos");
void Event_GetBattleDataConfig() asm("_shenmue_s2_event_GetBattleDataConfig");
void Event_GetConverseDataForChara() asm("_shenmue_s2_event_GetConverseDataForChara");
void Event_GetMazeTerrainField() asm("_shenmue_s2_event_GetMazeTerrainField");
void Event_SelectMazeTerrainField() asm("_shenmue_s2_event_SelectMazeTerrainField");
void Event_SetMazeTargetCharacter() asm("_shenmue_s2_event_SetMazeTargetCharacter");
void Event_mazeCheckInsideQuad() asm("_shenmue_s2_event_mazeCheckInsideQuad");
void Event_GetMazeUnitAuthoredName() asm("_shenmue_s2_event_GetMazeUnitAuthoredName");
void Event_GetMazeObjectPosition() asm("_shenmue_s2_event_GetMazeObjectPosition");
void Event_GetMazeObjectYaw() asm("_shenmue_s2_event_GetMazeObjectYaw");
void Event_GetMazeObjectData() asm("_shenmue_s2_event_GetMazeObjectData");
void Event_EV_mazeUtil() asm("_shenmue_s2_event_EV_mazeUtil");
void Event_clmdUtil() asm("_shenmue_s2_event_clmdUtil");
void Event_shenGetMinutesUntilNextFrameMinute() asm("_shenmue_s2_event_shenGetMinutesUntilNextFrameMinute");
void Event_EV_FBQTE_Util() asm("_shenmue_s2_event_EV_FBQTE_Util");
void Event_ryoIsCurrentScenarioFlagSatisfied() asm("_shenmue_s2_event_ryoIsCurrentScenarioFlagSatisfied");
void Event_ReadMotionPropsFromDirectory() asm("_shenmue_s2_event_ReadMotionPropsFromDirectory");
void Event_DeleteMotionProps() asm("_shenmue_s2_event_DeleteMotionProps");
void Event_srchClearTarget() asm("_shenmue_s2_event_srchClearTarget");
void Event_SetupLightConfig() asm("_shenmue_s2_event_SetupLightConfig");
void Event_SetDefaultVS() asm("_shenmue_s2_event_SetDefaultVS");
void Event_SetFightGaugeLayout() asm("_shenmue_s2_event_SetFightGaugeLayout");
void Event_SetCharacterAlpha() asm("_shenmue_s2_event_SetCharacterAlpha");
void Event_IsScenarioBusy() asm("_shenmue_s2_event_IsScenarioBusy");
void Event_ControlFightLoadSprites() asm("_shenmue_s2_event_ControlFightLoadSprites");
void Event_LoadNPCRoute() asm("_shenmue_s2_event_LoadNPCRoute");
void Event_ExecuteCleanupFuncForAreaWithCachedScene() asm("_shenmue_s2_event_ExecuteCleanupFuncForAreaWithCachedScene");
void Event_ExecuteCleanupFuncForAreaWithCachedScene_0() asm("_shenmue_s2_event_ExecuteCleanupFuncForAreaWithCachedScene_0");
void Event_QueueFightAdviceControl() asm("_shenmue_s2_event_QueueFightAdviceControl");
void Event_srchGetTargetName() asm("_shenmue_s2_event_srchGetTargetName");
void Event_srchReset() asm("_shenmue_s2_event_srchReset");
void Event_mapSetupLighting() asm("_shenmue_s2_event_mapSetupLighting");
void Event_EV_SetCinemaBarVisibility() asm("_shenmue_s2_event_EV_SetCinemaBarVisibility");
void Event_UpdateItemStateFlags() asm("_shenmue_s2_event_UpdateItemStateFlags");
void Event_RequestTitleMenu() asm("_shenmue_s2_event_RequestTitleMenu");
void Event_btlSetCharaFlagBatch() asm("_shenmue_s2_event_btlSetCharaFlagBatch");
void Event_SetAnimatedObjectState() asm("_shenmue_s2_event_SetAnimatedObjectState");
void Event_GetMazeObjectCount() asm("_shenmue_s2_event_GetMazeObjectCount");
void Event_FindMazeObjectNearCharacter() asm("_shenmue_s2_event_FindMazeObjectNearCharacter");
void Event_FindMazeObjectNearPosition() asm("_shenmue_s2_event_FindMazeObjectNearPosition");
void Event_GetMazeObjectTransform() asm("_shenmue_s2_event_GetMazeObjectTransform");
void Event_GetMazeObjectIndices() asm("_shenmue_s2_event_GetMazeObjectIndices");
void Event_spmFadeInBank() asm("_shenmue_s2_event_spmFadeInBank");
void Event_spmFadeOutBank() asm("_shenmue_s2_event_spmFadeOutBank");
void Event_Author_SetPlacementMatrix() asm("_shenmue_s2_event_Author_SetPlacementMatrix");
void Event_CopyScenarioNodeMotionTimes() asm("_shenmue_s2_event_CopyScenarioNodeMotionTimes");
void Event_SetHelpIconState() asm("_shenmue_s2_event_SetHelpIconState");
void Event_CheckIfCharaIsInRadiusOf() asm("_shenmue_s2_event_CheckIfCharaIsInRadiusOf");
void Event_EV_CheckIfCharaIsInRadiusOf_0() asm("_shenmue_s2_event_EV_CheckIfCharaIsInRadiusOf_0");
void Event_doorLoopRigidMotion() asm("_shenmue_s2_event_doorLoopRigidMotion");
void Event_GetFightModeFlags() asm("_shenmue_s2_event_GetFightModeFlags");
void Event_SetFightModeFlags() asm("_shenmue_s2_event_SetFightModeFlags");
void Event_SetFireAlpha() asm("_shenmue_s2_event_SetFireAlpha");
void Event_AttachFireEffect() asm("_shenmue_s2_event_AttachFireEffect");
void Event_SetFirePosition() asm("_shenmue_s2_event_SetFirePosition");
void Event_SetFireInstanceAlpha() asm("_shenmue_s2_event_SetFireInstanceAlpha");
void Event_AttachFireInstance() asm("_shenmue_s2_event_AttachFireInstance");
void Event_SetFireInstanceSize() asm("_shenmue_s2_event_SetFireInstanceSize");
void Event_cvsFindVoiceDataFor() asm("_shenmue_s2_event_cvsFindVoiceDataFor");
void Event_CountNpcLiveStreams() asm("_shenmue_s2_event_CountNpcLiveStreams");
void Event_Shenmue_ApplyTranslationRotationScaleXYZ_ToCharaByID() asm("_shenmue_s2_event_Shenmue_ApplyTranslationRotationScaleXYZ_ToCharaByID");
void Event_shenSetFreeQuestInactive() asm("_shenmue_s2_event_shenSetFreeQuestInactive");
void Event_SetAnimatedObjectFlags() asm("_shenmue_s2_event_SetAnimatedObjectFlags");
void Event_SetElevatorDisplayMode() asm("_shenmue_s2_event_SetElevatorDisplayMode");
void Event_shenGetSleepScheduleFlag() asm("_shenmue_s2_event_shenGetSleepScheduleFlag");
void Event_UnknownScenarioCharacterAttachmentA() asm("_shenmue_s2_event_UnknownScenarioCharacterAttachmentA");
void Event_AnimateMoneyAmount() asm("_shenmue_s2_event_AnimateMoneyAmount");
void Event_IsWithinRadiusFromVector3PolyIntersectTest() asm("_shenmue_s2_event_IsWithinRadiusFromVector3PolyIntersectTest");
void __attribute__((renesas)) Event_MagicRoom_Enqueue_1(
    const char *directory, const char *archive, const shenmue::s2::MagicRoomFloorGroup *groups, std::int32_t seed) asm("_shenmue_s2_event_MagicRoom_Enqueue_1");
void Event_shenUpdatePlanetByLayer() asm("_shenmue_s2_event_shenUpdatePlanetByLayer");
void Event_SetPropVector3() asm("_shenmue_s2_event_SetPropVector3");
void Event_InitiateCleanupForSceneChara() asm("_shenmue_s2_event_InitiateCleanupForSceneChara");
void Event_ApplyCharacterGeometryLighting() asm("_shenmue_s2_event_ApplyCharacterGeometryLighting");
void Event_ApplyCharacterVertexLighting() asm("_shenmue_s2_event_ApplyCharacterVertexLighting");
void Event_mazeSetTerrainDrawEnabled() asm("_shenmue_s2_event_mazeSetTerrainDrawEnabled");
void Event_TelCode_Enqueue() asm("_shenmue_s2_event_TelCode_Enqueue");
void Event_EV_GetScreenFilmIsNotShowing() asm("_shenmue_s2_event_EV_GetScreenFilmIsNotShowing");
void Event_UpdateTelephoneState() asm("_shenmue_s2_event_UpdateTelephoneState");
void Event_SetWeatherLightManager() asm("_shenmue_s2_event_SetWeatherLightManager");
void Event_srchSetLookMode() asm("_shenmue_s2_event_srchSetLookMode");
void Event_SetMazeTimeParameters() asm("_shenmue_s2_event_SetMazeTimeParameters");
void Event_e0527_EVT_Auth_UpdateGameDateTime() asm("_shenmue_s2_event_e0527_EVT_Auth_UpdateGameDateTime");
void Event_SetHandRegionXYZWithFlags() asm("_shenmue_s2_event_SetHandRegionXYZWithFlags");
void Event_RequestScenarioPlayerInterpolation() asm("_shenmue_s2_event_RequestScenarioPlayerInterpolation");
void Event_npcSetCycleTimeLimitExpiry() asm("_shenmue_s2_event_npcSetCycleTimeLimitExpiry");
void Event_mazeFindIndexForCurrentDistanceForMazeFile() asm("_shenmue_s2_event_mazeFindIndexForCurrentDistanceForMazeFile");
void Event_ActivateDefaultDoor() asm("_shenmue_s2_event_ActivateDefaultDoor");
void Event_PrepareNpcRouteReload() asm("_shenmue_s2_event_PrepareNpcRouteReload");
void Event_IsNpcRouteReloadPrepared() asm("_shenmue_s2_event_IsNpcRouteReloadPrepared");
void Event_ReleaseNpcForRouteReload() asm("_shenmue_s2_event_ReleaseNpcForRouteReload");
void Event_ReleaseNpcForRouteReloadAndStoreCount() asm("_shenmue_s2_event_ReleaseNpcForRouteReloadAndStoreCount");
void Event_SmokeTextureParticles() asm("_shenmue_s2_event_SmokeTextureParticles");
void Event_ReleaseSmokeTextureResources() asm("_shenmue_s2_event_ReleaseSmokeTextureResources");
void Event_SetSmokePosition() asm("_shenmue_s2_event_SetSmokePosition");
void Event_SetSmokeParameter() asm("_shenmue_s2_event_SetSmokeParameter");
void Event_SetNpcRouteUpdatePaused() asm("_shenmue_s2_event_SetNpcRouteUpdatePaused");
void Event_GetMagicRoomDebugState() asm("_shenmue_s2_event_GetMagicRoomDebugState");
void Event_GetNpcAnimationState() asm("_shenmue_s2_event_GetNpcAnimationState");
void Event_srchRequestLockonExit() asm("_shenmue_s2_event_srchRequestLockonExit");
void Event_SetMagicRoomDebugState() asm("_shenmue_s2_event_SetMagicRoomDebugState");
void Event_AdjustScreenAspectRatio() asm("_shenmue_s2_event_AdjustScreenAspectRatio");
void Event_CreateCenteredWindowTextSurface() asm("_shenmue_s2_event_CreateCenteredWindowTextSurface");
void Event_SetPlanetLayerOffset() asm("_shenmue_s2_event_SetPlanetLayerOffset");
void Event_ResetPlanetLayerOffset() asm("_shenmue_s2_event_ResetPlanetLayerOffset");
void Event_ControlMazeFish() asm("_shenmue_s2_event_ControlMazeFish");
void Event_SetMazeFish() asm("_shenmue_s2_event_SetMazeFish");
extern std::byte Event_UnknownReservedEventData asm("_shenmue_s2_event_UnknownReservedEventData");
void Event_SetMazeUnitDrawDistance() asm("_shenmue_s2_event_SetMazeUnitDrawDistance");
}

extern "C" std::int32_t __attribute__((renesas)) Event_npcLockUnlock(std::uint32_t name) {
    return ::shenmue::s2::EV_npcLockUnlock(name);
}

extern "C" std::int32_t __attribute__((renesas)) Event_EV_authSceneCtrl(std::int32_t operation) {
    return ::shenmue::s2::EV_authSceneCtrl(operation);
}

extern "C" std::int32_t __attribute__((renesas)) Event_ptycCharaUnlock(std::uint32_t name) {
    return ::shenmue::s2::ptycCharaUnlock(name);
}

namespace shenmue {
namespace s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void EV_SetCharacterMirrorPlane(const CharacterMirrorPlane *plane, const Vector3i *rotation);
#endif

std::uintptr_t g_eventFunctionTable[EVENT_FUNCTION_COUNT] __attribute__((section(".data.dc_event_functions"))) = {
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetSavedMinigameScore),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetSavedMinigameScore),
    reinterpret_cast<std::uintptr_t>(&HLib::HasActiveTaskCallback),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetTimeRelated),
    reinterpret_cast<std::uintptr_t>(&Event_GetNpcNewLoadState),
    reinterpret_cast<std::uintptr_t>(&Event_EV_NPC_NewLoadSw_0),
    reinterpret_cast<std::uintptr_t>(&Event_EV_ResetFreeTalkFlags),
    reinterpret_cast<std::uintptr_t>(&Event_CheckWarmStartNpcRoute),
    reinterpret_cast<std::uintptr_t>(&Event_npcLockUnlock),
    reinterpret_cast<std::uintptr_t>(&Event_IsNpcRouteLoadedOrVisible),
    reinterpret_cast<std::uintptr_t>(&Event_ForceLoadNpc),
    reinterpret_cast<std::uintptr_t>(&Event_npcFree),
    reinterpret_cast<std::uintptr_t>(&Event_SetMoneyRelatedUIElement),
    reinterpret_cast<std::uintptr_t>(&Event_SetPersistentHelpIconState),
    reinterpret_cast<std::uintptr_t>(&FinishLoading),
    reinterpret_cast<std::uintptr_t>(&SetLoadingBusy),
    reinterpret_cast<std::uintptr_t>(&filmCheckFadeTrackColour),
    reinterpret_cast<std::uintptr_t>(&SetScreenFilmFade),
    reinterpret_cast<std::uintptr_t>(&Event_dbgPrintEventMessage),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ctrlStopSubtitles),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_scenQueueLoad),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::scenSetTaskFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::dynaReadEvaluate),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetPlayerStats),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetPlayerStat),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetMapControlStatusFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenGetCurrentDateTime),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetCurrentDateTimeTo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenQueueChangeModuleAndScene),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::HasSavedAreaJumpLocation),
    reinterpret_cast<std::uintptr_t>(&Event_EV_ApplyTranslationClockVisibility),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_SetCanDoAreaJumpTo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetAreaJumpPlacementByte),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetAreaJumpPlacementByte),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetAreaJumpPlacementFloat),
    reinterpret_cast<std::uintptr_t>(&Event_SetAreaJumpPlacementFloat),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventAssertNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventAssertMessageNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::DebugLogToFileRemoved),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetDebugScreenTextPosition),
    reinterpret_cast<std::uintptr_t>(&Event_DebugLogToScreen),
    reinterpret_cast<std::uintptr_t>(&Event_BeginPrimarySampleExecution),
    reinterpret_cast<std::uintptr_t>(&Event_scenCopyCharaToGame),
    reinterpret_cast<std::uintptr_t>(&Event_scenGetCharaGameNode),
    reinterpret_cast<std::uintptr_t>(&Event_SetPosRotForGame),
    reinterpret_cast<std::uintptr_t>(&Event_UpdateGameNodeFlags),
    reinterpret_cast<std::uintptr_t>(&SetMinigameSeed),
    reinterpret_cast<std::uintptr_t>(&IncrementMinigameRandFloat),
    reinterpret_cast<std::uintptr_t>(&Event_SetMapModelSlotTransform),
    reinterpret_cast<std::uintptr_t>(&Event_GetCurrentControlInput),
    reinterpret_cast<std::uintptr_t>(&EVT_GetControllerInput),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetFovTransitionTo),
    reinterpret_cast<std::uintptr_t>(&Event_camCtrlSetRotationXYZ),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CameraControlNullsub),
    reinterpret_cast<std::uintptr_t>(&Event_camCtrlSetFarDistance),
    reinterpret_cast<std::uintptr_t>(&Event_camCtrlSetNearDistance),
    reinterpret_cast<std::uintptr_t>(&Event_camCtrlSetNpcRadius),
    reinterpret_cast<std::uintptr_t>(&camCtrlGetFarDistance),
    reinterpret_cast<std::uintptr_t>(&camCtrlGetNearDistance),
    reinterpret_cast<std::uintptr_t>(&camCtrlGetNpcRadius),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EVS_LoopCamera),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlCheckIsEventCam),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::spliGetCurrentFrmCnt),
    reinterpret_cast<std::uintptr_t>(&Event_sctlAuthSeekSplines),
    reinterpret_cast<std::uintptr_t>(&Event_EV_SetCharaControllerFlags),
    reinterpret_cast<std::uintptr_t>(&Event_shenFindIntersectionWithScene),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetPlayerControlEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_setHoldStatesFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetPlayerGameFlags),
    reinterpret_cast<std::uintptr_t>(&Event_SetCharacterMapEventRegionEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_IsCharacterMapEventRegionEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_GetCharacterMapEventRegion),
    reinterpret_cast<std::uintptr_t>(&Event_SetMapEventCallbacks),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterRadius),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterFlag40),
    reinterpret_cast<std::uintptr_t>(&Event_GetNpcBatchCountOverride),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetMapModelCommandRoutingEnabled),
    reinterpret_cast<std::uintptr_t>(&GetMapModelCommandRoutingEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_ResumeParticleEffect),
    reinterpret_cast<std::uintptr_t>(&Event_SuspendParticleEffect),
    reinterpret_cast<std::uintptr_t>(&Event_FreeParticleEffect),
    reinterpret_cast<std::uintptr_t>(&Event_GetParticleEffectVisibility),
    reinterpret_cast<std::uintptr_t>(&Event_SetParticleEffectPosition),
    reinterpret_cast<std::uintptr_t>(&Event_SetParticleEffectVelocity),
    reinterpret_cast<std::uintptr_t>(&Event_SetParticleEffectBirthRate),
    reinterpret_cast<std::uintptr_t>(&Event_SetParticleEffectSize),
    reinterpret_cast<std::uintptr_t>(&Event_SetParticleEffectInitialScale),
    reinterpret_cast<std::uintptr_t>(&Event_TranslateParticleEffectParticles),
    reinterpret_cast<std::uintptr_t>(&Event_SetParticleEffectColour),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CreateFallingParticleEffect),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CreateSmokeEffect),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokeEffectFalloff),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokeEffectFadeFactor),
    reinterpret_cast<std::uintptr_t>(&Event_SetStationarySmokeAlpha),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokeEffectRadialMotion),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokeEffectSpread),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokeEffectPhaseRate),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokeEffectFalloffCurve),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ResetLightEntry),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReinitializeLights),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::LGHT_Parser_Internal),
    reinterpret_cast<std::uintptr_t>(&UpdateGlobalLighting),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetLightEntryIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLightEntryIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLightFlagForIndex),
    reinterpret_cast<std::uintptr_t>(&SetLightRuntimeColour),
    reinterpret_cast<std::uintptr_t>(&Event_SetLightSceneIntensity),
    reinterpret_cast<std::uintptr_t>(&Event_SetLightRange),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterDrawControl),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLightFalloff),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLightRuntimePosition),
    reinterpret_cast<std::uintptr_t>(&SetLightSunDirection),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLightConeAngle),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLightGeometryEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetRankedLightLimit),
    reinterpret_cast<std::uintptr_t>(&Event_CheckIsArgEquZero),
    reinterpret_cast<std::uintptr_t>(&Event_PushGlobalLightingProgram),
    reinterpret_cast<std::uintptr_t>(&Event_SetMapModelDataDefaultVertexShader),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReplaceScenarioImageModule),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AssignImageModuleParameters),
    reinterpret_cast<std::uintptr_t>(&Event_AssignLightProgram),
    reinterpret_cast<std::uintptr_t>(&Event_SetDefaultVSByIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetReflectorBoardDefaultValues),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioReflectorBoardValues),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetScenarioReflectorBoardMode),
    reinterpret_cast<std::uintptr_t>(&Event_QueueImmediateNextFadeTask),
    reinterpret_cast<std::uintptr_t>(&Event_EV_AT_SetPos),
    reinterpret_cast<std::uintptr_t>(&Event_GetMTWK_NodeParent_0),
    reinterpret_cast<std::uintptr_t>(&Event_GetCharaPosition),
    reinterpret_cast<std::uintptr_t>(&Event_AttachScenarioCharacterWithTransform),
    reinterpret_cast<std::uintptr_t>(&Event_AttachScenarioCharacter),
    reinterpret_cast<std::uintptr_t>(&Event_ResetScenarioCharacterAttachment),
    reinterpret_cast<std::uintptr_t>(&Event_scenSetCharaPosRotById),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterPositionAndYaw),
    reinterpret_cast<std::uintptr_t>(&Event_EV_AT_SetAngY_0),
    reinterpret_cast<std::uintptr_t>(&Event_GetScenarioCharacterEulerAngles),
    reinterpret_cast<std::uintptr_t>(&Event_AT_SetLocalBaseChara),
    reinterpret_cast<std::uintptr_t>(&Event_SCEN_FindChar_Wrapper),
    reinterpret_cast<std::uintptr_t>(&Event_scenApplyPosOrRotToChildId),
    reinterpret_cast<std::uintptr_t>(&Event_EV_authSceneCtrl),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetAuthorTableFile),
    reinterpret_cast<std::uintptr_t>(&Queue_ST_WaitSubTitle),
    reinterpret_cast<std::uintptr_t>(&Event_ST_FadeInSubTitle),
    reinterpret_cast<std::uintptr_t>(&ST_FadeOutSubTitle),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::sndLoadSceneMap),
    reinterpret_cast<std::uintptr_t>(&Event_EV_LoadAudio),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ProcessAicaCommand),
    reinterpret_cast<std::uintptr_t>(&Event_Loop3DSoundEffect),
    reinterpret_cast<std::uintptr_t>(&Event_Queue_ST_LoadTitleFadeIn),
    reinterpret_cast<std::uintptr_t>(&ST_LoadTitleFade),
    reinterpret_cast<std::uintptr_t>(&SetupControlSpeakAndUI),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenGetCtrlSpeakIconFlag),
    reinterpret_cast<std::uintptr_t>(&Event_GetNumBet),
    reinterpret_cast<std::uintptr_t>(&Event_SetMoneyRelatedUIElementAlternative),
    reinterpret_cast<std::uintptr_t>(&GetShouldRenderIcons),
    reinterpret_cast<std::uintptr_t>(&Event_SetRenderClockAndCompassTo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_EventSetUpHelpIcon_0),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CheckHelpIcon),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfSetMoveInstructionConfiguration),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_GetShenmueFlag),
    reinterpret_cast<std::uintptr_t>(&Event_EV_stepSetFlags),
    reinterpret_cast<std::uintptr_t>(&CreateEventCharacter),
    reinterpret_cast<std::uintptr_t>(&Event_IsScenarioCharacterDisplayEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_EV_EnableCharacterDisplay),
    reinterpret_cast<std::uintptr_t>(&Event_EV_DisableCharacterDisplay),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterKillMode),
    reinterpret_cast<std::uintptr_t>(&Event_EV_ToggleCharacterFlag),
    reinterpret_cast<std::uintptr_t>(&Event_EV_EnsureCharacterDraw),
    reinterpret_cast<std::uintptr_t>(&Event_GetScenarioCharacter),
    reinterpret_cast<std::uintptr_t>(&Event_DoesScenarioCharacterExist),
    reinterpret_cast<std::uintptr_t>(&Event_EV_SetCharacterAlphaEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterTransparency),
    reinterpret_cast<std::uintptr_t>(&Event_mtwkSetMotionworkScale3),
    reinterpret_cast<std::uintptr_t>(&Event_EV_SetCharacterImageSelection),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenStartFGST),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenInitFightData),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::PublishFightRespawn),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfmQueueFightStart),
    reinterpret_cast<std::uintptr_t>(&Event_IsFightActive),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetBtlStatus),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfCopyPosFromEntity),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfUpdateFlagsFor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetFightRuntime),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::btlGetCharaFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfGetElapsedSeconds),
    reinterpret_cast<std::uintptr_t>(&Event_GetFightFrame),
    reinterpret_cast<std::uintptr_t>(&Event_GetFightStateFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::check_dead_status),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetActiveFightModeFlag),
    reinterpret_cast<std::uintptr_t>(&Event_ctrlGetCharaByName),
    reinterpret_cast<std::uintptr_t>(&Event_TurnScenarioCharacterTowardPosition),
    reinterpret_cast<std::uintptr_t>(&Event_scenMotionworkUtil),
    reinterpret_cast<std::uintptr_t>(&Event_scenFindMotionwork),
    reinterpret_cast<std::uintptr_t>(&SetCharaTargetTransformOffset),
    reinterpret_cast<std::uintptr_t>(&mtwkSetEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkIsEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_ReadMotiFromDirectory),
    reinterpret_cast<std::uintptr_t>(&Event_DeleteMotion),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetRigBoneId),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetAnimationId),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMotionFrameCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkRequestMotion),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetTrackedRootMode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetTransitionFramesRemaining),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkCopyRegionTimes),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetSplitFrame),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetFrame),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetEndFrame),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetSpeed),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetRequestedRootMode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetFrameTime),
    reinterpret_cast<std::uintptr_t>(&Event_SetMotionLayerFrameRange),
    reinterpret_cast<std::uintptr_t>(&Event_SetMotionLayerLoopStartTime),
    reinterpret_cast<std::uintptr_t>(&Event_SetMotionRegionEvalTime),
    reinterpret_cast<std::uintptr_t>(&Event_SetMotionLayerEndFrame),
    reinterpret_cast<std::uintptr_t>(&Event_mtwkSetSpeed),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetParentBoneWrapper),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetExtraFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetRegionFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkCopyTransition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkUpdateWithPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkRequestTargetFacing),
    reinterpret_cast<std::uintptr_t>(&Event_ConfigureScenarioNodeMotion),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::IsScenarioNodeMotionFinished),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::StopScenarioNodeMotion),
    reinterpret_cast<std::uintptr_t>(&Event_figpAssignModeFor),
    reinterpret_cast<std::uintptr_t>(&Event_figpSetFlagsBlendForBlur),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::figpLoadBlurModelFor),
    reinterpret_cast<std::uintptr_t>(&Event_figpSetFlagModesFor),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVOBJ),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageDefinitionSetMotion),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageDefinitionAddHitbox),
    reinterpret_cast<std::uintptr_t>(&Event_ToggleVOBJArrayIndex),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageDefinitionSetHitResponse),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageDefinitionSetAttachment),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageDefinitionAddContactMask),
    reinterpret_cast<std::uintptr_t>(&Event_vfAllocateVOBJ),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageObjectSetBrokenModel),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageObjectAssignPosXYZ),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageObjectAssignRotXYZ),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageObjectAssignMotFlags),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageObjectAssignMotFlagsAlternative),
    reinterpret_cast<std::uintptr_t>(&Event_vfInitializeStageObjectHitboxes),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageObjectReset),
    reinterpret_cast<std::uintptr_t>(&Event_vfStageObjectAssignVelocityXYZ),
    reinterpret_cast<std::uintptr_t>(&Event_SetShowQteFlagForButtonType),
    reinterpret_cast<std::uintptr_t>(&Event_GetCurrentQteStatus),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_FreeAuthorPak),
    reinterpret_cast<std::uintptr_t>(&Event_EV_PlayDialogue),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::cvsHasValidConverseData),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::scenCvsCtrlFree),
    reinterpret_cast<std::uintptr_t>(&Event_EV_ControlPackageResources),
    reinterpret_cast<std::uintptr_t>(&Event_BeginSecondarySampleExecution),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_GetPlayerFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_SetPlayerFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::LoadAreaFunc_Wrapper_0),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::scenFreeEvent),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::scenFreeEventByIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetAreaSlotName),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenGetAreaSlotByName),
    reinterpret_cast<std::uintptr_t>(&Event_ExecuteCurrAreaMainFunc),
    reinterpret_cast<std::uintptr_t>(&Event_ExecuteCurrAreaMainFunc_Wrapper),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetSquaredDistanceXZ),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetSquaredEuclideanDistance),
    reinterpret_cast<std::uintptr_t>(&CalculatePlanarDistanceBetweenPoints),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetEuclideanDistance),
    reinterpret_cast<std::uintptr_t>(&Event_GetRandomNumberInRange),
    reinterpret_cast<std::uintptr_t>(&shenGetRandomInt32),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::scenApplyTransRot),
    reinterpret_cast<std::uintptr_t>(&Event_InverseFixedAtan2),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ryoNotBusy),
    reinterpret_cast<std::uintptr_t>(&ProcessDebugInputNullsub),
    reinterpret_cast<std::uintptr_t>(&AddInventoryItem),
    reinterpret_cast<std::uintptr_t>(&Event_RemoveInventoryItem),
    reinterpret_cast<std::uintptr_t>(&GetItemState),
    reinterpret_cast<std::uintptr_t>(&Event_GiveItemOf),
    reinterpret_cast<std::uintptr_t>(&HLib::EnqueueTaskWithoutParameter),
    reinterpret_cast<std::uintptr_t>(&HLib::EnqueueTaskWithoutParameterWrapper),
    reinterpret_cast<std::uintptr_t>(&HLib::QueueTaskAfterCurrentTask),
    reinterpret_cast<std::uintptr_t>(&HLib::DestroyCurrentTask),
    reinterpret_cast<std::uintptr_t>(&HLib::GetTaskParameter),
    reinterpret_cast<std::uintptr_t>(&HLib::DestroyTaskInternal),
    reinterpret_cast<std::uintptr_t>(&HLib::SetCurrentTaskTo),
    reinterpret_cast<std::uintptr_t>(&HLib::SetupTaskDelay),
    reinterpret_cast<std::uintptr_t>(&HLib::EnqueueTaskWithParameter),
    reinterpret_cast<std::uintptr_t>(&HLib::PauseTaskRangeExecution),
    reinterpret_cast<std::uintptr_t>(&HLib::UnpauseTaskEnqueue),
    reinterpret_cast<std::uintptr_t>(&HLib::SetupTimedTask),
    reinterpret_cast<std::uintptr_t>(&HLib::ResolveDeferredCallback),
    reinterpret_cast<std::uintptr_t>(&HLib::DestroyCurrentEventTask),
    reinterpret_cast<std::uintptr_t>(&HLib::DestroyTask),
    reinterpret_cast<std::uintptr_t>(&HLib::GetCurrentTask),
    reinterpret_cast<std::uintptr_t>(&HLib::AssignCallbackForTask),
    reinterpret_cast<std::uintptr_t>(&HLib::SetCurrentTaskData),
    reinterpret_cast<std::uintptr_t>(&HLib::GetCurrentParentTask),
    reinterpret_cast<std::uintptr_t>(&HLib::GetCurrentTaskData),
    reinterpret_cast<std::uintptr_t>(&HLib::FreeCurrentTaskData),
    reinterpret_cast<std::uintptr_t>(&HLib::MarkCurrentTaskForCleanup),
    reinterpret_cast<std::uintptr_t>(&Event_EV_HLKillOtherHierarchyTask),
    reinterpret_cast<std::uintptr_t>(&TestCharaPositionWithinRange),
    reinterpret_cast<std::uintptr_t>(&Event_ComputeAngle),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::vfmDispatchControl),
    reinterpret_cast<std::uintptr_t>(&Event_GetCurrentBgm),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfStageObjectRestartMotion),
    reinterpret_cast<std::uintptr_t>(&Event_shenGetCurrentSbt),
    reinterpret_cast<std::uintptr_t>(&Event_GetSelectedSoundSceneIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFightOutcomeMode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetFightResult),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SuspendFight),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::SetFightFrame),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetWeatherUpdateEnabledTo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetWeatherUpdateEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::UpdateWeather),
    reinterpret_cast<std::uintptr_t>(&Event_FreeWAIR),
    reinterpret_cast<std::uintptr_t>(&Event_RefreshWeatherForCurrentTime),
    reinterpret_cast<std::uintptr_t>(&vfmaGetTaskParam),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ExchangeMapModelEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_SetOccupancyForThisFieldSegment),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::flddSetPointFlagOnGrid),
    reinterpret_cast<std::uintptr_t>(&Event_SetMotionBoneTarget),
    reinterpret_cast<std::uintptr_t>(&Event_LatchMotionBoneTarget),
    reinterpret_cast<std::uintptr_t>(&Event_SetSleepActionIcon),
    reinterpret_cast<std::uintptr_t>(&Event_GetSleepActionIconResult),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetCharacterFaceCacheIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseCharacterFaceCache),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetCharacterFaceParameters),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetCharacterFaceSecondaryParameters),
    reinterpret_cast<std::uintptr_t>(&Event_ApplyCharacterHandPoseCommand),
    reinterpret_cast<std::uintptr_t>(&Event_SetHighModelXYZ),
    reinterpret_cast<std::uintptr_t>(&Event_imgmGetPosXYZ),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ApplyCharacterHandRotation),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLowQualityHands),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ResetCachedFacialModels),
    reinterpret_cast<std::uintptr_t>(&Event_hndlSetFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ScenarioSoundNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetupHandPoseScript),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetupHandIdlePoseCallback),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetHandShapeFor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::hmdlSetFlagsFor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::hmdlSetPoseFor),
    reinterpret_cast<std::uintptr_t>(&Event_getSpecialBonePos),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetEyeToChara),
    reinterpret_cast<std::uintptr_t>(static_cast<void (*)(std::int32_t, std::int32_t)>(&SetPlayerCashOrMoney)),
    reinterpret_cast<std::uintptr_t>(&CalculateFixedSine),
    reinterpret_cast<std::uintptr_t>(&CalculateFixedCosine),
    reinterpret_cast<std::uintptr_t>(&FixedAngleTangent),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::scenProjectToScreen),
    reinterpret_cast<std::uintptr_t>(&Event_SetFogRed),
    reinterpret_cast<std::uintptr_t>(&Event_GetFogRed),
    reinterpret_cast<std::uintptr_t>(&Event_SetFogGreen),
    reinterpret_cast<std::uintptr_t>(&Event_GetFogGreen),
    reinterpret_cast<std::uintptr_t>(&Event_SetFogBlue),
    reinterpret_cast<std::uintptr_t>(&Event_GetFogBlue),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFogDistance),
    reinterpret_cast<std::uintptr_t>(&Event_GetFogColour),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReinitFogTasks),
    reinterpret_cast<std::uintptr_t>(&Event_GetFogEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::LoadFogDefinition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlGetNextCamPos),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlGetTargetPosition),
    reinterpret_cast<std::uintptr_t>(&DrawPIN),
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_SetCharacterMirrorPlane),
#else
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetCharacterMirrorPlane),
#endif
    reinterpret_cast<std::uintptr_t>(&EV_SetMapModelAlternateTraversalEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_FindChunkInCurrentMapInfo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_wthrApplyRainGroups),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ConfigureWeatherVolume),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::Weather_LoadWCCPEntries),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseWeatherVolumeGroup),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetWeatherVolumeGroupEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetWeatherVolumeEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_LoadGauge),
    reinterpret_cast<std::uintptr_t>(&Event_SetFightGaugeVisible),
    reinterpret_cast<std::uintptr_t>(&Event_UpdateDualFightGauge),
    reinterpret_cast<std::uintptr_t>(&Event_SetWeatherInstruction),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_6),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_9),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_8),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_4),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_5),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_10),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_14),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_16),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_3),
    reinterpret_cast<std::uintptr_t>(&Event_FreeCameraDialogueNode),
    reinterpret_cast<std::uintptr_t>(&Event_SetFlagForTaskData),
    reinterpret_cast<std::uintptr_t>(&Event_vfDisableCameraDialogue),
    reinterpret_cast<std::uintptr_t>(&Event_ResetCameraDialogueNode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::Queue_FLAG_CheckTask),
    reinterpret_cast<std::uintptr_t>(&Event_shenUpdatePlayerDataFlags),
    reinterpret_cast<std::uintptr_t>(&shenUpdateFlagEvaluation),
    reinterpret_cast<std::uintptr_t>(&Event_SetCameraDialogueTransition),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_11),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_12),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_2),
    reinterpret_cast<std::uintptr_t>(&Event_GetCameraDialogueState),
    reinterpret_cast<std::uintptr_t>(&Event_GetCameraDialogueFlags),
    reinterpret_cast<std::uintptr_t>(&Event_SetQteButtonState),
    reinterpret_cast<std::uintptr_t>(&TransformScenarioDirection),
    reinterpret_cast<std::uintptr_t>(&GetVector3DotProduct),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfSetFightControlFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::OpenMemoFromNotebookMenu),
    reinterpret_cast<std::uintptr_t>(&IsItemSelectable),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlToggleSmoothing),
    reinterpret_cast<std::uintptr_t>(&shenSetTimeSimulationTo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::TranslateLightDefinition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkCopyBoneTransform),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetBonePosition),
    reinterpret_cast<std::uintptr_t>(&Event_SampleNativeMotionRoot),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SampleNativeMotionRootAndRotation),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::charaSetDisplayEnabledFlag),
    reinterpret_cast<std::uintptr_t>(&Event_camCtrlSetMotionScaleFactor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetScaleTo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetScaleToAndRecalcFov),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetFlags),
    reinterpret_cast<std::uintptr_t>(&Event_PrepareNpcAllFromArray),
    reinterpret_cast<std::uintptr_t>(&Filepath_Gen),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetSoundSequenceSceneCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetMoveLearningEntry),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetNewEwzoTypeForMoveLearned),
    reinterpret_cast<std::uintptr_t>(&Event_FindMapInfoChunkSize),
    reinterpret_cast<std::uintptr_t>(&Event_LoadCurrentPakBinary),
    reinterpret_cast<std::uintptr_t>(&Event_GetCurrentPakBinarySize),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetUseEnvFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetUnknownField),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::FieldDebugNullsub),
    reinterpret_cast<std::uintptr_t>(&Event_EV_SetupImageModule),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetHighModelAndFaceFlagsForChara),
    reinterpret_cast<std::uintptr_t>(&Event_SetFacialMouthExpression),
    reinterpret_cast<std::uintptr_t>(&Event_IsFacialReplacementSelected),
    reinterpret_cast<std::uintptr_t>(&Event_SelectCharacterFacialNode),
    reinterpret_cast<std::uintptr_t>(&SetAllMapFieldDepthSlotsEnabled),
    reinterpret_cast<std::uintptr_t>(&GetMapFieldDepthSlotEnabled),
    reinterpret_cast<std::uintptr_t>(&SetMapFieldDepthSlotEnabled),
    reinterpret_cast<std::uintptr_t>(&GetMapModelGridCullingEnabled),
    reinterpret_cast<std::uintptr_t>(&SetMapModelGridCullingEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_FindScenarioCharacterType),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterType),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::LoadBlurModelForCharacter),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::OpenPAWNShop),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::IsPAWNShopOpen),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ClosePAWNShop),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetPawnSaleResult),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CommitPawnSale),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_vfBattleUtil),
    reinterpret_cast<std::uintptr_t>(&GetMapModelReaderMask),
    reinterpret_cast<std::uintptr_t>(&HasMapModelReader),
    reinterpret_cast<std::uintptr_t>(&Event_MoveObjectToPosFromThisScene),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AllocateMaterialSoundHandle),
    reinterpret_cast<std::uintptr_t>(&Event_ExecuteAicaCommand_Wrapper),
    reinterpret_cast<std::uintptr_t>(&Event_StopMaterialSoundEffect),
    reinterpret_cast<std::uintptr_t>(&Event_UpdateMaterialSoundEffectPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::spmSetSampleEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::spmSetSecondarySampleEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GenerateFFT),
    reinterpret_cast<std::uintptr_t>(&Event_fft_fill_dist),
    reinterpret_cast<std::uintptr_t>(&Event_fft_bins_load_pairwise_tables),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GenerateDistanceFalloff),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GenerateDistanceKernel),
    reinterpret_cast<std::uintptr_t>(&Event_fft_fill_krn),
    reinterpret_cast<std::uintptr_t>(&Event_FFTUtil),
    reinterpret_cast<std::uintptr_t>(&Event_set_lighting_flags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::BuildWaveLightingTable),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::ApplyWaveformToMesh),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ApplyWaveformToNode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ApplyWaveformToModel),
    reinterpret_cast<std::uintptr_t>(&::shenmue::dc::sdk::SetMT7VertexColourType),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMapModelRoot),
    reinterpret_cast<std::uintptr_t>(&Event_GetInterpolatedWeatherValues),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_13),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_15),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::AssignFightDialogueCallback),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkGetBoneTargetStepLimit),
    reinterpret_cast<std::uintptr_t>(&Event_SetMotionBoneTargetStepLimit),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CaptureMotionSnapshot),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseMotionSnapshot),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::RestoreMotionSnapshot),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::RestoreMotionSnapshotPlacement),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenPlayAudioWithFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetSoundListenerUsesCamera),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::qteBattleUtil),
    reinterpret_cast<std::uintptr_t>(&Event_Shenmue_Execute3DSoundCommand),
    reinterpret_cast<std::uintptr_t>(&Event_PlayFootstepForNode),
    reinterpret_cast<std::uintptr_t>(&Event_FindMaterialSoundBank),
    reinterpret_cast<std::uintptr_t>(&Event_ExecuteAicaBankCommand),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::nlSetTexanimUV),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ApplyTexAnimUVs),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::RecursiveApplyTexAnimUVs),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::UpdateModelTextureCoordinates),
    reinterpret_cast<std::uintptr_t>(&Event_qronSetFloorDescriptor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMagicRoomFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AttachMapLightingPlacement),
    reinterpret_cast<std::uintptr_t>(&Event_UpdateWeatherForMinute),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFacialBlinkDelay),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFaceFlagsForChara),
    reinterpret_cast<std::uintptr_t>(&Event_AT_SetEyeCtrl),
    reinterpret_cast<std::uintptr_t>(&Event_EV_WeatherEffectUtil_Impl),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EnsureWeatherFixcRegistered),
    reinterpret_cast<std::uintptr_t>(&Event_Weather_FreeActiveFIXC),
    reinterpret_cast<std::uintptr_t>(&Event_SetIsSkySetup),
    reinterpret_cast<std::uintptr_t>(&Event_ReadSkyboxTexture),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_17),
    reinterpret_cast<std::uintptr_t>(&Event_AllocateVCDS_7),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::ApplyWaveReflectionToMesh),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::ApplyWaveReflectionToNode),
    reinterpret_cast<std::uintptr_t>(&Event_SetCameraDialogueNodeType),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlUtil),
    reinterpret_cast<std::uintptr_t>(&Event_EV_SetMapLightingPlacementEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetPropSlotTransform),
    reinterpret_cast<std::uintptr_t>(&Event_RaycastInWorldGrid),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::transformIntersectionResult),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::toggleOsagFlags),
    reinterpret_cast<std::uintptr_t>(&Vector3::NormalizeInPlace),
    reinterpret_cast<std::uintptr_t>(&Event_ComputeNormalizedAngleRatio),
    reinterpret_cast<std::uintptr_t>(&ComputeVectorAlignmentRotation),
    reinterpret_cast<std::uintptr_t>(&Event_FindNearestRayIntersection),
    reinterpret_cast<std::uintptr_t>(&Event_npcGetFlagFor),
    reinterpret_cast<std::uintptr_t>(&Event_SetNpcFlag),
    reinterpret_cast<std::uintptr_t>(&Event_StartNpcScriptAreaRoute),
    reinterpret_cast<std::uintptr_t>(&Event_SetNpcMotionScale),
    reinterpret_cast<std::uintptr_t>(&Event_ClaimDoor),
    reinterpret_cast<std::uintptr_t>(&Event_ReleaseDoor),
    reinterpret_cast<std::uintptr_t>(&Event_GetNpcDoorAnimation),
    reinterpret_cast<std::uintptr_t>(&Event_ComputeDoorIngress),
    reinterpret_cast<std::uintptr_t>(&Event_ComputeDoorIngressAlternative),
    reinterpret_cast<std::uintptr_t>(&Event_SetDoorCallback),
    reinterpret_cast<std::uintptr_t>(&Event_GetDoorCallback),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::AssignAnimatedFlag),
    reinterpret_cast<std::uintptr_t>(&Event_UpdateTempHourMinTimeScale),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_AssignLightParams),
    reinterpret_cast<std::uintptr_t>(&Event_SetNpcMotionTransition),
    reinterpret_cast<std::uintptr_t>(&Event_npcSampleRouteNodeWindow),
    reinterpret_cast<std::uintptr_t>(&Event_GetControllerEventWork),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::FindWaterSurfaceHeight),
    reinterpret_cast<std::uintptr_t>(&Event_GetCurrentWeather),
    reinterpret_cast<std::uintptr_t>(&Event_InitializeNpcMovement),
    reinterpret_cast<std::uintptr_t>(static_cast<std::int32_t (*)(const char *, const char *, void *, std::uint32_t)>(&::shenmue::s2::shenLoadFile)),
    reinterpret_cast<std::uintptr_t>(&Event_LoadLegacySceneFileWithoutSize),
    reinterpret_cast<std::uintptr_t>(&HLib::FreeTaskData),
    reinterpret_cast<std::uintptr_t>(&Event_NpcWait),
    reinterpret_cast<std::uintptr_t>(&Event_npcResetDateTimeIfNeeded),
    reinterpret_cast<std::uintptr_t>(&Event_npcAdvanceTime),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ActivatePartyCompanion),
    reinterpret_cast<std::uintptr_t>(&Event_ptycCharaUnlock),
    reinterpret_cast<std::uintptr_t>(&DeactivatePartyCompanion),
    reinterpret_cast<std::uintptr_t>(&PTYCUtils),
    reinterpret_cast<std::uintptr_t>(&Event_EV_spmBeginSampleExecute),
    reinterpret_cast<std::uintptr_t>(&Event_spmFadeOut),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::sndSaveSceneState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::sndRestoreSceneState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::Handcuffs_Attach),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::cuffsSetFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventRuntimeNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetNextPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetTargetPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetRotation),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlGetRotationXform),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetRotationCallback),
    reinterpret_cast<std::uintptr_t>(&Event_GetQronNextStream),
    reinterpret_cast<std::uintptr_t>(&Event_GetQronTemporaryFloorHeight),
    reinterpret_cast<std::uintptr_t>(&Event_qronSetFloor),
    reinterpret_cast<std::uintptr_t>(&Event_qronRepositionPlayerForFloor),
    reinterpret_cast<std::uintptr_t>(&Event_npcUpdateFlags),
    reinterpret_cast<std::uintptr_t>(&Event_UnknownCharaFunc),
    reinterpret_cast<std::uintptr_t>(&Event_IsNpcAnimationActive),
    reinterpret_cast<std::uintptr_t>(&Event_npcStreamOrLoadHuman),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseGauge),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::vfAddGeneratorPoint),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CreateSplashEffect),
    reinterpret_cast<std::uintptr_t>(&Event_ToggleFlagOnFieldEntity),
    reinterpret_cast<std::uintptr_t>(&Event_SetDisplayFlagForProp),
    reinterpret_cast<std::uintptr_t>(&Event_HasPropCharacterZeroUpperSignature),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::TestScriptCameraSplineCollision),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetUpdateCallback),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetPerspective),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlGetPerspective),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ToggleStreetFighterUI),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::StopNpcScriptRoute),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlHandleContextSwitch),
    reinterpret_cast<std::uintptr_t>(&Event_camCtrlSetTargetChara),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetSplineShouldRebuildRotTo),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::LoadHandPoseBank),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseHandPoseBank),
    reinterpret_cast<std::uintptr_t>(&Event_doorPlayRigidMotion),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlGetRot),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CreateFireEffect),
    reinterpret_cast<std::uintptr_t>(&Event_SetFireFalloffScalar),
    reinterpret_cast<std::uintptr_t>(&Event_SetFirePalette),
    reinterpret_cast<std::uintptr_t>(&Event_SetFireFalloff),
    reinterpret_cast<std::uintptr_t>(&Event_SetMotionIkTargetPosition),
    reinterpret_cast<std::uintptr_t>(&Event_SetNewWindPhaseAndSpeed),
    reinterpret_cast<std::uintptr_t>(&Event_SyncWindWithWDATWeatherState),
    reinterpret_cast<std::uintptr_t>(&matrix_stack::GetMatrix),
    reinterpret_cast<std::uintptr_t>(&matrix_stack::LoadMatrix),
    reinterpret_cast<std::uintptr_t>(&matrix_stack::GetTranslation),
    reinterpret_cast<std::uintptr_t>(&SetCurrentMatrixTranslation),
    reinterpret_cast<std::uintptr_t>(&matrix_stack::UnitCurrentMatrix),
    reinterpret_cast<std::uintptr_t>(&AddCurrentMatrix),
    reinterpret_cast<std::uintptr_t>(&SubtractCurrentMatrix),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::MultiplyWithPrePassMatrix),
    reinterpret_cast<std::uintptr_t>(&MultiplyCurrentMatrixFromRight),
    reinterpret_cast<std::uintptr_t>(&matrix_stack::InvertMatrix),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::NormalizeAndSetTransformMatrix),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::InvertRigidTransform),
    reinterpret_cast<std::uintptr_t>(&Event_Matrix4x4_ScaleXYZ),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::TransformPoint),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::Transform),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4_ApplyAngleRotationToVector),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::RotateAroundX_Wrapper),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::RotateAroundY_Wrapper),
    reinterpret_cast<std::uintptr_t>(&Matrix4x4::RotateAroundZ_Wrapper),
    reinterpret_cast<std::uintptr_t>(&matrix_stack::TranslateVector),
    reinterpret_cast<std::uintptr_t>(&RetrieveSearchFlagFromPtr),
    reinterpret_cast<std::uintptr_t>(&PopCurrentMatrix),
    reinterpret_cast<std::uintptr_t>(&matrix_stack::Matrix4x4_Push),
    reinterpret_cast<std::uintptr_t>(&Event_GetCurrentElevatorFloor),
    reinterpret_cast<std::uintptr_t>(&Event_GetAnimatedObjectState),
    reinterpret_cast<std::uintptr_t>(&Event_GetElevatorTransform),
    reinterpret_cast<std::uintptr_t>(&Event_SetAnimatedObjectDestination),
    reinterpret_cast<std::uintptr_t>(&Event_SetAnimatedObjectMotionRange),
    reinterpret_cast<std::uintptr_t>(&Event_FinishAnimatedObjectTransition),
    reinterpret_cast<std::uintptr_t>(&Event_CancelAnimatedObjectTransition),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterAnimation),
    reinterpret_cast<std::uintptr_t>(&Event_IsScenarioCharacterAnimationPaused),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterAnimationFlags),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterAnimationSpeed),
    reinterpret_cast<std::uintptr_t>(&Event_GetScenarioCharacterAnimationFrame),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mazeInit),
    reinterpret_cast<std::uintptr_t>(&shenIsGamePaused),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::UpdateFloatingSurfaceModels),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetNaviDisplayOverride),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventSystemNullsub),
    reinterpret_cast<std::uintptr_t>(&Event_SetScenarioCharacterAnimationFrame),
    reinterpret_cast<std::uintptr_t>(&Event_GetLeftOrRight),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EventSystemAlternateNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::PauseConversation),
    reinterpret_cast<std::uintptr_t>(&Event_GetConversationPaused),
    reinterpret_cast<std::uintptr_t>(&Event_npcSetAreaName),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::LcdmSelectMessage),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::LcdmInit),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::lcdmRenderButtonCommands),
    reinterpret_cast<std::uintptr_t>(&Event_SetAreaCleanupCallback),
    reinterpret_cast<std::uintptr_t>(&Event_drGetCurrentAuthorId),
    reinterpret_cast<std::uintptr_t>(&Event_SetDoorSubjectEnabled),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::MapModelSlotCacheUtil),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ExchangeLegacyModelColour),
    reinterpret_cast<std::uintptr_t>(&HLib::AllocateTaskData),
    reinterpret_cast<std::uintptr_t>(&HLib::AllocateStaticOrDynamicBlock),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ConversationResultNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ConversationNullsubA),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ConversationNullsubB),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ConversationStateNullsub),
    reinterpret_cast<std::uintptr_t>(&Event_GetStreetFightProgress),
    reinterpret_cast<std::uintptr_t>(&Event_SetStreetFightProgress),
    reinterpret_cast<std::uintptr_t>(&Event_AddStreetFightProgress),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetSetFightActorByte),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetSetFightActorFloat),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::RetireIconInputSlot),
    reinterpret_cast<std::uintptr_t>(&Event_hsFadeVoice),
    reinterpret_cast<std::uintptr_t>(&Event_WSCN_LoadWeatherData),
    reinterpret_cast<std::uintptr_t>(&Event_WSCN_DeleteWeatherData),
    reinterpret_cast<std::uintptr_t>(&Event_SetMoneyRelatedUIElement_0),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::InitializeMT7Model),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetPlanetLayerParameter),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetPlanetLayerParameter),
    reinterpret_cast<std::uintptr_t>(&Event_adfsSetCameraIdxForChara),
    reinterpret_cast<std::uintptr_t>(&Event_ClearConversationTaskRegistration),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetWeatherCloudOrSkyColour),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetCloudAndSkyColour),
    reinterpret_cast<std::uintptr_t>(&Event_GetPlanetLayerColour),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_SetPlanetLayerColours_Impl),
    reinterpret_cast<std::uintptr_t>(&Event_srchSetTargetEnabled),
    reinterpret_cast<std::uintptr_t>(&Event_UpdateSingleFightGauge),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetPawnSalePrice),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetPawnAvailableSaleModes),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkRequestPositionTransition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::stepEvaluateFlagStepRange),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::QueueQronElevatorSceneChange),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::QueueExtraTelop),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::StartScenarioFreeTalkRecord),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::StopScenarioFreeTalkRecord),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenCheckIsBusy),
    reinterpret_cast<std::uintptr_t>(&Event_SetMapModelVertexShaderType),
    reinterpret_cast<std::uintptr_t>(&Event_LoadSoundProgramWithoutSelectedRecord),
    reinterpret_cast<std::uintptr_t>(&Event_FindAuthor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::plymIsInterpolationActive),
    reinterpret_cast<std::uintptr_t>(&Event_plymAdvanceTime),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::plymGetAreaExitState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLightRootPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetLightRootPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CLMD),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetAreaFogParams),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::StartShowScreenOfId),
    reinterpret_cast<std::uintptr_t>(&Event_SetSceneInfos),
    reinterpret_cast<std::uintptr_t>(&Event_GetBattleDataConfig),
    reinterpret_cast<std::uintptr_t>(&Event_GetConverseDataForChara),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeUnitCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeRoadCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::FindCharacterNearestMazeFirstBlockRoad),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetCharacterMazeUnitIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeUnitAuthoredName),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeUnitPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeUnitYaw),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeRoadYaw),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeRoadPosition),
    reinterpret_cast<std::uintptr_t>(&Event_clmdUtil),
    reinterpret_cast<std::uintptr_t>(&Event_shenGetMinutesUntilNextFrameMinute),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EV_FBQTE_Util),
    reinterpret_cast<std::uintptr_t>(&ryoIsCurrentScenarioFlagSatisfied),
    reinterpret_cast<std::uintptr_t>(&Event_ReadMotionPropsFromDirectory),
    reinterpret_cast<std::uintptr_t>(&Event_DeleteMotionProps),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::srchSetTarget),
    reinterpret_cast<std::uintptr_t>(&Event_srchClearTarget),
    reinterpret_cast<std::uintptr_t>(&Event_SetupLightConfig),
    reinterpret_cast<std::uintptr_t>(&Event_SetDefaultVS),
    reinterpret_cast<std::uintptr_t>(&Event_SetFightGaugeLayout),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetCharacterAlpha),
    reinterpret_cast<std::uintptr_t>(&Event_SetCharacterAlpha),
    reinterpret_cast<std::uintptr_t>(&Event_IsScenarioBusy),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ControlFightLoadSprites),
    reinterpret_cast<std::uintptr_t>(&Event_LoadNPCRoute),
    reinterpret_cast<std::uintptr_t>(&Event_ExecuteCleanupFuncForAreaWithCachedScene),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ExecuteDoorAreaCleanup),
    reinterpret_cast<std::uintptr_t>(&Event_QueueFightAdviceControl),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetPawnSaleTransition),
    reinterpret_cast<std::uintptr_t>(&Event_srchGetTargetName),
    reinterpret_cast<std::uintptr_t>(&Event_srchReset),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mapSetupLighting),
    reinterpret_cast<std::uintptr_t>(&Event_EV_SetCinemaBarVisibility),
    reinterpret_cast<std::uintptr_t>(&Event_UpdateItemStateFlags),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::RequestTitleMenu),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ConsumeItemMenuRequest),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetScenarioFollowCharacter),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFightActorParameterBytes),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::EnterDoorSubscene),
    reinterpret_cast<std::uintptr_t>(&Event_SetAnimatedObjectState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeMarkerCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::FindCharacterMazeMarkerWithinRadius),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::FindMazeMarkerWithinRadius),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mazeGetMarkerPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMazeMarkerRouteCoordinates),
    reinterpret_cast<std::uintptr_t>(&Event_spmFadeInBank),
    reinterpret_cast<std::uintptr_t>(&Event_spmFadeOutBank),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetAuthorCameraPlacement),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::plymIsSearchBlocked),
    reinterpret_cast<std::uintptr_t>(&Event_CopyScenarioNodeMotionTimes),
    reinterpret_cast<std::uintptr_t>(&Event_SetHelpIconState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::IsCharacterWithinMazeMarkerRadius),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::IsPositionWithinMazeMarkerRadius),
    reinterpret_cast<std::uintptr_t>(&Event_doorLoopRigidMotion),
    reinterpret_cast<std::uintptr_t>(&Event_GetFightModeFlags),
    reinterpret_cast<std::uintptr_t>(&Event_SetFightModeFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetOsageMode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::LeaveDoorSubscene),
    reinterpret_cast<std::uintptr_t>(&Event_SetFireAlpha),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetClothFlag),
    reinterpret_cast<std::uintptr_t>(&Event_AttachFireEffect),
    reinterpret_cast<std::uintptr_t>(&Event_SetFirePosition),
    reinterpret_cast<std::uintptr_t>(&Event_SetFireInstanceAlpha),
    reinterpret_cast<std::uintptr_t>(&Event_AttachFireInstance),
    reinterpret_cast<std::uintptr_t>(&Event_SetFireInstanceSize),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetNormalTriggerMapping),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetS2ControlMode),
    reinterpret_cast<std::uintptr_t>(&Event_cvsFindVoiceDataFor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenToggleAreaFlags),
    reinterpret_cast<std::uintptr_t>(&Event_CountNpcLiveStreams),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CreateLegacyHudText),
    reinterpret_cast<std::uintptr_t>(&ApplyTranslationRotationScaleXYZToCharaByID),
    reinterpret_cast<std::uintptr_t>(&Event_shenSetFreeQuestInactive),
    reinterpret_cast<std::uintptr_t>(&Event_SetAnimatedObjectFlags),
    reinterpret_cast<std::uintptr_t>(&Event_SetElevatorDisplayMode),
    reinterpret_cast<std::uintptr_t>(&Event_shenGetSleepScheduleFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ScenarioConversationResultNullsub),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CancelSelectorMemo),
    reinterpret_cast<std::uintptr_t>(&DrawScenarioModelChild),
    reinterpret_cast<std::uintptr_t>(&Event_AnimateMoneyAmount),
    reinterpret_cast<std::uintptr_t>(&flddPushIntersectionFlag),
    reinterpret_cast<std::uintptr_t>(&Event_IsWithinRadiusFromVector3PolyIntersectTest),
    reinterpret_cast<std::uintptr_t>(&TransformAgainstIntersectionResult),
    reinterpret_cast<std::uintptr_t>(&Event_MagicRoom_Enqueue_1),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetPluralControlFor),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::camCtrlSetNextCamRot),
    reinterpret_cast<std::uintptr_t>(&Event_shenUpdatePlanetByLayer),
    reinterpret_cast<std::uintptr_t>(&Event_SetPropVector3),
    reinterpret_cast<std::uintptr_t>(&InitiateCleanupForSceneChara),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CopyLightRuntimeEntry),
    reinterpret_cast<std::uintptr_t>(&Event_ApplyCharacterGeometryLighting),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ResetCharacterGeometryLighting),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetMazeTerrainDrawEnabled),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::EnqueueTelephoneCord),
    reinterpret_cast<std::uintptr_t>(&Event_EV_GetScreenFilmIsNotShowing),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::CleanupTelephoneCord),
    reinterpret_cast<std::uintptr_t>(&Event_SetWeatherLightManager),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetSunDirectionVector),
    reinterpret_cast<std::uintptr_t>(&Event_srchSetLookMode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mtwkSetReactFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ResetMazeHourAnimation),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetMazeHourAnimationFrame),
    reinterpret_cast<std::uintptr_t>(&Event_SetHandRegionXYZWithFlags),
    reinterpret_cast<std::uintptr_t>(&Event_RequestScenarioPlayerInterpolation),
    reinterpret_cast<std::uintptr_t>(&Event_npcSetCycleTimeLimitExpiry),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::FindNearestMazeRoad),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::emuSetNextState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::shenIsCurfewSceneChange),
    reinterpret_cast<std::uintptr_t>(&Event_ActivateDefaultDoor),
    reinterpret_cast<std::uintptr_t>(&Event_PrepareNpcRouteReload),
    reinterpret_cast<std::uintptr_t>(&Event_IsNpcRouteReloadPrepared),
    reinterpret_cast<std::uintptr_t>(&Event_ReleaseNpcForRouteReload),
    reinterpret_cast<std::uintptr_t>(&Event_ReleaseNpcForRouteReloadAndStoreCount),
    reinterpret_cast<std::uintptr_t>(&Event_SmokeTextureParticles),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseDropParticles),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokePosition),
    reinterpret_cast<std::uintptr_t>(&Event_SetSmokeParameter),
    reinterpret_cast<std::uintptr_t>(&Event_SetNpcRouteUpdatePaused),
    reinterpret_cast<std::uintptr_t>(&Event_GetMagicRoomDebugState),
    reinterpret_cast<std::uintptr_t>(&shenmue::s2::navPlaceMarker),
    reinterpret_cast<std::uintptr_t>(&Event_srchRequestLockonExit),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::CheckMediatedDoublePress),
    reinterpret_cast<std::uintptr_t>(&Event_SetMagicRoomDebugState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ComputeSecondsSince1980),
    reinterpret_cast<std::uintptr_t>(&IncrementDateTimeBySeconds),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ResetWeatherOverrideState),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::TransformScenarioMotionBonePose),
    reinterpret_cast<std::uintptr_t>(static_cast<std::int32_t (*)(const TextureChunk *, std::int32_t *)>(&::shenmue::s2::LoadTextureChunks)),
    reinterpret_cast<std::uintptr_t>(&Event_AdjustScreenAspectRatio),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::DrawLegacySpritePacket),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFixoAlphaPropagation),
    reinterpret_cast<std::uintptr_t>(&Event_CreateCenteredWindowTextSurface),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetupUIElement),
    reinterpret_cast<std::uintptr_t>(&GetParentMenu),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::UpdateSelectedItemStateFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::IsModeBitActive),
    reinterpret_cast<std::uintptr_t>(&Event_SetPlanetLayerOffset),
    reinterpret_cast<std::uintptr_t>(&Event_ResetPlanetLayerOffset),
    reinterpret_cast<std::uintptr_t>(&ReleaseNamedTexture),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::InitializeMazeFishPath),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::DrawMazeFish),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::omakeSelection),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetMGOTOSIDAMA),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMGOTOSIDAMA),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetMazeUnitDrawDistance),
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetOmakeVideoCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetOmakeVideoCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AddLightPriority),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::InitializeMazeNavigationTrail),
    reinterpret_cast<std::uintptr_t>(&flddPushIntersectionFlag),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::Queue_OP_StaffRoll),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetNpcRouteYaw),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetNpcColdResetMode),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetLoadingPhaseFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetLoadingPhaseFlags),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::InitializeShipSpray),
    reinterpret_cast<std::uintptr_t>(static_cast<void (*)(const Vector3 &)>(&::shenmue::s2::AssignShipSprayPosition)),
    reinterpret_cast<std::uintptr_t>(static_cast<void (*)(const Vector3 *)>(&::shenmue::s2::AssignShipSprayLocalPosition)),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AssignShipSprayYaw),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::HIKI_Enqueue),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AssignHikiPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AssignHikiLocalPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AssignHikiRotation),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AssignHikiYaw),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ResetAllNpcRouteStates),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseNpcScriptRouteOwner),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::srchHasActiveLockonTarget),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mazeSetCurrentRefCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::mazeGetCurrentRefCount),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::npcStartPlayEntityAtPos),
    reinterpret_cast<std::uintptr_t>(static_cast<void (*)(std::int32_t, std::int32_t, std::int32_t)>(&::shenmue::s2::SetPlayerCashOrMoney)),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetChildObjectFlagByIndex),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::AssignMapFacialClip),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMainIconInputAdmitted),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ReleaseLoadTitleFade),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::FindOSAG),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::StopNpcFollowingMotionForEvent),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFireCameraOffset),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetFireInstanceAlpha),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetMagicRoomState3),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetNewNpcBatchCountOverride),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::ChangeToMovieTask),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetShipSprayFadeRate),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::InitializeDakuSpray),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetDakuSprayPosition),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetDakuSpraySize),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::SetDakuSprayColour),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::RegisterNpcFlags),
#if defined(SHENMUE_TARGET_S2_DC_EU)
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetGameLanguage),
    reinterpret_cast<std::uintptr_t>(&::shenmue::s2::GetRenderCallbackState),
#endif
#endif
};

} // namespace s2
} // namespace shenmue

#endif
