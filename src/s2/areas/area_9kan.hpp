#pragma once
#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/hlib.hpp"
#include <bit>
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name NINE_KAN_TASK{SHEN_NAME(EVNT)}, NINE_KAN_SOUND_TASK{SHEN_NAME(setg)};
inline constexpr shen_name NINE_KAN_RYO{SHEN_NAME(RYO_)}, NINE_KAN_REN{SHEN_NAME(REN_)}, NINE_KAN_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name NINE_KAN_CHO{SHEN_NAME(CHO_)}, NINE_KAN_YUA{SHEN_NAME(YUA_)}, NINE_KAN_DUMMY{SHEN_NAME(DAMY)};
inline constexpr shen_name NINE_KAN_BIRD{SHEN_NAME(QKAN)}, NINE_KAN_AUTHOR_BIRD{SHEN_NAME(a336)}, NINE_KAN_DOOR{SHEN_NAME(R141)};
inline constexpr shen_name NINE_KAN_LEFT_DOOR{SHEN_NAME(DOA1)}, NINE_KAN_RIGHT_DOOR{SHEN_NAME(DOA2)}, NINE_KAN_BOX{SHEN_NAME(_box)};
inline constexpr shen_name NINE_KAN_SHELF{SHEN_NAME(TANS)}, NINE_KAN_PROP{SHEN_NAME(ME14)}, NINE_KAN_INVALID{0xffffffffU};
inline constexpr shen_name NINE_KAN_LIGHT{SHEN_NAME(l001)}, NINE_KAN_INTRO{SHEN_NAME(A020)}, NINE_KAN_RETRY_INTRO{SHEN_NAME(A030)};
inline constexpr shen_name NINE_KAN_NEXT_DAY{SHEN_NAME(NDAY)}, NINE_KAN_DESTINATION{SHEN_NAME(0240)};
inline constexpr shen_name NINE_KAN_FIRST_CHARACTERS[]{NINE_KAN_REN, NINE_KAN_TGY, NINE_KAN_CHO, NINE_KAN_YUA, NINE_KAN_INVALID};
inline constexpr shen_name NINE_KAN_REPEAT_CHARACTERS[]{NINE_KAN_REN, NINE_KAN_INVALID};
inline constexpr shen_name NINE_KAN_SEARCH_TARGETS[]{SHEN_NAME(MA09), SHEN_NAME(MA10), SHEN_NAME(MA07), SHEN_NAME(MA06), SHEN_NAME(MA08), SHEN_NAME(MA03),
    SHEN_NAME(MA04), SHEN_NAME(MA05), SHEN_NAME(MA11), SHEN_NAME(MA02), SHEN_NAME(M113), NINE_KAN_PROP, shen_name{}};
inline constexpr Vector3 NINE_KAN_ORIGIN{.x = 237.60001f, .y = 114.4f, .z = 56.700001f};
inline constexpr Vector3i NINE_KAN_ROTATION{.x = 0, .y = 0, .z = 0};
inline constexpr float NINE_KAN_DOOR_STEPS[]{1, 1, 1.5f, 1.5f, 2, 2, 3, 3, 3.5f, 3.5f, 4.5f, 4.5f, 5.5f, 5.5f, 6.5f, 6.5f, 7, 7, 7, 7, 7};
inline constexpr float NINE_KAN_LEFT_FINAL = std::bit_cast<float>(0xc2b40000U), NINE_KAN_RIGHT_FINAL = std::bit_cast<float>(0x43870000U);
inline constexpr std::int32_t NINE_KAN_SHELF_PARTS[]{152, -1};
inline constexpr Vector3i NINE_KAN_HAND_ROTATION{.x = 0, .y = 61350, .z = 63898};
inline std::int32_t nineKanLeftHandPose[19][3]{{0, 726, 63535}, {0, 0, 65354}, {0, 62624, 3821}, {0, 0, 60258}, {0, 0, 60987}, {0, 0, 65172}, {0, 546, 1820},
    {0, 0, 58802}, {0, 0, 64263}, {0, 0, 0}, {0, 546, 1456}, {0, 0, 57710}, {0, 0, 64627}, {0, 0, 546}, {0, 546, 908}, {0, 0, 52979}, {0, 0, 56801},
    {0, 0, 59349}, {0, 546, 1091}};
inline std::int32_t nineKanRightHandPose[19][3]{{0, 726, 63535}, {0, 63898, 0}, {0, 62624, 3821}, {0, 0, 60258}, {0, 0, 61168}, {0, 0, 64080}, {0, 546, 1820},
    {0, 0, 58802}, {0, 0, 62989}, {0, 0, 63716}, {0, 546, 1456}, {0, 0, 56436}, {0, 0, 56072}, {0, 0, 59167}, {0, 546, 908}, {0, 0, 52979}, {0, 0, 56801},
    {0, 0, 59349}, {0, 546, 1091}};
struct NineKanWork {
    HLib::Task *task;
    std::int32_t state;
    DateTime date;
    std::byte reserved13[5];
    std::int32_t introRetry, comboRetry, promptRetry;
    std::uint32_t deadline;
    HLib::Task *soundTask;
    std::int32_t countdown;
    std::uint32_t reserved34;
};
static_assert(sizeof(NineKanWork) == 0x38 && offsetof(NineKanWork, state) == 8 && offsetof(NineKanWork, date) == 0xc);
static_assert(offsetof(NineKanWork, introRetry) == 0x18 && offsetof(NineKanWork, comboRetry) == 0x1c && offsetof(NineKanWork, promptRetry) == 0x20);
static_assert(offsetof(NineKanWork, deadline) == 0x24 && offsetof(NineKanWork, soundTask) == 0x28 && offsetof(NineKanWork, countdown) == 0x30);
struct NineKanTinyMotion {
    shen_name character;
    std::uint32_t motion;
};
static_assert(sizeof(NineKanTinyMotion) == 8);
struct NineKanHidingPlace {
    std::int32_t state;
    Vector3 position;
    float radius;
};
inline constexpr NineKanHidingPlace NINE_KAN_HIDING_PLACES[]{{.state = 14, .position = {.x = 4.1f, .y = 0, .z = 4.1f}, .radius = 1.6f},
    {.state = 15, .position = {.x = 3.4f, .y = 0, .z = 8.36f}, .radius = 1.3f}, {.state = 16, .position = {.x = 11.15f, .y = 0, .z = 10.7f}, .radius = 1.2f},
    {.state = 17, .position = {}, .radius = 0}};
extern HLib::Task *nineKanOpenDoorTask, *nineKanBurstDoorTask, *nineKanEntryDoorTask, *nineKanCloseDoorTask;
extern HLib::Task *nineKanAuxiliaryTask, *nineKanOpenBoxTask, *nineKanCloseBoxTask;
extern std::int32_t nineKanSoundCounter;
void Area9KANInit();
void Area9KANMain(std::uintptr_t argument);
void NineKanLoop(void *data);
void NineKanCleanupTasks();
void NineKanSetAuthoredControl(std::int32_t enabled);
bool NineKanAtIntroductionPoint();
std::int32_t NineKanChooseHidingPlace();
void NineKanOpenDoor(void *data);
void NineKanCloseDoor(void *data);
void NineKanBurstDoor(void *data);
void NineKanPlayTinyMotion(void *data);
void NineKanEntryDoor(void *data);
void NineKanTogyuSound(void *data);
void NineKanOpenBox(void *data);
void NineKanCloseBox(void *data);
void NineKanQueueOpenDoor();
void NineKanQueueCloseDoor();
void NineKanInitialPlacement();
std::int32_t NineKanBeginSearch();
void NineKanEnterHidingPlace(std::int32_t state);
std::int32_t NineKanTogyuSearch();
std::int32_t NineKanSearchFailure();
void NineKanComboSuccess();
std::int32_t NineKanComboFailure();
void NineKanShelfSuccess();
std::int32_t NineKanShelfFailure();
void NineKanEscape();
std::int32_t NineKanRepeatBox();
void NineKanRepeatFinish();
extern const ScriptAreaDefinition area9KANScriptDefinition;
} // namespace shenmue::s2::areas
