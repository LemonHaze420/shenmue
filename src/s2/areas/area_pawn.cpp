#include "s2/areas/area_pawn.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {
namespace {
void PawnDone(void *);
void PawnWaitPackage(void *);
void PawnWaitOwner(void *);
void PawnBegin(void *);
void PawnToSelect(void *);
void PawnExecuteSelect(void *);
void PawnCancel(void *);
void PawnNotSelling(void *);
void PawnSetTransaction(void *);
void PawnItemTransaction(void *);
void PawnSaleDialogue(void *);
bool PawnFlag(std::int16_t flag) {
    return EV_GetPlayerFlag(PlayerFlagReadMode::Internal, flag) != 0;
}
void SetPawnFlag(std::int16_t flag, std::int32_t value) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, flag, value);
}
void PawnTalkCallback(void *parameter) {
    PawnTalkWork *work = static_cast<PawnTalkWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    const bool idle = !IsScenarioBusy();
    if (work->started) {
        if (camCtrlCheckIsEventCam())
            SetPawnSaleTransition(65537);
        if (!idle)
            return;
        if (work->parent) {
            HLib::ResolveDeferredCallback(work->parent);
            HLib::SetupTimedTask(work->parent, 1);
        }
        SetPlayerGameFlags(PlayerGameFlagMode::Bit26, reinterpret_cast<void *>(1));
        if (static_cast<std::uint32_t>(work->flag - 1224) <= 1U) {
            srchReset();
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(CameraState::Init);
            InitiateCleanupForSceneChara(work->character);
        } else
            SetScenarioFollowCharacter(work->character);
        HLib::DestroyCurrentTask();
    } else if (idle)
        work->started = static_cast<std::int32_t>(StartScenarioFreeTalkRecord(work->character, -1));
}
void QueuePawnTalk(HLib::Task *parent, shen_name character, std::int32_t flag) {
    if (!character.name || !flag)
        return;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(PawnTalkCallback, 4, 0x0b, sizeof(PawnTalkWork), PAWN_EVENT);
    if (!task)
        return;
    PawnTalkWork *work = static_cast<PawnTalkWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = parent;
    work->flag = flag;
    work->character = character;
    SetPawnFlag(static_cast<std::int16_t>(flag), 1);
    EVS_LoopCamera(3000, PAWN_BASE, PAWN_BASE);
    camCtrlSetState(CameraState::SnapYawTarget);
    camCtrlDisableFilmBars();
    SetScenarioFollowCharacter(character);
    work->started = static_cast<std::int32_t>(StartScenarioFreeTalkRecord(character, -1));
}
void PawnPositionCallback(void *parameter) {
    AreaLoopPositionWork *work = static_cast<AreaLoopPositionWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->flags & 1U)
        EV_AT_SetPos(
            work->character, (work->flags & 4U) ? EventTransformFlags::AllAxes : EventTransformFlags::World | EventTransformFlags::AllAxes, work->position);
    if (work->flags & 2U)
        EV_AT_SetAngY(
            work->character, (work->flags & 4U) ? EventTransformFlags::AllAxes : EventTransformFlags::World | EventTransformFlags::AllAxes, work->rotation);
    if (work->completion)
        work->completion(0, work->character.name, work->parent);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyCurrentTask();
}
void PawnInitialize(void *parameter) {
    if (filmCheckFadeTrackColour())
        return;
    PawnEventWork &work = *static_cast<PawnEventWork *>(parameter);
    HLib::SetCurrentTaskTo(PawnWaitPackage);
    HLib::SetupTaskDelay(-1);
    EV_ResetFreeTalkFlags();
    EV_NPC_NewLoadSw(0);
    if (work.package[0])
        QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), work.package);
}
void PawnWaitPackage(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    EV_LoadCutsceneFromCurrentScene("01210000");
    const shen_name characters[]{FindPawnShopScene(work->shop).owner, shen_name{0}};
    HLib::SetCurrentTaskTo(PawnWaitOwner);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopNpcs(characters, ResolveAreaLoopParent);
}
void PawnWaitOwner(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work) {
        EV_NPC_NewLoadSw(1);
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    const PawnShopScene &scene = FindPawnShopScene(work->shop);
    EV_GetCharaPosition(scene.owner, CHARACTER_TRANSLATION_ROOT, &work->savedPosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(scene.owner, &work->savedRotation, EventTransformFlags::World);
    SetAreaLoopCharacterControl(PAWN_RYO, 1);
    if (scene.flyer.name)
        SetAreaLoopCharacterControl(scene.flyer, 1);
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(PawnBegin);
}
void PawnBegin(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    const PawnShopScene &scene = FindPawnShopScene(work->shop);
    if (!scene.owner.name) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    const std::int32_t index = static_cast<std::int32_t>(&scene - pawnShopScenes);
    bool flyer = GetItemState(static_cast<std::uint32_t>(9000 + index)) == 0;
    if (scene.presentationFlag) {
        const bool requested = PawnFlag(static_cast<std::int16_t>(scene.presentationFlag));
        flyer = flyer || requested;
        SetPawnFlag(static_cast<std::int16_t>(scene.presentationFlag), 0);
    }
    Vector3 ownerPosition;
    EV_GetCharaPosition(PAWN_RYO, CHARACTER_TRANSLATION_ROOT, &ownerPosition, EventTransformFlags::World);
    ownerPosition.x = flyer ? scene.flyerX : scene.businessX;
    ownerPosition.z = flyer ? scene.flyerZ : scene.businessZ;
    const Vector3i ownerRotation{0, flyer ? scene.flyerYaw : scene.businessYaw, 0};
    Vector3 playerPosition{flyer ? scene.flyerOffsetX : scene.businessOffsetX, 0, flyer ? scene.flyerOffsetZ : scene.businessOffsetZ};
    ApplyScenarioTranslationAndRotation(&ownerPosition, &ownerRotation, &playerPosition);
    QueuePawnScenePosition(PAWN_RYO, playerPosition, Vector3i{0, ownerRotation.y + 0x8000, 0});
    QueuePawnScenePosition(scene.owner, ownerPosition, ownerRotation);
    QueuePawnScenePosition(PAWN_BASE, ownerPosition, ownerRotation);
    for (std::int16_t flag : pawnDialogueFlags)
        SetPawnFlag(flag, 0);
    if (flyer)
        HLib::SetCurrentTaskTo(PawnBeginFlyer);
    else {
        HLib::SetCurrentTaskTo(PawnToSelect);
        HLib::SetupTaskDelay(-1);
        QueuePawnTalk(HLib::GetCurrentTask(), scene.owner, 1222);
    }
}
void PawnDone(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (filmCheckFadeTrackColour() || !work || IsScenarioBusy())
        return;
    if (work->flyerTask)
        HLib::DestroyTask(work->flyerTask);
    SetAreaLoopCharacterControl(PAWN_RYO, 0);
    const PawnShopScene &scene = FindPawnShopScene(work->shop);
    if (scene.flyer.name)
        SetAreaLoopCharacterControl(scene.flyer, 0);
    if (!scene.owner.name)
        return;
    if (work->package[0])
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
    const shen_name characters[]{scene.owner, shen_name{0}};
    RestoreAreaLoopCharacters(characters);
    camCtrlSetState(CameraState::SnapYawTarget);
    SetScenarioFollowCharacter(shen_name{0});
    camCtrlSetFovTransitionTo(true);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetState(CameraState::Init);
    mtwkRequestMotion(scenFindMotionwork(PAWN_RYO), 0xf003, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(PAWN_RYO), 0);
    HLib::ResolveDeferredCallback(work->parent);
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
}
void PawnToSelect(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work)
        HLib::SetCurrentTaskTo(PawnDone);
    else if (!IsScenarioBusy()) {
        HLib::SetCurrentTaskTo(PawnBeginSelect);
        RestorePawnTransactionPose(*work);
    }
}
void PawnCancel(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    HLib::SetCurrentTaskTo(PawnDone);
    if (!work)
        return;
    const shen_name owner = FindPawnShopScene(work->shop).owner;
    if (owner.name) {
        HLib::SetupTaskDelay(-1);
        QueuePawnTalk(HLib::GetCurrentTask(), owner, 1225);
    }
}
void PawnNotSelling(void *parameter) {
    HLib::SetCurrentTaskTo(PawnCancel);
    PawnCancel(parameter);
}
void PawnExecuteSelect(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    const std::int16_t choice = static_cast<std::int16_t>(shenGetCtrlSpeakIconFlag());
    if (choice == 203) {
        HLib::SetCurrentTaskTo(PawnNotSelling);
        PawnNotSelling(parameter);
        return;
    }
    if (choice != 204 && choice != 205 && choice != 237 && choice != 238) {
        HLib::SetCurrentTaskTo(PawnCancel);
        PawnCancel(parameter);
        return;
    }
    const std::uint32_t mode = choice == 204 ? 5U : choice == 238 ? 3U : 2U;
    if (!OpenPAWNShop(work->shop, mode)) {
        EV_ResetFreeTalkFlags();
        return;
    }
    HLib::SetCurrentTaskTo(choice == 205 || choice == 237 ? PawnSetTransaction : PawnItemTransaction);
}
void PawnTransaction(void *parameter, bool sets) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work || !FindPawnShopScene(work->shop).owner.name) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    if (IsPAWNShopOpen()) {
        if (shenGetMinutesUntilNextFrameMinute() <= 0) {
            HLib::SetCurrentTaskTo(PawnNotSelling);
            SetPawnSaleTransition(2);
            ClosePAWNShop();
        }
        return;
    }
    HLib::Task *parent = nullptr;
    std::int32_t dialogue = 1227;
    if (GetPawnSaleResult() < 0) {
        HLib::SetCurrentTaskTo(PawnToSelect);
        HLib::SetupTaskDelay(-1);
        parent = HLib::GetCurrentTask();
        work->saleOutcome = sets ? -1 : 0;
        dialogue = 1226;
    } else {
        HLib::SetCurrentTaskTo(PawnSaleDialogue);
        if (!sets)
            work->saleOutcome = 4;
        else {
            const std::int32_t price = GetPawnSalePrice();
            work->saleOutcome = price > 200 ? 1 : price > 100 ? 2 : 3;
            dialogue = price > 200 ? 1228 : price > 100 ? 1227 : 1229;
        }
    }
    QueuePawnTalk(parent, FindPawnShopScene(work->shop).owner, dialogue);
}
void PawnSetTransaction(void *parameter) {
    PawnTransaction(parameter, true);
}
void PawnItemTransaction(void *parameter) {
    PawnTransaction(parameter, false);
}
void PawnSaleDialogue(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work || !FindPawnShopScene(work->shop).owner.name) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    if (PawnFlag(1223)) {
        SetPawnFlag(1223, 0);
        AddThisToCashOrSegaCoins(CommitPawnSale(), ECashOrCoins::Cash2);
    }
    if (IsScenarioBusy())
        return;
    for (std::int16_t flag : pawnPendingDialogueFlags)
        if (PawnFlag(flag))
            return;
    RestorePawnTransactionPose(*work);
    HLib::SetCurrentTaskTo(PawnBeginSelect);
}
void InitializePawn() {
}
void RunPawn(std::uintptr_t argument) {
    const shen_name shop{static_cast<std::uint32_t>(argument)};
    if (!FindPawnShopScene(shop).owner.name)
        return;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(PawnInitialize, 4, 0x0b, sizeof(PawnEventWork), PAWN_EVENT);
    if (!task)
        return;
    PawnEventWork *work = static_cast<PawnEventWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    std::memset(work, 0, sizeof(*work));
    work->self = task;
    work->parent = HLib::GetCurrentTask();
    work->shop = shop;
    std::memcpy(work->package, "PAWN", 4);
    std::memcpy(work->package + 4, &shop.name, 4);
    work->package[8] = 0;
    EV_setHoldStatesFlags(0);
    srchReset();
    camCtrlSetState(CameraState::Init);
    camCtrlDisableFilmBars();
    HLib::SetupTimedTask(work->parent, -1);
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(8, 0, 0, 0, 0, 255, 0, 0, 0);
}
} // namespace
void QueuePawnScenePosition(shen_name character, const Vector3 &position, const Vector3i &rotation) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(PawnPositionCallback, 4, 0x0b, sizeof(AreaLoopPositionWork), PAWN_EVENT);
    if (!task)
        return;
    AreaLoopPositionWork *work = static_cast<AreaLoopPositionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->character = character;
    work->flags = 0x80000003U;
    work->position = position;
    work->rotation = rotation;
    work->completion = nullptr;
    PawnPositionCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
void PawnFinish(void *parameter) {
    PawnDone(parameter);
}
void RestorePawnTransactionPose(PawnEventWork &work) {
    const PawnShopScene &scene = FindPawnShopScene(work.shop);
    const std::int32_t index = static_cast<std::int32_t>(&scene - pawnShopScenes);
    constexpr std::int32_t cameras[]{2600, 2610, 2620, 2630, 2650, 2660, 2670, 2690, 2680, 2640, 2670, 2670};
    constexpr std::uint32_t motions[]{7324, 7328, 7325, 7324, 7325, 7322, 7324, 7324, 7324, 7325, 7322, 7325};
    if (index >= 12)
        return;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work.package), 0);
    EVS_LoopCamera(cameras[index], PAWN_BASE, PAWN_BASE);
    camCtrlDisableFilmBars();
    HLib::Task *ownerTask = HLib::EnqueueTaskWithParameter(AreaLoopMotionCallback, 4, 0x0b, sizeof(AreaLoopMotionWork), PAWN_EVENT);
    if (ownerTask) {
        AreaLoopMotionWork *ownerMotion = static_cast<AreaLoopMotionWork *>(HLib::GetTaskParameter(ownerTask));
        if (ownerMotion) {
            ownerMotion->parent = HLib::GetCurrentTask();
            ownerMotion->character = scene.owner;
            ownerMotion->flags = 0x80000002U;
            ownerMotion->motion = motions[index];
            ownerMotion->duration = 1;
            ownerMotion->elapsed = 0;
            ownerMotion->startFrame = -1;
            ownerMotion->endFrame = -1;
            ownerMotion->speed = 1;
            ownerMotion->frameTime = 0;
            ownerMotion->completion = nullptr;
            AreaLoopMotionCallback(ownerMotion);
            if (ownerMotion->flags & 0x80000000U)
                ownerMotion->flags &= 0x7fffffffU;
            else
                HLib::DestroyTask(ownerTask);
        } else
            HLib::DestroyTask(ownerTask);
    }

    HLib::Task *task = HLib::EnqueueTaskWithParameter(AreaLoopMotionCallback, 4, 0x0b, sizeof(AreaLoopMotionWork), PAWN_EVENT);
    if (!task)
        return;
    AreaLoopMotionWork *motion = static_cast<AreaLoopMotionWork *>(HLib::GetTaskParameter(task));
    if (!motion) {
        HLib::DestroyTask(task);
        return;
    }
    motion->parent = HLib::GetCurrentTask();
    motion->character = PAWN_RYO;
    motion->motion = 7323;
    motion->duration = 1;
    motion->startFrame = -1;
    motion->endFrame = -1;
    motion->speed = 1;
    motion->frameTime = 0;
    motion->elapsed = 0;
    motion->flags = 0x80000002U;
    AreaLoopMotionCallback(motion);
    if (motion->flags & 0x80000000U)
        motion->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
void PawnBeginSelect(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    const PawnShopScene &scene = FindPawnShopScene(work->shop);
    if (!scene.owner.name) {
        HLib::SetCurrentTaskTo(PawnDone);
        return;
    }
    InitiateCleanupForSceneChara(scene.owner);
    if (shenGetMinutesUntilNextFrameMinute() <= 0) {
        HLib::SetCurrentTaskTo(PawnNotSelling);
        return;
    }
    Vector3 position;
    EV_GetCharaPosition(PAWN_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = scene.businessX;
    position.z = scene.businessZ;
    const Vector3i rotation{0, scene.businessYaw, 0};
    Vector3 offset{scene.businessOffsetX, 0, scene.businessOffsetZ};
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    EV_SetCharacterAlphaEnabled(scene.owner, false);
    EV_SetCharacterAlphaEnabled(PAWN_RYO, false);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit26, reinterpret_cast<void *>(1));
    if (GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
        return;
    const std::uint32_t modes = GetPawnAvailableSaleModes(work->shop) & 0xbU;
    if (!modes) {
        HLib::SetCurrentTaskTo(PawnDone);
        HLib::SetupTaskDelay(-1);
        QueuePawnTalk(HLib::GetCurrentTask(), scene.owner, 1224);
        return;
    }
    std::int16_t choices[4]{-1, -1, -1, -1};
    switch (modes) {
    case 1:
        choices[1] = 237;
        choices[2] = 203;
        break;
    case 2:
        choices[1] = 238;
        choices[2] = 203;
        break;
    case 3:
        choices[1] = 237;
        choices[2] = 238;
        choices[3] = 203;
        break;
    case 8:
        choices[1] = 204;
        choices[2] = 203;
        break;
    case 9:
        choices[1] = 237;
        choices[2] = 204;
        choices[3] = 203;
        break;
    case 10:
        choices[1] = 238;
        choices[2] = 204;
        choices[3] = 203;
        break;
    default:
        choices[0] = 204;
        choices[1] = 237;
        choices[2] = 238;
        choices[3] = 203;
        break;
    }
    SetupControlSpeakAndUI(choices, 800, -1, 2);
    HLib::SetCurrentTaskTo(PawnExecuteSelect);
}
const ScriptAreaDefinition pawnScriptDefinition{.areaId = SHEN_NAME(PAWN), .field04 = 0, .init = InitializePawn, .main = RunPawn};
} // namespace shenmue::s2::areas
