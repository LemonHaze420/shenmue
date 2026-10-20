#include "s2/areas/area_ro10.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
struct Ro10ExitWork {
    HLib::Task *parent;
    HLib::Task *owner;
};
void ShopMain(void *);
void ShopSelect(void *);
void ShopSelectionIcon(void *);
void ShopChoice(void *);
void ShopDeclined(void *);
void ShopAward(void *);
void ShopHand(void *);
void WaitShopPackage(void *);
void WaitShopOwner(void *);
void WaitShopReady(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work || filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 0x20000U)) {
        HLib::SetCurrentTaskTo(WaitShopPackage);
        HLib::SetupTaskDelay(-1);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), work->package);
    } else {
        InitiateCleanupForSceneChara(work->owner);
        if (work->flags & 1U) {
            HLib::SetCurrentTaskTo(WaitShopPackage);
            HLib::SetupTaskDelay(-1);
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), work->package);
        } else {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1U;
        }
    }
}
void WaitShopPackage(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work)
        return;
    HLib::SetCurrentTaskTo(WaitShopOwner);
    HLib::SetupTaskDelay(-1);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    CreateRo10PriceBoards(*work);
    Vector3 position;
    GetRo10ItemTransform(*work->definition, 0, 0, &position, nullptr, nullptr, false);
    EV_AT_SetPos(RO10_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(RO10_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
    SetLowQualityHands(ctrlGetCharaByName(RO10_HAND));
    SetHighModelXYZ(ctrlGetCharaByName(RO10_HAND), 1, 1, 0);
    const shen_name actors[]{work->owner, shen_name{0}};
    QueueAreaLoopNpcs(actors, ResolveAreaLoopParent);
}
void WaitShopOwner(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work)
        return;
    const Ro10Shop &shop = *work->definition;
    const bool alternate = (work->flags & 0x20000U) != 0;
    HLib::SetCurrentTaskTo(Ro10EnterItems);
    if (!alternate)
        HLib::SetupTaskDelay(-1);
    SetAreaLoopCharacterControl(RO10_RYO, 1);
    SetAreaLoopCharacterControl(work->owner, 1);
    if (shop.prop.name)
        SetDisplayFlagForProp(shop.prop, 0);
    if (work->audio[0]) {
        EV_FreeAuthorPak(1, work->audio);
        work->flags |= 2U;
    }
    QueueRo10Motion(RO10_RYO, 11462, 8, 18);
    QueueRo10Motion(work->owner, shop.ownerMotion, 8, 18);
    Vector3 position;
    EV_GetCharaPosition(RO10_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = shop.playerX;
    position.z = shop.playerZ;
    position.y += .5f;
    position.y = shenFindIntersectionWithScene(position, position.y);
    const AreaLoopPositionWork player{
        .parent = HLib::GetCurrentTask(), .character = RO10_RYO, .flags = 3, .position = position, .rotation = {0, shop.playerYaw, 0}, .completion = nullptr};
    QueueAreaLoopPosition(player, 0);
    position.x = shop.ownerX;
    position.z = shop.ownerZ;
    if (work->owner.name) {
        const AreaLoopPositionWork owner{.parent = HLib::GetCurrentTask(),
            .character = work->owner,
            .flags = 3,
            .position = position,
            .rotation = {0, shop.ownerYaw, 0},
            .completion = nullptr};
        QueueAreaLoopPosition(owner, 0);
    }
    RefreshRo10Items(*work);
    if (!alternate) {
        PlayRo10Greeting(*work);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
    }
}
void ShopMain(void *parameter) {
    if (shenGetMinutesUntilNextFrameMinute() <= 1) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work)
        return;
    const Ro10Shop &shop = *work->definition;
    if (++work->frames > 10800) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    const std::int16_t input = static_cast<std::int16_t>(GetCurrentControlInput(2));
    bool moved = true;
    if (input < 0) {
        if (++work->column >= shop.columns) {
            work->column = shop.columns - 1;
            moved = false;
        }
    } else if (input & 0x4000) {
        if (--work->column < 0) {
            work->column = 0;
            moved = false;
        }
    } else if (input & 0x1000) {
        if (--work->row < 0) {
            work->row = 0;
            moved = false;
        }
    } else if (input & 0x2000) {
        if (++work->row >= shop.rows) {
            work->row = shop.rows - 1;
            moved = false;
        }
    } else
        moved = false;
    if (moved)
        work->cameraFrames = 8;
    float distance =
        Ro10FlushSmall(static_cast<float>(static_cast<std::int32_t>(GetCurrentControlInput(4)) - static_cast<std::int32_t>(GetCurrentControlInput(3))));
    distance = Ro10FlushSmall(distance * .00390625f);
    distance = Ro10FlushSmall(distance * .2f);
    distance = Ro10FlushSmall(distance + .5f);
    distance = Ro10FlushSmall(distance * .2f);
    work->cameraDistance = Ro10FlushSmall(work->cameraDistance * .80000001f);
    work->cameraDistance = Ro10FlushSmall(distance + work->cameraDistance);
    UpdateRo10ItemCamera(shop, work->column, work->row, work->cameraDistance, work->cameraFrames);
    const std::int32_t previousFrames = work->cameraFrames;
    if (--work->cameraFrames < 0)
        work->cameraFrames = 0;
    const std::int32_t index = work->column + work->row * shop.columns;
    std::int32_t price = shop.prices[index], description = shop.dialogues[index];
    if (work->purchased[index] & 1U) {
        price = -1;
        description = 0;
    }
    if (!work->cameraFrames) {
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        if (price > 0)
            GetBattleDataConfig(0, SHEN_NAME(BTEV));
        if (description > 0) {
            if (previousFrames)
                Loop3DSoundEffect(0xab, std::fabs(work->cameraDistance));
            GetBattleDataConfig(0, SHEN_NAME(BTFT));
        }
    }
    if (moved || work->cameraFrames)
        return;
    CheckHelpIcon(33, 0, CheckHelpIconOp::FrameOverlay);
    if (GetBattleDataConfig(1, SHEN_NAME(BTCC)) && GetBattleDataConfig(2, SHEN_NAME(BTCC))) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    if (GetBattleDataConfig(1, SHEN_NAME(BTEV)) && GetBattleDataConfig(2, SHEN_NAME(BTEV))) {
        HLib::SetCurrentTaskTo(ShopSelect);
        return;
    }
    if (GetBattleDataConfig(1, SHEN_NAME(BTFT)) && GetBattleDataConfig(2, SHEN_NAME(BTFT)) && shop.dialogues[index] > 0) {
        HLib::SetupTaskDelay(-1);
        PlayRo10ItemDescription(shop.dialogues[index]);
    }
}
void ShopSelect(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    HLib::SetCurrentTaskTo(ShopSelectionIcon);
    HLib::SetupTaskDelay(40);
    PlayRo10Selection(work->shop);
}
void ShopSelectionIcon(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    std::int16_t choices[]{-1, 80, 81, -1};
    SetupControlSpeakAndUI(choices, 10800, -1, 2);
    HLib::SetCurrentTaskTo(ShopChoice);
}
void ShopChoice(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        if (std::fabs(work->cameraDistance - .5f) >= .00000011920929f) {
            work->cameraDistance = Ro10FlushSmall(work->cameraDistance);
            work->cameraDistance = Ro10FlushSmall(work->cameraDistance * .80000001f);
            work->cameraDistance = Ro10FlushSmall(work->cameraDistance + .1f);
            UpdateRo10ItemCamera(*work->definition, work->column, work->row, work->cameraDistance, 0);
        }
        return;
    }
    if (shenGetCtrlSpeakIconFlag() != 80) {
        HLib::SetCurrentTaskTo(ShopDeclined);
        return;
    }
    const std::int32_t index = work->column + work->row * work->definition->columns;
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < work->definition->prices[index]) {
        HLib::SetCurrentTaskTo(ShopMain);
        HLib::SetupTaskDelay(-1);
        PlayRo10InsufficientCash(work->shop);
    } else {
        work->purchased[index] |= 1U;
        HLib::SetCurrentTaskTo(ShopAward);
        HLib::SetupTaskDelay(-1);
        QueueRo10Voice("0110A001", 1, -1, true);
    }
}
void ShopDeclined(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    HLib::SetCurrentTaskTo(ShopMain);
    HLib::SetupTaskDelay(-1);
    QueueRo10Voice("0110A002", 1, -1, true);
}
void ShopAward(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    const std::int32_t grip = work->definition->grips[work->column + work->row * work->definition->columns];
    if (static_cast<std::uint32_t>(grip - 1) > 2U) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    PrepareRo10Award(*work);
    HLib::SetCurrentTaskTo(ShopHand);
}
void ShopHand(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    UpdateRo10Hand(*work);
}
void ExitAfterFade(void *parameter) {
    if (filmCheckFadeTrackColour())
        return;
    Ro10ExitWork &work = *static_cast<Ro10ExitWork *>(parameter);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.owner);
    HLib::ResolveDeferredCallback(work.parent);
    HLib::DestroyCurrentTask();
    SetPlayerControlEnabled(1);
}
void InitializeRo10() {
}
void RunRo10(std::uintptr_t parameter) {
    const shen_name *argument = reinterpret_cast<const shen_name *>(parameter);
    if (!argument)
        return;
    shen_name shop = *argument;
    const std::uint8_t first = static_cast<std::uint8_t>(shop.name);
    const bool alternate = static_cast<std::uint32_t>(first - 'a') <= 25U;
    if (alternate)
        shop.name &= ~0x20U;
    const Ro10Shop *definition = ro10Shops;
    while (definition->shop.name && definition->shop.name != shop.name)
        ++definition;
    if (!definition->shop.name)
        return;
    if (alternate && definition->resetFlag)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(definition->resetFlag), 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(WaitShopReady, 4, 0x0b, sizeof(Ro10Work), RO10_EVENT);
    if (!task)
        return;
    Ro10Work *work = static_cast<Ro10Work *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    std::memset(work, 0, sizeof(*work));
    work->parent = HLib::GetCurrentTask();
    work->self = task;
    std::memcpy(work->package, "RO10", 4);
    std::memcpy(work->package + 4, &shop.name, 4);
    work->package[8] = 0;
    std::strcpy(work->audio, "0110");
    work->flags = alternate ? 0x20000U : 0;
    work->shop = shop;
    work->owner = definition->owner;
    work->definition = definition;
    if (GetNpcNewLoadState())
        work->flags |= 0x10000U;
    if (!alternate) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
    SetPlayerControlEnabled(0);
    EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(definition->help), 0);
}
} // namespace
void Ro10EnterItems(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
    }
    RefreshRo10Items(*work);
    EV_DisableCharacterDisplay(RO10_RYO);
    EV_DisableCharacterDisplay(work->owner);
    work->flags |= 4U;
    work->cameraFrames = 0;
    work->cameraDistance = .5f;
    UpdateRo10ItemCamera(*work->definition, work->column, work->row, .5f, 0);
    HLib::SetCurrentTaskTo(ShopMain);
}
void Ro10Quit(void *parameter) {
    using enum CameraState;
    if (filmCheckFadeTrackColour())
        return;
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    HLib::Task *parent = nullptr;
    std::uint32_t alternate = 0;
    if (work) {
        if (!(work->flags & 1U)) {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1U;
            return;
        }
        parent = work->parent;
        if (work->definition->prop.name)
            SetDisplayFlagForProp(work->definition->prop, 1);
        for (std::int32_t index = 0; index < 16; ++index) {
            if (work->boards[index].name)
                SetCharaKill(work->boards[index], 1);
            if (work->hundreds[index].name)
                SetCharaKill(work->hundreds[index], 1);
            if (work->tens[index].name)
                SetCharaKill(work->tens[index], 1);
            if (work->ones[index].name)
                SetCharaKill(work->ones[index], 1);
        }
        if (work->flags & 4U) {
            EV_EnableCharacterDisplay(RO10_RYO);
            EV_EnableCharacterDisplay(work->owner);
        }
        SetAreaLoopCharacterControl(work->owner, 0);
        if (work->owner.name && DoesScenarioCharacterExist(work->owner)) {
            SetAreaLoopCharacterControl(work->owner, 0);
            EV_npcLockUnlock(work->owner);
        }
        EV_NPC_NewLoadSw((work->flags >> 16) & 1U);
        if ((work->flags & 2U) && work->audio[0]) {
            EV_FreeAuthorPak(0, work->audio);
            work->flags &= ~2U;
        }
        if (work->package[0] && EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work->package), 0)) {
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
            work->package[0] = 0;
        }
        const std::int32_t flag = work->flags & 0x10U ? work->definition->purchasedFlag : work->definition->declinedFlag;
        if (flag)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(flag), 1);
        alternate = work->flags & 0x20000U;
        EV_EventSetUpHelpIcon_0(0, 0);
    }
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    SetAreaLoopCharacterControl(RO10_RYO, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    QueueRo10Motion(RO10_RYO, 0xf002, 0, 2);
    HLib::Task *fadeTask = alternate ? HLib::EnqueueTaskWithParameter(ExitAfterFade, 4, 0x0b, sizeof(Ro10ExitWork), RO10_EVENT) : nullptr;
    if (fadeTask) {
        Ro10ExitWork *exit = static_cast<Ro10ExitWork *>(HLib::GetTaskParameter(fadeTask));
        exit->parent = parent;
        exit->owner = HLib::GetCurrentTask();
        HLib::SetupTaskDelay(-1);
        SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    } else {
        HLib::MarkCurrentTaskForCleanup();
        if (parent)
            HLib::ResolveDeferredCallback(parent);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        SetPlayerControlEnabled(1);
    }
}
const ScriptAreaDefinition ro10ScriptDefinition{.areaId = SHEN_NAME(RO10), .field04 = 0, .init = InitializeRo10, .main = RunRo10};
} // namespace shenmue::s2::areas
