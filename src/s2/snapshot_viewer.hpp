#pragma once

#include "shenmue.h"
#include "s2/hlib.hpp"
#include "s2/ui.hpp"
#include "s2/area_event.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name SNAPSHOT_DEBUG_TASK{SHEN_NAME(STVW)}, SNAPSHOT_DEBUG_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name SNAPSHOT_BIO_SYP{SHEN_NAME(SYP_)}, SNAPSHOT_BIO_SYX{SHEN_NAME(SYX_)}, SNAPSHOT_BIO_SKY{SHEN_NAME(SKY_)};
inline constexpr shen_name SNAPSHOT_BIO_WSH{SHEN_NAME(WSH_)}, SNAPSHOT_BIO_SN5{SHEN_NAME(SN5_)}, SNAPSHOT_BIO_SNB{SHEN_NAME(SNB_)};
inline constexpr shen_name SNAPSHOT_BIO_GAI2{SHEN_NAME(GAI2)};
void InitializeSnapshotViewer();
void RunSnapshotViewer(std::uintptr_t argument);
std::int64_t CloseSnapshotViewer(std::int32_t mode);
inline constexpr ScriptAreaDefinition snapshotViewerScriptDefinition{
    .areaId = SNAPSHOT_DEBUG_TASK, .field04 = 0, .init = InitializeSnapshotViewer, .main = RunSnapshotViewer};
std::int32_t GetSnapshotBioSortKey(std::uint32_t name) noexcept;
struct SnapshotMetadataRecord;
void UpdateSnapshotAreaSortKey(SnapshotMetadataRecord &record) noexcept;
inline constexpr shen_name SNAPSHOT_EMPTY_AREA{SHEN_NAME(0000)};
inline constexpr shen_name SNAPSHOT_AREA_ABERDEEN{SHEN_NAME(A000)}, SNAPSHOT_AREA_GUILIN{SHEN_NAME(K000)};
inline constexpr shen_name SNAPSHOT_AREA_KOWLOON{SHEN_NAME(Q000)}, SNAPSHOT_AREA_WAN_CHAI{SHEN_NAME(W000)};
#pragma pack(push, 1)
struct SnapshotDebugWork {
    std::int32_t mode{};
    std::uint32_t reserved04{};
    HLib::Task *waitingTask{};
    std::int32_t drawnPhase{}, phase{}, returnPhase{-1}, count{}, answer{};
    std::uint8_t previousHelp{};
    std::array<std::byte, 3> reserved25{};
    std::int32_t suppressSelection{};
    std::int32_t suppressContent{}, reserved30{}, loadingPhase{}, reserved38{}, frames{}, waitFrames{};
    std::int32_t selectionKind{}, page{}, selected{}, sort{}, first{}, visible{}, column{}, row{};
    std::int32_t drawnSelectionKind{}, drawnPage{}, drawnSelected{}, drawnSort{}, drawnFirst{}, drawnVisible{}, drawnColumn{}, drawnRow{};
    float depth{};
    std::int32_t reserved88{-1};
    std::int32_t selectedBiography{}, biographyPhoto{};
    std::array<std::array<std::int32_t, 2>, 48> biographyPictures{};
    std::int32_t panX{}, panY{}, zoomWidth{}, zoomHeight{}, hideHud{}, fullscreen{};
    std::uint8_t previousClockSuppression{};
    std::array<std::byte, 3> reserved22d{};
    std::uint32_t previousNpcLoad{}, reserved234{};
};
#pragma pack(pop)
static_assert(sizeof(SnapshotDebugWork) == 0x238);
static_assert(offsetof(SnapshotDebugWork, drawnPhase) == 0x10);
static_assert(offsetof(SnapshotDebugWork, count) == 0x1c);
static_assert(offsetof(SnapshotDebugWork, suppressSelection) == 0x28);
static_assert(offsetof(SnapshotDebugWork, selectionKind) == 0x44);
static_assert(offsetof(SnapshotDebugWork, depth) == 0x84);
static_assert(offsetof(SnapshotDebugWork, selectedBiography) == 0x8c);
static_assert(offsetof(SnapshotDebugWork, biographyPictures) == 0x94);
static_assert(offsetof(SnapshotDebugWork, panX) == 0x214);
static_assert(offsetof(SnapshotDebugWork, fullscreen) == 0x228);
static_assert(offsetof(SnapshotDebugWork, previousClockSuppression) == 0x22c);
static_assert(offsetof(SnapshotDebugWork, previousNpcLoad) == 0x230);

struct SnapshotDebugRect {
    std::int32_t x{}, y{}, width{}, height{};
};
static_assert(sizeof(SnapshotDebugRect) == 16);
inline constexpr std::array<float, 8> SNAPSHOT_FULL_IMAGE_UV{0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f};
inline constexpr std::array<const char *, 12> SNAPSHOT_MONTH_NAMES{
    "Jan.", "Feb.", "Mar.", "Apr.", "May", "Jun.", "Jul.", "Aug.", "Sep.", "Oct.", "Nov.", "Dec."};
inline constexpr std::array<const char*,4> SNAPSHOT_REGION_LABELS{"UIMAREABER","UIMAREWACH","UIMAREKOWL","UIMAREGUIL"}; // @text UIMAREABER EN: "Aberdeen" | JA: "香港仔" ; UIMAREWACH EN: "Wan Chai" | JA: "湾仔" ; UIMAREKOWL EN: "Kowloon" | JA: "九龍城" ; UIMAREGUIL EN: "Guilin" | JA: "桂林"
inline constexpr std::array<std::array<std::uint16_t, 4>, 2> SNAPSHOT_ARROW_UV{{{683, 457, 722, 593}, {722, 457, 761, 593}}};
inline constexpr std::array<std::array<std::uint16_t, 4>, 2> SNAPSHOT_TAB_UV{{{0, 398, 625, 500}, {0, 296, 625, 398}}};
inline constexpr std::array<LegacyIconSprite, 9> SNAPSHOT_DEBUG_ICONS{
    {{.x = 148, .y = 438, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 773, .width = 32, .height = 32, .alignment = 9},
        {.x = 148, .y = 438, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 772, .width = 32, .height = 32, .alignment = 9},
        {.x = 572, .y = 272, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 773, .width = 32, .height = 32, .alignment = 9},
        {.x = 68, .y = 272, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 772, .width = 32, .height = 32, .alignment = 9},
        {.x = 486, .y = 71, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 768, .width = 32, .height = 32, .alignment = 9},
        {.x = 534, .y = 71, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 776, .width = 32, .height = 32, .alignment = 9},
        {.x = 582, .y = 71, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 777, .width = 32, .height = 32, .alignment = 9},
        {.x = 108, .y = 71, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 778, .width = 32, .height = 32, .alignment = 9},
        {.x = 156, .y = 71, .scaleX = 1, .scaleY = 1, .depth = -2.0899999f, .icon = 779, .width = 32, .height = 32, .alignment = 9}}};
inline constexpr shen_name SNAPSHOT_TEXT_TEXTURE{SHEN_NAME(SHWV)}, SNAPSHOT_TEXT_PTX{SHEN_NAME(PTX0)};
inline constexpr std::size_t SNAPSHOT_TEXT_STRIDE = 41008;

struct SnapshotDebugTextBank {
    std::array<std::byte, 14 * SNAPSHOT_TEXT_STRIDE> pixels{};
    std::array<std::int32_t, 14> textures{};
    std::int32_t count{}, capacity{};
};
struct SnapshotDebugTextResources {
    SnapshotDebugTextBank text{}, secret{};
};
static_assert(offsetof(SnapshotDebugTextBank, textures) == 574112);
static_assert(offsetof(SnapshotDebugTextBank, count) == 574168);
static_assert(offsetof(SnapshotDebugTextBank, capacity) == 574172);
static_assert(sizeof(SnapshotDebugTextBank) == 574176);
static_assert(offsetof(SnapshotDebugTextResources, secret) == 574176);
static_assert(sizeof(SnapshotDebugTextResources) == 1148352);
struct SnapshotDebugSprite {
    std::int32_t x, y;
    std::uint16_t width, height;
    std::array<float, 4> uv;
    std::uint8_t alignment;
    std::array<std::byte, 3> reserved1d;
    std::uint32_t texture;
};
static_assert(sizeof(SnapshotDebugSprite) == 36);
static_assert(offsetof(SnapshotDebugSprite, uv) == 12);
static_assert(offsetof(SnapshotDebugSprite, alignment) == 28);
static_assert(offsetof(SnapshotDebugSprite, texture) == 32);
inline constexpr std::array<SnapshotDebugSprite, 28> SNAPSHOT_DEBUG_SPRITES{
    {{.x = 543, .y = 440, .width = 176, .height = 40, .uv = {0.0f, 0.6875f, 0.6875f, 1.0f}, .alignment = 5, .texture = 0},
        {.x = 536, .y = 435, .width = 112, .height = 30, .uv = {0.0f, 0.4531f, 0.4375f, 0.6875f}, .alignment = 5, .texture = 0},
        {.x = 536, .y = 436, .width = 112, .height = 30, .uv = {0.0f, 0.2187f, 0.4375f, 0.4531f}, .alignment = 5, .texture = 0},
        {.x = 536, .y = 435, .width = 112, .height = 30, .uv = {0.4375f, 0.4531f, 0.875f, 0.6875f}, .alignment = 5, .texture = 0},
        {.x = 536, .y = 436, .width = 112, .height = 30, .uv = {0.4375f, 0.2187f, 0.875f, 0.4531f}, .alignment = 5, .texture = 0},
        {.x = 320, .y = 320, .width = 256, .height = 28, .uv = {0.0f, 0.0f, 1.0f, 0.2187f}, .alignment = 5, .texture = 0},
        {.x = 320, .y = 240, .width = 256, .height = 28, .uv = {0.0f, 0.0f, 0.935f, 0.2187f}, .alignment = 5, .texture = 0},
        {.x = 474, .y = 370, .width = 70, .height = 80, .uv = {0.0f, 0.6875f, 0.2734f, 1.0f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 70, .height = 80, .uv = {0.2734f, 0.6875f, 0.5469f, 1.0f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 70, .height = 80, .uv = {0.5469f, 0.6875f, 0.8203f, 1.0f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 70, .height = 80, .uv = {0.0f, 0.375f, 0.2734f, 0.6875f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 70, .height = 80, .uv = {0.2734f, 0.375f, 0.5469f, 0.6875f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 70, .height = 80, .uv = {0.5469f, 0.375f, 0.8203f, 0.6875f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 32, .height = 32, .uv = {0.875f, 0.875f, 1.0f, 1.0f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 32, .height = 32, .uv = {0.875f, 0.75f, 1.0f, 0.875f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 32, .height = 32, .uv = {0.875f, 0.625f, 1.0f, 0.75f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 32, .height = 32, .uv = {0.875f, 0.375f, 1.0f, 0.5f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 32, .height = 32, .uv = {0.875f, 0.5f, 1.0f, 0.625f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 32, .height = 32, .uv = {0.875f, 0.25f, 1.0f, 0.375f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 70, .height = 80, .uv = {0.0f, 0.0625f, 0.2734f, 0.375f}, .alignment = 5, .texture = 1},
        {.x = 474, .y = 370, .width = 32, .height = 32, .uv = {0.875f, 0.125f, 1.0f, 0.25f}, .alignment = 5, .texture = 1},
        {.x = 480, .y = 376, .width = 32, .height = 32, .uv = {0.875f, 0.0f, 1.0f, 0.125f}, .alignment = 5, .texture = 1},
        {.x = 475, .y = 370, .width = 70, .height = 80, .uv = {0.2734f, 0.0625f, 0.5469f, 0.375f}, .alignment = 5, .texture = 1},
        {.x = 320, .y = 240, .width = 512, .height = 512, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .alignment = 5, .texture = 2},
        {.x = 320, .y = 240, .width = 512, .height = 512, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .alignment = 5, .texture = 3},
        {.x = 320, .y = 240, .width = 512, .height = 512, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .alignment = 5, .texture = 4},
        {.x = 320, .y = 240, .width = 512, .height = 512, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .alignment = 5, .texture = 5},
        {.x = 320, .y = 240, .width = 512, .height = 512, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .alignment = 5, .texture = 6}}};

struct SnapshotMetadataOwner;
struct SnapshotViewerStorage {
    std::array<std::byte, 126 * 36> images{};
    SnapshotMetadataOwner *metadata{};
    std::array<SnapshotMetadataRecord *, 126> order{};
};
static_assert(sizeof(SnapshotViewerStorage) == 0x15b0);
static_assert(offsetof(SnapshotViewerStorage, metadata) == 0x11b8);
static_assert(offsetof(SnapshotViewerStorage, order) == 0x11c0);
struct SnapshotBiography {
    shen_name character;
    std::int32_t index;
    const char *name;
    const char *age;
    const char *birthDate;
    const char *starSign;
    const char *bloodType;
    const char *description;
};
static_assert(sizeof(SnapshotBiography) == 56);
static_assert(offsetof(SnapshotBiography, name) == 8);
static_assert(offsetof(SnapshotBiography, description) == 48);

inline constexpr std::array<SnapshotBiography, 48> SNAPSHOT_BIOGRAPHIES{{
    {.character = SHEN_NAME(JOP_),
        .index = 0,
        .name = "Joy",
        .age = "18",
        .birthDate = "8/16/1968",
        .starSign = "Leo",
        .bloodType = "B",
        .description = "She is a very spirited woman who speaks\nher mind."},
    {.character = SHEN_NAME(WON_),
        .index = 1,
        .name = "Wong",
        .age = "13",
        .birthDate = "8/21/1973",
        .starSign = "Leo",
        .bloodType = "B",
        .description = "He is the young member of the Heavens."},
    {.character = SHEN_NAME(SAM_),
        .index = 2,
        .name = "Sam",
        .age = "15",
        .birthDate = "4/25/1971",
        .starSign = "Taurus",
        .bloodType = "O",
        .description = "He is the member of the Heavens."},
    {.character = SHEN_NAME(LLY_),
        .index = 3,
        .name = "Larry",
        .age = "16",
        .birthDate = "6/10/1970",
        .starSign = "Gemini",
        .bloodType = "A",
        .description = "He is the member of the Heavens."},
    {.character = SHEN_NAME(CLJ_),
        .index = 4,
        .name = "Cool Z",
        .age = "18",
        .birthDate = "11/15/1968",
        .starSign = "Scorpio",
        .bloodType = "AB",
        .description = "He is the member of the Heavens."},
    {.character = SHEN_NAME(MM5_),
        .index = 5,
        .name = "Ailian He",
        .age = "21",
        .birthDate = "2/7/1966",
        .starSign = "Aquarius",
        .bloodType = "AB",
        .description = "She is a popular fortune teller\nin Hong Kong."},
    {.character = SHEN_NAME(GOI_),
        .index = 6,
        .name = "Haohai Du",
        .age = "33",
        .birthDate = "8/14/1953",
        .starSign = "Leo",
        .bloodType = "AB",
        .description = "He is the gangster in Aberdeen.\n(Poison Bros.)"},
    {.character = SHEN_NAME(HTK_),
        .index = 7,
        .name = "Bangzhuo Du",
        .age = "30",
        .birthDate = "10/30/1956",
        .starSign = "Scorpio",
        .bloodType = "O",
        .description = "He is the gangster in Aberdeen.\n(Poison Bros.)"},
    {.character = SHEN_NAME(SYE_),
        .index = 8,
        .name = "Xiuying Hong",
        .age = "26",
        .birthDate = "5/24/1960",
        .starSign = "Gemini",
        .bloodType = "A",
        .description = "She is well versed in Taoism, history,\nand martial arts. "},
    {.character = SHEN_NAME(KEM_),
        .index = 9,
        .name = "Jianmin Tao",
        .age = "75",
        .birthDate = "3/22/1911",
        .starSign = "Aries",
        .bloodType = "O",
        .description = "He is the master of Chen style Tai Chi."},
    {.character = SHEN_NAME(BKN_),
        .index = 10,
        .name = "Zhoushan Xuan",
        .age = "68",
        .birthDate = "9/9/1918",
        .starSign = "Virgo",
        .bloodType = "O",
        .description = "He is the master\nof Guang Martial Arts School."},
    {.character = SHEN_NAME(CHY_),
        .index = 11,
        .name = "Zhangyu Kong",
        .age = "50",
        .birthDate = "11/8/1936",
        .starSign = "Scorpio",
        .bloodType = "B",
        .description = "He is the manager\nof Zhangyu Barber's Shop."},
    {.character = SHEN_NAME(KEK_),
        .index = 12,
        .name = "Guixiang Lee",
        .age = "76",
        .birthDate = "1/8/1911",
        .starSign = "Capricorn",
        .bloodType = "B",
        .description = "She is the caretaker\nof the Yan Tin Apartments."},
    {.character = SHEN_NAME(TNZ_),
        .index = 13,
        .name = "Ren Dan",
        .age = "48",
        .birthDate = "4/29/1938",
        .starSign = "Taurus",
        .bloodType = "B",
        .description = "He is the clerk at Come Over Guest House."},
    {.character = SHEN_NAME(SOS_),
        .index = 14,
        .name = "Zongquan Bai",
        .age = "37",
        .birthDate = "11/8/1949",
        .starSign = "Scorpio",
        .bloodType = "O",
        .description = "He is the ex-disciple of Zhoushan."},
    {.character = SHEN_NAME(TSK_),
        .index = 15,
        .name = "May (Meiming Yu)",
        .age = "18",
        .birthDate = "9/23/1967",
        .starSign = "Virgo",
        .bloodType = "O",
        .description = "She runs Fu Hoi Diner well \nwith her mother. "},
    {.character = SHEN_NAME(KUN_),
        .index = 16,
        .name = "Fangmei Xun",
        .age = "14",
        .birthDate = "3/3/1972",
        .starSign = "Pisces",
        .bloodType = "B",
        .description = "She is cheerful\nand likes to help everyone. "},
    {.character = SHEN_NAME(SAI_),
        .index = 17,
        .name = "Jiliang Chang",
        .age = "16",
        .birthDate = "6/18/1970",
        .starSign = "Gemini",
        .bloodType = "B",
        .description = "He is the apprentice at Man Mo Temple."},
    {.character = SHEN_NAME(SIZ_),
        .index = 18,
        .name = "Shiquan Fu",
        .age = "16",
        .birthDate = "7/1/1970",
        .starSign = "Cancer",
        .bloodType = "A",
        .description = "He is the apprentice at Man Mo Temple."},
    {.character = SHEN_NAME(KNK_),
        .index = 19,
        .name = "Hanhui Liu",
        .age = "43",
        .birthDate = "9/30/1943",
        .starSign = "Libra",
        .bloodType = "AB",
        .description = "He is the monk at Man Mo Temple."},
    {.character = SHEN_NAME(ANA_),
        .index = 20,
        .name = "Anan Liu",
        .age = "23",
        .birthDate = "1/30/1964",
        .starSign = "Aquarius",
        .bloodType = "O",
        .description = "He is the employee of the sign maker."},
    {.character = SHEN_NAME(TKR_),
        .index = 21,
        .name = "Delin Hong",
        .age = "35",
        .birthDate = "6/29/1951",
        .starSign = "Cancer",
        .bloodType = "A",
        .description = "He is Ryo's friend on a part-time job."},
    {.character = SHEN_NAME(MYS_),
        .index = 22,
        .name = "Mingzhen Ye",
        .age = "21",
        .birthDate = "3/13/1965",
        .starSign = "Pisces",
        .bloodType = "O",
        .description = "He is the assistant instructor\nat Guang Martial Arts School."},
    {.character = SHEN_NAME(LKN_),
        .index = 23,
        .name = "Linjian Chuan",
        .age = "19",
        .birthDate = "9/16/1967",
        .starSign = "Virgo",
        .bloodType = "B",
        .description = "He is the assistant instructor\nat Guang Martial Arts School."},
    {.character = SHEN_NAME(EDL_),
        .index = 24,
        .name = "Eileen M Edelweiss",
        .age = "17",
        .birthDate = "7/29/1969",
        .starSign = "Leo",
        .bloodType = "O",
        .description = "She is a Canadian student\nlearning kung fu. "},
    {.character = SHEN_NAME(XGH_),
        .index = 25,
        .name = "Izumi Takano",
        .age = "18",
        .birthDate = "10/4/1968",
        .starSign = "Libra",
        .bloodType = "AB",
        .description = "She is a Japanese student working\nat Tomato Convenience Store."},
    {.character = SHEN_NAME(CKT_),
        .index = 26,
        .name = "Gongjue Zhou",
        .age = "40",
        .birthDate = "4/13/1946",
        .starSign = "Aries",
        .bloodType = "O",
        .description = "He devotes himself to the Capsule Toys."},
    {.character = SHEN_NAME(YAN_),
        .index = 27,
        .name = "Yang",
        .age = "55",
        .birthDate = "6/15/1931",
        .starSign = "Gemini",
        .bloodType = "A",
        .description = "He is the owner of Great View Herbs."},
    {.character = SHEN_NAME(GAI_),
        .index = 28,
        .name = "Kai",
        .age = "57",
        .birthDate = "3/14/1929",
        .starSign = "Pisces",
        .bloodType = "B",
        .description = "He plays the Chinese fiddle on Dragon St."},
    {.character = SHEN_NAME(CHO_),
        .index = 29,
        .name = "Shuqin Zhang",
        .age = "44",
        .birthDate = "11/8/1942",
        .starSign = "Scorpio",
        .bloodType = "A",
        .description = "He is the aide of Yuanda Zhu."},
    {.character = SHEN_NAME(SF7_),
        .index = 30,
        .name = "Haoyi Ping",
        .age = "?",
        .birthDate = "2/28/????",
        .starSign = "Pisces",
        .bloodType = "O",
        .description = "He is a bookie of Lucky Hit Museum."},
    {.character = SHEN_NAME(CEN_),
        .index = 31,
        .name = "Guangyan Wang",
        .age = "19",
        .birthDate = "7/8/1967",
        .starSign = "Cancer",
        .bloodType = "O",
        .description = "He is an older brother of Guangji who\nworks for Ajiichi in Yokosuka."},
    {.character = SHEN_NAME(REN_),
        .index = 32,
        .name = "Wuying Ren",
        .age = "19",
        .birthDate = "12/4/1967",
        .starSign = "Sagittarius",
        .bloodType = "AB",
        .description = "He is the leader of the Heavens."},
    {.character = SHEN_NAME(AM1_),
        .index = 33,
        .name = "Pengcui Yamei",
        .age = "18",
        .birthDate = "2/14/1969",
        .starSign = "Aquarius",
        .bloodType = "B",
        .description = "She is a waitress at Cafe Anna."},
    {.character = SHEN_NAME(AM2_),
        .index = 34,
        .name = "Loufei Zong",
        .age = "18",
        .birthDate = "12/24/1968",
        .starSign = "Capricorn",
        .bloodType = "O",
        .description = "She is a waitress at Cafe Anna."},
    {.character = SHEN_NAME(AW3_),
        .index = 35,
        .name = "Rod Stunt",
        .age = "37",
        .birthDate = "12/12/1949",
        .starSign = "Sagittarius",
        .bloodType = "O",
        .description = "He is a Street Fighter\nin the Phoenix Bldg."},
    {.character = SHEN_NAME(SF6_),
        .index = 36,
        .name = "Greg More",
        .age = "36",
        .birthDate = "6/1/1950",
        .starSign = "Gemini",
        .bloodType = "AB",
        .description = "He is a Street Fighter\nat Blue Dragon Garden."},
    {.character = SHEN_NAME(SF3_),
        .index = 37,
        .name = "Chunyan Xu",
        .age = "21",
        .birthDate = "8/31/1965",
        .starSign = "Virgo",
        .bloodType = "A",
        .description = "She is a Street Fighter\nin the Black Heaven Bldg."},
    {.character = SHEN_NAME(SF4_),
        .index = 38,
        .name = "Baihu",
        .age = "?",
        .birthDate = "10/22/????",
        .starSign = "Libra",
        .bloodType = "A",
        .description = "He is a Street Fighter\nin the Big Ox Bldg."},
    {.character = SHEN_NAME(SD4_),
        .index = 39,
        .name = "Zhenwu Gu",
        .age = "30",
        .birthDate = "7/27/1956",
        .starSign = "Leo",
        .bloodType = "O",
        .description = "He runs a toy store in the Stand Qr."},
    {.character = SHEN_NAME(SIN_),
        .index = 40,
        .name = "Shenhua Ling",
        .age = "16",
        .birthDate = "9/9/1970",
        .starSign = "Virgo",
        .bloodType = "A",
        .description = "She is a kind-hearted but mysterious girl\nwho lives in the mountains."},
    {.character = SHEN_NAME(SRY_),
        .index = 41,
        .name = "Lan Di",
        .age = "31",
        .birthDate = "8/13/1955",
        .starSign = "Leo",
        .bloodType = "A",
        .description = "He is one of the top executives\nof the Chiyoumen."},
    {.character = SHEN_NAME(TGY_),
        .index = 42,
        .name = "Dou Niu",
        .age = "38",
        .birthDate = "5/6/1948",
        .starSign = "Taurus",
        .bloodType = "B",
        .description = "He is the leader of the Yellow Heads."},
    {.character = SHEN_NAME(YUA_),
        .index = 43,
        .name = "Yuan",
        .age = "24",
        .birthDate = "4/4/1962",
        .starSign = "Aries",
        .bloodType = "AB",
        .description = "She is the executive of the Yellow Heads."},
    {.character = SHEN_NAME(RNP_),
        .index = 44,
        .name = "Lianfei Yang",
        .age = "17",
        .birthDate = "12/14/1969",
        .starSign = "Sagittarius",
        .bloodType = "O",
        .description = "She is the manager\nof a cargo-handling job."},
    {.character = SHEN_NAME(AW2_),
        .index = 45,
        .name = "Congo Maccius",
        .age = "31",
        .birthDate = "11/25/1955",
        .starSign = "Sagittarius",
        .bloodType = "O",
        .description = "He is Ryo's friend on a part-time job."},
    {.character = SHEN_NAME(DM2_),
        .index = 46,
        .name = "Haishan Lie",
        .age = "38",
        .birthDate = "2/4/1949",
        .starSign = "Aquarius",
        .bloodType = "B",
        .description = "He is a bookie of Lucky Hit in Dragon St."},
    {.character = SHEN_NAME(CH8_),
        .index = 47,
        .name = "Yanjing (Specs)",
        .age = "10",
        .birthDate = "2/9/1977",
        .starSign = "Aquarius",
        .bloodType = "B",
        .description = "A kid in Guilin."},
}};
inline constexpr std::array<std::array<std::int32_t, 4>, 6> SNAPSHOT_SECRET_PICTURES{
    {{0, 4, 352, 448}, {1, 4, 352, 448}, {2, 4, 352, 448}, {3, 4, 352, 448}, {4, 9, 534, 356}, {5, 9, 534, 356}}};
}
