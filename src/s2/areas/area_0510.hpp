#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0510_LIGHT_FIRST{SHEN_NAME(l102)}, AREA_0510_LIGHT_SECOND{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0510_LIGHT_SEVENTH{SHEN_NAME(l041)};
inline constexpr shen_name AREA_0510_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0510_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0510_a246{SHEN_NAME(a246)};
inline constexpr shen_name AREA_0510_a349{SHEN_NAME(a349)};
inline constexpr shen_name AREA_0510_ab09{SHEN_NAME(ab09)};
inline constexpr shen_name AREA_0510_a726{SHEN_NAME(a726)};
inline constexpr shen_name AREA_0510_a728{SHEN_NAME(a728)};
inline constexpr shen_name AREA_0510_a345{SHEN_NAME(a345)};
inline constexpr shen_name AREA_0510_a904{SHEN_NAME(a904)};
inline constexpr shen_name AREA_0510_ab21{SHEN_NAME(ab21)};
inline constexpr shen_name AREA_0510_a932{SHEN_NAME(a932)};
inline constexpr shen_name AREA_0510_a906{SHEN_NAME(a906)};
inline constexpr shen_name AREA_0510_a908{SHEN_NAME(a908)};
inline constexpr shen_name AREA_0510_a918{SHEN_NAME(a918)};
inline constexpr shen_name AREA_0510_a920{SHEN_NAME(a920)};
inline constexpr shen_name AREA_0510_a912{SHEN_NAME(a912)};
inline constexpr shen_name AREA_0510_a914{SHEN_NAME(a914)};
inline constexpr shen_name AREA_0510_a916{SHEN_NAME(a916)};
inline constexpr shen_name AREA_0510_a902{SHEN_NAME(a902)};
inline constexpr shen_name AREA_0510_a900{SHEN_NAME(a900)};
inline constexpr shen_name AREA_0510_a910{SHEN_NAME(a910)};
inline constexpr shen_name AREA_0510_a938{SHEN_NAME(a938)};
inline constexpr shen_name AREA_0510_a940{SHEN_NAME(a940)};
inline constexpr shen_name AREA_0510_IWA_{SHEN_NAME(IWA_)};
inline constexpr shen_name AREA_0510_LRY_{SHEN_NAME(LRY_)};
inline constexpr shen_name AREA_0510_SN5_{SHEN_NAME(SN5_)};
inline constexpr shen_name AREA_0510_SNP_{SHEN_NAME(SNP_)};
inline constexpr shen_name AREA_0510_SNM_{SHEN_NAME(SNM_)};
inline constexpr shen_name AREA_0510_CHARACTERS[]{
    AREA_0510_IWA_, AREA_0510_LRY_, AREA_0510_SIN_, AREA_0510_SN5_, AREA_0510_SNP_, AREA_0510_SNM_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0510_MOTION[] = "0510A.MOT", AREA_0510_CONVERSATION[] = "0510";
inline constexpr const char *AREA_0510_DEBUG_NAMES[]{
    "0510A000.c\n", "0511A010.c\n", "0510A020.c\n", "0511A000.c\n", "0511A020.c\n", "0511A030.c\n", "0511A040.c\n", "0509A004.c\n"};
inline constexpr shen_name AREA_0510_MAIN_TASK{SHEN_NAME(051A)}, AREA_0510_LOOP_TASK{SHEN_NAME(051B)};
void Setup0510Environment();
void Select0510Package(std::int32_t selection);
void Create0510FallingLeaves(bool alternate);
void Create0510BreathEffects();
void Prepare0510Character(shen_name name);
void Restore0510Character(shen_name name);
void Restore0510Prop(shen_name name);
void Initialize0510();
void Start0510(std::uintptr_t argument);
void Update0510(void *taskData);
void Cleanup0510(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0510Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0510Author(AuthorViewer517ELoopTaskParameter *child);
void Update0510AuthorTask(void *taskData);
void Prepare0510Characters(std::uint32_t selection);
void Restore0510Characters(std::uint32_t selection);
extern const ScriptAreaDefinition area0510ScriptDefinition;

} // namespace shenmue::s2::areas
