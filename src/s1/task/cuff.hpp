#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct MotionInformationWork;
struct ScenarioContext;

struct CuffMotionCommand {
    CuffMotionCommand *otherCommand;
    Character *character;
    std::int32_t nodeSelector;
    float maxDistance;
    Vector3 constrainedTarget;
    std::int32_t mode;
    void *model;
    std::byte reserved030[0x28];
};

struct CuffTaskData {
    void *rootModel;
    void *chainSegmentModel;
    std::uint32_t active;
    std::int32_t segmentCount;
    CuffMotionCommand source;
    CuffMotionCommand target;
    Vector3 chainSegmentOffset;
    std::byte reserved0d4[0x0c];
};

struct CuffScriptParameters {
    std::uint32_t sourceCharacterId;
    std::int32_t sourceNodeSelector;
    std::uint32_t targetCharacterId;
    std::int32_t targetNodeSelector;
    float maximumDistance;
};

static_assert(sizeof(CuffMotionCommand) == 0x58);
static_assert(offsetof(CuffMotionCommand, character) == 0x08);
static_assert(offsetof(CuffMotionCommand, nodeSelector) == 0x10);
static_assert(offsetof(CuffMotionCommand, maxDistance) == 0x14);
static_assert(offsetof(CuffMotionCommand, constrainedTarget) == 0x18);
static_assert(offsetof(CuffMotionCommand, mode) == 0x24);
static_assert(offsetof(CuffMotionCommand, model) == 0x28);
static_assert(sizeof(CuffTaskData) == 0xe0);
static_assert(offsetof(CuffTaskData, active) == 0x10);
static_assert(offsetof(CuffTaskData, segmentCount) == 0x14);
static_assert(offsetof(CuffTaskData, source) == 0x18);
static_assert(offsetof(CuffTaskData, target) == 0x70);
static_assert(offsetof(CuffTaskData, chainSegmentOffset) == 0xc8);
static_assert(sizeof(CuffScriptParameters) == 0x14);

HLib::Task *CUFF_Initialize();
void CUFF_Shutdown();
void CUFF_Callback(HLib::TaskData *taskData);
HLib::Task *CUFF_GetTask();
CuffTaskData *CUFF_GetTaskData();

std::uint32_t CUFF_ComputeBoneMidpointAndInitCommand(
    Character *source, std::int32_t sourceNodeSelector, Character *target, std::int32_t targetNodeSelector, float maximumDistance);
void CUFF_ClearInstalledCallbacks();

void Script_ComputeBoneMidpoint(ScenarioContext *context, void *rawParams);
void Script_cuffUtils(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
