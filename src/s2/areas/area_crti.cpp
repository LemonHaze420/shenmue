#include "s2/areas/area_crti.hpp"
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
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"

namespace shenmue::s2::areas {
namespace {
std::int32_t crtiMode;
std::int32_t crtiState;
std::int32_t crtiStoryStep;
std::int32_t crtiAct;
shen_name crtiCharacter;
shen_name crtiScene;
std::int32_t crtiEntry;
ConversationBuffer *crtiDialogue;
} // namespace

// @verified - p
void AreaCRTIInit() {
    crtiMode = 0;
    crtiState = 0;
    crtiStoryStep = 0;
    crtiAct = 0;
    crtiCharacter = {};
    crtiScene = {};
    crtiEntry = 0;
    crtiDialogue = nullptr;
    crtiDormantScratch0 = {};
    crtiDormantScratch1 = {};
}

void CRTIRetryCallback(void *) {
    if (crtiState) {
        if (crtiState != 1)
            return;
    } else {
        if (cvsHasValidConverseData(crtiDialogue))
            return;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ++crtiState;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (crtiStoryStep)
        EV_stepSetFlags(0, crtiStoryStep);
    EV_scenQueueLoad(crtiAct, crtiScene, crtiEntry);
    DateTime dateTime;
    std::uint32_t seconds;
    if ((static_cast<std::uint32_t>(crtiMode) & 0xfffffff5U) || crtiMode == 2) {
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
    HLib::DestroyCurrentTask();
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
}

// @verified - p
void CRTIResumeChaseCallback(void *) {
    if (!crtiState) {
        crtiState = 2;
        return;
    }
    if (crtiState == 2) {
        EV_CheckWarmStartNpcRoute(crtiCharacter);
        EV_ForceLoadNpc(crtiCharacter);
        ++crtiState;
    } else if (crtiState == 3) {
        if (!EV_ForceLoadNpc(crtiCharacter))
            return;
        if (crtiMode == 1) {
            const Vector3 position{101.9f, 89.279999f, 151.2f};
            const Vector3i rotation{0, 0x8000, 0};
            EV_AT_SetPos(CRTI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(CRTI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        }
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        ++crtiState;
    } else if (crtiState == 4 && !filmCheckFadeTrackColour()) {
        if ((static_cast<std::uint32_t>(crtiMode - 1) & 0xfffffffdU) == 0)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1267, 0);
        InitiateCleanupForSceneChara(crtiCharacter);
        SetScenarioFollowCharacter(crtiCharacter);
        EV_npcLockUnlock(crtiCharacter);
        HLib::DestroyCurrentTask();
        EV_NPC_NewLoadSw(1);
        SetPlayerControlEnabled(1);
    }
}

// @verified - p
void AreaCRTIMain(std::uintptr_t argument) {
    const std::int32_t mode = static_cast<std::int32_t>(argument);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("CRTI"), 0);
    crtiMode = mode;
    crtiState = 0;
    crtiStoryStep = 0;
    SetPlayerControlEnabled(0);
    HLib::TaskCallback callback = CRTIRetryCallback;
    switch (mode) {
    case 0:
        crtiCharacter = CRTI_WONG;
        crtiDialogue = EV_PlayDialogue("0011A011");
        crtiAct = 1;
        crtiScene = CRTI_ABERDEEN;
        crtiEntry = 25;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 9))
            crtiEntry = 91;
        else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 12))
            crtiEntry = 92;
        else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 15))
            crtiEntry = 93;
        else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 22))
            crtiEntry = 94;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 9, 0);
        break;
    case 1:
    case 3:
        crtiCharacter = CRTI_REN;
        callback = CRTIResumeChaseCallback;
        break;
    case 2:
    case 4:
        crtiCharacter = CRTI_REN;
        crtiDialogue = EV_PlayDialogue("0011A011");
        crtiAct = 3;
        crtiScene = CRTI_KOWLOON;
        crtiEntry = mode == 2 ? 97 : 96;
        break;
    case 5:
        crtiCharacter = CRTI_TIGER;
        callback = CRTIResumeChaseCallback;
        break;
    case 6:
        crtiCharacter = CRTI_TIGER;
        crtiDialogue = EV_PlayDialogue("0011A011");
        crtiStoryStep = 884;
        crtiAct = 2;
        crtiScene = CRTI_WANCHAI;
        crtiEntry = 89;
        break;
    case 7:
        crtiCharacter = CRTI_LARRY;
        callback = CRTIResumeChaseCallback;
        break;
    case 8:
        crtiCharacter = CRTI_LARRY;
        crtiDialogue = EV_PlayDialogue("0011A011");
        crtiAct = 2;
        crtiScene = CRTI_ABERDEEN;
        crtiEntry = 95;
        break;
    case 9:
        crtiCharacter = CRTI_SAM;
        callback = CRTIResumeChaseCallback;
        break;
    case 10:
        crtiCharacter = CRTI_SAM;
        crtiDialogue = EV_PlayDialogue("0011A011");
        crtiAct = 2;
        crtiScene = CRTI_ABERDEEN;
        crtiEntry = 0;
        break;
    default:
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("CRTI"), 0);
        EV_NPC_NewLoadSw(1);
        SetPlayerControlEnabled(1);
        return;
    }
    HLib::EnqueueTaskWithoutParameter(callback, 3, 0x0b, static_cast<std::int32_t>(CRTI_TASK.name));
}

const ScriptAreaDefinition areaCRTIScriptDefinition{.areaId = AREA_CRTI, .field04 = 0, .init = AreaCRTIInit, .main = AreaCRTIMain};

} // namespace shenmue::s2::areas
