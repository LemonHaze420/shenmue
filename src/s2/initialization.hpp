#pragma once
#include <cstdint>

namespace shenmue::s2 {

int Shenmue2_Main(int argc, char **argv);
[[noreturn]] void ShutdownS2Runtime();
std::int32_t GetRootRenderDeferredMode() noexcept;
void SetRootRenderDeferredMode(std::int32_t mode) noexcept;
void SetUnusedEventState(std::int32_t state) noexcept;
void SetShouldReturnToTitleScreen(std::int32_t requested) noexcept;
std::int32_t GetShouldReturnToTitleScreen() noexcept;
void SetRootSoftResetEnabled(std::int32_t enabled) noexcept;

} // namespace shenmue::s2
