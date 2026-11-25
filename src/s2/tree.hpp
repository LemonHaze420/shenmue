#pragma once

#include "common/types.hpp"
#include "shenmue.h"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/model_dc.hpp"
#endif
#include "s2/hlib.hpp"

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include <array>
#include <cstddef>
#include <cstdint>
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define SHENMUE_TREE_CONSTANT static const
#else
#define SHENMUE_TREE_CONSTANT inline constexpr
#endif
namespace shenmue { namespace s2 {

struct MT7Node;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define TREE_TASK SHEN_NAME(TREE)
#else
inline constexpr shen_name TREE_TASK{SHEN_NAME(TREE)};
#endif
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
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    MT7Node branchCopies[2];
#else
    std::array<std::byte, 0x80> branchCopies;
#endif
    std::int32_t phaseX;
    std::int32_t phaseZ;
    std::int32_t speedX;
    std::int32_t speedZ;
    std::int32_t amplitudeX;
    std::int32_t amplitudeZ;
};
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char TreeLayoutCheck1[(sizeof(TreeSwayRecord) == 0x9c) ? 1 : -1];
typedef char TreeLayoutCheck2[(offsetof(TreeSwayRecord, branchCopies) == 4) ? 1 : -1];
typedef char TreeLayoutCheck3[(offsetof(TreeSwayRecord, phaseX) == 0x84) ? 1 : -1];
typedef char TreeLayoutCheck4[(offsetof(TreeSwayRecord, amplitudeZ) == 0x98) ? 1 : -1];
#else
typedef char TreeLayoutCheck5[(sizeof(TreeSwayRecord) == 0xa0) ? 1 : -1];
typedef char TreeLayoutCheck6[(offsetof(TreeSwayRecord, phaseX) == 0x88) ? 1 : -1];
typedef char TreeLayoutCheck7[(offsetof(TreeSwayRecord, amplitudeZ) == 0x9c) ? 1 : -1];

#endif

extern HLib::Task *g_treeTask;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void tree_Callback_Enqueue(char nextFunctionIndex);
#else
void tree_Callback_Enqueue(std::uint8_t nextFunctionIndex);
#endif
void tree_Callback_Callback(void *parameter);

} } // namespace shenmue::s2

#undef SHENMUE_TREE_CONSTANT
