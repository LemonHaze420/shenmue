#pragma once

#include "common/types.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
namespace HLib {
struct Task;
}

struct EventSetPosition {
    std::int32_t cleanup;
    shen_name character, parent;
    Vector3 position;
    float delay;
};
struct EventSetAngle {
    std::int32_t cleanup;
    shen_name character, parent;
    float degrees, delay;
};
struct EventSetRawAngle {
    std::int32_t cleanup;
    shen_name character, parent;
    Vector3i rotation;
    float delay;
};
struct EventSetPositionAngle {
    std::int32_t cleanup;
    shen_name character, parent;
    Vector3 position;
    float degrees, delay;
};
struct EventSetMotion {
    std::int32_t cleanup;
    shen_name character;
    std::uint32_t animation;
    float frame, speed;
    std::uint32_t flags;
    float delay;
};
struct EventSetCamera {
    std::int32_t cleanup, camera;
    shen_name eye, target;
    std::uint8_t context;
    std::byte reserved11[3];
    float angle, delay;
};
struct EventSetVoice {
    std::int32_t cleanup, mode;
    char voice[32];
    float delay;
};
struct EventFilmControl {
    std::int32_t cleanup, frames, startAlpha, startRed, startGreen, startBlue, targetAlpha, targetRed, targetGreen, targetBlue;
    float delay;
};
struct EventDisplayBlur {
    shen_name character;
    std::uint8_t mode, scale, direction, reserved07;
    std::int32_t flags, frames;
    float delay;
};
struct EventSetSound {
    std::int32_t cleanup;
    std::uint32_t sound;
    float delay;
};
static_assert(sizeof(EventSetSound) == 0x0c);
static_assert(offsetof(EventSetSound, sound) == 4);
static_assert(offsetof(EventSetSound, delay) == 8);
inline constexpr shen_name EVENT_QUEUED_SOUND_TASK{SHEN_NAME(EVNT)};
struct EventChoiceIcon {
    std::int32_t cleanup;
    std::int16_t choices[4];
    std::int32_t timeout;
    std::int32_t defaultChoice;
    std::uint32_t mode;
    float delay;
};
static_assert(sizeof(EventChoiceIcon) == 0x1c);
static_assert(offsetof(EventChoiceIcon, choices) == 4);
static_assert(offsetof(EventChoiceIcon, timeout) == 0x0c);
static_assert(offsetof(EventChoiceIcon, defaultChoice) == 0x10);
static_assert(offsetof(EventChoiceIcon, mode) == 0x14);
static_assert(offsetof(EventChoiceIcon, delay) == 0x18);
inline constexpr shen_name EVENT_QUEUED_CHOICE_TASK{SHEN_NAME(EVNT)};
struct EventSetPoint {
    shen_name character, target;
    std::int32_t targetBone, trackingMode, positionMode, duration;
    std::uint32_t flags;
    std::int32_t frame, cleanup;
    float delay;
};
static_assert(sizeof(EventSetPoint) == 0x28);
static_assert(offsetof(EventSetPoint, flags) == 0x18);
static_assert(offsetof(EventSetPoint, frame) == 0x1c);
static_assert(offsetof(EventSetPoint, cleanup) == 0x20);
static_assert(offsetof(EventSetPoint, delay) == 0x24);
inline constexpr std::int16_t EVENT_POINT_EYE_FRAMES[]{0, 32, 24, 16, 8, 4};
inline constexpr shen_name EVENT_QUEUED_POINT_TASK{SHEN_NAME(EVNT)};
struct EventControlHand {
    std::int32_t cleanup;
    shen_name character;
    std::uint32_t side, reserved0c;
    std::byte *command;
    std::uint32_t ticks;
    float delay;
};
static_assert(sizeof(EventControlHand) == 0x20);
static_assert(offsetof(EventControlHand, command) == 0x10);
static_assert(offsetof(EventControlHand, ticks) == 0x18);
inline constexpr shen_name EVENT_QUEUED_HAND_TASK{SHEN_NAME(EVNT)};
static_assert(sizeof(EventSetPosition) == 0x1c);
static_assert(sizeof(EventSetAngle) == 0x14);
static_assert(sizeof(EventSetRawAngle) == 0x1c);
static_assert(offsetof(EventSetRawAngle, rotation) == 0x0c);
static_assert(offsetof(EventSetRawAngle, delay) == 0x18);
static_assert(sizeof(EventSetPositionAngle) == 0x20);
static_assert(sizeof(EventSetMotion) == 0x1c);
static_assert(sizeof(EventSetCamera) == 0x1c);
static_assert(sizeof(EventSetVoice) == 0x2c);
static_assert(sizeof(EventFilmControl) == 0x2c);
static_assert(sizeof(EventDisplayBlur) == 0x14);

struct EventPlayDynamics {
    shen_name character;
    std::uint32_t reserved04;
    const char *resource;
    std::int32_t trackCount;
    std::uint32_t reserved14;
    const std::int32_t *parts;
    float endFrame, frameStep, currentFrame, delay;
};
static_assert(sizeof(EventPlayDynamics) == 0x30);
static_assert(offsetof(EventPlayDynamics, resource) == 8);
static_assert(offsetof(EventPlayDynamics, parts) == 0x18);
static_assert(offsetof(EventPlayDynamics, endFrame) == 0x20);
void EVT_AT_PlayDynamix_Callback(void *data);
inline constexpr shen_name EVENT_QUEUED_DYNAMICS_TASK{SHEN_NAME(EVNT)};
HLib::Task *Queue_EVT_AT_PlayDynamix(shen_name character, const char *resource, float frame, float endFrame, std::int32_t trackCount, const std::int32_t *parts,
    float frameStep, std::int32_t delay);

void EVT_AT_SetPosRot(void *data);
void EVT_AT_SetAngY_Callback(void *data);
void EVT_AT_SetAng_Callback(void *data);
void EVT_AT_SetPosAng_Callback(void *data);
void EVT_AT_SetMot_Callback(void *data);
void EVT_AT_SetCam_Callback(void *data);
void EVT_AT_SetVoice_Callback(void *data);
void EVT_AT_MenCtrl_Callback(void *data);
void EVT_AT_DispBlur_Callback(void *data);
void EVT_AT_SetPos(shen_name character, shen_name parent, const Vector3 &position, std::int32_t delay);
void EVT_AT_SetAngY(shen_name character, shen_name parent, float degrees, std::int32_t delay);
void EVT_AT_SetAng_Enqueue(shen_name character, shen_name parent, float degreesX, float degreesY, float degreesZ, std::int32_t delay);
void EVT_AT_SetPosAng(shen_name character, shen_name parent, float x, float z, float degrees, std::int32_t delay);
std::int32_t EVT_AT_SetMot(shen_name character, std::uint32_t animation, float frame, float speed, std::uint32_t flags, std::int32_t delay);
void EVT_AT_SetCam(std::int32_t camera, shen_name eye, shen_name target, float angle, std::uint8_t context, std::int32_t delay);
void EVT_AT_SetVoice(const char *voice, std::int32_t mode, std::int32_t delay);

void EVT_AT_SetVoice(const char *voice, std::int32_t startFrame);
void EVT_AT_MenCtrl(const EventFilmControl &request);
void EVT_AT_DispBlur(shen_name character, std::uint8_t mode, std::uint8_t scale, std::uint8_t direction, std::int32_t frames, std::int32_t delay);
void EVT_AT_SetSound_Callback(void *data);
std::uintptr_t EVT_AT_SetSound(std::uint32_t sound, std::int32_t delay);
void EVT_AT_KsIcon_Callback(void *data);
std::int32_t EVT_AT_KsIcon_Enqueue(std::int16_t first, std::int16_t second, std::int16_t third, std::int16_t fourth, std::int32_t timeout,
    std::int32_t defaultChoice, std::int32_t unused, std::int32_t delay);
void EVT_AT_SetPoint_Callback(void *data);
void EVT_AT_SetPoint(shen_name character, shen_name target, std::int32_t targetBone, std::int32_t trackingMode, std::int32_t positionMode, std::int32_t delay,
    std::int32_t duration, std::uint32_t flags);
void EVT_AT_CtrlHandUp_Callback(void *data);
std::uintptr_t Queue_EVT_AT_CtrlHandUp_1(shen_name character, std::uint32_t side, std::byte *command, std::uint32_t ticks, std::int32_t delay);
struct EventCameraShake {
    std::int32_t cleanup;
    float x, y, decay, delay;
};
static_assert(sizeof(EventCameraShake) == 0x14);
void EVT_AT_SetCameraShake_Callback(void *data);
void QueueCameraShake(float x, float y, float decay, std::int32_t delay = 27);

struct EventMessageRecord {
    const char *message;
    std::int32_t frames, delay;
};
struct EventMessageTaskData {
    const char *message;
    std::uint32_t remainingFrames, reserved0c;
};
static_assert(sizeof(EventMessageRecord) == 0x10 && offsetof(EventMessageRecord, frames) == 8 && offsetof(EventMessageRecord, delay) == 0x0c);
static_assert(sizeof(EventMessageTaskData) == 0x10 && offsetof(EventMessageTaskData, remainingFrames) == 8);
inline constexpr shen_name EVENT_QUEUED_MESSAGE_TASK{SHEN_NAME(EVNT)};
void EventMessageBeginCallback(void *data);
void EventMessageEndCallback(void *data);
std::uint32_t QueueEventMessageSequence(const EventMessageRecord *records);

} // namespace shenmue::s2
