#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_302E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_302E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_302E_CHARACTERS[]{AREA_302E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_302E_MOTION[] = "0302E.MOT", AREA_302E_CONVERSATION[] = "0302";
inline constexpr const char *AREA_302E_DEBUG_NAMES[]{"0302E010.c\n", "0302E020.c\n", "0302E030.c\n"};
inline constexpr Vector3 AREA_302E_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x41e66666U), .z = 0.0f};
void Setup302E();
void Initialize302E();
void Start302E(std::uintptr_t argument);
void Update302E(void *taskData);
void Cleanup302E(AuthorViewerTaskParameter *parent);
std::int64_t Queue302EAuthor(AuthorViewerTaskParameter *parent);
extern const ScriptAreaDefinition area302EScriptDefinition;

} // namespace shenmue::s2::areas
