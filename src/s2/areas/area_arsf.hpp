#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr std::uint32_t ARSF_NIGHT_FLAG = 0xffU;
inline constexpr std::int32_t ARSF_NIGHT_OBJECT_INDEX = 3;
inline constexpr char ARSF_AUDIO_BANK[] = "ARSF_000";

void AreaARSFInit();
void AreaARSFMain(std::uint32_t startParameters);
void AreaARSFCleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void eQTB1_EVT_mainloop_Callback(void *taskData);
extern const GameplayAreaDefinition areaARSFGameplayDefinition;

} // namespace shenmue::s2::areas
