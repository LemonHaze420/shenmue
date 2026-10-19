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

namespace shenmue::s2 {
struct ScenarioCharacterAssignment;
struct ScenarioFreeTalkRecord;


struct Character;
struct AuthorController;
struct DateTime;
enum class FilmControlFlag : std::uint32_t;
struct PadControlEntry;
struct MapEventCallbackEntry;
struct ScenarioFreeTalkCandidate;
struct ReflectorBoardParams;
struct WindowSlot;
struct ObjectXFormsParams;
struct MT7Node;



#pragma pack(push, 4)
struct ScenarioEventRuntime {
    HLib::Task *task;
    std::uint32_t flags;
    std::uint8_t reserved00c;
    std::uint8_t dialogueSpeaker;  // +0x00d, participant letter
    std::uint8_t dialogueListener; // +0x00e, participant letter
    std::byte reserved00f[0x11];
    SCENDialogueCue *dialogueCursor;
    shen_name activeCharacter;
    shen_name previousCharacter;
    shen_name lastSpeakingRecord;
    std::uint32_t reserved034;
    Character *hiddenFreeTalkCharacter;
    const std::byte *freeTalkData;                // +0x040 borrowed SCNF container
    PadControlEntry *padControlData;
    PadControlEntry *inputState;                  // +0x050 previous PADC binding
    std::uint32_t freeTalkRecordCount;
    shen_name faceResetCharacter;
    std::array<std::int16_t, 80> faceSequence;
    std::uint32_t randomSelectionCount;
    std::array<std::int32_t, 3> randomSelections;
    float selectedTargetDistanceSquared;
    std::uint32_t selectedTargetAbsYaw;
    shen_name pendingCharacter;
    std::uint32_t field11c;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioTaskData {
    shen_name name;
    std::uint32_t field04;
    void *scenarioWindow;
    ScenarioEventRuntime *scenarioEventRuntime;
    AuthorController *scenarioControl;
    ScenarioCharacterAssignment *characterTree;
    std::array<ScenarioCharacterAssignment *, 13> characterBuckets;
    WindowSlot *windowDialog;
    ScenarioWindowTextState windowTextState;
    std::uint8_t field0e4;
    std::byte reserved0e5[0xbb];
    std::array<ScenarioActionQueue, 2> actionQueues; // +0x1a0 SCNA, +0x228 SCNF
};
#pragma pack(pop)

static_assert(offsetof(ScenarioEventRuntime, flags) == 0x08);
static_assert(offsetof(ScenarioEventRuntime, activeCharacter) == 0x28);
static_assert(offsetof(ScenarioEventRuntime, hiddenFreeTalkCharacter) == 0x38);
static_assert(offsetof(ScenarioEventRuntime, freeTalkData) == 0x40);
static_assert(offsetof(ScenarioEventRuntime, inputState) == 0x50);
static_assert(offsetof(ScenarioEventRuntime, freeTalkRecordCount) == 0x58);
static_assert(offsetof(ScenarioEventRuntime, pendingCharacter) == 0x118);
static_assert(offsetof(ScenarioEventRuntime, lastSpeakingRecord) == 0x30);
static_assert(sizeof(ScenarioFreeTalkControl) == 0x10);
static_assert(sizeof(ScenarioEventRuntime) == 0x120);
static_assert(offsetof(ScenarioTaskData, scenarioEventRuntime) == 0x10);
static_assert(offsetof(ScenarioTaskData, characterTree) == 0x20);
static_assert(offsetof(ScenarioTaskData, characterBuckets) == 0x28);
static_assert(offsetof(ScenarioTaskData, windowTextState) == 0x98);
static_assert(offsetof(ScenarioTaskData, windowTextState) + offsetof(ScenarioWindowTextState, scenarioStateFlags) == 0xd0);
static_assert(offsetof(ScenarioTaskData, actionQueues) == 0x1a0);
static_assert(sizeof(ScenarioTaskData) == 0x2b0);
inline constexpr std::uint32_t ScenarioEventFlagFreeState = 0x0800;
// ReinitScenarioAndFlags' SCEN step for a new or loaded game.
void ResetScenarioGlobals();
ScenarioEventRuntime *GetScenarioEventRuntime();
void SetNewScenarioTask(const std::byte *scnf);

HLib::Task *ScenarioTaskEntry_Enqueue(std::uint8_t nextFunctionIndex);
void EV_SetScenarioTaskFlags(std::uint32_t flags);

struct ScenarioAuthorContext {
    AuthorController *current;
    shen_name areaId;
    std::int32_t sceneId;
    std::int8_t track;
    bool active;
    bool trackLatch;
    std::int32_t currentFrame;
    std::uint32_t framesRemaining;
};

} // namespace shenmue::s2
