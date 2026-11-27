#pragma once

#include "s2/area_jump.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_character_api.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/elevator.hpp"
#include "s2/event_weather.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/fog.hpp"
#include "s2/game_display.hpp"
#include "s2/help.hpp"
#include "s2/hiki.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/ui.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scenario_internal.hpp"
#include "s2/scenario_window.hpp"
#include "s2/scenario_free_talk.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/ship_spray.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2 {

// PC calls bind to their original implementations without a runtime table.
#if defined(_MSC_VER)
#define SHENMUE_EVENT_INLINE __forceinline
#else
#define SHENMUE_EVENT_INLINE inline __attribute__((always_inline))
#endif

struct EventFunctionBindings {
    static SHENMUE_EVENT_INLINE void InitiateCleanupForSceneChara(const shen_name &character) {
        s2::InitiateCleanupForSceneChara(character);
    }
    static SHENMUE_EVENT_INLINE std::uint16_t GetMotionAnimationId(const MotionWork *work, std::uint32_t region) {
        return s2::mtwkGetAnimationId(work, static_cast<MotionRegion>(region));
    }
    static SHENMUE_EVENT_INLINE std::uint64_t SetNpcRouteUpdatePaused(const shen_name &character, std::int32_t paused) {
        return s2::SetNpcRouteControlHold(character, paused);
    }
    static SHENMUE_EVENT_INLINE std::uint64_t CreateFireEffect(const Vector3 *position, float width, float height, std::int32_t drawMode, std::uint16_t flags) {
        return s2::CreateFireEffect(*position, width, height, drawMode, flags);
    }
    static SHENMUE_EVENT_INLINE Character *AttachFireEffect(std::uint32_t effect, const shen_name &character, std::int16_t bone, float offset) {
        return s2::AttachFireEffect(effect, character, bone, offset);
    }
    static SHENMUE_EVENT_INLINE std::uintptr_t ConfigureScenarioNodeMotion(
        const shen_name &character, std::int32_t motionId, float begin, float end, float frame, float speed, std::int32_t flags) {
        return s2::ConfigureScenarioNodeMotion(character, motionId, begin, end, frame, speed, flags);
    }
    static SHENMUE_EVENT_INLINE void ControlMazeFish() {
        s2::EV_MAZ_fishCtrl();
    }
    static SHENMUE_EVENT_INLINE void SetCurrentRefCount(std::uint32_t count) {
        s2::EV_SetCurrentRefCount(count);
    }
    static SHENMUE_EVENT_INLINE std::uint32_t mazeGetCurrentRefCount() {
        return s2::EV_GetCurrentRefCount();
    }
    static SHENMUE_EVENT_INLINE bool qronSetFloor(std::int32_t floor) {
        return s2::SetQronFloor(floor);
    }
    static SHENMUE_EVENT_INLINE void SetQteButtonState(std::uint16_t mask) noexcept {
        s2::SetQteButtonState(mask);
    }
    static SHENMUE_EVENT_INLINE std::int32_t GetCurrentQteStatus() noexcept {
        return s2::GetCurrentQteStatus();
    }
    static SHENMUE_EVENT_INLINE FigurePose *figpAssignModeFor(const shen_name &character, std::int8_t mode) {
        return s2::figpAssignModeFor(character, mode);
    }
    static SHENMUE_EVENT_INLINE std::uintptr_t figpSetFlagsBlendForBlur(const shen_name &character, std::int8_t scale, std::int8_t direction) {
        return s2::figpSetFlagsBlendForBlur(character, scale, direction);
    }
    static SHENMUE_EVENT_INLINE ConversationBuffer *EV_PlayDialogue(const char *voiceCode) {
        return s2::EV_PlayDialogue(voiceCode);
    }
    static SHENMUE_EVENT_INLINE std::int32_t shenGetMinutesUntilNextFrameMinute() {
        return s2::shenGetMinutesUntilNextFrameMinute();
    }
    static SHENMUE_EVENT_INLINE void GetScenarioCharacterEulerAngles(const shen_name &character, Vector3i *rotation, std::uint32_t flags) {
        s2::GetScenarioCharacterEulerAngles(character, rotation, flags);
    }
    static SHENMUE_EVENT_INLINE std::int32_t npcSetAreaName(const shen_name &area, const Vector3 *centre, float radius, std::int32_t enabled) {
        return s2::npcSetAreaName(area, centre, radius, enabled);
    }
    static SHENMUE_EVENT_INLINE std::int64_t npcLockUnlock(const shen_name &character) {
        return s2::EV_npcLockUnlock(character);
    }
    static SHENMUE_EVENT_INLINE std::int64_t npcFree(const shen_name &character) {
        return s2::EV_npcFree(character);
    }
    static SHENMUE_EVENT_INLINE std::int64_t ForceLoadNpc(const shen_name &character) {
        return s2::EV_ForceLoadNpc(character);
    }
    static SHENMUE_EVENT_INLINE std::int64_t CheckWarmStartNpcRoute(const shen_name &character) {
        return s2::EV_CheckWarmStartNpcRoute(character);
    }
    static SHENMUE_EVENT_INLINE void SetLightSceneIntensity(std::uint32_t index, float intensity) {
        s2::SetLightSceneIntensity(index, intensity);
    }
    static SHENMUE_EVENT_INLINE void SetLightRange(std::uint32_t index, float range) {
        s2::SetLightRange(index, range);
    }
    static SHENMUE_EVENT_INLINE void SetLightGeometryEnabled(std::uint32_t index, std::int32_t enabled) {
        s2::SetLightGeometryEnabled(index, enabled);
    }
    static SHENMUE_EVENT_INLINE bool ryoNotBusy() {
        return s2::ryoNotBusy();
    }
    static SHENMUE_EVENT_INLINE bool EV_GetScreenFilmIsNotShowing() {
        return s2::EV_GetScreenFilmIsNotShowing();
    }
    static SHENMUE_EVENT_INLINE void SetFirePalette(std::uint32_t effect, const std::uint16_t *palette) {
        s2::SetFirePalette(effect, palette);
    }
    static SHENMUE_EVENT_INLINE void SetFireFalloff(std::uint32_t effect, float radiusX, float radiusY, float gain) {
        s2::SetFireFalloff(effect, radiusX, radiusY, gain);
    }
    static SHENMUE_EVENT_INLINE void SetScenarioCharacterTransparency(const shen_name &character, float alpha) {
        s2::SetScenarioCharacterTransparency(character, alpha);
    }
    static SHENMUE_EVENT_INLINE void EventFunction870(std::int32_t) {
        s2::ReleaseNpcScriptRouteOwner();
    }
    static SHENMUE_EVENT_INLINE void SetScenarioFollowCharacter(const shen_name &character) {
        s2::SetScenarioFollowCharacter(character);
    }
    static SHENMUE_EVENT_INLINE std::int32_t SetElevatorDisplayMode(const shen_name &elevator, std::int32_t selector, std::int32_t value) noexcept {
        return s2::SetElevatorDisplayMode(elevator, selector, value);
    }
    static SHENMUE_EVENT_INLINE void LoadNPCRoute(const char *filename) {
        s2::EV_LoadNPCRoute(filename);
    }
    static SHENMUE_EVENT_INLINE void EV_stepSetFlags(std::int32_t flag, std::int32_t mode) {
        s2::EV_stepSetFlags(static_cast<std::uint8_t>(mode), flag);
    }
    static SHENMUE_EVENT_INLINE std::uint64_t SetPlayerGameFlags(std::int32_t mode, std::intptr_t value) {
        return s2::SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(mode), reinterpret_cast<void *>(value));
    }
    static SHENMUE_EVENT_INLINE bool GetShouldRenderIcons(std::int32_t slot) {
        return s2::GetShouldRenderIcons(static_cast<RenderIconSlot>(slot));
    }
    static SHENMUE_EVENT_INLINE void EV_nlSetVertexColourType(std::int32_t type) {
        s2::SetLegacyVertexColourType(type);
    }
    static SHENMUE_EVENT_INLINE std::int64_t AddInventoryItem(std::uint32_t item, std::uint8_t amount, std::uint8_t flags = 0) {
        return s2::AddInventoryItem(item, amount, flags);
    }
    static SHENMUE_EVENT_INLINE std::uint64_t GetQronNextStream() noexcept {
        return s2::GetQronNextStream();
    }
    static SHENMUE_EVENT_INLINE bool plymIsSearchBlocked() {
        return s2::plymIsSearchBlocked();
    }
    static SHENMUE_EVENT_INLINE void EV_EventSetUpHelpIcon_0(std::uint8_t topic, std::uint8_t mode) {
        s2::EV_EventSetUpHelpIcon_0(topic, mode);
    }
    static constexpr auto AssignHikiRotation = s2::AssignHikiRotation;
    static constexpr auto AssignShipSprayYaw = s2::AssignShipSprayYaw;
    static SHENMUE_EVENT_INLINE void EV_DisableCharacterDisplay(const shen_name &character) {
        s2::EV_DisableCharacterDisplay(character);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_LoadAudio(const char *bank, std::int64_t sceneIndex) {
        return s2::EV_LoadAudio(bank, sceneIndex);
    }
    static SHENMUE_EVENT_INLINE std::uint64_t EV_LoadCutsceneFromCurrentScene(const char *name) noexcept {
        return s2::EV_LoadCutsceneFromCurrentScene(name);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_NPC_NewLoadSw_0(std::uint32_t state) {
        return s2::EV_NPC_NewLoadSw(state);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_ResetFreeTalkFlags() {
        return s2::EV_ResetFreeTalkFlags();
    }
    static SHENMUE_EVENT_INLINE void EV_SetCharacterAlphaEnabled(const shen_name &name, bool enabled) {
        s2::EV_SetCharacterAlphaEnabled(name, enabled);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_SetNewSceneIndex() {
        return s2::EV_SetNewSceneIndex();
    }
    static SHENMUE_EVENT_INLINE void EV_scenQueueLoad(std::int32_t act, const shen_name &scene, std::int32_t entry) {
        s2::EV_scenQueueLoad(act, scene, entry);
    }
    static SHENMUE_EVENT_INLINE void FreeParticleEffect(std::int32_t type, std::uint32_t index) {
        s2::FreeParticleEffect(type, index);
    }
    static constexpr auto HIKI_Enqueue = s2::HIKI_Enqueue;
    static SHENMUE_EVENT_INLINE std::intptr_t HLib_DestroyCurrentTask() {
        return HLib::DestroyCurrentTask();
    }
    static SHENMUE_EVENT_INLINE std::intptr_t HLib_DestroyTask(HLib::Task *task) {
        return HLib::DestroyTask(task);
    }
    static SHENMUE_EVENT_INLINE HLib::Task *HLib_EnqueueTaskWithParameter(
        HLib::TaskCallback callback, std::uint8_t queue, std::uint8_t flags, std::size_t size, const shen_name &name) {
        return HLib::EnqueueTaskWithParameter(callback, queue, flags, size, name);
    }
    static constexpr auto HLib_EnqueueTaskWithoutParameter = HLib::EnqueueTaskWithoutParameter;
    static SHENMUE_EVENT_INLINE void *HLib_GetTaskParameter(HLib::Task *task) {
        return HLib::GetTaskParameter(task);
    }
    static constexpr auto InitializeShipSpray = s2::InitializeShipSpray;
    static SHENMUE_EVENT_INLINE bool LoadCutsceneAndSoundProgramForArea() noexcept {
        return s2::sbtLoadProgram();
    }
    static constexpr auto ProcessAicaCommand = s2::ProcessAicaCommand;
    static constexpr auto QueueImmediateNextFadeTask = s2::QueueImmediateNextFadeTask;
    static constexpr auto ResumeParticleEffect = s2::ResumeParticleEffect;
    static constexpr auto SetNewWindPhaseAndSpeed = s2::SetNewWindPhaseAndSpeed;
    static constexpr auto SetParticleEffectBirthRate = s2::SetParticleEffectBirthRate;
    static constexpr auto SetParticleEffectColour = s2::SetParticleEffectColour;
    static constexpr auto SetParticleEffectPosition = s2::SetParticleEffectPosition;
    static constexpr auto SetFireInstanceSize = s2::SetFireInstanceSize;
    static constexpr auto ToggleFlagOnFieldEntity = s2::ToggleFlagOnFieldEntity;
    static constexpr auto SetParticleEffectSize = s2::SetParticleEffectSize;
    static SHENMUE_EVENT_INLINE std::uint64_t SetPlayerControlEnabled(std::int32_t mode) {
        return s2::SetPlayerControlEnabled(mode);
    }
    static SHENMUE_EVENT_INLINE void SetRenderClockAndCompassTo(std::int32_t suppressed) noexcept {
        s2::shenSetClockAndCompassRenderTo(suppressed);
    }
    static constexpr auto SetScenarioCharacterKillMode = s2::SetScenarioCharacterKillMode;
    static SHENMUE_EVENT_INLINE void SetScreenFilmFade(std::uint32_t frames, std::uint8_t a0, std::uint8_t r0, std::uint8_t g0, std::uint8_t b0,
        std::uint8_t a1, std::uint8_t r1, std::uint8_t g1, std::uint8_t b1) {
        s2::SetScreenFilmFade(frames, a0, r0, g0, b0, a1, r1, g1, b1);
    }
    static constexpr auto SuspendParticleEffect = s2::SuspendParticleEffect;
    static SHENMUE_EVENT_INLINE CameraRuntime *camCtrlSetFovTransitionTo(bool enabled) {
        return s2::camCtrlSetFovTransitionTo(enabled);
    }
    static constexpr auto hsFadeVoice = s2::hsFadeVoice;
    static constexpr auto mtwkSetExtraFlag = s2::mtwkSetExtraFlag;
    static SHENMUE_EVENT_INLINE void EV_SetCharaControllerFlags(const shen_name &character, std::uint32_t flags, std::int32_t enabled) {
        s2::EV_SetCharaControllerFlags(character, flags, static_cast<NpcCharacterControllerState>(enabled));
    }
    static constexpr auto shenGetCurrentSbt = s2::shenGetCurrentSbt;
    static constexpr auto shenPlayAudioWithFlags = s2::shenPlayAudioWithFlags;

    static constexpr auto AT_SetLocalBaseChara = s2::AT_SetLocalBaseChara;
    static constexpr auto AssignLightParams = s2::EV_AssignLightParams;
    static constexpr auto AssignLightProgram = s2::AssignLightProgram;
    static constexpr auto BeginPrimarySampleExecution = s2::BeginPrimarySampleExecution;
    static constexpr auto BeginSecondarySampleExecution = s2::BeginSecondarySampleExecution;
    static constexpr auto DoesScenarioCharacterExist = s2::DoesScenarioCharacterExist;
    static SHENMUE_EVENT_INLINE void EV_EnableCharacterDisplay(const shen_name &character) {
        s2::EV_EnableCharacterDisplay(character);
    }
    static constexpr auto EV_EnsureCharacterDraw = s2::EV_EnsureCharacterDraw;
    static SHENMUE_EVENT_INLINE bool EV_FreeAuthorPak(std::int32_t operation, const char *name) {
        return s2::EV_FreeAuthorPak(operation, name);
    }
    static constexpr auto EV_GetShenmueFlag = s2::EV_GetShenmueFlag;
    static SHENMUE_EVENT_INLINE void EV_SetWeatherInstruction(std::int32_t instruction, std::int32_t value) {
        s2::EV_SetWeatherInstruction(instruction, value);
    }
    static constexpr auto EV_ToggleCharacterFlag = s2::EV_ToggleCharacterFlag;
    static SHENMUE_EVENT_INLINE void EV_lightLoad(const shen_name &light) {
        s2::EV_lightLoad(light);
    }
    static SHENMUE_EVENT_INLINE void FinishLoading() {
        s2::FinishLoading();
    }
    static constexpr auto FreeWAIR = s2::EV_FreeWAIR;
    static constexpr auto GetNpcNewLoadState = s2::GetNpcNewLoadState;
    static constexpr auto GetSavedMinigameScore = s2::GetSavedMinigameScore;
    static constexpr auto HLib_GetCurrentTask = HLib::GetCurrentTask;
    static constexpr auto HLib_MarkCurrentTaskForCleanup = HLib::MarkCurrentTaskForCleanup;
    static constexpr auto HLib_ResolveDeferredCallback = HLib::ResolveDeferredCallback;
    static constexpr auto HLib_SetCurrentTaskTo = HLib::SetCurrentTaskTo;
    static constexpr auto HLib_SetupTaskDelay = HLib::SetupTaskDelay;
    static SHENMUE_EVENT_INLINE void HLib_SetupTimedTask(HLib::Task *task, std::int32_t frames) {
        HLib::SetupTimedTask(task, frames);
    }
    static constexpr auto HasSavedAreaJumpLocation = s2::HasSavedAreaJumpLocation;
    static constexpr auto NpcWait = s2::NpcWait;
    static SHENMUE_EVENT_INLINE void PushGlobalLightingProgram() {
        s2::PushGlobalLightingProgram();
    }
    static constexpr auto RefreshWeatherForCurrentTime = s2::RefreshWeatherForCurrentTime;
    static SHENMUE_EVENT_INLINE void ReinitializeLights(std::int32_t mode) {
        s2::EV_ReinitLights(mode);
    }
    static SHENMUE_EVENT_INLINE std::int64_t SetAreaFogParams() {
        return s2::SetAreaFogParams();
    }
    static constexpr auto SetDefaultVS = s2::SetDefaultVS;
    static constexpr auto SetDefaultVSByIndex = s2::SetDefaultVSByIndex;
    static constexpr auto SetDisplayFlagForProp = s2::SetDisplayFlagForProp;
    static constexpr auto SetDoorSubjectEnabled = s2::SetDoorSubjectEnabled;
    static constexpr auto SetIsSkySetup = s2::SetIsSkySetup;
    static SHENMUE_EVENT_INLINE void SetLightEntryIndex(std::int32_t index, std::int32_t value) {
        s2::EV_SetIndexForLightEntry(index, value);
    }
    static SHENMUE_EVENT_INLINE void SetLightFlagForIndex(std::int32_t index, std::int32_t value) {
        s2::EV_SetLightFlagForIndex(index, value);
    }
    static constexpr auto SetMapControlStatusFlag = s2::SetMapControlStatusFlag;
    static constexpr auto SetMapModelDataDefaultVertexShader = s2::EV_SetModelDataDefaultVertexShader;
    static SHENMUE_EVENT_INLINE std::int32_t SetObjectFlagByIndex(std::int32_t index, std::int32_t enabled) {
        return s2::EV_SetObjectFlagByIndex(index, enabled);
    }
    static constexpr auto SetPluralControlFor = s2::SetPluralControlFor;
    static constexpr auto SetPosRotForGame = s2::SetPosRotForGame;
    static constexpr auto SetRefBoardParamsForCharaById = s2::EV_SetRefBoardParamsFor;
    static constexpr auto SetScenarioCharacterFlag40 = s2::SetScenarioCharacterFlag40;
    static constexpr auto SetScenarioCharacterRadius = s2::SetScenarioCharacterRadius;
    static constexpr auto SetWeatherUpdateEnabledTo = s2::SetWeatherUpdateEnabledTo;
    static constexpr auto SetupLightConfig = s2::SetupLightConfig;
    static constexpr auto UpdateGameNodeFlags = s2::UpdateGameNodeFlags;
    static constexpr auto UpdateWeather = s2::UpdateWeather;
    static constexpr auto UpdateWeatherForMinute = s2::UpdateWeatherForMinute;
    static constexpr auto ctrlGetCharaByName = s2::ctrlGetCharaByName;
    static SHENMUE_EVENT_INLINE bool filmCheckFadeTrackColour() {
        return s2::filmCheckFadeTrackColour();
    }
    static constexpr auto flagRelated = s2::flagRelated;
    static constexpr auto flddSetPointFlagOnGrid = s2::flddSetPointFlagOnGrid;
    static constexpr auto scenCopyCharaToGame = s2::scenCopyCharaToGame;
    static constexpr auto shenUpdateFlagEvaluation = s2::shenUpdateFlagEvaluation;
    static constexpr auto shenUpdatePlayerDataFlags = s2::shenUpdatePlayerDataFlags;
    static constexpr auto srchSetTargetEnabled = s2::srchSetTargetEnabled;

    static constexpr auto GetCharaPosition = s2::EV_GetCharaPosition;
    static constexpr auto HasMapModelReader = s2::HasMapModelReader;
    static constexpr auto mapSetupLighting = s2::SetQronMapLightingEnabled;
    static constexpr auto Queue_ST_LoadTitleFadeIn = s2::Queue_ST_LoadTitleFadeIn;
    static constexpr auto ST_LoadTitleFade = s2::ST_LoadTitleFade;
    static SHENMUE_EVENT_INLINE std::intptr_t HLib_DestroyCurrentEventTask() {
        return HLib::DestroyCurrentTask();
    }
    static constexpr auto SetMapModelVertexShaderType = s2::SetMapModelVertexShaderType;
    static constexpr auto dbgPrintEventMessage = s2::dbgPrintEventMessage;
    static constexpr auto SetFogRed = s2::SetFogRed;
    static constexpr auto SetFogGreen = s2::SetFogGreen;
    static constexpr auto SetFogBlue = s2::SetFogBlue;
    static constexpr auto SetFogDistance = s2::SetFogDistance;
    static constexpr auto ReinitFogTasks = s2::ReinitFogTasks;
    static constexpr auto ActivatePartyCompanion = s2::EV_ActivatePartyCompanion;
    static constexpr auto PTYCUtils = s2::EV_PTYCUtils;
    static constexpr auto IsScenarioBusy = s2::IsScenarioBusy;
    static constexpr auto shenToggleAreaFlags = s2::shenToggleAreaFlags;
    static constexpr auto EV_setHoldStatesFlags = s2::EV_setHoldStatesFlags;
    static SHENMUE_EVENT_INLINE void SetFireFalloffScalar(std::uint32_t effect, float scalar) {
        s2::SetFireFalloffScalar(effect, scalar);
    }
    static SHENMUE_EVENT_INLINE std::uint32_t SetFirePosition(std::uint32_t effect, std::int32_t instance, const Vector3 *position) {
        return s2::SetFirePosition(effect, instance, position);
    }
    static constexpr auto EVT_GetControllerInput = s2::EVT_GetControllerInput;
    static constexpr auto SetDebugScreenTextPosition = s2::EV_set_debug_text_pos;
    static constexpr auto DebugLogToScreen = s2::EV_debug_log_to_screen;
    static SHENMUE_EVENT_INLINE std::uint64_t EventFunction857(std::uint32_t mode) {
        return s2::SetNpcColdResetMode(mode);
    }
    static SHENMUE_EVENT_INLINE DoorAreaCleanupCallback ExecuteCleanupFuncForAreaWithCachedScene() {
        return s2::ExecuteCleanupFuncForAreaWithCachedScene();
    }
    static SHENMUE_EVENT_INLINE void LoadBlurModelForCharacter_0(const shen_name &name, const char *path, float time) {
        s2::LoadBlurModelForCharacter(name, path, time);
    }

    static SHENMUE_EVENT_INLINE void AssignShipSprayPosition(const Vector3 *position) {
        s2::AssignShipSprayPosition(*position);
    }
    static SHENMUE_EVENT_INLINE void AssignHikiPosition(const Vector3 *position) {
        s2::AssignHikiPosition(*position);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_authSceneCtrl(std::int32_t command) {
        return s2::EV_authSceneCtrl(static_cast<AuthSceneCtrl>(command));
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_authSceneCtrl(shen_name character) {
        return s2::EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::uint32_t>(character)));
    }
    static SHENMUE_EVENT_INLINE float GetRandomNumberInRange(float bound) {
        return s2::IncrementSharedPrngFloat() * bound;
    }
    static SHENMUE_EVENT_INLINE std::int32_t shenGetRandomInt32(std::int32_t bound) {
        return static_cast<std::int32_t>(s2::IncrementSharedPrngFloat() * static_cast<float>(bound));
    }
    static SHENMUE_EVENT_INLINE void SetScenarioCharacterDrawControl(shen_name character, std::int32_t mode, std::int32_t flags, float = 0.0f) {
        s2::SetScenarioCharacterDrawControl(character, mode, flags);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_spmBeginSampleExecute(std::int32_t) {
        return s2::EV_spmBeginSampleExecute();
    }
    static SHENMUE_EVENT_INLINE bool mtwkRequestMotion(MotionWork *motion, std::int32_t id, std::int32_t region) {
        return s2::mtwkRequestMotion(motion, static_cast<std::uint16_t>(id), static_cast<MotionRegion>(region));
    }
    static SHENMUE_EVENT_INLINE std::uint16_t SetMotionRegionFlag(MotionWork *motion, std::uint32_t mode, std::int32_t operation, std::int32_t region) {
        return s2::mtwkSetRegionFlag(motion, mode, operation, static_cast<MotionRegion>(region));
    }
    static SHENMUE_EVENT_INLINE void mtwkSetSpeed(MotionWork *motion, float speed, std::int32_t region) {
        s2::mtwkSetSpeed(motion, speed, static_cast<MotionRegion>(region));
    }
    static SHENMUE_EVENT_INLINE std::uint64_t camCtrlSetState(std::int32_t state) {
        return s2::camCtrlSetState(static_cast<CameraState>(state));
    }
    static SHENMUE_EVENT_INLINE std::uint64_t camCtrlUtil(std::int32_t mode, std::intptr_t parameter) {
        return s2::camCtrlUtil(static_cast<CameraUtilityMode>(mode), parameter);
    }
    static SHENMUE_EVENT_INLINE MotionWork *scenFindMotionwork(shen_name character) {
        return FindMotionWork(ctrlGetScenarioChara(character));
    }

    static SHENMUE_EVENT_INLINE bool CheckIsArgEquZero(std::int32_t value, std::int32_t = 0) {
        return s2::CheckIsArgEquZero(value);
    }
    static SHENMUE_EVENT_INLINE void EV_ApplyTranslationClockVisibility() {
        s2::EV_ApplyTranslationClockVisibility(0);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_GetPlayerFlag(std::int32_t mode, std::int16_t flag) {
        return s2::EV_GetPlayerFlag(static_cast<PlayerFlagReadMode>(mode), flag);
    }
    static SHENMUE_EVENT_INLINE void EV_SetPlayerFlag(std::int32_t mode, std::int16_t flag, std::int32_t value) {
        s2::EV_SetPlayerFlag(static_cast<PlayerFlagWriteMode>(mode), flag, value);
    }
    static SHENMUE_EVENT_INLINE void scenApplyPosOrRotToChildId(shen_name name, std::int32_t part, Vector3 *position, Vector3i *rotation, std::int32_t mode) {
        s2::EV_scenApplyPosOrRotToPartOf(name, static_cast<std::uint16_t>(part), position, rotation, static_cast<EventCharacterPartTransformMode>(mode));
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_ControlPackageResources(
        std::int32_t category, std::int32_t operation, const void *resource, std::uintptr_t value, std::int32_t flags) {
        return s2::EV_pakUtils(category, static_cast<std::uint64_t>(operation), reinterpret_cast<std::uintptr_t>(resource), value, flags);
    }
    static SHENMUE_EVENT_INLINE void EV_AT_SetPos(shen_name name, std::uint32_t flags, const Vector3 *position) {
        s2::EV_AT_SetPos(name, flags, *position);
    }
    static SHENMUE_EVENT_INLINE std::int64_t EV_AT_SetAngY_0(shen_name name, std::uint32_t flags, const Vector3i *rotation, std::int32_t reserved) {
        return s2::EV_AT_SetAngY(name, flags, *rotation, reinterpret_cast<const std::uint16_t *>(static_cast<std::uintptr_t>(reserved)));
    }

    static SHENMUE_EVENT_INLINE const char *Filepath_Gen(std::int32_t mode) {
        return s2::Filepath_Gen(static_cast<FilepathGenMode>(mode));
    }
#undef SHENMUE_EVENT_INLINE
};

inline constexpr EventFunctionBindings FTBL{};
using HLibTask = HLib::Task;
inline constexpr std::int32_t TASK_EVNT = static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name);

} // namespace shenmue::s2
