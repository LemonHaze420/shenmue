#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_194E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_194E_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_194E_SIS_{SHEN_NAME(SIS_)};
inline constexpr shen_name AREA_194E_EDL_{SHEN_NAME(EDL_)};
inline constexpr shen_name AREA_194E_aa76{SHEN_NAME(aa76)};
inline constexpr shen_name AREA_194E_a044{SHEN_NAME(a044)};
inline constexpr shen_name AREA_194E_a046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_194E_CHARACTERS[]{AREA_194E_KUN_, AREA_194E_SIS_, AREA_194E_EDL_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_194E_CONVERSATION[] = "0194";
inline constexpr const char *AREA_194E_DEBUG_NAMES[]{"0194E000.c\n", "0194E010.c\n", "0194E020.c\n", "0194E030.c\n"};
struct Area194EParameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area194EAuthorParameter {
    Area194EParameter *parent;
};
static_assert(sizeof(Area194EParameter) == 16);
static_assert(offsetof(Area194EParameter, selection) == 8 && offsetof(Area194EParameter, state) == 10);
static_assert(sizeof(Area194EAuthorParameter) == 8);
inline constexpr char AREA_194E_ARCHIVE[] = "0194E";
inline constexpr shen_name AREA_194E_AUTHORS[]{SHEN_NAME(194A), SHEN_NAME(194B), SHEN_NAME(194C), SHEN_NAME(194D)};
inline constexpr shen_name AREA_194E_NPC_AREA{SHEN_NAME(ANP_)};
inline constexpr shen_name AREA_194E_HIDE_MAP{SHEN_NAME(WTA0)};
inline constexpr shen_name AREA_194E_LIGHT_0001{SHEN_NAME(0001)};
inline constexpr shen_name AREA_194E_VARIANT_0[]{AREA_194E_KUN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr shen_name AREA_194E_VARIANT_1[]{AREA_194E_SIS_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr shen_name AREA_194E_VARIANT_2[]{AREA_194E_KUN_, AREA_194E_EDL_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr shen_name AREA_194E_VARIANT_3[]{AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr const shen_name *AREA_194E_VARIANTS[]{AREA_194E_VARIANT_0, AREA_194E_VARIANT_1, AREA_194E_VARIANT_2, AREA_194E_VARIANT_3};
void Select194EPackage(std::int32_t selection);
void Initialize194E();
void Start194E(std::uintptr_t argument);
void Update194E(void *taskData);
void Cleanup194E(Area194EParameter *parent);
std::int64_t Queue194EAuthor(Area194EParameter *parent);
std::int64_t Update194EAuthor(Area194EAuthorParameter *child);
void Update194EAuthorTask(void *taskData);
void Prepare194ECharacters(std::int16_t selection);
void Restore194ECharacters(std::int16_t selection);
extern const ScriptAreaDefinition area194EScriptDefinition;

} // namespace shenmue::s2::areas
