#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_anan.hpp"

const char ananPackage[] = "ANAN";

void RunAnan(int entry)
{
    int started;
    HLibTask *task, *ryoTask = 0, *ananTask = 0;
    AnanPakWork *pak;
    AnanWork *work;
    AnanWork **ryoParameter, **ananParameter;
    char *bytes;
    int index;
    started = 0;
    do {
        if (entry == 2 || entry == 102) {
            task = FTBL.HLib_EnqueueTaskWithParameter(AnanLoadBoard, 4, 11, sizeof(AnanPakWork), ANAN_TASK);
            if (task) {
                pak = (AnanPakWork *)FTBL.HLib_GetTaskParameter(task);
                if (pak) {
                    pak->caller = FTBL.HLib_GetCurrentTask();
                    pak->npcState = FTBL.GetNpcNewLoadState();
                    pak->specialEntry = entry == 102;
                    FTBL.EV_NPC_NewLoadSw_0(0);
                    FTBL.EV_ResetFreeTalkFlags();
                    FTBL.HLib_SetupTaskDelay(-1);
                    started = 1;
                }
            } else
                FTBL.DebugLogToFileRemoved(ananCreateTaskError);
        } else {
            FTBL.EventFunction870(0);
            FTBL.srchRequestLockonExit();
            task = FTBL.HLib_EnqueueTaskWithParameter(AnanInitialize, 4, 11, sizeof(AnanWork), ANAN_TASK);
            if (task) {
                work = (AnanWork *)FTBL.HLib_GetTaskParameter(task);
                if (work) {
                    ryoTask = FTBL.HLib_EnqueueTaskWithParameter(AnanTrackRyo, 4, 11, 4, ANAN_TASK);
                    if (ryoTask) {
                        ryoParameter = (AnanWork **)FTBL.HLib_GetTaskParameter(ryoTask);
                        if (ryoParameter) {
                            ananTask = FTBL.HLib_EnqueueTaskWithParameter(AnanTrackMan, 4, 11, 4, ANAN_TASK);
                            if (ananTask) {
                                ananParameter = (AnanWork **)FTBL.HLib_GetTaskParameter(ananTask);
                                bytes = (char *)work;
                                for (index = 0; index < (int)sizeof(AnanWork); index++)
                                    bytes[index] = 0;
                                work->caller = FTBL.HLib_GetCurrentTask();
                                work->lifecycle = 0;
                                if (entry)
                                    work->entry = 0;
                                else
                                    work->entry = 1;
                                work->game.tilt = -0.85714287f;
                                work->ryoTask = ryoTask;
                                work->ananTask = ananTask;
                                *ryoParameter = work;
                                *ananParameter = work;
                                work->targets[0] = ANAN_INVALID;
                                work->targets[1] = ANAN_INVALID;
                                work->transitionFrames[0] = 0;
                                work->transitionFrames[1] = 0;
                                work->positionModes[0] = 0;
                                work->positionModes[1] = 0;
                                FTBL.HLib_SetupTaskDelay(-1);
                                started = 1;
                                EventDebugRegister(ANAN_DEBUG_ID, AnanDebugSkip, work, 0x7fffffff);
                                work->self = task;
                                FTBL.EV_EventSetUpHelpIcon_0(69, 0);
                                FTBL.SetLoadingBusy();
                                work->lifecycle &= ~0x8000U;
                                FTBL.EV_SetNewSceneIndex();
                                FTBL.spmFadeOut(0);
                            }
                        }
                    }
                }
            }
        }
    } while (0);
    if (!started) {
        if (task)
            FTBL.HLib_DestroyTask(task);
        if (ryoTask)
            FTBL.HLib_DestroyTask(ryoTask);
        if (ananTask)
            FTBL.HLib_DestroyTask(ananTask);
    }
}

void AnanInitializeGame(void *data)
{
    AnanWork *work = (AnanWork *)data;
    AnanGame *game;
    int index;
    if (work) {
        game = &work->game;
        for (index = 0; ananGamePlacement[index].character != ANAN_INVALID; index++) {
            FTBL.EV_AT_SetPos(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].position);
            FTBL.EV_AT_SetAngY_0(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].rotation, 0);
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ananGamePlacement[index].character), 17, 0);
        }
        game->gameFlags = 0;
        game->idleFrames = 0;
        AnanClearInput(game);
        game->directionVoice = 0;
        game->responseVoice = 0;
        game->failures = 0;
        game->endHoldFrames = 0;
        QueueAnanCamera(ananPackage, 1757, ANAN_BASE, 0);
        FTBL.HLib_SetCurrentTaskTo(AnanRunGame);
        AnanRunGame(work);
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanRestartGame(void *data)
{
    AnanWork *work = (AnanWork *)data;
    AnanGame *game;
    int index;
    if (work) {
        game = &work->game;
        for (index = 0; ananGamePlacement[index].character != ANAN_INVALID; index++) {
            FTBL.EV_AT_SetPos(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].position);
            FTBL.EV_AT_SetAngY_0(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].rotation, 0);
        }
        game->gameFlags &= ~0x8000U;
        AnanClearInput(game);
        game->directionVoice = 0;
        game->responseVoice = 0;
        game->endHoldFrames = 0;
        QueueAnanCamera(ananPackage, 1757, ANAN_BASE, 0);
        AnanUpdatePose(game);
        FTBL.HLib_SetCurrentTaskTo(AnanRunGame);
        AnanRunGame(work);
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanRunGame(void *data)
{
    AnanWork *work = (AnanWork *)data;
    AnanGame *game;
    int state, normal;
    if (work) {
        game = &work->game;
        AnanUpdateDirectionVoice(work);
        state = AnanUpdateGame(work);
        normal = 0;
        switch (state) {
        case -1:
            break;
        case 0:
        case 6:
        case 8:
        default:
            AnanGetHandPosition(game->tilt, &work->positions[0]);
            work->targets[0] = 0;
            work->positionModes[0] = 0;
            work->targets[1] = ANAN_INVALID;
            work->positionModes[1] = 0;
            normal = 1;
            AnanSetTargets(game->tilt, normal, normal);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            work->targets[0] = 0;
            AnanGetHandPosition(game->tilt, &work->positions[0]);
            work->positionModes[0] = 0;
            work->targets[1] = state == 2 ? 0 : ANAN_RYO;
            work->positionModes[1] = 1;
            AnanSetTargets(game->tilt, 0, 1);
            break;
        case 5:
        case 7:
            work->targets[0] = ANAN_MAN;
            work->targets[1] = ANAN_RYO;
            work->positionModes[0] = 1;
            work->positionModes[1] = 1;
            AnanSetTargets(game->tilt, 0, 1);
            break;
        }
        if (state <= 0 || (state > 5 && state != 7))
            AnanUpdatePose(game);
        if (normal) {
            if (!(work->lifecycle & 0x8000U)) {
                FTBL.FinishLoading();
                work->lifecycle |= 0x8000U;
            }
            FTBL.CheckHelpIcon(12321, 0, 3);
        } else if (work->lifecycle & 0x8000U) {
            FTBL.SetLoadingBusy();
            work->lifecycle &= ~0x8000U;
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

int AnanUpdateDirectionVoice(AnanWork *work)
{
    AnanGame *game;
    int current, previous, nonzero;
    int index;
    do {
        if (!work)
            break;
        game = &work->game;
        AnanReadInput(game);
        if (game->directionVoice && !FTBL.cvsHasValidConverseData(game->directionVoice))
            game->directionVoice = 0;
        current = (unsigned char)AnanHistoryInput(game, 23);
        previous = (unsigned char)AnanHistoryInput(game, 22);
        nonzero = 0;
        index = 22;
        while (index >= 0) {
            nonzero = (unsigned char)AnanHistoryInput(game, index);
            index--;
            if (nonzero)
                break;
        }
        if (current == previous) {
            if (!game->directionVoice) {
                switch (current) {
                case 2:
                    game->directionVoice = FTBL.EV_PlayDialogue("0020A006");
                    break;
                case 3:
                    game->directionVoice = FTBL.EV_PlayDialogue("0020A009");
                    break;
                }
            }
        } else {
            if (game->directionVoice) {
                switch (current) {
                case 1:
                case 2:
                case 3:
                    if (current != nonzero) {
                        FTBL.scenCvsCtrlFree(game->directionVoice);
                        game->directionVoice = 0;
                    }
                    break;
                }
            }
            if (!game->directionVoice) {
                switch (current) {
                case 1:
                    game->directionVoice = FTBL.EV_PlayDialogue("0020A011");
                    break;
                case 2:
                    game->directionVoice = FTBL.EV_PlayDialogue("0020A005");
                    break;
                case 3:
                    game->directionVoice = FTBL.EV_PlayDialogue("0020A008");
                    break;
                }
            }
        }
        switch (current) {
        case 0:
            game->idleFrames++;
            break;
        case 1:
            game->gameFlags = (game->gameFlags | 0x8000U) & ~1U;
            game->idleFrames = 0;
            break;
        default:
            game->gameFlags &= ~1U;
            game->idleFrames = 0;
            break;
        }
    } while (0);
    return 0;
}

AnanPlacement ananGamePlacement[] = {{ANAN_RYO, {0.0f, 0.0f, 1.92f}, {0, 0, 0}}, {ANAN_MAN, {-1.35f, 1.27f, 0.43f}, {0, 0xfddd, 0}},
    {ANAN_STOOL, {-1.35f, -0.2f, 0.46f}, {0, 0x3999, 0}}, {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
#else
#include "s2/areas/area_anan.hpp"
#include "s2/areas/anan_tables.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include <cstring>
namespace shenmue::s2::areas {
void InitializeAnan() {
}
void AnanResetHelp() {
}
void RunAnan(std::uintptr_t argument) {
    const std::int32_t entry = static_cast<std::int32_t>(argument);
    HLib::Task *ryoTask = nullptr;
    HLib::Task *ananTask = nullptr;
    if (entry == 2 || entry == 102) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(AnanLoadBoard, 4, 11, 0x10, ANAN_EVENT);
        if (!task)
            return;
        AnanPakWork *work = static_cast<AnanPakWork *>(HLib::GetTaskParameter(task));
        if (work) {
            work->caller = HLib::GetCurrentTask();
            work->npcState = GetNpcNewLoadState();
            work->specialEntry = entry == 102;
            work->reserved14 = 0;
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            HLib::SetupTaskDelay(-1);
            return;
        }
        HLib::DestroyTask(task);
        return;
    }
    ReleaseNpcScriptRouteOwner();
    srchRequestLockonExit();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AnanInitialize, 4, 11, 0x88, ANAN_EVENT);
    if (!task)
        return;
    AnanWork *work = static_cast<AnanWork *>(HLib::GetTaskParameter(task));
    if (work) {
        ryoTask = HLib::EnqueueTaskWithParameter(AnanTrackRyo, 4, 11, 8, ANAN_EVENT);
        if (ryoTask) {
            AnanWork **ryoParameter = static_cast<AnanWork **>(HLib::GetTaskParameter(ryoTask));
            if (ryoParameter) {
                ananTask = HLib::EnqueueTaskWithParameter(AnanTrackMan, 4, 11, 8, ANAN_EVENT);
                if (ananTask) {
                    AnanWork **ananParameter = static_cast<AnanWork **>(HLib::GetTaskParameter(ananTask));
                    std::memset(work, 0, 0x88);
                    work->caller = HLib::GetCurrentTask();
                    work->lifecycle = 0;
                    work->tilt = -.85714287f;
                    work->entry = entry == 0;
                    work->ryoTask = ryoTask;
                    work->ananTask = ananTask;
                    *ryoParameter = work;
                    *ananParameter = work;
                    work->targets[0] = ANAN_INVALID;
                    work->targets[1] = ANAN_INVALID;
                    work->transitionFrames[0] = work->transitionFrames[1] = 0;
                    work->positionModes[0] = work->positionModes[1] = 0;
                    HLib::SetupTaskDelay(-1);
                    work->self = task;
                    EV_EventSetUpHelpIcon_0(0x45, 0);
                    AnanResetHelp();
                    work->lifecycle &= ~0x8000U;
                    EV_SetNewSceneIndex();
                    spmFadeOut();
                    return;
                }
            }
        }
    }
    HLib::DestroyTask(task);
    if (ryoTask)
        HLib::DestroyTask(ryoTask);
    if (ananTask)
        HLib::DestroyTask(ananTask);
}
void AnanLoadBoard(void *data) {
    AnanPakWork *work = static_cast<AnanPakWork *>(data);
    if (!work)
        return;
    const char *directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    if (!EV_pakUtils(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(ANBD_BOARD_PACKAGE)))
        return;
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    Vector3 position = ANBD_BOARD_LOCAL_POSITION;
    Vector3i rotation = ANBD_BOARD_ROTATION;
    scenApplyTransRot(&ANBD_BOARD_ORIGIN, &rotation, &position);
    EV_AT_SetPos(ANAN_BOARD, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    rotation = ANBD_BOARD_ROTATION;
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 29))
        rotation.z = static_cast<std::uint16_t>(rotation.z + 1092);
    EV_AT_SetAngY(ANAN_BOARD, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    HLib::Task *caller = work->caller;
    const std::uint64_t state = work->npcState;
    HLib::DestroyCurrentTask();
    HLib::ResolveDeferredCallback(caller);
    EV_NPC_NewLoadSw(static_cast<std::uint32_t>(state));
    if (!work->specialEntry)
        SetPlayerControlEnabled(1);
}
void AnanTrackRyo(void *data) {
    AnanWork *work = *static_cast<AnanWork **>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->lifecycle & 0x10000000U) {
        if (work->lifecycle & 0x40000000U) {
            mtwkUpdateWithPosition(scenFindMotionwork(ANAN_RYO), nullptr, 0);
            work->lifecycle &= ~0x40000000U;
        }
        work->ryoTask = nullptr;
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->targets[0].name == ANAN_INVALID.name) {
        if (work->lifecycle & 0x40000000U) {
            mtwkUpdateWithPosition(scenFindMotionwork(ANAN_RYO), nullptr, 0);
            work->lifecycle &= ~0x40000000U;
        }
        return;
    }
    Vector3 point;
    if (work->targets[0].name)
        mtwkGetBonePosition(scenFindMotionwork(work->targets[0]), MB_Head, &point);
    else
        point = work->positions[0];
    mtwkUpdateWithPosition(scenFindMotionwork(ANAN_RYO), &point, static_cast<std::uint16_t>(work->positionModes[0]));
    const std::uint16_t operation = work->transitionFrames[0] ? static_cast<std::uint16_t>((work->transitionFrames[0] << 8) + 4) : 3;
    mtwkUpdateWithPosition(scenFindMotionwork(ANAN_RYO), nullptr, operation);
    work->lifecycle |= 0x40000000U;
}
void AnanTrackMan(void *data) {
    AnanWork *work = *static_cast<AnanWork **>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->lifecycle & 0x20000000U) {
        if (work->lifecycle & 0x80000000U) {
            mtwkUpdateWithPosition(scenFindMotionwork(ANAN_MAN), nullptr, 0);
            work->lifecycle &= ~0x80000000U;
        }
        work->ananTask = nullptr;
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->targets[1].name == ANAN_INVALID.name) {
        if (work->lifecycle & 0x80000000U) {
            mtwkUpdateWithPosition(scenFindMotionwork(ANAN_MAN), nullptr, 0);
            work->lifecycle &= ~0x80000000U;
        }
        return;
    }
    Vector3 point;
    if (work->targets[1].name)
        mtwkGetBonePosition(scenFindMotionwork(work->targets[1]), MB_Head, &point);
    else
        point = work->positions[1];
    mtwkUpdateWithPosition(scenFindMotionwork(ANAN_MAN), &point, static_cast<std::uint16_t>(work->positionModes[1]));
    const std::uint16_t operation = work->transitionFrames[1] ? static_cast<std::uint16_t>((work->transitionFrames[1] << 8) + 4) : 3;
    mtwkUpdateWithPosition(scenFindMotionwork(ANAN_MAN), nullptr, operation);
    work->lifecycle |= 0x80000000U;
}
const ScriptAreaDefinition ananScriptDefinition{.areaId = ANAN_AREA, .field04 = 0, .init = InitializeAnan, .main = RunAnan};
} // namespace shenmue::s2::areas
#endif
