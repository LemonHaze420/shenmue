#include "s2/carry_event.hpp"
#include "s2/event_queued.hpp"
namespace shenmue::s2 {
// @verified - p
std::uint32_t PlayPlankCarryAuthoredTransition(std::int32_t kind, std::int32_t dialogue) {
    EVT_AT_SetPosAng(CARRY_EVENT_PLAYER, CARRY_EVENT_STAGE, 0, 0, 0, 0);
    const Vector3 &renPosition = CARRY_TRANSITION_REN_POSITIONS[kind];
    EVT_AT_SetPosAng(CARRY_EVENT_REN, CARRY_EVENT_PLAYER, renPosition.x, renPosition.z, 0, 0);
    const std::int32_t frames = EVT_AT_SetMot(CARRY_EVENT_PLAYER, CARRY_TRANSITION_MOTIONS[kind][0], 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(CARRY_EVENT_REN, CARRY_TRANSITION_MOTIONS[kind][1], 0, 1, 0x40000000U, 0);
    for (std::int32_t index = 0; index < 5; ++index)
        EVT_AT_SetCam(CARRY_TRANSITION_CAMERAS[kind][index], CARRY_EVENT_STAGE, CARRY_EVENT_STAGE, 0, 0, CARRY_TRANSITION_CAMERA_DELAYS[kind][index]);
    for (std::int32_t index = 0; index < 5; ++index)
        EVT_AT_SetVoice(CARRY_TRANSITION_VOICES[dialogue][index], 1, CARRY_TRANSITION_VOICE_DELAYS[dialogue][kind][index]);
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<float>(frames) + 0.0f));
}
// @verified - p
std::uint32_t PlayPlankCarryAuthoredTransitionTail(std::int32_t kind, std::int32_t dialogue) {
    EVT_AT_SetPosAng(CARRY_EVENT_PLAYER, CARRY_EVENT_STAGE, 0, 0, 0, 0);
    const Vector3 &renPosition = CARRY_TRANSITION_TAIL_REN_POSITIONS[kind];
    EVT_AT_SetPosAng(CARRY_EVENT_REN, CARRY_EVENT_PLAYER, renPosition.x, renPosition.z, 0, 0);

    const float frame = CARRY_TRANSITION_TAIL_START_FRAMES[kind];
    const std::int32_t frames = EVT_AT_SetMot(CARRY_EVENT_PLAYER, CARRY_TRANSITION_MOTIONS[kind][0], frame, 1, 0x44000000U, 0);
    EVT_AT_SetMot(CARRY_EVENT_REN, CARRY_TRANSITION_MOTIONS[kind][1], frame, 1, 0x44000000U, 0);
    for (std::int32_t index = 0; index < 3; ++index)
        EVT_AT_SetCam(CARRY_TRANSITION_TAIL_CAMERAS[kind][index], CARRY_EVENT_STAGE, CARRY_EVENT_STAGE, 0, 0, CARRY_TRANSITION_TAIL_CAMERA_DELAYS[kind][index]);
    for (std::int32_t index = 0; index < 3; ++index)
        EVT_AT_SetVoice(CARRY_TRANSITION_TAIL_VOICES[dialogue][index], 1, CARRY_TRANSITION_TAIL_VOICE_DELAYS[kind][index]);
    return static_cast<std::uint32_t>(static_cast<std::int32_t>((static_cast<float>(frames) - frame) + 0.0f));
}
} // namespace shenmue::s2
