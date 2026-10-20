#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TASK_DAKU{SHEN_NAME(DAKU)};
inline constexpr std::uint32_t DAKU_SPRAY_DEFAULT_COLOUR = 0x3effffffU;
extern std::array<std::int32_t, 3> g_dakuSpraySlots;
extern std::array<Vector3, 3> g_dakuSprayPositions;
extern std::array<std::uint32_t, 3> g_dakuSprayColours;
extern std::array<Vector2, 3> g_dakuSpraySizes;
void ResetDakuSpray();
void InitializeDakuSpray();
void DakuSprayControlCallback(void *);
void SetDakuSprayColour(std::uint32_t emitter, std::uint32_t colour);
void SetDakuSprayPosition(std::uint32_t emitter, const Vector3 &position);
void SetDakuSpraySize(std::uint32_t emitter, float width, float height);

} // namespace shenmue::s2
