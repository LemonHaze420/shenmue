#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_053E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_053E_HV6_{SHEN_NAME(HV6_)};
inline constexpr shen_name AREA_053E_HV7_{SHEN_NAME(HV7_)};
inline constexpr shen_name AREA_053E_HV8_{SHEN_NAME(HV8_)};
inline constexpr shen_name AREA_053E_HVA_{SHEN_NAME(HVA_)};
inline constexpr shen_name AREA_053E_KEI_{SHEN_NAME(KEI_)};
inline constexpr shen_name AREA_053E_TGI_{SHEN_NAME(TGI_)};
inline constexpr shen_name AREA_053E_CHARACTERS[]{
    AREA_053E_HV6_, AREA_053E_HV7_, AREA_053E_HV8_, AREA_053E_HVA_, AREA_053E_KEI_, AREA_053E_TGI_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_053E_MOTION[] = "0053E.MOT", AREA_053E_CONVERSATION[] = "0053";
inline constexpr const char *AREA_053E_DEBUG_NAMES[]{"0053E000.c\n"};
struct Area053ELoopParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area053ELoopParameter) == 0x10);
static_assert(offsetof(Area053ELoopParameter, currentFrame) == 8);
void Initialize053E();
void Start053E(std::uintptr_t argument);
void Update053E(void *taskData);
void Cleanup053E(AuthorViewerTaskParameter *parent);
std::int64_t Update053EAuthor(Area053ELoopParameter *child);
void Update053EAuthorTask(void *taskData);
void Prepare053ECharacters();
void Restore053ECharacters();
extern const ScriptAreaDefinition area053EScriptDefinition;

} // namespace shenmue::s2::areas
