#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0275_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0275_a334{SHEN_NAME(a334)};
inline constexpr shen_name AREA_0275_a372{SHEN_NAME(a372)};
inline constexpr shen_name AREA_0275_a532{SHEN_NAME(a532)};
inline constexpr shen_name AREA_0275_aa72{SHEN_NAME(aa72)};
inline constexpr shen_name AREA_0275_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0275_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0275_SGT_{SHEN_NAME(SGT_)};
inline constexpr shen_name AREA_0275_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0275_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0275_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0275_SRY_{SHEN_NAME(SRY_)};
inline constexpr shen_name AREA_0275_SOY_{SHEN_NAME(SOY_)};
inline constexpr shen_name AREA_0275_MRC_{SHEN_NAME(MRC_)};
inline constexpr shen_name AREA_0275_CHARACTERS[]{AREA_0275_SYE_, AREA_0275_JOP_, AREA_0275_SGT_, AREA_0275_REN_, AREA_0275_WON_, AREA_0275_TGY_,
    AREA_0275_SRY_, AREA_0275_SOY_, AREA_0275_MRC_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0275_MOTION[] = "0275A.MOT", AREA_0275_CONVERSATION[] = "0275";
inline constexpr shen_name AREA_0275_LIGHT{SHEN_NAME(l501)};
void Initialize0275();
void Start0275(std::uintptr_t argument);
void Update0275(void *taskData);
void Cleanup0275(AuthorViewerTaskParameter *parent);
std::int64_t Queue0275Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0275Author(AuthorViewerLoopTaskParameter *child);
void Update0275AuthorTask(void *taskData);
void Prepare0275Characters();
void Restore0275Characters();
extern const ScriptAreaDefinition area0275ScriptDefinition;

} // namespace shenmue::s2::areas
