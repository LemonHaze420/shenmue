#include "s2/hide_event.hpp"
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

// @verified - p
void HIDEMain(std::uintptr_t) {
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    (void)mtwkRequestMotion(scenFindMotionwork(HIDE_RYO), 0xf003, MotionRegion::LowerBody);
    HLib::EnqueueTaskWithoutParameter(HIDEPlayAuthor, 6, 0x0b, static_cast<std::int32_t>(HIDE_TASK.name));
}

// @verified - p
std::uint64_t SetupRyoForHIDE() {
    const Vector4 values = HIDE_REFBOARD;
    EV_EnableCharacterDisplay(HIDE_RYO);
    (void)EV_SetCharaControllerFlags(HIDE_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    (void)EV_SetCharaControllerFlags(HIDE_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(HIDE_RYO, 0);
    EV_SetRefBoardParamsFor(HIDE_RYO, 1);
    (void)SetScenarioReflectorBoardValues(HIDE_RYO, &values);
    (void)SetScenarioReflectorBoardValues(HIDE_REN, &values);
    (void)SetScenarioCharacterFlag40(HIDE_RYO, 0);
    (void)SetScenarioCharacterFlag40(HIDE_REN, 0);
    return SetDoorSubjectEnabled(HIDE_DOOR, 0);
}

// @verified - p
void HIDEPlayAuthor(void *) {
    (void)SetupRyoForHIDE();
    EV_FreeAuthorPak(1, HIDE_PACKAGE);
    (void)EV_LoadCutsceneFromCurrentScene(HIDE_MEDIA);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(HIDE_AUTHOR.name));
    EV_lightLoad(HIDE_LIGHT);
    PushGlobalLightingProgram();
    HLib::SetCurrentTaskTo(HIDEWaitAuthor);
}

// @verified - p
void HIDEWaitAuthor(void *) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(HIDEEndRoom);
    }
}

// @verified - p
void HIDEEndRoom(void *) {
    (void)SetDoorSubjectEnabled(HIDE_DOOR, 1);
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("HIDE"), 0, 0);
    EV_FreeAuthorPak(0, HIDE_PACKAGE);
    EV_stepSetFlags(0, 0x274);
    HLib::DestroyCurrentTask();
}

const ScriptAreaDefinition hideScriptDefinition{.areaId = HIDE_AREA, .field04 = 0, .init = &HIDEInit, .main = &HIDEMain};

} // namespace shenmue::s2
