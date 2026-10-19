#include "s2/areas/area_pvsj.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/help.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/map_control.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {

void PvsjBeginGame(PvsjEventWork *work) {
    if (DoesScenarioCharacterExist(work->owner)) {
        UpdateMotionWorkWithPosition(scenFindMotionwork(work->owner), nullptr, 0);
        EV_SetCharacterAlphaEnabled(work->owner, true);
    }
    SetMotionRegionFlag(scenFindMotionwork(PVSJ_PLAYER), 71, 0, MotionRegion::LowerBody);
    shenLoadEventFrom("PATI.EV3", EventFunctionType::Event3);
    ExecuteCurrAreaMainFunc("PATI.EV3", reinterpret_cast<std::uintptr_t>(pvsjGameArgument));
    HLib::SetupTaskDelay(0);
}

void PvsjFinish(PvsjEventWork *work) {
    auto *const invalid = reinterpret_cast<PakAfsPartition *>(~std::uintptr_t{});
    if (work->gamePartition != invalid) {
        EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Unmount, reinterpret_cast<std::uintptr_t>(work->gamePartition), 0);
        work->gamePartition = invalid;
    }
    HLib::FreeTaskData(work->voiceLoadBuffer);
    HLib::FreeTaskData(work->voiceLengths);
    if (work->jobPartition != invalid) {
        if (work->customerPartition != -1) {
            const auto mode = EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0) ? 13 : 12;
            EV_ControlPackageResources(PakCategory::Package, mode, reinterpret_cast<std::uintptr_t>(work->jobPartition),
                static_cast<std::uintptr_t>(PVSJ_CUSTOMER_PARTITIONS[work->customerPartition].models),
                static_cast<std::uint32_t>(PVSJ_CUSTOMER_PARTITIONS[work->customerPartition].hands));
            work->customerPartition = -1;
        }
        EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Unmount, reinterpret_cast<std::uintptr_t>(work->jobPartition), 0);
        work->jobPartition = invalid;
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("PVSJ"), 0);
    PvsjReleaseVoice(work);
    EV_LoadCutsceneFromCurrentScene(work->bgm);
    EV_LoadAudio(work->sbt, work->soundScene);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_NPC_NewLoadSw(1);
    EV_SetRefBoardParamsFor(PVSJ_PLAYER, 0);
    auto *player = FindCharacterByScenarioName(PVSJ_PLAYER);
    SetHighModelAndFaceFlagsForChara(player, FacialExpressionId::Normal, 10, 1);
    HasCharacterMotionWork(player);
    GetCharacterMotionCompatibilityResult(player);
    SetHighModelAndFaceFlagsForChara(player, FacialExpressionId::Normal, 10, 1);
    EV_EnableCharacterDisplay(PVSJ_PLAYER);
    if (DoesScenarioCharacterExist(PATI_BACKPACK) == 1)
        EV_EnableCharacterDisplay(PATI_BACKPACK);
    EV_SetCharaControllerFlags(PVSJ_PLAYER, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(PVSJ_PLAYER, 3, NpcCharacterControllerState::Enabled);
    SetPlayerStat(PlayerStatsId::Cash, static_cast<std::uint32_t>(work->previousCash));
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    HLib::DestroyCurrentTask();
    shen_name area;
    if (g_currentMap.name == SHEN_NAME(AR09))
        area = SHEN_NAME(AR03);
    else if (g_currentMap.name == SHEN_NAME(QT09))
        area = SHEN_NAME(QTB3);
    else
        area = shen_name{(g_currentMap.name & 0x00ffffffU) | (static_cast<std::uint32_t>('0') << 24)};
    EV_scenQueueLoad(g_actNumber, area, g_entryPoint + 1);
    EVS_LoopCamera(0, shen_name{}, shen_name{});
}
} // namespace shenmue::s2
