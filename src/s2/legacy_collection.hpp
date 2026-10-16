#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct LegacyCollectionWindow {
    float depth;
    float x;
    float y;
    std::int8_t row;
    std::int8_t column;
    std::int8_t columns;
    std::int8_t rows;
    std::int16_t labelTexture;
    std::int16_t quantity;
};
struct LegacyCollectionWork {
    std::uint8_t previousHelpTopic;
    std::uint8_t previousHelpMode;
    std::int16_t mode;
    std::int32_t selectedItem;
    std::int32_t firstItem;
    std::int32_t phase;
    std::uint32_t flags;
    std::int32_t count;
    LegacyCollectionWindow *window;
    std::int32_t page;
    std::uint32_t reserved24;
    std::uint32_t previousNpcLoad;
    std::uint32_t reserved2c;
};
static_assert(sizeof(LegacyCollectionWindow) == 0x14);
static_assert(offsetof(LegacyCollectionWindow, labelTexture) == 0x10);
static_assert(sizeof(LegacyCollectionWork) == 0x30);
static_assert(offsetof(LegacyCollectionWork, window) == 0x18);
static_assert(offsetof(LegacyCollectionWork, previousNpcLoad) == 0x28);

void InitializeLegacyCollectionMenu();
std::int64_t OpenLegacyCollectionMenu(std::int32_t mode);
std::int64_t UpdateLegacyCollectionMenu();
std::int64_t CloseLegacyCollectionMenu(std::int32_t force);

} // namespace shenmue::s2
