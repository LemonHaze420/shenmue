#include "s2/areas/area_pvsj.hpp"
#include "s2/areas/pati_event.hpp"
#include "s2/camera.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2 {
PvsjBoardPosition pvsjBoardPositions[32];
std::int32_t pvsjBoardCount, pvsjCustomerModelIndex, pvsjPlayerCamera, pvsjCustomerCamera, pvsjSkipMotion;
std::int16_t pvsjCameraBlend;
Vector3i pvsjSavedPlayerRotation;
Vector3 pvsjFilteredEye, pvsjFilteredTarget, pvsjDesiredEye, pvsjDesiredTarget;
char pvsjGameArgument[24];

void AreaPVSJInit() {
    pvsjBoardCount = 0;
    std::memset(pvsjBoardPositions, 0, sizeof(pvsjBoardPositions));
    pvsjCustomerModelIndex = 0;
    std::memset(pvsjGameArgument, 0, sizeof(pvsjGameArgument));
    pvsjFilteredEye = {};
    pvsjFilteredTarget = {};
    pvsjDesiredEye = {};
    pvsjDesiredTarget = {};
    pvsjSavedPlayerRotation = {};
    for (auto &entry : g_patiMotionCache)
        entry = {};
    pvsjPlayerCamera = 0;
    pvsjCustomerCamera = 0;
    pvsjCameraBlend = 0;
    g_patiCameraTime = 0;
    pvsjSkipMotion = 0;
}

void AreaPVSJMain(std::uintptr_t argument) {
    using enum CameraState;
    auto *task = HLib::EnqueueTaskWithParameter(PvsjMainCallback, 5, 11, sizeof(PvsjEventWork), PVSJ_TASK);
    auto *work = static_cast<PvsjEventWork *>(HLib::GetTaskParameter(task));
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    std::memcpy(&work->argument, reinterpret_cast<const void *>(argument), 4);
    work->soundScene = GetSelectedSoundSceneIndex();
    std::strcpy(work->sbt, shenGetCurrentSbt());
    std::strcpy(work->bgm, GetCurrentBgm());
    work->previousCash = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash));
    work->state = 0;
    work->sessionCash = 300;
    work->jobPartition = reinterpret_cast<PakAfsPartition *>(~std::uintptr_t{});
    work->customerPartition = -1;
    work->firstCustomer = 1;
    work->gamePartition = reinterpret_cast<PakAfsPartition *>(~std::uintptr_t{});
    GetScenarioCharacterEulerAngles(PVSJ_PLAYER, &pvsjSavedPlayerRotation, EventTransformFlags::World);
    g_patiMotionCache[0].character = {};
    g_patiMotionCache[0].age = 0;
    g_patiMotionCache[1].character = {};
    g_patiMotionCache[1].age = 1;
    g_patiMotionCache[2].character = {};
    g_patiMotionCache[2].age = 2;
    g_patiMotionCache[3].age = 3;
    pvsjSkipMotion = 0;
    auto *hint = HLib::EnqueueTaskWithParameter(PvsjHintCallback, 5, 11, sizeof(PvsjEventWork *), PVSJ_TASK);
    work->hintTask = hint;
    *static_cast<PvsjEventWork **>(HLib::GetTaskParameter(hint)) = work;
    work->greetingIndex = shenGetRandomInt32(15);
    work->voiceLoadBuffer = HLib::AllocateTaskData(0x4000, PVSJ_AREA);
    work->voiceLengths = static_cast<PatiVoiceLength *>(HLib::AllocateTaskData(0x4000, PVSJ_AREA));
    SetPlayerControlEnabled(0);
    const auto directory = reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, directory, reinterpret_cast<std::uintptr_t>("PVSJ"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, directory, reinterpret_cast<std::uintptr_t>("PATI"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    work->jobPartition = reinterpret_cast<PakAfsPartition *>(
        EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Mount, directory, reinterpret_cast<std::uintptr_t>("PVSJ.afs")));
    work->gamePartition = reinterpret_cast<PakAfsPartition *>(
        EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Mount, directory, reinterpret_cast<std::uintptr_t>("PATI.afs")));
    shenLoadEventFrom("PATI.EV3", EventFunctionType::Event3);
    PvsjInitializeBoards(work);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    camCtrlSetState(Init);
    EV_EventSetUpHelpIcon_0(145, 0);
    HLib::SetupTaskDelay(0);
}
const ScriptAreaDefinition areaPVSJScriptDefinition{PVSJ_AREA, 0, AreaPVSJInit, AreaPVSJMain};
} // namespace shenmue::s2
