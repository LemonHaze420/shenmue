#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0517_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0517_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0517_a988{SHEN_NAME(a988)};
inline constexpr shen_name AREA_0517_a990{SHEN_NAME(a990)};
inline constexpr shen_name AREA_0517_a992{SHEN_NAME(a992)};
inline constexpr shen_name AREA_0517_a994{SHEN_NAME(a994)};
inline constexpr shen_name AREA_0517_a840{SHEN_NAME(a840)};
inline constexpr shen_name AREA_0517_a842{SHEN_NAME(a842)};
inline constexpr shen_name AREA_0517_a844{SHEN_NAME(a844)};
inline constexpr shen_name AREA_0517_a850{SHEN_NAME(a850)};
inline constexpr shen_name AREA_0517_a852{SHEN_NAME(a852)};
inline constexpr shen_name AREA_0517_a856{SHEN_NAME(a856)};
inline constexpr shen_name AREA_0517_CHARACTERS[]{AREA_0517_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0517_MOTION[] = "0517A.MOT", AREA_0517_CONVERSATION[] = "0517";
inline constexpr const char *AREA_0517_DEBUG_NAMES[]{"0517A000.c\n", "0517A011.c\n", "0517A012.c\n", "0517A014.c\n", "0517A015.c\n"};
inline constexpr shen_name AREA_0517_RETURN{SHEN_NAME(KWQA)};
inline constexpr shen_name AREA_0517_LIGHT3{SHEN_NAME(l141)}, AREA_0517_LIGHT4{SHEN_NAME(l151)};
inline constexpr char AREA_0517_WEATHER[] = "W_COND.BIN";
inline constexpr char AREA_0517_PACKAGE0[] = "KWW1", AREA_0517_PACKAGE1[] = "KWA1", AREA_0517_PACKAGE2[] = "KWA2";
struct Area0517LoopParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0517LoopParameter) == 16);
static_assert(offsetof(Area0517LoopParameter, currentFrame) == 8);
void Select0517Package(std::int32_t selection);
void Update0517Fireflies(void *taskData);
void Initialize0517();
void Start0517(std::uintptr_t argument);
void Update0517(void *taskData);
void Cleanup0517(AuthorViewerTaskParameter *parent);
std::int64_t Queue0517Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0517Author(Area0517LoopParameter *child);
void Update0517AuthorTask(void *taskData);
void Prepare0517Characters();
void Restore0517Characters();
extern const ScriptAreaDefinition area0517ScriptDefinition;

} // namespace shenmue::s2::areas
