#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0364_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0364_a452{SHEN_NAME(a452)};
inline constexpr shen_name AREA_0364_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0364_KT2_{SHEN_NAME(KT2_)};
inline constexpr shen_name AREA_0364_AD1_{SHEN_NAME(AD1_)};
inline constexpr shen_name AREA_0364_KTA_{SHEN_NAME(KTA_)};
inline constexpr shen_name AREA_0364_CHARACTERS[]{AREA_0364_REN_, AREA_0364_KT2_, AREA_0364_AD1_, AREA_0364_KTA_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0364_MOTION[] = "0364A.MOT", AREA_0364_CONVERSATION[] = "0364";
inline constexpr const char *AREA_0364_DEBUG_NAMES[]{"0364A000.c\n"};
inline constexpr Vector3 AREA_0364_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x4274cccdU), .z = 0.0f};
void Setup0364();
void Initialize0364();
void Start0364(std::uintptr_t argument);
void Update0364(void *taskData);
void Cleanup0364(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0364Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0364Author(AuthorViewer519ELoopTaskParameter *child);
void Update0364AuthorTask(void *taskData);
void Prepare0364Characters();
void Restore0364Characters();
extern const ScriptAreaDefinition area0364ScriptDefinition;

} // namespace shenmue::s2::areas
