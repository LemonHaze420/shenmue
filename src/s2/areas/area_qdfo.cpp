#include "s2/areas/area_qdfo.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
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
#endif

#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void QdfoMain(unsigned int argument) {
#else
void QdfoMain(std::uintptr_t) {
#endif
    HLibTask *const task = FTBL.HLib_EnqueueTaskWithParameter(QdfoTaskCallback, 4, 0x0b, sizeof(QdfoEventWork), QDFO_TASK);
    (void)InitializeQdfoCharacters(task);
#if !defined(SHENMUE_EVENT_OVERLAY)
    SetSoundProgramPhaseFlags(2);
#endif
}
#endif

#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
HLibTask *InitializeQdfoCharacters(HLibTask *task) {
    QdfoEventWork *const work = (QdfoEventWork *)FTBL.HLib_GetTaskParameter(task);
    do {
        work->state = 0;
        work->task = task;
    } while (0);
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    do {
        FTBL.EV_SetCharaControllerFlags(QDFO_RYO, 2, 0);
        FTBL.EV_SetCharaControllerFlags(QDFO_RYO, 3, 0);
    } while (0);
    FTBL.SetPlayerGameFlags(2, 0);
    FTBL.SetRefBoardParamsForCharaById(QDFO_RYO, 1);
    FTBL.EV_ControlPackageResources(2, 2, "QDFO", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0246");
#else
    EV_SetCharaControllerFlags(QDFO_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(QDFO_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    (void)SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_SetRefBoardParamsFor(QDFO_RYO, 1);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDFO"), 0);
    (void)EV_FreeAuthorPak(1, "0246");
#endif
    return Queue_EVT_UTL_LoadNPCChara_3(QDFO_CHARACTERS, work->task);
}

// @verified - pd
void QdfoTaskCallback(void *taskData) {
    QdfoEventWork *const work = (QdfoEventWork *)taskData;
    switch (work->state) {
    case 0:
#if defined(SHENMUE_EVENT_OVERLAY)
        HideAuthorCharacter(QDFO_RYO);
        HideAuthorCharacter(QDFO_REN);
        HideAuthorProp(QDFO_BACKGROUND);
        FTBL.EV_authSceneCtrl(AREA_QDFO);
#else
        (void)mtwkSetExtraFlag(scenFindMotionwork(QDFO_RYO), 6, 2);
        EV_SetCharacterAlphaEnabled(QDFO_RYO, false);
        (void)mtwkSetExtraFlag(scenFindMotionwork(QDFO_REN), 6, 2);
        EV_SetCharacterAlphaEnabled(QDFO_REN, false);
        EV_SetCharacterAlphaEnabled(QDFO_BACKGROUND, false);
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_QDFO.name));
#endif
        work->state = 2;
        break;
    case 2:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 3;
        break;
    case 3:
        FTBL.EV_scenQueueLoad(g_actNumber, QDFO_RETURN_AREA, 1);
        break;
    default:
        break;
    }
}

#if defined(SHENMUE_EVENT_OVERLAY)
// @verified - d
void HideAuthorCharacter(shen_name character) {
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

// @verified - d
void HideAuthorProp(shen_name prop) {
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

const shen_name QDFO_CHARACTERS[] = {QDFO_REN, 0xffffffffU};
#else
const ScriptAreaDefinition qdfoScriptDefinition{.areaId = AREA_QDFO, .field04 = 0, .init = QdfoInit, .main = QdfoMain};
} // namespace shenmue::s2
#endif
#endif
