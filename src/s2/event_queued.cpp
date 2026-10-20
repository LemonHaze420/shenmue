#include "s2/event_queued.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"

#include <bit>
#include <cmath>
#include <cstring>
#include <xmmintrin.h>

namespace shenmue::s2 {
namespace {
constexpr std::uint32_t axes = EventTransformFlags::AllAxes;

bool Waiting(float &delay) {
    if (delay <= 0)
        return false;
    delay -= HLib::GetScaledDeltaTick();
    return true;
}
void Complete(std::int32_t cleanup) {
    if (!cleanup)
        return;
    UnregisterEventTask(HLib::GetCurrentTask());
    HLib::DestroyCurrentTask();
}
std::uint16_t FixedAngle(float degrees) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>((degrees * 8192.0f) / 45.0f));
}
template <class T> void Queue(HLib::TaskCallback callback, const T &data) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(callback, 6, 0x0b, sizeof(T), SHEN_NAME(EVNT));
    RegisterEventTask(task);
    *static_cast<T *>(HLib::GetTaskParameter(task)) = data;
}
}



void EVT_AT_SetPosRot(void *data) {
    auto &p = *static_cast<EventSetPosition *>(data);
    if (Waiting(p.delay))
        return;
    if (p.character.name == p.parent.name) {
        Vector3 position{};
        Vector3i rotation{};
        EV_GetCharaPosition(p.character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(p.character, &rotation, EventTransformFlags::World);
        ApplyScenarioTranslationAndRotation(&position, &rotation, &p.position);
        EV_AT_SetPos(p.character, axes | EventTransformFlags::World, p.position);
    } else {
        const shen_name previous = EV_SCEN_FindChar_Wrapper(p.character);
        AT_SetLocalBaseChara(p.character, p.parent);
        EV_AT_SetPos(p.character, axes, p.position);
        AT_SetLocalBaseChara(p.character, previous);
    }
    Complete(p.cleanup);
}
void EVT_AT_SetAngY_Callback(void *data) {
    auto &p = *static_cast<EventSetAngle *>(data);
    if (Waiting(p.delay))
        return;
    const shen_name previous = p.character.name == p.parent.name ? p.parent : EV_SCEN_FindChar_Wrapper(p.character);
    if (p.character.name != p.parent.name)
        AT_SetLocalBaseChara(p.character, p.parent);
    Vector3i rotation{};
    GetScenarioCharacterEulerAngles(p.character, &rotation, EventTransformFlags::Local);
    rotation.y = p.character.name == p.parent.name ? rotation.y + FixedAngle(p.degrees) : FixedAngle(p.degrees);
    EV_AT_SetAngY(p.character, axes, rotation);
    if (p.character.name != p.parent.name)
        AT_SetLocalBaseChara(p.character, previous);
    Complete(p.cleanup);
}

// @verified - p
void EVT_AT_SetAng_Callback(void *data) {
    EventSetRawAngle &parameter = *static_cast<EventSetRawAngle *>(data);
    if (Waiting(parameter.delay))
        return;
    if (parameter.character.name == parameter.parent.name) {
        Vector3i rotation;
        GetScenarioCharacterEulerAngles(parameter.character, &rotation, EventTransformFlags::Local);
        rotation.x = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(rotation.x) + static_cast<std::uint32_t>(parameter.rotation.x));
        rotation.y = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(rotation.y) + static_cast<std::uint32_t>(parameter.rotation.y));
        rotation.z = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(rotation.z) + static_cast<std::uint32_t>(parameter.rotation.z));
        EV_AT_SetAngY(parameter.character, axes, rotation);
    } else {
        const shen_name previous = EV_SCEN_FindChar_Wrapper(parameter.character);
        AT_SetLocalBaseChara(parameter.character, parameter.parent);
        EV_AT_SetAngY(parameter.character, axes, parameter.rotation);
        AT_SetLocalBaseChara(parameter.character, previous);
    }
    Complete(parameter.cleanup);
}


void EVT_AT_SetAng_Enqueue(shen_name character, shen_name parent, float degreesX, float degreesY, float degreesZ, std::int32_t delay) {
    EventSetRawAngle parameter{delay != 0 ? 1 : 0, character, parent,
        {static_cast<std::int32_t>((degreesX * 8192.0f) / 45.0f), static_cast<std::int32_t>((degreesY * 8192.0f) / 45.0f),
            static_cast<std::int32_t>((degreesZ * 8192.0f) / 45.0f)},
        static_cast<float>(delay)};
    if (delay != 0)
        Queue(EVT_AT_SetAng_Callback, parameter);
    else
        EVT_AT_SetAng_Callback(&parameter);
}
void EVT_AT_SetPosAng_Callback(void *data) {
    auto &p = *static_cast<EventSetPositionAngle *>(data);
    if (Waiting(p.delay))
        return;
    Vector3i rotation{};
    if (p.character.name == p.parent.name) {
        Vector3 position{};
        EV_GetCharaPosition(p.character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(p.character, &rotation, EventTransformFlags::World);
        ApplyScenarioTranslationAndRotation(&position, &rotation, &p.position);
        EV_AT_SetPos(p.character, axes | EventTransformFlags::World, p.position);
        GetScenarioCharacterEulerAngles(p.character, &rotation, EventTransformFlags::Local);
        rotation.y += FixedAngle(p.degrees);
        EV_AT_SetAngY(p.character, axes, rotation);
    } else {
        const shen_name previous = EV_SCEN_FindChar_Wrapper(p.character);
        AT_SetLocalBaseChara(p.character, p.parent);
        Vector3 position{};
        EV_GetCharaPosition(p.character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        p.position.y = position.y;
        rotation.y = FixedAngle(p.degrees);
        EV_AT_SetPos(p.character, axes, p.position);
        EV_AT_SetAngY(p.character, axes, rotation);
        AT_SetLocalBaseChara(p.character, previous);
    }
    Complete(p.cleanup);
}
// The flag word is bit-tested, not interpreted as a numeric float.
// @verified - p
void EVT_AT_SetMot_Callback(void *data) {
    EventSetMotion &p = *static_cast<EventSetMotion *>(data);
    if (p.delay > 0.0f) {
        p.delay -= HLib::GetScaledDeltaTick();
        return;
    }
    MotionWork *const work = FindMotionWork(FindCharacterByScenarioName(p.character));
    if ((p.flags & 0x80000000U) == 0) {
        SetMotionRegionFlag(work, 71, (p.flags & 0x40000000U) ? 0 : 2, MotionRegion::LowerBody);
        SetMotionRegionFlag(work, 69, (p.flags & 0x20000000U) ? 0 : 2, MotionRegion::LowerBody);
        mtwkSetExtraFlag(work, 14, (p.flags & 0x04000000U) ? 0 : 2);
        mtwkSetExtraFlag(work, 17, (p.flags & 0x02000000U) ? 2 : 0);
        if (p.flags & 0x01000000U)
            SetMotionRegionFlag(work, 66, 0, MotionRegion::LowerBody);
        if (p.flags & 0x00800000U)
            SetMotionRegionFlag(work, 66, 2, MotionRegion::LowerBody);
        EV_mtwkSetEnabled(work, (p.flags & 0x10000000U) == 0);
        mtwkSetExtraFlag(work, 0, (p.flags & 0x10000000U) ? 2 : 0);
    }
    mtwkRequestMotion(work, static_cast<std::uint16_t>(p.animation), MotionRegion::LowerBody);
    SetMotionRegionEvalTime(work, p.frame, MotionRegion::LowerBody);
    SetMotionSpeed(work, p.speed, MotionRegion::LowerBody);
    Complete(p.cleanup);
}
void EVT_AT_SetCam_Callback(void *data) {
    using enum CameraState;
    auto &p = *static_cast<EventSetCamera *>(data);
    if (Waiting(p.delay))
        return;
    if (p.camera >= 0) {
        if (p.angle < 0)
            camCtrlSetSplineShouldRebuildRotTo(true);
        const bool unattached = p.eye.name == 0xffffffffU && p.target.name == 0xffffffffU;
        EVS_LoopCamera(p.camera, unattached ? shen_name{} : p.eye, unattached ? shen_name{} : p.target);
    } else {
        if (p.camera == -2)
            camCtrlSetFovTransitionTo(true);
        camCtrlHandleContextSwitch(p.context);
        camCtrlSetState(Init);
    }
    camCtrlDisableFilmBars();
    Complete(p.cleanup);
}
void EVT_AT_SetVoice_Callback(void *data) {
    auto &p = *static_cast<EventSetVoice *>(data);
    if (Waiting(p.delay))
        return;
    if (p.mode != 0 || !cvsHasValidConverseData())
        EV_PlayDialogue(p.voice);
    Complete(p.cleanup);
}
void EVT_AT_MenCtrl_Callback(void *data) {
    auto &p = *static_cast<EventFilmControl *>(data);
    if (Waiting(p.delay))
        return;
    const ScreenFilmColourRequest request{{static_cast<std::uint8_t>(p.startAlpha), static_cast<std::uint8_t>(p.startRed),
                                              static_cast<std::uint8_t>(p.startGreen), static_cast<std::uint8_t>(p.startBlue)},
        {static_cast<std::uint8_t>(p.targetAlpha), static_cast<std::uint8_t>(p.targetRed), static_cast<std::uint8_t>(p.targetGreen),
            static_cast<std::uint8_t>(p.targetBlue)},
        p.frames};
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    WaitForNextTask(control->screenFilmTask, request, 0);
    Complete(p.cleanup);
}
void EVT_AT_DispBlur_Callback(void *data) {
    auto &p = *static_cast<EventDisplayBlur *>(data);
    if (Waiting(p.delay))
        return;
    if (p.frames <= 0) {
        figpAssignModeFor(p.character, 0);
        Complete(1);
    } else {
        figpAssignModeFor(p.character, static_cast<std::int8_t>(p.mode));
        figpSetFlagsBlendForBlur(p.character, static_cast<std::int8_t>(p.scale), static_cast<std::int8_t>(p.direction));
        figpSetFlagModesFor(p.character, 0, p.flags < 0);
        figpSetFlagModesFor(p.character, 3, (p.flags & 0x40000000) != 0);
        figpSetFlagModesFor(p.character, 4, (p.flags & 0x20000000) != 0);
        p.delay = static_cast<float>(p.frames);
        p.frames = 0;
    }
}
void EVT_AT_SetPos(shen_name character, shen_name parent, const Vector3 &position, std::int32_t delay) {
    EventSetPosition request{delay != 0, character, parent, position, static_cast<float>(delay)};
    if (delay)
        Queue(EVT_AT_SetPosRot, request);
    else
        EVT_AT_SetPosRot(&request);
}
void EVT_AT_SetAngY(shen_name character, shen_name parent, float degrees, std::int32_t delay) {
    EventSetAngle request{delay != 0, character, parent, degrees, static_cast<float>(delay)};
    if (delay)
        Queue(EVT_AT_SetAngY_Callback, request);
    else
        EVT_AT_SetAngY_Callback(&request);
}
void EVT_AT_SetPosAng(shen_name character, shen_name parent, float x, float z, float degrees, std::int32_t delay) {
    Queue(EVT_AT_SetPosAng_Callback, EventSetPositionAngle{1, character, parent, {x, 0, z}, degrees, static_cast<float>(delay)});
}
std::int32_t EVT_AT_SetMot(shen_name character, std::uint32_t animation, float frame, float speed, std::uint32_t flags, std::int32_t delay) {
    EventSetMotion request{delay != 0, character, animation, frame, speed, flags, static_cast<float>(delay)};
    if (delay)
        Queue(EVT_AT_SetMot_Callback, request);
    else
        EVT_AT_SetMot_Callback(&request);
    const float divisor = std::fabs(speed) < 0.00000011920928955078125F ? 1.0F : speed;
    const float motionFrames = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(static_cast<std::uint16_t>(animation))));
    return _mm_cvttss_si32(_mm_set_ss((motionFrames - frame) / divisor + static_cast<float>(delay)));
}
void EVT_AT_SetCam(std::int32_t camera, shen_name eye, shen_name target, float angle, std::uint8_t context, std::int32_t delay) {
    EventSetCamera immediate;
    EventSetCamera *request = &immediate;
    if (delay) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), SHEN_NAME(EVNT));
        RegisterEventTask(task);
        request = static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
    }
    request->cleanup = delay != 0;
    request->camera = camera;
    request->eye = eye;
    request->target = target;
    request->context = context;
    request->angle = angle;
    request->delay = static_cast<float>(delay);
    if (!delay) {
        EVT_AT_SetCam_Callback(request);
        camCtrlDisableFilmBars();
    }
}
void EVT_AT_SetVoice(const char *voice, std::int32_t mode, std::int32_t delay) {
    EventSetVoice request{delay != 0, mode, {}, static_cast<float>(delay)};
    std::strcpy(request.voice, voice);
    if (delay)
        Queue(EVT_AT_SetVoice_Callback, request);
    else
        EVT_AT_SetVoice_Callback(&request);
}
void EVT_AT_SetVoice(const char *voice, std::int32_t startFrame) {
    if (startFrame) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 0x0b, sizeof(EventSetVoice), SHEN_NAME(EVNT));
        RegisterEventTask(task);
        EventSetVoice *const request = static_cast<EventSetVoice *>(HLib::GetTaskParameter(task));
        request->cleanup = 1;
        request->mode = 1;
        request->delay = static_cast<float>(startFrame);
        std::strcpy(request->voice, voice);
    } else {
        char voiceCode[32];
        std::strcpy(voiceCode, voice);
        EV_PlayDialogue(voiceCode);
    }
}
// @verified - p
void EVT_AT_MenCtrl(const EventFilmControl &request) {
    EventFilmControl parameter = request;
    parameter.cleanup = parameter.delay != 0;
    if (parameter.delay)
        Queue(EVT_AT_MenCtrl_Callback, parameter);
    else
        EVT_AT_MenCtrl_Callback(&parameter);
}
void EVT_AT_DispBlur(shen_name character, std::uint8_t mode, std::uint8_t scale, std::uint8_t direction, std::int32_t frames, std::int32_t delay) {
    Queue(EVT_AT_DispBlur_Callback, EventDisplayBlur{character, mode, scale, direction, 0, 0, frames, static_cast<float>(delay)});
}

// @verified - p
void EVT_AT_SetSound_Callback(void *data) {
    EventSetSound &parameter = *static_cast<EventSetSound *>(data);
    if (Waiting(parameter.delay))
        return;
    Loop3DSoundEffect(parameter.sound);
    Complete(parameter.cleanup);
}

// @verified - p
std::uintptr_t EVT_AT_SetSound(std::uint32_t sound, std::int32_t delay) {
    if (delay == 0)
        return static_cast<std::uintptr_t>(Loop3DSoundEffect(sound));
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetSound_Callback, 6, 0x0b, 0x0c, EVENT_QUEUED_SOUND_TASK);
    RegisterEventTask(task);
    EventSetSound *const parameter = static_cast<EventSetSound *>(HLib::GetTaskParameter(task));
    parameter->cleanup = 1;
    parameter->sound = sound;
    parameter->delay = static_cast<float>(delay);
    return reinterpret_cast<std::uintptr_t>(parameter);
}

// @verified - p
void EVT_AT_KsIcon_Callback(void *data) {
    EventChoiceIcon &parameter = *static_cast<EventChoiceIcon *>(data);
    if (Waiting(parameter.delay))
        return;
    SetupControlSpeakAndUI(parameter.choices, static_cast<std::uint16_t>(parameter.timeout), static_cast<std::int16_t>(parameter.defaultChoice),
        static_cast<std::uint16_t>(parameter.mode));
    Complete(parameter.cleanup);
}

// @verified - p
std::int32_t EVT_AT_KsIcon_Enqueue(std::int16_t first, std::int16_t second, std::int16_t third, std::int16_t fourth, std::int32_t timeout,
    std::int32_t defaultChoice, std::int32_t, std::int32_t delay) {
    if (delay != 0) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_KsIcon_Callback, 6, 0x0b, 0x1c, EVENT_QUEUED_CHOICE_TASK);
        RegisterEventTask(task);
        EventChoiceIcon *const parameter = static_cast<EventChoiceIcon *>(HLib::GetTaskParameter(task));
        parameter->timeout = timeout;
        parameter->cleanup = 1;
        parameter->choices[0] = first;
        parameter->choices[1] = second;
        parameter->delay = static_cast<float>(delay);
        parameter->choices[2] = third;
        parameter->choices[3] = fourth;
        parameter->defaultChoice = defaultChoice;
        parameter->mode = 2;
        return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(parameter)));
    }
    std::int16_t choices[]{first, second, third, fourth};
    return SetupControlSpeakAndUI(choices, static_cast<std::uint16_t>(timeout), static_cast<std::int16_t>(defaultChoice), 2);
}

// @verified - p
void EVT_AT_SetPoint_Callback(void *data) {
    EventSetPoint &parameter = *static_cast<EventSetPoint *>(data);
    if (Waiting(parameter.delay))
        return;
    if ((parameter.flags & 0x10000000U) == 0) {
        Vector3 position;
        EV_GetCharaPosition(parameter.target, static_cast<std::uint32_t>(parameter.targetBone), &position, EventTransformFlags::World);
        if ((parameter.flags & 0x08000000U) == 0) {
            if (parameter.frame == 0 && (parameter.flags & 0x80000000U) != 0)
                UpdateMotionWorkWithPosition(scenFindMotionwork(parameter.character), nullptr, 3);
            const std::uint32_t operation = static_cast<std::uint32_t>(parameter.positionMode) + (static_cast<std::uint32_t>(parameter.trackingMode) << 8);
            UpdateMotionWorkWithPosition(scenFindMotionwork(parameter.character), &position, static_cast<std::uint16_t>(operation));
        }
        if ((parameter.flags & 0x04000000U) == 0)
            SetEyeToChara(FindCharacterByScenarioName(parameter.character), &position, EVENT_POINT_EYE_FRAMES[parameter.trackingMode], 2);
        if ((parameter.flags & 0x20000000U) != 0 && parameter.cleanup != 0)
            Complete(parameter.cleanup);
    }
    parameter.frame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(parameter.frame) + 1U);
    if (parameter.frame > parameter.duration || (parameter.flags & 0x10000000U) != 0) {
        if ((parameter.flags & 0x20000000U) == 0) {
            if ((parameter.flags & 0x08000000U) == 0) {
                if ((parameter.flags & 0x40000000U) != 0)
                    UpdateMotionWorkWithPosition(scenFindMotionwork(parameter.character), nullptr, 3);
                UpdateMotionWorkWithPosition(scenFindMotionwork(parameter.character), nullptr, 0);
            }
            if ((parameter.flags & 0x04000000U) == 0)
                SetEyeToChara(FindCharacterByScenarioName(parameter.character), nullptr, 16, 0);
            if (parameter.cleanup != 0)
                Complete(parameter.cleanup);
        }
    }
}

// @verified - p
void EVT_AT_SetPoint(shen_name character, shen_name target, std::int32_t targetBone, std::int32_t trackingMode, std::int32_t positionMode, std::int32_t delay,
    std::int32_t duration, std::uint32_t flags) {
    if ((flags & 0x30000000U) == 0 || delay != 0) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetPoint_Callback, 6, 0x0b, 0x28, EVENT_QUEUED_POINT_TASK);
        RegisterEventTask(task);
        EventSetPoint *const parameter = static_cast<EventSetPoint *>(HLib::GetTaskParameter(task));
        parameter->positionMode = positionMode;
        parameter->duration = duration;
        parameter->character = character;
        parameter->target = target;
        parameter->targetBone = targetBone;
        parameter->trackingMode = trackingMode;
        parameter->delay = static_cast<float>(delay);
        parameter->flags = flags;
        parameter->frame = 0;
        parameter->cleanup = 1;
    } else {
        EventSetPoint parameter{character, target, targetBone, trackingMode, positionMode, duration, flags, 0, 0, 0.0f};
        EVT_AT_SetPoint_Callback(&parameter);
    }
}

// @verified - p
void EVT_AT_CtrlHandUp_Callback(void *data) {
    EventControlHand &parameter = *static_cast<EventControlHand *>(data);
    if (Waiting(parameter.delay))
        return;
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(parameter.character), static_cast<std::int32_t>(parameter.side), parameter.command,
        static_cast<std::int16_t>(parameter.ticks));
    Complete(parameter.cleanup);
}

// @verified - p
std::uintptr_t Queue_EVT_AT_CtrlHandUp_1(shen_name character, std::uint32_t side, std::byte *command, std::uint32_t ticks, std::int32_t delay) {
    if (delay == 0)
        return ApplyCharacterHandPoseCommand(
            FindCharacterByScenarioName(character), static_cast<std::int32_t>(side), command, static_cast<std::int16_t>(ticks));
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_CtrlHandUp_Callback, 6, 0x0b, 0x20, EVENT_QUEUED_HAND_TASK);
    RegisterEventTask(task);
    EventControlHand *const parameter = static_cast<EventControlHand *>(HLib::GetTaskParameter(task));
    parameter->cleanup = 1;
    parameter->character = character;
    parameter->side = side;
    parameter->command = command;
    parameter->ticks = ticks;
    parameter->delay = static_cast<float>(delay);
    return reinterpret_cast<std::uintptr_t>(parameter);
}

// @verified - p
void EVT_AT_PlayDynamix_Callback(void *data) {
    EventPlayDynamics &parameter = *static_cast<EventPlayDynamics *>(data);
    if (Waiting(parameter.delay))
        return;
    if (parameter.endFrame <= parameter.currentFrame) {
        Complete(1);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    const std::int32_t frame = _mm_cvtt_ss2si(_mm_set_ss(parameter.currentFrame));
    if (parameter.trackCount == 0) {
        dynaReadEvaluate(0, frame, &position.x, &rotation, reinterpret_cast<const std::uint8_t *>(parameter.resource));
        position.x *= 10.0f;
        position.y *= 10.0f;
        position.z *= 10.0f;
        EV_AT_SetPos(parameter.character, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(parameter.character, EventTransformFlags::AllAxes, rotation, 0);
    } else {
        for (std::int32_t track = 0; track < parameter.trackCount; ++track) {
            dynaReadEvaluate(track, frame, &position.x, &rotation, reinterpret_cast<const std::uint8_t *>(parameter.resource));
            position.x *= 10.0f;
            position.y *= 10.0f;
            position.z *= 10.0f;
            const std::uint16_t part = static_cast<std::uint16_t>(parameter.parts ? parameter.parts[track] : track + 152);
            EV_scenApplyPosOrRotToPartOf(parameter.character, part, &position, &rotation, EventCharacterPartTransformMode::Absolute);
        }
    }
    parameter.currentFrame += parameter.frameStep;
}
// @verified - p
HLib::Task *Queue_EVT_AT_PlayDynamix(shen_name character, const char *resource, float frame, float endFrame, std::int32_t trackCount, const std::int32_t *parts,
    float frameStep, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), EVENT_QUEUED_DYNAMICS_TASK);
    RegisterEventTask(task);
    EventPlayDynamics *const work = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
    work->trackCount = trackCount;
    work->endFrame = endFrame;
    work->currentFrame = frame;
    work->character = character;
    work->resource = resource;
    work->parts = parts;
    work->frameStep = frameStep;
    work->delay = static_cast<float>(delay);
    return task;
}
// @verified - p
void EVT_AT_SetCameraShake_Callback(void *data) {
    auto &parameter = *static_cast<EventCameraShake *>(data);
    if (Waiting(parameter.delay))
        return;
    camCtrlSetRotationXYZ(parameter.x, parameter.y, parameter.decay);
    Complete(parameter.cleanup);
}
// @verified - p
void QueueCameraShake(float x, float y, float decay, std::int32_t delay) {
    Queue(EVT_AT_SetCameraShake_Callback, EventCameraShake{1, x, y, decay, static_cast<float>(delay)});
}
// @verified - p
void EventMessageBeginCallback(void *data) {
    dbgPrintEventMessage(static_cast<EventMessageTaskData *>(data)->message);
    HLib::SetCurrentTaskTo(EventMessageEndCallback);
}
// @verified - p
void EventMessageEndCallback(void *data) {
    EventMessageTaskData &work = *static_cast<EventMessageTaskData *>(data);
    if (--work.remainingFrames == 0) {
        ctrlStopSubtitles();
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
std::uint32_t QueueEventMessageSequence(const EventMessageRecord *records) {
    const EventMessageRecord *cursor = records;
    while (cursor->frames || cursor->delay) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EventMessageBeginCallback, 6, 0x0b, sizeof(EventMessageTaskData), EVENT_QUEUED_MESSAGE_TASK);
        if (cursor->delay)
            HLib::SetupTimedTask(task, cursor->delay);
        EventMessageTaskData *work = static_cast<EventMessageTaskData *>(HLib::GetTaskParameter(task));
        work->message = cursor->message;
        work->remainingFrames = static_cast<std::uint32_t>(cursor->frames);
        ++cursor;
    }
    return cursor == records ? 5U : static_cast<std::uint32_t>(cursor[-1].frames) + static_cast<std::uint32_t>(cursor[-1].delay) + 5U;
}
} // namespace shenmue::s2
