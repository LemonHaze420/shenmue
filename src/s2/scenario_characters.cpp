#include "s2/scenario_characters.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {


ScenarioCharacterAssignment *&ScenarioRoot(ScenarioTaskData &scenario) {
    return scenario.characterTree;
}

ScenarioCharacterAssignment *&ScenarioBucket(ScenarioTaskData &scenario, std::size_t index) {
    return scenario.characterBuckets[index];
}
Character *scenFindCharaById(ScenarioCharacterAssignment *node, shen_name characterId) {
    while (node) {
        if (node->id.name == characterId.name)
            return node->character;
        node = static_cast<std::int32_t>(characterId.name) < static_cast<std::int32_t>(node->id.name) ? node->lower : node->higher;
    }
    return nullptr;
}

// @verified - p
void ScenarioCharacterPublicationCallback(shen_name, shen_name) {
}

// @verified - p
bool InsertScenarioCharacterId(ScenarioCharacterAssignment *&root, ScenarioCharacterAssignment &node) {
    node.higher = nullptr;
    node.lower = nullptr;
    ScenarioCharacterAssignment **insertion = &root;
    while (*insertion) {
        if ((*insertion)->id.name == node.id.name)
            return false;
        insertion = static_cast<std::int32_t>(node.id.name) > static_cast<std::int32_t>((*insertion)->id.name) ? &(*insertion)->higher : &(*insertion)->lower;
    }
    *insertion = &node;
    return true;
}
} // namespace event_runtime_detail


// @verified - p
Character *GetScenarioCharacterNode(HLib::Task *scenarioTask, shen_name characterId) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    return scenFindCharaById(ScenarioRoot(*scenario), characterId);
}

void scenSetCharaPosRotById(shen_name characterId, const Vector3 *position, const Vector3i *rotation) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = GetScenarioCharacterNode(control->scenarioTask, characterId);
    SetFixoInversePositionRotation(character, position, rotation);
}

Character *scenFindChara(HLib::Task *scenarioTask, shen_name characterId) {
    ScenarioTaskData *const scenario = ScenarioData(scenarioTask);
    if (!scenario || characterId.name == 0)
        return nullptr;
    return scenFindCharaById(ScenarioRoot(*scenario), characterId);
}

shen_name scenGetCharaName(HLib::Task *scenarioTask, Character *character) {
    ScenarioTaskData *const scenario = ScenarioData(scenarioTask);
    if (!scenario)
        return {};
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    ScenarioCharacterAssignment **link = &ScenarioBucket(*scenario, bucket);
    while (*link && (*link)->character != character)
        link = &(*link)->bucketNext;
    ScenarioCharacterAssignment *const assignment = *link;
    if (!assignment)
        return {};
    *link = assignment->bucketNext;
    assignment->bucketNext = ScenarioBucket(*scenario, bucket);
    ScenarioBucket(*scenario, bucket) = assignment;
    return assignment->id;
}
// @verified - p
void CreateScenarioCharacterId(HLib::Task *scenarioTask, shen_name characterId, Character *character) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    ScenarioCharacterAssignment *const node =
        static_cast<ScenarioCharacterAssignment *>(HLib::AllocateTaskData(sizeof(ScenarioCharacterAssignment), MEMORY_CHID));
    node->id = characterId;
    node->character = character;
    InsertScenarioCharacterId(ScenarioRoot(*scenario), *node);
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    node->bucketNext = ScenarioBucket(*scenario, bucket);
    ScenarioBucket(*scenario, bucket) = node;
    ScenarioCharacterPublicationCallback(g_currentMap, characterId);
}

bool AssignScenarioCharacter(HLib::Task *scenarioTask, shen_name characterId, Character *character) {
    ScenarioTaskData *const scenario = ScenarioData(scenarioTask);
    if (!scenario || !character || characterId.name == 0)
        return false;

    ScenarioCharacterAssignment **insertion = &ScenarioRoot(*scenario);
    while (*insertion) {
        if ((*insertion)->id.name == characterId.name)
            return false;
        insertion =
            static_cast<std::int32_t>(characterId.name) < static_cast<std::int32_t>((*insertion)->id.name) ? &(*insertion)->lower : &(*insertion)->higher;
    }

    ScenarioCharacterAssignment *const node =
        static_cast<ScenarioCharacterAssignment *>(HLib::AllocateTaskData(sizeof(ScenarioCharacterAssignment), MEMORY_CHID));
    if (!node)
        return false;
    std::memset(node, 0, sizeof(*node));
    node->id = characterId;
    node->character = character;
    *insertion = node;

    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    node->bucketNext = ScenarioBucket(*scenario, bucket);
    ScenarioBucket(*scenario, bucket) = node;
    return true;
}

bool UnassignScenarioCharacter(HLib::Task *scenarioTask, Character *character) {
    ScenarioTaskData *const scenario = ScenarioData(scenarioTask);
    if (!scenario)
        return false;

    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    ScenarioCharacterAssignment **bucketLink = &ScenarioBucket(*scenario, bucket);
    while (*bucketLink && (*bucketLink)->character != character) {
        bucketLink = &(*bucketLink)->bucketNext;
    }
    ScenarioCharacterAssignment *const node = *bucketLink;
    if (!node)
        return false;
    *bucketLink = node->bucketNext;

    ScenarioCharacterAssignment **treeLink = &ScenarioRoot(*scenario);
    while (*treeLink && *treeLink != node) {
        treeLink = static_cast<std::int32_t>(node->id.name) < static_cast<std::int32_t>((*treeLink)->id.name) ? &(*treeLink)->lower : &(*treeLink)->higher;
    }
    if (*treeLink == node) {
        if (!node->lower) {
            *treeLink = node->higher;
        } else if (!node->higher) {
            *treeLink = node->lower;
        } else {
            ScenarioCharacterAssignment **successorLink = &node->higher;
            while ((*successorLink)->lower) {
                successorLink = &(*successorLink)->lower;
            }
            ScenarioCharacterAssignment *const successor = *successorLink;
            *successorLink = successor->higher;
            successor->lower = node->lower;
            successor->higher = node->higher;
            *treeLink = successor;
        }
    }
    HLib::FreeTaskData(node);
    return true;
}

void ClearScenarioCharacterAssignments(HLib::Task *scenarioTask) {
    ScenarioTaskData *const scenario = ScenarioData(scenarioTask);
    if (!scenario)
        return;
    // AUTH restores actors while SCEN's CHID lookup and characters are alive.
    FreeScenarioControl(*scenario);
    ReleaseScenarioEventRuntime(*scenario);
    while (ScenarioCharacterAssignment *const root = ScenarioRoot(*scenario)) {
        if (!UnassignScenarioCharacter(scenarioTask, root->character))
            break;
    }
}

} // namespace shenmue::s2
