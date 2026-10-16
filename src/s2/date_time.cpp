#include "s2/date_time.hpp"
#include "s2/time_program.hpp"

#include "d3t/object_store.hpp"
#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/story_state.hpp"
#include "s2/game_runtime.hpp"
#include "s2/gauge.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/load_title.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"

#include <array>
#include <cstring>
#include <bit>
#include <cmath>
#include <cstdio>

namespace shenmue::s2 {

DateTime g_currentDateTime{};
std::int32_t g_freezeGameTime{};

namespace {

constexpr std::int32_t monthStartDays[12]{
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
constexpr std::int8_t daysInMonth[16]{
    31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0, 0, 0};

DateTime currentTime{};
DateTime nextDateTime{};
std::uint8_t currentCalendarReservedByte{};
std::uint16_t currentCalendarReservedWord{}, savedCalendarReservedWord{}, nextCalendarReservedWord{};
float realSecondsToGameMinute = 1.0f / 3.0f;
float realTimeScale = -1.0f;
float milliseconds = 0.0f;
float secondsPerGameMinute = 3.0f;
float defaultSecondsPerGameMinute = 3.0f;
float secondAccumulator = 0.0f;
std::int32_t temporaryDay = 0;
std::int32_t temporaryHour = 0;
std::int32_t temporaryMinute = 0;
std::uint8_t gameClockUpdateFlag{1};
std::uint8_t timeUpdateAllowed{1};
std::uint8_t timeUpdateBlocked{};
std::uint8_t timeFrozen{};
std::uint8_t canTickForFrame{};
std::uint8_t framesPerSecond{30};
std::uint8_t incrementedYear{};
std::uint8_t incrementedMonth{};
std::uint8_t incrementedDay{};
std::uint8_t incrementedMinutes{};
std::uint8_t incrementedSeconds{};
std::uint8_t incrementedUnused{};
std::uint8_t previousItemState{};
std::uint8_t clockAndCompassSuppressed{};
std::int32_t timeProgramWait{5};
std::int32_t minuteHand{};
std::int32_t hourHand{};
std::int32_t compassRotation{};
std::int32_t timeHudVariant{};
Vector3 timeHudPosition{};
TimeCompassContext timeCompass{};
std::array<LegacyIconSprite, 3> timeWatchSprites{};
LegacyIconSprite timeCompassSprite{};
LegacyIconSprite timePeriodSprite{.x = 0, .y = 0, .scaleX = 1, .scaleY = 1, .depth = 4.5f, .flags = 0x400,
    .reserved16 = 0, .rotation = 0, .icon = 1545, .width = 32, .height = 16, .alignment = 5, .reserved23 = {}};


HandWatchControl handWatch{};
MT7Model handWatchModel{};
std::byte* handWatchSource{};
bool handWatchPlacementRegistered{};


void UpdateWatchHandAngles(MT7Node* root) {
    MT7Node* const minute = GetChildObjectByIndex(handWatchModel, root, 151);
    MT7Node* const hour = GetChildObjectByIndex(handWatchModel, root, 150);
    if (minute) minute->rotation.y = static_cast<std::uint16_t>(-18 * g_currentDateTime.second - 1092 * g_currentDateTime.minute);
    if (hour) hour->rotation.y = static_cast<std::uint16_t>(-91 * g_currentDateTime.minute - 5461 * g_currentDateTime.hour - g_currentDateTime.second);
}

// LoadRyoTimeArm's SafeTryLoad call has zero region-headroom requirement. Retain
// its NPC-load suspension, free-talk pruning and heap's extra 32-byte guard.
bool LoadRyoTimeArm() {
    constexpr char path[] = "model/item/YKBT2U2G.MT7";
    const std::uint32_t previousLoadState = static_cast<std::uint32_t>(GetNpcNewLoadState());
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const std::int64_t fileSize = static_cast<std::int32_t>(ArchiveData_GetFileSize(path));
    std::byte* source = nullptr;
    if (fileSize != 0 && !HLib::WouldAllocationFail(0, fileSize)) {
        source = static_cast<std::byte*>(HLib::AllocateTaskData(fileSize, WATCH_MODEL_DATA));
        if (source && !ArchiveData_Read(path, 0, source, fileSize)) {
            HLib::FreeTaskData(source);
            source = nullptr;
        }
    }
    EV_NPC_NewLoadSw(previousLoadState);
    if (!source) return false;
    if (!ReadModelFormat7({source, static_cast<std::size_t>(fileSize)},
                         path, handWatchModel, 2)) {
        HLib::FreeTaskData(source);
        return false;
    }
    handWatchSource = source;
    handWatch.reader = &handWatchModel;
    handWatch.root = handWatchModel.nodes.empty() ? nullptr : handWatchModel.nodes.data();
    return GetChildObjectByIndex(handWatchModel, handWatch.root, 150) && GetChildObjectByIndex(handWatchModel, handWatch.root, 151) &&
           GetChildObjectByIndex(handWatchModel, handWatch.root, 162);
}

bool SetupHandWatchCamera() {
    MapControlData* const control = static_cast<MapControlData*>(HLib::GetTaskParameter(g_mapControlTask));
    Character* const player = control
        ? static_cast<Character*>(control->ryo) : nullptr;
    if (!player) return false;
    handWatch.savedPlayerDisplay = IsOfThisVisibilityType(player, 3);
    handWatch.savedPlayerVisibility = (player->flags & 0x10U) != 0;
    player->renderFlags &= static_cast<std::uint8_t>(~4U);
    player->flags &= ~0x10U;
    MT7Node* const hour = GetChildObjectByIndex(handWatchModel, handWatch.root, 150);
    MT7Node* const minute = GetChildObjectByIndex(handWatchModel, handWatch.root, 151);
    MT7Node* const light = GetChildObjectByIndex(handWatchModel, handWatch.root, 162);
    camCtrlGetNextCamPos(&handWatch.savedCameraPosition);
    camCtrlGetTargetPosition(&handWatch.savedCameraTarget);
    handWatch.savedFov = static_cast<std::int16_t>(camCtrlGetFov());
    handWatch.savedPerspective = static_cast<std::int16_t>(camCtrlGetPerspective());
    handWatch.savedCameraState = camCtrlGetState();
    light->position.y = -0.001f;
    light->scale = {};
    const Vector3 position = GetCharacterWorldPosition(player);
    Vector3 cameraPosition{};
    GetCharacterMotionWorldPosition(player, 5, &cameraPosition);
    cameraPosition.x = position.x;
    cameraPosition.y += 0.12f;
    cameraPosition.z = position.z;
    camCtrlSetState(CameraState::Event);
    camCtrlSetNextPosition(cameraPosition);
    const std::int32_t inverseYaw = -GetAllAngYForNode(&player->entity.node);
    Vector3 hip{};
    GetCharacterMotionWorldPosition(player, 1, &hip);
    handWatch.armPosition = {
        sin_fixed16(static_cast<std::int16_t>(inverseYaw)) * 0.2f + position.x,
        hip.y,
        sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(
            static_cast<std::uint16_t>(inverseYaw)))) * -0.2f + position.z};
    handWatch.armRotation = {0, -inverseYaw, 0};
    hour->rotation.y = static_cast<std::uint16_t>(-91 * g_currentDateTime.minute - 5461 * g_currentDateTime.hour - g_currentDateTime.second);
    minute->rotation.y = static_cast<std::uint16_t>(-18 * g_currentDateTime.second - 1092 * g_currentDateTime.minute);
    handWatch.root->rotation.y = 32768;
    camCtrlSetTargetPosition(handWatch.armPosition);
    camCtrlSetPerspective(1274);
    return true;
}

// @verified - p
std::int32_t TimeX_LegacyDrawNoop() { return 0; }

void TimeX_ConstructMatrices(void*) {
    if (!handWatch.root) return;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(handWatch.armPosition.x,
                          handWatch.armPosition.y, handWatch.armPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(
        static_cast<std::uint16_t>(handWatch.armRotation.y));
    const std::uint32_t projection = GetLegacyPerspectiveProjection();
    if (projection == 0 && !handWatchPlacementRegistered)
        handWatchPlacementRegistered = d3t::RegisterObjectPlacement(
            GetMT7ReaderData(&handWatchModel), handWatch.root, 0);
    const bool hashed = d3t::HashModel(handWatch.root, projection);
    Matrix4x4::Pop(1);
    if (!hashed) {
        Matrix4x4_Push();
        TimeX_LegacyDrawNoop();
        Matrix4x4::preTranslate(handWatch.armPosition.x,
                              handWatch.armPosition.y, handWatch.armPosition.z);
        Matrix4x4::RotateAroundY_Wrapper(
            static_cast<std::uint16_t>(handWatch.armRotation.y));
        DrawMT7Model(handWatchModel);
        Matrix4x4::Pop(1);
        TimeX_LegacyDrawNoop();
    } else {
        UpdateModelFrame(handWatch.reader, nullptr, 0, 0);
    }
}

void tmx_CtrlHandWatch_Callback(void*) {
    const std::uint16_t held = GetPadControlData(0);
    std::uint16_t pressed = GetPadControlData(1);
    if (handWatch.flags & 0x10U) {
        pressed &= ~0x200U;
        if (handWatch.state == 3) handWatch.state = 2;
    }
    if (handWatch.state == 1) {
        camCtrlUpdate();
        if (!LoadRyoTimeArm() || !SetupHandWatchCamera()) {
            CloseWristwatch(1);
            return;
        }
        handWatch.savedHelpMode = static_cast<std::uint16_t>(GetUiFlag(2));
        handWatch.savedHelpVisible = static_cast<std::uint16_t>(GetUiFlag(0));
        handWatch.savedHelpSelected = static_cast<std::uint16_t>(GetUiFlag(1));
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        handWatch.state = 2;
        handWatch.flags |= 0x20U;
    } else if (handWatch.state == 2) {
        UpdateWatchHandAngles(handWatch.root);
        if (GetShouldRenderIcons(RenderIconSlot::ItemSelect))
            set_icon_select_flags_to((handWatch.flags & 0x10U) ? 1024 : 1536);
        if (pressed & 0x200U) {
            handWatch.state = 3;
        } else if (MT7Node* const light = GetChildObjectByIndex(handWatchModel, handWatch.root, 162)) {
            const bool lit = (handWatch.flags & 4U) || (held & 0x400U);
            if (lit) {
                light->position.y = 0.0f;
                light->scale.z = light->scale.y = light->scale.x = 1.0f;
            } else {
                light->scale.y = light->scale.z = light->scale.x = 0.0f;
                light->position.y = -0.001f;
            }
        }
    } else if (handWatch.state == 3) {
        CloseWristwatch(1);
    }
    HLib::ExecuteCallbackNow(TimeX_ConstructMatrices, nullptr);
}

constexpr std::uint32_t guilinForest1 =
    SHEN_NAME(KMZ1);
constexpr std::uint32_t guilinForest2 =
    SHEN_NAME(KMZ2);
constexpr std::uint32_t guilinForest3 =
    SHEN_NAME(KMZ3);
constexpr std::uint32_t guilinForest4 =
    SHEN_NAME(KMZ4);
constexpr std::uint32_t kowloonRoute2 =
    SHEN_NAME(KRK2);
constexpr std::uint32_t kowloonRoute4 =
    SHEN_NAME(KRK4);
constexpr std::uint32_t kowloonRoute6 =
    SHEN_NAME(KRK6);
constexpr std::uint32_t kowloonUnderground0 =
    SHEN_NAME(QUG0);
constexpr std::uint32_t bigOxBuilding =
    SHEN_NAME(QKBT);

bool UsesFastGameClock(std::uint32_t map) {
    switch (map) {
    case guilinForest1:
    case guilinForest2:
    case guilinForest3:
    case guilinForest4:
    case kowloonRoute2:
    case kowloonRoute4:
    case kowloonRoute6:
        return true;
    default:
        return false;
    }
}

bool UsesSlowGameClock(std::uint32_t map) {
    return (map & 0xffffU) == 0x4651U ||
           map == kowloonUnderground0 || map == bigOxBuilding;
}

bool IsGuilinNavigationMap(std::uint32_t map) {
    return ((map - guilinForest1) & 0xfcffffffU) == 0;
}

std::int32_t CurrentCompassHeading() {
    const std::int16_t cameraYaw = static_cast<std::int16_t>(-camCtrlGetNextRotY());
    const MapControlData* const control = static_cast<const MapControlData*>(HLib::GetTaskParameter(g_mapControlTask));
    return cameraYaw + static_cast<std::int16_t>(control->sunLocation);
}

std::int32_t UpdateCompassAdmission(TimeCompassContext& context) {
    constexpr float epsilon = 0.00000011920929f;
    Vector3 position;
    camCtrlGetNextCamPos(&position);
    std::int32_t counter = context.counter;
    const std::uint32_t frame = static_cast<std::uint32_t>(spliGetCurrentFrmCnt());
    const std::int32_t searchRequested = plymIsSearchStartRequested();
    const CameraState state = camCtrlGetState();
    if ((!searchRequested || state == CameraState::InterpolationFollow || HasScenarioCharacterControl()) && !IsModeBitActive(64)) {
        if (counter <= 0) {
            context.enabled = 0;
        } else {
            if (context.previousSearch == 0) {
                const std::int32_t distance = std::bit_cast<std::int32_t>(frame - context.previousFrame);
                const std::int32_t magnitude = distance >= 0 ? distance : std::bit_cast<std::int32_t>(context.previousFrame - frame);
                if (magnitude > 2 && counter > 22) counter = 22;
            }
            if (position.x < epsilon) position.x = 0.0f;
            if (position.y < epsilon) position.y = 0.0f;
            if (position.z < epsilon) position.z = 0.0f;
            if (state == CameraState::Event || state == CameraState::InterpolationFollow || authGetLoaded() ||
                std::fabs(position.x - context.previousCameraPosition.x) > epsilon ||
                std::fabs(position.y - context.previousCameraPosition.y) > epsilon ||
                std::fabs(position.z - context.previousCameraPosition.z) > epsilon) --counter;
        }
    } else if (counter >= 45) {
        context.enabled = 1;
    } else {
        ++counter;
    }
    context.previousCameraPosition = position;
    context.counter = counter;
    context.previousFrame = frame;
    context.previousSearch = searchRequested;
    const bool movesUiVisible = IsElementEnabled(UIElement::SubMenuMoves) || IsHelpOverlayVisible(UIElement::SubMenuMoves);
    return context.enabled | static_cast<std::int32_t>(movesUiVisible) | static_cast<std::int32_t>(clockAndCompassSuppressed != 0);
}

void TimeProgramHudCallback(void* parameter) {
    if (timeProgramWait >= 0) {
        timeCompass.heading = CurrentCompassHeading();
        return;
    }
    const Vector3& position = *static_cast<const Vector3*>(parameter);
    timeWatchSprites[0].x = position.x;
    timeWatchSprites[0].y = position.y;
    timeWatchSprites[0].scaleX = position.z;
    timeWatchSprites[0].scaleY = position.z;
    timeWatchSprites[1].x = position.x;
    timeWatchSprites[1].y = position.y;
    timeWatchSprites[1].scaleX = position.z * 0.25f;
    timeWatchSprites[1].scaleY = position.z;
    timeWatchSprites[2].x = position.x;
    timeWatchSprites[2].y = position.y;
    timeWatchSprites[2].scaleX = position.z * 0.2f;
    timeWatchSprites[2].scaleY = position.z * 1.6f;
    if (GetDebugForceUIEnabled(UIElement::GameWatch)) {
        EnableElement(UIElement::GameHudWatch);
    } else {
        PrepareDrawSimple(timeWatchSprites[0], 0xcc000000U, ScreenMode::Stretched);
        PrepareDrawSimple(timeWatchSprites[1], 0x7f000000U, ScreenMode::Stretched);
        PrepareDrawSimple(timeWatchSprites[2], 0x7f000000U, ScreenMode::Stretched);
    }
    timePeriodSprite.x = position.x + 22.0f;
    timePeriodSprite.y = position.y + 44.0f;
    if (!GetDebugForceUIEnabled(UIElement::GameWatch)) PrepareDrawWithMode(timePeriodSprite, ScreenMode::Stretched);
    std::int32_t imageEnabled = 0;
    if (!GetDebugForceUIEnabled(UIElement::GameWatch)) {
        imageEnabled = UpdateCompassAdmission(timeCompass);
        if (imageEnabled) {
            if (compassRotation < 100) compassRotation += 8;
        } else if (compassRotation > 0) {
            imageEnabled = 1;
            compassRotation -= 8;
        }
    }
    std::int32_t heading = timeCompass.heading;
    const std::int32_t currentHeading = CurrentCompassHeading();
    const std::int32_t delta = static_cast<std::int16_t>(currentHeading - heading);
    if (imageEnabled && (timeCompass.direction != 0 || std::abs(delta) >= 2184)) {
        std::int32_t direction = 0;
        if (delta > 0) {
            heading = static_cast<std::int16_t>(heading + 364);
            direction = 1;
        } else if (delta < 0) {
            heading = static_cast<std::int16_t>(heading - 364);
            direction = -1;
        }
        if (timeCompass.direction != 0 && timeCompass.direction != direction && std::abs(delta) < 2184) {
            heading = currentHeading;
            direction = 0;
        }
        timeCompass.direction = direction;
    } else {
        heading = currentHeading;
    }
    if (imageEnabled) {
        timeCompassSprite.rotation = static_cast<std::uint32_t>(-heading);
        timeCompassSprite.x = position.x;
        timeCompassSprite.y = position.y;
        timeCompassSprite.scaleX = position.z * 1.2f;
        timeCompassSprite.scaleY = timeCompassSprite.scaleX;
        PrepareDrawSimple(timeCompassSprite, static_cast<std::uint32_t>(175 * compassRotation / 100) << 24, ScreenMode::Stretched);
    }
    timeCompass.heading = heading;
    if (GetDebugForceUIEnabled(UIElement::GameWatch)) {
        constexpr float fixedAngleToRadians = 10430.378f;
        UpdateCompass(6.2831898f - static_cast<float>(heading) / fixedAngleToRadians);
        UpdateMinuteHand(static_cast<float>(minuteHand) / fixedAngleToRadians);
        UpdateHourHand(static_cast<float>(hourHand) / fixedAngleToRadians);
        imageEnabled = UpdateCompassAdmission(timeCompass);
        if (imageEnabled) {
            if (compassRotation < 100) {
                compassRotation += 8;
                UpdateCompassImage(imageEnabled, compassRotation);
                return;
            }
        } else if (compassRotation > 0) {
            imageEnabled = 1;
            compassRotation -= 8;
        }
        UpdateCompassImage(imageEnabled, compassRotation);
    }
}

void ResetTimeHudRuntime() {
    timeProgramWait = 5;
    clockAndCompassSuppressed = 0;
    timeWatchSprites[0].depth = 4.0f;
    timeWatchSprites[1].scaleX = 0.25f;
    timeWatchSprites[1].depth = 4.5f;
    timeWatchSprites[2].scaleX = 0.2f;
    timeWatchSprites[2].scaleY = 1.6f;
    timeWatchSprites[2].depth = 5.0f;
    for (std::uint32_t index = 0; index != 3; ++index) {
        LegacyIconSprite& sprite = timeWatchSprites[index];
        sprite.x = 578.6f;
        sprite.y = 96.0f;
        sprite.flags = 0;
        sprite.icon = index == 0 ? 2050 : 1553;
        sprite.width = sprite.height = index == 0 ? 64 : 16;
        sprite.alignment = index == 0 ? 5 : 9;
    }
    timeWatchSprites[0].scaleX = timeWatchSprites[0].scaleY = 1.0f;
    timeWatchSprites[0].rotation = 0;
    timeWatchSprites[1].scaleY = 1.0f;
    minuteHand = hourHand = 0;
    timeHudVariant = 0;
    timeCompassSprite.x = 578.6f;
    timeCompassSprite.y = 96.0f;
    timeCompassSprite.scaleX = timeCompassSprite.scaleY = 1.2f;
    timeCompassSprite.depth = 7.0f;
    timeCompassSprite.flags = 0;
    timeCompassSprite.rotation = 0;
    timeCompassSprite.icon = 2051;
    timeCompassSprite.width = timeCompassSprite.height = 64;
    timeCompassSprite.alignment = 5;
    timeCompass = {.enabled = 0, .heading = 0, .direction = 0, .counter = 45, .previousCameraPosition = {},
        .previousFrame = 0, .previousSearch = 0};
    compassRotation = 0;
}

void UpdateTimeProgramWaitTime() {
    const std::int32_t minuteTerm =
        -18 * g_currentDateTime.second -
        1092 * g_currentDateTime.minute;
    const std::int32_t hourTerm =
        -91 * g_currentDateTime.minute -
        5461 * g_currentDateTime.hour -
        g_currentDateTime.second;
    minuteHand = -static_cast<std::int32_t>(
            static_cast<std::uint16_t>(minuteTerm));
    hourHand = -static_cast<std::int32_t>(
            static_cast<std::uint16_t>(hourTerm));
    timePeriodSprite.icon = TIME_HUD_PERIOD_ICONS[static_cast<std::size_t>(g_currentDateTime.hour / 12)];
    const std::size_t variant = static_cast<std::size_t>(timeHudVariant);
    timeHudPosition.x = TIME_HUD_POSITIONS[variant].x;
    timeHudPosition.y = TIME_HUD_POSITIONS[variant].y;
    timeHudPosition.z = TIME_HUD_SCALES[variant];

    bool resetWait =
        GetItemState(2) == 0 || clockAndCompassSuppressed != 0 ||
        EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1012) != 0;
    if (!resetWait && !IsItemSelectable() && !IsAnyItemSelectActive()) {
        const MapControlData* const control = static_cast<const MapControlData*>(HLib::GetTaskParameter(g_mapControlTask));
        resetWait =
            !IsScreenFilmNotShowing(control->screenFilmTask) ||
            authGetLoaded() || GetIsLoadScreenActive() ||
            g_moveUiClockAndNavigationHold != 0 ||
            g_fightUiActive != 0 || IsFightHealthUIActive() ||
            EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1021) != 0;
    }
    if (resetWait) timeProgramWait = 5;
    else --timeProgramWait;
    HLib::ExecuteCallbackNow(TimeProgramHudCallback, &timeHudPosition);
}

bool IsGameTimeUpdateBlocked() {
    return GetParentMenu() == 0 || IsItemSelectable() ||
           IsEasyOptionActive() || IsSaveAllowed() != 0 ||
           IsAnyItemSelectActive() || IsWristwatchOpenable() ||
           IsWazamakiInactive() != 0 ||
           EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1021) != 0 ||
           (GetSleepActionIconFlags() & 1U) != 0 ||
           (IsNaviActive() &&
            IsGuilinNavigationMap(g_currentMap.name));
}

void UpdateGameTime(DateTime& dateTime) {
    if (IsGameTimeUpdateBlocked()) return;

    float accumulated = secondAccumulator;
    if (realTimeScale > 0.0f &&
        temporaryDay == dateTime.day &&
        (temporaryHour > dateTime.hour ||
         (temporaryHour == dateTime.hour &&
          temporaryMinute > dateTime.minute))) {
        accumulated += realTimeScale;
    } else {
        if (realTimeScale > 0.0f) realTimeScale = -1.0f;
        if (!g_freezeGameTime) accumulated += shenGetScaledDeltaTickTime(g_gameTimeMultiplier * realSecondsToGameMinute);
    }
    secondAccumulator = accumulated;
    while (accumulated >= 60.0f) {
        incrementedSeconds = true;
        ++dateTime.minute;
        accumulated =
            static_cast<float>(static_cast<double>(accumulated) - 60.0);
        if (dateTime.minute > 59) {
            dateTime.minute = 0;
            incrementedMinutes = true;
            if (++dateTime.hour > 23) {
                dateTime.hour = 0;
                incrementedDay = true;
                const std::int8_t lastDay =
                    (dateTime.year & 3) == 0 && dateTime.month == 2
                    ? 29 : daysInMonth[
                        static_cast<std::uint8_t>(dateTime.month)];
                if (++dateTime.day > lastDay) {
                    dateTime.day = 1;
                    incrementedMonth = true;
                    if (++dateTime.month > 12) {
                        dateTime.month = 1;
                        incrementedYear = true;
                        if (++dateTime.year > 99) dateTime.year = 0;
                    }
                }
                ComputeDayOfWeek(dateTime);
            }
        }
    }
    secondAccumulator = accumulated;
    dateTime.second = static_cast<std::int8_t>(accumulated);
    milliseconds = static_cast<float>(
        std::fmod(static_cast<double>(accumulated), 1.0) * 1000.0);
}

void CtrlTimeProgram_Callback(void*) {
    if (timeUpdateBlocked) return;
    if (timeFrozen != 1) {
        incrementedYear = false;
        incrementedMonth = false;
        incrementedDay = false;
        incrementedMinutes = false;
        incrementedSeconds = false;
        incrementedUnused = 0;
        if (gameClockUpdateFlag == 1 &&
            EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1011) == 0) {
            UpdateGameTime(g_currentDateTime);
            if (CompareDates(g_currentDateTime, nextDateTime) !=
                DateComparisonResult::LessThan) {
                gameClockUpdateFlag = 2;
            }
        }
    }
    previousItemState =
        static_cast<std::uint8_t>(GetItemState(2));
    if (!canTickForFrame && previousItemState != 0)
        UpdateTimeProgramWaitTime();
}

void InitTimeProgram_Callback(void*) {
    HLib::SetCurrentTaskTo(CtrlTimeProgram_Callback);
}

} // namespace

// @verified - p
void GetCurrentDateTime(DateTime& dateTime) {
    dateTime = g_currentDateTime;
}

// @verified - p
void CopyCurrentCalendarRecord(std::span<std::byte, CALENDAR_RECORD_SIZE> output) {
    std::memcpy(output.data(), &g_currentDateTime, sizeof(DateTime));
    output[7] = static_cast<std::byte>(currentCalendarReservedByte);
    std::memcpy(output.data() + 8, &currentCalendarReservedWord, sizeof(currentCalendarReservedWord));
}

// @verified - p
void IncrementDateTimeInPlace(DateTime& current, const DateTime& increment) {
    current.second = static_cast<std::int8_t>(current.second + increment.second);
    if (current.second > 59) {
        ++current.minute;
        current.second = 0;
    }
    current.minute = static_cast<std::int8_t>(current.minute + increment.minute);
    if (current.minute > 59) {
        ++current.hour;
        current.minute = 0;
    }
    current.hour = static_cast<std::int8_t>(current.hour + increment.hour);
    if (current.hour > 23) {
        ++current.day;
        current.hour = 0;
    }
    current.day = static_cast<std::int8_t>(current.day + increment.day);
    const std::uint8_t month = static_cast<std::uint8_t>(current.month);
    const std::int8_t lastDay = (current.year & 3) == 0 && month == 2 ? 29 : month >= 13 ? 30 : daysInMonth[month];
    if (current.day > lastDay) {
        ++current.month;
        current.day = 1;
    }
    current.month = static_cast<std::int8_t>(current.month + increment.month);
    if (current.month > 12) {
        ++current.year;
        current.month = 1;
    }
    current.year = static_cast<std::int8_t>(current.year + increment.year);
    if (current.year > 99) current.year = 0;
    ComputeDayOfWeek(current);
}

// @verified - p
void ComputeDayOfWeek(DateTime& dateTime) {
    std::int16_t year = static_cast<std::int16_t>(dateTime.year + 1900);
    std::int8_t month = dateTime.month;
    if (month < 3) {
        year = static_cast<std::int16_t>(dateTime.year + 1899);
        month = static_cast<std::int8_t>(month + 12);
    }
    dateTime.week = static_cast<std::uint8_t>(
        (year / 400 + (13 * month + 8) / 5 - year / 100 + year +
         year / 4 + dateTime.day) % 7);
}

// @verified - p
std::int64_t AddDaysAndCalculateWeekday(DateTime& dateTime, std::int16_t days) {
    if (days > 0) {
        std::int8_t day = dateTime.day;
        for (std::int16_t remaining = days; remaining != 0; --remaining) {
            const std::int8_t year = dateTime.year;
            const std::int8_t month = dateTime.month;
            const std::int16_t lastDay = (year & 3) != 0 || month != 2 ? daysInMonth[static_cast<std::uint8_t>(month)] : 29;
            if (++day > lastDay) {
                day = 1;
                dateTime.month = static_cast<std::int8_t>(month + 1);
                if (dateTime.month > 12) {
                    dateTime.month = 1;
                    dateTime.year = static_cast<std::int8_t>(year + 1);
                    if (dateTime.year > 99) dateTime.year = 0;
                }
            }
        }
        dateTime.day = day;
    }
    std::int16_t year = static_cast<std::int16_t>(dateTime.year + 1900);
    std::int8_t month = dateTime.month;
    if (month < 3) {
        --year;
        month = static_cast<std::int8_t>(month + 12);
    }
    const std::int32_t weekday = year / 400 + (13 * month + 8) / 5 - year / 100 + year + year / 4 + dateTime.day;
    const std::int32_t wholeWeeks = 7 * (weekday / 7);
    dateTime.week = static_cast<std::uint8_t>(weekday - wholeWeeks);
    return static_cast<std::uint32_t>(wholeWeeks);
}

// @verified - p
std::int64_t AddDaysAndCalculateWeekdayFromCurrentDate(std::uint16_t days) {
    return AddDaysAndCalculateWeekday(g_currentDateTime, static_cast<std::int16_t>(days));
}


void shenGetDayInMonth(const DateTime& input, DateTime& output) {
    output.day = input.day;
    output.month = input.month;
    const std::int8_t year = input.year;
    const bool wrapped = --output.day < 1;
    output.year = year;
    if (!wrapped) return;
    if (--output.month < 1) {
        const bool underflow = --output.year < 0;
        output.month = 12;
        if (underflow) output.year = 99;
    }
    output.day = (output.year & 3) != 0 || output.month != 2 ? daysInMonth[static_cast<std::uint8_t>(output.month)] : 29;
}

// @verified - p
DateComparisonResult CompareDates(const DateTime& first, const DateTime& second) {
    const std::int32_t dateA = first.day + 64 * (first.month + 64 * first.year);
    const std::int32_t dateB = second.day + 64 * (second.month + 64 * second.year);
    if (dateA < dateB) return DateComparisonResult::LessThan;
    if (dateA > dateB) return DateComparisonResult::GreaterThan;
    const std::int32_t timeA = first.second + 64 * (first.minute + 64 * first.hour);
    const std::int32_t timeB = second.second + 64 * (second.minute + 64 * second.hour);
    if (timeA < timeB) return DateComparisonResult::LessThan;
    return timeA > timeB ? DateComparisonResult::GreaterThan
                         : DateComparisonResult::Equal;
}

// @verified - p
void SetCurrentDateTimeTo(DateTime& dateTime) {
    if (dateTime.second > 59) {
        ++dateTime.minute;
        dateTime.second = 0;
    }
    if (dateTime.minute > 59) {
        ++dateTime.hour;
        dateTime.minute = 0;
    }
    if (dateTime.hour > 23) {
        ++dateTime.day;
        dateTime.hour = 0;
    }
    const std::int8_t year = dateTime.year;
    std::int8_t month = dateTime.month;
    std::int8_t lastDay = 29;
    if ((dateTime.year & 3) != 0 || month != 2) {
        lastDay = static_cast<std::uint8_t>(month) >= 13U
                      ? 30 : daysInMonth[static_cast<std::uint8_t>(month)];
    }
    if (dateTime.day > lastDay) {
        dateTime.day = 1;
        dateTime.month = static_cast<std::int8_t>(month + 1);
    }
    if (dateTime.month > 12) {
        dateTime.month = 1;
        dateTime.year = static_cast<std::int8_t>(year + 1);
    }
    if (dateTime.year > 99) dateTime.year = 0;
    ComputeDayOfWeek(dateTime);
    g_currentDateTime = dateTime;
    milliseconds = 0.0f;
    ComputeDayOfWeek(g_currentDateTime);
}

// @verified - p
std::int64_t shenAdjustCurrentTimeByMinutes(std::int16_t minutes) {
    std::int32_t minute = g_currentDateTime.minute + minutes;
    std::int32_t hour = g_currentDateTime.hour;
    std::uint32_t result = static_cast<std::uint32_t>(minutes);
    std::uint32_t days = 0;
    if (minute > 59) {
        result = 0x88888889U * static_cast<std::uint32_t>(minute - 60);
        const std::int32_t carry = (minute - 60) / 60 + 1;
        minute -= 60 * carry;
        hour += carry;
    }
    if (minute < 0) {
        const std::int32_t carry = (-1 - minute) / 60 + 1;
        result = static_cast<std::uint32_t>(60 * carry);
        minute += 60 * carry;
        hour -= carry;
    }
    if (hour > 23) {
        result = 0xaaaaaaabU * static_cast<std::uint32_t>(hour - 24);
        days = static_cast<std::uint32_t>((hour - 24) / 24 + 1);
        hour -= 24 * static_cast<std::int32_t>(days);
    }
    if (hour < 0) {
        const std::int32_t carry = (-1 - hour) / 24;
        result = static_cast<std::uint32_t>(3 * carry);
        hour += 24 * carry + 24;
    }
    g_currentDateTime.minute = static_cast<std::int8_t>(minute);
    g_currentDateTime.hour = static_cast<std::int8_t>(hour);
    if (days != 0) return AddDaysAndCalculateWeekday(g_currentDateTime, static_cast<std::int16_t>(days));
    return result;
}

std::int64_t shenUpdateCurrentDateTime() {
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (dateTime.hour >= 8) ++dateTime.day;
    dateTime.hour = 8;
    dateTime.minute = 0;
    dateTime.second = 0;
    SetCurrentDateTimeTo(dateTime);
    EV_stepSetFlags(0, 100);
    EV_stepSetFlags(0, 0x12f);
    SelectPostFxBank(1, 1);
    UploadGlowParams(defaultGlowParameters);
    SFIL_UploadBloomParams(0);
    if (EV_GetShenmueFlag(0x430)) {
        dateTime.hour = 7;
        dateTime.minute = 45;
        dateTime.second = 0;
        SetCurrentDateTimeTo(dateTime);
    }
    return EV_setHoldStatesFlags(2);
}

void FormatLoadingScreenDate(std::span<char> output, std::int32_t year, std::int32_t month,
                            std::uint32_t day, std::int32_t week, std::uint8_t flags) {
    if (output.empty()) return;
    // ResetLoadingFlags admits English text only when no language table has
    // been selected yet. Subsequent calls use the canonical loaded language.
    if (g_sceneTextLanguage < 0) LoadLocalizedSceneText(0);
    constexpr const char* months[]{
        "UILOADMN00", "UILOADMN01", "UILOADMN02", "UILOADMN03", // @text UILOADMN00 EN: "Jan" | JA: "１月" ; UILOADMN01 EN: "Feb" | JA: "２月" ; UILOADMN02 EN: "Mar" | JA: "３月" ; UILOADMN03 EN: "Apr" | JA: "４月"
        "UILOADMN04", "UILOADMN05", "UILOADMN06", "UILOADMN07", // @text UILOADMN04 EN: "May" | JA: "５月" ; UILOADMN05 EN: "Jun" | JA: "６月" ; UILOADMN06 EN: "Jul" | JA: "７月" ; UILOADMN07 EN: "Aug" | JA: "８月"
        "UILOADMN08", "UILOADMN09", "UILOADMN10", "UILOADMN11"}; // @text UILOADMN08 EN: "Sep" | JA: "９月" ; UILOADMN09 EN: "Oct" | JA: "１０月" ; UILOADMN10 EN: "Nov" | JA: "１１月" ; UILOADMN11 EN: "Dec" | JA: "１２月"
    constexpr const char* weekdays[]{
        "UILOADSUN0", "UILOADMON0", "UILOADTUE0", "UILOADWED0", // @text UILOADSUN0 EN: "Sun." | JA: "日" ; UILOADMON0 EN: "Mon." | JA: "月" ; UILOADTUE0 EN: "Tue." | JA: "火" ; UILOADWED0 EN: "Wed." | JA: "水"
        "UILOADTHU0", "UILOADFRI0", "UILOADSAT0"}; // @text UILOADTHU0 EN: "Thu." | JA: "木" ; UILOADFRI0 EN: "Fri." | JA: "金" ; UILOADSAT0 EN: "Sat." | JA: "土"
    const std::int32_t shortYear = year > 1900 ? year - 1900 : year;
    const auto displayYear = static_cast<std::uint32_t>(
        shortYear + ((flags & 4U) != 0 ? 0 : 1900));
    const std::int32_t language = g_sceneTextLanguage;
    output[0] = 0;
    if (language == 1 || (language >= 4 && language <= 6)) {
        std::snprintf(output.data(), output.size(), "%u/%02u/%02u",
            displayYear, static_cast<std::uint32_t>(month), day);
    } else if ((flags & 1U) != 0) {
        const std::string_view monthText = GetLocalizedSceneText(months[month - 1]);
        if (language == 2 || language == 3) {
            std::snprintf(output.data(), output.size(),
                language == 2 ? "%02u %.*s %u" : "%02u. %.*s %u",
                day, static_cast<int>(monthText.size()), monthText.data(), displayYear);
        } else {
            std::snprintf(output.data(), output.size(), "%.*s. %02u, %u",
                static_cast<int>(monthText.size()), monthText.data(), day, displayYear);
        }
    } else {
        std::snprintf(output.data(), output.size(), "%02u/%02u/%u",
            day, static_cast<std::uint32_t>(month), displayYear);
    }
    if ((flags & 2U) != 0 && week >= 0) {
        const auto written = std::strlen(output.data());
        const auto weekday = GetLocalizedSceneText(weekdays[week]);
        std::snprintf(output.data() + written, output.size() - written, "%s(%.*s)",
            language >= 4 && language <= 6 ? "" : " ",
            static_cast<int>(weekday.size()), weekday.data());
    }
}
std::string FormatLoadingScreenDate(std::int32_t year, std::int32_t month,
                                    std::uint32_t day, std::int32_t week,
                                    std::uint8_t flags) {
    std::array<char, 512> output{};
    FormatLoadingScreenDate(output, year, month, day, week, flags);
    return output.data();
}
void initTimeProgram() {
    HLib::EnqueueTaskWithoutParameter(InitTimeProgram_Callback, 7, 0, static_cast<std::int32_t>(TASK_TIME.name));

    float seconds = defaultSecondsPerGameMinute;
    secondsPerGameMinute = seconds;
    timeUpdateAllowed = true;
    timeFrozen = false;
    canTickForFrame = false;
    framesPerSecond = 30;
    timeUpdateBlocked = false;

    if (UsesSlowGameClock(g_currentMap.name)) {
        seconds = 30.0f;
        secondsPerGameMinute = seconds;
    } else if (UsesFastGameClock(g_currentMap.name)) {
        seconds = 4.5f;
        secondsPerGameMinute = seconds;
    }
    realSecondsToGameMinute = 1.0f / seconds;
    realTimeScale = -1.0f;

    std::uint8_t update = 1;
    if (g_actNumber == 0) {
        update = 0;
        timeUpdateAllowed = false;
    } else if (g_actNumber == 1 &&
               (g_currentMap.name ==
                    SHEN_NAME(1000) ||
                g_currentMap.name ==
                    SHEN_NAME(2000))) {
        timeUpdateBlocked = true;
        update = 0;
        timeUpdateAllowed = false;
    }
    gameClockUpdateFlag = update;
    nextDateTime.year = 99;
    nextDateTime.month = 12;
    nextDateTime.day = 31;
    nextDateTime.hour = 23;
    nextDateTime.minute = 59;
    nextDateTime.second = 59;
    incrementedYear = false;
    incrementedMonth = false;
    incrementedDay = false;
    incrementedMinutes = false;
    incrementedSeconds = false;
    incrementedUnused = 0;
    ResetTimeHudRuntime();
}

std::string FormatLoadingScreenTime(std::int32_t hour, std::int32_t minute,
                                    std::int32_t meridiem) {
    if (g_sceneTextLanguage < 0) LoadLocalizedSceneText(0);
    const std::uint32_t clockHour = static_cast<std::uint32_t>(
        hour > 12 ? hour - 12 : hour);
    const std::string_view period = GetLocalizedSceneText(
        meridiem == 1 ? "UILOADPM00" : "UILOADAM00"); // @text UILOADPM00 EN: "pm" | JA: "午後" ; UILOADAM00 EN: "am" | JA: "午前"
    char output[512]{};
    if (g_sceneTextLanguage == 1) {
        std::snprintf(output, sizeof(output), "%.*s %2u:%02u",
            static_cast<int>(period.size()), period.data(), clockHour,
            static_cast<std::uint32_t>(minute));
    } else {
        std::snprintf(output, sizeof(output), "%2u:%02u %.*s",
            clockHour, static_cast<std::uint32_t>(minute),
            static_cast<int>(period.size()), period.data());
    }
    return output;
}

namespace {

std::int32_t DateSeconds(const DateTime& dateTime) {
    std::int32_t leapDays = (dateTime.year - 80) / 4;
    if ((((dateTime.year - 80) % 4) == 0) && dateTime.month <= 2) {
        --leapDays;
    }
    const std::int32_t days = leapDays + monthStartDays[dateTime.month - 1] + dateTime.day + 365 * (dateTime.year - 80);
    const std::uint32_t seconds = 60U * (static_cast<std::uint32_t>(dateTime.minute) +
        60U * (static_cast<std::uint32_t>(dateTime.hour) + 24U * static_cast<std::uint32_t>(days))) + static_cast<std::uint32_t>(dateTime.second);
    return std::bit_cast<std::int32_t>(seconds);
}

} // namespace
// @verified - p
std::int32_t ComputeSecondsSince1980(const DateTime& dateTime, std::int32_t* output) {
    const std::int32_t seconds = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(DateSeconds(dateTime)) - 86400U);
    if (output) *output = seconds;
    return seconds;
}


// @verified - p
void IncrementDateTimeBySeconds(std::uint32_t seconds, DateTime& dateTime) {
    const std::uint32_t hours = seconds / 3600U;
    dateTime.second = static_cast<std::int8_t>(seconds % 60U);
    dateTime.minute = static_cast<std::int8_t>((seconds / 60U) % 60U);
    dateTime.hour = static_cast<std::int8_t>(hours % 24U);
    const std::uint32_t years = hours / 24U / 365U;
    const std::int32_t yearDay = static_cast<std::int32_t>(hours / 24U % 365U);
    const std::int32_t leapDays = static_cast<std::int32_t>(years - 1U) / 4;
    std::int32_t dayOfYear = 0;
    if (leapDays > yearDay) {
        dateTime.year = static_cast<std::int8_t>(years + 79U);
        dayOfYear = 365 - leapDays + yearDay;
        if ((dateTime.year % 4) == 0) ++dayOfYear;
    } else {
        dayOfYear = yearDay - leapDays;
        dateTime.year = static_cast<std::int8_t>(years + 80U);
    }
    std::int32_t month = 1;
    const std::int32_t yearRemainder = dateTime.year % 4;
    if (yearRemainder != 0) {
        while (month < 12 && dayOfYear >= monthStartDays[month]) ++month;
    } else if (dayOfYear >= monthStartDays[1]) {
        for (month = 2; month < 12; ++month) {
            if (dayOfYear < monthStartDays[month] + 1) break;
        }
    }
    dateTime.month = static_cast<std::int8_t>(month);
    dateTime.day = static_cast<std::int8_t>(
        dayOfYear - monthStartDays[month - 1] + 1);
    if (yearRemainder == 0 && month > 2) --dateTime.day;
    ComputeDayOfWeek(dateTime);
}

// @verified - p
void DateTime_AddMinutesInPlace(DateTime& dateTime, std::int32_t minutes) {
    const std::uint32_t seconds = static_cast<std::uint32_t>(DateSeconds(dateTime)) + 60U * static_cast<std::uint32_t>(minutes) - 86400U;
    IncrementDateTimeBySeconds(seconds, dateTime);
}

// @verified - p
std::int32_t CompareDateTimeSeconds(const DateTime& first, const DateTime& second) {
    const std::uint32_t firstSeconds = static_cast<std::uint32_t>(DateSeconds(first)) - 86400U;
    const std::uint32_t secondSeconds = static_cast<std::uint32_t>(DateSeconds(second)) - 86400U;
    if (firstSeconds >= secondSeconds) return firstSeconds > secondSeconds;
    return -1;
}

// @verified - p
std::int32_t DiffMinutes(const DateTime& first, const DateTime& second) {
    const std::int32_t seconds = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(DateSeconds(first)) - static_cast<std::uint32_t>(DateSeconds(second)));
    return seconds / 60;
}

void InitCalendar() {
    EV_SetScenarioTaskFlags(5);
    g_currentDateTime = DateTime{87, 2, 23, 0, 12, 30, 0};
    currentCalendarReservedWord = 0;
    ComputeDayOfWeek(g_currentDateTime);

    currentTime = DateTime{87, 2, 23, 0, 12, 30, 0};
    savedCalendarReservedWord = 0;
    ComputeDayOfWeek(currentTime);

    nextDateTime = DateTime{87, 2, 23, 0, 12, 30, 0};
    nextCalendarReservedWord = 0;
    ComputeDayOfWeek(nextDateTime);

    realSecondsToGameMinute = 1.0f / 3.0f;
    realTimeScale = -1.0f;
    milliseconds = 0.0f;
    secondsPerGameMinute = 3.0f;
    defaultSecondsPerGameMinute = 3.0f;
    secondAccumulator = 0.0f;
    ResetSTEP();
    InitST_TitleFunc();
}

// @verified - p
void InitializeNewGameCalendar() {
    EV_SetScenarioTaskFlags(4);
    InitCalendar();
}

// @verified - p
std::uint64_t shenSetTimeSimulationTo(std::uint32_t state) {
    const std::uint8_t value = static_cast<std::uint8_t>(state);
    if (timeUpdateAllowed) gameClockUpdateFlag = value;
    return value;
}

bool IsTimeSimulationEnabled() {
    return gameClockUpdateFlag == 1;
}

void SetRenderClockAndCompassTo(std::int32_t suppressed) noexcept {
    clockAndCompassSuppressed = static_cast<std::uint8_t>(suppressed);
}

bool IsClockAndCompassSuppressed() noexcept {
    return clockAndCompassSuppressed != 0;
}

std::uint32_t UpdateTempHourMinTimeScale(std::int32_t hour,
                                         std::int32_t minute,
                                         float scale) {
    temporaryDay = g_currentDateTime.day;
    temporaryHour = hour;
    temporaryMinute = minute;
    realTimeScale = scale / 30.0f;
    return static_cast<std::uint32_t>(g_currentDateTime.day);
}

// @verified - p
std::int32_t IsCurrentTimeRange(std::uint16_t start, std::uint16_t end) {
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    std::int8_t hour = dateTime.hour;
    if (hour < 7) hour = static_cast<std::int8_t>(hour + 24);
    const std::uint16_t current = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(static_cast<std::int16_t>(dateTime.minute)) |
        (static_cast<std::uint16_t>(static_cast<std::int16_t>(hour)) << 8U));
    if (end >= start) {
        if (end < current || current < start) return 0;
    } else if (current < end || start < current) {
        return 0;
    }
    return 1;
}

void tmx_CtrlHandWatch(std::uint8_t nextFunctionIndex) {
    handWatch.state = 0;
    handWatch.armPosition.x = 0.0f;
    handWatch.armPosition.y = 0.0f;
    handWatch.armPosition.z = 0.0f;
    handWatch.armRotation = {};
    handWatch.task = nullptr;
    handWatch.flags = 0;
    handWatch.root = nullptr;
    handWatch.reader = nullptr;
    handWatch.task = HLib::EnqueueTaskWithoutParameter(
        tmx_CtrlHandWatch_Callback, nextFunctionIndex, 0,
        static_cast<std::int32_t>(TASK_TMEX.name));
    if (handWatch.task) HLib::SetupTimedTask(handWatch.task, 0);
}

bool IsWristwatchOpenable() {
    return handWatch.state != 0;
}

bool OpenWristwatch() {
    if (handWatch.state != 0 || !handWatch.task) return false;
    HLib::ResolveDeferredCallback(handWatch.task);
    handWatch.flags |= 9U;
    handWatch.state = 1;
    icsToggleIconState(1, 1);
    ShowElementByIndex(UIElement::WristwatchOverlay);
    return true;
}

// @verified - p
void RequestWristwatchFromHeldInput() {
    if (GetShouldRenderIcons(RenderIconSlot::ItemSelect) || handWatch.state != 0 || !handWatch.task) return;
    HLib::ResolveDeferredCallback(handWatch.task);
    handWatch.flags |= 9U;
    handWatch.state = 1;
    icsToggleIconState(1, 1);
    ShowElementByIndex(UIElement::WristwatchOverlay);
    handWatch.flags |= 2U;
}

void CloseWristwatch(std::int32_t immediate) {
    if (immediate) {
        if (handWatch.reader) {
            if (handWatchPlacementRegistered)
                d3t::RetireObjectPlacement(handWatch.root);
            ReleaseModelFormat7(*handWatch.reader);
            HLib::FreeTaskData(handWatchSource);
            handWatchSource = nullptr;
            handWatchPlacementRegistered = false;
        }
        handWatch.reader = nullptr;
        handWatch.root = nullptr;
        if (handWatch.flags & 0x20U) {
            MapControlData* const control = static_cast<MapControlData*>(HLib::GetTaskParameter(g_mapControlTask));
            Character* const player = control
                ? static_cast<Character*>(control->ryo) : nullptr;
            camCtrlSetTargetPosition(handWatch.savedCameraTarget);
            camCtrlSetNextPosition(handWatch.savedCameraPosition);
            camCtrlSetPerspective(static_cast<std::uint16_t>(handWatch.savedPerspective));
            camCtrlSetFov(static_cast<std::int16_t>(handWatch.savedFov));
            camCtrlSetState(handWatch.savedCameraState);
            if (player) {
                if (handWatch.savedPlayerDisplay) player->renderFlags |= 4U;
                else player->renderFlags &= static_cast<std::uint8_t>(~4U);
                if (handWatch.savedPlayerVisibility) player->flags |= 0x10U;
                else player->flags &= ~0x10U;
            }
            if (handWatch.savedHelpMode)
                SetHelpIconMenuFlags(handWatch.savedHelpVisible);
            else
                ResetHelpIconsToItemPrompt();
        }
        if (handWatch.task) HLib::SetupTimedTask(handWatch.task, 0);
        handWatch.flags = 0;
        handWatch.state = 0;
        icsToggleIconState(1, 0);
    } else if (handWatch.state) {
        handWatch.state = 3;
    }
    DestroyElementByIndex(UIElement::WristwatchOverlay);
}

void SetCtrlHandWatchFlags(std::int32_t enabled) {
    if (enabled) handWatch.flags |= 4U;
    else handWatch.flags &= ~4U;
}

} // namespace shenmue::s2
