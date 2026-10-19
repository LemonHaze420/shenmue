#pragma once
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
