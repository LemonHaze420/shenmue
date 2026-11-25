#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {

struct FogState {
    float nearDistance, farDistance, intensity;
    std::uint32_t colour;
    std::int32_t enabled, dirty;
    float publishedNearDistance, publishedFarDistance, publishedIntensity;
};
typedef char Layout_fog_dc_0[(sizeof(FogState) == 0x24) ? 1 : -1];
typedef char Layout_fog_dc_1[(offsetof(FogState, colour) == 0x0c) ? 1 : -1];
typedef char Layout_fog_dc_2[(offsetof(FogState, dirty) == 0x14) ? 1 : -1];
typedef char Layout_fog_dc_3[(offsetof(FogState, publishedNearDistance) == 0x18) ? 1 : -1];

extern FogState g_fogState;
extern float g_fogIntensityTable[128];
void SetupDefaultFogParams();
void ApplyFogDefinition(void *definition);
void LoadFogDefinition(std::int32_t index);
void ResetFogState();
void QueueFogClear();
void QueueFogPublication();
void ClearFogParameters(void *);
void PublishFogParameters(void *);
void UpdateFogParameters();
void SetFogRed(float value);
void SetFogGreen(float value);
void SetFogBlue(float value);
void SetFogDistance(std::uint32_t colour);
void ReinitFogTasks(std::int32_t enabled);

} } // namespace shenmue::s2

#endif
