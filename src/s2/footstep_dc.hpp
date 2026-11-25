#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

#define CHARACTER_FOOTSTEP SHEN_NAME4('S', 'T', 'E', 'P')
typedef std::uint8_t FootstepSurface;
enum { FootstepSoft = 0, FootstepHard = 1, FootstepNormal = 2 };
struct FootstepComponent {
    std::uint8_t surface, enabled;
    std::int8_t authoredMode;
};
typedef char Layout_footstep_dc_0[(sizeof(FootstepComponent) == 3) ? 1 : -1];
std::int32_t ResolveFootstepGroundToken(Character *character);
std::uint32_t ResolveFootstepMaterialKey(Character *character, std::uint32_t stepToken, std::uint32_t flags);
void AttachFootstepComponent(Character *character, const char *surface, std::int32_t collisionFallback);

} } // namespace shenmue::s2

#endif
