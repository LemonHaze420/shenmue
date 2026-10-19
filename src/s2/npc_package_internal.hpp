#pragma once
#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace shenmue::s2 {
namespace npc_package_detail {}
using namespace npc_package_detail;

extern Vector3 npcPlayerPosition;

extern float npcPlayerRadius;

extern float npcHourWeatherForecast;

extern std::int32_t g_usingCustomAuthorCharacterList;

extern std::int32_t npcPoolTarget;

namespace npc_package_detail {

inline constexpr std::size_t npcNoSpawnAreaCapacity = 8;

struct NpcNoSpawnArea {
    shen_name name{};
    Vector3 center{};
    float radius{};
};

struct NpcRegistryTask {
    HLib::Task *pending{};
    HLib::Task *current{};
    std::array<NpcNoSpawnArea, npcNoSpawnAreaCapacity> areas{};
    std::int32_t areaCount{};
};

struct AfsEntry {
    std::uint32_t offset;
    std::uint32_t size;
};
extern std::uint32_t npcNewLoadState;

extern NpcRegistryTask npcRegistryTask;

extern std::span<NpcRouteLiveEntry> npcRouteEntries;

extern std::span<NpcRouteLiveEntry> npcEntryStorage;

extern std::span<std::uint8_t> npcSlotFlags;

extern std::span<std::byte> npcBinaryStorage;

extern bool npcSceneCheckEnabled;

extern std::size_t npcAuthoredEntryCount;

inline constexpr std::size_t npcScriptEntryCount = 6;
extern const std::array<std::byte, 0x38> npcScriptRecord;

NpcRouteLiveEntry *FindActiveNpcEntry(shen_name name);

extern float npcCrowdDensityScale;

extern float npcEndOfDayWeatherForecast;

extern std::uint32_t npcWaitFrames;

extern bool npcReloadPassStarted;

extern std::array<char, 64> npcScheduleFilename;

extern bool npcWaitBypassActive;

extern bool npcWaitSawLoadFailure;

extern std::int32_t npcCurrentTimeSimState;

extern std::int32_t npcIsTimeAdvancing;

extern std::int32_t npcTimeAdvanceBreak;

extern std::uint32_t npcSimDeltaTime;

extern std::uint32_t npcPresentedTime;

void YieldNpcLoadingPresentation();

extern std::int32_t npcLatestTime;

extern DateTime npcPausedDateTime;

extern NpcRouteLiveEntry *npcCrowdHead;

extern NpcRouteLiveEntry *npcCrowdBookmark;

extern Vector3 npcPlayerCenter;

extern float npcPlayerDisplacement;

extern bool npcPlayerPresent;

extern std::uint32_t npcRouteBuildCount;

extern std::uint32_t npcFrameCount;

extern std::uint8_t npcCrowdPhase;

extern std::int32_t npcActiveStreamCost;

extern std::int32_t npcPoolMinimum;

extern float npcStreamThrottle;

extern float npcStreamThrottleWithBias;

extern std::uint32_t npcColdResetMode;

extern NpcRouteResetGroupHeader *npcRouteResetGroup;

extern std::array<NpcTransitionRuntimeSlot, 4> npcTransitionRuntimeSlots;

extern std::array<TimedNodeMotion, 4> npcTransitionMotionCaches;

extern bool npcDrawActive;

extern shen_name npcInterpolatingCharacter;

extern shen_name npcLoadedArea;

extern const shen_name *areaNpcLoadCharacters;

extern HLib::Task *areaNpcLoadStartupTask;

extern std::int32_t areaNpcLoadPhase;

extern std::int32_t areaNpcLoadCharactersVisible;

extern std::int32_t areaNpcLoadLowQualityHands;

extern const shen_name *genericNpcLoadCharacters;

extern HLib::Task *genericNpcLoadStartupTask;

extern std::int32_t genericNpcLoadPhase;

extern std::int32_t genericNpcLoadCharactersVisible;

extern std::int32_t genericNpcLoadLowQualityHands;

struct ParsedNpcRecord {
    shen_name name{};
    std::uint32_t offset{};
    std::uint32_t endOffset{};
    std::uint16_t branchCount{};
    std::int16_t humanPackageIndex{-1};
};

struct NpcScheduleStorage {
    std::span<std::byte> bytes;
    std::vector<ParsedNpcRecord> records;
    std::vector<shen_name> humanPackages;
    std::size_t branchCount{};
    std::size_t commandCount{};
    std::size_t areaPointCount{};
    std::size_t routeNodeCount{};
    std::uint32_t areaPointTableOffset{};
    std::uint32_t routeNodeTableOffset{};
    std::uint32_t routeResetTableOffset{};
    std::uint32_t routeFlags{};
    std::uint32_t entryPointScalarTableOffset{};
    std::int32_t floor{-1000};
    std::vector<std::uint8_t> routeVisited;
    Vector3 areaOrigin{};
};
extern NpcScheduleStorage npcSchedule;

extern std::array<shen_name, 2> npcScheduleSeedNames;

extern DateTime npcCurrentDateTime;

extern std::uint32_t npcCurrentSeconds;

extern std::uint32_t npcCondition2StartSeconds;

extern std::uint32_t npcCondition2OutsideFrames;

extern const std::byte *pendingNpcScheduleSeeds;

extern bool hasPendingNpcScheduleSeeds;

extern NpcRouteLiveEntry *npcScriptRouteOwner;

extern NpcRouteStateContext *npcActiveRouteContext;

extern NpcRouteStateContext npcCachedAreaRouteContext;

extern shen_name npcCachedAreaRouteCharacter;

extern std::uint32_t npcCachedAreaRouteId;

struct NpcMotionClip {
    std::byte *allocation{};
    const Matrix4x4 *frames{};
    std::uint32_t boneCount{};
    std::uint32_t frameCount{};
};
extern std::array<NpcMotionClip, 8> npcMotionClips;

extern bool npcMotionClipsLoaded;

extern bool npcUsePositionCullCallback;

extern std::array<std::int32_t, 32> npcMotionBankHandles;

extern std::size_t npcMotionBankHandleCount;

extern bool npcMotionBanksLoaded;

extern std::array<std::int32_t, 5> npcMotionPropHandles;

extern std::size_t npcMotionPropHandleCount;

struct NpcTimedMotionSlot {
    shen_name name{};
    std::uint32_t padding04{};
    Character *character{};
    std::uint16_t occupied{};
    std::uint16_t motion{};
    std::uint32_t padding14{};
    TimedNodeMotion animation{};
    Vector3 offset{};
    std::uint32_t padding54{};
};
extern std::array<NpcTimedMotionSlot, 2> npcTimedMotionSlots;

extern std::int32_t npcTimedMotionCount;

enum class NpcHumanStreamState : std::uint8_t {
    Inactive,
    Requested,
    PackageReady,
    ReleasePending,
    Live,
    Missing,
};

struct NpcRouteRuntimeRecord {
    const std::byte *begin{};
    const std::byte *end{};
    std::uint16_t branchCount{};
    Character *character{};
    shen_name packageAlias{};
    NpcHumanStreamState streamState{NpcHumanStreamState::Inactive};
};
extern std::vector<NpcRouteRuntimeRecord> npcRouteRuntimeRecords;

extern const std::array<std::uint8_t, 59> npcOpcodeSizeDwords;

void InitializeNpcInterpreterState(NpcRouteLiveEntry &entry, std::int32_t slot);
bool NpcRouteConditionsChanged(NpcRouteLiveEntry &entry);
void TickNpcRouteLiveEntries();
void DrawNpcTransitions();

void InitializeStreamedNpcRoute(NpcRouteLiveEntry &entry, bool warm = false);
void RefreshNpcScriptRouteMotion(NpcRouteStateContext &context);
bool RequestNpcStreamLoad(NpcRouteLiveEntry &entry);
std::int64_t TickNpcAreaRoute(NpcRouteLiveEntry *routeEntry, std::int32_t currentTime);
void SetNpcHumanVisibilityChannels(NpcRouteLiveEntry &entry, bool enabled);
bool ResumeNpcTimedMotion(NpcRouteLiveEntry &entry);
void ApplyNpcScriptStateSeedPayload(NpcRouteLiveEntry &entry, const NpcSavedRouteState &saved);
void RestoreNpcAreaPointSeedPosition(NpcRouteLiveEntry &entry);
extern std::int32_t npcPositionQueryMode;

extern std::byte *npcHumanIndex;

extern std::int32_t npcHumanCount;

extern NpcHumanPakSlot *npcHumanSlots;

extern PakAfsPartition *npcHumansAfs;

extern std::int32_t npcLastHumanLoadResult;

extern std::int32_t npcHumansInUse;

extern bool npcHasCachedPak;

extern std::uint64_t npcLargestSceneBlock;

extern std::uint64_t npcLargestDynamicBlock;

bool ReleaseNativeNpcHumanPackage(shen_name name);

void ReleaseNpcTransitionHumanPackages();

bool IsNpcGloballyBlocked(const NpcRouteLiveEntry &entry);

std::int32_t LoadNativeNpcHuman(shen_name packageName, shen_name characterName, bool fromHumans);

std::uint16_t ReadU16(const std::byte *source);

std::uint32_t ReadU32(const std::byte *source);

void ReleaseNativeNpcHumanPackages();

bool AllocateNPCH();

bool Contains(std::span<const std::byte> bytes, std::uint64_t offset, std::uint64_t size);

bool AllocateNpcTimedMotion(NpcRouteLiveEntry &entry);

void ReleaseNpcTimedMotion(NpcRouteLiveEntry &entry);

inline constexpr std::size_t npcControlFlagsOffset = 0x308;
inline constexpr std::size_t npcActiveBranchOffset = 0x344;
inline constexpr std::size_t npcLocalBranchesOffset = 0x350;
inline constexpr std::size_t npcForcedYPositionOffset = 0x388;
inline constexpr std::size_t npcDelayStartOffset = 0x390;
inline constexpr std::size_t npcDelayDurationOffset = 0x394;
inline constexpr std::size_t npcCurrentCommandOffset = 0x398;
inline constexpr std::size_t npcBranchCommandOffset = 0x3a0;
inline constexpr std::size_t npcResumeCommandOffset = 0x3a8;
inline constexpr std::size_t npcLoadHumanResultOffset = 0x345;
inline constexpr std::size_t npcNextTimedCommandOffset = 0x3b0;
inline constexpr std::size_t npcNextTimedCommandTimeOffset = 0x3b8;
inline constexpr std::size_t npcLoadHumanNameOffset = 0x3bc;
inline constexpr std::size_t npcYawOffset = 0x3c4;
inline constexpr std::size_t npcLocalPositionOffset = 0x3cc;
inline constexpr std::size_t npcWorldPositionOffset = 0x3d8;
inline constexpr std::uint32_t npcMotionStateActive = 0x00020000U;
inline constexpr std::uint32_t npcForcedYPosition = 0x00002000U;

struct NpcLocalBranchTrigger {
    std::uint32_t packedCondition{0xffffffffU};
    std::uint32_t conditionArgument{};
    std::uint8_t targetBranch{};
    std::uint8_t reserved{};
    std::uint16_t commandOffset{};
};

struct NpcGlobalBranchTrigger {
    shen_name owner;
    std::uint32_t packedCondition{0xffffffffU};
    std::uint32_t conditionArgument{};
    std::uint8_t targetBranch{};
    std::uint8_t reserved{};
    std::uint16_t commandOffset{};
};
extern std::array<NpcGlobalBranchTrigger, 16> npcGlobalBranchTriggers;

enum class NpcConditionResult {
    False,
    True,
};

template <typename T> T ReadNpcLiveField(const NpcRouteLiveEntry &entry, std::size_t absoluteOffset) {
    T value{};
    std::memcpy(&value, reinterpret_cast<const std::byte *>(&entry) + absoluteOffset, sizeof(value));
    return value;
}

template <typename T> void WriteNpcLiveField(NpcRouteLiveEntry &entry, std::size_t absoluteOffset, const T &value) {
    std::memcpy(reinterpret_cast<std::byte *>(&entry) + absoluteOffset, &value, sizeof(value));
}
NpcRouteRuntimeRecord *GetNpcRuntimeRecord(NpcRouteLiveEntry &entry);

bool NpcRecordContains(const NpcRouteRuntimeRecord &runtime, const std::byte *cursor, std::size_t size);

float ReadF32(const std::byte *source);

std::int16_t FindNpcHumanPackageIndex(shen_name name);
void InitializeNpcInterpreterState(NpcRouteLiveEntry &entry, std::int32_t slot);

std::int16_t FindNpcHumanPackageIndex(shen_name name);

void ReleaseNpcRouteStream(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, bool clearPackageAlias);

void SetNpcRouteDirection(NpcRouteLiveEntry &entry, std::uint16_t point);
const std::byte *GetNpcAreaPoint(std::uint16_t point);

const std::byte *GetNpcRouteNode(std::uint16_t node);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

} // namespace npc_package_detail

std::uint64_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

namespace npc_package_detail {

void ResetNpcRoutes(NpcRouteResetGroupHeader *groups);

std::int32_t NpcRouteYaw(float deltaX, float deltaZ);

void SetNpcYawBits(NpcRouteLiveEntry &entry, std::int32_t yaw);

bool SetNpcAreaPointTangentYaw(NpcRouteLiveEntry &entry, std::uint16_t point);

struct NpcRouteSearch {
    std::array<std::int16_t, 72> nodes{};
    std::array<std::int8_t, 72> connections{};
    std::int32_t count{};
    std::int32_t pointCount{};
    float length{};
};
void UpdateNpcRouteWindow(NpcRouteStateContext &context, std::int32_t parameter, bool maintainWindow = false);

std::int32_t ReadNpcEulerYaw(const Vector3 &euler);

Vector3 TransformNpcPoint(const Vector3 &point);

Vector3 NpcAreaPosition(Vector3 position);

std::uint16_t SelectNpcLocomotion(const NpcRouteStateContext &context, std::uint32_t selector);

void InitializeNpcLocomotionProfile(NpcRouteStateContext &context);

void SetNpcLocomotionScale(NpcRouteStateContext &context, float scale);

std::uint8_t UpdateNpcRouteAvoidance(NpcRouteStateContext &context);
bool RequestNpcRouteMotion(NpcRouteStateContext &context, bool explicitRequest);

void InitializeNpcRouteRequest(NpcRouteStateContext &context, std::uint16_t current, std::uint16_t target);

struct NpcAvoidanceCandidate {
    std::uint16_t kind{};
    std::uint16_t flags{};
    std::uint32_t reserved{};
    NpcRouteLiveEntry *source{};
    float localX{};
    float localZ{};
    float lateralRadius{};
    float forwardRadius{};
    float distance{};
    Vector3 position{};
    NpcAvoidanceCandidate *previous{};
    NpcAvoidanceCandidate *next{};
};
extern std::array<NpcAvoidanceCandidate, 5> npcAvoidanceCandidates;

extern NpcAvoidanceCandidate *npcAvoidanceFirst;

extern NpcAvoidanceCandidate *npcAvoidanceLast;

extern float npcAvoidanceLow;

extern float npcAvoidanceHigh;

extern float npcAvoidanceLateral;

extern float npcAvoidanceRadius;

extern float npcAvoidanceDistance;

std::uint8_t UpdateNpcRouteAvoidance(NpcRouteStateContext &context);

std::int32_t FindNearestNpcParameter(NpcRouteStateContext &context, const Vector3 &position);

void EnterNpcRoute(NpcRouteStateContext &context);

void BuildNpcRoute(NpcRouteStateContext &context);

void BindNpcRouteWindow(NpcRouteStateContext &context);

void TickNpcPausedRoute(NpcRouteStateContext &context);

void InitializeStreamedNpcRoute(NpcRouteLiveEntry &entry, bool warm);

std::int64_t NpcRouteScheduleCallback(NpcRouteLiveEntry *entry);

bool BeginNpcAreaRoute(NpcRouteLiveEntry &entry, std::uint16_t targetPoint, float authoredScale);

std::int64_t TickNpcAreaRoute(NpcRouteLiveEntry *routeEntry, std::int32_t currentTime);

void SetNpcRouteDirection(NpcRouteLiveEntry &entry, std::uint16_t point);

void SetNpcHumanVisibilityChannels(NpcRouteLiveEntry &entry, bool enabled);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry &entry);

void InterpretNpcSchedule(NpcRouteLiveEntry &entry, std::int32_t currentTime);

std::int32_t AdjustedNpcStreamTarget();

bool RequestNpcStreamLoad(NpcRouteLiveEntry &entry);

std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry &entry, bool reload);

void RankNpcStreamEntry(NpcRouteLiveEntry &entry, std::uint8_t &slotFlags, const Vector3 &cameraPosition);

void UpdateNpcStreamFade(NpcRouteLiveEntry &entry);

void UpdateNpcMotionAttachments(NpcRouteLiveEntry &entry);

void UpdateNpcFrameCulling(NpcRouteLiveEntry &entry);

void UpdateNpcStreamThrottle(bool forceForecast);

void TickNpcRouteLiveEntries();

void DrawNpcTransitions();

bool ReadArchiveFile(const char *path, std::vector<std::byte> &bytes);

bool ReadCString(std::span<const std::byte> bytes, std::uint32_t offset, std::string &value);

bool IsNpcRouteArea(shen_name area);

void ReleaseNpcMapResources();

bool npcInitDataInternal();

void RefreshNpcScriptRouteMotion(NpcRouteStateContext &context);

bool MoveForNpcInData(NpcRouteLiveEntry &entry, std::uint32_t area, bool apply);

void ApplyNpcScriptStateSeedPayload(NpcRouteLiveEntry &entry, const NpcSavedRouteState &saved);

void RestoreNpcAreaPointSeedPosition(NpcRouteLiveEntry &entry);

bool NpcRouteConditionsChanged(NpcRouteLiveEntry &entry);

void WarmBootstrapNpcRoutes();

bool LoadCurrentNpcSchedule(bool loadResources, const char *requestedFilename = nullptr);

} // namespace npc_package_detail

void BuildRouteTimeWindow(std::int32_t startTick, std::int32_t endTick, std::int32_t skipAuthoredSeed);

void TickNpcMotionClip(NpcRouteLiveEntry &entry);

bool SetNpcMotionAttachment(NpcRouteLiveEntry &entry, std::uint16_t motion, std::uint8_t flags, std::int32_t slot);

void ClearNpcMotionAttachment(NpcRouteLiveEntry &entry, std::int32_t slot);

std::int32_t SetNpcStreamLifecycleState(NpcRouteLiveEntry &entry, std::int32_t mode);

bool RequestNpcStreamOut(NpcRouteLiveEntry &entry, std::int32_t type);

bool CanRevealNpcStream(const NpcRouteLiveEntry &entry);

bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags);

Vector3 NpcRouteAreaToScene(Vector3 position);

void SetNpcRouteMotionState(NpcRouteStateContext &context, std::int32_t operation, bool explicitRequest);

void ResumeNpcScheduleMotion(NpcRouteLiveEntry &entry);

bool TickNpcRouteStateContext(NpcRouteStateContext *state);

bool ReleaseNpcRouteControl(NpcRouteStateContext *context, shen_name character, std::int32_t yaw);

void RebuildNpcRouteCollision(bool reloading);

void ResetNpcScheduleSeeds();

std::span<NpcRouteLiveEntry> CreateNpcRouteLiveEntries(std::span<const NpcRouteRecordSource> records);

void FreeNpcRouteLiveEntries();

bool npcIsNpcTaskActive();

NpcRouteLiveEntry *WarmStartNpcRoute(shen_name character);

std::int64_t EV_CheckWarmStartNpcRoute(shen_name character);

std::int64_t npcSetLock(shen_name character, std::int32_t preserveMotion);

namespace npc_package_detail {

struct ConditionalNpcLoadSlot {
    shen_name character;
    std::int32_t entryIndex;
};
struct ConditionalNpcLoadWork {
    const ConditionalNpcLoadEntry *entries;
    std::uint8_t phase;
    std::int8_t entryIndex;
    std::uint16_t reserved0a;
    std::uint32_t admitted[8];
    std::uint32_t reserved2c;
};
extern std::array<ConditionalNpcLoadSlot, 8> conditionalNpcLoadSlots;

extern std::int32_t conditionalNpcLoadsReady;

void ConditionalNpcLoadCallback(void *);
void ConditionalNpcLoadCallback(void *data);

} // namespace npc_package_detail

std::int64_t EV_NPC_NewLoadSw(std::uint32_t state);

std::int64_t EV_ResetFreeTalkFlags();

} // namespace shenmue::s2
