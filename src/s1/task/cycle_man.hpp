#pragma once
#include "logging.hpp"

#include "common/types.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct AuxiliaryCollisionEntry;
namespace CycleManager {

struct TaggedPayload;
} // namespace CycleManager

struct FaceWork;
struct MotionInformationWork;
struct PackSlot;
using CHAR_t = Character;

#pragma pack(push, 1)
struct CycleScriptOwnerHeader {
    int32_t dword0;
    int32_t name;
};

struct CycleScriptFloatState {
    uint8_t gap0[0x58];
    float float58;
};

struct CycleScriptStack {
    CycleScriptOwnerHeader *dword0;
    int16_t state;
    uint8_t gapA[0x1E];
    Vector3 v;
    uint8_t gap34[0x24];
    float float58;
    uint8_t gap5C[0x1C];
    CycleScriptFloatState *qword78;
    uint8_t gap80[8];
    int32_t dword88;
    uint8_t gap8C[0x2C];
    union {
        int32_t dwordB8;
        uint16_t wordB8;
    };
    uint8_t gapBC[0x14];
    uint8_t *qwordD0;
    uint8_t gapD8[0x3C];
    union {
        int32_t dword114;
        struct {
            uint16_t word114;
            uint16_t word116;
        };
    };
    int32_t dword118;
    uint8_t gap11C[0x7F];
    uint8_t motnSteeringActive;
    uint8_t gap19C[0x9D];
    uint8_t byte239;
};
namespace CycleManager {


struct ProgramNode {
    uint8_t gap0[0x8];
    CycleScriptStack *script_stack;
    uint8_t gap10[0x8];
    CycleManager::ProgramNode *next;
};

struct LineData {
    uint8_t gap0[0x8];
    uint32_t dword8;
};

struct SeatLine {
    uint8_t gap0[0xC];
    int32_t offset;
};

struct MorsEntry {
    shen_name name;
    int32_t gap4;
    CycleManager::SeatLine *qword8;
    uint8_t gap10[0x18];
};

struct CycleParams {
    uint8_t gap0[0x4];
    shen_name name;
    int32_t dword8;
};
} // namespace CycleManager


enum CycleOpcode : uint16_t {
    CycleOpcode_RemoveCharacter = 0x0,
    CycleOpcode_InitTimedPath = 0x1,
    CycleOpcode_SetActiveAnimAndMotionVariant = 0x2,
    CycleOpcode_SetPositionAndHeading = 0x3,
    CycleOpcode_ScheduleBoundary = 0x4,
    CycleOpcode_RideVehiclePath2D = 0x5,
    CycleOpcode_WaitSeconds = 0x7,
    CycleOpcode_SetAreaTag = 0x8,
    CycleOpcode_SetActionMode = 0x9,
    CycleOpcode_BeginTalkMoveToTaggedPosition = 0xB,
    CycleOpcode_WaitForTaggedCycleInRadius = 0xC,
    CycleOpcode_RequestNearestDoorTransition = 0xD,
    CycleOpcode_SetScriptVisibleState = 0xF,
    CycleOpcode_BeginObjectInteraction = 0x10,
    CycleOpcode_EndObjectInteraction = 0x11,
    CycleOpcode_WaitHold = 0x12,
    CycleOpcode_FollowStairPath2D = 0x15,
    CycleOpcode_StartLinkedCycle = 0x16,
    CycleOpcode_UseMnrBEntry = 0x17,
    CycleOpcode_ScheduleMnrBEntry = 0x18,
    CycleOpcode_PlayRootedAnim = 0x19,
    CycleOpcode_SetAnimationState = 0x1A,
    CycleOpcode_DriveVehiclePath2D = 0x1B,
    CycleOpcode_DriveForkliftPath2D = 0x1C,
    CycleOpcode_ArmOutsideRadiusFallback = 0x1D,
    CycleOpcode_SavePointerMarker = 0x20,
    CycleOpcode_WalkPath3D = 0x21,
    CycleOpcode_PlayEntryStateAnimPoolUntilTime = 0x22,
    CycleOpcode_SetVehicleActiveState = 0x24,
    CycleOpcode_ModeByteMarker = 0x27,
    CycleOpcode_SetModeByte = 0x28,
    CycleOpcode_SetShopShutterState = 0x2A,
    CycleOpcode_GetOtherChara = 0x2B,
    CycleOpcode_BeginLinkedObjectInteraction = 0x2D,
    CycleOpcode_EndLinkedObjectInteraction = 0x2E,
    CycleOpcode_CreateCharacter = 0x2F,
    CycleOpcode_SetAuxiliaryAnim = 0x30,
    CycleOpcode_Transform = 0x33,
    CycleOpcode_SetForcedMotionToken = 0x35,
    CycleOpcode_InitTimedPathIfAreaMatches = 0x36,
    CycleOpcode_WaitSecondsIfAreaMatches = 0x37,
    CycleOpcode_SetSteeringProfile = 0x38,
    CycleOpcode_WalkPath3DIfAreaMatches = 0x39,
    CycleOpcode_WaitForTaggedTalkMoveInRadius = 0x3C,
    CycleOpcode_FalseReturnMarker = 0x3D,
    CycleOpcode_LiveMoveRequestState = 0x6,
    CycleOpcode_AdvanceScript = 0xA,
    CycleOpcode_DoorUse = 0xE,
    CycleOpcode_PayloadYield = 0x13,
    CycleOpcode_YieldOneFrame = 0x14,
    CycleOpcode_WaitTurnAnimation = 0x1E,
    CycleOpcode_WaitOutsideRadius = 0x1F,
    CycleOpcode_Reserved23 = 0x23,
    CycleOpcode_ApplyTransitionDelta = 0x25,
    CycleOpcode_WaitMotionFullRestore = 0x26,
    CycleOpcode_ScriptTransformOverride = 0x29,
    CycleOpcode_AuxiliaryAnimationTransition = 0x2C,
    CycleOpcode_BikeDismountInteraction = 0x31,
    CycleOpcode_BikeMoveInteraction = 0x32,
    CycleOpcode_SetPositionAndHeadingImmediate = 0x34,
    CycleOpcode_Reserved3A = 0x3A,
    CycleOpcode_Reserved3B = 0x3B,
};

enum CycleMoveObjPhase : uint32_t {
    CycleMoveObjPhase_Uninitialized = 0x0,
    CycleMoveObjPhase_BeginPath = 0x1,
    CycleMoveObjPhase_SetIdleAnim = 0x2,
    CycleMoveObjPhase_SnapToAnchor = 0x3,
    CycleMoveObjPhase_ProcessSubcommands = 0x4,
    CycleMoveObjPhase_ProcessSubcommandsActive = 0x5,
    CycleMoveObjPhase_ProcessSubcommandsEnd = 0x6,
    CycleMoveObjPhase_EndSubcommands = 0x7,
    CycleMoveObjPhase_CalcReturnAnchor = 0x8,
    CycleMoveObjPhase_SetReturnArea = 0x9,
    CycleMoveObjPhase_BeginReturnPath = 0xA,
    CycleMoveObjPhase_Finished = 0xB,
    CycleMoveObjPhase_ResumePath = 0xC,
};

enum CycleFlags : uint32_t {
    CycleFlag_CheckMapIntersections = 0x1,
    CycleFlag_CheckForwardIntersectionRay = 0x2,
    CycleFlag_CheckPlayerTurningBox = 0x4,
    CycleFlag_CheckMoveObjectGeometry = 0x8,
    CycleFlag_SkipMoveObjectSteeringTest = CycleFlag_CheckMoveObjectGeometry,
    CycleFlag_CheckActorClasses0To3 = 0x10,
    CycleFlag_CheckActorClasses4To7 = 0x20,
    CycleFlag_CheckMoveObjectRuntimeActors = 0x40,
    CycleFlag_CheckStaticSteeringVolumes = 0x80,
    CycleFlag_CheckPredictiveActorCorridor = 0x100,
};

enum CycleSteeringState : uint8_t {
    CycleSteerState_Invalid = 0x0,
    CycleSteerState_Normal = 0x1,
    CycleSteerState_TargetReached = 0x2,
    CycleSteerState_ForceWait = 0x3,
    CycleSteerState_PreTurnWait = 0x4,
    CycleSteerState_RequestTurn = 0x5,
    CycleSteerState_BlockedForward = 0x6,
    CycleSteerState_BlockedOther = 0x7,
    CycleSteerState_YawCorrecting = 0x8,
    CycleSteerState_ReversePause = 0x9,
};

enum CycleSteeringCommand : uint8_t {
    CycleSteerCmd_None = 0x0,
    CycleSteerCmd_Resume = 0x1,
    CycleSteerCmd_ToggleReversePath = 0x2,
    CycleSteerCmd_ForceForwardPath = 0x3,
    CycleSteerCmd_BlockedOther = 0x4,
    CycleSteerCmd_YawCorrect = 0x5,
};
namespace CycleManager {



struct MobjData {
    int32_t currentBranchOfs;
    shen_name obj_id;
    uint32_t programIdSuffix;
    float motionRadiusTimingScalar;
    float steeringBodyHalfWidth;
    float steeringBodyHalfDepth;
    float motionScalar18;
    float motionScalar1C;
    uint32_t reserved20;
    uint32_t reserved24;
    uint32_t cycleClassCode;
    uint32_t configWord2C;
    uint32_t anchorGroupCode;
    uint32_t reserved34;
    uint32_t motionVariantCode;
    uint16_t packedMotionParams[4];
    uint8_t reserved44[48];
    uint32_t conditionCount;
    int32_t conditionsStartOffset;
    uint16_t motionParamIds[32];
    int32_t timetableOffsets[16];
};
} // namespace CycleManager

static_assert(sizeof(CycleManager::MobjData) == 0xfc);
static_assert(offsetof(CycleManager::MobjData, motionRadiusTimingScalar) == 0x0c);
static_assert(offsetof(CycleManager::MobjData, cycleClassCode) == 0x28);
static_assert(offsetof(CycleManager::MobjData, packedMotionParams) == 0x3c);
static_assert(offsetof(CycleManager::MobjData, conditionCount) == 0x74);
static_assert(offsetof(CycleManager::MobjData, motionParamIds) == 0x7c);
static_assert(offsetof(CycleManager::MobjData, timetableOffsets) == 0xbc);



struct CycleTimetableRow {
    int32_t timeOrEndMarker;
    int32_t commandStreamOffset;
};
static_assert(sizeof(CycleTimetableRow) == 8);



struct CycleMoveObjCommand {
    uint32_t opcode;
    uint32_t chairGroupId;
    int32_t time;
    uint32_t subcommandCount;
};
static_assert(sizeof(CycleMoveObjCommand) == 0x10);

struct CycleMoveObjectRuntime;

enum BikeSteeringState : uint8_t;
namespace CycleManager {


struct MobjCycle {
    union {
        CycleManager::MobjData *data;
        CycleManager::MobjData *moveConfig;
    };
    union {
        CycleOpcode currentOpcode;
        int16_t state;
    };
    uint16_t activeAnimId;
    int32_t humansIdx;
    shen_name areaID;
    int32_t loadQueueDisposition;
    uint32_t modeValue;
    float pathMoveSpeed;
    int32_t bbbbbbbb;
    float pathSegmentProgress;
    Vector3 pos;
    Vector3 moveTargetPos;
    Vector3 currentWaypointPos;
    float workFloat4C;
    float pathSegmentDistanceOrWork;
    int16_t desiredPathYaw;
    int16_t moveTargetYaw;
    float animMoveSpeedScale;
    float baseMoveSpeed;
    // The current path's points in the cycleman file: Vector3s, or x/z pairs (Vector2) for the 2D vehicle and bike paths.
    Vector3 *pathPoints;
    uint16_t pathPointCount;
    int16_t pathPointIndex;
    int32_t gap6C;
    char *cycleCommandPtr;
    CHAR_t *obj;

    MotionInformationWork *mot;
    uint32_t xm_resume_state;
    int32_t gap8C;
    uint32_t steeringWaitFrames;
    int32_t gap94;
    CycleManager::MobjCycle *trackedMobj;
    uint8_t gapA0[8];
    uint16_t talkMoveFlags;
    uint16_t talkMoveAvoidanceState;
    int32_t actionTimerOrDoorHandle;
    // The cycle's loaded human pack.
    PackSlot *filename;
    int32_t contextValue;
    int32_t gapBC;
    CycleManager::TaggedPayload *taggedPayloadList;
    CycleMoveObjectRuntime *moveObjRuntime;
    // The human pack's BIN file named for the character; SCEN reads its SCNF record 16 bytes in.
    void *loadedBinHandle;
    float cameraSortDistanceSq;
    int32_t gapDC;
    char *activeCommandRecord;
    uint32_t moveObjSubOpcode;
    union {
        struct {
            CycleOpcode savedOpcodeBeforeMoveObj;
            uint16_t field_EE;
        };
        int32_t entryStateCommandOffset;
    };
    uint32_t moveObjCommandOffset;
    uint32_t foundExistingMoveObjRuntime;
    float moveObjRuntimeOffset;
    int32_t moveObjActivationPending;
    int32_t bikeTokenOffset;
    int32_t requestAnimId;
    uint32_t morsCommandArg;
    uint32_t scheduledTriggerTime;
    union {
        int32_t morsAuxOffsetOrState;
        int32_t dword110;
    };
    union {
        uint32_t morsCommandStatus;
        int32_t dword114;
    };
    uint32_t morsCommandOffset;
    uint32_t morsEntryOffset;
    uint32_t morsDeadlineTime;
    uint32_t morsQueueNodeOffset;
    uint32_t field_128;
    int32_t gap12C;
    char *field_130;
    uint32_t currentAnimToken;
    union {
        uint32_t selectedMotionToken;
        int32_t dword13C;
    };
    uint32_t animStateMode;
    uint32_t moveObjBaseYaw;
    Vector3 runningCharaPos;
    uint32_t field_154;
    CycleMoveObjPhase moveObjPhase;
    uint32_t time;
    CycleMoveObjCommand *moveObjRef;
    uint32_t moveObjSubcommandCount;
    uint32_t moveObjSubcommandIndex;
    float dist;
    float range;
    int64_t gap178;
    char *branchResumeCommandPtr;
    float steeringHalfWidth;
    float steeringHalfDepth;
    float moveObjSpeedParam;
    float approachStartSpeed;
    BikeSteeringState bikeSteeringState;
    uint8_t proximityGroup;
    uint8_t gap19A;
    uint8_t motnSteeringActive;
    uint8_t doorAnimActive;
    uint8_t xmptResumePending;
    CycleOpcode tempState41SavedOpcode;
    uint32_t cycleObjectId;
    int32_t primarySoundBankId;
    int32_t secondarySoundBankId;
    uint32_t flags;
    uint32_t previousFlags;
    uint32_t field_1B4;
    Vector3 steeringTarget;
    int16_t steeringTargetYaw;
    uint16_t tmpReqAnimId;
    uint32_t steeringBlockedFrames;
    uint32_t reversePathFrames;
    CycleSteeringState steeringState;
    CycleSteeringCommand steeringCommand;
    uint8_t gap1D2[2];
    uint32_t reversePath;
    int32_t pathTurnRequested;
    uint8_t facingConstraintFlag;
    uint8_t animHandleActive;
    int8_t create_chara_name[14];
    Vector3 lastMoveObjPos;
    int16_t lastMoveObjYaw;
    uint16_t lastMoveObjAnimId;
    Vector3 xmptResumePos;
    uint8_t resumePositionCapturePending;
    int8_t gap209;
    uint8_t snapToPathStart;
    uint8_t xformDirty;
    int32_t gap20C;
    uint32_t forcedMotionToken;
    uint8_t gap214[4];
    CycleManager::MobjCycle *selfCyclePtr;
    uint32_t cycleObjectIdCopy;
    uint16_t bikeClearanceTimer;
    int8_t map_probe_cooldown;
    int8_t ryoBlockingCooldownFrames;
    int8_t bikeClearanceBlocked;
    uint8_t steeringProfile;
    char faceNearActive;
    char prevMotnSteeringActive;
    int32_t gap22C;
    FaceWork *pFace;
    uint8_t b_paused;
    int8_t moveObjPathActive;
    uint8_t gap23A[6];
    char *moveObjSubcommandPtr;
    Vector3 moveObjAnchorPos;
    float savedSteeringHalfDepth;
};

struct MobjEntry {
    CycleTimetableRow *currentScheduleEntry;
    CycleManager::MobjCycle *ctx;
    CycleManager::MobjEntry *prev_or_backlink;
    CycleManager::MobjEntry *nextDormantProgram;
};
} // namespace CycleManager


static_assert(sizeof(CycleManager::MobjCycle) == 0x258);
static_assert(offsetof(CycleManager::MobjCycle, currentOpcode) == 0x08);
static_assert(offsetof(CycleManager::MobjCycle, pos) == 0x28);
static_assert(offsetof(CycleManager::MobjCycle, pathPoints) == 0x60);
static_assert(offsetof(CycleManager::MobjCycle, obj) == 0x78);
static_assert(offsetof(CycleManager::MobjCycle, mot) == 0x80);
static_assert(offsetof(CycleManager::MobjCycle, trackedMobj) == 0x98);
static_assert(offsetof(CycleManager::MobjCycle, taggedPayloadList) == 0xc0);
static_assert(offsetof(CycleManager::MobjCycle, moveObjRuntime) == 0xc8);
static_assert(offsetof(CycleManager::MobjCycle, entryStateCommandOffset) == 0xec);
static_assert(offsetof(CycleManager::MobjCycle, morsCommandArg) == 0x108);
static_assert(offsetof(CycleManager::MobjCycle, cameraSortDistanceSq) == 0xd8);
static_assert(offsetof(CycleManager::MobjCycle, runningCharaPos) == 0x148);
static_assert(offsetof(CycleManager::MobjCycle, moveObjPhase) == 0x158);
static_assert(offsetof(CycleManager::MobjCycle, steeringHalfWidth) == 0x188);
static_assert(offsetof(CycleManager::MobjCycle, motnSteeringActive) == 0x19b);
static_assert(offsetof(CycleManager::MobjCycle, flags) == 0x1ac);
static_assert(offsetof(CycleManager::MobjCycle, previousFlags) == 0x1b0);
static_assert(offsetof(CycleManager::MobjCycle, steeringTargetYaw) == 0x1c4);
static_assert(offsetof(CycleManager::MobjCycle, lastMoveObjAnimId) == 0x1fa);
static_assert(offsetof(CycleManager::MobjCycle, steeringState) == 0x1d0);
static_assert(offsetof(CycleManager::MobjCycle, create_chara_name) == 0x1de);
static_assert(offsetof(CycleManager::MobjCycle, xmptResumePos) == 0x1fc);
static_assert(offsetof(CycleManager::MobjCycle, selfCyclePtr) == 0x218);
static_assert(offsetof(CycleManager::MobjCycle, pFace) == 0x230);
static_assert(offsetof(CycleManager::MobjCycle, moveObjAnchorPos) == 0x248);
static_assert(offsetof(CycleManager::MobjCycle, savedSteeringHalfDepth) == 0x254);
static_assert(sizeof(CycleManager::MobjEntry) == 0x20);
static_assert(offsetof(CycleManager::MobjEntry, ctx) == 0x08);
static_assert(offsetof(CycleManager::MobjEntry, nextDormantProgram) == 0x18);
namespace CycleManager {


struct EnvAudioNode {
    uint8_t gap0[0x8];
    shen_name name2;
    uint8_t gapC[0x4];
    int64_t field_10;
    uint8_t gap18[0x10];
    Vector3 field_28;
    uint8_t gap34[0x5C];
    int64_t qword90;
    CycleManager::EnvAudioNode *field_98;
    float tmp;
    uint32_t dwordA4;
    uint8_t gapA8[0x88];
    uint8_t byte130;
};

struct CycleManFilePtr {
    uint32_t token;
    int32_t index;
};
} // namespace CycleManager


struct HumansEntry {
    int32_t numCharas;
    shen_name name;
};
namespace CycleManager {


struct SimCycle {
    uint8_t gap0[0x78];
    CHAR_t *obj;
    uint8_t gap80[0x100];
    float steeringHalfWidth;
    float steeringHalfDepth;
    uint8_t gap188[0x13];
    uint8_t motnSteeringActive;
};

struct SimNode {
    void *currentScheduleEntry;
    CycleManager::SimCycle *ctx;
    CycleManager::SimNode *prev_or_backlink;
    CycleManager::SimNode *nextDormantProgram;
};
} // namespace CycleManager

#pragma pack(pop)

} // namespace shenmue::s1

#pragma once

// Cycle owns only its script/simulation data.  Integration points are explicit
// bindings to the current S1 owners; no renderer or aggregate runtime state is
// reachable from this boundary.
#include "s1/task/char.hpp"
#include "s1/date_time.hpp"
#include "s1/task/dswk.hpp"
#include "s1/hlib.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/collision.hpp"
#include "s1/task/effe.hpp"
#include "s1/archive_data.hpp"
#include "s1/archive_handles.hpp"
#include "s1/file_state.hpp"
#include "s1/task/pack.hpp"
#include "s1/motion.hpp"
#include "s1/task/scen.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <deque>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include <limits>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace shenmue::s1 {
extern std::array<std::uint8_t, 128> g_shenmueFlags;
inline uint32_t Token(const char *text) noexcept {
    uint32_t value = 0;
    if (text)
        std::memcpy(&value, text, sizeof(value));
    return value;
}

inline std::string TokenString(uint32_t token) {
    char text[5]{};
    std::memcpy(text, &token, sizeof(token));
    return std::string(text);
}

template <typename T> inline bool ReadValue(const std::vector<uint8_t> &bytes, size_t offset, T &out) {
    if (offset > bytes.size() || sizeof(T) > bytes.size() - offset)
        return false;
    std::memcpy(&out, bytes.data() + offset, sizeof(T));
    return true;
}
namespace CycleManager {


} // namespace CycleManager



struct CycleScheduleEntry;
namespace CycleManager {

struct NpcRecord;
int16_t AngleDegreesToFixed16(float degrees);
int16_t Atan2Fixed16(float x, float z);
uint8_t *MoveObj_FindEntry(CycleScheduleEntry *entries, uint32_t requestCycle, uint64_t unused, int32_t size);
} // namespace CycleManager


inline bool SCN_LooksLikeFourCcToken(uint32_t value) noexcept {
    const uint8_t a = static_cast<uint8_t>(value);
    const uint8_t b = static_cast<uint8_t>(value >> 8U);
    const uint8_t c = static_cast<uint8_t>(value >> 16U);
    const uint8_t d = static_cast<uint8_t>(value >> 24U);
    const auto alpha = [](uint8_t ch) {
        return (ch >= static_cast<uint8_t>('A') && ch <= static_cast<uint8_t>('Z')) || (ch >= static_cast<uint8_t>('a') && ch <= static_cast<uint8_t>('z'));
    };
    return alpha(a) && alpha(b) && alpha(c) && alpha(d);
}
namespace CycleManager {


using ScenarioOwner = void;
bool IsReasonablePosition(const Vector3 &position);
bool NormalizeVectorInPlace(Vector3 &value);
float DistanceSquaredXZ(const Vector3 &lhs, const Vector3 &rhs);
float Distance(const Vector3 &lhs, const Vector3 &rhs);
} // namespace CycleManager


struct HMDL_t;
struct BikeMoveObjectToken;
struct ScnfRecordHeader;

enum CycleProgramList : uint8_t {
    CycleProgramList_Dormant = 0,
    CycleProgramList_Near,
    CycleProgramList_SimSet,
    CycleProgramList_Promote,
    CycleProgramList_Mid,
    CycleProgramList_Load,
    CycleProgramList_Far,
    CycleProgramList_Running,
};

enum BikeSteeringState : uint8_t {
    BikeSteering_Normal = 0x0,
    BikeSteering_FrontBlocked = 0x1,
    BikeSteering_SideAvoidMoving = 0x2,
    BikeSteering_SideBlockedStop = 0x3,
    BikeSteering_SlowForTurn = 0x4,
    BikeSteering_PathOnly = 0x5,
    BikeSteering_ReachedEnd = 0x6,
    BikeSteering_SharpTurn = 0x7,
    BikeSteering_Crawl = 0x8,
    BikeSteering_ProbeActive = 0x9,
};


enum BikeWaypointAdvanceResult : uint32_t {
    BikeWaypoint_ReachedStart = 0x1,
    BikeWaypoint_ReachedEnd = 0x2,
    BikeWaypoint_Continuing = 0x3,
};

/* Native move-object constants selected by GetBikeTOKEN.  They are runtime
 * behavior data, not asset-derived MOTN state, and are shared by all three
 * vehicle path opcodes. */
struct BikeAnimTokenSet {
    int16_t anim[6];
};

struct BikeMoveObjectToken {
    Vector3 pos;
    int16_t steering_clamp;
    int16_t wheel_pitch;
    Vector3 left_ankle_local;
    Vector3 right_ankle_local;
    Vector3 wheel_pivot_local;
    float mount_target_x;
    float mount_target_z;
    int16_t mount_yaw_delta;
    int16_t runtime_c0_value;
    const BikeAnimTokenSet *anims;
    float steering_half_width;
    float steering_half_depth;
    uint8_t speed_curve_mode;
    uint8_t pad[3];
    uint32_t reserved54;
};

struct BikeSpeedProfile {
    float presetA;
    float presetB;
    float presetC;
    float presetD;
    float steeringReferenceSpeed;
    uint32_t reserved0;
};

#pragma pack(push, 8)
struct CycleMoveObjectRuntime {
    uint32_t ownerObjectId;
    uint32_t moveObjectId;
    uint32_t areaId;
    uint32_t pad0C;
    HMDL_t *model;
    void *shadowOrAuxModel;
    uint64_t unk20;
    Vector3 pos;
    uint8_t unk34[12];
    Vector3 tokenLocalOffset;
    int32_t slopeAngleX;
    int32_t yaw;
    int32_t animLeanAngle;
    Vector3i renderEuler;
    int32_t steeringAngle;
    int32_t bikeBankAngle;
    int32_t wheelSpin;
    int32_t yawVelocity;
    int32_t steeringStep;
    uint8_t unk78[20];
    struct CycleMoveObjectRuntime *visibleNext;
    struct CycleMoveObjectRuntime *next;
    float renderOpacity;
    uint32_t moveObjectKind;
    int16_t activeAnim;
    int16_t unkAA;
    int16_t waitAnim;
    uint8_t unkAE[2];
    uint32_t token;
    uint32_t unkB4;
    void *auxModel;
    float runtimeC0;
    uint32_t unkC4;
    uint64_t yawRuntime;
    int32_t animOutput;
    Vector3 modelAttachOffset;
    uint32_t activeFlag;
    uint32_t unkE4;
    void *overrideTransform;
    CharacterShadowState shadowState;
    float pathStartX;
    float pathStartZ;
    int32_t pathCurveDelta;
    int16_t pathCurveBase;
    int16_t unk11E;
    int32_t pathCurveStep;
    int32_t pathYaw;
    int32_t leanHistory[2];
    uint8_t visibilityState;
    uint8_t clearanceCooldown;
    uint8_t pad132[6];
};
static_assert(sizeof(CycleMoveObjectRuntime) == 0x138);
static_assert(offsetof(CycleMoveObjectRuntime, activeFlag) == 0xe0);
static_assert(offsetof(CycleMoveObjectRuntime, shadowState) == 0xf0);
static_assert(offsetof(CycleMoveObjectRuntime, visibilityState) == 0x130);
#pragma pack(pop)



struct BikeSideVolumeQuery {
    const Vector3 *origin;
    const Vector3 *localOffset;
    int16_t yaw;
    Vector3 extents;
};
static_assert(sizeof(BikeSideVolumeQuery) == 0x20);
static_assert(offsetof(BikeSideVolumeQuery, extents) == 0x14);
namespace CycleManager {


struct IntersectionEntry {
    uint32_t operand = 0;
    uint32_t offset = 0;
    uint32_t dwordC = 0;
    uint32_t dword10 = 0;
    uint32_t dword14 = 0;
    uint32_t dword18 = 0;
    uint32_t dword1C = 0;
};
static_assert(sizeof(IntersectionEntry) == 28);
inline constexpr std::size_t kDynamicTurnVolumeCapacity = 44;
inline constexpr std::size_t kIntersectionVolumeCapacity = 20;
#if SHENMUE_IS_PC
extern IntersectionEntry *g_cycleSteeringVolumes;
extern IntersectionEntry *g_pIntersectionPtr;
bool InitMemory();
#endif
} // namespace CycleManager



struct SteeringShapeXZ {
    Vector3 center;
    int16_t yaw;
    uint16_t reserved0e;
    float radius;
    Vector3 quad_corners[4];
    uint8_t push_resistance;
    uint8_t reserved45[3];
};
static_assert(sizeof(SteeringShapeXZ) == 0x48);
static_assert(offsetof(SteeringShapeXZ, radius) == 0x10);
static_assert(offsetof(SteeringShapeXZ, quad_corners) == 0x14);
static_assert(offsetof(SteeringShapeXZ, push_resistance) == 0x44);


struct OrientedSteeringQuadParams {
    const Vector3 *center;
    int32_t yaw;
    float half_width;
    float half_depth;
    float yaw_offset;
    Vector3 *out_corners;
};
static_assert(sizeof(OrientedSteeringQuadParams) == 0x20);




struct ObjectProjectionData {
    Vector3 center;
    int16_t facingAngle16;
    int16_t reserved0e;
    Vector3 forward;
    Vector3 heading;
    float projHeight;
    Vector3 halfBoxExtent;
    float angYoffset;
    Vector3 corners[4];
};
static_assert(sizeof(ObjectProjectionData) == 0x6C);
static_assert(offsetof(ObjectProjectionData, forward) == 0x10);
static_assert(offsetof(ObjectProjectionData, heading) == 0x1C);
static_assert(offsetof(ObjectProjectionData, projHeight) == 0x28);
static_assert(offsetof(ObjectProjectionData, halfBoxExtent) == 0x2C);
static_assert(offsetof(ObjectProjectionData, corners) == 0x3C);

// The CycleManager_AdjustDesiredYawForSteeringCollision frame fields written by ComputeRotatedProjectionBounds:
// the left/right blocking boundary directions and the nearest squared obstacle distance.
struct SteeringProjectionBounds {
    Vector3 directions[4];
    float minDistanceSquared;
};
static_assert(sizeof(SteeringProjectionBounds) == 0x34);

// One g_pMOBJ entry: a CEWP collision object and its CCOW world-space shape, collected once per cycle tick.
struct MobjCollisionObject {
    Character *character;
    const QueryShape *shape;
};
static_assert(sizeof(MobjCollisionObject) == 0x10);

enum timeContextType_t : int32_t {
    NORMAL = 0,
    WEEKEND = 1,
    XMAS = 2,
    NEW_YEAR = 3,
    XMAS_WEEKEND = 4,
};

#pragma pack(push, 1)
struct TimeContext {
    std::uint8_t year;
    std::uint8_t month;
    std::uint8_t day;
    std::uint8_t dayInWeek;
    std::uint8_t hours;
    std::uint8_t minutes;
    std::uint8_t seconds;
};
#pragma pack(pop)

static_assert(sizeof(TimeContext) == 7);

timeContextType_t ConstructTimeContext(TimeContext *output);



struct ConditionRow {
    uint16_t setFlagA;
    uint16_t clearFlagA;
    uint16_t setFlagB;
    uint16_t clearFlagB;
    uint8_t startMonth;
    uint8_t startDay;
    uint8_t endMonth;
    uint8_t endDay;
    int16_t dayType;
    int16_t timetable;
};
static_assert(sizeof(ConditionRow) == 0x10);
namespace CycleManager {


struct Section {
    uint32_t token = 0;
    uint32_t offset = 0;
    uint32_t size = 0;
};
} // namespace CycleManager


struct CycleScheduleEntry {
    int32_t time = 0;
    int32_t offset = 0;
};

struct struct_v16_2 {
    uint32_t charaCnt = 0;
    uint32_t offset = 0;
};

struct struct_InCharaCycleFileDataPtr {
    uint32_t offset = 0;
    shen_name chara_id{};
};
namespace CycleManager {


struct ProgramScan {
    shen_name chara_id{};
    uint32_t headerOffset = 0;
    uint32_t timeTableOffset = 0;
    uint32_t timeTableEntries = 0;
    uint32_t commandStreams = 0;
    uint32_t opcodeCount = 0;
    uint32_t truncatedStreams = 0;
    std::vector<uint32_t> commandStreamOffsets;
};
} // namespace CycleManager


static constexpr uint32_t kMscrPackedMotionParamOff = 0x3C;
static constexpr uint32_t kMscrPackedMotionParamCount = 4;
static constexpr uint32_t kMscrMotionAnimParamTableOff = 0x7C;
static constexpr uint32_t kMscrBranchTableOff = 0xBC;
static constexpr uint32_t kMscrMotionAnimParamTableCount = (kMscrBranchTableOff - kMscrMotionAnimParamTableOff) / 2;
namespace CycleManager {


struct ShutRow {
    shen_name token{};
    shen_name modelToken{};
    Vector3 pos{};
    Vector3 targetPos{};
    float angle1Degrees = 0.0f;
    float angle2Degrees = 0.0f;
    float float20 = 0.0f;
    float dword20AsFloat = 0.0f;
    float yStep = 0.0f;
    float minY = 0.0f;
    float maxY = 0.0f;
    uint32_t dword34 = 0;
    uint64_t qword38 = 0;
    uint32_t dword40 = 0;
    float angle3Degrees = 0.0f;
    int16_t angle1 = 0;
    int16_t angle2 = 0;
    int16_t angle3 = 0;
    uint32_t cycleIndex = 0;
    uint32_t sourceOffset = 0;
    int32_t lastDispatchTime = 0;
    uint32_t lastDispatchState = 0;
    uint8_t byte31 = 0;
    int32_t objectInstanceIndex = -1;
    int32_t pairedObjectInstanceIndex = -1;
};

struct NpcRecord;

struct MNRBEntry {
    struct Reservation {
        Reservation *prev = nullptr;
        Reservation *next = nullptr;
        CycleManager::NpcRecord *owner = nullptr;
        Vector3 seatPos{};
        int32_t seatOrderKey = 0;
    };

    shen_name name{};
    uint32_t sourceIndexOffset = 0;
    uint32_t lineDataOffset = 0;
    uint32_t entryKind = 0;
    float radius = 0.0f;
    uint32_t ownerTokenCount = 0;
    uint32_t ownerTokenOffset = 0;
    uint32_t areaPointCount = 0;
    uint32_t areaPointOffset = 0;
    uint32_t ownerPointCount = 0;
    uint32_t ownerPointOffset = 0;
    uint32_t activeTimeStart = 0;
    uint32_t activeTimeEnd = 0;
    uint32_t runtimeStatus = 0;
    uint32_t reservationCount = 0;
    Reservation *reservationHead = nullptr;
};
} // namespace CycleManager

static_assert(sizeof(CycleManager::MNRBEntry::Reservation) == 40);
static_assert(offsetof(CycleManager::MNRBEntry::Reservation, seatPos) == 24);
static_assert(offsetof(CycleManager::MNRBEntry::Reservation, seatOrderKey) == 36);



struct SplineAxis4Cache {
    std::array<float, 4> control;
    std::array<float, 4> secondDeriv;
};

struct NaturalCubicSpline3DCache {
    SplineAxis4Cache x;
    SplineAxis4Cache y;
    SplineAxis4Cache z;
    std::array<float, 4> cumulativeArcLen;
};
static_assert(sizeof(NaturalCubicSpline3DCache) == 0x70);


inline constexpr std::array<std::uint8_t, 10> kCycleTaggedPayloadBoneIds{12, 18, 12, 18, 5, 9, 15, 7, 13, 2};

inline constexpr std::size_t kCycleTaggedPayloadPoolSize = 80;
namespace CycleManager {



struct TaggedPayload {
    // The holder's list, or the free list.
    CycleManager::TaggedPayload *next;
    // The g_CM_obj_list link of cmRenderAttachedSubobjects' draw list.
    CycleManager::TaggedPayload *drawNext;
    uint32_t mode;
    uint32_t field_14;
    void *model;
    // The placement in the attachment frame: position x/y/z (floats), then euler x/y/z.
    std::array<uint32_t, 6> payload;
    // The state (low byte: 0 hidden, 1 shown, 2 fading in, 3 fading out) and the fade counter (high byte).
    uint16_t lifetime;
    uint8_t gap3A[6];
    CycleManager::MobjCycle *owner;
    // The begin command (0x10/0x2D): its identifier at word 1, model token at 2, upright flag at 3 and hand pose at 5.
    const uint32_t *command;
    // A fade-out leaves the payload hidden in the list instead of freeing it (the free talk's suspension).
    uint8_t retainAfterFade;
    uint8_t gap51[7];
};
} // namespace CycleManager

static_assert(sizeof(CycleManager::TaggedPayload) == 0x58);
static_assert(offsetof(CycleManager::TaggedPayload, model) == 0x18);
static_assert(offsetof(CycleManager::TaggedPayload, lifetime) == 0x38);
static_assert(offsetof(CycleManager::TaggedPayload, owner) == 0x40);
static_assert(offsetof(CycleManager::TaggedPayload, retainAfterFade) == 0x50);
namespace CycleManager {


CycleManager::MobjCycle DefaultCycle();

struct NpcRecord {

    CycleManager::MobjCycle *cycle = nullptr;
    int16_t yaw = 0;
    uint16_t vehiclePathAnimId = 0;
    uint32_t auxiliaryAnimId = 0;
    uint32_t auxiliaryAnimFlag = 0;
    bool activeAnimRootYawPending = false;
    uint32_t headerOffset = 0;
    uint32_t moveConfigOffset = 0;


    uint32_t morsLinkedOwnerObjectId = 0;




    CycleMoveObjectRuntime *releasedMoveObjRuntime = nullptr;

    CycleManager::MobjEntry *entry = nullptr;
    uint32_t steeringCollisionResult = 0;


    int32_t selfCycleIndex = -1;

    bool skipBlend = false;
    uint32_t talkMoveAvoidanceFrames = 0;
    uint32_t vehicleToken = 0;
    Vector3 vehicleCommandPos{};
    int16_t vehicleCommandYaw = 0;
    bool vehicleCommandEnabled = false;
    const BikeMoveObjectToken *bikeToken = nullptr;

    int32_t driveWheelSteering = 0;

    Vector3 umbrellaReleaseAnklePos{};


    uint32_t morsSoundGateMask = 0;
    uint32_t field148 = 0;
    int32_t motnCycleYawDelta = 0;
    float motnCycleBlend = 0.5f;
    uint32_t motnCycleState = 0;
    float morsMotionCursor = 0.0f;




    uint32_t stairPackedAnimIds = 0;
    int32_t stairGuideEntryId = -1;
    float stairProbeTravel = 0.0f;
    float stairCommandWork = 0.0f;
    uint8_t stairStage = 0;







    bool visible = true;
    CycleProgramList programList = CycleProgramList_Dormant;
    uint32_t lastLocalAnimAdvanceEpoch = 0xFFFFFFFFu;
    uint16_t localAnimFrameStallFrames = 0;
};

struct RuntimeState {
    // File/archive payload owned by the cycle manager, never by a renderer.
    std::vector<uint8_t> bytes;
    // The offset fields CycleManager_RelinkCycleProgram has relinked since the file was loaded, by file offset.
    std::vector<bool> relinkedOffsetFields;
    std::vector<CycleManager::Section> sections;
    std::vector<CycleScheduleEntry> schedule;
    std::vector<CycleManager::ProgramScan> programs;
    std::vector<CycleManager::NpcRecord> npcRecords;
    std::vector<CycleManager::ShutRow> shutRows;
    std::vector<CycleManager::MNRBEntry> mnrbEntries;
    std::array<CycleManager::MNRBEntry::Reservation, 144> mnrbReservationPool{};
    bool mnrbReservationPoolAvailable = false;
    std::vector<uint32_t> shopShutterCommandOffsets;
    std::set<uint32_t> shopShutterCommandOffsetSet;
    std::map<uint32_t, uint32_t> opcodeCounts;
    std::string sourcePath;
    shen_name shutAreaID{};
    uint32_t currentSectionToken = 0;
    uint32_t currentSectionOffset = 0;
    uint32_t currentSectionSize = 0;
    uint32_t mscrOffset = 0;
    uint32_t mnrbOffset = 0;
    uint32_t mcirOffset = 0;
    uint32_t mscrCharaCount = 0;
    uint32_t totalTimeTableEntries = 0;
    uint32_t totalCommandStreams = 0;
    uint32_t totalOpcodeCount = 0;
    uint32_t truncatedCommandStreams = 0;
    uint32_t shutDispatches = 0;
    uint32_t shutDispatchMissing = 0;
    uint32_t shutDispatchStateChanges = 0;
    uint32_t shutReplayStreams = 0;
    uint32_t shutReplayCommands = 0;
    uint32_t shutReplayWaitStops = 0;
    uint32_t taggedWaitBegins = 0;
    uint32_t taggedWaitCompletions = 0;
    uint32_t taggedWaitTimeouts = 0;
    uint32_t taggedWaitTargetMisses = 0;
    uint32_t talkMoveBegins = 0;
    uint32_t talkMoveCompletions = 0;
    uint32_t talkMoveTargetMisses = 0;
    uint32_t originalProgramCreateCharacterCommands = 0;
    uint32_t originalProgramCreateCharacterLoads = 0;
    uint32_t originalProgramCreateCharacterFailures = 0;
    uint32_t motnSpeedLookups = 0;
    uint32_t motnSpeedLookupHits = 0;
    uint32_t motnRootChannelHits = 0;
    uint32_t motnRootChannelFallbacks = 0;
    uint32_t motnRootLoopBoundarySteps = 0;
    uint32_t doorPseudoRecords = 0;
    uint32_t doorRequestAllocations = 0;
    uint32_t doorRequestPendingPolls = 0;
    uint32_t doorRequestActivePolls = 0;
    uint32_t doorRequestDonePolls = 0;
    uint32_t doorRequestFailedPolls = 0;
    uint32_t doorRequestNotReadyPolls = 0;
    uint32_t doorRequestAllocFailures = 0;
#if SHENMUE_IS_PC
    std::span<CycleManager::IntersectionEntry> dynamicTurnCollisionVolumes{g_cycleSteeringVolumes, g_cycleSteeringVolumes ? kDynamicTurnVolumeCapacity : 0};
    std::span<CycleManager::IntersectionEntry> intersectionEntries{g_pIntersectionPtr, g_pIntersectionPtr ? kIntersectionVolumeCapacity : 0};
#else
    std::array<CycleManager::IntersectionEntry, 44> dynamicTurnCollisionVolumes{};
    std::array<CycleManager::IntersectionEntry, 20> intersectionEntries{};
#endif
    uint32_t mobjFrames = 0;
    uint32_t mobjLastActive = 0;
    uint64_t mobjAdvancedTotal = 0;
    uint64_t mobjPathStartedTotal = 0;
    uint64_t mobjPathTickedTotal = 0;
    uint64_t mobjPathCompletedTotal = 0;
    uint64_t mobjNpcAnimAdvances = 0;
    uint64_t mobjNpcAnimDuplicateSyncs = 0;
    uint64_t moveObjSitBegins = 0;
    uint64_t moveObjSitAnchors = 0;
    uint64_t moveObjStandBegins = 0;
    uint64_t moveObjResumePathStarts = 0;
    uint64_t moveObjSitCompletions = 0;
    uint64_t steeringChecks = 0;
    uint64_t steeringAdjustments = 0;
    uint64_t steeringYawChecks = 0;
    uint64_t steeringYawAdjustments = 0;
    uint64_t steeringYawActorAvoidance = 0;
    uint64_t steeringYawRyoAvoidance = 0;
    uint64_t steeringYawVolumeAvoidance = 0;
    uint64_t steeringYawMapAvoidance = 0;
    uint64_t steeringActorClass0To3Hits = 0;
    uint64_t steeringActorClass4To7Hits = 0;
    uint64_t steeringMoveObjectActorHits = 0;
    uint64_t steeringRyoHits = 0;
    uint64_t steeringVolumeHits = 0;
    uint64_t steeringMotnClampAdjustments = 0;
    uint64_t steeringMapFallbackHits = 0;
    uint32_t npcAnimSyncEpoch = 0;

    bool umbrellaRainActive = false;
    uint32_t updateFrames = 0;
    uint32_t cycleLoadSettleFrameCounter = 0;
    bool loaded = false;
    bool localNpcCharactersCreated = false;

    // Explicit bindings to current S1 owners.  These are populated by map/scenario
    // setup; cycle code must not reach through a process-wide aggregate state.
    bool ready = false;
    bool collisionReady = false;
    bool inGame = false;
    bool mainMenu = false;
    bool disableWeather = false;
    bool skyEnabled = true;
    std::string skyRoot;


    std::array<std::uint32_t, 9> skyGeometryRuntimeData{};
    std::array<std::uint32_t, 9> skyProbeGeometryRuntimeData{};
    Vector3 skyDirection{0.0f, -1.0f, 0.0f};
    void *ctrl = nullptr;
    void *doorTaskData = nullptr;
    CycleManager::ScenarioOwner *scenarioVmStack = nullptr;
    shen_name areaId{};
    GameDateTime currentDateTime{};
    bool isWithinDobuita = false;
    int32_t authorState = 0;
    float groundY = 0.0f;
    bool groundValid = false;
    uint32_t staticCommandObjects = 0;
    uint32_t staticCommandObjectsUnresolved = 0;
    Vector3 center{};
    uint32_t motnPoseCacheHits = 0;
    uint32_t motnPoseCacheMisses = 0;
    uint32_t motnPositionSampleHits = 0;
    uint32_t motnPositionSampleMisses = 0;
};

using ReadFileFn = bool (*)(std::string_view path, std::vector<uint8_t> &bytes);
using FileExistsFn = bool (*)(std::string_view path);
using ObjectNodeRotationFn = void (*)(void *object, uint32_t nodeId, int32_t rotationY);
using EnqueueTaskFn = HLib::Task *(*)(HLib::TaskCallback callback, int32_t priority, int32_t flags, uint32_t name, const char *label);
using UmbrellaAnimIdFn = uint16_t (*)();
} // namespace CycleManager

using HLibTaskData = HLib::TaskData;

extern CycleManager::RuntimeState g_cycleManagerRuntime;
extern std::int32_t g_MOBJ_num_entries;
extern CycleManager::MobjEntry *g_MOBJ_pData;
extern CycleManager::MobjCycle *g_MOBJ_Cycles;
extern char g_tmp_cm_path[32];
extern std::int32_t g_MOBJ_needs_new_update;
extern std::int32_t g_bDebugSkipCyclesForCharacters;
extern std::int32_t g_bProcessCharaCycles;
extern std::int32_t gCycleManager;
extern std::int32_t g_cm_unk_flag;
extern std::int32_t g_dbg_flags;
extern std::int32_t MaxNearNPCs;
extern std::int32_t numCharactersSimulated;
extern std::int32_t g_midQueueCycleCount;
extern std::int32_t g_farQueueCycleCount;
extern std::int32_t g_nearQueueCycleCount;
extern std::int32_t g_promoteListCycleCount;
extern std::int32_t g_perFrameNpcWorkCnt;
extern const std::byte *g_pCycleManagerTimeFlag;
extern std::uint64_t g_cycleShopExitPositionXY;
extern std::int32_t g_cycleShopExitPositionZ;
extern std::int32_t g_cycleShopExitYaw;
extern std::int32_t g_cycleShopExitPublished;
extern std::uint16_t g_steeringIntersectionVolumeCount;
bool CheckVectorIntersectionWithMaxAngleAndIterate(const Vector3 &start, const Vector3 &end);
std::int32_t checkCharacterDistanceFromCoords(std::uint32_t charaId, float distance);
extern float g_default_turning_box_half_depth_z;
extern float g_angYoffset;
extern PackOwner *HUMANS_AFS_Ptr;
extern HumansEntry *HumansIndexTable;
extern PackSlot *g_pszTempLoadedHumanSched;
extern std::int32_t g_bHasLoadedHuman;
extern std::uint32_t g_cycleScriptHumanChara;
extern std::uint8_t g_tmp_new_day;
extern std::uint32_t g_prevCycleSeconds;
extern std::uint16_t g_dynamicTurnCollisionVolumeCount;
// A SimSet cycle is being held for the running conversation.
extern std::int32_t g_cycleConversationHoldActive;
// An SCNT-referenced SimSet cycle was held; the next pass leaves the near queue alone.
extern std::int32_t g_scntReferencedCycleHeld;
// The MFSY talk-walk drives Ryo through g_cyclTalkCycle.
extern std::int32_t g_cyclTalkCycleActive;
// Blocks cycle talk (cmDoForThisEntry and the talk-walk begin); only the CTRL enqueue writes it (0).
extern std::int32_t g_cycleTalkSuppressed;

extern PackSlot *gRainModelPtr;
extern std::int32_t nRainState;
// The RAIN, RAI2, RAI1 and RAI3 models Weather_GetRainState collects for the umbrella holders.
extern std::array<void *, 4> g_rainStateModels;


extern std::int32_t g_bGlobalFlag;

extern float g_fGlobalTurningRadius;
extern CycleManager::ReadFileFn g_cycleManagerReadFile;
extern CycleManager::FileExistsFn g_cycleManagerFileExists;
extern CycleManager::ObjectNodeRotationFn g_cycleManagerObjectNodeRotation;
extern CycleManager::EnqueueTaskFn g_cycleManagerEnqueueTask;
extern CycleManager::UmbrellaAnimIdFn g_cycleManagerUmbrellaAnimId;
namespace CycleManager {

inline uint16_t UmbrellaAnimationId() noexcept {
    return g_cycleManagerUmbrellaAnimId ? g_cycleManagerUmbrellaAnimId() : 0x060bu;
}
using FindCharacterFn = Character *(*)(uint32_t token);
} // namespace CycleManager

extern CycleManager::FindCharacterFn g_cycleManagerFindCharacter;
namespace CycleManager {

Character *DefaultFindCharacter(uint32_t token);
inline Character *FindCharacter(uint32_t token) {
    return g_cycleManagerFindCharacter ? g_cycleManagerFindCharacter(token) : CycleManager::DefaultFindCharacter(token);
}
} // namespace CycleManager


inline float Angle16ToRadians(int32_t angle) noexcept {
    return static_cast<float>(angle) * 6.28318530717958647692f / 65536.0f;
}
inline int16_t RadiansToAngle16(float radians) noexcept {
    return static_cast<int16_t>(static_cast<int32_t>(std::lround(radians * 65536.0f / 6.28318530717958647692f)) & 0xFFFF);
}
inline int16_t Angle16Delta(uint16_t target, uint16_t current) noexcept {
    return static_cast<int16_t>(static_cast<uint16_t>(target - current));
}
inline Vector3 RotateVectorYaw16(const Vector3 &value, int32_t yaw) noexcept {
    const float radians = Angle16ToRadians(static_cast<int16_t>(yaw & 0xFFFF));
    const float sine = std::sin(radians);
    const float cosine = std::cos(radians);
    return Vector3{value.x * cosine - value.z * sine, value.y, value.x * sine + value.z * cosine};
}
inline float SCN_FloatFromU32(uint32_t value) noexcept {
    float out = 0.0f;
    std::memcpy(&out, &value, sizeof(out));
    return out;
}
inline uint32_t SCN_U32FromFloat(float value) noexcept {
    uint32_t out = 0;
    std::memcpy(&out, &value, sizeof(out));
    return out;
}
inline bool SCN_IsStackLValue(uint32_t) noexcept {
    return false;
}
namespace CycleManager {


uint32_t CommandRecordSize(uint32_t opcode, bool &stop, uint32_t recordOffset = UINT32_MAX);
bool ReadCommandPositionYaw(uint32_t recordOffset, Vector3 &pos, int16_t &yaw);
uint32_t CurrentTimeSeconds();
bool ApplyNpcCommandState(CycleManager::NpcRecord &npc, uint32_t opcode, uint32_t recordOffset, bool *positionChanged = nullptr);
void ApplyNpcRecordToLocal(CycleManager::NpcRecord &npc);
std::uint32_t GetRelativePositionToCycles(Character *point);

void MoveObj_FileLoad(const char *InFilename);
int32_t ReadCyclemanForScene(const char *filename);
bool DispatchSceneTask(uint32_t token, uint32_t dispatchState, uint32_t currentTime);
} // namespace CycleManager

void MOBJ_UpdateTransformsRecursively_0();
namespace CycleManager {


uint32_t ResolveOffsetFromChara(uint32_t charaHeaderOffset, int32_t offset);
bool SelectCurrentSection(uint32_t token);
} // namespace CycleManager

int32_t Humans_n_GetIndexForCharacter(uint32_t charaName);
namespace CycleManager {

PackSlot *SetCyclePtrByCharaID(uint32_t chara);
CycleManager::NpcRecord *FindMutableNpcRecord(std::uint32_t token);
bool DoesCycleEntryHaveFlagsSet(std::uint32_t token);
bool GetCycleFlagForSpecificChara(std::uint32_t token);
CycleManager::NpcRecord *NextProgramInList(CycleProgramList list, const CycleManager::NpcRecord *after = nullptr);
CycleManager::MobjEntry *&ProgramListHead(CycleProgramList list);
extern MobjEntry *SimSetCursor;
CycleManager::NpcRecord *EntryRecord(CycleManager::MobjEntry *entry);
char *FilePointer(uint32_t offset);
uint32_t FileOffset(const void *pointer);
CycleManager::NpcRecord *CycleRecord(CycleManager::MobjCycle *ctx);
uint32_t ExecuteProgramStep(CycleManager::MobjCycle *ctx, int32_t time, int32_t *inOutTime);
void ParseForScript(std::uint32_t programName, bool enabled);
PackSlot *ParseForScript_InternalForHuman(std::uint32_t programName);
std::uint32_t GetNextTaskPointer(PackSlot *slot);
void ParseProgramForChara(std::uint32_t charaToken, std::uint32_t timetableIndex);
void LinkCyclesInStack(int32_t enabled);
void ProcessSNDForCycles();
void ProcessSNDForCyclesExtra();
void ProcessCycle(bool processType);
void ProcessCycleInternal();
void Update(CycleManager::MobjEntry *currEntry);
void UpdateCurrentCursor(CycleManager::MobjCycle *cycle);
void ZeroFields();
std::size_t CollectAuxiliaryCollisionShapes(AuxiliaryCollisionEntry *outShapes, std::size_t capacity, std::uint32_t eventGroup);
std::array<Vector3, 4> BuildForkliftSteeringQuad(const Vector3 &center, std::int16_t yaw);

float GetBikeSteeringReferenceSpeed(const CycleMoveObjectRuntime *runtime);
uint16_t SelectRandomMotionToken(CycleManager::NpcRecord &npc);
} // namespace CycleManager

float MOBJ_MovementDistanceStepFromMOTN(CycleManager::NpcRecord &npc, float scalarStep);
namespace CycleManager {

uint16_t MotionParamWord(const CycleManager::NpcRecord &npc, uint32_t wordIndex);
void RecomputeSteeringExtentsForNpc(CycleManager::NpcRecord &npc);
void UpdateSndStreaming(CycleManager::NpcRecord &npc);
void SyncVehicleRuntimeFromNpc(CycleManager::NpcRecord &npc);
void ReleaseVehicleRuntime(CycleManager::NpcRecord &npc);
bool BeginBikeMoveObjectRelease(CycleManager::NpcRecord &npc);
bool BeginVehicleActiveState(CycleManager::NpcRecord &npc, bool live);
bool TickVehicleActiveState(CycleManager::NpcRecord &npc);
void StartDoorFacingHandoff(CycleManager::NpcRecord &npc);
} // namespace CycleManager

void PlayAnim_Wrapper_2(CycleManager::NpcRecord &npc, bool steeringActive);
void PlayAnim_Wrapper_0(CycleManager::NpcRecord &npc, uint32_t recordOffset);
void motAssignCallbackReactions(CycleManager::NpcRecord &npc);
bool npcUpdateMovementPath(CycleManager::NpcRecord &npc);
namespace CycleManager {

bool InitTimedPathMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive);
bool InitPathFollowMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive);
bool InitPathFollowMotion(CycleManager::NpcRecord &npc, uint32_t offset, uint32_t count, bool steeringActive);
bool InitStairPath2D(CycleManager::NpcRecord &npc, uint32_t recordOffset);
bool InitVehiclePath2D(CycleManager::NpcRecord &npc, uint32_t recordOffset, uint32_t opcode);
bool TickPositionAndHeadingMove(CycleManager::NpcRecord &npc);
bool UpdateMoveObjectPathing(CycleManager::NpcRecord &npc, bool moveObjMode);
bool TickPathFollowMotion(CycleManager::NpcRecord &npc, bool steeringActive);
bool AdjustDesiredYawForSteeringCollision(CycleManager::NpcRecord &npc);
} // namespace CycleManager

bool UpdateNpcSteeringFromMOTN(CycleManager::NpcRecord &npc, bool applyPosition);
bool shenResolveSteeringShapeOverlapXZ(SteeringShapeXZ &lhs, SteeringShapeXZ &rhs);
bool cycleCheckIsActorInsideForwardBlockingQuad(CycleManager::NpcRecord &npc, bool skipRyo);


void npcSetMoveFacingMot(CycleManager::NpcRecord &npc, bool enableRootMotion, bool facingConstraint);
void cyclSubmitMoveRequest(CycleManager::NpcRecord &npc);
void cycleRequestAnim(CycleManager::NpcRecord &npc, uint16_t animId);
void cyclFreeTalkConvSetMoveAndFacingConstraint(CycleManager::NpcRecord &npc, uint16_t animId);
void cyclUpdateCharaMoveRequestForAnimChange(CycleManager::NpcRecord &npc);
namespace CycleManager {

void SubmitXMPTMoveWithCycleParams(CycleManager::NpcRecord &npc, Character &character, const Vector3 &target, int16_t yaw, uint16_t animId);
} // namespace CycleManager

void UpdateEntityOverPositionWithFlags(CycleManager::NpcRecord &npc, bool restoreIdle);
void charaClearMotionRequests(Character *character);
bool PlayAnim_Wrapper(Character *character, MotionInformationWork *motion, float startFrame, uint16_t animId, uint16_t nodeId, uint32_t flags);
namespace CycleManager {

void PlayAnim_Wrapper(CycleManager::NpcRecord &npc, uint32_t recordOffset);
void FreeIfDoorOpeningAnim(CycleManager::NpcRecord &npc);
} // namespace CycleManager

bool npcApplyUmbrellaAnim(bool umbrella, CycleManager::NpcRecord &npc);
namespace CycleManager {

void ApplyUmbrellaWalk(CycleManager::NpcRecord &npc);
} // namespace CycleManager

void PlayAnim_Wrapper_1(CycleManager::NpcRecord &npc, bool raining);

int32_t MOTN_getFrameCountFromAnimId(uint16_t animationId);
int32_t motnGetRootMotionEndMarker(uint16_t animationId);
float MOTN_CalculateRootMotionSpeedZ(CycleManager::NpcRecord &npc);
float npcCalculateMovementDistanceStep(const CycleManager::NpcRecord &npc);

void LoadFile(PackSlot *slot);
namespace CycleManager {

bool PollDoorRequest(CycleManager::NpcRecord &npc) noexcept;

inline uint32_t CurrentAreaId() noexcept {
    return g_cycleManagerRuntime.areaId.name ? static_cast<uint32_t>(g_cycleManagerRuntime.areaId.name) : g_areaId;
}
inline shen_name CurrentArea() noexcept {
    return shen_name{CycleManager::CurrentAreaId()};
}
} // namespace CycleManager

void Init_0();
void LoadSkyGeometry(const char *root);
void InitSkyGeometry(std::array<std::uint32_t, 9> &geometry);
void LoadSky(const char *directory);
namespace CycleManager {


inline CycleManager::RuntimeState &Runtime() noexcept {
    return g_cycleManagerRuntime;
}
inline bool IsReady() noexcept {
    return g_cycleManagerRuntime.ready;
}
inline bool HasCollisionOwner() noexcept {
    return g_cycleManagerRuntime.collisionReady;
}
Character *MapRyo();
inline bool HasCharacterOwner() noexcept {
    return CycleManager::MapRyo() != nullptr;
}
bool MnrBEntryAllowsTalk(const CycleManager::NpcRecord &npc);
inline Vector3 CurrentRyoPosition() noexcept {
    Character *const ryo = CycleManager::MapRyo();
    return ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
}
inline bool ReadCurrentFile(std::string_view path, std::vector<uint8_t> &bytes) {
    if (g_cycleManagerReadFile && g_cycleManagerReadFile(path, bytes))
        return true;
    const std::string pathText(path);
    const uint64_t size = ArchiveData_GetFileSize(pathText.c_str());
    if (size == 0 || size > static_cast<uint64_t>(std::numeric_limits<size_t>::max()))
        return false;
    bytes.resize(static_cast<size_t>(size));
    return ArchiveData_Read(pathText.c_str(), 0, bytes.data(), size);
}
inline bool FileExists(std::string_view path) noexcept {
    if (g_cycleManagerFileExists && g_cycleManagerFileExists(path))
        return true;
    const std::string pathText(path);
    return ArchiveData_GetFileSize(pathText.c_str()) != 0;
}
inline HLib::Task *EnqueueTask(HLib::TaskCallback callback, int32_t priority, int32_t flags, uint32_t name, const char *label) {
    if (g_cycleManagerEnqueueTask)
        return g_cycleManagerEnqueueTask(callback, priority, flags, name, label);
    (void)label;
    return HLib_EnqueueTaskWithoutParameter(callback, static_cast<int16_t>(priority), static_cast<uint16_t>(flags), static_cast<int32_t>(name));
}
} // namespace CycleManager


struct MobjSection {
    shen_name token;
    std::uint32_t offset;
    std::uint32_t size;
};

extern HLib::Task *g_mobjCoreTask;
extern HLib::Task *g_mobjMotionTask;
extern std::byte *g_pMOBJ;
extern std::byte *g_pMOBJ_HeapHead;
extern std::byte *g_tmp_MOBJ;
const MobjSection MOBJ_FindSection(std::uint32_t token);


struct ForkliftLoadToken {
    shen_name driver;
    shen_name load;
    std::int8_t probePhase;
};

inline constexpr std::array<ForkliftLoadToken, 18> kForkliftLoadTokens{{
    {.driver = SHEN_NAME(FLDC), .load = SHEN_NAME(NP51), .probePhase = 12},
    {.driver = SHEN_NAME(FLD8), .load = SHEN_NAME(NP32), .probePhase = 10},
    {.driver = SHEN_NAME(FLD9), .load = SHEN_NAME(NP35), .probePhase = 4},
    {.driver = SHEN_NAME(FLDA), .load = SHEN_NAME(NP07), .probePhase = 5},
    {.driver = SHEN_NAME(FLDB), .load = SHEN_NAME(NP47), .probePhase = 11},
    {.driver = SHEN_NAME(FLD6), .load = SHEN_NAME(NP25), .probePhase = 8},
    {.driver = SHEN_NAME(FLD1), .load = SHEN_NAME(NP09), .probePhase = 1},
    {.driver = SHEN_NAME(FLD2), .load = SHEN_NAME(NP15), .probePhase = 2},
    {.driver = SHEN_NAME(FLD4), .load = SHEN_NAME(NP08), .probePhase = 6},
    {.driver = SHEN_NAME(FLD5), .load = SHEN_NAME(NP16), .probePhase = 7},
    {.driver = SHEN_NAME(YOHI), .load = SHEN_NAME(NP04), .probePhase = 5},
    {.driver = SHEN_NAME(FLDD), .load = SHEN_NAME(NP65), .probePhase = 13},
    {.driver = SHEN_NAME(FLDE), .load = SHEN_NAME(NP76), .probePhase = 14},
    {.driver = SHEN_NAME(TAKE), .load = SHEN_NAME(NP01), .probePhase = 2},
    {.driver = SHEN_NAME(FLDG), .load = SHEN_NAME(NP91), .probePhase = 0},
    {.driver = SHEN_NAME(HTSI), .load = SHEN_NAME(NP02), .probePhase = 3},
    {.driver = SHEN_NAME(TYMK), .load = SHEN_NAME(NP03), .probePhase = 4},
    {.driver = SHEN_NAME(MARK), .load = SHEN_NAME(NP06), .probePhase = 1},
}};

// Map region whose value is the 1-based SHUT row of the shop a point stands in.
inline constexpr shen_name REGION_INSH{SHEN_NAME(INSH)};
inline constexpr shen_name CYCLE_CHAIR_AREA_DRME{SHEN_NAME(DRME)};
inline constexpr shen_name CYCLE_CHAIR_AREA_DSLI{SHEN_NAME(DSLI)};
namespace CycleManager {

std::int32_t InitINSHForScene(std::uint32_t areaId, Vector3 &position, std::int32_t &yaw);
} // namespace CycleManager

bool check_if_point_in_quadrilateral(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d, const Vector3 &point);
void InitCycleStack();

struct MoveObjChairAnchorAdjustment {
    std::uint32_t preset;
    std::uint32_t anchorGroup;
    float localX;
    float localZ;
};

inline constexpr std::array kMoveObjChairAnchorAdjustments{
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSBA), 10, 0.0f, 0.083f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSBA), 13, 0.0f, 0.033f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSBA), 12, 0.0f, 0.030f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSUS), 10, 0.0f, 0.0659f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSUS), 13, 0.0f, 0.0774f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSUS), 12, 0.0f, 0.0054f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(FBEA), 13, 0.0f, 0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSLI), 10, 0.006f, 0.0993f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DSLI), 7, 0.004f, 0.0629f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DPIZ), 10, 0.0f, -0.007f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(DPIZ), 12, 0.0f, 0.030f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(KPA1), 10, 0.0f, 0.070f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(KPA1), 13, 0.0f, 0.149f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(BYO2), 10, 0.0f, -0.040f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(BYO2), 7, 0.0f, -0.040f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(BYO3), 10, 0.0f, 0.0528f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(BYO3), 7, 0.0f, 0.0469f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(JAZ2), 12, -0.0001f, 0.0379f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(JAZ2), 13, -0.0001f, 0.0379f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM1), 13, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM1), 7, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM2), 13, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM2), 7, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM3), 13, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM3), 7, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM4), 13, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SZM4), 7, 0.0f, -0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(MO01), 13, 0.0042f, 0.0042f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO1), 10, 0.0f, 0.150f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO1), 13, 0.0f, 0.150f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO1), 12, 0.0f, 0.130f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO1), 19, 0.0f, 0.100f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO2), 10, 0.0f, 0.150f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO2), 13, 0.0f, 0.150f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO2), 12, 0.0f, 0.130f},
    MoveObjChairAnchorAdjustment{SHEN_NAME(SYO2), 19, 0.0f, 0.100f},
};
namespace CycleManager {





struct FrameCacheStats {
    uint32_t hits = 0;
    uint32_t misses = 0;
    uint32_t entries = 0;
    uint32_t renderBatchHits = 0;
    uint32_t renderBatchMisses = 0;
    uint32_t renderBatchEntries = 0;
};



struct SeatInterval {
    float start = 0.0f;
    float end = 0.0f;
    bool startBoundaryFree = true;
    bool endBoundaryFree = true;
};
} // namespace CycleManager


#pragma pack(push, 1)

namespace CycleManager {

struct UtilityParameters {
    std::uint32_t command;
    std::uint32_t operand;
    std::uint32_t offset;
    shen_name areaId;
    std::uint32_t argument10;
    std::uint32_t argument14;
    std::uint32_t argument18;
    std::uint32_t argument1c;
};
} // namespace CycleManager

#pragma pack(pop)

} // namespace shenmue::s1
