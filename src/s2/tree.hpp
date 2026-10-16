#pragma once

#include "common/types.hpp"
#include "s2/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct MT7Node;

inline constexpr shen_name TREE_TASK{SHEN_NAME(TREE)};
// Prop nodes whose model marker's low byte is 't' sway as trees; the next byte names the template tree they share.
inline constexpr std::uint8_t TREE_MODEL_MARKER = 't';
inline constexpr std::int32_t TREE_TEMPLATE_CAPACITY = 32;


inline constexpr std::int32_t TREE_SPEED_X_BASE = 540;
inline constexpr std::int32_t TREE_SPEED_Z_BASE = 480;
inline constexpr std::int32_t TREE_AMPLITUDE_X_BASE = 256;
inline constexpr std::int32_t TREE_AMPLITUDE_Z_BASE = 384;
inline constexpr std::int32_t TREE_SWAY_X_PERCENT = 30;



struct TreeSwayRecord {
    MT7Node *tree;
    std::array<std::byte, 0x80> branchCopies;
    std::int32_t phaseX;
    std::int32_t phaseZ;
    std::int32_t speedX;
    std::int32_t speedZ;
    std::int32_t amplitudeX;
    std::int32_t amplitudeZ;
};
static_assert(sizeof(TreeSwayRecord) == 0xa0);
static_assert(offsetof(TreeSwayRecord, phaseX) == 0x88);
static_assert(offsetof(TreeSwayRecord, amplitudeZ) == 0x9c);

extern HLib::Task *g_treeTask;

void tree_Callback_Enqueue(std::uint8_t nextFunctionIndex);
void tree_Callback_Callback(void *parameter);

} // namespace shenmue::s2
