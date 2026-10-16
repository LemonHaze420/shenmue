#include "s2/areas/area_tgbt.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/street_fight_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2 {
namespace {
std::int16_t tgbtChoices[]{-1, 231, 232, -1};
}
// @verified - p
void TgbtInit() {
}
// @verified - p
void TgbtDormantBoundary() {
}
// @verified - p
void TgbtMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (DoesScenarioCharacterExist(TGBT_REN)) {
        if (IsNpcLoadedOrVisible(TGBT_REN))
            EV_npcLockUnlock(TGBT_REN);
        EV_npcFree(TGBT_REN);
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TgbtEventCallback, 4, 0x0b, sizeof(TgbtEventWork), TGBT_TASK);
    TgbtEventWork *const work = static_cast<TgbtEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    work->characters = Queue_EVT_AT_ReadChr(task, TGBT_LOAD_CHARACTERS);
    work->frames = 0;
    work->retries = 0;
    InitializeTgbtResources();
    SetRenderClockAndCompassTo(1);
}
// @verified - p
void InitializeTgbtResources() {
    doorLoadAuthorWithAudio(TGBT_PACKAGE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0280A000.smp");
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("TGBT"), 0);
    SetDoorSubjectEnabled(TGBT_PROP, 0);
    EV_LoadAudio("0280A000", 0);
    EV_FreeAuthorPak(1, "0280");
    EV_spmBeginSampleExecute();
    CreateEventCharacter(CharacterCreationHighModel, nullptr, TGBT_BASE);
    EVT_AT_SetPos(TGBT_BASE, TGBT_INVALID, {244.77f, 118.0f, 153.14999f}, 0);
    EVT_AT_SetAngY(TGBT_BASE, TGBT_INVALID, 0, 0);
}
// @verified - p
void QueueTgbtAuthor(shen_name author) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TgbtAuthorCallback, 4, 0x0b, sizeof(TgbtAuthorWork), TGBT_AUTHOR_TASK);
    TgbtAuthorWork *const work = static_cast<TgbtAuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    SetPlayerControlEnabled(0);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    SetWeatherInstruction(0x0cU, false);
    work->author = author;
}
// @verified - p
void TgbtAuthorCallback(void *data) {
    TgbtAuthorWork *const work = static_cast<TgbtAuthorWork *>(data);
    if (work->state == 0) {
        HLib::DestroyCurrentTask();
    } else if (work->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(TgbtAuthorWaitCallback, 4, 0x0b, sizeof(TgbtAuthorWaitWork), TGBT_AUTHOR_TASK);
        TgbtAuthorWaitWork *const wait = static_cast<TgbtAuthorWaitWork *>(HLib::GetTaskParameter(task));
        wait->parent = work;
        wait->state = 0;
        PrepareTgbtAuthoredCharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->author.name));
        work->state = 2;
    } else if (work->state == 3) {
        HLib::DestroyTask(work->task);
    }
}
// @verified - p
void TgbtAuthorWaitCallback(void *data) {
    TgbtAuthorWaitWork *const work = static_cast<TgbtAuthorWaitWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        work->parent->state = 0;
        RestoreTgbtAuthoredCharacters();
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void PrepareTgbtAuthoredCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(TGBT_PLAYER), 6, 2);
    EV_SetCharacterAlphaEnabled(TGBT_PLAYER, false);
    mtwkSetExtraFlag(scenFindMotionwork(TGBT_FIRST_FIGHTER), 6, 2);
    EV_SetCharacterAlphaEnabled(TGBT_FIRST_FIGHTER, false);
    for (const shen_name character : TGBT_AUTHOR_CHARACTERS) {
        EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{});
        EV_SetCharacterAlphaEnabled(character, false);
    }
    for (const shen_name character : TGBT_HIDDEN_CHARACTERS) {
        if (character.name == TGBT_INVALID.name)
            break;
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
    }
}
// @verified - p
void RestoreTgbtAuthoredCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(TGBT_PLAYER), 6, 0);
    EV_SetCharacterAlphaEnabled(TGBT_PLAYER, true);
    for (const shen_name character : TGBT_AUTHOR_CHARACTERS)
        EV_SetCharacterAlphaEnabled(character, true);
    for (const shen_name character : TGBT_HIDDEN_CHARACTERS) {
        if (character.name == TGBT_INVALID.name)
            break;
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
        EV_SetCharacterAlphaEnabled(character, true);
    }
}
// @verified - p
void CleanupTgbtEvent(TgbtEventWork *work) {
    for (const shen_name character : TGBT_LOAD_CHARACTERS) {
        if (character.name == TGBT_INVALID.name)
            break;
        EV_npcLockUnlock(character);
    }
    ET_AT_KillWork(work->characters);
    EV_setHoldStatesFlags(1);
    LoadCutsceneAndSoundProgramForArea();
    doorLoadAuthorSilently(shen_name{});
    EV_FreeAuthorPak(0, "0280");
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetDoorSubjectEnabled(TGBT_PROP, 1);
    EV_AT_SetPos(TGBT_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, TGBT_EXIT_POSITION);
    EV_AT_SetAngY(TGBT_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{});
    SetCharaKill(TGBT_BASE, 1);
    SetRenderClockAndCompassTo(0);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FinishLoading();
    scenFreeEvent("TGBT.EV3");
}
// @verified - p
void TgbtEventCallback(void *data) {
    TgbtEventWork *const work = static_cast<TgbtEventWork *>(data);
    if (work->frames != 0) {
        --work->frames;
        return;
    }
    switch (work->state) {
    case 0:
        EV_LoadAudio("0280A000", 0);
        EV_spmBeginSampleExecute();
        work->state = 1;
        [[fallthrough]];
    case 1:
        if (NpcWait())
            return;
        for (const shen_name character : TGBT_LOAD_CHARACTERS) {
            if (character.name == TGBT_INVALID.name)
                break;
            EV_CheckWarmStartNpcRoute(character);
        }
        work->state = 2;
        [[fallthrough]];
    case 2:
        shenStartFGST(10, 10, 0, 0);
        shenInitFightData(FindCharacterByScenarioName(TGBT_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (work->retries + 2)), 0);
        shenInitFightData(FindCharacterByScenarioName(TGBT_FIRST_FIGHTER), nullptr, 1, 1, 100, 2);
        vfUpdateFlagsFor(FindCharacterByScenarioName(TGBT_FIRST_FIGHTER), 0, 0x400000, 128);
        vfUpdateFlagsFor(FindCharacterByScenarioName(TGBT_FIRST_FIGHTER), 0, 0x200000, 2048);
        shenInitFightData(FindCharacterByScenarioName(TGBT_SECOND_FIGHTER), nullptr, 1, 1, 60, 3);
        vfUpdateFlagsFor(FindCharacterByScenarioName(TGBT_SECOND_FIGHTER), 0, 0x400000, 128);
        vfUpdateFlagsFor(FindCharacterByScenarioName(TGBT_SECOND_FIGHTER), 0, 0x200000, 2048);
        shenInitFightData(FindCharacterByScenarioName(TGBT_THIRD_FIGHTER), nullptr, 1, 1, 60, 4);
        vfUpdateFlagsFor(FindCharacterByScenarioName(TGBT_THIRD_FIGHTER), 0, 0x400000, 128);
        vfUpdateFlagsFor(FindCharacterByScenarioName(TGBT_THIRD_FIGHTER), 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 243.66000366210938f, 152.77000427246094f, 3.9000000953674316f);
        [[fallthrough]];
    case 3:
        if (work->retries)
            EV_SetAuthorActiveTo(1);
        QueueTgbtAuthor(TGBT_AUTHORS[0]);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 4;
        break;
    case 4:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 5;
        return;
    case 5:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 6;
        else if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 849) {
            EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
            work->state = 6;
        }
        if (GetCurrentAuthorId().name == TGBT_AUTHORS[0].name) {
            const std::int64_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
            if (frame == 190)
                snapSetDesc(1);
            else if (frame == 370)
                snapSetDesc(0);
        }
        break;
    case 6:
        SetupControlSpeakAndUI(tgbtChoices, 600, -1, 2);
        FinishLoading();
        work->state = 7;
        return;
    case 7:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            if (work->retries)
                EV_SetAuthorActiveTo(1);
            const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
            if (choice == 231)
                QueueTgbtAuthor(TGBT_AUTHORS[2]);
            else if (choice == 232)
                QueueTgbtAuthor(TGBT_AUTHORS[3]);
            else
                QueueTgbtAuthor(TGBT_AUTHORS[1]);
            work->state = 8;
        }
        return;
    case 8:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 9;
        return;
    case 9:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (work->retries)
                EV_SetAuthorActiveTo(1);
            QueueTgbtAuthor(TGBT_AUTHORS[4]);
            work->state = 10;
        }
        return;
    case 10:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 11;
        return;
    case 11:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            TgbtDormantBoundary();
            spmFadeOut();
            work->state = 12;
            work->frames = 30;
        }
        return;
    case 12:
        EV_LoadAudio("0280A000", 1);
        AT_SetLocalBaseChara(TGBT_PLAYER, TGBT_BASE);
        AT_SetLocalBaseChara(TGBT_FIRST_FIGHTER, TGBT_BASE);
        AT_SetLocalBaseChara(TGBT_SECOND_FIGHTER, TGBT_BASE);
        AT_SetLocalBaseChara(TGBT_THIRD_FIGHTER, TGBT_BASE);
        EVT_AT_SetPos(TGBT_PLAYER, TGBT_BASE, TGBT_PLAYER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_PLAYER, TGBT_BASE, 230, 0);
        EVT_AT_SetPos(TGBT_FIRST_FIGHTER, TGBT_BASE, TGBT_FIRST_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_FIRST_FIGHTER, TGBT_BASE, 75, 0);
        EVT_AT_SetPos(TGBT_SECOND_FIGHTER, TGBT_BASE, TGBT_SECOND_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_SECOND_FIGHTER, TGBT_BASE, 339, 0);
        EVT_AT_SetPos(TGBT_THIRD_FIGHTER, TGBT_BASE, TGBT_THIRD_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_THIRD_FIGHTER, TGBT_BASE, 40, 0);
        EVT_AT_SetMot(TGBT_PLAYER, 0x132, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_FIRST_FIGHTER, 0xe30, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_SECOND_FIGHTER, 0xe34, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_THIRD_FIGHTER, 0xe38, 0, 1, 0x40000000U, 0);
        UnparentAndSetExistingPosRot(TGBT_PLAYER);
        UnparentAndSetExistingPosRot(TGBT_FIRST_FIGHTER);
        UnparentAndSetExistingPosRot(TGBT_SECOND_FIGHTER);
        UnparentAndSetExistingPosRot(TGBT_THIRD_FIGHTER);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        EVT_AT_SetCam(100, TGBT_PLAYER, TGBT_PLAYER, 0, 0, 0);
        camCtrlDisableFilmBars();
        work->state = 13;
        return;
    case 13:
        if (!camCtrlCheckIsEventCam()) {
            flddSetPointFlagOnGrid(TGBT_MAP_PART, 0);
            flddSetPointFlagOnGrid(TGBT_FIELD_PART, 0);
            EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
            vfmQueueFightStart(4, 99, 1, 0, -1);
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x80000000U}, 1);
            EVT_AT_SetAngY(TGBT_AUTHOR_CHARACTERS[0], TGBT_INVALID, 0, 0);
            work->state = 14;
        }
        return;
    case 14:
        if (!IsFightActive()) {
            if (GetBtlStatus(FindCharacterByScenarioName(TGBT_PLAYER)) == 3)
                work->state = 17;
            else {
                ++work->retries;
                if (work->retries > 15)
                    work->retries = 15;
                work->state = 15;
            }
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            work->frames = 30;
        }
        return;
    case 15:
        Queue_ST_LoadTitleFadeIn(TGBT_RETRY_TITLE);
        work->state = 16;
        work->frames = 60;
        return;
    case 16:
        ST_LoadTitleFade();
        work->state = 0;
        return;
    case 17:
        flddSetPointFlagOnGrid(TGBT_MAP_PART, 1);
        flddSetPointFlagOnGrid(TGBT_FIELD_PART, 1);
        QueueTgbtAuthor(TGBT_AUTHORS[5]);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 18;
        return;
    case 18:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 19;
        return;
    case 19:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 20;
        return;
    case 20:
        Loop3DSoundEffect(0x5bb);
        EVT_AT_SetSound(0x5bc, 30);
        work->frames = 45;
        work->state = 21;
        return;
    case 21:
        spmFadeOut();
        CleanupTgbtEvent(work);
        return;
    default:
        return;
    }
}
const ScriptAreaDefinition tgbtScriptDefinition{TGBT_AREA, 0, TgbtInit, TgbtMain};
} // namespace shenmue::s2
