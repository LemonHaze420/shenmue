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

namespace shenmue::s2 {



namespace event_runtime_detail {
void ResetScenarioFlags();
void scenProcessCharacterActionQueue();
std::int64_t QueueScenarioSpeakerFacing(ScenarioFreeTalkRecord &record);
void EndScenarioTalkSession(ScenarioFreeTalkRecord &record);
void SetFaceXFlags(ScenarioFreeTalkRecord &record);
} // namespace event_runtime_detail

} // namespace shenmue::s2
