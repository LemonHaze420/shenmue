#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_loop_dc.h"

#define PAWN_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define PAWN_BASE SHEN_NAME4('p', 'a', 'w', 'n')
#define PAWN_ARP0 SHEN_NAME4('A', 'R', 'P', '0')
#define PAWN_WSP0 SHEN_NAME4('W', 'S', 'P', '0')
#define PAWN_WKP0 SHEN_NAME4('W', 'K', 'P', '0')
#define PAWN_WKP1 SHEN_NAME4('W', 'K', 'P', '1')
#define PAWN_WTP0 SHEN_NAME4('W', 'T', 'P', '0')
#define PAWN_WTP1 SHEN_NAME4('W', 'T', 'P', '1')
#define PAWN_WTP2 SHEN_NAME4('W', 'T', 'P', '2')
#define PAWN_WTP3 SHEN_NAME4('W', 'T', 'P', '3')
#define PAWN_WTP4 SHEN_NAME4('W', 'T', 'P', '4')
#define PAWN_WRP0 SHEN_NAME4('W', 'R', 'P', '0')
#define PAWN_QBL0 SHEN_NAME4('Q', 'B', 'L', '0')
#define PAWN_QBR0 SHEN_NAME4('Q', 'B', 'R', '0')
typedef struct PawnWork {
    HLibTask *parent;
    HLibTask *task;
    HLibTask *awardTask;
    shen_name shop;
    Vector3 position;
    Vector3i rotation;
    int reserved28;
    char package[12];
    int saleOutcome;
} PawnWork;
typedef struct PawnTalkWork {
    HLibTask *parent;
    int flag;
    shen_name character;
    int started;
} PawnTalkWork;
typedef struct PawnFlyerAwardWork {
    int elapsed, frame, item;
} PawnFlyerAwardWork;
typedef struct PawnShopItem {
    shen_name shop;
    int item;
} PawnShopItem;
typedef struct PawnShopScene {
    shen_name shop, owner, flyer;
    int presentationFlag;
    float flyerX, flyerZ;
    int flyerYaw;
    float flyerOffsetX, flyerOffsetZ;
    float businessX, businessZ;
    int businessYaw;
    float businessOffsetX, businessOffsetZ;
    int help;
} PawnShopScene;
void PawnMain(shen_name shop);
void PawnInitialize(void *data);
void PawnWaitPackage(void *data);
void PawnWaitOwner(void *data);
void PawnDone(void *data);
void PawnBegin(void *data);
void PawnBeginFlyer(void *data);
void PawnFlyerAward(void *data);
void PawnFlyerDone(void *data);
void PawnTalkOpening(void *data);
void RestorePawnTransactionPose(PawnWork *work, shen_name shop);
void PawnToSelect(void *data);
void PawnBeginSelect(void *data);
void PawnExecuteSelect(void *data);
void PawnCancel(void *data);
void PawnSetTransaction(void *data);
void PawnItemTransaction(void *data);
void PawnNotSelling(void *data);
void PawnSaleDialogue(void *data);
int PawnFlyerRequested(shen_name shop);
shen_name FindPawnOwner(shen_name shop);
shen_name FindPawnFlyer(shen_name shop);
int FindPawnItem(shen_name shop);
int GetPawnStandData(shen_name shop, Vector3 *position, int *yaw, Vector3 *offset, int *playerYaw, int flyer);
int QueuePawnTalk(HLibTask *parent, shen_name character, int flag);
void PawnTalkCallback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/area_loop.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name PAWN_RYO{SHEN_NAME(RYO_)}, PAWN_BASE{SHEN_NAME(pawn)}, PAWN_EVENT{SHEN_NAME(EVNT)};
struct PawnEventWork {
    HLib::Task *parent;
    HLib::Task *self;
    HLib::Task *flyerTask;
    shen_name shop;
    Vector3 savedPosition;
    Vector3i savedRotation;
    std::int32_t reserved34;
    char package[12];
    std::int32_t saleOutcome;
};
static_assert(sizeof(PawnEventWork) == 0x48);
static_assert(offsetof(PawnEventWork, parent) == 0x00);
static_assert(offsetof(PawnEventWork, self) == 0x08);
static_assert(offsetof(PawnEventWork, flyerTask) == 0x10);
static_assert(offsetof(PawnEventWork, shop) == 0x18);
static_assert(offsetof(PawnEventWork, savedPosition) == 0x1c);
static_assert(offsetof(PawnEventWork, savedRotation) == 0x28);
static_assert(offsetof(PawnEventWork, package) == 0x38);
static_assert(offsetof(PawnEventWork, saleOutcome) == 0x44);
struct PawnTalkWork {
    HLib::Task *parent;
    std::int32_t flag;
    shen_name character;
    std::int32_t started, reserved14;
};
static_assert(sizeof(PawnTalkWork) == 0x18);
static_assert(offsetof(PawnTalkWork, flag) == 0x08);
static_assert(offsetof(PawnTalkWork, character) == 0x0c);
static_assert(offsetof(PawnTalkWork, started) == 0x10);
inline constexpr std::int16_t pawnDialogueFlags[]{1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229};
inline constexpr std::int16_t pawnPendingDialogueFlags[]{1222, 1224, 1225, 1226, 1227, 1228, 1229};
struct PawnShopScene {
    shen_name shop, owner, flyer;
    std::int32_t presentationFlag;
    float flyerX, flyerZ;
    std::int32_t flyerYaw;
    float flyerOffsetX, flyerOffsetZ;
    float businessX, businessZ;
    std::int32_t businessYaw;
    float businessOffsetX, businessOffsetZ;
    std::int32_t help;
};
static_assert(sizeof(PawnShopScene) == 60);
extern const PawnShopScene pawnShopScenes[13];
const PawnShopScene &FindPawnShopScene(shen_name shop);
void PawnFlyerDone(void *);
void PawnBeginFlyer(void *);
void PawnBeginSelect(void *);
void PawnFinish(void *);
void RestorePawnTransactionPose(PawnEventWork &work);
void QueuePawnScenePosition(shen_name character, const Vector3 &position, const Vector3i &rotation);
extern const ScriptAreaDefinition pawnScriptDefinition;
} // namespace shenmue::s2::areas
#endif
