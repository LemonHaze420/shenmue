#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

#include <array>
#include <cstddef>

namespace shenmue::s2::areas {

inline constexpr shen_name CRET_TASK{SHEN_NAME(CRET)};
inline constexpr shen_name CRET_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name CRET_WONG{SHEN_NAME(WON_)};
inline constexpr shen_name CRET_REN{SHEN_NAME(REN_)};
inline constexpr shen_name CRET_TIGER{SHEN_NAME(TGI_)};
inline constexpr shen_name CRET_LARRY{SHEN_NAME(LLY_)};
inline constexpr shen_name CRET_SAM{SHEN_NAME(SAM_)};
inline constexpr shen_name CRET_ABERDEEN{SHEN_NAME(AR02)};
inline constexpr shen_name CRET_KOWLOON{SHEN_NAME(QR00)};
inline constexpr shen_name CRET_WANCHAI{SHEN_NAME(AK00)};

inline std::array<std::byte, 12> cretDormantScratch0{}, cretDormantScratch1{};

void AreaCRETInit();
void CRETRetryCallback(void *);
void CRETResumeChaseCallback(void *);
void CRETDirectRetryCallback(void *);
void AreaCRETMain(std::uintptr_t mode);
extern const ScriptAreaDefinition areaCRETScriptDefinition;

} // namespace shenmue::s2::areas
