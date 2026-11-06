#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::host {

inline constexpr std::size_t KeyCount = 261;
inline constexpr std::size_t ControllerCount = 4;

struct Controller {
    bool connected{};
    std::uint16_t buttons{};
    std::uint8_t leftTrigger{}, rightTrigger{};
    std::int16_t leftX{}, leftY{}, rightX{}, rightY{};
};

struct InputSnapshot {
    std::array<bool, KeyCount> keys{};
    std::array<Controller, ControllerCount> controllers{};
    std::int32_t mouseX{}, mouseY{};
    std::int32_t width{}, height{};
    std::int32_t leftClicks{};
    float wheel{};
    bool focused{}, changed{};
};

#if defined(SHENMUE_GFX_DXVK_NATIVE)
bool Initialize();
void PollInput(InputSnapshot &input, bool consumeEvents = true);
std::int32_t ConsumeEnterPresses();
void ReadController(std::size_t index, Controller &controller);
void SetStatus(const char *text);
void RequestQuit();
bool QuitRequested();
void *CreateWindow(std::int32_t width, std::int32_t height);
void ResizeWindow(std::int32_t width, std::int32_t height);
#endif

} // namespace shenmue::host
