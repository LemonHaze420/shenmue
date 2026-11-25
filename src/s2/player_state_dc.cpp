#include "s2/player_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/player_model_dc.hpp"
#include <cstring>
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/party_companion.hpp"
#endif

namespace shenmue {
namespace s2 {

PlayerRuntimeState g_playerRuntime;
PlayerRuntimeState *g_playerRuntimePointer = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
PlayerPersistentState *g_pendingPlayerPersistentStatePointer = &g_pendingPlayerPersistentState;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_playerRuntimeResetWord0;
std::uint32_t g_playerRuntimeResetWord1;
std::uint32_t g_playerRuntimeResetWord2;
std::uint32_t g_playerRuntimeResetWord3;
std::uint32_t g_playerRuntimeResetWord4;
std::uint32_t g_playerRuntimeResetWord5;
std::uint32_t g_playerRuntimeResetWord6;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetPendingPlayerPersistentState(std::int32_t) {
    PlayerPersistentState *const pending = g_pendingPlayerPersistentStatePointer;
    pending->present = 0;
    pending->runtimeState = 0;
    *reinterpret_cast<std::uint32_t *>(&pending->playerStateFlags) = 0;
    pending->reserved = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetPlayerRuntimeWords() {
    g_playerRuntimeResetWord1 = 0;
    g_playerRuntimeResetWord3 = 0;
    g_playerRuntimeResetWord5 = 0;
    g_playerRuntimeResetWord0 = 0;
    g_playerRuntimeResetWord6 = 0;
    g_playerRuntimeResetWord4 = 0;
    g_playerRuntimeResetWord2 = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PlayerModuleNullsubA(void *, float) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PlayerModuleNullsubB(void *, std::int16_t) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PlayerModuleNullsubC(std::uint32_t) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PlayerModuleNullsubD() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PlayerModuleNullsubE() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PlayerModuleNullsubF() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymImportPersistentState(const PlayerPersistentState *state) {
    PlayerPersistentState *const pending = g_pendingPlayerPersistentStatePointer;
    if (!state->present)
        return -1;
    *pending = *state;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t plymExportPersistentState(PlayerPersistentState *state) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!state)
        return -1;
    state->present = 0;
    state->runtimeState = 0;
    *reinterpret_cast<std::uint32_t *>(&state->playerStateFlags) = 0;
    state->reserved = 0;
    if (model) {
        state->present = 1;
        state->runtimeState = runtime->field08;
        state->playerStateFlags = model->heldStateFlags;
        state->callbackValue = model->savedHeldStateFlags;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializePlayerRuntime() {
    PlayerRuntimeState *const state = g_playerRuntimePointer;
    state->flags = 0;
    state->field08 = 0;
    state->currentDay = 0;
    state->model = 0;
    ResetPlayerRuntimeWords();
    ResetPendingPlayerPersistentState(0);
    ResetPartyCompanionTransition(0);
}
#endif

// @verified - d
void InitializePlayerRuntimeStorage() {
    g_playerRuntimePointer = &g_playerRuntime;
    g_playerRuntime.character = 0;
    g_playerRuntime.field14 = 0;
    *reinterpret_cast<std::uint32_t *>(&g_playerRuntime.savedHeldStateFlags) = 0;
    g_playerRuntime.model = 0;
    g_playerRuntime.flags = 0;
    g_playerRuntime.field04 = 0;
    g_playerRuntime.field08 = 0;
    g_playerRuntime.currentDay = 0;
}

// @verified - d
void InitializePlayerSystem() {
    InitializePlayerRuntimeStorage();
}

// @verified - d
void plymSetDefaultFlags() {
    g_playerRuntimePointer->flags &= ~1U;
    g_playerRuntimePointer->flags &= ~0x4000U;
    g_playerRuntimePointer->model = 0;
}

// @verified - d
std::uint32_t plymConsumeFlags8() {
    const std::uint32_t previous = static_cast<std::uint32_t>(g_playerRuntimePointer->field08);
    const std::uint32_t consumed = previous & 8U;
    g_playerRuntimePointer->field08 = static_cast<std::int32_t>(previous & ~8U);
    return consumed;
}

// @verified - d
std::uint32_t plymSetFlags(std::uint32_t flags) {
    const std::uint32_t previous = g_playerRuntimePointer->flags;
    g_playerRuntimePointer->flags = flags;
    return previous;
}

// @verified - d
std::uint32_t plymGetCurrentFlags() {
    return g_playerRuntimePointer->flags;
}

// @verified - d
std::uint8_t plymGetLookZoomScale() {
    return g_playerRuntimePointer->model->lookZoomScale;
}

// @verified - d
bool plymIsSearchStartRequested() {
    return (g_playerRuntimePointer->flags & 2U) != 0;
}

} // namespace s2
} // namespace shenmue

#endif
