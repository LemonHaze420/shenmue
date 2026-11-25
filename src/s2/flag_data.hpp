#pragma once

#include "common/types.hpp"
#include "common/math.hpp"

#include <stddef.h>

namespace shenmue { namespace s2 {

// Context table rows name the record list and search-target table of a map or callback context.
// Both offsets are dwords from the start of the context table; a zero name terminates the table.
struct FlagContextEntry {
    shen_name name;
    std::int32_t recordList;
    std::int32_t targetTable;
};

typedef char FlagDataLayoutCheck0[(sizeof(FlagContextEntry) == 0x0c) ? 1 : -1];

#if SHENMUE_IS_DC
namespace FlagOp {
enum Value {
#else
enum class FlagOp : std::uint8_t {
#endif
    End = 0x00,
    IfPlayerNearPoint = 0x01,
    IfPlayerInRect = 0x02,
    IfShenmueFlag = 0x03,
    IfEntryPoint = 0x04,
    IfCharaSector = 0x05,
    SceneChange = 0x06,
    LoadAreaRestart = 0x07,
    LoadAreaRestartValue = 0x08,
    IfCharaWithinDistance = 0x09,
    SetStepFlags = 0x0a,
    IfInternalFlagEquals = 0x0b,
    IfEvaluationMode1 = 0x0c,
    IfEvaluationMode0 = 0x0d,
    IfActionButton = 0x0e,
    IfRyoNotBusy = 0x0f,
    IfAuthorNotLoaded = 0x10,
    LockPlayerControl = 0x11,
    CallContextRecords = 0x12,
    IfPlayerControlEnabled = 0x13,
    FadeOutSoundAndHold = 0x14,
    SetInternalFlag = 0x15,
    SetTimeOfDay = 0x16,
    IfQronStream = 0x17,
    LoadPackRestart = 0x18,
    LoadPackRestartValue = 0x19,
    WarmStartNpcsRelease = 0x1a,
    WaitForIdleEvaluations = 0x1b,
    SceneChangeFadeMode9 = 0x1c,
    FadeOutAndHold = 0x1d,
    IfPlayerHeightNear = 0x1f,
    ExecAreaSlotRestart = 0x20,
    ExecAreaSlotRestartValue = 0x21,
    ShowEventMessage = 0x22,
    IfPlayerIdle = 0x24,
    IfPlayerNotRunning = 0x25,
#if SHENMUE_IS_DC
    TurnPlayerAndWait = 0x26,
#else
    Repeat = 0x26,
#endif
    PlayPlayerMotionF003 = 0x27,
    ReturnToFreeQuest = 0x28,
    WarmStartNpcs = 0x29,
    IfCashAtLeast = 0x2a,
    WaitAreaSlotUnloaded = 0x2b,
    LoadEventPackRestartValue = 0x2c,
    DeferEventPackRestartValue = 0x2d,
    IfScenarioRecord = 0x2e,
    IfItemOwned = 0x30,
    IfNamedActionButton = 0x31,
    IfPlayerNotInMenu = 0x32,
    IfSearchTarget = 0x33,
    IfCharaSectorOrSearch = 0x34,
    SetLoadingPhaseBits = 0x35,
    IfPointOnScreen = 0x36,
    IfCharaPointOnScreen = 0x37,
    IfPointOnScreenOrSearch = 0x38,
    IfActionButtonOrSearch = 0x39,
    IfCloseAndFacing = 0x3a,
    LoadAreaNext = 0x3b,
    LoadAreaNextValue = 0x3c,
    ExecAreaSlotNext = 0x3d,
    ExecAreaSlotNextValue = 0x3e,
    LoadPackNext = 0x3f,
    LoadPackNextAlt = 0x40,
    DeferEventPackNext = 0x41,
    DeferEventPackNextValue = 0x42,
    IfEntityEnabledOnScreen = 0x43,
    IfCharaVisible = 0x44,
    AlwaysFalse = 0x45,
    IfEvaluationMode2 = 0x46,
    IfQueryMode2 = 0x47,
    IfTimeReached = 0x48,
    SceneChangeFadeMode8 = 0x49,
    IfEntryPointSaveScene = 0x4a,
    AdvanceTimeToDeadline = 0x4b,
    WaitNpcRelease = 0x4c,
    IfEvaluationMode3 = 0x4d,
    IfQueryMode3 = 0x4e,
    WaitSpecialPose = 0x4f,
    ChooseAction = 0x50,
    LockPlayerControlMode3 = 0x51,
    IfSegaCoins = 0x52,
    PlayVoice = 0x53,
    IfCurfewSceneChange = 0x54,
    WarmStartHumanNpcs = 0x55,
    UnlockPartyCharas = 0x56,
    IfSavedAreaJump = 0x57,
    IfRyoNotBusyOrSearch = 0x58,
    WaitSearchRelease = 0x59,
    IfActEquals = 0x5a,
    IfPointOnScreenOrSearchB = 0x5b,
    StartFlagCamera = 0x5c,
    IfMapEquals = 0x5d,
    LockPlayerControlMode2 = 0x5f,
    IfCurfewWithin = 0x60,
    ApplyInvertedFlagSteps = 0x61,
    IfEntityOnScreenEnable = 0x62,
    SetPlayerFlag1000 = 0x63,
};
#if SHENMUE_IS_DC
} // namespace FlagOp
#endif

struct FlagSector {
    float radius;
    std::int32_t yawOffset;
    std::int32_t playerHalfAngle;
    std::int32_t charaHalfAngle;
    Vector3 offset;
    float heightTolerance;
};

typedef char FlagSectorLayoutCheck[(sizeof(FlagSector) == 0x20) ? 1 : -1];

struct FlagTargetOffset {
    Vector3 position;
    float lockonAngle;
    std::int16_t yaw;
    std::uint16_t yawLimit;
};

typedef char FlagTargetOffsetLayoutCheck[(sizeof(FlagTargetOffset) == 0x14) ? 1 : -1];

// Native flag_ctx_t: one shenEvaluateFlag record. Records are variable length. A clear bit 15 in
// `branch` selects the short form (length in `length`, false branch in `branch`); a set bit selects
// the long form (length in `branch & 0x7fff`, false branch in `length`). Offsets are in dwords.
struct FlagRecord {
    std::uint8_t opcode;
    std::uint8_t length;
    std::uint16_t branch;
    std::int32_t argument[5];
};

typedef char FlagDataLayoutCheck1[(sizeof(FlagRecord) == 0x18) ? 1 : -1];

#if SHENMUE_IS_DC
enum ShenPlayerFlags {
#else
enum ShenPlayerFlags : std::uint32_t {
#endif
    SPF_Active = 0x00000002U,
    SPF_HelpIconShown = 0x00000020U,
    SPF_WaitTimeAdvance = 0x00000040U,
    SPF_PackLoadPending = 0x00000080U,
    SPF_PackExecutePending = 0x00000100U,
    SPF_NpcReleasePending = 0x00000200U,
    SPF_SpecialPosePending = 0x00000400U,
    SPF_ActionChoicePending = 0x00000800U,
    SPF_EventMessageShown = 0x00001000U,
    SPF_VoicePending = 0x00002000U,
    SPF_FadeHold = 0x00004000U,
    SPF_CurfewSceneChange = 0x00008000U,
    SPF_SearchReleasePending = 0x00010000U,
    SPF_CurfewReached = 0x00040000U,
    SPF_DetachedVoicePending = 0x00080000U,
};

struct FlagBinHeader {
    std::int32_t field00;
    std::int32_t flagRecordTable; // +0x04
    std::uint32_t flagCount;      // +0x08
    std::int32_t flagWorkCount;   // +0x0c
    std::int32_t flagWorkTable;   // +0x10
    std::int32_t field14;
    std::int32_t contextTable; // +0x18
};

typedef char FlagDataLayoutCheck2[(sizeof(FlagBinHeader) == 0x1c) ? 1 : -1];
typedef char FlagDataLayoutCheck3[(offsetof(FlagBinHeader, flagCount) == 0x08) ? 1 : -1];
typedef char FlagDataLayoutCheck4[(offsetof(FlagBinHeader, contextTable) == 0x18) ? 1 : -1];

#if SHENMUE_IS_DC
enum FlagDescriptorSelector {
#else
enum FlagDescriptorSelector : std::uint32_t {
#endif
    FlagRecordCount = 1,
    FlagWorkCount = 2,
    FlagCountOverride = 3,
    ClearFlagRecordCount = 4,
    SetFlagCountOverride = 0x10000,
};

std::uint32_t GetFlagDescriptorValue(std::uint32_t selector);
std::uint32_t GetInitialFlagValue(std::uint32_t index);
void ResetFlagWorkFromDescriptor();

} } // namespace shenmue::s2
