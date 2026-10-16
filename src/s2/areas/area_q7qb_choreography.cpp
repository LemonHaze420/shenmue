#include "s2/areas/area_q7qb.hpp"
#include "s2/event_queued.hpp"
#include "s2/conversation.hpp"
#include <bit>

namespace shenmue::s2::areas {
void Q7QbFirstAttackBegin(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    EVT_AT_SetPos(p.player, Q7QB_BASE, {.x = -1, .y = 0, .z = -.5f}, delay);
    EVT_AT_SetAngY(p.player, Q7QB_BASE, 0, delay);
    EVT_AT_SetPos(p.firstOpponent, Q7QB_BASE, {.x = -1.4f, .y = 0, .z = -4.4f}, delay);
    EVT_AT_SetAngY(p.firstOpponent, Q7QB_BASE, 180, delay);
    EVT_AT_SetMot(p.player, 13, 0, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0xf006, 0, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7700, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
}
void Q7QbFirstAttackSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .3347f : -.3347f, -1.94273f, mirrored ? -180.0f : 180.0f, delay);
    EVT_AT_SetMot(p.player, 0x1bc1, 13, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1be1, 13, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 4, 12, 1, 10, delay + 20);
    EVT_AT_SetCam(7701, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVA002", 1, 20);
    EVT_AT_SetVoice("S2BVB019", 1, 30);
}
void Q7QbFirstAttackFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .34f : -.34f, -2.18f, mirrored ? -180.0f : 180.0f, delay);
    EVT_AT_SetMot(p.player, 0x1c1c, 37, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bec, 9, 1, p.motionFlags, delay);
    EVT_AT_SetSound(651, delay + 6);
    EVT_AT_SetCam(7715, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVB001", 1, 5);
    EVT_AT_SetVoice("S2BVA008", 1, 30);
}
void Q7QbSecondAttackBegin(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    EVT_AT_SetPosAng(p.firstOpponent, Q7QB_BASE, -.8f, 6.8f, 0, delay);
    EVT_AT_SetMot(p.firstOpponent, 0xf006, 0, 1, p.motionFlags, delay);
}
void Q7QbSecondAttackSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetAngY(p.player, p.player, 180, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? -.41f : .41f, -1.71f, mirrored ? -180.0f : 180.0f, delay);
    EVT_AT_SetMot(p.player, 0x1bc2, 30, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1be2, 0, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 6, 12, 1, 20, delay + 13);
    EVT_AT_SetCam(7702, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EV_PlayDialogue("S2BVA001");
    EVT_AT_SetVoice("S2BVB017", 1, 15);
}
void Q7QbSecondAttackFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPos(p.player, Q7QB_BASE, {.x = -1, .y = 0, .z = -.5f}, delay);
    EVT_AT_SetAngY(p.player, Q7QB_BASE, 0, delay);
    EVT_AT_SetAngY(p.player, p.player, 180, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .24f : -.24f, -1.79f, mirrored ? -180.0f : 180.0f, delay);
    EVT_AT_SetMot(p.player, 0x1c1c, 26, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bef, 0, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7716, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVB009", 1, 15);
    EVT_AT_SetVoice("S2BVA006", 1, 45);
}
void Q7QbThirdAttackBegin(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    EVT_AT_SetPosAng(p.player, Q7QB_BASE, -1.25f, -.87f, 180, delay);
    EVT_AT_SetPos(p.firstOpponent, Q7QB_BASE, {.x = -1.25f, .y = 0, .z = -10.6f}, delay);
    EVT_AT_SetAngY(p.firstOpponent, Q7QB_BASE, 180, delay);
    EVT_AT_SetMot(p.firstOpponent, 0xf006, 0, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7703, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
}
void Q7QbThirdAttackSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPos(p.player, Q7QB_BASE, {.x = -1, .y = 0, .z = .5f}, delay);
    EVT_AT_SetAngY(p.player, Q7QB_BASE, 180, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? -.07f : .07f, 3.33287f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1bc3, 49, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1be3, 5, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 2, 6, 1, 12, delay + 5);
    EVT_AT_SetCam(7704, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVA004", 1, 5);
    EVT_AT_SetVoice("S2BVB023", 1, 35);
}
void Q7QbThirdAttackFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetAngY(p.player, p.player, 180, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? -.46434f : .46434f, -5.3143f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1c20, 1, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bf2, 1, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7717, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVB008", 1, 20);
    EVT_AT_SetVoice("S2BVA012", 1, 50);
}
void Q7QbFourthAttackBegin(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.player, Q7QB_BASE, -1.4f, -2.78f, 180, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, 0, -3.67f, mirrored ? -180.0f : 180.0f, delay);
    EVT_AT_SetMot(p.player, 0x1bc4, 3, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1be4, 0, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7705, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
}
void Q7QbFourthAttackSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, 0, -3.67f, mirrored ? -180.0f : 180.0f, delay);
    EVT_AT_SetMot(p.player, 0x1bc4, 23, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1be4, 20, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 6, 12, 1, 12, delay + 7);
    EVT_AT_DispBlur(p.player, 3, 8, 1, 15, delay + 20);
    EVT_AT_SetCam(7706, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVA003", 1, 5);
    EVT_AT_SetVoice("S2BVB025", 1, 30);
}
void Q7QbFourthAttackFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? -.1054f : .1054f, -1.623f, mirrored ? -180.0f : 180.0f, delay);
    EVT_AT_SetMot(p.player, 0x1bd5, 29, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bf5, 3, 1, p.motionFlags, delay);
    EVT_AT_SetSound(650, delay + 8);
    EVT_AT_SetCam(7718, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVB001", 1, 5);
    EVT_AT_SetVoice("S2BVA011", 1, 15);
}
void Q7QbFifthAttackBegin(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.player, Q7QB_BASE, -.48f, -3.79f, 180, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? -.77329f : .77329f, 5.11953f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1bc5, 0, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1be5, 0, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7707, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
}
void Q7QbFifthAttackSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? -.77329f : .77329f, 5.11953f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1bc5, 23, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1be5, 23, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1, 6, 1, 15, delay);
    EVT_AT_DispBlur(p.player, 3, 6, 1, 15, delay + 30);
    EVT_AT_SetCam(7708, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVB007", 1, 15); // @text EN: "Take this!" | JA: "くらえっ！"
    EVT_AT_SetVoice("S2BVA005", 1, 30);
    EVT_AT_SetVoice("S2BVB017", 1, 60);
}
void Q7QbFifthAttackFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, .39917f, mirrored ? -5.33619f : 5.33619f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1bd8, 23, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bf7, 23, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7719, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVB013", 1, 5);
    EVT_AT_SetVoice("S2BVA006", 1, 50);
}
void Q7QbSixthAttackBegin(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.player, p.player, .5f, 0, 0, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .69143f : -.69143f, .00154f, 0, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? .21262f : -.21262f, -7.3741f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1bc6, 22, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bfe, 22, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 0x1bff, 22, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7709, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
}
void Q7QbSixthAttackSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .69143f : -.69143f, .00154f, 0, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? .21262f : -.21262f, -7.3741f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1bc6, 42, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bfe, 42, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 0x1bff, 42, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 6, 12, 1, 15, delay + 5);
    EVT_AT_DispBlur(p.player, 1, 8, 1, 20, delay + 20);
    EVT_AT_SetCam(7710, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(7724, Q7QB_CAMERA_DUMMY, Q7QB_CAMERA_DUMMY, 0, 0, delay + 30);
    EVT_AT_SetVoice("S2BVA004", 1, 5);
    EVT_AT_SetVoice("S2BVB018", 1, 10);
    EVT_AT_SetVoice("S2BVB024", 1, 50);
}
void Q7QbSixthAttackFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetAngY(p.player, p.player, 90, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? -7.00693f : 7.00693f, -.29555f, 0, delay);
    EVT_AT_SetMot(p.player, 0x1c1d, 30, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1bfe, 30, 0, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 0x1bfa, 33, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7720, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EV_PlayDialogue("S2BVB006");
    EVT_AT_SetVoice("S2BVA011", 1, 50);
}
void Q7QbFinalAttackBegin(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.player, Q7QB_BASE, -.89f, -4.82f, 0, delay);
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .91827f : -.91827f, -1.24702f, 180, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? -.37f : .37f, -1.96f, 180, delay);
    EVT_AT_SetMot(p.player, 0x1bc9, 0, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1c02, 0, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 0x1c06, 0, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7711, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
}
void Q7QbFinalAttackSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .91827f : -.91827f, -1.24702f, 180, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? -.37f : .37f, -1.96f, 180, delay);
    EVT_AT_SetMot(p.player, 0x1bc9, 18, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1c02, 18, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 0x1c06, 18, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 2, 12, 1, 10, delay + 10);
    EVT_AT_SetCam(7712, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVA005", 1, 5);
    EVT_AT_SetVoice("S2BVA002", 1, 15);
}
void Q7QbFinalAttackFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .56f : -.56f, -1.15f, 180, delay);
    const std::int32_t counterDelay = delay + 23;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .16286001f : -.16286001f, -1.52514f, 180, counterDelay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? -.57f : .57f, -1.81f, mirrored ? -165.0f : 165.0f, delay);
    EVT_AT_SetMot(p.player, 0x1bdf, 18, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.player, 0x1c1a, 60, 1, p.motionFlags & 0xfbffffffU, counterDelay);
    EVT_AT_SetSound(650, delay + 17);
    EVT_AT_SetMot(p.firstOpponent, 0x1c02, 18, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1c13, 13, 1, p.motionFlags ^ 0x20000000U, counterDelay);
    EVT_AT_SetMot(p.secondOpponent, 0x1c06, 18, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 13, 0, 1, p.motionFlags, counterDelay);
    EVT_AT_SetCam(7721, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(7722, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, counterDelay);
    EVT_AT_SetVoice("S2BVB001", 1, 5);
    EVT_AT_SetVoice("S2BVC004", 1, 15);
    EVT_AT_SetVoice("S2BVB013", 1, 30);
    EVT_AT_SetVoice("S2BVA011", 1, 55);
}
void Q7QbFinalCounterSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .72f : -.72f, -1.49f, 180, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? -.21f : .21f, -.85f, 180, delay);
    EVT_AT_SetMot(p.player, 0x1bc8, 6, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1c01, 0, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 0x1c05, 1, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3, 12, 1, 10, delay);
    EVT_AT_SetCam(7713, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
}
void Q7QbFinalCounterFailure(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .16286001f : -.16286001f, -1.52514f, 180, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? -.57f : .57f, -1.81f, mirrored ? -165.0f : 165.0f, delay);
    EVT_AT_SetMot(p.player, 0x1c1a, 60, 1, p.motionFlags & 0xfbffffffU, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1c13, 13, 1, p.motionFlags ^ 0x20000000U, delay);
    EVT_AT_SetMot(p.secondOpponent, 13, 0, 1, p.motionFlags, delay);
    EVT_AT_SetCam(7722, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVB013", 1, 5);
    EVT_AT_SetVoice("S2BVA011", 1, 20);
}
void Q7QbBattleSuccess(void *data, std::uint32_t delayFrames) {
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    Q7QbResultParameters &p = *static_cast<Q7QbResultParameters *>(data);
    const bool mirrored = (p.motionFlags & 0x20000000U) != 0;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, mirrored ? .07f : -.07f, -1.23f, 180, delay);
    EVT_AT_SetPosAng(p.secondOpponent, p.player, mirrored ? -1.16f : 1.16f, -.63f, 180, delay);
    EVT_AT_SetMot(p.player, 0x1bca, 2, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.firstOpponent, 0x1c03, 0, 1, p.motionFlags, delay);
    EVT_AT_SetMot(p.secondOpponent, 0x1c07, 2, 1, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3, 12, 1, 15, delay + 5);
    EVT_AT_SetCam(7714, Q7QB_DUMMY, Q7QB_DUMMY, 0, 0, delay);
    EVT_AT_SetVoice("S2BVA004", 1, 5);
    EVT_AT_SetVoice("S2BVB031", 1, 20);
}
} // namespace shenmue::s2::areas
