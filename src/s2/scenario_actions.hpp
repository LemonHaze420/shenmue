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

enum FreeTalkCommandCode : std::int32_t {
#define SHENMUE_S2_FREE_TALK_COMMAND(name, value) FT_##name = value,
#include "common/free_talk_commands.def"
#undef SHENMUE_S2_FREE_TALK_COMMAND
};



namespace event_runtime_detail {
void ResetScenarioFlags();
void scenProcessCharacterActionQueue();
std::int64_t QueueScenarioSpeakerFacing(ScenarioFreeTalkRecord &record);
void EndScenarioTalkSession(ScenarioFreeTalkRecord &record);
void SetFaceXFlags(ScenarioFreeTalkRecord &record);
} // namespace event_runtime_detail

} // namespace shenmue::s2
