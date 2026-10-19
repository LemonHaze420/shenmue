#include "s2/areas/ksht_authored.hpp"
#include "s2/areas/ksht_hands.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
namespace shenmue::s2::areas {
namespace {
void FreeTalkMotion(shen_name actor, std::uint16_t motion) {
    ResetKshtMotion(actor);
    mtwkRequestMotion(scenFindMotionwork(actor), motion, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(actor), 1, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 3, 2);
    MotionWork *work = scenFindMotionwork(actor);
    if (work)
        work->runtime.callbackFlags |= 1;
    SetMotionRegionFlag(scenFindMotionwork(actor), 66, 0, MotionRegion::LowerBody);
    EV_SetCharaControllerFlags(actor, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 6, 2);
    EV_SetCharacterAlphaEnabled(actor, false);
    SetFaceFlagsForChara(FindCharacterByScenarioName(actor), 1);
}
void SetHand(shen_name actor, std::int32_t side, std::int32_t (*commands)[3]) {
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(actor), side, reinterpret_cast<std::byte *>(commands), 0);
}
} // namespace
void PrepareKshtFreeTalkActors() {
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(KSHT_RYO, 3, NpcCharacterControllerState::Disabled);
    EV_AT_SetPos(KSHT_RYO, EventTransformFlags::AllAxes, {145.6905059814453f, 100.4520034790039f, 205.2657012939453f});
    EV_AT_SetAngY(KSHT_RYO, EventTransformFlags::AllAxes, {0, 2251, 0});
    FreeTalkMotion(KSHT_RYO, 0x2001);
    SetFixoDataForMotionWork(FindCharacterByScenarioName(SHEN_NAME(a726)), FindCharacterByScenarioName(KSHT_RYO), 14,
        {0.11400000005960464f, -0.012000000104308128f, -0.004000000189989805f}, {1547, 0, 0});
    EV_EnableCharacterDisplay(SHEN_NAME(a726));
    SetHand(KSHT_RYO, 0, kshtHighRightHand);
    UpdateMotionWorkWithPosition(scenFindMotionwork(KSHT_RYO), nullptr, 3);
    UpdateMotionWorkWithPosition(scenFindMotionwork(KSHT_RYO), nullptr, 0);
    EV_SetCharaControllerFlags(KSHT_SIN, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(KSHT_SIN, 3, NpcCharacterControllerState::Disabled);
    EV_AT_SetPos(KSHT_SIN, EventTransformFlags::AllAxes, {145.61920166015625f, 100.4302978515625f, 203.05679321289062f});
    EV_AT_SetAngY(KSHT_SIN, EventTransformFlags::AllAxes, {0, 63442, 0});
    clmdUtil(FindCharacterByScenarioName(KSHT_SIN), 14);
    FreeTalkMotion(KSHT_SIN, 0x2002);
    SetFixoDataForMotionWork(FindCharacterByScenarioName(SHEN_NAME(a728)), FindCharacterByScenarioName(KSHT_SIN), 14,
        {0.06800000369548798f, -0.014999999664723873f, -0.020999999716877937f}, {1091, 0, 62442});
    EV_EnableCharacterDisplay(SHEN_NAME(a728));
    SetHand(KSHT_SIN, 0, kshtHighRightHand);
}
void KshtSetHighHands(void *) {
    for (shen_name actor : {KSHT_RYO, KSHT_SIN}) {
        SetHighModelXYZ(FindCharacterByScenarioName(actor), 1, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(actor), 1, 0, 0);
        SetHand(actor, 1, kshtHighLeftHand);
        SetHand(actor, 0, kshtHighRightHand);
    }
    HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2::areas
