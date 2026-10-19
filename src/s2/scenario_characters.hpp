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

struct ScenarioCharacterAssignment {
    ScenarioCharacterAssignment *higher;
    ScenarioCharacterAssignment *lower;
    shen_name id;
    std::uint32_t padding14;
    ScenarioCharacterAssignment *bucketNext;
    Character *character;
};
static_assert(offsetof(ScenarioCharacterAssignment, higher) == 0x00);
static_assert(offsetof(ScenarioCharacterAssignment, lower) == 0x08);
static_assert(offsetof(ScenarioCharacterAssignment, id) == 0x10);
static_assert(offsetof(ScenarioCharacterAssignment, bucketNext) == 0x18);
static_assert(offsetof(ScenarioCharacterAssignment, character) == 0x20);
static_assert(sizeof(ScenarioCharacterAssignment) == 0x28);
inline constexpr shen_name MEMORY_CHID{SHEN_NAME(CHID)};
void scenSetCharaPosRotById(shen_name character, const Vector3 *position, const Vector3i *rotation);
Character *GetScenarioCharacterNode(HLib::Task *scenarioTask, shen_name characterId);
Character *FindScenarioCharacter(HLib::Task *scenarioTask, shen_name characterId);

shen_name scenGetCharaName(HLib::Task *scenarioTask, Character *character);
void CreateScenarioCharacterId(HLib::Task *scenarioTask, shen_name characterId, Character *character);
bool AssignScenarioCharacter(HLib::Task *scenarioTask, shen_name characterId, Character *character);
bool UnassignScenarioCharacter(HLib::Task *scenarioTask, Character *character);
void ClearScenarioCharacterAssignments(HLib::Task *scenarioTask);

} // namespace shenmue::s2
