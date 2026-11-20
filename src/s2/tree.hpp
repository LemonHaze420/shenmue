#pragma once

#include "common/types.hpp"
#include "shenmue.h"
#include "s2/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

#define SHENMUE_TREE_CONSTANT inline constexpr
namespace shenmue { namespace s2 {

struct MT7Node;

inline constexpr shen_name TREE_TASK{SHEN_NAME(TREE)};
// Prop nodes whose model marker's low byte is 't' sway as trees; the next byte names the template tree they share.
SHENMUE_TREE_CONSTANT std::uint8_t TREE_MODEL_MARKER = 't';
SHENMUE_TREE_CONSTANT std::int32_t TREE_TEMPLATE_CAPACITY = 32;


SHENMUE_TREE_CONSTANT std::int32_t TREE_SPEED_X_BASE = 540;
SHENMUE_TREE_CONSTANT std::int32_t TREE_SPEED_Z_BASE = 480;
SHENMUE_TREE_CONSTANT std::int32_t TREE_AMPLITUDE_X_BASE = 256;
SHENMUE_TREE_CONSTANT std::int32_t TREE_AMPLITUDE_Z_BASE = 384;
SHENMUE_TREE_CONSTANT std::int32_t TREE_SWAY_X_PERCENT = 30;



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
typedef char TreeLayoutCheck5[(sizeof(TreeSwayRecord) == 0xa0) ? 1 : -1];
typedef char TreeLayoutCheck6[(offsetof(TreeSwayRecord, phaseX) == 0x88) ? 1 : -1];
typedef char TreeLayoutCheck7[(offsetof(TreeSwayRecord, amplitudeZ) == 0x9c) ? 1 : -1];

extern HLib::Task *g_treeTask;

void tree_Callback_Enqueue(std::uint8_t nextFunctionIndex);
void tree_Callback_Callback(void *parameter);

} } // namespace shenmue::s2

#undef SHENMUE_TREE_CONSTANT
