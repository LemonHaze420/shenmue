#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name KBRG_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name KBRG_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name KBRG_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name KBRG_SHENHUA{SHEN_NAME(SIN_)};
inline constexpr shen_name KBRG_INVALID{0xffffffffU};
inline constexpr shen_name KBRG_STORY_AUTHOR{SHEN_NAME(STR0)};
inline constexpr shen_name KBRG_QTE_AUTHORS[3]{SHEN_NAME(QTE0), SHEN_NAME(QTE1), SHEN_NAME(QTE2)};
void AreaKBRGInit();
void AreaKBRGMain(std::uintptr_t parameter);
void KBRGPrepareCharacter(shen_name character);
void KBRGRestoreGameplay();
void KBRGStoryCallback(void *data);
void KBRGQteCallback(void *data);
extern const ScriptAreaDefinition areaKBRGScriptDefinition;
} // namespace shenmue::s2::areas
