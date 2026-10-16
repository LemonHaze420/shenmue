#pragma once

#include "s2/author_viewer_event.hpp"

namespace shenmue::s2 {

inline constexpr shen_name AUTHOR_VIEWER_517E_AREA{SHEN_NAME(517E)}, AUTHOR_VIEWER_519E_AREA{SHEN_NAME(519E)};
inline constexpr shen_name AUTHOR_VIEWER_SIN{SHEN_NAME(SIN_)}, AUTHOR_VIEWER_517E_LIGHT{SHEN_NAME(0001)};
inline constexpr shen_name AUTHOR_VIEWER_517E_CHARACTERS[]{AUTHOR_VIEWER_SIN, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr shen_name AUTHOR_VIEWER_519E_CHARACTERS[]{AUTHOR_VIEWER_SIN, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AUTHOR_VIEWER_517E_MOTION[] = "0507A.MOT", AUTHOR_VIEWER_519E_MOTION[] = "0519E.MOT";
inline constexpr char AUTHOR_VIEWER_517E_CONVERSATION[] = "0517", AUTHOR_VIEWER_519E_CONVERSATION[] = "0519";
inline constexpr char AUTHOR_VIEWER_517E_DEBUG_NAME[] = "0517E020.c\n";
inline constexpr const char *AUTHOR_VIEWER_519E_DEBUG_NAMES[]{"0519E005.c\n", "0519E020.c\n", "0519E045.c\n", "0519E060.c\n"};
inline constexpr char AUTHOR_VIEWER_519E_FIRST_PACKAGE[] = "KRK2", AUTHOR_VIEWER_519E_SECOND_PACKAGE[] = "KRK4";
// UpdateSceneLightingParams | SetGlobalLighting | ApplyFcas | UpdateSunVector.
inline constexpr std::int32_t AUTHOR_VIEWER_517E_WEATHER_LIGHTING_MASK = 8 | 4 | 2 | 1;
inline constexpr std::uint64_t AUTHOR_VIEWER_PACKAGE_LOAD = 0, AUTHOR_VIEWER_PACKAGE_IS_LOADED = 5;
inline constexpr std::uint64_t AUTHOR_VIEWER_PACKAGE_FREE_IF_LOADED = 6, AUTHOR_VIEWER_PACKAGE_WAIT = 7;


struct AuthorViewerPairTaskParameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::uint32_t selection;
    std::int16_t state;
};
struct AuthorViewer517ELoopTaskParameter {
    AuthorViewerPairTaskParameter *parent;
    std::uint16_t currentFrame;
};
struct AuthorViewer519ELoopTaskParameter {
    AuthorViewerPairTaskParameter *parent;
};
static_assert(sizeof(AuthorViewerPairTaskParameter) == 0x18);
static_assert(alignof(AuthorViewerPairTaskParameter) == 8);
static_assert(offsetof(AuthorViewerPairTaskParameter, self) == 0x00);
static_assert(offsetof(AuthorViewerPairTaskParameter, motion) == 0x08);
static_assert(offsetof(AuthorViewerPairTaskParameter, selection) == 0x0c);
static_assert(offsetof(AuthorViewerPairTaskParameter, state) == 0x10);
static_assert(sizeof(AuthorViewer517ELoopTaskParameter) == 0x10);
static_assert(alignof(AuthorViewer517ELoopTaskParameter) == 8);
static_assert(offsetof(AuthorViewer517ELoopTaskParameter, parent) == 0x00);
static_assert(offsetof(AuthorViewer517ELoopTaskParameter, currentFrame) == 0x08);
static_assert(sizeof(AuthorViewer519ELoopTaskParameter) == 0x08);
static_assert(alignof(AuthorViewer519ELoopTaskParameter) == 8);
static_assert(offsetof(AuthorViewer519ELoopTaskParameter, parent) == 0x00);

void AuthorViewer517EInit();
void AuthorViewer517EMain(std::uintptr_t argument);
void AuthorViewer517EMainCallback(void *taskData);
void SetupAuthorViewer517E();
void CleanupAuthorViewer517E(AuthorViewerPairTaskParameter *parent);
std::int64_t QueueAuthorViewer517ELoop(AuthorViewerPairTaskParameter *parent);
std::int64_t AuthorViewer517ELoopCallback(void *taskData);
void AuthorViewer517ELoopTaskCallback(void *taskData);
void AuthorViewer519EInit();
void AuthorViewer519EMain(std::uintptr_t argument);
void AuthorViewer519EMainCallback(void *taskData);
std::int64_t SelectAuthorViewer519EPackage(std::int32_t selection);
void CleanupAuthorViewer519E(AuthorViewerPairTaskParameter *parent);
std::int64_t QueueAuthorViewer519ELoop(AuthorViewerPairTaskParameter *parent);
std::int64_t AuthorViewer519ELoopCallback(void *taskData);
void AuthorViewer519ELoopTaskCallback(void *taskData);
extern const ScriptAreaDefinition authorViewer517EScriptDefinition;
extern const ScriptAreaDefinition authorViewer519EScriptDefinition;

} // namespace shenmue::s2
