#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_smoke.hpp"
#include "s2/areas/area_loop_dc.h"

#define QDTG_AREA SHEN_NAME4('Q', 'D', 'T', 'G')
#define QDTG_RETURN_AREA SHEN_NAME4('Q', 'F', '0', '0')
#define QDTG_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define QDTG_REN SHEN_NAME4('R', 'E', 'N', '_')
#define QDTG_TGY SHEN_NAME4('T', 'G', 'Y', '_')
#define QDTG_SOY SHEN_NAME4('S', 'O', 'Y', '_')
#define QDTG_SEE SHEN_NAME4('S', 'E', 'E', '_')
#define QDTG_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define QDTG_YUKA SHEN_NAME4('Y', 'U', 'K', 'A')
#define QDTG_YUKB SHEN_NAME4('Y', 'U', 'K', 'B')
#define QDTG_HIBI SHEN_NAME4('H', 'I', 'B', 'I')
#define QDTG_A896 SHEN_NAME4('a', '8', '9', '6')
#define QDTG_BOX0 SHEN_NAME4('B', 'O', 'X', '0')
#define QDTG_DUMR SHEN_NAME4('D', 'U', 'M', 'R')
#define QDTG_DUML SHEN_NAME4('D', 'U', 'M', 'L')
#define QDTG_DUMT SHEN_NAME4('D', 'U', 'M', 'T')
#define QDTG_ATH0 SHEN_NAME4('A', 'T', 'H', '0')
#define QDTG_QTES SHEN_NAME4('Q', 'T', 'E', 'S')
#define QDTG_QTEF SHEN_NAME4('Q', 'T', 'E', 'F')
#define QDTG_TOGS SHEN_NAME4('T', 'O', 'G', 'S')
#define QDTG_TOGF SHEN_NAME4('T', 'O', 'G', 'F')
#define QDTG_TOG0 SHEN_NAME4('T', 'O', 'G', '0')
#define QDTG_WIN0 SHEN_NAME4('W', 'I', 'N', '0')
#define QDTG_RETRY_TITLE SHEN_NAME4('R', 'T', 'R', '2')
#define QDTG_DEBUG_ID 3
#define QDTG_STEP_FLAG 351
#define QDTG_HAND_POSE 40
#define QDTG_REN_RUN 8778
#define QDTG_RYO_RUN 8792
#define QDTG_TOGYU_STUCK 8830
#define QDTG_CRUSH 8832
#define QDTG_ARM_D 8833
#define QDTG_ARM_E 8834
#define QDTG_COIN_TOSS 8835
#define QDTG_SEE_STANCE 3632
typedef struct QdtgEventWork {
    EventReadCharactersWork *characters;
    HLibTask *debugTask;
    HLibTask *task;
    void *motion;
    int state;
    int savedNavi;
    HLibTaskCallback continuation;
    const int *entry;
} QdtgEventWork;
extern shen_name qdtgCharacters[];
extern int qdtgSequenceState;
void QdtgMain(void *argument);
void QueueQdtgEvent(const int *entry);
void QdtgDebugExitCallback(void *data);
void FinishTogyuCallback(void *data);
void QdtgDebugDisplayCallback(void *data);
void SetTogyuStartContinuation(HLibTaskCallback *continuation);
void StartTogyuCallback(void *data);
void AuthorWaitCallback(void *data);
void RunRyoRenCallback(void *data);
void Qte2Callback(void *data);
void Qte0Callback(void *data);
void Qte0ResultCallback(void *data);
void ComQteCallback(void *data);
void ComQteResultCallback(void *data);
void StartFloorBattleCallback(void *data);
void FloorBattleCallback(void *data);
void WinAuthorCallback(void *data);
void Author1Callback(void *data);
void Author2Callback(void *data);
void RetryTogyuCallback(void *data);
void FinishAuthorThunk(void *data);
void InitializeTogyu(QdtgEventWork *work);
void CleanupTogyu(QdtgEventWork *work);
int DoRyoRenSequence(int delay);
int Qte2MotionSuccess(int delay);
int Qte2MotionFailure(int delay);
int Qte0MotionSuccess(int delay);
int Qte0MotionFailure(int delay);
void ResetTogyuObject(shen_name name);
int QueueTogyuObjectCollapse(shen_name name, shen_name hidden, int delay);
int QueueHibiDisplay(shen_name character, shen_name hidden, int delay);
int QueueFloorCollapse(shen_name character, shen_name hidden, int delay);
int QueueBoxCollapse(shen_name character, shen_name hidden, int delay);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
inline constexpr shen_name QDTG_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name QDTG_RYO{SHEN_NAME(RYO_)}, QDTG_REN{SHEN_NAME(REN_)}, QDTG_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name QDTG_SOY{SHEN_NAME(SOY_)}, QDTG_SEE{SHEN_NAME(SEE_)}, QDTG_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name QDTG_YUKA{SHEN_NAME(YUKA)}, QDTG_YUKB{SHEN_NAME(YUKB)}, QDTG_HIBI{SHEN_NAME(HIBI)};
inline constexpr shen_name QDTG_A896{SHEN_NAME(a896)}, QDTG_BOX0{SHEN_NAME(BOX0)};
inline constexpr shen_name QDTG_DUMR{SHEN_NAME(DUMR)}, QDTG_DUML{SHEN_NAME(DUML)}, QDTG_DUMT{SHEN_NAME(DUMT)};
inline constexpr shen_name QDTG_ATH0{SHEN_NAME(ATH0)}, QDTG_QTES{SHEN_NAME(QTES)}, QDTG_QTEF{SHEN_NAME(QTEF)};
inline constexpr shen_name QDTG_TOGS{SHEN_NAME(TOGS)}, QDTG_TOGF{SHEN_NAME(TOGF)}, QDTG_TOG0{SHEN_NAME(TOG0)};
inline constexpr shen_name QDTG_WIN0{SHEN_NAME(WIN0)}, QDTG_RETRY_TITLE{SHEN_NAME(RTR2)}, QDTG_RETURN_AREA{SHEN_NAME(QF00)};
inline constexpr shen_name QDTG_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name QDTG_CHARACTERS[]{QDTG_RYO, QDTG_REN, QDTG_TGY, QDTG_SOY, QDTG_SEE, EVENT_CHARACTER_INVALID};
inline constexpr std::int32_t QDTG_BOX_PARTS[]{161, 160, 159, 158, 157, 156, 155, 154, 153, 152};
inline constexpr Vector3 QDTG_BASE_POSITION{199, 154, 157.5f};
inline constexpr Vector3 QDTG_YUKA_POSITION{198.9100037f, 154, 169.1600037f};
inline constexpr Vector3 QDTG_YUKB_POSITION{198.9100037f, 153, 169.1600037f};
inline constexpr Vector3 QDTG_HIBI_POSITION{198.9100037f, 154.0019989f, 169.1999969f};
inline constexpr std::uint32_t QDTG_REN_RUN = 8778, QDTG_RYO_RUN = 8792, QDTG_ARM_D = 8833, QDTG_ARM_E = 8834;
inline constexpr std::uint32_t QDTG_CRUSH = 8832, QDTG_COIN_TOSS = 8835, QDTG_TOGYU_STUCK = 8830, QDTG_SEE_STANCE = 3632;
struct QdtgEventWork {
    EventReadCharactersWork *characters;
    std::uint64_t unused08;
    HLib::Task *task;
    std::uint32_t motion;
    std::int32_t state;
    std::uint32_t savedNavi, reserved24;
    HLib::TaskCallback continuation;
    const std::int32_t *entry;
    std::int32_t retrySkip, authorState, loss;
    std::uint32_t reserved44;
};
static_assert(sizeof(QdtgEventWork) == 0x48);
static_assert(offsetof(QdtgEventWork, task) == 0x10);
static_assert(offsetof(QdtgEventWork, savedNavi) == 0x20);
static_assert(offsetof(QdtgEventWork, continuation) == 0x28);
static_assert(offsetof(QdtgEventWork, entry) == 0x30);
static_assert(offsetof(QdtgEventWork, authorState) == 0x3c);
void QdtgInit();
void QdtgMain(std::uintptr_t argument);
extern const ScriptAreaDefinition qdtgScriptDefinition;
} // namespace shenmue::s2
#endif
