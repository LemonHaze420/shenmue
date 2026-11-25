#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "common/math.hpp"
#include "s2/motion_node_dc.hpp"
#endif

#define SHENMUE_NPC_JOIN_(a, b) a##b
#define SHENMUE_NPC_JOIN(a, b) SHENMUE_NPC_JOIN_(a, b)
#define SHENMUE_NPC_LAYOUT(condition) typedef char SHENMUE_NPC_JOIN(NpcLayout_, __LINE__)[(condition) ? 1 : -1]
#define SHENMUE_NPC_NOEXCEPT
namespace shenmue {
namespace s2 {

struct PakGlobalData;
namespace NpcHumanPakState {
enum Type { Empty, Requested, Ready, Attached };
}
struct NpcHumanPakSlot {
    NpcHumanPakState::Type state;
    PakGlobalData *pak;
};
SHENMUE_NPC_LAYOUT(sizeof(NpcHumanPakSlot) == 8);
SHENMUE_NPC_LAYOUT(offsetof(NpcHumanPakSlot, pak) == 4);

static const std::uint32_t NPC_HUMAN_INDEX_MEMORY = SHEN_NAME4('N', 'P', 'C', 'I'), NPC_HUMAN_SLOT_MEMORY = SHEN_NAME4('N', 'P', 'C', 'H');
static const std::uint32_t NPC_BINARY_MEMORY = SHEN_NAME4('N', 'P', 'C', 'B'), NPC_ENTRY_MEMORY = SHEN_NAME4('N', 'P', 'C', 'C');
static const std::uint32_t NPC_AREA_ROUTE_MEMORY = SHEN_NAME4('S', 'T', 'R', 'L');
static const std::size_t kNpcEntryMemoryBytes = 0x73eac;
static const std::size_t kNpcAreaRouteMemoryBytes = 0x2000;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const char *const kNpcBinaryFilenames[6] = {"NPC.BIN", "NPC.BIN", "NPC.BIN", "NPC_Q100.BIN", "NPC_KRN1.BIN", 0};
#else
static const char *const kNpcBinaryFilenames[6] = {"NPC.BIN", "NPC.BIN", "NPC.BIN", "NPC_YUAN.BIN", "NPC.BIN", "NPC.BIN"};
#endif

extern std::byte *npcHumanIndex;
extern NpcHumanPakSlot *npcHumanSlots;
extern std::byte *npcBinaryStorage;
extern std::uint32_t npcBinaryStorageSize;
extern std::byte *npcEntryStorage;
extern std::byte *npcAreaRouteStore;
extern std::uint32_t npcNewLoadState;
extern shen_name npcScheduleSeedNames[2];
extern std::uint32_t hasPendingNpcScheduleSeeds;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct NpcRouteLiveEntry;
extern const std::byte *pendingNpcScheduleSeeds;
void RestoreNpcScheduleSeedPayload(const void *payload);
void CaptureNpcScheduleSeedPayload(void *payload);
std::int32_t CaptureNpcRouteSeedData(const NpcRouteLiveEntry *entry, std::uint32_t *data);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct NpcAreaProfile {
    union {
        std::uint32_t selectorsOffset;
        shen_name *selectors;
    };
    union {
        std::uint32_t filenameOffset;
        const char *filename;
    };
    union {
        std::uint32_t motionFilesOffset;
        const char **motionFiles;
    };
    std::int16_t maximumLiveCount, condition;
    union {
        std::uint32_t clipFilesOffset;
        const char **clipFiles;
    };
    std::uint32_t reserved;
    std::uint32_t motionMode;
};
SHENMUE_NPC_LAYOUT(sizeof(NpcAreaProfile) == 0x1c);
SHENMUE_NPC_LAYOUT(offsetof(NpcAreaProfile, maximumLiveCount) == 0x0c);
SHENMUE_NPC_LAYOUT(offsetof(NpcAreaProfile, condition) == 0x0e);
SHENMUE_NPC_LAYOUT(offsetof(NpcAreaProfile, clipFiles) == 0x10);
SHENMUE_NPC_LAYOUT(offsetof(NpcAreaProfile, motionMode) == 0x18);
struct NpcMotionClipHeader {
    std::uint32_t boneCount, frameCount;
    std::byte reserved[0x38];
};
SHENMUE_NPC_LAYOUT(sizeof(NpcMotionClipHeader) == 0x40);
static const std::uint32_t NPC_AREA_PROFILE_MEMORY = SHEN_NAME4('N', 'P', 'C', 'A');
static const std::uint32_t NPC_MOTION_CLIP_MEMORY = SHEN_NAME4('N', 'P', 'C', 'M');
static const std::uint32_t NPC_BASE_MOTION_PROP_MEMORY = SHEN_NAME4('N', 'P', 'C', 'P');
static const std::uint32_t NPC_PROFILE_MOTION_PROP_MEMORY = SHEN_NAME4('N', 'P', 'C', 'p');
extern char npcScheduleFilename[32];
extern const Matrix4x4 *npcMotionClipFrames[8];
extern std::uint32_t npcMotionClipBoneCounts[8], npcMotionClipFrameCounts[8];
extern std::uint16_t npcMotionClipFallbackAnimations[8];
std::int32_t MatchNpcAreaSelector(shen_name selector, shen_name area);
void LoadNpcMotionClipFile(const char *filename, std::int32_t slot, std::uint16_t fallbackMotion);
std::int32_t ReadNPCArea(shen_name area, std::int32_t loadResources, const char *filename);
struct Character;
struct MotionWork;
struct NpcRouteLiveEntry;
struct NpcRouteSpline {
    Vector3 *points;
    std::int32_t pointCount;
    float *segmentLengths;
    std::int16_t startConnections[4], endConnections[4];
    float connectionLengths[8];
    float *laneBounds;
    std::uint8_t *auxiliary;
    shen_name name;
    std::uint32_t flags;
};
SHENMUE_NPC_LAYOUT(sizeof(NpcRouteSpline) == 0x4c);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteSpline, segmentLengths) == 8);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteSpline, laneBounds) == 0x3c);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteSpline, auxiliary) == 0x40);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteSpline, flags) == 0x48);
struct DateTime;
extern DateTime npcSavedDateTime;
extern std::int32_t npcSavedDateTimeFlag;
std::int32_t GetNpcSavedDateTimeFlag();
void SetNpcSavedDateTimeFlag();
std::int32_t ClearNpcSavedDateTimeFlag();
void SaveNpcDateTime();
struct NpcAreaRestriction {
    shen_name name;
    Vector3 center;
    float radius;
};
SHENMUE_NPC_LAYOUT(sizeof(NpcAreaRestriction) == 0x14);
SHENMUE_NPC_LAYOUT(offsetof(NpcAreaRestriction, center) == 4);
SHENMUE_NPC_LAYOUT(offsetof(NpcAreaRestriction, radius) == 0x10);
struct NpcRouteStateContextPrefix {
    NpcRouteLiveEntry *owner;
    const NpcRouteSpline *stateTable;
    std::int16_t startNode, endNode;
    std::uint8_t routeCount, activeRouteIndex;
    std::int16_t routeParameter;
    std::uint16_t stride;
    std::int16_t lowerParameter, upperParameter;
    std::int16_t activeNode, nextNode, activeSegment;
    std::int16_t previousSegment;
    std::int16_t pointCount;
    Vector3 spline[4];
    std::int16_t activeWindow;
    std::uint16_t retained52;
    std::uint8_t routeType;
    std::uint8_t routePending;
    std::int16_t activePointBase;
    float segmentLength;
    std::int16_t sampledWindow, sampledPointBase, sampledRouteIndex, sampledParameter;
    Vector3 position;
    shen_name scriptOwner;
    std::uint32_t flags, routeFlags, moveFlags;
    std::uint8_t phase, savedPhase, moveState, stairFlags;
    NpcRouteLiveEntry *other;
    std::uint8_t retained88, avoidanceFlags, avoidanceResult;
    std::int8_t avoidanceAuxiliary;
    std::byte retained8c[4];
    Vector3 scenePosition, localPosition;
    Vector3 previousPosition;
    Vector3 moveTarget, savedSplinePosition, savedMoveTarget;
    std::byte retainedd8[0x0c];
    float motionStep;
    Vector3i savedEuler;
    std::int32_t tangentYaw;
    std::uint32_t retainedf8, retainedfc;
    std::int32_t maximumYawStep;
    std::uint32_t targetYaw;
    Vector3 sceneTarget;
    std::int16_t targetParameter;
    std::uint16_t stuckCounter;
    Vector3 targetOrigin;
    std::int32_t arrivalYaw;
    Vector3 *appliedMovement;
    std::byte retained12c[4];
    MotionWork *motionWork;
    float motionScale;
    std::uint16_t motion;
    std::uint8_t field13a, field13b;
    float retained13c, laneOffset, savedLaneOffset;
    float lateralPosition, laneHigh, laneLow;
    float segmentLaneHigh, segmentLaneLow, nextLaneHigh, nextLaneLow;
    float minimumLaneHigh, maximumLaneLow, interpolatedLaneHigh, interpolatedLaneLow;
    float laneStep;
    float movementProgress;
    std::int16_t currentAreaPoint, pendingAreaPoint;
    std::uint32_t doorMotion;
    shen_name otherName;
    Vector3 doorPosition;
    std::uint8_t avoidancePhase;
    std::int8_t stairRemaining;
    std::uint16_t stairCount;
    Vector3 stairTarget;
    std::int32_t stairYaw;
    float stairMotionFrames;
    NpcRouteLiveEntry *followingEntry;
    float followingDistance;
    std::int16_t routePath[72];
    std::int8_t routeConnections[72];
    std::int16_t doorWaitFrames, avoidanceWaitFrames;
    std::int32_t pauseStartTime;
};
struct NpcTimedMotionSlotPrefix {
    shen_name name;
    Character *character;
    std::uint16_t occupied, motion;
    std::byte retained0c[4];
    TimedNodeMotion animation;
    Vector3 offset;
};
struct NpcAreaAnimationScriptPrefix {
    std::uint32_t retained00;
    std::uint16_t flags;
};
struct NpcMotionAttachment {
    std::uint16_t flags, motion;
    float frame;
};
struct NpcLocalBranchTrigger {
    std::uint32_t packedCondition, parameter;
    std::int8_t target;
    std::byte retained09;
    std::uint16_t commandOffset;
};
struct NpcRouteLiveEntry {
    shen_name name;
    Character *node;
    MotionWork *motionWork;
    std::byte retained0c[4];
    NpcRouteLiveEntry *previous, *next;
    std::byte *scriptRecord;
    std::int32_t (*stateCallback)(NpcRouteLiveEntry *entry);
    std::byte retained20[4];
    NpcRouteStateContextPrefix stateContext;
    std::int8_t routeMotionState;
    std::uint8_t opcodeFlags;
    std::int8_t primarySlotIndex;
#if SHENMUE_S2_GCC295
    std::int8_t opcodeByte;
#else
    std::uint8_t opcodeByte;
#endif
    std::int8_t controlRowIndex, nextControlRowIndex;
    std::int8_t ownerRowIndex;
    std::int8_t nextOwnerRowIndex;
    std::int8_t animationRepeats, animationRepeatGroups;
    std::uint16_t waitParameter;
    std::uint16_t commandFlags;
    std::uint16_t retained2c6;
    std::int16_t areaTransitionFrames;
    std::byte retained2ca[2];
    const NpcAreaAnimationScriptPrefix *specialAnimationContext;
    std::uint32_t retained2d0;
    NpcRouteLiveEntry *areaPointPrevious, *areaPointNext;
    NpcTimedMotionSlotPrefix *timedMotionHandle;
    std::uint32_t controlFlags;
    std::uint32_t attachmentFrameCounter;
    NpcMotionAttachment motionAttachments[3];
    std::uint32_t flags;
    std::byte retained304[2];
    std::int16_t humanPackageIndex;
    std::uint8_t scheduleProgress, retained309;
    std::uint8_t liveState, retained30b, previousLiveState, specialMotionPhase, streamRouteChangeKind;
    std::int8_t fadeCounter;
    std::int8_t routeQueryState, pendingRouteQueryState;
    std::byte retained312[2];
    std::int16_t currentAreaPoint;
    std::int16_t pendingAreaPoint;
    std::int16_t animationFrame;
    std::uint16_t streamRetryTicks, retained31c, currentMotionId;
    std::uint32_t packedMoveFlags;
    std::byte retained324;
    std::int8_t streamCost;
    std::int8_t localBranchCount, globalBranchCount;
    NpcLocalBranchTrigger localBranches[4];
    float motionSpeed;
    std::byte retained35c[4];
    float streamSortKey;
    std::byte retained364[4];
    float forcedY;
    std::byte retained36c[8];
    std::int32_t waitStartTime, waitDuration;
    const std::byte *currentCommand, *branchCommandStart;
    const std::byte *loopCommand;
    const std::byte *nextTimedCommand;
    std::int32_t nextTimedCommandTime;
    shen_name loadHumanName;
    Vector3i rotation;
    Vector3 localPosition, worldPosition;
    Vector3 previousPosition;
    Vector3i alternateRotation;
    Vector3 alternatePosition, alternateWorldPosition;
    Vector3 specialMotionTarget;
    Vector3 specialMotionRestorePosition;
    Vector3 streamCenter;
    float streamRadius;
    shen_name attachedCharacter;
    Vector3 attachmentOffset;
    std::byte retained420[4];
    std::int32_t attachmentYaw;
    std::byte retained428[8];
    std::int32_t poolIndex, loadHumanResult;
    std::uint32_t areaLookFrames;
};
struct NpcRouteResetGroupHeader {
    shen_name area;
    std::uint32_t retained04;
};
struct NpcRouteResetEntry {
    shen_name character;
    union {
        std::int32_t nodeOffset;
        Character *node;
    };
    std::int32_t routeNode, parameter;
    std::uint32_t parameterStep;
    Vector3 previousPosition, position;
};
struct NpcTransitionRuntimeSlot {
    shen_name character;
    Vector3 position;
    std::int16_t yaw, visibility;
    const std::uint16_t *motionSequence;
    Character *node;
    std::int8_t animationSlot, motionCursor;
    std::byte retained1e[2];
};
SHENMUE_NPC_LAYOUT(sizeof(NpcTimedMotionSlotPrefix) == 0x3c);
SHENMUE_NPC_LAYOUT(offsetof(NpcTimedMotionSlotPrefix, animation) == 0x10);
SHENMUE_NPC_LAYOUT(offsetof(NpcTimedMotionSlotPrefix, offset) == 0x30);
SHENMUE_NPC_LAYOUT(sizeof(NpcRouteStateContextPrefix) == 0x294);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, flags) == 0x74);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, routeFlags) == 0x78);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, stateTable) == 4);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, motionStep) == 0xe4);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, motionWork) == 0x130);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, motionScale) == 0x134);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, position) == 0x64);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, phase) == 0x80);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, localPosition) == 0x9c);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, targetYaw) == 0x104);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, doorMotion) == 0x180);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, otherName) == 0x184);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, doorPosition) == 0x188);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, avoidancePhase) == 0x194);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, stairTarget) == 0x198);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteStateContextPrefix, routeConnections) == 0x244);
SHENMUE_NPC_LAYOUT(sizeof(NpcRouteLiveEntry) == 0x43c);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, node) == 4);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, stateContext) == 0x24);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, routeMotionState) == 0x2b8);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, ownerRowIndex) == 0x2be);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, specialAnimationContext) == 0x2cc);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, timedMotionHandle) == 0x2dc);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, controlFlags) == 0x2e0);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, flags) == 0x300);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, fadeCounter) == 0x30f);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, currentAreaPoint) == 0x314);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, streamCost) == 0x325);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, loadHumanName) == 0x390);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, poolIndex) == 0x430);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, loadHumanResult) == 0x434);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, scriptRecord) == 0x18);
SHENMUE_NPC_LAYOUT(offsetof(NpcRouteLiveEntry, worldPosition) == 0x3ac);
SHENMUE_NPC_LAYOUT(sizeof(NpcRouteResetGroupHeader) == 8);
SHENMUE_NPC_LAYOUT(sizeof(NpcRouteResetEntry) == 0x2c);
SHENMUE_NPC_LAYOUT(sizeof(NpcTransitionRuntimeSlot) == 0x20);
SHENMUE_NPC_LAYOUT(offsetof(NpcTransitionRuntimeSlot, node) == 0x18);
extern NpcAreaRestriction npcAreaRestrictions[8];
extern std::int32_t npcAreaRestrictionCount, npcScheduleFloor;
bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags);
std::int32_t npcSetAreaName(shen_name name, const Vector3 *center, float radius, std::int32_t enabled);
extern std::int32_t npcActiveStreamCost;
extern std::int32_t npcPoolTarget;
extern std::uint8_t npcSlotFlags[0x1c0];
extern NpcRouteResetGroupHeader *npcRouteResetGroup;
extern shen_name npcLoadedArea;
shen_name GetNpcAreaPointOwnerName(std::int32_t point);
shen_name GetNpcAreaPointAreaName(std::int32_t point);
shen_name npcFindAreaPointOccupant(shen_name owner);
extern NpcTransitionRuntimeSlot npcTransitionRuntimeSlots[4];
std::uint32_t EV_NPC_NewLoadSw(std::uint32_t state) SHENMUE_NPC_NOEXCEPT;
std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;
std::int32_t npcCancelHumanPak(shen_name name);
std::int32_t npcReleasePak(shen_name name);
void ResetNpcRouteContext(NpcRouteStateContextPrefix *context);
void ReleaseNpcRouteStream(NpcRouteLiveEntry *entry);
std::int32_t BeginNpcRouteStreamOut(NpcRouteLiveEntry *entry, std::int32_t forced);
std::int32_t UpdateNpcStreamFade(NpcRouteLiveEntry *entry);
void ReleaseNpcTransitionRoutePackages();
void ReleaseNpcTransitionSlotPackages();
void ReleaseNpcTransitionHumanPackages();
std::int32_t EV_ResetFreeTalkFlags();
std::int32_t PrepareNpcAllFromArray();
std::int32_t npcReleaseUnusedStreams();
namespace HLib {
struct Task;
}
extern HLib::Task *npcRegistryTask;
extern std::int32_t npcAuthoredEntryCount, npcRouteEntryCount, npcRouteTableReady;
extern std::uint32_t npcFrameCount;
extern std::int32_t npcScheduleRecordCount;
extern std::byte **npcScheduleRecords;
extern NpcRouteSpline *npcScriptRouteTable;
void InitializeNpcRouteContext(NpcRouteLiveEntry *entry);
void ResetNpcRouteAvoidance(NpcRouteStateContextPrefix *context);
void InitializeNpcLocomotionProfile(Character *character, NpcRouteStateContextPrefix *context);
void InitializeNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, float scale);
std::int32_t TickNpcRouteStair(Character *character, NpcRouteStateContextPrefix *context);
bool RequestNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, std::int32_t explicitRequest);
void ResetNpcScriptContext(NpcRouteStateContextPrefix *context);
std::int32_t CreateScriptNpcRouteSlot(shen_name name);
std::int32_t StartNpcScriptAreaRoute(shen_name name, shen_name oldPoint, shen_name newPoint, std::int32_t yaw);
std::int32_t InitializeNpcMovement(shen_name name, NpcRouteSpline *routes, const Vector3 *origin, std::int32_t yaw);
void InitializeNpcInterpreterState(NpcRouteLiveEntry *entry, std::int32_t slot);
void LoadNPCFiles(NpcRouteSpline *routes);
HLib::Task *npc_Callback_Enqueue(std::int8_t index, std::int32_t act, shen_name area);
void NPC_Route_Control();
void LoadCurrentNpcSchedule(const char *filename, std::int32_t loadResources);
extern std::uint32_t npcScheduleVersion, npcScheduleFlags;
extern std::byte *npcScheduleOriginData, *npcScheduleEntryData;
char *GetNpcScheduleFilename();
std::uint32_t GetNpcScheduleVersion();
std::byte *GetNpcScheduleEntryData();
extern std::int32_t npcScheduleAreaPointCount, npcScheduleRouteCount;
extern NpcRouteLiveEntry *npcCrowdHead, *npcCrowdBookmark;
extern std::uint16_t npcRouteMovementMask;
extern std::uint8_t npcUsesMainSchedule;
extern std::int8_t npcRouteMovementCursor;
extern NpcRouteStateContextPrefix *npcActiveRouteContext;
extern Character *npcRouteCharacter;
extern MotionWork *npcRouteMotionWork;
extern NpcRouteLiveEntry *npcRouteOwner;
extern NpcRouteLiveEntry *npcSceneRouteOwner;
extern shen_name npcPreviewRouteName, npcPreviewRouteArea;
extern Vector3 npcRouteTarget;
std::int32_t ResumeNpcRouteAfterSceneChange(shen_name name);
std::int32_t StopNpcScriptRoute(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t yaw);
bool SetNpcRouteYaw(shen_name name, std::int32_t yaw);
std::int32_t TickNpcRouteStateContext(NpcRouteStateContextPrefix *context);
std::uint32_t SelectNpcRouteMovementSlot();
bool QueryNpcSceneCheck(shen_name name);
std::int32_t SelectNpcSpecialMotion(NpcRouteLiveEntry *entry, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::int32_t SelectNpcIdleSpecialMotion(NpcRouteLiveEntry *entry, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry *entry);
void ResumeNpcAreaSpecialMotion(NpcRouteLiveEntry *entry);
std::int32_t TickNpcSpecialMotion(NpcRouteLiveEntry *entry);
void UpdateNpcMotionAttachments(NpcRouteLiveEntry *entry);
std::int32_t AcquireNpcRuntimeEntryForScript(shen_name name, NpcRouteLiveEntry **entry);
bool BeginNpcSpecialMotion(shen_name name);
std::int32_t BeginNpcFollowingMotion(shen_name name);
std::int32_t StopNpcFollowingMotion(shen_name name);
std::int32_t SetNpcSpecialRouteQueryPaused(shen_name name, std::int32_t enabled);
bool IsNpcSpecialTargetWithinRange(Character *character, const NpcRouteLiveEntry *entry);
bool CheckNpcSpecialMotionTarget(shen_name name, Character *character);
bool EndNpcSpecialMotion(shen_name name);
bool RequestNpcSpecialMotionTarget(shen_name name, const Vector3 *target);
bool ResetNpcSpecialMotionTarget(shen_name name);
bool RequestNpcSpecialMotionLookTarget(shen_name name, const Vector3 *target);
bool ResetNpcSpecialMotionLookTarget(shen_name name);
bool MoveForNpcInData(NpcRouteLiveEntry *entry, shen_name area, std::int32_t apply);
std::int32_t RequestNpcSceneRoute(shen_name name, shen_name area);
extern std::int32_t npcTransitionsEnabled;
void InitializeNpcRouteResetGroups(NpcRouteResetGroupHeader *groups);
std::int32_t IsNpcRegistryTaskActive() SHENMUE_NPC_NOEXCEPT;
std::uint32_t GetNpcNewLoadState() SHENMUE_NPC_NOEXCEPT;
std::int32_t npcGetCurrentTimeSimState();
std::int32_t npcSetCurrentTimeSimStateEnabled();
std::int32_t ResetCurrentTimeSimState();
void npcAdvanceTime(std::int32_t hour, std::int32_t minute);
void SimulateNpcScheduleTime(std::int32_t startTime, std::int32_t endTime, std::int32_t onlyUninitialized);
std::int32_t EV_npcStreamOrLoadHuman(shen_name name);
void RestorePendingNpcScheduleSeeds(std::int32_t currentTime);
void InterpretNpcScheduleEntry(NpcRouteLiveEntry *entry, std::int32_t currentTime);
std::int32_t ExecuteNpcScheduleOpcode(NpcRouteLiveEntry *entry, std::int32_t currentTime);
std::int32_t QueueNpcMotionAttachment(NpcRouteLiveEntry *entry, std::uint16_t motion, std::int32_t flags, std::int32_t slot);
void ClearNpcMotionAttachment(NpcRouteLiveEntry *entry, std::int32_t slot);
void ReleaseNpcTimedMotion(NpcRouteLiveEntry *entry);
NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;
std::int32_t npcGetPosFor(shen_name name, Vector3 *position);
std::int32_t GetNpcRestingMotion(const Character *character);
struct PakAfsPartition;
extern PakAfsPartition *npcHumansAfs;
extern std::int32_t npcHumansReady;
extern std::int32_t npcHumansInUse, npcHasCachedPak;
extern std::size_t npcLargestSceneBlock;
void npcSetupHumans();
extern std::int32_t npcLastHumanLoadResult;
extern NpcTimedMotionSlotPrefix npcTimedMotionSlots[2];
extern std::int32_t npcTimedMotionCount;
bool AllocateNpcTimedMotion(NpcRouteLiveEntry *entry);
bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);
void UpdateStreamedNpcMotion(NpcRouteLiveEntry *entry);
void BindNpcRouteMotionWork(NpcRouteStateContextPrefix *context, shen_name name, MotionWork *work);
void BindNpcRouteAppliedMovement(NpcRouteStateContextPrefix *context, shen_name name);
std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);
void SetNpcRouteMotionState(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t operation);
void BindNpcRouteCharacter(NpcRouteLiveEntry *entry);
std::int32_t RequestNpcStreamLoad(NpcRouteLiveEntry *entry);
std::int32_t EV_ForceLoadNpc(shen_name name);
std::int32_t EV_npcLockUnlock(shen_name name);
void ResumeNpcMotionAfterUnlock(NpcRouteLiveEntry *entry);
void ResumeNpcScheduleMotion(NpcRouteLiveEntry *entry);
std::uint16_t SelectNpcWalkingMotion(const Character *character);
std::uint16_t SelectNpcProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::uint32_t IsNpcRouteActive(shen_name name);
bool IsNpcRouteContextMoving(const NpcRouteStateContextPrefix *context);
bool IsNpcRouteMoving(shen_name name);
float GetNpcRouteCurrentStep(shen_name name);
std::int32_t GetNpcRouteAvoidanceSide(shen_name name);
std::int32_t QueryNpcRouteLiveState(shen_name name);
std::int32_t GetNpcRouteMotionMode(shen_name name);
NpcRouteStateContextPrefix *FindNpcRouteStateContext(shen_name name);
std::int32_t IsNpcRouteReady(shen_name name);
std::int32_t GetNpcRouteCurrentParameter(shen_name name);
std::int32_t GetNpcRouteLowParameter(shen_name name);
std::int32_t GetNpcRouteHighParameter(shen_name name);
std::int32_t GetNpcRouteStride(shen_name name);
std::int32_t GetNpcRouteMaximumParameter(shen_name name);
std::int32_t FindNearestNpcRouteParameter(shen_name name, const Vector3 *position);
std::int32_t SampleNpcRoute(shen_name name, std::int32_t parameter, Vector3 *position, std::int32_t *yaw);
float QueryNpcRouteGeometry(shen_name name, std::int32_t parameter, std::int32_t mode, float *low, float *high);
float GetNpcRouteSegmentLength(shen_name name, std::int32_t parameter);
bool QueryNpcCameraMotionProfile(shen_name name, std::uint32_t flags);
bool IsNpcSceneMotionPlaying(shen_name name);
bool QueryNpcSceneMotionPlaying(shen_name name);
std::int32_t IsNpcRouteLoadedOrVisible(shen_name name) SHENMUE_NPC_NOEXCEPT;
std::int32_t NpcWait() SHENMUE_NPC_NOEXCEPT;
std::int32_t CountNpcLiveStreams() SHENMUE_NPC_NOEXCEPT;
std::int32_t npcGetCanInteract(shen_name name);
bool CanNpcEnterScenario(shen_name name);
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name name);
std::int32_t EV_CheckWarmStartNpcRoute(shen_name name);
std::int32_t PrepareNpcRouteReload(shen_name name);
std::int32_t IsNpcRouteReloadPrepared(shen_name name);
std::int32_t EV_npcFree(shen_name name);
std::int32_t EV_npcUpdateFlags(shen_name name);
std::int32_t EV_npcUnknownFunc(shen_name name);
std::uint32_t SetNpcRouteControlHold(shen_name name, std::int32_t enabled);
std::int32_t GetNpcScriptRouteCompletion(shen_name name);
static const std::uint8_t npcMotionRigScales[] = {11, 11, 11, 10, 10, 11, 12, 12, 12, 12, 11, 10, 10, 11, 11, 10};
float GetNpcMotionRigScale(const Character *character);
void SetNpcLocomotionScale(NpcRouteStateContextPrefix *context, shen_name name, float scale);
void SetNpcScriptRouteSpeed(NpcRouteStateContextPrefix *context, shen_name name, float scale);
extern Vector3 npcRouteOrigin;
extern NpcRouteSpline *npcRouteNodes;
static const Matrix4x4 npcSplineBasis = {SHEN_FIELD(rows){
    {-0.16666667f, 0.5f, -0.5f, 0.16666667f}, {0.5f, -1.0f, 0.0f, 0.66666669f}, {-0.5f, 0.5f, 0.5f, 0.16666667f}, {0.16666667f, 0.0f, 0.0f, 0.0f}}};
std::int32_t SampleNpcRouteNodeWindow(shen_name name, std::int32_t parameter, const Vector3 *start, Vector3 *position, std::int32_t *yaw, std::int32_t step);
void InitializeNpcRouteTable(NpcRouteSpline *routes, std::int32_t storedClearance);
void ProjectNpcRouteHeights(NpcRouteSpline *routes);
void RebuildNpcRouteLanes(NpcRouteSpline *routes, std::int32_t measure, std::int32_t sampleStep);
void BuildNpcSplineInterpolationMatrix(Matrix4x4 *matrix, float parameter);
std::int32_t SampleNpcSplineWindow(const Vector3 *points, std::int32_t pointCount, float parameter, Vector3 *position);
std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);
void MeasureNpcRouteClearance(const Vector3 &start, const Vector3 &end, float &left, float &right, std::uint32_t moveFlags, std::int32_t step);
std::int32_t CheckNpcRouteIntersections(
    const Vector3 *points, std::int32_t pointCount, float *bounds, std::int32_t sampleStep, std::uint32_t moveFlags, std::uint8_t *auxiliary);
void SetNpcScriptRouteFlag(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t operation, std::uint32_t flags);
#endif

void npcInitData();
void AllocateNPCH();
void ClearNpcHumanSlots();
bool npcInitDataInternal();
void InitializeNpcEntryStorage();
bool InitializeNpcAreaRouteStore();
void ResetNpcLoadState();
void ResetNpcScheduleSeeds();

} // namespace s2
} // namespace shenmue
#undef SHENMUE_NPC_JOIN_
#undef SHENMUE_NPC_JOIN
#undef SHENMUE_NPC_LAYOUT
#undef SHENMUE_NPC_NOEXCEPT

#endif
