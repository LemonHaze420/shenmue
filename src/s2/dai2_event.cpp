#include "s2/dai2_event.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/hlib.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/step_event.hpp"
namespace shenmue::s2 {
// @verified - p
void DAI2Init() {
}
#endif

// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void DAI2Main(int selector) {
    DAI2Setup();
    if (selector == 0)
        FTBL.EV_authSceneCtrl(DAI2_AUTHOR_RIGHT);
    else
        FTBL.EV_authSceneCtrl(DAI2_AUTHOR_LEFT);
    FTBL.HLib_EnqueueTaskWithoutParameter(DAI2WaitAuthor, 6, 0x0b, DAI2_LOOK_TASK);
#else
void DAI2Main(std::uintptr_t argument) {
    const std::int32_t selector = static_cast<std::int32_t>(static_cast<std::uint32_t>(argument));
    (void)FTBL.EV_NPC_NewLoadSw_0(1);
    (void)FTBL.SetPlayerControlEnabled(0);
    (void)EV_SetCharaControllerFlags(STEP_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    (void)EV_SetCharaControllerFlags(STEP_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    FTBL.SetRefBoardParamsForCharaById(STEP_RYO, 1);
    (void)FTBL.EV_authSceneCtrl(-8);
    AuthSceneCtrl author = static_cast<AuthSceneCtrl>(DAI2_AUTHOR_RIGHT.name);
    if (selector != 0)
        author = static_cast<AuthSceneCtrl>(DAI2_AUTHOR_LEFT.name);
    (void)EV_authSceneCtrl(author);
    FTBL.HLib_EnqueueTaskWithoutParameter(DAI2WaitAuthor, 6, 0x0b, static_cast<std::int32_t>(DAI2_LOOK_TASK.name));
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
// @verified - d
void DAI2Setup(void) {
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.SetPlayerControlEnabled(0);
    do {
        FTBL.EV_SetCharaControllerFlags(DAI2_RYO, 2, 0);
        FTBL.EV_SetCharaControllerFlags(DAI2_RYO, 3, 0);
    } while (0);
    FTBL.SetRefBoardParamsForCharaById(DAI2_RYO, 1);
#if defined(SHENMUE_DAI2_HIDE_FILM_BARS)
    FTBL.EV_authSceneCtrl(-8);
#endif
}
#endif

// @verified - pd
void DAI2WaitAuthor(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_authSceneCtrl(-1) == 0) {
#else
    if (static_cast<std::int32_t>(FTBL.EV_authSceneCtrl(-1)) == 0) {
#endif
        (void)FTBL.camCtrlSetState(0);
        (void)FTBL.camCtrlSetFovTransitionTo(1);
        (void)FTBL.SetPlayerControlEnabled(1);
#if defined(SHENMUE_EVENT_OVERLAY)
        do {
            FTBL.EV_SetCharaControllerFlags(DAI2_RYO, 2, 1);
            FTBL.EV_SetCharaControllerFlags(DAI2_RYO, 3, 1);
        } while (0);
        FTBL.SetRefBoardParamsForCharaById(DAI2_RYO, 0);
#else
        (void)EV_SetCharaControllerFlags(STEP_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
        (void)EV_SetCharaControllerFlags(STEP_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        FTBL.SetRefBoardParamsForCharaById(STEP_RYO, 0);
#endif
        FTBL.HLib_DestroyCurrentTask();
    }
}
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition dai2ScriptDefinition{.areaId = AREA_DAI2, .field04 = 0, .init = &DAI2Init, .main = &DAI2Main};
} // namespace shenmue::s2
#endif
