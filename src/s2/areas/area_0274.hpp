#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0274_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0274_a334{SHEN_NAME(a334)};
inline constexpr shen_name AREA_0274_a372{SHEN_NAME(a372)};
inline constexpr shen_name AREA_0274_a532{SHEN_NAME(a532)};
inline constexpr shen_name AREA_0274_aa72{SHEN_NAME(aa72)};
inline constexpr shen_name AREA_0274_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0274_SRY_{SHEN_NAME(SRY_)};
inline constexpr shen_name AREA_0274_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0274_SGT_{SHEN_NAME(SGT_)};
inline constexpr shen_name AREA_0274_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0274_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0274_ESY_{SHEN_NAME(ESY_)};
inline constexpr shen_name AREA_0274_BF8_{SHEN_NAME(BF8_)};
inline constexpr shen_name AREA_0274_BF7_{SHEN_NAME(BF7_)};
inline constexpr shen_name AREA_0274_BF1_{SHEN_NAME(BF1_)};
inline constexpr shen_name AREA_0274_BF9_{SHEN_NAME(BF9_)};
inline constexpr shen_name AREA_0274_KF5_{SHEN_NAME(KF5_)};
inline constexpr shen_name AREA_0274_CHARACTERS[]{AREA_0274_REN_, AREA_0274_SRY_, AREA_0274_JOP_, AREA_0274_SGT_, AREA_0274_TGY_, AREA_0274_WON_,
    AREA_0274_ESY_, AREA_0274_BF8_, AREA_0274_BF7_, AREA_0274_BF1_, AREA_0274_BF9_, AREA_0274_KF5_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0274_MOTION[] = "0274A.MOT", AREA_0274_CONVERSATION[] = "0274";
void Setup0274();
void Initialize0274();
void Start0274(std::uintptr_t argument);
void Update0274(void *taskData);
void Cleanup0274(AuthorViewerTaskParameter *parent);
std::int64_t Queue0274Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0274Author(AuthorViewerLoopTaskParameter *child);
void Update0274AuthorTask(void *taskData);
void Prepare0274Characters();
void Restore0274Characters();
extern const ScriptAreaDefinition area0274ScriptDefinition;

} // namespace shenmue::s2::areas
