#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {
struct LegacyCollectionWindow {
    float depth, x, y;
    std::int8_t row, column, columns, rows;
    std::int16_t labelTexture, quantity;
};
struct LegacyCollectionWork {
    std::uint8_t previousHelpTopic, previousHelpMode;
    std::int16_t mode;
    std::int32_t selectedItem, firstItem, phase;
    std::uint32_t flags;
    std::int32_t count;
    LegacyCollectionWindow *window;
    std::int32_t page;
};
typedef char LegacyCollectionWindowSizeCheck[sizeof(LegacyCollectionWindow) == 0x14 ? 1 : -1];
typedef char LegacyCollectionWindowLabelCheck[offsetof(LegacyCollectionWindow, labelTexture) == 0x10 ? 1 : -1];
typedef char LegacyCollectionWorkWindowCheck[offsetof(LegacyCollectionWork, window) == 0x18 ? 1 : -1];
typedef char LegacyCollectionWorkPageCheck[offsetof(LegacyCollectionWork, page) == 0x1c ? 1 : -1];
void InitializeLegacyCollectionMenu();
std::int32_t OpenLegacyCollectionMenu(std::int32_t mode);
std::uint8_t UpdateLegacyCollectionMenu();
void CloseLegacyCollectionMenu(std::int32_t force);
} } // namespace shenmue::s2

#else

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

#endif
