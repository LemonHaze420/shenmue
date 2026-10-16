#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name TGBT_AREA{SHEN_NAME(TGBT)}, TGBT_PACKAGE{SHEN_NAME(TBGT)}, TGBT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name TGBT_AUTHOR_TASK{SHEN_NAME(auth)}, TGBT_PLAYER{SHEN_NAME(RYO_)}, TGBT_REN{SHEN_NAME(REN_)};
inline constexpr shen_name TGBT_FIRST_FIGHTER{SHEN_NAME(KT1_)}, TGBT_SECOND_FIGHTER{SHEN_NAME(KT2_)}, TGBT_THIRD_FIGHTER{SHEN_NAME(EHY_)};
inline constexpr shen_name TGBT_BASE{SHEN_NAME(BASE)}, TGBT_PROP{SHEN_NAME(R110)}, TGBT_INVALID{0xffffffffU};
inline constexpr shen_name TGBT_MAP_PART{SHEN_NAME(0000)}, TGBT_FIELD_PART{SHEN_NAME(QF0X)}, TGBT_RETRY_TITLE{SHEN_NAME(RTR2)};
inline constexpr shen_name TGBT_AUTHORS[]{SHEN_NAME(AU_0), SHEN_NAME(AU_1), SHEN_NAME(AU_2), SHEN_NAME(AU_3), SHEN_NAME(AU_4), SHEN_NAME(AU_5)};
inline constexpr shen_name TGBT_AUTHOR_CHARACTERS[]{SHEN_NAME(aa49), SHEN_NAME(a452), SHEN_NAME(ab19), SHEN_NAME(ab18)};
inline constexpr shen_name TGBT_LOAD_CHARACTERS[]{TGBT_REN, TGBT_FIRST_FIGHTER, TGBT_SECOND_FIGHTER, TGBT_THIRD_FIGHTER, TGBT_INVALID};
inline constexpr shen_name TGBT_HIDDEN_CHARACTERS[]{TGBT_REN, TGBT_SECOND_FIGHTER, TGBT_FIRST_FIGHTER, TGBT_THIRD_FIGHTER, TGBT_INVALID};
inline constexpr Vector3 TGBT_PLAYER_POSITION{-1.6399999856948853f, 0, -1.649999976158142f};
inline constexpr Vector3 TGBT_FIRST_FIGHTER_POSITION{1.309999942779541f, 0, -0.8899999856948853f};
inline constexpr Vector3 TGBT_SECOND_FIGHTER_POSITION{-1.4600000381469727f, 0, 0.8899999856948853f};
inline constexpr Vector3 TGBT_THIRD_FIGHTER_POSITION{0.6100000143051147f, 0, 0.9300000071525574f};
inline constexpr Vector3 TGBT_EXIT_POSITION{242.02999877929688f, 118, 147.89999389648438f};
struct TgbtEventWork {
    HLib::Task *task;
    std::int32_t state;
    std::int32_t frames;
    EventReadCharactersWork *characters;
    std::uint8_t retries;
    std::byte reserved19[7];
};
struct TgbtAuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::int16_t state;
    std::byte reserved12[6];
};
struct TgbtAuthorWaitWork {
    TgbtAuthorWork *parent;
    std::int16_t state;
    std::byte reserved0a[6];
};
static_assert(sizeof(TgbtEventWork) == 0x20);
static_assert(offsetof(TgbtEventWork, characters) == 0x10);
static_assert(offsetof(TgbtEventWork, retries) == 0x18);
static_assert(sizeof(TgbtAuthorWork) == 0x18);
static_assert(offsetof(TgbtAuthorWork, author) == 0x0c);
static_assert(offsetof(TgbtAuthorWork, state) == 0x10);
static_assert(sizeof(TgbtAuthorWaitWork) == 0x10);
void TgbtInit();
void TgbtMain(std::uintptr_t);
void TgbtEventCallback(void *);
void InitializeTgbtResources();
void CleanupTgbtEvent(TgbtEventWork *);
void QueueTgbtAuthor(shen_name author);
void TgbtAuthorCallback(void *);
void TgbtAuthorWaitCallback(void *);
void PrepareTgbtAuthoredCharacters();
void RestoreTgbtAuthoredCharacters();
void TgbtDormantBoundary();
extern const ScriptAreaDefinition tgbtScriptDefinition;
} // namespace shenmue::s2
