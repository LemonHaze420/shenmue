#include "s2/sap_event.hpp"
#include "s2/scene_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>

namespace shenmue::s2 {
SapEventData sapEventData{};

// @verified - p
void InitializeSapAuthorEvent(const char *authorBank, std::uintptr_t, const shen_name *characters, HLib::TaskCallback next) {
    sapEventData.nextCallback = next;
    sapEventData.pendingMask = 0;
    sapEventData.packageName[0] = 0;
    char *bank = sapEventData.authorBank;
    if (authorBank)
        while (*authorBank)
            *bank++ = *authorBank++;
    *bank = 0;
    shen_name *destination = sapEventData.characters;
    if (characters)
        while (characters->name)
            *destination++ = *characters++;
    *destination = shen_name{};
    if (sapEventData.authorBank[0])
        (void)EV_FreeAuthorPak(1, sapEventData.authorBank);
    for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index) {
        sapEventData.pendingMask |= 2U << (index & 31U);
        (void)EV_CheckWarmStartNpcRoute(sapEventData.characters[index]);
    }
    std::uint64_t packageWord;
    std::memcpy(&packageWord, sapEventData.packageName, sizeof(packageWord));
    if (packageWord) {
        sapEventData.pendingMask |= 1;
        HLib::SetCurrentTaskTo(SapPackageLoadCallback);
    } else {
        HLib::SetCurrentTaskTo(SapPackageWaitCallback);
    }
}

// @verified - p
void SapPackageLoadCallback(void *) {
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    if (EV_ControlPackageResources(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(sapEventData.packageName)))
        HLib::SetCurrentTaskTo(SapPackageWaitCallback);
}

// @verified - p
void SapPackageWaitCallback(void *) {
    if ((sapEventData.pendingMask & 1U) && !EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0))
        sapEventData.pendingMask &= ~1U;
    for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index) {
        const std::uint32_t mask = 2U << (index & 31U);
        if ((sapEventData.pendingMask & mask) && EV_ForceLoadNpc(sapEventData.characters[index]))
            sapEventData.pendingMask &= ~mask;
    }
    if (!sapEventData.pendingMask) {
        for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index)
            ResetSapAuthorCharacter(sapEventData.characters[index]);
        ResetSapAuthorCharacter(SAP_PLAYER);
        HLib::SetCurrentTaskTo(sapEventData.nextCallback);
    }
}

// @verified - p
void ResetSapAuthorCharacter(shen_name character) {
    constexpr Vector3 hiddenPosition{-1000.0F, -1000.0F, -1000.0F};
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(character, false);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    (void)mtwkSetRequestedRootMode(scenFindMotionwork(character), 0);
    (void)SetMotionRegionFlag(scenFindMotionwork(character), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, hiddenPosition);
}

// @verified - p
void SapAuthorWaitCallback(void *) {
    const std::int32_t endFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 31;
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == static_cast<std::uint32_t>(endFrame)) {
        if (sapEventData.authorFlags & 2U)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (sapEventData.authorFlags & 0x100U)
            spmFadeOut();
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index)
            ResetSapAuthorCharacter(sapEventData.characters[index]);
        if (sapEventData.authorFlags & 0x20U)
            (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (!(sapEventData.authorFlags & 0x40U))
            (void)EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        HLib::SetCurrentTaskTo(sapEventData.nextCallback);
    }
}
} // namespace shenmue::s2
