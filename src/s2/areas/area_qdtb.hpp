#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/areas/common/et_thr.h"

#define QDTB_TASK TASK_EVNT
#define QDTB_SEQUENCE SHEN_NAME4('S', 'E', 'Q', 'P')
#define QDTB_BATTLE SHEN_NAME4('B', 'S', 'B', 'T')
#define QDTB_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define QDTB_RETURN_AREA SHEN_NAME4('0', '2', '5', '0')
#define QDTB_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define QDTB_TGY SHEN_NAME4('T', 'G', 'Y', '_')
#define QDTB_YOH SHEN_NAME4('Y', 'O', 'H', '_')
#define QDTB_RKN SHEN_NAME4('R', 'K', 'N', '_')
#define QDTB_III SHEN_NAME4('I', 'I', 'I', '_')
#define QDTB_SEE SHEN_NAME4('S', 'E', 'E', '_')
#define QDTB_SYE SHEN_NAME4('S', 'Y', 'E', '_')
#define QDTB_TENT SHEN_NAME4('T', 'E', 'N', 'T')
#define QDTB_A235 SHEN_NAME4('a', '2', '3', '5')
#define QDTB_A352 SHEN_NAME4('a', '3', '5', '2')
#define QDTB_A397 SHEN_NAME4('a', '3', '9', '7')
#define QDTB_A398 SHEN_NAME4('a', '3', '9', '8')
#define QDTB_A401 SHEN_NAME4('a', '4', '0', '1')
#define QDTB_AUTHOR_0 SHEN_NAME4('A', 'U', '_', '0')
#define QDTB_AUTHOR_1 SHEN_NAME4('A', 'U', '_', '1')
#define QDTB_AUTHOR_2 SHEN_NAME4('A', 'U', '_', '2')
#define QDTB_INVALID 0xffffffffU
#define QDTB_DEBUG_ID 3
#define QDTB_MOTION_SLOTS 16

typedef struct QdtbAuthorWork {
    HLibTask *task;
    unsigned int reserved04;
    shen_name author;
    short state;
} QdtbAuthorWork;
typedef struct QdtbAuthorLoopWork {
    QdtbAuthorWork *parent;
    short frame;
} QdtbAuthorLoopWork;
typedef struct QdtbEventWork {
    Thr *thread;
    HLibTask *task;
    int threadActive, mode, reserved10, reserved14;
    void *motions[QDTB_MOTION_SLOTS];
    EventReadCharactersWork *characters;
    int reserved5c[3];
    unsigned int fightFrames;
    int frames;
    unsigned char fade, exitFade, retries, reserved73;
} QdtbEventWork;
typedef struct QdtbSkyFadeWork {
    HLibTask *task;
    unsigned char state, started, reserved06[2];
    unsigned int frames, cloud, sky;
    int alpha[3], red[3], green[3], blue[3];
    float cloudStep[4], cloudValue[4], skyStep[4], skyValue[4];
} QdtbSkyFadeWork;

extern const shen_name qdtbCharacters[];
extern Vector3 qdtbRyoPosition, qdtbTgyPosition, qdtbRknPosition, qdtbSeePosition, qdtbYohPosition, qdtbIiiPosition;
extern Vector3i qdtbRyoRotation, qdtbTgyRotation, qdtbRknRotation, qdtbSeeRotation, qdtbYohRotation, qdtbIiiRotation;
extern unsigned char qdtbGangFightLost;
extern HLibTask *qdtbSkyFadeTask;
void QdtbMain(void);
void QueueBoutAuthor(shen_name author);
void BoutAuthorCallback(void *data);
void QueueBoutAuthorLoop(QdtbAuthorWork *parent);
void BoutAuthorLoopCallback(void *data);
void DisableBoutAuthorPlayerControl(void);
void DestroyBoutAuthor(QdtbAuthorWork *work);
void BeginBoutAuthorCharacters(void);
void EndBoutAuthorCharacters(void);
void HideAuthorCharacter(shen_name character);
void ShowAuthorCharacter(shen_name character);
void HideAuthorProp(shen_name prop);
void ShowAuthorProp(shen_name prop);
void QueueQdtbEvent(void);
void InitializeQdtbEvent(QdtbEventWork *work);
void QdtbDebugExitCallback(void *data);
void FinishQdtbEvent(QdtbEventWork *work);
void QdtbEventCallback(void *data);
void QdtbSequenceThread(void *data);
void QdtbGangFightThread(void *data);
void QdtbTgyFightThread(void *data);
void QueueQdtbSkyFade(unsigned int minutes, int red, int green, int blue);
void QdtbSkyFadeCallback(void *data);
void BeginQdtbSkyFade(QdtbSkyFadeWork *work);
void UpdateQdtbSkyFade(QdtbSkyFadeWork *work);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
inline constexpr shen_name QDTB_TASK{SHEN_NAME(EVNT)}, QDTB_SEQUENCE{SHEN_NAME(SEQP)}, QDTB_BATTLE{SHEN_NAME(BSBT)};
inline constexpr shen_name QDTB_RYO{SHEN_NAME(RYO_)}, QDTB_TGY{SHEN_NAME(TGY_)}, QDTB_YOH{SHEN_NAME(YOH_)}, QDTB_RKN{SHEN_NAME(RKN_)};
inline constexpr shen_name QDTB_III{SHEN_NAME(III_)}, QDTB_SEE{SHEN_NAME(SEE_)}, QDTB_SYE{SHEN_NAME(SYE_)}, QDTB_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name QDTB_TENT{SHEN_NAME(TENT)}, QDTB_A235{SHEN_NAME(a235)}, QDTB_A352{SHEN_NAME(a352)}, QDTB_A401{SHEN_NAME(a401)};
inline constexpr shen_name QDTB_A397{SHEN_NAME(a397)}, QDTB_A398{SHEN_NAME(a398)}, QDTB_RETURN{SHEN_NAME(0250)};
inline constexpr shen_name QDTB_AUTHORS[]{SHEN_NAME(AU_0), SHEN_NAME(AU_1), SHEN_NAME(AU_2)};
inline constexpr shen_name QDTB_CHARACTERS[]{QDTB_RYO, QDTB_TGY, QDTB_YOH, QDTB_RKN, QDTB_III, QDTB_SEE, QDTB_SYE, EVENT_CHARACTER_INVALID};
inline constexpr shen_name QDTB_OPPONENTS[]{QDTB_TGY, QDTB_YOH, QDTB_RKN, QDTB_III, QDTB_SEE, QDTB_SYE, EVENT_CHARACTER_INVALID};
inline constexpr shen_name QDTB_ALPHA_MODELS[]{QDTB_A235, QDTB_A352, QDTB_A397, QDTB_A398, QDTB_A401};
inline constexpr std::int32_t QDTB_TENT_PARTS[]{175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157, 166, 154, 173, 168, 153, 165, 160, 176, 171, 164,
    159, 161, 156, 192, 193, 180, 181, 182, 189, 190, 191, 186, 187, 188, 183, 184, 185, 177, 178, 179};
inline constexpr Vector3 QDTB_POSITIONS[]{{176.1300048828125f, 128.60000610351562f, 58.060001373291016f},
    {176.10000610351562f, 128.60000610351562f, 59.470001220703125f}, {178.5800018310547f, 128.60000610351562f, 56.970001220703125f},
    {173.47000122070312f, 128.60000610351562f, 57.29999923706055f}, {177.00999450683594f, 128.60000610351562f, 55.52000045776367f},
    {175.2899932861328f, 128.60000610351562f, 55.779998779296875f}};
inline constexpr Vector3i QDTB_ROTATIONS[]{{0, 34588, 0}, {0, 109, 0}, {0, 15561, 0}, {0, 49519, 0}, {0, 26460, 0}, {0, 45219, 0}};
inline constexpr shen_name QDTB_POSE_CHARACTERS[]{QDTB_RYO, QDTB_TGY, QDTB_RKN, QDTB_SEE, QDTB_YOH, QDTB_III};
inline constexpr shen_name QDTB_STANCE_CHARACTERS[]{QDTB_RYO, QDTB_TGY, QDTB_YOH, QDTB_III, QDTB_SEE, QDTB_RKN};
inline constexpr std::uint32_t QDTB_STANCES[]{306, 2070, 3632, 3636, 3640, 3644};

struct QdtbBoutWork {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::uint32_t sequenceActive, state, unused24, reserved28;
    std::uint32_t motions[16];
    EventReadCharactersWork *characters;
    std::byte reserved68[12];
    std::uint32_t battleFrames;
    std::int32_t countdown;
    std::uint8_t fadeStarted, closingFadeStarted, retryCount, reserved7f;
};
struct QdtbAuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t state;
    std::byte reserved12[6];
};
struct QdtbAuthorLoopWork {
    QdtbAuthorWork *parent;
    std::uint16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(QdtbBoutWork) == 0x80 && offsetof(QdtbBoutWork, characters) == 0x60 && offsetof(QdtbBoutWork, battleFrames) == 0x74);
static_assert(sizeof(QdtbAuthorWork) == 0x18 && offsetof(QdtbAuthorWork, state) == 0x10 && sizeof(QdtbAuthorLoopWork) == 0x10);
void QdtbInit();
void QdtbMain(std::uintptr_t);
extern const ScriptAreaDefinition qdtbScriptDefinition;
} // namespace shenmue::s2
#endif
