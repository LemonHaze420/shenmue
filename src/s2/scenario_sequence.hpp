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






enum class ScenarioSequenceMode : std::uint8_t {
    Teardown = 1,
    FrameWait = 2,
    CameraWait = 3,
    DialogueWait = 4,
    MotionWait = 5,
    UIResultWait = 6,
    ScenarioMotionWait = 7,
    ChildResultWait = 8,
    ChildResultBranch = 9,
    ChildTaskWait = 10,
    FadeWait = 11,
    FightWait = 12,
    AuthorWait = 13,
};

enum class ScenarioSequenceStatus : std::uint8_t {
    Empty,
    Ready,
    Running,
    Complete,
    InvalidRecords,
    VoiceFailed,
};

// Retention is transactional.  A failed parse leaves no runnable program and
// publishes every distinct unsupported mode through the diagnostic span.
bool RetainScenarioSequenceRecords(std::span<const std::byte> records);
HLib::Task *StartRetainedScenarioSequence(shen_name sequenceName);
void StopScenarioSequence();
ScenarioSequenceStatus GetScenarioSequenceStatus();
std::uint32_t GetScenarioSequenceResult();
std::span<const std::uint8_t> GetScenarioSequenceUnsupportedModes();
void SetScenarioSequenceFightActive(bool active);
#pragma pack(push, 4)
struct ScenarioSequenceTaskData {
    std::uint16_t mode;
    std::uint16_t nextMode;
    std::uint16_t recordState;
    std::uint16_t previousMode;
    shen_name sequenceName;
    std::uint32_t recordCursor;
    std::uint32_t nextRecordCursor;
    std::uint32_t order;
    std::uint32_t childSlot;
    std::uint32_t resultIndex;
    std::uint64_t waitSlot;
    HLib::Task *task;
    ScenarioSequenceTaskData *parent;
    std::array<HLib::Task *, 10> children;
    std::uint32_t dispatchState;
    std::uint32_t recordFlags;
    std::array<std::uint32_t, 10> results;
    std::array<std::uint32_t, 10> branchValues;
    std::byte reserved0e0[0x50];
    ScenarioEventRuntime *scenario;
    std::byte reserved138[0x54];
    std::uint32_t uiResult;
    shen_name motionCharacter;
    std::uint32_t countdown;
    std::uint32_t waitState;
    std::uint32_t finalResult;
    std::uint32_t completed;
    std::byte reserved1a4[0xcc];
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioSequenceChildHeader {
    std::uint32_t mode;
    std::uint32_t slot;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioSequenceCameraChild {
    ScenarioSequenceChildHeader header;
    std::int32_t cameraId;
    shen_name eyeAttachment;
    shen_name targetAttachment;
    Vector3 eye;
    Vector3 target;
    std::int32_t roll;
    std::uint32_t fieldOfView;
    std::uint32_t transitionFrames;
    std::uint32_t cut;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioSequenceDialogueChild {
    ScenarioSequenceChildHeader header;
    std::array<char, 13> voice;
    std::array<char, 13> subtitle;
    std::uint16_t reserved;
    std::uint32_t durationFrames;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioSequenceMotionChild {
    ScenarioSequenceChildHeader header;
    shen_name character;
    std::uint32_t motion;
    std::uint32_t region;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioSequenceUIChild {
    ScenarioSequenceChildHeader header;
    std::array<std::uint16_t, 4> flagIds;
    std::uint16_t timeoutFrames;
    std::uint16_t defaultFlagId;
    std::uint32_t resultSlot;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioSequenceFadeChild {
    ScenarioSequenceChildHeader header;
    std::uint32_t durationFrames;
    std::uint32_t startAlpha;
    std::uint32_t targetAlpha;
};
#pragma pack(pop)

static_assert(offsetof(ScenarioSequenceTaskData, task) == 0x28);

static_assert(offsetof(ScenarioSequenceTaskData, children) == 0x38);

static_assert(offsetof(ScenarioSequenceTaskData, results) == 0x90);

static_assert(offsetof(ScenarioSequenceTaskData, scenario) == 0x130);

static_assert(offsetof(ScenarioSequenceTaskData, countdown) == 0x194);

static_assert(sizeof(ScenarioSequenceTaskData) == 0x270);

struct ScenarioSequenceRecord {
    std::uint32_t offset{};
    ScenarioSequenceMode mode{};
    std::uint16_t flags{};
    std::uint8_t argumentCount{};
    std::array<std::uint32_t, 12> arguments{};
};

} // namespace shenmue::s2
