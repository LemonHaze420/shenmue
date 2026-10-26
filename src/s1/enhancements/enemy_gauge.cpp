#include "s1/enhancements/enemy_gauge.hpp"

#if SHENMUE_ENHANCEMENT_S1_ENEMY_HP_GAUGE && SHENMUE_IS_S1 && SHENMUE_IS_PC

#include "s1/task/vfma.hpp"
#include "s1/task/char.hpp"
#include "s1/ui_internal.hpp"

namespace shenmue::s1::enhancements {
namespace {
using namespace ui_runtime_detail;

EnemyGaugeState g_enemyGauge{};

bool IsEnemy(const VFMAData &game, std::uint32_t playerSlot, std::uint32_t slot) {
    if (slot >= kBattleSlots || slot == playerSlot || game.slotTeamId[slot] == game.slotTeamId[playerSlot])
        return false;
    const VFActor *const actor = game.actors[slot];
    return actor && actor->game == &game && actor->maximumHp > 0 && actor->currentHp >= 0;
}

VFActor *FindEnemy(const VFMAData &game, std::uint32_t playerSlot) {
    if (IsEnemy(game, playerSlot, game.primaryTargetSlot))
        return game.actors[game.primaryTargetSlot];
    const VFActor *const target = game.actors[playerSlot]->targetActor;
    for (std::uint32_t slot = 0; slot < kBattleSlots; ++slot) {
        if (game.actors[slot] == target && IsEnemy(game, playerSlot, slot))
            return game.actors[slot];
    }
    for (std::uint32_t slot = 0; slot < kBattleSlots; ++slot) {
        if (IsEnemy(game, playerSlot, slot) && game.actors[slot]->currentHp > 0)
            return game.actors[slot];
    }
    for (std::uint32_t slot = 0; slot < kBattleSlots; ++slot) {
        if (game.actors[slot] == g_enemyGauge.target && IsEnemy(game, playerSlot, slot))
            return game.actors[slot];
    }
    return nullptr;
}

void HideEnemy() {
    HUD_FightGauge_SetDisplayLayout(0);
    g_fightGaugeFrameFlags = (g_fightGaugeFrameFlags & ~3U) | 1U;
    g_enemyGauge.target = nullptr;
    g_enemyGauge.character = nullptr;
    g_enemyGauge.characterName = {};
    g_enemyGauge.displayedHp = 0;
}
} // namespace

void ResetEnemyGauge() {
    g_enemyGauge = {};
}

void UpdateEnemyGauge(VFMAData &game, float pulsePosition) {
    using namespace ui_runtime_detail;
    if (g_enemyGauge.game != &game)
        g_enemyGauge = {.game = &game};
    const std::uint32_t playerSlot = static_cast<std::uint8_t>(game.playerSlot);
    if (playerSlot >= kBattleSlots || !game.actors[playerSlot]) {
        HideEnemy();
        return;
    }
    VFActor *const enemy = FindEnemy(game, playerSlot);
    if (!enemy) {
        HideEnemy();
        return;
    }
    const std::uint32_t playerIndex = static_cast<std::uint32_t>(g_fightGaugeWidgetIndices[0]);
    const std::uint32_t enemyIndex = static_cast<std::uint32_t>(g_fightGaugeWidgetIndices[1]);
    if (playerIndex >= g_fightGaugeWidgetStates.size() || enemyIndex >= g_fightGaugeWidgetStates.size() || playerIndex == enemyIndex)
        return;
    d3t::UIGaugeState &playerGauge = g_fightGaugeWidgetStates[playerIndex];
    d3t::UIGaugeState &enemyGauge = g_fightGaugeWidgetStates[enemyIndex];
    if (!playerGauge.allocated || !enemyGauge.allocated || enemyGauge.count <= 0 || static_cast<std::size_t>(enemyGauge.count) > enemyGauge.orbColours.size())
        return;
    const Character *const character = enemy->character;
    const shen_name name = character ? character->entity.name : shen_name{};
    if (g_enemyGauge.target != enemy || g_enemyGauge.character != character || g_enemyGauge.characterName != name) {
        g_enemyGauge.target = enemy;
        g_enemyGauge.character = character;
        g_enemyGauge.characterName = name;
        g_enemyGauge.displayedHp = enemy->currentHp;
    }
    std::int32_t delta = static_cast<std::int32_t>(enemy->currentHp) - g_enemyGauge.displayedHp;
    if (delta > 8)
        delta = 8;
    else if (delta < -8)
        delta = -8;
    std::int32_t hp = static_cast<std::int32_t>(g_enemyGauge.displayedHp) + delta;
    if (hp < 0)
        hp = 0;
    else if (hp > enemy->maximumHp)
        hp = enemy->maximumHp;
    g_enemyGauge.displayedHp = static_cast<std::int16_t>(hp);
    HUD_FightGauge_SetDisplayLayout(1);
    playerGauge.translucentLastOrb = 1;
    enemyGauge.translucentLastOrb = 1;
    for (std::size_t index = 0; index < enemyGauge.orbColours.size(); ++index)
        enemyGauge.orbColours[index] = index < 8 ? 2 : index < 11 ? 3 : 1;
    const float filled = static_cast<float>(enemyGauge.count) * (static_cast<float>(hp) / static_cast<float>(enemy->maximumHp));
    enemyGauge.filledCount = static_cast<std::int32_t>(filled);
    if (filled > static_cast<float>(enemyGauge.filledCount) && enemyGauge.filledCount < enemyGauge.count)
        ++enemyGauge.filledCount;
    enemyGauge.suppressEmptyOrbs = 0;
    playerGauge.lastOrbAlpha = pulsePosition;
    enemyGauge.lastOrbAlpha = pulsePosition;
    g_fightGaugeFrameFlags = (g_fightGaugeFrameFlags & ~3U) | 2U;
}

} // namespace shenmue::s1::enhancements

#endif
