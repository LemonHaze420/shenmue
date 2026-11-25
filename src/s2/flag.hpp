#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/flag_data.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/scenario_flags_dc.hpp"
#endif

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

namespace HLib {
struct Task;
}
struct ConversationBuffer;
struct PakGlobalData;
struct SearchTarget;
struct MT7Model;
struct MT7Node;
struct Character;

inline constexpr std::array<std::uint32_t, 62> FLAG_DEBUG_MODEL_COMMAND_TEMPLATE{
    0x00000001U,
    0x00000001U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x3f3504f3U,
    0x8200002dU,
    0x83400000U,
    0x909804c0U,
    0x04000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x3f3504f3U,
    0x00000000U,
    0x00000000U,
    0x3f800000U,
    0x3f333333U,
    0x3f333333U,
    0x3f333333U,
    0x3f333333U,
    0x00000000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x00000088U,
    0x00000011U,
    0x00000004U,
    0xbf000001U,
    0x00000000U,
    0x3f000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x00000001U,
    0xbf000001U,
    0x00000000U,
    0xbf000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x00000001U,
    0x3f000001U,
    0x00000000U,
    0x3f000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x00000001U,
    0x3f000001U,
    0x00000000U,
    0xbf000000U,
    0x00000000U,
    0x3f800000U,
    0x80000000U,
    0x00000000U,
    0x00000001U,
    0x00000000U,
    0x00000004U,
};
extern std::uint8_t g_flagDebugIntersectionDraw;
std::uint32_t flagDrawDebugQuad(const Vector3 &first, const Vector3 &second, const Vector3 &third, const Vector3 &fourth);
std::uint32_t flagDrawDebugCylinder(const Vector3 &location, float radius);
std::uint32_t flagDrawDebugBox(float firstX, float firstZ, float secondX, float secondZ, float y);

inline constexpr std::int32_t FLAG_NIGHT_TIME_ENFORCED = 0xff;
inline constexpr shen_name FLAG_WIND_COMPONENT{SHEN_NAME(FLAG)};



struct FlagWindVertex {
    Vector3 *position;
    Vector3 originalPosition;
    std::uint32_t reserved14;
};
struct FlagWindModel {
    std::int32_t type;
    std::uint32_t reserved04;
    MT7Node *node;
    void *mesh;
    std::int32_t vertexCount;
    std::uint32_t reserved1c;
    FlagWindVertex *vertices;
    std::int32_t phase;
    std::int32_t speed;
    float maximumX, minimumX, maximumY, minimumY, maximumZ, minimumZ;
};
struct FlagWindObject {
    MT7Node *root;
    std::uint32_t modelCount;
    std::uint32_t reserved0c;
    FlagWindModel *models;
    FlagWindVertex *vertices;
};
static_assert(sizeof(FlagWindVertex) == 0x18);
static_assert(sizeof(FlagWindModel) == 0x48);
static_assert(offsetof(FlagWindModel, vertexCount) == 0x18);
static_assert(offsetof(FlagWindModel, vertices) == 0x20);
static_assert(sizeof(FlagWindObject) == 0x20);
extern FlagWindObject *g_flagWindObject;
FlagWindObject *GetCurrentFlagObjectForWind();
FlagWindModel *FindFlagWindModel(const MT7Node *node);
// One placement of an animated FLAG mesh. The first placement moves the character, the rest redraw its node tree.
struct FlagAnimationPlacement {
    Vector3 translation;
    std::int32_t rotationZ;
    std::int32_t rotationY;
    std::uint32_t reserved14;
    std::int32_t phase;
    std::int32_t speed;
};
static_assert(sizeof(FlagAnimationPlacement) == 0x20);
static_assert(offsetof(FlagAnimationPlacement, phase) == 0x18);

struct FlagAnimationObject {
    MT7Model *model;
    FlagWindVertex *vertices;
    std::int32_t vertexCount;
    std::uint32_t reserved14;
    FlagAnimationPlacement *placements;
    std::int32_t placementCount;
    std::uint32_t reserved24;
};
static_assert(sizeof(FlagAnimationObject) == 0x28);
static_assert(offsetof(FlagAnimationObject, placements) == 0x18);
FlagWindObject *AllocateFlagObject(MT7Node *root);

void AssignAnimatedFlag(shen_name name, FlagAnimationPlacement *placements, std::int32_t count);
void AttachFlagWindComponent(Character *character);
void UpdateWindForModel(FlagWindObject *object);

void UpdateWindForFlag();

void nullsub(...) noexcept;

void nullsub(void *) noexcept;

bool ComputeAngle(const Vector3 &origin, float radius, std::int16_t yaw, std::int32_t halfAngle, const Vector3 &point);



// Context table rows name the record list and search-target table of a map or callback context.
// Both offsets are dwords from the start of the context table; a zero name terminates the table.

// Flag records start with the stepUpdateFlagUtil mode table offset and mode count; the
// GetShenmueFlag condition stream follows this header.
struct FlagDefinition {
    std::int32_t stepModeTable;
    std::uint16_t stepModeCount;
    std::uint16_t reserved06;
};

static_assert(sizeof(FlagDefinition) == 0x08);

// GetShenmueFlag condition opcodes; the operand is the low 24 bits of each command dword.
enum class ShenFlagOp : std::uint8_t {
    End = 0x00,
    StepAtLeastToggleA = 0x01,
    StepAtLeastToggleB = 0x02,
    RequireInternalFlagSet = 0x03,
    RequireInternalFlagClear = 0x04,
    CommitGroup = 0x05,
    ResetGroup = 0x06,
    ToggleMode = 0x07,
    RequireFlag = 0x08,
    RequireCalmWind = 0x09,
    RequireTimeInRange = 0x0c,
    RequireItem = 0x0e,
    RequireStatAtLeast = 0x10,
    RequireStatBelow = 0x11,
};

// stepUpdateFlagUtil commands.
enum class StepFlagOp : std::uint8_t {
    End = 0x00,
    SetInternalFlag = 0x02,
    ClearInternalFlag = 0x03,
    SetStep = 0x04,
    RaiseStep = 0x05,
    ApplyInvertedFlagSteps = 0x06,
    Extended = 0x07,
    ClearStat = 0x09,
    AddStat = 0x0a,
    SubtractStat = 0x0b,
};

// Operand of StepFlagOp::Extended; its handler returns the argument dwords it consumed.
enum class StepFlagExtendedOp : std::uint32_t {
    ScheduleFlagCallback = 2,
    Skip3 = 3,
    SetDefaultStats = 7,
    SelectSleepSchedule = 8,
    RandomizeStats = 9,
    UpdateCurfewFlag = 10,
};

// stepEvaluateFlagStepRange commands and modes.
enum class StepRangeOp : std::uint8_t {
    End = 0x00,
    MinimumStep = 0x01,
    MaximumStep = 0x02,
    SetInternalFlag = 0x03,
    ClearInternalFlag = 0x04,
    EndGroup = 0x05,
    EndGroupB = 0x06,
    ResetRange = 0x07,
    Include = 0x08,
    EndGroupC = 0x09,
    RaiseStat = 0x10,
    LowerStat = 0x11,
};

enum StepRangeMode : std::uint32_t {
    StepRangeMode_Minimum = 0,
    StepRangeMode_Maximum = 1,
    StepRangeMode_ApplyFlags = 2,
    StepRangeMode_ApplyAndSetStep = 3,
};

// shenEvaluateFlag record opcodes.



struct ShenPlayerData {
    std::uint16_t fadeHoldFrames;
    std::int16_t sleepScheduleFlag;
    std::int16_t queryArgument;
    std::uint16_t requestedHelpTopic;
    std::uint16_t helpTopic;
    std::uint8_t freeQuestActive;
    std::uint8_t reserved0b;
    std::uint32_t loadingPhase;
    std::uint32_t flags;
    std::uint32_t state;          // +0x14, bit 0: saved area-jump location at FLGS construction
    FlagRecord *firstRecord;
    FlagRecord *triggerRecord;
    FlagRecord *requiredRecord;
    std::int32_t idleEvaluations;
    std::uint32_t reserved34;
    PakGlobalData *pendingPack;
    std::int32_t pendingEventType;
    std::uint32_t reserved44;
    SearchTarget *targets;
    shen_name context;
    std::uint32_t reserved54;
    FlagRecord *record;
    FlagRecord *deadlineRecord;
    std::int32_t updateCount;
    std::int32_t queryId;
    shen_name callbackContext;
    std::uint32_t reserved74;
    FlagRecord *deferredRecord;
    std::int32_t messageFrames;
    std::uint32_t reserved84;
    ConversationBuffer *voice;
    std::int32_t callDepth;
    std::uint32_t reserved94;
};

static_assert(sizeof(ShenPlayerData) == 0x98);
static_assert(offsetof(ShenPlayerData, loadingPhase) == 0x0c);
static_assert(offsetof(ShenPlayerData, state) == 0x14);
static_assert(offsetof(ShenPlayerData, idleEvaluations) == 0x30);
static_assert(offsetof(ShenPlayerData, pendingPack) == 0x38);
static_assert(offsetof(ShenPlayerData, targets) == 0x48);
static_assert(offsetof(ShenPlayerData, record) == 0x58);
static_assert(offsetof(ShenPlayerData, callbackContext) == 0x70);
static_assert(offsetof(ShenPlayerData, voice) == 0x88);
static_assert(offsetof(ShenPlayerData, callDepth) == 0x90);


struct FlagCallbackEntry {
    std::int32_t flag;
    std::int32_t minute;
};

inline constexpr std::array<FlagTargetOffset, 14> flagTargetOffsets{{
    {.position = {.x = 0.0f, .y = 0.74f, .z = 0.13f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.3f, .y = 1.11f, .z = 0.32f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = -0.34f, .y = 1.08f, .z = 0.57f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 1.04f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = 16384, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 1.04f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = 16384, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 1.35f, .z = 0.1f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 1.6f, .z = 0.1f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 1.36f, .z = 0.37f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 0.45f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 0.55f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 0.95f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 7.6f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 1.3f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 15928},
    {.position = {.x = 0.0f, .y = 1.3f, .z = 0.0f}, .lockonAngle = 0.5f, .yaw = -32768, .yawLimit = 5461},
}};

struct SleepScheduleEntry {
    std::int16_t flag;
    std::int16_t holdWindowStartMinute;
    std::int16_t holdWindowEndMinute;
    std::uint16_t kind;
    std::int16_t entryId;
    std::int16_t reserved0a;
    shen_name area;
};

static_assert(sizeof(SleepScheduleEntry) == 0x10);

struct FlagContextSelector {
    std::int32_t flag;
    shen_name context;
};

struct FlagCandidate {
    FlagRecord *record;
    float distanceSquared;
    std::uint32_t reserved0c;
};

static_assert(sizeof(FlagCandidate) == 0x10);

extern ShenPlayerData shenPlayerData;
extern std::int32_t g_systemManageDebugDisplay;

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetShenmueFlag(std::int32_t flag);
#endif
std::int32_t EV_GetShenmueFlag(std::int32_t flag);
// Bit 0 of the result reports a STEP write.
std::uint32_t stepUpdateFlagUtil(std::uint8_t mode, std::int32_t flag);
void EV_stepSetFlags(std::uint8_t mode, std::int32_t flag);
std::int32_t stepEvaluateFlagStepRange(std::int32_t flag, std::uint32_t mode);
std::int32_t stepScheduleFlagCallback();

std::int32_t flagInitContext(ShenPlayerData &data, shen_name context);
std::int32_t flagRelated(shen_name map);
std::int32_t shenUpdatePlayerDataFlags(std::int32_t active);
std::int64_t shenRestartPlayerDataFlagEvaluation();
std::int32_t shenUpdateFlagEvaluation(std::int32_t mode);
std::int32_t shenEvaluateFlag(ShenPlayerData &data, std::int32_t mode);
std::int32_t shenSetPlayerDataFields(std::int32_t queryId, std::int8_t queryArgument);
std::int32_t shenEvaluateFlagFor(std::int32_t queryId, std::int8_t queryArgument);
std::int32_t shenScheduleFlagCallback(std::int16_t minute);
std::int32_t npcSetCycleTimeLimitExpiry(shen_name id, std::int16_t timeLimit);
SearchTarget *shenFindFlagTarget(shen_name id);
std::int32_t ryoIsCurrentScenarioFlagSatisfied();
std::int32_t shenGetMinutesUntilNextFrameMinute();
std::int16_t shenGetSleepScheduleFlag();
std::uint32_t shenIsCurfewSceneChange();
void shenSetFreeQuestInactive();
std::uint32_t GetLoadingPhaseFlags();
void SetLoadingPhaseFlags(std::uint32_t flags);


void Queue_FLAG_CheckTask();
void FLAG_CheckTask_Callback(void *taskData);



HLib::Task *initCtrlSystemManage();
void CtrlSystemManage_Callback(void *taskData);

const char *GetAberdeenSearchData() noexcept;

} // namespace shenmue::s2
