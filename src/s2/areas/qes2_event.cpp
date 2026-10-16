#include "s2/areas/qes2_event.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/face.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <xmmintrin.h>
namespace shenmue::s2 {
SoundEffectHandle *qes2SoundHandle;
float qes2SoundIntensity, qes2FloorHeight;
std::int16_t qes2EntryMode, qes2FirstPairedPhase, qes2SecondPairedPhase;
shen_name qes2FirstPairedCharacters[3], qes2SecondPairedCharacters[3];
void Qes2SoundIntensityCallback(void *data) {
    QesSoundIntensityWork *const work = static_cast<QesSoundIntensityWork *>(data);
    if (work->phase) {
        qes2SoundIntensity += work->step;
        if (--work->frames <= 0) {
            qes2SoundIntensity = static_cast<float>(work->target);
            HLib::DestroyCurrentTask();
        }
    } else {
        work->phase = 1;
        qes2SoundIntensity = static_cast<float>(work->initial);
        if (work->initial == work->target)
            HLib::DestroyCurrentTask();
    }
    ExecuteMaterialSoundHandleCommand(qes2SoundHandle, 0xa6, _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity)));
}
namespace {
std::int32_t Qes2PairFirstSuccess() {
    EVT_AT_DispBlur(qes2FirstPairedCharacters[0], 8, 10, 1, 20, 0);
    EV_EnableCharacterDisplay(qes2FirstPairedCharacters[2]);
    EVT_AT_SetPos(qes2FirstPairedCharacters[2], QES_BASE, QES2_PAIR_SECOND_POSITION, 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[2], QES_BASE, 180, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[2], 8190, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice("S2BVA005", 1, 20);
    return 0;
}
std::int32_t Qes2PairFirstFailure() {
    EVT_AT_SetPos(qes2FirstPairedCharacters[0], QES_BASE, {0, 0, 2.5f}, 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[0], QES_BASE, 0, 0);
    EVT_AT_SetPos(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], {0, 0, 0}, 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8157, 40, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[1], 8172, 40, 1, 0x44000000U, 0);
    EVT_AT_SetFaceLooks(qes2FirstPairedCharacters[0], FacialExpressionId::Suffer, 8, 1, 10);
    EVT_AT_SetFaceLooks(qes2FirstPairedCharacters[0], FacialExpressionId::Angry, 8, 1, 30);
    EVT_AT_SetVoice("S2BVB008", 1, 0);
    EVT_AT_SetCam(7013, QES_BASE, QES_BASE, 0, 0, 0);
    return 30;
}
std::int32_t Qes2PairSplitSuccess() {
    EVT_AT_SetPos(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], QES2_PAIR_SPLIT_POSITIONS[0], 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], 0, 0);
    EVT_AT_SetPos(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], QES2_PAIR_SPLIT_POSITIONS[1], 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8161, 0, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[1], 8174, 0, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[2], 8188, 0, 1, 0x44000000U, 0);
    EVT_AT_DispBlur(qes2FirstPairedCharacters[0], 6, 10, 2, 5, 0);
    EVT_AT_DispBlur(qes2FirstPairedCharacters[0], 6, 10, 2, 10, 30);
    EVT_AT_ObjectDisp_1(QES_REN, 30, 1);
    EVT_AT_SetCam(7003, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(7004, QES_BASE, QES_BASE, 0, 0, 30);
    EVT_AT_SetCam(7015, QES_BASE, QES_BASE, 0, 0, 60);
    EVT_AT_SetVoice("S2BVC020", 1, 40);
    EVT_AT_SetVoice("S2BVB022", 1, 50);
    return 100;
}
std::int32_t Qes2PairDoubleFailure() {
    EVT_AT_SetPos(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], QES2_PAIR_DOUBLE_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8163, 30, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[1], 8176, 30, 1, 0x44000000U, 0);
    EVT_AT_SetCam(7002, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVB009", 1, 0);
    EVT_AT_SetFaceLooks(qes2FirstPairedCharacters[0], FacialExpressionId::Suffer, 8, 1, 5);
    return 60;
}
std::int32_t Qes2PairSecondRecovery() {
    EVT_AT_DispBlur(qes2FirstPairedCharacters[0], 1, 10, 2, 15, 0);
    EVT_AT_DispBlur(qes2FirstPairedCharacters[0], 4, 10, 2, 20, 20);
    EVT_AT_SetCam(7011, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(7014, QES_BASE, QES_BASE, 0, 0, 40);
    EVT_AT_SetFaceLooks(qes2FirstPairedCharacters[2], FacialExpressionId::Suffer, 8, 1, 5);
    EVT_AT_SetVoice("S2BVA002", 1, 0);
    EVT_AT_SetVoice("S2BVC023", 1, 35);
    return 60;
}
std::int32_t Qes2PairSecondFailure() {
    EVT_AT_SetCam(7006, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetPos(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], QES2_PAIR_SECOND_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8159, 10, 1, 0, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[2], 8186, 10, 1, 0, 0);
    EVT_AT_SetVoice("S2BVA009", 1, 15);
    return 45;
}
std::int32_t Qes2PairCounterSuccess() {
    EVT_AT_DispBlur(qes2FirstPairedCharacters[0], 1, 10, 2, 20, 0);
    EVT_AT_SetCam(7008, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVC004", 1, 0);
    EVT_AT_SetVoice("S2BVA003", 1, 10);
    return 35;
}
std::int32_t Qes2PairCounterFailure() {
    EVT_AT_SetAngY(qes2FirstPairedCharacters[0], qes2FirstPairedCharacters[0], -45, 0);
    EVT_AT_SetPos(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], QES2_PAIR_COUNTER_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8145, 20, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2FirstPairedCharacters[2], 8180, 20, 1, 0x44000000U, 0);
    EVT_AT_SetFaceLooks(qes2FirstPairedCharacters[0], FacialExpressionId::Suffer, 8, 1, 5);
    EVT_AT_SetCam(7010, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(7012, QES_BASE, QES_BASE, 0, 0, 30);
    EVT_AT_SetVoice("S2BVC013", 1, 0);
    EVT_AT_SetVoice("S2BVA012", 1, 30);
    return 60;
}
std::int32_t Qes2SecondPairFirstSuccess() {
    EVT_AT_DispBlur(qes2SecondPairedCharacters[0], 5, 15, 2, 30, 0);
    EVT_AT_SetCam(8000, QES_BASE, QES_BASE, 0, 0, 30);
    EVT_AT_SetPos(qes2SecondPairedCharacters[2], QES_BASE, {-2.2f, 0, 1.3f}, 30);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[2], QES_BASE, 270, 30);
    EVT_AT_SetMot(qes2SecondPairedCharacters[2], 8189, 0, 1, 0x40000000U, 30);
    EVT_AT_SetVoice("S2BVA001", 1, 0);
    EVT_AT_SetVoice("S2BVC023", 1, 40);
    return 50;
}
std::int32_t Qes2SecondPairFirstFailure() {
    EVT_AT_SetPos(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], QES2_SECOND_PAIR_FIRST_POSITION, 0);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], 0, 0);
    EVT_AT_SetPos(qes2SecondPairedCharacters[2], QES_BASE, QES2_SECOND_PAIR_WAIT_POSITION, 0);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[2], QES_BASE, 270, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8156, 20, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[1], 8171, 20, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[2], 8189, 0, 1, 0x40000000U, 0);
    EVT_AT_SetCam(8002, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVC007", 1, 0);
    return 20;
}
std::int32_t Qes2SecondPairSplitSuccess() {
    EVT_AT_DispBlur(qes2SecondPairedCharacters[0], 5, 15, 2, 10, 0);
    EVT_AT_DispBlur(qes2SecondPairedCharacters[0], 6, 15, 2, 10, 30);
    EVT_AT_SetPos(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], QES2_SECOND_PAIR_SPLIT_POSITION, 0);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[2], 8187, 15, 1, 0x44000000U, 0);
    EVT_AT_SetCam(8004, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(8017, QES_BASE, QES_BASE, 0, 0, 45);
    EVT_AT_ObjectDisp_1(QES_REN, 45, 1);
    EVT_AT_SetVoice("S2BVC029", 1, 10);
    EVT_AT_SetVoice("S2BVB027", 1, 40);
    return 90;
}
std::int32_t Qes2SecondPairDoubleFailure() {
    EVT_AT_SetPos(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], QES2_SECOND_PAIR_DOUBLE_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8162, 15, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[1], 8175, 15, 1, 0x44000000U, 0);
    EVT_AT_SetCam(8003, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVA012", 1, 0);
    return 60;
}
std::int32_t Qes2SecondPairRecovery() {
    EVT_AT_SetPos(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], QES2_SECOND_PAIR_RECOVERY_POSITION, 0);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8153, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[2], 8183, 0, 1, 0x40000000U, 0);
    EVT_AT_DispBlur(qes2SecondPairedCharacters[0], 6, 15, 2, 10, 10);
    EVT_AT_SetCam(8008, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(8015, QES_BASE, QES_BASE, 0, 0, 30);
    EVT_AT_SetVoice("S2BVA002", 1, 5);
    EVT_AT_SetVoice("S2BVC026", 1, 35);
    return 80;
}
std::int32_t Qes2SecondPairFailure() {
    EVT_AT_SetPos(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], QES2_SECOND_PAIR_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8158, 10, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[2], 8185, 10, 1, 0x44000000U, 0);
    EVT_AT_SetCam(8005, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVB011", 1, 10);
    return 30;
}
std::int32_t Qes2SecondPairCounterSuccess() {
    EVT_AT_DispBlur(qes2SecondPairedCharacters[0], 6, 15, 2, 10, 5);
    EVT_AT_SetCam(8009, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(8016, QES_BASE, QES_BASE, 0, 0, 20);
    EVT_AT_SetVoice("S2BVC019", 1, 10);
    EVT_AT_SetVoice("S2BVC030", 1, 15);
    return 50;
}
std::int32_t Qes2SecondPairCounterFailure() {
    EVT_AT_SetPos(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], QES2_SECOND_PAIR_COUNTER_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8144, 10, 1, 0x44000000U, 0);
    EVT_AT_SetMot(qes2SecondPairedCharacters[2], 8179, 10, 1, 0x44000000U, 0);
    EVT_AT_SetCam(8012, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVB007", 1, 15); // @text EN: "Take this!" | JA: "くらえっ！"
    return 60;
}
} // namespace
void InitializeQes2FirstPairedSequence(float x, float z, float degrees) {
    qes2FirstPairedPhase = 0;
    EVT_AT_SetPos(QES_BASE, QES_INVALID, {x, qes2FloorHeight, z}, 0);
    EVT_AT_SetAngY(QES_BASE, QES_INVALID, degrees, 0);
}
std::int32_t UpdateQes2FirstPairedSequence(std::int32_t *result) {
    switch (qes2FirstPairedPhase) {
    case 0:
        EV_EnableCharacterDisplay(qes2FirstPairedCharacters[1]);
        EV_DisableCharacterDisplay(qes2FirstPairedCharacters[2]);
        EVT_AT_SetPos(qes2FirstPairedCharacters[0], QES_BASE, {0, 0, 2.5f}, 0);
        EVT_AT_SetAngY(qes2FirstPairedCharacters[0], QES_BASE, 0, 0);
        EVT_AT_SetPos(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], QES2_PAIR_FIRST_POSITION, 0);
        EVT_AT_SetAngY(qes2FirstPairedCharacters[1], qes2FirstPairedCharacters[0], 0, 0);
        EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8152, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(qes2FirstPairedCharacters[1], 8170, 0, 1, 0x40000000U, 0);
        QueueSkQteWait(result, 256, 24, 16, Qes2PairFirstSuccess, Qes2PairFirstFailure, 10);
        EVT_AT_SetCam(7000, QES_BASE, QES_BASE, 0, 0, 0);
        qes2FirstPairedPhase = 1;
        break;
    case 1:
        qes2FirstPairedPhase = result[3] ? 4 : 2;
        break;
    case 2:
        EV_EnableCharacterDisplay(qes2FirstPairedCharacters[2]);
        EVT_AT_SetPos(qes2FirstPairedCharacters[2], QES_BASE, QES2_PAIR_SECOND_POSITION, 0);
        EVT_AT_SetAngY(qes2FirstPairedCharacters[2], QES_BASE, 180, 0);
        EVT_AT_SetMot(qes2FirstPairedCharacters[2], 8190, 0, 1, 0x40000000U, 0);
        QueueSkQteWait(result, 512, 24, 16, Qes2PairSplitSuccess, Qes2PairDoubleFailure, 0);
        EVT_AT_SetCam(7001, QES_BASE, QES_BASE, 0, 0, 0);
        qes2FirstPairedPhase = 3;
        break;
    case 3:
    case 7:
        qes2FirstPairedPhase = result[3] ? 8 : 9;
        break;
    case 4:
        EVT_AT_SetPos(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], {0.5f, 0, -2}, 30);
        EVT_AT_SetAngY(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], 0, 30);
        EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8154, 0, 1, 0x40000000U, 30);
        EVT_AT_SetMot(qes2FirstPairedCharacters[2], 8184, 0, 1, 0x40000000U, 30);
        QueueSkQteWait(result, 64, 24, 16, Qes2PairSecondRecovery, Qes2PairSecondFailure, 20);
        EVT_AT_SetCam(7005, QES_BASE, QES_BASE, 0, 0, 30);
        qes2FirstPairedPhase = 5;
        break;
    case 5:
        qes2FirstPairedPhase = result[3] ? 8 : 6;
        break;
    case 6:
        EVT_AT_SetPos(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], QES2_PAIR_COUNTER_POSITION, 0);
        EVT_AT_SetAngY(qes2FirstPairedCharacters[2], qes2FirstPairedCharacters[0], 0, 0);
        EVT_AT_SetMot(qes2FirstPairedCharacters[0], 8143, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(qes2FirstPairedCharacters[2], 8178, 0, 1, 0x40000000U, 0);
        EVT_AT_SetCam(7007, QES_BASE, QES_BASE, 0, 0, 0);
        QueueSkQteWait(result, 512, 24, 16, Qes2PairCounterSuccess, Qes2PairCounterFailure, 0);
        qes2FirstPairedPhase = 7;
        break;
    case 8:
    case 9:
        RemoveConversationSpeakerAlias(qes2FirstPairedCharacters[1]);
        RemoveConversationSpeakerAlias(qes2FirstPairedCharacters[2]);
        return qes2FirstPairedPhase == 8 ? 1 : 0;
    default:
        break;
    }
    return -1;
}
void InitializeQes2SecondPairedSequence(float x, float z, float degrees) {
    qes2SecondPairedPhase = 0;
    EVT_AT_SetPos(QES_BASE, QES_INVALID, {x, qes2FloorHeight, z}, 0);
    EVT_AT_SetAngY(QES_BASE, QES_INVALID, degrees, 0);
}
std::int32_t UpdateQes2SecondPairedSequence(std::int32_t *result) {
    switch (qes2SecondPairedPhase) {
    case 0:
        EV_EnableCharacterDisplay(qes2SecondPairedCharacters[1]);
        EV_EnableCharacterDisplay(qes2SecondPairedCharacters[2]);
        EVT_AT_SetPos(qes2SecondPairedCharacters[0], QES_BASE, {18.5f, 0, -7}, 0);
        EVT_AT_SetAngY(qes2SecondPairedCharacters[0], QES_BASE, 180, 0);
        EVT_AT_SetPos(qes2SecondPairedCharacters[0], QES_BASE, {0, 0, -2.5f}, 0);
        EVT_AT_SetAngY(qes2SecondPairedCharacters[0], QES_BASE, 180, 0);
        EVT_AT_SetPos(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], QES2_SECOND_PAIR_FIRST_POSITION, 0);
        EVT_AT_SetAngY(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], 0, 0);
        EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8151, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(qes2SecondPairedCharacters[1], 8169, 0, 1, 0x40000000U, 0);
        EVT_AT_SetCam(8011, QES_BASE, QES_BASE, 0, 0, 0);
        QueueSkQteWait(result, 256, 24, 16, Qes2SecondPairFirstSuccess, Qes2SecondPairFirstFailure, 0);
        qes2SecondPairedPhase = 1;
        break;
    case 1:
        qes2SecondPairedPhase = result[3] ? 4 : 2;
        break;
    case 2:
        EVT_AT_SetPos(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], {0.4f, 0, -1.7f}, 10);
        EVT_AT_SetAngY(qes2SecondPairedCharacters[1], qes2SecondPairedCharacters[0], 0, 10);
        EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8160, 0, 1, 0, 10);
        EVT_AT_SetMot(qes2SecondPairedCharacters[1], 8173, 0, 1, 0, 10);
        QueueSkQteWait(result, 512, 24, 16, Qes2SecondPairSplitSuccess, Qes2SecondPairDoubleFailure, 0);
        qes2SecondPairedPhase = 3;
        break;
    case 3:
    case 7:
        qes2SecondPairedPhase = result[3] ? 8 : 9;
        break;
    case 4:
        EVT_AT_SetCam(8013, QES_BASE, QES_BASE, 0, 0, 0);
        QueueSkQteWait(result, 4, 24, 16, Qes2SecondPairRecovery, Qes2SecondPairFailure, 20);
        qes2SecondPairedPhase = 5;
        break;
    case 5:
        qes2SecondPairedPhase = result[3] ? 8 : 6;
        break;
    case 6:
        EVT_AT_SetPos(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], {-0.2f, 0, -1.8f}, 20);
        EVT_AT_SetAngY(qes2SecondPairedCharacters[2], qes2SecondPairedCharacters[0], 0, 20);
        EVT_AT_SetMot(qes2SecondPairedCharacters[0], 8142, 0, 1, 0x40000000U, 20);
        EVT_AT_SetMot(qes2SecondPairedCharacters[2], 8177, 0, 1, 0x40000000U, 20);
        QueueSkQteWait(result, 512, 24, 16, Qes2SecondPairCounterSuccess, Qes2SecondPairCounterFailure, 0);
        qes2SecondPairedPhase = 7;
        break;
    case 8:
    case 9:
        RemoveConversationSpeakerAlias(qes2SecondPairedCharacters[1]);
        RemoveConversationSpeakerAlias(qes2SecondPairedCharacters[2]);
        return qes2SecondPairedPhase == 8 ? 1 : 0;
    default:
        break;
    }
    return -1;
}
} // namespace shenmue::s2
