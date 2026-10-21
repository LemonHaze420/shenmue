#pragma once
#include "s1/task/mobj.hpp"
#include "s1/task/kdan.hpp"
#include "s1/motion.hpp"
#include "s1/collision.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/skinning_cache.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <cctype>
#include "s1/matrix_stack.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/conv.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/wexe.hpp"
#include "s1/mt5_types.hpp"
#include "s1/mt5_render.hpp"
#include "s1/face.hpp"
#include "s1/render_array.hpp"
#include "s1/player_control.hpp"
#include "s1/timing.hpp"
#include "s1/boot.hpp"
#include "s1/subtitles.hpp"
#include "s1/file_loader.hpp"

#include <cfloat>
#include "s1/task/cycle_man_vehicle.hpp"
#include "s1/task/cycle_man_steering.hpp"

namespace shenmue::s1 {
namespace CycleManager {}
using namespace CycleManager;

extern CycleManager::RuntimeState g_cycleManagerRuntime;

extern std::int32_t g_MOBJ_num_entries;

extern std::uint16_t g_steeringIntersectionVolumeCount;

extern float g_default_turning_box_half_depth_z;

extern float g_angYoffset;

extern uint32_t g_CycleTable[20];

extern uint32_t g_TotalActiveCycles;

namespace CycleManager {

enum class CycleListInsert {
    Head,
    Tail,
};
} // namespace CycleManager

extern std::uint16_t g_dynamicTurnCollisionVolumeCount;

extern float g_fGlobalTurningRadius;

extern CycleManager::ObjectNodeRotationFn g_cycleManagerObjectNodeRotation;

namespace CycleManager {

void EnableRootMotion(Character *character, bool enable);

int16_t NodeChainYaw(const Character *character);

void SetNodeChainYaw(Character &character, int16_t yaw);

} // namespace CycleManager

Mt5RuntimeNode *FindRuntimeNodeByKind(Mt5RuntimeNode *node, std::byte *modelBase, uint16_t nodeKind);

extern std::byte *g_pMOBJ;

extern CycleMoveObjectRuntime *g_visibleMoveObjects;

extern CycleMoveObjectRuntime *g_parkedMoveObjects;

extern CycleMoveObjectRuntime *g_freeMoveObjects;

namespace CycleManager {

void UnlinkParkedMoveObject(CycleMoveObjectRuntime *runtime);

} // namespace CycleManager

namespace CycleManager {

Character *MapRyo();
} // namespace CycleManager


namespace CycleManager {

extern const std::array<std::int32_t, 8> kDobuitaBikeSounds;

extern const std::array<std::int32_t, 8> kSakuragaokaBikeSounds;

extern const std::array<std::int32_t, 8> kJu00BikeSounds;

extern const std::array<std::int32_t, 8> kAmihamaBikeSounds;

extern const std::array<BikeAnimCurve, 8> kYkhiBikeAnimCurves;

extern const std::array<BikeAnimCurve, 8> kSobaBikeAnimCurves;

extern const std::array<BikeAnimCurve, 2> kHalBikeAnimCurves;

std::int32_t GetBikeSoundsForArea(CycleManager::NpcRecord &npc);

void PerformBikeSoundUpdate(CycleManager::NpcRecord &npc, float observedSpeed);

} // namespace CycleManager

namespace CycleManager {

int16_t AngleDegreesToFixed16(float degrees);

char *FilePointer(uint32_t offset);

uint32_t FileOffset(const void *pointer);

CycleManager::NpcRecord *NextProgramInList(CycleProgramList list, const CycleManager::NpcRecord *after);

uint8_t *MoveObj_FindEntry(CycleScheduleEntry *entries, uint32_t requestCycle, uint64_t unused, int32_t size);

CycleManager::MNRBEntry *FindMNRBEntry(uint32_t token);
} // namespace CycleManager


void InitBikePathRuntime(CycleManager::NpcRecord &npc, const char *record);
namespace CycleManager {


bool InitVehiclePath2D(CycleManager::NpcRecord &npc, uint32_t recordOffset, uint32_t opcode);

uint32_t ReadCommandDwordOrZero(uint32_t recordOffset, uint32_t payloadOffset);
} // namespace CycleManager


namespace CycleManager {

void RecomputeSteeringExtents(CycleManager::NpcRecord &npc);

} // namespace CycleManager

namespace CycleManager {

void RecomputeSteeringExtentsForNpc(CycleManager::NpcRecord &npc);
} // namespace CycleManager


int32_t MOTN_getFrameCountFromAnimId(uint16_t animationId);

float npcCalculateMovementDistanceStep(const CycleManager::NpcRecord &npc);
namespace CycleManager {


uint16_t SelectRandomMotionToken(CycleManager::NpcRecord &npc);

void ApplyActiveAnimAndMotionVariant(CycleManager::NpcRecord &npc, uint16_t activeAnim, bool facingConstraint);
} // namespace CycleManager


namespace CycleManager {

uint32_t MCIR_FindChairPosInfoByOccupantObjectId(uint32_t occupantObjectId, uint32_t chairGroup, uint32_t *chairRow = nullptr);
BikeMoveObjectToken MakeBikeToken(const BikeAnimTokenSet *anims, Vector3 localOffset, int16_t steeringClamp, int16_t wheelPitch,
    Vector3 leftAnkleLocal, Vector3 rightAnkleLocal, Vector3 wheelPivotLocal, float mountTargetX, float mountTargetZ, int16_t mountYawDelta, int16_t runtimeC0,
    float steeringHalfWidth, float steeringHalfDepth, uint8_t speedCurveMode);

extern const BikeMoveObjectToken kBikeTokenHAL0;

extern const BikeMoveObjectToken kBikeTokenYKHI;

extern const BikeMoveObjectToken kBikeTokenK013;

extern const BikeMoveObjectToken kBikeTokenHAL;

extern const BikeMoveObjectToken kBikeTokenSOBA;

extern const BikeMoveObjectToken kBikeTokenHAL0Alt;

extern const BikeMoveObjectToken kBikeTokenHALAlt;

extern const BikeSpeedProfile kBikeSpeedDefault;

extern const BikeSpeedProfile kBikeSpeedBike;

extern const BikeSpeedProfile kBikeSpeedHAL;

extern const BikeSpeedProfile kBikeSpeedFallback;

void ResolveVisibleBikeModels(CycleMoveObjectRuntime &runtime);

void PublishVisibleParkedMoveObjects();

void AppendParkedVehicleSteeringVolumes();

} // namespace CycleManager

namespace CycleManager {

const BikeMoveObjectToken *GetBikeMoveObjectToken(uint32_t ownerToken, uint32_t vehicleToken);

float ResolveBikeSpeedParam(const CycleMoveObjectRuntime *runtime, float rawSpeed);
} // namespace CycleManager


namespace CycleManager {

inline constexpr std::uint8_t kMotionLeftAnkleNode = 11;
inline constexpr std::uint8_t kMotionRightAnkleNode = 17;
inline constexpr std::uint8_t kMotionRightFootEndNode = 18;
inline constexpr std::uint8_t kMotionNodeEnd = 0xff;
inline constexpr std::uint8_t kTransitionRowsBlendMode = 4;
void MOBJ_BikeMotionConstraintCallback(MotionInformationWork *motion, void *user);

} // namespace CycleManager

namespace CycleManager {

CycleMoveObjectRuntime *AttachVehicleRuntime(CycleManager::NpcRecord &npc, uint32_t vehicleToken, uint32_t moveObjectKind);

void SyncVehicleRuntimeFromNpc(CycleManager::NpcRecord &npc);

void ReleaseVehicleRuntime(CycleManager::NpcRecord &npc);

bool BeginBikeMoveObjectRelease(CycleManager::NpcRecord &npc);
} // namespace CycleManager


extern NaturalCubicSpline3DCache g_spline3d;
namespace CycleManager {


bool TickVehicleActiveState(CycleManager::NpcRecord &npc);

bool BeginVehicleActiveState(CycleManager::NpcRecord &npc, bool live);
} // namespace CycleManager


namespace CycleManager {

CycleManager::TaggedPayload *FindTaggedPayload(CycleManager::NpcRecord &npc, uint32_t identifier);

bool QueueObjectInteraction(CycleManager::NpcRecord &npc, uint32_t offset);

} // namespace CycleManager

namespace CycleManager {

bool ApplyNpcCommandState(CycleManager::NpcRecord &npc, uint32_t opcode, uint32_t recordOffset, bool *positionChanged);

CycleManager::NpcRecord *CycleRecord(CycleManager::MobjCycle *ctx);

uint32_t HandleTimedMorsApproach(CycleManager::MobjCycle *ctx, char *record, uint32_t live, int32_t time);

void AppendDynamicTurnVolume(const Vector3 &center, int16_t yaw, float halfWidth, float halfDepth);

uint32_t CurrentTimeSeconds();

float Distance(const Vector3 &lhs, const Vector3 &rhs);

int16_t Atan2Fixed16(float x, float z);
} // namespace CycleManager


bool npcActivateIfNearInArea(CycleManager::NpcRecord &npc, float radius, const Vector3 *target, int16_t targetYaw, bool commit, uint16_t animId);

bool IsTargetPointBlockedByRyoOrActor(const CycleManager::NpcRecord &npc, const Vector3 &target, float distanceSquared);
namespace CycleManager {

CycleManager::NpcRecord *GetTrackedMobj(CycleManager::NpcRecord &npc);

float DistanceSquared(const Vector3 &a, const Vector3 &b);

int16_t FoldUnsignedYaw(uint16_t yaw);
} // namespace CycleManager


namespace CycleManager {

inline constexpr float kSteeringEpsilon = 0.00000011920929f;
extern uint16_t g_mobjCollisionObjectCount;

extern uint16_t g_mobjCollisionObjectCursor;

extern uint16_t g_dynamicTurnCollisionVolumeCursor;

extern uint16_t g_steeringIntersectionVolumeCursor;

extern uint32_t g_currentMobjRectangleOffset;

float GetMoveObjectShapeOffset(const CycleManager::NpcRecord &npc);

void ApplyRotationMatrix(const OrientedSteeringQuadParams &params);

} // namespace CycleManager

bool check_if_point_in_quadrilateral(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d, const Vector3 &point);

bool CheckIntersectionOnPoint(float positionX, float positionZ, float targetX, float targetZ);
namespace CycleManager {


void CollectCollisionObjects();

bool CheckClearanceAgainstRyoAndCycles(const CycleManager::NpcRecord &npc, bool runStart);

bool AdjustDesiredYawForSteeringCollision(CycleManager::NpcRecord &npc);
} // namespace CycleManager


bool UpdateNpcSteeringFromMOTN(CycleManager::NpcRecord &npc, bool applyPosition);

namespace CycleManager {

extern std::int32_t g_cycleCommandScanHasPosition;

extern Vector3 g_cycleCommandScanPosition;

extern const CycleAreaCapacity kCycleAreaCapacities[3];

bool AreaCapacityExceeded(std::uint32_t areaId);

std::uint32_t CommandWord(std::uint32_t offset);

} // namespace CycleManager

namespace CycleManager {

bool ApproachSteeringTargetXZ(CycleManager::NpcRecord &npc, float factor);
} // namespace CycleManager


int32_t shenUpdateSteeringState(CycleManager::NpcRecord &npc);

bool npcUpdateMovementPath(CycleManager::NpcRecord &npc);
namespace CycleManager {


bool UpdateMoveObjectPathing(CycleManager::NpcRecord &npc, bool moveObjMode);

bool TickPathFollowMotion(CycleManager::NpcRecord &npc, bool steeringActive);

bool InitTimedPathMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive);

bool InitPathFollowMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive);

bool InitPathFollowMotion(CycleManager::NpcRecord &npc, uint32_t offset, uint32_t count, bool steeringActive);
} // namespace CycleManager


void cyclSubmitMoveRequest(CycleManager::NpcRecord &npc);

void cycleRequestAnim(CycleManager::NpcRecord &npc, uint16_t animId);

void cyclFreeTalkConvSetMoveAndFacingConstraint(CycleManager::NpcRecord &npc, uint16_t animId);

void UpdateEntityOverPositionWithFlags(CycleManager::NpcRecord &npc, bool restoreIdle);
namespace CycleManager {


void StartDoorFacingHandoff(CycleManager::NpcRecord &npc);
} // namespace CycleManager


namespace CycleManager {

CycleManager::MNRBEntry *FindMNRBEntryByLineOffset(uint32_t lineDataOffset);

bool ReadMNRBVector(uint32_t tableOffset, uint32_t index, Vector3 &out);

void PruneMNRBReservations(CycleManager::MNRBEntry &entry);

void ReleaseMNRBReservation(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry *entry);

void CheckBoundaryTriggerAndPlaySounds(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry &entry, uint32_t recordOffset);

uint32_t BeginUseMnrbEntry(CycleManager::NpcRecord &npc, char *record, uint32_t live, int32_t time);

bool TickMnrbCounterOwner(CycleManager::NpcRecord &npc, uint32_t live, int32_t time);

bool TickMnrbBusStopOwner(CycleManager::NpcRecord &npc);

bool TickMnrbQueueOwner(CycleManager::NpcRecord &npc, uint32_t live);

bool TickMnrbBusStopUse(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry *entry, uint32_t live);

bool TickUseMnrbEntry(CycleManager::NpcRecord &npc, uint32_t live, int32_t time);

void ClearAllMoveObjChairOccupancy();

uint32_t MCIR_FindChairGroupById(uint32_t chairGroupId);

bool EndMoveObjForHistory(CycleManager::NpcRecord &npc);

Vector3 MoveObjPresetChairPosition(const CycleManager::NpcRecord &npc, uint32_t chairPos);

uint16_t FindMoveObjIdleMotion(const CycleManager::NpcRecord &npc);

bool HasMoveObjChairPreset(const CycleManager::NpcRecord &npc);

bool ExecuteMoveObjState(CycleManager::NpcRecord &npc, bool live, int32_t time);

void npcUpdateBaseSpeed(CycleManager::NpcRecord &npc, uint16_t pathIndex);

float SnapshotMoveObjTransform(CycleManager::NpcRecord &npc, bool live);

void UpdatePathSegmentCache(CycleMoveObjectRuntime &runtime, const Vector2 &from, const Vector2 &to);

void RotateForBIKE(CycleMoveObjectRuntime &runtime, int32_t slope, int32_t yaw, int32_t lean);

void DoBikeAngles(CycleManager::NpcRecord &npc, CycleMoveObjectRuntime &runtime, float speed);

bool TickBikeMoveObjectRuntime(CycleManager::NpcRecord &npc, bool live);

bool DoBikeStuff(CycleManager::NpcRecord &npc, bool live);

bool DoBikeStuff_0(CycleManager::NpcRecord &npc, bool live);

void UpdateBikeReverseAvoidance(CycleManager::NpcRecord &npc, float observedSpeed, bool doForwardConeCheck);

void AdjustDrivePathForClearance(CycleManager::NpcRecord &npc);

} // namespace CycleManager

namespace CycleManager {

void InitBikeMoveObjectPathContext(CycleManager::NpcRecord &npc);

void TickBikeMoveObjectTransition(CycleManager::NpcRecord &npc);

void TickBikeMoveObjectRelease(CycleManager::NpcRecord &npc);
} // namespace CycleManager


namespace CycleManager {

float DistanceXZWithEpsilon(const Vector3 &lhs, const Vector3 &rhs);

} // namespace CycleManager

namespace CycleManager {

bool TickPositionAndHeadingMove(CycleManager::NpcRecord &npc);
} // namespace CycleManager


namespace CycleManager {

int32_t UpdateClass6DoorCounter(int32_t counter, int32_t step, bool opening);

float Class6DoorBlend(int32_t counter);

void UpdateVehicleWheelNodes(CycleManager::NpcRecord &npc);

void UpdateClass6DoorNodes(CycleManager::NpcRecord &npc);

} // namespace CycleManager

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

inline constexpr float kCyclePositionEpsilon = 0.00000011920929f;

inline constexpr float kCyclePositionStep = 0.041666668f;

inline constexpr uint32_t kCycleMotionTransitionActive = 0x00010000u;

inline constexpr uint8_t kCycleWalkCubicBlend = 11u;

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1::CycleManager {

inline constexpr std::size_t kMobjCoreAllocationSize = 0x2c0;

inline constexpr std::size_t kMobjHeapAllocationSize = 0x1bc0;

inline constexpr std::size_t kMobjDynamicAllocationSize = 0x2740;

inline constexpr std::size_t kHLibAllocationHeaderSize = 0x40;

inline constexpr std::size_t kMobjDynamicWorkSize = kMobjDynamicAllocationSize - kHLibAllocationHeaderSize;

inline constexpr std::size_t kMobjDynamicRuntimeCount = kMobjDynamicWorkSize / sizeof(CycleMoveObjectRuntime);

static_assert(kMobjDynamicRuntimeCount == 32);

static_assert(kMobjDynamicRuntimeCount * sizeof(CycleMoveObjectRuntime) == kMobjDynamicWorkSize);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {

extern std::int8_t g_lastProximityGroup;

extern std::uint64_t g_cycleShopExitPositionXY;

extern std::int32_t g_cycleShopExitPositionZ;

extern std::int32_t g_cycleShopExitYaw;

extern std::int32_t g_cycleShopExitPublished;

extern PackOwner *HUMANS_AFS_Ptr;

extern HumansEntry *HumansIndexTable;

extern std::int32_t g_bHasLoadedHuman;

extern std::uint32_t g_cycleScriptHumanChara;

extern char g_cycle_file_str[16];

extern char g_tmp_cm_path[32];

extern std::int32_t g_MOBJ_needs_new_update;

extern CycleManager::MobjEntry *g_SimSetHead;

extern CycleManager::MobjEntry *g_PromoteList;

extern CycleManager::MobjEntry *g_MidQueue;

extern CycleManager::MobjEntry *g_LoadQueue;

extern CycleManager::MobjEntry *g_FarQueue;

extern CycleManager::MobjEntry *g_RunningPrograms;
namespace CycleManager {


CycleManager::MobjEntry *&ProgramListHead(CycleProgramList list);
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void MoveProgramToList(CycleManager::NpcRecord &npc, CycleProgramList destination, CycleListInsert insert);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {

extern std::int32_t g_cycleConversationHoldActive;

extern std::int32_t g_scntReferencedCycleHeld;

extern std::int32_t g_cycleTalkSuppressed;

extern PackSlot *gRainModelPtr;

extern float g_cycle_distance_check;

extern CycleManager::TaggedPayload *g_CycleManager_Current;

extern CycleManager::TaggedPayload *g_CM_obj_list;

extern std::int32_t nRainState;

extern std::array<void *, 4> g_rainStateModels;

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void ApplyCharacterTransform(Character &owner, const Vector3 &position, const Vector3i &rotation);

void AttachEntityAndInitCycle(Character *character, bool deactivate);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {

extern std::byte *g_tmp_MOBJ;

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

CycleMoveObjectRuntime *MOBJ_DynamicRuntimes();

void MOBJ_ResetDynamicRuntimes();

std::size_t MOBJ_DynamicRuntimeCount();

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


CycleManager::NpcRecord *EntryRecord(CycleManager::MobjEntry *entry);

void LoadMNRBAndMCIR();

void CopyCycleValues();

void LoadSHUT();

bool IsReasonablePosition(const Vector3 &pos);

template <typename T> bool ReadCommandValue(uint32_t recordOffset, uint32_t payloadOffset, T &out);

bool WriteCommandDword(uint32_t absoluteOffset, uint32_t value);
} // namespace CycleManager


int32_t Humans_n_GetIndexForCharacter(uint32_t charaName);

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

uint64_t LargestFreeTaskHeapBlock();

bool Weather_ProcessRain();

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {

timeContextType_t ConstructTimeContext(TimeContext *output);
namespace CycleManager {


uint16_t MotionParamWord(const CycleManager::NpcRecord &npc, uint32_t wordIndex);
} // namespace CycleManager


float MOTN_CalculateRootMotionSpeedZ(CycleManager::NpcRecord &npc);
namespace CycleManager {


bool ApplyActiveAnimRootYaw(CycleManager::NpcRecord &npc);

bool ReadCommandPositionYaw(uint32_t recordOffset, Vector3 &pos, int16_t &yaw);

bool InitStairPath2D(CycleManager::NpcRecord &npc, uint32_t recordOffset);
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

bool ProcessMatrixTransformState(const CycleManager::NpcRecord &npc, const CycleManager::TaggedPayload &payload, std::uint32_t mode);

void ApplyTaggedPayloadPlacement(const CycleManager::TaggedPayload &payload);

void CommitXForm_RotateTranslateZYX(CycleManager::NpcRecord &npc, uint32_t offset);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


void CreateCharacter(CycleManager::NpcRecord &npc);

bool InitializeMoveObjContext(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool history);

void ApplyAnimationStateCommand(CycleManager::MobjCycle *ctx, char *command);
} // namespace CycleManager


uint32_t AppendCycleEntry(CycleManager::MobjCycle *ctx, char *record, uint32_t live, int32_t time);
namespace CycleManager {


uint32_t CheckPointer(int32_t time, CycleManager::MobjCycle *ctx, char **outData, uint32_t *outResult);

uint32_t ExecuteProgramStep(CycleManager::MobjCycle *ctx, int32_t time, int32_t *inOutTime);

void UpdateSHUTRowObject(CycleManager::ShutRow &row);

void BindSHUTRowsToSceneObjects();

bool DispatchSceneTask(uint32_t token, uint32_t dispatchState, uint32_t currentTime);


bool SelectBranchByFlagsAndDate(uint32_t charaHeaderOffset, timeContextType_t type, const TimeContext &context);

int32_t ReadCyclemanForScene(const char *InFilename);
} // namespace CycleManager


void npcSetupFace(CycleManager::MobjCycle *npc);
namespace CycleManager {


void MoveObj_FileLoad(const char *InFilename);

void ApplyNpcRecordToLocal(CycleManager::NpcRecord &npc);
} // namespace CycleManager


bool PerformAicaSoundUpdates(CycleManager::NpcRecord &npc, const Vector3 &target, int16_t targetYaw, bool commit);
namespace CycleManager {


bool StartDoorRequest(CycleManager::NpcRecord &npc);

bool PollDoorRequest(CycleManager::NpcRecord &npc) noexcept;
} // namespace CycleManager


CycleManager::MobjCycle *npcFindEntry(uint32_t charaName);
namespace CycleManager {


bool BeginTaggedWait(CycleManager::NpcRecord &npc, uint32_t opcode, uint32_t recordOffset, uint32_t currentTime);

bool UpdateTaggedWait(CycleManager::NpcRecord &npc, uint32_t currentTime);
} // namespace CycleManager


void npcSetMoveFacingMot(CycleManager::NpcRecord &npc, bool enableRootMotion, bool facingConstraint);

void cyclUpdateCharaMoveRequestForAnimChange(CycleManager::NpcRecord &npc);
namespace CycleManager {


void SubmitXMPTMoveWithCycleParams(CycleManager::NpcRecord &npc, Character &character, const Vector3 &target, int16_t yaw, uint16_t animId);
} // namespace CycleManager


bool PlayAnim_Wrapper(Character *character, MotionInformationWork *motion, float startFrame, std::uint16_t animId, std::uint16_t nodeId, std::uint32_t flags);

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void FreeSecondaryMotion(MotionInformationWork &motion);

void BeginSecondaryBlendOut(MotionInformationWork &motion);

void PlayOverlayAnim(CycleManager::NpcRecord &npc, std::uint16_t animId, std::uint16_t nodeId, std::uint32_t flags);

void UNFO_InitWrapper(MotionInformationWork &motion);

void cyclUmbrellaHandCallback(MotionInformationWork *motion, void *user);

void cyclUmbrellaReleaseCallback(MotionInformationWork *motion, void *user);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


void PlayAnim_Wrapper(CycleManager::NpcRecord &npc, std::uint32_t recordOffset);

void FreeIfDoorOpeningAnim(CycleManager::NpcRecord &npc);
} // namespace CycleManager


void PlayAnim_Wrapper_1(CycleManager::NpcRecord &npc, bool raining);

void PlayAnim_Wrapper_2(CycleManager::NpcRecord &npc, bool steeringActive);

void PlayAnim_Wrapper_0(CycleManager::NpcRecord &npc, std::uint32_t recordOffset);
namespace CycleManager {


bool UpdateTalkMoveObjectState(CycleManager::NpcRecord &npc, bool motnSteeringActive, bool ryoTalk);
} // namespace CycleManager


extern CycleManager::NpcRecord g_cyclTalkCycle;
namespace CycleManager {


bool BeginRyoTalkWalk(Character *ryo);
} // namespace CycleManager


bool TALK(Character *ryo);
namespace CycleManager {


bool BeginTalkMoveObjectState(CycleManager::NpcRecord &npc, uint32_t recordOffset);

void UpdateSndStreaming(CycleManager::NpcRecord &npc);

void FinishTargetApproachAndStopSounds(CycleManager::NpcRecord &npc);

bool TickTargetApproachWithSound(CycleManager::NpcRecord &npc);
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void TickWaitTurnAnimation(CycleManager::NpcRecord &npc);

void TickWaitOutsideRadius(CycleManager::NpcRecord &npc);

void TickTransitionDelta(CycleManager::NpcRecord &npc);

void TickWaitMotionFullRestore(CycleManager::NpcRecord &npc);

bool IsGlobalSimulationEnabled(const ScenarioFunctionWork *scnf);

bool SimSetSkipBlend(CycleManager::NpcRecord &npc, const ScenarioFunctionWork *scnf, bool globalSimulation);

void HoldForConversation(CycleManager::NpcRecord &npc);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


uint32_t BeginShopShutterCommand(CycleManager::NpcRecord &npc, const char *record);
} // namespace CycleManager


uint32_t MOBJ_executeTask(CycleManager::MobjEntry *CM, int32_t time, uint32_t motnSteeringActive, uint32_t skipBlend);
namespace CycleManager {


void UpdateCurrentCursor(CycleManager::MobjCycle *cycle);

void Update(CycleManager::MobjEntry *currEntry);

void UpdateMOTNSplines();

void RenderTaggedPayloads();

void RenderMoveObjects();

void DrawUmbrellas();
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

#if !SHENMUE_IS_PC
extern std::array<std::byte, kMobjCoreAllocationSize> g_mobjCoreAllocation;
extern std::array<std::byte, kMobjHeapAllocationSize> g_mobjHeapAllocation;
extern std::array<std::byte, kMobjDynamicAllocationSize> g_mobjDynamicAllocation;
#endif
void ClearProgramLists();
void ReleaseWorldOwnersForTeardown();
void Setup(const char *InFilename);
void CoreCallback_Mo_Exec(HLibTaskData *);
void MotionCallback_Mo_Exec2(HLibTaskData *);

} // namespace shenmue::s1::CycleManager
