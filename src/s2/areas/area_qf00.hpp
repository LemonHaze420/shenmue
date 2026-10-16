#pragma once
#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <cstddef>
namespace shenmue::s2::areas {
struct Qf00FloorFbx2Placement {
    Vector3 position;
    std::int32_t particleHandle;
};
static_assert(sizeof(Qf00FloorFbx2Placement) == 16 && offsetof(Qf00FloorFbx2Placement, particleHandle) == 12);

inline Qf00FloorFbx2Placement qf00FloorFbx2Placement{
    .position = {.x = 223.30999755859375f, .y = 129.6999969482422f, .z = 178.77000427246094f}, .particleHandle = -1};
void AreaQF00Init();
void AreaQF00Main(std::uint32_t startParameters);
void AreaQF00Cleanup(std::int32_t afternoon, std::uint32_t startParameters);
extern const GameplayAreaDefinition areaQF00GameplayDefinition;
} // namespace shenmue::s2::areas
