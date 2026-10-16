#include "s2/event_darts.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {


constexpr std::array<std::int8_t, 10> fcjDartsPattern{
    5, 18, 13, 15, 3, 25, 25, 25, 50, -1};
constexpr std::array<std::int8_t, 20> ychDartsPattern{
    20, 13, 15, 19, 16, 14, 25, 25, 25, 25,
    25, 25, 25, 25, 25, 25, 25, 25, 50, -1};
constexpr std::array<std::int8_t, 5> hlnDartsPattern{12, 18, 6, 16, -1};
constexpr std::array<std::int8_t, 10> jonDartsPattern{
    18, 6, 25, 2, 3, 25, 9, 8, 33, -1};
constexpr std::array<std::int8_t, 8> xhoDartsPattern{
    1, 12, 8, 25, 25, 50, 50, -1};

constexpr std::array<EventDartsAiRecord, 5> dartsAiRecords{{
    {SHEN_NAME(FCJ_), 0x1a82, 0x1a81, fcjDartsPattern},
    {SHEN_NAME(YCH_), 0x1a8e, 0x1a8d, ychDartsPattern},
    {SHEN_NAME(HLN_), 0x1ab9, 0x1ab8, hlnDartsPattern},
    {SHEN_NAME(JON_), 0x1a9a, 0x1a99, jonDartsPattern},
    {SHEN_NAME(XHO_), 0x1aa0, 0x1a9f, xhoDartsPattern},
}};

constexpr std::array<EventDartsAiScoreRecord, 5> dartsAiScores{{
    {25, 0x421dc8, 0x1f24a8, 0x03d090},
    {20, 0x50d368, 0x423920, 0x03d090},
    {35, 0x1024d0, 0x0bbfd0, 0x03d090},
    {6445, 0x3924e8, 0x0d9c60, 0x03d090},
    {20, 0x3924e8, 0x1ba620, 0x03d090},
}};

void EventDartsArrowCallback(void *taskData);
void EventDartsEasyCallback(void *taskData);
void EventEnemyDartsCallback(void *taskData);

const EventDartsAiRecord *FindDartsAiRecord(shen_name character) {
    const auto found = std::find_if(
        dartsAiRecords.begin(), dartsAiRecords.end(), [character](const EventDartsAiRecord &record) { return record.character.name == character.name; });
    return found == dartsAiRecords.end() ? nullptr : &*found;
}

void SetEventCharacterRotation(shen_name character, const Vector3i &rotation) {
    Character *const found = FindCharacterByScenarioName(character);
    if (!found)
        return;
    found->entity.node.rotation[0] = rotation.x;
    found->entity.node.rotation[1] = rotation.y;
    found->entity.node.rotation[2] = rotation.z;
    if (MotionWork *const motionWork = FindMotionWork(found)) {
        motionWork->runtime.entityRotation = rotation;
    }
}

void SetEventCharacterControllerFlags(shen_name character, std::uint32_t flags, bool enabled) {
    Character *const found = FindCharacterByScenarioName(character);
    CharacterControlModel *const control = FindCharacterControlModel(found);
    if (!control)
        return;
    if (enabled)
        control->flags |= flags;
    else
        control->flags &= ~flags;
}
} // namespace event_runtime_detail

void QueueDartsPosition(shen_name character, shen_name parent, const Vector3 *position, const Vector3i *rotation, std::int32_t delayFrames) {
    EventDartsPositionTaskData local{};
    EventDartsPositionTaskData *data = &local;
    if (delayFrames != 0) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(DartsPositionCallback, 4, 0x0b, sizeof(EventDartsPositionTaskData), eventTaskToken);
        data = static_cast<EventDartsPositionTaskData *>(HLib::GetTaskParameter(task));
        if (!data)
            return;
        *data = {};
        data->cleanupTask = 1;
        if (rotation) {
            data->hasRotation = 1;
            data->rotation = *rotation;
        }
        if (position) {
            data->hasPosition = 1;
            data->position = *position;
        }
        data->parent = parent;
        data->character = character;
        HLib::SetupTimedTask(task, delayFrames);
        return;
    }
    if (position) {
        data->hasPosition = 1;
        data->position = *position;
    }
    if (rotation) {
        data->hasRotation = 1;
        data->rotation = *rotation;
    }
    data->parent = parent;
    data->character = character;
    DartsPositionCallback(data);
}

void DartsPositionCallback(void *taskData) {
    EventDartsPositionTaskData *const data = static_cast<EventDartsPositionTaskData *>(taskData);
    if (!data)
        return;
    const shen_name previousParent = EV_SCEN_FindChar_Wrapper(data->character);
    AT_SetLocalBaseChara(data->character, data->parent);
    if (data->hasPosition)
        EV_AT_SetPos(data->character, EventTransformFlags::AllAxes, data->position);
    if (data->hasRotation)
        EV_AT_SetAngY(data->character, EventTransformFlags::AllAxes, data->rotation, nullptr);
    AT_SetLocalBaseChara(data->character, previousParent);
    SetClothFlag(FindCharacterByScenarioName(data->character), 16, 1);
    toggleOsagFlags(FindCharacterByScenarioName(data->character));
    if (data->cleanupTask)
        HLib::DestroyCurrentTask();
}
namespace event_runtime_detail {


std::uint16_t SetDartsAiMotion(shen_name character, std::uint32_t flags) {
    const EventDartsAiRecord *const record = FindDartsAiRecord(character);
    std::uint16_t motion = dartsAiRecords.front().motionA;
    if (record) {
        motion = IncrementSharedPrngFloat() >= 0.5f ? record->motionB : record->motionA;
        if (!motion)
            motion = dartsAiRecords.front().motionA;
    }
    EVT_AT_SetMot(character, motion, -1.0f, 1.0f, flags, 1);
    return motion;
}

void SetDartsPosition(shen_name character, shen_name parent, float xOffset, std::int32_t delayFrames) {
    const Vector3 position{xOffset, 0.0f, 3.5999999f};
    const Vector3i rotation{};
    if (Character *const found = FindCharacterByScenarioName(character)) {
        SetMotionRegionFlag(FindMotionWork(found), 71, 0, MotionRegion::LowerBody);
    }
    QueueDartsPosition(character, parent, &position, &rotation, delayFrames);
}

void QueueDartsArrow(shen_name character, shen_name arrow) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventDartsArrowCallback, 4, 0x0b, sizeof(EventDartsArrowTaskData), eventTaskToken);
    auto *const data = static_cast<EventDartsArrowTaskData *>(HLib::GetTaskParameter(task));
    if (!data)
        return;
    data->character = character;
    data->arrow = arrow;
    eventDartsTemporaryCharacter = {};
}

void EventDartsArrowCallback(void *taskData) {
    auto *const data = static_cast<EventDartsArrowTaskData *>(taskData);
    if (!data) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (data->character.name == eventDartsTemporaryCharacter.name) {
        const shen_name target = data->arrow;
        HLib::DestroyCurrentTask();
        ResetCharaFixo(FindCharacterByScenarioName(target));
        return;
    }
    if (!DoesScenarioCharacterExist(data->character) || !DoesScenarioCharacterExist(data->arrow)) {
        const shen_name target = data->arrow;
        EV_DisableCharacterDisplay(target);
        EV_ToggleCharacterFlag(target);
        HLib::DestroyCurrentTask();
        ResetCharaFixo(FindCharacterByScenarioName(target));
        return;
    }

    EV_EnableCharacterDisplay(data->arrow);
    EV_EnsureCharacterDraw(data->arrow);
    Character *const source = FindCharacterByScenarioName(data->character);
    Character *const arrow = FindCharacterByScenarioName(data->arrow);
    constexpr Vector3 jonOffset{0.1400000006f, -0.0599999987f, 0.0399999991f};
    constexpr Vector3 fcjHlnOffset{0.1099999994f, -0.0900000036f, -0.0f};
    constexpr Vector3 defaultOffset{0.1400000006f, -0.0599999987f, -0.0199999996f};
    constexpr std::array<std::int32_t, 3> rotation{0x48df, 0x7a9f, 0xc111};
    const shen_name jon{SHEN_NAME(JON_)};
    const shen_name fcj{SHEN_NAME(FCJ_)};
    const shen_name hln{SHEN_NAME(HLN_)};
    const bool isJon = data->character.name == jon.name;
    const bool isFcjOrHln = data->character.name == fcj.name || data->character.name == hln.name;
    SetFixoDataForMotionWork(arrow, source, isJon ? 21 : 14, isJon ? jonOffset : (isFcjOrHln ? fcjHlnOffset : defaultOffset), rotation);
}

void PrepareDartsArrows(EventEnemyDartsTaskData &data) {
    const EventDartsAiRecord *const record = FindDartsAiRecord(data.enemy78->name);
    if (!record)
        return;
    std::size_t arrowIndex = 0;
    if (record) {
        for (const std::int8_t score : record->dartPattern) {
            if (score < 0 || arrowIndex >= 15)
                break;
            const shen_name arrow{make_shen_name('Y', 'a', static_cast<char>('0' + arrowIndex / 10), static_cast<char>('0' + arrowIndex % 10))};
            EV_EnableCharacterDisplay(arrow);
            EV_EnsureCharacterDraw(arrow);
            Vector3 position;
            CalculateDartsPositionOrScore(score, &position);
            SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(arrow)), 71, 0, MotionRegion::LowerBody);
            const Vector3i rotation{0, 0xc000, 0};
            QueueDartsPosition(arrow, data.configuration70->character, &position, &rotation, 0);
            ++arrowIndex;
        }
    }
    while (arrowIndex < 15) {
        const shen_name arrow{make_shen_name('Y', 'a', static_cast<char>('0' + arrowIndex / 10), static_cast<char>('0' + arrowIndex % 10))};
        EV_DisableCharacterDisplay(arrow);
        EV_ToggleCharacterFlag(arrow);
        ++arrowIndex;
    }
}

void ClearDartsResultFlags() {
    constexpr std::array<std::int16_t, 4> resultFlags{1419, 1420, 1422, 1423};
    for (const std::int16_t flag : resultFlags) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, flag, 0);
    }
}

void BeginDartsEvent(shen_name enemy, std::uint8_t mode, EventEnemyDartsTaskData *parameter) {
    eventDartsState.enemy = enemy;
    eventDartsState.mode = mode;
    SetPlayerControlEnabled(0);
    srchReset();
    LoadAreaFunc_Wrapper_0(DART_EVENT_SLOT, 0);
    ExecuteCurrAreaMainFunc(DART_EVENT_SLOT, reinterpret_cast<std::uintptr_t>(&parameter->byte48));
    HLib::SetupTaskDelay(0);
}

void EventDartsEasyCallback(void *taskData) {
    auto *const data = static_cast<EventEnemyDartsTaskData *>(taskData);
    if (!data) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (data->state3c == 4)
        data->state3c = 0;

    std::int32_t result = -1;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1419)) {
        result = 0;
    } else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1420)) {
        result = 1;
    } else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1422)) {
        result = 2;
    }
    if (result < 0)
        return;

    ClearDartsResultFlags();
    data->byte49 = static_cast<std::uint8_t>(result);
    data->byte4a = 0;
    data->state3c = 4;
    BeginDartsEvent({}, data->byte49, data);
}

void EventEnemyDartsCallback(void *taskData) {
    auto *const data = static_cast<EventEnemyDartsTaskData *>(taskData);
    if (!data) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!data->enemy78 || !DoesScenarioCharacterExist(data->enemy78->name)) {
        EventDartsEasyCallback(data);
        return;
    }

    const shen_name enemy = data->enemy78->name;
    if (data->state3c == 2) {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1423)) {
            ClearDartsResultFlags();
            data->byte49 = static_cast<std::uint8_t>(data->count38);
            data->task68 = data->task28;
            data->state3c = 3;
            data->byte48 = 1;
            data->byte4a = 2;
            eventDartsTemporaryCharacter = enemy;
            BeginDartsEvent(enemy, 2, data);
        } else if (Character *const character = FindCharacterByScenarioName(enemy)) {
            MotionWork *const motionWork = FindMotionWork(character);
            if (SetMotionRegionFlag(motionWork, 65, 1, MotionRegion::LowerBody) && ryoNotBusy()) {
                SetDartsAiMotion(enemy, 0x02000000U);
                const bool aks0Xho = g_currentMap.name == SHEN_NAME(AKS0) && enemy.name == SHEN_NAME(XHO_);
                SetDartsPosition(enemy, data->configuration70->character, aks0Xho ? -0.30000001f : 0.0f, 1);
            }
        }
        return;
    }
    if (data->state3c == 3) {
        data->state3c = 1;
        eventDartsState = {};
        SetPlayerControlEnabled(1);
        return;
    }
    if (data->state3c != 0 && data->state3c != 1)
        return;

    if (data->state3c == 0) {
        SetDartsAiMotion(enemy, 0x40000000U);
        const bool aks0Xho = g_currentMap.name == SHEN_NAME(AKS0) && enemy.name == SHEN_NAME(XHO_);
        SetDartsPosition(enemy, data->configuration70->character, aks0Xho ? -0.30000001f : 0.0f, 1);
    }

    data->state3c = 2;
    Vector3i boardRotation{};
    GetScenarioCharacterEulerAngles(data->configuration70->character, &boardRotation, EventTransformFlags::World);
    SetNpcRouteYaw(enemy, boardRotation.y);
    EV_EnableCharacterDisplay(data->configuration70->character);
    EV_SetCharacterAlphaEnabled(enemy, true);
    QueueDartsArrow(enemy, SHEN_NAME(Ya14));
    mtwkSetRequestedRootMode(FindMotionWork(FindCharacterByScenarioName(enemy)), 3);
    EV_mtwkSetEnabled(FindMotionWork(FindCharacterByScenarioName(enemy)), true);
    SetEventCharacterControllerFlags(enemy, CharacterControlApplyGravity, false);
    SetEventCharacterControllerFlags(enemy, CharacterControlUpdateHeight | CharacterControlApplyGravity, false);
    SetEventCharacterControllerFlags(enemy, CharacterControlUpdateHeight, false);
    RestoreOpponentDartsScores(enemy, &data->field58, &data->field5c);
    PrepareDartsArrows(*data);

    if (data->enemy78->aiIndex < 0 || data->enemy78->aiIndex >= static_cast<std::int32_t>(dartsAiScores.size())) {
        return;
    }
    const std::size_t aiIndex = static_cast<std::size_t>(data->enemy78->aiIndex);
    const EventDartsAiScoreRecord &scores = dartsAiScores[aiIndex];
    data->field58 = std::max(data->field58, scores.minimumScore);
    data->field5c = data->field5c ? std::max(data->field5c, scores.scoreFloor) : scores.defaultScore;
    data->field54 = scores.scoreFloor;
    data->byte4b = static_cast<std::uint8_t>(scores.packed);
    data->byte4c = static_cast<std::uint8_t>(scores.packed >> 8U);
    scenCopyCharaToGame(data->configuration70->character);
    UpdateDartsScoreDisplay(data->configuration70->character, data->field58, 0);
    UpdateDartsScoreDisplay(data->configuration70->character, data->field5c, 1);
    UpdateDartsScoreDisplay(data->configuration70->character, 0, 2);
    const std::int32_t possibleLoss = std::min(scores.defaultScore - scores.scoreFloor, data->field5c);
    UpdateDartsScoreDisplay(data->configuration70->character, possibleLoss, 3);
}
} // namespace event_runtime_detail


void EVT_EnsureDartsPackageMounted() {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("DARTS"), 0))
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("DARTS"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

void EVT_PrepareDartsPlacement(const EventDartsPlacement &placement) {
    if (!DoesScenarioCharacterExist(placement.character))
        return;
    EV_EnsureCharacterDraw(placement.character);
    EV_AT_SetPos(placement.character, EventTransformFlags::AllAxes, placement.position);
    EV_AT_SetAngY(placement.character, EventTransformFlags::AllAxes, placement.rotation, nullptr);
    scenCopyCharaToGame(placement.character);
    if (placement.character.name == DART_COMPACT_BOARD.name) {
        UpdateDartsCompactScoreDisplay(placement.character, 0, 3);
        const std::int32_t firstScore = static_cast<std::int32_t>(GetSavedMinigameScore(12));
        UpdateDartsCompactScoreDisplay(placement.character, static_cast<std::uint32_t>(std::max(firstScore, 50)), 0);
        UpdateDartsCompactScoreDisplay(placement.character, static_cast<std::uint32_t>(GetSavedMinigameScore(13)), 1);
    } else {
        UpdateDartsScoreDisplay(placement.character, 0, 2);
        UpdateDartsScoreDisplay(placement.character, 0, 3);
        const std::int32_t firstScore = static_cast<std::int32_t>(GetSavedMinigameScore(2));
        UpdateDartsScoreDisplay(placement.character, std::max(firstScore, 777777), 0);
        UpdateDartsScoreDisplay(placement.character, static_cast<std::int32_t>(GetSavedMinigameScore(3)), 1);
    }
}

HLib::Task *EVT_SetEnemyDarts(const EventDartsPlacement *placement, const EventDartsEnemy *enemy) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventEnemyDartsCallback, 4, 0x0b, sizeof(EventEnemyDartsTaskData), eventTaskToken);
    EventEnemyDartsTaskData *const data = static_cast<EventEnemyDartsTaskData *>(HLib::GetTaskParameter(task));
    if (!data)
        return nullptr;
    *data = {};
    data->task28 = task;
    data->context30 = enemy->context;
    data->task68 = task;
    data->configuration70 = placement;
    data->enemy78 = enemy;
    EVT_EnsureDartsPackageMounted();
    EVT_PrepareDartsPlacement(*placement);
    return task;
}

HLib::Task *EVT_DartsEasySingle(std::int32_t count, const EventDartsPlacement *firstPlacement) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventDartsEasyCallback, 4, 0x0b, sizeof(EventEnemyDartsTaskData), eventTaskToken);
    EventEnemyDartsTaskData *const data = static_cast<EventEnemyDartsTaskData *>(HLib::GetTaskParameter(task));
    if (!data)
        return nullptr;
    *data = {};
    data->count38 = count;
    data->task68 = task;
    data->configuration70 = firstPlacement;
    EVT_EnsureDartsPackageMounted();
    for (std::int32_t index = 0; index < count; ++index) {
        EVT_PrepareDartsPlacement(firstPlacement[index]);
    }
    return task;
}

const EventDartsState &EVT_GetDartsState() noexcept {
    eventDartsState.active = dartsEventActive != 0;
    return eventDartsState;
}

void EVT_SubmitDartsResult(EventDartsResult result) noexcept {
    const std::size_t index = static_cast<std::size_t>(result);
    constexpr std::array<std::int16_t, 4> resultFlags{1419, 1420, 1422, 1423};
    if (index >= resultFlags.size())
        return;
    ClearDartsResultFlags();
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, resultFlags[index], 1);
}

} // namespace shenmue::s2
