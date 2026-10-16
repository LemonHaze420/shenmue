#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

inline constexpr shen_name MODEL_COMPONENT_GAME{SHEN_NAME(GAME)};
struct GameDisplayEntry {
    Vector3 position;
    std::uint32_t reserved0c;
    MT7Node *node;
    std::int32_t nodeIdentity;
    float alpha;
    Vector3 scale;
    std::uint32_t reserved2c;
};
struct GameDisplayWork {
    MT7Node *inserted;
    MT7Node *previous;
    std::int32_t stagedCount;
    std::int32_t copiedCount;
    std::uint32_t baseNodeId;
    Vector3 digitStep;
    std::uint32_t maximumValue;
    std::int32_t decimalShift;
    std::uint32_t reserved30;
    std::uint32_t cachedWord34;
    GameDisplayEntry staged[64];
    GameDisplayEntry copied[64];
    std::byte reserved1838[0x100];
};
static_assert(sizeof(GameDisplayEntry) == 0x30);
static_assert(offsetof(GameDisplayEntry, node) == 0x10);
static_assert(offsetof(GameDisplayEntry, nodeIdentity) == 0x18);
static_assert(offsetof(GameDisplayEntry, alpha) == 0x1c);
static_assert(offsetof(GameDisplayEntry, scale) == 0x20);
static_assert(sizeof(GameDisplayWork) == 0x1938);
static_assert(offsetof(GameDisplayWork, stagedCount) == 0x10);
static_assert(offsetof(GameDisplayWork, copiedCount) == 0x14);
static_assert(offsetof(GameDisplayWork, baseNodeId) == 0x18);
static_assert(offsetof(GameDisplayWork, digitStep) == 0x1c);
static_assert(offsetof(GameDisplayWork, maximumValue) == 0x28);
static_assert(offsetof(GameDisplayWork, decimalShift) == 0x2c);
static_assert(offsetof(GameDisplayWork, cachedWord34) == 0x34);
static_assert(offsetof(GameDisplayWork, staged) == 0x38);
static_assert(offsetof(GameDisplayWork, copied) == 0xc38);
static_assert(offsetof(GameDisplayWork, reserved1838) == 0x1838);
bool AttachGameDisplayComponent(Character *character);
void RunGameCallback(Character *character, void *payload);
void GAME_Callback(void *character);
std::uint64_t CopyNodeIntoGame(const void *model, GameDisplayWork *game);
void StageGameDisplayNode(GameDisplayWork *game, std::uint16_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale);
std::uint64_t gameSetObjectFlags(GameDisplayWork *game, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale);
std::uint64_t scenCopyCharaToGame(shen_name id);
void scenGetCharaGameNode(shen_name id, std::uint16_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale);
GameDisplayWork *SetPosRotForGame(shen_name id, std::uint32_t baseNodeId, const Vector3 *digitStep, std::int32_t maximumValue, std::int32_t decimalShift);
std::uint64_t UpdateGameNodeFlags(shen_name id, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale);

} // namespace shenmue::s2
