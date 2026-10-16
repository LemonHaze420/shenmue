#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0531_LIGHT{SHEN_NAME(l310)};
inline constexpr shen_name AREA_0531_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0531_a988{SHEN_NAME(a988)};
inline constexpr shen_name AREA_0531_a990{SHEN_NAME(a990)};
inline constexpr shen_name AREA_0531_a992{SHEN_NAME(a992)};
inline constexpr shen_name AREA_0531_a994{SHEN_NAME(a994)};
inline constexpr shen_name AREA_0531_aa51{SHEN_NAME(aa51)};
inline constexpr shen_name AREA_0531_aa52{SHEN_NAME(aa52)};
inline constexpr shen_name AREA_0531_aa53{SHEN_NAME(aa53)};
inline constexpr shen_name AREA_0531_aa54{SHEN_NAME(aa54)};
inline constexpr shen_name AREA_0531_aa55{SHEN_NAME(aa55)};
inline constexpr shen_name AREA_0531_aa56{SHEN_NAME(aa56)};
inline constexpr shen_name AREA_0531_aa57{SHEN_NAME(aa57)};
inline constexpr shen_name AREA_0531_aa58{SHEN_NAME(aa58)};
inline constexpr shen_name AREA_0531_aa59{SHEN_NAME(aa59)};
inline constexpr shen_name AREA_0531_aa60{SHEN_NAME(aa60)};
inline constexpr shen_name AREA_0531_aa61{SHEN_NAME(aa61)};
inline constexpr shen_name AREA_0531_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0531_CHARACTERS[]{AREA_0531_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0531_MOTION[] = "0531A.MOT", AREA_0531_CONVERSATION[] = "0531";
inline constexpr const char *AREA_0531_DEBUG_NAMES[]{"0531A000.c\n"};
void Initialize0531();
void Start0531(std::uintptr_t argument);
void Update0531(void *taskData);
void Cleanup0531(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0531Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0531Author(AuthorViewer519ELoopTaskParameter *child);
void Update0531AuthorTask(void *taskData);
void Prepare0531Characters();
void Restore0531Characters();
void Setup0531Fire();
extern const ScriptAreaDefinition area0531ScriptDefinition;

} // namespace shenmue::s2::areas
