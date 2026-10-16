#include "s2/areas/area_toma.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
std::int32_t tomaRaceEntry;
void InitToma() {
    tomaRaceEntry = 0;
}
void TomaSearchNullsub() {
}
void TomaMain(std::uintptr_t argument) {
    const TomaRequest *const request = reinterpret_cast<const TomaRequest *>(argument);
    if (!request)
        return;
    const TomaMapEntry *const row = FindTomaMap(request->map);
    if ((request->selector == 1 || request->selector == 2) && !row)
        return;
    tomaRaceEntry = 0;
    switch (request->selector) {
    case 0:
    case 2:
        SetPlayerControlEnabled(1);
        break;
    case 1:
        QueueTomaWaitPackage(request->map, request->argument);
        srchReset();
        SetPlayerControlEnabled(0);
        TomaSearchNullsub();
        break;
    case 100:
        QueueTomaFirstTalk();
        break;
    case 101:
        QueueTomaTalk();
        break;
    case 102:
        if (HLib::EnqueueTaskWithParameter(TomaWaitPackageHaveFirstTalk, 4, 0x0b, 0, AREA_EVENT_TASK)) {
            tomaRaceEntry = 2;
            EV_FreeAuthorPak(1, "0122");
            SetPlayerControlEnabled(0);
            EV_NPC_NewLoadSw(0);
        }
        break;
    case 103:
        if (HLib::EnqueueTaskWithParameter(TomaWaitPackageHaveTalk, 4, 0x0b, 0, AREA_EVENT_TASK)) {
            tomaRaceEntry = 0;
            EV_FreeAuthorPak(1, "0122");
            SetPlayerControlEnabled(0);
            EV_NPC_NewLoadSw(0);
        }
        break;
    case 200:
        if (HLib::EnqueueTaskWithParameter(TomaWaitPackageBye, 4, 0x0b, 0, AREA_EVENT_TASK)) {
            EV_SetNewSceneIndex();
            spmFadeOut();
            EV_FreeAuthorPak(1, "0122");
            SetPlayerControlEnabled(0);
            EV_NPC_NewLoadSw(0);
        }
        break;
    default:
        return;
    }
}
void QueueTomaWaitPackage(shen_name map, std::uint32_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TomaWaitPackage, 4, 0x0b, sizeof(TomaWork), AREA_EVENT_TASK);
    if (!task)
        return;
    TomaWork *const work = static_cast<TomaWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->state = 1;
    work->map = map;
    work->argument = argument;
    EV_NPC_NewLoadSw(0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2095, 1);
}
void TomaWaitPackage(void *parameter) {
    TomaWork *const work = static_cast<TomaWork *>(parameter);
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("TOMA")))
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_NPC_NewLoadSw(1);
    HLib::SetCurrentTaskTo(TomaStart);
    const TomaMapEntry *const row = FindTomaMap(work->map);
    if (row) {
        HLib::SetupTaskDelay(-1);
        const shen_name characters[]{row->character, {}};
        QueueTomaNpcs(characters);
    }
}
void TomaStart(void *parameter) {
    TomaWork *const work = static_cast<TomaWork *>(parameter);
    if (filmCheckFadeTrackColour() || !work)
        return;
    const TomaMapEntry *const row = FindTomaMap(work->map);
    if (!row)
        return;
    HLib::SetCurrentTaskTo(TomaDone);
    HLib::SetupTaskDelay(-1);
    SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
    Vector3 playerPosition;
    EV_GetCharaPosition(TOMA_PLAYER, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
    Vector3 sellerPosition = playerPosition;
    std::int32_t yaw = 0;
    EV_FreeAuthorPak(1, "0110");
    if (work->map.name == TOMA_WET0.name) {
        QueueTomaCamera(1550, 0);
        QueueTomaCamera(1551, 200);
        const std::int32_t voice = shenGetRandomInt32(3);
        QueueTomaVoice(voice == 0 ? "0110H001" : voice == 1 ? "0110H002" : "0110H003", 10, nullptr);
        QueueTomaVoice("0110A014", 130, nullptr);
        playerPosition.x = 185.0f;
        playerPosition.z = 107.8f;
        sellerPosition.x = 185.0f;
        sellerPosition.z = 106.27999877929688f;
        yaw = 0x8000;
    } else if (work->map.name == TOMA_ARZ0.name) {
        QueueTomaCamera(1550, 0);
        QueueTomaCamera(1551, 200);
        const std::int32_t voice = shenGetRandomInt32(3);
        QueueTomaVoice(voice == 0 ? "0110G001" : voice == 1 ? "0110G002" : "0110G003", 10, nullptr);
        QueueTomaVoice("0110A012", 130, nullptr);
        playerPosition.x = 331.510009765625f;
        playerPosition.z = 456.2900085449219f;
        sellerPosition.x = 331.5299987792969f;
        sellerPosition.z = 454.8500061035156f;
        yaw = 0x8000;
    }
    QueueTomaVoice(shenGetRandomInt32(2) == 0 ? "0110A020" : "0110A021", 200, ResolveAreaLoopParent);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = TOMA_PLAYER, .flags = 3, .position = playerPosition, .rotation = {0, 0, 0}, .completion = nullptr}, 0);
    if (row->character.name)
        QueueAreaLoopPosition({.parent = HLib::GetCurrentTask(),
                                  .character = row->character,
                                  .flags = 3,
                                  .position = sellerPosition,
                                  .rotation = {0, yaw, 0},
                                  .completion = nullptr},
            0);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = TOMA_PROP, .flags = 3, .position = sellerPosition, .rotation = {0, yaw, 0}, .completion = nullptr}, 0);
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(TOMA_PLAYER));
    SetLowQualityHands(ctrlGetCharaByName(TOMA_PLAYER));
    SetHighModelXYZ(ctrlGetCharaByName(TOMA_PLAYER), 1, 0, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(TOMA_PLAYER), 0, reinterpret_cast<std::byte *>(TOMA_HAND_POSE.data()), 1);
    QueueTomaMotion(TOMA_PLAYER, 0xf003, nullptr, 0);
    if (row->character.name)
        QueueTomaMotion(row->character, 0xf087, TomaChargeCompletion, 0);
    QueueTomaMotion(TOMA_PLAYER, 0xf11f, TomaRewardCompletion, 200);
    if (work->state & 1U) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state &= ~1U;
    }
}
std::int64_t TomaChargeCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    TomaWork *const work = static_cast<TomaWork *>(HLib::GetTaskParameter(parent));
    const TomaMapEntry *const row = work ? FindTomaMap(work->map) : nullptr;
    if (phase == 0 && row)
        for (const TomaOffer &offer : row->offers)
            if (offer.motion.name == work->argument) {
                AddThisToCashOrSegaCoins(-static_cast<std::int16_t>(offer.price), ECashOrCoins::Cash2);
                break;
            }
    return 1;
}
std::int64_t TomaRewardCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    TomaWork *const work = static_cast<TomaWork *>(HLib::GetTaskParameter(parent));
    const TomaMapEntry *const row = work ? FindTomaMap(work->map) : nullptr;
    if (phase == 0 && row)
        for (const TomaOffer &offer : row->offers)
            if (offer.motion.name == work->argument) {
                const std::int32_t category = shenGetRandomInt32(100);
                const std::uint32_t item = category < 25 ? TOMA_REWARDS[shenGetRandomInt32(9)] :
                    category < 40                        ? TOMA_REWARDS[shenGetRandomInt32(5) + 9] :
                                                           TOMA_REWARDS[shenGetRandomInt32(9) + 14];
                AddInventoryItem(item, 1, 1);
                break;
            }
    return 1;
}
void TomaDone(void *parameter) {
    if (GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
        return;
    TomaWork *const work = static_cast<TomaWork *>(parameter);
    if (work) {
        if (filmCheckFadeTrackColour())
            return;
        if (work->state & 1U) {
            work->state &= ~1U;
            return;
        }
        const TomaMapEntry *const row = FindTomaMap(work->map);
        if (row) {
            if (work->argument) {
                const shen_name characters[]{row->character, {}};
                RestoreAreaLoopCharacters(characters);
                work->argument = 0;
                EV_NPC_NewLoadSw(0);
                PrepareNpcAllFromArray();
            } else if (!EV_ForceLoadNpc(row->character))
                return;
        }
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    HasCharacterMotionWork(ctrlGetCharaByName(TOMA_PLAYER));
    hndlSetFlags(ctrlGetCharaByName(TOMA_PLAYER));
    EV_FreeAuthorPak(0, "0110");
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    FinishLoading();
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("TOMA"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TOMA"), 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2095, 0);
    EV_NPC_NewLoadSw(1);
    EV_SetModelDataDefaultVertexShader();
    EV_AssignLightParams(1, 1);
    EV_lightLoad(TOMA_DEFAULT_LIGHT);
    AssignLightProgram(0);
    CheckIsArgEquZero(1);
    SetAreaLoopCharacterControl(TOMA_PLAYER, 0);
    SetPlayerControlEnabled(1);
}
const ScriptAreaDefinition tomaScriptDefinition{.areaId = TOMA_AREA, .field04 = 0, .init = InitToma, .main = TomaMain};
} // namespace shenmue::s2::areas
