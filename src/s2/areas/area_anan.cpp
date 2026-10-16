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
    if (!EV_ControlPackageResources(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(ANBD_BOARD_PACKAGE)))
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    Vector3 position = ANBD_BOARD_LOCAL_POSITION;
    Vector3i rotation = ANBD_BOARD_ROTATION;
    ApplyScenarioTranslationAndRotation(&ANBD_BOARD_ORIGIN, &rotation, &position);
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
            UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_RYO), nullptr, 0);
            work->lifecycle &= ~0x40000000U;
        }
        work->ryoTask = nullptr;
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->targets[0].name == ANAN_INVALID.name) {
        if (work->lifecycle & 0x40000000U) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_RYO), nullptr, 0);
            work->lifecycle &= ~0x40000000U;
        }
        return;
    }
    Vector3 point;
    if (work->targets[0].name)
        GetMotionBonePosition(scenFindMotionwork(work->targets[0]), 5, &point);
    else
        point = work->positions[0];
    UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_RYO), &point, static_cast<std::uint16_t>(work->positionModes[0]));
    const std::uint16_t operation = work->transitionFrames[0] ? static_cast<std::uint16_t>((work->transitionFrames[0] << 8) + 4) : 3;
    UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_RYO), nullptr, operation);
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
            UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_MAN), nullptr, 0);
            work->lifecycle &= ~0x80000000U;
        }
        work->ananTask = nullptr;
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->targets[1].name == ANAN_INVALID.name) {
        if (work->lifecycle & 0x80000000U) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_MAN), nullptr, 0);
            work->lifecycle &= ~0x80000000U;
        }
        return;
    }
    Vector3 point;
    if (work->targets[1].name)
        GetMotionBonePosition(scenFindMotionwork(work->targets[1]), 5, &point);
    else
        point = work->positions[1];
    UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_MAN), &point, static_cast<std::uint16_t>(work->positionModes[1]));
    const std::uint16_t operation = work->transitionFrames[1] ? static_cast<std::uint16_t>((work->transitionFrames[1] << 8) + 4) : 3;
    UpdateMotionWorkWithPosition(scenFindMotionwork(ANAN_MAN), nullptr, operation);
    work->lifecycle |= 0x80000000U;
}
const ScriptAreaDefinition ananScriptDefinition{.areaId = ANAN_AREA, .field04 = 0, .init = InitializeAnan, .main = RunAnan};
} // namespace shenmue::s2::areas
