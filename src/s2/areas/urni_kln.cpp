#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_urni.hpp"

#define URNI_CARD_TABLE SHEN_NAME4('H', 'A', 'R', '0')
#define URNI_CARD_DECK SHEN_NAME4('H', 'A', 'R', '1')
#define URNI_PLAYER_CARD SHEN_NAME4('H', 'A', 'R', '2')
#define URNI_READER_CARD SHEN_NAME4('H', 'A', 'R', '3')
#define URNI_KLN_TALK_MOTION 6616
#define URNI_KLN_RESULT_MOTION 6617
#define URNI_KLN_DECLINE_MOTION 6619
#define URNI_KLN_READING_MOTION 6621
#define URNI_PLAYER_CARD_READING_MOTION 6622

typedef struct UranaiCardRotation {
    float baseYaw, reserved04, stepAngle, spinAngle, amplitude;
} UranaiCardRotation;

static int UranaiKlnQueueMessages(HLibTask *parent, const char **messages, int voiceDelayFrames, unsigned int mode, int delay)
{
    int offset = 0;
    while (*messages) {
        QueueEventMessage(*messages++, delay + offset, 60);
        offset += 60;
    }
    return offset;
}

static const char *uranaiKlnModeOneVoices[] = {"G001", "G015", 0};
static const char *uranaiKlnIntroductionVoices[] = {"A001", "G002", 0};
static const char *uranaiKlnSameVoices[] = {"A001", "G010", "G011", "A010", 0};
static const char *uranaiKlnDeclineVoices[] = {"A003", "G003", "A077", 0};
static const char *uranaiKlnBeginVoices[] = {"A002", "G004", 0};
static const char *uranaiKlnReadingVoices[] = {"G005", 0};
static const char *uranaiKlnReadingEndVoices[] = {"G006", "G009", 0};
static const char *uranaiKlnPaymentVoices[] = {"A007", 0};
static const char *uranaiKlnInsufficientVoices[] = {"A020", "G012", "A131", "G013", "G014", "A132", 0};
static const char *uranaiKlnResultVoices[10][10] = {{"G017", "G018", 0}, {"G019", "G020", 0}, {"G021", "G022", 0}, {"G023", "G024", 0},
    {"G025", "G026", 0}, {"G027", "G028", 0}, {"G029", "G030", 0}, {"G031", "G032", 0}, {"G033", "G034", 0}, {"G035", "G036", 0}};
static void UranaiKlnRotateCards(void *data, int ticks);

static Vector3 uranaiKlnPlayerCardPosition = {0.133f, 0.035f, -0.041f};
static Vector3i uranaiKlnPlayerCardRotation = {35953, 46967, 0};
static Vector3 uranaiKlnReaderCardPosition = {0.243f, 0.03f, -0.01f};
static Vector3i uranaiKlnReaderCardRotation = {40504, 51518, 25484};

int UranaiKlnSetup(shen_name reader, int delay)
{
    EVT_AT_CharaDisp(URNI_CARD_DECK, 0, 0);
    EVT_AT_SetPos(URNI_CARD_TABLE, EVENT_CHARACTER_INVALID, 278.575012f, 100.810997f, 135.824997f, 0);
    EVT_AT_SetAngY(URNI_CARD_TABLE, EVENT_CHARACTER_INVALID, 270.0f, 0);
    EVT_AT_SetPos(URNI_CARD_DECK, EVENT_CHARACTER_INVALID, 278.494995f, 100.761002f, 136.184006f, 0);
    EVT_AT_SetAngY(URNI_CARD_DECK, EVENT_CHARACTER_INVALID, 270.0f, 0);
    Queue_EVT_AT_Cling(URNI_PLAYER_CARD, URNI_PLAYER, 14, &uranaiKlnPlayerCardPosition, &uranaiKlnPlayerCardRotation, 0, 0);
    Queue_EVT_AT_Cling(URNI_READER_CARD, reader, 14, &uranaiKlnReaderCardPosition, &uranaiKlnReaderCardRotation, 0, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(URNI_PLAYER), 0, (void *)areaLoopHandPoses[URNI_READING_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(URNI_PLAYER), 1, (void *)areaLoopHandPoses[URNI_READING_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_READING_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 1, (void *)areaLoopHandPoses[URNI_READING_HAND_POSE], 0);
    return 0;
}

int UranaiKlnModeOne(shen_name reader, int delay)
{
    int end = delay + 260;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiKlnModeOneVoices, 10, 0x80000000, delay + 30);
    EVT_AT_CharaDisp(URNI_PLAYER, 0, 1);
    EVT_AT_CharaDisp(URNI_PLAYER, end, 0);
    return end;
}

int UranaiKlnIntroduction(shen_name reader, int delay)
{
    int end = delay + 200;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(0, uranaiKlnIntroductionVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiKlnSame(shen_name reader, int delay)
{
    int leave = delay + 300;
    int turn, end;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, turn);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, turn);
    EVT_AT_SetMot(reader, URNI_KLN_DECLINE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay);
    QueueEventVoiceList(0, uranaiKlnSameVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiKlnDecline(shen_name reader, int delay)
{
    int leave = delay + 220;
    int turn, end;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, turn);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, turn);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay);
    QueueEventVoiceList(0, uranaiKlnDeclineVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiKlnBeginReading(shen_name reader, int delay)
{
    int end = delay + 180;
    int frames;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.5f, 180.0f, delay + 120);
    frames = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, delay + 120);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 200.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 120);
    EVT_AT_SetSound(993, delay + 120);
    QueueEventVoiceList(0, uranaiKlnBeginVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiKlnReading(shen_name reader, int delay)
{
    int end = delay + 1255 - 200.0f;
    int shuffle, deal, reveal;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.8f, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_CARD_READING_MOTION, 200.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_KLN_READING_MOTION, 200.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_KLN_READING_MOTION, 670.0f, 0.5f, 0, delay + 670 - 200.0f);
    shuffle = delay + 250 - 200.0f;
    EVT_AT_CharaDisp(URNI_CARD_DECK, shuffle, 1);
    EVT_AT_CharaDisp(URNI_READER_CARD, shuffle, 0);
    deal = delay + 340 - 200.0f;
    EVT_AT_CharaDisp(URNI_READER_CARD, deal, 1);
    EVT_AT_CharaDisp(URNI_PLAYER_CARD, deal, 0);
    reveal = delay + 570 - 200.0f;
    EVT_AT_CharaDisp(URNI_PLAYER_CARD, reveal, 1);
    EVT_AT_CharaDisp(URNI_CARD_TABLE, reveal, 0);
    EVT_AT_SetSound(992, reveal);
    EVT_AT_SetCam(1220, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1235, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, shuffle);
    EVT_AT_SetCam(1236, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, deal);
    EVT_AT_SetCam(1237, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, reveal);
    QueueEventVoiceList(0, uranaiKlnReadingVoices, 10, 0x80000000, delay);
    QueueEventVoiceList(0, uranaiKlnReadingEndVoices, 10, 0x80000000, delay + 650 - 200.0f);
    EVT_RepeatCallData(UranaiKlnRotateCards, 20, delay + 715 - 200.0f, 540);
    return end;
}

static void UranaiKlnRotateCards(void *data, int ticks)
{
    UranaiCardRotation *work = (UranaiCardRotation *)data;
    Vector3i rotation;
    float value;
    if (ticks == 0) {
        FTBL.GetScenarioCharacterEulerAngles(URNI_CARD_TABLE, &rotation, 0);
        work->baseYaw = (float)rotation.y / 182.044449f;
        work->stepAngle = (float)(FTBL.EV_GetPlayerFlag(15, 35) + 1) * 10.0f;
        work->spinAngle = (float)FTBL.EV_GetPlayerFlag(15, 35) * 36.0f + work->stepAngle;
        work->amplitude = 0.0f;
    } else if (ticks < 150) {
        value = FTBL.CalculateFixedSine((unsigned short)(int)((float)(ticks * 24576) / 45.0f)) * work->amplitude;
        EVT_AT_SetAngY(URNI_CARD_TABLE, EVENT_CHARACTER_INVALID, value * work->stepAngle + work->baseYaw, 0);
        work->amplitude += 0.00666666683f;
    } else if (ticks < 555) {
        value = FTBL.CalculateFixedCosine((unsigned short)(int)((float)((ticks - 150) * 16384) / 45.0f)) * work->amplitude;
        value = value * work->spinAngle - work->spinAngle;
        EVT_AT_SetAngY(URNI_CARD_TABLE, EVENT_CHARACTER_INVALID, value + work->baseYaw + work->stepAngle, 0);
        work->amplitude -= 0.00246913591f;
    }
}

int UranaiKlnResult(shen_name reader, int delay)
{
    int frames;
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(URNI_PLAYER), 0, (void *)areaLoopHandPoses[URNI_GRIP_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(URNI_PLAYER), 1, (void *)areaLoopHandPoses[URNI_GRIP_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_GRIP_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 1, (void *)areaLoopHandPoses[URNI_GRIP_HAND_POSE], 0);
    frames = EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(reader, URNI_KLN_RESULT_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetCam(1238, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, 0);
    QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiKlnResultVoices[FTBL.EV_GetPlayerFlag(15, 35)], 10, 0x80000000, 0);
    return 0;
}

int UranaiKlnPayment(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 100);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 90);
    EVT_AT_SetSound(996, delay + 100);
    QueueEventVoiceList(0, uranaiKlnPaymentVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiKlnInsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 480;
    int wallet = delay + 80;
    int turn, end;
    EVT_AT_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0.0f, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, wallet);
    turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, turn);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, turn);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, leave - delay - 80, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, wallet);
    QueueEventVoiceList(0, uranaiKlnInsufficientVoices, 10, 0x80000000, delay);
    return end;
}
#else
#include "s2/areas/area_urni.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_voice.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
void UranaiKlnRotateCards(void *data, std::int32_t ticks) {
    UranaiCardRotation *const work = static_cast<UranaiCardRotation *>(data);
    if (!ticks) {
        Vector3i rotation;
        GetScenarioCharacterEulerAngles(URNI_CARD_TABLE, &rotation, EventTransformFlags::Local);
        work->baseYaw = static_cast<float>(rotation.y) / 182.04445f;
        work->stepAngle = static_cast<float>(static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 35)) + 1) * 10.0f;
        const std::int32_t flag = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 35));
        work->amplitude = 0;
        work->spinAngle = static_cast<float>(flag) * 36.0f + work->stepAngle;
        return;
    }
    if (ticks >= 555)
        return;
    EventSetAngle request{.cleanup = 0, .character = URNI_CARD_TABLE, .parent = URNI_INVALID, .degrees = 0, .delay = 0};
    if (ticks < 150) {
        const std::uint16_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(3 * ticks) * 8192.0f / 45.0f));
        const float value = sin_fixed16(static_cast<std::int16_t>(angle)) * work->amplitude;
        request.degrees = value * work->stepAngle + work->baseYaw;
        EVT_AT_SetAngY_Callback(&request);
        work->amplitude += 0.0066666668f;
    } else {
        const std::uint16_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(2 * ticks - 300) * 8192.0f / 45.0f));
        const float value = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle))) * work->amplitude;
        request.degrees = ((work->spinAngle * value - work->spinAngle) + work->baseYaw) + work->stepAngle;
        EVT_AT_SetAngY_Callback(&request);
        work->amplitude -= 0.0024691359f;
    }
}
std::int64_t UranaiKlnSetup(shen_name reader, std::int32_t) {
    EV_EnableCharacterDisplay(URNI_CARD_DECK);
    EventSetPosition position{.cleanup = 0, .character = URNI_CARD_TABLE, .parent = URNI_INVALID, .position = URNI_CARD_TABLE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = URNI_CARD_TABLE, .parent = URNI_INVALID, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = URNI_CARD_DECK;
    position.position = URNI_CARD_DECK_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = URNI_CARD_DECK;
    EVT_AT_SetAngY_Callback(&angle);
    EventCling cling{.cleanup = 0,
        .target = URNI_PLAYER_CARD,
        .character = URNI_PLAYER,
        .bone = 14,
        .position = URNI_PLAYER_CARD_POSITION,
        .rotation = URNI_PLAYER_CARD_ROTATION,
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    cling.target = URNI_READER_CARD;
    cling.character = reader;
    cling.position = URNI_READER_CARD_POSITION;
    cling.rotation = URNI_READER_CARD_ROTATION;
    EVT_AT_Cling(&cling);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(URNI_PLAYER), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(URNI_PLAYER), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 0);
    return 0;
}
std::int64_t UranaiKlnModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 260;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_KLN_MODE_ONE_VOICES, 10, 0x80000000U, delay + 30);
    EV_DisableCharacterDisplay(URNI_PLAYER);
    if (end) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), URNI_TASK);
        RegisterEventTask(task);
        StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
        *work = {.cleanup = 1, .character = URNI_PLAYER, .mode = 0, .delay = static_cast<float>(end)};
        return static_cast<std::uint32_t>(end);
    }
    EV_EnableCharacterDisplay(URNI_PLAYER);
    return 0;
}
std::int64_t UranaiKlnIntroduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 200;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_KLN_INTRODUCTION_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiKlnSame(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 298) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_KLN_DECLINE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 290, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_KLN_SAME_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiKlnDecline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 218) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 210, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_KLN_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiKlnBeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 120;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0, -1.5f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 200, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_KLN_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(delay + 180);
}
std::int64_t UranaiKlnReading(shen_name reader, std::int32_t delay) {
    const std::int32_t frames = delay - 200;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0, -1.8f, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_CARD_READING_MOTION, 200, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_KLN_READING_MOTION, 200, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_KLN_READING_MOTION, 670, 0.5f, 0, frames + 670);
    constexpr shen_name names[]{URNI_CARD_DECK, URNI_READER_CARD, URNI_READER_CARD, URNI_PLAYER_CARD, URNI_PLAYER_CARD, URNI_CARD_TABLE};
    const std::int32_t delays[]{frames + 250, frames + 250, frames + 340, frames + 340, frames + 570, frames + 570};
    for (std::int32_t i = 0; i < 6; ++i) {
        const std::int32_t mode = (i & 1) ? 0 : 1;
        if (!delays[i]) {
            if (mode)
                EV_DisableCharacterDisplay(names[i]);
            else
                EV_EnableCharacterDisplay(names[i]);
        } else {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), URNI_TASK);
            RegisterEventTask(task);
            StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
            *work = {.cleanup = 1, .character = names[i], .mode = mode, .delay = static_cast<float>(delays[i])};
        }
    }
    EVT_AT_SetSound(0x3e0, frames + 570);
    EVT_AT_SetCam(1220, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1235, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, frames + 250);
    EVT_AT_SetCam(1236, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, frames + 340);
    EVT_AT_SetCam(1237, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, frames + 570);
    QueueEventVoiceList(nullptr, URNI_KLN_READING_VOICES, 10, 0x80000000U, delay);
    QueueEventVoiceList(nullptr, URNI_KLN_READING_END_VOICES, 10, 0x80000000U, frames + 650);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventTickFunction_Callback, 6, 0x0b, 0x34, URNI_TASK);
    RegisterEventTask(task);
    EventTickFunctionWork *const work = static_cast<EventTickFunctionWork *>(HLib::GetTaskParameter(task));
    work->callback = UranaiKlnRotateCards;
    work->maxTime = 540;
    work->currentTicks = 0;
    work->paramSize = 20;
    work->delay = static_cast<float>(frames + 715);
    return static_cast<std::uint32_t>(frames + 1255);
}
std::int64_t UranaiKlnResult(shen_name reader, std::int32_t) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(URNI_PLAYER), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(URNI_PLAYER), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    EventSetMotion motion{.cleanup = 0, .character = reader, .animation = URNI_KLN_TALK_MOTION, .frame = 360, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    const std::int32_t frames = static_cast<std::int32_t>(GetMotionFrameCount(URNI_KLN_TALK_MOTION));
    EVT_AT_SetMot(reader, URNI_KLN_RESULT_MOTION, 2, 1, 0, static_cast<std::int32_t>(static_cast<float>(frames) - 360.0f + 0.0f));
    EventSetCamera camera{
        .cleanup = 0, .camera = 1238, .eye = URNI_READER_DUMMY, .target = URNI_READER_DUMMY, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueEventVoiceList(
        HLib::GetCurrentTask(), URNI_KLN_RESULT_VOICES[static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 35))], 10, 0x80000000U, 0);
    return 0;
}
std::int64_t UranaiKlnPayment(shen_name reader, std::int32_t delay) {
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360, 1, 0x40000000U, delay);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 100);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 90);
    EVT_AT_SetSound(0x3e4, delay + 100);
    QueueEventVoiceList(nullptr, URNI_KLN_PAYMENT_VOICES, 10, 0x80000000U, delay);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiKlnInsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 478) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_KLN_TALK_MOTION, 360, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 400, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_KLN_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
#endif
