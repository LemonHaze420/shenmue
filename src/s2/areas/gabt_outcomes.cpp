#include "s2/areas/area_gabt.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/extra_telop.hpp"
namespace shenmue::s2::areas {
namespace {
std::int32_t choreographyFrame;
void SetChoreographyMotion(shen_name character, std::uint32_t animation) {
    EventSetMotion motion{.cleanup = 0, .character = character, .animation = animation, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(static_cast<std::uint16_t>(animation));
}
void QueueChoreographyVoice(const char *voice, float delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 0x0b, 0x2c, GABT_MOLE_TASK);
    RegisterEventTask(task);
    EventSetVoice &work = *static_cast<EventSetVoice *>(HLib::GetTaskParameter(task));
    work.cleanup = 1;
    work.mode = 1;
    work.delay = delay;
    std::size_t index = 0;
    do {
        work.voice[index] = voice[index];
    } while (voice[index++]);
}
void QueueChoreographyCamera(std::int32_t camera, float delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, 0x1c, GABT_MOLE_TASK);
    RegisterEventTask(task);
    EventSetCamera &work = *static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
    work = {.cleanup = 1, .camera = camera, .eye = GABT_MOLE_BASE, .target = GABT_MOLE_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = delay};
}
void PrepareChoreographyCharacters() {
    HasCharacterMotionWork(FindCharacterByScenarioName(GABT_MOLE_RYO));
    HasCharacterMotionWork(FindCharacterByScenarioName(GABT_MOLE_GAI));
    for (shen_name character : {GABT_MOLE_RYO, GABT_MOLE_GAI}) {
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 1, 0);
    }
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(GABT_MOLE_RYO), 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 1);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(GABT_MOLE_RYO), 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 1);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(GABT_MOLE_GAI), 0, reinterpret_cast<std::byte *>(gabtOpponentLeftHandPose), 1);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(GABT_MOLE_GAI), 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 1);
    AT_SetEyeCtrl(FindCharacterByScenarioName(GABT_MOLE_RYO), 0, 1, 1);
    AT_SetEyeCtrl(FindCharacterByScenarioName(GABT_MOLE_GAI), 0, 1, 1);
    AT_SetLocalBaseChara(GABT_ATTACHMENT, GABT_PIVOT);
    AT_SetLocalBaseChara(GABT_MOLE_RYO, GABT_ATTACHMENT);
    AT_SetLocalBaseChara(GABT_MOLE_GAI, GABT_ATTACHMENT);
}
void SetChoreographyPositions(const Vector3 &pivot, const Vector3 &opponent) {
    EventSetPosition position{.cleanup = 0, .character = GABT_ATTACHMENT, .parent = GABT_PIVOT, .position = {.x = 0, .y = 0, .z = 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = GABT_ATTACHMENT, .parent = GABT_PIVOT, .degrees = 165, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = GABT_PIVOT;
    position.parent = GABT_MOLE_INVALID;
    position.position = pivot;
    EVT_AT_SetPosRot(&position);
    angle.character = GABT_PIVOT;
    angle.parent = GABT_MOLE_INVALID;
    angle.degrees = 0;
    EVT_AT_SetAngY_Callback(&angle);
    EV_AT_SetPos(GABT_MOLE_SCENERY, EventTransformFlags::World | EventTransformFlags::AllAxes, GABT_SCENERY_POSITION);

    const Vector3i rotation{.x = 0, .y = 0, .z = 0};
    EV_AT_SetAngY(GABT_MOLE_SCENERY, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    position.character = GABT_MOLE_RYO;
    position.parent = GABT_ATTACHMENT;
    position.position = {.x = 0, .y = 0, .z = 0};
    EVT_AT_SetPosRot(&position);
    angle.character = GABT_MOLE_RYO;
    angle.parent = GABT_ATTACHMENT;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = GABT_MOLE_GAI;
    position.position = opponent;
    EVT_AT_SetPosRot(&position);
    angle.character = GABT_MOLE_GAI;
    EVT_AT_SetAngY_Callback(&angle);
}
// @verified - p: native CheckECXequalToZero is deliberately effect-free.
void GabtReflectorNullsub(std::int32_t value) {
    (void)value;
}
} // namespace
// @verified - p: GabtPrepareReflectors.
void GabtPrepareReflectors() {
    SetPluralControlFor(FindCharacterByScenarioName(GABT_MOLE_RYO), 0);
    SetPluralControlFor(FindCharacterByScenarioName(GABT_MOLE_GAI), 0);
    GabtReflectorNullsub(1);
    EV_SetRefBoardParamsFor(GABT_MOLE_RYO, 1);
    EV_SetRefBoardParamsFor(GABT_MOLE_GAI, 1);
    EV_SetRefBoardParamsFor(GABT_MOLE_SCENERY, 1);
    const Vector4 player{.x = 1, .y = 0, .z = 0, .w = 0}, opponent{.x = 1, .y = -.4f, .z = -.3f, .w = -.4f};
    SetScenarioReflectorBoardValues(GABT_MOLE_RYO, &player);
    SetScenarioReflectorBoardValues(GABT_MOLE_GAI, &opponent);
    SetScenarioReflectorBoardValues(GABT_MOLE_SCENERY, &player);
}
// @verified - p: QteMiss_Callback107bb0.
void GabtFailureChoreographyCallback(void *) {
    if (choreographyFrame == 160)
        AT_SetEyeCtrl(FindCharacterByScenarioName(GABT_MOLE_RYO), 15, .1f, .1f);
    else if (choreographyFrame == 200) {
        SetFacialBlinkDelay(FindCharacterByScenarioName(GABT_MOLE_RYO), 90);
        HLib::DestroyCurrentTask();
    }
    ++choreographyFrame;
}
// @verified - p: QteSuccess_Callback107a00.
void GabtSuccessChoreographyCallback(void *) {
    if (choreographyFrame <= 45) {
        switch (choreographyFrame) {
        case 0:
            figpSetFlagsBlendForBlur(GABT_MOLE_RYO, 4, -1);
            figpAssignModeFor(GABT_MOLE_RYO, 1);
            break;
        case 20:
            figpAssignModeFor(GABT_MOLE_RYO, 0);
            Loop3DSoundEffect(0xb0d);
            break;
        case 30:
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_RYO), 0, MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_GAI), 0, MotionRegion::LowerBody);
            break;
        case 45:
            QueueExtraTelop("BBBB", static_cast<std::int32_t>(0xc0000000U));
            break;
        default:
            break;
        }
    } else
        switch (choreographyFrame) {
        case 150:
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_RYO), 1, MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_GAI), 1, MotionRegion::LowerBody);
            break;
        case 220:
            AT_SetEyeCtrl(FindCharacterByScenarioName(GABT_MOLE_RYO), 15, .1f, .1f);
            break;
        case 320:
            SetFacialBlinkDelay(FindCharacterByScenarioName(GABT_MOLE_RYO), 90);
            HLib::DestroyCurrentTask();
            break;
        default:
            break;
        }
    ++choreographyFrame;
}
// @verified - p: failure producer108360; preserves all queue6/11 direct producer registrations.
std::int32_t QueueGabtFailureChoreography() {
    choreographyFrame = 0;
    HLib::EnqueueTaskWithoutParameter(GabtFailureChoreographyCallback, 4, 0x0b, static_cast<std::int32_t>(GABT_MOLE_TASK.name));
    PrepareChoreographyCharacters();
    SetChoreographyPositions(GABT_FAILURE_PIVOT_POSITION, GABT_FAILURE_OPPONENT_POSITION);
    SetChoreographyMotion(GABT_MOLE_RYO, 9476);
    SetChoreographyMotion(GABT_MOLE_GAI, 9475);
    QueueChoreographyVoice("0232B026", 30);
    QueueChoreographyVoice("0232B027", 140);
    QueueChoreographyVoice("0232B028", 210);
    QueueChoreographyVoice("0232A006", 300);
    EventSetCamera camera{
        .cleanup = 0, .camera = 9020, .eye = GABT_MOLE_BASE, .target = GABT_MOLE_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueChoreographyCamera(9021, 140);
    SetScreenFilmFade(30, 255, 0, 0, 0, 40, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, 0x2c, GABT_MOLE_TASK);
    RegisterEventTask(task);
    EventFilmControl &film = *static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
    film = {.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 360};
    return 420;
}
// @verified - p: success producer107c40.
std::int32_t QueueGabtSuccessChoreography() {
    choreographyFrame = 0;
    HLib::EnqueueTaskWithoutParameter(GabtSuccessChoreographyCallback, 4, 0x0b, static_cast<std::int32_t>(GABT_MOLE_TASK.name));
    PrepareChoreographyCharacters();
    SetChoreographyPositions(GABT_SUCCESS_PIVOT_POSITION, GABT_SUCCESS_OPPONENT_POSITION);
    SetChoreographyMotion(GABT_MOLE_RYO, 9486);
    SetChoreographyMotion(GABT_MOLE_GAI, 9485);
    QueueChoreographyVoice("0232B023", 160);
    QueueChoreographyVoice("0232B024", 290);
    QueueChoreographyVoice("0232B025", 410);
    QueueChoreographyVoice("0232A006", 490);
    EventSetCamera camera{
        .cleanup = 0, .camera = 9010, .eye = GABT_MOLE_BASE, .target = GABT_MOLE_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueChoreographyCamera(9011, 145);
    QueueChoreographyCamera(9012, 460);
    SetScreenFilmFade(10, 255, 0, 0, 0, 40, 0, 0, 0);
    return 520;
}
} // namespace shenmue::s2::areas
