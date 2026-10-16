#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;
struct PackModel;

enum class TMEXUtilityMode : std::uint32_t {
    OpenScripted = 0,
    OpenTimed = 1,
    OpenSleep = 2,
    CloseSetResult0 = 3,
    CloseSetResult1 = 4,
};

struct TMEXUtilitiesParameters {
    std::uint32_t operation;
    std::uint32_t mode;
};

static_assert(sizeof(TMEXUtilitiesParameters) == 8);
static_assert(offsetof(TMEXUtilitiesParameters, operation) == 0);
static_assert(offsetof(TMEXUtilitiesParameters, mode) == 4);

enum class TMEXLifecycleState : std::uint32_t {
    Inactive = 0,
    Starting = 1,
    Active = 4,
    DestroyRequested = 6,
};



struct TMEXRuntimeState {
    std::int32_t hourHandAngle = 0;
    std::int32_t minuteHandAngle = 0;
    std::int32_t auxiliaryHourHandAngle = 0;
    std::int32_t auxiliaryMinuteHandAngle = 0;
    std::uint16_t inputFlags = 0;
    std::uint16_t maskedInputFlags = 0;
    std::uint8_t showDateWindow = 0;
    std::uint8_t showAuxiliaryHands = 0;
    std::uint8_t renderRequested = 0;
    std::uint8_t overlayReady = 0;
    std::uint32_t handUpdateCount = 0;
    std::uint32_t renderCount = 0;
    PackModel *watchModelOwner = nullptr;
};






extern std::uint8_t g_TMEX_Flag;
extern std::uint8_t g_tmex_val;
extern std::uint32_t g_TMEX_CurrentMode;
extern TMEXLifecycleState g_TMEX_lifecycleState;


extern std::uint8_t g_TMEX_State;
extern std::uint8_t g_TMEX_displayFlag;
extern std::uint32_t g_tmexRequestFrames;
extern std::uint32_t g_tmexTransitionFrames;
extern std::uint32_t g_tmexAuthorFrames;
extern std::uint8_t g_tmex_suppressKeepOrRevertPrompt;
extern std::uint8_t g_TMEX_forceShowWatchNode162;
extern Vector3 g_tmpCameraLookAtV3_2;
extern Vector3i g_TMEX_watchModelRotation;
extern void *WatchModelFilePtr;
extern std::uint32_t g_TMEX_auxHandsPackedTime;
extern Vector3 g_tmpCameraPosV3;
extern Vector3 g_tmpCameraLookAtV3_1;
extern std::int32_t g_dCameraPerspective;
extern std::int32_t g_TMEX_savedFieldOfView;
extern std::int32_t g_TMEX_savedAuthorState;
extern std::int32_t g_TMEX_savedTempGameState;
extern std::uint8_t g_TMEX_savedRyoVisibilityFlag;
extern std::uint8_t g_TMEX_savedRyoUnknown10Flag;
extern std::uint16_t g_TMEX_savedUiState;
extern std::uint8_t g_TMEX_savedSicnTaskActive;
extern std::uint16_t g_TMEX_savedCurrentSicn;
extern std::uint8_t g_TMEX_savedDefaultHelpTokenID;
extern std::uint8_t g_TMEX_savedDButtonID;

extern HLib::Task *g_tmexTask;

TMEXRuntimeState &TMEX_GetRuntimeState();
void TMEX_ResetForceCancelRequest();
bool TMEX_Process();
void TMEX_SetAuxiliaryTime(std::uint32_t packedTime, bool forceDateWindow);
std::uint32_t TMEXUtilities(std::uint32_t mode);
void Script_TMEXUtilities(ScenarioContext *context, void *rawParams);
void TMEX_Callback(HLib::TaskData *taskData);
void EnqueueTMEX();
void DestroyTMEX();

} // namespace shenmue::s1
