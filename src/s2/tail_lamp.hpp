#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

#pragma pack(push, 4)
constexpr shen_name kTailLampToken{SHEN_NAME(LAMP)};
constexpr std::uint32_t kTailLampLightSlot = 13;
constexpr std::uint32_t kTailLampNoLightSlot = 17;
inline constexpr Vector4 kTailLampColour{1.0f, 1.0f, 0.0f, 0.0f};

struct TailLampResource {
    std::vector<std::byte> bytes;
    MT7Model model;
};

#pragma pack(push, 4)
struct TailLampComponent {
    TailLampResource *resource;
    std::uint32_t lightSlot;
    std::uint32_t active;
    std::int32_t lifetime;
#if SHENMUE_IS_PC
    std::uint32_t reserved14;
#endif
};
#pragma pack(pop)

struct TailLampTaskData {
    Matrix4x4 transform;
    TailLampResource *resource;
    float alpha;
    float fadeDelta;
};

static_assert(offsetof(TailLampComponent, resource) == 0x00);
static_assert(offsetof(TailLampTaskData, resource) == 0x40);
#if SHENMUE_IS_PC
static_assert(sizeof(TailLampComponent) == 0x18);
static_assert(offsetof(TailLampComponent, lightSlot) == 0x08);
static_assert(offsetof(TailLampComponent, active) == 0x0c);
static_assert(offsetof(TailLampComponent, lifetime) == 0x10);
static_assert(sizeof(TailLampTaskData) == 0x50);
static_assert(offsetof(TailLampTaskData, alpha) == 0x48);
static_assert(offsetof(TailLampTaskData, fadeDelta) == 0x4c);
#else
static_assert(sizeof(TailLampComponent) == 0x10);
static_assert(offsetof(TailLampComponent, lightSlot) == 0x04);
static_assert(offsetof(TailLampComponent, active) == 0x08);
static_assert(offsetof(TailLampComponent, lifetime) == 0x0c);
static_assert(sizeof(TailLampTaskData) == 0x4c);
static_assert(offsetof(TailLampTaskData, alpha) == 0x44);
static_assert(offsetof(TailLampTaskData, fadeDelta) == 0x48);
#endif
#pragma pack(pop)

bool AttachTailLampComponent(Character *character);

} // namespace shenmue::s2
