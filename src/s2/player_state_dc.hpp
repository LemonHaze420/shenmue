#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstddef>
#include <cstdint>

namespace shenmue {
namespace s2 {

struct Character;
struct PlayerModelState;
struct PlayerRuntimeState {
    std::uint32_t flags;
    std::int32_t field04;
    std::int32_t field08;
    std::int32_t currentDay;
    Character *character;
    std::uint32_t field14;
    PlayerModelState *model;
    std::uint16_t savedHeldStateFlags;
    std::int16_t scheduledCallbackMinute;
};
typedef char Layout_player_state_dc_0[(sizeof(PlayerRuntimeState) == 0x20) ? 1 : -1];
typedef char Layout_player_state_dc_1[(offsetof(PlayerRuntimeState, field08) == 8) ? 1 : -1];
typedef char Layout_player_state_dc_2[(offsetof(PlayerRuntimeState, character) == 0x10) ? 1 : -1];
typedef char Layout_player_state_dc_3[(offsetof(PlayerRuntimeState, model) == 0x18) ? 1 : -1];
typedef char Layout_player_state_dc_4[(offsetof(PlayerRuntimeState, savedHeldStateFlags) == 0x1c) ? 1 : -1];
typedef char Layout_player_state_dc_5[(offsetof(PlayerRuntimeState, scheduledCallbackMinute) == 0x1e) ? 1 : -1];

extern PlayerRuntimeState g_playerRuntime;
extern PlayerRuntimeState *g_playerRuntimePointer;
struct PlayerPersistentState {
    std::uint32_t present;
    std::int32_t runtimeState;
    std::uint16_t playerStateFlags, callbackValue;
    std::uint32_t reserved;
};
typedef char Layout_player_state_dc_6[(sizeof(PlayerPersistentState) == 0x10) ? 1 : -1];
extern PlayerPersistentState g_pendingPlayerPersistentState, *g_pendingPlayerPersistentStatePointer;
std::uint32_t plymConsumeFlags8();
std::uint32_t plymSetFlags(std::uint32_t flags);
std::uint32_t plymGetCurrentFlags();
std::uint8_t plymGetLookZoomScale();
bool plymIsSearchStartRequested();
void InitializePlayerRuntimeStorage();
void InitializePlayerSystem();
void InitializePlayerRuntime();
std::int32_t ResetPendingPlayerPersistentState(std::int32_t unused);
std::int32_t plymImportPersistentState(const PlayerPersistentState *state);
std::int32_t plymExportPersistentState(PlayerPersistentState *state);

} // namespace s2
} // namespace shenmue

#endif
