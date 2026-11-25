#pragma once

#include "s2/map_event_task_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct PadControlEntry;
static const std::uint16_t kMapEventActionPressMask = 0x400;
bool ryoNotBusy();
bool ExecMapEventSceneChange(MapEventTaskParameters *parameters, std::int32_t region);
void SetCharacterLayer(Character *character, std::int32_t layer);
std::int32_t IsMapEventRegionEnabled(const MapEventComponentState *state, std::uint32_t region);
std::int32_t IsPlayerControlEnabled();
PadControlEntry *GetMapPadControlEntry();
bool CheckMapEventActionInput();

} // namespace s2
} // namespace shenmue

#endif
