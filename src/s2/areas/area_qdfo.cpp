#include "s2/areas/area_qdfo.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {

// @verified - p
void QdfoInit() {
}

// @verified - p
void QdfoMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QdfoTaskCallback, 4, 0x0b, sizeof(QdfoEventWork), QDFO_TASK);
    (void)InitializeQdfoCharacters(task);
    SetSoundProgramPhaseFlags(2);
}

// @verified - p
void QdfoTaskCallback(void *taskData) {
    QdfoEventWork *const work = static_cast<QdfoEventWork *>(taskData);
    switch (work->state) {
    case 0:
        (void)mtwkSetExtraFlag(scenFindMotionwork(QDFO_RYO), 6, 2);
        EV_SetCharacterAlphaEnabled(QDFO_RYO, false);
        (void)mtwkSetExtraFlag(scenFindMotionwork(QDFO_REN), 6, 2);
        EV_SetCharacterAlphaEnabled(QDFO_REN, false);
        EV_SetCharacterAlphaEnabled(QDFO_BACKGROUND, false);
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_QDFO.name));
        work->state = 2;
        break;
    case 2:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 3;
        break;
    case 3:
        EV_scenQueueLoad(g_actNumber, QDFO_RETURN_AREA, 1);
        break;
    default:
        break;
    }
}

// @verified - p
HLib::Task *InitializeQdfoCharacters(HLib::Task *task) {
    QdfoEventWork *const work = static_cast<QdfoEventWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->task = task;
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(QDFO_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(QDFO_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    (void)SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_SetRefBoardParamsFor(QDFO_RYO, 1);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDFO"), 0);
    (void)EV_FreeAuthorPak(1, "0246");
    return Queue_EVT_UTL_LoadNPCChara_3(QDFO_CHARACTERS, work->task);
}

const ScriptAreaDefinition qdfoScriptDefinition{.areaId = AREA_QDFO, .field04 = 0, .init = QdfoInit, .main = QdfoMain};

} // namespace shenmue::s2
