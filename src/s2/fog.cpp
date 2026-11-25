#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/fog_dc.hpp"

#include "platform/dc/ninja.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/hlib.hpp"
#include "s2/map_info.hpp"
#include <cstring>
#include <cmath>
#include "s2/screen_film_dc.hpp"
#include "s2/event_film_dc.hpp"
#include "s2/scenario_subtitles_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "shenmue.h"
#include "s2/map_load_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/window.hpp"
#include "s2/input_dc.hpp"
#include <sg_pad.h>
#include "s2/dynamic_tracks.hpp"
#include "s2/pak_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/map_vertex_cache_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include <cstdio>
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/search_dc.hpp"
#include "s2/map_draw_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/model_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include <math.h>
#include "s2/scalar_math_dc.hpp"
#include <float.h>
#define SHENMUE_SCALAR_NOEXCEPT
#define SHENMUE_SCALAR_ABS ::fabsf
#define SHENMUE_SCALAR_SIGNED_ANGLE(value) static_cast<std::int16_t>(value)
#else
#include "s2/fog.hpp"

#include "s2/legacy_draw.hpp"

#include <cmath>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

struct FogState {
    float nearDistance = 20.0f;
    float farDistance = 800.0f;
    float intensity = 1.0f;
    float publishedNearDistance = 20.0f;
    float publishedFarDistance = 800.0f;
    float publishedIntensity = 1.0f;
    std::uint32_t colour = 0xffffffffU;
    bool enabled = true;
    bool dirty = false;
};

FogState fog;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

void PublishFogParameters();

} // namespace

void SetupDefaultFogParams();
std::uint64_t SetFogRed(float value);
std::uint64_t SetFogGreen(float value);
std::uint64_t SetFogBlue(float value);
std::uint64_t SetFogDistance(std::uint32_t colour);
void ReinitFogTasks(std::int32_t enabled);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint64_t SetFogScalar(float &current, float published, float value) {
    current = value;

    if (std::abs(published - value) > std::abs(value) * 0.002) {
        fog.dirty = true;
    }
    return static_cast<std::uint32_t>(fog.dirty);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ZeroFogParams() {
    if (!fog.dirty)
        return;
    fog.dirty = false;
    SetLegacyFogEnabled(0);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void QueueFogClear();

void SetFogRed(float value);

void SetFogGreen(float value);

void SetFogBlue(float value);

void SetFogDistance(std::uint32_t colour);

void ReinitFogTasks(std::int32_t enabled);

const std::byte *dynaRead(const std::uint8_t name[4]);

void dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation);

void SetMapLightingPlacementPosition(const Vector3 *position, std::int32_t index);

void SetMapLightingPlacementRotation(std::int32_t value, std::int32_t index);

FogState g_fogState = {};

float g_fogIntensityTable[128] = {};

void SCEN_ShowEventMessage(HLib::Task *scenarioTask, std::int32_t mode, const char *message);

void ClearWindowText(WindowSlot *window);

void SetWindowColourMode(WindowSlot *window, std::int32_t mode);

void RevealWindowGlyphs(WindowSlot *window);

void InitializeScenarioTextDecoder(std::int32_t mode, const char *text, ScenarioTextDecoder &decoder);

std::int32_t FindScenarioTextOpeningQuote(std::int32_t mode, ScenarioTextDecoder &decoder);

std::int32_t AdvanceScenarioTextDecoder(ScenarioTextDecoder &decoder);

void SCNF_RenderWindowText(WindowSlot *window, ScenarioWindowTextState *state);

void scenDestroySubtitles(HLib::Task *task);

void shenQueueChangeModuleAndScene(SystemModule module, std::int32_t actNumber, shen_name areaId, std::int32_t entryId);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RequestForcedScenarioSubtitles(HLib::Task *task);
static MapInfoChunk *FogFindChunk(std::uint32_t name, MapInfoChunk *chunk);
void ApplyFogDefinition(void *definition);
void LoadFogDefinition(std::int32_t index);
void ClearFogParameters(void *);
void UpdateFogParameters();
void PublishFogParameters(void *);
void QueueFogPublication();
void ResetFogState();
void SetupDefaultFogParams();
void QueueFogClear();
void SetFogRed(float value);
float GetFogRed();
void SetFogGreen(float value);
float GetFogGreen();
void SetFogBlue(float value);
float GetFogBlue();
void SetFogDistance(std::uint32_t colour);
std::uint32_t GetFogColour();
void ReinitFogTasks(std::int32_t enabled);
std::int32_t GetFogEnabled();
bool filmCheckFadeTrackColour();
void SetScreenFilmFade(std::uint32_t frames, std::uint8_t startAlpha, std::uint8_t startRed, std::uint8_t startGreen, std::uint8_t startBlue,
    std::uint8_t targetAlpha, std::uint8_t targetRed, std::uint8_t targetGreen, std::uint8_t targetBlue);
void EV_SetCinemaBarVisibility(FilmControlFlag mode, std::int32_t frames);
void dbgPrintEventMessage(const char *message);
void RequestScenarioWindowRecovery(HLib::Task *scenarioTask);
void ctrlStopSubtitles();
bool scenHasActiveSubtitles(HLib::Task *task);
void ForceScenarioSubtitles(HLib::Task *task);
void EV_scenQueueLoad(std::int32_t actNumber, shen_name areaId, std::int32_t entryId);
void dynaReadEvaluate(std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation, const std::uint8_t name[4]);
void SetPropSlotTransform(std::int32_t index, const Vector3 *position, std::int32_t rotationY);
void scenProjectToScreen(const Vector3 *position, Vector2 *screen);
#else
namespace {

std::uint64_t SetFogScalar(float &current, float published, float value);
void ZeroFogParams();
void PublishFogParameters();

} // namespace

void SetupDefaultFogParams();
std::uint64_t SetFogRed(float value);
std::uint64_t SetFogGreen(float value);
std::uint64_t SetFogBlue(float value);
std::uint64_t SetFogDistance(std::uint32_t colour);
float GetFogRed();
float GetFogGreen();
float GetFogBlue();
std::uint64_t GetFogColour();
std::uint64_t GetFogEnabled();
void ReinitFogTasks(std::int32_t enabled);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static MapInfoChunk *FogFindChunk(std::uint32_t name, MapInfoChunk *chunk) {
    while (chunk->name != name) {
        if (!chunk->nextByteOffset)
            return 0;
        chunk = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(chunk) + chunk->nextByteOffset);
    }
    return chunk;
}

void ApplyFogDefinition(void *definition) {
    MapInfoChunk *const first = static_cast<MapInfoChunk *>(definition);
    const float *parameters = reinterpret_cast<const float *>(FogFindChunk(SHEN_NAME4('F', 'O', 'G', ' '), first));
    parameters += 2;
    FogFindChunk(SHEN_NAME4('B', 'A', 'C', 'K'), first);
    SetFogRed(parameters[0]);
    SetFogGreen(parameters[1]);
    SetFogBlue(parameters[2]);
    SetFogDistance(reinterpret_cast<const std::uint32_t *>(parameters)[3]);
    ReinitFogTasks(reinterpret_cast<const std::int32_t *>(parameters)[4]);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void LoadFogDefinition(std::int32_t index) {
    MapInfoChunk *chunk = FindNativeChunk("FOG ", GetCurrentMapInfo());
    ++chunk;
    ApplyFogDefinition(reinterpret_cast<std::byte *>(chunk) + static_cast<std::uint32_t>(index) * 0x50U);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ClearFogParameters(void *) {
    if (!g_fogState.dirty)
        return;
    g_fogState.dirty = 0;
    for (std::int32_t index = 0; index < 128; ++index)
        g_fogIntensityTable[index] = 0.0f;
    njSetFogTable(g_fogIntensityTable);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool filmCheckFadeTrackColour() {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask)));
    return DoesScreenFilmColourExist(control->filmTask);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetScreenFilmFade(std::uint32_t frames, std::uint8_t startAlpha, std::uint8_t startRed, std::uint8_t startGreen, std::uint8_t startBlue,
    std::uint8_t targetAlpha, std::uint8_t targetRed, std::uint8_t targetGreen, std::uint8_t targetBlue) {
    ScreenFilmColourRequest request;
    request.durationFrames = SHENMUE_FILM_SIGNED_FRAMES(frames);
    request.start[0] = startAlpha;
    request.start[1] = startRed;
    request.start[2] = startGreen;
    request.start[3] = startBlue;
    request.target[0] = targetAlpha;
    request.target[1] = targetRed;
    request.target[2] = targetGreen;
    request.target[3] = targetBlue;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask)));
    WaitForNextTask(control->filmTask, request, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EV_SetCinemaBarVisibility(FilmControlFlag mode, std::int32_t frames) {
    if (!frames)
        mode = static_cast<FilmControlFlag>(static_cast<std::uint32_t>(mode) | static_cast<std::uint32_t>(FF_Force));
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask)));
    filmSetCinemaBarVisibility(control->filmTask, mode, frames);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void dbgPrintEventMessage(const char *message) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SCEN_ShowEventMessage(control->scenarioTask, 1, message);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ctrlStopSubtitles() {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    scenDestroySubtitles(control->scenarioTask);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EV_scenQueueLoad(std::int32_t actNumber, shen_name areaId, std::int32_t entryId) {
    shenQueueChangeModuleAndScene(actNumber < 0 ? SystemModule_Credits : SystemModule_Game, actNumber, areaId, entryId);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void dynaReadEvaluate(std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation, const std::uint8_t name[4]) {
    dynaEvaluate(dynaRead(name) + 8, track, frame, position, rotation);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetPropSlotTransform(std::int32_t index, const Vector3 *position, std::int32_t rotationY) {
    SetMapLightingPlacementPosition(position, index);
    SetMapLightingPlacementRotation(rotationY, index);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void scenProjectToScreen(const Vector3 *position, Vector2 *screen) {
    matrix_stack::UpdateWorldToScreenMatrix(640.0f, 480.0f);
    matrix_stack::ProjectWorldPointToScreen(position, screen);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PublishFogParameters(void *) {
    if (!g_fogState.dirty)
        return;
    g_fogState.dirty = 0;
    g_fogState.publishedNearDistance = g_fogState.nearDistance;
    g_fogState.publishedFarDistance = g_fogState.farDistance;
    g_fogState.publishedIntensity = g_fogState.intensity;
    UpdateFogParameters();
    njSetFogTable(g_fogIntensityTable);
}

#else

namespace {

void PublishFogParameters() {
    if (!fog.dirty)
        return;
    fog.publishedNearDistance = fog.nearDistance;
    fog.publishedFarDistance = fog.farDistance;
    fog.publishedIntensity = fog.intensity;
    fog.dirty = false;




    SetLegacyFogColour(fog.colour);
    SetLegacyFogEnabled(1);
    SetLegacyDepthFadeRange(fog.nearDistance, fog.farDistance);
    SetLegacyDepthFadeBlue(fog.intensity);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void QueueFogPublication() {
    HLib::SetCurrentTaskCustomUserData(PublishFogParameters, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void QueueFogClear() {
    HLib::SetCurrentTaskCustomUserData(ClearFogParameters, 0);
}

void SetFogRange(float nearDistance, float farDistance) {
    if (std::fabs(g_fogState.publishedNearDistance - nearDistance) > std::fabs(nearDistance) * 0.002f ||
        std::fabs(g_fogState.publishedFarDistance - farDistance) > std::fabs(farDistance) * 0.002f)
        g_fogState.dirty = 1;
    g_fogState.nearDistance = nearDistance;
    g_fogState.farDistance = farDistance;
    QueueFogPublication();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetFogState() {
    g_fogState.colour = 0xffffffffU;
    g_fogState.enabled = 1;
    g_fogState.nearDistance = 20.0f;
    g_fogState.farDistance = 800.0f;
    g_fogState.intensity = 1.0f;
    g_fogState.dirty = 1;
    g_fogState.publishedNearDistance = g_fogState.nearDistance;
    g_fogState.publishedFarDistance = g_fogState.farDistance;
    g_fogState.publishedIntensity = g_fogState.intensity;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetupDefaultFogParams() {
    ResetFogState();
    QueueFogClear();
    QueueFogPublication();
}

static std::int32_t fogColourComponents[4];

std::int32_t *UnpackFogColour(std::uint32_t colour) {
    for (std::uint8_t index = 0; index < 4; ++index) {
        fogColourComponents[index] = (colour & 0xff000000U) >> 24;
        colour <<= 8;
    }
    return fogColourComponents;
}

#else

void SetupDefaultFogParams() {
    fog = {};
    fog.dirty = true;



    ZeroFogParams();
    PublishFogParameters();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetFogRed(float value) {
    if (std::fabs(g_fogState.publishedNearDistance - value) > std::fabs(value) * 0.002f)
        g_fogState.dirty = 1;
    g_fogState.nearDistance = value;
}

float GetFogRed() {
    return g_fogState.nearDistance;
}

#else

// @verified - p
std::uint64_t SetFogRed(float value) {
    return SetFogScalar(fog.nearDistance, fog.publishedNearDistance, value);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetFogGreen(float value) {
    if (std::fabs(g_fogState.publishedFarDistance - value) > std::fabs(value) * 0.002f)
        g_fogState.dirty = 1;
    g_fogState.farDistance = value;
}

float GetFogGreen() {
    return g_fogState.farDistance;
}

#else

// @verified - p
std::uint64_t SetFogGreen(float value) {
    return SetFogScalar(fog.farDistance, fog.publishedFarDistance, value);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetFogBlue(float value) {
    if (std::fabs(g_fogState.publishedIntensity - value) > std::fabs(value) * 0.002f)
        g_fogState.dirty = 1;
    g_fogState.intensity = value;
}

float GetFogBlue() {
    return g_fogState.intensity;
}

#else

// @verified - p
std::uint64_t SetFogBlue(float value) {
    return SetFogScalar(fog.intensity, fog.publishedIntensity, value);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetFogDistance(std::uint32_t colour) {
    if (g_fogState.colour != colour)
        g_fogState.dirty = 1;
    g_fogState.colour = colour;
}

std::uint32_t GetFogColour() {
    return g_fogState.colour;
}

#else

// @verified - p
std::uint64_t SetFogDistance(std::uint32_t colour) {
    if (fog.colour != colour)
        fog.dirty = true;
    fog.colour = colour;
    return static_cast<std::uint32_t>(fog.dirty);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
float GetFogRed() {
    return fog.nearDistance;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
float GetFogGreen() {
    return fog.farDistance;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
float GetFogBlue() {
    return fog.intensity;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint64_t GetFogColour() {
    return fog.colour;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint64_t GetFogEnabled() {
    return static_cast<std::uint32_t>(fog.enabled);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReinitFogTasks(std::int32_t enabled) {
    if (enabled == 1) {
        if (g_fogState.enabled != 1)
            g_fogState.dirty = 1;
        g_fogState.enabled = 1;
        QueueFogPublication();
    } else {
        if (g_fogState.enabled != 0)
            g_fogState.dirty = 1;
        g_fogState.enabled = 0;
        QueueFogClear();
    }
}

std::int32_t GetFogEnabled() {
    return g_fogState.enabled;
}

#else

void ReinitFogTasks(std::int32_t enabled) {
    const bool nextEnabled = enabled == 1;
    if (fog.enabled != nextEnabled)
        fog.dirty = true;
    fog.enabled = nextEnabled;
    if (nextEnabled) {
        PublishFogParameters();
    } else {
        ZeroFogParams();
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void UpdateFogParameters() {
    if (g_fogState.enabled == 1) {
        njSetFogColor(g_fogState.colour);
        dc::sdk::ComputeFogAttenuation(g_fogIntensityTable, -g_fogState.nearDistance, -g_fogState.farDistance);
        dc::sdk::ScaleFogIntensity(g_fogIntensityTable, g_fogState.intensity);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ForceScenarioSubtitles(HLib::Task *task) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(task));
    WindowSlot *const window = scenario->windowDialog;
    ScenarioWindowTextState *const text = &scenario->windowTextState;
    if (text->scenarioStateFlags & SSF_Suppressed)
        return;
    if (text->scenarioStateFlags & 0x02)
        return;
    if (text->scenarioStateFlags & SSF_PageReady) {
        ShowWindowSlot(window);
        text->scenarioStateFlags ^= SSF_PageReady;
    } else {
        text->scenarioStateFlags |= SSF_AdvanceRequested;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RequestScenarioWindowRecovery(HLib::Task *scenarioTask) {
    ScenarioWindowTextState *const text = &static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask))->windowTextState;
    text->surfaceRecoveryFrames = 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool scenHasActiveSubtitles(HLib::Task *task) {
    const ScenarioWindowTextState *const text = &static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(task))->windowTextState;
    if (text->scenarioStateFlags & SSF_InactiveMask)
        return false;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RequestForcedScenarioSubtitles(HLib::Task *task) {
    ScenarioWindowTextState *const text = &static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(task))->windowTextState;
    text->scenarioStateFlags |= SSF_ForceSubtitles;
}
#endif

#endif

} // namespace s2
} // namespace shenmue
