#include "s2/areas/area_rati.hpp"
#include "s2/sound_program.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/load_title.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2 {

HLib::Task *ratiTask{};
std::int32_t ratiMode{};
std::int32_t ratiQteFrames{};
DateTime ratiStartDate{};
std::int32_t ratiTitle{};
std::int32_t ratiQteStarted{};
const shen_name *ratiSequence{};

// @verified - p
void RatiInit() {
    ratiTask = nullptr;
    ratiMode = 0;
    ratiQteFrames = 0;
    ratiStartDate = {};
    ratiTitle = 0;
    ratiQteStarted = 0;
    ratiSequence = nullptr;
}

// @verified - p
void RatiMain(std::uintptr_t argument) {
    ratiMode = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(argument));
    ratiQteFrames = 16;
    ratiTitle = 0;
    ratiQteStarted = 0;
    ratiSequence = RATI_SEQUENCES + 5LL * ratiMode;
    SetPlayerControlEnabled(0);
    (void)mtwkRequestMotion(scenFindMotionwork(RATI_RYO), 0xf003, MotionRegion::LowerBody);
    GetCurrentDateTime(ratiStartDate);
    ratiTask = HLib::EnqueueTaskWithoutParameter(RatiPlayImprisonmentCallback, 6, 0x0b, static_cast<std::int32_t>(RATI_TASK.name));
    (void)HLib::EnqueueTaskWithoutParameter(RatiTitleCallback, 6, 0x0b, static_cast<std::int32_t>(RATI_TITLE_TASK.name));
    (void)Queue_EVT_UTL_LoadNPCChara_3(RATI_CHARACTERS, ratiTask);
}

// @verified - p
void PrepareRatiCharacters() {
    EV_EnableCharacterDisplay(RATI_RYO);
    EV_SetCharaControllerFlags(RATI_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(RATI_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(RATI_RYO, false);
    EV_SetRefBoardParamsFor(RATI_RYO, 1);
    (void)SetScenarioReflectorBoardValues(RATI_RYO, &RATI_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(RATI_REN, &RATI_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(RATI_CHR, &RATI_REFLECTOR);
    (void)SetScenarioCharacterFlag40(RATI_RYO, 0);
    (void)SetScenarioCharacterFlag40(RATI_REN, 0);
    (void)EV_cuffsAttach(RATI_RYO, 1, RATI_REN, 0, 0.3f);
}

// @verified - p
void RatiPlayImprisonmentCallback(void *) {
    ratiQteStarted = 0;
    PrepareRatiCharacters();
    (void)EV_FreeAuthorPak(1, "0210");
    (void)EV_LoadCutsceneFromCurrentScene("0210A000");
    HLib::SetCurrentTaskTo(RatiPlayAuthorCallback);
    if (ratiSequence->name == RATI_AUT2.name) {
        DateTime date = ratiStartDate;
        date.day = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(date.day + 1));
        date.hour = 12;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        ratiTitle = static_cast<std::int32_t>(RATI_TITLE_ONE_DAY.name);
    } else {
        ratiTitle = static_cast<std::int32_t>(RATI_TITLE_OPENING.name);
    }
    HLib::SetupTaskDelay(90);
    HLib::SetupTaskDelay(90);
}

// @verified - p
void RatiTitleCallback(void *) {
    if (ratiTitle == -1)
        ST_LoadTitleFade();
    else if (ratiTitle != 0)
        Queue_ST_LoadTitleFadeIn(shen_name{static_cast<std::uint32_t>(ratiTitle)});
    if (ratiTitle != 0)
        ratiTitle = 0;
    if (!ratiTask)
        (void)HLib::DestroyCurrentTask();
}

// @verified - p
void RatiPlayAuthorCallback(void *) {
    ratiTitle = -1;
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ratiSequence->name));
    if (ratiMode != 0) {
        EV_SetAuthorActiveTo(1);
        HLib::SetCurrentTaskTo(RatiWaitAuthorCallback);
    } else if (ratiSequence->name == RATI_AUT4.name) {
        (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        HLib::SetCurrentTaskTo(RatiWaitQteCallback);
    } else {
        HLib::SetCurrentTaskTo(RatiWaitAuthorCallback);
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}

// @verified - p
void RatiWaitQteCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (!ratiQteStarted) {
        (void)qteBattleUtil(BattleQteMode::SetWait, 1, 6, ratiQteFrames, 0);
        (void)qteBattleUtil(BattleQteMode::SetFirstCommands, 1024, 256, 0, 0);
        (void)qteBattleUtil(BattleQteMode::Start, 0, 0, 0, 0);
        if (ratiQteFrames < 24)
            ratiQteFrames += 4;
        ratiQteStarted = 1;
        return;
    }
    const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult, 0, 0, 0, 0);
    if (result == 1) {
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(RATI_AUT5.name));
        HLib::SetCurrentTaskTo(RatiWaitAuthorCallback);
    } else if (result == -1) {
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(RATI_AUT6.name));
        HLib::SetCurrentTaskTo(RatiWaitFailureAuthorCallback);
    }
    (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if (ratiMode != 0)
        EV_SetAuthorActiveTo(1);
}

// @verified - p
void RatiWaitAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (ratiSequence->name != RATI_AUT4.name)
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    ++ratiSequence;
    if (ratiSequence->name == RATI_INVALID.name) {
        (void)EndRatiEvent();
    } else if (ratiSequence->name == RATI_AUT5.name) {
        if (ratiMode != 0)
            EV_SetAuthorActiveTo(1);
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ratiSequence->name));
    } else {
        HLib::SetCurrentTaskTo(RatiPlayAuthorCallback);
        DateTime date = ratiStartDate;
        date.hour = 12;
        date.minute = 0;
        if (ratiSequence->name == RATI_AUT2.name) {
            date.day = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(date.day + 1));
            SetCurrentDateTimeTo(date);
            ratiTitle = static_cast<std::int32_t>(RATI_TITLE_ONE_DAY.name);
        } else {
            for (std::int32_t day = 0; day < 2; ++day) {
                date.day = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(date.day + 1));
                SetCurrentDateTimeTo(date);
            }
            ratiTitle = static_cast<std::int32_t>(RATI_TITLE_TWO_DAYS.name);
        }
        HLib::SetupTaskDelay(90);
    }
}

// @verified - p
void RatiWaitFailureAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    ratiQteStarted = 0;
    ratiSequence = RATI_SEQUENCES + 1;
    HLib::SetCurrentTaskTo(RatiPlayAuthorCallback);
    DateTime date = ratiStartDate;
    date.hour = 12;
    date.minute = 0;
    for (std::int32_t day = 0; day < 2; ++day) {
        date.day = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(date.day + 1));
        SetCurrentDateTimeTo(date);
    }
    ratiTitle = static_cast<std::int32_t>(RATI_TITLE_TWO_DAYS.name);
    HLib::SetupTaskDelay(90);
}

// @verified - p
HLib::Task *EndRatiEvent() {
    for (const shen_name *character = RATI_CHARACTERS + 1; character->name != RATI_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        (void)EV_npcLockUnlock(*character);
        (void)EV_npcFree(*character);
    }
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("RATI"), 0);
    (void)EV_FreeAuthorPak(0, "0210");
    EV_stepSetFlags(0, 0x138);
    EV_CleanupMapControlTask();
    HLib::Task *const result = HLib::HLKillOtherHierarchyTask(ratiTask);
    ratiTask = nullptr;
    return result;
}

const ScriptAreaDefinition ratiScriptDefinition{.areaId = AREA_RATI, .field04 = 0, .init = RatiInit, .main = RatiMain};

} // namespace shenmue::s2
