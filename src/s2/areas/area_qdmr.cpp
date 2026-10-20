#include "s2/areas/area_qdmr.hpp"
#include "s2/areas/area_loop.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/magic_room.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/scene_data.hpp"
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {
void QdmrInitialize(void *);
void QdmrInitialize2(void *);
void QdmrStartInitialAuthor(void *);
void QdmrEndInitialAuthor(void *);
void QdmrExitInitialAuthor(void *);
void QdmrInFree(void *);
void QdmrStartTimeoutAuthor(void *);
void QdmrWaitTimeoutNpcs(void *);
void QdmrEndTimeoutAuthor(void *);
void QdmrExitTimeoutAuthor(void *);
void QdmrStartYuanAuthor(void *);
void QdmrWaitYuanNpcs(void *);
void QdmrEndYuanAuthor(void *);
void QdmrExitYuanAuthor(void *);
void QdmrBeginRetry(void *);
void QdmrRetryTitle(void *);
void QdmrStartTransition(void *);
void QdmrMoveToTransition(void *);
void QdmrEndTransition(void *);
void QdmrStartOpenDoor(void *);
void QdmrEscapeToLadder(void *);
void QdmrWaitLadderMotion(void *);
void QdmrOpenFirstRoom(void *);
void QdmrOpenSecondRoom(void *);
void QdmrOpenThirdRoom(void *);
void QdmrRepeatFirstRoom(void *);
void QdmrRepeatSecondRoom(void *);
void QdmrRepeatThirdRoom(void *);
void QdmrOpenBingoRoom(void *);
void QdmrOpenNormalRoom(void *);
void QdmrWaitFirstRoomPackage(void *);
void QdmrWaitSecondRoomPackage(void *);
void QdmrWaitThirdRoomPackage(void *);
void QdmrWaitFirstRepeatPackage(void *);
void QdmrWaitSecondRepeatPackage(void *);
void QdmrWaitThirdRepeatPackage(void *);
void QdmrWaitBingoPackage(void *);
void QdmrWaitNormalRoom(void *);
void QdmrFirstRoomPresentation(void *);
void QdmrSecondRoomPresentation(void *);
void QdmrThirdRoomPresentation(void *);
void QdmrFirstRepeatPresentation(void *);
void QdmrSecondRepeatPresentation(void *);
void QdmrThirdRepeatPresentation(void *);
void QdmrWaitBingoNpcs(void *);
void QdmrNormalRoomPresentation(void *);
void QdmrBeginFirstRoomFight(void *);
void QdmrBeginSecondRoomFight(void *);
void QdmrBeginThirdRoomFight(void *);
void QdmrFirstRoomFight(void *);
void QdmrSecondRoomFight(void *);
void QdmrThirdRoomFight(void *);
void QdmrEndFirstRepeat(void *);
void QdmrEndSecondRepeat(void *);
void QdmrEndThirdRepeat(void *);
void QdmrEndNormalRoom(void *);
void QdmrEndBingoOpening(void *);
void QdmrBingoFight(void *);
void QdmrBingoWin(void *);
void QdmrBingoLose(void *);
void QdmrAdmissionNoop() {
}

void QdmrMain(std::uintptr_t) {
    const std::uint64_t previousLoad = GetNpcNewLoadState();
    if (ctrlGetCharaByName(QDMR_REN)) {
        EV_PTYCUtils(QDMR_REN, 8, 0);
        EV_npcFree(QDMR_REN);
    }
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    std::int32_t count = 0;
    while (QDMR_ROOMS[count].door.name)
        ++count;
    const std::int32_t roomBytes = 4 * count;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QdmrInitialize, 4, 0x0b, sizeof(QdmrEventWork) + roomBytes, AREA_EVENT_TASK);
    if (task) {
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(task));
        if (work) {
            std::memset(static_cast<void *>(work), 0, sizeof(*work));
            HLib::Task *const timer = HLib::EnqueueTaskWithParameter(QdmrTimerCallback, 4, 0x0b, sizeof(HLib::Task *), AREA_EVENT_TASK);
            if (timer) {
                HLib::Task **const timerParent = static_cast<HLib::Task **>(HLib::GetTaskParameter(timer));
                if (timerParent) {
                    *timerParent = nullptr;
                    work->task = task;
                    work->timerTask = timer;
                    work->parent = HLib::GetCurrentTask();
                    if (roomBytes) {
                        work->roomFlags = reinterpret_cast<std::uint32_t *>(work + 1);
                        if (roomBytes > 0)
                            std::memset(work->roomFlags, 0, roomBytes);
                    } else
                        work->roomFlags = nullptr;
                    work->timer = 0;
                    work->timerFlags = 0;
                    work->timerFadeFrames = 0;
                    work->timerWarningThreshold = 900;
                    work->timerRequests = 0;
                    work->timerText = CreateLegacyHudText();
                    if (work->timerText) {
                        *timerParent = task;
                        spmFadeOut();
                        work->flags |= 0x20U;
                        EV_SetCanDoAreaJumpTo(0);
                        EV_EventSetUpHelpIcon_0(10, 0);
                        SetPlayerControlEnabled(0);
                        work->flags |= (previousLoad ? 0x10U : 0U) | 5U;
                        EV_stepSetFlags(0, 0x1be);
                        work->canSkipIntro = 0;
                        return;
                    }
                }
                HLib::DestroyTask(timer);
            }
        }
        HLib::DestroyTask(task);
    }
    if (previousLoad)
        EV_NPC_NewLoadSw(1);
}
void QdmrInitialize(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrInitialize2);
    HLib::SetupTimedTask(work->task, -1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMR");
}
void QdmrInitialize2(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    flddSetPointFlagOnGrid(QDMR_QFBA, 0);
    HLib::SetCurrentTaskTo(QdmrStartInitialAuthor);
}
void QdmrStartInitialAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        if (!SetQronFloor(18))
            return;
        RepositionPlayerForQronFloor(18);
        EV_FreeAuthorPak(1, "0266");
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDMR"), 0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrEndInitialAuthor);
        HLib::SetupTimedTask(work->task, -1);
        work->flags |= 1U;
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (work->canSkipIntro)
            EV_SetAuthorActiveTo(1);
        QueueAreaLoopAuthor("QDMRINIT\0", QDMR_INIT_LIGHT, QdmrInitialAuthorCompletion);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrEndInitialAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        EV_FreeAuthorPak(0, "0266");
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        HLib::SetCurrentTaskTo(QdmrExitInitialAuthor);
    } else {
        spmFadeOut();
        work->flags |= 0x20U;
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrExitInitialAuthor(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (!SetQronFloor(18))
        return;
    RepositionPlayerForQronFloor(18);
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_LoadAudio("0266A000", 0);
    EV_spmBeginSampleExecute();
    work->flags &= ~0x20U;
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 2,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    work->timer = 1800;
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 218.75999f;
    position.z = 175.24001f;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = {0, 10922, 0}, .completion = nullptr}, 0);
    mtwkSetExtraFlag(scenFindMotionwork(QDMR_PLAYER), 17, 0);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrInFree(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index)
        if (!(work->roomFlags[index] & 1U) && ClaimDoor(QDMR_ROOMS[index].door, QDMR_PLAYER))
            work->roomFlags[index] |= 1U;
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    float x = position.x - QDMR_LADDER_POSITION.x, z = position.z - QDMR_LADDER_POSITION.z;
    if (std::fabs(x) < 1.1920929e-7f)
        x = 0;
    if (std::fabs(z) < 1.1920929e-7f)
        z = 0;
    float xx = x * x, zz = z * z;
    if (std::fabs(xx) < 1.1920929e-7f)
        xx = 0;
    if (std::fabs(zz) < 1.1920929e-7f)
        zz = 0;
    float distance = xx + zz;
    if (std::fabs(distance) < 1.1920929e-7f || distance < 4) {
        HLib::SetCurrentTaskTo(QdmrEscapeToLadder);
        SetPlayerControlEnabled(0);
        work->flags |= 4U;
        return;
    }
    work->flags |= 8U;
    if (work->flags & 4U) {
        FinishLoading();
        SetPlayerControlEnabled(1);
        work->flags &= ~4U;
        return;
    }
    SetPlayerGameFlags(PlayerGameFlagMode::SearchBlocked, reinterpret_cast<void *>(1));
    const std::int32_t timerMode = work->timerFlags & 3;
    const std::int64_t soundScene = timerMode ? 2 : 0;
    if (GetSelectedSoundSceneIndex() != soundScene) {
        EV_LoadAudio("0266A000", soundScene);
        EV_spmBeginSampleExecute();
    }
    if (!timerMode) {
        x = position.x - 217.05f;
        z = position.z - 126.56f;
        if (std::fabs(x) < 1.1920929e-7f)
            x = 0;
        if (std::fabs(z) < 1.1920929e-7f)
            z = 0;
        xx = x * x;
        zz = z * z;
        if (std::fabs(xx) < 1.1920929e-7f)
            xx = 0;
        if (std::fabs(zz) < 1.1920929e-7f)
            zz = 0;
        distance = xx + zz;
        if (std::fabs(distance) < 1.1920929e-7f || distance < 4)
            work->timer = 0;
    }
    if (work->timer <= 0) {
        if (!timerMode) {
            HLib::SetCurrentTaskTo(QdmrStartTransition);
            return;
        }
        work->timerRequests |= 0x40000000U;
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 0,
                                .motion = 0xf002,
                                .duration = 1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        HLib::SetCurrentTaskTo(QdmrStartTimeoutAuthor);
        if (srchHasActiveLockonTarget())
            srchReset();
        SetPlayerControlEnabled(0);
        work->flags |= 4U;
        return;
    }
    const shen_name door = FindNearestQdmrDoor(work->roomFlags);
    std::uint16_t buttons = 0;
    if (door.name) {
        if (GetBattleDataConfig(1, QDMR_BTKK) && GetBattleDataConfig(2, QDMR_BTKK))
            buttons = 1024;
        else
            GetBattleDataConfig(0, QDMR_BTKK);
        if (GetBattleDataConfig(1, QDMR_BTPP) && GetBattleDataConfig(2, QDMR_BTPP))
            buttons |= 4U;
        else
            GetBattleDataConfig(0, QDMR_BTPP);
        if (GetBattleDataConfig(1, QDMR_BTAA) && GetBattleDataConfig(2, QDMR_BTAA))
            buttons |= 2U;
        else
            GetBattleDataConfig(0, QDMR_BTAA);
        if (buttons) {
            work->doorQteButtons = buttons;
            SetPlayerControlEnabled(0);
            work->flags |= 4U;
            QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                    .character = QDMR_PLAYER,
                                    .flags = 0,
                                    .motion = 0xf002,
                                    .duration = -1,
                                    .elapsed = 0,
                                    .startFrame = 0,
                                    .endFrame = -1,
                                    .speed = 1,
                                    .frameTime = 8,
                                    .reserved2c = 0,
                                    .completion = nullptr},
                0);
            SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
            if (GetQdmrDoorVariant(door, work->roomFlags))
                spmFadeOut();
            work->flags |= 1U;
            work->door = door;
            HLib::SetCurrentTaskTo(QdmrStartOpenDoor);
        }
    } else
        work->doorQteButtons = 0;
}
void QdmrStartOpenDoor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        return;
    }
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &work->savedPosition, EventTransformFlags::World);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, EventTransformFlags::World);
    work->savedYaw = rotation.y;
    switch (GetQdmrDoorVariant(work->door, work->roomFlags)) {
    case 1:
        QdmrOpenFirstRoom(work);
        break;
    case 2:
        QdmrOpenSecondRoom(work);
        break;
    case 3:
        QdmrOpenThirdRoom(work);
        break;
    case 4:
        QdmrRepeatFirstRoom(work);
        break;
    case 5:
        QdmrRepeatSecondRoom(work);
        break;
    case 6:
        QdmrRepeatThirdRoom(work);
        break;
    case 7:
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        ReleaseDoor(work->door.name, QDMR_PLAYER);
        work->timerRequests |= 0x40000000U;
        QdmrOpenBingoRoom(work);
        work->battleRetries = 0;
        break;
    default:
        QdmrOpenNormalRoom(work);
        break;
    }
}
void QdmrEscapeToLadder(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_FreeAuthorPak(1, "0266");
    QueueAreaLoopVoice("0266A009", 0, -1, nullptr, 0);
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = QDMR_LADDER_POSITION.x;
    position.z = QDMR_LADDER_POSITION.z;
    CopyMotionTransition(scenFindMotionwork(QDMR_PLAYER), &position, 10922, 0, 0);
    HLib::SetCurrentTaskTo(QdmrWaitLadderMotion);
}
void QdmrWaitLadderMotion(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (!mtwkSetExtraFlag(scenFindMotionwork(QDMR_PLAYER), 58, 1)) {
        HLib::SetCurrentTaskTo(QdmrInFree);
        EV_FreeAuthorPak(0, "0266");
    }
}
void QdmrStartTimeoutAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (srchHasActiveLockonTarget())
        srchReset();
    SetPlayerControlEnabled(0);
    work->flags |= 4U;
    if (GetCurrentAuthorId().name) {
        if (!doorLoadAuthorSilently({}))
            return;
        ExecuteCleanupFuncForAreaWithCachedScene();
        EV_NPC_NewLoadSw(0);
    }
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitTimeoutNpcs);
    HLib::SetupTimedTask(work->task, -1);
    QueueAreaLoopNpcs(QDMR_NPCS, ResolveAreaLoopParent);
}
void QdmrWaitTimeoutNpcs(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    work->flags &= ~0x20U;
    HLib::SetCurrentTaskTo(QdmrEndTimeoutAuthor);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetupTimedTask(work->task, -1);
    work->flags |= 1U;
    if (work->canSkipIntro)
        EV_SetAuthorActiveTo(1);
    QueueAreaLoopAuthor("QDMRTOVR\0", QDMR_TIMEOUT_AUTHOR, QdmrStoryAuthorCompletion);
}
void QdmrEndTimeoutAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        RestoreAreaLoopCharacters(QDMR_NPCS);
        EV_FreeAuthorPak(0, "0266");
        spmFadeOut();
        work->flags |= 0x20U;
        HLib::SetCurrentTaskTo(QdmrExitTimeoutAuthor);
        Queue_ST_LoadTitleFadeIn(QDMR_TIMEOUT_TITLE);
        HLib::SetupTaskDelay(100);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrExitTimeoutAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrStartYuanAuthor);
    EV_spmBeginSampleExecute();
    work->flags &= ~0x20U;
    HLib::SetupTaskDelay(30);
    ST_LoadTitleFade();
    work->canSkipIntro = 1;
}
void QdmrStartYuanAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
            work->roomFlags[index] &= ~2U;
            if (work->roomFlags[index] & 1U) {
                ReleaseDoor(QDMR_ROOMS[index].door.name, QDMR_PLAYER);
                work->roomFlags[index] &= ~1U;
            }
        }
        if (!SetQronFloor(17))
            return;
        RepositionPlayerForQronFloor(17);
        ExecuteCleanupFuncForAreaWithCachedScene();
        EV_FreeAuthorPak(1, "0364");
        HLib::SetCurrentTaskTo(QdmrWaitYuanNpcs);
        HLib::SetupTimedTask(work->task, -1);
        QueueAreaLoopNpcs(QDMR_YUAN_NPCS, ResolveAreaLoopParent);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrWaitYuanNpcs(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrEndYuanAuthor);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetupTimedTask(work->task, -1);
    work->flags |= 1U;
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if (work->canSkipIntro)
        EV_SetAuthorActiveTo(1);
    QueueAreaLoopAuthor("QES3END\0", QDMR_YUAN_AUTHOR, QdmrYuanAuthorCompletion);
}
void QdmrEndYuanAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        RestoreAreaLoopCharacters(QDMR_YUAN_NPCS);
        EV_FreeAuthorPak(0, "0364");
        spmFadeOut();
        work->flags |= 0x20U;
        HLib::SetCurrentTaskTo(QdmrExitYuanAuthor);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrExitYuanAuthor(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (SetQronFloor(18)) {
        RepositionPlayerForQronFloor(18);
        HLib::SetCurrentTaskTo(QdmrStartInitialAuthor);
    }
}
void QdmrBeginRetry(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    work->canSkipIntro = 1;
    if ((work->timerFlags & 3) == 1)
        work->timerRequests |= 0x40000000U;
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        Queue_ST_LoadTitleFadeIn(QDMR_RETRY_TITLE);
        HLib::SetCurrentTaskTo(QdmrRetryTitle);
        HLib::SetupTaskDelay(50);
        spmFadeOut();
        work->flags |= 0x20U;
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrRetryTitle(void *) {
    ST_LoadTitleFade();
    HLib::SetCurrentTaskTo(QdmrStartYuanAuthor);
    HLib::SetupTaskDelay(50);
}
void QdmrStartTransition(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_FreeAuthorPak(1, "0266");
    SetPlayerControlEnabled(0);
    work->flags |= 4U;
    HLib::SetCurrentTaskTo(QdmrMoveToTransition);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRTRNS");
}
void QdmrMoveToTransition(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetPlayerControlEnabled(0);
    work->flags |= 4U;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, EventTransformFlags::World);
    FindClearRotationPathForAngY(&rotation.y);
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0x16,
                            .motion = 0x1c45,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    HLib::SetCurrentTaskTo(QdmrEndTransition);
    HLib::SetupTaskDelay(-1);
    SetQdmrPlayerHandState(work, true);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QDMR_PLAYER), static_cast<std::int32_t>(FacialExpressionId::Angry), 0, 0.69999999f);
    work->flags |= 2U;
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(QDMR_PLAYER), 0, reinterpret_cast<std::byte *>(QDMR_TRANSITION_HAND_POSE.data()), 1);
    QueueAreaLoopCamera("QDMRTRNS", 2000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRTRNS", 2001, QDMR_DUMMY, nullptr, 220);
    QueueAreaLoopCamera("QDMRTRNS", 2002, QDMR_DUMMY, nullptr, 540);
    SetCharacterHandSecondaryPoseWork(ctrlGetCharaByName(QDMR_PLAYER), 0, ctrlGetCharaByName(QDMR_TRANSITION_TARGET));
    position = {0.029999999f, -0.050000001f, -0.039999999f};
    rotation = {64917, 32768, 57307};
    QueueQdmrTransitionAttachment(&position, &rotation);
    QueueQdmrTransitionLookAt();
    QueueAreaLoopVoice("0266C001", 0, -1, nullptr, 0);
    QueueAreaLoopVoice("0266C004", 0, -1, nullptr, 100);
    QueueAreaLoopVoice("0266C002", 0, -1, nullptr, 220);
    QueueAreaLoopVoice("0266C003", 0, -1, nullptr, 381);
    QueueAreaLoopVoice("0266A012", 0, -1, ResolveAreaLoopParent, 541);
}
void QdmrEndTransition(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work->flags |= 1U;
        return;
    }
    SetCharacterHandSecondaryPoseWork(ctrlGetCharaByName(QDMR_PLAYER), 0, nullptr);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    work->flags &= ~2U;
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 2,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRTRNS"), 0);
    EV_FreeAuthorPak(0, "0266");
    work->timerRequests |= 0x40000000U;
    work->timer = 10800;
    HLib::SetCurrentTaskTo(QdmrInFree);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
}
void QdmrOpenFirstRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitFirstRoomPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBTL1");
}
void QdmrOpenSecondRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitSecondRoomPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBTL2");
}
void QdmrOpenThirdRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitThirdRoomPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBTL3");
}
void QdmrRepeatFirstRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitFirstRepeatPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrRepeatSecondRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitSecondRepeatPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrRepeatThirdRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitThirdRepeatPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrOpenBingoRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitBingoPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBBTL");
}
void QdmrOpenNormalRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    SelectMagicRoomForPlayer();
    HLib::SetCurrentTaskTo(QdmrWaitNormalRoom);
}
void QdmrWaitFirstRoomPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_NPC_NewLoadSw(0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrFirstRoomPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_FIRST_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitSecondRoomPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_NPC_NewLoadSw(0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrSecondRoomPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_SECOND_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitThirdRoomPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_NPC_NewLoadSw(0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrThirdRoomPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_THIRD_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitFirstRepeatPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 2);
        EV_spmBeginSampleExecute();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QdmrFirstRepeatPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_FIRST_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitSecondRepeatPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 2);
        EV_spmBeginSampleExecute();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QdmrSecondRepeatPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_SECOND_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitThirdRepeatPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 2);
        EV_spmBeginSampleExecute();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QdmrThirdRepeatPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_THIRD_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitBingoPackage(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrWaitBingoNpcs);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopNpcs(QDMR_BINGO_NPCS, ResolveAreaLoopParent);
}
void QdmrWaitNormalRoom(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (!IsMagicRoomReady())
        return;
    HLib::SetCurrentTaskTo(QdmrNormalRoomPresentation);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrFirstRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrBeginFirstRoomFight);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    Vector3i rotation;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    Vector3 position{0, 0, -0.5f};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    position = {0, 0, -0.69999999f};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 0xf002,
                            .duration = 259,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        350);
    const shen_name door{work->door.name & 0xffffffdfU};
    doorLoopRigidMotion(door, 7234, 0, 1);
    HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (pause) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
        if (motion)
            *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
    }
    HLib::Task *const resume = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (resume) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(resume));
        if (motion)
            *motion = {.door = door, .frames = 351, .startFrame = -1, .speed = 1, .motion = 7234};
    }
    Loop3DSoundEffect(0x91b);
    QueueAreaLoopCamera("QDMRBTL1", 4000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRBTL1", 4001, QDMR_DUMMY, nullptr, 150);
    QueueAreaLoopCamera("QDMRBTL1", 4002, QDMR_DUMMY, nullptr, 260);
    QueueAreaLoopCamera("QDMRBTL1", 4003, QDMR_DUMMY, nullptr, 350);
    QueueAreaLoopVoice("0266E001", 0, -1, nullptr, 150);
    QueueAreaLoopVoice("0266A027", 0, -1, nullptr, 260);
    QueueAreaLoopVoice("0266E002", 0, -1, nullptr, 350);
    QueueAreaLoopVoice("0266E003", 0, -1, ResolveAreaLoopParent, 510);
    HLib::Task *const fade = HLib::EnqueueTaskWithParameter(QdmrFadeOutSoundCallback, 4, 0x0b, 0, AREA_EVENT_TASK);
    if (fade)
        HLib::SetupTimedTask(fade, 510);
    else
        spmFadeOut();
    work->flags |= 0x20U;
    position.x = 205.36f;
    position.z = 166.8f;
    rotation.y = 41214;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_SBK, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_SBK,
                            .flags = 6,
                            .motion = 7241,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 204.94f;
    position.z = 168.56f;
    rotation.y = 58345;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_ANC, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_ANC,
                            .flags = 6,
                            .motion = 7235,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrSecondRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrBeginSecondRoomFight);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    Vector3i rotation;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    Vector3 position{0, 0, -0.5f};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    position = {0, 0, -0.69999999f};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 0xf002,
                            .duration = 259,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        350);
    const shen_name door{work->door.name & 0xffffffdfU};
    doorLoopRigidMotion(door, 7234, 0, 1);
    HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (pause) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
        if (motion)
            *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
    }
    HLib::Task *const resume = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (resume) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(resume));
        if (motion)
            *motion = {.door = door, .frames = 351, .startFrame = -1, .speed = 1, .motion = 7234};
    }
    Loop3DSoundEffect(0x91b);
    QueueAreaLoopCamera("QDMRBTL2", 5000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRBTL2", 5001, QDMR_DUMMY, nullptr, 150);
    QueueAreaLoopCamera("QDMRBTL2", 5002, QDMR_DUMMY, nullptr, 260);
    QueueAreaLoopCamera("QDMRBTL2", 5003, QDMR_DUMMY, nullptr, 350);
    QueueAreaLoopVoice("0266F001", 0, -1, nullptr, 150);
    QueueAreaLoopVoice("0266A029", 0, -1, nullptr, 260);
    QueueAreaLoopVoice("0266F002", 0, -1, nullptr, 350);
    QueueAreaLoopVoice("0266F003", 0, -1, ResolveAreaLoopParent, 510);
    HLib::Task *const fade = HLib::EnqueueTaskWithParameter(QdmrFadeOutSoundCallback, 4, 0x0b, 0, AREA_EVENT_TASK);
    if (fade)
        HLib::SetupTimedTask(fade, 510);
    else
        spmFadeOut();
    work->flags |= 0x20U;
    position.x = 254.8f;
    position.z = 156.10001f;
    rotation.y = 40960;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KU8, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KU8,
                            .flags = 6,
                            .motion = 7241,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 255.3f;
    position.z = 157.7f;
    rotation.y = 60074;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KHO, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KHO,
                            .flags = 6,
                            .motion = 7235,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrThirdRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrBeginThirdRoomFight);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    Vector3i rotation;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    Vector3 position{0, 0, -0.5f};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    position = {0, 0, -0.69999999f};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 0xf002,
                            .duration = 259,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        350);
    const shen_name door{work->door.name & 0xffffffdfU};
    doorLoopRigidMotion(door, 7234, 0, 1);
    HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (pause) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
        if (motion)
            *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
    }
    HLib::Task *const resume = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (resume) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(resume));
        if (motion)
            *motion = {.door = door, .frames = 351, .startFrame = -1, .speed = 1, .motion = 7234};
    }
    QueueQdmrLoopSound(0x91b, 90);
    QueueAreaLoopCamera("QDMRBTL3", 6000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRBTL3", 6001, QDMR_DUMMY, nullptr, 150);
    QueueAreaLoopCamera("QDMRBTL3", 6002, QDMR_DUMMY, nullptr, 260);
    QueueAreaLoopCamera("QDMRBTL3", 6003, QDMR_DUMMY, nullptr, 350);
    QueueAreaLoopVoice("0266G001", 0, -1, nullptr, 150);
    QueueAreaLoopVoice("0266A027", 0, -1, nullptr, 260);
    QueueAreaLoopVoice("0266G002", 0, -1, nullptr, 350);
    QueueAreaLoopVoice("0266G003", 0, -1, nullptr, 430);
    QueueAreaLoopVoice("0266G004", 0, -1, ResolveAreaLoopParent, 510);
    HLib::Task *const fade = HLib::EnqueueTaskWithParameter(QdmrFadeOutSoundCallback, 4, 0x0b, 0, AREA_EVENT_TASK);
    if (fade)
        HLib::SetupTimedTask(fade, 560);
    else
        spmFadeOut();
    work->flags |= 0x20U;
    position.x = 219.02f;
    position.z = 119.31f;
    rotation.y = 12797;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_UYD, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_UYD,
                            .flags = 6,
                            .motion = 7236,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 216.75f;
    position.z = 119.36f;
    rotation.y = 52610;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KT7, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KT7,
                            .flags = 6,
                            .motion = 7242,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrFirstRepeatPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 204.66f;
    position.z = 167.3f;
    rotation = {0, 41214, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_SBK, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_SBK,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 205.53999f;
    position.z = 169.25999f;
    rotation = {0, 58345, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_ANC, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_ANC,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndFirstRepeat);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrSecondRepeatPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 254.3f;
    position.z = 156.5f;
    rotation = {0, 40960, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KU8, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KU8,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 254.60001f;
    position.z = 158.0f;
    rotation = {0, 10922, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KHO, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KHO,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndSecondRepeat);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrThirdRepeatPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 219.02f;
    position.z = 120.21f;
    rotation = {0, 20075, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_UYD, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_UYD,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 217.25f;
    position.z = 120.36f;
    rotation = {0, 60802, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KT7, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KT7,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndThirdRepeat);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, first == 3 ? 68 : 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrNormalRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndNormalRoom);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrBeginFirstRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_LoadAudio("0266A000", 3);
    EV_spmBeginSampleExecute();
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QF1B, 0);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_FIRST_ROOM_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_FIRST_ROOM_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_FIRST_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_FIRST_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_FIRST_ROOM_NPCS[index]), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_FIRST_ROOM_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(2, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrFirstRoomFight);
}
void QdmrFirstRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive() || filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueQdmrLoopSound(0x85d, 30);
            QueueQdmrLoopSound(0x85e, 50);
        }
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work->flags |= 1U;
        return;
    }
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QF1B, 1);
    flddSetPointFlagOnGrid(QDMR_QFB1, 0);
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_FIRST_ROOM_NPCS);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0);
        Vector3 origin;
        Vector3i rotation;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        Vector3 position{0.5f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        rotation.y += 0x8000;
        EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        HLib::SetCurrentTaskTo(QdmrInFree);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    } else {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0);
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        HLib::SetCurrentTaskTo(QdmrBeginRetry);
    }
}
void QdmrEndFirstRepeat(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_FIRST_ROOM_NPCS);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    EV_LoadAudio("0266A000", 2);
}
void QdmrBeginSecondRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_LoadAudio("0266A000", 3);
    EV_spmBeginSampleExecute();
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QF2B, 0);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_SECOND_ROOM_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_SECOND_ROOM_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_SECOND_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_SECOND_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_SECOND_ROOM_NPCS[index]), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_SECOND_ROOM_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(2, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrSecondRoomFight);
}
void QdmrSecondRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive())
        return;
    if (!(work->flags & 1U)) {
        if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueQdmrLoopSound(0x85d, 30);
            QueueQdmrLoopSound(0x85e, 50);
            spmFadeOut();
            work->flags |= 0x20U;
        }
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QF2B, 1);
    flddSetPointFlagOnGrid(QDMR_QFB2, 0);
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_SECOND_ROOM_NPCS);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0);
        Vector3 origin;
        Vector3i rotation;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        Vector3 position{0.5f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        rotation.y += 0x8000;
        EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        HLib::SetCurrentTaskTo(QdmrInFree);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    } else {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0);
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        HLib::SetCurrentTaskTo(QdmrBeginRetry);
    }
}
void QdmrEndSecondRepeat(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_SECOND_ROOM_NPCS);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    EV_LoadAudio("0266A000", 2);
}
void QdmrBeginThirdRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_LoadAudio("0266A000", 3);
    EV_spmBeginSampleExecute();
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QF3B, 0);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_THIRD_ROOM_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_THIRD_ROOM_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_THIRD_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_THIRD_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_THIRD_ROOM_NPCS[index]), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_THIRD_ROOM_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(2, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrThirdRoomFight);
}
void QdmrThirdRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive())
        return;
    if (!(work->flags & 1U)) {
        if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueQdmrLoopSound(0x85d, 30);
            QueueQdmrLoopSound(0x85e, 50);
            spmFadeOut();
            work->flags |= 0x20U;
        }
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QF3B, 1);
    flddSetPointFlagOnGrid(QDMR_QFB3, 0);
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_THIRD_ROOM_NPCS);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0);
        Vector3 origin;
        Vector3i rotation;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        Vector3 position{0.5f, 0, 0.69999999f};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        rotation.y += 0x8000;
        EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        HLib::SetCurrentTaskTo(QdmrInFree);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    } else {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0);
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        HLib::SetCurrentTaskTo(QdmrBeginRetry);
    }
}
void QdmrEndThirdRepeat(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_THIRD_ROOM_NPCS);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    EV_LoadAudio("0266A000", 2);
}
void QdmrEndNormalRoom(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    LeaveMagicRoom();
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    HLib::SetupTaskDelay(10);
}
void QdmrWaitBingoNpcs(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (QDMR_ROOMS[index].package.name) {
            if (!doorLoadAuthorSilently(QDMR_ROOMS[index].package))
                return;
            EV_NPC_NewLoadSw(0);
        }
        break;
    }
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndBingoOpening);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopAuthor("QDMRBGNB", QDMR_BINGO_OPEN_AUTHOR, QdmrBingoOpeningAuthorCompletion);
}
void QdmrEndBingoOpening(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    EV_FreeAuthorPak(0, "0266");
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDMR"), 0);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QFIX, 0);
    flddSetPointFlagOnGrid(QDMR_QFBA, 1);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_BINGO_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_BINGO_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_BINGO_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_BINGO_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_BINGO_NPCS[index]), 0, 0x400000, QDMR_BINGO_NPCS[index].name == QDMR_KF4.name ? 32 : 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_BINGO_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(4, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrBingoFight);
    work->flags &= ~1U;
}
void QdmrBingoFight(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive() || filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDMR"), 0);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QFIX, 1);
    flddSetPointFlagOnGrid(QDMR_QFBA, 0);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        SetQdmrPlayerHandState(work, true);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        EV_FreeAuthorPak(1, "0266");
        HLib::SetCurrentTaskTo(QdmrBingoWin);
        HLib::SetupTaskDelay(-1);
        EV_DisableCharacterDisplay(QDMR_P02);
        EV_DisableCharacterDisplay(QDMR_P07);
        EV_DisableCharacterDisplay(QDMR_P09);
        spmFadeOut();
        work->flags |= 0x20U;
        QueueAreaLoopAuthor("QDMRQITB", QDMR_BINGO_WIN_AUTHOR, QdmrBingoWinAuthorCompletion);
    } else {
        HLib::SetCurrentTaskTo(QdmrBingoLose);
        Queue_ST_LoadTitleFadeIn(QDMR_RETRY_TITLE);
        HLib::SetupTaskDelay(50);
    }
}
void QdmrBingoWin(void *parameter) {
    using enum CameraState;
    RestoreAreaLoopCharacters(QDMR_BINGO_NPCS);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0);
    EV_FreeAuthorPak(0, "0266");
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    if (parameter)
        ExecuteCleanupFuncForAreaWithCachedScene();
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 2,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    EV_stepSetFlags(0, 0x12a);
    EV_stepSetFlags(0, 0x135);
    HLib::SetCurrentTaskTo(QdmrDoneCallback);
}
void QdmrBingoLose(void *parameter) {
    if (!doorLoadAuthorSilently({}))
        return;
    ST_LoadTitleFade();
    RestoreAreaLoopCharacters(QDMR_BINGO_NPCS);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0);
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (work) {
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        QdmrOpenBingoRoom(work);
    } else
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
}
} // namespace
const ScriptAreaDefinition qdmrDefinition{.areaId = QDMR_AREA, .field04 = 0, .init = QdmrAdmissionNoop, .main = QdmrMain};
} // namespace shenmue::s2
