#pragma once
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
inline constexpr FloatingSurfaceBinding HARBOR_AUTHOR_FLOATING_MODELS[]{{5, {132.10000610351562f, 93.9280014038086f, 492.1600036621094f}},
    {6, {132.10000610351562f, 93.9280014038086f, 524.1599731445312f}}, {7, {132.10000610351562f, 93.9280014038086f, 556.1599731445312f}},
    {8, {100.0999984741211f, 93.9280014038086f, 524.1599731445312f}}, {9, {100.0999984741211f, 93.9280014038086f, 556.1599731445312f}}, {-1, {0, 0, 0}}};
void HarborAuthorWaveCallback(void *parameter);
void HarborAuthorShipCallback(void *parameter);
void HarborAuthorShip0059Callback(void *parameter);
} // namespace shenmue::s2::areas
