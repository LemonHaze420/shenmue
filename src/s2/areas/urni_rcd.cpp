#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_urni.hpp"

#define URNI_RCD_IDLE_MOTION 6611
#define URNI_RCD_DECLINE_MOTION 6612
#define URNI_RCD_RESULT_MOTION 6610

static int UranaiRcdQueueMessages(HLibTask *parent, const char **messages, int voiceDelayFrames, unsigned int mode, int delay)
{
    int offset = 0;
    while (*messages) {
        QueueEventMessage(*messages++, delay + offset, 60);
        offset += 60;
    }
    return offset;
}

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
#define QueueEventVoiceList UranaiRcdQueueMessages
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
static const char *uranaiRcdModeOneVoices[] = {
    "どうじゃ？＆占ってみるか。",
    "●青当たるも八卦か…",
    "こりゃ、わしの卦は＆百発百中じゃわい。",
    "●青すいません。",
    0
};
static const char *uranaiRcdIntroductionVoices[] = {
    "さて、見料は＆一回、１０ドルじゃ",
    "●青１０ドルか…",
    "お金で運命を知ることができる。＆しかも迷いが晴れる。",
    "安いものよ。＆わしなら占っとるな。",
    "●青はあ",
    "さあ、どうじゃ？",
    0
};
static const char *uranaiRcdSameVoices[] = {
    "●青あの…",
    "ああ、お前さんか。＆だめじゃ、だめじゃ。",
    "そうそう、陰陽の秘に頼ってはならん。",
    "新たな悩みが生まれるまで＆自分で考えることじゃ。",
    "●青わかりました。",
    0
};
static const char *uranaiRcdDeclineVoices[] = {
    "●青いえ、結構です。",
    "そうか、残念じゃの。＆気が向いたらまた来なされ。",
    "●青失礼します。",
    0
};
static const char *uranaiRcdBeginVoices[] = {
    "●青お願いします。",
    "それでは、＆そこに座って。",
    0
};
static const char *uranaiRcdReadingVoices[] = {
    "う〜〜〜ん＆うん？",
    "ふむ…",
    "●青あの？",
    0
};
static const char *uranaiRcdPaymentVoices[] = {
    "以上じゃ。",
    "●青それだけですか？",
    "１０ドルではそんなもんじゃ。",
    "●青…そうですか。",
    "易は太極にあり、是れ両義を生ず、",
    "両義は四象を生じ、四象は八卦を生ず。",
    "この筮竹には陰陽、全てがある。＆迷ったときにはここに来なされ。",
    "●青ありがとうございます。",
    0
};
static const char *uranaiRcdInsufficientVoices[] = {
    "●青それじゃあ…あれ？",
    "どうした？",
    "●青いえ、ちょっと…",
    "なんじゃ、２０ドルも持ってないのか？",
    "●青…すいません。",
    "まあ、わしは逃げんから、＆金ができたら、また来てくだされ。",
    "●青はい…わかりました。",
    0
};
static const char *uranaiRcdResultVoices[22][16] = {
    {"「まず心あたりのある＆場所に行ってみよ。」", "「お主は今ある運命しか選べん。＆新たな運命を知りたいなら…」", "「まず今ある運命を＆消さねばならん。」", "「●青…わかりました。」", 0},
    {"「文武は壬の方角にあり。」", "「●青え？」", "「ここを出てすぐ北に大きな寺がある。＆その門をくぐるがよい。」", "「武は男子のみのものあらず。＆心に留めておかれよ。」", 0},
    {"「『人、学ばざれば道知らず』か。」", "「●青……はい。」", "「しょうがない、若者じゃ。」", 0},
    {"「目的の人物には＆会えたようじゃな。」", "「●青…いえ。」", "「ならば、お主の心が足りぬのじゃ。＆精進するがよい。」", "「●青わかりました。＆ですが…」", "「西に吉ありと出ておる。＆紅南街のことじゃろう。」", "「●青もう少し、詳しい場所は？」", "「それでは心の鍛錬にはならん。＆ではないかな？」", "「●青わかりました。」", 0},
    {"「坤、即ち南西に人ありき＆かの人、紅に佇む。」", "「紅南街の…＆徳蓬公園のことであろう。」", 0},
    {"「庚の方角か…＆菜珍街、門をくぐってみよ。」", 0},
    {"「乾に師ありと出ておるな。＆福仙街のことじゃろう。」", "「福仙街に行ってみよ。」", 0},
    {"「坤の方角か…＆菜珍街にある、門をくぐってみよ。」", "「●青門…」", "「光武館という道場がある。」", "「●青光武館か…」", 0},
    {"「坤の方角か…＆菜珍街にある、門をくぐってみよ。」", "「●青門…」", "「光武館という道場がある。」", "「●青光武館か…」", "「紅南街との街境にある＆道場じゃ。」", "「●青その後は…＆どうすればいいですか？」", "「北に進めと…たぶん、＆詠黄街じゃろうな。」", 0},
    {"「坤、即ち南西に人ありき＆かの人、紅に佇む。」", "「紅南街、徳蓬公園にいる＆建民殿のことであろう。」", "「●青建民さん…」", "「うむ、徳蓬公園で太極拳の＆修練をしているお方じゃ。」", "「お主の力になってくれるであろう。」", 0},
    {"「乾に師ありと出ておるな。＆福仙街のことじゃろう。」", "「福仙街に行ってみよ。」", 0},
    {"「乾に達人あり。＆達人、未だ現役なり。」", "「ふむぅ…福仙街の張喩殿であろう。」", "「●青張喩さん、ですか…」", "「うむ、福仙街でも有名な御仁じゃ。＆街の者に聞いてみるがよい。」", 0},
    {"「乾！＆詠黄街にいってみるがよい。」", 0},
    {"「乾に横臥せし賢人あり。＆詠黄街にいってみるがよい。」", "「●青詠黄街か…」", "「賢人、武の力を商いの道具につかう＆と出てもいるな。」", "「詠黄百貨に＆行かれるとよい。」", "「●青詠黄百貨か…」", 0},
    {"「まだ、その者に出会うこと＆適わず。」", "「●青じゃあ、いったいどうすれば…」", "「痕跡は古き街で＆武を営む地にあり。」", "「まずは古き街に…＆そして武を売る地に向かうとよい。」", 0},
    {"「ただ、その痕跡を知るのみと＆出ておるな。」", "「その者はここに在らず。＆人となりを知るのみ。」", "「●青じゃあ、いったいどうすれば…」", "「古き街に武を営む地に＆足跡あり。」", "「まずは古き街に…＆そして武を売る地に向かうとよい。」", 0},
    {"「探しの物のありかは…」", "「お主を見守っている人物が＆知っておる。」", "「見守っている人は＆その者、以外にもおるはずじゃ。」", "「ぬしは１人ではない。＆無極は全である。」", "「全であり個であるとともに＆個であり全でもある。」", "「そして…陰陽の央は文武廟に＆つながっおる。」", "「文武廟に行かれるがよい。」", 0},
    {"「ふむ…これは難儀じゃのう。」", "「古き街、青き壁に金の文字…＆古き物が集る地。」", "「そこに行くが良い。」", 0},
    {"「ふむ…これは難儀じゃのう。」", "「福仙街に行ってみよ。」", 0},
    {"「まず心あたりのある＆場所に行ってみよ。」", "「『来来飯店』に行くがよい。＆アバディーンにある。」", "「●青…わかりました。」", 0},
    {"「ふむ、虎穴入らんず虎子を得ずか。」", "「海の見える場所に＆求める者はおるな。」", "「じゃが、会うには＆ちと、対面料がかかるじゃろう。」", "「じゃが避けようのない試練。＆がんばるんじゃの。」", 0},
    {"「この街を離れるようじゃの。」", "「●青……ええ。」", "「世話になった者に＆挨拶はしたか？」", "「●青…それは…」", "「出会った縁は大切せねばならん。」", "「お主は、文武廟で＆なにも学ばなかったのか？」", "「武術は技のみあらず＆心をも磨かねばならん。」", "「礼節を怠るな。＆わかったな。」", "「●青はい。ありがとうございます。」", "「では最後に手向けの言葉を贈ろう。」", "「お主は秋風が吹く前に＆この地から離れねばならん。」", "「よいか、秋風が吹く前じゃぞ。」", 0}
};
static int uranaiRcdFlags[22] = {1600, 1601, 1602, 1603, 1604, 1605, 1606, 1607, 1608, 1609, 1610, 1611, 1613, 1614, 1615, 1616, 1617, 1618, 1619, 1620, 1621, 1622};
#else
static const char *uranaiRcdModeOneVoices[] = {"J001", "A138", "J002", "A087", 0};
static const char *uranaiRcdIntroductionVoices[] = {"J003", "A139", "J004", "J005", "A140", "J006", 0};
static const char *uranaiRcdSameVoices[] = {"A001", "J013", "J014", "J015", "A010", 0};
static const char *uranaiRcdDeclineVoices[] = {"A003", "J007", "A015", 0};
static const char *uranaiRcdBeginVoices[] = {"A002", "J008", 0};
static const char *uranaiRcdReadingVoices[] = {"J009", "J010", "A035", 0};
static const char *uranaiRcdPaymentVoices[] = {"J011", "A018", "J012", "A019", "A007", 0};
static const char *uranaiRcdInsufficientVoices[] = {"A020", "J016", "A021", "J017", "A022", "J018", "A010", 0};
static const char *uranaiRcdResultVoices[21][32] = {
    {"J112", "J113", "J114", "A284", "J115", "A023", 0},
    {"J112", "J120", "J121", "J122", "J123", "J124", "A023", 0},
    {"J022", "A262", "J023", "J024", "A010", "J025", "J026", "A004", 0},
    {"J027", "J028", "A140", "J029", "J030", "J031", "A263", 0},
    {"J032", "A140", "J033", "A264", "J034", "J035", "J036", "A170", "J037", "A265", 0},
    {"J032", "A140", "J033", "A264", "J034", "J035", "J038", "A170", "J041", "A265", 0},
    {"J043", "J044", "J045", "J046", "J047", "A269", "J048", "A004", 0},
    {"J049", "J050", "J051", "J052", "J053", "A010", 0},
    {"J054", "J055", "J056", "A270", "J057", "J058", "A271", 0},
    {"J039", "A266", "J040", "A267", "J035", "J038", "A170", "J041", "A268", "J042", "A010", 0},
    {"J059", "J060", "J061", "A272", "J062", "A273", "J063", "A010", 0},
    {"J059", "J064", "J065", "J066", "A010", 0},
    {"J112", "J116", "J117", "J118", "J119", "A023", 0},
    {"J112", "J120", "J121", "J122", "J123", "J124", "A023", 0},
    {"J067", "J068", "J069", "A274", "J070", "J071", "J072", 0},
    {"J073", "J074", "A170", "J075", "A010", "J076", "J077", "A275", "J078", "A276", 0},
    {"J079", "J080", "J081", "A277", "J082", "A023", 0},
    {"J083", "A278", "J084", "A279", "J085", "A280", "J086", "J087", "A010", 0},
    {"J088", "A281", "J089", "J090", "J091", "J092", "A010", 0},
    {"J093", "J094", "J095", "A004", "J096", "A282", "J097", "J098", "J099", "A010", 0},
    {"J100", "J101", "A283", "J102", "J103", "J104", "A140", "J105", "A123", "J106", "J107", "J108", "A010", "J109", "J110", "J111", "A004", 0},
};
static int uranaiRcdFlags[21] = {
    1500, 1503, 1504, 1505, 1506, 1507, 1508, 1509, 1510, 1511, 1512, 1513, 1514, 1515, 1516, 1517, 1518, 1519, 1520, 1522, 1523};
#endif
#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
static Vector3i uranaiRcdHandRotation = {50972, 2730, 61895};
static Vector3 uranaiRcdAttach = {0.108f, 0.0f, 0.012f};
static Vector3i uranaiRcdRotation = {4915, 65171, 60620};
static Vector3 uranaiRcdScale = {2.0f, 2.0f, 1.0f};
static Vector3 uranaiRcdHand[5] = {
    {0.102f, 0.1f, 0.024f}, {0.13f, 0.1f, 0.036f}, {0.124f, 0.1f, 0.021f}, {0.102f, 0.1f, 0.032f}, {0.113f, 0.1f, 0.029f}};
static Vector3 uranaiRcdIdle[5] = {{-0.2f, 0.9f, -0.67f}, {-0.22f, 0.9f, -0.68f}, {-0.19f, 0.9f, -0.67f}, {-0.21f, 0.9f, -0.66f}, {-0.2f, 0.9f, -0.65f}};
static Vector3 uranaiRcdTable[6] = {
    {0.1f, 0.75f, -0.67f}, {0.07f, 0.75f, -0.68f}, {0.05f, 0.75f, -0.67f}, {-0.0f, 0.75f, -0.66f}, {-0.03f, 0.75f, -0.65f}, {-0.09f, 0.75f, -0.65f}};
#endif

#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdSetup(shen_name reader, int delay)
{
    int i;
    shen_name name;
    EVT_AT_SetPos(URNI_STICK_CUP, reader, -0.2f, 0.75f, -0.65f, 0);
    EVT_AT_SetAngY(URNI_STICK_CUP, reader, 0.0f, 0);
    Queue_EVT_AT_Cling(URNI_STICK_HAND, reader, 21, &uranaiRcdAttach, &uranaiRcdRotation, 0, 0);
    EVT_AT_CharaDisp(URNI_STICK_HAND, 0, 1);
    EVT_AT_CharaDisp(URNI_STICK_CUP, 0, 0);
    for (i = 0, name = URNI_STICK_TABLE_FIRST; i < 6; i++, name += 0x1000000) {
        EVT_AT_SetPos(name, reader, uranaiRcdTable[i].x, uranaiRcdTable[i].y, uranaiRcdTable[i].z, 0);
        EVT_AT_SetAng(name, reader, 270.0f, 0.0f, 0.0f, 0);
        EVT_AT_CharaDisp(name, 0, 1);
        FTBL.mtwkSetMotionworkScale3(name, &uranaiRcdScale);
    }
    for (i = 0, name = URNI_STICK_HELD_FIRST; i < 5; i++, name += 0x1000000) {
        Queue_EVT_AT_Cling(name, reader, 21, &uranaiRcdHand[i], &uranaiRcdHandRotation, 0, 0);
        EVT_AT_CharaDisp(name, 0, 1);
    }
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000) {
        EVT_AT_SetPos(name, reader, uranaiRcdIdle[i].x, uranaiRcdIdle[i].y, uranaiRcdIdle[i].z, 0);
        EVT_AT_SetAng(name, EVENT_CHARACTER_INVALID, 260.0f, 0.0f, 0.0f, 0);
        EVT_AT_CharaDisp(name, 0, 0);
    }
    return 0;
}
#endif

int UranaiRcdModeOne(shen_name reader, int delay)
{
#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    int end = delay + 180;
#else
    int end = delay + 270;
#endif
#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
#endif
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
#else
    EVT_AT_SetCam(1217, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
#endif
    QueueEventVoiceList(0, uranaiRcdModeOneVoices, 10, 0x80000000, delay);
#if !SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    EVT_AT_CharaDisp(URNI_PLAYER, 0, 1);
    EVT_AT_CharaDisp(URNI_PLAYER, end, 0);
#endif
    return end;
}

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdIntroduction(shen_name reader, int delay)
{
    int end = delay + 360;
    int i;
    for (i = 21; i > 0; i--)
        if (FTBL.EV_GetShenmueFlag(uranaiRcdFlags[i]))
            break;
    if (i == FTBL.EV_GetPlayerFlag(15, 43))
        end = 0;
    else {
        EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, end - delay, 0);
        EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, end - delay, 0);
        EVT_AT_SetPos(URNI_PLAYER, reader, 0.0f, 0.0f, -1.4f, delay);
        EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
        EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetCam(1202, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
        QueueEventVoiceList(0, uranaiRcdIntroductionVoices, 10, 0x80000000, delay);
    }
    return end;
}
#else
int UranaiRcdIntroduction(shen_name reader, int delay)
{
    int end = delay + 550;
    int index = 20;
    while (index > 0) {
        if (FTBL.EV_GetShenmueFlag(uranaiRcdFlags[index]))
            break;
        index--;
    }
    if (index == FTBL.EV_GetPlayerFlag(15, 43))
        end = 0;
    else {
        EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
        EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
        EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
        EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
        QueueEventVoiceList(0, uranaiRcdIntroductionVoices, 10, 0x80000000, delay);
    }
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdSame(shen_name reader, int delay)
{
    int leave = delay + 300;
    int end;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, leave - delay, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, leave - delay, 0);
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.4f, 180.0f, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_RCD_DECLINE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1202, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiRcdSameVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiRcdSame(shen_name reader, int delay)
{
    int leave = delay + 530;
    int end;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_RCD_DECLINE_MOTION, 210.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiRcdSameVoices, 10, 0x80000000, delay + 30);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdDecline(shen_name reader, int delay)
{
    int leave = delay + 180;
    int end;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, leave - delay, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, leave - delay, 0);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiRcdDeclineVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiRcdDecline(shen_name reader, int delay)
{
    int leave = delay + 280;
    int end;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiRcdDeclineVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdBeginReading(shen_name reader, int delay)
{
    int end = delay + 120;
    int frames;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay, end - delay, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay, end - delay, 0);
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.4f, 180.0f, delay);
    frames = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetSound(993, end);
    QueueEventVoiceList(0, uranaiRcdBeginVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiRcdBeginReading(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.0f, -1.5f, 180.0f, delay + 120);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, delay + 120);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 120);
    EVT_AT_SetSound(993, delay + 120);
    QueueEventVoiceList(0, uranaiRcdBeginVoices, 10, 0x80000000, delay);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdReading(shen_name reader, int delay)
{
    int end = delay + 370;
    int i;
    shen_name name;
    EVT_AT_SetMot(reader, URNI_SEH_READING_MOTION, 0.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1204, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetSound(991, delay + 90);
    QueueEventVoiceList(0, uranaiRcdReadingVoices, 10, 0x80000000, delay);
    for (i = 0, name = URNI_STICK_HELD_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, delay + 60, 0);
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, delay + 60, 1);
    for (i = 0, name = URNI_STICK_HELD_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 1);
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 0);
    return end;
}
#else
int UranaiRcdReading(shen_name reader, int delay)
{
    int end = delay + 370;
    int i;
    shen_name name;
    EVT_AT_SetMot(reader, URNI_SEH_READING_MOTION, 0.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1204, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1232, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 60);
    EVT_AT_SetCam(1233, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 250);
    QueueEventVoiceList(0, uranaiRcdReadingVoices, 10, 0x80000000, delay);
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
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdResult(shen_name reader, int delay)
{
    int index;
    int frames;
    EVT_AT_SetMot(reader, URNI_RCD_RESULT_MOTION, 2.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, 0);
    index = 21;
    while (index > 0) {
        if (FTBL.EV_GetShenmueFlag(uranaiRcdFlags[index]))
            break;
        index--;
    }
    FTBL.EV_SetPlayerFlag(16, 43, index);
    frames = QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiRcdResultVoices[index], 10, 0x80000000, 0);
    return frames;
}
#else
int UranaiRcdResult(shen_name reader, int delay)
{
    int index;
    EVT_AT_SetMot(reader, URNI_RCD_RESULT_MOTION, 2.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1218, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, 0);
    index = 20;
    while (index > 0) {
        if (FTBL.EV_GetShenmueFlag(uranaiRcdFlags[index]))
            break;
        index--;
    }
    FTBL.EV_stepSetFlags(uranaiRcdFlags[index], 0);
    FTBL.EV_SetPlayerFlag(16, 43, index);
    QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiRcdResultVoices[index], 10, 0x80000000, 0);
    return 0;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdPayment(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 2, delay + 100, 350, 0);
    EVT_AT_SetPoint(reader, URNI_PLAYER, 5, 2, 2, delay + 100, 350, 0);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 510);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1202, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetSound(992, delay + 90);
    EVT_AT_SetSound(996, delay + 480);
    QueueEventVoiceList(0, uranaiRcdPaymentVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiRcdPayment(shen_name reader, int delay)
{
    int i;
    shen_name name;
    int end;
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 250);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 240);
    EVT_AT_SetSound(996, delay + 250);
    QueueEventVoiceList(0, uranaiRcdPaymentVoices, 10, 0x80000000, delay);
    for (i = 0, name = URNI_STICK_TABLE_FIRST; i < 6; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 1);
    for (i = 0, name = URNI_STICK_IDLE_FIRST; i < 5; i++, name += 0x1000000)
        EVT_AT_CharaDisp(name, end, 0);
    return end;
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
int UranaiRcdInsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 420;
    int end;
    EVT_AT_StartMoneyCount(0, delay + 10);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.4f, 0, leave - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1203, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiRcdInsufficientVoices, 10, 0x80000000, delay);
    return end;
}
#else
int UranaiRcdInsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 520;
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
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, leave - delay - 80, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, wallet);
    QueueEventVoiceList(0, uranaiRcdInsufficientVoices, 10, 0x80000000, delay);
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
#include "s2/motion.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
std::int64_t UranaiRcdSetup(shen_name reader, std::int32_t) {
    EventSetPosition position{.cleanup = 0, .character = URNI_STICK_CUP, .parent = reader, .position = URNI_RCD_CUP, .delay = 0};
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
        position.position = URNI_RCD_IDLE[i];
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{.cleanup = 0, .character = name, .parent = URNI_INVALID, .rotation = {47331, 0, 0}, .delay = 0};
        EVT_AT_SetAng_Callback(&rotation);
        EV_EnableCharacterDisplay(name);
    }
    return 0;
}
std::int64_t UranaiRcdModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 270;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1217, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_RCD_MODE_ONE_VOICES, 10, 0x80000000U, delay);
    EV_DisableCharacterDisplay(URNI_PLAYER);
    EVT_AT_ObjectDisp_1(URNI_PLAYER, end, 0);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiRcdIntroduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 550;
    std::int32_t index = 20;
    while (index > 0) {
        if (EV_GetShenmueFlag(URNI_RCD_FLAGS[index]))
            break;
        --index;
    }
    if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 43)))
        return 0;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_RCD_INTRODUCTION_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiRcdSame(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 528) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_RCD_DECLINE_MOTION, 210, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 520, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_RCD_SAME_VOICES, 10, 0x80000000U, delay + 30);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiRcdDecline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 278) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 270, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_RCD_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiRcdBeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 120;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0, -1.5f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_RCD_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiRcdReading(shen_name reader, std::int32_t delay) {
    EVT_AT_SetMot(reader, URNI_SEH_READING_MOTION, 0, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1204, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1232, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 60);
    EVT_AT_SetCam(1233, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 250);
    QueueEventVoiceList(nullptr, URNI_RCD_READING_VOICES, 10, 0x80000000U, delay);
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
    return static_cast<std::uint32_t>(delay + 370);
}
std::int64_t UranaiRcdResult(shen_name reader, std::int32_t) {
    EventSetMotion motion{.cleanup = 0, .character = reader, .animation = URNI_RCD_RESULT_MOTION, .frame = 2, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(URNI_RCD_RESULT_MOTION);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1218, .eye = URNI_READER_DUMMY, .target = URNI_READER_DUMMY, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    std::int32_t index = 20;
    while (index > 0) {
        if (EV_GetShenmueFlag(URNI_RCD_FLAGS[index]))
            break;
        --index;
    }
    EV_stepSetFlags(0, URNI_RCD_FLAGS[index]);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 43, index);
    QueueEventVoiceList(HLib::GetCurrentTask(), URNI_RCD_RESULT_VOICES[index], 10, 0x80000000U, 0);
    return 0;
}
std::int64_t UranaiRcdPayment(shen_name, std::int32_t delay) {
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 250);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 240);
    EVT_AT_SetSound(0x3e4, delay + 250);
    QueueEventVoiceList(nullptr, URNI_RCD_PAYMENT_VOICES, 10, 0x80000000U, delay);
    shen_name name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 1);
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 0);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiRcdInsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 518) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_RCD_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 440, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_RCD_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
#endif
