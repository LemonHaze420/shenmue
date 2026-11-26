#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_state_flags_dc.hpp"
#endif
#include "s2/fight.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/math.hpp"
#include "s2/air_dc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/fog_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/event_film_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/load_title.hpp"
#include "s2/author_load_dc.hpp"
#include "s2/event_overlay.hpp"
#include "s2/qron_dc.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/pak_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "s2/weather.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#include "s2/window.hpp"
#include "s2/world_render.hpp"

namespace shenmue {
namespace s2 {
void AddWeatherInstructionFlags(std::uint32_t flags);
void RemoveWeatherInstructionFlags(std::uint32_t flags);
} // namespace s2
} // namespace shenmue

extern "C" {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool __attribute__((renesas)) shenmue_s2_event_SetMotionIkTargetPosition(shenmue::s2::MotionWork *work, shenmue::Vector3 *position) {
    return shenmue::s2::mtwkSetIkTargetPosition(work, position);
}
__attribute__((renesas)) std::uint8_t *shenmue_s2_event_figpAssignModeFor(std::uint32_t name, std::uint8_t mode) {
    const shenmue::s2::MapControlData *const control =
        reinterpret_cast<const shenmue::s2::MapControlData *>(shenmue::s2::HLib::GetTaskParameter(shenmue::s2::g_mapControlTask));
    return shenmue::s2::figpAssignModeForCharacter(shenmue::s2::scenFindChara(control->scenarioTask, shenmue::shen_name(name)), mode);
}
void __attribute__((renesas)) shenmue_s2_event_figpSetFlagsBlendForBlur(std::uint32_t name, std::uint8_t scale, std::uint8_t direction) {
    const shenmue::s2::MapControlData *const control =
        reinterpret_cast<const shenmue::s2::MapControlData *>(shenmue::s2::HLib::GetTaskParameter(shenmue::s2::g_mapControlTask));
    shenmue::s2::figpAssignMotScaleAndDirFor(shenmue::s2::scenFindChara(control->scenarioTask, shenmue::shen_name(name)), scale, direction);
}
__attribute__((renesas)) shenmue::s2::FigurePoseControlPrefix *shenmue_s2_event_figpSetFlagModesFor(
    std::uint32_t name, std::uint8_t bitIndex, std::uint8_t enabled) {
    const shenmue::s2::MapControlData *const control =
        reinterpret_cast<const shenmue::s2::MapControlData *>(shenmue::s2::HLib::GetTaskParameter(shenmue::s2::g_mapControlTask));
    return shenmue::s2::EV_figpAssignFlagsFor(shenmue::s2::scenFindChara(control->scenarioTask, shenmue::shen_name(name)), bitIndex, enabled);
}
void __attribute__((renesas)) shenmue_s2_event_SetParticleEffectBirthRate(std::int32_t type, std::uint32_t index, float fraction) {
    shenmue::s2::SetParticleEffectBirthRate(type, index, fraction);
}
__attribute__((renesas)) void *shenmue_s2_event_fft_fill_krn(float scale, void *kernel) {
    return shenmue::s2::FillFftKernel(scale, kernel);
}
void __attribute__((renesas)) shenmue_s2_event_fft_fill_dist(const void *kernel) {
    shenmue::s2::FillFftDistance(kernel);
}
void __attribute__((renesas)) shenmue_s2_event_fft_bins_load_pairwise_tables(const void *first, const void *second) {
    shenmue::s2::ApplyFft(first, second);
}
void __attribute__((renesas)) shenmue_s2_event_FFTUtil(float scrollU, float scrollV) {
    shenmue::s2::FinalizeFft(scrollU, scrollV);
}
void __attribute__((renesas)) shenmue_s2_event_set_lighting_flags(float ambient, const shenmue::Vector3 *direction) {
    shenmue::s2::SetWaveLighting(ambient, direction);
}
void __attribute__((renesas)) shenmue_s2_event_SetParticleEffectInitialScale(std::int32_t type, std::uint32_t index, float scale) {
    shenmue::s2::SetParticleEffectInitialScale(type, index, scale);
}
void __attribute__((renesas)) shenmue_s2_event_SetSmokeEffectRadialMotion(std::uint32_t index, float velocity, float decay) {
    shenmue::s2::SetSmokeEffectRadialMotion(index, velocity, decay);
}
void __attribute__((renesas)) shenmue_s2_event_SetFireAlpha(std::uint32_t index, float alpha) {
    shenmue::s2::SetFireAlpha(index, alpha);
}
shenmue::s2::FireInstance *__attribute__((renesas)) shenmue_s2_event_SetFirePosition(
    std::uint32_t index, std::int32_t instance, const shenmue::Vector3 *position) {
    return shenmue::s2::SetFirePosition(index, instance, position);
}
void __attribute__((renesas)) shenmue_s2_event_SetFireInstanceAlpha(std::uint32_t index, std::int32_t instance, float alpha) {
    shenmue::s2::SetFireInstanceAlpha(index, instance, alpha);
}
void __attribute__((renesas)) shenmue_s2_event_SetFireInstanceSize(std::uint32_t index, std::int32_t instance, float width, float height) {
    shenmue::s2::SetFireInstanceSize(index, instance, width, height);
}
void __attribute__((renesas)) shenmue_s2_event_SetFirePalette(std::uint32_t index, const std::uint16_t *palette) {
    shenmue::s2::SetFirePalette(index, palette);
}
void __attribute__((renesas)) shenmue_s2_event_FreeParticleEffect(std::int32_t type, std::uint32_t index) {
    shenmue::s2::FreeParticleEffect(type, index);
}
void __attribute__((renesas)) shenmue_s2_event_ResumeParticleEffect(std::int32_t type, std::uint32_t index) {
    shenmue::s2::ResumeParticleEffect(type, index);
}
void __attribute__((renesas)) shenmue_s2_event_SuspendParticleEffect(std::int32_t type, std::uint32_t index) {
    shenmue::s2::SuspendParticleEffect(type, index);
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_GetParticleEffectVisibility(std::int32_t type, std::uint32_t index) {
    return shenmue::s2::GetParticleEffectVisibility(type, index);
}
void __attribute__((renesas)) shenmue_s2_event_SetParticleEffectPosition(
    std::int32_t type, std::uint32_t index, const shenmue::Vector3 *position, const shenmue::Vector3 *variance) {
    shenmue::s2::SetParticleEffectPosition(type, index, position, variance);
}
void __attribute__((renesas)) shenmue_s2_event_SetParticleEffectVelocity(
    std::int32_t type, std::uint32_t index, const shenmue::Vector3 *velocity, const shenmue::Vector3 *variance) {
    shenmue::s2::SetParticleEffectVelocity(type, index, velocity, variance);
}
void __attribute__((renesas)) shenmue_s2_event_SetParticleEffectSize(std::int32_t type, std::uint32_t index, float width, float height) {
    shenmue::s2::SetParticleEffectSize(type, index, width, height);
}
void __attribute__((renesas)) shenmue_s2_event_SetParticleEffectColour(
    std::int32_t type, std::uint32_t index, std::int32_t alpha, std::int32_t red, std::int32_t green, std::int32_t blue) {
    shenmue::s2::SetParticleEffectColour(type, index, alpha, red, green, blue);
}
void __attribute__((renesas)) shenmue_s2_event_TranslateParticleEffectParticles(std::int32_t type, std::uint32_t index, const shenmue::Vector3 *delta) {
    shenmue::s2::TranslateParticleEffectParticles(type, index, delta);
}
void __attribute__((renesas)) shenmue_s2_event_SetStationarySmokeAlpha(std::uint32_t index, std::int32_t alpha) {
    shenmue::s2::SetStationarySmokeAlpha(index, alpha);
}

void __attribute__((renesas)) shenmue_s2_event_SetIsSkySetup(std::int32_t value) {
    shenmue::s2::SetIsSkySetup(value);
}

void __attribute__((renesas)) shenmue_s2_event_ReadSkyboxTexture(std::int32_t textureIndex, std::int32_t layer, float blendMax) {
    shenmue::s2::LoadAirTextures(textureIndex, blendMax, layer);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetPlanetLayerColour(std::int32_t channel, std::int32_t layer) {
    return shenmue::s2::GetPlanetLayerColour(channel, layer);
}

void __attribute__((renesas)) shenmue_s2_event_SetPlanetLayerOffset(const shenmue::Vector3 *offset, std::int32_t layer) {
    shenmue::s2::SetPlanetLayerOffset(offset, layer);
}

void __attribute__((renesas)) shenmue_s2_event_ResetPlanetLayerOffset(std::int32_t layer) {
    shenmue::s2::ResetPlanetLayerOffset(layer);
}

shenmue::s2::ConversationBuffer *__attribute__((renesas)) shenmue_s2_event_EV_PlayDialogue(const char *voiceCode) {
    return shenmue::s2::shenPlayAudio(voiceCode, shenmue::shen_name(), 4);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetConversationPaused(const shenmue::s2::ConversationBuffer *buffer) {
    return shenmue::s2::GetConversationPaused(buffer);
}

shenmue::s2::ConversationBuffer *__attribute__((renesas)) shenmue_s2_event_GetConverseDataForChara(std::uint32_t speaker) {
    return shenmue::s2::GetConverseDataForChara(shenmue::shen_name(speaker));
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_adfsSetCameraIdxForChara(std::uint32_t target, std::uint32_t source) {
    return shenmue::s2::RegisterConversationSpeakerAlias(shenmue::shen_name(target), shenmue::shen_name(source), 0);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_cvsFindVoiceDataFor(const shenmue::s2::ConversationBuffer *buffer) {
    return shenmue::s2::cvsFindVoiceDataFor(buffer);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetNumBet() {
    return shenmue::s2::GetNumBet();
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_AnimateMoneyAmount(std::int32_t initial, std::int32_t target, std::int32_t modeIndex) {
    return shenmue::s2::AnimateMoneyAmount(initial, target, modeIndex);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetSleepActionIcon(std::int16_t flags) {
    return shenmue::s2::SetSleepActionIcon(flags);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetSleepActionIconResult() {
    return shenmue::s2::GetSleepActionIconResult();
}

void __attribute__((renesas)) shenmue_s2_event_SetHelpIconState() {
    shenmue::s2::SetHelpIconState();
}

void __attribute__((renesas)) shenmue_s2_event_SetShowQteFlagForButtonType(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint32_t buttonType) {
    shenmue::s2::SetShowQteFlagForButtonType(requiredStatus, maximumFrames, buttonType);
}

void __attribute__((renesas)) shenmue_s2_event_SetQteButtonState(std::uint16_t mask) {
    shenmue::s2::SetQteButtonState(mask);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetCurrentQteStatus() {
    return shenmue::s2::GetCurrentQteStatus();
}

char *__attribute__((renesas)) shenmue_s2_event_shenGetCurrentSbt() {
    return shenmue::s2::shenGetCurrentSbt();
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_FindMaterialSoundBank(std::uint32_t material) {
    return shenmue::s2::FindMaterialSoundBank(material);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_ExecuteAicaBankCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) {
    return shenmue::s2::ExecuteAicaBankCommand(command, bank, value);
}

void __attribute__((renesas)) shenmue_s2_event_spmFadeInBank(std::int32_t bank, std::int32_t frames) {
    shenmue::s2::spmFadeInBank(bank, frames);
}

void __attribute__((renesas)) shenmue_s2_event_spmFadeOutBank(std::int32_t bank, std::int32_t frames) {
    shenmue::s2::spmFadeOutBank(bank, frames);
}

void __attribute__((renesas)) shenmue_s2_event_EV_spmBeginSampleExecute() {
    shenmue::s2::EV_spmBeginSampleExecute();
}

void __attribute__((renesas)) shenmue_s2_event_spmFadeOut(std::int32_t sample) {
    shenmue::s2::spmFadeOut(sample);
}

void __attribute__((renesas)) shenmue_s2_event_hsFadeVoice(shenmue::s2::ConversationBuffer *buffer, std::int32_t frames) {
    shenmue::s2::scenCvsCtrlFree(buffer, frames);
}

void __attribute__((renesas)) shenmue_s2_event_RefreshWeatherForCurrentTime(std::int32_t frames) {
    shenmue::s2::RefreshWeatherForCurrentTime(frames);
}

void __attribute__((renesas)) shenmue_s2_event_UpdateWeatherForMinute(std::int32_t mode, std::int32_t enabled, std::int32_t frames, float value) {
    shenmue::s2::UpdateWeatherForMinute(mode, enabled, frames, value);
}

void __attribute__((renesas)) shenmue_s2_event_FreeWAIR() {
    shenmue::s2::FreeWAIR();
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetCurrentWeather() {
    return shenmue::s2::GetCurrentWeather();
}

shenmue::s2::WeatherTableSample *__attribute__((renesas)) shenmue_s2_event_GetInterpolatedWeatherValues() {
    return shenmue::s2::GetInterpolatedWeatherValues();
}

void __attribute__((renesas)) shenmue_s2_event_SetNewWindPhaseAndSpeed(std::int32_t phase, float speed) {
    shenmue::s2::SetNewWindPhaseAndSpeed(phase, speed);
}

void __attribute__((renesas)) shenmue_s2_event_SyncWindWithWDATWeatherState() {
    shenmue::s2::SyncWindWithWDATWeatherState();
}

void __attribute__((renesas)) shenmue_s2_event_WSCN_LoadWeatherData(const char *directory, const char *filename) {
    shenmue::s2::WSCN_LoadWeatherData(directory, filename);
}

void __attribute__((renesas)) shenmue_s2_event_WSCN_DeleteWeatherData() {
    shenmue::s2::WSCN_DeleteWeatherData();
}

void __attribute__((renesas)) shenmue_s2_event_SetWeatherLightManager(std::int32_t enabled) {
    shenmue::s2::SetWeatherLightManager(enabled);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_IsFightActive() {
    return shenmue::s2::IsFightActive();
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetFightModeFlags() {
    return shenmue::s2::GetFightModeFlags();
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetFightModeFlags(std::uint16_t flags) {
    return shenmue::s2::SetFightModeFlags(flags);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_srchSetTargetEnabled(std::uint32_t name, std::int32_t enabled) {
    return shenmue::s2::srchSetTargetEnabled(shenmue::shen_name(name), enabled);
}

bool __attribute__((renesas)) shenmue_s2_event_srchClearTarget(std::uint32_t index) {
    return shenmue::s2::srchClearTarget(index);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_srchGetTargetName(std::uint32_t index) {
    return shenmue::s2::srchGetTargetName(index);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_srchSetLookMode() {
    return shenmue::s2::srchSetLookMode();
}

void __attribute__((renesas)) shenmue_s2_event_srchRequestLockonExit() {
    shenmue::s2::srchRequestLockonExit();
}

void __attribute__((renesas)) shenmue_s2_event_SetupLightConfig() {
    shenmue::s2::SetupLightConfig();
}

void __attribute__((renesas)) shenmue_s2_event_srchReset() {
    shenmue::s2::srchReset();
}

std::intptr_t __attribute__((renesas)) shenmue_s2_event_EV_ControlPackageResources(
    std::int32_t category, std::uint32_t mode, std::uintptr_t source, std::uintptr_t target, std::int32_t secondaryIndex) {
    return shenmue::s2::EV_pakUtils(category, mode, source, target, secondaryIndex);
}
#endif

void __attribute__((renesas)) shenmue_s2_event_SetPersistentHelpIconState(std::uint32_t visible, std::uint32_t selected, std::int32_t merge) {
    if (merge) {
        shenmue::s2::CheckHelpIcon(visible, selected, shenmue::s2::CheckHelpIconOp_Add);
    } else {
        shenmue::s2::CheckHelpIcon(visible, selected, shenmue::s2::CheckHelpIconOp_Set);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void __attribute__((renesas)) shenmue_s2_event_SetButtonHelpIcon(std::uint16_t button, std::int32_t, std::int32_t, std::uint16_t state) {
    if (button == 0x100)
        button = 4;
    shenmue::s2::CheckHelpIcon(state == 1 ? button : 0, 0, shenmue::s2::CheckHelpIconOp_Set);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetButtonHelpIconResult() {
    return 0;
}
#endif

void __attribute__((renesas)) shenmue_s2_event_EV_ApplyTranslationClockVisibility(std::uint32_t startParameters) {
    shenmue::s2::EV_ApplyTranslationClockVisibility(startParameters);
}

void __attribute__((renesas)) shenmue_s2_event_SetMoneyRelatedUIElement(std::int16_t maximum, std::uint16_t duration, std::int16_t step) {
    shenmue::s2::SetMoneyRelatedUIElement(maximum, duration, step);
}

void __attribute__((renesas)) shenmue_s2_event_SetMoneyRelatedUIElementAlternative(std::int16_t maximum, std::uint16_t duration, std::int16_t step) {
    shenmue::s2::SetMoneyRelatedUIElement(maximum, duration, step);
}

void __attribute__((renesas)) shenmue_s2_event_SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int16_t duration, std::int32_t step, std::uint32_t flags) {
    shenmue::s2::SetMoneyRelatedUIElement_0(maximum, duration, step, flags);
}

void __attribute__((renesas)) shenmue_s2_event_dbgPrintEventMessage(const char *message) {
    shenmue::s2::dbgPrintEventMessage(message);
}

void __attribute__((renesas)) shenmue_s2_event_AT_SetEyeCtrl(shenmue::s2::Character *character, float transitionFrames, float leftWeight, float rightWeight) {
    shenmue::s2::AT_SetEyeCtrl(character, transitionFrames, leftWeight, rightWeight);
}

void __attribute__((renesas)) shenmue_s2_event_SetAreaJumpPlacementFloat(std::int32_t offset, float value) {
    shenmue::s2::SetAreaJumpPlacementFloat(offset, value);
}

shenmue::s2::MotionWork *__attribute__((renesas)) shenmue_s2_event_scenFindMotionwork(std::uint32_t name) {
    return shenmue::s2::scenFindMotionwork(shenmue::shen_name(name));
}

bool __attribute__((renesas)) shenmue_s2_event_SampleNativeMotionRoot(std::uint16_t animation, std::int32_t bone, float frame, shenmue::Vector3 *position) {
    return shenmue::s2::SampleNativeMotionRoot(animation, bone, frame, position);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_RaycastInWorldGrid(shenmue::Vector3 *output, const shenmue::Vector3 *origin, float radius) {
    return shenmue::s2::RaycastInWorldGrid(output, origin, radius);
}

void __attribute__((renesas)) shenmue_s2_event_camCtrlSetRotationXYZ(float x, float y, float duration) {
    shenmue::s2::camCtrlSetRotationXYZ(x, y, duration);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_camCtrlSetTargetChara(std::uint32_t name) {
    return shenmue::s2::camCtrlSetTargetChara(shenmue::shen_name(name));
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterFlag40(std::uint32_t name, std::int32_t enabled) {
    return shenmue::s2::SetScenarioCharacterFlag40(shenmue::shen_name(name), enabled);
}

void __attribute__((renesas)) shenmue_s2_event_EV_ToggleCharacterFlag(std::uint32_t name) {
    shenmue::s2::EV_ToggleCharacterFlag(shenmue::shen_name(name));
}

void __attribute__((renesas)) shenmue_s2_event_mtwkSetMotionworkScale3(std::uint32_t name, const shenmue::Vector3 *scale) {
    shenmue::s2::mtwkSetMotionworkScale3(shenmue::shen_name(name), scale);
}

void __attribute__((renesas)) shenmue_s2_event_EV_SetCharacterImageSelection(std::uint32_t name, std::uint8_t selection) {
    shenmue::s2::EV_SetCharacterImageSelection(shenmue::shen_name(name), selection);
}

float __attribute__((renesas)) shenmue_s2_event_camCtrlSetMotionScaleFactor(float factor) {
    return shenmue::s2::camCtrlSetMotionScaleFactor(factor);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_drGetCurrentAuthorId() {
    return shenmue::s2::drGetCurrentAuthorId();
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetDoorSubjectEnabled(std::uint32_t subject, std::int32_t enabled) {
    return shenmue::s2::SetDoorSubjectEnabled(shenmue::shen_name(subject), enabled);
}

void __attribute__((renesas)) shenmue_s2_event_EV_SetCinemaBarVisibility(std::uint32_t mode, std::int32_t frames) {
    shenmue::s2::EV_SetCinemaBarVisibility(static_cast<shenmue::s2::FilmControlFlag>(mode), frames);
}

void __attribute__((renesas)) shenmue_s2_event_SetMotionRegionEvalTime(shenmue::s2::MotionWork *work, float frame, std::uint32_t region) {
    shenmue::s2::mtwkSetRegionEvalTime(work, frame, static_cast<shenmue::s2::MotionRegion>(region));
}

void __attribute__((renesas)) shenmue_s2_event_SetMotionLayerLoopStartTime(shenmue::s2::MotionWork *work, float frame, std::uint32_t region) {
    shenmue::s2::mtwkSetLayerLoopStartTime(work, frame, static_cast<shenmue::s2::MotionRegion>(region));
}

void __attribute__((renesas)) shenmue_s2_event_SetMotionLayerEndFrame(shenmue::s2::MotionWork *work, float frame, std::uint32_t region) {
    shenmue::s2::mtwkSetLayerEndFrame(work, frame, static_cast<shenmue::s2::MotionRegion>(region));
}

void __attribute__((renesas)) shenmue_s2_event_SetMotionLayerFrameRange(
    shenmue::s2::MotionWork *work, float loopStart, float start, float end, std::uint32_t region) {
    shenmue::s2::mtwkSetLayerFrameRange(work, loopStart, start, end, static_cast<shenmue::s2::MotionRegion>(region));
}

void __attribute__((renesas)) shenmue_s2_event_mtwkSetSpeed(shenmue::s2::MotionWork *work, float speed, std::uint32_t region) {
    shenmue::s2::mtwkSetSpeed(work, speed, static_cast<shenmue::s2::MotionRegion>(region));
}

bool __attribute__((renesas)) shenmue_s2_event_SetMotionBoneTargetStepLimit(shenmue::s2::MotionWork *work, std::int32_t bone, float limit) {
    return shenmue::s2::mtwkSetBoneTargetStepLimit(work, static_cast<shenmue::s2::MotionBoneId>(bone), limit);
}

void __attribute__((renesas)) shenmue_s2_event_AdjustScreenAspectRatio(
    shenmue::s2::LegacyScreenQuad *quad, std::int32_t alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) {
    shenmue::s2::AdjustScreenAspectRatio(*quad, static_cast<shenmue::s2::ScreenAlignment>(alignment), x, y, width, height);
}

float __attribute__((renesas)) shenmue_s2_event_camCtrlSetFarDistance(float value) {
    return shenmue::s2::camCtrlSetFarDistance(value);
}

float __attribute__((renesas)) shenmue_s2_event_camCtrlSetNearDistance(float value) {
    return shenmue::s2::camCtrlSetNearDistance(value);
}

float __attribute__((renesas)) shenmue_s2_event_camCtrlSetNpcRadius(float value) {
    return shenmue::s2::camCtrlSetNpcRadius(value);
}

void __attribute__((renesas)) shenmue_s2_event_SetFogRed(float value) {
    shenmue::s2::SetFogRed(value);
}

void __attribute__((renesas)) shenmue_s2_event_SetFogGreen(float value) {
    shenmue::s2::SetFogGreen(value);
}

void __attribute__((renesas)) shenmue_s2_event_SetFogBlue(float value) {
    shenmue::s2::SetFogBlue(value);
}

void __attribute__((renesas)) shenmue_s2_event_shenUpdatePlanetByLayer(float value, std::int32_t channel, std::int32_t layer) {
    shenmue::s2::shenUpdatePlanetByLayer(value, channel, layer);
}

void __attribute__((renesas)) shenmue_s2_event_ConfigureScenarioNodeMotion(
    std::uint32_t name, std::int32_t motionId, float begin, float end, float frame, float speed, std::int32_t flags) {
    shenmue::s2::ConfigureScenarioNodeMotion(shenmue::shen_name(name), motionId, begin, end, frame, speed, flags);
}

void __attribute__((renesas)) shenmue_s2_event_SetLightSceneIntensity(std::uint32_t index, float intensity) {
    shenmue::s2::SetLightSceneIntensity(index, intensity);
}

void __attribute__((renesas)) shenmue_s2_event_SetLightRange(std::uint32_t index, float range) {
    shenmue::s2::SetLightRange(index, range);
}

std::int16_t __attribute__((renesas)) shenmue_s2_event_InverseFixedAtan2(float first, float direction) {
    return shenmue::s2::InverseFixedAtan2(first, direction);
}

void __attribute__((renesas)) shenmue_s2_event_SetFacialMouthExpression(
    shenmue::s2::Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale) {
    shenmue::s2::SetFacialMouthExpression(character, expression, transitionFrames, scale);
}

void __attribute__((renesas)) shenmue_s2_event_SetCharacterAlpha(shenmue::s2::Character *character, float alpha) {
    shenmue::s2::SetCharacterAlpha(character, alpha);
}

void __attribute__((renesas)) shenmue_s2_event_Queue_ST_LoadTitleFadeIn(std::uint32_t author) {
    shenmue::s2::Queue_ST_LoadTitleFadeIn(shenmue::shen_name(author));
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_ST_FadeInSubTitle(shenmue::s2::HLib::Task *task, float x, float y, std::int16_t alphaStep) {
    return shenmue::s2::ST_FadeInSubTitle(task, x, y, alphaStep);
}

void __attribute__((renesas)) shenmue_s2_event_EV_HLKillOtherHierarchyTask(shenmue::s2::HLib::Task *task) {
    shenmue::s2::HLib::EV_HLKillOtherHierarchyTask(task);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetMotionBoneTarget(
    shenmue::s2::MotionWork *work, std::int32_t selector, const shenmue::Vector3 *position) {
    return shenmue::s2::mtwkSetBoneTarget(work, selector, position);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_LatchMotionBoneTarget(
    shenmue::s2::MotionWork *work, std::int32_t selector, const shenmue::Vector3 *position) {
    return shenmue::s2::mtwkLatchBoneTarget(work, selector, position);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_MoveObjectToPosFromThisScene(
    std::uint32_t name, std::int32_t entry, shenmue::Vector3 *position, std::int32_t *yaw) {
    return shenmue::s2::MoveObjectToPosFromThisScene(shenmue::shen_name(name), entry, *position, *yaw);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetQronNextStream() {
    return shenmue::s2::GetQronNextStream();
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_FindAuthor(std::uint32_t name) {
    return shenmue::s2::FindAuthor(shenmue::shen_name(name));
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_ExecuteCurrAreaMainFunc(const char *name, std::uintptr_t argument) {
    return shenmue::s2::ExecuteAreaMainFunction(name, argument);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_ExecuteCurrAreaMainFunc_Wrapper(std::uint32_t slot, std::uintptr_t argument) {
    return shenmue::s2::ExecuteCurrentAreaMainFunction(static_cast<shenmue::s2::EventFunctionType>(slot), argument);
}

void __attribute__((renesas)) shenmue_s2_event_sctlAuthSeekSplines(std::uint32_t frame) {
    shenmue::s2::SeekAnimatedCameraSequence(frame);
}

void __attribute__((renesas)) shenmue_s2_event_SetRenderClockAndCompassTo(std::uint8_t suppressed) {
    shenmue::s2::shenSetClockAndCompassRenderTo(suppressed);
}

void *__attribute__((renesas)) shenmue_s2_event_LoadLegacySceneFileWithoutSize(const char *directory, const char *filename, std::uint32_t allocationName) {
    return shenmue::s2::shenLoadFile(directory, filename, shenmue::shen_name(allocationName));
}

std::uint32_t *__attribute__((renesas)) shenmue_s2_event_FindChunkInCurrentMapInfo(const char name[4]) {
    return reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::uintptr_t>(shenmue::s2::FindNativeChunk(name, shenmue::s2::GetCurrentMapInfo())) + 4);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_EV_GetScreenFilmIsNotShowing() {
    __typeof__(reinterpret_cast<const shenmue::s2::MapControlData *>(shenmue::s2::HLib::GetTaskParameter(shenmue::s2::g_mapControlTask))) const control =
        reinterpret_cast<const shenmue::s2::MapControlData *>(shenmue::s2::HLib::GetTaskParameter(shenmue::s2::g_mapControlTask));
    return shenmue::s2::IsScreenFilmNotShowing(control->filmTask);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_BeginPrimarySampleExecution(std::uint32_t disabled) {
    return shenmue::s2::StartSampleProgram(disabled, shenmue::s2::SelectSampleProgram(shenmue::s2::g_samplePrograms));
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_BeginSecondarySampleExecution(std::uint32_t disabled) {
    return shenmue::s2::StartSampleProgram(disabled, shenmue::s2::SelectSampleProgram(shenmue::s2::g_secondarySamplePrograms));
}

char *__attribute__((renesas)) shenmue_s2_event_GetCurrentBgm() {
    return shenmue::s2::g_activeSoundMapName;
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetSelectedSoundSceneIndex() {
    return shenmue::s2::g_currentSoundSceneIndex;
}

void __attribute__((renesas)) shenmue_s2_event_SetWeatherInstruction(std::uint32_t mask, std::int32_t enabled) {
    if (enabled)
        shenmue::s2::AddWeatherInstructionFlags(mask);
    else
        shenmue::s2::RemoveWeatherInstructionFlags(mask);
}

float __attribute__((renesas)) shenmue_s2_event_GetFogRed() {
    return shenmue::s2::g_fogState.nearDistance;
}

float __attribute__((renesas)) shenmue_s2_event_GetFogGreen() {
    return shenmue::s2::g_fogState.farDistance;
}

float __attribute__((renesas)) shenmue_s2_event_GetFogBlue() {
    return shenmue::s2::g_fogState.intensity;
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetFogColour() {
    return shenmue::s2::g_fogState.colour;
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetFogEnabled() {
    return shenmue::s2::g_fogState.enabled;
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_EV_LoadAudio(const char *name, std::uint32_t sceneIndex) {
    return shenmue::s2::LoadSoundProgram(name, sceneIndex, shenmue::s2::g_actNumber);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_LoadSoundProgramWithoutSelectedRecord(const char *name, std::uint32_t sceneIndex) {
    const std::uint32_t result = shenmue::s2::LoadSoundProgram(name, sceneIndex, shenmue::s2::g_actNumber);
    shenmue::s2::ResetSelectedSoundProgram();
    return result;
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_CreateCenteredWindowTextSurface(
    shenmue::s2::HLib::Task *task, const char *text, std::int32_t width, std::uint16_t colour, std::uint16_t shadow, std::int32_t conversionMode) {
    return shenmue::s2::CreateCenteredWindowTextSurface(task, text, width, colour, shadow, conversionMode);
}
}

#endif
