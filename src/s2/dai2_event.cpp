#include "s2/dai2_event.hpp"
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

// @verified - p
void DAI2Main(std::uintptr_t argument) {
    const std::int32_t selector = static_cast<std::int32_t>(static_cast<std::uint32_t>(argument));
    (void)EV_NPC_NewLoadSw(1);
    (void)SetPlayerControlEnabled(0);
    (void)EV_SetCharaControllerFlags(STEP_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    (void)EV_SetCharaControllerFlags(STEP_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(STEP_RYO, 1);
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    AuthSceneCtrl author = static_cast<AuthSceneCtrl>(DAI2_AUTHOR_RIGHT.name);
    if (selector != 0)
        author = static_cast<AuthSceneCtrl>(DAI2_AUTHOR_LEFT.name);
    (void)EV_authSceneCtrl(author);
    HLib::EnqueueTaskWithoutParameter(DAI2WaitAuthor, 6, 0x0b, static_cast<std::int32_t>(DAI2_LOOK_TASK.name));
}

// @verified - p
void DAI2WaitAuthor(void *) {
    if (static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
        (void)camCtrlSetState(CameraState::Init);
        (void)camCtrlSetFovTransitionTo(true);
        (void)SetPlayerControlEnabled(1);
        (void)EV_SetCharaControllerFlags(STEP_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
        (void)EV_SetCharaControllerFlags(STEP_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetRefBoardParamsFor(STEP_RYO, 0);
        HLib::DestroyCurrentTask();
    }
}

const ScriptAreaDefinition dai2ScriptDefinition{
    .areaId = AREA_DAI2,
    .field04 = 0,
    .init = &DAI2Init,
    .main = &DAI2Main,
};

} // namespace shenmue::s2
