#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_urni.hpp"

#define URNI_SEH_AWAKE_MOTION 6557
#define URNI_SEH_START_MOTION 6608
#define URNI_SEH_RESULT_START_MOTION 6603
#define URNI_SEH_RESULT_LOOP_MOTION 6602
#define URNI_SEH_RESULT_END_MOTION 6601
#define URNI_SEH_DROWSY_MOTION 6609

static int UranaiSehQueueMessages(HLibTask *parent, const char **messages, int voiceDelayFrames, unsigned int mode, int delay)
{
    int offset = 0;
    while (*messages) {
        QueueEventMessage(*messages++, delay + offset, 60);
        offset += 60;
    }
    return offset;
}

static const char *uranaiSehModeOneVoices[] = {0};
static const char *uranaiSehIntroductionVoices[] = {"E001", "A125", "E031", "E032", 0};
static const char *uranaiSehSameVoices[] = {"E040", "A062", "E041", "E042", "A010", 0};
static const char *uranaiSehDeclineVoices[] = {"A126", "E033", "E034", "A146", 0};
static const char *uranaiSehBeginVoices[] = {"A128", "E035", 0};
static const char *uranaiSehReadingVoices[] = {0};
static const char *uranaiSehPaymentVoices[] = {"E036", "E037", "E039", "A007", 0};
static const char *uranaiSehInsufficientVoices[] = {"A020", "E043", "A130", 0};
static const char *uranaiSehResultVoices[19][16] = {
    {"E110", "E112", "E114", "A170", "E116", "E118", "E120", "A122", 0},
    {"E050", "A219", "E051", "A140", "E052", "A092", "E053", "E054", "E055", "A010", 0},
    {"E056", "A220", "E057", "E058", "A004", "E059", "A221", "E060", "E061", "A010", 0},
    {"E062", "A222", "E063", "A223", "E064", "E065", "A010", 0},
    {"E066", "E067", "E068", "E069", "E070", "A224", "E071", "A010", 0},
    {"E110", "E111", "E114", "A170", "E115", "E119", "E120", "A122", 0},
    {"E110", "E111", "E113", "A170", "E115", "E117", "E120", "A122", 0},
    {"E110", "E111", "E114", "A170", "E115", "E119", "E120", "A122", 0},
    {"E110", "E111", "E113", "A170", "E115", "E117", "E120", "A122", 0},
    {"E072", "E073", "E074", "A225", "E075", "E076", "A010", 0},
    {"E077", "A226", "E078", "A227", "E079", "E080", "E081", "A010", 0},
    {"E082", "E083", "E084", "A221", "E085", "E086", "A010", 0},
    {"E087", "A228", "E088", "A004", "A229", "E089", "A230", "E090", "E091", "A010", 0},
    {"E092", "E093", "E094", "A221", "E095", "E096", "A010", 0},
    {"E097", "A231", "E098", "E099", "E100", "A232", "E101", "E102", "A010", 0},
    {"E097", "A231", "E098", "E099", "E100", "A232", "E101", "E102", "A010", 0},
    {"E097", "A231", "E098", "E099", "E100", "A232", "E101", "E102", "A010", 0},
    {"E092", "E093", "E094", "A221", "E095", "E096", "A010", 0},
    {"E103", "E104", "A221", "E105", "E106", "E107", "E108", "E109", "A233", 0},
};
static int uranaiSehFlags[19] = {1500, 1524, 1526, 1529, 1531, 1532, 1533, 1534, 1535, 1536, 1537, 1538, 1541, 1542, 1543, 1544, 1545, 1546, 1547};
static Vector3i uranaiSehHandRotation = {50972, 2730, 61895};
static Vector3 uranaiSehAttach = {0.108f, 0.0f, 0.012f};
static Vector3i uranaiSehRotation = {4915, 65171, 60620};
static Vector3 uranaiSehScale = {2.0f, 2.0f, 1.0f};
static Vector3 uranaiSehHand[5] = {
    {0.102f, 0.1f, 0.024f}, {0.13f, 0.1f, 0.036f}, {0.124f, 0.1f, 0.021f}, {0.102f, 0.1f, 0.032f}, {0.113f, 0.1f, 0.029f}};
static Vector3 uranaiSehIdle[5] = {{-0.15f, 0.9f, -0.67f}, {-0.17f, 0.9f, -0.68f}, {-0.14f, 0.9f, -0.67f}, {-0.16f, 0.9f, -0.66f}, {-0.15f, 0.9f, -0.65f}};
static Vector3 uranaiSehTable[6] = {
    {0.1f, 0.75f, -0.67f}, {0.07f, 0.75f, -0.68f}, {0.05f, 0.75f, -0.67f}, {-0.0f, 0.75f, -0.66f}, {-0.03f, 0.75f, -0.65f}, {-0.09f, 0.75f, -0.65f}};

int UranaiSehSetup(shen_name reader, int delay)
{
    int i;
    shen_name name;
    EVT_AT_SetPos(URNI_STICK_CUP, reader, -0.15f, 0.75f, -0.65f, 0);
    EVT_AT_SetAngY(URNI_STICK_CUP, reader, 0.0f, 0);
    Queue_EVT_AT_Cling(URNI_STICK_HAND, reader, 21, &uranaiSehAttach, &uranaiSehRotation, 0, 0);
    EVT_AT_CharaDisp(URNI_STICK_HAND, 0, 1);
    EVT_AT_CharaDisp(URNI_STICK_CUP, 0, 0);
    for (i = 0, name = URNI_STICK_TABLE_FIRST; i < 6; i++, name += 0x1000000) {
        EVT_AT_SetPos(name, reader, uranaiSehTable[i].x, uranaiSehTable[i].y, uranaiSehTable[i].z, 0);
        EVT_AT_SetAng(name, reader, 270.0f, 0.0f, 0.0f, 0);
        EVT_AT_CharaDisp(name, 0, 1);
        FTBL.mtwkSetMotionworkScale3(name, &uranaiSehScale);
    }
    for (i = 0, name = URNI_STICK_HELD_FIRST; i < 5; i++, name += 0x1000000) {
        Queue_EVT_AT_Cling(name, reader, 21, &uranaiSehHand[i], &uranaiSehHandRotation, 0, 0);
        EVT_AT_CharaDisp(name, 0, 1);
    }
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000) {
        EVT_AT_SetPos(name, reader, uranaiSehIdle[i].x, uranaiSehIdle[i].y, uranaiSehIdle[i].z, 0);
        EVT_AT_SetAng(name, EVENT_CHARACTER_INVALID, 260.0f, 0.0f, 0.0f, 0);
        EVT_AT_CharaDisp(name, 0, 0);
    }
    return 0;
}

int UranaiSehModeOne(shen_name reader, int delay)
{
    int end = delay + 180;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100.0f, 1.0f, 0x50000000, delay);
    EVT_AT_SetCam(1217, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSehModeOneVoices, 10, 0x80000000, delay);
    EVT_AT_CharaDisp(URNI_PLAYER, 0, 1);
    EVT_AT_CharaDisp(URNI_PLAYER, end, 0);
    return end;
}

int UranaiSehIntroduction(shen_name reader, int delay)
{
    int end = delay + 360;
    int i;
    for (i = 18; i > 0; i--)
        if (FTBL.EV_GetShenmueFlag(uranaiSehFlags[i]))
            break;
    if (FTBL.EV_GetPlayerFlag(15, 45) == -100)
        end = 0;
    else {
        EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
        EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
        EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100.0f, 1.0f, 0x50000000, delay);
        EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
        EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
        QueueEventVoiceList(0, uranaiSehIntroductionVoices, 10, 0x80000000, delay);
    }
    return end;
}

int UranaiSehSame(shen_name reader, int delay)
{
    int i;
    shen_name name;
    int end;
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 350);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    EVT_AT_SetSound(996, delay + 350);
    QueueEventVoiceList(0, uranaiSehSameVoices, 10, 0x80000000, delay);
    for (i = 0, name = URNI_STICK_TABLE_FIRST; i < 6; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 1);
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 0);
    return end;
}

int UranaiSehDecline(shen_name reader, int delay)
{
    int leave = delay + 280;
    int end;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100.0f, 1.0f, 0x50000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSehDeclineVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiSehBeginReading(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.5f, 180.0f, delay + 120);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, delay + 120);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetMot(reader, URNI_SEH_START_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 120);
    EVT_AT_SetSound(993, delay + 120);
    QueueEventVoiceList(0, uranaiSehBeginVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiSehReading(shen_name reader, int delay)
{
    int end = delay + 417;
    int i;
    shen_name name;
    EVT_AT_SetMot(reader, URNI_SEH_READING_MOTION, 0.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1204, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1232, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 60);
    EVT_AT_SetCam(1233, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 250);
    EVT_AT_SetCam(1234, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    QueueEventVoiceList(0, uranaiSehReadingVoices, 10, 0x80000000, delay);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_READING_HAND_POSE], 60);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 1, (void *)areaLoopHandPoses[URNI_READING_HAND_POSE], 60);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_LEFT_READING_HAND_POSE], 250);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 1, (void *)areaLoopHandPoses[URNI_LEFT_READING_HAND_POSE], 250);
    for (i = 0, name = URNI_STICK_HELD_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, delay + 60, 0);
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, delay + 60, 1);
    for (i = 0, name = URNI_STICK_HELD_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, delay + 250, 1);
    for (i = 0, name = URNI_STICK_TABLE_FIRST; i < 6; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, delay + 340, 0);
    EVT_AT_CharaDisp(URNI_STICK_HAND, delay + 250, 0);
    EVT_AT_CharaDisp(URNI_STICK_HAND, delay + 340, 1);
    return end;
}

int UranaiSehResult(shen_name reader, int delay)
{
    int result = 0;
    int index = 18;
    int frames;
    while (index > 0) {
        if (FTBL.EV_GetShenmueFlag(uranaiSehFlags[index]))
            break;
        index--;
    }
    if (index == FTBL.EV_GetPlayerFlag(15, 45))
        result = -1;
    else {
        FTBL.EV_stepSetFlags(uranaiSehFlags[index], 0);
        FTBL.EV_SetPlayerFlag(16, 45, index);
        frames = EVT_AT_SetMot(reader, URNI_SEH_RESULT_START_MOTION, 2.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(reader, URNI_SEH_RESULT_LOOP_MOTION, 2.0f, 1.0f, 0, frames);
        EVT_AT_SetCam(1218, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, 0);
        QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiSehResultVoices[index], 10, 0x80000000, 0);
    }
    return result;
}

int UranaiSehPayment(shen_name reader, int delay)
{
    int i;
    shen_name name;
    int end;
    EVT_AT_SetMot(reader, URNI_SEH_RESULT_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SEH_DROWSY_MOTION, 2.0f, 1.0f, 0, delay + 340);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 350);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    EVT_AT_SetSound(996, delay + 350);
    QueueEventVoiceList(0, uranaiSehPaymentVoices, 10, 0x80000000, delay);
    for (i = 0, name = URNI_STICK_TABLE_FIRST; i < 6; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 1);
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 0);
    return end;
}

int UranaiSehInsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 250;
    int wallet = delay + 80;
    int end;
    EVT_AT_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0.0f, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, wallet);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100.0f, 1.0f, 0x50000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, leave - delay - 80, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, wallet);
    QueueEventVoiceList(0, uranaiSehInsufficientVoices, 10, 0x80000000, delay);
    return end;
}
#else
#include "s2/areas/area_urni.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
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
std::int64_t UranaiSehSetup(shen_name reader, std::int32_t) {
    EventSetPosition position{.cleanup = 0, .character = URNI_STICK_CUP, .parent = reader, .position = URNI_SEH_CUP, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = URNI_STICK_CUP, .parent = reader, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EventCling cling{.cleanup = 0,
        .target = URNI_STICK_HAND,
        .character = reader,
        .bone = 21,
        .position = URNI_SEH_ATTACH,
        .rotation = URNI_SEH_ROTATE,
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EV_DisableCharacterDisplay(URNI_STICK_HAND);
    EV_EnableCharacterDisplay(URNI_STICK_CUP);
    shen_name name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U) {
        position.character = name;
        position.position = URNI_SEH_TABLE[i];
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{.cleanup = 0, .character = name, .parent = reader, .rotation = {49152, 0, 0}, .delay = 0};
        EVT_AT_SetAng_Callback(&rotation);
        EV_DisableCharacterDisplay(name);
        mtwkSetMotionworkScale3(name, &URNI_SEH_SCALE);
    }
    name = URNI_STICK_HELD_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U) {
        cling.target = name;
        cling.position = URNI_SEH_HAND[i];
        cling.rotation = URNI_SEH_HAND_ROTATE;
        EVT_AT_Cling(&cling);
        EV_DisableCharacterDisplay(name);
    }
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U) {
        position.character = name;
        position.position = URNI_SEH_IDLE[i];
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{.cleanup = 0, .character = name, .parent = URNI_INVALID, .rotation = {47331, 0, 0}, .delay = 0};
        EVT_AT_SetAng_Callback(&rotation);
        EV_EnableCharacterDisplay(name);
    }
    return 0;
}
std::int64_t UranaiSehModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 180;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetCam(1217, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, urniSehModeOneVoices, 10, 0x80000000U, delay);
    EV_DisableCharacterDisplay(URNI_PLAYER);
    EVT_AT_ObjectDisp_1(URNI_PLAYER, end, 0);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehIntroduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 360;
    for (std::int32_t i = 18; i > 0; --i)
        if (EV_GetShenmueFlag(URNI_SEH_FLAGS[i]))
            break;
    if (static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 45)) == -100)
        return 0;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_SEH_INTRODUCTION_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehSame(shen_name, std::int32_t delay) {
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 350);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    EVT_AT_SetSound(0x3e4, delay + 350);
    QueueEventVoiceList(nullptr, URNI_SEH_SAME_VOICES, 10, 0x80000000U, delay);
    shen_name name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 1);
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 0);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehDecline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 278) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 270, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SEH_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehBeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 120;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0, -1.5f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetMot(reader, URNI_SEH_START_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_SEH_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehReading(shen_name reader, std::int32_t delay) {
    EVT_AT_SetMot(reader, URNI_SEH_READING_MOTION, 0, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1204, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1232, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 60);
    EVT_AT_SetCam(1233, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 250);
    EVT_AT_SetCam(1234, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    QueueEventVoiceList(nullptr, urniSehReadingVoices, 10, 0x80000000U, delay);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 60);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 60);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 250);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 250);
    shen_name name = URNI_STICK_HELD_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 60, 0);
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 60, 1);
    name = URNI_STICK_HELD_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 250, 1);
    name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 340, 0);
    EVT_AT_ObjectDisp_1(URNI_STICK_HAND, delay + 250, 0);
    EVT_AT_ObjectDisp_1(URNI_STICK_HAND, delay + 340, 1);
    return static_cast<std::uint32_t>(delay + 417);
}
std::int64_t UranaiSehResult(shen_name reader, std::int32_t) {
    std::int32_t index = 18;
    while (index > 0) {
        if (EV_GetShenmueFlag(URNI_SEH_FLAGS[index]))
            break;
        --index;
    }
    if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 45)))
        return 0xffffffffLL;
    EV_stepSetFlags(0, URNI_SEH_FLAGS[index]);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 45, index);
    EventSetMotion motion{
        .cleanup = 0, .character = reader, .animation = URNI_SEH_RESULT_START_MOTION, .frame = 2, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    const std::int32_t frames = static_cast<std::int32_t>(GetMotionFrameCount(URNI_SEH_RESULT_START_MOTION));
    EVT_AT_SetMot(reader, URNI_SEH_RESULT_LOOP_MOTION, 2, 1, 0, static_cast<std::int32_t>(static_cast<float>(frames) - 2.0f + 0.0f));
    EventSetCamera camera{
        .cleanup = 0, .camera = 1218, .eye = URNI_READER_DUMMY, .target = URNI_READER_DUMMY, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueEventVoiceList(HLib::GetCurrentTask(), URNI_SEH_RESULT_VOICES[index], 10, 0x80000000U, 0);
    return 0;
}
std::int64_t UranaiSehPayment(shen_name reader, std::int32_t delay) {
    EVT_AT_SetMot(reader, URNI_SEH_RESULT_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SEH_DROWSY_MOTION, 2, 1, 0, delay + 340);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 350);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    EVT_AT_SetSound(0x3e4, delay + 350);
    QueueEventVoiceList(nullptr, URNI_SEH_PAYMENT_VOICES, 10, 0x80000000U, delay);
    shen_name name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 1);
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 0);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehInsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 248) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 170, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_SEH_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
#endif
