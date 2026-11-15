#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_urni.hpp"

#define URNI_READING_OBJECT SHEN_NAME4('M', 'M', 'E', 'G')
#define URNI_INITIAL_HAND_POSE 32
#define URNI_SYK_WAIT_MOTION 6658
#define URNI_SYK_REPEAT_MOTION 6657
#define URNI_SYK_DECLINE_MOTION 6655
#define URNI_SYK_INVITE_MOTION 6656
#define URNI_SYK_READING_START_MOTION 6653
#define URNI_SYK_READING_LOOP_MOTION 6652
#define URNI_SYK_READING_END_MOTION 6651
#define URNI_SYK_ANSWER_MOTION 6654

static int UranaiSykQueueMessages(HLibTask *parent, const char **messages, int voiceDelayFrames, unsigned int mode, int delay)
{
    int offset = 0;
    while (*messages) {
        QueueEventMessage(*messages++, delay + offset, 60);
        offset += 60;
    }
    return offset;
}

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
#define QueueEventVoiceList UranaiSykQueueMessages
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
static const char *uranaiSykModeOneVoices[] = {
    "「いらっしゃい。＆どうぞ、どうぞ。」",
    "「お前さんに幸運をもたらす＆数字を教えてやるぞ。」",
    0
};
static const char *uranaiSykIntroductionVoices[] = {
    "「●青あの…」",
    "「はいはい、占いね。」",
    "「見料は一回、１０ドルよ。」",
    0
};
static const char *uranaiSykSameVoices[] = {
    "「●青あの…」",
    "「ああ、あんた。＆せっかちはだめ。」",
    "「また、今度来なさい。」",
    "「●青わかりました。」",
    0
};
static const char *uranaiSykDeclineVoices[] = {
    "「●青いえ、結構です。」",
    "「そう、残念ね。」",
    "「じゃあ、よかったらまた来ておくれ。」",
    "「●青失礼します。」",
    0
};
static const char *uranaiSykBeginVoices[] = {
    "「●青お願いします。」",
    "「はい、じゃあ座って。」",
    "「じゃあ、お顔、前にだして。」",
    "「●青こう、ですか…」",
    0
};
static const char *uranaiSykReadingVoices[] = {
    "「ふむふむ……。」",
    "「●青あの？」",
    "「ちょっと待って。」",
    "「●青はい」",
    0
};
static const char *uranaiSykPaymentVoices[] = {
    "「険の相あれど＆大望成就せり。」",
    "「はい、問題なし。」",
    "「●青人相で、数字なんかわかるのか？」",
    "「信じないのかい？　＆まあ騙されたと思ってやってみなさい。」",
    "「なんたって、このあたしはね、＆ずーっと、この街にいたのよ。」",
    "「それはもう、エリザベス女王が＆即位したときから…」",
    "「●青どうも、ありがとうございます。」",
    0
};
static const char *uranaiSykInsufficientVoices[] = {
    "「●青それじゃあ…あれ？」",
    "「あい？　どうした？」",
    "「●青いえ、ちょっと…」",
    "「金がないのかい？＆困ったわね。」",
    "「●青…すいません。」",
    "「お金ができたら、また来てね。＆いつでも話、相手になってあげるから。」",
    "「●青…またきます。」",
    0
};
static const char *uranaiSykResultVoices[10][16] = {
    {"「今日のラッキーナンバーは『０』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『１』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『２』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『３』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『４』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『５』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『６』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『７』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『８』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {"「今日のラッキーナンバーは『９』だね。」", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
#else
static const char *uranaiSykModeOneVoices[] = {"I001", 0};
static const char *uranaiSykIntroductionVoices[] = {"A001", "I002", 0};
static const char *uranaiSykSameVoices[] = {"A001", "I009", "I010", "A010", 0};
static const char *uranaiSykDeclineVoices[] = {"A003", "I003", "I004", "A015", 0};
static const char *uranaiSykBeginVoices[] = {"A002", "I005", 0};
static const char *uranaiSykReadingVoices[] = {"I007", "A035", "I008", "A004", 0};
static const char *uranaiSykPaymentVoices[] = {"I045", "I048", "A260", "I049", "A261", 0};
static const char *uranaiSykInsufficientVoices[] = {"A020", "I011", "A021", "I012", "A022", "I013", "A137", 0};
static const char *uranaiSykResultVoices[10][16] = {{"I015", "I016", "I017", 0}, {"I018", "I019", "I020", 0}, {"I021", "I022", "I023", 0},
    {"I024", "I025", "I026", 0}, {"I027", "I028", "I029", 0}, {"I030", "I031", "I032", 0}, {"I033", "I034", "I035", 0}, {"I036", "I037", "I038", 0},
    {"I039", "I040", "I041", 0}, {"I042", "I043", "I044", 0}};
#endif
#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
static Vector3 uranaiSykObjectPosition = {0.0659999996f, -0.057f, 0.00600000005f};
static Vector3i uranaiSykObjectRotation = {50972, 52610, 8192};
#endif

#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykSetup(shen_name reader, int delay)
{
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_INITIAL_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 1, (void *)areaLoopHandPoses[URNI_INITIAL_HAND_POSE], 0);
    Queue_EVT_AT_Cling(URNI_READING_OBJECT, reader, 14, &uranaiSykObjectPosition, &uranaiSykObjectRotation, 0, 0);
    EVT_AT_CharaDisp(URNI_READING_OBJECT, 0, 1);
    return 0;
}
#endif

int UranaiSykModeOne(shen_name reader, int delay)
{
    int end = delay + 120;
#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
#endif
    EVT_AT_SetMot(reader, URNI_SYK_WAIT_MOTION, 2.0f, 1.0f, 0x40000000, delay);
#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
#else
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay);
#endif
    QueueEventVoiceList(0, uranaiSykModeOneVoices, 10, 0x80000000, delay);
#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    EVT_AT_CharaDisp(URNI_PLAYER, 0, 1);
    EVT_AT_CharaDisp(URNI_PLAYER, end, 0);
#endif
    return end;
}

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykIntroduction(shen_name reader, int delay)
{
    int end = delay + 400;
    int frames;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, end - delay, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, end - delay, 0);
    EVT_AT_SetPos(URNI_PLAYER, reader, 0.0f, 0.0f, -1.4f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    frames = EVT_AT_SetMot(reader, URNI_SYK_REPEAT_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SYK_WAIT_MOTION, 2.0f, 1.0f, 0x40000000, frames);
    EVT_AT_SetCam(1202, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSykIntroductionVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiSykIntroduction(shen_name reader, int delay)
{
    int end = delay + 210;
    EVT_AT_SetPos(URNI_PLAYER, reader, 0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SYK_REPEAT_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1230, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0x80000000, end);
    QueueEventVoiceList(0, uranaiSykIntroductionVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykSame(shen_name reader, int delay)
{
    int leave = delay + 360;
    int end;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, leave - delay, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, leave - delay, 0);
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.4f, 180.0f, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, leave - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetMot(reader, URNI_SYK_DECLINE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1202, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSykSameVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiSykSame(shen_name reader, int delay)
{
    int leave = delay + 300;
    int end;
    EVT_AT_SetPos(URNI_PLAYER, reader, 0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_SYK_DECLINE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSykSameVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykDecline(shen_name reader, int delay)
{
    int leave = delay + 420;
    int end;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, leave - delay, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, leave - delay, 0);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSykDeclineVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiSykDecline(shen_name reader, int delay)
{
    int leave = delay + 240;
    int end;
    EVT_AT_SetPos(URNI_PLAYER, reader, 0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_SYK_REPEAT_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSykDeclineVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykBeginReading(shen_name reader, int delay)
{
    int end = delay + 400;
    int frames;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, end - delay, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, end - delay, 0);
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.4f, 180.0f, delay);
    frames = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, delay + 120);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetMot(reader, URNI_SYK_INVITE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetSound(993, delay + 120);
    QueueEventVoiceList(0, uranaiSykBeginVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiSykBeginReading(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.5f, 180.0f, delay + 120);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, delay + 120);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetMot(reader, URNI_SYK_INVITE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay + 120);
    EVT_AT_SetSound(993, delay + 120);
    QueueEventVoiceList(0, uranaiSykBeginVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykReading(shen_name reader, int delay)
{
    int end = delay + 270;
    int frames = EVT_AT_SetMot(reader, URNI_SYK_READING_START_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SYK_READING_LOOP_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetCam(1204, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetSound(991, delay + 90);
    QueueEventVoiceList(0, uranaiSykReadingVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiSykReading(shen_name reader, int delay)
{
    int end = delay + 270;
    int frames;
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_READING_HAND_POSE], delay);
    EVT_AT_CharaDisp(URNI_READING_OBJECT, delay, 0);
    frames = EVT_AT_SetMot(reader, URNI_SYK_READING_START_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SYK_READING_LOOP_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetCam(1220, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay);
    QueueEventVoiceList(0, uranaiSykReadingVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykResult(shen_name reader, int delay)
{
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, 0);
    return QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiSykResultVoices[FTBL.EV_GetPlayerFlag(15, 35)], 10, 0x80000000, 0);
}
#else
int UranaiSykResult(shen_name reader, int delay)
{
    EVT_AT_SetCam(1218, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, 0);
    QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiSykResultVoices[FTBL.EV_GetPlayerFlag(15, 35)], 10, 0x80000000, 0);
    return 0;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykPayment(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay + 100, 300, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay + 100, 300, 0);
    end = EVT_AT_SetMot(reader, URNI_SYK_READING_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SYK_ANSWER_MOTION, 2.0f, 1.0f, 0, end);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 510);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1202, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetSound(992, delay + 90);
    EVT_AT_SetSound(996, delay + 510);
    QueueEventVoiceList(0, uranaiSykPaymentVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiSykPayment(shen_name reader, int delay)
{
    int end;
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(reader), 0, (void *)areaLoopHandPoses[URNI_INITIAL_HAND_POSE], delay + 440);
    EVT_AT_CharaDisp(URNI_READING_OBJECT, delay + 440, 1);
    end = EVT_AT_SetMot(reader, URNI_SYK_READING_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SYK_ANSWER_MOTION, 2.0f, 1.0f, 0, end);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 450);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay + 440);
    EVT_AT_SetSound(996, delay + 450);
    QueueEventVoiceList(0, uranaiSykPaymentVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiSykInsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 580;
    int end;
    EVT_AT_StartMoneyCount(0, delay + 10);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSykInsufficientVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiSykInsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 600;
    int wallet = delay + 80;
    int end;
    EVT_AT_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, 0.45f, 0.0f, -1.45f, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0.0f, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, wallet);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_SYK_REPEAT_MOTION, 2.0f, 0.5f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, leave - delay - 80, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, wallet);
    QueueEventVoiceList(0, uranaiSykInsufficientVoices, 10, 0x80000000, delay);
    return end;
}
#endif
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
#include "s2/qte_hand_poses.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
std::int64_t UranaiSykSetup(shen_name reader, std::int32_t) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneInitialHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneInitialHandPose), 0);
    EventCling request{.cleanup = 0,
        .target = URNI_READING_OBJECT,
        .character = reader,
        .bone = 14,
        .position = URNI_SYK_OBJECT_POSITION,
        .rotation = URNI_SYK_OBJECT_ROTATION,
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&request);
    EV_DisableCharacterDisplay(URNI_READING_OBJECT);
    return 0;
}
std::int64_t UranaiSykModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 120;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SYK_WAIT_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SYK_MODE_ONE_VOICES, 10, 0x80000000U, delay);
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
std::int64_t UranaiSykIntroduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 210;
    EVT_AT_SetPos(URNI_PLAYER, reader, {0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SYK_REPEAT_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1230, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, -0.0f, 0, end);
    QueueEventVoiceList(nullptr, URNI_SYK_INTRODUCTION_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSykSame(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 298) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SYK_DECLINE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 290, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SYK_SAME_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSykDecline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 238) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SYK_REPEAT_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 230, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SYK_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSykBeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 120;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0, -1.5f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetMot(reader, URNI_SYK_INVITE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_SYK_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSykReading(shen_name reader, std::int32_t delay) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), static_cast<std::int16_t>(delay));
    if (delay) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), URNI_TASK);
        RegisterEventTask(task);
        StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
        *work = {.cleanup = 1, .character = URNI_READING_OBJECT, .mode = 0, .delay = static_cast<float>(delay)};
    } else
        EV_EnableCharacterDisplay(URNI_READING_OBJECT);
    const std::int32_t loop = EVT_AT_SetMot(reader, URNI_SYK_READING_START_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SYK_READING_LOOP_MOTION, 2, 1, 0, loop);
    EVT_AT_SetCam(1220, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SYK_READING_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(delay + 270);
}
std::int64_t UranaiSykResult(shen_name, std::int32_t) {
    EventSetCamera camera{
        .cleanup = 0, .camera = 1218, .eye = URNI_READER_DUMMY, .target = URNI_READER_DUMMY, .context = 0, .reserved11 = {}, .angle = -0.0f, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueEventVoiceList(
        HLib::GetCurrentTask(), URNI_SYK_RESULT_VOICES[static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 35))], 10, 0x80000000U, 0);
    return 0;
}
std::int64_t UranaiSykPayment(shen_name reader, std::int32_t delay) {
    const std::int32_t answer = delay + 440;
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneInitialHandPose), static_cast<std::int16_t>(answer));
    if (answer) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), URNI_TASK);
        RegisterEventTask(task);
        StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
        *work = {.cleanup = 1, .character = URNI_READING_OBJECT, .mode = 1, .delay = static_cast<float>(answer)};
    } else
        EV_DisableCharacterDisplay(URNI_READING_OBJECT);
    const std::int32_t speaking = EVT_AT_SetMot(reader, URNI_SYK_READING_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SYK_ANSWER_MOTION, 2, 1, 0, speaking);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 450);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, answer);
    EVT_AT_SetSound(0x3e4, delay + 450);
    QueueEventVoiceList(nullptr, URNI_SYK_PAYMENT_VOICES, 10, 0x80000000U, delay);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSykInsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 598) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SYK_REPEAT_MOTION, 2, 0.5f, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 520, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_SYK_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
#endif
