#include "s2/search.hpp"
#include "s2/search_internal.hpp"
#include "s2/pak.hpp"
#include "s2/world_render.hpp"
#include <new>

#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/hand.hpp"
#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/mt7.hpp"
#include "s2/flag.hpp"
#include "s2/inventory.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"

#include <cmath>
#include <bit>
#include <cstdio>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
namespace {

struct SearchTemporaryModel : MT7Model {
    void *sourceAllocation{};
};
}
SearchPropIKSlot g_searchPropIKSlots[4]{};
MT7Model *g_searchTemporaryModel{};
shen_name g_searchLastSignboard{};

HLib::Task *searchTask{};
SearchTaskData *searchControl{};
shen_name focusObject{};
shen_name currentFollowCharacter{};
Vector3 currentFollowPosition{};
Vector3 followPosition{};
Vector3 focusPosition{};
Vector3 playerSearchPosition{};
Vector3 anchorPosition{};
shen_name anchorCharacter{};
Vector3 selectedSearchPosition{};
const Vector3 *publishedSearchPosition{};
shen_name selectedSearchName{};
SearchSavedState savedPlayerState{};
std::byte *activeSearchConfig{};
std::array<std::span<std::byte>, 3> resourceBytes{};
std::byte *sceneSearchAllocation{};
std::array<SearchTarget, 256> staticTargets{};
std::array<std::uint8_t, 16> inputHistory{};
std::uint8_t inputHistoryIndex{};
std::uint8_t lastInputX{};
std::uint8_t lastInputY{};
std::uint16_t lastDirectionInput{};
std::uint32_t searchExitRequested{};
std::uint32_t lightConfigurationChanged{};
std::array<Vector3, 32> scenarioAdmissionPositions{};
Vector3 scenarioPlayerPosition{};
Vector3 scenarioFocusPosition{};
Vector3 scenarioLockonPosition{};
std::int16_t scenarioPlayerYaw{};
bool scenarioHasLockonPosition{};
Matrix4x4 projectionPrePassMatrix{};
float searchStep{};
float searchRadius{};
float searchTemporaryFrame{};
Vector3 searchTemporaryPosition{};
std::uint32_t searchTemporaryYaw{};
alignas(16) std::array<SnapshotSearchCandidate, 65> snapshotCandidates{};
std::int32_t searchPropIKIndex{};
alignas(16) std::byte searchPropIKTransformData[4][384]{};
alignas(16) std::byte searchPropIKAuxiliaryData[4][256]{};
std::int32_t searchPropIKResult{};
std::array<std::byte, 18> searchPropIKSelection{};

namespace {

Vector3 TransformSearchPoint(const Matrix4x4 &matrix, const Vector3 &point) {
    const auto &rows = matrix.rows;
    return {((point.y * rows[1].x + point.x * rows[0].x) + point.z * rows[2].x) + rows[3].x,
        ((point.y * rows[1].y + point.x * rows[0].y) + point.z * rows[2].y) + rows[3].y,
        ((point.y * rows[1].z + point.x * rows[0].z) + point.z * rows[2].z) + rows[3].z};
}

bool ProjectSearchPoint(const Vector3 &point, Vector2 &screen) {
    return PushTransform(point, screen) && screen.x > 0.0f && screen.x < d3t::g_originalScreenWidth && screen.y > 0.0f &&
        screen.y < d3t::g_originalScreenHeight;
}

bool IsSearchPointOnScreen(const Vector3 &point) {
    if (TransformSearchPoint(workTransformMatrix, point).z >= -0.1f)
        return false;
    Vector2 screen;
    return ProjectSearchPoint(point, screen);
}

void CollectSnapshotCandidate(Character *character, void *context) {
    if (!context || !FindMotionWork(character) || !IsOfThisVisibilityType(character, 0))
        return;
    auto *&output = *static_cast<SnapshotSearchCandidate **>(context);
    if (!output)
        return;
    Vector3 point{};
    GetCharacterMotionWorldPosition(character, 5, &point);
    const Vector3 view = TransformSearchPoint(workTransformMatrix, point);
    if (!(view.z < -0.1f))
        return;
    Vector2 screen;
    if (!ProjectSearchPoint(point, screen))
        return;
    screen.x -= static_cast<float>(d3t::g_originalScreenWidth) * 0.5f;
    screen.y -= static_cast<float>(d3t::g_originalScreenHeight) * 0.5f;
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    output->name = scenGetCharaName(control->scenarioTask, character);
    output->cameraDistanceSquared = view.z * view.z;
    output->cameraDistanceSquared += view.y * view.y;
    output->cameraDistanceSquared += view.x * view.x;
    output->screenDistanceSquared = screen.x * screen.x;
    output->screenDistanceSquared += screen.y * screen.y;
    ++output;
}

void GetScenarioSearchPosition(const ScenarioFreeTalkCandidate &candidate, Vector3 &position) {
    // GetScenarioSearchPosition prefers drawable-node matrix banks over the IK joint.
    auto *motion = static_cast<MotionWork *>(candidate.motionWork);
    if (const Matrix4x4 *matrices = GetMotionDrawOverrideData(motion)) {
        position = GetCharacterWorldPosition(candidate.character);
        const Vector3i rotation = GetCharacterWorldRotation(candidate.character);
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[candidate.flags]);
        position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
    } else if (!motion || !GetMotionBonePosition(motion, 5, &position)) {
        position = GetCharacterWorldPosition(candidate.character);
    }
}

std::int8_t EvaluateScenarioSearchGeometry(ScenarioFreeTalkCandidate &candidate, Vector3 &position, float &distanceSquared, std::int16_t &yaw) {
    // EvaluateScenarioSearchGeometry preserves the previous signed searchState as range hysteresis.
    const shen_name focus = srchGetCurrentFocus();
    const float margin = candidate.searchState ? 1.0f : -0.60000002f;
    auto &record = *candidate.record;
    float dx = 0.0f, dz = 0.0f, dy;
    yaw = 0x7fff;
    if (record.approachPointCount) {
        distanceSquared = 99999.0f;
        record.selectedApproachPoint = nullptr;
        for (std::uint32_t index = 0; index < record.approachPointCount; ++index) {
            Vector3 point;
            const auto *source = record.vectorData + index * sizeof(point);
            std::memcpy(&point, source, sizeof(point));
            dx = point.x - scenarioPlayerPosition.x;
            dz = point.z - scenarioPlayerPosition.z;
            const float height = point.y - scenarioPlayerPosition.y;
            float distance = (height * height + dx * dx) + dz * dz;
            if (std::fabs(height) > 2.0f)
                distance = 99999.0f;
            if (distanceSquared > distance) {
                distanceSquared = distance;
                record.selectedApproachPoint = reinterpret_cast<const Vector3 *>(source);
            }
        }
        if (!record.selectedApproachPoint)
            return 0;
        std::memcpy(&position, record.selectedApproachPoint, sizeof(position));
        dy = position.y - scenarioPlayerPosition.y;


    } else {
        GetScenarioSearchPosition(candidate, position);
        dx = position.x - scenarioPlayerPosition.x;
        dz = position.z - scenarioPlayerPosition.z;
        dy = position.y - scenarioPlayerPosition.y;
        distanceSquared = dz * dz + dx * dx;
        if (focus.name == record.name.name)
            dy = 0.0f;
        if (std::fabs(dy) > 2.0f)
            distanceSquared = 99999.0f;
    }
    const std::int16_t targetYaw = FixedAtan2(-dx, -dz);
    yaw = static_cast<std::int16_t>(scenarioPlayerYaw - targetYaw);
    if (distanceSquared >= 14.0f)
        record.control->flags = (record.control->flags & ~0xaU) | 4U;
    record.flags &= ~0x200000U;
    if (distanceSquared < 4.0f)
        record.flags |= 0x200000U;
    const double nearRange = static_cast<double>(margin) + 9.0;
    std::int8_t result = 1;
    if (nearRange <= distanceSquared || static_cast<std::uint16_t>(targetYaw - scenarioPlayerYaw + 16380) > 0x7ff8U || !IsSearchPointOnScreen(position)) {
        const float fx = position.x - scenarioFocusPosition.x;
        const float fz = position.z - scenarioFocusPosition.z;
        const float lx = position.x - scenarioLockonPosition.x;
        const float lz = position.z - scenarioLockonPosition.z;
        const bool nearFocus = focus.name && (focus.name == record.name.name || !IsNpcRouteActive(record.name)) &&
            static_cast<double>(margin) + 36.0 > distanceSquared && std::fabs(dy) < 2.0f && nearRange > fz * fz + fx * fx;
        const bool nearLockon =
            scenarioHasLockonPosition && static_cast<double>(margin) + 36.0 > distanceSquared && std::fabs(dy) < 2.0f && nearRange > lz * lz + lx * lx;
        if (!nearFocus && !nearLockon)
            return 0;
        result = -1;
    }
    flddToggleFlag(0x1000);
    if (flddFindIntersections(position, scenarioPlayerPosition)) {
        return focus.name && focus.name == record.name.name && (GetScenarioFollowEligibility(record.name) || IsNpcRouteActive(record.name)) &&
                candidate.availability > 0 ?
            -1 :
            0;
    }
    return distanceSquared > 9.0f ? -1 : result;
}

struct SignModelRecord {
    shen_name name;
    char file[16];
};
static_assert(sizeof(SignModelRecord) == 20);

constexpr SignModelRecord signsWN00[]{
    {SHEN_NAME(K400), "WN001M4G.MT7"},
    {SHEN_NAME(K401), "WN001M0G.MT7"},
    {SHEN_NAME(K402), "WN001M1G.MT7"},
    {SHEN_NAME(K403), "WN001M2G.MT7"},
    {SHEN_NAME(K404), "WN001M3G.MT7"},
    {SHEN_NAME(K405), "WT001M4G.MT7"},
};
constexpr SignModelRecord signsWK00[]{
    {SHEN_NAME(K406), "WK001M0G.MT7"},
    {SHEN_NAME(K407), "WK001M1G.MT7"},
    {SHEN_NAME(K408), "WK001M2G.MT7"},
    {SHEN_NAME(K409), "WK001M3G.MT7"},
    {SHEN_NAME(K410), "WK001M4G.MT7"},
    {SHEN_NAME(K411), "WK001M5G.MT7"},
};
constexpr SignModelRecord signsAK00[]{
    {SHEN_NAME(K404), "AK00WM0G.MT7"},
    {SHEN_NAME(K402), "AK00WM1G.MT7"},
    {SHEN_NAME(K400), "AK00WM2G.MT7"},
};
constexpr SignModelRecord signsWE00[]{
    {SHEN_NAME(K411), "WE00MM0G.MT7"},
    {SHEN_NAME(K407), "WE00MM1G.MT7"},
    {SHEN_NAME(K410), "WE00MM2G.MT7"},
    {SHEN_NAME(K409), "WE00MM3G.MT7"},
    {SHEN_NAME(K406), "WE00MM4G.MT7"},
    {SHEN_NAME(K408), "WE00MM5G.MT7"},
};
constexpr SignModelRecord signsWB00[]{
    {SHEN_NAME(K401), "WT001M4G.MT7"},
};
constexpr SignModelRecord signsWR00[]{
    {SHEN_NAME(K405), "WR00MM0G.MT7"},
    {SHEN_NAME(K403), "WR00MM1G.MT7"},
    {SHEN_NAME(K404), "WR00MM2G.MT7"},
};
constexpr SignModelRecord signsAR03[]{
    {SHEN_NAME(K402), "AR03WM0G.MT7"},
    {SHEN_NAME(K403), "AR03WM1G.MT7"},
};
constexpr SignModelRecord signsAR02[]{
    {SHEN_NAME(K400), "AR02WM0G.MT7"},
    {SHEN_NAME(K403), "AR02WM1G.MT7"},
    {SHEN_NAME(K404), "AR02WM2G.MT7"},
    {SHEN_NAME(K401), "AR02WM3G.MT7"},
    {SHEN_NAME(K402), "AR02WM4G.MT7"},
};
constexpr SignModelRecord signsWT00[]{
    {SHEN_NAME(K405), "WT001M0G.MT7"},
    {SHEN_NAME(K407), "WT001M2G.MT7"},
    {SHEN_NAME(K408), "WT001M3G.MT7"},
    {SHEN_NAME(K409), "WT001M4G.MT7"},
    {SHEN_NAME(K406), "WT001M5G.MT7"},
};
constexpr SignModelRecord signsWS00[]{
    {SHEN_NAME(K405), "WS00MM0G.MT7"},
    {SHEN_NAME(K408), "WS00MM1G.MT7"},
    {SHEN_NAME(K409), "WS00MM2G.MT7"},
    {SHEN_NAME(K406), "WS00MM3G.MT7"},
};

std::span<const SignModelRecord> SignModelsForCurrentArea() {
    switch (g_currentMap.name) {
    case SHEN_NAME(WN00):
        return signsWN00;
    case SHEN_NAME(WK00):
        return signsWK00;
    case SHEN_NAME(AK00):
        return signsAK00;
    case SHEN_NAME(WE00):
        return signsWE00;
    case SHEN_NAME(WB00):
        return signsWB00;
    case SHEN_NAME(WR00):
        return signsWR00;
    case SHEN_NAME(AR03):
        return signsAR03;
    case SHEN_NAME(AR02):
        return signsAR02;
    case SHEN_NAME(WT00):
        return signsWT00;
    case SHEN_NAME(WS00):
        return signsWS00;
    default:
        return {};
    }
}

std::uint32_t ReadWord32(const std::byte *source) {
    std::uint32_t value;
    std::memcpy(&value, source, sizeof(value));
    return value;
}

SearchTarget *FindSearchTarget(shen_name name) {
    SearchTarget *result = nullptr;
    for (std::size_t i = 0; i < 4; ++i) {
        SearchTarget *target = searchControl->targets[i];
        if (!target)
            continue;
        for (; target->name.name; ++target)
            if (target->name.name == name.name)
                result = target;
    }
    return result;
}

void SearchControlTask_Callback(void *taskData) {
    SearchTaskData *const search = static_cast<SearchTaskData *>(taskData);
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!search || !control || !control->ryo)
        return;
    search->initialCameraState = static_cast<std::int32_t>(camCtrlGetState());
    search->targetFov = plymGetLookZoomScale();
    PadControlEntry *const pad = static_cast<PadControlEntry *>(control->padControl);
    search->input = pad ? ctrlGetPadInputBuffer0(pad) : nullptr;
    if (!search->input)
        return;
    const LegacyInputSlot &input = *search->input;
    if ((input.held & 0xf7ffU) != 0 || input.analog0c != 0 || input.analog0d != 0 || input.analog0e != 0x80 || input.analog0f != 0x80) {
        search->idleTicks = 0;
    } else {
        ++search->idleTicks;
    }
    if (search->mode == SearchMode::Idle) {
        searchExitRequested = 0;
        search->idleTicks = 0;
        if (!plymIsSearchStartRequested())
            search->targetFov = 0;
        srchUpdate(*search);
    } else if (search->mode == SearchMode::Signboard) {
        srchUpdateInputBuff();
        if (!shenCheckCurrentFoVModeIs2()) {
            search->mode = SearchMode::Lockon;
            srchGetEulerAngles(search->currentTarget, &selectedSearchPosition, nullptr);
            publishedSearchPosition = &selectedSearchPosition;
            selectedSearchName = search->currentTarget->name;
            Character *const player = static_cast<Character *>(control->ryo);
            if (player && !search->savedState) {
                savedPlayerState.playerFlags = plymGetCurrentFlags();
                savedPlayerState.characterDisplay = IsOfThisVisibilityType(player, 3);
                EV_SetCharDisplay(player, 3, 0);
                savedPlayerState.characterFlags = charaGetWasVisible(player);
                charaSetVisibilityFlagTo(player, 0);
                search->savedState = &savedPlayerState;
            }
            const std::span<const std::byte> config = srchGetConfigFor(*search, selectedSearchName);
            activeSearchConfig = const_cast<std::byte *>(config.data());
            srchReadSelectionSounds(activeSearchConfig);
            search->context->cursor = activeSearchConfig && !(search->state & 4U) ? activeSearchConfig + 36 : nullptr;
            search->state &= ~0xcU;
        }
    } else if (search->mode == SearchMode::Lockon) {
        srchUpdateLockOn(*search);
    }
    srchIncrementTemporaryModel();
    HLib::ExecuteCallbackNow(srchStaticApplySearchPosRot, nullptr);
}

} // namespace



void PushProjectionPrePass(float, float) {
    const Matrix4x4 previous = workTransformMatrix;
    Matrix4x4_Push(g_matrixStack.entries[2].stack);
    Matrix4x4::MultiplyWithPrePassMatrix(&previous);
    projectionPrePassMatrix = workTransformMatrix;
    Matrix4x4::Pop(1);
}

bool PushTransform(const Vector3 &point, Vector2 &screen) {
    if (!d3t::DX11::g_ppD3DDeviceContext)
        return false;
    const std::array<float, 6> &viewport = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext).viewport;
    const Vector3 clip = TransformSearchPoint(projectionPrePassMatrix, point);
    const Vector4 *rows = projectionPrePassMatrix.rows;
    float w = ((point.y * rows[1].w + point.x * rows[0].w) + point.z * rows[2].w) + rows[3].w;
    if (w < 0.0000001f)
        w += 0.0000001f;
    screen.x = ((clip.x * (1.0f / w) + 1.0f) * viewport[2]) * 0.5f + viewport[0];
    screen.y = ((1.0f - clip.y * (1.0f / w)) * viewport[3]) * 0.5f + viewport[1];
    return true;
}

const SnapshotSearchCandidate *CollectSnapshotSearchCandidates() {
    PushProjectionPrePass(static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight));
    SnapshotSearchCandidate *output = snapshotCandidates.data();
    ForEachLiveCharacterInMapOrder(CollectSnapshotCandidate, &output);
    *output = {};
    return snapshotCandidates.data();
}

std::int64_t srchCheckFlags(std::int32_t mode) {
    if (mode == 3)
        return srchHasActiveLockonTarget() ? 1 : -1;
    if (mode == 6)
        return srchGetCurrentFocus().name;
    return 0;
}

bool srchHasActiveLockonTarget() {
    return searchControl && searchControl->mode != SearchMode::Idle && !currentFollowCharacter.name && searchControl->currentTarget;
}

std::span<const Vector3, 32> srchGetScenarioAdmissionPositions() {
    return scenarioAdmissionPositions;
}

ScenarioFreeTalkCandidate *srchEvaluateSearchTarget() {
    // srchEvaluateSearchTarget is a signed-state, quantized-distance selector. It admits at most
    // 30 ordinary names and appends the focus winner only if not already listed.
    auto *scenario = GetScenarioEventRuntime();
    auto names = GetScenarioCharacterAdmissionNames();
    ScenarioFreeTalkCandidate *selected = nullptr;
    ScenarioFreeTalkCandidate *focused = nullptr;
    float selectedDistance = 1000.0f;
    std::int16_t selectedYaw = 0x7fff, focusedYaw = 0x7fff;
    std::int32_t bestBucket = 999;
    std::size_t admitted = 0;
    Vector3 selectedPosition{};
    const shen_name focus = srchGetCurrentFocus();
    scenarioHasLockonPosition = srchHasActiveLockonTarget() && RetrieveSearchFlagFromPtr(scenarioLockonPosition);
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (scenario && control && control->ryo && plymCanEvaluateSearchTargets()) {
        auto *player = static_cast<Character *>(control->ryo);
        GetCharacterMotionWorldPosition(player, 5, &scenarioPlayerPosition);
        scenarioPlayerYaw = static_cast<std::int16_t>(GetCharacterWorldRotation(player).y);
        scenarioFocusPosition = scenarioPlayerPosition;
        for (auto *candidate = GetScenarioCandidateList(); candidate; candidate = candidate->previous) {
            if (focus.name && candidate->name.name == focus.name) {
                GetScenarioSearchPosition(*candidate, scenarioFocusPosition);
                break;
            }
        }
        PushProjectionPrePass(static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight));
        for (auto *candidate = GetScenarioCandidateList(); candidate; candidate = candidate->previous) {
            if (!candidate->availability)
                continue;
            Vector3 position{};
            float distance;
            std::int16_t yaw;
            const auto state = EvaluateScenarioSearchGeometry(*candidate, position, distance, yaw);
            candidate->proximity = distance;
            candidate->searchState = state;
            if (candidate->availability > 0 && state < 0)
                candidate->availability = -2;
            if (!state)
                continue;
            if (admitted < 30) {
                names[admitted] = candidate->name;
                scenarioAdmissionPositions[admitted++] = position;
            }
            if (focus.name && candidate->name.name == focus.name) {
                focused = candidate;
                focusedYaw = yaw;
            }
            const float sine = sin_fixed16(static_cast<std::uint16_t>(yaw));
            const float weightedDistance = std::sqrt((sine * sine * 3.0f + 1.0f) * distance);
            const float scaled = weightedDistance * 3.3333299f - 1.9f;
            const auto bucket = scaled >= 0.0f ? static_cast<std::int32_t>(scaled) : 0;
            if (bucket >= bestBucket - 1 && (bucket > bestBucket || std::abs(static_cast<int>(yaw)) >= std::abs(static_cast<int>(selectedYaw))))
                continue;
            selected = candidate;
            selectedDistance = distance;
            selectedPosition = position;
            selectedYaw = yaw;
            bestBucket = bucket;
        }
        if (focused) {
            selected = focused;
            selectedDistance = focused->proximity;
            selectedYaw = focusedYaw;
            selectedPosition = scenarioFocusPosition;
            if (scenario->activeCharacter.name != focus.name) {
                if (auto *previous = FindMutableScenarioFreeTalkRecord(scenario->activeCharacter))
                    previous->pendingScriptCursor = nullptr;
                scenario->activeCharacter = focus;
            }
        } else if (focus.name) {
            selected = nullptr;
        }
        if (selected) {
            std::size_t index = 0;
            while (index < admitted && names[index].name != selected->name.name)
                ++index;
            if (index == admitted) {
                names[admitted] = selected->name;
                scenarioAdmissionPositions[admitted++] = selectedPosition;
            }
        }
    }
    names[admitted] = {};
    if (scenario) {
        scenario->selectedTargetDistanceSquared = selectedDistance;
        scenario->selectedTargetAbsYaw = static_cast<std::uint32_t>(std::abs(static_cast<int>(selectedYaw)));
    }
    return selected;
}

shen_name srchGetCurrentFocus() {
    const shen_name follow = GetScenarioFollowCharacter();
    if (follow.name)
        return follow;
    if (focusObject.name)
        return focusObject;
    return currentFollowCharacter;
}

SearchTaskData *GetSearchControl() {
    return searchControl;
}
std::span<const std::byte> GetActiveSearchConfiguration() {
    return activeSearchConfig ? std::span<const std::byte>(activeSearchConfig, ReadWord32(activeSearchConfig + 4)) : std::span<const std::byte>{};
}
bool GetSearchExitRequested() {
    return searchExitRequested != 0;
}
std::span<const std::uint8_t, 16> GetSearchInputHistory() {
    return inputHistory;
}
std::uint8_t GetSearchLastInputX() {
    return lastInputX;
}
std::uint8_t GetSearchLastInputY() {
    return lastInputY;
}
std::uint16_t GetSearchLastDirectionInput() {
    return lastDirectionInput;
}

// @verified - p
shen_name srchGetCurrentFollowCharacter() {
    return GetScenarioFollowCharacter();
}
void srchSetFocusObject(shen_name name) {
    focusObject = name;
}

void srchInitialize(SearchTaskData &search) {
    search.state = 0;
    search.targetState = 0;
    search.mode = SearchMode::Idle;
    search.currentTarget = nullptr;
    search.targets.fill(nullptr);
    search.contextIndex = 0;
    search.savedState = nullptr;
    for (SearchResourceContext &context : search.contexts) {
        context.resource = nullptr;
        context.cursor = nullptr;
        context.strings = nullptr;
        context.flags = 0;
        context.remainingTicks = 0.0f;
    }
    search.context = &search.contexts[0];
    publishedSearchPosition = nullptr;
    savedPlayerState.initialized = 1;
    savedPlayerState.playerFlags = 0;
    savedPlayerState.used = 0;
    activeSearchConfig = nullptr;
    selectedSearchName = {};
    lightConfigurationChanged = 0;
    lastInputX = 0;
    lastInputY = 0;
    lastDirectionInput = 0;
    inputHistory.fill(0);
    staticTargets.fill({});
    searchExitRequested = 0;
}
HLib::Task *InitializeSearchControlTask() {
    searchTask = HLib::EnqueueTaskWithParameter(SearchControlTask_Callback, 12, 2, sizeof(SearchTaskData), SHEN_NAME(SRCH));
    searchControl = searchTask ? static_cast<SearchTaskData *>(HLib::GetTaskParameter(searchTask)) : nullptr;
    focusObject = {};
    currentFollowCharacter = {};
    if (!searchControl)
        return searchTask;
    g_searchTemporaryModel = nullptr;
    g_searchLastSignboard = {};
    searchTemporaryFrame = searchStep = 0.0f;
    switch (g_currentMap.name) {
    case SHEN_NAME(AK00):
        searchRadius = 20.0f;
        break;
    case SHEN_NAME(WK00):
        searchRadius = 8.0f;
        break;
    case SHEN_NAME(AR02):
        searchRadius = 5.0f;
        break;
    case SHEN_NAME(Q300):
    case SHEN_NAME(WN00):
    case SHEN_NAME(WS00):
    case SHEN_NAME(QA11):
        searchRadius = 4.0f;
        break;
    case SHEN_NAME(WT00):
        searchRadius = 3.5f;
        break;
    case SHEN_NAME(QB00):
    case SHEN_NAME(QE01):
    case SHEN_NAME(QAE1):
    case SHEN_NAME(QAW1):
    case SHEN_NAME(QE06):
    case SHEN_NAME(QAE6):
    case SHEN_NAME(QAW6):
        searchRadius = 2.0f;
        break;
    default:
        searchRadius = 2.5f;
        break;
    }
    srchInitialize(*searchControl);
    srchResetIK();
    srchReadFromData(*searchControl);
    srchSetupCameraFov();
    return searchTask;
}

std::span<const std::byte> srchGetConfigFor(const SearchTaskData &search, shen_name name) {
    if (!name.name || !search.context->resource)
        return {};
    const std::size_t contextIndex = static_cast<std::size_t>(search.context - search.contexts.data());
    const std::span<const std::byte> bytes = resourceBytes[contextIndex];
    std::size_t offset = 12;
    while (offset <= bytes.size() && bytes.size() - offset >= 8) {
        const std::byte *record = bytes.data() + offset;
        const std::uint32_t id = ReadWord32(record);
        if (!id)
            break;
        const std::uint32_t size = ReadWord32(record + 4);
        if (size < 36 || size > bytes.size() - offset)
            break;
        if (id == name.name)
            return bytes.subspan(offset, size);
        offset += size;
    }
    return {};
}

// @verified - p
MT7Node *FindModelCharacterRecursive(MT7Node *node, shen_name name) {
    if (IsMDPXorMDP7(node) == name.name)
        return node;
    if (node->runtimeChild) {
        if (MT7Node *const found = FindModelCharacterRecursive(node->runtimeChild, name))
            return found;
    }
    if (node->runtimeSibling)
        return FindModelCharacterRecursive(node->runtimeSibling, name);
    return nullptr;
}

// @verified - p
MT7Node *mtFindChildRecursive(MT7Node *node, std::uint32_t marker) {
    const std::uint32_t current = static_cast<std::uint32_t>(IsMDPXorMDP7(node));
    if (current != marker && static_cast<std::uint8_t>(current) == 'M')
        return node;
    if (node->runtimeChild) {
        if (MT7Node *const found = mtFindChildRecursive(node->runtimeChild, marker))
            return found;
    }
    if (node->runtimeSibling)
        return mtFindChildRecursive(node->runtimeSibling, marker);
    return nullptr;
}

// @verified - p
void srchApplyObjectTransform(const MT7Node *node, std::uint32_t *yaw) {
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
    Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    if (yaw)
        *yaw = (*yaw + static_cast<std::uint32_t>(node->rotation.y)) & 0xffffU;
}

// @verified - p
float srchGetDefaultModelRadius(std::int16_t name) {
    if (static_cast<std::uint8_t>(name) != 'M')
        return 0.0f;
    const std::int32_t character = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::uint16_t>(name) >> 8U));
    std::int32_t index = character - '0';
    if (static_cast<std::uint32_t>(index) > 9U)
        index = character - 55;
    return index < 16 ? searchModelRadii.data()[index] : 0.0f;
}

// @verified - p
std::uint64_t srchRegisterStaticObject(MT7Node *node, std::uint32_t marker) {
    if ((static_cast<std::uint32_t>(node->id) >> 16U) != 0)
        return 0;
    SearchTarget *target = nullptr;
    for (SearchTarget &entry : staticTargets) {
        if (entry.name.name == marker || (static_cast<std::uint8_t>(marker) == 'M' && static_cast<std::uint8_t>(marker >> 8U) != 'A' && !node->runtimeChild))
            return 0;
        if (!entry.name.name) {
            target = &entry;
            break;
        }
    }
    if (!target)
        return 0;
    Vector3 position;
    GetPropVector4(&position);
    target->name = shen_name{marker};
    const std::span<const std::byte> config = srchGetConfigFor(*searchControl, target->name);
    if (static_cast<std::uint8_t>(marker) == 'K') {
        std::uint32_t yaw = 0;
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        srchApplyObjectTransform(node, &yaw);
        target->position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        node->id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffff0000U) | 64198U);
        MT7Node *child = nullptr;
        if (node->runtimeChild)
            child = FindModelCharacterRecursive(node->runtimeChild, shen_name{(marker & 0xffffff00U) | 'k'});
        if (child) {
            child->id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(child->id) & 0xffff0000U) | static_cast<std::uint16_t>(-1339));
            target->lockonAngle =
                std::sqrt((child->position.x * child->position.x + child->position.y * child->position.y) + child->position.z * child->position.z);
        } else {
            target->lockonAngle = 0.60000002f;
        }
        target->field08 = yaw + 0x8000U;
    } else {
        node->id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffff0000U) | static_cast<std::uint16_t>(-1340));
        for (MT7Node *child = node->runtimeChild; child;) {
            child->id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(child->id) & 0xffff0000U) | static_cast<std::uint16_t>(-1341));
            MT7Node *const found = mtFindChildRecursive(child, marker);
            if (!found || found == child)
                break;
            found->id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(found->id) & 0xffff0000U) | static_cast<std::uint16_t>(-1340));
            child = found->runtimeChild;
        }
        target->position = position;
        target->lockonAngle = srchGetDefaultModelRadius(std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(marker)));
        target->field08 = 32768;
        if (!config.empty()) {
            float radius;
            std::memcpy(&radius, config.data() + 12, sizeof(radius));
            if (radius > (std::numeric_limits<float>::epsilon)())
                target->lockonAngle = radius;
        }
    }
    target->flags = 0;
    target->yawLimit = 15473;
    if (!config.empty())
        std::memcpy(&target->yawLimit, config.data() + 34, sizeof(target->yawLimit));
    return 1;
}

// @verified - p
std::uint64_t srchRegisterAndPush(MT7Node *root) {
    if (!root)
        return 0;
    staticTargets.fill({});
    std::uint32_t registered = 0;
    for (MT7Node *node = root; node; node = node->runtimeSibling) {
        const std::uint32_t marker = static_cast<std::uint32_t>(IsMDPXorMDP7(node));
        if (((static_cast<std::uint8_t>(marker) - 75) & 0xfd) == 0 && srchRegisterStaticObject(node, marker))
            registered = 1;
    }
    searchControl->targets[3] = staticTargets.data();
    return registered;
}

// @verified - p
void UpdatePropSearchVisibility(MT7Node *root, std::uint32_t parentMarker, std::int32_t enabled) {
    for (MT7Node *node = root; node; node = node->runtimeSibling) {
        const std::uint32_t marker = static_cast<std::uint32_t>(IsMDPXorMDP7(node));
        if (static_cast<std::uint8_t>(marker) == 'M') {
            if (parentMarker != marker) {
                const std::span<const std::byte> config = srchGetConfigFor(*searchControl, shen_name{marker});
                enabled = 0;
                if (!config.empty()) {
                    const std::uint32_t flags = ReadWord32(config.data() + 8);
                    const std::uint32_t flag = flags >> 16U;
                    bool present = false;
                    if (flags & 4U)
                        present = EV_GetShenmueFlag(static_cast<std::int32_t>(flag)) != 0;
                    else if (flags & 8U)
                        present = GetInternalFlag(static_cast<std::int32_t>(flag));
                    else if (flags & 0x10U)
                        present = GetItemState(flag) != 0;
                    if (flags & 0x1cU)
                        enabled = static_cast<std::int32_t>(((flags >> 5U) & 1U) ^ static_cast<std::uint32_t>(!present));
                }
            }
            node->id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffffU) | (static_cast<std::uint32_t>(enabled) << 16U));
        }
        if (node->runtimeChild)
            UpdatePropSearchVisibility(node->runtimeChild, marker, enabled);
    }
}

// The lock-on target, current follow character and focus object names.
shen_name srchGetTargetName(std::uint32_t index) {
    if (index >= 3)
        return {};
    const std::array<const shen_name *, 3> names{&selectedSearchName, &currentFollowCharacter, &focusObject};
    return *names[index];
}

std::int32_t srchRestoreRyoNpcStates(SearchTaskData &search) {
    Character *const player = static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
    if (!player)
        return 0;
    SearchSavedState *const saved = search.savedState;
    if (!saved)
        return 0;
    plymSetFlags(saved->playerFlags);
    EV_SetCharDisplay(player, 3, saved->characterDisplay);
    charaSetVisibilityFlagTo(player, saved->characterFlags);
    search.savedState = nullptr;
    saved->used = 0;
    return 1;
}

// @verified - p
void srchReset() {
    SearchTaskData *const search = searchControl;
    search->mode = SearchMode::Idle;
    search->state = 0;
    search->currentTarget = nullptr;
    search->context->flags = 0;
    search->context->cursor = nullptr;
    selectedSearchName = {};
    activeSearchConfig = nullptr;
    srchRestoreRyoNpcStates(*searchControl);
    searchStep = -0.1f;
    currentFollowCharacter = {};
    if (lightConfigurationChanged)
        SetupLightConfig();
    lightConfigurationChanged = 0;
}

// @verified - p
void shenFreeTempModel() {
    MT7Model *const model = g_searchTemporaryModel;
    if (model && (reinterpret_cast<std::uintptr_t>(model) & 31U) == 0) {
        HLib::TaskData *const allocation = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(model) - sizeof(HLib::TaskData));
        if (allocation->status.name == make_shen_name('U', 'S', 'E', ' ') && allocation->name.name) {
            SearchTemporaryModel *const owner = static_cast<SearchTemporaryModel *>(model);
            void *const source = owner->sourceAllocation;
            ReleaseModelFormat7(*model);
            owner->~SearchTemporaryModel();
            HLib::FreeTaskData(source);
            HLib::FreeTaskData(model);
        }
    }
    searchStep = 0.0f;
    g_searchTemporaryModel = nullptr;
    g_searchLastSignboard = {};
}

// @verified - p
void srchResetIK() {
    searchPropIKIndex = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        SearchPropIKSlot &slot = g_searchPropIKSlots[index];
        if (slot.node) {
            if (searchPropIKLookup[slot.lookupIndex] == 7 && (static_cast<std::uint32_t>(slot.node->id) >> 16U))
                slot.node->id = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(slot.node->id) & 0xffffU);
            if (slot.node->xb01Offset)
                SetMT7NodeTransformOverride(*slot.node, nullptr);
        }
        slot.flags = 0;
        slot.active = 0;
        slot.node = nullptr;
        slot.character = nullptr;
        slot.resource = nullptr;
        slot.owner = nullptr;
        slot.transformData = searchPropIKTransformData[index];
        slot.auxiliaryData = searchPropIKAuxiliaryData[index];
        slot.frame = 0;
    }
    searchPropIKResult = 0;
    for (std::size_t index = 0; index < 3; ++index)
        std::memset(searchPropIKSelection.data() + 6 * index, 0, 5);
}

// @verified - p
std::int64_t srchSetLookMode() {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    EV_SetCharDisplay(static_cast<Character *>(control->ryo), 3, 1);
    shenFreeTempModel();
    MT7Node *const root = GetPropRoot();
    if (root) {
        UpdatePropSearchVisibility(root->runtimeChild, 0, 0);
        srchRegisterAndPush(root->runtimeChild);
    }
    mtwkResetBlends();
    srchReset();
    camCtrlSetLookAroundMode();
    srchResetIK();
    return mtwkSetOverridesEnabled(1);
}

// @verified - p
void srchRequestLockonExit() {
    searchExitRequested = 1;
}

// @verified - p
void srchRequestRelease() {
    searchControl->state |= 2U;
}

// @verified - p
std::int64_t srchSetTarget(std::int32_t index, SearchTarget *targets) {
    if (index >= 4)
        return 0;
    const std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(index) * static_cast<std::ptrdiff_t>(sizeof(targets));
    std::memcpy(reinterpret_cast<std::byte *>(searchControl) + offsetof(SearchTaskData, targets) + offset, &targets, sizeof(targets));
    return 1;
}

// @verified - p
std::int64_t srchClearTarget(std::int32_t index) {
    if (index >= 4)
        return 0;
    SearchTarget *const target = nullptr;
    const std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(index) * static_cast<std::ptrdiff_t>(sizeof(target));
    std::memcpy(reinterpret_cast<std::byte *>(searchControl) + offsetof(SearchTaskData, targets) + offset, &target, sizeof(target));
    return 1;
}

bool ReadLOCK(SearchTarget *targets) {
    if (!targets || searchControl->currentTarget || (searchControl->targetState & 2))
        return false;
    searchControl->targets[4] = searchControl->targets[0];
    searchControl->targets[0] = targets;
    searchControl->targetState |= 2;
    return true;
}

void srchPushPopTarget() {
    if (searchControl->currentTarget)
        return;
    if (searchControl->targetState & 2) {
        searchControl->targets[0] = searchControl->targets[4];
        searchControl->targets[4] = nullptr;
    }
    searchControl->targetState &= ~2U;
}

bool ReadSRCH(std::span<const std::byte> resource) {
    SearchTaskData *const search = searchControl;
    if (resource.empty() || search->currentTarget || search->contextIndex >= 2)
        return false;
    const std::size_t index = static_cast<std::size_t>(++search->contextIndex);
    search->context = &search->contexts[index];
    std::byte *const bytes = const_cast<std::byte *>(resource.data());
    resourceBytes[index] = {bytes, resource.size()};
    search->context->resource = bytes;
    const std::uint32_t stringOffset = resource.size() >= 12 ? ReadWord32(bytes + 8) : 0;
    search->context->strings = stringOffset ? bytes + stringOffset : nullptr;
    if (MT7Node *const root = GetPropRoot()) {
        UpdatePropSearchVisibility(root, 0, 0);
        srchRegisterAndPush(root->runtimeChild);
    }
    return true;
}

void SRCHParser(const std::byte *resource) {
    SearchTaskData *const search = searchControl;
    if (search->currentTarget)
        return;
    if (search->contextIndex != 0) {
        for (std::int32_t index = 1; index < 3; ++index) {
            if (search->contexts[static_cast<std::size_t>(index)].resource != resource)
                continue;
            for (std::int32_t next = index; next < search->contextIndex; ++next) {
                search->contexts[static_cast<std::size_t>(next)] = search->contexts[static_cast<std::size_t>(next) + 1];
                resourceBytes[static_cast<std::size_t>(next)] = resourceBytes[static_cast<std::size_t>(next) + 1];
            }
            search->contexts[static_cast<std::size_t>(search->contextIndex)] = {};
            resourceBytes[static_cast<std::size_t>(search->contextIndex)] = {};
        }
        search->context = &search->contexts[static_cast<std::size_t>(--search->contextIndex)];
    }
    if (MT7Node *const root = GetPropRoot()) {
        UpdatePropSearchVisibility(root, 0, 0);
        srchRegisterAndPush(root->runtimeChild);
    }
}

// @verified - p
std::uint64_t srchSetTargetEnabled(shen_name name, std::int32_t enabled) {
    SearchTarget *target = FindSearchTarget(name);
    if (!target)
        return 0xffffffffU;
    const std::uint32_t previous = target->flags;
    if (enabled)
        target->flags &= ~2U;
    else
        target->flags |= 2;
    return (previous & 2) == 0;
}

bool RetrieveSearchFlagFromPtr(Vector3 &position) {
    if (!publishedSearchPosition)
        return false;
    position = *publishedSearchPosition;
    return true;
}

void srchUpdateInputBuff() {
    const LegacyInputSlot &input = *searchControl->input;
    std::uint8_t &history = inputHistory[inputHistoryIndex];
    history = 0;
    if (input.analog0f != 0x80) {
        history |= 1;
        lastInputY = input.analog0f;
    }
    if (input.analog0e != 0x80) {
        history |= 2;
        lastInputX = input.analog0e;
    }
    if (input.held & 0xf000) {
        history |= 4;
        lastDirectionInput = input.held;
    }
    inputHistoryIndex = (inputHistoryIndex + 1) & 15;
}

bool srchReadFromData(SearchTaskData &search) {
    char path[64];
    std::snprintf(path, sizeof(path), "/SCENE/%02d/search.bin", g_actNumber);
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (!size || size > (std::numeric_limits<std::size_t>::max)())
        return false;
    std::byte *allocation = static_cast<std::byte *>(HLib::AllocateTaskData(size, SHEN_NAME(SRCH)));
    if (!allocation)
        return false;
    if (!ArchiveData_Read(path, 0, allocation, size)) {
        HLib::FreeTaskData(allocation);
        return false;
    }
    HLib::AssignDynamicAlignmentForTaskData(allocation);
    sceneSearchAllocation = allocation;
    resourceBytes[0] = {allocation, static_cast<std::size_t>(size)};
    search.context->resource = allocation;
    const std::uint32_t stringOffset = size >= 12 ? ReadWord32(allocation + 8) : 0;
    search.context->strings = stringOffset && stringOffset < size ? allocation + stringOffset : nullptr;
    return true;
}

bool npcGetCanInteract(shen_name name) {
    if (!name.name || !HasNpcRuntimeEntry(name) || IsNpcLoadedOrVisible(name))
        return false;
    if (QueryNpcRouteLiveState(name) == 10 || IsNpcRouteActive(name))
        return true;
    switch (name.name) {
    case SHEN_NAME(REN_):
        return EV_GetShenmueFlag(0x305) || EV_GetShenmueFlag(0x342);
    case SHEN_NAME(YUA_):
        return EV_GetShenmueFlag(0x115);
    case SHEN_NAME(SYE_):
        return EV_GetShenmueFlag(0x105);
    case SHEN_NAME(SAM_):
        return EV_GetShenmueFlag(0x375);
    case SHEN_NAME(WON_):
        return EV_GetShenmueFlag(0x2de);
    case SHEN_NAME(LLY_):
        return EV_GetShenmueFlag(0x377);
    default:
        return false;
    }
}

bool IsSpecialSignModeForCurrentArea() {
    if (!searchControl || !searchControl->currentTarget)
        return false;
    const shen_name name = searchControl->currentTarget->name;
    if ((name.name & 0xff) != 'K')
        return false;
    for (const SignModelRecord &model : SignModelsForCurrentArea())
        if (model.name.name == name.name)
            return true;
    return false;
}

// @verified - p
void srchLoadSignBoardModelForArea(SearchTaskData &search) {
    const shen_name name = search.currentTarget->name;
    if (g_searchLastSignboard.name == name.name) {
        if (g_searchTemporaryModel)
            searchStep = 0.1f;
        return;
    }
    if (static_cast<std::uint8_t>(name.name) != 'K')
        return;
    for (const SignModelRecord &sign : SignModelsForCurrentArea()) {
        if (sign.name.name != name.name)
            continue;
        char directory[256];
        char path[272];
        std::snprintf(directory, sizeof(directory), "SCENE/%02d/MODEL/SIGNBD", g_actNumber);
        std::snprintf(path, sizeof(path), "%s/%s", directory, sign.file);
        shenFreeTempModel();
        EV_ResetFreeTalkFlags();
        g_searchLastSignboard = name;
        srchGetEulerAngles(search.currentTarget, &searchTemporaryPosition, &searchTemporaryYaw);
        std::uint32_t size = 0;
        constexpr shen_name signMemory{SHEN_NAME(SGBD)};
        constexpr shen_name readerMemory{make_shen_name('M', 'A', 'I', ' ')};
        void *const source = LoadLegacySceneFile(directory, sign.file, signMemory, &size);
        if (!source)
            return;
        void *const storage = HLib::AllocateTaskData(sizeof(SearchTemporaryModel), readerMemory);
        if (!storage) {
            HLib::FreeTaskData(source);
            return;
        }
        SearchTemporaryModel *const model = new (storage) SearchTemporaryModel{};
        model->sourceAllocation = source;
        if (!ReadModelFormat7({static_cast<const std::byte *>(source), size}, path, *model, 0)) {
            ReleaseModelFormat7(*model);
            model->~SearchTemporaryModel();
            HLib::FreeTaskData(storage);
            HLib::FreeTaskData(source);
            return;
        }
        BindMT7WritableRuntimeMeshes(*model, {static_cast<std::byte *>(source), size});
        g_searchTemporaryModel = model;
        searchTemporaryFrame = 0.0f;
        searchStep = 0.1f;
        return;
    }
}

// @verified - p
void srchIncrementTemporaryModel() {
    if (!g_searchTemporaryModel)
        return;
    searchTemporaryFrame += searchStep;
    if (searchStep > 0.0f && searchTemporaryFrame > 1.0f)
        searchTemporaryFrame = 1.0f;
    if (searchStep < 0.0f && searchTemporaryFrame < 0.0f) {
        searchTemporaryFrame = 0.0f;
        shenFreeTempModel();
    }
}

// @verified - p
void srchStaticApplySearchPosRot(void *) {
    if (!g_searchTemporaryModel)
        return;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(searchTemporaryPosition.x, searchTemporaryPosition.y, searchTemporaryPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(searchTemporaryYaw));
    Matrix4x4::RotateAroundY_Wrapper(32768);
    Matrix4x4::preTranslate(0.0f, 0.0f, 0.005f);
    SetLegacyBusyPreparingDraw(true);
    charaPrepareNodesForDraw(g_searchTemporaryModel, g_searchTemporaryModel ? g_searchTemporaryModel->nodes.data() : nullptr, searchTemporaryFrame);
    SetLegacyBusyPreparingDraw(false);
    Matrix4x4::Pop(1);
}

} // namespace shenmue::s2
