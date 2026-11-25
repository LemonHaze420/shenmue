#pragma once

#include "s2/character_dc.hpp"
#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#define MODEL_COMPONENT_GAME SHEN_NAME4('G', 'A', 'M', 'E')
struct GameDisplayEntry {
    Vector3 position;
    MT7Node *node;
    float alpha;
    Vector3 scale;
};
struct GameDisplayWork {
    MT7Node *inserted;
    std::int32_t stagedCount;
    std::uint32_t baseNodeId;
    Vector3 digitStep;
    std::uint32_t maximumValue;
    std::int32_t decimalShift;
    std::uint32_t retainedState;
    GameDisplayEntry staged[64];
};
typedef char Layout_game_display_dc_0[(sizeof(GameDisplayEntry) == 0x20) ? 1 : -1];
typedef char Layout_game_display_dc_1[(offsetof(GameDisplayEntry, node) == 0x0c) ? 1 : -1];
typedef char Layout_game_display_dc_2[(offsetof(GameDisplayEntry, alpha) == 0x10) ? 1 : -1];
typedef char Layout_game_display_dc_3[(offsetof(GameDisplayEntry, scale) == 0x14) ? 1 : -1];
typedef char Layout_game_display_dc_4[(sizeof(GameDisplayWork) == 0x824) ? 1 : -1];
typedef char Layout_game_display_dc_5[(offsetof(GameDisplayWork, stagedCount) == 4) ? 1 : -1];
typedef char Layout_game_display_dc_6[(offsetof(GameDisplayWork, digitStep) == 0x0c) ? 1 : -1];
typedef char Layout_game_display_dc_7[(offsetof(GameDisplayWork, staged) == 0x24) ? 1 : -1];
void AttachGameDisplayComponent(Character *character);
void RunGameCallback(Character *character, void *payload);
void GAME_Callback(void *character);
void StageGameDisplayNode(GameDisplayWork *game, std::int32_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
GameDisplayWork *scenCopyCharaToGame(shen_name name);
void scenGetCharaGameNode(shen_name name, std::int32_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale);
GameDisplayWork *SetPosRotForGame(shen_name name, std::uint32_t baseNodeId, const Vector3 *digitStep, std::uint32_t maximumValue, std::int32_t decimalShift);
void gameSetObjectFlags(GameDisplayWork *game, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale);
void UpdateGameNodeFlags(shen_name name, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale);
#endif

} // namespace s2
} // namespace shenmue

#endif
