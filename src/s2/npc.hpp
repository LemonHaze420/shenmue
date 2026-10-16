#pragma once

#include "common/types.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct Character;
struct MotionWork;
extern float npcHourWeatherForecast;
enum NpcCharacterControllerFlags : std::uint32_t {
    UpdateHeight = 0x01U,
    ApplyGravity = 0x02U,
    EvaluateMotion = 0x04U,
    SolverAssist = 0x08U,
    Unknown10 = 0x10U,
};

enum class NpcCharacterControllerState : std::int32_t {
    Current = -1,
    Disabled = 0,
    Enabled = 1,
};

enum NpcRouteStreamFlags : std::uint32_t {
    NpcRouteStreamRequested = 0x00000001U,
    NpcRouteStreamLive = 0x00000002U,
    NpcRouteStreamFadeOut = 0x00000004U,
    NpcRouteAbstractActive = 0x00000008U,
    NpcRouteStreamForceHidden = 0x00000040U,
    NpcRouteFlag80 = 0x00000080U,
    NpcRouteStreamPinned = 0x00000200U,
    NpcRouteStreamStartHidden = 0x00001000U,
    NpcRouteStreamUseHumansAfs = 0x40000000U,
    NpcRouteStreamLoadedHidden = 0x80000000U,
};

struct NpcRouteLiveEntry;

struct NpcRouteNode {
    std::int32_t pointsOffset;
    std::uint32_t pointCount;
    std::int32_t lengthsOffset;
    std::array<std::int16_t, 8> connections;
    std::array<float, 8> connectionLengths;
    std::int32_t laneBoundsOffset;
    std::int32_t auxiliaryOffset;
    shen_name name;
    std::uint32_t flags;
};
static_assert(sizeof(NpcRouteNode) == 0x4c);

#pragma pack(push, 4)
struct NpcRouteLink {
    NpcRouteLiveEntry *character{};
    float leashDistance{};
};
#pragma pack(pop)
static_assert(sizeof(NpcRouteLink) == 12);

struct NpcRouteStateContext {
    NpcRouteLiveEntry *owner{};
    MotionWork *motionWork{};
    const NpcRouteNode *stateTable{};
    std::int16_t startNode{};
    std::int16_t endNode{};
    std::uint8_t routeCount{};
    std::uint8_t activeRouteIndex{};
    std::uint16_t parameter{};
    std::uint16_t stride{};
    std::uint16_t extraStride{};
    std::uint16_t lowParameter{};
    std::uint16_t highParameter{};
    std::int16_t activeNode{};
    std::int16_t nextNode{};
    std::int16_t activeSegment{};
    std::int16_t previousSegment{};
    std::int16_t pointCount{};
    std::int16_t activeWindow{};
    std::int16_t activePointBase{};
    std::uint8_t field036{};
    std::uint8_t crossingPoint{};
    float segmentLength{};
    std::array<Vector3, 4> spline{};
    std::int16_t sampledWindow{};
    std::int16_t sampledPointBase{};
    std::int16_t sampledRouteIndex{};
    std::uint16_t sampledParameter{};
    Vector3 position{};
    shen_name scriptOwner{};
    std::uint32_t flags{};
    std::uint32_t routeFlags{};
    std::uint32_t moveFlags{};
    std::uint8_t phase{};
    std::uint8_t savedPhase{};
    std::uint8_t moveState{};
    std::uint8_t stairFlags{};
    std::array<std::byte, 4> runtime094{};
    NpcRouteLiveEntry *other{};
    std::uint8_t field0a0{};
    std::int8_t field0a1{};
    std::uint8_t avoidanceResult{};
    std::int8_t avoidanceAuxiliary{};
    Vector3 scenePosition{};
    Vector3 localPosition{};
    Vector3 moveTarget{};
    Vector3 savedSplinePosition{};
    Vector3 savedMoveTarget{};
    float motionStep{};
    Vector3 savedEuler{};
    std::uint32_t tangentYaw{};
    std::uint32_t field0f4{};
    std::uint32_t targetYaw{};
    std::uint32_t runtime0fc{};
    std::int32_t maximumYawStep{};
    std::uint16_t motion{};
    std::uint16_t stuckCounter{};
    std::uint16_t arrivalYaw{};
    std::uint16_t field10a{};
    Vector3 stuckPosition{};
    Vector3 sceneTarget{};
    Vector3 targetPosition{};
    float motionScale{};
    std::int8_t floor{};
    std::uint8_t field135{};
    std::uint16_t doorMotion{};
    float laneOffset{};
    float field13c{};
    float lateralPosition{};
    float laneHigh{};
    float laneLow{};
    float segmentLaneHigh{};
    float segmentLaneLow{};
    float nextLaneHigh{};
    float nextLaneLow{};
    float interpolatedLaneHigh{};
    float interpolatedLaneLow{};
    float laneStep{};
    float movementProgress{};
    std::uint16_t currentAreaPoint{};
    std::uint16_t pendingAreaPoint{};
    shen_name otherName{};
    Vector3 doorPosition{};
    std::uint8_t avoidancePhase{};
    std::int8_t stairRemaining{};
    std::uint16_t stairCount{};
    Vector3 stairTarget{};
    std::int32_t stairYaw{};
    float stairMotionFrames{};
    NpcRouteLink linkedRoute{};
    std::array<std::int16_t, 72> routeNodes{};
    std::array<std::int8_t, 72> routeConnections{};
    std::int16_t doorWaitFrames{};
    std::int16_t avoidanceWaitFrames{};
    std::int16_t laneCorrectionFrames{};
    std::int16_t obstructionFrames{};
    std::int32_t waitStartTime{};
};
static_assert(offsetof(NpcRouteStateContext, parameter) == 0x1e);
static_assert(offsetof(NpcRouteStateContext, pointCount) == 0x30);
static_assert(offsetof(NpcRouteStateContext, spline) == 0x3c);
static_assert(offsetof(NpcRouteStateContext, flags) == 0x84);
static_assert(offsetof(NpcRouteStateContext, motionStep) == 0xe0);
static_assert(offsetof(NpcRouteStateContext, phase) == 0x90);
static_assert(offsetof(NpcRouteStateContext, scenePosition) == 0xa4);
static_assert(offsetof(NpcRouteStateContext, localPosition) == 0xb0);
static_assert(offsetof(NpcRouteStateContext, laneOffset) == 0x138);
static_assert(offsetof(NpcRouteStateContext, motionScale) == 0x130);
static_assert(offsetof(NpcRouteStateContext, currentAreaPoint) == 0x16c);
static_assert(offsetof(NpcRouteStateContext, linkedRoute) == 0x198);
static_assert(offsetof(NpcRouteStateContext, routeNodes) == 0x1a4);
static_assert(sizeof(NpcRouteStateContext) == 0x288);
struct NpcMotionAttachment {
    std::uint16_t flags{};
    std::uint16_t motion{};
    float frame{};
};
static_assert(sizeof(NpcMotionAttachment) == 8);




struct NpcRouteLiveEntry {
    shen_name name{};
    float field004{};
    Character *node{};
    MotionWork *motionWork{};
    std::array<std::byte, 8> runtime018{};
    NpcRouteLiveEntry *previous{};
    NpcRouteLiveEntry *next{};
    const std::byte *recordBase{};
    std::int64_t (*stateCallback)(NpcRouteLiveEntry *){};
    NpcRouteStateContext stateContext{};
    std::uint8_t routeMotionState{};
    std::uint8_t opcodeFlags{};
    std::int8_t primarySlotIndex{};
    std::uint8_t opcodeByte{};
    std::int8_t controlRowIndex{};
    std::int8_t nextControlRowIndex{};
    std::int8_t ownerRowIndex{};
    std::int8_t nextOwnerRowIndex{};
    std::uint16_t field2d0{};
    std::uint16_t waitParameter{};
    std::uint16_t commandFlags{};
    std::uint16_t field2d6{};
    std::uint64_t field2d8{};
    const std::byte *specialAnimationContext{};
    std::int32_t field2e8{};
    std::uint32_t field2ec{};
    NpcRouteLiveEntry *areaPointPrevious{};
    NpcRouteLiveEntry *areaPointNext{};
    void *timedMotionHandle{};
    std::uint32_t controlFlags{};
    std::uint32_t idleCounter{};
    NpcMotionAttachment motionAttachments[3];
    std::uint32_t flags{};
    std::int16_t humanPackageIndex{};
    std::uint8_t scheduleProgress{};
    std::uint8_t field32f{};
    std::uint8_t liveState{};
    std::uint8_t field331{};
    std::uint8_t previousLiveState{};
    std::uint8_t specialMotionPhase{};
    std::uint8_t streamRouteChangeKind{};
    std::int8_t field335{};
    std::int8_t routeDirectionMode{};
    std::int8_t eyeControlMode{};
    std::uint8_t field338{};
    std::int8_t eyeControlProfile{};
    std::int16_t routeEyeMode{};
    std::uint16_t currentAreaPoint{0xffff};
    std::uint16_t pendingAreaPoint{0xffff};
    std::uint16_t field340{};
    std::uint16_t field342{};
    std::int8_t activeBranchIndex{};
    std::int8_t loadHumanResult{};
    std::uint16_t currentMotionId{};
    std::uint32_t packedMoveFlags{};
    std::int8_t clipOverrideIndex{};
    std::int8_t streamCost{};
    std::uint8_t localBranchCount{};
    std::uint8_t globalBranchCount{};
    std::array<std::byte, 0x30> runtime350{};
    float motionSpeed{};
    float streamSortKey{};
    float forcedY{};
    float streamDistanceBias{};
    std::int32_t delayStartTime{};
    std::int32_t delayDuration{};
    const std::byte *currentCommand{};
    const std::byte *branchCommandStart{};
    const std::byte *resumeCommand{};
    const std::byte *nextTimedCommand{};
    std::int32_t nextTimedCommandTime{};
    shen_name loadHumanName{};
    Vector3 euler{};
    Vector3 localPosition{};
    Vector3 worldPosition{};
    Vector3 specialMotionTarget{};
    Vector3 specialMotionSavedPosition{};
    Vector3 streamCenter{};
    float streamRadius{};
    std::int16_t poolIndex{};
    std::uint16_t field40e{};
    std::uint16_t streamingCountdown{};
    std::uint16_t field412{};
    std::uint16_t field414{};
    std::int16_t counter416{};
    float animationFrame{};
    std::uint32_t pad41c{};
};

static_assert(offsetof(NpcRouteLiveEntry, node) == 0x008);
static_assert(offsetof(NpcRouteLiveEntry, recordBase) == 0x030);
static_assert(offsetof(NpcRouteLiveEntry, flags) == 0x328);
static_assert(offsetof(NpcRouteLiveEntry, liveState) == 0x330);
static_assert(offsetof(NpcRouteLiveEntry, routeDirectionMode) == 0x336);
static_assert(offsetof(NpcRouteLiveEntry, eyeControlMode) == 0x337);
static_assert(offsetof(NpcRouteLiveEntry, eyeControlProfile) == 0x339);
static_assert(offsetof(NpcRouteLiveEntry, currentAreaPoint) == 0x33c);
static_assert(offsetof(NpcRouteLiveEntry, currentMotionId) == 0x346);
static_assert(offsetof(NpcRouteLiveEntry, animationFrame) == 0x418);
static_assert(offsetof(NpcRouteLiveEntry, specialMotionTarget) == 0x3e4);
static_assert(sizeof(NpcRouteLiveEntry) == 0x420);

void npcInit();
void PublishNpcRouteLiveEntries(std::span<NpcRouteLiveEntry> entries);
void UnpublishNpcRouteLiveEntries();
void ResetNpcRouteLiveEntries();
void BindNpcRouteCharacter(shen_name name, Character *character);
void UnbindNpcRouteCharacter(const Character *character);
std::int32_t QueryNpcRouteLiveState(shen_name character);

bool npcIsNpcTaskActive();

NpcRouteStateContext *FindNpcRouteStateContext(shen_name character);
std::uint64_t IsNpcLoadedOrVisible(shen_name character);
bool IsNpcRouteReady(shen_name character);
void npcReleaseSpecialAnchor(shen_name character);
bool IsNpcRouteActive(shen_name character);
bool HasNpcFreeTalkFlag(shen_name character);
std::int32_t AcquireNpcRuntimeEntryForScript(shen_name character, NpcRouteLiveEntry **output);
bool InitializeNpcSpecialRouteDirection(NpcRouteLiveEntry &entry);
std::uint16_t SelectNpcIdleMotion(const NpcRouteLiveEntry &entry);
std::uint16_t SelectNpcSeatedIdleMotion(const NpcRouteLiveEntry &entry);
void HandleNpcEyeControl(NpcRouteLiveEntry &entry);
std::uint16_t SelectNpcSpecialMotion(const NpcRouteLiveEntry &entry, bool varied);
std::uint16_t SelectNpcAreaActionBaseMotion(const NpcRouteLiveEntry &entry, std::uint8_t action);
std::uint16_t RemapNpcMotionForRig(std::int32_t rig, std::uint16_t motion);
bool TickNpcSpecialMotion(NpcRouteLiveEntry &entry);
std::int32_t GetNpcRouteCurrentParameter(shen_name character);
std::int32_t GetNpcRouteStride(shen_name character);
std::int32_t GetNpcRouteLowParameter(shen_name character);
std::int32_t GetNpcRouteHighParameter(shen_name character);
std::int32_t GetNpcRouteMaximumParameter(shen_name character);
std::int32_t SampleNpcRoute(shen_name character, std::int32_t parameter, Vector3 *position, std::int32_t *yaw);
std::int32_t FindNearestNpcRouteParameter(shen_name character, const Vector3 *position);
float GetNpcRouteSegmentLength(shen_name character, std::int32_t parameter);
bool GetNpcRouteLaneBounds(shen_name character, std::int32_t parameter, float *low, float *high);
std::int64_t SetNpcRouteYaw(shen_name character, std::int32_t yaw);
std::int64_t MarkNpcRoutePartyControl(shen_name character);
std::int64_t ClearNpcRoutePartyControl(shen_name character);
bool RequestNpcSpecialMotionTarget(shen_name character, const Vector3 *target);
void PauseNpcRouteMotion(NpcRouteStateContext *context, shen_name character);
std::int8_t EV_SetCharaControllerFlags(shen_name character, std::uint32_t flags, NpcCharacterControllerState state);
void EV_SetRefBoardParamsFor(shen_name character, std::int32_t enabled);


void SetScenarioFollowCharacter(shen_name character);
shen_name GetScenarioFollowCharacter();
std::int32_t GetScenarioFollowGraceFrames();
void SetScenarioFollowGraceFrames(std::int32_t frames);
std::int32_t GetScenarioFollowCandidateIndex();
void SetScenarioFollowCandidateIndex(std::int32_t index);
void UpdateScenarioFollowCharacter(bool characterPresent);

} // namespace shenmue::s2
