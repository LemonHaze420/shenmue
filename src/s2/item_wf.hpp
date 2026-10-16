#pragma once

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

inline constexpr shen_name TASK_ICWF{SHEN_NAME(ICWF)};
inline constexpr shen_name TASK_ICLI{SHEN_NAME(ICLI)};
inline constexpr shen_name TASK_ICSS{SHEN_NAME(ICSS)};
inline constexpr shen_name TASK_ICIS{SHEN_NAME(ICIS)};
inline constexpr shen_name TASK_WFTX{SHEN_NAME(WFTX)};
inline constexpr shen_name TASK_ITEM_MODEL{SHEN_NAME(ITEM)};

#pragma pack(push, 1)
struct alignas(8) WfTaskData {
    std::int32_t state;
    std::uint32_t npcLoadState;
    void *ptx;
    std::uint32_t flags;
    std::uint32_t owned;
    std::uint32_t learned;
    std::int32_t row;
    std::int32_t first;
    float remaining;
    float x;
    float y;
    float velocity;
    std::uint32_t reserved34;
    std::array<std::byte, 8> reserved38;
    std::uint32_t reserved40;
    std::array<std::int32_t, 6> textures;
    std::uint32_t reserved5c;
};

struct WfScrollDefinition {
    std::int8_t itemOffset;
    std::uint8_t category;
    std::int16_t move;
    std::uint16_t prompt;
};
#pragma pack(pop)
static_assert(sizeof(WfTaskData) == 0x60);
static_assert(alignof(WfTaskData) == 8);
static_assert(offsetof(WfTaskData, ptx) == 0x08);
static_assert(offsetof(WfTaskData, owned) == 0x14);
static_assert(offsetof(WfTaskData, x) == 0x28);
static_assert(offsetof(WfTaskData, velocity) == 0x30);
static_assert(offsetof(WfTaskData, textures) == 0x44);
static_assert(sizeof(WfScrollDefinition) == 6);

inline constexpr std::array<WfScrollDefinition, 25> wfScrollDefinitions{{
    {11, 2, 22, 1}, {12, 3, 18, 1}, {13, 3, 16, 1}, {14, 1, 27, 1}, {15, 1, 26, 1},
    {16, 1, 23, 1}, {17, 2, 19, 1}, {18, 1, 25, 1}, {19, 1, 19, 1}, {20, 2, 21, 1},
    {21, 2, 20, 1}, {22, 3, 19, 1}, {23, 3, 17, 1}, {24, 2, 4, 1}, {25, 3, 20, 2},
    {0, 1, 14, 0}, {1, 2, 18, 0}, {2, 1, 16, 0}, {3, 2, 17, 0}, {4, 2, 14, 0},
    {5, 1, 13, 0}, {6, 2, 15, 0}, {7, 3, 11, 0}, {8, 3, 10, 0}, {10, 3, -1, 2},
}};
inline constexpr std::array<WfScrollDefinition, 2> wfSpecialScrollDefinitions{{{10, 3, -1, 2}, {9, 3, -1, 2}}};
inline constexpr std::array<std::array<std::uint16_t, 2>, 3> wfPromptIcons{{{266, 265}, {268, 267}, {270, 269}}};


inline constexpr std::int8_t wfMenuSpecialReadThroughOffset = 1;

struct WfMenuEntry {
    std::int32_t index;
    std::int8_t itemOffset;
    std::uint8_t category;
    std::int16_t move;
    std::uint16_t prompt;
    std::uint16_t reserved0a;
    std::uint32_t reserved0c;
    const char *label;
};
static_assert(sizeof(WfMenuEntry) == 0x18);
static_assert(offsetof(WfMenuEntry, label) == 0x10);

inline constexpr std::array<Vector2s, 5> itemInspectAngles{{{0, 330}, {0, 0}, {350, 330}, {10, 330}, {0, 30}}};

#pragma pack(push, 4)
struct ItemPreviewDrawData {
    struct MT7Node *root;
    float scale;
    Matrix4x4 transform;
    float alpha;
    Vector3 colour;
    std::uint32_t lightYaw;
};
#pragma pack(pop)
static_assert(sizeof(ItemPreviewDrawData) == 0x60);
static_assert(offsetof(ItemPreviewDrawData, scale) == 0x08);
static_assert(offsetof(ItemPreviewDrawData, transform) == 0x0c);
static_assert(offsetof(ItemPreviewDrawData, alpha) == 0x4c);
static_assert(offsetof(ItemPreviewDrawData, colour) == 0x50);
static_assert(offsetof(ItemPreviewDrawData, lightYaw) == 0x5c);

std::uint8_t SetGuiItemAction(std::uint32_t item, std::uint8_t flags, std::int32_t enabled) noexcept;
void SetMovesMenuItemActionState(std::int32_t state) noexcept;
void Queue_wf_MainControl(std::uint8_t nextFunctionIndex);
void wf_MainControl_Callback(void *parameter);
bool GetwfMainControlState() noexcept;
bool OpenWfScrollMenu();
void OpenWfScrollWidget();
std::int64_t CloseWfScrollMenu(std::int32_t force);
void StopWfScrollMenu();
void LearnWfScroll(std::int32_t index);
void InspectWfScroll(std::int32_t index);
std::uint32_t GetWfOwnedScrolls();
std::uint32_t GetWfLearnedScrolls();
std::uint16_t GetWfPromptIcon(std::uint16_t prompt, std::uint32_t scroll);
void BuildWfMenuEntries(std::span<WfMenuEntry, 25> entries);
void BuildOwnedWfScrollSlots(std::span<std::int32_t, 26> slots);
std::uint32_t GetWfScrollItem(std::int32_t index) noexcept;
std::int32_t FindWfScrollDefinition(std::int32_t item) noexcept;
std::int32_t GetWfScrollPreviewRow(std::int32_t index);

bool OpenItemLook(std::uint32_t item, std::int32_t mode);
void CloseItemLook();
void SetItemLookShowingGameMenu() noexcept;
void ToggleGameMenu() noexcept;
std::uint32_t GetGuiItemAction(std::uint32_t item) noexcept;
void ClearGuiItemActions() noexcept;
std::int64_t UpdateSelectedItemStateFlag(std::int16_t item);
void SetItemLookRotation(std::int16_t pitch, std::int16_t yaw, std::int16_t roll) noexcept;
void ICSU_InitItemLook_Callback(void *parameter);
void ICSU_CtrlItemLook_Callback(void *parameter);
void ReleaseItemWfOwners();

} // namespace shenmue::s2
