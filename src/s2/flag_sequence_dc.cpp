#include "s2/flag_context_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_flags_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/system_manage_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/area_event.hpp"
#include "s2/scene_data.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/date_time.hpp"
#include "s2/math.hpp"
#include "platform/dc/matrix.hpp"
#include <math.h>
#include <string.h>
#include "s2/flag_startup_dc.hpp"
#include "s2/weather_data_dc.hpp"
#include <cstring>
#include "s2/player_state_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/flag_data.hpp"
#include "s2/story_state.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "platform/dc/sdk.hpp"
#include "s2/legacy_sprite_dc.hpp"
#endif
#include "s2/hlib.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/conversation_fade_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include "shenmue.h"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/time_program_dc.hpp"

namespace shenmue {
namespace s2 {

namespace {

struct FlagCandidate {
    FlagRecord *record;
    float distanceSquared;
};

void FlagFilmTask(void *);
bool IsWithinLocationScreenSpace(float x, float y, float z);
std::int32_t flddCheckCharaIntersection(const Vector3 &point, shen_name name, std::int32_t halfAngle, float radius);
std::int32_t DoAngleCheck(const FlagSector &sector, shen_name name, const Vector3 &playerPosition, std::int32_t playerYaw);
void flagSetLocationToChara(shen_name name);
std::int32_t shenCheckCloseAndFacing(const Vector3 &first, std::int32_t firstYaw, const Vector3 &second, std::int32_t secondYaw);
std::int32_t flagIsPlayerNearPoint(const FlagRecord *record);
bool ryoCheckInRangeForFlag(const FlagRecord *record);
std::int32_t LockFlagPlayerControl(std::int32_t mode);
void shenMoveToFreeQuest();
std::uint32_t FlagBranch(FlagRecord **record, std::int32_t condition);
void flagResetNpcLoad();
std::int32_t *FlagContextTable();
FlagRecord *FlagRecordAt(std::int32_t *table, std::int32_t offset);
MapControlData *FlagControl();
Character *FlagPlayer();
Character *FlagChara(shen_name name);
Vector3 FlagPosition(Character *character);
Vector3 FlagCharaBone(Character *character);
std::int32_t shenLoadScene2(const char *area, std::uintptr_t argument);
void shenLoadScene(std::uint32_t slot, std::uintptr_t argument);
bool flagTransformCharaOffset(shen_name name, const Vector3 &offset, Vector3 &point);

} // namespace

std::int32_t IsCurrentTimeRange(std::uint32_t range);
void InitializeFlagWeatherMode();
std::int32_t flagInitContext(ShenPlayerData &data, shen_name context);
std::int32_t shenUpdatePlayerDataFlags(std::int32_t active);
void shenRestartPlayerDataFlagEvaluation();
std::int32_t shenEvaluateFlag(ShenPlayerData &data, std::int32_t mode);
SearchTarget *shenFindFlagTarget(shen_name name);
static std::int32_t FlagRecordLength(const FlagRecord *record);
std::int32_t EV_authSceneCtrl(std::int32_t operation);
void ResetFlagWorkFromDescriptor();
void SetSystemManageSource(const char *source);

std::int32_t DiffMinutes(const DateTime &first, const DateTime &second);
void GetCurrentCalendarRecord(DateTime &record);
void DateTime_AddMinutesInPlace(DateTime &dateTime, std::int32_t minutes);
void SetCurrentDateTimeTo(DateTime &dateTime);
void shenGetCurrentDateTime(DateTime &dateTime);
bool GetInternalFlag(std::int32_t flag);
const std::int32_t *FindFlagDefinition(std::int32_t flag);
std::uint32_t stepUpdateFlagUtil(std::int32_t flag, std::int32_t mode);
bool GetInternalFlag(std::int32_t flag);
const std::int32_t *FindFlagDefinition(std::int32_t flag);
std::uint32_t stepUpdateFlagUtil(std::int32_t flag, std::int32_t mode);
bool EV_FreeAuthorPak(std::int32_t operation, const char *name);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t cvsHasValidConverseData(const ConversationBuffer *buffer);
#else
bool cvsHasValidConverseData(const ConversationBuffer *buffer);
#endif
void flagDrawDebugCylinder(const Vector3 &location, float radius);
void flagDrawDebugBox(float firstX, float firstZ, float secondX, float secondZ, float y);
std::int32_t npcReleaseUnusedStreams();
bool shenGetIsPlayerInMenu();
std::int32_t authGetLoaded();
std::int32_t plymGetSpecialFlag();
std::uint32_t GetPlayerAreaControlState(std::int32_t unused);

namespace {

Vector3 flagLocation;
FlagCandidate flagCandidates[10] = {};
std::int32_t flagCandidateCount;
std::uint32_t flagDebugFlags;

const FlagSector flagSectors[13] = {
    {1.8f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.0f, 0x8000, 0x3000, 0x3e38, {0.3f, 0.0f, 0.0f}, 0.3f},
    {2.0f, 0x8000, 0x3000, 0x3e38, {-0.34f, 0.0f, 0.0f}, 0.3f},
    {-1.0f, 0x4000, 0x4000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {3.0f, 0x4000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.0f, 0x8000, 0x238e, 0x238e, {0.0f, 0.0f, 0.0f}, 0.3f},
};

const FlagTargetOffset flagTargetOffsets[13] = {
    {{0.0f, 0.74f, 0.13f}, 0.5f, -32768, 15928},
    {{0.3f, 1.11f, 0.32f}, 0.5f, -32768, 15928},
    {{-0.34f, 1.08f, 0.57f}, 0.5f, -32768, 15928},
    {{0.0f, 1.04f, 0.0f}, 0.5f, 16384, 15928},
    {{0.0f, 1.04f, 0.0f}, 0.5f, 16384, 15928},
    {{0.0f, 1.35f, 0.1f}, 0.5f, -32768, 15928},
    {{0.0f, 1.6f, 0.1f}, 0.5f, -32768, 15928},
    {{0.0f, 1.36f, 0.37f}, 0.5f, -32768, 15928},
    {{0.0f, 0.45f, 0.0f}, 0.5f, -32768, 15928},
    {{0.0f, 0.55f, 0.0f}, 0.5f, -32768, 15928},
    {{0.0f, 0.95f, 0.0f}, 0.5f, -32768, 15928},
    {{0.0f, 7.6f, 0.0f}, 0.5f, -32768, 15928},
    {{0.0f, 1.3f, 0.5f}, 0.5f, -32768, 15928},
};

} // namespace

ShenPlayerData shenPlayerData;
FlagRecord *g_selectedFlagCandidate;
char systemManageSource[0x14];
std::int32_t systemManageFrames;
std::byte *g_flagDescriptorData = 0;
std::uint16_t *g_flagWork;
std::uint32_t g_flagCountOverride = 0;
void shenGetCurrentDateTime(DateTime &dateTime);

namespace {

float FlagFloat(std::int32_t value) {
    float result;
    memcpy(&result, &value, sizeof(result));
    return result;
}

FlagRecord *FlagAdvance(FlagRecord *record, std::uint32_t count) {
    return reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + count);
}

} // namespace

// @verified - d
std::int32_t EV_GetShenmueFlag(std::int32_t flag) {
    return GetShenmueFlag(flag, 0);
}

// @verified - d
void EV_stepSetFlags(std::int32_t flag, std::int32_t mode) {
    if (stepUpdateFlagUtil(flag, mode) & 1U)
        stepUpdateFlagUtil(200, 0);
    stepUpdateFlagUtil(500, 0);
    InitializeFlagWeatherMode();
}

std::int32_t GetShenmueFlag(std::int32_t flag, std::int32_t) {
    std::int32_t running = 1, state = 2, result = 0, deferred = 0;
    const std::int32_t *command = FindFlagDefinition(flag) + 2;
    do {
        std::int32_t operand = *command++;
        const std::int16_t op = operand >> 24;
        operand &= 0xffffff;
        switch (op) {
        case ShenFlagOp_End:
            running = 0;
            break;
        case ShenFlagOp_RequireInternalFlagSet:
            if (!EV_GetPlayerFlag(PlayerFlagReadMode_Internal, static_cast<std::int16_t>(operand)))
                state |= 1;
            break;
        case ShenFlagOp_RequireInternalFlagClear:
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, static_cast<std::int16_t>(operand)))
                state |= 1;
            break;
        case ShenFlagOp_StepAtLeastToggleA:
            if (static_cast<std::int32_t>(GetCurrentStep()) >= operand)
                state ^= 2;
            break;
        case ShenFlagOp_StepAtLeastToggleB:
            if (static_cast<std::int32_t>(GetCurrentStep()) >= operand)
                state ^= 2;
            break;
        case ShenFlagOp_RequireStatBelow:
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(operand >> 16)) >= static_cast<std::int16_t>(operand))
                state |= 1;
            break;
        case ShenFlagOp_RequireStatAtLeast:
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(operand >> 16)) < static_cast<std::int16_t>(operand))
                state |= 1;
            break;
        case ShenFlagOp_RequireItem:
            if (!GetItemState(static_cast<std::uint32_t>(operand)))
                state |= 1;
            break;
        case ShenFlagOp_ToggleMode:
            state ^= 2;
            break;
        case ShenFlagOp_RequireTimeInRange:
            if (!IsCurrentTimeRange(static_cast<std::uint32_t>(*command++)))
                state |= 1;
            break;
        case ShenFlagOp_CommitGroup:
            if (!state)
                result = 1;
            state &= 0xe;
            break;
        case ShenFlagOp_ResetGroup:
            if (!state)
                result = 1;
            state = 2;
            break;
        case ShenFlagOp_RequireFlag:
            if (!GetShenmueFlag(operand, 0))
                state |= 1;
            break;
        }
    } while (running);
    return result && deferred ? 2 : result;
}

// @verified - d
std::int32_t shenGetMinutesUntilNextFrameMinute() {
    DateTime target;
    DateTime now;
    shenGetCurrentDateTime(now);
    BuildTargetDateFromFrameClock(target);
    DateTime_AddMinutesInPlace(target, 1);
    const std::int32_t minutes = DiffMinutes(target, now);
    return minutes < 0 ? 0 : minutes;
}

// @verified - d
std::uint32_t ScenarioConversationResultNullsub() {
    return 0;
}

// @verified - d
std::int32_t IsCurrentTimeRange(std::uint32_t range) {
    DateTime current;
    shenGetCurrentDateTime(current);
    if (current.hour < 7)
        current.hour += 24;
    const std::uint16_t time = static_cast<std::uint16_t>((current.hour << 8) | current.minute);
    const std::uint16_t end = static_cast<std::uint16_t>(range >> 16);
    const std::uint16_t start = static_cast<std::uint16_t>(range);
    if (end < start) {
        if (time >= end && start >= time)
            return 1;
    } else if (end >= time && time >= start)
        return 1;
    return 0;
}

// @verified - d
void AllocateFlagWork() {
    std::byte *const data = static_cast<std::byte *>(shenLoadFile("misc", "FLAG.BIN", FLAG_DESCRIPTOR_STORAGE));
    HLib::SetTaskDataMotionFlag(data);
    g_flagDescriptorData = data;
    g_flagWork = static_cast<std::uint16_t *>(HLib::AllocateTaskData(kFlagWorkBytes, FLAG_WORK_STORAGE));
    HLib::SetTaskDataMotionFlag(g_flagWork);
    ResetFlagWorkFromDescriptor();
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t stepEvaluateFlagStepRange(std::int32_t flag, std::uint32_t mode) {
    std::int32_t minimumStep = 3000000, maximumStep = 0, scanning = 1;
    const std::int32_t *const definition = FindFlagDefinition(flag);
    if (!definition)
        return -1;
    const std::int32_t *command = definition + 2;
    do {
        std::int32_t operand = *command++;
        const std::int16_t op = static_cast<std::int16_t>(operand >> 24);
        operand &= 0xffffff;
        switch (op) {
        case StepRangeOp_End:
        case StepRangeOp_EndGroup:
        case StepRangeOp_EndGroupB:
        case StepRangeOp_EndGroupC:
            scanning = 0;
            break;
        case StepRangeOp_SetInternalFlag:
            if (mode & StepRangeMode_ApplyFlags)
                EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, static_cast<std::int16_t>(operand), 1);
            break;
        case StepRangeOp_ClearInternalFlag:
            if (mode & StepRangeMode_ApplyFlags)
                EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, static_cast<std::int16_t>(operand), 0);
            break;
        case StepRangeOp_MinimumStep:
            if (operand < minimumStep)
                minimumStep = operand;
            break;
        case StepRangeOp_MaximumStep:
            if (maximumStep < operand)
                maximumStep = operand;
            break;
        case StepRangeOp_ResetRange:
            minimumStep = 0;
            maximumStep = 0;
            break;
        case StepRangeOp_RaiseStat:
            if (mode & StepRangeMode_ApplyFlags) {
                const std::int16_t stat = static_cast<std::int16_t>(operand >> 16);
                const std::int32_t current = EV_GetPlayerFlag(PlayerFlagReadMode_Stat, stat);
                const std::int32_t value = operand & 0xff;
                if (current < value)
                    EV_SetPlayerFlag(PlayerFlagWriteMode_Stat, stat, value);
            }
            break;
        case StepRangeOp_LowerStat:
            if (mode & StepRangeMode_ApplyFlags) {
                const std::int16_t stat = static_cast<std::int16_t>(operand >> 16);
                const std::int32_t current = EV_GetPlayerFlag(PlayerFlagReadMode_Stat, stat);
                const std::int32_t value = operand & 0xff;
                if (current >= value && value)
                    EV_SetPlayerFlag(PlayerFlagWriteMode_Stat, stat, value - 1);
            }
            break;
        case StepRangeOp_Include:
            stepEvaluateFlagStepRange(operand, mode);
            break;
        }
    } while (scanning);
    switch (static_cast<std::int32_t>(mode)) {
    case StepRangeMode_Minimum:
        return minimumStep;
    case StepRangeMode_Maximum:
        return maximumStep;
    case StepRangeMode_ApplyAndSetStep:
        SetSTEP(minimumStep);
        break;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const char *GetFlagName(std::uint32_t flag);
void PrintFlagHundredths(std::int32_t value);

static std::uint32_t flagPrintFrame;
static std::int32_t flagPrintLine = 6;
static std::int32_t flagPrintColumn = 0;

void PrintFlagDefinition(std::int32_t flag, std::uint32_t mode) {
    std::int32_t scanning = 1;
    const std::int32_t *const definition = FindFlagDefinition(flag);
    if (!definition)
        return;
    const std::int32_t *command = definition + 2;
    if (mode & 1) {
        if (flagPrintFrame != g_frameCounter) {
            flagPrintFrame = g_frameCounter;
            flagPrintLine = 6;
            flagPrintColumn = 0;
        } else {
            flagPrintLine++;
        }
        SetDebugScreenTextPosition(flagPrintColumn, flagPrintLine);
        shenmue_s2_event_DebugLogToScreen("%3d %s\n", flag, GetFlagName(flag));
    }
    do {
        std::int32_t operand = *command++;
        const std::int16_t op = static_cast<std::int16_t>(operand >> 24);
        operand &= 0xffffff;
        switch (op) {
        case StepRangeOp_End:
            scanning = 0;
            if (mode & 4)
                shenmue_s2_event_DebugLogToScreen("END\n");
            break;
        case StepRangeOp_SetInternalFlag:
            if (mode & 4) {
                SetDebugScreenTextColour(EV_GetPlayerFlag(PlayerFlagReadMode_Internal, static_cast<std::int16_t>(operand)) ? 0xffffffffU : 0x9fffffffU);
                shenmue_s2_event_DebugLogToScreen("FLAG %d ON\n", operand);
            }
            break;
        case StepRangeOp_ClearInternalFlag:
            if (mode & 4) {
                SetDebugScreenTextColour(EV_GetPlayerFlag(PlayerFlagReadMode_Internal, static_cast<std::int16_t>(operand)) ? 0x9fffffffU : 0xffffffffU);
                shenmue_s2_event_DebugLogToScreen("FLAG %d OFF\n", operand);
            }
            break;
        case StepRangeOp_MinimumStep:
            if (mode & 4) {
                shenmue_s2_event_DebugLogToScreen("START_STEP ");
                PrintFlagHundredths(operand);
                shenmue_s2_event_DebugLogToScreen("\n");
            }
            break;
        case StepRangeOp_MaximumStep:
            if (mode & 4) {
                shenmue_s2_event_DebugLogToScreen("END_STEP   ");
                PrintFlagHundredths(operand);
                shenmue_s2_event_DebugLogToScreen("\n");
            }
            break;
        case StepRangeOp_ResetRange:
            if (mode & 4)
                shenmue_s2_event_DebugLogToScreen("STEP_FREE\n");
            break;
        case StepRangeOp_EndGroup:
            if (mode & 4)
                shenmue_s2_event_DebugLogToScreen(" or\n");
            break;
        case StepRangeOp_EndGroupB:
            if (mode & 4)
                shenmue_s2_event_DebugLogToScreen(" OR\n");
            break;
        case StepRangeOp_Include:
            if (mode & 4) {
                SetDebugScreenTextColour(GetShenmueFlag(operand, 0) ? 0xffffffffU : 0x9fffffffU);
                shenmue_s2_event_DebugLogToScreen("check(%d)\n", operand);
            }
            break;
        case StepRangeOp_EndGroupC:
            if (mode & 4)
                shenmue_s2_event_DebugLogToScreen("CHECKEX(%d)\n", operand);
            break;
        case StepRangeOp_RaiseStat:
            if (mode & 4) {
                SetDebugScreenTextColour(
                    EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(operand >> 16)) >= static_cast<std::int16_t>(operand) ? 0xffffffffU :
                                                                                                                                                0x9fffffffU);
                const std::int32_t stat = static_cast<std::int16_t>(operand >> 16);
                shenmue_s2_event_DebugLogToScreen(
                    "COUNTER[%d](%d)>=%d\n", stat, EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(stat)), static_cast<std::int16_t>(operand));
            }
            break;
        case StepRangeOp_LowerStat:
            if (mode & 4) {
                SetDebugScreenTextColour(
                    EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(operand >> 16)) < static_cast<std::int16_t>(operand) ? 0xffffffffU :
                                                                                                                                               0x9fffffffU);
                const std::int32_t stat = static_cast<std::int16_t>(operand >> 16);
                shenmue_s2_event_DebugLogToScreen(
                    "COUNTER[%d](%d)<%d\n", stat, EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(stat)), static_cast<std::int16_t>(operand));
            }
            break;
        case 13:
            shenmue_s2_event_DebugLogToScreen("WAIT_");
        case 12: {
            const std::uint8_t minute1 = *reinterpret_cast<const std::uint8_t *>(command);
            command = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::uint8_t *>(command) + 1);
            const std::uint8_t hour1 = *reinterpret_cast<const std::uint8_t *>(command);
            command = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::uint8_t *>(command) + 1);
            const std::uint8_t minute0 = *reinterpret_cast<const std::uint8_t *>(command);
            command = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::uint8_t *>(command) + 1);
            const std::uint8_t hour0 = *reinterpret_cast<const std::uint8_t *>(command);
            command = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::uint8_t *>(command) + 1);
            shenmue_s2_event_DebugLogToScreen("TIME %2d:%2d-%2d:%2d\n", hour0, minute0, hour1, minute1);
            break;
        }
        }
        SetDebugScreenTextColour(0xffffffffU);
    } while (scanning);
    if (mode & 2)
        shenmue_s2_event_DebugLogToScreen("\n");
}
#endif

// @verified - d
std::uint32_t GetInitialFlagValue(std::uint32_t index) {
    if (reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->flagWorkTable == 0)
        return 0xffffffff;
    const FlagBinHeader *const header = reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData);
    if (index >= static_cast<std::uint32_t>(header->flagWorkCount))
        return 0xffffffff;
    const std::uint32_t *const values = reinterpret_cast<const std::uint32_t *>(header) + header->flagWorkTable;
    return values[index];
}

const char *GetFlagName(std::uint32_t flag) {
    if (!reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->field14)
        return "-empty-";
    if (!FindFlagDefinition(static_cast<std::int32_t>(flag)))
        return "-empty-";
    if (reinterpret_cast<const std::uint8_t *>(FindFlagDefinition(static_cast<std::int32_t>(flag)))[7] & 0x20)
        return "-empty-";
    if (flag >= reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->flagCount)
        return "-empty-";
    const std::byte *const table = g_flagDescriptorData + reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->field14 * 4;
    const std::uint32_t offset = reinterpret_cast<const std::uint32_t *>(table)[flag];
    if (offset)
        return reinterpret_cast<const char *>(table + offset);
    return "-empty-";
}

// @verified - d
std::uint32_t GetFlagDescriptorValue(std::uint32_t selector) {
    if (g_flagDescriptorData) {
        const std::int16_t operation = static_cast<std::int16_t>(selector >> 16);
        switch (operation) {
        case 0:
            switch (selector) {
            case FlagRecordCount:
                return reinterpret_cast<FlagBinHeader *>(g_flagDescriptorData)->flagCount;
            case FlagWorkCount:
                return static_cast<std::uint32_t>(reinterpret_cast<FlagBinHeader *>(g_flagDescriptorData)->flagWorkCount);
            case FlagCountOverride:
                return g_flagCountOverride;
            case ClearFlagRecordCount:
                reinterpret_cast<FlagBinHeader *>(g_flagDescriptorData)->flagCount = 0;
                break;
            default:
                return 0;
            }
            break;
        case 1:
            g_flagCountOverride = selector & 0xffff;
            break;
        default:
            return 0;
        }
    }
    return 0;
}

// @verified - d
std::int16_t shenGetSleepScheduleFlag() {
    return shenPlayerData.sleepScheduleFlag;
}

// @verified - d
void InitializeFlagWeatherMode() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    WSCN_SetWeatherShouldUpdateDay(EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 1001) != 0);
#else
    SetWeatherShouldUpdateDay(EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 1001) != 0);
#endif
}

// @verified - d
std::int32_t shenScheduleFlagCallback(shen_name, std::int16_t minute) {
    plymSetCallback(shenRestartPlayerDataFlagEvaluation);
    return plymScheduleCycleFlagCallback(minute);
}

// @verified - d
std::int32_t npcSetCycleTimeLimitExpiry(shen_name name, std::int16_t timeLimit) {
    std::int32_t result;
    if (timeLimit) {
        DateTime now;
        shenGetCurrentDateTime(now);
        const std::int16_t minute = static_cast<std::int16_t>(now.hour * 60 + (now.minute + timeLimit));
        result = shenScheduleFlagCallback(name, minute);
    } else
        result = shenScheduleFlagCallback(shen_name(timeLimit), timeLimit);
    return result;
}

// @verified - d
std::int32_t Queue_FLAG_CheckTask(shen_name area) {
    plymSetCallback(shenRestartPlayerDataFlagEvaluation);
    InitializeFlagWeatherMode();
    const std::int32_t initialized = flagInitContext(shenPlayerData, area);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    shenPlayerData.callbackContext = FLAG_CALLBACK_CYCLE;
#else
    std::memcpy(reinterpret_cast<std::byte *>(&shenPlayerData) + kFlagCallbackContextOffset, &FLAG_CALLBACK_CYCLE, sizeof(FLAG_CALLBACK_CYCLE));
#endif
    if (initialized)
        shenUpdatePlayerDataFlags(1);
    return initialized;
}

// @verified - d
std::int32_t flagInitContext(ShenPlayerData &data, shen_name context) {
    data.flags = 0;
    data.fadeHoldFrames = 0;
    data.updateCount = 0;
    data.messageFrames = 0;
    data.deferredRecord = 0;
    data.triggerRecord = 0;
    data.targets = 0;
    data.idleEvaluations = 3;
    data.voice = 0;
    data.callbackContext = shen_name();
    data.callDepth = 0;
    data.record = 0;
    std::int32_t *words = reinterpret_cast<std::int32_t *>(g_flagDescriptorData) + reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->contextTable;
    std::int32_t index = 0;
    while (reinterpret_cast<const FlagContextEntry *>(words)[index].name != context) {
        if (!reinterpret_cast<const FlagContextEntry *>(words)[index].name)
            return 0;
        ++index;
    }
    data.contextIndex = static_cast<std::int16_t>(index);
    data.context = context;
    words = reinterpret_cast<std::int32_t *>(g_flagDescriptorData) + reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->contextTable;
    if (!reinterpret_cast<const FlagContextEntry *>(words)[data.contextIndex].name)
        return 0;
    words += reinterpret_cast<const FlagContextEntry *>(words)[data.contextIndex].recordList;
    data.firstRecord = reinterpret_cast<FlagRecord *>(words);
    data.record = data.firstRecord;
    words = reinterpret_cast<std::int32_t *>(g_flagDescriptorData) + reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->contextTable;
    words += reinterpret_cast<const FlagContextEntry *>(words)[data.contextIndex].targetTable;
    data.targets = reinterpret_cast<SearchTarget *>(words);
    if (plymConsumeFlags8())
        data.flags |= SPF_CurfewSceneChange;
    g_selectedFlagCandidate = 0;
    return data.contextIndex + 1;
}

// @verified - d
std::int32_t shenUpdatePlayerDataFlags(std::int32_t active) {
    std::int32_t previous = (static_cast<std::int32_t>(shenPlayerData.flags) >> 1) & 1;
    if (active)
        shenPlayerData.flags |= SPF_Active;
    else
        shenPlayerData.flags &= ~SPF_Active;
    return previous;
}

// @verified - d
std::int32_t shenUpdateFlagTargets(SearchTarget *targets) {
    SearchTarget *target = targets;
    std::int32_t count = 0;
    for (; target->name; ++target, ++count) {
        switch (target->flags & 0xffff0000U) {
        case 0x80030000U:
            target->flags = 0;
            target->field08 = 0;
            break;
        case 0x80010000U: {
            const std::uint32_t name = target->field08;
            std::int16_t &yaw = target->yaw;
            const std::int32_t index = yaw;
            Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
            if (!character) {
                target->flags = 2;
                break;
            }
            Vector3 position;
            GetCharacterWorldPosition(character, &position);
            const std::int32_t characterYaw = GetCharacterWorldYaw(character);
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&position);
            Matrix4x4::RotateAroundY_Wrapper(characterYaw);
            Matrix4x4::TransformPoint(flagTargetOffsets[index].position, target->position);
            PopCurrentMatrix(1);
            yaw = static_cast<std::int16_t>(flagTargetOffsets[index].yaw + characterYaw);
            target->flags = 0;
            target->field08 = 0;
            target->lockonAngle = flagTargetOffsets[0].lockonAngle;
            target->yawLimit = flagTargetOffsets[index].yawLimit;
            break;
        }
        case 0x80020000U: {
            const std::uint32_t name = target->field08;
            Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
            if (character) {
                Vector3 position;
                GetCharacterWorldPosition(character, &position);
                const std::int32_t characterYaw = GetCharacterWorldYaw(character);
                const Vector3 local = target->position;
                matrix_stack::PushUnitMatrix();
                Matrix4x4::preTranslate(position.x, position.y, position.z);
                Matrix4x4::RotateAroundY_Wrapper(characterYaw);
                Matrix4x4::TransformPoint(local, target->position);
                PopCurrentMatrix(1);
                target->yaw = static_cast<std::int16_t>(target->yaw + characterYaw);
                target->flags = 0;
                target->field08 = 0;
                target->yawLimit = flagTargetOffsets[4].yawLimit;
            } else
                target->flags = 2;
            break;
        }
        }
    }
    return count;
}

// @verified - d
std::int32_t shenUpdateFlagEvaluation(std::int32_t mode) {
    flagCandidateCount = 0;
    shenPlayerData.queryId = 0;
    shenPlayerData.queryArgument = 0;
    ++shenPlayerData.updateCount;
    if (!(shenPlayerData.flags & SPF_Active) || !shenPlayerData.record)
        return 0;
    if (mode == 1)
        shenUpdateFlagTargets(shenPlayerData.targets);
    if ((shenPlayerData.flags & SPF_FadeHold) && --reinterpret_cast<std::int16_t &>(shenPlayerData.fadeHoldFrames) == 0)
        shenPlayerData.flags &= ~SPF_FadeHold;
    if (shenPlayerData.flags & SPF_EventMessageShown) {
        if (--shenPlayerData.messageFrames == 0) {
            scenDestroySubtitles(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask);
            shenPlayerData.flags &= ~SPF_EventMessageShown;
        }
        return 0;
    }
    if (shenPlayerData.flags & SPF_VoicePending) {
        if (!cvsHasValidConverseData(shenPlayerData.voice)) {
            std::int32_t bank[2];
            bank[0] = shenPlayerData.record->argument[0];
            bank[1] = 0;
            EV_FreeAuthorPak(0, reinterpret_cast<const char *>(bank));
            FlagBranch(&shenPlayerData.record, 1);
            shenPlayerData.flags &= ~SPF_VoicePending;
        }
        return 0;
    }
    if (shenPlayerData.eventTransitionPhase) {
        MotionWork *motion = charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
        if (static_cast<std::int16_t>(shenPlayerData.eventTransitionPhase) == 1 && !mtwkSetExtraFlag(motion, 58, 1)) {
            mtwkRequestMotion(motion, 0xf005, LowerBody);
            shenPlayerData.eventTransitionPhase = 2;
        }
        if (static_cast<std::int16_t>(shenPlayerData.eventTransitionPhase) == 2 && mtwkSetRegionFlag(motion, 65, 1, LowerBody)) {
            mtwkRequestMotion(motion, 0xf005, LowerBody);
            shenPlayerData.eventTransitionPhase = 0;
            SetPlayerControlEnabled(1);
        }
        return 1;
    }
    if (shenPlayerData.flags & SPF_PackLoadPending) {
        if (!PakLoadWait() && GetPlayerAreaControlState(1))
            shenPlayerData.flags = (shenPlayerData.flags & ~SPF_PackLoadPending) | SPF_PackExecutePending;
        else
            return 1;
    }
    if (shenPlayerData.flags & SPF_PackExecutePending) {
        shenPlayerData.flags &= ~SPF_PackExecutePending;
        shenMoveToFreeQuest();
        if (shenPlayerData.record->opcode == 0x2c || shenPlayerData.record->opcode == 0x41)
            ExecuteCurrentAreaMainFunction(
                static_cast<EventFunctionType>(shenPlayerData.record->argument[3]), reinterpret_cast<std::uintptr_t>(&shenPlayerData.record->argument[4]));
        else
            ExecuteCurrentAreaMainFunction(
                static_cast<EventFunctionType>(shenPlayerData.record->argument[3]), static_cast<std::uintptr_t>(shenPlayerData.record->argument[4]));
        if (static_cast<std::uint8_t>(shenPlayerData.record->opcode - 0x2c) <= 1U) {
            shenPlayerData.record = shenPlayerData.firstRecord;
            shenPlayerData.callDepth = 0;
        } else
            FlagBranch(&shenPlayerData.record, 1);
        return 1;
    }
    if (shenPlayerData.flags & SPF_NpcReleasePending) {
        if (PrepareNpcAllFromArray())
            shenPlayerData.flags &= ~SPF_NpcReleasePending;
        return 1;
    }
    if (shenPlayerData.flags & SPF_SpecialPosePending) {
        if (!GetPlayerAreaControlState(1))
            return 1;
        shenPlayerData.flags &= ~SPF_SpecialPosePending;
    }
    flagCandidateCount = 0;
    const std::int32_t result = shenEvaluateFlag(shenPlayerData, mode);
    std::int32_t count = flagCandidateCount;
    if (count) {
        float nearest = 99.0f;
        FlagCandidate *candidate = flagCandidates;
        FlagRecord *selected = 0;
        g_selectedFlagCandidate = candidate->record;
        do {
            if (nearest > candidate->distanceSquared) {
                nearest = candidate->distanceSquared;
                selected = candidate->record;
            }
            ++candidate;
        } while (--count);
        g_selectedFlagCandidate = selected;
    }
    return result;
}

namespace {

std::int32_t flagAddCandidate(FlagRecord *record, float distanceSquared) {
    if (flagCandidateCount < 10) {
        flagCandidates[flagCandidateCount].distanceSquared = distanceSquared;
        flagCandidates[flagCandidateCount].record = record;
        ++flagCandidateCount;
    }
    if (record == g_selectedFlagCandidate)
        return 1;
    return 0;
}

std::int32_t flagRegisterCandidate(FlagRecord *record) {
    Character *player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (shenPlayerData.queryId || !player)
        return 1;
    Vector3 position;
    GetCharacterWorldPosition(player, &position);
    position.x -= flagLocation.x;
    position.z -= flagLocation.z;
    return flagAddCandidate(record, position.x * position.x + position.z * position.z);
}

} // namespace

static void flagTurnPlayerAndWait(Vector3 *position, std::int32_t angle) {
    mtwkCopyTransition(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), position, angle, 6, 0);
    shenPlayerData.eventTransitionPhase = 1;
    LockFlagPlayerControl(0);
    shenToggleAreaFlags(0);
}

// @verified - d
std::int32_t scenEvaluateCurrentSequence(shen_name name, std::int8_t mode) {
    if (!shenPlayerData.record)
        return 0;
    shenPlayerData.queryId = name;
    shenPlayerData.queryArgument = mode;
    return shenEvaluateFlag(shenPlayerData, 2);
}

// @verified - d
std::int32_t shenEvaluateFlagFor(std::int32_t subject, std::int8_t mode) {
    if (!shenPlayerData.record)
        return 0;
    shenPlayerData.queryId = subject;
    shenPlayerData.queryArgument = mode;
    return shenEvaluateFlag(shenPlayerData, 3);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// GameJam copies these two indeterminate scratch fields without initializing the context first.
// Defined evaluator paths must not depend on their values; retail initializes its different-sized owner here.
void shenRestartPlayerDataFlagEvaluation() {
    ShenPlayerData context;
    shenPlayerData.record = context.record;
    shenPlayerData.callDepth = context.callDepth;
    shenEvaluateFlag(shenPlayerData, 0);
}
#endif

std::int32_t shenEvaluatePlayerDataFlag(std::int32_t mode) {
    if (!shenPlayerData.record)
        return 0;
    return shenEvaluateFlag(shenPlayerData, mode);
}

static void flagRewindPlayerData(ShenPlayerData &) {
    shenPlayerData.record = shenPlayerData.firstRecord;
    shenPlayerData.callDepth = 0;
}

std::int32_t shenEvaluateFlag(ShenPlayerData &data, std::int32_t mode) {
    FlagRecord *record = shenPlayerData.record;
    if (shenPlayerData.flags & SPF_FadeHold)
        return 0;
    if (shenPlayerData.flags & SPF_WaitTimeAdvance) {
        FlagRecord *const deadline = shenPlayerData.deadlineRecord;
        switch (static_cast<std::int32_t>(plymGetAreaExitState())) {
        case 0:
            shenPlayerData.flags &= ~SPF_WaitTimeAdvance;
            record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(deadline) + deadline->argument[3]);
            shenPlayerData.idleEvaluations = 0;
            break;
        case 1:
            shenPlayerData.flags &= ~SPF_WaitTimeAdvance;
            data.deferredRecord = 0;
            record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(deadline) + deadline->argument[2]);
            break;
        case -1:
            return 1;
        }
    }
    if (shenPlayerData.flags & SPF_ActionChoicePending) {
        if (!GetShouldRenderIcons(ActionSelector)) {
            const std::int32_t choice = static_cast<std::int32_t>(shenGetCtrlSpeakIconFlag());
            shenPlayerData.idleEvaluations = 0;
            if (choice == record->argument[0])
                record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + record->argument[3]);
            else
                record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + record->argument[4]);
            shenPlayerData.flags &= ~SPF_ActionChoicePending;
        } else {
            return 1;
        }
    }
    struct {
        std::int8_t result;
        std::int8_t actionButtonRegistered;
        std::int8_t queueFilmTask;
        std::int8_t debugDraw;
        std::int32_t continueEvaluation;
        std::int32_t helpTopic;
        Character *player;
        Vector3 playerPosition;
        std::int32_t playerYaw;
        std::int32_t searchTarget;
    } state;
    state.playerYaw = 0;
    state.continueEvaluation = 1;
    state.result = 0;
    state.helpTopic = 0;
    state.actionButtonRegistered = 0;
    state.queueFilmTask = 0;
    state.searchTarget = srchGetTargetName(0);
    state.debugDraw = static_cast<std::int8_t>(flagDebugFlags & 2U);
    state.player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (state.player) {
        GetCharacterWorldPosition(state.player, &state.playerPosition);
        state.playerYaw = GetCharacterWorldYaw(state.player);
    } else {
        state.playerPosition.x = state.playerPosition.y = state.playerPosition.z = -1000.0f;
    }
    do {
        switch (record->opcode) {
        case FlagOp::End:
            state.continueEvaluation = 0;
            if (shenPlayerData.callDepth) {
                record = shenPlayerData.returnRecords[--shenPlayerData.callDepth];
                state.continueEvaluation = 1;
                break;
            }
            shenPlayerData.record = shenPlayerData.firstRecord;
            shenPlayerData.callDepth = 0;
            if (ryoNotBusy()) {
                data.deferredRecord = 0;
                ++shenPlayerData.idleEvaluations;
            }
            state.continueEvaluation = 0;
            break;
        case FlagOp::WaitAreaSlotUnloaded:
            if (GetAreaSlotName(static_cast<std::uint32_t>(record->argument[0])) && GetAreaSlotName(static_cast<std::uint32_t>(record->argument[0]))[0] &&
                shenPlayerData.freeQuestActive) {
                shenPlayerData.record = record;
                state.result = 1;
                state.continueEvaluation = 0;
                break;
            }
            FlagBranch(&record, 1);
            break;
        case FlagOp::WaitSpecialPose:
            LockFlagPlayerControl(1);
            FlagBranch(&record, 1);
            shenPlayerData.flags |= SPF_SpecialPosePending;
            shenPlayerData.record = record;
            state.result = 1;
            state.continueEvaluation = 0;
            break;
        case FlagOp::IfTimeReached: {
            DateTime dateTime;
            std::uint8_t hour;
            shenGetCurrentDateTime(dateTime);
            if (dateTime.hour > 6)
                hour = static_cast<std::uint8_t>(dateTime.hour);
            else
                hour = static_cast<std::uint8_t>(dateTime.hour + 24);
            if (hour < static_cast<std::uint32_t>(record->argument[0]) ||
                (hour == static_cast<std::uint32_t>(record->argument[0]) && dateTime.minute < record->argument[1])) {
                if (shenPlayerData.idleEvaluations <= 2 || data.deferredRecord == record) {
                    flagRewindPlayerData(data);
                    shenPlayerData.idleEvaluations = 0;
                    state.continueEvaluation = 0;
                } else {
                    shenPlayerData.deadlineRecord = record;
                    FlagBranch(&record, 1);
                }
            } else {
                record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + record->argument[2]);
            }
            break;
        }
        case FlagOp::ChooseAction: {
            std::int16_t choices[4];
            LockFlagPlayerControl(1);
            choices[0] = choices[3] = -1;
            choices[1] = static_cast<std::int16_t>(record->argument[0]);
            choices[2] = static_cast<std::int16_t>(record->argument[1]);
            SetupControlSpeakAndUI_Internal(choices, 9999, static_cast<std::int16_t>(record->argument[2]), 2, 0);
            shenPlayerData.flags |= SPF_ActionChoicePending;
            data.deferredRecord = record;
            shenPlayerData.record = record;
            state.continueEvaluation = 0;
            state.result = 1;
            break;
        }
        case FlagOp::AdvanceTimeToDeadline: {
            FlagRecord *const deadline = shenPlayerData.deadlineRecord;
            plymAdvanceTime(static_cast<std::uint8_t>(deadline->argument[0]), static_cast<std::uint8_t>(deadline->argument[1]));
            shenPlayerData.flags |= SPF_WaitTimeAdvance;
            data.deferredRecord = deadline;
            shenPlayerData.record = deadline;
            state.continueEvaluation = 0;
            state.result = 1;
            break;
        }
        case FlagOp::IfPlayerNearPoint: {
            Vector3 location;
            location.x = reinterpret_cast<const float *>(record->argument)[0];
            location.y = state.playerPosition.y;
            location.z = reinterpret_cast<const float *>(record->argument)[1];
            if (flagDebugFlags & 1U)
                flagDrawDebugCylinder(location, reinterpret_cast<const float *>(record->argument)[2]);
            location.y = state.playerPosition.y;
            flagLocation = location;
            if (flagIsPlayerNearPoint(record)) {
                shenPlayerData.triggerRecord = record;
                FlagBranch(&record, 1);
            } else {
                FlagBranch(&record, 0);
            }
            break;
        }
        case FlagOp::IfCloseAndFacing:
            flagLocation = *reinterpret_cast<const Vector3 *>(&record->argument[0]);
            FlagBranch(&record,
                shenCheckCloseAndFacing(*reinterpret_cast<const Vector3 *>(&record->argument[0]), record->argument[3], state.playerPosition, state.playerYaw));
            break;
        case FlagOp::IfPlayerHeightNear: {
            const float height = reinterpret_cast<const float *>(record->argument)[0];
            if (height - 1.0f < state.playerPosition.y && height + 1.0f > state.playerPosition.y)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        }
        case FlagOp::IfCashAtLeast:
            if (GetPlayerStats(2) >= record->argument[0])
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfSegaCoins:
            if (GetPlayerStats(8))
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfQronStream:
            if (static_cast<std::int32_t>(GetQronNextStream()) == record->argument[0])
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfEntryPoint:
        case FlagOp::IfEntryPointSaveScene:
            if (g_entryPoint == record->argument[0]) {
                if (record->opcode == FlagOp::IfEntryPointSaveScene)
                    SetSceneInfos(g_actNumber, g_currentMap, record->argument[1]);
                FlagBranch(&record, 1);
            } else {
                FlagBranch(&record, 0);
            }
            break;
        case FlagOp::IfPlayerInRect: {
            Vector3 first, second;
            if (flagDebugFlags & 1U)
                flagDrawDebugBox(reinterpret_cast<const float *>(record->argument)[0], reinterpret_cast<const float *>(record->argument)[1],
                    reinterpret_cast<const float *>(record->argument)[2], reinterpret_cast<const float *>(record->argument)[3], state.playerPosition.y);
            first.x = reinterpret_cast<const float *>(record->argument)[0];
            first.z = reinterpret_cast<const float *>(record->argument)[1];
            second.x = reinterpret_cast<const float *>(record->argument)[2];
            second.z = reinterpret_cast<const float *>(record->argument)[3];
            flagLocation = first;
            if (ryoCheckInRangeForFlag(record)) {
                shenPlayerData.triggerRecord = record;
                FlagBranch(&record, 1);
            } else {
                FlagBranch(&record, 0);
            }
            break;
        }
        case FlagOp::IfShenmueFlag:
            if (GetShenmueFlag(record->argument[0]) == 1)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::SetStepFlags:
            EV_stepSetFlags(record->argument[0], record->argument[1]);
            FlagBranch(&record, 1);
            break;
        case FlagOp::SetLoadingPhaseBits:
            state.queueFilmTask = 1;
            FlagBranch(&record, 1);
            break;
        case FlagOp::IfCurfewSceneChange:
            if (shenIsCurfewSceneChange())
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::SetTimeOfDay: {
            DateTime dateTime;
            shenGetCurrentDateTime(dateTime);
            dateTime.hour = static_cast<std::int8_t>(record->argument[0]);
            dateTime.minute = static_cast<std::int8_t>(record->argument[1]);
            SetCurrentDateTimeTo(dateTime);
            FlagBranch(&record, 1);
            break;
        }
        case FlagOp::SceneChange:
        case FlagOp::SceneChangeFadeMode9:
        case FlagOp::SceneChangeFadeMode8:
            if (record->opcode == FlagOp::SceneChangeFadeMode9)
                scenSetTaskFlags(9);
            else if (record->opcode == FlagOp::SceneChangeFadeMode8)
                scenSetTaskFlags(8);
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(2);
            shenQueueChangeModuleAndScene(SystemModule_Game, g_actNumber, shen_name(static_cast<std::uint32_t>(record->argument[0])), record->argument[1]);
            state.result = 1;
            state.continueEvaluation = 0;
            break;
        case FlagOp::LoadAreaRestart:
        case FlagOp::LoadAreaRestartValue:
        case FlagOp::LoadAreaNext:
        case FlagOp::LoadAreaNextValue: {
            std::int32_t areaId[3];
            std::int32_t loaded;
            areaId[0] = record->argument[0];
            areaId[1] = record->argument[1];
            areaId[2] = 0;
            if (record->opcode == FlagOp::LoadAreaRestartValue || record->opcode == FlagOp::LoadAreaNextValue)
                loaded = shenLoadScene2(reinterpret_cast<const char *>(areaId), static_cast<std::uintptr_t>(record->argument[2]));
            else
                loaded = shenLoadScene2(reinterpret_cast<const char *>(areaId), reinterpret_cast<std::uintptr_t>(&record->argument[2]));
            if (!loaded)
                goto packFailed;
            if (static_cast<std::uint8_t>(static_cast<std::uint8_t>(record->opcode) - 7U) <= 1U) {
                flagRewindPlayerData(data);
                state.continueEvaluation = 0;
            } else {
                FlagBranch(&record, 1);
            }
            state.result = 1;
            break;
        }
        case FlagOp::ExecAreaSlotRestart:
        case FlagOp::ExecAreaSlotRestartValue:
        case FlagOp::ExecAreaSlotNext:
        case FlagOp::ExecAreaSlotNextValue:
            if (record->opcode == FlagOp::ExecAreaSlotRestartValue || record->opcode == FlagOp::ExecAreaSlotNextValue)
                shenLoadScene(static_cast<std::uint32_t>(record->argument[0]), static_cast<std::uintptr_t>(record->argument[1]));
            else
                shenLoadScene(static_cast<std::uint32_t>(record->argument[0]), reinterpret_cast<std::uintptr_t>(&record->argument[1]));
            if (static_cast<std::uint8_t>(static_cast<std::uint8_t>(record->opcode) - 0x20U) <= 1U) {
                flagRewindPlayerData(data);
                state.continueEvaluation = 0;
            } else {
                FlagBranch(&record, 1);
            }
            state.result = 1;
            break;
        case FlagOp::LoadPackRestart:
        case FlagOp::LoadPackRestartValue:
        case FlagOp::LoadEventPackRestartValue:
        case FlagOp::DeferEventPackRestartValue:
        case FlagOp::LoadPackNext:
        case FlagOp::LoadPackNextAlt:
        case FlagOp::DeferEventPackNext:
        case FlagOp::DeferEventPackNextValue: {
            std::int32_t areaId[3];
            PakGlobalData *pack;
            const char *directory;
            areaId[0] = record->argument[1];
            areaId[1] = record->argument[2];
            areaId[2] = 0;
            SetSystemManageSource(reinterpret_cast<const char *>(areaId));
            if (static_cast<std::uint8_t>(static_cast<std::uint8_t>(record->opcode) - 0x2cU) <= 1U || record->opcode == FlagOp::DeferEventPackNext ||
                record->opcode == FlagOp::DeferEventPackNextValue) {
                npcReleaseUnusedStreams();
                if (record->argument[0])
                    directory = Filepath_Gen(FilepathGenMode::PackDirectory);
                else
                    directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
                pack = RequestMapPackage(directory, reinterpret_cast<const char *>(areaId), eventExecutionRegions[record->argument[3]] - 0x20U);
                if (pack)
                    shenLoadEventFrom(reinterpret_cast<const char *>(areaId), static_cast<EventFunctionType>(record->argument[3]));
            } else {
                flagResetNpcLoad();
                if (record->argument[0])
                    directory = Filepath_Gen(FilepathGenMode::PackDirectory);
                else
                    directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
                pack = RequestMapPackage(directory, reinterpret_cast<const char *>(areaId));
            }
            if (!pack)
                goto packFailed;
            if (static_cast<std::uint8_t>(static_cast<std::uint8_t>(record->opcode) - 0x18U) <= 1U || record->opcode == FlagOp::LoadPackNext ||
                record->opcode == FlagOp::LoadPackNextAlt) {
                pakLoadFlush();
                if (record->argument[3]) {
                    shenMoveToFreeQuest();
                    if (record->opcode == FlagOp::LoadPackRestartValue)
                        ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(record->argument[3]), static_cast<std::uintptr_t>(record->argument[4]));
                    else
                        ExecuteCurrentAreaMainFunction(
                            static_cast<EventFunctionType>(record->argument[3]), reinterpret_cast<std::uintptr_t>(&record->argument[4]));
                    if (static_cast<std::uint8_t>(static_cast<std::uint8_t>(record->opcode) - 0x18U) <= 1U) {
                        flagRewindPlayerData(data);
                        state.continueEvaluation = 0;
                    } else {
                        FlagBranch(&record, 1);
                    }
                    state.result = 1;
                }
            } else {
                EV_NPC_NewLoadSw(0);
                LockFlagPlayerControl(1);
                shenPlayerData.flags |= SPF_PackLoadPending;
                shenPlayerData.pendingPack = pack;
                shenPlayerData.pendingEventType = record->argument[3];
                shenPlayerData.record = record;
                state.result = 1;
                state.continueEvaluation = 0;
            }
            state.result = 1;
            break;
        packFailed:
            data.deferredRecord = 0;
            shenPlayerData.record = record;
            state.continueEvaluation = 0;
            break;
        }
        case FlagOp::WarmStartNpcsRelease:
        case FlagOp::WarmStartNpcs:
        case FlagOp::WarmStartHumanNpcs: {
            EV_NPC_NewLoadSw(0);
            const std::int32_t length = FlagRecordLength(record);
            for (std::int32_t index = 1; index < length; ++index) {
                if (record->opcode == FlagOp::WarmStartHumanNpcs)
                    PrepareNpcRouteReload(shen_name(reinterpret_cast<std::uint32_t *>(record)[index]));
                else
                    EV_CheckWarmStartNpcRoute(shen_name(reinterpret_cast<std::uint32_t *>(record)[index]));
            }
            if (record->opcode == FlagOp::WarmStartNpcsRelease)
                EV_ResetFreeTalkFlags();
            FlagBranch(&record, 1);
            break;
        }
        case FlagOp::UnlockPartyCharas: {
            const std::int32_t length = FlagRecordLength(record);
            for (std::int32_t index = 1; index < length; ++index) {
                if (FindPTYCForChara(shen_name(reinterpret_cast<std::uint32_t *>(record)[index])))
                    ptycCharaUnlock(shen_name(reinterpret_cast<std::uint32_t *>(record)[index]));
            }
            FlagBranch(&record, 1);
            break;
        }
        case FlagOp::WaitNpcRelease:
            LockFlagPlayerControl(1);
            shenPlayerData.flags |= SPF_NpcReleasePending;
            FlagBranch(&record, 1);
            shenPlayerData.record = record;
            state.result = 1;
            state.continueEvaluation = 0;
            break;
        case FlagOp::IfCharaSector: {
            const FlagSector *const sector = &flagSectors[record->argument[1]];
            if (0.0f > sector->radius) {
                FlagBranch(&record, 0);
                break;
            }
            FlagBranch(&record, DoAngleCheck(*sector, shen_name(static_cast<std::uint32_t>(record->argument[0])), state.playerPosition, state.playerYaw));
            break;
        }
        case FlagOp::IfCharaSectorOrSearch: {
            const FlagSector *const sector = &flagSectors[record->argument[2]];
            flagSetLocationToChara(shen_name(static_cast<std::uint32_t>(record->argument[0])));
            if (state.searchTarget) {
                if (state.searchTarget == record->argument[1])
                    FlagBranch(&record, 1);
                else
                    FlagBranch(&record, 0);
                break;
            }
            FlagBranch(&record, DoAngleCheck(*sector, shen_name(static_cast<std::uint32_t>(record->argument[0])), state.playerPosition, state.playerYaw));
            break;
        }
        case FlagOp::IfCharaPointOnScreen: {
            flagSetLocationToChara(shen_name(static_cast<std::uint32_t>(record->argument[0])));
            if (state.searchTarget) {
                if (state.searchTarget == record->argument[1])
                    FlagBranch(&record, 1);
                else
                    FlagBranch(&record, 0);
                break;
            }
            std::int32_t condition = 0;
            Vector3 point;
            if (flagTransformCharaOffset(
                    shen_name(static_cast<std::uint32_t>(record->argument[0])), *reinterpret_cast<const Vector3 *>(&record->argument[2]), point))
                condition = IsWithinLocationScreenSpace(point.x, point.y, point.z);
            FlagBranch(&record, condition);
            break;
        }
        case FlagOp::IfPointOnScreenOrSearch:
            flagLocation = *reinterpret_cast<const Vector3 *>(&record->argument[0]);
            if (state.searchTarget) {
                if (state.searchTarget == record->argument[4])
                    FlagBranch(&record, 1);
                else
                    FlagBranch(&record, 0);
                break;
            }
            FlagBranch(&record,
                IsWithinLocationScreenSpace(reinterpret_cast<const float *>(record->argument)[0], reinterpret_cast<const float *>(record->argument)[1],
                    reinterpret_cast<const float *>(record->argument)[2]));
            break;
        case FlagOp::IfEntityEnabledOnScreen: {
            SearchTarget *const target = shenFindFlagTarget(shen_name(static_cast<std::uint32_t>(record->argument[0])));
            if (!target || (target->flags & 2U)) {
                FlagBranch(&record, 0);
                break;
            }
            flagLocation = target->position;
            if (state.searchTarget) {
                if (state.searchTarget == record->argument[0])
                    FlagBranch(&record, 1);
                else
                    FlagBranch(&record, 0);
                break;
            }
            FlagBranch(&record, IsWithinLocationScreenSpace(target->position.x, target->position.y, target->position.z));
            break;
        }
        case FlagOp::IfScenarioRecord:
            flagSetLocationToChara(shen_name(static_cast<std::uint32_t>(record->argument[0])));
            if (static_cast<std::int32_t>(GetActiveScenarioCharacter()) == record->argument[0])
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfItemOwned:
            if (GetItemState(static_cast<std::uint32_t>(record->argument[0])))
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfCharaWithinDistance: {
            Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask,
                shen_name(static_cast<std::uint32_t>(record->argument[0])));
            if (!character) {
                FlagBranch(&record, 0);
                break;
            }
            Vector3 delta;
            GetCharacterWorldPosition(character, &delta);
            flagLocation = delta;
            delta.x -= state.playerPosition.x;
            delta.y -= state.playerPosition.y;
            delta.z -= state.playerPosition.z;
            if (reinterpret_cast<const float *>(record->argument)[1] > Vector3::CalcMagnitude(delta))
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        }
        case FlagOp::IfInternalFlagEquals:
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, static_cast<std::int16_t>(record->argument[0])) == record->argument[1])
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::SetInternalFlag:
            EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, static_cast<std::int16_t>(record->argument[0]), record->argument[1]);
            FlagBranch(&record, 1);
            break;
        case FlagOp::IfEvaluationMode0:
            if (mode == 0)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfEvaluationMode1:
            if (mode == 1)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfEvaluationMode2:
            if (mode == 2)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfEvaluationMode3:
            if (mode == 3)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfQueryMode2:
            if (mode == 2 && record->argument[0] == shenPlayerData.queryId && record->argument[1] == shenPlayerData.queryArgument)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfQueryMode3:
            if (mode == 3 && record->argument[0] == shenPlayerData.queryId && record->argument[1] == shenPlayerData.queryArgument)
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfActionButton:
        case FlagOp::IfNamedActionButton:
        case FlagOp::IfActionButtonOrSearch: {
            shen_name button;
            std::int32_t named, bindings, nearest;
            if (record->opcode == FlagOp::IfActionButtonOrSearch && state.searchTarget) {
                FlagBranch(&record, 1);
                break;
            }
            if (record->opcode == FlagOp::IfNamedActionButton)
                button = shen_name(static_cast<std::uint32_t>(record->argument[0]));
            else
                button = SHEN_NAME(BTEV);
            named = record->opcode == FlagOp::IfNamedActionButton;
            bindings = static_cast<std::int32_t>(GetMediatedButtonBindings(button));
            nearest = flagRegisterCandidate(record);
            if ((state.searchTarget || nearest) && bindings && btnCheck(button)) {
                FlagBranch(&record, 1);
                break;
            }
            if (!state.actionButtonRegistered) {
                if (record->opcode == FlagOp::IfNamedActionButton)
                    GetBattleDataConfig(0, button);
                else
                    RegisterButtonMediation(button, &flagLocation, 0x40004U);
                state.actionButtonRegistered = 1;
            }
            if (FlagRecordLength(record) >= named + 2 && nearest)
                state.helpTopic = record->argument[named];
            FlagBranch(&record, 0);
            break;
        }
        case FlagOp::IfSearchTarget:
            if (state.searchTarget && state.searchTarget == record->argument[0]) {
                RetrieveSearchFlagFromPtr(flagLocation);
                FlagBranch(&record, 1);
            } else {
                FlagBranch(&record, 0);
            }
            break;
        case FlagOp::IfRyoNotBusy:
            if (ryoNotBusy())
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfPlayerIdle:
            if (shenCheckIsBusy())
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfPlayerNotInMenu:
            if (shenGetIsPlayerInMenu())
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::IfAuthorNotLoaded:
            if (!EV_authSceneCtrl(-1))
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::ReturnToFreeQuest:
            shenMoveToFreeQuest();
            shenToggleAreaFlags(0);
            SetPlayerControlEnabled(1);
            FlagBranch(&record, 1);
            break;
        case FlagOp::PlayPlayerMotionF003: {
            MotionWork *const motionWork = charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(0);
            mtwkSetRequestedRootMode(motionWork, 1);
            mtwkSetExtraFlag(motionWork, 15, 0);
            mtwkRequestMotion(motionWork, 0xf003, LowerBody);
            mtwkSetFrameTime(motionWork, 16, LowerBody);
            FlagBranch(&record, 1);
            state.result = 1;
            break;
        }
        case FlagOp::LockPlayerControlMode3:
            LockFlagPlayerControl(2);
            FlagBranch(&record, 1);
            state.result = 1;
            break;
        case FlagOp::LockPlayerControl:
            LockFlagPlayerControl(1);
            FlagBranch(&record, 1);
            state.result = 1;
            break;
        case FlagOp::IfPlayerControlEnabled:
            if (IsPlayerControlEnabled())
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        case FlagOp::CallContextRecords: {
            const std::int32_t context = record->argument[0];
            shenPlayerData.returnRecords[shenPlayerData.callDepth++] = reinterpret_cast<FlagRecord *>(&record->argument[1]);
            record = reinterpret_cast<FlagRecord *>(
                reinterpret_cast<std::int32_t *>(g_flagDescriptorData) + reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->contextTable);
            record =
                reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + reinterpret_cast<FlagContextEntry *>(record)[context].recordList);
            break;
        }
        case FlagOp::FadeOutSoundAndHold:
        case FlagOp::FadeOutAndHold: {
            shenPlayerData.fadeHoldFrames = 30;
            shenPlayerData.flags |= SPF_FadeHold;
            shenPlayerData.record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + FlagRecordLength(record));
            LockFlagPlayerControl(1);
            const ScreenFilmColourRequest request = {{0, 0, 0, 0}, {255, 0, 0, 0}, 30};
            WaitForNextTask(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask, request, 0);
            if (record->opcode == FlagOp::FadeOutSoundAndHold)
                spmFadeOut(0);
            state.result = 1;
            state.continueEvaluation = 0;
            break;
        }
        case FlagOp::TurnPlayerAndWait:
            flagTurnPlayerAndWait(&state.playerPosition,
                static_cast<std::uint16_t>(static_cast<std::int32_t>(reinterpret_cast<const float *>(record->argument)[0] * 8192.0f / 45.0f)));
            shenPlayerData.record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + FlagRecordLength(record));
            state.result = 1;
            state.continueEvaluation = 0;
            break;
        case FlagOp::ShowEventMessage:
            shenPlayerData.messageFrames = 60;
            shenPlayerData.flags |= SPF_EventMessageShown;
            shenPlayerData.record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + FlagRecordLength(record));
            SCEN_ShowEventMessage(
                static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, 1, reinterpret_cast<const char *>(&record->argument[0]));
            state.continueEvaluation = 0;
            break;
        case FlagOp::PlayVoice: {
            std::int32_t bank[2];
            std::int32_t voiceCode[3];
            bank[0] = record->argument[0];
            bank[1] = 0;
            voiceCode[0] = record->argument[0];
            voiceCode[1] = record->argument[1];
            voiceCode[2] = 0;
            if (!EV_FreeAuthorPak(1, reinterpret_cast<const char *>(bank))) {
                FlagBranch(&record, 1);
                break;
            }
            shenPlayerData.voice = EV_PlayDialogue(reinterpret_cast<const char *>(voiceCode));
            shenPlayerData.flags |= SPF_VoicePending;
            shenPlayerData.record = record;
            state.continueEvaluation = 0;
            break;
        }
        case FlagOp::IfPlayerNotRunning:
            if (plymIsInterpolationActive())
                FlagBranch(&record, 0);
            else
                FlagBranch(&record, 1);
            break;
        case FlagOp::WaitForIdleEvaluations:
            if (shenPlayerData.idleEvaluations <= 2 || data.deferredRecord == record) {
                shenPlayerData.record = shenPlayerData.firstRecord;
                shenPlayerData.callDepth = 0;
                state.continueEvaluation = 0;
            } else {
                data.deferredRecord = record;
                FlagBranch(&record, 1);
            }
            break;
        case FlagOp::IfPointOnScreen:
            if (state.searchTarget) {
                FlagBranch(&record, 1);
                break;
            }
            flagLocation = *reinterpret_cast<const Vector3 *>(&record->argument[0]);
            if (IsWithinLocationScreenSpace(reinterpret_cast<const float *>(record->argument)[0], reinterpret_cast<const float *>(record->argument)[1],
                    reinterpret_cast<const float *>(record->argument)[2])) {
                shenPlayerData.triggerRecord = record;
                FlagBranch(&record, 1);
            } else {
                FlagBranch(&record, 0);
            }
            break;
        case FlagOp::IfCharaVisible: {
            if (state.searchTarget) {
                FlagBranch(&record, 1);
                break;
            }
            Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask,
                shen_name(static_cast<std::uint32_t>(record->argument[1])));
            if (!character) {
                FlagBranch(&record, 0);
                break;
            }
            Vector3 bone;
            charaGetMotWorldPos(character, 5, &bone);
            if (flddCheckCharaIntersection(bone, shen_name(static_cast<std::uint32_t>(record->argument[0])), record->argument[2],
                    reinterpret_cast<const float *>(record->argument)[3]))
                FlagBranch(&record, 1);
            else
                FlagBranch(&record, 0);
            break;
        }
        case FlagOp::AlwaysFalse:
            FlagBranch(&record, 0);
            break;
        default:
            record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::int32_t *>(record) + 1);
            break;
        }
    } while (state.continueEvaluation);
    if (ryoNotBusy()) {
        if (state.helpTopic) {
            if (!(shenPlayerData.flags & SPF_HelpIconShown)) {
                EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(state.helpTopic), 0);
                shenPlayerData.flags |= SPF_HelpIconShown;
            }
        } else if (shenPlayerData.flags & SPF_HelpIconShown) {
            EV_EventSetUpHelpIcon_0(0, 0);
            shenPlayerData.flags &= ~SPF_HelpIconShown;
        }
    }
    if (mode == 1 && state.queueFilmTask) {
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(FlagFilmTask, 15, 11, SHEN_NAME(FLAG));
        HLib::SetupTimedTask(task, 2);
    }
    return state.result;
}

namespace {

void FlagFilmTask(void *) {
    ScreenFilmColourRequest request;
    request.start[0] = 255;
    request.start[1] = 0;
    request.start[2] = 0;
    request.start[3] = 0;
    request.target[0] = 255;
    request.target[1] = 0;
    request.target[2] = 0;
    request.target[3] = 0;
    request.durationFrames = 1;
    WaitForNextTask(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask, request, 0);
    HLib::DestroyCurrentTask();
}

bool IsWithinLocationScreenSpace(float pointX, float pointY, float pointZ) {
    bool result = false;
    Vector3 point;
    point.x = pointX;
    point.y = pointY;
    point.z = pointZ;
    flagLocation = point;
    Vector3 bone;
    charaGetMotWorldPos(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 5, &bone);
    const float height = point.y - bone.y;
    if (height < 0.0f ? 2.0f > -height : 2.0f > height) {
        const float x = point.x - bone.x, z = point.z - bone.z;
        if (9.0f > x * x + z * z) {
            if (static_cast<std::uint16_t>(InverseFixedAtan2(x, z) -
                    shenGetRotYFromNode(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo) + 0x4000) <= 0x8000U) {
                Vector3 view;
                Matrix4x4::TransformPoint(point, view);
                if (-0.1f > view.z) {
                    matrix_stack::UpdateWorldToScreenMatrix(640.0f, 480.0f);
                    matrix_stack::ProjectWorldPointToScreen(&point, reinterpret_cast<Vector2 *>(&view));
                    if (view.x > 0.0f && 640.0f > view.x && view.y > 0.0f && 480.0f > view.y) {
                        flddToggleFlag(0x1000);
                        if (!TestFieldWorldSegment(&point, &bone))
                            result = true;
                    }
                }
            }
        }
    }
    return result;
}

std::int32_t flddCheckCharaIntersection(const Vector3 &point, shen_name name, std::int32_t halfAngle, float radius) {
    Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
    if (!character)
        return false;
    flagLocation = point;
    Vector3 bone;
    charaGetMotWorldPos(character, 5, &bone);
    const float height = point.y - bone.y;
    if (height < 0.0f ? 2.0f > -height : 2.0f > height) {
        const float x = point.x - bone.x, z = point.z - bone.z;
        if (!(x * x + z * z < radius * radius))
            return false;
        if (static_cast<std::uint16_t>(2 * halfAngle) < static_cast<std::uint16_t>(InverseFixedAtan2(x, z) - shenGetRotYFromNode(character) + halfAngle))
            return false;
        flddToggleFlag(0x1000);
        if (TestFieldWorldSegment(&point, &bone))
            return false;
        return true;
    }
    return false;
}

std::int32_t DoAngleCheck(const FlagSector &sector, shen_name name, const Vector3 &playerPosition, std::int32_t playerYaw) {
    Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
    if (!character)
        return false;
    const std::int32_t yaw = GetCharacterWorldYaw(character);
    Vector3 point;
    GetCharacterWorldPosition(character, &point);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&point);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::TransformPoint(sector.offset, point);
    PopCurrentMatrix(1);
    flagLocation = point;
    if (ComputeAngle(playerPosition, sector.radius, playerYaw, sector.playerHalfAngle, point) &&
        ComputeAngle(point, sector.radius, GetCharacterWorldYaw(character) + sector.yawOffset, sector.charaHalfAngle, playerPosition) &&
        point.y - sector.heightTolerance < playerPosition.y && point.y + sector.heightTolerance > playerPosition.y)
        return 1;
    return 0;
}

void flagSetLocationToChara(shen_name name) {
    Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
    if (character)
        GetCharacterWorldPosition(character, &flagLocation);
}

std::int32_t shenCheckCloseAndFacing(const Vector3 &first, std::int32_t firstYaw, const Vector3 &second, std::int32_t secondYaw) {
    flagLocation = first;
    if (ComputeAngle(second, 3.0f, secondYaw, 0x3000, first) && ComputeAngle(first, 3.0f, firstYaw, 0x3e38, second) && first.y - 1.0f < second.y &&
        first.y + 1.0f > second.y)
        return 1;
    return 0;
}

} // namespace

// @verified - d
std::int32_t ryoIsCurrentScenarioFlagSatisfied() {
    FlagRecord *const record = shenPlayerData.triggerRecord;
    switch (record->opcode) {
    case FlagOp::IfPlayerInRect:
        return ryoCheckInRangeForFlag(record);
    case FlagOp::IfPlayerNearPoint:
        return flagIsPlayerNearPoint(record);
    }
    return true;
}

namespace {

std::int32_t flagIsPlayerNearPoint(const FlagRecord *record) {
    Vector3 position, point;
    Character *player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (player)
        GetCharacterWorldPosition(player, &position);
    else
        position.x = position.y = position.z = -1000.0f;
    point.x = *reinterpret_cast<const float *>(&record->argument[0]);
    point.y = position.y;
    point.z = *reinterpret_cast<const float *>(&record->argument[1]);
    if (flagDebugFlags & 1U)
        flagDrawDebugCylinder(point, *reinterpret_cast<const float *>(&record->argument[2]));
    std::int32_t result;
    if (record->argument[4] == 0x8000) {
        position.x -= point.x;
        position.z -= point.z;
        if (position.x * position.x + position.z * position.z >
            *reinterpret_cast<const float *>(&record->argument[2]) * *reinterpret_cast<const float *>(&record->argument[2]))
            return 0;
        return 1;
    } else {
        result = ComputeAngle(position, *reinterpret_cast<const float *>(&record->argument[2]), record->argument[3], record->argument[4], point);
    }
    return result;
}

bool ryoCheckInRangeForFlag(const FlagRecord *record) {
    if (flagDebugFlags & 1U)
        flagDrawDebugBox(reinterpret_cast<const float *>(record->argument)[0], reinterpret_cast<const float *>(record->argument)[1],
            reinterpret_cast<const float *>(record->argument)[2], reinterpret_cast<const float *>(record->argument)[3], 50.0f);
    Vector3 first;
    Vector3 second;
    first.x = reinterpret_cast<const float *>(record->argument)[0];
    first.z = reinterpret_cast<const float *>(record->argument)[1];
    second.x = reinterpret_cast<const float *>(record->argument)[2];
    second.z = reinterpret_cast<const float *>(record->argument)[3];
    return TestCharaPositionWithinRange(shen_name(SHEN_NAME(RYO_)), &first, &second, shen_name());
}

} // namespace

// @verified - d
SearchTarget *shenFindFlagTarget(shen_name name) {
    SearchTarget *target = shenPlayerData.targets;
    if (!target)
        return 0;
    for (; target->name; ++target)
        if (target->name == name)
            return target;
    return 0;
}

// @verified - d
void shenSetFreeQuestInactive() {
    shenPlayerData.freeQuestActive = 0;
}

namespace {

std::int32_t LockFlagPlayerControl(std::int32_t mode) {
    if (!IsPlayerControlEnabled())
        return 0;
    SetPlayerControlEnabled(0);
    switch (mode) {
    case 1:
        shenToggleAreaFlags(1);
        break;
    case 2:
        shenToggleAreaFlags(3);
        break;
    }
    return 1;
}

} // namespace

// @verified - d
std::uint32_t shenIsCurfewSceneChange() {
    return shenPlayerData.flags & SPF_CurfewSceneChange;
}

namespace {

void shenMoveToFreeQuest() {
    shenToggleAreaFlags(0);
    srchReset();
    shenPlayerData.freeQuestActive = 1;
}

// @verified - d
std::uint32_t FlagBranch(FlagRecord **record, std::int32_t condition) {
    std::uint32_t count;
    if (*reinterpret_cast<std::int32_t *>(*record) < 0)
        condition ^= 1;
    if (condition)
        count = (*reinterpret_cast<std::uint32_t *>(*record) >> 8) & 0xffU;
    else {
        std::uint32_t packed = *reinterpret_cast<std::uint32_t *>(*record);
        packed <<= 1;
        count = packed >> 17;
    }
    *record = reinterpret_cast<FlagRecord *>(reinterpret_cast<std::uint32_t *>(*record) + count);
    return count;
}

std::int32_t *FlagContextTable() {
    return reinterpret_cast<std::int32_t *>(g_flagDescriptorData) + reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->contextTable;
}

FlagRecord *FlagRecordAt(std::int32_t *table, std::int32_t offset) {
    return reinterpret_cast<FlagRecord *>(table + offset);
}

MapControlData *FlagControl() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
}

Character *FlagPlayer() {
    return FlagControl()->ryo;
}

Character *FlagChara(shen_name name) {
    return scenFindChara(FlagControl()->scenarioTask, name);
}

Vector3 FlagPosition(Character *character) {
    Vector3 result = {-1000.0f, -1000.0f, -1000.0f};
    if (character)
        GetCharacterWorldPosition(character, &result);
    return result;
}

Vector3 FlagCharaBone(Character *character) {
    Vector3 position;
    charaGetMotWorldPos(character, 5, &position);
    return position;
}

std::int32_t shenLoadScene2(const char *area, std::uintptr_t argument) {
    if (!LoadAreaFunc_Wrapper_0(area, 0))
        return 0;
    SetSystemManageSource(area);
    shenMoveToFreeQuest();
    return ExecuteAreaMainFunction(area, argument);
}

void shenLoadScene(std::uint32_t slot, std::uintptr_t argument) {
    const char *const name = GetAreaSlotName(slot);
    std::strlen(name);
    SetSystemManageSource(name);
    shenMoveToFreeQuest();
    ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(slot), argument);
}

void flagResetNpcLoad() {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
}

} // namespace

static std::int32_t FlagRecordLength(const FlagRecord *record) {
    return (*reinterpret_cast<const std::uint32_t *>(record) >> 8) & 0xffU;
}

void SwapFlagWords(std::uint16_t *first, std::uint16_t *second) {
    const std::uint16_t value = *first;
    *first = *second;
    *second = value;
}

std::int32_t flagWorkSortMode = 0;

// @verified - d
void ResetFlagWorkFromDescriptor() {
    const std::int32_t count = static_cast<std::int32_t>(GetFlagDescriptorValue(FlagWorkCount));
    std::uint16_t *destination = g_flagWork;
    std::int32_t index = 0, other;
    switch (flagWorkSortMode) {
    case 0:
        for (; index < count; ++index)
            *destination++ = static_cast<std::uint16_t>(GetInitialFlagValue(static_cast<std::uint32_t>(index)));
        break;
    case 1:
        for (; index < count - 1; ++index, ++destination)
            for (other = index + 1; other < count; ++other)
                if (g_flagWork[other] < *destination)
                    SwapFlagWords(&g_flagWork[other], destination);
        break;
    case 2:
        for (index = 0; index < count - 1; ++index, ++destination)
            for (other = index + 1; other < count; ++other)
                if (std::strcmp(GetFlagName(*destination), GetFlagName(g_flagWork[other])) > 0)
                    SwapFlagWords(&g_flagWork[other], destination);
        break;
    }
}

void AssignEventPrintPointers() {
}

void UpdateDebugMenuSelection(std::int32_t column, std::int32_t line, std::int32_t count, std::int32_t *selection, const char **items) {
    for (std::int32_t index = 0; index < count; ++index) {
        SetDebugScreenTextPosition(column, line + index);
        SetDebugScreenTextColour(index == *selection ? 0xffffffffU : 0x806fffffU);
        shenmue_s2_event_DebugLogToScreen(items[index]);
    }
    if ((g_inputBuffer[3].repeatedPresses & 0x1000) && *selection > 0)
        --*selection;
    if ((g_inputBuffer[3].repeatedPresses & 0x2000) && *selection < count - 1)
        ++*selection;
}

void PrintFlagHundredths(std::int32_t value) {
    if (value != value / 100 * 100)
        shenmue_s2_event_DebugLogToScreen("%7.4f", static_cast<float>(value) / 10000.0f);
    else
        shenmue_s2_event_DebugLogToScreen("%5.2f", static_cast<float>(value) / 10000.0f);
}

// @verified - d
void SetSystemManageSource(const char *source) {
    std::strcpy(systemManageSource, source);
    systemManageFrames = 0;
}

// @verified - d
void CtrlSystemManage_Callback(void *) {
    if (systemManageSource[0]) {
        systemManageFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(systemManageFrames) + 1U);
        if (systemManageFrames > 30 && IsPlayerControlEnabled())
            systemManageSource[0] = '\0';
    } else {
        systemManageFrames = 0;
    }
    if (g_developerDisplayEnabled && systemManageSource[0]) {
        SetDebugScreenTextPosition(24, 3);
        const float seconds = static_cast<float>(systemManageFrames) / 30.0f;
        shenmue_s2_event_DebugLogToScreen("%8s-%7.2f", systemManageSource, seconds);
    }
}

// @verified - d
void initCtrlSystemManage() {
    HLib::EnqueueTaskWithoutParameter(CtrlSystemManage_Callback, 14, 2, static_cast<std::int32_t>(TASK_SYSM));
    systemManageSource[0] = '\0';
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void flagDrawDebugScreenRect(std::int16_t x, std::int16_t y, std::int16_t width, std::int16_t height, std::uint32_t colour) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.secondaryColour = colour & 0xffffff;
    packet.colour = colour & 0xff000000;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), x, y, width, height, 1.0f, 1.0f, 0);
    packet.uv[0] = 0.24f;
    packet.uv[1] = 0.26f;
    packet.uv[2] = 0.24f;
    packet.uv[3] = 0.26f;
    packet.quad.flags |= 0x400;
    DrawLegacySpritePacket(packet, 0, 1.0f);
}
#endif

namespace {

bool flagTransformCharaOffset(shen_name name, const Vector3 &offset, Vector3 &point) {
    Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
    if (character) {
        Vector3 position;
        GetCharacterWorldPosition(character, &position);
        const std::int32_t yaw = GetCharacterWorldYaw(character);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&position);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::TransformPoint(offset, point);
        PopCurrentMatrix(1);
        return true;
    }
    return false;
}

} // namespace

} // namespace s2
} // namespace shenmue

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_shenUpdatePlayerDataFlags(std::int32_t active) {
    return shenmue::s2::shenUpdatePlayerDataFlags(active);
}

extern "C" {
void __attribute__((renesas)) shenmue_s2_event_EV_stepSetFlags(std::int32_t flag, std::int32_t mode) {
    shenmue::s2::EV_stepSetFlags(flag, mode);
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_shenGetMinutesUntilNextFrameMinute() {
    return shenmue::s2::shenGetMinutesUntilNextFrameMinute();
}
std::int16_t __attribute__((renesas)) shenmue_s2_event_shenGetSleepScheduleFlag() {
    return shenmue::s2::shenGetSleepScheduleFlag();
}
void __attribute__((renesas)) shenmue_s2_event_shenSetFreeQuestInactive() {
    shenmue::s2::shenSetFreeQuestInactive();
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_RequestScenarioPlayerInterpolation(std::uint32_t name, std::int16_t minute) {
    return shenmue::s2::shenScheduleFlagCallback(shenmue::shen_name(name), minute);
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_npcSetCycleTimeLimitExpiry(std::uint32_t name, std::int16_t timeLimit) {
    return shenmue::s2::npcSetCycleTimeLimitExpiry(shenmue::shen_name(name), timeLimit);
}
}
#endif
