#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_urni.hpp"

#define URNI_READING_OBJECT SHEN_NAME4('M', 'M', 'E', 'G')
#define URNI_CHEST_HAND_POSE 18
#define URNI_MM5_IDLE_MOTION 6626
#define URNI_MM5_GREETING_MOTION 6643
#define URNI_MM5_END_MOTION 6627
#define URNI_MM5_DECLINE_MOTION 6624
#define URNI_MM5_START_MOTION 6630
#define URNI_MM5_LOOP_MOTION 6629
#define URNI_PLAYER_PALM_END_MOTION 6639
#define URNI_PLAYER_PALM_START_MOTION 6641
#define URNI_PLAYER_PALM_LOOP_MOTION 6640

static int UranaiMm5QueueMessages(HLibTask *parent, const char **messages, int voiceDelayFrames, unsigned int mode, int delay)
{
    int offset = 0;
    while (*messages) {
        QueueEventMessage(*messages++, delay + offset, 60);
        offset += 60;
    }
    return offset;
}

static const char *uranaiMm5ModeOneVoices[] = {"D001", 0};
static const char *uranaiMm5IntroductionVoices[] = {"D001", "D002", "D003", 0};
static const char *uranaiMm5SameVoices[] = {"D008", "A062", "D009", "D010", "A063", 0};
static const char *uranaiMm5DeclineVoices[] = {"A003", "D004", 0};
static const char *uranaiMm5BeginVoices[] = {"A128", 0};
static const char *uranaiMm5ReadingVoices[] = {0};
static const char *uranaiMm5PaymentVoices[] = {"D005", "D007", "A060", 0};
static const char *uranaiMm5InsufficientVoices[] = {"A020", "D011", "D012", "A064", 0};
static const char *uranaiMm5HkVoices[20][32] = {
    {"D152", "D153", "D157", "A194", "D276", "A177", 0},
    {"D179", "D180", "D181", "D182", "A152", "D183", "A153", "D184", 0},
    {"D185", "D186", "D187", "A154", "D188", "D183", "A153", "D184", 0},
    {"D189", "D190", "A155", "D191", "A156", "D192", "D193", "D184", 0},
    {"D194", "A157", "D195", "A158", "D191", "D196", "A159", "D197", "D198", "A160", "D199", "D184", 0},
    {"D194", "A157", "D195", "A158", "D191", "D200", "A161", "D197", "D198", "A160", "D199", "D184", 0},
    {"D200", "A161", "D197", "D198", "A160", "D199", "D184", 0},
    {"D201", "D202", "A162", "D203", "D204", "A163", "D205", "D206", "A164", "D184", 0},
    {"D207", "D208", "D209", "D210", "A165", "D211", "A166", 0},
    {"D212", "D213", "A167", "D214", "D215", "D216", "D217", "A169", 0},
    {"D218", "D213", "A167", "D219", "A170", "D220", "A171", 0},
    {"D221", "D222", "A172", "D223", "A170", "D224", "A171", 0},
    {"D225", "D226", "A173", "D227", "A174", "D228", "A175", 0},
    {"D229", "A177", "D230", "D231", "D232", "A174", "D233", "A177", 0},
    {"D234", "D235", "A178", "D236", "A179", "D237", "D238", "A174", "D184", 0},
    {"D240", "A157", "D241", "A181", "D242", "A198", 0},
    {"D244", "D245", "D246", "A182", "D247", "A183", "D248", "A184", "D249", "D250", "A166", 0},
    {"D251", "D252", "A185", "D253", "D254", "D255", "D256", "A186", "D184", 0},
    {"D257", "A187", "D258", "D259", "D260", "D261", "A188", "D184", "D262", "A189", 0},
    {"D264", "A156", "D265", "D266", "A174", "D267", "D268", "A195", "D269", "D270", "A191", "D271", "D272", "A192", "D273", "D274", "A193", 0},
};
static int uranaiMm5HkFlags[20] = {
    1500, 1501, 1502, 1503, 1506, 1507, 1508, 1509, 1510, 1511, 1512, 1513, 1514, 1515, 1518, 1519, 1520, 1521, 1522, 1523};
static const char *uranaiMm5KlVoices[15][16] = {
    {"D152", "D153", "D157", "A194", "D276", "A177", 0},
    {"D280", "D281", "A195", "D282", "A196", "D283", "D284", "D285", "A197", 0},
    {"D286", "D287", "D288", "A195", "D289", "A092", "D290", "A156", "D291", "D292", "A198", 0},
    {"D293", "D294", "D295", "A199", "D296", "D297", "A197", 0},
    {"D298", "D299", "A200", "D300", "A201", "D301", "D302", "D303", "D304", "D305", "A202", 0},
    {"D306", "D307", "D308", "A203", "D309", "D310", "A204", "D311", 0},
    {"D312", "A205", "D313", "D314", "A206", "D315", "A207", "D316", "D317", "A177", 0},
    {"D318", "A174", "D319", "D320", "A208", "D321", "D322", "A198", 0},
    {"D323", "D324", "D325", "D326", "A209", "D327", "A210", "D328", "A211", "D329", "A212", "D330", "A166", 0},
    {"D152", "D277", "D156", "D157", "D276", "A177", 0},
    {"D331", "D332", "D333", "A213", "D334", "A214", "D335", "A215", "D336", "A216", 0},
    {"D331", "D337", "D333", "A213", "D338", "A214", "D335", "A215", "D336", "A216", 0},
    {"D331", "D339", "D340", "A217", "D341", "A214", "D335", "A215", "D336", "A216", 0},
    {"D152", "D158", "D159", "D160", "D278", "D279", "A177", 0},
    {"D343", "A196", "D344", "D345", "A195", "D346", "D347", "A215", "D348", "D349", "D350", "D351", "D352", "A197", 0},
};
static int uranaiMm5KlFlags[15] = {1500, 1526, 1529, 1531, 1532, 1536, 1537, 1538, 1541, 1542, 1543, 1544, 1545, 1546, 1547};
static const char *uranaiMm5SpecialVoices[5][32] = {
    {"D353", "D354", "D355", "D356", "D357", "D358", "D359", 0},
    {"D382", "D383", "A292", "D384", "D385", "D386", "D387", "D388", 0},
    {"D360", "A285", "D361", "A286", "D362", "D363", "D364", "D365", "A287", "D366", "D367", "A288", "D368", "D369", 0},
    {"D371", "A289", "D372", "A290", "D373", "D374", "D375", "D376", "D377", "D378", "D379", "D380", "A291", "D381", 0},
    {"D389", "A293", "D390", "D391", "A285", "D392", "A294", "D393", "D394", "D395", "D396", "D397", "D398", "D399", "D400", "D401", 0},
};
static Vector3 uranaiMm5ObjectPosition = {0.0825529993f, 0.00466299988f, -0.0127940001f};
static Vector3i uranaiMm5ObjectRotation = {32449, 32709, 30381};
static int uranaiMm5Cameras[6] = {1212, 1243, 1240, 1241, 1242, 1244};

int UranaiMm5Setup(shen_name reader, int delay)
{
    Queue_EVT_AT_Cling(URNI_READING_OBJECT, reader, 14, &uranaiMm5ObjectPosition, &uranaiMm5ObjectRotation, 0, 0);
    EVT_AT_CharaDisp(URNI_READING_OBJECT, 0, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_CHEST_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 1, (void *)areaLoopHandPoses[URNI_LEFT_READING_HAND_POSE], 0);
    return 0;
}

int UranaiMm5ModeOne(shen_name reader, int delay)
{
    int end = delay + 75;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiMm5ModeOneVoices, 10, 0x80000000, delay + 30);
    EVT_AT_CharaDisp(URNI_PLAYER, 0, 1);
    EVT_AT_CharaDisp(URNI_PLAYER, end, 0);
    return end;
}

int UranaiMm5Introduction(shen_name reader, int delay)
{
    int end = delay + 250;
    int i;
    if ((unsigned int)g_actNumber < 3) {
        for (i = 19; i > 0; i--)
            if (FTBL.EV_GetShenmueFlag(uranaiMm5HkFlags[i]))
                break;
    } else {
        for (i = 14; i > 0; i--)
            if (FTBL.EV_GetShenmueFlag(uranaiMm5KlFlags[i]))
                break;
    }
    if (FTBL.EV_GetPlayerFlag(15, 42) == -100)
        end = 0;
    else {
        EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
        EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
        EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetMot(reader, URNI_MM5_GREETING_MOTION, 40.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
        EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
        QueueEventVoiceList(0, uranaiMm5IntroductionVoices, 10, 0x80000000, delay + 20);
    }
    return end;
}

int UranaiMm5Same(shen_name reader, int delay)
{
    int end;
    end = EVT_AT_SetMot(reader, URNI_MM5_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 380);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 365);
    EVT_AT_SetSound(996, delay + 380);
    QueueEventVoiceList(0, uranaiMm5SameVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiMm5Decline(shen_name reader, int delay)
{
    int leave = delay + 190;
    int turn, end;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, turn);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, turn);
    EVT_AT_SetMot(reader, URNI_MM5_DECLINE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay);
    QueueEventVoiceList(0, uranaiMm5DeclineVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiMm5BeginReading(shen_name reader, int delay)
{
    int end = delay + 140;
    int frames;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.1f, -0.9f, 180.0f, delay + 90);
    frames = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, delay + 90);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 90);
    EVT_AT_SetSound(993, delay + 90);
    QueueEventVoiceList(0, uranaiMm5BeginVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiMm5Reading(shen_name reader, int delay)
{
    int end;
    end = EVT_AT_SetMot(reader, URNI_MM5_START_MOTION, 90.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_MM5_LOOP_MOTION, 2.0f, 1.0f, 0, end);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_START_MOTION, 90.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_LOOP_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetCam(1211, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiMm5ReadingVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiMm5Result(shen_name reader, int delay)
{
    int result = 0;
    int special = -1;
    int index;
    if ((unsigned int)g_actNumber < 3) {
        if (FTBL.shenGetRandomInt32(10) == 0 && FTBL.EV_GetShenmueFlag(1147)) {
            if (FTBL.EV_GetPlayerFlag(11, 2000)) {
                special = 1;
                QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiMm5SpecialVoices[1], 10, 0x80000000, 0);
            } else {
                FTBL.EV_stepSetFlags(1147, 0);
                special = 0;
                QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiMm5SpecialVoices[0], 10, 0x80000000, 0);
            }
            EVT_AT_SetFaceLooks(reader, 13, 30, 1.0f, 0);
        } else if (FTBL.shenGetRandomInt32(5) == 0 && FTBL.EV_GetPlayerFlag(11, 2000)) {
            if (FTBL.EV_GetShenmueFlag(1148)) {
                special = FTBL.shenGetRandomInt32(2);
                QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiMm5SpecialVoices[special + 1], 10, 0x80000000, 0);
            } else {
                special = FTBL.shenGetRandomInt32(3);
                QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiMm5SpecialVoices[special + 1], 10, 0x80000000, 0);
            }
            EVT_AT_SetFaceLooks(reader, 13, 30, 1.0f, 0);
        } else {
            index = 19;
            while (index > 0) {
                if (FTBL.EV_GetShenmueFlag(uranaiMm5HkFlags[index]))
                    break;
                index--;
            }
            if (index == FTBL.EV_GetPlayerFlag(15, 42))
                result = -1;
            else {
                FTBL.EV_SetPlayerFlag(16, 42, index);
                FTBL.EV_stepSetFlags(uranaiMm5HkFlags[index], 0);
                QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiMm5HkVoices[index], 10, 0x80000000, 0);
            }
        }
    } else if (FTBL.shenGetRandomInt32(3) == 0 && FTBL.EV_GetPlayerFlag(11, 2000)) {
        special = FTBL.shenGetRandomInt32(4);
        QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiMm5SpecialVoices[special + 1], 10, 0x80000000, 0);
        EVT_AT_SetFaceLooks(reader, 13, 30, 1.0f, 0);
    } else {
        index = 14;
        while (index > 0) {
            if (FTBL.EV_GetShenmueFlag(uranaiMm5KlFlags[index]))
                break;
            index--;
        }
        if (index == FTBL.EV_GetPlayerFlag(15, 42))
            result = -1;
        else {
            FTBL.EV_SetPlayerFlag(16, 42, index);
            FTBL.EV_stepSetFlags(uranaiMm5KlFlags[index], 0);
            QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiMm5KlVoices[index], 10, 0x80000000, 0);
        }
    }
    if (result >= 0)
        EVT_AT_SetCam(uranaiMm5Cameras[special + 1], URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, 0);
    return result;
}

int UranaiMm5Payment(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetFaceLooks(reader, 0, 30, 1.0f, delay);
    end = EVT_AT_SetMot(reader, URNI_MM5_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 215);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 200);
    EVT_AT_SetSound(996, delay + 215);
    QueueEventVoiceList(0, uranaiMm5PaymentVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiMm5InsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 300;
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
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, leave - delay - 80, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, wallet);
    QueueEventVoiceList(0, uranaiMm5InsufficientVoices, 10, 0x80000000, delay);
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
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
std::int64_t UranaiMm5Setup(shen_name reader, std::int32_t) {
    EventCling request{.cleanup = 0,
        .target = URNI_READING_OBJECT,
        .character = reader,
        .bone = 14,
        .position = URNI_MM5_OBJECT_POSITION,
        .rotation = URNI_MM5_OBJECT_ROTATION,
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&request);
    EV_EnableCharacterDisplay(URNI_READING_OBJECT);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 0);
    return 0;
}
std::int64_t UranaiMm5ModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 75;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_MM5_MODE_ONE_VOICES, 10, 0x80000000U, delay + 30);
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
std::int64_t UranaiMm5Introduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 250;
    if (g_actNumber >= 3) {
        for (std::int32_t i = 14; i > 0; --i)
            if (EV_GetShenmueFlag(URNI_MM5_KL_FLAGS[i]))
                break;
    } else {
        for (std::int32_t i = 19; i > 0; --i)
            if (EV_GetShenmueFlag(URNI_MM5_HK_FLAGS[i]))
                break;
    }
    if (static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 42)) == -100)
        return 0;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_GREETING_MOTION, 40, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_MM5_INTRODUCTION_VOICES, 10, 0x80000000U, delay + 20);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5Same(shen_name reader, std::int32_t delay) {
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_MM5_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0, readingEnd);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_END_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 380);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 365);
    EVT_AT_SetSound(0x3e4, delay + 380);
    QueueEventVoiceList(nullptr, URNI_MM5_SAME_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5Decline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 188) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_MM5_DECLINE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 180, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_MM5_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5BeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 90;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.1f, -0.89999998f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_MM5_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(delay + 140);
}
std::int64_t UranaiMm5Reading(shen_name reader, std::int32_t delay) {
    const std::int32_t loop = EVT_AT_SetMot(reader, URNI_MM5_START_MOTION, 90, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_LOOP_MOTION, 2, 1, 0, loop);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_START_MOTION, 90, 1, 0x40000000U, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_LOOP_MOTION, 2, 1, 0, end);
    EVT_AT_SetCam(1211, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, urniMm5ReadingVoices, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5Result(shen_name reader, std::int32_t) {
    std::int32_t special = -1;
    const char *const *voices;
    if (g_actNumber < 3) {
        if (shenGetRandomInt32(10) == 0 && EV_GetShenmueFlag(1147)) {
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2000)) {
                special = 1;
                voices = URNI_MM5_SPECIAL_VOICES[1];
            } else {
                EV_stepSetFlags(0, 1147);
                special = 0;
                voices = URNI_MM5_SPECIAL_VOICES[0];
            }
            QueueEventVoiceList(HLib::GetCurrentTask(), voices, 10, 0x80000000U, 0);
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(reader), 13, 30, 1);
        } else if (shenGetRandomInt32(5) == 0 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2000)) {
            special = shenGetRandomInt32(EV_GetShenmueFlag(1148) ? 2 : 3);
            voices = URNI_MM5_SPECIAL_VOICES[special + 1];
            QueueEventVoiceList(HLib::GetCurrentTask(), voices, 10, 0x80000000U, 0);
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(reader), 13, 30, 1);
        } else {
            std::int32_t index = 19;
            while (index > 0) {
                if (EV_GetShenmueFlag(URNI_MM5_HK_FLAGS[index]))
                    break;
                --index;
            }
            if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 42)))
                return 0xffffffffLL;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 42, index);
            EV_stepSetFlags(0, URNI_MM5_HK_FLAGS[index]);
            QueueEventVoiceList(HLib::GetCurrentTask(), URNI_MM5_HK_VOICES[index], 10, 0x80000000U, 0);
        }
    } else if (shenGetRandomInt32(3) != 0 || !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2000)) {
        std::int32_t index = 14;
        while (index > 0) {
            if (EV_GetShenmueFlag(URNI_MM5_KL_FLAGS[index]))
                break;
            --index;
        }
        if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 42)))
            return 0xffffffffLL;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 42, index);
        EV_stepSetFlags(0, URNI_MM5_KL_FLAGS[index]);
        QueueEventVoiceList(HLib::GetCurrentTask(), URNI_MM5_KL_VOICES[index], 10, 0x80000000U, 0);
    } else {
        special = shenGetRandomInt32(4);
        QueueEventVoiceList(HLib::GetCurrentTask(), URNI_MM5_SPECIAL_VOICES[special + 1], 10, 0x80000000U, 0);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(reader), 13, 30, 1);
    }
    EventSetCamera camera{.cleanup = 0,
        .camera = URNI_MM5_CAMERAS[special + 1],
        .eye = URNI_READER_DUMMY,
        .target = URNI_READER_DUMMY,
        .context = 0,
        .reserved11 = {},
        .angle = 0,
        .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 0;
}
std::int64_t UranaiMm5Payment(shen_name reader, std::int32_t delay) {
    EVT_AT_SetFaceLooks(reader, FacialExpressionId::Normal, 30, 1, delay);
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_MM5_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0, readingEnd);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_END_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 215);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 200);
    EVT_AT_SetSound(0x3e4, delay + 215);
    QueueEventVoiceList(nullptr, URNI_MM5_PAYMENT_VOICES, 10, 0x80000000U, delay);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5InsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 298) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 220, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_MM5_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
#endif
