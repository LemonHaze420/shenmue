#include "s2/areas/wbbk_scenario.hpp"
#include "s2/areas/wbbk_motion.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>

namespace shenmue::s2::areas {
BookScenarioData bookScenarioData;
void HideBookScenarioActor(shen_name actor) {
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(actor, false);
    EV_SetCharaControllerFlags(actor, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetRequestedRootMode(scenFindMotionwork(actor), 0);
    SetMotionRegionFlag(scenFindMotionwork(actor), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(actor, EventTransformFlags::AllAxes, {.x = -1000, .y = -1000, .z = -1000});
}
void QueueBookScenarioActors(const char *authorBank, const shen_name *actors, HLib::TaskCallback continuation) {
    bookScenarioData.continuation = continuation;
    bookScenarioData.pendingResources = 0;
    bookScenarioData.package[0] = 0;
    if (authorBank)
        std::strcpy(bookScenarioData.authorBank, authorBank);
    else
        bookScenarioData.authorBank[0] = 0;
    shen_name *destination = bookScenarioData.actors;
    if (actors)
        while (actors->name)
            *destination++ = *actors++;
    *destination = shen_name{};
    if (bookScenarioData.authorBank[0])
        EV_FreeAuthorPak(1, bookScenarioData.authorBank);
    for (std::uint32_t i = 0; bookScenarioData.actors[i].name; ++i) {
        bookScenarioData.pendingResources |= 2U << (i & 31U);
        EV_CheckWarmStartNpcRoute(bookScenarioData.actors[i]);
    }
    if (bookScenarioData.package[0]) {
        bookScenarioData.pendingResources |= 1U;
        HLib::SetCurrentTaskTo(BookScenarioPackageRetryCallback);
    } else
        HLib::SetCurrentTaskTo(BookScenarioPackageWaitCallback);
}
void BookScenarioPackageRetryCallback(void *) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(bookScenarioData.package)))
        HLib::SetCurrentTaskTo(BookScenarioPackageWaitCallback);
}
void BookScenarioPackageWaitCallback(void *) {
    if ((bookScenarioData.pendingResources & 1U) && !EV_ControlPackageResources(PakCategory::Package, PakPackageOp::LoadAfs, 0, 0))
        bookScenarioData.pendingResources &= ~1U;
    for (std::uint32_t i = 0; bookScenarioData.actors[i].name; ++i) {
        const std::uint32_t mask = 2U << (i & 31U);
        if ((bookScenarioData.pendingResources & mask) && EV_ForceLoadNpc(bookScenarioData.actors[i]))
            bookScenarioData.pendingResources &= ~mask;
    }
    if (bookScenarioData.pendingResources)
        return;
    for (const shen_name *actor = bookScenarioData.actors; actor->name; ++actor)
        HideBookScenarioActor(*actor);
    HideBookScenarioActor(BOOK_RYO);
    HLib::SetCurrentTaskTo(bookScenarioData.continuation);
}
void BookScenarioAuthorWaitCallback(void *) {
    const std::int32_t fadeFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 31;
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == fadeFrame) {
        if (bookScenarioData.flags & 2U)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (bookScenarioData.flags & 0x100U)
            spmFadeOut();
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    for (const shen_name *actor = bookScenarioData.actors; actor->name; ++actor)
        HideBookScenarioActor(*actor);
    if (bookScenarioData.flags & 0x20U)
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if (!(bookScenarioData.flags & 0x40U))
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    HLib::SetCurrentTaskTo(bookScenarioData.continuation);
}
} // namespace shenmue::s2::areas
