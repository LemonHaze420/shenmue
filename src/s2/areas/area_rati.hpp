#pragma once

#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name RATI_TASK{SHEN_NAME(rati)}, RATI_TITLE_TASK{SHEN_NAME(titl)};
inline constexpr shen_name RATI_RYO{SHEN_NAME(RYO_)}, RATI_REN{SHEN_NAME(REN_)}, RATI_CHR{SHEN_NAME(CHR_)};
inline constexpr shen_name RATI_INVALID{0xffffffffU};
inline constexpr shen_name RATI_AUT0{SHEN_NAME(AUT0)}, RATI_AUT1{SHEN_NAME(AUT1)}, RATI_AUT2{SHEN_NAME(AUT2)};
inline constexpr shen_name RATI_AUT3{SHEN_NAME(AUT3)}, RATI_AUT4{SHEN_NAME(AUT4)}, RATI_AUT5{SHEN_NAME(AUT5)};
inline constexpr shen_name RATI_AUT6{SHEN_NAME(AUT6)}, RATI_AUT7{SHEN_NAME(AUT7)};
inline constexpr shen_name RATI_TITLE_ONE_DAY{SHEN_NAME(2KAM)}, RATI_TITLE_TWO_DAYS{SHEN_NAME(3KAM)}, RATI_TITLE_OPENING{SHEN_NAME(QAEN)};
inline constexpr shen_name RATI_CHARACTERS[]{RATI_REN, RATI_CHR, RATI_INVALID};
inline constexpr shen_name RATI_SEQUENCES[]{RATI_AUT0, RATI_AUT4, RATI_INVALID, RATI_INVALID, RATI_INVALID, RATI_AUT1, RATI_AUT2, RATI_AUT4, RATI_AUT5,
    RATI_INVALID, RATI_AUT2, RATI_AUT4, RATI_AUT5, RATI_INVALID, RATI_INVALID, RATI_AUT3, RATI_AUT2, RATI_AUT7, RATI_INVALID, RATI_INVALID, RATI_AUT2,
    RATI_AUT7, RATI_INVALID, RATI_INVALID, RATI_INVALID};
inline constexpr Vector4 RATI_REFLECTOR{.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
extern HLib::Task *ratiTask;
extern std::int32_t ratiMode;
extern std::int32_t ratiQteFrames;
extern DateTime ratiStartDate;
extern std::int32_t ratiTitle;
extern std::int32_t ratiQteStarted;
extern const shen_name *ratiSequence;
void RatiInit();
void RatiMain(std::uintptr_t argument);
void PrepareRatiCharacters();
void RatiPlayImprisonmentCallback(void *taskData);
void RatiTitleCallback(void *taskData);
void RatiPlayAuthorCallback(void *taskData);
void RatiWaitQteCallback(void *taskData);
void RatiWaitAuthorCallback(void *taskData);
void RatiWaitFailureAuthorCallback(void *taskData);
HLib::Task *EndRatiEvent();
extern const ScriptAreaDefinition ratiScriptDefinition;

} // namespace shenmue::s2
