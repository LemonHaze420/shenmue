#pragma once

#include "shenmue.h"

#ifndef SHENMUE_ENHANCEMENT_S1_ENEMY_HP_GAUGE
#define SHENMUE_ENHANCEMENT_S1_ENEMY_HP_GAUGE 0
#endif

#if SHENMUE_ENHANCEMENT_S1_ENEMY_HP_GAUGE && SHENMUE_IS_S1 && SHENMUE_IS_PC

#include "common/types.hpp"
#include <cstdint>

namespace shenmue::s1 {
struct Character;
struct VFActor;
struct VFMAData;

namespace enhancements {
struct EnemyGaugeState {
    const VFMAData *game;
    const VFActor *target;
    const Character *character;
    shen_name characterName;
    std::int16_t displayedHp;
};

void ResetEnemyGauge();
void UpdateEnemyGauge(VFMAData &game, float pulsePosition);
} // namespace enhancements
} // namespace shenmue::s1

#endif
