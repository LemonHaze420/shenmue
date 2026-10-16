#pragma once

#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name YMQB_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name YMQB_RYO{SHEN_NAME(RYO_)}, YMQB_BASE{SHEN_NAME(BASE)}, YMQB_DUMMY{SHEN_NAME(DUMR)};
inline constexpr shen_name YMQB_ANC{SHEN_NAME(ANC_)}, YMQB_A128{SHEN_NAME(a128)}, YMQB_GEN{SHEN_NAME(GEN_)};
inline constexpr shen_name YMQB_SBO{SHEN_NAME(SBO_)}, YMQB_CHR{SHEN_NAME(CHR_)}, YMQB_SYE{SHEN_NAME(SYE_)};
inline constexpr shen_name YMQB_INVALID{0xffffffffU}, YMQB_DEFAULT_GRID{SHEN_NAME(0000)}, YMQB_FIGHT_GRID{SHEN_NAME(BTC1)};
inline constexpr shen_name YMQB_WIN_AREA{SHEN_NAME(WT00)}, YMQB_LOSE_AREA{SHEN_NAME(0039)};
inline constexpr std::array<shen_name, 7> YMQB_CHARACTERS{YMQB_RYO, YMQB_GEN, YMQB_SBO, YMQB_CHR, YMQB_ANC, YMQB_SYE, YMQB_INVALID};
inline constexpr Vector3 YMQB_BASE_POSITION{244.6999969482422f, 115.1500015258789f, 347.70001220703125f};
inline constexpr Vector3 YMQB_BATTLE_EYE{241.5115966796875f, 117.04940032958984f, 348.73779296875f};
inline constexpr Vector3 YMQB_BATTLE_TARGET{246.410400390625f, 116.04640197753906f, 349.1564025878906f};

struct YmqbWork {
    EventReadCharactersWork *characters;
    HLib::Task *task;
    std::uint32_t motion;
    std::int32_t won;
    HLib::TaskCallback nextCallback;
    std::int32_t qteSucceeded;
    std::uint32_t reserved24;
};
static_assert(sizeof(YmqbWork) == 0x28 && offsetof(YmqbWork, task) == 8);
static_assert(offsetof(YmqbWork, motion) == 0x10 && offsetof(YmqbWork, won) == 0x14);
static_assert(offsetof(YmqbWork, nextCallback) == 0x18 && offsetof(YmqbWork, qteSucceeded) == 0x20);

void YmqbInitNullsub();
void YmqbMain(std::uintptr_t argument);
void YmqbSetupNullsub();
void YmqbSetup(YmqbWork *work);
void YmqbCleanup(YmqbWork *work);
void YmqbStartCallback(void *data);
void YmqbIntroCallback(void *data);
void YmqbQteResultCallback(void *data);
void YmqbWaitAuthorCallback(void *data);
void YmqbStartBattleCallback(void *data);
void YmqbBattleCallback(void *data);
void YmqbWinAuthorCallback(void *data);
void YmqbLoseAuthorCallback(void *data);
void YmqbResultAuthorCallback(void *data);
void YmqbEndCallback(void *data);
extern const ScriptAreaDefinition ymqbScriptDefinition;
} // namespace shenmue::s2::areas
