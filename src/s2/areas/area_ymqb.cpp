#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_ymqb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void YmqbEntry(void)
{
    YmqbMain();
}
#else
const shen_name ymqbCharacters[] = {YMQB_RYO, YMQB_GEN, YMQB_SBO, YMQB_CHR, YMQB_ANC, YMQB_SYE, EVENT_CHARACTER_INVALID};
Vector3 ymqbBattleEye = {241.5115966796875f, 117.04940032958984f, 348.73779296875f};
Vector3 ymqbBattleTarget = {246.410400390625f, 116.04640197753906f, 349.1564025878906f};

void YmqbMain(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(YmqbStartCallback, 4, 11, sizeof(YmqbWork), TASK_EVNT);
    YmqbWork *work = (YmqbWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    YmqbSetup(work);
    EventDebugRegister(YMQB_DEBUG_ID, YmqbDebugWinCallback, work, 2000000000);
}

void YmqbDebugWinCallback(void *data)
{
    FTBL.EV_stepSetFlags(50, 0);
    YmqbEnd((YmqbWork *)data);
}

void YmqbEnd(YmqbWork *work)
{
    int won = work->won;
    YmqbCleanup(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(YMQB_DEBUG_ID);
    if (g_currentMap == YMQB_AREA) {
        if (won) {
            FTBL.scenSetTaskFlags(8);
            FTBL.EV_scenQueueLoad(g_actNumber, YMQB_WIN_AREA, 58);
        } else
            FTBL.EV_scenQueueLoad(g_actNumber, YMQB_LOSE_AREA, 5);
    }
}

void YmqbResetFight(YmqbFight *fight)
{
    fight->nextCallback = YmqbStartBattleCallback;
    fight->qteSucceeded = 0;
}

void YmqbStartCallback(void *data)
{
    YmqbWork *work = (YmqbWork *)data;
    EventDebugCheck(YMQB_DEBUG_ID, 4);
    YmqbResetFight(&work->fight);
    FTBL.AT_SetLocalBaseChara(YMQB_DUMMY, YMQB_RYO);
    EVT_AT_SetPos(YMQB_BASE, EVENT_CHARACTER_INVALID, 244.7f, 115.15f, 347.7f, 0);
    EVT_AT_SetAngY(YMQB_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    EVT_AT_SetPos(YMQB_DUMMY, YMQB_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(YMQB_DUMMY, YMQB_RYO, 0.0f, 0);
    FTBL.EV_LoadAudio("0039A000", 2);
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_authSceneCtrl(-11);
    FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'T', 'H', '2'));
    FTBL.HLib_SetCurrentTaskTo(YmqbIntroCallback);
}

void YmqbIntroCallback(void *data)
{
    int frame = FTBL.EV_authSceneCtrl(-6);
    int end = FTBL.EV_authSceneCtrl(-5);
    if (frame > end - 40 || !FTBL.EV_authSceneCtrl(-1)) {
        EVT_AT_QTE(4, 16, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(YmqbQteResultCallback);
        FTBL.HLib_SetupTaskDelay(21);
    }
}

void YmqbQteResultCallback(void *data)
{
    YmqbFight *fight = &((YmqbWork *)data)->fight;
    FTBL.EV_authSceneCtrl(-10);
    FTBL.EV_LoadAudio("0039A000", 2);
    EVT_AT_SetSample(0, 0, 0);
    if ((FTBL.GetCurrentQteStatus() & 0xfff) == 4) {
        fight->qteSucceeded = 1;
        FTBL.EV_authSceneCtrl(SHEN_NAME4('Q', 'T', 'E', 'S'));
    } else {
        fight->qteSucceeded = 0;
        FTBL.EV_authSceneCtrl(SHEN_NAME4('Q', 'T', 'E', 'F'));
    }
    fight->nextCallback = YmqbStartBattleCallback;
    FTBL.HLib_SetCurrentTaskTo(YmqbWaitAuthorCallback);
}

void YmqbWaitAuthorCallback(void *data)
{
    if (!FTBL.EV_authSceneCtrl(-1))
        FTBL.HLib_SetCurrentTaskTo(((YmqbWork *)data)->fight.nextCallback);
}

void YmqbStartBattleCallback(void *data)
{
    YmqbFight *fight = &((YmqbWork *)data)->fight;
    Vector3 basePosition;
    int i;
    EVT_AT_CharaDisp(YMQB_A128, 0, 1);
    EVT_AT_CharaDisp(YMQB_ANC, 0, 1);
    FTBL.camCtrlSetState(9);
    FTBL.camCtrlSetNextPosition(&ymqbBattleEye);
    FTBL.camCtrlSetTargetPosition(&ymqbBattleTarget);
    FTBL.shenStartFGST(10, 10, 0, 0);
    EVT_AT_SetPos(YMQB_RYO, YMQB_RYO, 0.0f, 0.2f, 0.0f, 0);
    EVT_AT_SetAngY(YMQB_RYO, YMQB_RYO, 180.0f, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(YMQB_RYO), 0, 0, 0, 0, 0);
    FTBL.GetCharaPosition(YMQB_BASE, -1, &basePosition, 0x40000000);
    FTBL.vfCopyPosFromEntity(0, 0, basePosition.x, basePosition.z, 5.0f);
    if (fight->qteSucceeded) {
        for (i = 2; i < 4; i++) {
            shen_name character = ymqbCharacters[i];
            EVT_AT_SetPos(character, character, 0.0f, 0.2f, 0.0f, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(character), 0, 1, 1, 200, 2);
        }
        FTBL.vfmQueueFightStart(3, 2, 1, 1.0f, -1);
    } else {
        for (i = 1; i < 4; i++) {
            shen_name character = ymqbCharacters[i];
            EVT_AT_SetPos(character, character, 0.0f, 0.2f, 0.0f, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(character), 0, 1, 1, 200, 2);
        }
        FTBL.vfmQueueFightStart(4, 2, 1, 1.0f, -1);
    }
    FTBL.EV_vfBattleUtil(1, 2, 1);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(YmqbBattleCallback);
}

void YmqbBattleCallback(void *data)
{
    YmqbWork *work = (YmqbWork *)data;
    if (FTBL.IsFightActive())
        return;
    if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(YMQB_RYO)) == 3) {
        work->won = 1;
        FTBL.EV_stepSetFlags(50, 0);
        FTBL.HLib_SetCurrentTaskTo(YmqbWinAuthorCallback);
    } else {
        work->won = 0;
        FTBL.EV_stepSetFlags(50, 1);
        FTBL.HLib_SetCurrentTaskTo(YmqbLoseAuthorCallback);
    }
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
    EVT_AT_SetSample(0, 1, 0);
    FTBL.HLib_SetupTaskDelay(60);
}

void YmqbWinAuthorCallback(void *data)
{
    EVT_AT_CharaDisp(YMQB_A128, 0, 0);
    EVT_AT_CharaDisp(YMQB_GEN, 0, 0);
    EVT_AT_CharaDisp(YMQB_ANC, 0, 0);
    FTBL.EV_LoadAudio("0039A000", 3);
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'T', 'H', '3'));
    FTBL.HLib_SetCurrentTaskTo(YmqbResultAuthorCallback);
}

void YmqbLoseAuthorCallback(void *data)
{
    EVT_AT_CharaDisp(YMQB_A128, 0, 0);
    EVT_AT_CharaDisp(YMQB_GEN, 0, 0);
    EVT_AT_CharaDisp(YMQB_ANC, 0, 0);
    FTBL.EV_LoadAudio("0039A000", 3);
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'T', 'H', '4'));
    FTBL.HLib_SetCurrentTaskTo(YmqbResultAuthorCallback);
}

void YmqbResultAuthorCallback(void *data)
{
    int i;
    if (FTBL.EV_authSceneCtrl(-6) == 1)
        for (i = 0; i < 6; i++)
            FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ymqbCharacters[i]), 1);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetCurrentTaskTo(YmqbEndCallback);
        FTBL.HLib_SetupTaskDelay(60);
    }
}

void YmqbEndCallback(void *data)
{
    YmqbEnd((YmqbWork *)data);
}

void YmqbSetup(YmqbWork *work)
{
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    work->characters = Queue_EVT_AT_ReadChr(work->task, ymqbCharacters);
    work->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    work->won = 1;
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"039E", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"0039", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"YMQB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "YMQB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "YMQB", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0039");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0039A000");
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.camCtrlSetFarDistance(60.0f);
    FTBL.camCtrlSetNearDistance(20.0f);
    FTBL.flddSetPointFlagOnGrid(YMQB_FIGHT_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(YMQB_DEFAULT_GRID, 0);
}

void YmqbCleanup(YmqbWork *work)
{
    if (!work->won)
        AdvanceSleepToMorning();
    FTBL.flddSetPointFlagOnGrid(YMQB_DEFAULT_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(YMQB_FIGHT_GRID, 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(YMQB_RYO), 0);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0039");
    FTBL.EV_ControlPackageResources(0, 6, "YMQB", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "0039", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "039E", 0, 0);
    FTBL.DeleteMotion(work->motion);
    ET_AT_KillWork(work->characters);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(1);
}
#endif
#else
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
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("039E"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("0039"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("YMQB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("YMQB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("YMQB"), 0);
    EV_FreeAuthorPak(1, "0039");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0039A000");
    shenSetClockAndCompassRenderTo(1);
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
    using enum CameraState;
    YmqbWork *const work = static_cast<YmqbWork *>(data);
    DisableScenarioCharacterDisplay(YMQB_A128);
    DisableScenarioCharacterDisplay(YMQB_ANC);
    camCtrlSetState(Event);
    camCtrlSetNextPosition(YMQB_BATTLE_EYE);
    camCtrlSetTargetPosition(YMQB_BATTLE_TARGET);
    shenStartFGST(10, 10, 0, 0);
    EventSetPosition position{.cleanup = 0, .character = YMQB_RYO, .parent = YMQB_RYO, .position = {0, 0.2f, 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = YMQB_RYO, .parent = YMQB_RYO, .degrees = 180.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    PublishFightFormation(ctrlGetScenarioChara(YMQB_RYO), nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
        PublishFightFormation(ctrlGetScenarioChara(character), nullptr, 1, 1, 200, 2, 0, 0, 0, 0, 0);
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
    if (GetBtlStatus(ctrlGetScenarioChara(YMQB_RYO)) == 3) {
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
            SetPluralControlFor(ctrlGetScenarioChara(YMQB_CHARACTERS[index]), 1);
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
    SetPluralControlFor(ctrlGetScenarioChara(YMQB_RYO), 0);
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0039");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YMQB"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0039"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("039E"), 0);
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
#endif
