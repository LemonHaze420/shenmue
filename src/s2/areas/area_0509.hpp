#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0509_LIGHT{SHEN_NAME(0002)};
inline constexpr shen_name AREA_0509_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0509_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0509_a540{SHEN_NAME(a540)};
inline constexpr shen_name AREA_0509_a436{SHEN_NAME(a436)};
inline constexpr shen_name AREA_0509_a528{SHEN_NAME(a528)};
inline constexpr shen_name AREA_0509_a526{SHEN_NAME(a526)};
inline constexpr shen_name AREA_0509_CHARACTERS[]{AREA_0509_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0509_MOTION[] = "0509A.MOT", AREA_0509_CONVERSATION[] = "0509";
inline constexpr const char *AREA_0509_DEBUG_NAMES[]{"0509A000.c\n", "0509A001.c\n", "0509A002.c\n", "0509A005.c\n", "0509A006.c\n"};
void Initialize0509();
void Start0509(std::uintptr_t argument);
void Update0509(void *taskData);
void Cleanup0509(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0509Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0509Author(AuthorViewer519ELoopTaskParameter *child);
void Update0509AuthorTask(void *taskData);
void Prepare0509Characters();
void Restore0509Characters();
void Setup0509Fire();
extern const ScriptAreaDefinition area0509ScriptDefinition;

} // namespace shenmue::s2::areas
