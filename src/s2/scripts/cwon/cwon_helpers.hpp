#pragma once
#include "s2/scripts/cwon/area_cwon.hpp"
namespace shenmue::s2 {
void CwonSetPositionMasked(shen_name, std::uint8_t, const Vector3 &);
void CwonSetRotationMasked(shen_name, std::uint8_t, const Vector3i &);
void CwonRotateDoorCallback(void *);
void CwonHideAtOrigin(shen_name);
void CwonMoveBackward(shen_name, float);
std::int32_t CwonPositionThreshold(shen_name, float, bool, bool);
std::int32_t CwonTurnTowardYaw(shen_name, std::int32_t, std::int32_t, bool);
void CwonSetPositionAnglesDegrees(shen_name, const Vector3 &, const Vector3 &);
void CwonDisableGravity(shen_name);
std::uint32_t CwonCreateDust(const Vector3 &, const Vector3 &, const Vector3 &);
HLib::Task *CwonQueueLoadingTitle(shen_name, std::int32_t);
HLib::Task *CwonQueueFadeWait(HLib::Task *);
struct CwonMotionWaitWork {
    HLib::Task *parent;
    shen_name character;
    float frame;
};
static_assert(sizeof(CwonMotionWaitWork) == 0x10);
HLib::Task *CwonQueueMotionWait(HLib::Task *, shen_name, float);
void CwonMotionWaitCallback(void *);
struct CwonLookWork {
    Vector3 position;
    std::int32_t state;
    shen_name character, target;
    std::uint32_t operation, flags;
    std::int32_t duration, bone;
};
static_assert(sizeof(CwonLookWork) == 0x28);
HLib::Task *CwonQueueLook(shen_name, shen_name, std::int32_t, std::int32_t, std::int32_t, std::uint32_t, std::int32_t, std::int32_t);
void CwonLookCallback(void *);
void CwonBirdCallback(void *);
void CwonBirdFlightCallback(void *);
void CwonPipeCallback(void *);
void CwonPlayerDustCallback(void *);
void CwonPlayerOffsetDustCallback(void *);
void CwonResetSmoke(CwonEventWork *);
void CwonRestoreGravity(shen_name);
} // namespace shenmue::s2
namespace shenmue::s2 {
std::uint32_t CwonFinishEscape(CwonEventWork *);
void CwonEnablePositionedCharacter(shen_name, const Vector3 &, float);
void CwonGetDodgeCallback(void *);
void CwonMarketPropsCallback(void *);
void CwonRestoreMarketProps(std::int32_t);
void CwonRestoreBoxes();
void CwonWarehousePropsCallback(void *);
HLib::Task *CwonQueuePipeDynamics();
void CwonFallingWorkerCallback(void *);
void CwonFallingSignCallback(void *);
void CwonOpeningDoorCallback(void *);
std::int32_t CwonInitializePedestrian();
void CwonTurningPedestrianCallback(void *);
void CwonSetCollisionTables();
void CwonSetFallenTable();
} // namespace shenmue::s2
