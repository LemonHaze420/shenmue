#include "s2/sleep_event.hpp"

#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

#include <cstring>

namespace shenmue::s2 {
SleepEventData sleepEventData{};
shen_name sleepCurrentAuthor{};

namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name sleepButton{SHEN_NAME(BTSP)};
void SleepSelectionCallback(void *);
void SleepPackageRetryCallback(void *);
void SleepPackageWaitCallback(void *);
void SleepSubtitleStartCallback(void *);
void SleepSubtitleWaitCallback(void *);
void SleepNoticeWaitCallback(void *);

void SleepIconCallback(void *) {
    if ((GetLoadingPhaseFlags() & 8U) != 0)
        return;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1022, 1);
    shenSetTimeSimulationTo(false);
    SetSleepActionIcon(static_cast<std::int16_t>(sleepEventData.icon == 1 ? 271 : sleepEventData.icon == 2 ? 15 : 266));
    HLib::SetCurrentTaskTo(SleepSelectionCallback);
}

void SleepSelectionCallback(void *) {
    const std::uint8_t result = GetSleepActionIconResult();
    if (result == 2) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1022, 0);
        HLib::SetCurrentTaskTo(sleepEventData.continuation);
        shenSetClockAndCompassRenderTo(0);
        shenSetTimeSimulationTo(true);
        if ((sleepEventData.flags & 2U) != 0) {
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(32);
        }
    } else if (result == 1) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1022, 0);
        HLib::SetCurrentTaskTo(sleepEventData.cancelled);
        shenSetClockAndCompassRenderTo(0);
        shenSetTimeSimulationTo(true);
    }
}

void SleepPackageRetryCallback(void *) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(sleepEventData.package)))
        HLib::SetCurrentTaskTo(SleepPackageWaitCallback);
}

void SleepPackageWaitCallback(void *) {
    if ((sleepEventData.pendingResources & 1U) != 0 && !EV_ControlPackageResources(PakCategory::Package, PakPackageOp::LoadAfs, 0, 0))
        sleepEventData.pendingResources &= ~1U;
    for (std::uint32_t index = 0; sleepEventData.actors[index].name; ++index) {
        const shen_name actor = sleepEventData.actors[index];
        const std::uint32_t mask = 2U << (index & 31U);
        if ((actor.name & 0xff000000U) == 0x5f000000U && (sleepEventData.pendingResources & mask) != 0 && EV_ForceLoadNpc(actor))
            sleepEventData.pendingResources &= ~mask;
    }
    if (sleepEventData.pendingResources != 0)
        return;
    for (const shen_name *actor = sleepEventData.actors; actor->name; ++actor)
        HideSleepActor(*actor);
    HideSleepActor(ryo);
    HLib::SetCurrentTaskTo(sleepEventData.continuation);
}

void SleepBadEndCallback(void *) {
    scenSetTaskFlags(8);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(0549), 1);
}

void SleepBadEndNoticeCallback(void *) {
    EV_FreeAuthorPak(1, "0549");
    char *const savedBgm = sleepEventData.package + 16;
    const char *const bgm = GetCurrentBgm();
    if (bgm)
        std::strcpy(savedBgm, bgm);
    else
        savedBgm[0] = 0;
    EV_LoadCutsceneFromCurrentScene("0549A000");
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(NBAD)));
    HLib::SetCurrentTaskTo(SleepNoticeWaitCallback);
}

void SleepNoticeWaitCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_LoadCutsceneFromCurrentScene(sleepEventData.package + 16);
    EV_FreeAuthorPak(0, "0549");
    HLib::SetCurrentTaskTo(SleepSubtitleStartCallback);
}

void SleepSubtitleStartCallback(void *) {
    switch (g_currentMap.name) {
    case SHEN_NAME(AR01):
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(ARK0));
        break;
    case SHEN_NAME(WSY0):
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(WSY1));
        break;
    case SHEN_NAME(WTA0):
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(WTSH));
        break;
    case SHEN_NAME(AB00):
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(ABB0));
        break;
    default:
        break;
    }
    Queue_ST_LoadTitleFadeIn(shen_name{});
    HLib::SetCurrentTaskTo(SleepSubtitleWaitCallback);
}

void SleepSubtitleWaitCallback(void *) {
    const std::int32_t frames = --sleepEventData.subtitleFrames;
    if (frames == 31)
        ST_LoadTitleFade();
    if (sleepEventData.subtitleFrames == 120)
        EV_spmBeginSampleExecute();
    if (!sleepEventData.subtitleFrames)
        HLib::SetCurrentTaskTo(sleepEventData.continuation);
}

void SleepSceneChangeCallback(void *) {
    if ((sleepEventData.flags & 4U) == 0)
        scenSetTaskFlags(6);
    EV_scenQueueLoad(g_actNumber, sleepEventData.nextScene, sleepEventData.nextEntry);
    HLib::DestroyCurrentTask();
}
} // namespace


void QueueSleepCallback(HLib::TaskCallback accepted, HLib::TaskCallback cancelled, std::int32_t expectedSchedule) {
    sleepEventData.continuation = accepted;
    sleepEventData.flags = 2;
    sleepEventData.cancelled = cancelled;
    DateTime date;
    GetCurrentDateTime(date);
    std::int32_t icon = 2;
    if (shenGetMinutesUntilNextFrameMinute()) {
        icon = 1;
        if (static_cast<std::uint32_t>(static_cast<std::int32_t>(date.hour) - 7) <= 12U)
            icon = 3;
    }
    if (expectedSchedule != shenGetSleepScheduleFlag())
        icon = 3;
    sleepEventData.icon = icon;
    shenSetClockAndCompassRenderTo(1);
    HLib::SetCurrentTaskTo(SleepIconCallback);
}


void QueueSleepPackageActors(const char *package, const shen_name *actors, HLib::TaskCallback continuation) {
    sleepEventData.continuation = continuation;
    sleepEventData.pendingResources = 0;
    if (package)
        std::strcpy(sleepEventData.package, package);
    else
        sleepEventData.package[0] = 0;
    shen_name *destination = sleepEventData.actors;
    if (actors)
        while (actors->name)
            *destination++ = *actors++;
    *destination = shen_name{};
    for (std::uint32_t index = 0; sleepEventData.actors[index].name; ++index) {
        const shen_name actor = sleepEventData.actors[index];
        if ((actor.name & 0xff000000U) == 0x5f000000U) {
            sleepEventData.pendingResources |= 2U << (index & 31U);
            EV_CheckWarmStartNpcRoute(actor);
        }
    }
    if (sleepEventData.package[0]) {
        sleepEventData.pendingResources |= 1U;
        HLib::SetCurrentTaskTo(SleepPackageRetryCallback);
    } else
        HLib::SetCurrentTaskTo(SleepPackageWaitCallback);
}


void FreeSleepPackageActors() {
    if (sleepEventData.package[0])
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(sleepEventData.package), 0);
    for (const shen_name *actor = sleepEventData.actors; actor->name; ++actor) {
        if ((actor->name & 0xff000000U) == 0x5f000000U) {
            EV_npcFree(*actor);
            EV_npcLockUnlock(*actor);
        }
    }
}


void HideSleepActor(shen_name actor) {
    if ((actor.name & 0xff000000U) == 0x5f000000U) {
        EV_SetCharacterAlphaEnabled(actor, false);
        EV_mtwkSetEnabled(FindMotionWork(FindCharacterByScenarioName(actor)), 0);
        SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(actor)), 71, 0, MotionRegion::LowerBody);
    }
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_AT_SetPos(actor, EventTransformFlags::AllAxes, Vector3{-1000, -1000, -1000});
}


void SleepAuthorWaitCallback(void *) {
    const std::int32_t endFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 31;
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == endFrame && (sleepEventData.flags & 2U))
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (GetAuthorFlags() && sleepCurrentAuthor.name == SHEN_NAME(SLPJ))
        spmFadeOut();
    for (const shen_name *actor = sleepEventData.actors; actor->name; ++actor)
        HideSleepActor(*actor);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if ((sleepEventData.flags & 0x100U) != 0)
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    if ((sleepEventData.flags & 0x40U) == 0)
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    HLib::SetCurrentTaskTo(sleepEventData.continuation);
}

std::int32_t GetBadEndingState() {
    DateTime date;
    GetCurrentDateTime(date);
    if (date.year < 87)
        return 0;
    if (date.month >= 7 && EV_GetShenmueFlag(0x3a1)) {
        EV_stepSetFlags(0, 0x3a1);
        return 1;
    }
    return date.year >= 87 && date.month >= 8 && EV_GetShenmueFlag(0x3a2) ? 2 : 0;
}


void SelectBadEndingState(HLib::TaskCallback continuation) {
    sleepEventData.continuation = continuation;
    sleepEventData.subtitleFrames = 200;
    shenUpdateCurrentDateTime();
    switch (GetBadEndingState()) {
    case 1:
        HLib::SetCurrentTaskTo(SleepBadEndNoticeCallback);
        HLib::SetupTaskDelay(60);
        break;
    case 2:
        HLib::SetCurrentTaskTo(SleepBadEndCallback);
        break;
    default:
        HLib::SetCurrentTaskTo(SleepSubtitleStartCallback);
        HLib::SetupTaskDelay(60);
        break;
    }
}

void SleepMemoWaitCallback(void *) {
    if (!IsItemSelectable())
        HLib::SetCurrentTaskTo(sleepEventData.continuation);
}


void QueueSleepSceneChange(shen_name scene, std::int32_t entry) {
    sleepEventData.nextScene = scene;
    sleepEventData.nextEntry = entry;
    sleepEventData.flags = 0;
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(SleepSceneChangeCallback, 4, 0xb, static_cast<std::int32_t>(SLEEP_EVENT_TASK.name));
    if ((sleepEventData.flags & 2U) != 0) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTimedTask(task, 31);
    }
}


std::uint32_t shenTrySleepTrigger(SleepTriggerBox *box) {
    std::uint32_t result = 0;
    DateTime date;
    GetCurrentDateTime(date);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(ryo, &rotation, EventTransformFlags::Local);
    if (position.y > box->minimum.y && box->maximum.y > position.y) {
        if (ryoNotBusy() && static_cast<std::uint32_t>(static_cast<std::int32_t>(date.hour) - 7) > 12U) {
            const Vector3 minimum{box->minimum.x - .5f, box->minimum.y - .5f, box->minimum.z - .5f};
            const Vector3 maximum{box->maximum.x + .5f, box->maximum.y + .5f, box->maximum.z + .5f};
            if (!TestCharaPositionWithinRange(ryo, &minimum, &maximum, shen_name{}))
                box->state &= ~1U;
            if (TestCharaPositionWithinRange(ryo, &box->minimum, &box->maximum, shen_name{}) && !(box->state & 1U)) {
                box->state |= 1U;
                result = 2U - static_cast<std::uint32_t>(shenGetMinutesUntilNextFrameMinute() != 0);
            }
        }
        if (TestCharaPositionWithinRange(ryo, &box->minimum, &box->maximum, shen_name{}) && ryoNotBusy()) {
            GetBattleDataConfig(0, sleepButton);
            if (GetBattleDataConfig(2, sleepButton))
                return 3;
        }
    }
    return result;
}
} // namespace shenmue::s2
