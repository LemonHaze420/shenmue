#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name WBBR_TASK{SHEN_NAME(sap_)}, WBBR_AUTHOR{SHEN_NAME(AUT0)};
extern shen_name wbbrCharacters[1];
void WbbrInit();
void WbbrMain(std::uintptr_t argument);
void WbbrInitializeAuthorCallback(void *);
void WbbrSelectAuthorCallback(void *);
void StartWbbrAuthor(shen_name author);
void WbbrCompleteAuthorCallback(void *);
extern const ScriptAreaDefinition wbbrScriptDefinition;
} // namespace shenmue::s2
