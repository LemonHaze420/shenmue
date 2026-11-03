#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_urni.hpp"

#define URNI_CRYSTAL SHEN_NAME4('S', 'U', 'I', 'S')
#define URNI_DOVE SHEN_NAME4('H', 'A', 'T', 'O')
#define URNI_SRI_TALK_TWO_MOTION 6649
#define URNI_SRI_END_MOTION 6646
#define URNI_SRI_START_MOTION 6648
#define URNI_SRI_LOOP_MOTION 6647
#define URNI_SRI_TALK_MOTION 6650

typedef struct UranaiProjection {
    shen_name character;
    Vector3 position;
    float yaw, scale;
} UranaiProjection;

static int UranaiSriQueueMessages(HLibTask *parent, const char **messages, int voiceDelayFrames, unsigned int mode, int delay)
{
    int offset = 0;
    while (*messages) {
        QueueEventMessage(*messages++, delay + offset, 60);
        offset += 60;
    }
    return offset;
}

static const char *uranaiSriModeOneVoices[] = {0};
static const char *uranaiSriIntroductionVoices[] = {"A087", "H013", "H001", 0};
static const char *uranaiSriSameVoices[] = {"H009", "A062", "H010", "H011", "A010", 0};
static const char *uranaiSriDeclineVoices[] = {"A133", "H002", "A134", 0};
static const char *uranaiSriBeginVoices[] = {"A002", "H003", 0};
static const char *uranaiSriReadingVoices[] = {"H004", 0};
static const char *uranaiSriPaymentVoices[] = {"H005", "H006", "A007", 0};
static const char *uranaiSriInsufficientVoices[] = {"A020", "H012", "A135", 0};
static const char *uranaiSriResultVoices[28][16] = {
    {"H016", "H017", "H018", "A221", 0},
    {"H019", "H021", "A234", "H022", "A233", 0},
    {"H023", "A235", "H024", "A236", "H025", "H026", "H027", "A233", 0},
    {"H023", "H029", "A237", "H030", "H031", "A152", "H032", "H033", "A233", 0},
    {"H034", "H035", "A229", "H036", "H037", "A233", 0},
    {"H038", "A240", "H039", "A241", "H040", "A221", 0},
    {"H041", "A242", "H042", "H043", "A243", 0},
    {"H048", "H049", "H050", "A047", "H051", "A233", 0},
    {"H044", "A244", "H045", "H046", "H047", "A233", 0},
    {"H044", "A244", "H045", "H046", "H047", "A233", 0},
    {"H048", "H049", "H050", "A047", "H051", "A233", 0},
    {"H052", "H053", "A245", "H054", "H055", "A246", "H056", "A243", 0},
    {"H057", "H058", "A227", "H059", "H060", "H061", "A221", 0},
    {"H062", "A248", "H063", "A233", 0},
    {"H062", "A248", "H063", "A233", 0},
    {"H065", "H066", "A251", "H067", "H068", "A250", "H069", "A221", 0},
    {"H073", "A251", "H074", "H075", "A243", 0},
    {"H070", "H085", "A255", "H086", "H087", "H088", "A221", 0},
    {"H070", "H090", "A256", "H091", "H092", "A233", 0},
    {"H070", "H093", "A257", "H094", "H095", "A233", 0},
    {"H070", "H096", "A258", "H097", "H098", "A233", 0},
    {"H070", "H085", "A255", "H086", "H087", "H088", "A221", 0},
    {"H099", "H100", "H101", "H102", "H103", "H104", "H105", "H106", "H108", "H109", "A259", 0},
    {"H076", "A252", "H079", "H081", "A254", "H080", "A243", 0},
    {"H070", "A249", "H079", "A253", "H080", "A243", 0},
    {"H070", "A249", "H082", "H083", "A243", 0},
    {"H076", "A252", "H077", "H078", "A243", 0},
    {"H070", "A249", "H071", "H072", "A243", 0},
};
static int uranaiSriStoryFlags[28] = {1524, 1525, 1527, 1528, 1529, 1530, 1531, 1532, 1533, 1534, 1535, 1536, 1537, 1538, 1539, 1540, 1541, 1542,
    1543, 1544, 1545, 1546, 1547, 1548, 1549, 1550, 1551, 1552};
static const char *uranaiSriPackages[28] = {"0120_YAN", "0120_CHD", "0120_TEJ", "0120_YAN", "0120_QRR", "0120_GAI", "0120_KUK", "0120_HAT",
    "0120_QCN", "0120_QCN", "0120_HAT", "0120_SGT", "0120_KT1", "0120_KYC", "0120_KYC", "0120_GAI", "0120_SSY", "0120_CHN", "0120_AW3", "0120_SF6",
    "0120_SF3", "0120_CHN", "0120_TGY", "0120_AW1", "0120_VNE", "0120_SF5", "0120_KFT", "0120_OXO"};
static const UranaiProjection uranaiSriProjections[28] = {
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.89f, -0.83f}, 0.0f, 0.4f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.89f, -0.83f}, 0.0f, 0.7f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 2.0f},
    {URNI_DOVE, {0.025f, 0.89f, -0.83f}, 30.0f, 0.12f},
    {URNI_CRYSTAL, {0.025f, 0.89f, -0.83f}, 0.0f, 0.23f},
    {URNI_CRYSTAL, {0.025f, 0.89f, -0.83f}, 0.0f, 0.23f},
    {URNI_DOVE, {0.025f, 0.89f, -0.83f}, 30.0f, 0.12f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.6f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.4f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.4f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.6f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.1f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.4f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.6f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.2f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
    {URNI_CRYSTAL, {0.025f, 0.82f, -0.83f}, 0.0f, 1.5f},
};

int UranaiStoryPackageIndex(void)
{
    int index;
    if (FTBL.EV_GetShenmueFlag(1526))
        index = 1;
    else {
        index = 27;
        while (index > 0) {
            if (FTBL.EV_GetShenmueFlag(uranaiSriStoryFlags[index]))
                break;
            index--;
        }
    }
    return index;
}

const char *UranaiStoryPackage(void)
{
    return uranaiSriPackages[UranaiStoryPackageIndex()];
}

int UranaiSriSetup(shen_name reader, int delay)
{
    int index;
    Vector3 scale;
    FTBL.UnknownScenarioGeometryLightA(URNI_CRYSTAL, "SUISYO00", 0);
    index = UranaiStoryPackageIndex();
    FTBL.AT_SetLocalBaseChara(uranaiSriProjections[index].character, reader);
    EVT_AT_SetPos(uranaiSriProjections[index].character, reader, uranaiSriProjections[index].position.x, uranaiSriProjections[index].position.y,
        uranaiSriProjections[index].position.z, 0);
    EVT_AT_SetAngY(uranaiSriProjections[index].character, reader, uranaiSriProjections[index].yaw, 0);
    scale.x = scale.y = scale.z = uranaiSriProjections[index].scale;
    FTBL.mtwkSetMotionworkScale3(uranaiSriProjections[index].character, &scale);
    FTBL.SetScenarioCharacterTransparency(uranaiSriProjections[index].character, 1.0f);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(uranaiSriProjections[index].character), 0);
    return 0;
}

int UranaiSriModeOne(shen_name reader, int delay)
{
    int end = delay + 330;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(0, uranaiSriModeOneVoices, 10, 0x80000000, delay);
    EVT_AT_CharaDisp(URNI_PLAYER, 0, 1);
    EVT_AT_CharaDisp(URNI_PLAYER, end, 0);
    return end;
}

int UranaiSriIntroduction(shen_name reader, int delay)
{
    int end = delay + 175;
    UranaiStoryPackageIndex();
    if (FTBL.EV_GetPlayerFlag(15, 44) == -100)
        end = 0;
    else {
        EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
        EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
        EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
        EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
        QueueEventVoiceList(0, uranaiSriIntroductionVoices, 10, 0x80000000, delay);
    }
    return end;
}

int UranaiSriSame(shen_name reader, int delay)
{
    int end;
    end = EVT_AT_SetMot(reader, URNI_SRI_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2.0f, 1.0f, 0, end);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 390);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 400);
    EVT_AT_SetSound(996, delay + 390);
    QueueEventVoiceList(0, uranaiSriSameVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiSriDecline(shen_name reader, int delay)
{
    int leave = delay + 280;
    int turn, end;
    EVT_AT_SetPos(URNI_PLAYER, reader, -0.45f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180.0f, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0, leave - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, turn);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, turn);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, leave - delay - 10, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, delay);
    QueueEventVoiceList(0, uranaiSriDeclineVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiSriBeginReading(shen_name reader, int delay)
{
    int end;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, -0.0f, -1.4f, 180.0f, delay + 120);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2.0f, 1.0f, 0, delay + 120);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2.0f, 1.0f, 0, end);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 120);
    EVT_AT_SetSound(993, delay + 120);
    QueueEventVoiceList(0, uranaiSriBeginVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiSriReading(shen_name reader, int delay)
{
    int end = delay + 340;
    int frames;
    frames = EVT_AT_SetMot(reader, URNI_SRI_START_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SRI_LOOP_MOTION, 2.0f, 1.0f, 0, frames);
    EVT_AT_SetCam(1237, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    if (UranaiStoryPackageIndex() != FTBL.EV_GetPlayerFlag(15, 44))
        QueueEventVoiceList(0, uranaiSriReadingVoices, 10, 0x80000000, delay + 150);
    return end;
}

int UranaiSriResult(shen_name reader, int delay)
{
    int result = 0;
    int index = UranaiStoryPackageIndex();
    if (index == FTBL.EV_GetPlayerFlag(15, 44))
        result = -1;
    else {
        FTBL.EV_stepSetFlags(uranaiSriStoryFlags[index], 0);
        FTBL.EV_SetPlayerFlag(16, 44, index);
        EVT_AT_CharaDisp(uranaiSriProjections[index].character, 0, 0);
        EVT_AT_SetMot(reader, URNI_SRI_TALK_MOTION, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetCam(1207, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, 0);
        QueueEventVoiceList(FTBL.HLib_GetCurrentTask(), uranaiSriResultVoices[index], 10, 0x80000000, 0);
    }
    return result;
}

int UranaiSriPayment(shen_name reader, int delay)
{
    int index, end;
    index = UranaiStoryPackageIndex();
    EVT_AT_CharaDisp(uranaiSriProjections[index].character, delay, 1);
    end = EVT_AT_SetMot(reader, URNI_SRI_END_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2.0f, 1.0f, 0, end);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2.0f, 1.0f, 0, delay + 320);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2.0f, 1.0f, 0x20000000, end - 2);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2.0f, 0.35f, 0x20000000, end - 2);
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180.0f, end - 7);
    end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2.0f, 1.0f, 0, end - 7);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 330);
    EVT_AT_SetSound(996, delay + 320);
    QueueEventVoiceList(0, uranaiSriPaymentVoices, 10, 0x80000000, delay);
    return end;
}

int UranaiSriInsufficientCash(shen_name reader, int delay)
{
    int leave = delay + 340;
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
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, leave - delay - 80, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0x80000000, wallet);
    QueueEventVoiceList(0, uranaiSriInsufficientVoices, 10, 0x80000000, delay);
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
#include "s2/model_image.hpp"
#include "s2/motion.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
std::int64_t UranaiSriSetup(shen_name reader, std::int32_t) {
    ReplaceScenarioImageModule(URNI_CRYSTAL, "SUISYO00", 0);
    const UranaiProjection &projection = URNI_SRI_PROJECTIONS[UranaiStoryPackageIndex()];
    AT_SetLocalBaseChara(projection.character, reader);
    EventSetPosition position{.cleanup = 0, .character = projection.character, .parent = reader, .position = projection.position, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = projection.character, .parent = reader, .degrees = projection.yaw, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    const Vector3 scale{projection.scale, projection.scale, projection.scale};
    mtwkSetMotionworkScale3(projection.character, &scale);
    SetScenarioCharacterTransparency(projection.character, 1);
    SetPluralControlFor(ctrlGetCharaByName(projection.character), 0);
    return 0;
}
std::int64_t UranaiSriModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 330;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, urniSriModeOneVoices, 10, 0x80000000U, delay);
    EV_DisableCharacterDisplay(URNI_PLAYER);
    EVT_AT_ObjectDisp_1(URNI_PLAYER, end, 0);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriIntroduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 175;
    UranaiStoryPackageIndex();
    if (static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 44)) == -100)
        return 0;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_SRI_INTRODUCTION_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriSame(shen_name reader, std::int32_t delay) {
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_SRI_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0, readingEnd);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 390);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 400);
    EVT_AT_SetSound(0x3e4, delay + 390);
    QueueEventVoiceList(nullptr, URNI_SRI_SAME_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriDecline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 278) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 270, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SRI_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriBeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 120;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, -0.0f, -1.4f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_SRI_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriReading(shen_name reader, std::int32_t delay) {
    const std::int32_t loop = EVT_AT_SetMot(reader, URNI_SRI_START_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_LOOP_MOTION, 2, 1, 0, loop);
    EVT_AT_SetCam(1237, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    const std::int32_t index = UranaiStoryPackageIndex();
    if (index != static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 44)))
        QueueEventVoiceList(nullptr, URNI_SRI_READING_VOICES, 10, 0x80000000U, delay + 150);
    return static_cast<std::uint32_t>(delay + 340);
}
std::int64_t UranaiSriResult(shen_name reader, std::int32_t) {
    const std::int32_t index = UranaiStoryPackageIndex();
    if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 44)))
        return 0xffffffffLL;
    EV_stepSetFlags(0, URNI_SRI_STORY_FLAGS[index]);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 44, index);
    EV_EnableCharacterDisplay(URNI_SRI_PROJECTIONS[index].character);
    EventSetMotion motion{.cleanup = 0, .character = reader, .animation = URNI_SRI_TALK_MOTION, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(URNI_SRI_TALK_MOTION);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1207, .eye = URNI_READER_DUMMY, .target = URNI_READER_DUMMY, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueEventVoiceList(HLib::GetCurrentTask(), URNI_SRI_RESULT_VOICES[index], 10, 0x80000000U, 0);
    return 0;
}
std::int64_t UranaiSriPayment(shen_name reader, std::int32_t delay) {
    EVT_AT_ObjectDisp_1(URNI_SRI_PROJECTIONS[UranaiStoryPackageIndex()].character, delay, 1);
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_SRI_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0, readingEnd);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 320);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 330);
    EVT_AT_SetSound(0x3e4, delay + 320);
    QueueEventVoiceList(nullptr, URNI_SRI_PAYMENT_VOICES, 10, 0x80000000U, delay);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriInsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 338) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 260, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_SRI_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
#endif
