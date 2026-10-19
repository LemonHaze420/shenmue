#include "s2/areas/area_0029.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name characters[]{SHEN_NAME(BKN_), AREA_AUTHOR_INVALID};
void MainCallback(void *);
void LoopCallback(void *);
// @verified - p
void InitNullsub() {
}
void SetCharacters(bool restore) {
    for (shen_name name : {AREA_AUTHOR_RYO, characters[0]}) {
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(name, restore);
    }
    EV_SetCharacterAlphaEnabled(SHEN_NAME(aa08) | 0x10000U, restore);
    EV_SetCharacterAlphaEnabled(SHEN_NAME(ab01) | 0x8020000U, restore);
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
void Cleanup(AreaAuthorTaskParameter *parent) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0029A"), 0);
    EV_FreeAuthorPak(0, "0029");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == SHEN_NAME(WS00)) {
        if (GetItemState(35))
            RemoveInventoryItem(35, 1);
        SetPlayerControlEnabled(1);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void LoopCallback(void *data) {
    auto *parent = *static_cast<AreaAuthorTaskParameter **>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (GetAuthorFlags())
            spmFadeOut();
        QueueExtraTelop("KK", 1);
        RestoreCharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTimedTask(parent->self, 30);
        parent->state = 0;
        mtwkRequestMotion(scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, MotionRegion::LowerBody);
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {112.9000015258789f, 100, 204.5f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 41870, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
            if (FindCharacterByScenarioName(*name)) {
                EV_npcLockUnlock(*name);
                EV_npcFree(*name);
            }
        if (g_currentMap.name == SHEN_NAME(WS00)) {
            EV_EnableCharacterDisplay(SHEN_NAME(ttt0));
            EV_EnableCharacterDisplay(SHEN_NAME(ttt1));
        }
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaAuthorTaskParameter *>(data);
    switch (parent->state) {
    case 0:
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetRenderClockAndCompassTo(0);
        parent->state = 1;
        break;
    case 1:
        if (!filmCheckFadeTrackColour())
            parent->state = 4;
        break;
    case 2: {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
        *static_cast<AreaAuthorTaskParameter **>(HLib::GetTaskParameter(task)) = parent;
        EV_NPC_NewLoadSw(1);
        EV_spmBeginSampleExecute();
        SetupCharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(0029)));
        parent->state = 3;
        break;
    }
    case 4:
        Cleanup(parent);
        break;
    default:
        break;
    }
}
// @verified - p
void Main(std::uintptr_t) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 2;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    SetRenderClockAndCompassTo(1);
    EV_LoadCutsceneFromCurrentScene("0029A000");
    EV_FreeAuthorPak(1, "0029");
    if (g_currentMap.name == SHEN_NAME(WS00)) {
        EV_DisableCharacterDisplay(SHEN_NAME(ttt0));
        EV_DisableCharacterDisplay(SHEN_NAME(ttt1));
    }
    Queue_EVT_UTL_LoadNPCChara_3(characters, task);
}
} // namespace
const ScriptAreaDefinition area0029ScriptDefinition{.areaId = SHEN_NAME(0029), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
