#pragma once

#include "s2/character_dc.hpp"
#include "s2/player_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const std::size_t kPlayerSearchLocationOffset = 0x58, kPlayerInterpolationOffset = 0x8c, kPlayerMotionStateOffset = 0x1b0;
static const std::size_t kPlayerControllerGroundBlendOffset = 0x290, kPlayerControllerVerticalAxisOffset = 0x298;
static const std::size_t kPlayerFirstControllerDeltaOffset = 0x2b8, kPlayerSecondControllerDeltaOffset = 0x2c4, kPlayerSpeedZOffset = 0x4e4;
std::int32_t shenGetRotYFromNode(const Character *character);
std::int32_t playerGetRotY();
Vector3 *plymUpdatePlayerSearchLocation(Vector3 *position);
std::uint32_t plymGetAuxiliaryState();
float plymGetSpeedZ();
float plymGetInterpolationValue();
void plymGetControllerVerticalAxis(float *first, float *second);
void plymGetControllerGroundBlendedXZ(float *first, float *second);
void plymGetControllerDeltaXZ(Vector3 *first, Vector3 *second);

} } // namespace shenmue::s2

#endif
