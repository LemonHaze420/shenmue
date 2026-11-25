#pragma once
#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/scenario_talk_dc.hpp"

namespace shenmue {
namespace s2 {

extern std::int32_t g_scntExpressionResult;
void ResetScntBranchHistory();
std::int32_t VisitScntBranch(const std::byte *target);
const std::byte *DecodeScntExpression(const std::byte *cursor, ScenarioFreeTalkCharacter *record);
const std::byte *FindScenarioConversationRoot(ScenarioFreeTalkCharacter *record, std::int32_t step);

} // namespace s2
} // namespace shenmue

#else
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

#include "s2/scenario_free_talk.hpp"
namespace shenmue::s2 {

// These are the native EV function-table modes baked into S2 area code. Read
// and write use overlapping numeric values with different meanings.
struct ScenarioExpressionOperand {
    std::uint32_t mode{1};
    std::int32_t index{};
};
static_assert(sizeof(ScenarioExpressionOperand) == 8);
static_assert(offsetof(ScenarioExpressionOperand, index) == 4);

struct ScenarioChoiceCandidate {
    std::uint16_t flagId;
    std::int8_t rank;
    std::int8_t priority;
    std::uint32_t reserved04;
    const std::byte *target;
};

static_assert(sizeof(ScenarioChoiceCandidate) == 0x10);

} // namespace shenmue::s2

#endif
