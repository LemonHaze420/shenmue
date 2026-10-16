#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_527E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_527E_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_527E_a980{SHEN_NAME(a980)};
inline constexpr shen_name AREA_527E_a984{SHEN_NAME(a984)};
inline constexpr shen_name AREA_527E_a982{SHEN_NAME(a982)};
inline constexpr shen_name AREA_527E_a986{SHEN_NAME(a986)};
inline constexpr shen_name AREA_527E_a016{SHEN_NAME(a016)};
inline constexpr shen_name AREA_527E_a778{SHEN_NAME(a778)};
inline constexpr shen_name AREA_527E_a784{SHEN_NAME(a784)};
inline constexpr shen_name AREA_527E_a786{SHEN_NAME(a786)};
inline constexpr shen_name AREA_527E_a788{SHEN_NAME(a788)};
inline constexpr shen_name AREA_527E_a790{SHEN_NAME(a790)};
inline constexpr shen_name AREA_527E_a792{SHEN_NAME(a792)};
inline constexpr shen_name AREA_527E_a794{SHEN_NAME(a794)};
inline constexpr shen_name AREA_527E_a738{SHEN_NAME(a738)};
inline constexpr shen_name AREA_527E_a740{SHEN_NAME(a740)};
inline constexpr shen_name AREA_527E_a742{SHEN_NAME(a742)};
inline constexpr shen_name AREA_527E_a744{SHEN_NAME(a744)};
inline constexpr shen_name AREA_527E_a746{SHEN_NAME(a746)};
inline constexpr shen_name AREA_527E_a748{SHEN_NAME(a748)};
inline constexpr shen_name AREA_527E_a750{SHEN_NAME(a750)};
inline constexpr shen_name AREA_527E_a752{SHEN_NAME(a752)};
inline constexpr shen_name AREA_527E_a754{SHEN_NAME(a754)};
inline constexpr shen_name AREA_527E_a756{SHEN_NAME(a756)};
inline constexpr shen_name AREA_527E_a758{SHEN_NAME(a758)};
inline constexpr shen_name AREA_527E_a760{SHEN_NAME(a760)};
inline constexpr shen_name AREA_527E_a762{SHEN_NAME(a762)};
inline constexpr shen_name AREA_527E_a764{SHEN_NAME(a764)};
inline constexpr shen_name AREA_527E_a766{SHEN_NAME(a766)};
inline constexpr shen_name AREA_527E_a768{SHEN_NAME(a768)};
inline constexpr shen_name AREA_527E_a770{SHEN_NAME(a770)};
inline constexpr shen_name AREA_527E_a772{SHEN_NAME(a772)};
inline constexpr shen_name AREA_527E_a774{SHEN_NAME(a774)};
inline constexpr shen_name AREA_527E_a776{SHEN_NAME(a776)};
inline constexpr shen_name AREA_527E_a780{SHEN_NAME(a780)};
inline constexpr shen_name AREA_527E_a782{SHEN_NAME(a782)};
inline constexpr shen_name AREA_527E_a736{SHEN_NAME(a736)};
inline constexpr shen_name AREA_527E_CHARACTERS[]{AREA_527E_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_527E_MOTION[] = "0527E.MOT", AREA_527E_CONVERSATION[] = "0527";
inline constexpr const char *AREA_527E_DEBUG_NAMES[]{"0527E370.c\n", "0527E380.c\n", "0527E310.c\n", "0527E320.c\n", "0527E440.c\n", "0527E330.c\n",
    "0527E350.c\n", "0527E360.c\n", "0527E340.c\n", "0527E410.c\n", "0527E420.c\n", "0527E411.c\n", "0527E412.c\n", "0527E020.c\n", "0527E311.c\n",
    "0527E312.c\n", "0527E000.c\n", "0527E010.c\n", "0527E325.c\n", "0527E326.c\n", "0527E300.c\n", "0527E210.c\n", "0527E435.c\n", "0527E430.c\n",
    "0527E431.c\n", "0527E432.c\n", "0527E365.c\n", "0527E400.c\n", "0527E425.c\n", "0527E426.c\n", "0527E460.c\n", "0527E441.c\n", "0527E001.c\n"};
inline constexpr shen_name AREA_527E_PROPS[]{AREA_527E_a778, AREA_527E_a784, AREA_527E_a786, AREA_527E_a788, AREA_527E_a790, AREA_527E_a792, AREA_527E_a794,
    AREA_527E_a738, AREA_527E_a740, AREA_527E_a742, AREA_527E_a744, AREA_527E_a746, AREA_527E_a748, AREA_527E_a750, AREA_527E_a752, AREA_527E_a754,
    AREA_527E_a756, AREA_527E_a758, AREA_527E_a760, AREA_527E_a762, AREA_527E_a764, AREA_527E_a766, AREA_527E_a768, AREA_527E_a770, AREA_527E_a772,
    AREA_527E_a774, AREA_527E_a776, AREA_527E_a780, AREA_527E_a782, AREA_527E_a736, AUTHOR_VIEWER_INVALID_CHARACTER};
static_assert(sizeof(AREA_527E_PROPS) == 124);
void Initialize527E();
void Start527E(std::uintptr_t argument);
void Update527E(void *taskData);
void Cleanup527E(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue527EAuthor(AuthorViewerPairTaskParameter *parent);
std::int64_t Update527EAuthor(AuthorViewer519ELoopTaskParameter *child);
void Update527EAuthorTask(void *taskData);
void Prepare527ECharacters();
void Restore527ECharacters();
extern const ScriptAreaDefinition area527EScriptDefinition;

} // namespace shenmue::s2::areas
