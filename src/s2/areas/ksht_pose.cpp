#include "s2/areas/area_ksht.hpp"
#include "s2/areas/ksht_poses.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/model_component.hpp"
namespace shenmue::s2::areas {
namespace {
struct KshtEyeRequest {
    std::int32_t cleanup;
    shen_name character;
    float frames, right, left;
    std::uint32_t reserved14;
};
static_assert(sizeof(KshtEyeRequest) == 0x18);
void KshtEyeCallback(void *data) {
    KshtEyeRequest *request = static_cast<KshtEyeRequest *>(data);
    AT_SetEyeCtrl(FindCharacterByScenarioName(request->character), request->frames, request->left, request->right);
    if (request->cleanup)
        HLib::DestroyCurrentTask();
}
void SetKshtActorPose(shen_name actor, const Vector3 &position, const Vector3i &rotation, std::uint32_t animation, float loopTime, bool companion) {
    EV_SetCharaControllerFlags(actor, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    AT_SetLocalBaseChara(actor, KSHT_DUMMY);
    EV_AT_SetPos(actor, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, rotation);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(actor), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(actor), static_cast<std::uint16_t>(animation), MotionRegion::LowerBody);
    if (companion)
        SetMotionLayerLoopStartTime(scenFindMotionwork(actor), loopTime, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(actor), 1, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 3, 2);
    MotionWork *motion = scenFindMotionwork(actor);
    if (motion)
        motion->runtime.callbackFlags |= 1;
    if (!companion)
        mtwkSetExtraFlag(scenFindMotionwork(actor), 6, 2);
    FindMotionWork(FindCharacterByScenarioName(actor));
    FindMotionWork(FindCharacterByScenarioName(actor));
    SetFaceFlagsForChara(FindCharacterByScenarioName(actor), 0);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(actor), 0, 0, 1);
    SetHighModelXYZ(FindCharacterByScenarioName(actor), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(actor), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(actor), 0, reinterpret_cast<std::byte *>(kshtDefaultHand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(actor), 1, reinterpret_cast<std::byte *>(kshtDefaultHand), 0);
}
} // namespace
void QueueKshtWaitEye(float frames, float left, float right, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(KshtEyeCallback, 6, 0x0b, sizeof(KshtEyeRequest), SHEN_NAME(EVNT));
    HLib::SetupTimedTask(task, delay);
    KshtEyeRequest *request = static_cast<KshtEyeRequest *>(HLib::GetTaskParameter(task));
    request->frames = frames;
    request->right = left;
    request->left = right;
    request->cleanup = 1;
    request->character = KSHT_SIN;
}
void SetupKshtConversationPose(bool companion) {
    const KshtPoseDefinition &pose = kshtPoses[kshtSelection];
    EV_AT_SetPos(KSHT_DUMMY, EventTransformFlags::World | EventTransformFlags::AllAxes, pose.anchorPosition);
    EV_AT_SetAngY(KSHT_DUMMY, EventTransformFlags::World | EventTransformFlags::AllAxes, pose.anchorRotation);
    SetKshtActorPose(KSHT_RYO, pose.playerPosition, pose.playerRotation, pose.playerMotion, pose.playerTime, companion);
    if (companion) {
        SetKshtActorPose(KSHT_SIN, pose.companionPosition, pose.companionRotation, pose.companionMotion, pose.companionTime, true);
        EVS_LoopCamera(kshtCameraIds[kshtSelection], KSHT_DUMMY, KSHT_DUMMY);
    }
}
} // namespace shenmue::s2::areas
