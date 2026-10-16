#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_284E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_284E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_284E_ANC_{SHEN_NAME(ANC_)};
inline constexpr shen_name AREA_284E_IKO_{SHEN_NAME(IKO_)};
inline constexpr shen_name AREA_284E_MOK_{SHEN_NAME(MOK_)};
inline constexpr shen_name AREA_284E_RNT_{SHEN_NAME(RNT_)};
inline constexpr shen_name AREA_284E_SJN_{SHEN_NAME(SJN_)};
inline constexpr shen_name AREA_284E_KK1_{SHEN_NAME(KK1_)};
inline constexpr shen_name AREA_284E_KK5_{SHEN_NAME(KK5_)};
inline constexpr shen_name AREA_284E_KT9_{SHEN_NAME(KT9_)};
inline constexpr shen_name AREA_284E_KU6_{SHEN_NAME(KU6_)};
inline constexpr shen_name AREA_284E_KU7_{SHEN_NAME(KU7_)};
inline constexpr shen_name AREA_284E_P06_{SHEN_NAME(P06_)};
inline constexpr shen_name AREA_284E_P07_{SHEN_NAME(P07_)};
inline constexpr shen_name AREA_284E_VUN_{SHEN_NAME(VUN_)};
inline constexpr shen_name AREA_284E_CHARACTERS[]{AREA_284E_REN_, AREA_284E_ANC_, AREA_284E_IKO_, AREA_284E_MOK_, AREA_284E_RNT_, AREA_284E_SJN_,
    AREA_284E_KK1_, AREA_284E_KK5_, AREA_284E_KT9_, AREA_284E_KU6_, AREA_284E_KU7_, AREA_284E_P06_, AREA_284E_P07_, AREA_284E_VUN_,
    AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_284E_MOTION[] = "0284E.MOT", AREA_284E_CONVERSATION[] = "0284";
inline constexpr const char *AREA_284E_DEBUG_NAMES[]{
    "0284E000.c\n", "0284E010.c\n", "0284E020.c\n", "0284E030.c\n", "0284E040.c\n", "0284E042.c\n", "0284E045.c\n", "0284E050.c\n"};
inline constexpr Vector3 AREA_284E_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x4201999aU), .z = 0.0f};
void Setup284E();
void Initialize284E();
void Start284E(std::uintptr_t argument);
void Update284E(void *taskData);
void Cleanup284E(AuthorViewerTaskParameter *parent);
std::int64_t Queue284EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update284EAuthor(AuthorViewerLoopTaskParameter *child);
void Update284EAuthorTask(void *taskData);
void Prepare284ECharacters(std::uint32_t selection, float unusedNativeArgument);
void Restore284ECharacters();
extern const ScriptAreaDefinition area284EScriptDefinition;

} // namespace shenmue::s2::areas
