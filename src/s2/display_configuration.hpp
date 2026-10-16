#pragma once

#include "d3t/dx11_renderer.hpp"
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace shenmue::s2 {
struct S2DisplayOutput {
    char name[64];
    std::int32_t left;
    std::int32_t top;
    std::int32_t right;
    std::int32_t bottom;
    std::uint8_t primary;
    std::byte reserved51[7];
    d3t::DisplayModeInfo *modes;
    std::uint32_t modeCount;
    std::uint32_t modeCapacity;
};
static_assert(sizeof(S2DisplayOutput) == 104);
static_assert(offsetof(S2DisplayOutput, modes) == 88);
static_assert(offsetof(S2DisplayOutput, modeCount) == 96);

void EnumerateS2Displays();
void ReleaseS2Displays();
const S2DisplayOutput *GetS2DisplayOutput(std::int32_t index) noexcept;
const d3t::DisplayModeInfo *GetS2DisplayResolution(std::int32_t display, std::int32_t index) noexcept;
std::int32_t GetS2PrimaryDisplayIndex() noexcept;
std::int32_t GetS2DesktopResolutionIndex(std::int32_t display) noexcept;
std::string_view GetS2DisplayOutputName(std::int32_t index) noexcept;
std::string_view GetS2DisplayResolutionName(std::int32_t display, std::int32_t index) noexcept;
}
