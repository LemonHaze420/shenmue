#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0339_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0339_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0339_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0339_a396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0339_CHARACTERS[]{AREA_0339_JOP_, AREA_0339_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0339_CONVERSATION[] = "0339";
inline constexpr shen_name AREA_0339_RETURN{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_0339_NPC_AREA{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0339_AUTHOR{SHEN_NAME(0339)};
void Initialize0339();
void Start0339(std::uintptr_t argument);
void Update0339(void *taskData);
void Cleanup0339(AuthorViewerTaskParameter *parent);
std::int64_t Queue0339Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0339Author(AuthorViewerLoopTaskParameter *child);
void Update0339AuthorTask(void *taskData);
void Prepare0339Characters();
void Restore0339Characters();
void Setup0339Scene();
extern const ScriptAreaDefinition area0339ScriptDefinition;

} // namespace shenmue::s2::areas
