#include "s2/area_jump.hpp"

#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/ui.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
namespace {

union AreaJumpDestination {
    shen_name areaId;
    std::array<std::int8_t, 4> candidateIndices;
    std::uint32_t specialMode;
};

struct AreaJumpDestinationEntry {
    std::int32_t speakIconFlag;
    AreaJumpDestination destination;
    std::int32_t destinationEntryId;
};

struct AreaJumpScheduleEntry {
    std::array<std::int16_t, 4> destinationIndices;
    std::int32_t requiredShenmueFlag;
};

struct CtrlAreaJumpEntry {
    shen_name sourceAreaId;
    float startCheckHeight;
    float startX;
    float startY;
    float startZ;
    float endCheckHeight;
    float endX;
    float endY;
    float endZ;
    float checkRangeRadiusOrYDelta;
    shen_name requiredAuthorId;
    const AreaJumpScheduleEntry *scheduleData;
};

struct AreaEntryLocation {
    shen_name areaId;
    std::int32_t entryId;
    Vector2 position;
};

static_assert(sizeof(AreaJumpDestination) == 4);
static_assert(sizeof(AreaJumpDestinationEntry) == 0x0c);
static_assert(sizeof(AreaJumpScheduleEntry) == 0x0c);
static_assert(sizeof(CtrlAreaJumpEntry) == 0x38);
static_assert(offsetof(CtrlAreaJumpEntry, scheduleData) == 0x30);
static_assert(sizeof(AreaEntryLocation) == 0x10);

enum class CtrlAreaJumpState : std::uint8_t {
    LocateEntry = 0,
    PrimaryCheck = 1,
    SecondaryCheck = 2,
    ValidateGameState = 3,
    LoadScheduleEntry = 4,
    WaitForIconMatch = 5,
    WaitForAltIconMatch = 6,
    ExecuteWarp = 7,
    AbortAndReset = 8,
    Done = 9,
};

constexpr AreaJumpScheduleEntry ar01Schedule[]{
    {{0, 15, 16, 0}, 495},
    {{0, 0, 0, 0}, -1},
};

constexpr AreaJumpScheduleEntry wsy0Schedule[]{
    {{15, 1, 3, 20}, 182},
    {{15, 1, 10, 20}, 183},
    {{15, 7, 10, 20}, 184},
    {{0, 15, 16, 0}, 186},
    {{15, 6, 7, 20}, 210},
    {{0, 1, 20, 15}, 791},
    {{15, 27, 10, 20}, 1149},
    {{0, 0, 0, 0}, -1},
};

constexpr AreaJumpScheduleEntry ab00Schedule[]{
    {{15, 4, 14, 20}, 187},
    {{2, 6, 14, 20}, 188},
    {{15, 6, 9, 20}, 189},
    {{0, 0, 0, 0}, -1},
};

constexpr AreaJumpScheduleEntry qr00Schedule[]{
    {{0, 15, 16, 0}, 492},
    {{0, 24, 20, 0}, 493},
    {{15, 18, 17, 20}, 494},
    {{0, 0, 0, 0}, -1},
};

constexpr AreaJumpScheduleEntry ak00Schedule[]{
    {{0, 21, 20, 0}, 522},
    {{0, 0, 0, 0}, -1},
};

constexpr AreaJumpScheduleEntry wb00Schedule[]{
    {{0, 15, 16, 0}, 523},
    {{0, 15, 16, 0}, 524},
    {{0, 0, 0, 0}, -1},
};

constexpr AreaJumpScheduleEntry we00Schedule[]{
    {{0, 26, 20, 0}, 1150},
    {{0, 0, 0, 0}, -1},
};

constexpr AreaJumpDestinationEntry areaJumpDestinations[]{
    {-1, {.specialMode = 0}, 0},
    {165, {.areaId = SHEN_NAME(AR03)}, 3},
    {165, {.candidateIndices = {0, 3, 4, 0}}, -1},
    {166, {.areaId = SHEN_NAME(AR02)}, 25},
    {167, {.areaId = SHEN_NAME(AK00)}, 1},
    {168, {.areaId = SHEN_NAME(AB00)}, 95},
    {169, {.areaId = SHEN_NAME(WB00)}, 1},
    {170, {.areaId = SHEN_NAME(WT00)}, 1},
    {170, {.areaId = SHEN_NAME(WT00)}, 80},
    {171, {.areaId = SHEN_NAME(WTA0)}, 1},
    {172, {.areaId = SHEN_NAME(WK00)}, 1},
    {172, {.areaId = SHEN_NAME(WK00)}, 3},
    {173, {.areaId = SHEN_NAME(WS00)}, 79},
    {173, {.areaId = SHEN_NAME(WS00)}, 2},
    {174, {.candidateIndices = {7, 10, 28, 29}}, -1},
    {233, {.specialMode = 2}, 0},
    {253, {.specialMode = 1}, 0},
    {249, {.areaId = SHEN_NAME(Q100)}, 1},
    {250, {.areaId = SHEN_NAME(Q200)}, 6},
    {251, {.areaId = SHEN_NAME(QR00)}, 80},
    {253, {.specialMode = 0}, 0},
    {174, {.areaId = SHEN_NAME(WSY0)}, 50},
    {255, {.areaId = SHEN_NAME(WK00)}, 32},
    {256, {.areaId = SHEN_NAME(WN00)}, 28},
    {233, {.areaId = SHEN_NAME(QWG0)}, 0},
    {165, {.areaId = SHEN_NAME(AR02)}, 26},
    {254, {.areaId = SHEN_NAME(WSY0)}, 50},
    {287, {.areaId = SHEN_NAME(WE00)}, 77},
    {258, {.areaId = SHEN_NAME(WN00)}, 3},
    {173, {.areaId = SHEN_NAME(WS00)}, 1},
};

constexpr CtrlAreaJumpEntry areaJumps[]{
    {SHEN_NAME(WSY0), 66.5f, 121.0f, 64.5f, 120.5f, 63.5f, 121.0f, 66.5f, 122.0f, 98.199997f, {}, wsy0Schedule},
    {SHEN_NAME(AB00), 163.5f, 536.5f, 163.5f, 538.5f, 164.5f, 536.5f, 164.5f, 538.5f, 95.300003f, {}, ab00Schedule},
    {SHEN_NAME(AR01), 176.5f, 308.0f, 176.5f, 302.5f, 176.5f, 308.0f, 173.5f, 308.0f, 100.2f, {}, ar01Schedule},
    {SHEN_NAME(QR00), 109.0f, 152.5f, 107.5f, 152.5f, 109.0f, 152.5f, 109.0f, 150.5f, 92.0f, {}, qr00Schedule},
    {SHEN_NAME(AK00), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, SHEN_NAME(AKB0), ak00Schedule},
    {SHEN_NAME(WB00), 334.5f, 277.0f, 337.0f, 277.0f, 334.5f, 277.5f, 337.0f, 277.5f, 127.0f, {}, wb00Schedule},
    {SHEN_NAME(WE00), 150.3f, 98.099998f, 4.0f, 999.90002f, 150.3f, 98.099998f, 5.0f, 999.90002f, 102.0f, {}, we00Schedule},
    {},
};

constexpr AreaEntryLocation areaEntryLocations[]{
    {SHEN_NAME(AR01), 86, {179.31f, 308.91f}},
    {SHEN_NAME(WSY0), 79, {69.860001f, 120.57f}},
    {SHEN_NAME(WTA0), 80, {230.75f, 228.0f}},
    {SHEN_NAME(AB00), 95, {120.19f, 549.31f}},
    {SHEN_NAME(QRR0), 80, {107.1f, 154.3f}},
    {SHEN_NAME(WS00), 40, {65.199997f, 124.2f}},
    {SHEN_NAME(WB00), 40, {336.70001f, 272.39999f}},
    {SHEN_NAME(WTA0), 41, {230.75f, 228.0f}},
    {SHEN_NAME(WTA0), 40, {230.75f, 228.0f}},
    {SHEN_NAME(YMQB), 77, {258.04001f, 338.60001f}},
    {SHEN_NAME(TOFB), 0, {217.74001f, 213.17f}},
    {SHEN_NAME(WN00), 40, {266.5f, 270.60001f}},
    {SHEN_NAME(SYW0), 0, {330.79001f, 234.38f}},
    {SHEN_NAME(QR00), 40, {116.94f, 172.97f}},
    {SHEN_NAME(QR00), 41, {116.94f, 172.97f}},
    {SHEN_NAME(QWG0), 40, {155.48f, 185.98f}},
    {},
};

AreaJumpPersistentState persistentState{};
SavedAreaJumpLocation savedAreaJumpLocation{};
std::array<std::byte, 0x20> areaJumpPlacement{};
std::int32_t canDoAreaJump{};
std::int32_t areaJumpBlocked{};
bool savedAreaJumpLocationActive{};
bool savedAreaJumpLocationRequested{};

std::uint8_t g_unusedCtrlFlag{};
std::int32_t retainedMagicRoomFlag{};
CtrlAreaJumpState ctrlAreaJumpState{CtrlAreaJumpState::LocateEntry};
const CtrlAreaJumpEntry *activeJumpEntry{areaJumps};
const AreaJumpScheduleEntry *initialScheduleTable{};
const AreaJumpScheduleEntry *currentScheduleEntry{};
const AreaJumpDestination *alternateScheduleTable{};
std::int32_t selectedDestinationIndex{-1};
Vector3 currentCheckPosition{};
Vector3 lastCheckPosition{};

void SetPreviousDay(DateTime &dateTime) {
    if (--dateTime.day >= 1)
        return;
    if (--dateTime.month < 1) {
        if (--dateTime.year < 0)
            dateTime.year = 99;
        dateTime.month = 12;
    }
    constexpr std::array<std::int8_t, 13> daysInMonth{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    dateTime.day = dateTime.month == 2 && (dateTime.year & 3) == 0 ? 29 : daysInMonth[static_cast<std::size_t>(dateTime.month)];
}

bool CheckSimpleXZDistanceRange(float targetX, float targetZ, float radius, float targetY) {
    float nearDistance = 0.0f;
    float farDistance = 0.0f;
    const float lastZ = targetZ - lastCheckPosition.z;
    const float currentX = targetX - currentCheckPosition.x;
    const float currentZ = targetZ - currentCheckPosition.z;
    const float radiusSquared = radius * radius;
    if (std::abs(targetY - currentCheckPosition.y) < 1.0f && lastZ > 0.0f && currentZ > 0.0f) {
        const float lastX = targetX - lastCheckPosition.x;
        nearDistance = lastZ * lastZ + lastX * lastX;
        farDistance = currentX * currentX + currentZ * currentZ;
        if (nearDistance > farDistance) {
            nearDistance = currentX * currentX + currentZ * currentZ;
            farDistance = lastZ * lastZ + lastX * lastX;
        }
    }
    return radiusSquared > nearDistance && farDistance >= radiusSquared;
}

bool CheckLineSegmentXZIntersection(float firstX, float firstZ, float secondX, float secondZ, float targetY) {
    const float movementZ = lastCheckPosition.z - currentCheckPosition.z;
    const float movementX = lastCheckPosition.x - currentCheckPosition.x;
    const float firstOffsetX = firstX - currentCheckPosition.x;
    const float segmentZ = secondZ - firstZ;
    const float firstOffsetZ = firstZ - currentCheckPosition.z;
    const float divisor = movementZ * (secondX - firstX) - movementX * segmentZ;
    if (std::abs(targetY - currentCheckPosition.y) >= 1.0f || (divisor <= 0.00001f && divisor >= -0.00001f))
        return false;

    float segmentFraction = (firstOffsetZ * movementX - firstOffsetX * movementZ) / divisor;
    if (segmentFraction >= 1.00001f || segmentFraction <= -0.00001f)
        return false;
    if (movementX > 0.00001f || movementX < -0.00001f)
        segmentFraction = (segmentFraction * (secondX - firstX) + firstOffsetX) / movementX;
    else if (movementZ > 0.00001f || movementZ < -0.00001f)
        segmentFraction = (segmentFraction * segmentZ + firstOffsetZ) / movementZ;
    else
        segmentFraction = -1.0f;
    return segmentFraction < 1.00001f && segmentFraction > -0.00001f;
}

bool CheckJumpBoundary(const CtrlAreaJumpEntry &entry, bool secondary) {
    const float firstX = secondary ? entry.endCheckHeight : entry.startCheckHeight;
    const float firstZ = secondary ? entry.endX : entry.startX;
    const float secondX = secondary ? entry.endY : entry.startY;
    const float secondZ = secondary ? entry.endZ : entry.startZ;
    if (secondZ <= 999.0f)
        return CheckLineSegmentXZIntersection(firstX, firstZ, secondX, secondZ, entry.checkRangeRadiusOrYDelta);
    return CheckSimpleXZDistanceRange(firstX, firstZ, secondX, entry.checkRangeRadiusOrYDelta);
}

bool AreaJumpWarpCompatible() {
    DateTime now{};
    GetCurrentDateTime(now);
    if (now.hour < 7)
        SetPreviousDay(now);
    SetPreviousDay(now);
    if (persistentState.capturedDay != static_cast<std::uint16_t>(static_cast<std::uint8_t>(now.day)))
        return false;

    const shen_name mapName = GetMapRegionName(persistentState.returnPosition);
    switch (g_currentMap.name) {
    case SHEN_NAME(AB00):
        return mapName.name != SHEN_NAME(AB20);
    case SHEN_NAME(QR00):
        return mapName != AREA_QRR0;
    case SHEN_NAME(WTA0):
        return mapName != AREA_WTA0;
    case SHEN_NAME(WSY0):
        return mapName != AREA_WSY0;
    case SHEN_NAME(AR01):
        return mapName.name != SHEN_NAME(ARK0);
    default:
        return true;
    }
}

std::int16_t GetAreaJumpSpeakIconFlag(std::int32_t destinationIndex) {
    if (destinationIndex < 0 || destinationIndex >= static_cast<std::int32_t>(std::size(areaJumpDestinations)))
        return -1;
    const AreaJumpDestinationEntry &destination = areaJumpDestinations[static_cast<std::size_t>(destinationIndex)];
    if (destination.destinationEntryId < 0 || destination.destination.specialMode - 1U > 1U || (persistentState.warpEnabled != 0 && AreaJumpWarpCompatible())) {
        return static_cast<std::int16_t>(destination.speakIconFlag);
    }
    return -1;
}

bool SetupAreaJumpChoices(const std::array<std::int16_t, 4> &indices) {
    std::array<std::int16_t, 4> choices{};
    bool hasChoice = false;
    for (std::size_t index = 0; index < choices.size(); ++index) {
        choices[index] = GetAreaJumpSpeakIconFlag(indices[index]);
        if (choices[index] >= 0)
            hasChoice = true;
    }
    return hasChoice && SetupControlSpeakAndUI_Internal(choices.data(), 10800, -1, 2, 4) != 0;
}

std::int32_t FindSelectedDestination(std::uint16_t icon, const std::array<std::int16_t, 4> &indices) {
    for (std::int16_t index : indices) {
        if (index >= 0 && index < static_cast<std::int16_t>(std::size(areaJumpDestinations)) &&
            static_cast<std::uint16_t>(areaJumpDestinations[static_cast<std::size_t>(index)].speakIconFlag) == icon)
            return index;
    }
    return -1;
}

const AreaEntryLocation *FindAreaEntryLocation(shen_name areaId, std::int32_t entryId) {
    for (const AreaEntryLocation &location : areaEntryLocations) {
        if (location.areaId.name == 0)
            break;
        if (location.areaId.name == areaId.name && location.entryId == entryId)
            return &location;
    }
    return nullptr;
}

bool FindMapEventEntryLocation(shen_name areaId, std::int32_t entryId, Vector3 &position) {
    for (const MapEventAreaDefinition *area = GetMapEventAreaDefinitions(); area->areaId.name != 0xffffffffU; ++area) {
        if (area->areaId != areaId)
            continue;
        for (const MapEventTransition *entry = area->transitions; entry && entry->eventId != -1; ++entry) {
            if (entry->eventId != entryId)
                continue;
            position = entry->position;
            return true;
        }
    }
    return false;
}

void AdvanceTimeForDistance(const Vector2 &destination) {
    const float x = currentCheckPosition.x - destination.x;
    const float z = currentCheckPosition.z - destination.y;
    shenAdjustCurrentTimeByMinutes(static_cast<std::int16_t>(std::sqrt(z * z + x * x) * 0.22222222f));
}

void AbortAreaJump() {
    ctrlAreaJumpState = CtrlAreaJumpState::AbortAndReset;
}

void ExecuteAreaJump() {
    if (selectedDestinationIndex <= 0 || selectedDestinationIndex >= static_cast<std::int32_t>(std::size(areaJumpDestinations))) {
        AbortAreaJump();
        return;
    }

    const AreaJumpDestinationEntry &destination = areaJumpDestinations[static_cast<std::size_t>(selectedDestinationIndex)];
    if (destination.destinationEntryId < 0) {
        alternateScheduleTable = &destination.destination;
        if (SetupAreaJumpChoices({alternateScheduleTable->candidateIndices[0], alternateScheduleTable->candidateIndices[1],
                alternateScheduleTable->candidateIndices[2], alternateScheduleTable->candidateIndices[3]})) {
            ctrlAreaJumpState = CtrlAreaJumpState::WaitForAltIconMatch;
        } else {
            ctrlAreaJumpState = CtrlAreaJumpState::LoadScheduleEntry;
        }
        return;
    }

    if (destination.destination.specialMode <= 1U) {
        AbortAreaJump();
        return;
    }

    if (destination.destination.specialMode == 2U) {
        scenSetTaskFlags(0);
        AdvanceTimeForDistance({persistentState.returnPosition.x, persistentState.returnPosition.z});
        GetCurrentDateTime(persistentState.capturedTime);
        doorQueueAuthorForMap(persistentState.authorId);
        SetTemporaryAreaJumpAuthor(persistentState.authorId);
        SetQronEntryFloor(persistentState.qronFloor);
        retainedMagicRoomFlag = persistentState.magicRoomFlag;
        SavedAreaJumpLocation location;
        std::memcpy(&location, &persistentState, sizeof(location));
        RestoreSavedAreaJumpLocation(location, persistentState.playerPlacement);
        ActivateSavedAreaJumpLocation();
        shenQueueChangeModuleAndScene(SystemModule::Game, persistentState.sceneActId, persistentState.sceneAreaId, persistentState.sceneEntryId);
    } else {
        HLib::Task *const filmTask = GetCurrentAreaScreenFilmTask();
        if (filmTask)
            WaitForNextTask_0(filmTask, 1, 0);
        scenSetTaskFlags(0);
        Vector3 position;
        if (FindMapEventEntryLocation(destination.destination.areaId, destination.destinationEntryId, position)) {
            AdvanceTimeForDistance({position.x, position.z});
        }
        shenQueueChangeModuleAndScene(SystemModule::Game, g_actNumber, destination.destination.areaId, destination.destinationEntryId);
    }
    ctrlAreaJumpState = CtrlAreaJumpState::Done;
    persistentState.warpEnabled = 0;
}

void CtrlAreaJump_Callback(void *) {
    lastCheckPosition = currentCheckPosition;
    plymUpdatePlayerSearchLocation(&currentCheckPosition);

    switch (ctrlAreaJumpState) {
    case CtrlAreaJumpState::LocateEntry:
        activeJumpEntry = areaJumps;
        while (activeJumpEntry->sourceAreaId.name != 0 && activeJumpEntry->sourceAreaId.name != g_currentMap.name)
            ++activeJumpEntry;
        ctrlAreaJumpState = activeJumpEntry->sourceAreaId.name == 0 ? CtrlAreaJumpState::Done : CtrlAreaJumpState::PrimaryCheck;
        if (g_currentMap.name == SHEN_NAME(QR00) && g_entryPoint != 99)
            ctrlAreaJumpState = CtrlAreaJumpState::Done;
        break;
    case CtrlAreaJumpState::PrimaryCheck:
        if (activeJumpEntry->requiredAuthorId.name != 0) {
            if (activeJumpEntry->requiredAuthorId.name == GetCurrentAuthorId().name)
                ctrlAreaJumpState = CtrlAreaJumpState::SecondaryCheck;
        } else if (CheckJumpBoundary(*activeJumpEntry, false)) {
            ctrlAreaJumpState = CtrlAreaJumpState::SecondaryCheck;
        }
        break;
    case CtrlAreaJumpState::SecondaryCheck: {
        const bool admitted = activeJumpEntry->requiredAuthorId.name != 0 ? activeJumpEntry->requiredAuthorId.name != GetCurrentAuthorId().name :
                                                                            CheckJumpBoundary(*activeJumpEntry, true);
        if (admitted)
            ctrlAreaJumpState = CtrlAreaJumpState::ValidateGameState;
        break;
    }
    case CtrlAreaJumpState::ValidateGameState:
        if (authGetLoaded() == 0) {
            ctrlAreaJumpState = CtrlAreaJumpState::AbortAndReset;
            if (!srchHasActiveLockonTarget() && srchGetCurrentFocus().name == 0 && !IsScenarioBusy()) {
                ctrlAreaJumpState = CtrlAreaJumpState::ValidateGameState;
                HLib::Task *const filmTask = GetCurrentAreaScreenFilmTask();
                if (plymIsSearchable() && !plymIsSearchBlocked() && GetParentMenu() != 0 && !IsItemSelectable() && IsCtrlSpeakIconInactive() &&
                    IsSaveAllowed() == 0 && !IsEasyOptionActive() && !IsAnyItemSelectActive() && !IsWristwatchOpenable() && IsWazamakiInactive() == 0 &&
                    filmTask && IsScreenFilmNotShowing(filmTask)) {
                    initialScheduleTable = activeJumpEntry->scheduleData;
                    ctrlAreaJumpState = CtrlAreaJumpState::LoadScheduleEntry;
                }
            }
        }
        if (ctrlAreaJumpState != CtrlAreaJumpState::LoadScheduleEntry)
            break;
        [[fallthrough]];
    case CtrlAreaJumpState::LoadScheduleEntry:
        currentScheduleEntry = initialScheduleTable;
        while (currentScheduleEntry->requiredShenmueFlag > 0 && EV_GetShenmueFlag(currentScheduleEntry->requiredShenmueFlag) == 0)
            ++currentScheduleEntry;
        if (currentScheduleEntry->requiredShenmueFlag < 0 || !SetupAreaJumpChoices(currentScheduleEntry->destinationIndices)) {
            AbortAreaJump();
            break;
        }
        SetPlayerControlEnabled(0);
        shenToggleAreaFlags(1);
        ctrlAreaJumpState = CtrlAreaJumpState::WaitForIconMatch;
        break;
    case CtrlAreaJumpState::WaitForIconMatch:
        if (IsCtrlSpeakIconInactive()) {
            selectedDestinationIndex =
                FindSelectedDestination(static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()), currentScheduleEntry->destinationIndices);
            ctrlAreaJumpState = selectedDestinationIndex > 0 ? CtrlAreaJumpState::ExecuteWarp : CtrlAreaJumpState::AbortAndReset;
        }
        break;
    case CtrlAreaJumpState::WaitForAltIconMatch:
        if (IsCtrlSpeakIconInactive()) {
            selectedDestinationIndex = FindSelectedDestination(static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()),
                {alternateScheduleTable->candidateIndices[0], alternateScheduleTable->candidateIndices[1], alternateScheduleTable->candidateIndices[2],
                    alternateScheduleTable->candidateIndices[3]});
            ctrlAreaJumpState = selectedDestinationIndex > 0 ? CtrlAreaJumpState::ExecuteWarp : CtrlAreaJumpState::LoadScheduleEntry;
        }
        break;
    case CtrlAreaJumpState::ExecuteWarp:
        ExecuteAreaJump();
        break;
    case CtrlAreaJumpState::AbortAndReset:
        if (!plymIsSearchable()) {
            shenToggleAreaFlags(0);
            SetPlayerControlEnabled(1);
        }
        ctrlAreaJumpState =
            activeJumpEntry->sourceAreaId.name == 0 || g_currentMap.name == SHEN_NAME(QR00) ? CtrlAreaJumpState::Done : CtrlAreaJumpState::PrimaryCheck;
        break;
    case CtrlAreaJumpState::Done:
        break;
    }
}

} // namespace

HLib::Task *initCtrlAreaJump() {
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(CtrlAreaJump_Callback, 8, 2, static_cast<std::int32_t>(SHEN_NAME(ASJP)));
    ctrlAreaJumpState = CtrlAreaJumpState::LocateEntry;
    currentCheckPosition = {};
    return task;
}

// @verified - p
std::int64_t ResetAreaJumpWarpFlag() noexcept {
    persistentState.warpEnabled = 0;
    return 0;
}

void CaptureCurrentAreaJumpState() {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (Character *const character = static_cast<Character *>(control->ryo)) {
        persistentState.returnPosition = GetCharacterWorldPosition(character);
        persistentState.returnYaw = GetAllAngYForNode(&character->entity.node);
        persistentState.playerInternalState = static_cast<std::uint8_t>(character->internalState);
        persistentState.playerPlacement = areaJumpPlacement;
    } else {
        persistentState.returnPosition = {};
        persistentState.returnYaw = 0;
        persistentState.playerInternalState = 0;
        persistentState.reserved11 = {};
        persistentState.capturedTime = {};
        persistentState.reserved2b = {};
    }
    persistentState.sceneActId = g_actNumber;
    persistentState.sceneAreaId = g_currentMap;
    persistentState.sceneEntryId = g_entryPoint;
    persistentState.warpEnabled = EV_CanDoAreaJump() ? 1U : 0U;
    DateTime day{};
    GetCurrentDateTime(day);
    if (day.hour < 7)
        SetPreviousDay(day);
    persistentState.capturedDay = static_cast<std::uint8_t>(day.day);
    persistentState.authorId = GetCurrentAuthorId();
    persistentState.qronFloor = static_cast<std::int32_t>(GetQronNextStream());
    persistentState.magicRoomFlag = retainedMagicRoomFlag;
}

void AdvanceAreaJumpTimeForDestination(shen_name areaId, std::int32_t entryId) noexcept {
    Vector3 position;
    if (!FindMapEventEntryLocation(areaId, entryId, position)) {
        const AreaEntryLocation *const location = FindAreaEntryLocation(areaId, entryId);
        if (!location)
            return;
        position.x = location->position.x;
        position.z = location->position.y;
    }
    const float x = persistentState.returnPosition.x - position.x;
    const float z = persistentState.returnPosition.z - position.z;
    shenAdjustCurrentTimeByMinutes(static_cast<std::int16_t>(std::sqrt(z * z + x * x) * 0.22222222f));
}

void CaptureAreaJumpPersistentState(std::span<std::byte, sizeof(AreaJumpPersistentState)> output) noexcept {
    std::memcpy(output.data(), &persistentState, sizeof(persistentState));
}

void RestoreAreaJumpPersistentState(std::span<const std::byte, sizeof(AreaJumpPersistentState)> input) noexcept {
    std::memcpy(&persistentState, input.data(), sizeof(persistentState));
    retainedMagicRoomFlag = persistentState.magicRoomFlag;
}

// @verified - p
bool GetSavedAreaJumpLocation(Vector3 &position, std::int32_t &yaw) noexcept {
    position = savedAreaJumpLocation.position;
    yaw = savedAreaJumpLocation.yaw;
    return savedAreaJumpLocationActive;
}

// @verified - p
std::uint64_t HasSavedAreaJumpLocation() noexcept {
    return savedAreaJumpLocationActive ? 0xffffffffULL : 0;
}

// @verified - p
void ResetShenmueFlags() noexcept {
    savedAreaJumpLocationRequested = false;
    savedAreaJumpLocationActive = false;
}

// @verified - p
void ResetGlobalStateFlags() noexcept {
    savedAreaJumpLocationActive = false;
}


void SetUnusedFlag() noexcept {
    g_unusedCtrlFlag = 0;
}

shen_name GetAreaJumpLoadAuthor() noexcept {
    return persistentState.authorId;
}

std::int32_t GetRetainedMagicRoomFlag() noexcept {
    return retainedMagicRoomFlag;
}

void SetRetainedMagicRoomFlag(std::int32_t flag) noexcept {
    retainedMagicRoomFlag = flag;
}

// @verified - p
void RestoreSavedAreaJumpLocation(const SavedAreaJumpLocation &location, std::span<const std::byte, 0x20> placement) noexcept {
    savedAreaJumpLocation = location;
    std::memcpy(areaJumpPlacement.data(), placement.data(), areaJumpPlacement.size());
}

// @verified - p
void ActivateSavedAreaJumpLocation() noexcept {
    savedAreaJumpLocationRequested = true;
    savedAreaJumpLocationActive = true;
}

// @verified - p
std::uint64_t GetAreaJumpPlacementByte(std::int32_t offset) noexcept {
    return std::to_integer<std::uint8_t>(areaJumpPlacement.data()[offset]);
}

// @verified - p
std::int64_t SetAreaJumpPlacementByte(std::int32_t offset, std::uint8_t value) noexcept {
    areaJumpPlacement.data()[offset] = static_cast<std::byte>(value);
    return offset;
}

// @verified - p
float GetAreaJumpPlacementFloat(std::int32_t offset) noexcept {
    float value;
    std::memcpy(&value, areaJumpPlacement.data() + offset, sizeof(value));
    return value;
}

// @verified - p
std::uint64_t SetAreaJumpPlacementFloat(std::int32_t offset, float value) noexcept {
    std::memcpy(areaJumpPlacement.data() + offset, &value, sizeof(value));
    return std::bit_cast<std::uint32_t>(value);
}

// @verified - p
void EV_SetCanDoAreaJumpTo(std::int32_t enabled) {
    canDoAreaJump = enabled;
}

// @verified - p
std::uint64_t EV_CanDoAreaJump() {
    return areaJumpBlocked ? 0 : static_cast<std::uint32_t>(canDoAreaJump);
}

// @verified - p
void EV_ApplyTranslationClockVisibility(std::uint32_t) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = static_cast<Character *>(control->ryo);
    if (!character)
        return;
    shenApplyTranslationToAll(&character->entity.node, savedAreaJumpLocation.position);
    shenDecrementRotYAllNodes(&character->entity.node, static_cast<std::int16_t>(savedAreaJumpLocation.yaw));
    SetCharacterInternalState(character, savedAreaJumpLocation.internalState);
    const CameraState state = camCtrlGetState();
    camCtrlSetState(CameraState::Event);
    camCtrlSetState(state);
    camCtrlSetFovTransitionTo(true);
    savedAreaJumpLocationActive = false;
}

} // namespace shenmue::s2
