#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/air_data.hpp"
#include "s2/model_dc.hpp"
#include "s2/hlib.hpp"

namespace shenmue {
namespace s2 {

struct AirCloudLayer {
    std::uint32_t *packets, *packetEnd, *header;
    MT7Model *model;
    Vector2 scroll;
    Vector2 retainedScroll;
    Vector2 scrollRate;
    std::uint32_t firstColour, secondColour;
    std::uint16_t colours[8][2];
    float blend, blendStep, blendScale;
    std::int32_t textureHandle, textureIndex;
    Vector3 offset;
};
struct AirDomeLayer {
    std::uint32_t *packets, *packetEnd, *header;
    std::uint32_t firstColour, secondColour;
    std::uint16_t colours[5][2];
    float blend;
};
struct AirDrawState {
    MT7Model *domes[2];
    std::int32_t planetState;
    MT7Model *planet;
    std::int32_t transitionActive, transitionFrames;
    std::uint32_t clearColour;
    std::int32_t drawEnabled;
    float timeOfDay;
    std::uint32_t previousSeconds, currentSeconds;
};
struct AirDrawTaskParameters {
    shen_name name;
    std::int32_t mode;
};
typedef char Layout_air_dc_0[(sizeof(AirDrawTaskParameters) == 8) ? 1 : -1];
typedef char Layout_air_dc_1[(sizeof(AirCloudLayer) == 0x70) ? 1 : -1];
typedef char Layout_air_dc_2[(offsetof(AirCloudLayer, scroll) == 0x10) ? 1 : -1];
typedef char Layout_air_dc_3[(offsetof(AirCloudLayer, scrollRate) == 0x20) ? 1 : -1];
typedef char Layout_air_dc_4[(offsetof(AirCloudLayer, colours) == 0x30) ? 1 : -1];
typedef char Layout_air_dc_5[(offsetof(AirCloudLayer, blend) == 0x50) ? 1 : -1];
typedef char Layout_air_dc_6[(offsetof(AirCloudLayer, textureHandle) == 0x5c) ? 1 : -1];
typedef char Layout_air_dc_7[(offsetof(AirCloudLayer, offset) == 0x64) ? 1 : -1];
typedef char Layout_air_dc_8[(sizeof(AirDomeLayer) == 0x2c) ? 1 : -1];
typedef char Layout_air_dc_9[(offsetof(AirDomeLayer, colours) == 0x14) ? 1 : -1];
typedef char Layout_air_dc_10[(offsetof(AirDomeLayer, blend) == 0x28) ? 1 : -1];
typedef char Layout_air_dc_11[(sizeof(AirDrawState) == 0x2c) ? 1 : -1];
typedef char Layout_air_dc_12[(offsetof(AirDrawState, transitionActive) == 0x10) ? 1 : -1];
typedef char Layout_air_dc_13[(offsetof(AirDrawState, timeOfDay) == 0x20) ? 1 : -1];
#define AIR_DRAW_TASK SHEN_NAME4('A', 'I', 'R', 'D')
#define AIR_MODEL_STORAGE SHEN_NAME4('D', 'O', 'O', 'M')
static const std::uint8_t airCloudVertexLevels[84] = {7, 7, 7, 6, 7, 5, 7, 4, 7, 4, 7, 4, 7, 5, 7, 6, 7, 7, 7, 5, 4, 3, 2, 3, 4, 5, 7, 7, 4, 3, 2, 1, 2, 3, 4,
    7, 7, 4, 2, 1, 0, 1, 2, 4, 7, 7, 4, 3, 2, 1, 2, 3, 4, 7, 7, 5, 4, 3, 2, 3, 4, 5, 7, 7, 6, 5, 4, 4, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0};
static const std::uint8_t airDomeVertexLevels[72] = {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 3, 2, 3, 2, 3, 4, 4, 4, 4, 4, 3, 3, 2, 2, 2, 3,
    4, 1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 3, 2, 3, 3, 4, 4, 4, 1, 2, 2, 1, 2, 4, 4, 3, 3, 2, 2, 3, 4, 2, 1, 0, 0, 0};
static const float airCloudColourLevels[8] = {0.0f, 1.0f, 1.5f, 2.0f, 5.0f, 7.0f, 10.0f, 10.0f};
static const float airDomeColourLevels[5] = {0.0f, 1.0f, 2.0f, 3.0f, 10.0f};
static const std::int32_t planetRotationDays[3] = {20, 41, 62};
extern AirCloudLayer g_airCloudLayers[2];
extern AirDomeLayer g_airDomeLayers[2];
extern AirPlanetDrawRecord g_airPlanet[1];
extern AirDrawState g_airDrawState;

void InitializeAirLayers();
void ResetAirDrawState();
void SetPlanetRotByIndex(const Vector3i &rotation, std::int32_t index);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetIsSkySetup(std::int32_t value);
void SetPlanetLayerParameter(float value, std::int32_t layer);
float GetPlanetLayerParameter(std::int32_t layer);
std::uint32_t GetPlanetLayerColour(std::int32_t channel, std::int32_t layer);
void SetPlanetLayerOffset(const Vector3 *offset, std::int32_t layer);
void ResetPlanetLayerOffset(std::int32_t layer);
#endif
void shenUpdatePlanetByLayer(float value, std::int32_t channel, std::int32_t layer);
void EV_SetPlanetLayerColours_Impl(std::uint32_t first, std::uint32_t second, std::int32_t layer);
void SetCloudAndSkyColour(std::uint32_t first, std::uint32_t second);
std::uint32_t GetWeatherCloudOrSkyColour(std::int32_t channel);
void LoadAirTextures(std::int32_t texture, float scale, std::int32_t layer);
std::int32_t IsAirTextureTransitionActive();
std::int32_t BeginAirTextureTransition(std::int32_t texture, std::int32_t frames, std::int32_t fadePrevious, float scale);
void CancelAirTextureTransition();
void AdvanceAirTextureTransition();
void UpdateAirTimeOfDay();
void UpdateAirSeconds();
void UpdateCelestialCoordinates();
void UpdateCelestialPosition();
void UpdateAirDomeBlend();
void UpdateAirCloudColours(std::int32_t layer);
void UpdateAirDomeColours();
void UpdateAirMeshPackets();
void InitializeAirPresentation(const void *mapInfo);
void AirDrawCallback(void *parameter);
void DrawAir(void *parameter);
HLib::Task *Queue_CtrlAir(std::uint8_t index);

} // namespace s2
} // namespace shenmue

#endif
