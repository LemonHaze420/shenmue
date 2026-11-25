#include "s2/hide_event.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2 {
// @verified - p
void HIDEInit() {
}
#endif

// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void HIDEMain(unsigned int argument) {
#else
void HIDEMain(std::uintptr_t) {
#endif
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(HIDE_RYO), 0xf003, 0);
    FTBL.HLib_EnqueueTaskWithoutParameter(HIDEPlayAuthor, 6, 0x0b, HIDE_TASK);
#else
    (void)mtwkRequestMotion(scenFindMotionwork(HIDE_RYO), 0xf003, MotionRegion::LowerBody);
    FTBL.HLib_EnqueueTaskWithoutParameter(HIDEPlayAuthor, 6, 0x0b, static_cast<std::int32_t>(HIDE_TASK.name));
#endif
}

// @verified - pd
void HIDEPlayAuthor(void *taskData) {
    (void)SetupRyoForHIDE();
    FTBL.EV_FreeAuthorPak(1, HIDE_PACKAGE);
    (void)FTBL.EV_LoadCutsceneFromCurrentScene(HIDE_MEDIA);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(HIDE_AUTHOR);
#else
    (void)FTBL.EV_authSceneCtrl(static_cast<std::int32_t>(HIDE_AUTHOR.name));
#endif
    FTBL.EV_lightLoad(HIDE_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.HLib_SetCurrentTaskTo(HIDEWaitAuthor);
}

// @verified - pd
void HIDEWaitAuthor(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_authSceneCtrl(-1) == 0) {
#else
    if (static_cast<std::uint32_t>(FTBL.EV_authSceneCtrl(-1)) == 0) {
#endif
        FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(HIDEEndRoom);
    }
}

// @verified - pd
void HIDEEndRoom(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EnableHideDoor();
    FTBL.EV_ControlPackageResources(0, 6, "HIDE", 0, 0);
#else
    (void)FTBL.SetDoorSubjectEnabled(HIDE_DOOR, 1);
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("HIDE"), 0, 0);
#endif
    FTBL.EV_FreeAuthorPak(0, HIDE_PACKAGE);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_stepSetFlags(0x274, 0);
#else
    EV_stepSetFlags(0, 0x274);
#endif
    FTBL.HLib_DestroyCurrentTask();
}

// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void SetupRyoForHIDE() {
    const float values[4] = {1.0f, 0.3f, 0.3f, 0.3f};
#else
std::uint64_t SetupRyoForHIDE() {
    const Vector4 values = HIDE_REFBOARD;
#endif
    FTBL.EV_EnableCharacterDisplay(HIDE_RYO);
#if defined(SHENMUE_EVENT_OVERLAY)
    do {
        FTBL.EV_SetCharaControllerFlags(HIDE_RYO, 2, 0);
        FTBL.EV_SetCharaControllerFlags(HIDE_RYO, 3, 0);
    } while (0);
#else
    (void)EV_SetCharaControllerFlags(HIDE_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    (void)EV_SetCharaControllerFlags(HIDE_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(HIDE_RYO, 0);
    FTBL.SetRefBoardParamsForCharaById(HIDE_RYO, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetScenarioReflectorBoardValues(HIDE_RYO, values);
    FTBL.SetScenarioReflectorBoardValues(HIDE_REN, values);
#else
    (void)SetScenarioReflectorBoardValues(HIDE_RYO, &values);
    (void)SetScenarioReflectorBoardValues(HIDE_REN, &values);
#endif
    (void)FTBL.SetScenarioCharacterFlag40(HIDE_RYO, 0);
    (void)FTBL.SetScenarioCharacterFlag40(HIDE_REN, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetDoorSubjectEnabled(HIDE_DOOR, 0);
#else
    return FTBL.SetDoorSubjectEnabled(HIDE_DOOR, 0);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
// @verified - d
void EnableHideDoor() {
    FTBL.SetDoorSubjectEnabled(HIDE_DOOR, 1);
}
#else
const ScriptAreaDefinition hideScriptDefinition{.areaId = HIDE_AREA, .field04 = 0, .init = &HIDEInit, .main = &HIDEMain};
} // namespace shenmue::s2
#endif
