#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/et_tejo_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_utility_dc.h"

#define TEJO_MAIN_TASK SHEN_NAME4('m', 'a', 'i', 'n')
#define TEJO_QTE_TASK SHEN_NAME4('t', 'e', 'j', 'o')
#define TEJO_TRAP SHEN_NAME4('T', 'R', 'A', 'P')
#define TEJO_RATI SHEN_NAME4('R', 'A', 'T', 'I')
#define TEJO_FIRST_QTE SHEN_NAME4('T', 'E', 'J', '1')
#define TEJO_FORCE SHEN_NAME4('F', 'O', 'R', 'C')
#define TEJO_SECOND_QTE SHEN_NAME4('T', 'E', 'J', '2')
#define TEJO_HIDE SHEN_NAME4('H', 'I', 'D', 'E')
#define TEJO_ROOF SHEN_NAME4('R', 'O', 'O', 'F')
#define TEJO_YUAN SHEN_NAME4('Y', 'U', 'A', 'N')
#define TEJO_WAIT SHEN_NAME4('W', 'A', 'I', 'T')
#define TEJO_DESTINATION SHEN_NAME4('Q', 'A', 'X', 'X')
#define TEJO_SOY SHEN_NAME4('S', 'O', 'Y', '_')
#define TEJO_LIGHT_FIRST_QTE SHEN_NAME4('l', 't', '5', 'F')
#define TEJO_LIGHT_SECOND_QTE SHEN_NAME4('l', 't', '4', 'F')
#define TEJO_LIGHT_FIRST_END SHEN_NAME4('l', 'g', 't', '2')
#define TEJO_LIGHT_SECOND_END SHEN_NAME4('l', 'g', 't', '1')
#define TEJO_FIRST_END_AUTHOR SHEN_NAME4('A', 'U', 'T', '0')
#define TEJO_SECOND_END_AUTHOR SHEN_NAME4('A', 'U', 'T', '1')
#define TEJO_DEBUG_ID 2
extern HLibTask *tejoMainTask, *tejoQteTask;
extern int tejoProgression;
extern int tejoQteIndex, tejoWaitFlag, tejoRetryMode, tejoExtendedRetry;
extern EventDateTime tejoForceCheckpoint;
extern RenQteWork tejoQteWork;
extern RenQteEntry *tejoQteEntries;
extern RenQteEntry tejoLowerQteEntries[], tejoUpperQteEntries[];
extern RenQteCamera tejoDoorFirstRetryCameras[], tejoDoorSecondRetryCameras[], tejoDoorThirdRetryCameras[];
extern RenQteVoice tejoDoorSecondRetrySuccessVoices[], tejoDoorFirstRetryFailureVoices[], tejoDoorSecondRetryFailureVoices[];
extern RenQteVoice tejoDoorThirdRetryFailureVoices[];
extern shen_name tejoMainCharacters[], tejoQteCharacters[];
extern Vector3 tejoSecondBasePosition;
extern Vector3i tejoSecondBaseRotation;
void TejoMain(void);
void TejoProgressionCallback(void *data);
void TejoWaitEventCallback(void *data);
void QueueTejoQte(void);
void PrepareTejoQteResources(void);
void TejoQteCallback(void *data);
void BeginTejoEndAuthor(void);
void TejoEndAuthorCallback(void *data);
void HideTejoPursuers(void);
void CleanupTejoQte(void);
void PrepareTejoEvent(void);
void CleanupTejoEvent(void);
void TejoDebugExitCallback(void *data);
#else
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
#endif
