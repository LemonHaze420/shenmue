#include "s2/areas/area_frbt.hpp"
#include "s2/areas/area_frbt_messages.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>

namespace shenmue::s2::areas {
std::uint32_t frbtVariant;
std::int32_t frbtResponse, frbtVoiceCount;
char frbtVoicePrefixes[64], frbtVoiceSequence[16][12];
namespace {
shen_name frbtCharacters[7]{FRBT_RYO, FRBT_INVALID};
void FrbtInitializeCallback(void *);
void FrbtAutoCallback(void *);
void FrbtStartBattleCallback(void *);
void FrbtWaitBattleCallback(void *);
void FrbtWonCallback(void *);
void FrbtLoseCallback(void *);
void FrbtEndCallback(void *);

void FrbtChooseResponse() {
    if (frbtVariant <= 6) {
        if (EV_GetShenmueFlag(0x74))
            frbtResponse = 0;
        else if (EV_GetShenmueFlag(0x75))
            frbtResponse = 3;
        else
            frbtResponse = EV_GetShenmueFlag(0x30c) ? 1 : 6;
    } else if (frbtVariant - 7 <= 7)
        frbtResponse = EV_GetShenmueFlag(0x74) ? 4 : 5;
    else
        frbtResponse = 7;
}
void FrbtCopyEnemies(FrbtWork &work) {
    std::int32_t count = 0;
    while (FRBT_ENEMIES[frbtVariant][count].name != FRBT_INVALID.name) {
        work.enemies[count] = FRBT_ENEMIES[frbtVariant][count];
        ++count;
    }
    work.enemyCount = count;
    for (std::int32_t i = 0; i < work.enemyCount; ++i) {
        const unsigned char *name = reinterpret_cast<const unsigned char *>(&work.enemies[i]);
        EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 626, "ID : %c%c%c%c", name[0], name[1], name[2], name[3]);
    }
}
void FrbtStartCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234) || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
        return;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(FrbtInitializeCallback, 5, 0x0b, sizeof(FrbtWork), FRBT_TASK);
    FrbtWork *work = static_cast<FrbtWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->motion = -1;
    work->outcome = 1;
    work->phase = 0;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
    FrbtChooseResponse();
    FrbtCopyEnemies(*work);
    EV_SetNewSceneIndex();
    work->characters = Queue_EVT_AT_ReadChr(task, frbtCharacters);
    HLib::DestroyCurrentTask();
}
void FrbtInitializeResources() {
    CreateEventCharacter(CharacterCreationHighModel, nullptr, FRBT_BASE);
    SetLowQualityHands(FindCharacterByScenarioName(FRBT_RYO));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("FRBT"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("FRBT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("FRBT"), 0);
    EV_FreeAuthorPak(1, "0052");
    EV_FreeAuthorPak(1, "S2BV");
    const char *cutscene = g_currentMap.name == FRBT_AB00.name ? "0052E010" : "0073E000";
    EV_LoadCutsceneFromCurrentScene(cutscene);
    EV_LoadAudio(cutscene, 0);
    std::strcpy(frbtVoicePrefixes, "0052A");
    std::strcpy(frbtVoicePrefixes + 16, FRBT_OUTCOMES[frbtVariant].voicePrefix);
    EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 603, "%s %d\n", FRBT_OUTCOMES[frbtVariant].voicePrefix, frbtVariant);
    flddSetPointFlagOnGrid(FRBT_BATTLE_GRID, 1);
    flddSetPointFlagOnGrid(FRBT_DEFAULT_GRID, 0);
    EV_spmBeginSampleExecute();
}
void FrbtSetupEnemies(FrbtWork &work) {
    for (std::int32_t i = 0; i < work.enemyCount; ++i) {
        std::int32_t definition = 0;
        while (FRBT_ENEMY_DEFINITIONS[definition].character.name && FRBT_ENEMY_DEFINITIONS[definition].character.name != work.enemies[i].name)
            ++definition;
        shenInitFightData(FindCharacterByScenarioName(work.enemies[i]), nullptr, 1, 1, i == 0 && frbtVariant <= 6 ? 120 : 60, 2);
        vfUpdateFlagsFor(FindCharacterByScenarioName(work.enemies[i]), 0, 0x400000, 128);
        vfUpdateFlagsFor(FindCharacterByScenarioName(work.enemies[i]), 0, 0x200000, 2048);
        const unsigned char *name = reinterpret_cast<const unsigned char *>(&work.enemies[i]);
        EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 850, "ID : %c%c%c%c", name[0], name[1], name[2], name[3]);
        if (!FRBT_ENEMY_DEFINITIONS[definition].character.name)
            EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 851, "  Is Not Enmdata\n");
    }
}
void FrbtSetupBattle(FrbtWork &work) {
    work.motion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    shenStartFGST(10, 10, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80}, 1);
    shenInitFightData(FindCharacterByScenarioName(FRBT_RYO), nullptr, 0, 0, 0, 0);
    FrbtSetupEnemies(work);
}
void FrbtInitializeCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    FrbtInitializeResources();
    FrbtSetupBattle(work);
    for (std::int32_t i = 0; i < work.enemyCount; ++i) {
        EV_SetCharaControllerFlags(work.enemies[i], ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(work.enemies[i], UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_EnableCharacterDisplay(work.enemies[i]);
        EV_SetCharacterAlphaEnabled(work.enemies[i], false);
        EV_SetCharaControllerFlags(work.enemies[i], UpdateHeight, NpcCharacterControllerState::Disabled);
    }
    if (frbtVariant - 7 <= 7)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (frbtVariant <= 6)
        for (std::int32_t i = 0; i < work.enemyCount; ++i) {
            const FrbtPlacement &placement = FRBT_POSITIONS[frbtVariant * 5 + static_cast<std::uint32_t>(i)];
            EventSetPosition position{.cleanup = 0, .character = work.enemies[i], .parent = FRBT_INVALID, .position = placement.position, .delay = 0};
            EVT_AT_SetPosRot(&position);
            EventSetAngle angle{.cleanup = 0, .character = work.enemies[i], .parent = FRBT_INVALID, .degrees = placement.yaw, .delay = 0};
            EVT_AT_SetAngY_Callback(&angle);
            EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 671, "%3.2f  %3.2f %3.2f\n", placement.position.x, placement.position.y, placement.position.z);
        }
    HLib::SetCurrentTaskTo(FrbtAutoCallback);
}
void FrbtMoneyHint(FrbtWork &work) {
    EVT_AT_SetMot(FRBT_RYO, 0xf003, 0, 1, 0x40000000U, -1);
    const std::uint32_t delay = QueueEventMessageSequence(FRBT_MONEY_HINT_MESSAGES);
    work.outcome = 2;
    HLib::SetCurrentTaskTo(FrbtStopCallback);
    HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
}
void FrbtAutoCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    if (frbtVariant <= 11 && !EV_GetShenmueFlag(0x207) && GetPlayerStat(PlayerStatsId::Cash) >= 500 && EV_GetShenmueFlag(0x1e6)) {
        FrbtMoneyHint(work);
        return;
    }
    std::int32_t delay;
    if (frbtVariant - 7 > 7) {
        EVT_AT_SetMot(FRBT_RYO, 0xf003, 0, 1, 0x40000000U, -1);
        delay = static_cast<std::int32_t>(QueueEventMessageSequence(FRBT_BEGIN_MESSAGES[frbtResponse]));
    } else
        delay = FrbtStartAuthor(work);
    HLib::SetCurrentTaskTo(FrbtStartBattleCallback);
    HLib::SetupTaskDelay(delay);
}
void FrbtStartBattleCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    if (frbtVariant <= 6)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    vfCopyPosFromEntity(1, FindCharacterByScenarioName(FRBT_RYO), 0, 0, 5.0f);
    vfmQueueFightStart(static_cast<std::uint8_t>(work.enemyCount + 1), 2, 1, 1, -1);
    HLib::SetCurrentTaskTo(FrbtWaitBattleCallback);
}
void FrbtWaitBattleCallback(void *data) {
    if (IsFightActive())
        return;
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    if (GetBtlStatus(FindCharacterByScenarioName(FRBT_RYO)) == 3) {
        if (frbtResponse == 3)
            HLib::SetCurrentTaskTo(FrbtMapGetCallback);
        else if (frbtResponse <= 1 || frbtResponse == 6)
            HLib::SetCurrentTaskTo(FrbtVictoryAuthorCallback);
        else
            HLib::SetCurrentTaskTo(FrbtWonCallback);
        SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(90);
    } else
        HLib::SetCurrentTaskTo(FrbtLoseCallback);
    for (std::int32_t i = 0; i < work.enemyCount; ++i)
        FrbtResetEnemyMotion(work.enemies[i]);
}
void FrbtWonCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    work.outcome = 1;
    const std::int32_t delay = static_cast<std::uint32_t>(frbtResponse - 4) <= 1 ? FrbtRunAway(work) : FrbtWinMessage(work);
    HLib::SetCurrentTaskTo(FrbtStopCallback);
    HLib::SetupTaskDelay(delay);
}
void FrbtLoseCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    std::int32_t delay = 0;
    if (!work.phase) {
        SetScreenFilmFade(90, 0, 0, 0, 0, 255, 0, 0, 0);
        if (frbtVariant - 7 > 7)
            FrbtPlayPrefixVoice(1, "024", 0);
        else
            EVT_AT_SetVoice(FRBT_LOSE_VOICES[frbtVariant - 7], 1, 0);
        ++work.phase;
        HLib::SetupTaskDelay(150);
        return;
    }
    if (work.phase != 1)
        return;
    for (std::int32_t i = 0; i < work.enemyCount; ++i)
        EV_DisableCharacterDisplay(work.enemies[i]);
    if (FRBT_OUTCOMES[frbtVariant].advanceTime) {
        DateTime time;
        GetCurrentDateTime(time);
        time.hour = static_cast<std::int8_t>(time.hour + 2);
        SetCurrentDateTimeTo(time);
        FrbtPositionBase();
        EVT_AT_SetCam(3000, FRBT_BASE, FRBT_BASE, 0, 0, 0);
        EventSetPosition position{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .position = {}, .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .degrees = 0, .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
        EventSetMotion motion{.cleanup = 0, .character = FRBT_RYO, .animation = 4935, .frame = 300, .speed = 1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(4935);
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), AREA_EVENT_TASK);
        RegisterEventTask(task);
        EventFilmControl *film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
        *film = {.cleanup = 1,
            .frames = 30,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 30};
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 10,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 190});
        delay = 200;
    }
    work.outcome = 0;
    work.phase = 0;
    HLib::SetCurrentTaskTo(FrbtStopCallback);
    if (delay)
        HLib::SetupTaskDelay(delay);
}
void FrbtApplyWonFlags(FrbtWork &work) {
    if (FRBT_OUTCOMES[frbtVariant].won != -1)
        EV_stepSetFlags(static_cast<std::uint8_t>(FRBT_OUTCOMES[frbtVariant].won), FRBT_OUTCOMES[frbtVariant].flag);
    if (frbtVariant <= 6) {
        const std::int32_t flag = EV_GetShenmueFlag(0x207) ? 0x207 : EV_GetShenmueFlag(0x74) ? 0x74 : EV_GetShenmueFlag(0x75) ? 0x75 : 0x30c;
        EV_stepSetFlags(0, flag);
        if (frbtResponse == 1)
            SetScenarioFollowCharacter(work.enemies[0]);
    }
}
void FrbtEndCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    DeleteMotion(work.motion);
    SetHighModelXYZ(FindCharacterByScenarioName(FRBT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(FRBT_RYO), 0, 1, 0);
    SetCharaKill(FRBT_BASE, 1);
    EV_FreeAuthorPak(0, "0052");
    EV_FreeAuthorPak(0, "S2BV");
    if (work.outcome == 1)
        FrbtApplyWonFlags(work);
    else if (!work.outcome) {
        if (EV_GetShenmueFlag(0x87))
            EV_stepSetFlags(1, 0x87);
        if (FRBT_OUTCOMES[frbtVariant].lost != -1)
            EV_stepSetFlags(static_cast<std::uint8_t>(FRBT_OUTCOMES[frbtVariant].lost), FRBT_OUTCOMES[frbtVariant].flag);
    }
    ET_AT_KillWork(work.characters);
    SetPlayerControlEnabled(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    flddSetPointFlagOnGrid(FRBT_DEFAULT_GRID, 1);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("FRBT"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("FRBT"), 0);
    EV_NPC_NewLoadSw(1);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    scenFreeEvent("FRBT.EV3");
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
}
} // namespace
void FrbtStopCallback(void *) {
    spmFadeOut();
    HLib::SetCurrentTaskTo(FrbtEndCallback);
    HLib::SetupTaskDelay(30);
}
void InitializeFrbtEvent() {
    frbtVariant = 0;
    frbtResponse = 0;
    frbtVoiceCount = 0;
    std::memset(frbtVoicePrefixes, 0, sizeof(frbtVoicePrefixes));
    std::memset(frbtVoiceSequence, 0, sizeof(frbtVoiceSequence));
}
void RunFrbtEvent(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    SetScenarioFollowCharacter(FRBT_EMPTY);
    frbtVariant = static_cast<std::uint32_t>(argument);
    HLib::EnqueueTaskWithoutParameter(FrbtStartCallback, 5, 0x0b, static_cast<std::int32_t>(FRBT_WAIT_TASK.name));
    std::uint8_t count = 0;
    while (FRBT_ENEMIES[frbtVariant][count].name != FRBT_INVALID.name) {
        frbtCharacters[count + 1] = FRBT_ENEMIES[frbtVariant][count];
        ++count;
    }
    frbtCharacters[count + 1] = FRBT_INVALID;
}
const ScriptAreaDefinition frbtScriptDefinition{.areaId = FRBT_AREA, .field04 = 0, .init = InitializeFrbtEvent, .main = RunFrbtEvent};
} // namespace shenmue::s2::areas
