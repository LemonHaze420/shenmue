#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0340_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0340_aa97{SHEN_NAME(aa97)};
inline constexpr shen_name AREA_0340_aa98{SHEN_NAME(aa98)};
inline constexpr shen_name AREA_0340_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0340_KRE_{SHEN_NAME(KRE_)};
inline constexpr shen_name AREA_0340_BF2_{SHEN_NAME(BF2_)};
inline constexpr shen_name AREA_0340_BF4_{SHEN_NAME(BF4_)};
inline constexpr shen_name AREA_0340_BF8_{SHEN_NAME(BF8_)};
inline constexpr shen_name AREA_0340_BFA_{SHEN_NAME(BFA_)};
inline constexpr shen_name AREA_0340_CHARACTERS[]{
    AREA_0340_JOP_, AREA_0340_KRE_, AREA_0340_BF2_, AREA_0340_BF4_, AREA_0340_BF8_, AREA_0340_BFA_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0340_MOTION[] = "0340A.MOT", AREA_0340_CONVERSATION[] = "0340";
inline constexpr const char *AREA_0340_DEBUG_NAMES[]{"0340A000.c\n", "0340A020.c\n"};
void Initialize0340();
void Start0340(std::uintptr_t argument);
void Update0340(void *taskData);
void Cleanup0340(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0340Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0340Author(AuthorViewer519ELoopTaskParameter *child);
void Update0340AuthorTask(void *taskData);
void Prepare0340Characters();
void Restore0340Characters();
extern const ScriptAreaDefinition area0340ScriptDefinition;

} // namespace shenmue::s2::areas
