#include "s2/areas/area_wsye.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sl_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
std::int32_t wsyeSelection{2};
// @verified - p
void WsyeInit() {
    wsyeSelection = 2;
}
// @verified - p
void WsyeMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    if (!*reinterpret_cast<const std::uint32_t *>(argument))
        (void)HLib::EnqueueTaskWithoutParameter(WsyeInitializeAuthorCallback, 4, 0x0d, static_cast<std::int32_t>(WSYE_TASK.name));
}
// @verified - p
void WsyeInitializeAuthorCallback(void *) {
    (void)EV_FreeAuthorPak(1, "0015");
    slEventData.nextCallback = WsyeStartFirstAuthorCallback;
    slEventData.pendingMask = 0;
    slEventData.packageName[0] = 0;
    slEventData.characters[0] = shen_name{};
    HLib::SetCurrentTaskTo(SlPackageWaitCallback);
    (void)SetDoorSubjectEnabled(WSYE_DOOR, 0);
    SetDisplayFlagForProp(WSYE_PROP_FIRST, 0);
    SetDisplayFlagForProp(WSYE_PROP_SECOND, 0);
    (void)ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&wsyeSelection));
}
// @verified - p
void WsyeStartFirstAuthorCallback(void *) {
    constexpr shen_name authors[]{WSYE_AUTHOR_FIRST};
    slEventData.nextCallback = WsyeStartSecondAuthorCallback;
    slEventData.authorFlags = 129;
    slCurrentAuthor = authors[shenGetRandomInt32(1)];
    if (slEventData.authorFlags & 1U)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!(slEventData.authorFlags & 0x40U)) {
        slEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    if (slEventData.authorFlags & 0x20U)
        (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    else
        (void)EV_authSceneCtrl((slEventData.authorFlags & 0x80U) ? AuthSceneCtrl::AllowSkipAuthor : static_cast<AuthSceneCtrl>(-16));
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(slCurrentAuthor.name));
    HLib::SetCurrentTaskTo(SlAuthorWaitCallback);
    if (slCurrentAuthor.name == SL_SNAPSHOT_AUTHOR.name)
        snapAssignFrmCntFlags(136, 120);
}
// @verified - p
void WsyeStartSecondAuthorCallback(void *) {
    constexpr shen_name authors[]{WSYE_AUTHOR_SECOND};
    EV_stepSetFlags(0, 0x266);
    slEventData.authorFlags = 130;
    slEventData.nextCallback = WsyeCompleteAuthorCallback;
    slCurrentAuthor = authors[shenGetRandomInt32(1)];
    if (slEventData.authorFlags & 1U)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!(slEventData.authorFlags & 0x40U)) {
        slEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    if (slEventData.authorFlags & 0x20U)
        (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    else
        (void)EV_authSceneCtrl((slEventData.authorFlags & 0x80U) ? AuthSceneCtrl::AllowSkipAuthor : static_cast<AuthSceneCtrl>(-16));
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(slCurrentAuthor.name));
    HLib::SetCurrentTaskTo(SlAuthorWaitCallback);
    if (slCurrentAuthor.name == SL_SNAPSHOT_AUTHOR.name)
        snapAssignFrmCntFlags(136, 120);
}
// @verified - p
void PlaceWsyePlayer(const Vector3 &position) {
    constexpr Vector3i rotation{0, 1226, 0};
    (void)SetMotionRegionFlag(scenFindMotionwork(WSYE_PLAYER), 71, 0, MotionRegion::LowerBody);
    (void)mtwkSetRequestedRootMode(scenFindMotionwork(WSYE_PLAYER), 0);
    (void)SetMotionFrameTime(scenFindMotionwork(WSYE_PLAYER), 0, MotionRegion::LowerBody);
    EV_AT_SetPos(WSYE_PLAYER, EventTransformFlags::AllAxes, position);
    (void)EV_AT_SetAngY(WSYE_PLAYER, EventTransformFlags::AllAxes, rotation, nullptr);
}
// @verified - p
void WsyeCompleteAuthorCallback(void *) {
    using enum CameraState;
    constexpr Vector3 position{61.16F, 101.35F, 121.58F};
    (void)SetDoorSubjectEnabled(WSYE_DOOR, 1);
    SetDisplayFlagForProp(WSYE_PROP_SECOND, 1);
    SetDisplayFlagForProp(WSYE_PROP_FIRST, 1);
    (void)camCtrlSetFovTransitionTo(true);
    (void)camCtrlSetState(Init);
    PlaceWsyePlayer(position);
    EV_SetCharaControllerFlags(WSYE_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(WSYE_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    (void)HLib::DestroyCurrentTask();
    (void)EV_FreeAuthorPak(0, "0015");
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WSYE"), 0);
    (void)EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition wsyeScriptDefinition{.areaId = AREA_WSYE, .field04 = 0, .init = WsyeInit, .main = WsyeMain};
} // namespace shenmue::s2
