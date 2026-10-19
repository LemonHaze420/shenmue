#pragma once
#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/areas/area_entities.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name CHAW_AREA{SHEN_NAME(CHAW)}, CHAW_TASK{SHEN_NAME(chaw)}, CHAW_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name CHAW_RYO{SHEN_NAME(RYO_)}, CHAW_BASE{SHEN_NAME(BASE)}, CHAW_HAND{SHEN_NAME(hnd1)}, CHAW_FACE{SHEN_NAME(face)};
inline constexpr shen_name CHAW_INVALID{0xffffffffU}, CHAW_CUP_BASE{SHEN_NAME(cha1)}, CHAW_VOICE_TASK{SHEN_NAME(voic)};
inline constexpr shen_name CHAW_LECTURE_CHARACTERS[]{SHEN_NAME(KEK_), CHAW_INVALID};
inline constexpr shen_name CHAW_TOSHOKEN_CHARACTERS[]{SHEN_NAME(CHO_), CHAW_INVALID};
inline constexpr shen_name CHAW_KUN_CHARACTERS[]{SHEN_NAME(KUN_), SHEN_NAME(EDL_), CHAW_INVALID};
inline constexpr shen_name CHAW_HYAKUSI_CHARACTERS[]{SHEN_NAME(LCN_), CHAW_INVALID};
inline constexpr shen_name CHAW_AUTHOR_CUPS[]{SHEN_NAME(a058), SHEN_NAME(a059), SHEN_NAME(a060), SHEN_NAME(a061)};
inline constexpr shen_name CHAW_TOSHOKEN_PROPS[]{SHEN_NAME(a020), SHEN_NAME(a022), SHEN_NAME(a021)};
inline constexpr char CHAW_AUDIO_BANKS[4][16]{"0038E000", "0038E000", "0038E000", "0228A000"};
struct ChawEventWork {
    std::uint16_t pendingButtons, lectureWord;
    std::uint32_t reserved04;
    HLib::Task *task;
    std::uint32_t helpFlags;
    std::int32_t entry, reserved18;
    Vector3 handTarget;
    std::int32_t handYaw, handMovement, selectedCup, row, carriedCup;
    shen_name cups[4];
    std::int32_t cupRows[4];
    std::int32_t alternate, started, state, advanceTime, lectureLoaded, cupMoved, error, wrongSign, enabled, reminders, reserved84;
    std::int32_t lecturePhase, lastReminderSecond, progression, cameraOffset;
    std::uint32_t ticks, reserved9c;
    std::int64_t savedAudioScene;
    char savedBgm[16], savedSbt[16];
};
struct ChawSerifWork {
    const char *choices[9];
    std::int32_t phase;
    std::uint32_t reserved4c;
};
struct ChawModelProfile {
    char cupModel[16], faceModel[16];
    shen_name lighting;
    float alpha;
};
static_assert(sizeof(ChawEventWork) == 0xc8 && offsetof(ChawEventWork, task) == 8 && offsetof(ChawEventWork, entry) == 0x14);
static_assert(offsetof(ChawEventWork, cups) == 0x3c && offsetof(ChawEventWork, cupRows) == 0x4c && offsetof(ChawEventWork, lecturePhase) == 0x88);
static_assert(offsetof(ChawEventWork, ticks) == 0x98 && offsetof(ChawEventWork, savedAudioScene) == 0xa0 && offsetof(ChawEventWork, savedSbt) == 0xb8);
static_assert(sizeof(ChawSerifWork) == 0x50 && sizeof(ChawModelProfile) == 0x28);
inline constexpr float CHAW_HAND_X[12]{0.01f, 0.09f, 0.17f, 0.24f, 0.01f, 0.09f, 0.17f, 0.24f, 0.01f, 0.09f, 0.17f, 0.24f};
inline constexpr float CHAW_HAND_Z[12]{0.39f, 0.41f, 0.43f, 0.44f, 0.49f, 0.51f, 0.53f, 0.54f, 0.59f, 0.61f, 0.63f, 0.64f};
inline constexpr float CHAW_HAND_BLEND[13]{0, 0.1305f, 0.2588f, 0.3827f, 0.5f, 0.6088f, 0.7071f, 0.7934f, 0.866f, 0.9239f, 0.9659f, 0.9914f, 1};
inline constexpr std::int32_t CHAW_HAND_YAW[12]{40960, 39685, 37865, 36408, 41870, 40049, 38229, 36408, 44600, 41870, 39139, 36408};
inline constexpr std::int32_t CHAW_SUCCESS_SIGN[4]{0, 1, 2, 1};
inline constexpr std::int32_t CHAW_LECTURE_SIGNS[3][4]{{1, 1, 1, 1}, {0, 1, 1, 1}, {0, 1, 2, 1}};
inline constexpr std::int32_t CHAW_PLACEMENT_FLAGS[22]{
    1472, 1473, 1474, 1476, 1475, 1490, 1489, 1487, 1488, 1471, 1481, 1477, 1478, 1479, 1480, 1491, 1482, 1483, 1484, 1485, 1486, -1};
inline constexpr ChawModelProfile CHAW_MODELS[22]{{"CWJN2K2G", "CWJN2T6G", {}, 0.58f}, {"CWJN2O2G", "CWJN2T6G", {}, 0.58f},
    {"CWJN2G2G", "CWJN2T3G", SHEN_NAME(WNS4), 0.58f}, {"CWJN2E2G", "CWJN2T2G", SHEN_NAME(WNC0), 0.58f}, {"CWJN2Y2G", "CWJN2T3G", SHEN_NAME(WNS5), 0.58f},
    {"CWJN2O2G", "CWJN2T5G", {}, 0.58f}, {"CWJN2Y2G", "CWJN2T5G", {}, 0.58f}, {"CWJN2B2G", "CWJN2T3G", {}, 0.58f}, {"CWJN2Q2G", "CWJN2T4G", {}, 0.58f},
    {"CWJN2Q2G", "CWJN2T2G", SHEN_NAME(WTS3), 0.58f}, {"CWJN2D2G", "CWJN2T5G", {}, 0.58f}, {"CWJN2B2G", "CWJN2T3G", {}, 0.58f},
    {"CWJN2K2G", "CWJN2T3G", {}, 0.58f}, {"CWJN2P2G", "CWJN2T3G", {}, 0.58f}, {"CWJN2D2G", "CWJN2T2G", {}, 0.58f}, {"CWJN2COG", "CWJN2T5G", {}, 0.58f},
    {"CWJN2B2G", "CWJN2T3G", {}, 0.58f}, {"CWJN2B2G", "CWJN2T4G", {}, 0.58f}, {"CWJN2COG", "CWJN2T5G", {}, 0.58f}, {"CWJN2B2G", "CWJN2T5G", {}, 0.58f},
    {"CWJN2B2G", "CWJN2T5G", {}, 0.58f}, {"CWJN2R2G", "CWJN2T1G", SHEN_NAME(0000), 0.58f}};
extern Vector3 chawPreviousHand;
extern std::int32_t chawLectureErrors, chawSerifDelay;

Vector3 ChawGetHandTarget(std::int32_t cell);
std::int32_t ChawGetHandYaw(std::int32_t cell);
void ChawUpdateHand(ChawEventWork *);
void ChawSelectCup(ChawEventWork *, std::int32_t);
void ChawMoveCup(ChawEventWork *, std::int32_t);
void ChawAttachCup(ChawEventWork *, std::int32_t);
void ChawPlaceCup(ChawEventWork *, std::int32_t);
void ChawSetupChoice(std::int16_t);
void ChawStoreSign(ChawEventWork *);
std::int32_t ChawGetProgression(ChawEventWork *);
void ChawQueueSerif(const char *, const char *, const char *, const char *, const char *, const char *);
void ChawSerifCallback(void *);
bool ChawReminderDue(ChawEventWork *);
void ChawAdvanceLecture(ChawEventWork *, std::int32_t);
bool ChawCheckLectureSelection(ChawEventWork *, std::int32_t);
bool ChawCheckLectureMovement(ChawEventWork *, std::int32_t);
extern AreaEntityPlacement chawPlacements[22];
extern DateTime chawTargetDate;
extern shen_name chawAuthor, chawMapProp, chawAmbushCharacters[2];
extern std::uint32_t chawPrize, chawSmoke;
void AreaCHAWInit();
void AreaCHAWMain(std::uintptr_t);
extern const ScriptAreaDefinition areaCHAWScriptDefinition;
void ChawInitialize(ChawEventWork *);
void ChawCleanup(ChawEventWork *, bool);
void ChawRestoreLighting(ChawEventWork *);
void ChawLoadAuthorCups(ChawEventWork *);
void ChawCreateAuthorCups(const char *);
void ChawSetupAuthorCharacter(shen_name);
void ChawFadeOutCallback(void *);
void ChawFadeAudioOutCallback(void *);
void ChawQueueFadeAudioOut(std::int32_t);
void ChawForwardTimeCallback(void *);
void ChawStartCallback(void *);
void ChawInitCallback(void *);
void ChawPlayCallback(void *);
void ChawQueueExit(ChawEventWork *);
void ChawCheckChoiceCallback(void *);
void ChawSuccessAuthorCallback(void *);
void ChawPlaySuccessAuthorCallback(void *);
void ChawFinishSuccessAuthorCallback(void *);
void ChawSuccessBurinCallback(void *);
void ChawGotoYamiutiCallback(void *);
void ChawPlayYamiutiCallback(void *);
void ChawFinishYamiutiCallback(void *);
void ChawGotoDokuhebiCallback(void *);
void ChawGotoSeiyaCallback(void *);
void ChawGotoKunCallback(void *);
void ChawPlayKunCallback(void *);
void ChawFinishKunCallback(void *);
void ChawGotoHyakusiCallback(void *);
void ChawPlayHyakusiCallback(void *);
void ChawFinishHyakusiCallback(void *);
void ChawFailCallback(void *);
void ChawFailMemoCallback(void *);
void ChawStandUpCallback(void *);
void ChawFinishCallback(void *);
void ChawPreLectureCallback(void *);
void ChawPreLectureAuthorCallback(void *);
void ChawStartLectureCallback(void *);
bool ChawLectureAllowsInput(ChawEventWork *);
void ChawEndLectureCallback(void *);
std::uint32_t ChawCreateLectureSmoke();
} // namespace shenmue::s2
