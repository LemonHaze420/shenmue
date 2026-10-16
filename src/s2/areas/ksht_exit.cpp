#include "s2/areas/ksht_authored.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2::areas {
namespace {
void RestoreConversationActor(shen_name actor) {
    EV_SetCharaControllerFlags(actor, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(actor, 3, NpcCharacterControllerState::Enabled);
    AT_SetLocalBaseChara(actor, KSHT_INVALID);
    if (mtwkSetExtraFlag(scenFindMotionwork(actor), 59, 1))
        UpdateMotionWorkWithPosition(scenFindMotionwork(actor), nullptr, 0);
    FindMotionWork(FindCharacterByScenarioName(actor));
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(actor), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(actor), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(actor), 0, 1, 0);
}
} // namespace
void KshtWaitConversationMotion(void *) {
    if (SetMotionRegionFlag(scenFindMotionwork(KSHT_RYO), 65, 1, MotionRegion::LowerBody)) {
        HLib::SetCurrentTaskTo(KshtExitCallback);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(30);
    }
}
void KshtExitCallback(void *) {
    RestoreConversationActor(KSHT_RYO);
    RestoreConversationActor(KSHT_SIN);
    FreeKshtCharacters(kshtCompanionList);
    EV_npcLockUnlock(SHEN_NAME(M01_));
    EV_FreeAuthorPak(0, "0521");
    EV_NPC_NewLoadSw(1);
    ResetKshtCamera();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(KshtExitWaitCallback);
}
void KshtExitWaitCallback(void *) {
    SetRenderClockAndCompassTo(0);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
    camCtrlResetClipDistances();
    camCtrlRestoreCachedTransform();
}
void KshtSimpleExitCallback(void *) {
    RestoreConversationActor(KSHT_RYO);
    RestoreConversationActor(KSHT_SIN);
    EV_npcLockUnlock(KSHT_SIN);
    EV_npcLockUnlock(SHEN_NAME(M01_));
    EV_FreeAuthorPak(0, "0521");
    ResetKshtCamera();
    HLib::SetCurrentTaskTo(KshtExitWaitCallback);
}
} // namespace shenmue::s2::areas
