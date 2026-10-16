#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name ELEVATOR_LAMP_CHARACTER{SHEN_NAME(TLIG)};
struct ElevatorLampChoice {
    shen_name elevator;
    std::int32_t flag;
};
inline constexpr std::array<ElevatorLampChoice, 3> ELEVATOR_LAMP_CHOICES{{
    {.elevator = SHEN_NAME(R599), .flag = 627},
    {.elevator = SHEN_NAME(R598), .flag = 629},
    {.elevator = SHEN_NAME(R597), .flag = 630},
}};
inline constexpr Vector3 ELEVATOR_LAMP_OFFSET{.x = 0.0f, .y = 2.6949999f, .z = 0.2f};
inline constexpr Vector4 ELEVATOR_LAMP_COLOUR{.x = 1.0f, .y = 1.0f, .z = 0.80000001f, .w = 0.5f};
std::int64_t SetElevatorLamp(shen_name elevator, std::int32_t enabled);
std::int64_t ConfigureAreaElevatorLamp(std::int32_t enabled = 1);
void AreaElevatorLampInit();
void AreaElevatorLampMainLoop(void *data);
void AreaElevatorLampCleanup(std::int32_t afternoon, std::uint32_t areaIndex);

} // namespace shenmue::s2::areas
