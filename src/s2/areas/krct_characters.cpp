#include "s2/areas/area_krct.hpp"
#include "s2/areas/krct_tables.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
namespace shenmue::s2::areas {
namespace {
std::array<std::int32_t, 60> restHandPose = krctRestHandPose;
void RestoreMotion(shen_name name) {
    mtwkSetExtraFlag(scenFindMotionwork(name), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(name), 13, 0);
}
} // namespace
void KrctRestoreHighModels(void *) {
    Character *character = FindCharacterByScenarioName(KRCT_RYO);
    SetHighModelXYZ(character, 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(KRCT_RYO), 1, 0, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(KRCT_RYO), 1, reinterpret_cast<std::byte *>(restHandPose.data()), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(KRCT_RYO), 0, reinterpret_cast<std::byte *>(restHandPose.data()), 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(KRCT_RYO), FacialNodeReplacement);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(KRCT_RYO), 0, 1, 1);
    SetHighModelXYZ(FindCharacterByScenarioName(KRCT_SHENHUA), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(KRCT_SHENHUA), 1, 0, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(KRCT_SHENHUA), FacialNodeReplacement);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(KRCT_SHENHUA), 0, 1, 1);
    HLib::DestroyCurrentTask();
}
void KrctConfigureRestoredParty() {
    mtwkRequestMotion(scenFindMotionwork(KRCT_RYO), 0xf085, MotionRegion::LowerBody);
    EV_AT_SetPos(KRCT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {150.2f, 100, 401.2f});
    EV_AT_SetAngY(KRCT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 32403, 0});
    RestoreMotion(KRCT_RYO);
    mtwkRequestMotion(scenFindMotionwork(KRCT_SHENHUA), 0x9503, MotionRegion::LowerBody);
    EV_AT_SetPos(KRCT_SHENHUA, EventTransformFlags::AllAxes, {150, 100, 404});
    EV_AT_SetAngY(KRCT_SHENHUA, EventTransformFlags::AllAxes, {0, 36864, 0});
    RestoreMotion(KRCT_SHENHUA);
    shenToggleAreaFlags(3);
    SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(64), reinterpret_cast<void *>(1));
}
void KrctConfigureSeatedParty() {
    SetPlayerControlEnabled(0);
    for (shen_name name : {KRCT_RYO, KRCT_SHENHUA}) {
        EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        const bool alternate = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 680) != 0;
        if (alternate)
            EV_authSceneCtrl(AuthSceneCtrl::ApplyYangleOffset);
        else if (name.name == KRCT_RYO.name) {
            EV_AT_SetPos(name, EventTransformFlags::AllAxes, {155.9f, 100, 391.6f});
            EV_AT_SetAngY(name, EventTransformFlags::AllAxes, {0, 22755, 0});
        } else {
            EV_AT_SetPos(name, EventTransformFlags::AllAxes, {154.9f, 100, 393.5f});
            EV_AT_SetAngY(name, EventTransformFlags::AllAxes, {-1, -1, -1});
        }
        RestoreMotion(name);
        mtwkRequestMotion(scenFindMotionwork(name),
            static_cast<std::uint16_t>(name.name == KRCT_RYO.name ? (alternate ? 0x1009 : 0x1006) : (alternate ? 0x100a : 0x1008)), MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(name), 1, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(name), 3, 2);
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
        EV_SetCharacterAlphaEnabled(name, false);
        SetFaceFlagsForChara(FindCharacterByScenarioName(name), 1);
    }
    clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 13);
}
} // namespace shenmue::s2::areas
