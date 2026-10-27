#include "s2/areas/area_0014.hpp"
#include "s2/character.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/initialization.hpp"
#include "s2/inventory.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <array>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name characters[]{SHEN_NAME(WON_), AREA_AUTHOR_INVALID};
constexpr std::array<shen_name, 4> alphaCharacters{SHEN_NAME(a016), SHEN_NAME(a009), SHEN_NAME(ab11) | 0x4000000U, SHEN_NAME(aa36) | 0x1000000U};
std::int32_t savedRootRenderMode{};
void MainCallback(void *);
void LoopCallback(void *);
// @verified - p
void InitNullsub() {
}
void SetCharacters(bool restore) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, restore ? 0 : 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, restore);
    for (shen_name name : alphaCharacters)
        EV_SetCharacterAlphaEnabled(name, restore);
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name) {
        mtwkSetExtraFlag(scenFindMotionwork(*name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(*name, restore);
    }
}
// @verified - p
void SetupCharacters() {
    SetCharacters(false);
}
// @verified - p
void RestoreCharacters() {
    SetCharacters(true);
}
// @verified - p
void ConfigureEnvironment() {
    shenSetClockAndCompassRenderTo(1);
    if (g_currentMap.name == SHEN_NAME(0014)) {
        DateTime time;
        GetCurrentDateTime(time);
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
            EV_SetObjectFlagByIndex(14, 0);
            EV_SetObjectFlagByIndex(15, 1);
            SetLegacyVertexColourType(2);
        } else {
            EV_lightLoad(SHEN_NAME(0000));
            PushGlobalLightingProgram();
            EV_SetObjectFlagByIndex(14, 1);
            EV_SetObjectFlagByIndex(15, 0);
            SetLegacyVertexColourType(1);
        }
    }
    EV_LoadCutsceneFromCurrentScene("0014A000");
    EV_FreeAuthorPak(1, "0014");
}
// @verified - p
void LoopCallback(void *data) {
    using enum CameraState;
    const std::uint32_t frame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1)
        savedRootRenderMode = GetRootRenderDeferredMode();
    else if (frame == 521) {
        SetRootRenderDeferredMode(0);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    RestoreCharacters();
    mtwkRequestMotion(scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, MotionRegion::LowerBody);
    EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {255.8800048828125f, 100.19999694824219f, 350.5f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 4523, 0}, 0};
    EVT_AT_SetAng_Callback(&angle);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0014A"), 0);
    auto *parent = *static_cast<AreaImmediateAuthorTaskParameter **>(data);
    if (g_currentMap.name == SHEN_NAME(AR02)) {
        AddInventoryItem(0x178a, 1, 1);
        parent->state = 6;
    } else
        parent->state = 5;
    SetRootRenderDeferredMode(savedRootRenderMode);
    HLib::DestroyCurrentTask();
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaImmediateAuthorTaskParameter *>(data);
    switch (parent->state) {
    case 0:
        EV_LoadNPCRoute("NPC.BIN");
        parent->state = 1;
        break;
    case 1:
        EV_NPC_NewLoadSw(1);
        EV_LoadCutsceneFromCurrentScene("AR02");
        ExecuteCleanupFuncForAreaWithCachedScene();
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        shenSetClockAndCompassRenderTo(0);
        parent->state = 2;
        break;
    case 2:
        if (!filmCheckFadeTrackColour())
            parent->state = 5;
        break;
    case 3: {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
        *static_cast<AreaImmediateAuthorTaskParameter **>(HLib::GetTaskParameter(task)) = parent;
        SetupCharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(0014)));
        parent->state = 4;
        break;
    }
    case 5:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
        EV_FreeAuthorPak(0, "0014");
        EV_CleanupMapControlTask();
        HLib::DestroyTask(parent->self);
        if (g_currentMap.name == SHEN_NAME(AR02))
            SetPlayerControlEnabled(1);
        else
            EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    case 6:
        if (!GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
            parent->state = 0;
        break;
    default:
        break;
    }
}
// @verified - p
void Main(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{0});
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 3;
    EV_stepSetFlags(0, 64);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    ConfigureEnvironment();
    Queue_EVT_UTL_LoadNPCChara_3(characters, parent->self);
}
} // namespace
const ScriptAreaDefinition area0014ScriptDefinition{.areaId = SHEN_NAME(0014), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
