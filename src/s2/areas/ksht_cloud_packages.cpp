#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2::areas {
namespace {
void FreeCloudBanks(std::int32_t operation) {
    EV_FreeAuthorPak(operation, "0509");
    EV_FreeAuthorPak(operation, "0510");
    EV_FreeAuthorPak(operation, "0511");
}
void LoadMap(const char *name) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(name));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
void FreeMap(const char *name) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
}
void RestoreMapObjects() {
    for (std::int32_t i = 0; i < 4; ++i)
        EV_SetObjectFlagByIndex(i, 1);
    EV_SetObjectFlagByIndex(4, 0);
    EV_SetObjectFlagByIndex(5, 1);
}
} // namespace
void SelectKshtCloudPackage(std::int32_t selection) {
    switch (selection) {
    case 0:
    case 3:
    case 4:
    case 5:
    case 6:
        kshtAuthorController.state = 2;
        break;
    case 1:
    case 7:
        if (selection == 1)
            EV_SetCinemaBarVisibility(FilmControlFlag::Show, 1);
        FreeCloudBanks(0);
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Disabled);
        for (std::int32_t i = 0; i < 6; ++i)
            EV_SetObjectFlagByIndex(i, 0);
        FreeKshtCharacters(kshtCompanionList);
        FreeMap("MPK01");
        LoadMap(selection == 1 ? "MAP_KSH3" : "MAP_KSH5");
        Queue_EVT_UTL_LoadNPCChara(selection == 1 ? kshtParentList : kshtGuestList, kshtAuthorController.task);
        EV_SetObjectFlagByIndex(0, 1);
        if (selection == 1)
            for (std::int32_t i = 1; i < 5; ++i)
                EV_SetObjectFlagByIndex(i, 1);
        FreeCloudBanks(1);
        break;
    case 2:
        EV_SetCinemaBarVisibility(FilmControlFlag::Show, 1);
        FreeCloudBanks(0);
        for (std::int32_t i = 0; i < 5; ++i)
            EV_SetObjectFlagByIndex(i, 0);
        FreeKshtCharacters(kshtParentList);
        FreeMap("MAP_KSH3");
        LoadMap("MPK01");
        Queue_EVT_UTL_LoadNPCChara(kshtCompanionAndChildList, kshtAuthorController.task);
        RestoreMapObjects();
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Enabled);
        FreeCloudBanks(1);
        break;
    case 102:
        FreeCloudBanks(0);
        EV_SetObjectFlagByIndex(0, 0);
        scenCvsCtrlFree(GetConverseDataForChara(SHEN_NAME(IWA_)));
        scenCvsCtrlFree(GetConverseDataForChara(SHEN_NAME(LRY_)));
        FreeKshtCharacters(kshtGuestList);
        FreeMap("MAP_KSH5");
        LoadMap("MPK01");
        Queue_EVT_UTL_LoadNPCChara(kshtCompanionList, kshtAuthorController.task);
        RestoreMapObjects();
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Enabled);
        FreeCloudBanks(1);
        break;
    default:
        break;
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("MPK00"), 0);
}
} // namespace shenmue::s2::areas
