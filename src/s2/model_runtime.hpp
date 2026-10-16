#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

std::uint32_t GetModelBodyKind(const void *model);
bool UpdateModelFrame(MT7Model *model, Character *character, std::int32_t mode, std::uint32_t flags, const void *placement = nullptr);
void SetModelPlacementVisible(const void *placement, std::int32_t visible);
void SetModelPlacementChildVisible(const void *placement, std::int32_t kind, std::uint32_t visible);
void SetModelPlacementChildTransform(const void *placement, std::int32_t kind, const Matrix4x4 *transform);

} // namespace shenmue::s2
