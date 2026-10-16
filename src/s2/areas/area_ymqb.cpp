#include "s2/areas/area_ymqb.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
void YmqbInitNullsub() {
}
void YmqbSetupNullsub() {
}

void YmqbMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(YmqbStartCallback, 4, 0x0b, sizeof(YmqbWork), YMQB_TASK);
    YmqbWork *const work = static_cast<YmqbWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    YmqbSetup(work);
}

void YmqbSetup(YmqbWork *work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    work->characters = Queue_EVT_AT_ReadChr(work->task, YMQB_CHARACTERS.data());
    work->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT"));
    work->won = 1;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("039E"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("0039"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("YMQB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("YMQB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("YMQB"), 0);
    EV_FreeAuthorPak(1, "0039");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0039A000");
    SetRenderClockAndCompassTo(1);
    YmqbSetupNullsub();
    EV_SetWeatherInstruction(12, 0);
    camCtrlSetFarDistance(60.0f);
    camCtrlSetNearDistance(20.0f);
    flddSetPointFlagOnGrid(YMQB_FIGHT_GRID, 1);
    flddSetPointFlagOnGrid(YMQB_DEFAULT_GRID, 0);
}

void YmqbStartCallback(void *data) {
    YmqbWork *const work = static_cast<YmqbWork *>(data);
    work->nextCallback = YmqbStartBattleCallback;
    work->qteSucceeded = 0;
    AT_SetLocalBaseChara(YMQB_DUMMY, YMQB_RYO);
    EventSetPosition position{.cleanup = 0, .character = YMQB_BASE, .parent = YMQB_INVALID, .position = YMQB_BASE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = YMQB_BASE, .parent = YMQB_INVALID, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = YMQB_DUMMY;
    position.parent = YMQB_RYO;
    position.position = {};
    EVT_AT_SetPosRot(&position);
    angle.character = YMQB_DUMMY;
    angle.parent = YMQB_RYO;
    EVT_AT_SetAngY_Callback(&angle);
    EV_LoadAudio("0039A000", 2);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(ATH2)));
    HLib::SetCurrentTaskTo(YmqbIntroCallback);
}

void YmqbIntroCallback(void *) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    const std::int32_t end = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining));
    if (frame > static_cast<std::int32_t>(static_cast<std::uint32_t>(end) - 40U) || !EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        EVT_AT_QTE(4, 16, 0, 0);
        HLib::SetCurrentTaskTo(YmqbQteResultCallback);
        HLib::SetupTaskDelay(21);
    }
}

void YmqbQteResultCallback(void *data) {
    YmqbWork *const work = static_cast<YmqbWork *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_LoadAudio("0039A000", 2);
    EV_spmBeginSampleExecute();
    if ((EV_GetCurrentQteStatus() & 0x0fff) == 4) {
        work->qteSucceeded = 1;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(QTES)));
    } else {
        work->qteSucceeded = 0;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(QTEF)));
    }
    work->nextCallback = YmqbStartBattleCallback;
    HLib::SetCurrentTaskTo(YmqbWaitAuthorCallback);
}

void YmqbWaitAuthorCallback(void *data) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::SetCurrentTaskTo(static_cast<YmqbWork *>(data)->nextCallback);
}

void YmqbStartBattleCallback(void *data) {
    YmqbWork *const work = static_cast<YmqbWork *>(data);
    DisableScenarioCharacterDisplay(YMQB_A128);
    DisableScenarioCharacterDisplay(YMQB_ANC);
    camCtrlSetState(CameraState::Event);
    camCtrlSetNextPosition(YMQB_BATTLE_EYE);
    camCtrlSetTargetPosition(YMQB_BATTLE_TARGET);
    shenStartFGST(10, 10, 0, 0);
    EventSetPosition position{.cleanup = 0, .character = YMQB_RYO, .parent = YMQB_RYO, .position = {0, 0.2f, 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = YMQB_RYO, .parent = YMQB_RYO, .degrees = 180.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    PublishFightFormation(FindCharacterByScenarioName(YMQB_RYO), nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    Vector3 basePosition;
    EV_GetCharaPosition(YMQB_BASE, CHARACTER_TRANSLATION_ROOT, &basePosition, EventTransformFlags::World);
    vfCopyPosFromEntity(0, nullptr, basePosition.x, basePosition.z, 5.0f);
    const std::int32_t first = work->qteSucceeded ? 2 : 1;
    const std::int32_t count = work->qteSucceeded ? 2 : 3;
    for (std::int32_t index = 0; index < count; ++index) {
        const shen_name character = YMQB_CHARACTERS.data()[first + index];
        position.character = character;
        position.parent = character;
        EVT_AT_SetPosRot(&position);
        PublishFightFormation(FindCharacterByScenarioName(character), nullptr, 1, 1, 200, 2, 0, 0, 0, 0, 0);
    }
    vfmQueueFightStart(static_cast<std::uint8_t>(count + 1), 2, 1.0f, 1, -1);
    EV_camCtrlToggleCombatCamera(2, 1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(YmqbBattleCallback);
}

void YmqbBattleCallback(void *data) {
    if (g_fightActive)
        return;
    YmqbWork *const work = static_cast<YmqbWork *>(data);
    if (GetBtlStatus(FindCharacterByScenarioName(YMQB_RYO)) == 3) {
        work->won = 1;
        EV_stepSetFlags(0, 0x32);
        HLib::SetCurrentTaskTo(YmqbWinAuthorCallback);
    } else {
        work->won = 0;
        EV_stepSetFlags(1, 0x32);
        HLib::SetCurrentTaskTo(YmqbLoseAuthorCallback);
    }
    EventFilmControl fade{.cleanup = 0,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&fade);
    spmFadeOut();
    HLib::SetupTaskDelay(60);
}

void YmqbWinAuthorCallback(void *) {
    EnableScenarioCharacterDisplay(YMQB_A128);
    EnableScenarioCharacterDisplay(YMQB_GEN);
    EnableScenarioCharacterDisplay(YMQB_ANC);
    EV_LoadAudio("0039A000", 3);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(ATH3)));
    HLib::SetCurrentTaskTo(YmqbResultAuthorCallback);
}

void YmqbLoseAuthorCallback(void *) {
    EnableScenarioCharacterDisplay(YMQB_A128);
    EnableScenarioCharacterDisplay(YMQB_GEN);
    EnableScenarioCharacterDisplay(YMQB_ANC);
    EV_LoadAudio("0039A000", 3);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(ATH4)));
    HLib::SetCurrentTaskTo(YmqbResultAuthorCallback);
}

void YmqbResultAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1) {
        for (std::size_t index = 0; index < 6; ++index)
            SetPluralControlFor(FindCharacterByScenarioName(YMQB_CHARACTERS[index]), 1);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(YmqbEndCallback);
        HLib::SetupTaskDelay(60);
    }
}

void YmqbCleanup(YmqbWork *work) {
    if (!work->won)
        shenUpdateCurrentDateTime();
    flddSetPointFlagOnGrid(YMQB_DEFAULT_GRID, 1);
    flddSetPointFlagOnGrid(YMQB_FIGHT_GRID, 0);
    SetPluralControlFor(FindCharacterByScenarioName(YMQB_RYO), 0);
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0039");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YMQB"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0039"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("039E"), 0);
    DeleteMotion(static_cast<std::int32_t>(work->motion));
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(1);
}

void YmqbEndCallback(void *data) {
    YmqbWork *const work = static_cast<YmqbWork *>(data);
    const std::int32_t won = work->won;
    YmqbCleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    RequestGameSystemReset();
    if (g_currentMap.name == AREA_YMQB.name) {
        if (won) {
            scenSetTaskFlags(8);
            EV_scenQueueLoad(g_actNumber, YMQB_WIN_AREA, 58);
        } else {
            EV_scenQueueLoad(g_actNumber, YMQB_LOSE_AREA, 5);
        }
    }
}

const ScriptAreaDefinition ymqbScriptDefinition{.areaId = AREA_YMQB, .field04 = 0, .init = YmqbInitNullsub, .main = YmqbMain};
} // namespace shenmue::s2::areas
