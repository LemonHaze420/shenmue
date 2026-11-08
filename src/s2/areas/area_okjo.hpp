#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name OKJO_TASK{SHEN_NAME(okjo)};
inline constexpr shen_name OKJO_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name OKJO_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name OKJO_REN{SHEN_NAME(REN_)};
inline constexpr shen_name OKJO_INVALID{0xffffffffU};
inline constexpr shen_name OKJO_NEXT_DAY{SHEN_NAME(NDAY)};
inline constexpr shen_name OKJO_SCALED_PROP{SHEN_NAME(a413)};
inline constexpr shen_name OKJO_AUTHORS[4]{SHEN_NAME(AUT0), SHEN_NAME(AUT1), SHEN_NAME(AUT2), SHEN_NAME(AUT3)};
inline constexpr shen_name OKJO_LIGHTS[4]{SHEN_NAME(LGT0), SHEN_NAME(LGT1), SHEN_NAME(LGT2), SHEN_NAME(LGT3)};
void AreaOKJOInit();
void AreaOKJOMain(std::uintptr_t parameter);
void OKJOSetupCharacters();
void OKJOStartAuthor();
void OKJOReleaseScene();
void OKJOPlayAuthorCallback(void *data);
void OKJOWaitAuthorCallback(void *data);
void OKJOWaitDisplayCallback(void *data);
extern const ScriptAreaDefinition areaOKJOScriptDefinition;
} // namespace shenmue::s2::areas
