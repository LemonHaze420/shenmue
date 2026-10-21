#include "s1/task/time.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/date_time.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/timing.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/came.hpp"
#include "s1/task/cdtt.hpp"

#include "s1/task/film.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/sygh.hpp"

#include "s1/task/cycle_man.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/tmex.hpp"
#include "s1/task/wzog.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

void TIME_Callback_0(HLib::TaskData *taskData);

void TIME_Callback(HLib::TaskData *) {
    if (CurrentTask) {
        CurrentTask->mainTaskCallbackFnPtr = TIME_Callback_0;
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

DateComparisonResult CompareDates(const GameDateTime *first, const GameDateTime *second) {
    const auto field = [](std::uint8_t value) { return static_cast<std::int32_t>(static_cast<std::int8_t>(value)); };
    const std::int32_t firstDate = field(first->day) + ((field(first->month) + (field(first->year) << 6)) << 6);
    const std::int32_t secondDate = field(second->day) + ((field(second->month) + (field(second->year) << 6)) << 6);
    if (firstDate < secondDate) {
        return DateComparisonResult::lessThan;
    }
    if (firstDate > secondDate) {
        return DateComparisonResult::greaterThan;
    }

    const std::int32_t firstTime = field(first->seconds) + ((field(first->minutes) + (field(first->hours) << 6)) << 6);
    const std::int32_t secondTime = field(second->seconds) + ((field(second->minutes) + (field(second->hours) << 6)) << 6);
    if (firstTime < secondTime) {
        return DateComparisonResult::lessThan;
    }
    return firstTime > secondTime ? DateComparisonResult::greaterThan : DateComparisonResult::equal;
}

std::uint8_t g_timeValue = 0;
std::uint8_t g_freezeTime = 0;
std::uint8_t g_timeDisabled = 0;
std::uint8_t g_timeBlockedUpdate = 0;
std::uint8_t g_shouldTimeAdvance = 0;
GameDateTime g_maximumDateTime{};
std::uint8_t g_hasIncrementedYear = 0;
std::uint8_t g_hasIncrementedMonth = 0;
std::uint8_t g_hasIncrementedDay = 0;
std::uint8_t g_hasIncrementedMinutes = 0;
std::uint8_t g_hasIncrementedSeconds = 0;
std::uint8_t g_unusedTimeFlag3 = 0;
std::uint32_t g_helpIconFlags = 0;
std::uint8_t g_globalFlags[1]{};
std::int32_t g_debugForceShowAllItems = 0;
std::uint8_t g_shouldUpdateClockUi = 0;
GameDateTime g_currentDateTime{};
GameDateTime g_temporaryDateTime{};
std::uint16_t g_temporaryDateTimeWord = 0;
std::uint8_t g_unusedTimeBypass = 0;
float g_newCycleTime = 0.0f;
float g_currentDeltaTimeSim = 0.0f;
float g_timeMultiplier = 1.0f;
float g_cycleTimeMultiplier = 0.5f;
float g_stepSize = 0.0f;
std::uint8_t g_disableClockAndActionSelector = 0;
TimeClockUiGlobals g_timeClockUi{};



float g_secondHandAngle = 0.0f;
float g_minuteHandRadians = 0.0f;
float g_hourHandRadians = 0.0f;

extern std::int32_t g_busy_reset;
namespace {



constexpr std::int32_t kUiGameHudWatch = 0x22;
constexpr std::int32_t kUiPracticeHud = 5;
constexpr std::int32_t kUiSubMenuMoves = 16;

// Shenmue::g_updateRenderFrameMode == SHEN_FRAME_ADVANCE_HOLD_OR_SUBSTEP.
constexpr std::int32_t kFrameAdvanceHoldOrSubstep = 0;
constexpr std::int32_t kAuthorStateScripted = 0x09;
constexpr std::int32_t kAuthorStateEvaluateWorld = 0x0e;

constexpr double kWatchAngleScale = 10430.37835047046;
constexpr double kWatchTwoPi = 6.28318977355957;
constexpr float kWatchEpsilon = 1.1920929e-7f;
constexpr std::int32_t kWatchYawSnapThreshold = 0x888;
constexpr std::int32_t kWatchYawStep = 364;

std::int32_t AbsInt32(std::int32_t value) {
    return value < 0 ? -value : value;
}

MapControlData *CurrentMapControl() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
}

// ctrlGetCompassCamYaw: CTRL publishes the compass yaw in its trailing word.
std::int32_t CompassCamYaw(const MapControlData *control) {
    return control ? static_cast<std::int16_t>(control->direction) : 0;
}

std::int32_t CameraAccumulatedYaw() {
    return static_cast<std::int16_t>(g_fCamAccumYaw);
}

const FilmColourInterpolation *FilmInterpolation(const MapControlData *control) {
    HLib::Task *const filmTask = control ? control->filmTask : nullptr;
    if (!filmTask || !filmTask->task_data_ptr)
        return nullptr;
    return reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(filmTask->task_data_ptr) + 4);
}



bool CanUpdateWithoutBlockingScreens() {
    if (g_pSCNC && g_pSCNC->header.active)
        return false;
    if (g_movesScreenTransitionFlags)
        return false;
    if (LoadScreen.isVisible)
        return false;
    if (CDTT_IsLoadingUIActive())
        return false;
    return true;
}



void ResetWatchElement() {
    if (g_currUIState != -1 && g_currUIState != kUiGameHudWatch)
        return;
    UIBaseRuntime &watch = g_uiElements[static_cast<std::size_t>(kUiGameHudWatch)];
    if (watch.isVisible && watch.destroy) {
        if (watch.destroy() != 1 && g_busy_reset != 1)
            return;
        watch.isVisible = 0;
    }
    watch.isVisible = 0;
}

void PublishWatchYaw(TimeWatchTransform &watch, std::int32_t yaw) {
    watch.field_04 = static_cast<std::uint16_t>(yaw);
    watch.field_06 = static_cast<std::int16_t>(static_cast<std::uint32_t>(yaw) >> 16U);
}

std::int32_t CurrentWatchYaw(const TimeWatchTransform &watch) {
    return static_cast<std::int32_t>(
        static_cast<std::uint32_t>(watch.field_04) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>(watch.field_06)) << 16U));
}





std::int32_t UpdateWatchEligibility(TimeWatchTransform &watch) {
    const Vector3 cameraEye = g_CameraEyePos;
    Vector3 publishedEye = cameraEye;
    float x = cameraEye.x;
    float y = cameraEye.y;
    float z = cameraEye.z;
    auto holdFrames = static_cast<std::int32_t>(watch.field_0c);
    const std::int32_t frameCount = CurrentFrameCount;

    const std::int32_t playerOwnsFrame = (ShouldRun() || FLMW_IsWatchInteractionActive()) ? 1 : 0;

    std::int32_t cycleTimeActive = 0;
    if (const std::byte *const flag = g_pCycleManagerTimeFlag) {
        std::uint16_t value = 0;
        std::memcpy(&value, flag, sizeof(value));
        cycleTimeActive = value != 0xffffU ? 1 : 0;
    }

    const std::int32_t authorState = g_nAuthorState;
    if ((!playerOwnsFrame || authorState == kAuthorStateEvaluateWorld) && (authorState != 0 || !cycleTimeActive)) {
        if (holdFrames <= 0) {
            watch.field_00 = 0;
        } else {
            if (!watch.field_20) {
                const auto previousFrame = static_cast<std::int32_t>(watch.field_1c);
                const std::int32_t elapsed = frameCount - previousFrame >= 0 ? frameCount - previousFrame : previousFrame - frameCount;
                if (elapsed > 2 && static_cast<float>(holdFrames) > 22.5f)
                    holdFrames = 22;
            }
            const float previousX = watch.position.x;
            const float previousY = watch.position.y;
            if (cameraEye.x < kWatchEpsilon) {
                x = 0.0f;
                publishedEye.x = 0.0f;
            }
            if (cameraEye.y < kWatchEpsilon) {
                y = 0.0f;
                publishedEye.y = 0.0f;
            }
            if (cameraEye.z < kWatchEpsilon) {
                z = 0.0f;
                publishedEye.z = 0.0f;
            }
            if (authorState == kAuthorStateScripted || authorState == kAuthorStateEvaluateWorld || (g_pSCNC && g_pSCNC->header.active) ||
                std::fabs(x - previousX) > kWatchEpsilon || std::fabs(y - previousY) > kWatchEpsilon || std::fabs(z - watch.position.z) > kWatchEpsilon) {
                --holdFrames;
            }
        }
    } else if (static_cast<float>(holdFrames) >= 45.0f) {
        watch.field_00 = 1;
    } else {
        ++holdFrames;
    }

    const auto eligible = static_cast<std::int32_t>(watch.field_00);
    watch.position = publishedEye;
    watch.field_0c = static_cast<std::uint32_t>(holdFrames);
    watch.field_1c = static_cast<std::uint32_t>(frameCount);
    watch.field_20 = static_cast<std::uint32_t>(playerOwnsFrame);

    std::int32_t screenActive = 0;
    const UIBaseRuntime &practice = g_uiElements[static_cast<std::size_t>(kUiPracticeHud)];
    const UIBaseRuntime &moves = g_uiElements[static_cast<std::size_t>(kUiSubMenuMoves)];
    if (practice.isVisible || practice.isActive || SYGH_GetOverlayState().visible) {
        screenActive = 1;
    }
    if (moves.isVisible || moves.isActive || WZOG_IsViewActive() || g_movesScreenTransitionFlags) {
        screenActive = 1;
    }
    return screenActive | eligible | (shenGetCanUpdate() ? 1 : 0);
}

} // namespace

// shenGetCanUpdate: the watch/clock update gate shared with the HUD owners.
bool shenGetCanUpdate() {

    if (!GetInventoryItemCount(3))
        return false;
    if (g_disableClockAndActionSelector)
        return false;
    if (g_notebookHelpOverrideActive)
        return true;
    if (static_cast<std::uint8_t>(g_isPlayerGameStateActive - 7) > 1U)
        return true;
    if (UI_ICLISharedItemMenuIsActive())
        return true;

    if (!g_NumFramesProcessed) {
        const FilmColourInterpolation *const film = FilmInterpolation(CurrentMapControl());
        if (film && film->current[0] != 0 && film->remainingFrames == 0)
            return false;
    }
    return CanUpdateWithoutBlockingScreens();
}

void TIME_UpdateClockUI() {
    if (g_updateRenderFrameMode == kFrameAdvanceHoldOrSubstep)
        return;

    TimeWatchTransform &watch = g_timeClockUi.transform;

    if (!shenGetCanUpdate()) {


        const MapControlData *const control = CurrentMapControl();
        PublishWatchYaw(watch, CameraAccumulatedYaw() + CompassCamYaw(control));
        if (g_uiElements[static_cast<std::size_t>(kUiGameHudWatch)].isVisible)
            ResetWatchElement();
        return;
    }

    std::int32_t updatable = UpdateWatchEligibility(watch);
    std::int32_t smoothingDirection = 1;

    if (updatable) {
        // Render/Tick when the element is already built, otherwise
        // Initialize/PostInit, each bracketed by g_waitingForNextTick.
        d3t::ActivateElementByIdx(kUiGameHudWatch);
        if (g_timeClockUi.transition < 100)
            g_timeClockUi.transition += 8;
    } else {
        UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(kUiGameHudWatch)];
        if ((g_currUIState == -1 || g_currUIState == kUiGameHudWatch) &&
            (!element.isVisible || !element.destroy || element.destroy() == 1 || g_busy_reset == 1)) {
            element.isVisible = 0;
        }
        if (g_timeClockUi.transition > 0) {
            g_timeClockUi.transition -= 8;


            updatable = 1;
        }
    }

    const std::int32_t targetYaw = CameraAccumulatedYaw() + CompassCamYaw(CurrentMapControl());
    std::int32_t yaw = CurrentWatchYaw(watch);
    const auto delta = static_cast<std::int32_t>(static_cast<std::int16_t>(static_cast<std::uint16_t>(targetYaw) - static_cast<std::uint16_t>(yaw)));

    if (updatable && (watch.field_08 != 0 || AbsInt32(delta) >= kWatchYawSnapThreshold)) {
        if (delta < 0) {
            yaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(yaw) - static_cast<std::uint16_t>(kWatchYawStep));
            smoothingDirection = -1;
        } else if (delta > 0) {
            yaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(yaw) + static_cast<std::uint16_t>(kWatchYawStep));
        } else {
            smoothingDirection = 0;
        }
        if (watch.field_08 != 0 && watch.field_08 != smoothingDirection && AbsInt32(delta) < kWatchYawSnapThreshold) {
            yaw = targetYaw;
            smoothingDirection = 0;
        }
        watch.field_08 = smoothingDirection;
    } else {
        yaw = targetYaw;
    }

    if (updatable)
        g_timeClockUi.cameraYaw = -yaw;
    PublishWatchYaw(watch, yaw);

    g_secondHandAngle = static_cast<float>(kWatchTwoPi - static_cast<double>(static_cast<float>(yaw)) / kWatchAngleScale);
    g_minuteHandRadians = static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(g_timeClockUi.minuteHand))) / kWatchAngleScale);
    g_hourHandRadians = static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(g_timeClockUi.hourHand))) / kWatchAngleScale);
}

void TIME_SetupGVars() {
    auto &ui = g_timeClockUi;
    ui.clockFaceDepth = 4.0f;
    ui.hourHandStep = 0.25f;
    ui.hourHandDepth = 5.0f;
    ui.minuteHandStep = g_stepSize;
    ui.minuteHandDepth = 6.0f;
    ui.field_38 = 7.0f;
    ui.minuteHandScale = 1.6f;
    g_disableClockAndActionSelector = 0;

    ui.hourHandFlags = 0x400130;
    ui.hourHandLayer = 5;
    ui.clockFaceX = 592.0f;
    ui.clockFaceY = 418.0f;
    ui.hourHandMode = 64;
    ui.clockFaceScaleX = 1.0f;
    ui.clockFaceScaleY = 1.0f;
    ui.field_64 = 0;
    ui.field_68 = 0;

    ui.minuteHandFlags = 0x100131;
    ui.minuteHandLayer = 9;
    ui.hourHandX = 592.0f;
    ui.hourHandY = 418.0f;
    ui.minuteHandMode = 16;
    ui.hourHandScale = 1.0f;
    ui.field_88 = 0;
    ui.hourHand = 0;

    ui.secondHandFlags = 0x100131;
    ui.secondHandLayer = 9;
    ui.minuteHandX = 592.0f;
    ui.minuteHandY = 418.0f;
    ui.secondHandMode = 16;
    ui.field_ac = 0;
    ui.minuteHand = 0;

    ui.clockFaceFlags = 0x400136;
    ui.clockFaceLayer = 5;
    ui.field_28 = 592.0f;
    ui.field_2c = 418.0f;
    ui.clockFaceMode = 64;
    ui.field_30 = 1.2f;
    ui.field_34 = 1.2f;
    ui.field_3c = 0;
    ui.cameraYaw = 0;

    ui.transform.field_04 = 0;
    ui.transform.field_06 = 0;
    ui.transform.field_08 = 0;
    ui.transform.field_00 = 0;
    ui.transform.field_0c = 45;
    ui.transform.position[0] = 0.0f;
    ui.transform.position[1] = 0.0f;
    ui.transform.position[2] = 0.0f;
    ui.transform.field_1c = 0;
    ui.transform.field_20 = 0;
    ui.transition = 0;
}

void RecalculateDayInTheWeek(GameDateTime *dateTime) {
    std::int16_t year = static_cast<std::int16_t>(dateTime->year + 1900);
    std::int8_t month = static_cast<std::int8_t>(dateTime->month);
    if (month < 3) {
        year = static_cast<std::int16_t>(dateTime->year + 1899);
        month = static_cast<std::int8_t>(month + 12);
    }
    dateTime->dayOfWeek =
        static_cast<std::uint8_t>((year / 400 + (13 * month + 8) / 5 - year / 100 + year + year / 4 + static_cast<std::int8_t>(dateTime->day)) % 7);
}

void IncrementDateTimeByDays(GameDateTime &dateTime, std::int16_t days) {
    while (days-- > 0) {
        const std::uint8_t lastDay = ((dateTime.year & 3) == 0 && dateTime.month == 2) ? 29 : kDaysInMonth[dateTime.month];
        ++dateTime.day;
        if (static_cast<std::int8_t>(dateTime.day) > static_cast<std::int8_t>(lastDay)) {
            dateTime.day = 1;
            ++dateTime.month;
            if (static_cast<std::int8_t>(dateTime.month) > 12) {
                dateTime.month = 1;
                ++dateTime.year;
                if (static_cast<std::int8_t>(dateTime.year) > 99)
                    dateTime.year = 0;
            }
        }
    }
    RecalculateDayInTheWeek(&dateTime);
}

void IncrementCurrentDateTime(GameDateTime &now, const GameDateTime &request) {
    now.seconds += request.seconds;
    if (static_cast<std::int8_t>(now.seconds) > 59) {
        ++now.minutes;
        now.seconds = 0;
    }
    now.minutes += request.minutes;
    if (static_cast<std::int8_t>(now.minutes) > 59) {
        ++now.hours;
        now.minutes = 0;
    }
    now.hours += request.hours;
    if (static_cast<std::int8_t>(now.hours) > 23) {
        ++now.day;
        now.hours = 0;
    }
    now.day += request.day;
    const std::uint8_t year = now.year;
    const std::uint8_t month = now.month;
    std::uint8_t lastDay = 29;
    if ((year & 3U) != 0 || month != 2)
        lastDay = month >= 13 ? 30 : kDaysInMonth[month];
    if (static_cast<std::int8_t>(now.day) > static_cast<std::int8_t>(lastDay)) {
        now.day = 1;
        now.month = static_cast<std::uint8_t>(month + 1);
    }
    now.month += request.month;
    if (static_cast<std::int8_t>(now.month) > 12) {
        now.month = 1;
        now.year = static_cast<std::uint8_t>(year + 1);
    }
    now.year += request.year;
    if (static_cast<std::int8_t>(now.year) > 99)
        now.year = 0;
    RecalculateDayInTheWeek(&now);
}

void SetNewDateTime(GameDateTime &dateTime) {
    IncrementCurrentDateTime(dateTime, {});
    g_currentDateTime.year = dateTime.year;
    g_currentDateTime.month = dateTime.month;
    g_currentDateTime.day = dateTime.day;
    g_currentDateTime.hours = dateTime.hours;
    g_currentDateTime.minutes = dateTime.minutes;
    g_currentDateTime.seconds = dateTime.seconds;
    RecalculateDayInTheWeek(&g_currentDateTime);
}

void SetDateAndTime(std::uint8_t currentHours, std::uint8_t currentMinutes, std::uint8_t maximumHours, std::uint8_t maximumMinutes) {
    g_maximumDateTime.year = g_currentDateTime.year;
    g_maximumDateTime.month = g_currentDateTime.month;
    g_maximumDateTime.day = g_currentDateTime.day;
    g_currentDateTime.hours = currentHours;
    g_currentDateTime.minutes = currentMinutes;
    g_currentDateTime.seconds = 0;
    RecalculateDayInTheWeek(&g_currentDateTime);
    if (static_cast<std::int8_t>(currentHours) > static_cast<std::int8_t>(maximumHours) ||
        (currentHours == maximumHours && static_cast<std::int8_t>(currentMinutes) > static_cast<std::int8_t>(maximumMinutes))) {
        IncrementDateTimeByDays(g_maximumDateTime, 1);
    }
    g_maximumDateTime.hours = maximumHours;
    g_maximumDateTime.minutes = maximumMinutes;
    g_maximumDateTime.seconds = 0;
    RecalculateDayInTheWeek(&g_maximumDateTime);
    if (g_timeValue)
        g_shouldTimeAdvance = 1;
}

bool TIME_IsActive() {
    return g_timeValue != 0;
}

void TIME_EnableAdvance() {
    g_shouldTimeAdvance = 1;
}

void TIME_ResetCycleTime() {
    g_cycleTimeMultiplier = 0.5f;
    g_currentDeltaTimeSim = 0.0f;
    g_newCycleTime = 0.0f;
}

void TIME_SetBlocked(bool blocked) {
    g_timeBlockedUpdate = blocked ? 1 : 0;
}

void TIME_SetDisabled(bool disabled) {
    g_timeDisabled = disabled ? 1 : 0;
}

void TIME_SetUpdateEnabled(std::uint8_t state) {
    if (g_timeValue)
        g_shouldTimeAdvance = state;
}

void TIME_ApplyDateRange(const GameDateTime &current, const GameDateTime &maximum) {
    GameDateTime requested = current;
    SetNewDateTime(requested);
    g_maximumDateTime.year = maximum.year;
    g_maximumDateTime.month = maximum.month;
    g_maximumDateTime.day = maximum.day;
    g_maximumDateTime.hours = maximum.hours;
    g_maximumDateTime.minutes = maximum.minutes;
    g_maximumDateTime.seconds = maximum.seconds;
    RecalculateDayInTheWeek(&g_maximumDateTime);
    if (g_timeValue)
        g_shouldTimeAdvance = 1;
}

void TIME_WriteUpdateHistory(std::uint8_t *output) {
    if (!output)
        return;
    output[0] = g_hasIncrementedYear;
    output[1] = g_hasIncrementedMonth;
    output[2] = g_hasIncrementedDay;
    output[4] = g_hasIncrementedMinutes;
    output[5] = g_hasIncrementedSeconds;
}

void UpdateTime() {
    float newTime = g_newCycleTime;
    if (!g_unusedTimeBypass) {
        float increment = g_timeMultiplier * g_cycleTimeMultiplier;
        if (g_bEnableFrameTimeSmoothing) {
            increment = increment * 30.0f * fFrameTimeMs;
        }
        newTime += increment;
        g_newCycleTime = newTime;
    }

    if (newTime >= 60.0f) {
        std::uint8_t minutes = g_currentDateTime.minutes;
        std::uint8_t hours = g_currentDateTime.hours;
        do {
            ++minutes;
            g_currentDateTime.seconds = 0;
            g_hasIncrementedSeconds = 1;
            g_currentDateTime.minutes = minutes;
            newTime -= 60.0f;
            if (static_cast<std::int8_t>(minutes) > 59) {
                minutes = 0;
                g_hasIncrementedMinutes = 1;
                ++hours;
                g_currentDateTime.minutes = 0;
                g_currentDateTime.hours = hours;
                if (static_cast<std::int8_t>(hours) > 23) {
                    const std::uint8_t previousMonth = g_currentDateTime.month;
                    g_currentDateTime.hours = 0;
                    g_hasIncrementedDay = 1;
                    const std::uint8_t lastDay =
                        ((g_currentDateTime.year & 3) == 0 && g_currentDateTime.month == 2) ? 29 : kDaysInMonth[g_currentDateTime.month];
                    if (++g_currentDateTime.day > lastDay) {
                        g_currentDateTime.day = 1;
                        g_hasIncrementedMonth = 1;
                        ++g_currentDateTime.month;
                        if (static_cast<std::uint8_t>(previousMonth + 1) > 12) {
                            g_currentDateTime.month = 1;
                            std::uint8_t year = static_cast<std::uint8_t>(g_currentDateTime.year + 1);
                            g_hasIncrementedYear = 1;
                            if (static_cast<std::int8_t>(year) > 99) {
                                year = 0;
                            }
                            g_currentDateTime.year = year;
                        }
                    }
                    RecalculateDayInTheWeek(&g_currentDateTime);
                    minutes = g_currentDateTime.minutes;
                    hours = g_currentDateTime.hours;
                }
            }
        } while (newTime >= 60.0f);
        g_newCycleTime = newTime;
    }

    g_currentDateTime.seconds = static_cast<std::uint8_t>(newTime);
    g_currentDeltaTimeSim = static_cast<float>(std::fmod(static_cast<double>(newTime), 1.0) * 1000.0);
}

void EnqueueTIME() {
    HLib_EnqueueTaskWithoutParameter(TIME_Callback, 7, 0, SHEN_NAME(TIME));
    std::uint8_t status = 1;
    g_timeValue = 1;
    g_freezeTime = 0;
    g_timeDisabled = 0;
    g_timeBlockedUpdate = 0;
    if (g_sceneId == 0 || g_sceneId == 80) {
        if (g_sceneId == 80) {
            g_timeBlockedUpdate = 1;
        }
        status = 0;
        g_timeValue = 0;
    }
    g_shouldTimeAdvance = status;
    g_maximumDateTime.year = 99;
    g_maximumDateTime.month = 12;
    g_maximumDateTime.day = 31;
    g_maximumDateTime.hours = 23;
    g_maximumDateTime.minutes = 59;
    g_maximumDateTime.seconds = 59;
    g_hasIncrementedYear = 0;
    g_hasIncrementedMonth = 0;
    g_hasIncrementedDay = 0;
    g_hasIncrementedMinutes = 0;
    g_hasIncrementedSeconds = 0;
    g_unusedTimeFlag3 = 0;
    TIME_SetupGVars();
}

void TIME_Callback_0(HLib::TaskData *) {
    if ((g_helpIconFlags & 1) != 0 || g_timeBlockedUpdate) {
        return;
    }
    if (g_freezeTime != 1) {
        g_hasIncrementedYear = 0;
        g_hasIncrementedMonth = 0;
        g_hasIncrementedDay = 0;
        g_hasIncrementedMinutes = 0;
        g_hasIncrementedSeconds = 0;
        g_unusedTimeFlag3 = 0;
        if (g_shouldTimeAdvance == 1) {
            UpdateTime();
            if (CompareDates(&g_currentDateTime, &g_maximumDateTime) != DateComparisonResult::lessThan) {
                g_shouldTimeAdvance = 2;
            }
        }
    }


    const std::uint8_t shouldUpdate = GetInventoryItemCount(3);
    g_shouldUpdateClockUi = shouldUpdate;
    if (g_timeDisabled || !shouldUpdate) {
        return;
    }

    const std::int32_t minutes = static_cast<std::int8_t>(g_currentDateTime.minutes);
    const std::int32_t seconds = static_cast<std::int8_t>(g_currentDateTime.seconds);
    const std::int32_t hours = static_cast<std::int8_t>(g_currentDateTime.hours);
    const auto minuteAngle = static_cast<std::uint16_t>(seconds * -18 - minutes * 1092);
    g_timeClockUi.minuteHand = 0u - static_cast<std::uint32_t>(minuteAngle);
    const auto hourAngle = static_cast<std::uint16_t>(minutes * -91 - hours * 5461 - seconds);
    g_timeClockUi.hourHand = 0u - static_cast<std::uint32_t>(hourAngle);
    TIME_UpdateClockUI();
}

} // namespace shenmue::s1
