#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name URNI_AREA{SHEN_NAME(URNI)}, URNI_TASK{SHEN_NAME(EVNT)}, URNI_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name URNI_BASE{SHEN_NAME(BASE)}, URNI_PLAYER_DUMMY{SHEN_NAME(DUMR)}, URNI_READER_DUMMY{SHEN_NAME(DUMT)};
inline constexpr shen_name URNI_SYK{SHEN_NAME(SYK_)}, URNI_KLN{SHEN_NAME(KLN_)}, URNI_MM5{SHEN_NAME(MM5_)};
inline constexpr shen_name URNI_SEH{SHEN_NAME(SEH_)}, URNI_SRI{SHEN_NAME(SRI_)}, URNI_RCD{SHEN_NAME(RCD_)};
inline constexpr shen_name URNI_INVALID{0xffffffffU}, URNI_AR01{SHEN_NAME(AR01)}, URNI_WS00{SHEN_NAME(WS00)};
struct UranaiSelection {
    shen_name reader;
    std::int32_t mode;
};
using UranaiCallback = std::int64_t (*)(shen_name, std::int32_t);
struct UranaiConfiguration {
    Vector3 position;
    float yaw;
    std::int32_t price, reserved14;
    UranaiCallback setup, modeOne, introduction, same, decline, beginReading, reading, result, payment, insufficientCash;
};
struct UranaiWork {
    EventReadCharactersWork *characters;
    HLib::Task *task;
    const UranaiSelection *selection;
    const UranaiConfiguration *configuration;
    const char *package;
};
static_assert(sizeof(UranaiSelection) == 8);
static_assert(sizeof(UranaiConfiguration) == 0x68 && offsetof(UranaiConfiguration, setup) == 0x18);
static_assert(offsetof(UranaiConfiguration, beginReading) == 0x40 && offsetof(UranaiConfiguration, insufficientCash) == 0x60);
static_assert(sizeof(UranaiWork) == 0x28 && offsetof(UranaiWork, selection) == 0x10 && offsetof(UranaiWork, package) == 0x20);
inline constexpr UranaiSelection URNI_SELECTIONS[]{{.reader = URNI_SYK, .mode = 2}, {.reader = URNI_KLN, .mode = 2}, {.reader = URNI_MM5, .mode = 2},
    {.reader = URNI_SEH, .mode = 2}, {.reader = URNI_SRI, .mode = 2}, {.reader = URNI_RCD, .mode = 2}};
inline constexpr const char *URNI_SYK_MODE_ONE_VOICES[]{"I001", nullptr};
inline constexpr const char *URNI_SYK_INTRODUCTION_VOICES[]{"A001", "I002", nullptr};
inline constexpr const char *URNI_SYK_SAME_VOICES[]{"A001", "I009", "I010", "A010", nullptr};
inline constexpr const char *URNI_SYK_DECLINE_VOICES[]{"A003", "I003", "I004", "A015", nullptr};
inline constexpr const char *URNI_SYK_BEGIN_VOICES[]{"A002", "I005", nullptr};
inline constexpr const char *URNI_SYK_READING_VOICES[]{"I007", "A035", "I008", "A004", nullptr};
inline constexpr const char *URNI_SYK_PAYMENT_VOICES[]{"I045", "I048", "A260", "I049", "A261", nullptr};
inline constexpr const char *URNI_SYK_INSUFFICIENT_VOICES[]{"A020", "I011", "A021", "I012", "A022", "I013", "A137", nullptr};
inline constexpr const char *URNI_SYK_RESULT_VOICES[10][16]{{"I015", "I016", "I017", nullptr}, {"I018", "I019", "I020", nullptr},
    {"I021", "I022", "I023", nullptr}, {"I024", "I025", "I026", nullptr}, {"I027", "I028", "I029", nullptr}, {"I030", "I031", "I032", nullptr},
    {"I033", "I034", "I035", nullptr}, {"I036", "I037", "I038", nullptr}, {"I039", "I040", "I041", nullptr}, {"I042", "I043", "I044", nullptr}};
inline constexpr shen_name URNI_READING_OBJECT{SHEN_NAME(MMEG)};
inline constexpr Vector3 URNI_SYK_OBJECT_POSITION{0.06599999964237213f, -0.05700000002980232f, 0.006000000052154064f};
inline constexpr Vector3i URNI_SYK_OBJECT_ROTATION{50972, 52610, 8192};
inline constexpr std::uint32_t URNI_STAND_MOTION = 0xf002, URNI_TURN_MOTION = 0xf02f, URNI_WALK_MOTION = 0xf005, URNI_EMPTY_WALLET_MOTION = 0xf019;
inline constexpr std::uint32_t URNI_TALK_MOTION = 6615, URNI_SIT_MOTION = 6534, URNI_SITTING_MOTION = 6533, URNI_STAND_FROM_CHAIR_MOTION = 6535;
inline constexpr std::uint32_t URNI_SHIFT_LEFT_MOTION = 6623, URNI_SYK_WAIT_MOTION = 6658, URNI_SYK_REPEAT_MOTION = 6657, URNI_SYK_DECLINE_MOTION = 6655;
inline constexpr std::uint32_t URNI_SYK_INVITE_MOTION = 6656, URNI_SYK_READING_START_MOTION = 6653, URNI_SYK_READING_LOOP_MOTION = 6652;
inline constexpr std::uint32_t URNI_SYK_READING_END_MOTION = 6651, URNI_SYK_ANSWER_MOTION = 6654;
inline constexpr shen_name URNI_CARD_TABLE{SHEN_NAME(HAR0)}, URNI_CARD_DECK{SHEN_NAME(HAR1)};
inline constexpr shen_name URNI_PLAYER_CARD{SHEN_NAME(HAR2)}, URNI_READER_CARD{SHEN_NAME(HAR3)};
inline constexpr Vector3 URNI_CARD_TABLE_POSITION{278.57501220703125f, 100.81099700927734f, 135.8249969482422f};
inline constexpr Vector3 URNI_CARD_DECK_POSITION{278.4949951171875f, 100.76100158691406f, 136.1840057373047f};
inline constexpr Vector3 URNI_PLAYER_CARD_POSITION{0.13300000131130219f, 0.03500000014901161f, -0.04100000113248825f};
inline constexpr Vector3 URNI_READER_CARD_POSITION{0.24300000071525574f, 0.029999999329447746f, -0.009999999776482582f};
inline constexpr Vector3i URNI_PLAYER_CARD_ROTATION{35953, 46967, 0}, URNI_READER_CARD_ROTATION{40504, 51518, 25484};
struct UranaiCardRotation {
    float baseYaw, reserved04, stepAngle, spinAngle, amplitude;
};
static_assert(sizeof(UranaiCardRotation) == 20 && offsetof(UranaiCardRotation, amplitude) == 16);
inline constexpr std::uint32_t URNI_KLN_TALK_MOTION = 6616, URNI_KLN_DECLINE_MOTION = 6619, URNI_KLN_READING_MOTION = 6621;
inline constexpr std::uint32_t URNI_PLAYER_CARD_READING_MOTION = 6622, URNI_KLN_RESULT_MOTION = 6617;
inline constexpr const char *URNI_KLN_MODE_ONE_VOICES[]{"G001", "G015", nullptr};
inline constexpr const char *URNI_KLN_INTRODUCTION_VOICES[]{"A001", "G002", nullptr};
inline constexpr const char *URNI_KLN_SAME_VOICES[]{"A001", "G010", "G011", "A010", nullptr};
inline constexpr const char *URNI_KLN_DECLINE_VOICES[]{"A003", "G003", "A077", nullptr};
inline constexpr const char *URNI_KLN_BEGIN_VOICES[]{"A002", "G004", nullptr};
inline constexpr const char *URNI_KLN_READING_VOICES[]{"G005", nullptr};
inline constexpr const char *URNI_KLN_READING_END_VOICES[]{"G006", "G009", nullptr};
inline constexpr const char *URNI_KLN_PAYMENT_VOICES[]{"A007", nullptr};
inline constexpr const char *URNI_KLN_INSUFFICIENT_VOICES[]{"A020", "G012", "A131", "G013", "G014", "A132", nullptr};
inline constexpr const char *URNI_KLN_RESULT_VOICES[10][10]{{"G017", "G018", nullptr}, {"G019", "G020", nullptr}, {"G021", "G022", nullptr},
    {"G023", "G024", nullptr}, {"G025", "G026", nullptr}, {"G027", "G028", nullptr}, {"G029", "G030", nullptr}, {"G031", "G032", nullptr},
    {"G033", "G034", nullptr}, {"G035", "G036", nullptr}};
inline constexpr Vector3 URNI_MM5_OBJECT_POSITION{0.08255299925804138f, 0.0046629998832941055f, -0.01279400009661913f};
inline constexpr Vector3i URNI_MM5_OBJECT_ROTATION{32449, 32709, 30381};
inline constexpr std::int32_t URNI_MM5_HK_FLAGS[]{
    1500, 1501, 1502, 1503, 1506, 1507, 1508, 1509, 1510, 1511, 1512, 1513, 1514, 1515, 1518, 1519, 1520, 1521, 1522, 1523};
inline constexpr std::int32_t URNI_MM5_KL_FLAGS[]{1500, 1526, 1529, 1531, 1532, 1536, 1537, 1538, 1541, 1542, 1543, 1544, 1545, 1546, 1547};
inline constexpr std::int32_t URNI_MM5_CAMERAS[]{1212, 1243, 1240, 1241, 1242, 1244};
inline constexpr std::uint32_t URNI_MM5_IDLE_MOTION = 6626, URNI_MM5_GREETING_MOTION = 6643, URNI_MM5_END_MOTION = 6627;
inline constexpr std::uint32_t URNI_MM5_DECLINE_MOTION = 6624, URNI_MM5_START_MOTION = 6630, URNI_MM5_LOOP_MOTION = 6629;
inline constexpr std::uint32_t URNI_PLAYER_PALM_END_MOTION = 6639, URNI_PLAYER_PALM_START_MOTION = 6641, URNI_PLAYER_PALM_LOOP_MOTION = 6640;
inline constexpr const char *URNI_MM5_SPECIAL_VOICES[5][32]{
    {"D353", "D354", "D355", "D356", "D357", "D358", "D359", nullptr},
    {"D382", "D383", "A292", "D384", "D385", "D386", "D387", "D388", nullptr},
    {"D360", "A285", "D361", "A286", "D362", "D363", "D364", "D365", "A287", "D366", "D367", "A288", "D368", "D369", nullptr},
    {"D371", "A289", "D372", "A290", "D373", "D374", "D375", "D376", "D377", "D378", "D379", "D380", "A291", "D381", nullptr},
    {"D389", "A293", "D390", "D391", "A285", "D392", "A294", "D393", "D394", "D395", "D396", "D397", "D398", "D399", "D400", "D401", nullptr},
};
inline constexpr const char *URNI_MM5_HK_VOICES[20][32]{
    {"D152", "D153", "D157", "A194", "D276", "A177", nullptr},
    {"D179", "D180", "D181", "D182", "A152", "D183", "A153", "D184", nullptr},
    {"D185", "D186", "D187", "A154", "D188", "D183", "A153", "D184", nullptr},
    {"D189", "D190", "A155", "D191", "A156", "D192", "D193", "D184", nullptr},
    {"D194", "A157", "D195", "A158", "D191", "D196", "A159", "D197", "D198", "A160", "D199", "D184", nullptr},
    {"D194", "A157", "D195", "A158", "D191", "D200", "A161", "D197", "D198", "A160", "D199", "D184", nullptr},
    {"D200", "A161", "D197", "D198", "A160", "D199", "D184", nullptr},
    {"D201", "D202", "A162", "D203", "D204", "A163", "D205", "D206", "A164", "D184", nullptr},
    {"D207", "D208", "D209", "D210", "A165", "D211", "A166", nullptr},
    {"D212", "D213", "A167", "D214", "D215", "D216", "D217", "A169", nullptr},
    {"D218", "D213", "A167", "D219", "A170", "D220", "A171", nullptr},
    {"D221", "D222", "A172", "D223", "A170", "D224", "A171", nullptr},
    {"D225", "D226", "A173", "D227", "A174", "D228", "A175", nullptr},
    {"D229", "A177", "D230", "D231", "D232", "A174", "D233", "A177", nullptr},
    {"D234", "D235", "A178", "D236", "A179", "D237", "D238", "A174", "D184", nullptr},
    {"D240", "A157", "D241", "A181", "D242", "A198", nullptr},
    {"D244", "D245", "D246", "A182", "D247", "A183", "D248", "A184", "D249", "D250", "A166", nullptr},
    {"D251", "D252", "A185", "D253", "D254", "D255", "D256", "A186", "D184", nullptr},
    {"D257", "A187", "D258", "D259", "D260", "D261", "A188", "D184", "D262", "A189", nullptr},
    {"D264", "A156", "D265", "D266", "A174", "D267", "D268", "A195", "D269", "D270", "A191", "D271", "D272", "A192", "D273", "D274", "A193", nullptr},
};
inline constexpr const char *URNI_MM5_KL_VOICES[15][16]{
    {"D152", "D153", "D157", "A194", "D276", "A177", nullptr},
    {"D280", "D281", "A195", "D282", "A196", "D283", "D284", "D285", "A197", nullptr},
    {"D286", "D287", "D288", "A195", "D289", "A092", "D290", "A156", "D291", "D292", "A198", nullptr},
    {"D293", "D294", "D295", "A199", "D296", "D297", "A197", nullptr},
    {"D298", "D299", "A200", "D300", "A201", "D301", "D302", "D303", "D304", "D305", "A202", nullptr},
    {"D306", "D307", "D308", "A203", "D309", "D310", "A204", "D311", nullptr},
    {"D312", "A205", "D313", "D314", "A206", "D315", "A207", "D316", "D317", "A177", nullptr},
    {"D318", "A174", "D319", "D320", "A208", "D321", "D322", "A198", nullptr},
    {"D323", "D324", "D325", "D326", "A209", "D327", "A210", "D328", "A211", "D329", "A212", "D330", "A166", nullptr},
    {"D152", "D277", "D156", "D157", "D276", "A177", nullptr},
    {"D331", "D332", "D333", "A213", "D334", "A214", "D335", "A215", "D336", "A216", nullptr},
    {"D331", "D337", "D333", "A213", "D338", "A214", "D335", "A215", "D336", "A216", nullptr},
    {"D331", "D339", "D340", "A217", "D341", "A214", "D335", "A215", "D336", "A216", nullptr},
    {"D152", "D158", "D159", "D160", "D278", "D279", "A177", nullptr},
    {"D343", "A196", "D344", "D345", "A195", "D346", "D347", "A215", "D348", "D349", "D350", "D351", "D352", "A197", nullptr},
};
inline constexpr const char *URNI_MM5_MODE_ONE_VOICES[]{"D001", nullptr};
inline constexpr const char *URNI_MM5_INTRODUCTION_VOICES[]{"D001", "D002", "D003", nullptr};
inline constexpr const char *URNI_MM5_SAME_VOICES[]{"D008", "A062", "D009", "D010", "A063", nullptr};
inline constexpr const char *URNI_MM5_DECLINE_VOICES[]{"A003", "D004", nullptr};
inline constexpr const char *URNI_MM5_BEGIN_VOICES[]{"A128", nullptr};
inline constexpr const char *URNI_MM5_PAYMENT_VOICES[]{"D005", "D007", "A060", nullptr};
inline constexpr const char *URNI_MM5_INSUFFICIENT_VOICES[]{"A020", "D011", "D012", "A064", nullptr};
inline const char *urniMm5ReadingVoices[1]{};
inline constexpr Vector3 URNI_SEH_CUP{-0.15000000596046448f, 0.75f, -0.6499999761581421f};
inline constexpr Vector3 URNI_SEH_ATTACH{0.1080000028014183f, 0.0f, 0.012000000104308128f};
inline constexpr Vector3 URNI_SEH_SCALE{2.0f, 2.0f, 1.0f};
inline constexpr Vector3i URNI_SEH_ROTATE{4915, 65171, 60620};
inline constexpr Vector3i URNI_SEH_HAND_ROTATE{50972, 2730, 61895};
inline constexpr Vector3 URNI_SEH_TABLE[]{
    {0.10000000149011612f, 0.75f, -0.6700000166893005f},
    {0.07000000029802322f, 0.75f, -0.6800000071525574f},
    {0.05000000074505806f, 0.75f, -0.6700000166893005f},
    {-0.0f, 0.75f, -0.6600000262260437f},
    {-0.029999999329447746f, 0.75f, -0.6499999761581421f},
    {-0.09000000357627869f, 0.75f, -0.6499999761581421f},
};
inline constexpr Vector3 URNI_SEH_HAND[]{
    {0.10199999809265137f, 0.10000000149011612f, 0.024000000208616257f},
    {0.12999999523162842f, 0.10000000149011612f, 0.035999998450279236f},
    {0.12399999797344208f, 0.10000000149011612f, 0.020999999716877937f},
    {0.10199999809265137f, 0.10000000149011612f, 0.03200000151991844f},
    {0.11299999803304672f, 0.10000000149011612f, 0.028999999165534973f},
};
inline constexpr Vector3 URNI_SEH_IDLE[]{
    {-0.15000000596046448f, 0.8999999761581421f, -0.6700000166893005f},
    {-0.17000000178813934f, 0.8999999761581421f, -0.6800000071525574f},
    {-0.14000000059604645f, 0.8999999761581421f, -0.6700000166893005f},
    {-0.1599999964237213f, 0.8999999761581421f, -0.6600000262260437f},
    {-0.15000000596046448f, 0.8999999761581421f, -0.6499999761581421f},
};
inline constexpr std::int32_t URNI_SEH_FLAGS[]{
    1500, 1524, 1526, 1529, 1531, 1532, 1533, 1534, 1535, 1536, 1537, 1538, 1541, 1542, 1543, 1544, 1545, 1546, 1547};
inline constexpr const char *URNI_SEH_INTRODUCTION_VOICES[]{"E001", "A125", "E031", "E032", nullptr};
inline constexpr const char *URNI_SEH_SAME_VOICES[]{"E040", "A062", "E041", "E042", "A010", nullptr};
inline constexpr const char *URNI_SEH_DECLINE_VOICES[]{"A126", "E033", "E034", "A146", nullptr};
inline constexpr const char *URNI_SEH_BEGIN_VOICES[]{"A128", "E035", nullptr};
inline constexpr const char *URNI_SEH_PAYMENT_VOICES[]{"E036", "E037", "E039", "A007", nullptr};
inline constexpr const char *URNI_SEH_INSUFFICIENT_VOICES[]{"A020", "E043", "A130", nullptr};
inline constexpr const char *URNI_SEH_RESULT_VOICES[19][16]{
    {"E110", "E112", "E114", "A170", "E116", "E118", "E120", "A122", nullptr},
    {"E050", "A219", "E051", "A140", "E052", "A092", "E053", "E054", "E055", "A010", nullptr},
    {"E056", "A220", "E057", "E058", "A004", "E059", "A221", "E060", "E061", "A010", nullptr},
    {"E062", "A222", "E063", "A223", "E064", "E065", "A010", nullptr},
    {"E066", "E067", "E068", "E069", "E070", "A224", "E071", "A010", nullptr},
    {"E110", "E111", "E114", "A170", "E115", "E119", "E120", "A122", nullptr},
    {"E110", "E111", "E113", "A170", "E115", "E117", "E120", "A122", nullptr},
    {"E110", "E111", "E114", "A170", "E115", "E119", "E120", "A122", nullptr},
    {"E110", "E111", "E113", "A170", "E115", "E117", "E120", "A122", nullptr},
    {"E072", "E073", "E074", "A225", "E075", "E076", "A010", nullptr},
    {"E077", "A226", "E078", "A227", "E079", "E080", "E081", "A010", nullptr},
    {"E082", "E083", "E084", "A221", "E085", "E086", "A010", nullptr},
    {"E087", "A228", "E088", "A004", "A229", "E089", "A230", "E090", "E091", "A010", nullptr},
    {"E092", "E093", "E094", "A221", "E095", "E096", "A010", nullptr},
    {"E097", "A231", "E098", "E099", "E100", "A232", "E101", "E102", "A010", nullptr},
    {"E097", "A231", "E098", "E099", "E100", "A232", "E101", "E102", "A010", nullptr},
    {"E097", "A231", "E098", "E099", "E100", "A232", "E101", "E102", "A010", nullptr},
    {"E092", "E093", "E094", "A221", "E095", "E096", "A010", nullptr},
    {"E103", "E104", "A221", "E105", "E106", "E107", "E108", "E109", "A233", nullptr},
};
inline const char *urniSehModeOneVoices[1]{}, *urniSehReadingVoices[1]{};
inline constexpr shen_name URNI_STICK_CUP{SHEN_NAME(ZCUP)}, URNI_STICK_HAND{SHEN_NAME(ZEIZ)};
inline constexpr shen_name URNI_STICK_TABLE_FIRST{SHEN_NAME(ZEIA)}, URNI_STICK_HELD_FIRST{SHEN_NAME(ZEI0)}, URNI_STICK_IDLE_FIRST{SHEN_NAME(ZEI5)};
inline constexpr std::uint32_t URNI_SEH_AWAKE_MOTION = 6557, URNI_SEH_START_MOTION = 6608, URNI_SEH_READING_MOTION = 6659;
inline constexpr std::uint32_t URNI_SEH_RESULT_START_MOTION = 6603, URNI_SEH_RESULT_LOOP_MOTION = 6602;
inline constexpr std::uint32_t URNI_SEH_RESULT_END_MOTION = 6601, URNI_SEH_DROWSY_MOTION = 6609;
inline constexpr shen_name URNI_CRYSTAL{SHEN_NAME(SUIS)}, URNI_DOVE{SHEN_NAME(HATO)};
struct UranaiProjection {
    shen_name character;
    Vector3 position;
    float yaw, scale;
};
static_assert(sizeof(UranaiProjection) == 24 && offsetof(UranaiProjection, scale) == 20);
inline constexpr UranaiProjection URNI_SRI_PROJECTIONS[]{
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8899999856948853f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 0.4000000059604645f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8899999856948853f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 0.699999988079071f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 2.0f},
    {.character = URNI_DOVE, .position = {0.02500000037252903f, 0.8899999856948853f, -0.8299999833106995f}, .yaw = 30.0f, .scale = 0.11999999731779099f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8899999856948853f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 0.23000000417232513f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8899999856948853f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 0.23000000417232513f},
    {.character = URNI_DOVE, .position = {0.02500000037252903f, 0.8899999856948853f, -0.8299999833106995f}, .yaw = 30.0f, .scale = 0.11999999731779099f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.600000023841858f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.399999976158142f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.399999976158142f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.600000023841858f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.100000023841858f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.399999976158142f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.600000023841858f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.2000000476837158f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
    {.character = URNI_CRYSTAL, .position = {0.02500000037252903f, 0.8199999928474426f, -0.8299999833106995f}, .yaw = 0.0f, .scale = 1.5f},
};
inline constexpr const char *URNI_SRI_INTRODUCTION_VOICES[]{"A087", "H013", "H001", nullptr};
inline constexpr const char *URNI_SRI_SAME_VOICES[]{"H009", "A062", "H010", "H011", "A010", nullptr};
inline constexpr const char *URNI_SRI_DECLINE_VOICES[]{"A133", "H002", "A134", nullptr};
inline constexpr const char *URNI_SRI_BEGIN_VOICES[]{"A002", "H003", nullptr};
inline constexpr const char *URNI_SRI_READING_VOICES[]{"H004", nullptr};
inline constexpr const char *URNI_SRI_PAYMENT_VOICES[]{"H005", "H006", "A007", nullptr};
inline constexpr const char *URNI_SRI_INSUFFICIENT_VOICES[]{"A020", "H012", "A135", nullptr};
inline constexpr const char *URNI_SRI_RESULT_VOICES[28][16]{
    {"H016", "H017", "H018", "A221", nullptr},
    {"H019", "H021", "A234", "H022", "A233", nullptr},
    {"H023", "A235", "H024", "A236", "H025", "H026", "H027", "A233", nullptr},
    {"H023", "H029", "A237", "H030", "H031", "A152", "H032", "H033", "A233", nullptr},
    {"H034", "H035", "A229", "H036", "H037", "A233", nullptr},
    {"H038", "A240", "H039", "A241", "H040", "A221", nullptr},
    {"H041", "A242", "H042", "H043", "A243", nullptr},
    {"H048", "H049", "H050", "A047", "H051", "A233", nullptr},
    {"H044", "A244", "H045", "H046", "H047", "A233", nullptr},
    {"H044", "A244", "H045", "H046", "H047", "A233", nullptr},
    {"H048", "H049", "H050", "A047", "H051", "A233", nullptr},
    {"H052", "H053", "A245", "H054", "H055", "A246", "H056", "A243", nullptr},
    {"H057", "H058", "A227", "H059", "H060", "H061", "A221", nullptr},
    {"H062", "A248", "H063", "A233", nullptr},
    {"H062", "A248", "H063", "A233", nullptr},
    {"H065", "H066", "A251", "H067", "H068", "A250", "H069", "A221", nullptr},
    {"H073", "A251", "H074", "H075", "A243", nullptr},
    {"H070", "H085", "A255", "H086", "H087", "H088", "A221", nullptr},
    {"H070", "H090", "A256", "H091", "H092", "A233", nullptr},
    {"H070", "H093", "A257", "H094", "H095", "A233", nullptr},
    {"H070", "H096", "A258", "H097", "H098", "A233", nullptr},
    {"H070", "H085", "A255", "H086", "H087", "H088", "A221", nullptr},
    {"H099", "H100", "H101", "H102", "H103", "H104", "H105", "H106", "H108", "H109", "A259", nullptr},
    {"H076", "A252", "H079", "H081", "A254", "H080", "A243", nullptr},
    {"H070", "A249", "H079", "A253", "H080", "A243", nullptr},
    {"H070", "A249", "H082", "H083", "A243", nullptr},
    {"H076", "A252", "H077", "H078", "A243", nullptr},
    {"H070", "A249", "H071", "H072", "A243", nullptr},
};
inline const char *urniSriModeOneVoices[1]{};
inline constexpr std::uint32_t URNI_SRI_TALK_TWO_MOTION = 6649, URNI_SRI_END_MOTION = 6646;
inline constexpr std::uint32_t URNI_SRI_START_MOTION = 6648, URNI_SRI_LOOP_MOTION = 6647, URNI_SRI_TALK_MOTION = 6650;
inline constexpr Vector3 URNI_RCD_CUP{-0.20000000298023224f, 0.75f, -0.6499999761581421f};
inline constexpr Vector3 URNI_RCD_IDLE[]{
    {-0.20000000298023224f, 0.8999999761581421f, -0.6700000166893005f},
    {-0.2199999988079071f, 0.8999999761581421f, -0.6800000071525574f},
    {-0.1899999976158142f, 0.8999999761581421f, -0.6700000166893005f},
    {-0.20999999344348907f, 0.8999999761581421f, -0.6600000262260437f},
    {-0.20000000298023224f, 0.8999999761581421f, -0.6499999761581421f},
};
inline constexpr std::int32_t URNI_RCD_FLAGS[]{
    1500, 1503, 1504, 1505, 1506, 1507, 1508, 1509, 1510, 1511, 1512, 1513, 1514, 1515, 1516, 1517, 1518, 1519, 1520, 1522, 1523};
inline constexpr std::uint32_t URNI_RCD_IDLE_MOTION = 6611, URNI_RCD_DECLINE_MOTION = 6612, URNI_RCD_RESULT_MOTION = 6610;
inline constexpr const char *URNI_RCD_MODE_ONE_VOICES[]{"J001", "A138", "J002", "A087", nullptr};
inline constexpr const char *URNI_RCD_INTRODUCTION_VOICES[]{"J003", "A139", "J004", "J005", "A140", "J006", nullptr};
inline constexpr const char *URNI_RCD_SAME_VOICES[]{"A001", "J013", "J014", "J015", "A010", nullptr};
inline constexpr const char *URNI_RCD_DECLINE_VOICES[]{"A003", "J007", "A015", nullptr};
inline constexpr const char *URNI_RCD_BEGIN_VOICES[]{"A002", "J008", nullptr};
inline constexpr const char *URNI_RCD_READING_VOICES[]{"J009", "J010", "A035", nullptr};
inline constexpr const char *URNI_RCD_PAYMENT_VOICES[]{"J011", "A018", "J012", "A019", "A007", nullptr};
inline constexpr const char *URNI_RCD_INSUFFICIENT_VOICES[]{"A020", "J016", "A021", "J017", "A022", "J018", "A010", nullptr};
inline constexpr const char *URNI_RCD_RESULT_VOICES[21][32]{
    {"J112", "J113", "J114", "A284", "J115", "A023", nullptr},
    {"J112", "J120", "J121", "J122", "J123", "J124", "A023", nullptr},
    {"J022", "A262", "J023", "J024", "A010", "J025", "J026", "A004", nullptr},
    {"J027", "J028", "A140", "J029", "J030", "J031", "A263", nullptr},
    {"J032", "A140", "J033", "A264", "J034", "J035", "J036", "A170", "J037", "A265", nullptr},
    {"J032", "A140", "J033", "A264", "J034", "J035", "J038", "A170", "J041", "A265", nullptr},
    {"J043", "J044", "J045", "J046", "J047", "A269", "J048", "A004", nullptr},
    {"J049", "J050", "J051", "J052", "J053", "A010", nullptr},
    {"J054", "J055", "J056", "A270", "J057", "J058", "A271", nullptr},
    {"J039", "A266", "J040", "A267", "J035", "J038", "A170", "J041", "A268", "J042", "A010", nullptr},
    {"J059", "J060", "J061", "A272", "J062", "A273", "J063", "A010", nullptr},
    {"J059", "J064", "J065", "J066", "A010", nullptr},
    {"J112", "J116", "J117", "J118", "J119", "A023", nullptr},
    {"J112", "J120", "J121", "J122", "J123", "J124", "A023", nullptr},
    {"J067", "J068", "J069", "A274", "J070", "J071", "J072", nullptr},
    {"J073", "J074", "A170", "J075", "A010", "J076", "J077", "A275", "J078", "A276", nullptr},
    {"J079", "J080", "J081", "A277", "J082", "A023", nullptr},
    {"J083", "A278", "J084", "A279", "J085", "A280", "J086", "J087", "A010", nullptr},
    {"J088", "A281", "J089", "J090", "J091", "J092", "A010", nullptr},
    {"J093", "J094", "J095", "A004", "J096", "A282", "J097", "J098", "J099", "A010", nullptr},
    {"J100", "J101", "A283", "J102", "J103", "J104", "A140", "J105", "A123", "J106", "J107", "J108", "A010", "J109", "J110", "J111", "A004", nullptr},
};
std::int64_t UranaiSykSetup(shen_name, std::int32_t);
std::int64_t UranaiSykModeOne(shen_name, std::int32_t);
std::int64_t UranaiSykIntroduction(shen_name, std::int32_t);
std::int64_t UranaiSykSame(shen_name, std::int32_t);
std::int64_t UranaiSykDecline(shen_name, std::int32_t);
std::int64_t UranaiSykBeginReading(shen_name, std::int32_t);
std::int64_t UranaiSykReading(shen_name, std::int32_t);
std::int64_t UranaiSykResult(shen_name, std::int32_t);
std::int64_t UranaiSykPayment(shen_name, std::int32_t);
std::int64_t UranaiSykInsufficientCash(shen_name, std::int32_t);
inline constexpr UranaiConfiguration URNI_SYK_CONFIGURATION{.position = {246.39999389648438f, 104.5f, 167.25999450683594f},
    .yaw = 0,
    .price = 10,
    .reserved14 = 0,
    .setup = UranaiSykSetup,
    .modeOne = UranaiSykModeOne,
    .introduction = UranaiSykIntroduction,
    .same = UranaiSykSame,
    .decline = UranaiSykDecline,
    .beginReading = UranaiSykBeginReading,
    .reading = UranaiSykReading,
    .result = UranaiSykResult,
    .payment = UranaiSykPayment,
    .insufficientCash = UranaiSykInsufficientCash};
std::int64_t UranaiKlnSetup(shen_name, std::int32_t);
std::int64_t UranaiKlnModeOne(shen_name, std::int32_t);
std::int64_t UranaiKlnIntroduction(shen_name, std::int32_t);
std::int64_t UranaiKlnSame(shen_name, std::int32_t);
std::int64_t UranaiKlnDecline(shen_name, std::int32_t);
std::int64_t UranaiKlnBeginReading(shen_name, std::int32_t);
std::int64_t UranaiKlnReading(shen_name, std::int32_t);
std::int64_t UranaiKlnResult(shen_name, std::int32_t);
std::int64_t UranaiKlnPayment(shen_name, std::int32_t);
std::int64_t UranaiKlnInsufficientCash(shen_name, std::int32_t);
std::int64_t UranaiMm5Setup(shen_name, std::int32_t);
std::int64_t UranaiMm5ModeOne(shen_name, std::int32_t);
std::int64_t UranaiMm5Introduction(shen_name, std::int32_t);
std::int64_t UranaiMm5Same(shen_name, std::int32_t);
std::int64_t UranaiMm5Decline(shen_name, std::int32_t);
std::int64_t UranaiMm5BeginReading(shen_name, std::int32_t);
std::int64_t UranaiMm5Reading(shen_name, std::int32_t);
std::int64_t UranaiMm5Result(shen_name, std::int32_t);
std::int64_t UranaiMm5Payment(shen_name, std::int32_t);
std::int64_t UranaiMm5InsufficientCash(shen_name, std::int32_t);
std::int64_t UranaiSehSetup(shen_name, std::int32_t);
std::int64_t UranaiSehModeOne(shen_name, std::int32_t);
std::int64_t UranaiSehIntroduction(shen_name, std::int32_t);
std::int64_t UranaiSehSame(shen_name, std::int32_t);
std::int64_t UranaiSehDecline(shen_name, std::int32_t);
std::int64_t UranaiSehBeginReading(shen_name, std::int32_t);
std::int64_t UranaiSehReading(shen_name, std::int32_t);
std::int64_t UranaiSehResult(shen_name, std::int32_t);
std::int64_t UranaiSehPayment(shen_name, std::int32_t);
std::int64_t UranaiSehInsufficientCash(shen_name, std::int32_t);
std::int64_t UranaiSriSetup(shen_name, std::int32_t);
std::int64_t UranaiSriModeOne(shen_name, std::int32_t);
std::int64_t UranaiSriIntroduction(shen_name, std::int32_t);
std::int64_t UranaiSriSame(shen_name, std::int32_t);
std::int64_t UranaiSriDecline(shen_name, std::int32_t);
std::int64_t UranaiSriBeginReading(shen_name, std::int32_t);
std::int64_t UranaiSriReading(shen_name, std::int32_t);
std::int64_t UranaiSriResult(shen_name, std::int32_t);
std::int64_t UranaiSriPayment(shen_name, std::int32_t);
std::int64_t UranaiSriInsufficientCash(shen_name, std::int32_t);
std::int64_t UranaiRcdSetup(shen_name, std::int32_t);
std::int64_t UranaiRcdModeOne(shen_name, std::int32_t);
std::int64_t UranaiRcdIntroduction(shen_name, std::int32_t);
std::int64_t UranaiRcdSame(shen_name, std::int32_t);
std::int64_t UranaiRcdDecline(shen_name, std::int32_t);
std::int64_t UranaiRcdBeginReading(shen_name, std::int32_t);
std::int64_t UranaiRcdReading(shen_name, std::int32_t);
std::int64_t UranaiRcdResult(shen_name, std::int32_t);
std::int64_t UranaiRcdPayment(shen_name, std::int32_t);
std::int64_t UranaiRcdInsufficientCash(shen_name, std::int32_t);
inline constexpr UranaiConfiguration URNI_KLN_CONFIGURATION{.position = {277.7099914550781f, 100, 135.83999633789062f},
    .yaw = -90,
    .price = 20,
    .reserved14 = 0,
    .setup = UranaiKlnSetup,
    .modeOne = UranaiKlnModeOne,
    .introduction = UranaiKlnIntroduction,
    .same = UranaiKlnSame,
    .decline = UranaiKlnDecline,
    .beginReading = UranaiKlnBeginReading,
    .reading = UranaiKlnReading,
    .result = UranaiKlnResult,
    .payment = UranaiKlnPayment,
    .insufficientCash = UranaiKlnInsufficientCash};
inline constexpr UranaiConfiguration URNI_MM5_AR01_CONFIGURATION{.position = {177.89999389648438f, 100.3499984741211f, 314.20001220703125f},
    .yaw = 180,
    .price = 20,
    .reserved14 = 0,
    .setup = UranaiMm5Setup,
    .modeOne = UranaiMm5ModeOne,
    .introduction = UranaiMm5Introduction,
    .same = UranaiMm5Same,
    .decline = UranaiMm5Decline,
    .beginReading = UranaiMm5BeginReading,
    .reading = UranaiMm5Reading,
    .result = UranaiMm5Result,
    .payment = UranaiMm5Payment,
    .insufficientCash = UranaiMm5InsufficientCash};
inline constexpr UranaiConfiguration URNI_MM5_WS00_CONFIGURATION{.position = {71.86000061035156f, 98.19999694824219f, 118.94000244140625f},
    .yaw = 180,
    .price = 20,
    .reserved14 = 0,
    .setup = UranaiMm5Setup,
    .modeOne = UranaiMm5ModeOne,
    .introduction = UranaiMm5Introduction,
    .same = UranaiMm5Same,
    .decline = UranaiMm5Decline,
    .beginReading = UranaiMm5BeginReading,
    .reading = UranaiMm5Reading,
    .result = UranaiMm5Result,
    .payment = UranaiMm5Payment,
    .insufficientCash = UranaiMm5InsufficientCash};
inline constexpr UranaiConfiguration URNI_MM5_OTHER_CONFIGURATION{.position = {293.3500061035156f, 100, 153.72999572753906f},
    .yaw = 90,
    .price = 20,
    .reserved14 = 0,
    .setup = UranaiMm5Setup,
    .modeOne = UranaiMm5ModeOne,
    .introduction = UranaiMm5Introduction,
    .same = UranaiMm5Same,
    .decline = UranaiMm5Decline,
    .beginReading = UranaiMm5BeginReading,
    .reading = UranaiMm5Reading,
    .result = UranaiMm5Result,
    .payment = UranaiMm5Payment,
    .insufficientCash = UranaiMm5InsufficientCash};
inline constexpr UranaiConfiguration URNI_SEH_CONFIGURATION{.position = {291.7300109863281f, 100, 164.89999389648438f},
    .yaw = 0,
    .price = 20,
    .reserved14 = 0,
    .setup = UranaiSehSetup,
    .modeOne = UranaiSehModeOne,
    .introduction = UranaiSehIntroduction,
    .same = UranaiSehSame,
    .decline = UranaiSehDecline,
    .beginReading = UranaiSehBeginReading,
    .reading = UranaiSehReading,
    .result = UranaiSehResult,
    .payment = UranaiSehPayment,
    .insufficientCash = UranaiSehInsufficientCash};
inline constexpr UranaiConfiguration URNI_SRI_CONFIGURATION{.position = {122.2300033569336f, 93.0199966430664f, 165.4199981689453f},
    .yaw = 100,
    .price = 20,
    .reserved14 = 0,
    .setup = UranaiSriSetup,
    .modeOne = UranaiSriModeOne,
    .introduction = UranaiSriIntroduction,
    .same = UranaiSriSame,
    .decline = UranaiSriDecline,
    .beginReading = UranaiSriBeginReading,
    .reading = UranaiSriReading,
    .result = UranaiSriResult,
    .payment = UranaiSriPayment,
    .insufficientCash = UranaiSriInsufficientCash};
inline constexpr UranaiConfiguration URNI_RCD_CONFIGURATION{.position = {332.6700134277344f, 126.98999786376953f, 290.32000732421875f},
    .yaw = 0,
    .price = 20,
    .reserved14 = 0,
    .setup = UranaiRcdSetup,
    .modeOne = UranaiRcdModeOne,
    .introduction = UranaiRcdIntroduction,
    .same = UranaiRcdSame,
    .decline = UranaiRcdDecline,
    .beginReading = UranaiRcdBeginReading,
    .reading = UranaiRcdReading,
    .result = UranaiRcdResult,
    .payment = UranaiRcdPayment,
    .insufficientCash = UranaiRcdInsufficientCash};
inline constexpr const char *URNI_SRI_PACKAGES[]{"0120_YAN", "0120_CHD", "0120_TEJ", "0120_YAN", "0120_QRR", "0120_GAI", "0120_KUK", "0120_HAT", "0120_QCN",
    "0120_QCN", "0120_HAT", "0120_SGT", "0120_KT1", "0120_KYC", "0120_KYC", "0120_GAI", "0120_SSY", "0120_CHN", "0120_AW3", "0120_SF6", "0120_SF3", "0120_CHN",
    "0120_TGY", "0120_AW1", "0120_VNE", "0120_SF5", "0120_KFT", "0120_OXO"};
inline constexpr std::int32_t URNI_SRI_STORY_FLAGS[]{1524, 1525, 1527, 1528, 1529, 1530, 1531, 1532, 1533, 1534, 1535, 1536, 1537, 1538, 1539, 1540, 1541, 1542,
    1543, 1544, 1545, 1546, 1547, 1548, 1549, 1550, 1551, 1552};
std::int32_t UranaiStoryPackageIndex();
extern const ScriptAreaDefinition urniDefinition;
} // namespace shenmue::s2
