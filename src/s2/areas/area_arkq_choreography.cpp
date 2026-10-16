#include "s2/areas/area_arkq.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/particle_effects.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2::areas {
void QueueArkqKnifeAttachment(std::int32_t bone, const Vector3 &position, std::int32_t delay) {
    if (delay) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), ARKQ_EVENT);
        RegisterEventTask(task);
        *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1,
            .target = ARKQ_HAND_ANCHOR,
            .character = ARKQ_REN,
            .bone = bone,
            .position = position,
            .rotation = ARKQ_KNIFE_ROTATION,
            .mode = 0,
            .delay = static_cast<float>(delay)};
    } else {
        EventCling attachment{.cleanup = 0,
            .target = ARKQ_HAND_ANCHOR,
            .character = ARKQ_REN,
            .bone = bone,
            .position = position,
            .rotation = ARKQ_KNIFE_ROTATION,
            .mode = 0,
            .delay = 0};
        EVT_AT_Cling(&attachment);
    }
}
std::int32_t ArkqBeginKnifeDisplay(ArkqWork &work) {
    for (std::uint32_t &effect : arkqGlobals.glints) {
        if (effect != 0xffffffffU) {
            FreeParticleEffect(0, effect);
            effect = 0xffffffffU;
            effect = CreateArkqKnifeGlint();
            ResumeParticleEffect(0, effect);
        }
    }
    EVT_AT_SetCam(2002, ARKQ_BASE, ARKQ_BASE, 0, 0, 0);
    EVT_AT_SetCam(2001, ARKQ_BASE, ARKQ_BASE, 0, 0, 45);
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, ARKQ_KNIFE_OFFSET, 0);
    EventSetRawAngle angle{.cleanup = 0, .character = ARKQ_KNIFE, .parent = ARKQ_HAND_ANCHOR, .rotation = {.x = 0, .y = 0x8000, .z = 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    QueueArkqKnifeAttachment(14, {.x = .09f, .y = .05f, .z = 0}, 0);
    EVT_AT_SetMot(ARKQ_REN, 0x7f07, 610, 1, 0, 0);
    EVT_AT_SetMot(ARKQ_REN, 0x7f07, 640, 0, 0x40000000U, 45);
    EVT_AT_SetVoice("0057B003", 1, 10); // @text EN: "May the gods be with you=@" | JA: "神のご加護を…"
    HLib::Task *turn = HLib::EnqueueTaskWithoutParameter(ArkqTurnKnifeCallback, 4, 0x0b, static_cast<std::int32_t>(ARKQ_TURN_TASK.name));
    HLib::Task *light = HLib::EnqueueTaskWithoutParameter(ArkqKnifeLightCallback, 4, 0x0b, static_cast<std::int32_t>(ARKQ_LIGHT_TASK.name));
    HLib::SetupTimedTask(turn, 150);
    HLib::SetupTimedTask(light, 155);
    work.state = 4;
    return 145;
}
std::int32_t ArkqFirstDodge() {
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 1, reinterpret_cast<std::byte *>(arkqKnifeHandPose), 0, 20);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 0, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0, 20);
    QueueArkqKnifeAttachment(14, {.x = .09f, .y = .05f, .z = 0}, 0);
    QueueArkqKnifeAttachment(21, {.x = .11f, .y = .03f, .z = .03f}, 20);
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, ARKQ_KNIFE_OFFSET, 0);
    EVT_AT_SetAng_Enqueue(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 180, 180, 0, 0);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, {.x = 6.5f, .y = 0, .z = -8.2f}, 0);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 30, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, {.x = -.36f, .y = 0, .z = -1.9f}, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, {.x = -0.0f, .y = 0, .z = -1.2f}, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0, 0);
    EVT_AT_SetMot(ARKQ_RYO, 0x1712, 30, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARKQ_REN, 0x170c, 30, 1, 0x40000000U, 0);
    EVT_AT_SetVoice("0057A020", 1, 15);
    EVT_AT_SetCam(3000, ARKQ_BASE, ARKQ_BASE, 0, 0, 0);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 25, 0);
    return 25;
}
std::int32_t ArkqFirstFailure(std::int32_t delay) {
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, ARKQ_KNIFE_OFFSET, 0);
    EventSetRawAngle angle{.cleanup = 0, .character = ARKQ_KNIFE, .parent = ARKQ_HAND_ANCHOR, .rotation = {.x = 0x8000, .y = 0x8000, .z = 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    QueueArkqKnifeAttachment(14, {.x = .09f, .y = .005f, .z = 0}, 0);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, {.x = 6.4682f, .y = 0, .z = -8.2266f}, delay);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 389.94629f, delay);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, {.x = 0, .y = 0, .z = -1.1f}, delay);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0, delay);
    EVT_AT_SetMot(ARKQ_RYO, 0x1710, 0, 1, 0x40000000U, delay);
    EVT_AT_SetMot(ARKQ_REN, 0x1709, 0, 1, 0x40000000U, delay);
    EVT_AT_SetCam(3013, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(3014, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 30);
    EVT_AT_SetCam(3015, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 45);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 20, delay + 20);
    EVT_AT_DispBlur(ARKQ_REN, 7, 15, 1, 30, delay + 45);
    EVT_AT_SetVoice("0057A023", 1, delay + 30);
    EVT_AT_SetVoice("0057B009", 1, delay + 45);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 6, 15, 1, delay + 30);
    return delay + 90;
}
std::int32_t ArkqSecondDodge(std::int32_t delay) {
    EVT_AT_SetCam(3002, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    QueueArkqKnifeAttachment(21, {.x = .11f, .y = .03f, .z = .03f}, delay + 20);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 25, delay);
    EVT_AT_SetVoice("0057A021", 1, delay + 20);
    return delay + 25;
}
std::int32_t ArkqSecondFailure(std::int32_t delay) {
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, {.x = 0, .y = 0, .z = -1.3f}, delay);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0, delay);
    EVT_AT_SetMot(ARKQ_RYO, 0x1718, 0, 1, 0x40000000U, delay);
    EVT_AT_SetMot(ARKQ_REN, 0x170a, 0, 1, 0x40000000U, delay);
    EVT_AT_SetCam(3011, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(3012, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 20);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 15, delay + 5);
    EVT_AT_DispBlur(ARKQ_REN, 7, 15, 1, 30, delay + 20);
    EVT_AT_SetVoice("0057A024", 1, delay + 10);
    EVT_AT_SetVoice("0057B009", 1, delay + 35);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 6, 15, 1, delay + 20);
    return delay + 60;
}
std::int32_t ArkqThirdFailure(std::int32_t delay) {
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, {.x = -.5f, .y = 0, .z = -1.1f}, delay);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0, delay);
    EVT_AT_SetMot(ARKQ_RYO, 0x1711, 0, 1, 0x40000000U, delay);
    EVT_AT_SetMot(ARKQ_REN, 0x170b, 0, 1, 0x40000000U, delay);
    EVT_AT_SetCam(3008, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(3009, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 25);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 65, delay);
    EVT_AT_SetVoice("0057A025", 1, delay + 10);
    EVT_AT_SetVoice("0057B009", 1, delay + 35);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 6, 15, 1, delay + 25);
    return delay + 65;
}
std::int32_t ArkqAllDodges() {
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(ARKQ_RYO), 0, 0, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(ARKQ_REN), 0, 0, 1);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 1, 5, 1, 30);
    EVT_AT_SetFaceLooks(ARKQ_REN, 1, 5, 1, 55);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, ARKQ_KNIFE_OFFSET, 0);
    EventSetRawAngle angle{.cleanup = 0, .character = ARKQ_KNIFE, .parent = ARKQ_HAND_ANCHOR, .rotation = {.x = 0x8000, .y = 0x8000, .z = 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    QueueArkqKnifeAttachment(14, {.x = .09f, .y = .005f, .z = 0}, 0);
    QueueArkqKnifeAttachment(21, {.x = .11f, .y = .03f, .z = .03f}, 45);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 1, reinterpret_cast<std::byte *>(arkqKnifeHandPose), 0, 45);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 0, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0, 45);
    EVT_AT_ObjectDisp_1(ARKQ_KNIFE, 80, 1);
    EVT_AT_ObjectDisp_1(ARKQ_HAND_ANCHOR, 80, 1);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, {.x = 6.5f, .y = 0, .z = -8.2f}, 0);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 30, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, {.x = .1f, .y = 0, .z = -1.15f}, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0, 0);
    EVT_AT_SetMot(ARKQ_RYO, 0x1712, 30, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARKQ_REN, 0x170c, 30, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARKQ_RYO, 0x1712, 55, 0, 0x40000000U, 25);
    EVT_AT_SetMot(ARKQ_REN, 0x170c, 55, 0, 0x40000000U, 25);
    EVT_AT_SetMot(ARKQ_RYO, 0x1712, 55, 1, 0x40000000U, 35);
    EVT_AT_SetMot(ARKQ_REN, 0x170c, 55, 1, 0x40000000U, 35);
    EVT_AT_SetMot(ARKQ_RYO, 0x1712, 80, 0, 0x40000000U, 60);
    EVT_AT_SetMot(ARKQ_REN, 0x170c, 80, 0, 0x40000000U, 60);
    EVT_AT_SetMot(ARKQ_RYO, 0x1712, 80, 1, 0x40000000U, 70);
    EVT_AT_SetMot(ARKQ_REN, 0x170c, 80, 1, 0x40000000U, 70);
    EVT_AT_SetVoice("0057A020", 1, 15);
    EVT_AT_SetVoice("0057A021", 1, 45);
    EVT_AT_SetVoice("0057A022", 1, 70);
    EVT_AT_DispBlur(ARKQ_REN, 4, 7, 1, 20, 10);
    EVT_AT_DispBlur(ARKQ_REN, 4, 7, 1, 15, 35);
    EVT_AT_DispBlur(ARKQ_REN, 5, 7, 1, 15, 60);
    EVT_AT_DispBlur(ARKQ_RYO, 1, 10, 1, 15, 30);
    EVT_AT_DispBlur(ARKQ_RYO, 1, 10, 1, 25, 70);
    EVT_AT_SetCam(3000, ARKQ_BASE, ARKQ_BASE, 0, 0, 0);
    EVT_AT_SetCam(3001, ARKQ_BASE, ARKQ_BASE, 0, 0, 25);
    EVT_AT_SetCam(3002, ARKQ_BASE, ARKQ_BASE, 0, 0, 35);
    EVT_AT_SetCam(3003, ARKQ_BASE, ARKQ_BASE, 0, 0, 50);
    EVT_AT_SetCam(3004, ARKQ_BASE, ARKQ_BASE, 0, 0, 60);
    EVT_AT_SetCam(3005, ARKQ_BASE, ARKQ_BASE, 0, 0, 80);
    EVT_AT_SetCam(3006, ARKQ_BASE, ARKQ_BASE, 0, 0, 130);
    EVT_AT_SetVoice("0057B005", 1, 130); // @text EN: "All right.\nYou win=@" | JA: "オーライ、わかった…わかったよ。"
    return 230;
}
} // namespace shenmue::s2::areas
