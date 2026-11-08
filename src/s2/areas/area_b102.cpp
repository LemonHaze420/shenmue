#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_b102.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void B102_main_func(const shen_name *argument)
{
    shen_name area, prop;
    int mode;
    const BayItemShop *shop;
    while (argument) {
        area = argument[0];
        prop = argument[1];
        mode = argument[2];
        shop = FindBayItemShop(area);
        if ((unsigned int)mode <= 1 && !shop)
            break;
        switch (mode) {
        case 0:
            BayItemShopUpdateAreaItems(area, prop);
            FTBL.SetPlayerControlEnabled(1);
            break;
        case 1:
            BayItemShopQueueWait(area, prop);
            FTBL.srchReset();
            FTBL.SetPlayerControlEnabled(0);
            FTBL.SetLoadingBusy();
            break;
        case 2:
            FTBL.SetPlayerControlEnabled(1);
            break;
        }
        break;
    }
}
#elif defined(SHENMUE_EVENT_B102_SHOPS)
#define SHENMUE_AREA_LOOP_SOUND
#define SHENMUE_AREA_LOOP_MOTION_FRAMES
#define SHENMUE_AREA_LOOP_VOICE_REVISION 2
#define SHENMUE_AREA_LOOP_HAND_POSES
#include "s2/areas/area_loop_dc.c"

static BayItemShop bayItemShops[] = {
    {SHEN_NAME4('W', 'T', 'R', '0'), SHEN_NAME4('B', 'B', 'T', '_'),
     {{SHEN_NAME4('M', '4', '0', '1'), 0, 3024, 300},
      {SHEN_NAME4('M', '4', '0', '2'), 0, 3011, 200},
      {SHEN_NAME4('M', '4', '0', '3'), 0, 3016, 200},
      {SHEN_NAME4('M', '4', '0', '4'), 0, 3020, 100},
      {SHEN_NAME4('M', '4', '0', '5'), 0, 3019, 100},
      {0, 0, 0, 0}}},
    {SHEN_NAME4('Q', 'C', 'A', 'C'), SHEN_NAME4('J', 'I', '9', '_'),
     {{SHEN_NAME4('M', '4', '0', '1'), 0, 3018, 150},
      {SHEN_NAME4('M', '4', '0', '2'), 0, 3017, 150},
      {SHEN_NAME4('M', '4', '0', '3'), 0, 3014, 150},
      {SHEN_NAME4('M', '4', '0', '4'), 0, 3015, 100},
      {0, 0, 0, 0},
      {0, 0, 0, 0}}},
    {SHEN_NAME4('Q', 'C', 'A', 'D'), SHEN_NAME4('Z', 'Y', 'C', '_'),
     {{SHEN_NAME4('M', '4', '0', '5'), 0, 3023, 500},
      {SHEN_NAME4('M', '4', '0', '6'), 0, 3012, 250},
      {SHEN_NAME4('M', '4', '0', '7'), 0, 3013, 200},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}}},
    {0, 0, {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
};

int BayItemShopUpdateAreaItems(shen_name area, shen_name prop)
{
    int result = 0;
    const BayItemShop *shop;
    do {
        shop = FindBayItemShop(area);
        if (!shop)
            break;
        BayItemShopUpdateItems(shop, prop);
        result = 1;
    } while (0);
    return result;
}

const BayItemShop *FindBayItemShop(shen_name area)
{
    const BayItemShop *shop = 0;
    int index;
    for (index = 0; bayItemShops[index].area; index++) {
        if (bayItemShops[index].area == area) {
            shop = &bayItemShops[index];
            break;
        }
    }
    return shop;
}

void BayItemShopUpdateItems(const BayItemShop *shop, shen_name prop)
{
    int index;
    if (!shop)
        return;
    for (index = 0; index < 6; index++) {
        if (!shop->items[index].prop)
            continue;
        if (prop && prop != shop->items[index].prop)
            continue;
        if (FTBL.GetItemState(shop->items[index].item)) {
            FTBL.SetDisplayFlagForProp(shop->items[index].prop, 0);
            if (shop->items[index].target)
                FTBL.srchSetTargetEnabled(shop->items[index].target, 0);
        } else {
            FTBL.SetDisplayFlagForProp(shop->items[index].prop, 1);
            if (shop->items[index].target)
                FTBL.srchSetTargetEnabled(shop->items[index].target, 1);
        }
    }
}
#else
static char b102Package[] = "B102";

void BayItemShopQueueWait(shen_name area, shen_name prop)
{
    HLibTask *task;
    BayItemShopWork *work;
    while ((task = FTBL.HLib_EnqueueTaskWithParameter(BayItemShopWaitPackageCallback, 4, 11, sizeof(BayItemShopWork), TASK_EVNT)) &&
           (work = (BayItemShopWork *)FTBL.HLib_GetTaskParameter(task))) {
        work->parent = FTBL.HLib_GetCurrentTask();
        work->task = task;
        work->flags = 1;
        work->area = area;
        work->prop = prop;
        FTBL.EV_NPC_NewLoadSw_0(0);
        task = 0;
        break;
    }
    if (task) {
        FTBL.FinishLoading();
        FTBL.HLib_DestroyTask(task);
    }
}

void BayItemShopWaitPackageCallback(void *data)
{
    BayItemShopWork *work = (BayItemShopWork *)data;
    if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)b102Package, 0)) {
        const BayItemShop *shop;
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.HLib_SetCurrentTaskTo(BayItemShopStartCallback);
        shop = FindBayItemShop(work->area);
        if (shop) {
            shen_name names[2];
            FTBL.HLib_SetupTaskDelay(-1);
            names[0] = shop->npc;
            names[1] = 0;
            QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
        }
    }
}

void BayItemShopStartCallback(void *data)
{
    BayItemShopWork *work = (BayItemShopWork *)data;
    const BayItemShop *shop;
    Vector3 ryoPosition, npcPosition;
    Vector3i ryoRotation, npcRotation;
    shen_name npc;
    unsigned int npcMotion;
    while (!FTBL.filmCheckFadeTrackColour() && work && (shop = FindBayItemShop(work->area)) != 0) {
        FTBL.HLib_SetCurrentTaskTo(BayItemShopWaitItemEffectCallback);
        FTBL.HLib_SetupTaskDelay(-1);
        SetAreaLoopCharacterControl(B102_RYO, 1);
        npc = shop->npc;
        npcMotion = 11464;
        FTBL.GetCharaPosition(B102_RYO, -1, &ryoPosition, 0x40000000);
        npcPosition = ryoPosition;
        ryoRotation.x = 0;
        ryoRotation.y = 0;
        ryoRotation.z = 0;
        npcRotation.x = 0;
        npcRotation.y = 0;
        npcRotation.z = 0;
        FTBL.EV_FreeAuthorPak(1, "0110");
        switch (work->area) {
        case B102_AREA_WTR0:
            npcMotion = 11460;
            QueueAreaLoopCamera(b102Package, 1550, B102_ANCHOR, 0, 0);
            QueueAreaLoopCamera(b102Package, 1551, B102_ANCHOR, 200, 0);
            SHENMUE_EVENT_CLOBBER("r11");
            QueueAreaLoopVoice("0110A019", 1, 0, -1, 0);
            QueueAreaLoopVoice("0110J009", 1, 100, -1, 0);
            QueueAreaLoopVoice("0110J010", 1, 200, -1, ResolveAreaLoopParent);
            ryoPosition.x = 221.21f;
            ryoPosition.z = 276.74f;
            ryoRotation.y = 16384;
            npcPosition.x = 219.58f;
            npcPosition.z = 276.66f;
            npcRotation.y = 49152;
            break;
        case B102_AREA_QCAC:
            npcMotion = 11460;
            QueueAreaLoopCamera(b102Package, 1550, B102_ANCHOR, 0, 0);
            QueueAreaLoopCamera(b102Package, 1551, B102_ANCHOR, 200, 0);
            QueueAreaLoopVoice("0110A019", 1, 0, -1, 0);
            QueueAreaLoopVoice("0110K009", 1, 100, -1, 0);
            QueueAreaLoopVoice("0110K010", 1, 200, -1, ResolveAreaLoopParent);
            ryoPosition.x = 169.16f;
            ryoPosition.z = 140.6f;
            ryoRotation.y = 32768;
            npcPosition.x = 169.21f;
            npcPosition.z = 142.28f;
            npcRotation.y = 0;
            break;
        case B102_AREA_QCAD:
            npcMotion = 11460;
            QueueAreaLoopCamera(b102Package, 1550, B102_ANCHOR, 0, 0);
            QueueAreaLoopCamera(b102Package, 1551, B102_ANCHOR, 200, 0);
            QueueAreaLoopVoice("0110A019", 1, 0, -1, 0);
            QueueAreaLoopVoice("0110I012", 1, 100, -1, 0);
            QueueAreaLoopVoice("0110I013", 1, 200, -1, ResolveAreaLoopParent);
            ryoPosition.x = 174.56f;
            ryoPosition.z = 140.6f;
            ryoRotation.y = 32768;
            npcPosition.x = 174.62f;
            npcPosition.z = 142.24f;
            npcRotation.y = 0;
            break;
        }
        do {
            QueueAreaLoopPosition(B102_RYO, 0, 0, &ryoPosition, &ryoRotation, 0);
            QueueAreaLoopPosition(npc, 0, 0, &npcPosition, &npcRotation, 0);
            QueueAreaLoopPosition(B102_ANCHOR, 0, 0, &npcPosition, &npcRotation, 0);
            FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(B102_RYO));
            FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(B102_RYO));
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(B102_RYO), 1, 0, 0);
            FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(B102_RYO), 0, (void *)areaLoopHandPoses[18], 1);
            QueueAreaLoopMotion(B102_RYO, 11462, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 22);
            QueueAreaLoopMotion(npc, npcMotion, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, BayItemShopChargeItem, 22);
            QueueAreaLoopMotion(B102_RYO, 11461, 200, -1, 7.0f, -1.0f, 1.0f, 0.0f, BayItemShopDeliverItem, 22);
            if (work->flags & 1) {
                FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
                work->flags &= ~1;
            }
        } while (0);
        break;
    }
}

int BayItemShopChargeItem(int phase, int value, HLibTask *parent)
{
    BayItemShopWork *work = (BayItemShopWork *)FTBL.HLib_GetTaskParameter(parent);
    const BayItemShop *shop = 0;
    if (work)
        shop = FindBayItemShop(work->area);
    if (phase == 0 && shop) {
        int index;
        for (index = 0; index < 6; index++) {
            if (shop->items[index].prop == work->prop) {
                FTBL.AddThisToCashOrSegaCoins(-shop->items[index].cost, 2);
                break;
            }
        }
    }
    return 1;
}

int BayItemShopDeliverItem(int phase, int value, HLibTask *parent)
{
    BayItemShopWork *work;
    const BayItemShop *shop;
    work = (BayItemShopWork *)FTBL.HLib_GetTaskParameter(parent);
    shop = work ? FindBayItemShop(work->area) : 0;
    switch (phase) {
    case 0:
        if (shop) {
            int index;
            for (index = 0; index < 6; index++) {
                if (shop->items[index].prop == work->prop) {
                    FTBL.AddInventoryItem(shop->items[index].item, 1, 1);
                    break;
                }
            }
        }
        break;
    case 1:
        if (work)
            BayItemShopUpdateAreaItems(work->area, 0);
        if (shop) {
            shen_name names[2];
            names[0] = shop->npc;
            names[1] = 0;
            RestoreAreaLoopCharacters(names, 0);
        }
        FTBL.HLib_ResolveDeferredCallback(parent);
        FTBL.HLib_SetupTimedTask(parent, 1);
        break;
    }
    return 1;
}

void BayItemShopWaitItemEffectCallback(void *data)
{
    if (!FTBL.GetShouldRenderIcons(5)) {
        FTBL.HLib_SetCurrentTaskTo(BayItemShopDoneCallback);
        FTBL.SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
        ((BayItemShopWork *)data)->flags |= 1;
    }
}

void BayItemShopDoneCallback(void *data)
{
    BayItemShopWork *work = (BayItemShopWork *)data;
    while (!FTBL.GetShouldRenderIcons(5)) {
        if (work) {
            if (FTBL.filmCheckFadeTrackColour())
                break;
            if (work->flags & 1) {
                FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
                work->flags &= ~1;
            }
            FTBL.EV_FreeAuthorPak(0, "0110");
        }
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.HLib_MarkCurrentTaskForCleanup();
        FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(B102_RYO));
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(B102_RYO));
        if (FTBL.EV_ControlPackageResources(0, 5, b102Package, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, b102Package, 0, 0);
        FTBL.FinishLoading();
        SetAreaLoopCharacterControl(B102_RYO, 0);
        FTBL.SetPlayerControlEnabled(1);
        break;
    }
}

#endif
#else
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
    if (!EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("B102")))
        return;
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
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
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("B102"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("B102"), 0);
    FinishLoading();
    SetAreaLoopCharacterControl(ryo, 0);
    SetPlayerControlEnabled(1);
}
const ScriptAreaDefinition areaB102ScriptDefinition{.areaId = SHEN_NAME(B102), .field04 = 0, .init = nullptr, .main = B102_main_func};
} // namespace shenmue::s2
#endif
