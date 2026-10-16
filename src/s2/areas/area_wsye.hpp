#pragma once
#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name WSYE_TASK{SHEN_NAME(inn2)}, WSYE_AUTHOR_FIRST{SHEN_NAME(AUT0)};
inline constexpr shen_name WSYE_AUTHOR_SECOND{SHEN_NAME(AUT1)}, WSYE_DOOR{SHEN_NAME(R121)};
inline constexpr shen_name WSYE_PROP_FIRST{SHEN_NAME(M801)}, WSYE_PROP_SECOND{SHEN_NAME(M902)}, WSYE_PLAYER{SHEN_NAME(RYO_)};
extern std::int32_t wsyeSelection;
void WsyeInit();
void WsyeMain(std::uintptr_t argument);
void WsyeInitializeAuthorCallback(void *);
void WsyeStartFirstAuthorCallback(void *);
void WsyeStartSecondAuthorCallback(void *);
void WsyeCompleteAuthorCallback(void *);
void PlaceWsyePlayer(const Vector3 &position);
extern const ScriptAreaDefinition wsyeScriptDefinition;
} // namespace shenmue::s2
