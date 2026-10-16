#pragma once

#include <cstdint>

namespace shenmue::s2 {

void SetupDefaultFogParams();
std::uint64_t SetFogRed(float value);
std::uint64_t SetFogGreen(float value);
std::uint64_t SetFogBlue(float value);
std::uint64_t SetFogDistance(std::uint32_t colour);
float GetFogRed();
float GetFogGreen();
float GetFogBlue();
std::uint64_t GetFogColour();
std::uint64_t GetFogEnabled();
void ReinitFogTasks(std::int32_t enabled);
std::int64_t SetAreaFogParams();

} // namespace shenmue::s2
