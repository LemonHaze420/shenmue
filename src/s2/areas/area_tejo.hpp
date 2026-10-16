#pragma once
#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/hlib.hpp"
#include "s2/ren_qte_event.hpp"
#include "s2/screen_film.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {
inline constexpr shen_name TEJO_MAIN_TASK{SHEN_NAME(main)}, TEJO_QTE_TASK{SHEN_NAME(tejo)};
inline constexpr shen_name TEJO_TRAP{SHEN_NAME(TRAP)}, TEJO_RATI{SHEN_NAME(RATI)}, TEJO_FIRST_QTE{SHEN_NAME(TEJ1)};
inline constexpr shen_name TEJO_FORCE{SHEN_NAME(FORC)}, TEJO_SECOND_QTE{SHEN_NAME(TEJ2)}, TEJO_HIDE{SHEN_NAME(HIDE)};
inline constexpr shen_name TEJO_ROOF{SHEN_NAME(ROOF)}, TEJO_YUAN{SHEN_NAME(YUAN)}, TEJO_DESTINATION{SHEN_NAME(QAXX)};
inline constexpr shen_name TEJO_SOY{SHEN_NAME(SOY_)}, TEJO_LIGHT_FIRST_QTE{SHEN_NAME(lt5F)}, TEJO_LIGHT_SECOND_QTE{SHEN_NAME(lt4F)};
inline constexpr shen_name TEJO_LIGHT_FIRST_END{SHEN_NAME(lgt2)}, TEJO_LIGHT_SECOND_END{SHEN_NAME(lgt1)};
inline constexpr shen_name TEJO_FIRST_END_AUTHOR{SHEN_NAME(AUT0)}, TEJO_SECOND_END_AUTHOR{SHEN_NAME(AUT1)};
inline constexpr shen_name TEJO_MAIN_CHARACTERS[]{REN_QTE_REN, REN_QTE_INVALID};
inline constexpr shen_name TEJO_QTE_CHARACTERS[]{REN_QTE_KD7, REN_QTE_KK1, REN_QTE_KT1, REN_QTE_KU7, TEJO_SOY, REN_QTE_INVALID};
inline constexpr Vector3 TEJO_FIRST_QTE_POSITION{0, 118, 0}, TEJO_SECOND_QTE_POSITION{0, 114.4000015258789F, 0};
inline constexpr Vector3 TEJO_SECOND_BASE_POSITION{};
inline constexpr Vector3i TEJO_SECOND_BASE_ROTATION{};
inline constexpr GlowParameters TEJO_GLOW{5, 0.3F, {0.3F, 0.3F, 0.3F, 0.17F}};
struct TejoDateCheckpoint {
    DateTime date;
    std::uint8_t reserved07;
    std::uint16_t reserved08;
};
static_assert(sizeof(TejoDateCheckpoint) == 10 && offsetof(TejoDateCheckpoint, reserved08) == 8);
extern HLib::Task *tejoMainTask;
extern HLib::Task *tejoQteTask;
extern shen_name tejoProgression;
extern std::int32_t tejoQteIndex, tejoWaitFlag, tejoExtendedRetry;
extern std::intptr_t tejoRetryMode;
extern RenQteWork tejoQteWork;
extern RenQteEntry *tejoQteEntries;
extern TejoDateCheckpoint tejoForceCheckpoint;
void TejoInit();
void TejoMain(std::uintptr_t argument);
void TejoProgressionCallback(void *);
void TejoWaitEventCallback(void *);
void QueueTejoQte();
void PrepareTejoQteResources();
void TejoQteCallback(void *);
void BeginTejoEndAuthor();
void TejoEndAuthorCallback(void *);
void HideTejoPursuers();
void CleanupTejoQte();
void PrepareTejoEvent();
void CleanupTejoEvent();
extern const ScriptAreaDefinition tejoScriptDefinition;
} // namespace shenmue::s2
