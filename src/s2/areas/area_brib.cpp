#include "s2/areas/area_brib.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/qte_hand_poses.hpp"
namespace shenmue::s2::areas {
namespace {
void BribRenGetNpcs(void *);
void BribRenGetPackage(void *);
void BribRenStart(void *);
void BribRenLoop(void *);
void BribSenseGetNpcs(void *);
void BribSenseGetPackage(void *);
void BribSenseStart(void *);
void BribSenseLoop(void *);
void QueueBribRen() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BribRenGetNpcs, 4, 0x0b, sizeof(BribWork), AREA_EVENT_TASK);
    if (!task)
        return;
    BribWork *work = static_cast<BribWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    EV_FreeAuthorPak(1, "0234");
    work->parent = HLib::GetCurrentTask();
    work->flags = 0;
    HLib::SetupTaskDelay(-1);
    SetPlayerControlEnabled(0);
}
void QueueBribSense() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BribSenseGetNpcs, 4, 0x0b, sizeof(BribWork), AREA_EVENT_TASK);
    if (!task)
        return;
    BribWork *work = static_cast<BribWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    EV_FreeAuthorPak(1, "0234");
    work->parent = HLib::GetCurrentTask();
    work->flags = 0;
    HLib::SetupTaskDelay(-1);
    SetPlayerControlEnabled(0);
}
void BribRenGetNpcs(void *) {
    if (filmCheckFadeTrackColour())
        return;
    HLib::SetCurrentTaskTo(BribRenGetPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopNpcs(BRIB_REN_CHARACTERS, ResolveAreaLoopParent);
}
void BribRenGetPackage(void *) {
    HLib::SetCurrentTaskTo(BribRenStart);
    HLib::SetupTaskDelay(-1);
    EV_NPC_NewLoadSw(0);
    PrepareNpcAllFromArray();
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "BRIB");
}
std::int64_t BribRenHandCallback(std::int32_t phase, std::int64_t elapsed, HLib::Task *) {
    if (phase == 2 && static_cast<std::int32_t>(elapsed) == 80)
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(BRIB_REN), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 20);
    return 1;
}
std::int64_t BribKukMotionCallback(std::int32_t phase, std::int64_t elapsed, HLib::Task *) {
    if (phase == 1)
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
    else if (phase == 2) {
        switch (static_cast<std::int32_t>(elapsed)) {
        case 220:
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(BRIB_REN), FacialExpressionId::Difficulty, 10, 0.2f);
            break;
        case 340:
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(BRIB_REN), FacialExpressionId::Normal, 10, 0.4f);
            break;
        case 820:
            HasCharacterMotionWork(ctrlGetCharaByName(BRIB_REN));
            break;
        case 830:
            HasCharacterMotionWork(ctrlGetCharaByName(BRIB_REN));
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(BRIB_REN), FacialExpressionId::Smile, 15, 0.3f);
            break;
        default:
            break;
        }
    }
    return 1;
}
void BribTransformOffset(const Vector3 &position, const Vector3i &rotation, Vector3 &offset) {
    const Vector3 input = offset;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    if (rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    if (rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    if (rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    Matrix4x4::TransformPoint(input, offset);
    Matrix4x4::Pop(1);
}
void BribQueueLookAt(shen_name character, shen_name target, std::int32_t bone, std::int32_t delay, std::int32_t duration) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AreaLoopLookAtCallback, 4, 0x0b, sizeof(AreaLoopLookAtWork), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopLookAtWork *work = static_cast<AreaLoopLookAtWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->duration = duration;
    work->flags = 0;
    work->elapsed = 0;
    work->completion = nullptr;
    work->character = character;
    work->target = target;
    work->bone = bone;
    HLib::SetupTimedTask(task, delay);
}
void BribQueueAttachment(
    shen_name character, const Vector3 &position, const Vector3i &rotation, std::int32_t delay, std::int32_t duration, AreaLoopCompletion completion) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AreaLoopAttachmentCallback, 4, 0x0b, sizeof(AreaLoopAttachmentWork), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopAttachmentWork *work = static_cast<AreaLoopAttachmentWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->elapsed = 0;
    work->character = character;
    work->attachmentFlags = 14;
    work->target = BRIB_GREN;
    work->position = position;
    work->rotation = rotation;
    work->completion = completion;
    work->duration = duration;

    HLib::SetupTimedTask(task, delay);
}
void BribRenStart(void *data) {
    EV_NPC_NewLoadSw(1);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BRIB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BRIB"), 0);
    if (!data)
        return;
    HLib::SetCurrentTaskTo(BribRenLoop);
    HLib::SetupTaskDelay(914);
    const Vector3 scale{0.3f, 0.3f, 0.3f};
    SetMotionworkFlagForCharaById(BRIB_GREN, &scale);
    SetAreaLoopCharacterControl(BRIB_GREN, 1);
    SetAreaLoopCharacterControl(BRIB_RYO, 1);
    SetEyeToChara(ctrlGetCharaByName(BRIB_RYO), nullptr, 16, 0);
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(BRIB_RYO));
    SetLowQualityHands(ctrlGetCharaByName(BRIB_RYO));
    SetHighModelXYZ(ctrlGetCharaByName(BRIB_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(BRIB_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(BRIB_RYO), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 1);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(BRIB_RYO), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 1);
    for (const shen_name *name = BRIB_REN_CHARACTERS; name->name; ++name) {
        GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(*name));
        SetLowQualityHands(ctrlGetCharaByName(*name));
        SetHighModelXYZ(ctrlGetCharaByName(*name), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(*name), 1, 1, 0);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(*name), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 1);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(*name), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 1);
    }
    QueueAreaLoopMotion({.parent = nullptr,
                            .character = BRIB_RYO,
                            .flags = 22,
                            .motion = 28679,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = -1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = nullptr,
                            .character = BRIB_REN,
                            .flags = 22,
                            .motion = 28678,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = -1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = nullptr,
                            .character = BRIB_KUK,
                            .flags = 22,
                            .motion = 28680,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = -1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = BribKukMotionCallback},
        0);
    QueueAreaLoopMotion({.parent = nullptr,
                            .character = BRIB_RYO,
                            .flags = 22,
                            .motion = 0xf003,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = -1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        760);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = BRIB_KUK, .flags = 3, .position = BRIB_KUK_POSITION, .rotation = BRIB_ROTATION, .completion = nullptr},
        0);
    QueueAreaLoopPosition({.parent = HLib::GetCurrentTask(),
                              .character = BRIB_CAMERA_TARGET,
                              .flags = 3,
                              .position = BRIB_KUK_POSITION,
                              .rotation = BRIB_ROTATION,
                              .completion = nullptr},
        0);
    Vector3 ryo = BRIB_RYO_OFFSET;
    BribTransformOffset(BRIB_KUK_POSITION, BRIB_ROTATION, ryo);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = BRIB_RYO, .flags = 3, .position = ryo, .rotation = BRIB_ROTATION, .completion = nullptr}, 0);
    Vector3 prop = BRIB_RYOS_OFFSET;
    BribTransformOffset(ryo, BRIB_ROTATION, prop);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = BRIB_REN, .flags = 3, .position = prop, .rotation = BRIB_ROTATION, .completion = nullptr}, 0);
    camCtrlSetFovTransitionTo(1);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    QueueAreaLoopCamera("BRIB", 2100, BRIB_CAMERA_TARGET, nullptr, 0);
    QueueAreaLoopCamera("BRIB", 2109, BRIB_CAMERA_TARGET, nullptr, 160);
    QueueAreaLoopCamera("BRIB", 2102, BRIB_CAMERA_TARGET, nullptr, 242);
    QueueAreaLoopCamera("BRIB", 2103, BRIB_CAMERA_TARGET, nullptr, 360);
    QueueAreaLoopCamera("BRIB", 2107, BRIB_CAMERA_TARGET, nullptr, 470);
    QueueAreaLoopCamera("BRIB", 2104, BRIB_CAMERA_TARGET, nullptr, 605);
    QueueAreaLoopCamera("BRIB", 2105, BRIB_CAMERA_TARGET, nullptr, 700);
    QueueAreaLoopCamera("BRIB", 2108, BRIB_CAMERA_TARGET, nullptr, 760);
    QueueAreaLoopCamera("BRIB", 2106, BRIB_CAMERA_TARGET, nullptr, 810);
    BribQueueLookAt(BRIB_RYO, BRIB_REN, 5, 103, 200);
    BribQueueLookAt(BRIB_REN, BRIB_RYO, 5, 103, 200);
    BribQueueLookAt(BRIB_RYO, BRIB_RYO, 14, 360, 157);
    BribQueueLookAt(BRIB_REN, BRIB_KUK, 5, 700, 160);
    BribQueueAttachment(BRIB_RYOS, {0.06f, 0.004f, -0.023f}, {13835, 50244, 33860}, 318, 210, BribRenHandCallback);
    BribQueueAttachment(BRIB_RYO, {0.099f, 0, -0.029f}, {14381, 33860, 32039}, 535, 199, nullptr);
    BribQueueAttachment(BRIB_KUK, {0.03f, -0.006f, -0.027f}, {10012, 48969, 45875}, 735, 100, nullptr);
    QueueAreaLoopVoice("0234E001", 0, -1, nullptr, 103);
    QueueAreaLoopVoice("0234A005", 0, -1, nullptr, 170);
    QueueAreaLoopVoice("0234E002", 0, -1, nullptr, 222);
    QueueAreaLoopVoice("0234E003", 0, -1, nullptr, 360);
    QueueAreaLoopVoice("0234A006", 0, -1, nullptr, 605);
    QueueAreaLoopVoice("0234E004", 0, -1, nullptr, 700);
    QueueAreaLoopVoice("0234B008", 0, -1, nullptr, 750);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
}
void BribRenLoop(void *data) {
    using enum CameraState;
    if (filmCheckFadeTrackColour())
        return;
    BribWork &work = *static_cast<BribWork *>(data);
    if (!(work.flags & 1U)) {
        work.flags |= 1U;
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    camCtrlSetFovTransitionTo(1);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetState(Init);
    mtwkRequestMotion(scenFindMotionwork(BRIB_RYO), 0xf003, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(BRIB_REN), 0xf003, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(BRIB_KUK), 0xf09f, MotionRegion::LowerBody);
    SetAreaLoopCharacterControl(BRIB_GREN, 0);
    EV_DisableCharacterDisplay(BRIB_GREN);
    SetAreaLoopCharacterControl(BRIB_RYO, 0);
    InitiateCleanupForSceneChara(BRIB_KUK);
    InitiateCleanupForSceneChara(BRIB_REN);
    for (const shen_name *name = BRIB_REN_CHARACTERS; name->name; ++name) {
        if (!DoesScenarioCharacterExist(*name))
            continue;
        SetAreaLoopCharacterControl(*name, 0);
        EV_npcLockUnlock(*name);
    }
    EV_SetCharaControllerFlags(BRIB_KUK, 0, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(BRIB_KUK), 17, 0);
    Vector3 position;
    EV_GetCharaPosition(BRIB_KUK, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.y += 0.45f;
    EV_AT_SetPos(BRIB_KUK, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    HLib::ResolveDeferredCallback(work.parent);
    HLib::SetupTimedTask(work.parent, 1);
    HLib::MarkCurrentTaskForCleanup();
    scenFreeEvent("BRIB.EV3");
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("BRIB"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BRIB"), 0);
    SetPlayerControlEnabled(1);
    EV_NPC_NewLoadSw(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1869, 1);
}
void BribSenseGetNpcs(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetPlayerControlEnabled(0);
    HLib::SetCurrentTaskTo(BribSenseGetPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopNpcs(BRIB_SENSE_CHARACTERS, ResolveAreaLoopParent);
}
void BribSenseGetPackage(void *) {
    HLib::SetCurrentTaskTo(BribSenseStart);
    HLib::SetupTaskDelay(-1);
    EV_NPC_NewLoadSw(0);
    PrepareNpcAllFromArray();
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "BRIB");
}
std::int64_t BribSenseAuthorCallback(std::int32_t phase, std::int64_t frame, HLib::Task *parent) {
    if (!phase)
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    else if (phase == 1 && parent) {
        HLib::ResolveDeferredCallback(parent);
        HLib::SetupTimedTask(parent, 1);
    } else if (phase == 2 && static_cast<std::int32_t>(frame) == 1200)
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
    return 1;
}
void BribSenseAuthorLoop(void *data) {
    AreaLoopAuthorWork &work = *static_cast<AreaLoopAuthorWork *>(data);
    if (!(work.flags & 1U)) {
        if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load,
                reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>(work.package)))
            return;
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        work.flags |= 1U;
    }
    if (!(work.flags & 2U)) {
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author.name));
        work.flags |= 2U;
        if (work.completion)
            work.completion(0, 0, work.parent);
    }
    const std::uint32_t frame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work.completion)
        work.completion(2, frame, work.parent);
    const bool finished = !EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (!finished && frame < static_cast<std::uint32_t>(work.endFrame))
        return;
    if (work.completion)
        work.completion(1, 0, work.parent);
    if (!finished)
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    if (work.package && EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work.package), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work.package), 0);
    if (work.flags & 0x80000000U)
        work.flags &= ~0x80000000U;
    else
        HLib::DestroyCurrentTask();
}
bool QueueBribSenseAuthor() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BribSenseAuthorLoop, 4, 0x0b, sizeof(AreaLoopAuthorWork), AREA_EVENT_TASK);
    if (!task)
        return false;
    AreaLoopAuthorWork *work = static_cast<AreaLoopAuthorWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return false;
    }
    work->parent = HLib::GetCurrentTask();
    work->author = BRIB_SENSE;
    work->completion = BribSenseAuthorCallback;
    work->endFrame = 1000000;

    work->flags = 0x80000001U;
    BribSenseAuthorLoop(work);
    if (work->flags & 0x80000000U)
        work->flags &= ~0x80000000U;
    else
        HLib::DestroyTask(task);
    return true;
}
void BribSenseStart(void *data) {
    EV_NPC_NewLoadSw(1);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BRIB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BRIB"), 0);
    HLib::SetCurrentTaskTo(BribSenseLoop);
    if (data) {
        SetAreaLoopCharacterControl(BRIB_RYO, 1);
        HLib::SetupTaskDelay(-1);
        QueueBribSenseAuthor();
    }
}
void BribSenseLoop(void *data) {
    using enum CameraState;
    if (filmCheckFadeTrackColour())
        return;
    BribWork &work = *static_cast<BribWork *>(data);
    if (!(work.flags & 1U)) {
        work.flags |= 1U;
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    camCtrlSetFovTransitionTo(1);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetState(Init);
    mtwkRequestMotion(scenFindMotionwork(BRIB_RYO), 0xf003, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(BRIB_REN), 0xf003, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(BRIB_RYO), 17, 0);
    SetAreaLoopCharacterControl(BRIB_RYO, 0);
    InitiateCleanupForSceneChara(BRIB_KUK);
    InitiateCleanupForSceneChara(BRIB_REN);
    RestoreAreaLoopCharacters(BRIB_SENSE_CHARACTERS);
    HLib::ResolveDeferredCallback(work.parent);
    HLib::SetupTimedTask(work.parent, 1);
    HLib::MarkCurrentTaskForCleanup();
    scenFreeEvent("BRIB.EV3");
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("BRIB"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BRIB"), 0);
    SetPlayerControlEnabled(1);
    EV_NPC_NewLoadSw(1);
}
} // namespace
// @verified - p
void InitializeBribEvent() {
}
void RunBribEvent(std::uintptr_t argument) {
    if (!static_cast<std::uint32_t>(argument))
        QueueBribRen();
    else if (static_cast<std::uint32_t>(argument) == 2)
        QueueBribSense();
}
const ScriptAreaDefinition bribScriptDefinition{.areaId = BRIB_AREA, .field04 = 0, .init = InitializeBribEvent, .main = RunBribEvent};
} // namespace shenmue::s2::areas
