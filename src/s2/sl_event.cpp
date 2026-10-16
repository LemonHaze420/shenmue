#include "s2/sl_event.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2 {
shen_name slCurrentAuthor{};
SlEventData slEventData{};
// @verified - p
void SlPackageWaitCallback(void *) {
    if ((slEventData.pendingMask & 1U) && !EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0))
        slEventData.pendingMask &= ~1U;
    for (std::uint32_t index = 0; slEventData.characters[index].name; ++index) {
        const std::uint32_t mask = 2U << (index & 31U);
        if ((slEventData.characters[index].name & 0xff000000U) == 0x5f000000U && (slEventData.pendingMask & mask) &&
            EV_ForceLoadNpc(slEventData.characters[index]))
            slEventData.pendingMask &= ~mask;
    }
    if (!slEventData.pendingMask) {
        for (std::uint32_t index = 0; slEventData.characters[index].name; ++index)
            ResetSlAuthorCharacter(slEventData.characters[index]);
        ResetSlAuthorCharacter(SL_PLAYER);
        HLib::SetCurrentTaskTo(slEventData.nextCallback);
    }
}
// @verified - p
void ResetSlAuthorCharacter(shen_name character) {
    constexpr Vector3 hiddenPosition{-1000.0F, -1000.0F, -1000.0F};
    if ((character.name & 0xff000000U) == 0x5f000000U) {
        EV_SetCharacterAlphaEnabled(character, false);
        (void)mtwkSetRequestedRootMode(scenFindMotionwork(character), 0);
        (void)SetMotionRegionFlag(scenFindMotionwork(character), 71, 0, MotionRegion::LowerBody);
    }
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, hiddenPosition);
}
// @verified - p
void SlAuthorWaitCallback(void *) {
    const std::uint32_t endFrame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 31U;
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == endFrame && (slEventData.authorFlags & 2U))
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (GetAuthorFlags() && slCurrentAuthor.name == SL_BAD_END_AUTHOR.name)
            spmFadeOut();
        for (std::uint32_t index = 0; slEventData.characters[index].name; ++index)
            ResetSlAuthorCharacter(slEventData.characters[index]);
        (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (slEventData.authorFlags & 0x100U)
            SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        if (!(slEventData.authorFlags & 0x40U))
            (void)EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        HLib::SetCurrentTaskTo(slEventData.nextCallback);
    }
}
} // namespace shenmue::s2
