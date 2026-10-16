#include "s2/areas/harbor_author_event.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/weather.hpp"
#include <array>
namespace shenmue::s2::areas {
// @verified - p
void HarborAuthorWaveCallback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.75f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.75f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.8999999761581421f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.3100000023841858f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3100000023841858f,
            static_cast<float>(specular & 255U) / 255.0f * 0.30000001192092896f},
        42.540000915527344f, 1.0f);
    SetMapModelSlotEnabled(3, 0);
    SetMapModelSlotEnabled(12, 0);
    SetMapModelSlotEnabled(15, 0);
    AdvanceWaterTextureAnimation();
}
// @verified - p
void HarborAuthorShipCallback(void *) {
    UpdateFloatingSurfaceModels(2, HARBOR_AUTHOR_FLOATING_MODELS);
}
// @verified - p
void HarborAuthorShip0059Callback(void *) {
    UpdateFloatingSurfaceModels(2, HARBOR_AUTHOR_FLOATING_MODELS);
}
} // namespace shenmue::s2::areas
