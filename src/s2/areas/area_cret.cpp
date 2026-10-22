#include "s2/areas/area_cret.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
std::int32_t cretMode;
std::int32_t cretState;
std::int32_t cretStoryStep;
std::int32_t cretDelay;
std::int32_t cretAct;
shen_name cretCharacter;
shen_name cretScene;
std::int32_t cretEntry;
ConversationBuffer *cretDialogue;
} // namespace

// @verified - p
void AreaCRETInit() {
    cretMode = 0;
    cretState = 0;
    cretStoryStep = 0;
    cretAct = 0;
    cretCharacter = {};
    cretScene = {};
    cretEntry = 0;
    cretDialogue = nullptr;
    cretDormantScratch0 = {};
    cretDormantScratch1 = {};
}

void CRETRetryCallback(void *) {
    if (cretState) {
        if (cretState != 1)
            return;
    } else {
        ReleaseNpcScriptRouteOwner();
        if ((static_cast<std::uint32_t>(cretMode - 2) & 0xfffffffdU) == 0) {
            DateTime current;
            GetCurrentDateTime(current);
        }
        if (cvsHasValidConverseData(cretDialogue))
            return;
        spmFadeOut();
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ++cretState;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (cretStoryStep)
        EV_stepSetFlags(0, cretStoryStep);
    EV_scenQueueLoad(cretAct, cretScene, cretEntry);
    DateTime dateTime;
    std::uint32_t seconds;
    if ((static_cast<std::uint32_t>(cretMode) & 0xfffffff5U) || cretMode == 2) {
        GetCurrentDateTime(dateTime);
        seconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(dateTime));
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 48, seconds >> 24);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 49, (seconds >> 16) & 0xffU);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 32, (seconds >> 8) & 0xffU);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 33, seconds & 0xffU);
    } else {
        seconds = EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 48) << 24;
        seconds += EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 49) << 16;
        seconds += EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 32) << 8;
        seconds = seconds + EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 33) - 300;
        IncrementDateTimeBySeconds(seconds, dateTime);
        SetCurrentDateTimeTo(dateTime);
    }
    const std::int32_t retries = EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 34);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 34, retries + 1);
    InitiateCleanupForSceneChara(EVENT_CHARACTER_INVALID);
    HLib::DestroyCurrentTask();
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
}

// @verified - p
void CRETResumeChaseCallback(void *) {
    switch (cretState) {
    case 10:
        EV_CheckWarmStartNpcRoute(cretCharacter);
        EV_ForceLoadNpc(cretCharacter);
        ++cretState;
        break;
    case 11:
        if (EV_ForceLoadNpc(cretCharacter)) {
            Vector3 position;
            EV_GetCharaPosition(cretCharacter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            if (cretMode == 1 || cretMode == 11) {
                position = {102.22f, 89.279999f, 151.2f};
                const Vector3i rotation{0, 0x8000, 0};
                EV_AT_SetPos(CRET_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
                EV_AT_SetAngY(CRET_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
            }
            cretDelay = 30;
            ++cretState;
        }
        break;
    case 12:
        if ((static_cast<std::uint32_t>(cretMode - 1) & 0xfffffff5U) || cretMode == 9) {
            cretState = 13;
        } else {
            if (cretDelay <= 0) {
                SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
                ++cretState;
            }
            --cretDelay;
        }
        break;
    case 13:
        if ((static_cast<std::uint32_t>(cretMode - 1) & 0xfffffff5U) || cretMode == 9 || !filmCheckFadeTrackColour()) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1267, 0);
            InitiateCleanupForSceneChara(cretCharacter);
            SetScenarioFollowCharacter(cretCharacter);
            EV_npcLockUnlock(cretCharacter);
            HLib::DestroyCurrentTask();
            EV_NPC_NewLoadSw(1);
            SetPlayerControlEnabled(1);
        }
        break;
    default:
        ++cretState;
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1267)) {
            cretState = 10;
            EV_CheckWarmStartNpcRoute(cretCharacter);
            EV_ForceLoadNpc(cretCharacter);
            ++cretState;
        }
        break;
    }
}

void CRETDirectRetryCallback(void *) {
    if (!cretState) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        ++cretState;
        return;
    }
    if (cretState != 1 || filmCheckFadeTrackColour())
        return;
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    const std::uint32_t seconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(dateTime));
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 48, seconds >> 24);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 49, (seconds >> 16) & 0xffU);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 32, (seconds >> 8) & 0xffU);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 33, seconds & 0xffU);
    EV_scenQueueLoad(cretAct, cretScene, cretEntry);
    const std::int32_t retries = EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 34);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 34, retries + 1);
    HLib::DestroyCurrentTask();
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
}

// @verified - p
void AreaCRETMain(std::uintptr_t argument) {
    const std::int32_t mode = static_cast<std::int32_t>(argument);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("CRET"), 0);
    cretMode = mode;
    cretState = 0;
    cretStoryStep = 0;
    cretDelay = 0;
    SetPlayerControlEnabled(0);
    HLib::TaskCallback callback = CRETRetryCallback;
    switch (mode) {
    case 0:
        cretCharacter = CRET_WONG;
        cretDialogue = EV_PlayDialogue("0179A067");
        cretAct = 1;
        cretScene = CRET_ABERDEEN;
        cretEntry = 25;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 9))
            cretEntry = 91;
        else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 12))
            cretEntry = 92;
        else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 15))
            cretEntry = 93;
        else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 22))
            cretEntry = 94;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 9, 0);
        break;
    case 1:
    case 3:
    case 11:
    case 12:
        cretCharacter = CRET_REN;
        callback = CRETResumeChaseCallback;
        break;
    case 2:
    case 4:
        cretCharacter = CRET_REN;
        cretDialogue = EV_PlayDialogue("0327A067");
        cretAct = 3;
        cretScene = CRET_KOWLOON;
        cretEntry = mode == 2 ? 97 : 96;
        break;
    case 5:
        cretCharacter = CRET_TIGER;
        callback = CRETResumeChaseCallback;
        break;
    case 6:
        cretCharacter = CRET_TIGER;
        cretDialogue = EV_PlayDialogue("0180A067");
        cretStoryStep = 884;
        cretAct = 2;
        cretScene = CRET_WANCHAI;
        cretEntry = 89;
        break;
    case 7:
        cretCharacter = CRET_LARRY;
        callback = CRETResumeChaseCallback;
        break;
    case 8:
        cretCharacter = CRET_LARRY;
        cretDialogue = EV_PlayDialogue("0180A067");
        cretAct = 2;
        cretScene = CRET_ABERDEEN;
        cretEntry = 95;
        break;
    case 9:
        cretCharacter = CRET_SAM;
        callback = CRETResumeChaseCallback;
        break;
    case 10:
        cretCharacter = CRET_SAM;
        cretDialogue = EV_PlayDialogue("0180A067");
        cretAct = 2;
        cretScene = CRET_ABERDEEN;
        cretEntry = 0;
        break;
    case 13:
    case 14:
        cretEntry = mode == 13 ? 97 : 96;
        cretScene = CRET_KOWLOON;
        cretAct = 3;
        callback = CRETDirectRetryCallback;
        break;
    default:
        EV_NPC_NewLoadSw(1);
        SetPlayerControlEnabled(1);
        return;
    }
    HLib::EnqueueTaskWithoutParameter(callback, 3, 0x0b, static_cast<std::int32_t>(CRET_TASK.name));
}

const ScriptAreaDefinition areaCRETScriptDefinition{.areaId = AREA_CRET, .field04 = 0, .init = AreaCRETInit, .main = AreaCRETMain};

} // namespace shenmue::s2::areas
