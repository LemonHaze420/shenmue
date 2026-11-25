#pragma once

#include "s2/map_models_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {
void SetMapModelSlotEnabled(std::int32_t enabled, std::int32_t index);
void SetMapFieldDepthSlotEnabled(std::int32_t enabled, std::int32_t index);
std::uint32_t GetMapFieldDepthSlotEnabled(std::int32_t index);
void SetAllMapFieldDepthSlotsEnabled(std::int32_t enabled);
std::uint32_t GetMapModelCommandRoutingEnabled(std::int32_t index);
void SetMapModelCommandRoutingEnabled(std::uint32_t enabled, std::int32_t index);
std::uint32_t ExchangeMapModelEnabled(std::int32_t index, std::uint32_t enabled);
void SetMapModelAlternateTraversalEnabled(std::uint32_t enabled, std::int32_t index);
void EV_SetMapModelAlternateTraversalEnabled(std::uint32_t enabled, std::int32_t index);
std::uint32_t GetMapModelGridCullingEnabled(std::int32_t index);
void SetMapModelGridCullingEnabled(std::uint32_t enabled, std::int32_t index);
std::int32_t GetMapModelReaderMask();
bool HasMapModelReader(std::int32_t index);
} // namespace s2
} // namespace shenmue

#endif
