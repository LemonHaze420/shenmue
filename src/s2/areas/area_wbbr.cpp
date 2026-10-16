#include "s2/areas/area_wbbr.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/sap_event.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {
shen_name wbbrCharacters[1]{};
// @verified - p
void WbbrInit() {
    wbbrCharacters[0] = shen_name{};
}
// @verified - p
void WbbrMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    if (!*reinterpret_cast<const std::uint32_t *>(argument))
        (void)HLib::EnqueueTaskWithoutParameter(WbbrInitializeAuthorCallback, 4, 0x0b, static_cast<std::int32_t>(WBBR_TASK.name));
}
// @verified - p
void WbbrInitializeAuthorCallback(void *) {
    InitializeSapAuthorEvent("0035", 0, wbbrCharacters, WbbrSelectAuthorCallback);
}
// @verified - p
void WbbrSelectAuthorCallback(void *) {
    constexpr shen_name authors[]{WBBR_AUTHOR};
    StartWbbrAuthor(authors[shenGetRandomInt32(1)]);
}
// @verified - p
void StartWbbrAuthor(shen_name author) {
    sapEventData.nextCallback = WbbrCompleteAuthorCallback;
    sapEventData.authorFlags = 1;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (sapEventData.authorFlags & 0x80U)
        (void)EV_spmBeginSampleExecute();
    if (sapEventData.authorFlags & 0x20U)
        (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    if (!(sapEventData.authorFlags & 0x40U)) {
        sapEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author.name));
    HLib::SetCurrentTaskTo(SapAuthorWaitCallback);
}
// @verified - p
void WbbrCompleteAuthorCallback(void *) {
    camCtrlSetFovTransitionTo(true);
    (void)camCtrlSetState(CameraState::Init);
    EV_SetCharaControllerFlags(SAP_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(SAP_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WBBR"), 0);
    (void)EV_LoadCutsceneFromCurrentScene("WB00");
    (void)EV_LoadAudio("WB00_000", EV_GetShenmueFlag(0xff));
    (void)EV_spmBeginSampleExecute();
    (void)EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition wbbrScriptDefinition{.areaId = AREA_WBBR, .field04 = 0, .init = WbbrInit, .main = WbbrMain};
} // namespace shenmue::s2
