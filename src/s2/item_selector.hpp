#pragma once

#include "s2/camera.hpp"
#include "s2/ui.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name ITEM_TEXTURE_TYPE = SHEN_NAME(ITEM);
inline constexpr shen_name ITEM_COMMON_TEXTURE = SHEN_NAME(COMN);
inline constexpr shen_name ITEM_COUNTER_TEXTURE = SHEN_NAME(SCNT);

struct UniqueItemActionData {
    std::int32_t item{-1};
    std::uint32_t reserved04{};
    std::int64_t (*begin)(){};
    std::int64_t (*update)(){};
    void (*complete)(){};
    void (*cancel)(std::int32_t){};
};
static_assert(sizeof(UniqueItemActionData) == 0x28);
static_assert(offsetof(UniqueItemActionData, item) == 0x00);
static_assert(offsetof(UniqueItemActionData, reserved04) == 0x04);
static_assert(offsetof(UniqueItemActionData, begin) == 0x08);
static_assert(offsetof(UniqueItemActionData, update) == 0x10);
static_assert(offsetof(UniqueItemActionData, complete) == 0x18);
static_assert(offsetof(UniqueItemActionData, cancel) == 0x20);

struct ItemSelectorActionCallbacks {
    std::int64_t (*begin)();
    std::int64_t (*update)();
    void (*complete)();
    void (*cancel)(std::int32_t);
};
static_assert(sizeof(ItemSelectorActionCallbacks) == 0x20);
static_assert(offsetof(ItemSelectorActionCallbacks, begin) == 0x00);
static_assert(offsetof(ItemSelectorActionCallbacks, update) == 0x08);
static_assert(offsetof(ItemSelectorActionCallbacks, complete) == 0x10);
static_assert(offsetof(ItemSelectorActionCallbacks, cancel) == 0x18);

struct ItemSelectorCamera {
    Vector3 eye, savedEye, goalEye, eyeStep;
    Vector3 target, savedTarget, goalTarget, targetStep;
    float alpha{}, savedAlpha{}, alphaStep{};
    std::array<std::byte, 8> reserved6c{};
    std::int32_t duration{}, frame{};
    std::uint8_t mode{};
    std::array<std::byte, 3> reserved7d{};
};
static_assert(sizeof(ItemSelectorCamera) == 0x80);
static_assert(offsetof(ItemSelectorCamera, target) == 0x30);
static_assert(offsetof(ItemSelectorCamera, alpha) == 0x60);
static_assert(offsetof(ItemSelectorCamera, savedAlpha) == 0x64);
static_assert(offsetof(ItemSelectorCamera, alphaStep) == 0x68);
static_assert(offsetof(ItemSelectorCamera, duration) == 0x74);
static_assert(offsetof(ItemSelectorCamera, frame) == 0x78);
static_assert(offsetof(ItemSelectorCamera, mode) == 0x7c);

struct ItemSelectorContext {
    std::uint8_t helpTopic{}, selected{};
    std::uint16_t visible{};
    std::uint32_t playerDisplay{}, playerVisible{};
    CameraState cameraState{};
    std::uint32_t reserved10{};
    std::uint16_t perspective{}, reserved16{};
    Vector3 eye{}, target{};
    std::int32_t weather{};
};
static_assert(sizeof(ItemSelectorContext) == 0x34);
static_assert(offsetof(ItemSelectorContext, cameraState) == 0x0c);
static_assert(offsetof(ItemSelectorContext, perspective) == 0x14);
static_assert(offsetof(ItemSelectorContext, eye) == 0x18);
static_assert(offsetof(ItemSelectorContext, target) == 0x24);
static_assert(offsetof(ItemSelectorContext, weather) == 0x30);


#pragma pack(push, 4)
struct ItemSelectorSelection {
    std::int8_t row{}, column{}, visibleRows{}, columns{};
    std::uint8_t reserved04{};
    bool glowBrightening{};
    std::int16_t count{}, quantitySum{};
    std::uint16_t reserved0a{};
    std::int32_t activeMenu{};
    std::uint32_t actionResult{}, glowColour{}, exitFlags{}, reserved1c{};
    ItemSelectorContext *context{};
    std::int32_t selectedItem{}, selectedIndex{}, lowerRow{};
};
#pragma pack(pop)
static_assert(sizeof(ItemSelectorSelection) == 0x34);
static_assert(offsetof(ItemSelectorSelection, glowBrightening) == 0x05);
static_assert(offsetof(ItemSelectorSelection, count) == 0x06);
static_assert(offsetof(ItemSelectorSelection, quantitySum) == 0x08);
static_assert(offsetof(ItemSelectorSelection, activeMenu) == 0x0c);
static_assert(offsetof(ItemSelectorSelection, actionResult) == 0x10);
static_assert(offsetof(ItemSelectorSelection, glowColour) == 0x14);
static_assert(offsetof(ItemSelectorSelection, exitFlags) == 0x18);
static_assert(offsetof(ItemSelectorSelection, context) == 0x20);
static_assert(offsetof(ItemSelectorSelection, selectedItem) == 0x28);
static_assert(offsetof(ItemSelectorSelection, selectedIndex) == 0x2c);
static_assert(offsetof(ItemSelectorSelection, lowerRow) == 0x30);

inline constexpr std::array<std::int32_t, 3> ITEM_SELECTOR_SPECIAL_ITEMS{0, 2, 20};
inline constexpr std::array<std::int32_t, 9> ITEM_SELECTOR_ACTIONS{4, 5, 6, 7, 8, 9, 10, 11, 12};
inline constexpr std::array<std::int32_t, 2> ITEM_SELECTOR_LOWER_COUNTS{3, 6};
inline constexpr std::array<std::array<std::int32_t, 10>, 2> ITEM_SELECTOR_MENU_MAP{{{0, 1, 2, 0, 0, 0, 0, 0, 0, 0}, {3, 4, 5, 6, 7, 8, 0, 0, 0, 0}}};
inline constexpr std::array<const char *, 9> ITEM_SELECTOR_LABELS{
    "Save", "Options", "Snap-shots", "Moves Scroll", "Collection", "Event Photos", "Maps", "Pawnshop Fliers", "Move Handbook"};
inline constexpr std::array<std::uint16_t, 10> SELECTOR_DIGIT_ICONS{1030, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039};
inline constexpr std::array<std::uint16_t, 3> SELECTOR_CURRENCY_ICONS{1040, 1052, 1046};
inline constexpr std::array<std::uint16_t, 7> SELECTOR_WEEKDAY_ICONS{1287, 1288, 1289, 1290, 1291, 1292, 1293};
inline constexpr std::array<std::array<std::uint16_t, 6>, 2> ITEM_SELECTOR_LOWER_ICONS{{{0, 1, 7, 0, 0, 0}, {2, 8, 2304, 6, 4, 9}}};
inline constexpr float ITEM_SELECTOR_TILE_DEPTH = 0.19999999f;
inline constexpr LegacyIconSprite SELECTOR_DATE_DIGIT_INITIAL{.x = 0,
    .y = 0,
    .scaleX = 1,
    .scaleY = 1,
    .depth = 0,
    .flags = 1024,
    .reserved16 = 0,
    .rotation = 0,
    .icon = 1295,
    .width = 16,
    .height = 16,
    .alignment = 4,
    .reserved23 = {}};
inline constexpr LegacyIconSprite SELECTOR_DATE_WEEKDAY_INITIAL{.x = 0,
    .y = 0,
    .scaleX = 1,
    .scaleY = 1,
    .depth = 0,
    .flags = 1024,
    .reserved16 = 0,
    .rotation = 0,
    .icon = 1287,
    .width = 48,
    .height = 16,
    .alignment = 4,
    .reserved23 = {}};

} // namespace shenmue::s2
