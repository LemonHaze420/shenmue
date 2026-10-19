#pragma once

#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {



struct QKQBAuthoredEntry {
    std::int32_t cutMode;
    Vector3 basePosition;
    float baseYaw;
    Vector3 battlePosition;
    float battleRadius;
    std::int32_t enemyCount, enemyHp, fightStyle;
    Vector3 returnPosition;
    float returnYaw;
    std::array<std::uint16_t, 4> commands;
    std::int16_t maximumQteFrames;
    std::uint16_t retryQteFrames;
    std::int32_t requiredCommands;
};
static_assert(sizeof(QKQBAuthoredEntry) == 0x50);
static_assert(offsetof(QKQBAuthoredEntry, enemyCount) == 0x24);
static_assert(offsetof(QKQBAuthoredEntry, commands) == 0x40);
static_assert(offsetof(QKQBAuthoredEntry, requiredCommands) == 0x4c);
inline constexpr std::array<QKQBAuthoredEntry, 5> qkqbAuthoredEntries{{
    {4, {216.1f, 0, 145.8f}, 90, {217.4f, 0, 149}, 5, 1, 200, 2, {214, 0, 147}, 90, {16, 512, 0, 0}, 16, 24, 2},
    {2, {247.5f, 0, 149.3f}, 180, {250.2f, 0, 147.7f}, 5, 1, 300, 2, {248.6f, 0, 149.5f}, 180, {4, 512, 256, 0}, 24, 36, 3},
    {3, {250.4f, 0, 165.5f}, 0, {248.7f, 0, 166.4f}, 3, 1, 400, 2, {248.7f, 0, 165.2f}, 0, {256, 1, 256, 0}, 24, 36, 3},
    {1, {199.8f, 0, 176.4f}, 0, {197.5f, 0, 177.9f}, 5, 1, 500, 5, {199, 0, 176.3f}, 0, {256, 64, 1024, 0}, 24, 36, 3},
    {0, {218.8f, 0, 127.3f}, 270, {217.6f, 0, 124.2f}, 5, 1, 600, 6, {219, 0, 125.9f}, -90, {1024, 4, 4, 1024}, 32, 48, 4},
}};
inline constexpr std::array<std::int32_t, 6> qkqbEntryPoints{0, 1, 2, 3, 4, 0};
inline constexpr std::array<shen_name, 5> qkqbEnemyNames{SHEN_NAME(KF5_), SHEN_NAME(KR3_), SHEN_NAME(KF2_), SHEN_NAME(KF3_), SHEN_NAME(BF7_)};

struct QKQBEventWork {
    EventReadCharactersWork *characters;
    HLib::Task *task;
    std::uint64_t motionHandle;
    const std::int32_t *entry;
    const QKQBAuthoredEntry *authored;
    float playerY;
    std::int32_t remainingFrames;
};
static_assert(sizeof(QKQBEventWork) == 0x30);
static_assert(offsetof(QKQBEventWork, task) == 8);
static_assert(offsetof(QKQBEventWork, entry) == 0x18);
static_assert(offsetof(QKQBEventWork, authored) == 0x20);
static_assert(offsetof(QKQBEventWork, playerY) == 0x28);

extern const ScriptAreaDefinition qkqbScriptDefinition;
void Queue_eQKQB_EVT_StartQKuro(std::uintptr_t entry);
void eQKQB_EVT_StartQKuro_Callback(void *data);
void eQKQB_EVT_WaitQTE_Callback(void *data);
void eQKQB_EVT_WaitPA_Callback(void *data);
void eQKQB_EVT_StartBattle_Callback(void *data);
void eQKQB_EVT_PlayBattle_Callback(void *data);
void eQKQB_EVT_RetryQKuro_Callback(void *data);
void eQKQB_EVT_EndQKuro_Callback(void *data);

} // namespace shenmue::s2
