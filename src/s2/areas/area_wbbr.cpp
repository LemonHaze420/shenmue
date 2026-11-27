#include "s2/events/event_api.h"
#include "s2/areas/area_wbbr.hpp"
#if defined(SHENMUE_EVENT_OVERLAY)
extern shen_name wbbrCharacters[1];
#else
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
#endif
#if defined(SHENMUE_EVENT_ENTRY)
void WbbrMain(int *argument) {
    FTBL.SetPlayerControlEnabled(0);
    if (!*argument)
        FTBL.HLib_EnqueueTaskWithoutParameter(WbbrInitializeAuthorCallback, 4, 11, SAP_TASK);
    else {
        FTBL.DebugLogToFileRemoved("ERROR  :  0035A\n");
        FTBL.EventAssertNullsub("./src/et_auth_find_burin.c", 81);
    }
}
#else
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name wbbrCharacters[1] = {0};
void SetWbbrCharacterTransform(shen_name character, const Vector3 *position, int yaw) {
    Vector3i rotation = {0, yaw, 0};
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 71, 0, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(character), 0, 0);
    FTBL.EV_AT_SetPos(character, 0x38000000, position);
    FTBL.EV_AT_SetAngY_0(character, 0x38000000, &rotation, 0);
}
void SetWbbrCharacterPosition(shen_name character, float x, float y, float z, int yaw) {
    Vector3 position = {x, y, z};
    SetWbbrCharacterTransform(character, &position, yaw);
}
#else
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
#endif
// @verified - p
void WbbrInitializeAuthorCallback(void *taskData) {
    InitializeSapAuthorEvent("0035", 0, wbbrCharacters, WbbrSelectAuthorCallback);
}
// @verified - p
void WbbrSelectAuthorCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    shen_name authors[] = {WBBR_AUTHOR};
    SelectSapAuthor(authors, 1, WbbrCompleteAuthorCallback, 1);
#else
    constexpr shen_name authors[]{WBBR_AUTHOR};
    StartWbbrAuthor(authors[shenGetRandomInt32(1)]);
#endif
}
#if !defined(SHENMUE_EVENT_OVERLAY)
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
#endif
// @verified - p
void WbbrCompleteAuthorCallback(void *taskData) {

    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.EV_SetCharaControllerFlags(SAP_PLAYER, 2, 1);
    FTBL.EV_SetCharaControllerFlags(SAP_PLAYER, 3, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    SHENMUE_EVENT_USE(&FTBL.SetPlayerControlEnabled);
#endif
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.HLib_DestroyCurrentTask();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.DebugLogToFileRemoved("__ ぶりんしょ発見オーサ終了2 __\n");
#endif
    FTBL.EV_ControlPackageResources(0, 6, "WBBR", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("WB00");
    FTBL.EV_LoadAudio("WB00_000", FTBL.EV_GetShenmueFlag(255));
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_NPC_NewLoadSw_0(1);
}
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition wbbrScriptDefinition{.areaId = AREA_WBBR, .field04 = 0, .init = WbbrInit, .main = WbbrMain};
} // namespace shenmue::s2
#endif
#endif
