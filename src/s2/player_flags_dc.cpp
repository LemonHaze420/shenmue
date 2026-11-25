#include "s2/player_flags_dc.hpp"
#include "s2/player_model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

// @verified - d
std::uint32_t plymAddFlags(std::uint32_t flags) {
    PlayerRuntimeState *const player = g_playerRuntimePointer;
    PlayerModelState *const model = player->model;
    const std::uint32_t previous = player->flags;
    player->flags |= flags;
    if (model)
        *reinterpret_cast<std::uint32_t *>(model) |= flags;
    return previous;
}

// @verified - d
std::uint32_t plymRemoveFlags(std::uint32_t flags) {
    PlayerRuntimeState *const player = g_playerRuntimePointer;
    PlayerModelState *const model = player->model;
    const std::uint32_t previous = player->flags;
    player->flags &= ~flags;
    if (model)
        *reinterpret_cast<std::uint32_t *>(model) &= ~flags;
    return previous;
}

std::int16_t plymGetAngY() {
    if (!(g_playerRuntimePointer->flags & 2))
        return g_playerRuntimePointer->model->currentYaw;
    return g_playerRuntimePointer->model->yaw;
}

} // namespace s2
} // namespace shenmue

#endif
