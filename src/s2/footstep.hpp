#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

#pragma pack(push, 4)
struct FootstepComponent {
    std::uint8_t surface;
    std::uint8_t enabled;
    std::int8_t authoredMode;
};

#pragma pack(pop)

static_assert(sizeof(FootstepComponent) == 0x03);
static_assert(offsetof(FootstepComponent, surface) == 0x00);
static_assert(offsetof(FootstepComponent, enabled) == 0x01);
static_assert(offsetof(FootstepComponent, authoredMode) == 0x02);

FootstepComponent *FindFootstepComponent(const Character *character);
void AttachFootstepComponent(Character *character, const char *surface, std::int8_t authoredMode);
std::int32_t ResolveFootstepGroundToken(Character *character);
std::uint32_t ResolveFootstepMaterialKey(const Character *character, std::uint16_t stepId, std::uint32_t material);

} // namespace shenmue::s2
