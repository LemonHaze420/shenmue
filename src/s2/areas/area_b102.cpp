#include "s2/areas/area_b102.hpp"
#include "s2/areas/area_b102_data.hpp"
#include "s2/areas/area_loop.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2 {
const BayItemShop *FindBayItemShop(shen_name area) {
    for (const auto &shop : bay_item_shop::shops) {
        if (!shop.area.name)
            break;
        if (shop.area.name == area.name)
            return &shop;
    }
    return nullptr;
}
namespace bay_item_shop {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name anchor{SHEN_NAME(b102)};
void UpdateItems(const BayItemShop &shop, shen_name selected = {}) {
    for (const auto &item : shop.items) {
        if (!item.prop.name || (selected.name && selected.name != item.prop.name))
            continue;
        const bool present = GetItemState(static_cast<std::uint32_t>(static_cast<std::int32_t>(item.item))) != 0;
        SetDisplayFlagForProp(item.prop, !present);
        if (item.target.name)
            srchSetTargetEnabled(item.target, !present);
    }
}
std::int64_t ChargeItem(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    auto *work = static_cast<BayItemShopWork *>(HLib::GetTaskParameter(parent));
    const auto *shop = work ? FindBayItemShop(work->area) : nullptr;
    if (phase == 0 && shop)
        for (const auto &item : shop->items)
            if (item.prop.name == work->prop.name) {
                AddThisToCashOrSegaCoins(-item.cost, ECashOrCoins::Cash2);
                break;
            }
    return 1;
}
std::int64_t DeliverItem(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    auto *work = static_cast<BayItemShopWork *>(HLib::GetTaskParameter(parent));
    const auto *shop = work ? FindBayItemShop(work->area) : nullptr;
    if (!phase && shop) {
        for (const auto &item : shop->items)
            if (item.prop.name == work->prop.name) {
                AddInventoryItem(static_cast<std::uint32_t>(static_cast<std::int32_t>(item.item)), 1, 1);
                break;
            }
    } else if (phase == 1) {
        if (shop) {
            UpdateItems(*shop);
            const shen_name characters[]{shop->npc, {}};
            RestoreAreaLoopCharacters(characters);
        }
        HLib::ResolveDeferredCallback(parent);
        HLib::SetupTimedTask(parent, 1);
    }
    return 1;
}
void QueueWait(shen_name area, shen_name prop) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BayItemShopWaitPackageCallback, 4, 0x0b, sizeof(BayItemShopWork), B102_EVENT_TASK);
    if (!task)
        return;
    auto *work = static_cast<BayItemShopWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        FinishLoading();
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->flags = 1;
    work->area = area;
    work->prop = prop;
    EV_NPC_NewLoadSw(0);
}
} // namespace bay_item_shop
void B102_main_func(std::uintptr_t argument) {
    if (!argument)
        return;
    const auto *input = reinterpret_cast<const std::uint32_t *>(argument);
    const shen_name area{input[0]}, prop{input[1]};
    const auto mode = input[2];
    const auto *shop = FindBayItemShop(area);
    if (mode <= 1 && !shop)
        return;
    if (mode == 1) {
        bay_item_shop::QueueWait(area, prop);
        srchReset();
        SetPlayerControlEnabled(0);
        return;
    }
    if (mode == 0 && shop)
        bay_item_shop::UpdateItems(*shop, prop);
    if (mode <= 2)
        SetPlayerControlEnabled(1);
}
void BayItemShopWaitPackageCallback(void *parameter) {
    auto *work = static_cast<BayItemShopWork *>(parameter);
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("B102")))
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_NPC_NewLoadSw(1);
    HLib::SetCurrentTaskTo(BayItemShopStartCallback);
    if (const auto *shop = FindBayItemShop(work->area)) {
        HLib::SetupTaskDelay(-1);
        const shen_name names[]{shop->npc, {}};
        QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
    }
}
void BayItemShopStartCallback(void *parameter) {
    using namespace bay_item_shop;
    auto *work = static_cast<BayItemShopWork *>(parameter);
    if (filmCheckFadeTrackColour() || !work)
        return;
    const auto *shop = FindBayItemShop(work->area);
    if (!shop)
        return;
    HLib::SetCurrentTaskTo(BayItemShopWaitItemEffectCallback);
    HLib::SetupTaskDelay(-1);
    SetAreaLoopCharacterControl(ryo, 1);
    Vector3 ryoPosition{};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &ryoPosition, EventTransformFlags::World);
    Vector3 npcPosition = ryoPosition;
    std::int32_t ryoYaw = 0, npcYaw = 0;
    std::uint32_t npcMotion = 11464;
    EV_FreeAuthorPak(1, "0110");
    switch (work->area.name) {
    case SHEN_NAME(WTR0):
        npcMotion = 11460;
        QueueAreaLoopCamera("B102", 1550, anchor, nullptr, 0);
        QueueAreaLoopCamera("B102", 1551, anchor, nullptr, 200);
        QueueAreaLoopVoice("0110A019", 0, -1, nullptr, 0);
        QueueAreaLoopVoice("0110J009", 0, -1, nullptr, 100);
        QueueAreaLoopVoice("0110J010", 0, -1, ResolveAreaLoopParent, 200);
        ryoYaw = 16384;
        npcYaw = 49152;
        ryoPosition.x = 221.21001f;
        ryoPosition.z = 276.73999f;
        npcPosition.x = 219.58f;
        npcPosition.z = std::bit_cast<float>(1133139067U);
        break;
    case SHEN_NAME(QCAC):
        npcMotion = 11460;
        QueueAreaLoopCamera("B102", 1550, anchor, nullptr, 0);
        QueueAreaLoopCamera("B102", 1551, anchor, nullptr, 200);
        QueueAreaLoopVoice("0110A019", 0, -1, nullptr, 0);
        QueueAreaLoopVoice("0110K009", 0, -1, nullptr, 100);
        QueueAreaLoopVoice("0110K010", 0, -1, ResolveAreaLoopParent, 200);
        ryoYaw = 32768;
        ryoPosition.x = 169.16f;
        ryoPosition.z = 140.60001f;
        npcPosition.x = 169.21001f;
        npcPosition.z = std::bit_cast<float>(1125009326U);
        break;
    case SHEN_NAME(QCAD):
        npcMotion = 11460;
        QueueAreaLoopCamera("B102", 1550, anchor, nullptr, 0);
        QueueAreaLoopCamera("B102", 1551, anchor, nullptr, 200);
        QueueAreaLoopVoice("0110A019", 0, -1, nullptr, 0);
        QueueAreaLoopVoice("0110I012", 0, -1, nullptr, 100);
        QueueAreaLoopVoice("0110I013", 0, -1, ResolveAreaLoopParent, 200);
        ryoYaw = 32768;
        ryoPosition.x = 174.56f;
        ryoPosition.z = 140.60001f;
        npcPosition.x = 174.62f;
        npcPosition.z = std::bit_cast<float>(1125006705U);
        break;
    default:
        break;
    }
    QueueAreaLoopPosition({HLib::GetCurrentTask(), ryo, 3, ryoPosition, {0, ryoYaw, 0}, nullptr}, 0);
    if (shop->npc.name)
        QueueAreaLoopPosition({HLib::GetCurrentTask(), shop->npc, 3, npcPosition, {0, npcYaw, 0}, nullptr}, 0);
    QueueAreaLoopPosition({HLib::GetCurrentTask(), anchor, 3, npcPosition, {0, npcYaw, 0}, nullptr}, 0);
    SetLowQualityHands(ctrlGetCharaByName(ryo));
    SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 0, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ryo), 0, reinterpret_cast<std::byte *>(handPose.data()), 1);
    QueueAreaLoopMotion({HLib::GetCurrentTask(), ryo, 22, 11462, -1, 0, -1, -1, 1, 0, 0, nullptr}, 0);
    if (shop->npc.name)
        QueueAreaLoopMotion({HLib::GetCurrentTask(), shop->npc, 22, npcMotion, -1, 0, -1, -1, 1, 0, 0, ChargeItem}, 0);
    QueueAreaLoopMotion({HLib::GetCurrentTask(), ryo, 22, 11461, -1, 0, 7, -1, 1, 0, 0, DeliverItem}, 200);
    if (work->flags & 1) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
    }
}
void BayItemShopWaitItemEffectCallback(void *parameter) {
    if (GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
        return;
    HLib::SetCurrentTaskTo(BayItemShopDoneCallback);
    SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
    static_cast<BayItemShopWork *>(parameter)->flags |= 1;
}
void BayItemShopDoneCallback(void *parameter) {
    using enum CameraState;
    using namespace bay_item_shop;
    if (GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
        return;
    auto *work = static_cast<BayItemShopWork *>(parameter);
    if (work) {
        if (filmCheckFadeTrackColour())
            return;
        if (work->flags & 1) {
            SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~1U;
        }
        EV_FreeAuthorPak(0, "0110");
    }
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    hndlSetFlags(ctrlGetCharaByName(ryo));
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("B102"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("B102"), 0);
    FinishLoading();
    SetAreaLoopCharacterControl(ryo, 0);
    SetPlayerControlEnabled(1);
}
const ScriptAreaDefinition areaB102ScriptDefinition{.areaId = SHEN_NAME(B102), .field04 = 0, .init = nullptr, .main = B102_main_func};
} // namespace shenmue::s2
