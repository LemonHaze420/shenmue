#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_048E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_048E_IKO_{SHEN_NAME(IKO_)};
inline constexpr shen_name AREA_048E_KIC_{SHEN_NAME(KIC_)};
inline constexpr shen_name AREA_048E_SBK_{SHEN_NAME(SBK_)};
inline constexpr shen_name AREA_048E_VCN_{SHEN_NAME(VCN_)};
inline constexpr shen_name AREA_048E_CHARACTERS[]{AREA_048E_IKO_, AREA_048E_KIC_, AREA_048E_SBK_, AREA_048E_VCN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_048E_MOTION[] = "0048E.MOT", AREA_048E_CONVERSATION[] = "0048";
inline constexpr const char *AREA_048E_DEBUG_NAMES[]{"0048E020.c\n"};
void Initialize048E();
void Start048E(std::uintptr_t argument);
void Update048E(void *taskData);
void Cleanup048E(AuthorViewerTaskParameter *parent);
std::int64_t Queue048EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update048EAuthor(AuthorViewerLoopTaskParameter *child);
void Update048EAuthorTask(void *taskData);
void Prepare048ECharacters();
void Restore048ECharacters();
extern const ScriptAreaDefinition area048EScriptDefinition;

} // namespace shenmue::s2::areas
