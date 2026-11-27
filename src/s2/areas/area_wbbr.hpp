#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/sap_event.hpp"
#define WBBR_TASK SAP_TASK
#define WBBR_AUTHOR SHEN_NAME4('A', 'U', 'T', '0')
void SetWbbrCharacterTransform(shen_name character, const Vector3 *position, int yaw);
void SetWbbrCharacterPosition(shen_name character, float x, float y, float z, int yaw);
void WbbrInitializeAuthorCallback(void *taskData);
void WbbrSelectAuthorCallback(void *taskData);
void WbbrCompleteAuthorCallback(void *taskData);
#else
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

#endif
