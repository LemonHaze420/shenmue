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
