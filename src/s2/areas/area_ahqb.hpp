#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include <array>
#include <span>
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AHQB_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name AHQB_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name AHQB_HTK{SHEN_NAME(HTK_)};
inline constexpr shen_name AHQB_GOI{SHEN_NAME(GOI_)};
inline constexpr shen_name AHQB_BASE{SHEN_NAME(ABAS)};
inline constexpr shen_name AHQB_DUMMY{SHEN_NAME(DUM0)};
inline constexpr shen_name AHQB_WN00{SHEN_NAME(WN00)};
inline constexpr shen_name AHQB_WS00{SHEN_NAME(WS00)};
inline constexpr shen_name AHQB_AR03{SHEN_NAME(AR03)};
inline constexpr shen_name AHQB_DIS1{SHEN_NAME(dis1)};
inline constexpr shen_name AHQB_DIS2{SHEN_NAME(dis2)};
inline constexpr shen_name AHQB_FIRST_AUTHOR{SHEN_NAME(FIST)};
inline constexpr shen_name AHQB_WIN_AUTHOR{SHEN_NAME(WIN_)};
inline constexpr shen_name AHQB_LOSE_AUTHOR{SHEN_NAME(LOSE)};
inline constexpr shen_name AHQB_INVALID{0xffffffffU};
inline constexpr shen_name AHQB_CHARACTERS[]{AHQB_PLAYER, AHQB_GOI, AHQB_HTK, AHQB_INVALID};
struct AhqbEventWork {
    HLib::Task *task;
    std::uint16_t phase, reserved0a;
    std::uint32_t reserved0c;
    std::int32_t state;
    std::uint32_t reserved14;
    std::int32_t qteResult[4];
    EventReadCharactersWork *characters;
};
struct AhqbComboQteWork {
    HLib::Task *task;
    HLib::Task *parent;
    std::uint32_t frames;
    std::int32_t totalFrames;
    std::int16_t *result;
    std::int16_t count;
    std::uint16_t reserved22;
    std::array<std::uint32_t, 11> sequence;
};
static_assert(sizeof(AhqbEventWork) == 0x30);
static_assert(offsetof(AhqbEventWork, phase) == 8 && offsetof(AhqbEventWork, state) == 0x10);
static_assert(offsetof(AhqbEventWork, qteResult) == 0x18 && offsetof(AhqbEventWork, characters) == 0x28);
static_assert(sizeof(AhqbComboQteWork) == 0x50 && offsetof(AhqbComboQteWork, sequence) == 0x24);
void AHQBQueueComboQte(std::int16_t *result, std::int32_t totalFrames, std::span<const std::uint16_t> sequence);
void AHQBComboQteCallback(void *data);
void AreaAHQBInit();
void AreaAHQBMain(std::uintptr_t);
void AHQBQueueMain(void *);
void AHQBMainCallback(void *);
void AHQBEndCallback(void *);
std::int32_t AHQBInitialize(AhqbEventWork *);
std::int32_t AHQBCombatPhase(AhqbEventWork *);
void AHQBPlaceCombatants();
std::int32_t AHQBBeginCombat(AhqbEventWork *);
std::int32_t AHQBCompleteCombat();
std::int32_t AHQBFirstMiss();
std::int32_t AHQBFirstFailure();
std::int32_t AHQBFirstCounter();
std::int32_t AHQBFirstFailureCamera();
std::int32_t AHQBSecondSuccess();
std::int32_t AHQBSecondFailure();
std::int32_t AHQBFinalSuccess();
std::int32_t AHQBFinalFailure();
extern const ScriptAreaDefinition areaAHQBScriptDefinition;
} // namespace shenmue::s2::areas
