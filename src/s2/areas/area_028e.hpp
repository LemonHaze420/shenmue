#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_028E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_028E_a040{SHEN_NAME(a040)};
inline constexpr shen_name AREA_028E_a097{SHEN_NAME(a097)};
inline constexpr shen_name AREA_028E_a098{SHEN_NAME(a098)};
inline constexpr shen_name AREA_028E_a099{SHEN_NAME(a099)};
inline constexpr shen_name AREA_028E_a135{SHEN_NAME(a135)};
inline constexpr shen_name AREA_028E_a133{SHEN_NAME(a133)};
inline constexpr shen_name AREA_028E_a136{SHEN_NAME(a136)};
inline constexpr shen_name AREA_028E_SOS_{SHEN_NAME(SOS_)};
inline constexpr shen_name AREA_028E_M01_{SHEN_NAME(M01_)};
inline constexpr shen_name AREA_028E_SHI_{SHEN_NAME(SHI_)};
inline constexpr shen_name AREA_028E_SYO_{SHEN_NAME(SYO_)};
inline constexpr shen_name AREA_028E_Y01_{SHEN_NAME(Y01_)};
inline constexpr shen_name AREA_028E_Y02_{SHEN_NAME(Y02_)};
inline constexpr shen_name AREA_028E_Y05_{SHEN_NAME(Y05_)};
inline constexpr shen_name AREA_028E_TAO_{SHEN_NAME(TAO_)};
inline constexpr shen_name AREA_028E_CHARACTERS[]{AREA_028E_SOS_, AREA_028E_M01_, AREA_028E_SHI_, AREA_028E_SYO_, AREA_028E_Y01_, AREA_028E_Y02_,
    AREA_028E_Y05_, AREA_028E_TAO_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_028E_MOTION[] = "0028E.MOT", AREA_028E_CONVERSATION[] = "0028";
inline constexpr const char *AREA_028E_DEBUG_NAMES[]{"0028E012.c\n", "0028E013.c\n", "0028E011.c\n"};
void Initialize028E();
void Start028E(std::uintptr_t argument);
void Update028E(void *taskData);
void Cleanup028E(AuthorViewerTaskParameter *parent);
std::int64_t Queue028EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update028EAuthor(AuthorViewerLoopTaskParameter *child);
void Update028EAuthorTask(void *taskData);
void Prepare028ECharacters();
void Restore028ECharacters();
extern const ScriptAreaDefinition area028EScriptDefinition;

} // namespace shenmue::s2::areas
