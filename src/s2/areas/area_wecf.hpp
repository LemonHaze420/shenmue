#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name CHARACTER_WECF_JUKEBOX{SHEN_NAME(JBOX)};

void AreaWECFInit();
void AreaWECFMainLoop(void *taskData);
void JukeboxInitCallback(void *taskData);
void JukeboxRevolveCallback(void *taskData);
void AreaWECFMain(std::uint32_t startParameters);
void AreaWECFCleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWECFGameplayDefinition;

} // namespace shenmue::s2::areas
