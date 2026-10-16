#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_188E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_188E_XGH_{SHEN_NAME(XGH_)};
inline constexpr shen_name AREA_188E_EDL_{SHEN_NAME(EDL_)};
inline constexpr shen_name AREA_188E_CHARACTERS[]{AREA_188E_EDL_, AREA_188E_XGH_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_188E_MOTION[] = "0188E.MOT", AREA_188E_CONVERSATION[] = "0188";
inline constexpr const char *AREA_188E_DEBUG_NAMES[]{"0188E010.c\n", "0188E020.c\n", "0187E010.c\n", "0187E020.c\n", "0188E030.c\n"};
inline constexpr char AREA_188E_FIRST_MOTION[] = "0187E.MOT", AREA_188E_FIRST_CONVERSATION[] = "0187";
inline constexpr char AREA_188E_FIRST_AUDIO[] = "0187E010", AREA_188E_SECOND_AUDIO[] = "0188E000";
struct Area188EParameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::uint32_t secondMotion;
    std::int16_t selection;
    std::int16_t state;
};
struct Area188ELoopParameter {
    Area188EParameter *parent;
};
static_assert(sizeof(Area188EParameter) == 0x18);
static_assert(offsetof(Area188EParameter, secondMotion) == 12);
static_assert(offsetof(Area188EParameter, selection) == 16);
static_assert(offsetof(Area188EParameter, state) == 18);
static_assert(sizeof(Area188ELoopParameter) == 8);
void Move188EAuthorCharacter(shen_name character, float unusedNativeArgument);
void Initialize188E();
void Start188E(std::uintptr_t argument);
void Update188E(void *taskData);
void Cleanup188E(Area188EParameter *parent);
std::int64_t Queue188EAuthor(Area188EParameter *parent);
std::int64_t Update188EAuthor(Area188ELoopParameter *child);
void Update188EAuthorTask(void *taskData);
void Restore188ECharacters();
extern const ScriptAreaDefinition area188EScriptDefinition;

} // namespace shenmue::s2::areas
