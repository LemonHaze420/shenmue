#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

#include <array>
#include <cstddef>

namespace shenmue::s2::areas {

inline constexpr shen_name CRTI_TASK{SHEN_NAME(CRET)};
inline constexpr shen_name CRTI_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name CRTI_WONG{SHEN_NAME(WON_)};
inline constexpr shen_name CRTI_REN{SHEN_NAME(REN_)};
inline constexpr shen_name CRTI_TIGER{SHEN_NAME(TGI_)};
inline constexpr shen_name CRTI_LARRY{SHEN_NAME(LLY_)};
inline constexpr shen_name CRTI_SAM{SHEN_NAME(SAM_)};
inline constexpr shen_name CRTI_ABERDEEN{SHEN_NAME(AR02)};
inline constexpr shen_name CRTI_KOWLOON{SHEN_NAME(QR00)};
inline constexpr shen_name CRTI_WANCHAI{SHEN_NAME(AK00)};

inline std::array<std::byte, 12> crtiDormantScratch0{}, crtiDormantScratch1{};

void AreaCRTIInit();
void CRTIRetryCallback(void *);
void CRTIResumeChaseCallback(void *);
void AreaCRTIMain(std::uintptr_t mode);
extern const ScriptAreaDefinition areaCRTIScriptDefinition;

} // namespace shenmue::s2::areas
