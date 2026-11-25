#include "s2/npc_area_route.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/qron_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/math.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>
#include <cmath>
#include "s2/npc_data_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include <cstdio>
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/date_time.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#endif
#include "s2/character_state_flags_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#define SHENMUE_NPC_NULL 0
#else
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"
#include "s2/motion_pose.hpp"
#include "s2/player.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/weather.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

struct NpcAreaStoreSlot {
    NpcAreaRouteState *state;
    NpcAreaStoreSlot *previous;
    NpcAreaStoreSlot *next;
    float distanceSquared;
    std::uint32_t reserved;
};

static_assert(sizeof(NpcAreaStoreSlot) == 0x20);

NpcAreaStoreSlot *areaStore{};

NpcAreaStoreSlot *nearestAreaSlot{};

std::size_t areaStoreCount{};

std::span<NpcAreaPoint> areaPoints;

Vector3 routeOrigin{};

struct NpcAreaProbe {
    std::int8_t rowIndex{-1};
    std::int8_t linkedRowIndex{-1};
    std::int8_t ownerIndex{-1};
    std::int8_t nextOwnerIndex{-1};
    std::int8_t ownerRowIndex{-1};
    std::byte reserved05[3]{};
    float rowParameter{};
    float ownerParameter{};
    float lateralDistance{};
    std::uint32_t reserved14{};
    NpcAreaRouteState *state{};
};

static_assert(sizeof(NpcAreaProbe) == 0x20);

NpcAreaProbe areaProbes[2];

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

template <class T> T *HeapPointer(std::int32_t offset);
std::int32_t HeapOffset(const void *pointer);
NpcAreaControlRow *ControlRows(const NpcAreaRouteState &state);
NpcAreaOwnerRow *OwnerRows(const NpcAreaRouteState &state);
NpcRouteLiveEntry *ListHead(const NpcAreaRouteState &state, std::size_t list);
void SetListHead(NpcAreaRouteState &state, NpcRouteLiveEntry *entry, std::size_t list);
void RegisterAreaState(NpcAreaRouteState &state, std::int16_t point);
std::uint8_t ScriptByte(const std::byte *command);
std::uint16_t ScriptWord(const std::byte *command);
std::uint32_t ScriptDword(const std::byte *command);
const std::byte *NextInstruction(const std::byte *command);
const std::byte *FindInstructionEnd(const std::byte *command);
const std::byte *FindInstructionStart(const std::byte *command);
const std::byte *FindSection(const std::byte *command, std::uint8_t section);
const std::byte *ChooseMotionInstruction(NpcRouteLiveEntry &entry, const std::byte *command);
std::uint16_t BeginAnimationInstruction(NpcRouteLiveEntry &entry, const std::byte *command);
bool HasOccupiedPartner(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
const std::byte *FindAnimationDefinition(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaOwnerRow &owner);
float AreaSine(std::int32_t angle);
float AreaCosine(std::int32_t angle);
std::int32_t AreaYaw(const Vector3 &angles);
float AreaDistanceSquared(const Vector3 &first, const Vector3 &second);
Vector3 AreaLerp(const Vector3 &first, const Vector3 &second, float parameter);
std::uint32_t AreaMask(std::uint32_t bits, std::int32_t shift);
float AreaLaneWidth(const NpcAreaControlRow &row);
float AreaSegmentParameter(const Vector3 &start, const Vector3 &end, const Vector3 &position);
float AreaRowParameter(const NpcAreaControlRow &row, const NpcRouteLiveEntry &entry);
void AreaLinkedPosition(const NpcAreaOwnerRow &owner, Vector3 &position);
bool QueueAreaMotion(NpcRouteLiveEntry &entry, const Vector3 *target, const Vector3 *facing = nullptr, std::uint16_t yaw = 0, std::uint16_t motion = 0);
std::int32_t AreaQueueLength(const NpcAreaRouteState &state);
void ReserveAreaEntrance(NpcAreaRouteState &state);
bool AreaOwnerLaneAvailable(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool AreaTargetReached(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool SelectAreaExitDirection(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool QueueAreaOwnerExit(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool ProbeAreaControlRow(const NpcAreaControlRow &row, NpcAreaProbe &probe);
bool ProbeAreaOwnerRow(const NpcAreaOwnerRow &owner, NpcAreaProbe &probe);
void ProbeAreaState(NpcAreaRouteState &state);
void ReserveAreaProbe(const NpcAreaProbe &probe);
bool AreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool AreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool SelectFreeAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void UnlinkAreaStoreSlot(NpcAreaStoreSlot &slot);
void UpdateAreaDistance(NpcAreaStoreSlot &slot);
void PublishAreaWorldPosition(NpcRouteLiveEntry &entry);
bool IsAreaWaitCommand(const NpcRouteLiveEntry &entry);
bool PrepareAreaDoor(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool AreaDoorPathClear(NpcRouteLiveEntry &entry);
std::int32_t ReserveEndpointOwner(NpcAreaRouteState &state, NpcAreaOwnerRow &owner, NpcRouteLiveEntry &entry);
std::int32_t SelectEndpointOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool SelectClipAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool OccupyAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void FinishAreaRoute(NpcRouteLiveEntry &entry);
std::uint16_t SelectAreaApproachMotion(std::int32_t rig, std::uint8_t action, std::int32_t direction, bool exiting);
Vector3 SampleAreaMotion(std::uint16_t motion, std::int32_t rig, float frame);
Vector3 TransformAreaMotion(const Vector3 &origin, std::int16_t yaw, Vector3 motion);
bool IsAreaSeatedAction(std::uint8_t action);
void QueueAreaOwnerApproach(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerTarget(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaDoorMotion(NpcRouteLiveEntry &entry);
void PositionAreaQueueMember(NpcRouteLiveEntry &entry, const Vector3 &facing, const Vector3 &origin, float spacing);
void PositionAreaQueueHead(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void DisplaceAreaLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing, bool moving);
void LimitAreaCollisionRadius(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
std::int32_t AreaQueueCapacity(const NpcAreaRouteState &state);
bool AdmitAreaRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool initializing);
bool MaintainAreaQueue(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerLane(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool TickAreaTraversal(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding);
bool TickAreaOwnerApproach(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding);
bool TickAreaWaiting(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool ownerLane);
void PrepareAreaOwnerGround(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerExitMotion(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void ApplyAreaOwnerAnimation(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry, std::uint16_t motion);
void RestoreAreaMotionAttachment(NpcRouteLiveEntry &entry);
bool FinishAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool TickAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);

} // namespace

bool InitializeNpcAreaRouteStore();
void ResetNpcAreaRouteResources();
NpcAreaRouteState *GetNpcAreaPointRouteState(std::uint16_t point);
std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry);
void ActivateNpcAreaRoutes(bool active);
void PrepareNpcAreaRoutes();
std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry &entry);
std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing);
void ApplyNpcAreaMotion(NpcRouteLiveEntry &entry, std::uint16_t motion);
bool TickNpcAreaRouteState(NpcRouteLiveEntry &entry);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct NpcMotionChoice {
    std::uint16_t motions[15];
    std::uint8_t weights[5], reserved;
};

struct NpcNamedMotionChoice {
    std::uint16_t motions[5];
    std::uint8_t weights[5], reserved;
};

struct NpcAreaProbe {
    std::int8_t rowIndex, linkedRowIndex, ownerIndex, nextOwnerIndex;
    std::uint32_t retained04;
    float parameter, lateralDistance;
    NpcAreaRouteState *state;
};

typedef char Layout_npc_area_probe[(sizeof(NpcAreaProbe) == 0x14) ? 1 : -1];

NpcAreaControlRow *ControlRows(const NpcAreaRouteState &state);

std::uint32_t AreaMask(std::uint32_t bits, std::int32_t shift);

std::uint8_t ScriptByte(const std::byte *command);

std::uint16_t ScriptWord(const std::byte *command);

std::uint32_t ScriptDword(const std::byte *command);

const std::byte *NextInstruction(const std::byte *command);

const std::byte *FindInstructionEnd(const std::byte *command);

const std::byte *FindInstructionStart(const std::byte *command);

const std::byte *FindSection(const std::byte *command, std::uint8_t section);

std::uint16_t BeginAnimationInstruction(NpcRouteLiveEntry *entry, const std::byte *command);

bool HasOccupiedPartner(const NpcAreaRouteState *state, const NpcRouteLiveEntry *entry);

} // namespace

std::int32_t DisableNpcAreaRoute(NpcRouteLiveEntry *entry);

bool QueueAreaMotion(NpcRouteLiveEntry *entry, const Vector3 *target, const Vector3 *facing = 0, std::int16_t yaw = 0, std::uint16_t motion = 0);

bool PrepareNpcAreaDoor(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);

std::uint16_t RemapNpcMotionForRig(std::int32_t rig, std::uint16_t motion);

std::uint16_t SelectNpcSpecialMotion(NpcRouteLiveEntry *entry);

std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry *entry);

std::int32_t TickNpcCrowdAreaState(NpcAreaRouteState *state, NpcRouteLiveEntry *entryPointer);

NpcAreaRouteState *GetNpcAreaPointRouteState(std::int32_t point);

NpcAreaPoint *npcAreaPoints;

NpcAreaStoreSlot *npcNearestAreaSlot = 0;

std::int32_t npcAreaRouteCount = 0;

std::int32_t npcAreaQueueCapacities[4] = {3, 3, 0, 0};

extern Vector3 npcPlayerPosition;

extern float npcPlayerRadius;

extern shen_name npcNearbyDoor;

extern float npcEndOfDayWeatherForecast;

bool IsInsideNpcNoSpawnRadius(const Vector3 *position, float radius);

const Vector3 *GetNpcAreaPointPosition(std::int32_t point);

void SetCharacterWorldPosition(Character *character, const Vector3 *position);

namespace {

static const NpcMotionChoice npcSpecialProfileMotions[6][3] = {
    {{{0x809a, 0x80a1, 0x809f, 0x809e, 0x809d, 0x809a, 0x80a1, 0x809d, 0x80a0, 0x809f, 0x80d0, 0x80d1, 0x80a0, 0x809c, 0x809b}, {10, 10, 30, 25, 20}, 0},
        {{0x809a, 0x80a1, 0x809e, 0x809d, 0x80a0, 0x809a, 0x80a1, 0x809d, 0x80a0, 0x809f, 0x80d0, 0x80d1, 0x80a0, 0x809c, 0x809b}, {10, 10, 30, 25, 20}, 0},
        {{0x809a, 0x80a1, 0x809d, 0x809c, 0x80a0, 0x809a, 0x80a1, 0x809d, 0x80a0, 0x809f, 0x80d0, 0x80d1, 0x80a0, 0x809c, 0x809b}, {10, 10, 30, 25, 20}, 0}},
    {{{0x80aa, 0x80b0, 0x80ae, 0x80ad, 0x80ac, 0x80aa, 0x80b0, 0x80ac, 0x80af, 0x80ae, 0x80d5, 0x80d2, 0x80af, 0x80ae, 0x80ab}, {10, 10, 30, 25, 20}, 0},
        {{0x80aa, 0x80b0, 0x80ad, 0x80ac, 0x80af, 0x80aa, 0x80b0, 0x80ac, 0x80af, 0x80ae, 0x80d5, 0x80d2, 0x80af, 0x80ae, 0x80ab}, {10, 10, 30, 25, 20}, 0},
        {{0x80aa, 0x80b0, 0x80ac, 0x80ab, 0x80af, 0x80aa, 0x80b0, 0x80ac, 0x80af, 0x80ae, 0x80d5, 0x80d2, 0x80af, 0x80ae, 0x80ab}, {10, 10, 30, 25, 20}, 0}},
    {{{0x80b4, 0x80ba, 0x80b8, 0x80b9, 0x80b6, 0x80b4, 0x80ba, 0x80b7, 0x80b5, 0x80b6, 0x80b4, 0x80ba, 0x80b9, 0x80b5, 0x80b8}, {20, 5, 30, 25, 20}, 0},
        {{0x80b4, 0x80ba, 0x80b7, 0x80b9, 0x80b5, 0x80b4, 0x80ba, 0x80b7, 0x80b5, 0x80b6, 0x80b4, 0x80ba, 0x80b9, 0x80b5, 0x80b8}, {20, 5, 30, 25, 20}, 0},
        {{0x80b4, 0x80ba, 0x80b5, 0x80b6, 0x80b8, 0x80b4, 0x80ba, 0x80b7, 0x80b5, 0x80b6, 0x80b4, 0x80ba, 0x80b9, 0x80b5, 0x80b8}, {20, 5, 30, 25, 20}, 0}},
    {{{0x80b4, 0x80ba, 0x80b8, 0x80b9, 0x80b6, 0x80b4, 0x80ba, 0x80b7, 0x80b5, 0x80b6, 0x80b4, 0x80ba, 0x80b9, 0x80b5, 0x80b8}, {20, 5, 30, 25, 20}, 0},
        {{0x80b4, 0x80ba, 0x80b7, 0x80b9, 0x80b5, 0x80b4, 0x80ba, 0x80b7, 0x80b5, 0x80b6, 0x80b4, 0x80ba, 0x80b9, 0x80b5, 0x80b8}, {20, 5, 30, 25, 20}, 0},
        {{0x80b4, 0x80ba, 0x80b5, 0x80b6, 0x80b8, 0x80b4, 0x80ba, 0x80b7, 0x80b5, 0x80b6, 0x80b4, 0x80ba, 0x80b9, 0x80b5, 0x80b8}, {20, 5, 30, 25, 20}, 0}},
    {{{0x80c4, 0x80cb, 0x80c9, 0x80c6, 0x80c7, 0x80c4, 0x80cb, 0x80c8, 0x80ca, 0x80c5, 0x80c4, 0x80cb, 0x80c5, 0x80c9, 0x80c6}, {10, 10, 30, 25, 20}, 0},
        {{0x80c4, 0x80cb, 0x80c7, 0x80c8, 0x80c6, 0x80c4, 0x80cb, 0x80c8, 0x80ca, 0x80c5, 0x80c4, 0x80cb, 0x80c5, 0x80c9, 0x80c6}, {10, 10, 30, 25, 20}, 0},
        {{0x80c4, 0x80cb, 0x80ca, 0x80c8, 0x80c5, 0x80c4, 0x80cb, 0x80c8, 0x80ca, 0x80c5, 0x80c4, 0x80cb, 0x80c5, 0x80c9, 0x80c6}, {10, 10, 30, 25, 20}, 0}},
    {{{0x80c4, 0x80cb, 0x80c9, 0x80c6, 0x80c7, 0x80c4, 0x80cb, 0x80c8, 0x80ca, 0x80c5, 0x80c4, 0x80cb, 0x80c5, 0x80c9, 0x80c6}, {10, 10, 30, 25, 20}, 0},
        {{0x80c4, 0x80cb, 0x80c7, 0x80c8, 0x80c6, 0x80c4, 0x80cb, 0x80c8, 0x80ca, 0x80c5, 0x80c4, 0x80cb, 0x80c5, 0x80c9, 0x80c6}, {10, 10, 30, 25, 20}, 0},
        {{0x80c4, 0x80cb, 0x80c7, 0x80c8, 0x80c6, 0x80c4, 0x80cb, 0x80c8, 0x80ca, 0x80c5, 0x80c4, 0x80cb, 0x80c5, 0x80c9, 0x80c6}, {10, 10, 30, 25, 20}, 0}},
};

static const std::uint16_t npcSpecialMotionTransitions[][2] = {
    {0x80f5, 0x80da}, {0x80f2, 0x80d9}, {0x8105, 0x80d8}, {0x8104, 0x80d7}, {0x8103, 0x80d6}, {0x9309, 0x9312}, {0x9305, 0x9311}, {0, 0}};
static const shen_name npcSpecialMotionNames[] = {SHEN_NAME4('K', 'N', 'K', '_'), SHEN_NAME4('K', 'U', 'N', '_'), SHEN_NAME4('S', 'Y', 'E', '_'),
    SHEN_NAME4('R', 'E', 'N', '_'), SHEN_NAME4('J', 'O', 'Y', '_'), SHEN_NAME4('S', 'I', 'N', '_'), 0};

static const NpcNamedMotionChoice npcNamedSpecialMotions[6] = {
    {{0xe0e6, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}, 0},
    {{0x9313, 0x9313, 0x9315, 0x9316, 0x9314}, {15, 15, 30, 25, 20}, 0},
    {{0x9603, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}, 0},
    {{0x809a, 0x809e, 0x809d, 0x0, 0x0}, {30, 30, 30, 0, 0}, 0},
    {{0x9807, 0x9808, 0x0, 0x0, 0x0}, {50, 50, 0, 0, 0}, 0},
    {{0x9503, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}, 0},
};

static const std::uint16_t npcRecliningMotions[54] = {
    0xe38b,
    0xe38b,
    0xe38b,
    0xe38a,
    0xe38c,
    0xe38c,
    0xe38a,
    0xe38a,
    0xe38a,
    0xe388,
    0xe388,
    0xe388,
    0xe244,
    0xe244,
    0xe244,
    0xe387,
    0xe387,
    0xe387,
    0xe249,
    0xe249,
    0xe249,
    0xe38a,
    0xe38a,
    0xe38a,
    0xe249,
    0xe249,
    0xe249,
    0xe249,
    0xe249,
    0xe249,
    0xe249,
    0xe249,
    0xe249,
    0xe387,
    0xe387,
    0xe387,
    0xe24a,
    0xe24a,
    0xe24a,
    0xe38a,
    0xe38a,
    0xe38a,
    0xe389,
    0xe389,
    0xe389,
    0xe24a,
    0xe24a,
    0xe24a,
    0xe387,
    0xe387,
    0xe387,
    0xe389,
    0xe389,
    0xe389,
};

static NpcAreaProbe areaProbe;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool InitializeNpcAreaRouteStore();
bool InitializeNpcAreaRoute(NpcRouteLiveEntry *entry);
std::int32_t StopNpcAreaRoute(NpcRouteLiveEntry *entry);
std::int32_t ClearNpcAreaRouteForEntry(NpcRouteLiveEntry *entry);
std::int32_t DisableNpcAreaRoute(NpcRouteLiveEntry *entry);
void ResumeNpcAreaSpecialMotion(NpcRouteLiveEntry *entry);
std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);
void QueueNpcAreaDoorMotion(NpcRouteLiveEntry *entry);
void ApplyNpcAreaMotion(NpcRouteLiveEntry *entry, std::uint16_t motion);
bool QueueAreaMotion(NpcRouteLiveEntry *entry, const Vector3 *target, const Vector3 *facing = 0, std::int16_t yaw = 0, std::uint16_t motion = 0);
bool PrepareNpcAreaDoor(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);
std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry);
std::int32_t TickNpcLocalAreaState(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);
void PrepareNpcAreaRoutes();
std::int32_t TickNpcAreaRouteState(NpcRouteLiveEntry *entry);
void ActivateNpcAreaRoutes(std::int32_t active);
std::int32_t SelectNpcWeightedMotion(std::int32_t count, const std::uint8_t *weights);
std::uint16_t RemapNpcMotionForRig(std::int32_t rig, std::uint16_t motion);
std::uint16_t SelectNpcSpecialMotion(NpcRouteLiveEntry *entry);
std::uint16_t SelectNpcIdleSpecialMotion(NpcRouteLiveEntry *entry);
std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry *entry);
std::uint16_t SelectNpcLeaningProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::uint16_t SelectNpcRecliningProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::int32_t TickNpcCrowdAreaState(NpcAreaRouteState *state, NpcRouteLiveEntry *entryPointer);
std::int32_t PrepareNpcAreaRouteStore(std::int32_t count);
std::int32_t RegisterNpcAreaRouteState(NpcAreaRouteState *state, std::int32_t point);

namespace {

float AreaLaneWidth(const NpcAreaControlRow &row);
void AreaLinkedPosition(const NpcAreaOwnerRow &owner, Vector3 &position);
void ReserveAreaEntrance(NpcAreaRouteState &state);
void UpdateAreaDistance(NpcAreaStoreSlot &slot);
std::uint16_t ResolveNpcAreaOwnerAnimation(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);

inline std::uint8_t ScriptByte(const std::byte *command) {
    return static_cast<std::uint8_t>(*command);
}

inline std::uint16_t ScriptWord(const std::byte *command) {
    return *reinterpret_cast<const std::uint16_t *>(command);
}

inline std::uint32_t ScriptDword(const std::byte *command) {
    return *reinterpret_cast<const std::uint32_t *>(command);
}

inline NpcAreaControlRow *ControlRows(const NpcAreaRouteState &state) {
    return reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(state.controlRowsOffset));
}

inline NpcAreaOwnerRow *OwnerRows(const NpcAreaRouteState &state) {
    return reinterpret_cast<NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state.ownerRowsOffset));
}

inline NpcRouteLiveEntry *ListHead(const NpcAreaRouteState &state, std::int32_t list) {
    return reinterpret_cast<NpcRouteLiveEntry *>(static_cast<std::uintptr_t>(state.npcListHeadOffset[list]));
}

inline void SetListHead(NpcAreaRouteState &state, NpcRouteLiveEntry *entry, std::int32_t list) {
    state.npcListHeadOffset[list] = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(entry));
}

inline std::int32_t AreaQueueCapacity(const NpcAreaRouteState &state) {
    const std::int32_t capacity = npcAreaQueueCapacities[g_actNumber];
    return state.flags & 1U ? capacity : state.flags & 0x20U ? 0 : capacity != 0;
}

inline std::int32_t AreaEndpoint(const NpcRouteLiveEntry &entry) {
    return *reinterpret_cast<const std::int32_t *>(entry.scriptRecord + 0x10);
}

inline bool IsAreaWaitCommand(const NpcRouteLiveEntry &entry) {
    const std::int32_t command = static_cast<std::int32_t>(ScriptDword(entry.currentCommand));
    return entry.waitDuration != 0 || command == -1 || command == 1;
}

std::int32_t ReserveEndpointOwner(NpcAreaRouteState &state, NpcAreaOwnerRow &owner, NpcRouteLiveEntry &entry);
std::int32_t SelectEndpointOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool SelectFreeAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool SelectClipAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool MaintainAreaQueue(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void RestoreAreaMotionAttachment(NpcRouteLiveEntry &entry);
std::uint32_t AreaMask(std::uint32_t bits, std::int32_t shift);
std::int32_t ProbeAreaState(NpcAreaRouteState &state);
void UpdateNearestAreaProbe();
void ReserveAreaProbe(NpcAreaRouteState &state);

template <class T> inline T *HeapPointer(std::int32_t offset) {
    return reinterpret_cast<T *>(static_cast<std::uintptr_t>(offset));
}

inline Vector3 AreaToScene(const Vector3 &position) {
    Vector3 result = position;
    result.x -= npcRouteOrigin.x;
    result.z -= npcRouteOrigin.z;
    return result;
}

inline float AreaSine(std::int32_t angle) {
    return dc::sdk::SinFixed(angle);
}

inline float AreaCosine(std::int32_t angle) {
    return dc::sdk::CosFixed(angle);
}

float AreaSegmentParameter(const Vector3 &start, const Vector3 &end, const Vector3 &position);
float AreaRowParameter(const NpcAreaControlRow &row, const NpcRouteLiveEntry &entry);
bool AreaTargetReached(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool AreaOpposingLaneOccupied(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
std::int32_t ReserveAreaLane(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool SelectAreaExitDirection(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
std::int32_t AreaQueueLength(const NpcAreaRouteState &state);
bool OccupyAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool AreaDoorPathClear(NpcRouteLiveEntry &entry);
std::int32_t ClaimAreaEntrance(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void PublishAreaWorldPosition(NpcRouteLiveEntry &entry);
void PositionAreaQueueMember(NpcRouteLiveEntry &entry, const Vector3 &facing, const Vector3 &origin, float spacing);
void PositionAreaQueueHead(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void PositionAreaQueueFollower(NpcRouteLiveEntry &entry);
bool AreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool AreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
Vector3 AreaLerp(const Vector3 &first, const Vector3 &second, float parameter);
std::uint16_t SelectAreaApproachMotion(std::int32_t rig, std::uint8_t action, std::uint8_t direction);
std::uint16_t SelectAreaExitApproachMotion(std::int32_t rig, std::uint8_t action, std::uint8_t direction);
Vector3 SampleAreaMotion(std::uint16_t motion, std::int32_t rig, float frame);
Vector3 TransformAreaMotion(const Vector3 &origin, std::int16_t yaw, const Vector3 &motion);
bool IsAreaSeatedAction(std::uint8_t action);
bool AreaOwnerLaneAvailable(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
std::int32_t DisplaceAreaLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing, bool moving);
void PrepareAreaOwnerGround(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerApproach(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerExitMotion(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void ApplyAreaOwnerAnimation(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry, std::uint16_t motion);
bool FinishAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
const std::byte *NextInstruction(const std::byte *command);
const std::byte *PreviousInstruction(const std::byte *command);
const std::byte *FindInstructionEnd(const std::byte *command);
const std::byte *FindInstructionStart(const std::byte *command);
const std::byte *FindSection(const std::byte *command, std::uint8_t section);
const std::byte *ChooseMotionInstruction(NpcRouteLiveEntry *entry, const std::byte *command);
std::uint16_t BeginAnimationInstruction(NpcRouteLiveEntry *entry, const std::byte *command);
bool HasOccupiedPartner(const NpcAreaRouteState *state, const NpcRouteLiveEntry *entry);

} // namespace
#else
bool InitializeNpcAreaRouteStore();
bool BindNpcAreaRouteResources(std::span<NpcAreaPoint> points, const Vector3 &areaOrigin);
void SetNpcAreaRouteOrigin(const Vector3 &origin);
void ResetNpcAreaRouteResources();
void FreeNpcAreaRouteStore();
NpcAreaRouteState *GetNpcAreaPointRouteState(std::uint16_t point);
std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing);
void ApplyNpcAreaMotion(NpcRouteLiveEntry &entry, std::uint16_t motion);
bool RequestNpcAreaRouteCompletion(NpcRouteLiveEntry &entry);
std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry);
void PrepareNpcAreaRoutes();
bool ResetNpcAreaRouteState(NpcRouteLiveEntry &entry);
bool TryEnterNpcAreaRoute(NpcRouteLiveEntry &entry);
bool TickNpcAreaRouteState(NpcRouteLiveEntry &entry);
void RestoreNpcAreaMotion(NpcRouteLiveEntry &entry);
void ActivateNpcAreaRoutes(bool active);
std::uint32_t GetNpcAreaRouteStoreCount();
std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry &entry);
std::uint16_t npcResolveAnimationForRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);

namespace {

float AreaLaneWidth(const NpcAreaControlRow &row);
void AreaLinkedPosition(const NpcAreaOwnerRow &owner, Vector3 &position);
void ReserveAreaEntrance(NpcAreaRouteState &state);
void UpdateAreaDistance(NpcAreaStoreSlot &slot);
bool AreaUsesSpecialAttachment(const NpcRouteLiveEntry &entry);
void ApplyAreaTransform(NpcRouteLiveEntry &entry);
bool QueueAreaMotion(NpcRouteLiveEntry &entry, const Vector3 *target, const Vector3 *facing, std::uint16_t yaw, std::uint16_t motion);
NpcAreaControlRow *ControlRows(const NpcAreaRouteState &state);
NpcAreaOwnerRow *OwnerRows(const NpcAreaRouteState &state);
NpcRouteLiveEntry *ListHead(const NpcAreaRouteState &state, std::size_t list);
void SetListHead(NpcAreaRouteState &state, NpcRouteLiveEntry *entry, std::size_t list);
void RegisterAreaState(NpcAreaRouteState &state, std::int16_t point);
std::int32_t AreaQueueCapacity(const NpcAreaRouteState &state);
bool IsAreaWaitCommand(const NpcRouteLiveEntry &entry);
bool IsAreaEndpoint(const NpcRouteLiveEntry &entry);
bool PrepareAreaDoor(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
std::int32_t ReserveEndpointOwner(NpcAreaRouteState &state, NpcAreaOwnerRow &owner, NpcRouteLiveEntry &entry);
std::int32_t SelectEndpointOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool SelectFreeAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void AdvanceAreaMasks(NpcAreaRouteState &state);
void UnlinkAreaStoreSlot(NpcAreaStoreSlot &slot);
bool SelectClipAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool MaintainAreaQueue(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void RestoreAreaMotionAttachment(NpcRouteLiveEntry &entry);
std::uint32_t AreaMask(std::uint32_t bits, std::int32_t shift);
void ProbeAreaState(NpcAreaRouteState &state);
void ReserveAreaProbe(const NpcAreaProbe &probe);
bool IsAreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaProbe &probe);
template <class T> T *HeapPointer(std::int32_t offset);
std::int32_t HeapOffset(const void *pointer);
float AreaSine(std::int32_t angle);
float AreaCosine(std::int32_t angle);
std::int32_t AreaYaw(const Vector3 &angles);
void SetAreaYaw(Vector3 &angles, std::int32_t yaw);
float AreaDistanceSquared(const Vector3 &first, const Vector3 &second);
float AreaSegmentParameter(const Vector3 &start, const Vector3 &end, const Vector3 &position);
float AreaRowParameter(const NpcAreaControlRow &row, const NpcRouteLiveEntry &entry);
bool AreaTargetReached(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool AreaOpposingLaneOccupied(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool ReserveAreaLane(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool SelectAreaExitDirection(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool QueueAreaOwnerExit(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool ProbeAreaControlRow(const NpcAreaControlRow &row, NpcAreaProbe &probe);
bool ProbeAreaOwnerRow(const NpcAreaOwnerRow &owner, NpcAreaProbe &probe);
void AdvanceAreaControlRow(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerLane(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
std::int32_t AreaQueueLength(const NpcAreaRouteState &state);
bool OccupyAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void FinishAreaRoute(NpcRouteLiveEntry &entry);
bool AreaDoorPathClear(NpcRouteLiveEntry &entry);
bool ClaimAreaEntrance(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void PublishAreaWorldPosition(NpcRouteLiveEntry &entry);
void PositionAreaQueueMember(NpcRouteLiveEntry &entry, const Vector3 &facing, const Vector3 &origin, float spacing);
void PositionAreaQueueHead(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool AreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
bool IsAreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaProbe &probe);
bool AreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
Vector3 AreaLerp(const Vector3 &first, const Vector3 &second, float parameter);
std::uint16_t SelectAreaApproachMotion(std::int32_t rig, std::uint8_t action, std::int32_t direction, bool exiting);
Vector3 SampleAreaMotion(std::uint16_t motion, std::int32_t rig, float frame);
Vector3 TransformAreaMotion(const Vector3 &origin, std::int16_t yaw, Vector3 motion);
bool IsAreaSeatedAction(std::uint8_t action);
void LimitAreaCollisionRadius(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool AreaOwnerLaneAvailable(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
void DisplaceAreaLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing, bool moving);
void PrepareAreaOwnerGround(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerApproach(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaOwnerTarget(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void EnterAreaOwnerLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void QueueAreaDoorMotion(NpcRouteLiveEntry &entry);
bool TickAreaTraversal(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding);
bool TickAreaOwnerApproach(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding);
bool TickAreaWaiting(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool ownerLane);
void QueueAreaOwnerExitMotion(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void ApplyAreaOwnerAnimation(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry, std::uint16_t motion);
bool FinishAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
void RestoreAreaSavedPosition(NpcRouteLiveEntry &entry);
bool TickAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool TickInactiveAreaRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
bool AdmitAreaRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool initializing);
std::uint8_t ScriptByte(const std::byte *command);
std::uint16_t ScriptWord(const std::byte *command);
std::uint32_t ScriptDword(const std::byte *command);
const std::byte *NextInstruction(const std::byte *command);
const std::byte *PreviousInstruction(const std::byte *command);
bool IsScriptSectionEnd(std::uint8_t opcode);
const std::byte *FindInstructionEnd(const std::byte *command);
const std::byte *FindInstructionStart(const std::byte *command);
const std::byte *FindSection(const std::byte *command, std::uint8_t section);
const std::byte *ChooseMotionInstruction(NpcRouteLiveEntry &entry, const std::byte *command);
std::uint16_t BeginAnimationInstruction(NpcRouteLiveEntry &entry, const std::byte *command);
bool HasOccupiedPartner(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry);
const std::byte *FindAnimationDefinition(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaOwnerRow &owner);

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint16_t RemapNpcMotionForRig(std::int32_t rig, std::uint16_t motion) {
    if (static_cast<std::uint16_t>(motion + 0x2000U) < 0x386U) {
        const std::uint16_t next = static_cast<std::uint16_t>(motion + 0x1fffU);
        if (static_cast<std::uint16_t>(next % 17U) == 0)
            return static_cast<std::uint16_t>(next / 17U * 17U + (rig + 1) - 0x1fff);
    }
    return motion;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SelectNpcWeightedMotion(std::int32_t count, const std::uint8_t *weights) {
    std::int32_t total = 0;
    std::int32_t index;
    for (index = 0; index < count; ++index)
        total += weights[index];
    std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(total) * dc::sdk::RandomFloat());
    for (index = 0; index < count; ++index) {
        choice -= weights[index];
        if (choice < 1)
            return index;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SelectNpcSpecialMotion(NpcRouteLiveEntry *entry, std::int32_t age, std::int32_t sex, std::int32_t profile) {
    std::int32_t type;
    if (entry->node) {
        if (entry->motionWork && mtwkHasRegionTracks(entry->motionWork)) {
            const std::uint16_t current = mtwkGetAnimationId(entry->motionWork, LowerBody);
            std::uint16_t motion;
            for (std::int32_t index = 0; (motion = npcSpecialMotionTransitions[index][0]) != 0; ++index)
                if (motion == current)
                    return npcSpecialMotionTransitions[index][1];
            return 0;
        }
        type = GetCharacterImageSourceType(entry->node) / 32;
    } else
        type = 0;
    shen_name name;
    for (std::int32_t index = 0; (name = npcSpecialMotionNames[index]) != 0; ++index)
        if (name == entry->name) {
            const std::int32_t choice = SelectNpcWeightedMotion(2, npcNamedSpecialMotions[index].weights);
            return npcNamedSpecialMotions[index].motions[choice];
        }
    std::int32_t group;
    if (age < 13)
        group = 2;
    else
        group = age > 55 ? 4 : 0;
    if (sex)
        ++group;
    const NpcMotionChoice &choices = npcSpecialProfileMotions[group][profile];
    return choices.motions[type * 5 + SelectNpcWeightedMotion(2, choices.weights)];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SelectNpcIdleSpecialMotion(NpcRouteLiveEntry *entry, std::int32_t age, std::int32_t sex, std::int32_t profile) {
    std::int32_t type;
    if (entry->node) {
        if (entry->motionWork && mtwkHasRegionTracks(entry->motionWork)) {
            const std::uint16_t current = mtwkGetAnimationId(entry->motionWork, LowerBody);
            std::uint16_t motion;
            for (std::int32_t index = 0; (motion = npcSpecialMotionTransitions[index][0]) != 0; ++index)
                if (motion == current)
                    return npcSpecialMotionTransitions[index][1];
            return 0;
        }
        type = GetCharacterImageSourceType(entry->node) / 32;
    } else
        type = 0;
    shen_name name;
    for (std::int32_t index = 0; (name = npcSpecialMotionNames[index]) != 0; ++index)
        if (name == entry->name) {
            const std::int32_t choice = SelectNpcWeightedMotion(5, npcNamedSpecialMotions[index].weights);
            return npcNamedSpecialMotions[index].motions[choice];
        }
    std::int32_t group;
    if (age < 13)
        group = 2;
    else
        group = age > 55 ? 4 : 0;
    if (sex)
        ++group;
    const NpcMotionChoice &choices = npcSpecialProfileMotions[group][profile];
    return choices.motions[type * 5 + SelectNpcWeightedMotion(5, choices.weights)];
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t ScriptByte(const std::byte *command) {
    return static_cast<std::uint8_t>(*command);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint16_t ScriptWord(const std::byte *command) {
    std::uint16_t value;
    std::memcpy(&value, command, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t ScriptDword(const std::byte *command) {
    std::uint32_t value;
    std::memcpy(&value, command, sizeof(value));
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool InitializeNpcAreaRouteStore() {
    npcAreaRouteStore = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(kNpcAreaRouteMemoryBytes, NPC_AREA_ROUTE_MEMORY));
    if (!npcAreaRouteStore)
        return false;
    HLib::SetTaskDataMotionFlag(npcAreaRouteStore);
    std::memset(npcAreaRouteStore, 0, kNpcAreaRouteMemoryBytes);
    return true;
}

#else

// Native AllocateSTRL (npcInitDataInternal): once per act; a previous act's block went with the heap reset.
bool InitializeNpcAreaRouteStore() {
    areaStore = static_cast<NpcAreaStoreSlot *>(HLib::AllocateStaticOrDynamicBlock(0x2000, SHEN_NAME(STRL)));
    if (!areaStore)
        return false;
    HLib::SetTaskDataMotionFlag(areaStore);
    std::memset(areaStore, 0, 0x2000);
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool BindNpcAreaRouteResources(std::span<NpcAreaPoint> points, const Vector3 &areaOrigin) {
    if (!areaStore)
        return false;
    const std::size_t count =
        static_cast<std::size_t>(std::count_if(points.begin(), points.end(), [](const NpcAreaPoint &point) { return point.routeStateOffset != 0; }));
    if (count > 256)
        return false;
    ResetNpcAreaRouteResources();
    areaPoints = points;
    routeOrigin = areaOrigin;
    auto *const table = reinterpret_cast<std::byte *>(points.data());
    for (NpcAreaPoint &point : points) {
        if (point.routeStateOffset)
            point.routeStateOffset = HeapOffset(table + point.routeStateOffset);
    }
    for (std::size_t index = 0; index < points.size(); ++index) {
        if (NpcAreaRouteState *const state = GetNpcAreaPointRouteState(static_cast<std::uint16_t>(index))) {
            RegisterAreaState(*state, static_cast<std::int16_t>(index));
        }
    }
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetNpcAreaRouteOrigin(const Vector3 &origin) {
    routeOrigin = origin;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetNpcAreaRouteResources() {
    areaPoints = {};
    nearestAreaSlot = nullptr;
    areaStoreCount = 0;
    routeOrigin = {};
    if (areaStore)
        std::memset(areaStore, 0, 0x2000);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void FreeNpcAreaRouteStore() {
    ResetNpcAreaRouteResources();
    HLib::FreeTaskData(areaStore);
    areaStore = nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

NpcAreaRouteState *GetNpcAreaPointRouteState(std::uint16_t point) {
    if (point >= areaPoints.size())
        return nullptr;

    return HeapPointer<NpcAreaRouteState>(areaPoints[point].routeStateOffset);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t PrepareNpcAreaRouteStore(std::int32_t count) {
    npcAreaRouteCount = count;
    npcNearestAreaSlot = 0;
    if (!count)
        return 0;
    if (count > kNpcAreaRouteSlots)
        return 0;
    std::memset(npcAreaRouteStore, 0, kNpcAreaRouteMemoryBytes);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t RegisterNpcAreaRouteState(NpcAreaRouteState *state, std::int32_t point) {
    if (!npcAreaRouteStore || !state)
        return 0;
    std::int32_t index = 0;
    while (reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index].state)
        ++index;
    NpcAreaStoreSlot *const store = reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore);
    NpcAreaStoreSlot *const slot = reinterpret_cast<NpcAreaStoreSlot *>(index * sizeof(NpcAreaStoreSlot) + reinterpret_cast<std::uintptr_t>(store));
    store[index].state = state;
    if (index) {
        slot[-1].next = slot;
        slot->previous = &store[index - 1];
    } else if (!npcNearestAreaSlot) {
        npcNearestAreaSlot = reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore) + index;
    }
    if (state->animationScriptOffset)
        state->animationScriptOffset =
            static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(state) + static_cast<std::uint32_t>(state->animationScriptOffset));
    state->controlRowsOffset = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(state) + static_cast<std::uint32_t>(state->controlRowsOffset));
    state->ownerRowsOffset = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(state) + static_cast<std::uint32_t>(state->ownerRowsOffset));
    NpcAreaControlRow *controls = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(state->controlRowsOffset));
    for (index = 0; index < state->controlRowCount; ++index, ++controls) {
        if (controls->forwardOffset)
            controls->forwardOffset = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(state) + static_cast<std::uint32_t>(controls->forwardOffset));
        if (controls->backwardOffset)
            controls->backwardOffset =
                static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(state) + static_cast<std::uint32_t>(controls->backwardOffset));
    }
    state->areaPointIndex = point;
    state->field0a = 0;
    state->npcListHeadOffset[1] = 0;
    state->npcListHeadOffset[0] = 0;
    state->freeRowCount = 0;
    state->activeRowCount = 0;
    state->occupiedRowCount = 0;
    NpcAreaOwnerRow *owners = reinterpret_cast<NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state->ownerRowsOffset));
    for (index = 0; index < state->ownerRowCount; ++index, ++owners) {
        if (owners->linkedControlOffset) {
            owners->linkedControlOffset =
                static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(state) + static_cast<std::uint32_t>(owners->linkedControlOffset));
            if ((*reinterpret_cast<const std::uint32_t *>(owners) & 3U) != 1U)
                ++state->freeRowCount;
        }
        owners->runtimeMask = 0;
    }
    state->flags |= 8U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void QueueNpcAreaDoorMotion(NpcRouteLiveEntry *entry) {
    if (!entry->node)
        return;
    Vector3 position = entry->stateContext.position;
    position.x -= npcRouteOrigin.x;
    position.z -= npcRouteOrigin.z;
    entry->stateContext.doorMotion = GetNpcDoorAnimation(static_cast<std::int32_t>(entry->stateContext.otherName), entry->node, &position);
    if (entry->stateContext.otherName != g_doorIngress.selectedDoor) {
        mtwkCopyTransition(entry->motionWork, 0, entry->rotation.y, 13, 0);
    } else {
        if (entry->stateContext.doorMotion) {
            Vector3 track;
            SampleNativeMotionRoot(entry->stateContext.doorMotion, charGetMotionRigType(entry->node), 15.0f, &track);
            matrix_stack::PushUnitMatrix();
            Matrix4x4::SetViewMatrixFromEuler(0, entry->stateContext.targetYaw, 0);
            Matrix4x4::TransformPoint(track, track);
            PopCurrentMatrix(1);
            position.x += track.x;
            position.z += track.z;
        }
        AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) & ~0x80U);
        AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) | 0x800U);
        entry->commandFlags |= 1U;
        mtwkCopyTransition(entry->motionWork, &position, entry->stateContext.targetYaw, 6, 0);
        mtwkCopyTransition(entry->motionWork, 0, 0, 2, 0);
        if (entry->stateContext.otherName && entry->stateContext.doorMotion) {
            mtwkSetTimeRelated(entry->motionWork, 1, 5, entry->stateContext.doorMotion);
            mtwkSetRegionEvalTime(entry->motionWork, 14.0f, LowerBody);
        }
        position = entry->stateContext.doorPosition;
        position.x -= npcRouteOrigin.x;
        position.z -= npcRouteOrigin.z;
        mtwkUpdateWithPosition(entry->motionWork, &position, 0);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool InitializeNpcAreaRoute(NpcRouteLiveEntry *entry) {
    bool blocked = false;
    if (!entry)
        return false;
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry->currentAreaPoint);
    if (!state || entry->routeMotionState != 23)
        return false;
    if (entry->currentAreaPoint != *reinterpret_cast<const std::int32_t *>(entry->scriptRecord + 0x10)) {
        if (!(npcEndOfDayWeatherForecast < 0.15f) && (state->flags & 0x40U))
            blocked = true;
        if ((state->flags & 1U) && !IsDoorOpeningAllowed(GetNpcAreaPointOwnerName(state->areaPointIndex)))
            blocked = true;
    }
    if (blocked) {
        if (!entry->waitDuration && *reinterpret_cast<const std::int32_t *>(entry->currentCommand) != -1 &&
            *reinterpret_cast<const std::int32_t *>(entry->currentCommand) != 1)
            return false;
        if (!entry->node)
            return false;
        mtwkCopyTransition(entry->motionWork, 0, entry->rotation.y, 13, 0);
    } else {
        entry->routeMotionState = 3;
        return true;
    }
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t StopNpcAreaRoute(NpcRouteLiveEntry *entry) {
    if (!entry)
        return 0;
    if (entry->opcodeFlags & 2U) {
        switch (entry->routeMotionState) {
        case 16:
            entry->retained2d0 = 1000;
            return 1;
        case 1:
            ReleaseDoor(entry->stateContext.otherName, entry->name);
            if (entry->node)
                mtwkSetTimeRelated(entry->motionWork, 1, 5, 0);
        case 4:
            DisableNpcAreaRoute(entry);
            return 1;
        case 5:
            entry->opcodeFlags &= ~2U;
            PrepareNpcAreaDoor(GetNpcAreaPointRouteState(entry->currentAreaPoint), entry);
            if (entry->stateContext.otherName) {
                entry->commandFlags &= ~2U;
                entry->routeMotionState = 0;
            } else {
                entry->commandFlags |= 2U;
                if (QueueAreaMotion(entry, GetNpcAreaPointPosition(entry->currentAreaPoint), 0, 0, 0))
                    mtwkCopyTransition(entry->motionWork, 0, 0, 2, 0);
                entry->routeMotionState = 22;
            }
            break;
        case 2:
        case 7:
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            entry->commandFlags |= 0x20U;
            break;
        case 19:
            break;
        default:
            return 0;
        }
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ClearNpcAreaRouteForEntry(NpcRouteLiveEntry *entry) {
    if (!entry)
        return 0;
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry->currentAreaPoint);
    if (entry->areaPointPrevious) {
        entry->areaPointPrevious->areaPointNext = entry->areaPointNext;
        if (entry->areaPointNext)
            entry->areaPointNext->areaPointPrevious = entry->areaPointPrevious;
    } else if (entry->areaPointNext)
        entry->areaPointNext->areaPointPrevious = 0;
    if (state && static_cast<std::uint32_t>(state->npcListHeadOffset[0]) == reinterpret_cast<std::uintptr_t>(entry))
        state->npcListHeadOffset[0] = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(entry->areaPointNext));
    entry->areaPointPrevious = entry->areaPointNext = 0;
    if (entry->primarySlotIndex != -1) {
        if (state) {
            NpcAreaOwnerRow *const owners = reinterpret_cast<NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state->ownerRowsOffset));
            if (owners[entry->primarySlotIndex].runtimeMask & 1U) {
                owners[entry->primarySlotIndex].runtimeMask &= ~1U;
                if (owners[entry->primarySlotIndex].linkedControlOffset &&
                    (*reinterpret_cast<const std::uint32_t *>(&owners[entry->primarySlotIndex]) & 3U) != 1U) {
                    --state->activeRowCount;
                    ++state->freeRowCount;
                }
            }
            if (entry->ownerRowIndex != -1 && (owners[entry->ownerRowIndex].runtimeMask & 2U))
                owners[entry->ownerRowIndex].runtimeMask &= ~2U;
            if (entry->routeMotionState == 16)
                --state->occupiedRowCount;
        }
        entry->primarySlotIndex = -1;
    }
    entry->routeMotionState = 23;
    entry->commandFlags = 0;
    entry->opcodeFlags = 0;
    entry->stateContext.otherName = 0;
    entry->specialAnimationContext = 0;
    entry->animationRepeats = entry->animationRepeatGroups = 0;
    entry->retained2c6 = 0;
    entry->flags &= ~0x100U;
    if (entry->streamCost != 2)
        entry->streamCost = 2;
    entry->motionAttachments[0].flags &= ~0x40U;
    entry->motionAttachments[2].flags &= ~0x40U;
    entry->opcodeByte = 0;
    entry->waitParameter = 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t DisableNpcAreaRoute(NpcRouteLiveEntry *entry) {
    if (!entry)
        return 0;
    if (entry->node) {
        AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) | 4U);
        if (IsInsideNpcNoSpawnRadius(&entry->localPosition, 0.40000001f))
            BeginNpcRouteStreamOut(entry, 0);
    }
    return ClearNpcAreaRouteForEntry(entry);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
float AreaLaneWidth(const NpcAreaControlRow &row) {
    switch (static_cast<std::int8_t>(row.laneMode)) {
    case 0:
        return 0.34999999f;
    case 1:
        return 0.5f;
    case 2:
        return 1.0f;
    default:
        return 0.0f;
    }
}

} // namespace

#else

namespace {

float AreaLaneWidth(const NpcAreaControlRow &row) {
    switch (row.laneMode) {
    case 0:
        return 0.34999999f;
    case 1:
        return 0.5f;
    case 2:
        return 1.0f;
    default:
        return 0.0f;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AreaLinkedPosition(const NpcAreaOwnerRow &owner, Vector3 &position) {
    if (!owner.linkedControlOffset)
        return;
    if (reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->forwardOffset) {
        position.x = owner.positionAlongSegment *
                reinterpret_cast<const NpcAreaControlRow *>(
                    static_cast<std::uintptr_t>(
                        reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->forwardOffset))
                    ->position.x +
            (1.0f - owner.positionAlongSegment) *
                reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->position.x;
        position.y = owner.positionAlongSegment *
                reinterpret_cast<const NpcAreaControlRow *>(
                    static_cast<std::uintptr_t>(
                        reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->forwardOffset))
                    ->position.y +
            (1.0f - owner.positionAlongSegment) *
                reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->position.y;
        position.z = owner.positionAlongSegment *
                reinterpret_cast<const NpcAreaControlRow *>(
                    static_cast<std::uintptr_t>(
                        reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->forwardOffset))
                    ->position.z +
            (1.0f - owner.positionAlongSegment) *
                reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->position.z;
    } else {
        position = reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->position;
    }
}

} // namespace

#else

namespace {

void AreaLinkedPosition(const NpcAreaOwnerRow &owner, Vector3 &position) {
    const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
    if (!row)
        return;
    const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row->forwardOffset);
    position = next ? AreaLerp(row->position, next->position, owner.positionAlongSegment) : row->position;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool AreaTargetReached(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow *const controls = ControlRows(state);
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const Vector3 position = entry.localPosition;
    Vector3 target;
    float radius;
    if (entry.nextOwnerRowIndex != -1) {
        if (entry.controlRowIndex != -1) {
            if (!controls[entry.controlRowIndex].forwardOffset)
                return true;
            float parameter;
            const NpcAreaControlRow *segment;
            if (entry.opcodeFlags & 1U) {
                parameter = AreaRowParameter(controls[entry.controlRowIndex], entry);
                segment = &controls[entry.controlRowIndex];
            } else {
                parameter = AreaRowParameter(*HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset), entry);
                segment = HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset);
            }
            radius = 0.5f / segment->length;
            if (entry.opcodeFlags & 1U) {
                if (parameter > owners[entry.nextOwnerRowIndex].positionAlongSegment - radius)
                    return true;
                return false;
            }
            if (parameter < owners[entry.nextOwnerRowIndex].positionAlongSegment + radius)
                return true;
            return false;
        }
        if (entry.commandFlags & 8U) {
            if (mtwkSetExtraFlag(entry.motionWork, 58, 1))
                return false;
            return true;
        }
        target = owners[entry.nextOwnerRowIndex].position;
        radius = 0.34999999f;
    } else if (entry.ownerRowIndex != -1) {
        AreaLinkedPosition(owners[entry.ownerRowIndex], target);
        radius = AreaLaneWidth(controls[entry.nextControlRowIndex]);
        if (controls[entry.nextControlRowIndex].forwardOffset &&
            static_cast<std::int8_t>(controls[entry.nextControlRowIndex].laneMode) >
                static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[entry.nextControlRowIndex].forwardOffset)->laneMode))
            radius = AreaLaneWidth(*HeapPointer<NpcAreaControlRow>(controls[entry.nextControlRowIndex].forwardOffset));
    } else {
        target = controls[entry.nextControlRowIndex].position;
        radius = AreaLaneWidth(controls[entry.nextControlRowIndex]);
    }
    target.x -= npcRouteOrigin.x;
    target.z -= npcRouteOrigin.z;
    Vector3 delta;
    delta.x = target.x - position.x;
    delta.y = 0.0f;
    delta.z = target.z - position.z;
    if (Vector3::CalcMagnitude(delta) < radius)
        return true;
    return false;
}

} // namespace

#else

namespace {

bool AreaTargetReached(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow *const controls = ControlRows(state);
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    Vector3 target;
    float radius;
    if (entry.nextOwnerRowIndex != -1) {
        if (entry.controlRowIndex != -1) {
            const NpcAreaControlRow &current = controls[entry.controlRowIndex];
            if (!current.forwardOffset)
                return true;
            if (entry.opcodeFlags & 1U) {
                return AreaRowParameter(current, entry) > owners[entry.nextOwnerRowIndex].positionAlongSegment - 0.5f / current.length;
            }
            const NpcAreaControlRow &previous = *HeapPointer<NpcAreaControlRow>(current.backwardOffset);
            return AreaRowParameter(previous, entry) < owners[entry.nextOwnerRowIndex].positionAlongSegment + 0.5f / previous.length;
        }
        if (entry.commandFlags & 8U)
            return !mtwkSetExtraFlag(entry.motionWork, 58, 1);
        target = owners[entry.nextOwnerRowIndex].position;
        radius = 0.34999999f;
    } else if (entry.ownerRowIndex != -1) {
        AreaLinkedPosition(owners[entry.ownerRowIndex], target);
        const NpcAreaControlRow &row = controls[entry.nextControlRowIndex];
        radius = AreaLaneWidth(row);
        if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset); next && row.laneMode > next->laneMode)
            radius = AreaLaneWidth(*next);
    } else {
        const NpcAreaControlRow &row = controls[entry.nextControlRowIndex];
        target = row.position;
        radius = AreaLaneWidth(row);
    }
    return AreaDistanceSquared(NpcRouteAreaToScene(target), entry.localPosition) < radius * radius;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool SelectAreaExitDirection(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t owner = entry.ownerRowIndex;
    while (!owners[owner].linkedControlOffset) {
        if (!(owners[owner].flags & 8U))
            return false;
        --owner;
    }
    const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owners[owner].linkedControlOffset);
    if (entry.opcodeFlags & 2U) {
        std::int32_t target = entry.primarySlotIndex;
        while (!owners[target].linkedControlOffset) {
            if (!(owners[target].flags & 8U))
                return false;
            --target;
        }
        const NpcAreaControlRow *const targetRow = HeapPointer<NpcAreaControlRow>(owners[target].linkedControlOffset);
        if (static_cast<std::int8_t>(row->index) < static_cast<std::int8_t>(targetRow->index))
            entry.opcodeFlags |= 1U;
        else if (static_cast<std::int8_t>(row->index) > static_cast<std::int8_t>(targetRow->index))
            entry.opcodeFlags &= ~1U;
        else if (owners[owner].positionAlongSegment > owners[target].positionAlongSegment)
            entry.opcodeFlags &= ~1U;
        else
            entry.opcodeFlags |= 1U;
    } else if (state.flags & 4U) {
        if ((static_cast<std::int8_t>(row->index) + 1) * 2 < state.controlRowCount + 1)
            entry.opcodeFlags &= ~1U;
        else if ((static_cast<std::int8_t>(row->index) + 1) * 2 > state.controlRowCount + 1)
            entry.opcodeFlags |= 1U;
        else if (owners[owner].positionAlongSegment > 0.5f)
            entry.opcodeFlags |= 1U;
        else
            entry.opcodeFlags &= ~1U;
    } else {
        if (state.flags & 2U) {
            if (entry.opcodeFlags & 4U) {
                if (entry.opcodeFlags & 8U) {
                    entry.opcodeFlags |= 1U;
                    return true;
                }
            }
        }
        entry.opcodeFlags &= ~1U;
    }
    return true;
}

} // namespace

#else

namespace {

bool SelectAreaExitDirection(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow *owner = owners + entry.ownerRowIndex;
    while (!owner->linkedControlOffset) {
        if (!(owner->flags & 8U))
            return false;
        --owner;
    }
    const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner->linkedControlOffset);
    bool forward;
    if (entry.opcodeFlags & 2U) {
        const NpcAreaOwnerRow *target = owners + entry.primarySlotIndex;
        while (!target->linkedControlOffset) {
            if (!(target->flags & 8U))
                return false;
            --target;
        }
        const NpcAreaControlRow &targetRow = *HeapPointer<NpcAreaControlRow>(target->linkedControlOffset);
        forward = row.index < targetRow.index || (row.index == targetRow.index && owner->positionAlongSegment <= target->positionAlongSegment);
    } else if (state.flags & 4U) {
        const std::int32_t current = 2 * static_cast<std::int8_t>(row.index) + 2;
        const std::int32_t middle = state.controlRowCount + 1;
        forward = current > middle || (current == middle && owner->positionAlongSegment > 0.5f);
    } else
        forward = (state.flags & 2U) && (entry.opcodeFlags & 4U) && (entry.opcodeFlags & 8U);
    if (forward)
        entry.opcodeFlags |= 1U;
    else
        entry.opcodeFlags &= ~1U;
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool QueueAreaOwnerExit(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    if (const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset)) {
        entry.nextOwnerRowIndex = -1;
        entry.nextControlRowIndex = static_cast<std::int8_t>(row->index);
        entry.commandFlags |= 2U;
        Vector3 target;
        AreaLinkedPosition(owner, target);
        QueueAreaMotion(entry, &target);
        entry.routeMotionState = 9;
        return false;
    }
    if (owner.flags & 8U) {
        entry.commandFlags |= 2U;
        entry.nextOwnerRowIndex = static_cast<std::int8_t>(entry.ownerRowIndex - 1);
        QueueAreaMotion(entry, &OwnerRows(state)[entry.nextOwnerRowIndex].position);
        return false;
    }
    entry.commandFlags &= ~2U;
    QueueAreaMotion(entry, nullptr);
    entry.routeMotionState = 17;
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool ProbeAreaControlRow(const NpcAreaControlRow &row, NpcAreaProbe &probe) {
    const float x = npcPlayerPosition.x + routeOrigin.x - row.position.x;
    const float z = npcPlayerPosition.z + routeOrigin.z - row.position.z;
    const float playerRadius = std::fmax(0.40000001f, npcPlayerRadius);
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
    const float distance = std::sqrt(x * x + z * z);
    const float radius = AreaLaneWidth(row);
    if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset)) {
        const std::int16_t axis = static_cast<std::int16_t>(FixedAtan2(-(next->position.x - row.position.x), -(next->position.z - row.position.z)));
        const std::uint16_t difference = static_cast<std::uint16_t>(yaw - axis);
        if (static_cast<std::uint16_t>(difference + 0x3fffU) <= 0x7ffeU) {
            const float along = AreaCosine(difference) * distance;
            const float lateral = AreaSine(difference) * distance;
            if ((along < row.length && std::abs(lateral) < std::fmin(AreaLaneWidth(*next), radius) + playerRadius) || distance < radius) {
                probe.lateralDistance = lateral;
                probe.rowParameter = std::clamp(along / row.length, 0.0f, 1.0f);
                probe.rowIndex = static_cast<std::int8_t>(row.index);
                probe.linkedRowIndex = static_cast<std::int8_t>(next->index);
                return true;
            }
        }
    }
    if (radius + playerRadius <= distance)
        return false;
    probe.lateralDistance = distance;
    probe.rowParameter = 0.0f;
    probe.rowIndex = static_cast<std::int8_t>(row.index);
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool ProbeAreaOwnerRow(const NpcAreaOwnerRow &owner, NpcAreaProbe &probe) {
    const float x = npcPlayerPosition.x + routeOrigin.x - owner.position.x;
    const float z = npcPlayerPosition.z + routeOrigin.z - owner.position.z;
    const float radius = std::fmax(0.40000001f, npcPlayerRadius) * 1.5f + 0.34999999f;
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
    const float distance = std::sqrt(x * x + z * z);
    if (const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset)) {
        Vector3 target;
        AreaLinkedPosition(owner, target);
        const float dx = target.x - owner.position.x;
        const float dz = target.z - owner.position.z;
        const std::int16_t axis = static_cast<std::int16_t>(FixedAtan2(-dx, -dz));
        const float length = std::sqrt(dx * dx + dz * dz);
        const std::uint16_t difference = static_cast<std::uint16_t>(yaw - axis);
        if (static_cast<std::uint16_t>(difference + 0x3fffU) <= 0x7ffeU) {
            const float along = AreaCosine(difference) * distance;
            const float lateral = AreaSine(difference) * distance;
            if (along < length && std::abs(lateral) < radius) {
                probe.ownerParameter = std::clamp(along / length, 0.0f, 1.0f);
                probe.ownerIndex = static_cast<std::int8_t>(owner.index);
                probe.ownerRowIndex = static_cast<std::int8_t>(row->index);
                return true;
            }
        }
    }
    if (owner.flags & 4U) {
        const NpcAreaOwnerRow &next = *(&owner + 1);
        const float dx = next.position.x - owner.position.x;
        const float dz = next.position.z - owner.position.z;
        const std::uint16_t difference = static_cast<std::uint16_t>(yaw - static_cast<std::int16_t>(FixedAtan2(-dx, -dz)));
        const float length = std::sqrt(dx * dx + dz * dz);
        if (static_cast<std::uint16_t>(difference + 0x3fffU) <= 0x7ffeU) {
            const float along = AreaCosine(difference) * distance;
            const float lateral = AreaSine(difference) * distance;
            if (along < length && std::abs(lateral) < radius) {
                probe.ownerParameter = std::clamp(along / length, 0.0f, 1.0f);
                probe.ownerIndex = static_cast<std::int8_t>(owner.index);
                probe.nextOwnerIndex = static_cast<std::int8_t>(next.index);
                return true;
            }
        }
    }
    if (radius <= distance)
        return false;
    probe.ownerParameter = 0.0f;
    probe.ownerIndex = static_cast<std::int8_t>(owner.index);
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void AdvanceAreaControlRow(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    entry.controlRowIndex = entry.nextControlRowIndex;
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset);
    entry.nextControlRowIndex = next ? static_cast<std::int8_t>(next->index) : -1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void QueueAreaOwnerLane(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const NpcAreaControlRow &target = (entry.opcodeFlags & 1U) ? *HeapPointer<NpcAreaControlRow>(row.forwardOffset) : row;
    QueueAreaMotion(entry, &target.position);
    entry.routeMotionState = 8;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline std::int32_t AreaQueueLength(const NpcAreaRouteState &state) {
    std::int32_t count = 0;
    for (const NpcRouteLiveEntry *entry = ListHead(state, 0); entry; entry = entry->areaPointNext)
        ++count;
    return count;
}

} // namespace

#else

namespace {

std::int32_t AreaQueueLength(const NpcAreaRouteState &state) {
    std::int32_t count = 0;
    for (const NpcRouteLiveEntry *entry = ListHead(state, 0); entry; entry = entry->areaPointNext)
        ++count;
    return count;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool SelectFreeAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(state.freeRowCount)) * dc::sdk::RandomFloat());
    std::int32_t available = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        if (owners[index].runtimeMask & 1U)
            continue;
        if (!owners[index].linkedControlOffset)
            continue;
        if (owners[index].flags & 1U) {
            if (!(owners[index].flags & 2U))
                continue;
        }
        if (available == choice) {
            owners[index].runtimeMask |= 1U;
            ++state.activeRowCount;
            --state.freeRowCount;
            entry.primarySlotIndex = static_cast<std::int8_t>(owners[index].index);
            entry.opcodeFlags |= 2U;
            if (state.flags & 4U) {
                if ((static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(owners[index].linkedControlOffset)->index) + 1) * 2 < state.controlRowCount + 1)
                    entry.opcodeFlags |= 1U;
                else if ((static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(owners[index].linkedControlOffset)->index) + 1) * 2 >
                    state.controlRowCount + 1)
                    entry.opcodeFlags = static_cast<std::int8_t>(entry.opcodeFlags) & ~1;
                else if (owners[index].positionAlongSegment > 0.5f)
                    entry.opcodeFlags = static_cast<std::int8_t>(entry.opcodeFlags) & ~1;
                else
                    entry.opcodeFlags |= 1U;
            } else if ((state.flags & 2U) && (entry.opcodeFlags & 8U))
                entry.opcodeFlags = static_cast<std::int8_t>(entry.opcodeFlags) & ~1;
            else
                entry.opcodeFlags |= 1U;
            return true;
        }
        ++available;
    }
    return false;
}

} // namespace

#else

namespace {

bool SelectFreeAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    std::int32_t reserved = areaProbes[0].state == &state && areaProbes[0].ownerIndex != -1 && state.freeRowCount > 1 ? 1 : 0;
    if (areaProbes[1].state == &state && areaProbes[1].ownerIndex != -1 && static_cast<std::int8_t>(state.freeRowCount) > 2)
        reserved = 2;
    float random = IncrementSharedPrngFloat();
    if (random == 1.0f)
        random = 0.0f;
    const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(state.freeRowCount) - reserved) * random);
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t available = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &owner = owners[index];
        if ((owner.runtimeMask & 1U) || !owner.linkedControlOffset || (owner.flags & 3U) == 1 || (reserved && areaProbes[0].ownerIndex == owner.index) ||
            (reserved > 1 && areaProbes[1].ownerIndex == owner.index))
            continue;
        if (available++ != choice)
            continue;
        owner.runtimeMask |= 1U;
        ++state.activeRowCount;
        --state.freeRowCount;
        entry.primarySlotIndex = static_cast<std::int8_t>(owner.index);
        entry.opcodeFlags |= 2U;
        bool forward = true;
        if (state.flags & 4U) {
            const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
            const std::int32_t current = 2 * static_cast<std::int8_t>(row.index) + 2;
            const std::int32_t middle = state.controlRowCount + 1;
            forward = current < middle || (current == middle && owner.positionAlongSegment <= 0.5f);
        } else if ((state.flags & 2U) && (entry.opcodeFlags & 8U))
            forward = false;
        if (forward)
            entry.opcodeFlags |= 1U;
        else
            entry.opcodeFlags &= ~1U;
        return true;
    }
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void AdvanceAreaMasks(NpcAreaRouteState &state) {
    NpcAreaControlRow *const rows = ControlRows(state);
    for (std::int32_t index = 0; index < state.controlRowCount; ++index) {
        NpcAreaControlRow &row = rows[index];
        row.previousForwardMask = row.forwardMask;
        row.previousBackwardMask = row.backwardMask;
        row.forwardMask = row.backwardMask = 0;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void UnlinkAreaStoreSlot(NpcAreaStoreSlot &slot) {
    if (slot.next)
        slot.next->previous = slot.previous;
    if (slot.previous)
        slot.previous->next = slot.next;
    if (nearestAreaSlot == &slot)
        nearestAreaSlot = slot.next;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ReserveEndpointOwner(NpcAreaRouteState &state, NpcAreaOwnerRow &owner, NpcRouteLiveEntry &entry) {
    owner.runtimeMask |= 1U;
    entry.primarySlotIndex = static_cast<std::int8_t>(owner.index);
    entry.opcodeFlags |= 2U;
    if (owner.linkedControlOffset) {
        if ((owner.flags & 1U) == 0) {
            ++state.activeRowCount;
            --state.freeRowCount;
        }
        if (state.flags & 4U) {
            if ((static_cast<std::int8_t>(reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->index) + 1) * 2 <
                state.controlRowCount + 1) {
                entry.opcodeFlags |= 1U;
            } else if ((static_cast<std::int8_t>(reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(owner.linkedControlOffset))->index) +
                           1) *
                    2 >
                state.controlRowCount + 1) {
                entry.opcodeFlags &= ~1U;
            } else if (owner.positionAlongSegment > 0.5f) {
                entry.opcodeFlags &= ~1U;
            } else {
                entry.opcodeFlags |= 1U;
            }
        } else if (state.flags & 2U) {
            if (entry.opcodeFlags & 8U)
                entry.opcodeFlags &= ~1U;
            else
                entry.opcodeFlags |= 1U;
        } else {
            entry.opcodeFlags |= 1U;
        }
        return 1;
    }
    return 2;
}

} // namespace

#else

namespace {

std::int32_t ReserveEndpointOwner(NpcAreaRouteState &state, NpcAreaOwnerRow &owner, NpcRouteLiveEntry &entry) {
    owner.runtimeMask |= 1U;
    entry.opcodeFlags |= 2U;
    entry.primarySlotIndex = static_cast<std::int8_t>(owner.index);
    const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
    if (!row)
        return 2;
    if (!(owner.flags & 1U)) {
        ++state.activeRowCount;
        --state.freeRowCount;
    }
    bool forward = true;
    if (state.flags & 4U) {
        const std::int32_t current = 2 * static_cast<std::int8_t>(row->index) + 2;
        const std::int32_t middle = state.controlRowCount + 1;
        forward = current < middle || (current == middle && owner.positionAlongSegment <= 0.5f);
    } else if ((state.flags & 2U) && (entry.opcodeFlags & 8U))
        forward = false;
    if (forward)
        entry.opcodeFlags |= 1U;
    else
        entry.opcodeFlags &= ~1U;
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t SelectEndpointOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t ordinary = 0, index = 0, alternate = 0;
    while (index < state.ownerRowCount) {
        if ((*reinterpret_cast<const std::uint32_t *>(&owners[index]) & 3U) != 3 && owners[index].actionKind != 4 && (owners[index].runtimeMask & 1U) == 0) {
            if (owners[index].flags & 0x10U)
                ++alternate;
            else
                ++ordinary;
        }
        while (owners[index++].flags & 4U)
            ;
    }
    const std::int32_t total = ordinary + alternate;
    std::int32_t choice, available;
    if (alternate && ordinary) {
        if (static_cast<std::uint8_t>(entry.scriptRecord[0xb]) & 2U) {
            choice = static_cast<std::int32_t>(static_cast<float>(alternate) * dc::sdk::RandomFloat());
            for (index = 0, available = 0; index < state.ownerRowCount;) {
                if ((*reinterpret_cast<const std::uint32_t *>(&owners[index]) & 3U) != 3 && (owners[index].flags & 0x10U) && owners[index].actionKind != 4 &&
                    (owners[index].runtimeMask & 1U) == 0) {
                    if (choice == available)
                        return ReserveEndpointOwner(state, owners[index], entry);
                    ++available;
                }
                while (owners[index++].flags & 4U)
                    ;
            }
        } else {
            choice = static_cast<std::int32_t>(static_cast<float>(ordinary) * dc::sdk::RandomFloat());
            for (index = 0, available = 0; index < state.ownerRowCount;) {
                if ((*reinterpret_cast<const std::uint32_t *>(&owners[index]) & 3U) != 3 && !(owners[index].flags & 0x10U) && owners[index].actionKind != 4 &&
                    (owners[index].runtimeMask & 1U) == 0) {
                    if (choice == available)
                        return ReserveEndpointOwner(state, owners[index], entry);
                    ++available;
                }
                while (owners[index++].flags & 4U)
                    ;
            }
        }
    } else {
        choice = static_cast<std::int32_t>(static_cast<float>(total) * dc::sdk::RandomFloat());
        for (index = 0, available = 0; index < state.ownerRowCount;) {
            if ((*reinterpret_cast<const std::uint32_t *>(&owners[index]) & 3U) != 3 && owners[index].actionKind != 4 &&
                (owners[index].runtimeMask & 1U) == 0) {
                if (choice == available)
                    return ReserveEndpointOwner(state, owners[index], entry);
                ++available;
            }
            while (owners[index++].flags & 4U)
                ;
        }
    }
    return 0;
}

} // namespace

#else

namespace {

std::int32_t SelectEndpointOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t ordinary = 0;
    std::int32_t alternate = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        const NpcAreaOwnerRow &owner = owners[index];
        if ((owner.flags & 3U) != 3 && owner.actionKind != 4 && !(owner.runtimeMask & 1U)) {
            if (owner.flags & 0x10U)
                ++alternate;
            else
                ++ordinary;
        }
        while (owners[index].flags & 4U)
            ++index;
    }
    const bool split = ordinary && alternate;
    const bool alternateOnly = (ScriptByte(entry.recordBase + 0x0b) & 2U) != 0;
    const std::int32_t count = split ? alternateOnly ? alternate : ordinary : ordinary + alternate;
    float random = IncrementSharedPrngFloat();
    if (random == 1.0f)
        random = 0.0f;
    const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(count) * random);
    std::int32_t available = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &owner = owners[index];
        if ((owner.flags & 3U) != 3 && owner.actionKind != 4 && !(owner.runtimeMask & 1U) && (!split || ((owner.flags & 0x10U) != 0) == alternateOnly)) {
            if (available++ == choice)
                return ReserveEndpointOwner(state, owner, entry);
        }
        while (owners[index].flags & 4U)
            ++index;
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool SelectClipAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t index = 0;
    std::int32_t count = 0;
    for (; index < state.ownerRowCount; ++index)
        if (static_cast<std::uint8_t>(owners[index].actionKind - 13) <= 2U && (owners[index].runtimeMask & 1U) == 0)
            ++count;
    if (count) {
        const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(count) * dc::sdk::RandomFloat());
        std::int32_t available = 0;
        for (index = 0; index < state.ownerRowCount; ++index) {
            if (static_cast<std::uint8_t>(owners[index].actionKind - 13) <= 2U && (owners[index].runtimeMask & 1U) == 0) {
                if (choice == available) {
                    owners[index].runtimeMask |= 1U;
                    entry.primarySlotIndex = static_cast<std::int8_t>(owners[index].index);
                    entry.opcodeFlags |= 2U;
                    return true;
                }
                ++available;
            }
        }
    }
    return false;
}

} // namespace

#else

namespace {

bool SelectClipAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        const NpcAreaOwnerRow &owner = owners[index];
        if (static_cast<std::uint8_t>(owner.actionKind - 13U) <= 2U && !(owner.runtimeMask & 1U))
            ++count;
    }
    if (!count)
        return false;
    float random = IncrementSharedPrngFloat();
    if (random == 1.0f)
        random = 0.0f;
    const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(count) * random);
    std::int32_t available = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &owner = owners[index];
        if (static_cast<std::uint8_t>(owner.actionKind - 13U) > 2U || (owner.runtimeMask & 1U))
            continue;
        if (available++ != choice)
            continue;
        owner.runtimeMask |= 1U;
        entry.opcodeFlags |= 2U;
        entry.primarySlotIndex = static_cast<std::int8_t>(owner.index);
        return true;
    }
    return false;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ReserveAreaEntrance(NpcAreaRouteState &state) {
    NpcAreaControlRow *const first = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(state.controlRowsOffset));
    first->forwardMask |= 7U;
    first->backwardMask |= 7U;
    first->previousForwardMask |= 7U;
    first->previousBackwardMask |= 7U;
    if (first->backwardOffset) {
        const std::uint32_t mask =
            7U << (static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(first->backwardOffset))->spanCount) - 1);
        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(first->backwardOffset))->forwardMask |= mask;
        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(first->backwardOffset))->backwardMask |= mask;
        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(first->backwardOffset))->previousForwardMask |= mask;
        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(first->backwardOffset))->previousBackwardMask |= mask;
    }
}

} // namespace

#else

namespace {

void ReserveAreaEntrance(NpcAreaRouteState &state) {
    NpcAreaControlRow &first = ControlRows(state)[0];
    first.previousForwardMask |= 7U;
    first.previousBackwardMask |= 7U;
    first.forwardMask |= 7U;
    first.backwardMask |= 7U;
    if (NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(first.backwardOffset)) {
        const std::uint32_t mask = AreaMask(7, previous->spanCount - 1);
        previous->previousForwardMask |= mask;
        previous->previousBackwardMask |= mask;
        previous->forwardMask |= mask;
        previous->backwardMask |= mask;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateAreaDistance(NpcAreaStoreSlot &slot) {
    NpcAreaStoreSlot *backward = 0, *forward = 0;
    Vector3 delta = npcPlayerPosition;
    delta.x += npcRouteOrigin.x;
    delta.z += npcRouteOrigin.z;
    delta.x -= ControlRows(*slot.state)[0].position.x;
    delta.y = 0.0f;
    delta.z -= ControlRows(*slot.state)[0].position.z;
    const float distance = Vector3::CalcMagnitude(delta);
    if (slot.previous || slot.next) {
        if (!slot.previous) {
            forward = slot.next;
            while (forward->next) {
                if (!(distance > forward->distanceSquared) || !(forward->state->flags & 8U))
                    break;
                forward = forward->next;
            }
        } else if (!slot.next) {
            backward = slot.previous;
            while (backward->previous && !(distance > backward->distanceSquared && (backward->state->flags & 8U)))
                backward = backward->previous;
        } else if (distance > slot.distanceSquared) {
            forward = slot.next;
            while (forward->next) {
                if (!(distance > forward->distanceSquared) || !(forward->state->flags & 8U))
                    break;
                forward = forward->next;
            }
        } else if (!(distance > slot.distanceSquared)) {
            backward = slot.previous;
            while (backward->previous && !(distance > backward->distanceSquared && (backward->state->flags & 8U)))
                backward = backward->previous;
        }
        if (forward && forward != slot.next) {
            if (slot.next)
                slot.next->previous = slot.previous;
            if (slot.previous)
                slot.previous->next = slot.next;
            if (npcNearestAreaSlot == &slot)
                npcNearestAreaSlot = slot.next;
            slot.previous = forward->previous;
            slot.next = forward;
            forward->previous = &slot;
            if (slot.previous)
                slot.previous->next = &slot;
        } else if (backward && backward != slot.previous) {
            if (slot.next)
                slot.next->previous = slot.previous;
            if (slot.previous)
                slot.previous->next = slot.next;
            slot.next = backward->next;
            slot.previous = backward;
            backward->next = &slot;
            if (slot.next)
                slot.next->previous = &slot;
        }
    }
    slot.distanceSquared = distance;
}

} // namespace

#else

namespace {

void UpdateAreaDistance(NpcAreaStoreSlot &slot) {
    Vector3 position = npcPlayerPosition;
    position.x += routeOrigin.x;
    position.z += routeOrigin.z;
    const float distance = AreaDistanceSquared(position, ControlRows(*slot.state)[0].position);
    NpcAreaStoreSlot *const previous = slot.previous;
    NpcAreaStoreSlot *const next = slot.next;
    if (next && (!previous || distance > slot.distanceSquared)) {
        NpcAreaStoreSlot *insertion = next;
        for (NpcAreaStoreSlot *cursor = next->next; cursor; cursor = cursor->next) {
            if (insertion->distanceSquared >= distance || !(insertion->state->flags & 8U))
                break;
            insertion = cursor;
        }
        if (insertion != next) {
            UnlinkAreaStoreSlot(slot);
            slot.previous = insertion->previous;
            slot.next = insertion;
            insertion->previous = &slot;
            if (slot.previous)
                slot.previous->next = &slot;
        }
    } else if (previous) {
        NpcAreaStoreSlot *insertion = previous;
        for (NpcAreaStoreSlot *cursor = previous->previous; cursor; cursor = cursor->previous) {
            if (distance > insertion->distanceSquared && (insertion->state->flags & 8U))
                break;
            insertion = cursor;
        }
        if (insertion != previous) {
            UnlinkAreaStoreSlot(slot);
            slot.next = insertion->next;
            slot.previous = insertion;
            insertion->next = &slot;
            if (slot.next)
                slot.next->previous = &slot;
        }
    }
    slot.distanceSquared = distance;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool AreaUsesSpecialAttachment(const NpcRouteLiveEntry &entry) {
    return (entry.stateContext.stateTable[areaPoints[entry.currentAreaPoint].routeIndex].flags & 0x400U) != 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ApplyAreaTransform(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    charaSetWorldRot(entry.node, rotation);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float AreaSegmentParameter(const Vector3 &start, const Vector3 &end, const Vector3 &position) {
    Vector3 delta;
    delta.x = position.x - start.x;
    delta.y = 0.0f;
    delta.z = position.z - start.z;
    const float distance = Vector3::CalcMagnitude(delta);
    const std::int16_t yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
    delta.x = end.x - start.x;
    delta.y = 0.0f;
    delta.z = end.z - start.z;
    const float length = Vector3::CalcMagnitude(delta);
    const std::int16_t axis = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
    const float projected = distance * dc::sdk::CosFixed(static_cast<std::int16_t>(yaw - axis));
    float parameter = projected / length;
    if (parameter > 1.0f)
        parameter = 1.0f;
    else if (0.0f > parameter)
        parameter = 0.0f;
    return parameter;
}

} // namespace

#else

namespace {

float AreaSegmentParameter(const Vector3 &start, const Vector3 &end, const Vector3 &position) {
    const float x = position.x - start.x;
    const float z = position.z - start.z;
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    const std::int16_t axis = static_cast<std::int16_t>(FixedAtan2(-dx, -dz));
    const float parameter = AreaCosine(yaw - axis) * std::sqrt(x * x + z * z) / std::sqrt(dx * dx + dz * dz);
    return parameter > 1.0f ? 1.0f : std::fmax(0.0f, parameter);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float AreaRowParameter(const NpcAreaControlRow &row, const NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow *const next = reinterpret_cast<const NpcAreaControlRow *>(static_cast<std::uintptr_t>(row.forwardOffset));
    if (!next)
        return 0.0f;
    return AreaSegmentParameter(row.position, next->position, entry.worldPosition);
}

} // namespace

#else

namespace {

float AreaRowParameter(const NpcAreaControlRow &row, const NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
    if (!next)
        return 0.0f;
    Vector3 position = entry.localPosition;
    position.x += routeOrigin.x;
    position.z += routeOrigin.z;
    return AreaSegmentParameter(row.position, next->position, position);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool AreaOpposingLaneOccupied(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow *const controls = ControlRows(state);
    if (entry.controlRowIndex != -1) {
        std::int32_t rowIndex;
        const std::uint32_t *occupiedMask;
        if (entry.opcodeFlags & 1U) {
            rowIndex = static_cast<std::int8_t>(controls[entry.controlRowIndex].index);
            occupiedMask = &controls[rowIndex].previousBackwardMask;
        } else {
            rowIndex = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset)->index);
            occupiedMask = &controls[rowIndex].previousForwardMask;
        }
        const std::uint32_t span = static_cast<std::uint32_t>(
            AreaRowParameter(controls[rowIndex], entry) * static_cast<float>(static_cast<std::int8_t>(controls[rowIndex].spanCount)));
        std::int32_t mask = 7 << span;
        mask |= mask >> 2;
        if (entry.opcodeFlags & 1U)
            mask |= 7 << (span + 3);
        else
            mask |= (7 << span) >> 5;
        if ((*occupiedMask & mask) == 0)
            return false;
        return true;
    }
    return false;
}

} // namespace

#else

namespace {

bool AreaOpposingLaneOccupied(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    if (entry.controlRowIndex == -1)
        return false;
    const NpcAreaControlRow &current = ControlRows(state)[entry.controlRowIndex];
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    const NpcAreaControlRow &row = forward ? current : *HeapPointer<NpcAreaControlRow>(current.backwardOffset);
    const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * AreaRowParameter(row, entry));
    const std::uint32_t mask = AreaMask(7, span);
    const std::uint32_t advance = forward ? AreaMask(7, span + 3) : mask >> 5;
    return ((advance | mask | (mask >> 2)) & (forward ? row.previousBackwardMask : row.previousForwardMask)) != 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t DisplaceAreaLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing, bool moving) {
    Vector3 position, start, end, delta, target;
    float parameter;
    std::int32_t mode;
    std::uint16_t motion;
    float width = 0.0f;
    NpcAreaControlRow *const controls = ControlRows(state);
    if (entry.opcodeFlags & 1U) {
        start = controls[entry.controlRowIndex].position;
        end = HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].forwardOffset)->position;
        start.x -= npcRouteOrigin.x;
        start.z -= npcRouteOrigin.z;
        end.x -= npcRouteOrigin.x;
        end.z -= npcRouteOrigin.z;
        parameter = AreaRowParameter(controls[entry.controlRowIndex], entry);
        mode = static_cast<std::int8_t>(controls[entry.controlRowIndex].laneMode);
        if (static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].forwardOffset)->laneMode) < mode)
            mode = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].forwardOffset)->laneMode);
    } else {
        start = controls[entry.controlRowIndex].position;
        end = HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset)->position;
        start.x -= npcRouteOrigin.x;
        start.z -= npcRouteOrigin.z;
        end.x -= npcRouteOrigin.x;
        end.z -= npcRouteOrigin.z;
        parameter = 1.0f - AreaRowParameter(*HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset), entry);
        mode = static_cast<std::int8_t>(controls[entry.controlRowIndex].laneMode);
        if (static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset)->laneMode) < mode)
            mode = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset)->laneMode);
    }
    switch (mode) {
    case 0:
        width = 0.315f;
        break;
    case 1:
    case 2:
        width = 0.45f;
        break;
    }
    delta.x = end.x - start.x;
    delta.y = 0.0f;
    delta.z = end.z - start.z;
    std::int32_t yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
    if (advancing) {
        delta.x = entry.localPosition.x - start.x;
        delta.z = entry.localPosition.z - start.z;
        const std::int32_t bearing = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
        float lateral = Vector3::CalcMagnitude(delta);
        lateral *= dc::sdk::SinFixed(static_cast<std::int16_t>(bearing - yaw));
        if (lateral > -(0.8f * width)) {
            advancing = false;
            moving = false;
        }
    }
    target.x = (1.0f - parameter) * start.x + end.x * parameter;
    target.y = (1.0f - parameter) * start.y + end.y * parameter;
    target.z = (1.0f - parameter) * start.z + end.z * parameter;
    const std::int32_t side = yaw - 0x4000;
    target.x -= width * dc::sdk::SinFixed(side);
    target.z -= width * dc::sdk::CosFixed(side);
    position = entry.localPosition;
    if (entry.node) {
        if (advancing) {
            if (moving && !mtwkSetExtraFlag(entry.motionWork, 58, 1)) {
                target = end;
                target.x -= width * dc::sdk::SinFixed(side);
                target.z -= width * dc::sdk::CosFixed(side);
                mtwkCopyTransition(entry.motionWork, &target, 0, 0, 1);
                if (mode == 0) {
                    motion = RemapNpcMotionForRig(charGetMotionRigType(entry.node), 0xe221);
                    mtwkSetTimeRelated(entry.motionWork, 1, 5, motion);
                    mtwkSetTimeRelated(entry.motionWork, 1, 0, motion);
                    motion = RemapNpcMotionForRig(charGetMotionRigType(entry.node), 0xe232);
                    mtwkSetTimeRelated(entry.motionWork, 1, 1, motion);
                    return 1;
                }
            } else {
                if (!moving)
                    mtwkCopyTransition(entry.motionWork, 0, yaw - 16, 13, 0);
                return 1;
            }
        } else {
            if (moving) {
                target.x -= dc::sdk::SinFixed(yaw) * 0.5f;
                target.z -= dc::sdk::CosFixed(yaw) * 0.5f;
            }
            delta.x = target.x - position.x;
            delta.y = 0.0f;
            delta.z = target.z - position.z;
            float distance = Vector3::CalcMagnitude(delta);
            if (distance > 0.4f) {
                distance = 0.4f / distance;
                target.x = position.x + distance * delta.x;
                target.z = position.z + distance * delta.z;
            }
            mtwkCopyTransition(entry.motionWork, &target, yaw - 16, 6, 0);
            mtwkCopyTransition(entry.motionWork, 0, 0, 2, 0);
            if (mode == 0) {
                motion = RemapNpcMotionForRig(charGetMotionRigType(entry.node), 0xe221);
                mtwkSetTimeRelated(entry.motionWork, 1, 5, motion);
                return 1;
            }
        }
        mtwkSetTimeRelated(entry.motionWork, 0, 6, 0);
    } else {
        delta.x = end.x - start.x;
        delta.y = 0.0f;
        delta.z = end.z - start.z;
        yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
        if (advancing) {
            const float distance = Vector3::CalcMagnitude(delta);
            entry.localPosition.x += 0.025f / distance * delta.x;
            entry.localPosition.z += 0.025f / distance * delta.z;
        } else
            entry.localPosition = target;
        entry.rotation.y = yaw;
    }
    return 1;
}

} // namespace

#else

namespace {

void DisplaceAreaLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing, bool moving) {
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    const NpcAreaControlRow &adjacent = *HeapPointer<NpcAreaControlRow>(forward ? row.forwardOffset : row.backwardOffset);
    const Vector3 start = NpcRouteAreaToScene(row.position);
    const Vector3 end = NpcRouteAreaToScene(adjacent.position);
    const float parameter = forward ? AreaRowParameter(row, entry) : 1.0f - AreaRowParameter(adjacent, entry);
    const std::int8_t mode = std::min(static_cast<std::int8_t>(row.laneMode), static_cast<std::int8_t>(adjacent.laneMode));
    const float width = mode == 0 ? 0.315f : mode == 1 || mode == 2 ? 0.44999999f : 0.0f;
    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    const std::uint16_t yaw = FixedAtan2(-dx, -dz);
    if (advancing) {
        const float x = entry.localPosition.x - start.x;
        const float z = entry.localPosition.z - start.z;
        const std::int16_t bearing = static_cast<std::int16_t>(FixedAtan2(-x, -z));
        if (AreaSine(bearing - yaw) * std::sqrt(x * x + z * z) > -width * 0.80000001f)
            advancing = moving = false;
    }
    Vector3 target = AreaLerp(start, end, parameter);
    target.x -= AreaSine(yaw - 0x4000) * width;
    target.z -= AreaCosine(yaw - 0x4000) * width;
    if (!entry.node) {
        if (advancing) {
            const float factor = 0.025f / std::sqrt(dx * dx + dz * dz);
            entry.localPosition.x += factor * dx;
            entry.localPosition.z += factor * dz;
        } else {
            entry.localPosition.x = target.x;
            entry.localPosition.z = target.z;
        }
        SetAreaYaw(entry.euler, yaw);
        return;
    }
    if (advancing) {
        if (!moving) {
            mtwkCopyTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(yaw - 16), 13, 0);
            return;
        }
        if (mtwkSetExtraFlag(entry.motionWork, 58, 1))
            return;
        target = end;
        target.x -= AreaSine(yaw - 0x4000) * width;
        target.z -= AreaCosine(yaw - 0x4000) * width;
        mtwkCopyTransition(entry.motionWork, &target, 0, 0, 1);
        if (!mode) {
            const std::int32_t rig = GetCharacterBoneId(entry.node);
            const std::uint16_t motion = RemapNpcMotionForRig(rig, 0xe221);
            mtwkSetTimeRelated(entry.motionWork, 1, 5, motion);
            mtwkSetTimeRelated(entry.motionWork, 1, 0, motion);
            mtwkSetTimeRelated(entry.motionWork, 1, 1, RemapNpcMotionForRig(rig, 0xe232));
        } else
            mtwkSetTimeRelated(entry.motionWork, 0, 6, 0);
        return;
    }
    if (moving) {
        target.x -= AreaSine(yaw) * 0.5f;
        target.z -= AreaCosine(yaw) * 0.5f;
    }
    const float x = target.x - entry.localPosition.x;
    const float z = target.z - entry.localPosition.z;
    float distance = std::sqrt(x * x + z * z);
    if (distance > 0.40000001f) {
        distance = 0.40000001f / distance;
        target.x = entry.localPosition.x + distance * x;
        target.z = entry.localPosition.z + distance * z;
    }
    if (distance > 0.039999999f) {
        mtwkCopyTransition(entry.motionWork, &target, static_cast<std::uint16_t>(yaw - 16), 6, 0);
        mtwkCopyTransition(entry.motionWork, nullptr, 0, 2, 0);
        if (!mode)
            mtwkSetTimeRelated(entry.motionWork, 1, 5, RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe221));
        else
            mtwkSetTimeRelated(entry.motionWork, 0, 6, 0);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t ReserveAreaLane(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    std::uint32_t entranceOccupied;
    std::int32_t available = 1;
    NpcAreaControlRow *const controls = ControlRows(state);
    entranceOccupied = controls->forwardMask & 1U;
    if (entry.controlRowIndex != -1) {
        std::int32_t index;
        std::uint32_t *occupied;
        std::uint32_t *prior;
        if (entry.opcodeFlags & 1U) {
            index = static_cast<std::int8_t>(controls[entry.controlRowIndex].index);
            occupied = &controls[index].forwardMask;
            prior = &controls[index].previousForwardMask;
        } else {
            if (entry.nextControlRowIndex != -1)
                index = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[entry.controlRowIndex].backwardOffset)->index);
            else
                index = static_cast<std::int8_t>(controls[entry.controlRowIndex].index);
            occupied = &controls[index].backwardMask;
            prior = &controls[index].previousBackwardMask;
        }
        const float parameter = AreaRowParameter(controls[index], entry);
        std::int32_t count = static_cast<std::int8_t>(controls[index].spanCount);
        const std::int32_t span = static_cast<std::uint32_t>(parameter * static_cast<float>(count));
        const std::int32_t mask = 7 << span;
        *occupied |= mask | (mask >> 2);
        if (span <= 1 && controls[index].backwardOffset != 0) {
            if (entry.opcodeFlags & 1U)
                occupied = &HeapPointer<NpcAreaControlRow>(controls[index].backwardOffset)->forwardMask;
            else
                occupied = &HeapPointer<NpcAreaControlRow>(controls[index].backwardOffset)->backwardMask;
            *occupied |= 7 << (static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[index].backwardOffset)->spanCount) + (span - 1));
        } else if (count - span <= 1 && controls[index].forwardOffset != 0) {
            if (entry.opcodeFlags & 1U)
                occupied = &HeapPointer<NpcAreaControlRow>(controls[index].forwardOffset)->forwardMask;
            else
                occupied = &HeapPointer<NpcAreaControlRow>(controls[index].forwardOffset)->backwardMask;
            *occupied |= 7 >> (count - span);
        }
        if (entry.opcodeFlags & 1U) {
            if ((1 << (span + 3)) & *prior)
                available = 0;
            else if (count - span <= 1 && controls[index].forwardOffset != 0) {
                if (HeapPointer<NpcAreaControlRow>(controls[index].forwardOffset)->previousBackwardMask & (1 << (span + 3 - count)))
                    available = 0;
            }
        } else {
            if (((1 << span) >> 3) & *prior)
                available = 0;
            else if (span <= 1 && controls[index].backwardOffset != 0) {
                count = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(controls[index].backwardOffset)->spanCount);
                if ((1 << (span - 3 + count)) & HeapPointer<NpcAreaControlRow>(controls[index].backwardOffset)->previousBackwardMask)
                    available = 0;
            }
        }
    }
    if (!entranceOccupied)
        controls->forwardMask &= ~1U;
    return available;
}

} // namespace

#else

namespace {

bool ReserveAreaLane(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    NpcAreaControlRow *const controls = ControlRows(state);
    const bool entranceOccupied = (controls[0].previousForwardMask & 1U) != 0;
    bool available = true;
    if (entry.controlRowIndex != -1) {
        const bool forward = (entry.opcodeFlags & 1U) != 0;
        NpcAreaControlRow &current = controls[entry.controlRowIndex];
        NpcAreaControlRow &row = forward || entry.nextControlRowIndex == -1 ? current : *HeapPointer<NpcAreaControlRow>(current.backwardOffset);
        const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * AreaRowParameter(row, entry));
        std::uint32_t &occupied = forward ? row.previousForwardMask : row.previousBackwardMask;
        const std::uint32_t prior = forward ? row.forwardMask : row.backwardMask;
        const std::uint32_t mask = AreaMask(7, span);
        occupied |= mask | (mask >> 2);
        NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(row.backwardOffset);
        NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
        const std::int32_t remaining = static_cast<std::int8_t>(row.spanCount) - span;
        if (span < 2 && previous) {
            (forward ? previous->forwardMask : previous->backwardMask) |= AreaMask(7, previous->spanCount + span - 1);
        } else if (remaining < 2 && next) {
            (forward ? next->forwardMask : next->backwardMask) |= 7U >> (static_cast<std::uint32_t>(remaining) & 31U);
        }
        if (forward) {
            if (prior & AreaMask(1, span + 3))
                available = false;
            else if (remaining < 2 && next && (next->previousBackwardMask & AreaMask(1, span - row.spanCount + 3)))
                available = false;
        } else {
            if (prior & (AreaMask(1, span) >> 3))
                available = false;
            else if (span < 2 && previous && (previous->previousBackwardMask & AreaMask(1, span + previous->spanCount - 3)))
                available = false;
        }
    }
    if (!entranceOccupied)
        controls[0].previousForwardMask &= ~1U;
    return available;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool AreaOwnerLaneAvailable(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::uint32_t masks[3] = {0, 0, 0};
    const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owners[entry.ownerRowIndex].linkedControlOffset);
    const std::int32_t span = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(owners[entry.ownerRowIndex].positionAlongSegment * static_cast<float>(static_cast<std::int8_t>(row->spanCount))));
    std::uint32_t occupied = row->previousForwardMask | row->previousBackwardMask;
    const std::int32_t center = 15 << span;
    masks[1] = (center | (center >> 3)) & occupied;
    if (span <= 2 && row->backwardOffset) {
        occupied = (entry.opcodeFlags & 1U) ? HeapPointer<NpcAreaControlRow>(row->backwardOffset)->previousForwardMask :
                                              HeapPointer<NpcAreaControlRow>(row->backwardOffset)->previousBackwardMask;
        std::int32_t shift = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(row->backwardOffset)->spanCount) - 2;
        shift += span;
        masks[0] |= 15 << shift;
        masks[0] = occupied & masks[0];
    } else if (static_cast<std::int8_t>(row->spanCount) - span <= 2 && row->forwardOffset) {
        occupied = (entry.opcodeFlags & 1U) ? HeapPointer<NpcAreaControlRow>(row->forwardOffset)->previousForwardMask :
                                              HeapPointer<NpcAreaControlRow>(row->forwardOffset)->previousBackwardMask;
        masks[2] |= 15 >> (static_cast<std::int8_t>(row->spanCount) - span);
        masks[2] &= occupied;
    }
    if (masks[0] || masks[1] || masks[2])
        return false;
    return true;
}

} // namespace

#else

namespace {

bool AreaOwnerLaneAvailable(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
    const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * owner.positionAlongSegment);
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    if (const NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(row.backwardOffset); span < 3 && previous) {
        const std::uint32_t occupied = forward ? previous->previousForwardMask : previous->previousBackwardMask;
        if (occupied & AreaMask(15, previous->spanCount + span - 2))
            return false;
    } else if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
        static_cast<std::int8_t>(row.spanCount) - span < 3 && next) {
        const std::uint32_t occupied = forward ? next->previousForwardMask : next->previousBackwardMask;
        if (occupied & (15U >> (static_cast<std::uint32_t>(row.spanCount - span) & 31U)))
            return false;
    }
    const std::uint32_t mask = AreaMask(15, span);
    return !((mask | (mask >> 3)) & (row.previousForwardMask | row.previousBackwardMask));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool OccupyAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = reinterpret_cast<const NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state.ownerRowsOffset));
    if (entry.primarySlotIndex == -1)
        return false;
    std::int8_t index = entry.primarySlotIndex;
    if (owners[index].linkedControlOffset)
        while (owners[index].flags & 4U)
            ++index;
    entry.commandFlags &= ~2U;
    entry.controlRowIndex = -1;
    entry.ownerRowIndex = index;
    entry.nextControlRowIndex = -1;
    entry.nextOwnerRowIndex = -1;
    ++state.occupiedRowCount;
    entry.routeMotionState = 16;
    entry.retained2d0 = 0;
    entry.stateContext.otherName = 0;
    entry.rotation.y = owners[entry.primarySlotIndex].yaw;
    Vector3 position = owners[entry.primarySlotIndex].position;
    position.x -= npcRouteOrigin.x;
    position.z -= npcRouteOrigin.z;
    entry.localPosition = position;
    if (entry.node) {
        const float height = entry.localPosition.y;
        if (FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags))
            SetCharacterWorldPosition(entry.node, &entry.localPosition);
        else
            entry.localPosition.y = height;
        charaSetWorldRot(entry.node, entry.rotation);
    }
    entry.worldPosition = entry.localPosition;
    entry.worldPosition.x += npcRouteOrigin.x;
    entry.worldPosition.z += npcRouteOrigin.z;
    return true;
}

} // namespace

#else

namespace {

bool OccupyAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (entry.primarySlotIndex == -1)
        return false;
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int8_t index = entry.primarySlotIndex;
    if (owners[index].linkedControlOffset && (owners[index].flags & 4U)) {
        do
            ++index;
        while (owners[index].flags & 4U);
    }
    entry.ownerRowIndex = index;
    entry.controlRowIndex = entry.nextControlRowIndex = entry.nextOwnerRowIndex = -1;
    entry.commandFlags &= ~2U;
    ++state.occupiedRowCount;
    entry.routeMotionState = 16;
    entry.field2d8 &= ~0xffff0000ULL;
    entry.field2e8 = 0;
    entry.stateContext.otherName = {};
    SetAreaYaw(entry.euler, owners[index].yaw);
    entry.localPosition = NpcRouteAreaToScene(owners[index].position);
    if (entry.node) {
        const float height = entry.localPosition.y;
        if (FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags))
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        else
            entry.localPosition.y = height;
        Vector3i rotation;
        std::memcpy(&rotation, &entry.euler, sizeof(rotation));
        charaSetWorldRot(entry.node, rotation);
    }
    PublishAreaWorldPosition(entry);
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void FinishAreaRoute(NpcRouteLiveEntry &entry) {
    if (entry.node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags |= CCMF_EvaluateMotion;
        if (!CanRevealNpcStream(entry))
            RequestNpcStreamOut(entry, 0);
    }
    ResetNpcAreaRouteState(entry);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool HasOccupiedPartner(const NpcAreaRouteState *state, const NpcRouteLiveEntry *entry) {
    bool occupied = false;
    std::int32_t flag = 0;
    const NpcAreaOwnerRow *const owners = reinterpret_cast<const NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state->ownerRowsOffset));
    std::int32_t selected = 0;
    if (entry->nextOwnerRowIndex != -1)
        selected = entry->nextOwnerRowIndex;
    else if (entry->ownerRowIndex != -1)
        selected = entry->ownerRowIndex;
    std::int32_t index;
    if (owners[selected].flags & 0x20U) {
        if (owners[selected].flags & 1U) {
            if ((owners[selected].flags & 2U) == 0)
                flag = 1;
        }
        for (index = 0; index < state->ownerRowCount; ++index) {
            if (index == selected)
                continue;
            if (flag) {
                if ((owners[index].flags & 1) != 0) {
                    if ((owners[index].flags & 2) == 0)
                        continue;
                }
            } else {
                if ((owners[index].flags & 1) == 0 || (owners[index].flags & 2) != 0)
                    continue;
            }
            if (owners[index].runtimeMask & 2)
                occupied = true;
        }
    } else {
        std::int32_t adjacent;
        for (index = 0; index < 4; ++index) {
            if (owners[selected].adjacentOwners[index] == selected)
                continue;
            if ((adjacent = owners[selected].adjacentOwners[index]) == -1)
                break;
            if (adjacent < state->ownerRowCount && (owners[adjacent].runtimeMask & 2U))
                occupied = true;
        }
    }
    return occupied;
}

} // namespace

#else

namespace {

bool HasOccupiedPartner(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const std::int32_t selected = entry.nextOwnerRowIndex != -1 ? entry.nextOwnerRowIndex : entry.ownerRowIndex != -1 ? entry.ownerRowIndex : 0;
    const NpcAreaOwnerRow &owner = owners[selected];
    if (owner.flags & 0x20U) {
        for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
            if (index == selected)
                continue;
            if (((owner.flags & 3U) == 1) != ((owners[index].flags & 3U) == 1) && (owners[index].runtimeMask & 2U))
                return true;
        }
    } else {
        for (std::int8_t adjacent : owner.adjacentOwners) {
            if (adjacent == selected)
                continue;
            if (adjacent == -1)
                break;
            if (adjacent < state.ownerRowCount && (owners[adjacent].runtimeMask & 2U))
                return true;
        }
    }
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

const std::byte *FindAnimationDefinition(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaOwnerRow &owner) {
    const std::byte *definition = HeapPointer<std::byte>(state.animationScriptOffset);
    const std::uint8_t ownerClass = !(owner.flags & 1U) ? 0 : (owner.flags & 2U) ? 1 : static_cast<std::uint8_t>(((owner.flags & 0x10U) | 0x20U) >> 4);
    for (;;) {
        if (ScriptByte(definition + 1) == owner.actionKind) {
            const std::uint32_t target = ScriptDword(definition + 2);
            bool matches;
            switch (target) {
            case SHEN_NAME(___G):
                matches = ownerClass == 1;
                break;
            case SHEN_NAME(___M):
                matches = ownerClass == 3;
                break;
            case SHEN_NAME(___S):
                matches = ownerClass == 2;
                break;
            case SHEN_NAME(____):
                matches = true;
                break;
            default:
                matches = target == entry.name.name;
                break;
            }
            if (matches)
                return definition;
        }
        while (ScriptByte(definition) != 'a')
            definition = NextInstruction(definition);
        if (ScriptByte(definition + 1) == 'e')
            return nullptr;
        definition = NextInstruction(definition);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
const std::byte *NextInstruction(const std::byte *command) {
    switch (*reinterpret_cast<const char *>(command)) {
    default:
        command += 2;
        break;
    case 'A':
    case 'M':
        command += 8;
        break;
    case 'T':
        command += 10;
        break;
    }
    return command;
}

} // namespace

#else

namespace {

const std::byte *NextInstruction(const std::byte *command) {
    const std::uint8_t opcode = ScriptByte(command);
    return command + (opcode == 'A' || opcode == 'M' ? 8 : opcode == 'T' ? 10 : 2);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
const std::byte *PreviousInstruction(const std::byte *command) {
    switch (*reinterpret_cast<const char *>(command)) {
    default:
        command -= 2;
        break;
    case 'E':
    case 'm':
        command -= 8;
        break;
    case 't':
        command -= 10;
        break;
    }
    return command;
}

} // namespace

#else

namespace {

const std::byte *PreviousInstruction(const std::byte *command) {
    const std::uint8_t opcode = ScriptByte(command);
    return command - (opcode == 'E' || opcode == 'm' ? 8 : opcode == 't' ? 10 : 2);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool IsScriptSectionEnd(std::uint8_t opcode) {
    return opcode == 'i' || opcode == 'j' || opcode == 'n' || opcode == 'p' || opcode == 'r';
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
const std::byte *FindInstructionEnd(const std::byte *command) {
    while (static_cast<char>(*command) != 'r' && static_cast<char>(*command) != 'p' && static_cast<char>(*command) != 'n' &&
        static_cast<char>(*command) != 'i' && static_cast<char>(*command) != 'j') {
        if (static_cast<char>(*command) == 'R' || static_cast<char>(*command) == 'P')
            command = FindInstructionEnd(command + 2) + 2;
        else if (static_cast<char>(*command) == 'M') {
            do
                command = NextInstruction(command);
            while (static_cast<char>(*command) == 'M');
        }
    }
    return command;
}

} // namespace

#else

namespace {

const std::byte *FindInstructionEnd(const std::byte *command) {
    for (;;) {
        const std::uint8_t opcode = ScriptByte(command);
        if (IsScriptSectionEnd(opcode))
            return command;
        if (opcode == 'P' || opcode == 'R') {
            command = FindInstructionEnd(command + 2) + 2;
        } else if (opcode == 'M') {
            do
                command = NextInstruction(command);
            while (ScriptByte(command) == 'M');
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
const std::byte *FindInstructionStart(const std::byte *command) {
    while (static_cast<char>(*command) != 'R' && static_cast<char>(*command) != 'P' && static_cast<char>(*command) != 'N' &&
        static_cast<char>(*command) != 'I' && static_cast<char>(*command) != 'J') {
        if (static_cast<char>(*command) == 'r' || static_cast<char>(*command) == 'p')
            command = FindInstructionStart(command - 2) - 2;
        else if (static_cast<char>(*command) == 'm') {
            do
                command = PreviousInstruction(command);
            while (static_cast<char>(*command) == 'm');
        }
    }
    return command;
}

} // namespace

#else

namespace {

const std::byte *FindInstructionStart(const std::byte *command) {
    for (;;) {
        const std::uint8_t opcode = ScriptByte(command);
        if (opcode == 'I' || opcode == 'J' || opcode == 'N' || opcode == 'P' || opcode == 'R')
            return command;
        if (opcode == 'p' || opcode == 'r') {
            command = FindInstructionStart(command - 2) - 2;
        } else if (opcode == 'm') {
            do
                command = PreviousInstruction(command);
            while (ScriptByte(command) == 'm');
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

const std::byte *FindSection(const std::byte *command, std::uint8_t section) {
    while (ScriptByte(command) != 'a') {
        if (ScriptByte(command) == section)
            return command;
        command = NextInstruction(command);
    }
    return 0;
}

} // namespace

#else

namespace {

const std::byte *FindSection(const std::byte *command, std::uint8_t section) {
    while (ScriptByte(command) != 'a') {
        if (ScriptByte(command) == section)
            return command;
        command = NextInstruction(command);
    }
    return nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
const std::byte *ChooseMotionInstruction(NpcRouteLiveEntry *entry, const std::byte *command) {
    if (static_cast<char>(*command) == 'R') {
        std::int32_t cumulative = 0;
        const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(command[1])) * dc::sdk::RandomFloat());
        const std::byte *start;
        command = NextInstruction(command);
        if (static_cast<char>(*command) == 'M') {
            cumulative += static_cast<std::int8_t>(command[1]);
            goto check_m;
        }
        goto enter_group;
    done_group:
        command = start;
        goto finish;
    enter_group:
        for (;;) {
            start = command;
            command = NextInstruction(command);
            command = FindInstructionEnd(command);
            cumulative += static_cast<std::int8_t>(command[1]);
            if (cumulative > choice)
                goto done_group;
        next_cmd:
            command = NextInstruction(command);
            if (static_cast<char>(*command) != 'M')
                continue;
            cumulative += static_cast<std::int8_t>(command[1]);
        check_m:
            if (cumulative <= choice)
                goto next_cmd;
            break;
        }
    finish:
        command = ChooseMotionInstruction(entry, command);
        return command;
    } else if (static_cast<char>(*command) == 'P') {
        if (!entry->animationRepeatGroups)
            entry->animationRepeatGroups = static_cast<std::uint8_t>(command[1]);
        command = NextInstruction(command);
        command = ChooseMotionInstruction(entry, command);
        return command;
    } else if (static_cast<char>(*command) == 'M') {
        return command;
    }
    return 0;
}

} // namespace

#else

namespace {

const std::byte *ChooseMotionInstruction(NpcRouteLiveEntry &entry, const std::byte *command) {
    for (;;) {
        const std::uint8_t opcode = ScriptByte(command);
        if (opcode == 'R') {
            float random = IncrementSharedPrngFloat();
            if (random == 1.0f)
                random = 0.0f;
            const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(command[1])) * random);
            const std::byte *option = NextInstruction(command);
            std::int32_t cumulative = 0;
            for (;;) {
                const std::byte *const end = ScriptByte(option) == 'M' ? option : FindInstructionEnd(NextInstruction(option));
                cumulative += static_cast<std::int8_t>(end[1]);
                if (cumulative > choice)
                    break;
                option = NextInstruction(end);
            }
            command = option;
        } else if (opcode == 'P') {
            if (!(entry.field2d0 >> 8))
                entry.field2d0 |= static_cast<std::uint16_t>(ScriptByte(command + 1)) << 8;
            command = NextInstruction(command);
        } else {
            return opcode == 'M' ? command : nullptr;
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint16_t BeginAnimationInstruction(NpcRouteLiveEntry *entry, const std::byte *command) {
    const std::byte *const motion = ChooseMotionInstruction(entry, command);
    if (!motion)
        return 0;
    entry->specialAnimationContext = reinterpret_cast<const NpcAreaAnimationScriptPrefix *>(motion);
    if (*reinterpret_cast<const char *>(FindInstructionEnd(motion)) == 'r')
        entry->animationRepeats = 1;
    else
        entry->animationRepeats = reinterpret_cast<const std::uint8_t *>(entry->specialAnimationContext)[1];
    return *reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::uint8_t *>(entry->specialAnimationContext) + 2);
}

} // namespace

#else

namespace {

std::uint16_t BeginAnimationInstruction(NpcRouteLiveEntry &entry, const std::byte *command) {
    const std::byte *const motion = ChooseMotionInstruction(entry, command);
    if (!motion)
        return 0;
    entry.specialAnimationContext = motion;
    const std::uint8_t repeats = ScriptByte(FindInstructionEnd(motion)) == 'r' ? 1 : ScriptByte(motion + 1);
    entry.field2d0 = static_cast<std::uint16_t>((entry.field2d0 & 0xff00U) | repeats);
    return ScriptWord(motion + 2);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry *entry) {
    std::uint32_t result = 0;
    if (!entry->node)
        return 0;
    const std::uint16_t current = mtwkGetAnimationId(entry->motionWork, LowerBody);
    if (!current)
        return 0;
    const std::int32_t rig = charGetMotionRigType(entry->node);
    if (entry->routeMotionState == 16) {
        if (entry->specialAnimationContext) {
            const std::byte *command = reinterpret_cast<const std::byte *>(entry->specialAnimationContext);
            if (static_cast<char>(*command) != 'A') {
                while (static_cast<char>(*command) != 'E')
                    command = PreviousInstruction(command);
                command -= 6;
            }
            for (;;) {
                while (static_cast<char>(*command) != 'T' && static_cast<char>(*command) != 'a')
                    command = NextInstruction(command);
                if (static_cast<char>(*command) == 'a')
                    return 0;
                const std::uint16_t rawSource = *reinterpret_cast<const std::uint16_t *>(command + 2);
                std::uint16_t target = *reinterpret_cast<const std::uint16_t *>(command + 4);
                const std::uint16_t source = RemapNpcMotionForRig(rig, rawSource);
                target = RemapNpcMotionForRig(rig, target);
                if (!source || current == source || current == target) {
                    result = target | (static_cast<std::uint32_t>(*reinterpret_cast<const std::uint16_t *>(command + 6)) << 16);
                    break;
                }
                command = NextInstruction(command);
            }
        }
    }
    return result;
}

#else

std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return 0;
    const std::uint16_t current = mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody);
    if (!current || entry.routeMotionState != 16 || !entry.specialAnimationContext)
        return 0;
    const std::int32_t rig = GetCharacterBoneId(entry.node);
    const std::byte *command = entry.specialAnimationContext;
    if (ScriptByte(command) != 'A') {
        while (ScriptByte(command) != 'E')
            command = PreviousInstruction(command);
        command -= 6;
    }
    while ((command = FindSection(command, 'T'))) {
        const std::uint16_t source = RemapNpcMotionForRig(rig, ScriptWord(command + 2));
        const std::uint16_t target = RemapNpcMotionForRig(rig, ScriptWord(command + 4));
        if (!source || source == current || target == current) {
            return target | (static_cast<std::uint32_t>(ScriptWord(command + 6)) << 16);
        }
        command = NextInstruction(command);
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint16_t npcResolveAnimationForRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    return entry.routeMotionState == 16 ? ResolveNpcAreaAnimation(state, entry, false) : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResumeNpcAreaSpecialMotion(NpcRouteLiveEntry *entry) {
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry->currentAreaPoint);
    if (!state || entry->routeMotionState != 16)
        return;
    const NpcAreaOwnerRow *const rows = reinterpret_cast<const NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state->ownerRowsOffset));
    mtwkCopyTransition(entry->motionWork, &entry->specialMotionRestorePosition, rows[entry->ownerRowIndex].yaw, 6, 0);
    mtwkSetTimeRelated(entry->motionWork, 1, 5, entry->currentMotionId);
    mtwkSetRegionEvalTime(entry->motionWork, static_cast<float>(entry->animationFrame), LowerBody);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint16_t ResolveNpcAreaOwnerAnimation(NpcAreaRouteState *state, NpcRouteLiveEntry *entry) {
    const NpcAreaOwnerRow *const owners = reinterpret_cast<const NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state->ownerRowsOffset));
    if (!entry->node)
        return 0;
    std::int32_t selected;
    if (entry->nextOwnerRowIndex != -1)
        selected = entry->nextOwnerRowIndex;
    else if (entry->ownerRowIndex != -1)
        selected = entry->ownerRowIndex;
    else
        return 0;
    const std::uint32_t action = static_cast<std::uint8_t>(owners[selected].actionKind);
    std::uint16_t motion;
    switch (action) {
    case 0:
    default:
        motion = SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]), static_cast<std::int8_t>(entry->scriptRecord[4]),
            static_cast<std::int8_t>(entry->scriptRecord[8]));
        break;
    case 1:
    case 4:
    case 5:
        motion = SelectNpcLeaningProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]), static_cast<std::int8_t>(entry->scriptRecord[4]),
            static_cast<std::int8_t>(entry->scriptRecord[8]));
        break;
    case 2:
    case 6:
    case 7:
        motion = RemapNpcMotionForRig(charGetMotionRigType(entry->node), 0xe111);
        break;
    case 16:
    case 17:
    case 18:
        motion = RemapNpcMotionForRig(charGetMotionRigType(entry->node), 0xe078);
        break;
    case 3:
    case 8:
    case 9:
        motion = SelectNpcRecliningProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
            static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        break;
    }
    if (state->animationScriptOffset) {
        const std::uint16_t baseMotion = motion;
        const std::byte *script;
        if (!entry->specialAnimationContext) {
            std::int32_t found = 0;
            script = reinterpret_cast<const std::byte *>(static_cast<std::uintptr_t>(state->animationScriptOffset));
            std::int32_t ownerClass;
            if (owners[selected].flags & 1U) {
                if (owners[selected].flags & 2U)
                    ownerClass = 1;
                else if (owners[selected].flags & 0x10U)
                    ownerClass = 3;
                else
                    ownerClass = 2;
            } else
                ownerClass = 0;
            for (;;) {
                if (static_cast<std::int32_t>(action) == static_cast<char>(script[1])) {
                    const std::uint32_t target = static_cast<char>(script[2]) + static_cast<char>(script[3]) * 0x100 + static_cast<char>(script[4]) * 0x10000 +
                        static_cast<char>(script[5]) * 0x1000000;
                    switch (target) {
                    case SHEN_NAME4('_', '_', '_', 'M'):
                        if (ownerClass == 3)
                            found = 1;
                        break;
                    case SHEN_NAME4('_', '_', '_', 'S'):
                        if (ownerClass == 2)
                            found = 1;
                        break;
                    case SHEN_NAME4('_', '_', '_', 'G'):
                        if (ownerClass == 1)
                            found = 1;
                        break;
                    case SHEN_NAME4('_', '_', '_', '_'):
                        found = 1;
                        break;
                    default:
                        if (target == entry->name)
                            found = 1;
                        break;
                    }
                }
                if (found) {
                    entry->specialAnimationContext = reinterpret_cast<const NpcAreaAnimationScriptPrefix *>(script);
                    break;
                }
                while (static_cast<char>(*script) != 'a')
                    script = NextInstruction(script);
                if (static_cast<char>(script[1]) == 'e')
                    break;
                script = NextInstruction(script);
            }
            if (entry->specialAnimationContext) {
                script = reinterpret_cast<const std::byte *>(entry->specialAnimationContext);
                if (entry->commandFlags & 0x200U) {
                    motion = static_cast<std::uint16_t>(SelectNpcAreaTransitionMotion(entry));
                    if (!motion)
                        motion = SelectNpcSpecialMotion(entry, static_cast<std::int8_t>(entry->scriptRecord[5]),
                            static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
                    while (static_cast<char>(*script) != 'J') {
                        script = NextInstruction(script);
                        if (static_cast<char>(*script) == 'a') {
                            script = 0;
                            break;
                        }
                    }
                } else {
                    if (HasOccupiedPartner(state, entry)) {
                        while (static_cast<char>(*script) != 'J') {
                            script = NextInstruction(script);
                            if (static_cast<char>(*script) == 'a') {
                                script = 0;
                                break;
                            }
                        }
                        entry->commandFlags |= 0x400U;
                    } else {
                        script = 0;
                        entry->commandFlags &= ~0x400U;
                    }
                    if (!script) {
                        script = reinterpret_cast<const std::byte *>(entry->specialAnimationContext);
                        if (GetMaximumWeatherRainForecast(60) >= 0.15f) {
                            while (static_cast<char>(*script) != 'I') {
                                script = NextInstruction(script);
                                if (static_cast<char>(*script) == 'a') {
                                    script = 0;
                                    break;
                                }
                            }
                        } else
                            script = 0;
                    }
                    if (!script) {
                        script = reinterpret_cast<const std::byte *>(entry->specialAnimationContext);
                        while (static_cast<char>(*script) != 'N') {
                            script = NextInstruction(script);
                            if (static_cast<char>(*script) == 'a') {
                                script = 0;
                                break;
                            }
                        }
                    }
                }
                if (script) {
                    script = NextInstruction(script);
                    motion = BeginAnimationInstruction(entry, script);
                } else
                    entry->specialAnimationContext = 0;
            }
        } else {
            script = reinterpret_cast<const std::byte *>(entry->specialAnimationContext);
            const std::byte *cursor = script;
            for (;;) {
                script = FindInstructionEnd(script);
                if (static_cast<char>(*script) == 'r') {
                    cursor = script;
                    script = NextInstruction(script);
                    continue;
                }
                if (static_cast<char>(*script) == 'p') {
                    if (--entry->animationRepeats) {
                        motion = ScriptWord(reinterpret_cast<const std::byte *>(entry->specialAnimationContext) + 2);
                        break;
                    }
                    script = NextInstruction(cursor);
                    if (static_cast<char>(*script) != 'p') {
                        motion = BeginAnimationInstruction(entry, script);
                        break;
                    }
                    cursor = script;
                    if (--entry->animationRepeatGroups) {
                        script = FindInstructionStart(script);
                        script = NextInstruction(script);
                        motion = BeginAnimationInstruction(entry, script);
                        break;
                    }
                    script = NextInstruction(script);
                    continue;
                }
                if (static_cast<char>(*script) == 'n' || static_cast<char>(*script) == 'i' || static_cast<char>(*script) == 'j') {
                    if (static_cast<char>(*cursor) == 'M' && --entry->animationRepeats) {
                        motion = ScriptWord(reinterpret_cast<const std::byte *>(entry->specialAnimationContext) + 2);
                        break;
                    }
                    script = NextInstruction(cursor);
                    motion = BeginAnimationInstruction(entry, script);
                }
                break;
            }
        }
        if (!motion) {
            entry->specialAnimationContext = 0;
            motion = baseMotion;
        }
    }
    entry->currentMotionId = RemapNpcMotionForRig(charGetMotionRigType(entry->node), motion);
    return entry->currentMotionId;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState *state, NpcRouteLiveEntry *entry) {
    return entry->routeMotionState == 16 ? ResolveNpcAreaOwnerAnimation(state, entry) : 0;
}

#else

std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing) {
    if (!entry.node)
        return 0;
    const std::int8_t index = entry.nextOwnerRowIndex == -1 ? entry.ownerRowIndex : entry.nextOwnerRowIndex;
    if (index == -1)
        return 0;
    const NpcAreaOwnerRow &owner = OwnerRows(state)[index];
    const std::uint16_t baseMotion = SelectNpcAreaActionBaseMotion(entry, owner.actionKind);
    std::uint16_t motion = baseMotion;
    if (state.animationScriptOffset) {
        if (!entry.specialAnimationContext) {
            const std::byte *const definition = FindAnimationDefinition(state, entry, owner);
            entry.specialAnimationContext = definition;
            if (definition) {
                const std::byte *section = nullptr;
                if (HasOccupiedPartner(state, entry)) {
                    section = FindSection(definition, 'J');
                    entry.commandFlags |= 0x400U;
                } else {
                    entry.commandFlags &= ~0x400U;
                }
                if (!section && npcHourWeatherForecast >= 0.15f)
                    section = FindSection(definition, 'I');
                if (!section)
                    section = FindSection(definition, 'N');
                if (section)
                    motion = BeginAnimationInstruction(entry, NextInstruction(section));
                else
                    entry.specialAnimationContext = nullptr;
                if (entry.commandFlags & 0x200U) {
                    const std::uint16_t transition = static_cast<std::uint16_t>(SelectNpcAreaTransitionMotion(entry));
                    const bool constrained = (GetNpcAreaSpecialMotionFlags(entry) & 1U) != 0;
                    if (transition)
                        motion = transition;
                    else if (!constrained)
                        motion = SelectNpcSpecialMotion(entry, false);
                    section = FindSection(definition, 'J');
                    if (!section && constrained)
                        section = FindSection(definition, 'N');
                    if (section)
                        motion = BeginAnimationInstruction(entry, NextInstruction(section));
                    else
                        entry.specialAnimationContext = nullptr;
                }
            }
        } else {
            const std::byte *cursor = entry.specialAnimationContext;
            const std::byte *scan = cursor;
            const std::byte *next = nullptr;
            bool repeat = false;
            for (;;) {
                const std::byte *const end = FindInstructionEnd(scan);
                const std::uint8_t opcode = ScriptByte(end);
                if (opcode == 'r') {
                    if (advancing) {
                        next = NextInstruction(FindInstructionStart(end));
                        break;
                    }
                    cursor = end;
                    scan = end + 2;
                    continue;
                }
                if (opcode == 'p') {
                    if (!advancing)
                        entry.field2d0 = static_cast<std::uint16_t>((entry.field2d0 & 0xff00U) | ((entry.field2d0 - 1U) & 0xffU));
                    if (entry.field2d0 & 0xffU) {
                        repeat = true;
                        break;
                    }
                    cursor = NextInstruction(cursor);
                    if (ScriptByte(cursor) != 'p') {
                        next = cursor;
                        break;
                    }
                    if (!advancing)
                        entry.field2d0 = static_cast<std::uint16_t>(entry.field2d0 - 0x100U);
                    if (entry.field2d0 >> 8) {
                        next = NextInstruction(FindInstructionStart(cursor));
                        break;
                    }
                    scan = NextInstruction(cursor);
                    continue;
                }
                if (opcode != 'i' && opcode != 'j' && opcode != 'n')
                    break;
                if (ScriptByte(cursor) == 'M') {
                    if (!advancing)
                        entry.field2d0 = static_cast<std::uint16_t>((entry.field2d0 & 0xff00U) | ((entry.field2d0 - 1U) & 0xffU));
                    if (entry.field2d0 & 0xffU) {
                        repeat = true;
                        break;
                    }
                }
                next = NextInstruction(cursor);
                break;
            }
            if (repeat)
                motion = ScriptWord(entry.specialAnimationContext + 2);
            else if (next)
                motion = BeginAnimationInstruction(entry, next);
        }
        if (!motion) {
            motion = baseMotion;
            entry.specialAnimationContext = nullptr;
        }
    }
    entry.currentMotionId = RemapNpcMotionForRig(GetCharacterBoneId(entry.node), motion);
    return entry.currentMotionId;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ApplyNpcAreaMotion(NpcRouteLiveEntry *entry, std::uint16_t motion) {
    if (entry->routeMotionState == 1)
        QueueNpcAreaDoorMotion(entry);
    else {
        mtwkRequestMotion(entry->motionWork, motion, LowerBody);
        entry->currentMotionId = motion;
        mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
    }
}

#else

void ApplyNpcAreaMotion(NpcRouteLiveEntry &entry, std::uint16_t motion) {
    if (entry.routeMotionState == 1)
        QueueAreaDoorMotion(entry);
    else {
        mtwkRequestMotion(entry.motionWork, motion, MotionRegion::LowerBody);
        entry.currentMotionId = motion;
        mtwkSetSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
    }
    if (!(entry.commandFlags & 1U)) {
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~CCMF_EvaluateMotion;
        entry.commandFlags |= 1U;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool RequestNpcAreaRouteCompletion(NpcRouteLiveEntry &entry) {
    if (!(entry.opcodeFlags & 2U))
        return true;
    switch (entry.routeMotionState) {
    case 1:
        ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
        if (entry.node)
            mtwkSetTimeRelated(entry.motionWork, 1, 5, 0);
        [[fallthrough]];
    case 4:
        FinishAreaRoute(entry);
        return true;
    case 2:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        entry.commandFlags |= 0x20U;
        return true;
    case 5: {
        entry.opcodeFlags &= ~2U;
        NpcAreaRouteState &state = *GetNpcAreaPointRouteState(entry.currentAreaPoint);
        PrepareAreaDoor(state, entry);
        if (entry.stateContext.otherName.name) {
            entry.routeMotionState = 0;
            entry.commandFlags &= ~2U;
        } else {
            entry.commandFlags |= 2U;
            if (QueueAreaMotion(entry, &areaPoints[entry.currentAreaPoint].position))
                mtwkCopyTransition(entry.motionWork, nullptr, 0, 2, 0);
            entry.routeMotionState = 22;
        }
        return true;
    }
    case 16:
        entry.field2e8 = 1000;
        return true;
    case 19:
        return true;
    default:
        return false;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool QueueAreaMotion(NpcRouteLiveEntry *entry, const Vector3 *target, const Vector3 *facing, std::int16_t yaw, std::uint16_t motion) {
    if (!entry->node)
        return false;
    if (target) {
        Vector3 position = *target;
        if (facing) {
            Vector3 delta;
            delta.x = facing->x - target->x;
            delta.y = 0.0f;
            delta.z = facing->z - target->z;
            yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            mtwkCopyTransition(entry->motionWork, &position, yaw, 5, 0);
        } else {
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            if (motion) {
                mtwkCopyTransition(entry->motionWork, &position, yaw, 0, 0);
                mtwkSetTimeRelated(entry->motionWork, 1, 5, motion);
            } else {
                mtwkCopyTransition(entry->motionWork, &position, 0, 0, 1);
                mtwkSetTimeRelated(entry->motionWork, 0, 6, 0);
            }
        }
    } else {
        mtwkCopyTransition(entry->motionWork, 0, entry->rotation.y, 13, 0);
        mtwkCopyTransition(entry->motionWork, 0, 0, 2, 0);
        mtwkSetTimeRelated(entry->motionWork, 0, 6, 0);
    }
    return true;
}

#else

namespace {

bool QueueAreaMotion(NpcRouteLiveEntry &entry, const Vector3 *target, const Vector3 *facing, std::uint16_t yaw, std::uint16_t motion) {
    if (!entry.node)
        return false;
    if (!target) {
        mtwkCopyTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
        mtwkCopyTransition(entry.motionWork, nullptr, 0, 2, 0);
    } else {
        Vector3 position = NpcRouteAreaToScene(*target);
        if (facing) {
            mtwkCopyTransition(entry.motionWork, &position, FixedAtan2(-(facing->x - target->x), -(facing->z - target->z)), 5, 0);
            return true;
        }
        if (motion) {
            mtwkCopyTransition(entry.motionWork, &position, yaw, 0, 0);
            mtwkSetTimeRelated(entry.motionWork, 1, 5, motion);
            return true;
        }
        mtwkCopyTransition(entry.motionWork, &position, 0, 0, 1);
    }
    mtwkSetTimeRelated(entry.motionWork, 0, 6, 0);
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint16_t SelectAreaApproachMotion(std::int32_t rig, std::uint8_t action, std::uint8_t direction) {
    std::uint16_t result = 0;
    switch (action) {
    case 2:
    case 6:
    case 7:
        switch (direction) {
        case 0:
            result = RemapNpcMotionForRig(rig, 0xe001);
            break;
        case 1:
            result = RemapNpcMotionForRig(rig, 0xe023);
            break;
        case 2:
            result = RemapNpcMotionForRig(rig, 0xe045);
            break;
        }
        break;
    case 16:
    case 17:
    case 18:
        switch (direction) {
        case 0:
            result = RemapNpcMotionForRig(rig, 0xe001);
            break;
        case 1:
            result = RemapNpcMotionForRig(rig, 0xe023);
            break;
        case 2:
            result = RemapNpcMotionForRig(rig, 0xe067);
            break;
        }
        break;
    }
    return result;
}

std::uint16_t SelectAreaExitApproachMotion(std::int32_t rig, std::uint8_t action, std::uint8_t direction) {
    std::uint16_t result = 0;
    switch (action) {
    case 2:
    case 6:
    case 7:
        switch (direction) {
        case 0:
            result = RemapNpcMotionForRig(rig, 0xe012);
            break;
        case 1:
            result = RemapNpcMotionForRig(rig, 0xe034);
            break;
        case 2:
            result = RemapNpcMotionForRig(rig, 0xe056);
            break;
        }
        break;
    case 16:
    case 17:
    case 18:
        switch (direction) {
        case 0:
            result = RemapNpcMotionForRig(rig, 0xe012);
            break;
        case 1:
            result = RemapNpcMotionForRig(rig, 0xe034);
            break;
        case 2:
            result = RemapNpcMotionForRig(rig, 0xe089);
            break;
        }
        break;
    }
    return result;
}

} // namespace

#else

namespace {

std::uint16_t SelectAreaApproachMotion(std::int32_t rig, std::uint8_t action, std::int32_t direction, bool exiting) {
    if (action != 2 && action != 6 && action != 7 && static_cast<std::uint8_t>(action - 16) > 2U)
        return 0;
    std::int16_t motion;
    if (exiting) {
        motion = direction == 0 ? -8174 : direction == 1 ? -8140 : action >= 16 ? -8055 : -8106;
    } else {
        motion = direction == 0 ? -8191 : direction == 1 ? -8157 : action >= 16 ? -8089 : -8123;
    }
    return RemapNpcMotionForRig(rig, static_cast<std::uint16_t>(motion));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 SampleAreaMotion(std::uint16_t motion, std::int32_t rig, float frame) {
    Vector3 translation = {0.0f, 0.0f, 0.0f};
    SampleNativeMotionRoot(motion, rig, frame, &translation);
    return translation;
}

} // namespace

#else

namespace {

Vector3 SampleAreaMotion(std::uint16_t motion, std::int32_t rig, float frame) {
    Vector3 translation{};
    SampleNativeMotionRoot(motion, rig, frame, translation);
    return translation;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 TransformAreaMotion(const Vector3 &origin, std::int16_t yaw, const Vector3 &motion) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Vector3 result;
    Matrix4x4::TransformPoint(motion, result);
    PopCurrentMatrix(1);
    return result;
}

} // namespace

#else

namespace {

Vector3 TransformAreaMotion(const Vector3 &origin, std::int16_t yaw, Vector3 motion) {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    const Matrix4x4 &matrix = workTransformMatrix;
    const Vector3 transformed{motion.x * matrix.rows[0].x + motion.y * matrix.rows[1].x + motion.z * matrix.rows[2].x + matrix.rows[3].x,
        motion.x * matrix.rows[0].y + motion.y * matrix.rows[1].y + motion.z * matrix.rows[2].y + matrix.rows[3].y,
        motion.x * matrix.rows[0].z + motion.y * matrix.rows[1].z + motion.z * matrix.rows[2].z + matrix.rows[3].z};
    Matrix4x4::Pop(1);
    return transformed;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline bool IsAreaSeatedAction(std::uint8_t action) {
    return action <= 18U && ((0x700c4U >> action) & 1U);
}

} // namespace

#else

namespace {

bool IsAreaSeatedAction(std::uint8_t action) {
    return action <= 18U && ((458948U >> action) & 1U);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void LimitAreaCollisionRadius(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const NpcAreaControlRow &adjacent = *HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset);
    if (std::min(row.laneMode, adjacent.laneMode) != 0)
        return;
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
        control->flags |= 0x2000000U;
        control->collisionCenterRadius = 0.2f;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void QueueAreaOwnerApproach(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const std::int32_t index = entry.nextOwnerRowIndex;
    const NpcAreaOwnerRow *const owners = reinterpret_cast<NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state.ownerRowsOffset));
    if (!entry.node)
        return;
    std::int16_t yaw = static_cast<std::int16_t>(owners[index].yaw);
    Vector3 target = owners[index].position;
    Vector3 position = entry.worldPosition;
    Vector3 delta;
    Vector3 track;
    delta.x = target.x - position.x;
    delta.y = 0.0f;
    delta.z = target.z - position.z;
    const std::int16_t heading = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
    const std::int16_t difference = static_cast<std::int16_t>(heading - yaw);
    const std::int32_t rig = charGetMotionRigType(entry.node);
    std::uint16_t motion;
    if (std::fabs(static_cast<float>(difference)) < 8192.0f) {
        motion = SelectAreaApproachMotion(rig, owners[index].actionKind, 2);
    } else if (std::fabs(static_cast<float>(difference)) < 24576.0f) {
        motion = SelectAreaApproachMotion(rig, owners[index].actionKind, 1);
        if (difference < 0)
            yaw -= 0x4000;
        else
            yaw += 0x4000;
    } else {
        motion = SelectAreaApproachMotion(rig, owners[index].actionKind, 0);
        yaw = static_cast<std::int16_t>(yaw + 0x8000);
    }
    SampleNativeMotionRoot(motion, rig, -1.0f, &track);
    track.x = -track.x;
    track.z = -track.z;
    if (difference < 0)
        track.x = -track.x;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(target.x, 0.0f, target.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::TransformPoint(track, target);
    PopCurrentMatrix(1);
    QueueAreaMotion(&entry, &target, 0, yaw, motion);
    if (difference < 0)
        mtwkSetRegionFlag(entry.motionWork, 69, 0, static_cast<MotionRegion>(0));
    entry.flags |= 0x100U;
    entry.commandFlags |= 8U;
}

} // namespace

#else

namespace {

void QueueAreaOwnerApproach(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.nextOwnerRowIndex];
    std::uint16_t yaw = static_cast<std::uint16_t>(owner.yaw);
    const std::int16_t difference =
        static_cast<std::int16_t>(FixedAtan2(-(owner.position.x - entry.worldPosition.x), -(owner.position.z - entry.worldPosition.z)) - yaw);
    const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(difference));
    const std::int32_t rig = GetCharacterBoneId(entry.node);
    std::uint16_t motion;
    if (magnitude < 8192)
        motion = SelectAreaApproachMotion(rig, owner.actionKind, 2, false);
    else if (magnitude >= 24576) {
        motion = SelectAreaApproachMotion(rig, owner.actionKind, 0, false);
        yaw += 0x8000U;
    } else {
        motion = SelectAreaApproachMotion(rig, owner.actionKind, 1, false);
        yaw += difference < 0 ? -0x4000 : 0x4000;
    }
    Vector3 translation = SampleAreaMotion(motion, rig, -1.0f);
    translation.x = difference < 0 ? translation.x : -translation.x;
    translation.z = -translation.z;
    Vector3 origin = owner.position;
    origin.y = 0.0f;
    const Vector3 target = TransformAreaMotion(origin, static_cast<std::int16_t>(yaw), translation);
    entry.stateContext.targetPosition = target;
    entry.stateContext.targetPosition.y = entry.worldPosition.y;
    entry.commandFlags |= 0x1000U;
    QueueAreaMotion(entry, &target, nullptr, yaw, motion);
    if (difference < 0)
        mtwkSetRegionFlag(entry.motionWork, 69, 0, MotionRegion::LowerBody);
    entry.flags |= 0x100U;
    entry.commandFlags |= 8U;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void QueueAreaOwnerTarget(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow &owner = owners[entry.nextOwnerRowIndex];
    if (owner.flags & 4U)
        QueueAreaMotion(entry, &owner.position, &owners[entry.nextOwnerRowIndex + 1].position);
    else if (IsAreaSeatedAction(owner.actionKind))
        QueueAreaOwnerApproach(state, entry);
    else {
        const std::uint16_t motion = ResolveNpcAreaAnimation(state, entry, true);
        QueueAreaMotion(entry, &owner.position, nullptr, static_cast<std::uint16_t>(owner.yaw), motion);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void EnterAreaOwnerLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    entry.controlRowIndex = entry.nextControlRowIndex = -1;
    QueueAreaOwnerTarget(state, entry);
    entry.routeMotionState = 10;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void QueueAreaDoorMotion(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    NpcRouteStateContext &context = entry.stateContext;
    Vector3 position = NpcRouteAreaToScene(context.scenePosition);
    const std::uint16_t motion = static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(context.otherName.name), entry.node, &position));
    context.doorMotion = motion;
    if (context.otherName.name != static_cast<std::uint32_t>(GetSelectedNpcDoorId())) {
        mtwkCopyTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
        return;
    }
    if (motion) {
        Vector3 root = SampleAreaMotion(motion, GetCharacterBoneId(entry.node), 15.0f);
        root = TransformAreaMotion({}, static_cast<std::int16_t>(context.targetYaw), root);
        position.x += root.x;
        position.z += root.z;
    }
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
        control->flags = (control->flags & ~CCMF_FineRaycast) | CCMF_StairMode;
    entry.commandFlags |= 1U;
    mtwkCopyTransition(entry.motionWork, &position, static_cast<std::uint16_t>(context.targetYaw), 6, 0);
    mtwkCopyTransition(entry.motionWork, nullptr, 0, 2, 0);
    if (context.otherName.name && motion) {
        mtwkSetTimeRelated(entry.motionWork, 1, 5, motion);
        mtwkSetRegionEvalTime(entry.motionWork, 14.0f, MotionRegion::LowerBody);
    }
    const Vector3 exit = NpcRouteAreaToScene(context.doorPosition);
    mtwkUpdateWithPosition(entry.motionWork, &exit, 0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TickAreaTraversal(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding) {
    auto stop = [&] {
        entry.commandFlags &= ~2U;
        if (avoiding)
            DisplaceAreaLane(state, entry, true, false);
        else
            QueueAreaMotion(entry, nullptr);
        entry.routeMotionState = 13;
        return false;
    };
    if (entry.commandFlags & 0x40U)
        return stop();
    if (!avoiding) {
        if (AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags |= 4U;
            DisplaceAreaLane(state, entry, false, true);
            entry.routeMotionState = 11;
            return true;
        }
    } else {
        LimitAreaCollisionRadius(state, entry);
        if (!AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags &= ~4U;
            QueueAreaMotion(entry, &ControlRows(state)[entry.nextControlRowIndex].position);
            entry.routeMotionState = 7;
            return true;
        }
    }
    if ((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
        ReserveAreaLane(state, entry);
        return stop();
    }
    if (!ReserveAreaLane(state, entry) || AreaProbeAhead(state, entry))
        return stop();
    if (avoiding)
        DisplaceAreaLane(state, entry, true, true);
    if (!AreaTargetReached(state, entry))
        return false;
    if (entry.opcodeFlags & 2U) {
        AdvanceAreaControlRow(state, entry);
        entry.routeMotionState = 6;
        return true;
    }
    const NpcAreaControlRow &row = ControlRows(state)[entry.nextControlRowIndex];
    entry.controlRowIndex = entry.nextControlRowIndex;
    if (row.flags & 1U) {
        entry.nextControlRowIndex = -1;
        if (avoiding) {
            entry.commandFlags &= ~4U;
            if (entry.node)
                mtwkSetTimeRelated(entry.motionWork, 0, 6, 0);
        }
        entry.routeMotionState = 21;
        PrepareAreaDoor(state, entry);
        return true;
    }
    const NpcAreaControlRow &next = *HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset);
    entry.commandFlags |= 2U;
    entry.nextControlRowIndex = static_cast<std::int8_t>(next.index);
    if (!avoiding)
        QueueAreaMotion(entry, &next.position);
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TickAreaOwnerApproach(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding) {
    auto stop = [&] {
        entry.commandFlags &= ~2U;
        if (avoiding)
            DisplaceAreaLane(state, entry, true, false);
        else
            QueueAreaMotion(entry, nullptr);
        entry.routeMotionState = 14;
        return false;
    };
    if (entry.commandFlags & 0x40U) {
        ReserveAreaLane(state, entry);
        return stop();
    }
    if (!avoiding) {
        if (AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags |= 4U;
            DisplaceAreaLane(state, entry, false, true);
            entry.routeMotionState = 12;
            return true;
        }
    } else {
        LimitAreaCollisionRadius(state, entry);
        if (!AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags &= ~4U;
            QueueAreaOwnerLane(state, entry);
            return true;
        }
    }
    if ((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
        ReserveAreaLane(state, entry);
        return stop();
    }
    if (AreaProbeAhead(state, entry)) {
        stop();
        RequestNpcAreaRouteCompletion(entry);
        return false;
    }
    if (!ReserveAreaLane(state, entry))
        return stop();
    if (avoiding)
        DisplaceAreaLane(state, entry, true, true);
    if (AreaTargetReached(state, entry)) {
        if (!avoiding) {
            entry.commandFlags |= 2U;
            EnterAreaOwnerLane(state, entry);
            return true;
        }
        return stop();
    }
    ReserveAreaLane(state, entry);
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TickAreaWaiting(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool ownerLane) {
    const bool opposite = AreaOpposingLaneOccupied(state, entry);
    if (entry.commandFlags & 0x40U) {
        if (opposite) {
            entry.commandFlags |= 4U;
            DisplaceAreaLane(state, entry, false, false);
        } else
            entry.commandFlags &= ~4U;
        ReserveAreaLane(state, entry);
        return false;
    }
    if (opposite) {
        DisplaceAreaLane(state, entry, false, false);
        entry.commandFlags |= 4U;
        if (((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) || (ownerLane && AreaTargetReached(state, entry))) {
            ReserveAreaLane(state, entry);
            return false;
        }
    } else {
        entry.commandFlags &= ~4U;
        if ((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
            if (ownerLane) {
                entry.nextControlRowIndex = entry.controlRowIndex;
                const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
                entry.controlRowIndex =
                    static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset)->index);
                entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
            } else
                std::swap(entry.controlRowIndex, entry.nextControlRowIndex);
            entry.opcodeFlags ^= 1U;
            entry.opcodeFlags &= ~2U;
            entry.commandFlags &= ~0x20U;
            OwnerRows(state)[entry.primarySlotIndex].runtimeMask &= ~1U;
            entry.primarySlotIndex = -1;
            --state.activeRowCount;
            ++state.freeRowCount;
            if (ownerLane) {
                entry.routeMotionState = 13;
                return true;
            }
        } else if (ownerLane && AreaTargetReached(state, entry)) {
            entry.commandFlags |= 2U;
            EnterAreaOwnerLane(state, entry);
            return false;
        }
    }
    if (ReserveAreaLane(state, entry) && !AreaProbeAhead(state, entry)) {
        entry.commandFlags |= 2U;
        if (entry.commandFlags & 4U)
            entry.routeMotionState = ownerLane ? 12 : 11;
        else if (ownerLane)
            QueueAreaOwnerLane(state, entry);
        else {
            QueueAreaMotion(entry, &ControlRows(state)[entry.nextControlRowIndex].position);
            entry.routeMotionState = 7;
        }
    }
    return false;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void QueueAreaOwnerExitMotion(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = reinterpret_cast<const NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state.ownerRowsOffset));
    if (!entry.node)
        return;
    const std::uint8_t index = static_cast<std::uint8_t>(entry.ownerRowIndex);
    Vector3 target;
    Vector3 position;
    Vector3 delta;
    if (owners[index].linkedControlOffset) {
        AreaLinkedPosition(owners[index], target);
    } else if (owners[index].flags & 8U) {
        target = owners[index - 1].position;
    }
    const std::int16_t yaw = static_cast<std::int16_t>(owners[index].yaw);
    position = entry.worldPosition;
    delta.x = target.x - position.x;
    delta.y = 0.0f;
    delta.z = target.z - position.z;
    const std::int16_t heading = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
    const std::int16_t difference = static_cast<std::int16_t>(yaw - heading);
    const std::int32_t rig = charGetMotionRigType(entry.node);
    std::uint16_t motion;
    if (std::fabs(static_cast<float>(difference)) < 8192.0f)
        motion = SelectAreaExitApproachMotion(rig, owners[index].actionKind, 0);
    else if (std::fabs(static_cast<float>(difference)) < 24576.0f)
        motion = SelectAreaExitApproachMotion(rig, owners[index].actionKind, 1);
    else
        motion = SelectAreaExitApproachMotion(rig, owners[index].actionKind, 2);
    if (motion) {
        mtwkRequestMotion(entry.motionWork, motion, LowerBody);
        entry.rotation.y = yaw;
        charaSetWorldRot(entry.node, entry.rotation);
        if (difference < 0)
            mtwkSetRegionFlag(entry.motionWork, 69, 0, LowerBody);
    }
    entry.commandFlags |= 8U;
}

} // namespace

#else

namespace {

void QueueAreaOwnerExitMotion(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow &owner = owners[static_cast<std::uint8_t>(entry.ownerRowIndex)];
    Vector3 target{};
    if (owner.linkedControlOffset)
        AreaLinkedPosition(owner, target);
    else if (owner.flags & 8U)
        target = owners[static_cast<std::uint8_t>(entry.ownerRowIndex) - 1].position;
    const std::int16_t yaw = static_cast<std::int16_t>(owner.yaw);
    const std::int16_t difference = static_cast<std::int16_t>(yaw - FixedAtan2(-(target.x - entry.worldPosition.x), -(target.z - entry.worldPosition.z)));
    const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(difference));
    const std::int32_t rig = GetCharacterBoneId(entry.node);
    const std::uint16_t motion = SelectAreaApproachMotion(rig, owner.actionKind, magnitude < 8192 ? 0 : magnitude < 24576 ? 1 : 2, true);
    if (motion) {
        mtwkRequestMotion(entry.motionWork, motion, MotionRegion::LowerBody);
        SetAreaYaw(entry.euler, yaw);
        Vector3i rotation;
        std::memcpy(&rotation, &entry.euler, sizeof(rotation));
        charaSetWorldRot(entry.node, rotation);
        if (difference < 0)
            mtwkSetRegionFlag(entry.motionWork, 69, 0, MotionRegion::LowerBody);
        if (!(entry.commandFlags & 0x1000U)) {
            const MotnSequence *const sequence = FindLoadedMotionSequence(motion);
            Vector3 track = SampleAreaMotion(motion, rig, sequence ? static_cast<float>(sequence->frameCount) : 0.0f);
            if (difference < 0)
                track.x = -track.x;
            track.y = 0.0f;
            entry.stateContext.targetPosition = TransformAreaMotion(entry.worldPosition, yaw, track);
            entry.commandFlags |= 0x1000U;
        }
    }
    entry.commandFlags |= 8U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void PrepareAreaOwnerGround(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = reinterpret_cast<NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state.ownerRowsOffset));
    if (!entry.node)
        return;
    const std::uint8_t index = static_cast<std::uint8_t>(entry.ownerRowIndex);
    Vector3 target;
    Vector3 position;
    Vector3 delta;
    Vector3 track;
    if (owners[index].linkedControlOffset) {
        AreaLinkedPosition(owners[index], target);
    } else {
        if (!(owners[index].flags & 8U)) {
            FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            SetCharacterWorldPosition(entry.node, &entry.localPosition);
            return;
        }
        target = owners[index - 1].position;
    }
    const std::int16_t yaw = static_cast<std::int16_t>(owners[index].yaw);
    position = entry.worldPosition;
    delta.x = target.x - position.x;
    delta.y = 0.0f;
    delta.z = target.z - position.z;
    const std::int16_t heading = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
    const std::int16_t difference = static_cast<std::int16_t>(yaw - heading);
    const std::int32_t rig = charGetMotionRigType(entry.node);
    std::uint16_t motion;
    if (std::fabs(static_cast<float>(difference)) < 8192.0f)
        motion = SelectAreaExitApproachMotion(rig, owners[index].actionKind, 0);
    else if (std::fabs(static_cast<float>(difference)) < 24576.0f)
        motion = SelectAreaExitApproachMotion(rig, owners[index].actionKind, 1);
    else
        motion = SelectAreaExitApproachMotion(rig, owners[index].actionKind, 2);
    if (motion) {
        SampleNativeMotionRoot(motion, rig, -1.0f, &track);
        if (difference < 0)
            track.x = -track.x;
        matrix_stack::PushUnitMatrix();
        Matrix4x4::preTranslate(position.x, 0.0f, position.z);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::TransformPoint(track, position);
        PopCurrentMatrix(1);
        position.x -= npcRouteOrigin.x;
        position.z -= npcRouteOrigin.z;
    } else {
        position = entry.localPosition;
    }
    if (FindNpcIntersectionInScene(position, entry.packedMoveFlags))
        entry.localPosition.y = position.y;
    SetCharacterWorldPosition(entry.node, &entry.localPosition);
}

} // namespace

#else

namespace {

void PrepareAreaOwnerGround(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow &owner = owners[static_cast<std::uint8_t>(entry.ownerRowIndex)];
    Vector3 target;
    bool linked = false;
    if (owner.linkedControlOffset) {
        AreaLinkedPosition(owner, target);
        linked = true;
    } else if (owner.flags & 8U) {
        target = owners[static_cast<std::uint8_t>(entry.ownerRowIndex) - 1].position;
        linked = true;
    }
    if (linked) {
        const std::uint16_t yaw = static_cast<std::uint16_t>(owner.yaw);
        const std::int16_t difference = static_cast<std::int16_t>(yaw - FixedAtan2(-(target.x - owner.position.x), -(target.z - owner.position.z)));
        const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(difference));
        const std::int32_t rig = GetCharacterBoneId(entry.node);
        const std::uint16_t motion = SelectAreaApproachMotion(rig, owner.actionKind, magnitude < 8192 ? 0 : magnitude < 24576 ? 1 : 2, true);
        Vector3 position = entry.localPosition;
        if (motion) {
            Vector3 track = SampleAreaMotion(motion, rig, -1.0f);
            if (difference < 0)
                track.x = -track.x;
            Vector3 origin = owner.position;
            origin.y = 0.0f;
            position = NpcRouteAreaToScene(TransformAreaMotion(origin, static_cast<std::int16_t>(yaw), track));
        }
        if (FindNpcIntersectionInScene(position, entry.packedMoveFlags))
            entry.localPosition.y = position.y;
    } else
        FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
    Vector3 position = GetCharacterWorldPosition(entry.node);
    position.y = entry.localPosition.y;
    shenApplyTranslationToAll(&entry.node->entity.node, position);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void PositionAreaQueueMember(NpcRouteLiveEntry &entry, const Vector3 &facing, const Vector3 &origin, float spacing) {
    Vector3 delta;
    delta.x = facing.x - origin.x;
    delta.y = 0.0f;
    delta.z = facing.z - origin.z;
    float distance = Vector3::CalcMagnitude(delta);
    std::int16_t yaw;
    if (distance > 0.00000011920929f)
        yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
    else
        yaw = 0;
    Vector3 target;
    target.x = dc::sdk::SinFixed(yaw) * spacing + origin.x;
    target.y = 0.0f;
    target.z = dc::sdk::CosFixed(yaw) * spacing + origin.z;
    delta.x = target.x - entry.worldPosition.x;
    delta.y = 0.0f;
    delta.z = target.z - entry.worldPosition.z;
    distance = Vector3::CalcMagnitude(delta);
    if (entry.node) {
        if (distance < 0.69999999f) {
            Vector3 look = facing;
            look.y = entry.worldPosition.y + 1.0f;
            look.x -= npcRouteOrigin.x;
            look.z -= npcRouteOrigin.z;
            mtwkUpdateWithPosition(entry.motionWork, &look, 0);
        }
        const std::uint32_t moving = mtwkSetExtraFlag(entry.motionWork, 58, 1);
        if (!moving && distance > 0.1f) {
            target.x -= npcRouteOrigin.x;
            target.z -= npcRouteOrigin.z;
            mtwkCopyTransition(entry.motionWork, &target, yaw, 0, moving);
            mtwkCopyTransition(entry.motionWork, 0, 0, 2, moving);
            mtwkSetTimeRelated(entry.motionWork, 1, 5, RemapNpcMotionForRig(charGetMotionRigType(entry.node), 0xe0de));
        }
    } else {
        if (distance > 0.029999999f) {
            distance = 0.029999999f / distance;
            entry.worldPosition.x = distance * delta.x + entry.worldPosition.x;
            entry.worldPosition.z = distance * delta.z + entry.worldPosition.z;
        } else {
            entry.worldPosition = target;
        }
        entry.localPosition = entry.worldPosition;
        entry.localPosition.x -= npcRouteOrigin.x;
        entry.localPosition.z -= npcRouteOrigin.z;
    }
}

} // namespace

#else

namespace {

void PositionAreaQueueMember(NpcRouteLiveEntry &entry, const Vector3 &facing, const Vector3 &origin, float spacing) {
    const float dx = facing.x - origin.x;
    const float dz = facing.z - origin.z;
    const std::uint16_t yaw = std::sqrt(dx * dx + dz * dz) <= 0.00000011920929f ? 0 : FixedAtan2(-dx, -dz);
    Vector3 target{origin.x + AreaSine(yaw) * spacing, origin.y, origin.z + AreaCosine(yaw) * spacing};
    const float x = target.x - entry.worldPosition.x;
    const float z = target.z - entry.worldPosition.z;
    const float distance = std::sqrt(x * x + z * z);
    if (entry.node) {
        if (distance < 0.69999999f) {
            Vector3 look = NpcRouteAreaToScene(facing);
            look.y = entry.worldPosition.y + 1.0f;
            mtwkUpdateWithPosition(entry.motionWork, &look, 0);
        }
        if (distance > 0.1f) {
            target = NpcRouteAreaToScene(target);
            mtwkCopyTransition(entry.motionWork, &target, yaw, 0, 0);
            mtwkCopyTransition(entry.motionWork, nullptr, 0, 2, 0);
            mtwkSetTimeRelated(entry.motionWork, 1, 5, RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe0de));
        }
    } else {
        if (distance > 0.029999999f) {
            target.x = entry.worldPosition.x + 0.029999999f / distance * x;
            target.z = entry.worldPosition.z + 0.029999999f / distance * z;
        }
        entry.worldPosition.x = target.x;
        entry.worldPosition.z = target.z;
        entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void PositionAreaQueueHead(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    Vector3 origin = *GetNpcAreaPointPosition(entry.currentAreaPoint);
    Vector3 facing = origin;
    facing.x += dc::sdk::SinFixed(static_cast<std::int16_t>(state.field0c));
    facing.z += dc::sdk::CosFixed(static_cast<std::int16_t>(state.field0c));
    PositionAreaQueueMember(entry, facing, origin, 1.5f);
}

void PositionAreaQueueFollower(NpcRouteLiveEntry &entry) {
    Vector3 facing = *GetNpcAreaPointPosition(entry.currentAreaPoint);
    Vector3 origin = entry.areaPointPrevious->worldPosition;
    PositionAreaQueueMember(entry, facing, origin, 0.69999999f);
}

} // namespace

#else

namespace {

void PositionAreaQueueHead(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const Vector3 &origin = areaPoints[entry.currentAreaPoint].position;
    Vector3 facing = origin;
    facing.x += AreaSine(static_cast<std::uint16_t>(state.field0c));
    facing.z += AreaCosine(static_cast<std::uint16_t>(state.field0c));
    PositionAreaQueueMember(entry, facing, origin, 1.5f);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool MaintainAreaQueue(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    std::int32_t capacity = npcAreaQueueCapacities[g_actNumber];
    if ((state.flags & 1U) == 0) {
        if (state.flags & 0x20U)
            capacity = 0;
        else
            capacity = capacity != 0;
    }
    if (state.npcListHeadOffset[0]) {
        NpcRouteLiveEntry *tail = ListHead(state, 0);
        std::int32_t count = 1;
        while (tail->areaPointNext) {
            tail = tail->areaPointNext;
            ++count;
        }
        if (count > capacity) {
            for (; tail; tail = tail->areaPointPrevious) {
                if (!entry.waitDuration && static_cast<std::int32_t>(ScriptDword(entry.currentCommand)) != -1 &&
                    static_cast<std::int32_t>(ScriptDword(entry.currentCommand)) != 1 && tail->currentAreaPoint != AreaEndpoint(*tail)) {
                    DisableNpcAreaRoute(tail);
                    if (--count <= capacity)
                        break;
                }
            }
        }
        for (NpcRouteLiveEntry *queued = ListHead(state, 0); queued; queued = queued->areaPointNext) {
            if (queued->primarySlotIndex != -1)
                continue;
            if (queued->currentAreaPoint != AreaEndpoint(*queued)) {
                if (state.freeRowCount)
                    SelectFreeAreaOwner(state, *queued);
            } else
                SelectEndpointOwner(state, *queued);
        }
    }
    if (entry.routeMotionState == 23)
        return false;
    return true;
}

} // namespace

#else

namespace {

bool MaintainAreaQueue(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcRouteLiveEntry *tail = ListHead(state, 0);
    if (tail) {
        std::int32_t count = 1;
        while (tail->areaPointNext) {
            tail = tail->areaPointNext;
            ++count;
        }
        const std::int32_t capacity = AreaQueueCapacity(state);
        while (tail && count > capacity) {
            if (!IsAreaWaitCommand(entry) && !IsAreaEndpoint(*tail)) {
                FinishAreaRoute(*tail);
                if (--count <= capacity)
                    break;
            }
            tail = tail->areaPointPrevious;
        }
        NpcRouteLiveEntry *const head = ListHead(state, 0);
        if (head && head->primarySlotIndex == -1) {
            if (IsAreaEndpoint(*head))
                SelectEndpointOwner(state, *head);
            else if (state.freeRowCount &&
                static_cast<std::int8_t>(state.participantCount) - static_cast<std::int8_t>(state.occupiedRowCount) - AreaQueueLength(state) < 2)
                SelectFreeAreaOwner(state, *head);
        }
    }
    return entry.routeMotionState != 23;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline void RestoreAreaMotionAttachment(NpcRouteLiveEntry &entry) {
    if (entry.waitParameter)
        QueueNpcMotionAttachment(&entry, static_cast<std::uint16_t>(entry.waitParameter), entry.opcodeByte | 0x20U, 1);
    else if (static_cast<std::int8_t>(entry.opcodeByte) == -1)
        ClearNpcMotionAttachment(&entry, 1);
}

} // namespace

#else

namespace {

void RestoreAreaMotionAttachment(NpcRouteLiveEntry &entry) {
    entry.currentMotionId = 0;
    if (entry.waitParameter)
        SetNpcMotionAttachment(entry, entry.waitParameter, entry.opcodeByte | 0x20U, 1);
    else if (entry.opcodeByte == 0xffU)
        ClearNpcMotionAttachment(entry, 1);
    entry.motionAttachments[1].flags &= ~0x40U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool AreaDoorPathClear(NpcRouteLiveEntry &entry) {
    if (npcNearbyDoor == entry.stateContext.otherName) {
        Vector3 delta;
        am2Vector3Subtract(&delta, &entry.stateContext.doorPosition, &entry.worldPosition);
        Vector3 player = npcPlayerPosition;
        player.x += npcRouteOrigin.x;
        player.z += npcRouteOrigin.z;
        am2Vector3SubtractInPlace(&player, &entry.worldPosition);
        if (delta.x * player.x + delta.z * player.z > 0.30899999f)
            return false;
    }
    return true;
}

} // namespace

#else

namespace {

bool AreaDoorPathClear(NpcRouteLiveEntry &entry) {
    NpcRouteStateContext &context = entry.stateContext;
    if (!context.otherName.name)
        return true;
    if (!entry.node && context.doorWaitFrames != -1) {
        ++context.doorWaitFrames;
        if (context.doorWaitFrames == 1)
            entry.flags |= 0x1000000U;
        else if (context.doorWaitFrames == 150) {
            entry.flags &= ~0x1000000U;
            context.doorWaitFrames = -1;
        }
        return false;
    }
    if (QueryNpcMoveObstruction(entry, context.scenePosition, false, 0x3000, 0.34999999f, -1.0f) & 1U)
        return false;
    return QueryNpcMoveObstruction(entry, context.scenePosition, true, 0x4000, 0.44999999f, 1.5f) == 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ClaimAreaEntrance(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(state.controlRowsOffset))->previousForwardMask & 1U)
        return 0;
    if (state.flags & 1U) {
        if (!entry.stateContext.otherName)
            return 1;
        if (!AreaDoorPathClear(entry))
            return 0;
        return ClaimDoor(entry.stateContext.otherName, entry.name);
    }
    return 1;
}

} // namespace

#else

namespace {

bool ClaimAreaEntrance(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (ControlRows(state)[0].previousForwardMask & 1U)
        return false;
    if (!(state.flags & 1U) || !entry.stateContext.otherName.name)
        return true;
    return AreaDoorPathClear(entry) && ClaimDoor(entry.stateContext.otherName, entry.name);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline void PublishAreaWorldPosition(NpcRouteLiveEntry &entry) {
    entry.worldPosition = entry.localPosition;
    entry.worldPosition.x += npcRouteOrigin.x;
    entry.worldPosition.z += npcRouteOrigin.z;
}

} // namespace

#else

namespace {

void PublishAreaWorldPosition(NpcRouteLiveEntry &entry) {
    entry.worldPosition = entry.localPosition;
    entry.worldPosition.x += routeOrigin.x;
    entry.worldPosition.z += routeOrigin.z;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool PrepareNpcAreaDoor(NpcAreaRouteState *state, NpcRouteLiveEntry *entry) {
    const shen_name owner = GetNpcAreaPointOwnerName(state->areaPointIndex);
    entry->stateContext.otherName = owner;
    if ((state->flags & 1U) && owner && ComputeDoorIngress(static_cast<std::int32_t>(owner), &entry->localPosition, &entry->stateContext.position)) {
        entry->stateContext.targetYaw = g_doorIngress.rotation.y;
        ComputeDoorEgress(static_cast<std::int32_t>(entry->stateContext.otherName), &entry->localPosition, &entry->stateContext.doorPosition);
        entry->stateContext.doorPosition.x += npcRouteOrigin.x;
        entry->stateContext.doorPosition.z += npcRouteOrigin.z;
        entry->stateContext.position.x += npcRouteOrigin.x;
        entry->stateContext.position.z += npcRouteOrigin.z;
        return true;
    }
    entry->stateContext.otherName = 0;
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ActivateNpcAreaRoutes(std::int32_t active) {
    if (!npcAreaRouteStore)
        return;
    std::int32_t index;
    NpcAreaRouteState *state;
    if (active) {
        for (index = 0; index < npcAreaRouteCount && reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index].state; ++index) {
            state = reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index].state;
            if ((g_currentMap & 0xffffU) == (state->areaMask & 0xffffU)) {
                if (state->flags & 1U) {
                    const shen_name door = GetNpcAreaPointOwnerName(state->areaPointIndex);
                    if (door && HasDoorEntry(door)) {
                        state->flags |= 8U;
                        NpcAreaControlRow *row = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(state->controlRowsOffset));
                        for (std::int32_t i = 0; i < state->controlRowCount; ++i, ++row) {
                            row->previousForwardMask = row->forwardMask;
                            row->previousBackwardMask = row->backwardMask;
                            row->forwardMask = 0;
                            row->backwardMask = 0;
                        }
                        continue;
                    }
                } else {
                    state->flags |= 8U;
                    NpcAreaControlRow *row = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(state->controlRowsOffset));
                    for (std::int32_t i = 0; i < state->controlRowCount; ++i, ++row) {
                        row->previousForwardMask = row->forwardMask;
                        row->previousBackwardMask = row->backwardMask;
                        row->forwardMask = 0;
                        row->backwardMask = 0;
                    }
                    continue;
                }
            }
            state->flags &= ~8U;
            NpcAreaStoreSlot *const slot = &reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index];
            if (slot->previous || slot->next) {
                if (slot->next)
                    slot->next->previous = slot->previous;
                if (slot->previous)
                    slot->previous->next = slot->next;
                if (npcNearestAreaSlot == slot)
                    npcNearestAreaSlot = slot->next;
                NpcAreaStoreSlot *last = npcNearestAreaSlot;
                while (last->next)
                    last = last->next;
                last->next = slot;
                slot->previous = last;
                slot->next = 0;
            }
        }
    } else {
        for (index = 0; index < npcAreaRouteCount && reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index].state; ++index) {
            state = reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index].state;
            state->flags = static_cast<std::uint8_t>(static_cast<std::int8_t>(state->flags) & ~8);
        }
    }
}

#else

void ActivateNpcAreaRoutes(bool active) {
    if (!areaStore)
        return;

    for (std::size_t index = 0; index < areaStoreCount; ++index) {
        NpcAreaStoreSlot &slot = areaStore[index];
        if (!slot.state)
            break;

        NpcAreaRouteState &state = *slot.state;
        if (!active) {
            state.flags &= ~8U;
            continue;
        }

        const bool matches = (state.flags & 0x80U) ? static_cast<std::uint8_t>(g_currentMap.name) == static_cast<std::uint8_t>(state.areaMask) :
                                                     static_cast<std::uint16_t>(g_currentMap.name) == static_cast<std::uint16_t>(state.areaMask);
        const shen_name owner = areaPoints[state.areaPointIndex].owner;
        if (matches && (!(state.flags & 1U) || (owner.name && HasDoorEntry(owner)))) {
            state.flags |= 8U;
            AdvanceAreaMasks(state);
        } else {
            state.flags &= ~8U;
            if (slot.next) {
                UnlinkAreaStoreSlot(slot);
                NpcAreaStoreSlot *last = nearestAreaSlot;
                while (last->next)
                    last = last->next;
                last->next = &slot;
                slot.previous = last;
                slot.next = nullptr;
            }
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint32_t GetNpcAreaRouteStoreCount() {
    return static_cast<std::uint32_t>(areaStoreCount);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry) {
    std::uint32_t flags = 0;
    if (entry.routeMotionState == 16) {
        const NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
        const NpcAreaOwnerRow *const owners = reinterpret_cast<const NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state->ownerRowsOffset));
        if (entry.ownerRowIndex == -1)
            return 0;
        switch (owners[entry.ownerRowIndex].actionKind) {
        case 2:
        case 6:
        case 7:
        case 16:
        case 17:
        case 18:
            flags |= 1;
            if (!entry.specialAnimationContext)
                flags |= 2;
            if (!entry.specialAnimationContext)
                flags |= 4;
        }
        if (entry.specialAnimationContext)
            flags |= entry.specialAnimationContext->flags;
    }
    return flags;
}

#else

std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry) {
    const NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState != 16 || entry.ownerRowIndex == -1)
        return 0;

    const std::uint8_t action = OwnerRows(*state)[entry.ownerRowIndex].actionKind;
    std::uint32_t flags = action <= 18 && (0x700c4U & (1U << action)) ? entry.specialAnimationContext ? 1U : 7U : 0U;
    if (entry.specialAnimationContext && ScriptByte(entry.specialAnimationContext) == 'M') {
        flags |= ScriptWord(entry.specialAnimationContext + 4);
    }
    return flags;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool FinishAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    bool continueTick = true;
    const std::uint8_t action = static_cast<std::uint8_t>(owners[entry.ownerRowIndex].actionKind);
    if (action == 2 || action == 6 || action == 7 || action == 16 || action == 17 || action == 18) {
        QueueAreaOwnerExitMotion(state, entry);
        entry.flags |= 0x100U;
        entry.routeMotionState = 19;
        owners[entry.ownerRowIndex].runtimeMask &= ~2U;
        continueTick = false;
    } else {
        entry.routeMotionState = 20;
        owners[entry.ownerRowIndex].runtimeMask &= ~2U;
    }
    entry.currentMotionId = 0;
    RestoreAreaMotionAttachment(entry);
    entry.motionAttachments[1].flags &= ~0x40U;
    return continueTick;
}

} // namespace

#else

namespace {

bool FinishAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    bool continueTick = true;
    if (IsAreaSeatedAction(owner.actionKind)) {
        QueueAreaOwnerExitMotion(state, entry);
        entry.flags |= 0x100U;
        entry.routeMotionState = 19;
        continueTick = false;
    } else
        entry.routeMotionState = 20;
    owner.runtimeMask &= ~2U;
    RestoreAreaMotionAttachment(entry);
    return continueTick;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void RestoreAreaSavedPosition(NpcRouteLiveEntry &entry) {
    entry.worldPosition = entry.stateContext.targetPosition;
    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
    entry.commandFlags &= ~0x1000U;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TickAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    entry.commandFlags &= 0xfef5U;
    owner.runtimeMask |= 2U;
    const std::uint16_t counter = static_cast<std::uint16_t>(entry.field2d8 >> 16);
    const std::uint32_t opcode = ScriptDword(entry.currentCommand);
    if (counter > 900 && opcode != 1)
        entry.field2e8 = 1000;
    ++entry.field2e8;
    if (!(entry.commandFlags & 0x800U) && entry.node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags |= CCMF_StairMode;
        entry.commandFlags |= 0x800U;
    }
    entry.motionAttachments[1].flags |= 0x40U;
    const std::int32_t pending = static_cast<std::int8_t>(state.participantCount) - static_cast<std::int8_t>(state.occupiedRowCount) - AreaQueueLength(state);
    if (pending >= 2)
        entry.field2e8 = 0;
    else if (opcode == 0xffffffffU) {
        entry.commandFlags |= 0x80U;
        entry.field2e8 = 0;
    } else if (entry.delayDuration > 0)
        entry.field2e8 = 1000;
    else {
        if (entry.field2e8 > 1000 && !(entry.commandFlags & 0x40U)) {
            if (!entry.node) {
                entry.routeMotionState = 20;
                if (entry.commandFlags & 0x1000U)
                    RestoreAreaSavedPosition(entry);
                else if (IsAreaSeatedAction(owner.actionKind)) {
                    Vector3 target{};
                    if (owner.linkedControlOffset)
                        AreaLinkedPosition(owner, target);
                    else if (owner.flags & 8U)
                        target = OwnerRows(state)[static_cast<std::uint8_t>(entry.ownerRowIndex) - 1].position;
                    entry.worldPosition.x = (target.x + entry.worldPosition.x) * 0.5f;
                    entry.worldPosition.z = (target.z + entry.worldPosition.z) * 0.5f;
                    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
                }
                owner.runtimeMask &= ~2U;
                --state.occupiedRowCount;
                RestoreAreaMotionAttachment(entry);
                return true;
            }
            if ((!entry.specialAnimationContext && mtwkSetRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) || counter > 900) {
                const bool continueTick = FinishAreaOwnerAnimation(state, entry);
                --state.occupiedRowCount;
                return continueTick;
            }
        }
        if (opcode == 1) {
            entry.commandFlags |= 0x80U;
            entry.field2e8 = 0;
        }
    }
    auto restart = [&] {
        entry.specialAnimationContext = nullptr;
        ApplyAreaOwnerAnimation(state, entry, ResolveNpcAreaAnimation(state, entry, true));
    };
    if (entry.node && IsAreaEndpoint(entry) && !(entry.commandFlags & 0x40U)) {
        const float distance = AreaDistanceSquared(npcPlayerPosition, entry.localPosition);
        if (distance > 6.25f && (entry.commandFlags & 0x200U)) {
            entry.commandFlags &= ~0x200U;
            if (!entry.field2e8 || entry.delayDuration)
                restart();
            else
                return FinishAreaOwnerAnimation(state, entry);
        } else {
            bool interact = false;
            if (distance < 2.25f && !(entry.commandFlags & 0x200U) && !(owner.flags & 0x40U)) {
                entry.field412 = 0;
                entry.commandFlags |= 0x200U;
                interact = true;
            } else if (!(entry.commandFlags & 0x200U)) {
                const bool partner = HasOccupiedPartner(state, entry);
                if (!partner && (entry.commandFlags & 0x400U)) {
                    if (!entry.field2e8 || entry.delayDuration)
                        restart();
                    else
                        return FinishAreaOwnerAnimation(state, entry);
                } else if (partner && !(entry.commandFlags & 0x400U))
                    interact = true;
            }
            if (interact) {
                const std::int32_t flags = static_cast<std::int32_t>(GetNpcAreaSpecialMotionFlags(entry));
                const std::uint32_t transition = SelectNpcAreaTransitionMotion(entry);
                if (flags < 0)
                    entry.commandFlags &= ~0x200U;
                else {
                    entry.field2d8 = (entry.field2d8 & ~0xffffULL) | (transition >> 16);
                    if (!(flags & 1) || static_cast<std::uint16_t>(transition))
                        restart();
                }
            }
            if (entry.commandFlags & 0x200U) {
                if (++entry.field412 >= 600)
                    mtwkUpdateWithPosition(entry.motionWork, nullptr, 0);
                else {
                    Vector3 leg{};
                    mtwkGetBonePosition(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(g_playerRuntime.character), MB_Head, &leg);
                    mtwkUpdateWithPosition(entry.motionWork, &leg, 0);
                }
                return false;
            }
            mtwkUpdateWithPosition(entry.motionWork, nullptr, 0);
            if (entry.commandFlags & 0x400U)
                return false;
        }
    }
    if (entry.node) {
        if (!mtwkSetRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody) || (entry.commandFlags & 0x40U))
            return false;
        std::uint16_t motion;
        if (entry.specialAnimationContext) {
            motion = ResolveNpcAreaAnimation(state, entry, false);
            if (!entry.specialAnimationContext) {
                if (entry.field2e8 && !entry.delayDuration) {
                    const bool continueTick = FinishAreaOwnerAnimation(state, entry);
                    --state.occupiedRowCount;
                    return continueTick;
                }
                motion = ResolveNpcAreaAnimation(state, entry, false);
            }
        } else
            motion = ResolveNpcAreaAnimation(state, entry, false);
        const Vector3 position = GetCharacterWorldPosition(entry.node);
        const MotnSequence *const current = FindLoadedMotionSequence(mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody));
        if (motion != mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody) || !current || !current->allowsFrameOverflow ||
            AreaDistanceSquared(NpcRouteAreaToScene(owner.position), position) > 0.040000003f)
            ApplyAreaOwnerAnimation(state, entry, motion);
    }
    entry.motionAttachments[1].flags |= 0x40U;
    entry.flags &= ~0x100U;
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TickInactiveAreaRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    if (entry.stateContext.otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
        entry.stateContext.otherName = {};
    }
    if (!(entry.flags & 0x800000U)) {
        SetNpcStreamLifecycleState(entry, 1);
        entry.flags |= 0x800000U;
    }
    if (state.flags & 0x20U)
        entry.motionAttachments[0].flags &= ~0x40U;
    else
        entry.motionAttachments[0].flags |= 0x40U;
    for (;;) {
        switch (entry.routeMotionState) {
        case 3:
            if (!AdmitAreaRoute(state, entry, true))
                return true;
            continue;
        case 4:
            if (entry.commandFlags & 0x40U)
                return true;
            MaintainAreaQueue(state, entry);
            if (ListHead(state, 0) != &entry || entry.areaPointPrevious || entry.primarySlotIndex == -1)
                return true;
            SetListHead(state, entry.areaPointNext, 0);
            if (entry.areaPointNext) {
                entry.areaPointNext->areaPointPrevious = nullptr;
                entry.areaPointNext = nullptr;
            }
            OccupyAreaOwner(state, entry);
            continue;
        case 16: {
            entry.commandFlags &= 0xfef5U;
            owners[entry.ownerRowIndex].runtimeMask |= 2U;
            entry.motionAttachments[1].flags |= 0x40U;
            ++entry.field2e8;
            const std::uint32_t opcode = ScriptDword(entry.currentCommand);
            if (opcode == 0xffffffffU) {
                entry.field2e8 = 0;
                entry.commandFlags |= 0x80U;
            } else if (entry.delayDuration) {
                entry.field2e8 = 1000;
                return true;
            } else if (entry.field2e8 > 1000 && !(entry.commandFlags & 0x40U)) {
                entry.routeMotionState = 20;
                owners[entry.ownerRowIndex].runtimeMask &= ~2U;
                entry.currentMotionId = 0;
                --state.occupiedRowCount;
                continue;
            } else if (opcode == 1) {
                entry.field2e8 = 0;
                entry.commandFlags |= 0x80U;
            }
            return true;
        }
        case 18:
            return true;
        case 20:
            entry.opcodeFlags &= ~2U;
            entry.controlRowIndex = 0;
            entry.nextControlRowIndex = entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
            entry.field2e8 = 0;
            RestoreAreaMotionAttachment(entry);
            entry.routeMotionState = 21;
            continue;
        case 21:
            owners[entry.primarySlotIndex].runtimeMask &= ~1U;
            entry.primarySlotIndex = -1;
            --state.activeRowCount;
            ++state.freeRowCount;
            entry.worldPosition = areaPoints[entry.currentAreaPoint].position;
            entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
            FinishAreaRoute(entry);
            return true;
        default:
            if (entry.opcodeFlags & 2U) {
                entry.controlRowIndex = -1;
                std::int8_t owner = entry.primarySlotIndex;
                while (owners[owner].flags & 4U)
                    ++owner;
                entry.ownerRowIndex = owner;
                entry.nextControlRowIndex = entry.nextOwnerRowIndex = -1;
                ++state.occupiedRowCount;
                entry.specialAnimationContext = nullptr;
                entry.routeMotionState = 16;
                entry.field2e8 = 0;
                entry.field2d8 &= ~0xffff0000ULL;
            } else
                entry.routeMotionState = 21;
            continue;
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool AdmitAreaRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool initializing) {
    entry.controlRowIndex = -1;
    entry.nextControlRowIndex = 0;
    entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
    entry.specialAnimationContext = nullptr;
    PrepareAreaDoor(state, entry);
    if (entry.primarySlotIndex == -1) {
        if (ScriptDword(entry.currentCommand) == 19 && SelectClipAreaOwner(state, entry)) {
            OccupyAreaOwner(state, entry);
            entry.routeMotionState = 18;
            entry.motionSpeed = 1.0f;
            entry.clipOverrideIndex = static_cast<std::int8_t>(ScriptByte(entry.currentCommand + 4));
            entry.currentCommand += 8;
            entry.streamCost = 1;
            entry.routeDirectionMode = 3;
            entry.flags |= 0x100U;
            entry.animationFrame = 2.0f * static_cast<std::int8_t>(ScriptByte(entry.recordBase + 5));
            return true;
        }
        NpcRouteLiveEntry *tail = ListHead(state, 0);
        std::int32_t count = 0;
        if (tail) {
            count = 1;
            while (tail->areaPointNext) {
                tail = tail->areaPointNext;
                ++count;
            }
        }
        if (IsAreaEndpoint(entry)) {
            const std::int32_t reserved = SelectEndpointOwner(state, entry);
            if (reserved == 2) {
                OccupyAreaOwner(state, entry);
                return true;
            }
            if (!reserved) {
                FinishAreaRoute(entry);
                return false;
            }
        } else if (tail) {
            if (!IsAreaWaitCommand(entry) && count > AreaQueueCapacity(state) - 1) {
                FinishAreaRoute(entry);
                return false;
            }
        } else if (state.freeRowCount && static_cast<std::int8_t>(state.participantCount) - static_cast<std::int8_t>(state.occupiedRowCount) - count < 2) {
            SelectFreeAreaOwner(state, entry);
        }
        if (tail) {
            tail->areaPointNext = &entry;
            entry.areaPointPrevious = tail;
            entry.routeMotionState = 4;
            return true;
        }
        if (entry.primarySlotIndex == -1) {
            if (!IsAreaWaitCommand(entry) && !IsAreaEndpoint(entry) && !state.freeRowCount) {
                FinishAreaRoute(entry);
                return false;
            }
            if (initializing) {
                SetListHead(state, &entry, 0);
                entry.nextControlRowIndex = 0;
                entry.routeMotionState = 4;
                return true;
            }
        }
    }
    if (initializing)
        return !OccupyAreaOwner(state, entry);
    if ((entry.commandFlags & 0x100U) && OccupyAreaOwner(state, entry))
        return false;
    if (ClaimAreaEntrance(state, entry) && entry.primarySlotIndex != -1 && !AreaProbeAhead(state, entry)) {
        if (entry.stateContext.otherName.name) {
            entry.routeMotionState = 0;
            entry.commandFlags &= ~2U;
        } else {
            QueueAreaMotion(entry, &ControlRows(state)[0].position);
            entry.routeMotionState = 5;
        }
        entry.flags |= 0x100U;
        return true;
    }
    SetListHead(state, &entry, 0);
    entry.routeMotionState = 4;
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ApplyAreaOwnerAnimation(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry, std::uint16_t motion) {
    NpcAreaOwnerRow *rows = reinterpret_cast<NpcAreaOwnerRow *>(static_cast<std::uintptr_t>(state.ownerRowsOffset));
    Vector3i rotation;
    Vector3 position;
    GetCharacterWorldPosition(entry.node, &position);
    position.x = rows[entry.ownerRowIndex].position.x;
    position.z = rows[entry.ownerRowIndex].position.z;
    position.x -= npcRouteOrigin.x;
    position.z -= npcRouteOrigin.z;
    rotation.y = rows[entry.ownerRowIndex].yaw;
    rotation.x = rotation.z = 0;
    SetCharacterWorldPosition(entry.node, &position);
    charaSetWorldRot(entry.node, rotation);
    mtwkRequestMotion(entry.motionWork, motion, LowerBody);
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    mtwkSetFrameTime(entry.motionWork, 24, LowerBody);
    mtwkSetExtraFlag(entry.motionWork, 15, 0);
    mtwkSetExtraFlag(entry.motionWork, 16, 2);
}

} // namespace

#else

namespace {

void ApplyAreaOwnerAnimation(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry, std::uint16_t motion) {
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    Vector3 position = GetCharacterWorldPosition(entry.node);
    position.x = owner.position.x - routeOrigin.x;
    position.z = owner.position.z - routeOrigin.z;
    shenApplyTranslationToAll(&entry.node->entity.node, position);
    charaSetWorldRot(entry.node, {0, owner.yaw, 0});
    mtwkRequestMotion(entry.motionWork, motion, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    mtwkSetFrameTime(entry.motionWork, 24, MotionRegion::LowerBody);
    mtwkSetExtraFlag(entry.motionWork, 15, 0);
    mtwkSetExtraFlag(entry.motionWork, 16, 2);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

NpcAreaControlRow *ControlRows(const NpcAreaRouteState &state) {
    return HeapPointer<NpcAreaControlRow>(state.controlRowsOffset);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

NpcAreaOwnerRow *OwnerRows(const NpcAreaRouteState &state) {
    return HeapPointer<NpcAreaOwnerRow>(state.ownerRowsOffset);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

NpcRouteLiveEntry *ListHead(const NpcAreaRouteState &state, std::size_t list) {
    const std::int32_t offset = state.npcListHeadOffset[list];
    return offset ? reinterpret_cast<NpcRouteLiveEntry *>(reinterpret_cast<std::uintptr_t>(&state) + offset) : nullptr;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetListHead(NpcAreaRouteState &state, NpcRouteLiveEntry *entry, std::size_t list) {
    if (!entry) {
        state.npcListHeadOffset[list] = 0;
        return;
    }
    const auto offset = reinterpret_cast<std::intptr_t>(entry) - reinterpret_cast<std::intptr_t>(&state);
    assert(offset >= (std::numeric_limits<std::int32_t>::min)() && offset <= (std::numeric_limits<std::int32_t>::max)());
    state.npcListHeadOffset[list] = static_cast<std::int32_t>(offset);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// npcStoreRegisterAreaRouteState. Each offset is relative to
// its state on disk, but becomes relative to the dynamic heap after publication.
void RegisterAreaState(NpcAreaRouteState &state, std::int16_t point) {
    NpcAreaStoreSlot &slot = areaStore[areaStoreCount];
    slot.state = &state;
    if (areaStoreCount) {
        slot.previous = &areaStore[areaStoreCount - 1];
        slot.previous->next = &slot;
    } else if (!nearestAreaSlot) {
        nearestAreaSlot = &slot;
    }
    ++areaStoreCount;
    auto *const base = reinterpret_cast<std::byte *>(&state);
    if (state.animationScriptOffset)
        state.animationScriptOffset = HeapOffset(base + state.animationScriptOffset);
    state.controlRowsOffset = HeapOffset(base + state.controlRowsOffset);
    state.ownerRowsOffset = HeapOffset(base + state.ownerRowsOffset);
    NpcAreaControlRow *const controls = ControlRows(state);
    for (std::int32_t index = 0; index < state.controlRowCount; ++index) {
        NpcAreaControlRow &row = controls[index];
        if (row.forwardOffset)
            row.forwardOffset = HeapOffset(base + row.forwardOffset);
        if (row.backwardOffset)
            row.backwardOffset = HeapOffset(base + row.backwardOffset);
    }
    state.areaPointIndex = point;
    state.field0a = 0;
    state.npcListHeadOffset[0] = state.npcListHeadOffset[1] = 0;
    state.activeRowCount = state.freeRowCount = state.occupiedRowCount = state.participantCount = 0;
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &row = owners[index];
        if (row.linkedControlOffset) {
            row.linkedControlOffset = HeapOffset(base + row.linkedControlOffset);
            if ((row.flags & 3U) != 1U)
                ++state.freeRowCount;
        }
        row.runtimeMask = 0;
    }
    state.flags |= 8U;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t AreaQueueCapacity(const NpcAreaRouteState &state) {
    static constexpr std::int32_t capacities[4]{3, 3, 0, 0};
    const std::int32_t capacity = capacities[g_actNumber];
    return (state.flags & 1U) ? capacity : (state.flags & 0x20U) ? 0 : capacity != 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool IsAreaWaitCommand(const NpcRouteLiveEntry &entry) {
    return entry.delayDuration != 0 || ((ScriptDword(entry.currentCommand) + 1U) & ~2U) == 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool IsAreaEndpoint(const NpcRouteLiveEntry &entry) {
    return static_cast<std::int16_t>(entry.currentAreaPoint) == static_cast<std::int32_t>(ScriptDword(entry.recordBase + 0x10));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool PrepareAreaDoor(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcRouteStateContext &context = entry.stateContext;
    context.otherName = areaPoints[state.areaPointIndex].owner;
    if (!(state.flags & 1U) || !context.otherName.name ||
        !ComputeDoorIngress(static_cast<std::int32_t>(context.otherName.name), &entry.localPosition, &context.scenePosition)) {
        context.otherName = {};
        return false;
    }
    context.targetYaw = static_cast<std::uint32_t>(g_doorIngressRotation.y);
    context.savedSplinePosition = GetDoorIngressOrigin();
    ComputeDoorEgress(static_cast<std::int32_t>(context.otherName.name), &entry.localPosition, &context.doorPosition);
    context.doorPosition.x += routeOrigin.x;
    context.doorPosition.z += routeOrigin.z;
    context.scenePosition.x += routeOrigin.x;
    context.scenePosition.z += routeOrigin.z;
    context.savedSplinePosition.x += routeOrigin.x;
    context.savedSplinePosition.z += routeOrigin.z;
    context.doorWaitFrames = 0;
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ProbeAreaState(NpcAreaRouteState &state) {
    NpcAreaControlRow *controls;
    NpcAreaOwnerRow *owners;
    Vector3 position;
    Vector3 delta;
    Vector3 segment;
    std::int32_t index, yaw, angle;
    float laneRadius;
    float playerRadius = npcPlayerRadius;
    float distance, width, length, along, lateral;
    if (!(state.flags |= 8U))
        return 1;
    if (playerRadius < 0.40000001f)
        playerRadius = 0.40000001f;
    controls = ControlRows(state);
    owners = OwnerRows(state);
    areaProbe.rowIndex = -1;
    areaProbe.linkedRowIndex = -1;
    areaProbe.ownerIndex = -1;
    areaProbe.nextOwnerIndex = -1;
    areaProbe.state = 0;
    for (index = 0; index < state.controlRowCount; ++index) {
        position = controls[index].position;
        delta = npcPlayerPosition;
        delta.x += npcRouteOrigin.x;
        delta.z += npcRouteOrigin.z;
        delta.x -= position.x;
        delta.y = 0.0f;
        delta.z -= position.z;
        yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
        distance = Vector3::CalcMagnitude(delta);
        laneRadius = AreaLaneWidth(controls[index]);
        if (controls[index].forwardOffset) {
            segment = reinterpret_cast<NpcAreaControlRow *>(controls[index].forwardOffset)->position;
            segment.x -= position.x;
            segment.y = 0.0f;
            segment.z -= position.z;
            angle = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-segment.x, -segment.z));
            length = controls[index].length;
            lateral = AreaLaneWidth(*reinterpret_cast<NpcAreaControlRow *>(controls[index].forwardOffset));
            width = laneRadius > lateral ? lateral : laneRadius;
            width += playerRadius;
            angle = static_cast<std::int16_t>(yaw - angle);
            if (static_cast<std::uint16_t>(angle + 0x3fff) <= 0x7ffe) {
                along = distance * dc::sdk::CosFixed(angle);
                lateral = dc::sdk::SinFixed(angle) * distance;
                if ((along < length && std::fabs(lateral) < width) || distance < laneRadius) {
                    areaProbe.lateralDistance = lateral;
                    areaProbe.parameter = along / length;
                    if (areaProbe.parameter > 1.0f)
                        areaProbe.parameter = 1.0f;
                    else if (areaProbe.parameter < 0.0f)
                        areaProbe.parameter = 0.0f;
                    areaProbe.rowIndex = static_cast<std::int8_t>(controls[index].index);
                    areaProbe.linkedRowIndex = static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(controls[index].forwardOffset)->index);
                    areaProbe.state = &state;
                    return 1;
                }
            }
        }
        if (laneRadius + playerRadius > distance) {
            areaProbe.lateralDistance = distance;
            areaProbe.parameter = 0.0f;
            areaProbe.rowIndex = static_cast<std::int8_t>(controls[index].index);
            areaProbe.state = &state;
            return 1;
        }
    }
    for (index = 0; index < state.ownerRowCount; ++index) {
        position = owners[index].position;
        delta = npcPlayerPosition;
        delta.x += npcRouteOrigin.x;
        delta.z += npcRouteOrigin.z;
        delta.x -= position.x;
        delta.y = 0.0f;
        delta.z -= position.z;
        yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
        distance = Vector3::CalcMagnitude(delta);
        width = playerRadius + 0.34999999f;
        if (owners[index].linkedControlOffset) {
            AreaLinkedPosition(owners[index], segment);
            segment.x -= position.x;
            segment.y = 0.0f;
            segment.z -= position.z;
            angle = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-segment.x, -segment.z));
            length = Vector3::CalcMagnitude(segment);
            angle = static_cast<std::int16_t>(yaw - angle);
            if (static_cast<std::uint16_t>(angle + 0x3fff) <= 0x7ffe) {
                along = distance * dc::sdk::CosFixed(angle);
                lateral = dc::sdk::SinFixed(angle) * distance;
                if (along < length && std::fabs(lateral) < width) {
                    areaProbe.lateralDistance = lateral;
                    areaProbe.parameter = along / length;
                    if (areaProbe.parameter > 1.0f)
                        areaProbe.parameter = 1.0f;
                    else if (areaProbe.parameter < 0.0f)
                        areaProbe.parameter = 0.0f;
                    areaProbe.ownerIndex = static_cast<std::int8_t>(owners[index].index);
                    areaProbe.linkedRowIndex = static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(owners[index].linkedControlOffset)->index);
                    areaProbe.state = &state;
                    return 1;
                }
            }
        }
        if (owners[index].flags & 4U) {
            segment = owners[index + 1].position;
            segment.x -= position.x;
            segment.y = 0.0f;
            segment.z -= position.z;
            angle = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-segment.x, -segment.z));
            length = Vector3::CalcMagnitude(segment);
            angle = static_cast<std::int16_t>(yaw - angle);
            if (static_cast<std::uint16_t>(angle + 0x3fff) <= 0x7ffe) {
                along = distance * dc::sdk::CosFixed(angle);
                lateral = dc::sdk::SinFixed(angle) * distance;
                if (along < length && std::fabs(lateral) < width) {
                    areaProbe.lateralDistance = lateral;
                    areaProbe.parameter = along / length;
                    if (areaProbe.parameter > 1.0f)
                        areaProbe.parameter = 1.0f;
                    else if (areaProbe.parameter < 0.0f)
                        areaProbe.parameter = 0.0f;
                    areaProbe.ownerIndex = static_cast<std::int8_t>(owners[index].index);
                    areaProbe.nextOwnerIndex = static_cast<std::int8_t>(owners[index + 1].index);
                    areaProbe.state = &state;
                    return 1;
                }
            }
        }
        if (width > distance) {
            areaProbe.lateralDistance = distance;
            areaProbe.parameter = 0.0f;
            areaProbe.ownerIndex = static_cast<std::int8_t>(owners[index].index);
            areaProbe.state = &state;
            return 1;
        }
    }
    return 0;
}

} // namespace

#else

namespace {

void ProbeAreaState(NpcAreaRouteState &state) {
    for (NpcAreaProbe &probe : areaProbes) {
        probe.rowIndex = probe.linkedRowIndex = probe.ownerIndex = probe.nextOwnerIndex = probe.ownerRowIndex = -1;
        probe.state = nullptr;
    }
    if (!(state.flags & 8U))
        return;
    const NpcAreaControlRow *const controls = ControlRows(state);
    std::int32_t index = 0;
    for (NpcAreaProbe &probe : areaProbes) {
        for (; index < state.controlRowCount; ++index) {
            if (ProbeAreaControlRow(controls[index], probe)) {
                probe.state = &state;
                break;
            }
        }
        ++index;
    }
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    index = 0;
    for (NpcAreaProbe &probe : areaProbes) {
        for (; index < state.ownerRowCount; ++index) {
            if (ProbeAreaOwnerRow(owners[index], probe)) {
                probe.state = &state;
                break;
            }
        }
        ++index;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateNearestAreaProbe() {
    if (npcNearestAreaSlot)
        ProbeAreaState(*npcNearestAreaSlot->state);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ReserveAreaProbe(NpcAreaRouteState &state) {
    NpcAreaControlRow *row = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(state.controlRowsOffset));
    if (areaProbe.rowIndex == -1)
        return;
    row += areaProbe.rowIndex;
    std::uint32_t *const forwardMask = &row->previousForwardMask;
    std::uint32_t *const backwardMask = &row->previousBackwardMask;
    if (areaProbe.linkedRowIndex == -1) {
        if (std::fabs(areaProbe.lateralDistance) < 0.69999999f) {
            row->previousForwardMask |= 7U;
            row->previousBackwardMask |= 7U;
            NpcAreaControlRow *const previous = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(row->backwardOffset));
            if (previous) {
                previous->previousForwardMask |= 7U << (static_cast<std::int8_t>(previous->spanCount) - 1);
                previous->previousBackwardMask |=
                    7U << (static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(row->backwardOffset))->spanCount) - 1);
            }
        }
        return;
    }
    const std::uint32_t span = static_cast<std::uint32_t>(areaProbe.parameter * static_cast<float>(static_cast<std::int8_t>(row->spanCount)));
    NpcAreaControlRow *other;
    if (areaProbe.lateralDistance < 0.40000001f) {
        const std::int32_t mask = static_cast<std::int32_t>(7U << span);
        *forwardMask |= static_cast<std::uint32_t>(mask | (mask >> 2));
        if (static_cast<std::int32_t>(span) < 2 && (other = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(row->backwardOffset))) != 0) {
            std::int32_t shift = static_cast<std::int8_t>(other->spanCount) - 1;
            other->previousForwardMask |= 7U << (shift + static_cast<std::int32_t>(span));
        } else if (static_cast<std::int32_t>(static_cast<std::int8_t>(row->spanCount)) - static_cast<std::int32_t>(span) < 2 &&
            (other = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(row->forwardOffset))) != 0) {
            other->previousForwardMask |=
                static_cast<std::uint32_t>(7 >> (static_cast<std::int32_t>(static_cast<std::int8_t>(row->spanCount)) - static_cast<std::int32_t>(span)));
        }
    }
    if (areaProbe.lateralDistance > -0.40000001f) {
        const std::int32_t mask = static_cast<std::int32_t>(7U << span);
        *backwardMask |= static_cast<std::uint32_t>(mask | (mask >> 2));
        if (static_cast<std::int32_t>(span) < 2 && (other = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(row->backwardOffset))) != 0) {
            std::int32_t shift = static_cast<std::int8_t>(other->spanCount) - 1;
            other->previousBackwardMask |= 7U << (shift + static_cast<std::int32_t>(span));
        } else if (static_cast<std::int32_t>(static_cast<std::int8_t>(row->spanCount)) - static_cast<std::int32_t>(span) < 2 &&
            (other = reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(row->forwardOffset))) != 0) {
            other->previousBackwardMask |=
                static_cast<std::uint32_t>(7 >> (static_cast<std::int32_t>(static_cast<std::int8_t>(row->spanCount)) - static_cast<std::int32_t>(span)));
        }
    }
}

} // namespace

#else

namespace {

void ReserveAreaProbe(const NpcAreaProbe &probe) {
    if (!probe.state || probe.rowIndex == -1)
        return;
    NpcAreaControlRow &row = ControlRows(*probe.state)[probe.rowIndex];
    NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(row.backwardOffset);
    if (probe.linkedRowIndex == -1) {
        if (std::abs(probe.lateralDistance) < 0.69999999f) {
            row.previousForwardMask |= 7U;
            row.previousBackwardMask |= 7U;
            if (previous) {
                const std::uint32_t mask = AreaMask(7, previous->spanCount - 1);
                previous->previousForwardMask |= mask;
                previous->previousBackwardMask |= mask;
            }
        }
        return;
    }
    NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
    const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * probe.rowParameter);
    const std::int32_t remaining = static_cast<std::int8_t>(row.spanCount) - span;
    const std::uint32_t mask = AreaMask(7, span);
    if (probe.lateralDistance < 0.40000001f) {
        row.previousForwardMask |= mask | (mask >> 2);
        if (span < 2 && previous)
            previous->previousForwardMask |= AreaMask(7, span + previous->spanCount - 1);
        else if (remaining < 2 && next)
            next->previousForwardMask |= 7U >> (static_cast<std::uint32_t>(remaining) & 31U);
    }
    if (probe.lateralDistance > -0.40000001f) {
        row.previousBackwardMask |= mask | (mask >> 2);
        if (span < 2 && previous)
            previous->previousBackwardMask |= AreaMask(7, span + previous->spanCount - 1);
        else if (remaining < 2 && next)
            next->previousBackwardMask |= 7U >> (static_cast<std::uint32_t>(remaining) & 31U);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool IsAreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaProbe &probe) {
    if (probe.state != &state)
        return false;
    const NpcAreaControlRow *const controls = ControlRows(state);
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t probeRow = probe.rowIndex;
    std::int32_t probeOwner = -1;
    float probeParameter = probe.rowParameter;
    if (probeRow == -1) {
        probeOwner = probe.ownerIndex;
        if (probeOwner == -1)
            return false;
        if (probe.ownerRowIndex != -1)
            probeRow = probe.ownerRowIndex;
        else {
            while (!owners[probeOwner].linkedControlOffset && (owners[probeOwner].flags & 8U))
                --probeOwner;
            const NpcAreaControlRow *const linked = HeapPointer<NpcAreaControlRow>(owners[probeOwner].linkedControlOffset);
            if (!linked)
                return false;
            probeRow = static_cast<std::int8_t>(linked->index);
        }
        probeParameter = owners[probeOwner].positionAlongSegment;
    }
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    std::int32_t currentRow;
    float parameter;
    if (entry.controlRowIndex == -1) {
        currentRow = entry.nextControlRowIndex;
        if (currentRow == -1)
            return false;
        parameter = currentRow ? 1.0f : 0.0f;
    } else {
        const NpcAreaControlRow &row = controls[entry.controlRowIndex];
        currentRow = forward || entry.nextControlRowIndex == -1 ? static_cast<std::int8_t>(row.index) :
                                                                  static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(row.backwardOffset)->index);
        parameter = AreaRowParameter(controls[currentRow], entry);
    }
    bool crossed = false;
    std::int32_t lane;
    for (;;) {
        const NpcAreaControlRow &row = controls[probeRow];
        lane = static_cast<std::int8_t>(row.laneMode);
        if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset))
            lane = std::min(lane, static_cast<std::int32_t>(static_cast<std::int8_t>(next->laneMode)));
        if (currentRow == probeRow)
            break;
        if (lane > 0 || (row.flags & 1U))
            return false;
        const NpcAreaControlRow *const adjacent = HeapPointer<NpcAreaControlRow>(forward ? row.backwardOffset : row.forwardOffset);
        if (!adjacent)
            return false;
        probeRow = static_cast<std::int8_t>(adjacent->index);
        crossed = true;
    }
    const std::int32_t spans = static_cast<std::int8_t>(controls[currentRow].spanCount);
    if (crossed) {
        if (!lane)
            return true;
        if (forward) {
            const float limit = spans ? static_cast<float>(std::max(0, spans - 5)) / static_cast<float>(spans) : 1.0f;
            return parameter > limit;
        }
        return parameter < (spans ? 5.0f / static_cast<float>(spans) : 1.0f);
    }
    if (!lane)
        return forward ? parameter <= probeParameter : probeParameter <= parameter;
    if (entry.nextOwnerRowIndex == -1)
        return false;
    if (probeOwner == -1) {
        const float margin = spans ? 3.0f / static_cast<float>(spans) : 1.0f;
        if (margin - std::abs(probeParameter - owners[entry.nextOwnerRowIndex].positionAlongSegment) < 0.0f)
            return false;
    } else if (entry.primarySlotIndex != probeOwner)
        return false;
    const float limit = spans ? 5.0f / static_cast<float>(spans) : 1.0f;
    if (forward)
        return parameter <= probeParameter && probeParameter - parameter <= limit;
    return probeParameter <= parameter && parameter - probeParameter <= limit;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool AreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    NpcAreaControlRow *controls;
    NpcAreaOwnerRow *owners;
    std::int32_t probeRow, probeOwner, currentRow, lane, next, spans, crossed;
    float probeParameter, parameter, limit;
    if (areaProbe.state != &state)
        return false;
    controls = ControlRows(state);
    owners = OwnerRows(state);
    if (areaProbe.rowIndex != -1) {
        probeRow = areaProbe.rowIndex;
        probeOwner = -1;
        probeParameter = areaProbe.parameter;
    } else if (areaProbe.ownerIndex != -1) {
        probeOwner = areaProbe.ownerIndex;
        if (areaProbe.linkedRowIndex != -1) {
            probeRow = areaProbe.linkedRowIndex;
        } else if (areaProbe.ownerIndex != -1) {
            while (!owners[probeOwner].linkedControlOffset && (owners[probeOwner].flags & 8U))
                --probeOwner;
            if (owners[probeOwner].linkedControlOffset)
                probeRow = static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(owners[probeOwner].linkedControlOffset)->index);
            else
                return false;
        } else {
            return false;
        }
        probeParameter = owners[probeOwner].positionAlongSegment;
    } else {
        return false;
    }
    if (entry.controlRowIndex != -1) {
        if (entry.opcodeFlags & 1U)
            currentRow = static_cast<std::int8_t>(controls[entry.controlRowIndex].index);
        else if (entry.nextControlRowIndex != -1)
            currentRow = static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(controls[entry.controlRowIndex].backwardOffset)->index);
        else
            currentRow = static_cast<std::int8_t>(controls[entry.controlRowIndex].index);
        parameter = AreaRowParameter(controls[currentRow], entry);
    } else {
        if (entry.nextControlRowIndex == -1)
            return false;
        currentRow = entry.nextControlRowIndex;
        parameter = !currentRow ? 0.0f : 1.0f;
    }
    crossed = 0;
    for (;;) {
        lane = static_cast<std::int8_t>(controls[probeRow].laneMode);
        if (controls[probeRow].forwardOffset) {
            next = static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(controls[probeRow].forwardOffset)->laneMode);
            if (lane > next)
                lane = next;
        }
        if (currentRow == probeRow) {
            if (crossed) {
                if (!lane)
                    return true;
                if (entry.opcodeFlags & 1U) {
                    if (static_cast<std::int8_t>(controls[currentRow].spanCount) > 4) {
                        std::int32_t count = static_cast<std::int8_t>(controls[currentRow].spanCount);
                        spans = count - 5;
                    } else {
                        spans = 0;
                    }
                    limit = static_cast<std::int8_t>(controls[currentRow].spanCount) ?
                        static_cast<float>(spans) / static_cast<float>(static_cast<std::int8_t>(controls[currentRow].spanCount)) :
                        1.0f;
                    if (parameter > limit)
                        return true;
                    return false;
                }
                limit = static_cast<std::int8_t>(controls[currentRow].spanCount) ?
                    5.0f / static_cast<float>(static_cast<std::int8_t>(controls[currentRow].spanCount)) :
                    1.0f;
                if (parameter < limit)
                    return true;
                return false;
            }
            if (!lane) {
                limit = static_cast<std::int8_t>(controls[currentRow].spanCount) ?
                    5.0f / static_cast<float>(static_cast<std::int8_t>(controls[currentRow].spanCount)) :
                    1.0f;
                if (entry.opcodeFlags & 1U) {
                    if (parameter > probeParameter)
                        return false;
                    return true;
                }
                if (probeParameter > parameter)
                    return false;
                return true;
            }
            if (entry.nextOwnerRowIndex == -1)
                return false;
            if (probeOwner != -1) {
                if (entry.primarySlotIndex != probeOwner)
                    return false;
            } else {
                limit = static_cast<std::int8_t>(controls[currentRow].spanCount) ?
                    3.0f / static_cast<float>(static_cast<std::int8_t>(controls[currentRow].spanCount)) :
                    1.0f;
                if (limit - std::fabs(probeParameter - owners[entry.nextOwnerRowIndex].positionAlongSegment) < 0.0f)
                    return false;
            }
            limit = static_cast<std::int8_t>(controls[currentRow].spanCount) ?
                5.0f / static_cast<float>(static_cast<std::int8_t>(controls[currentRow].spanCount)) :
                1.0f;
            if (entry.opcodeFlags & 1U) {
                if (parameter > probeParameter)
                    return false;
                if (probeParameter - parameter > limit)
                    return false;
                return true;
            }
            if (probeParameter > parameter)
                return false;
            if (parameter - probeParameter > limit)
                return false;
            return true;
        }
        if (lane > 0)
            return false;
        if (controls[probeRow].flags & 1U)
            return false;
        crossed = 1;
        if (entry.opcodeFlags & 1U) {
            if (controls[probeRow].backwardOffset)
                probeRow = static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(controls[probeRow].backwardOffset)->index);
            else
                return false;
        } else {
            if (controls[probeRow].forwardOffset)
                probeRow = static_cast<std::int8_t>(reinterpret_cast<NpcAreaControlRow *>(controls[probeRow].forwardOffset)->index);
            else
                return false;
        }
    }
}

} // namespace

#else

namespace {

bool AreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    return IsAreaProbeAhead(state, entry, areaProbes[0]) || IsAreaProbeAhead(state, entry, areaProbes[1]);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool IsAreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaProbe &probe) {
    if (probe.state != &state || probe.ownerIndex == -1)
        return false;
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const std::int32_t owner = entry.ownerRowIndex == -1 ? entry.nextOwnerRowIndex : entry.ownerRowIndex;
    if (owner == -1)
        return false;
    const bool exiting = entry.ownerRowIndex != -1 && (entry.nextOwnerRowIndex == -1 || entry.ownerRowIndex >= entry.nextOwnerRowIndex);
    std::int32_t cursor = probe.ownerIndex;
    if (owner != cursor) {
        const std::uint8_t flag = exiting ? 4U : 8U;
        while (owners[cursor].flags & flag) {
            cursor += exiting ? 1 : -1;
            if (owner == cursor)
                return true;
        }
        return false;
    }
    const bool linked = owners[cursor].linkedControlOffset != 0;
    const bool probeLinked = probe.ownerRowIndex != -1;
    Vector3 target;
    if (exiting) {
        if (entry.nextOwnerRowIndex != -1) {
            if (linked && probeLinked)
                return true;
            return AreaSegmentParameter(owners[owner - 1].position, owners[owner].position, entry.worldPosition) > probe.ownerParameter;
        }
        if (entry.nextControlRowIndex == -1)
            return true;
        if (linked && !probeLinked)
            return false;
        AreaLinkedPosition(owners[owner], target);
        return probe.ownerParameter > AreaSegmentParameter(owners[owner].position, target, entry.worldPosition);
    }
    if (entry.ownerRowIndex == -1) {
        if (linked && !probeLinked)
            return true;
        AreaLinkedPosition(owners[entry.nextOwnerRowIndex], target);
        return AreaSegmentParameter(owners[entry.nextOwnerRowIndex].position, target, entry.worldPosition) > probe.ownerParameter;
    }
    if (linked && probeLinked)
        return false;
    return probe.ownerParameter > AreaSegmentParameter(owners[entry.ownerRowIndex].position, owners[entry.nextOwnerRowIndex].position, entry.worldPosition);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool AreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *owners;
    std::int32_t owner, probeOwner, cursor, entering, crossed;
    float parameter;
    Vector3 target;
    if (areaProbe.state != &state)
        return false;
    owners = OwnerRows(state);
    if (entry.ownerRowIndex != -1)
        owner = entry.ownerRowIndex;
    else {
        if (!entry.nextOwnerRowIndex)
            return false;
        owner = entry.nextOwnerRowIndex;
    }
    if (areaProbe.ownerIndex == -1)
        return false;
    probeOwner = areaProbe.ownerIndex;
    parameter = areaProbe.parameter;
    if (entry.ownerRowIndex == -1)
        entering = 1;
    else if (entry.nextOwnerRowIndex == -1)
        entering = 0;
    else
        entering = entry.ownerRowIndex < entry.nextOwnerRowIndex;
    crossed = 0;
    if (entering) {
        cursor = probeOwner;
        for (;;) {
            if (cursor == owner) {
                if (crossed)
                    return true;
                if (entry.ownerRowIndex == -1) {
                    if (owners[cursor].linkedControlOffset && areaProbe.linkedRowIndex == -1)
                        return true;
                    AreaLinkedPosition(owners[entry.nextOwnerRowIndex], target);
                    if (AreaSegmentParameter(owners[entry.nextOwnerRowIndex].position, target, entry.worldPosition) > parameter)
                        return true;
                    return false;
                }
                if (owners[cursor].linkedControlOffset && areaProbe.linkedRowIndex != -1)
                    return false;
                if (parameter > AreaSegmentParameter(owners[entry.ownerRowIndex].position, owners[entry.nextOwnerRowIndex].position, entry.worldPosition))
                    return true;
                return false;
            }
            if (!(owners[cursor].flags & 8U))
                return false;
            crossed = 1;
            --cursor;
        }
    } else {
        cursor = probeOwner;
        for (;;) {
            if (cursor == owner) {
                if (crossed)
                    return true;
                if (entry.nextOwnerRowIndex == -1) {
                    if (entry.nextControlRowIndex == -1)
                        return true;
                    if (owners[cursor].linkedControlOffset && areaProbe.linkedRowIndex == -1)
                        return false;
                    AreaLinkedPosition(owners[entry.ownerRowIndex], target);
                    if (parameter > AreaSegmentParameter(owners[entry.ownerRowIndex].position, target, entry.worldPosition))
                        return true;
                    return false;
                }
                if (owners[cursor].linkedControlOffset && areaProbe.linkedRowIndex != -1)
                    return true;
                if (AreaSegmentParameter(owners[entry.ownerRowIndex - 1].position, owners[entry.ownerRowIndex].position, entry.worldPosition) > parameter)
                    return true;
                return false;
            }
            if (!(owners[cursor].flags & 4U))
                return false;
            crossed = 1;
            ++cursor;
        }
    }
}

} // namespace

#else

namespace {

bool AreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    // AreaOwnerProbeAhead invokes the first probe twice, unlike the control-row helper.
    return IsAreaOwnerProbeAhead(state, entry, areaProbes[0]);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline Vector3 AreaLerp(const Vector3 &first, const Vector3 &second, float parameter) {
    const float inverse = 1.0f - parameter;
    const Vector3 result = {inverse * first.x + parameter * second.x, inverse * first.y + parameter * second.y, inverse * first.z + parameter * second.z};
    return result;
}

} // namespace

#else

namespace {

Vector3 AreaLerp(const Vector3 &first, const Vector3 &second, float parameter) {
    const float inverse = 1.0f - parameter;
    return {inverse * first.x + parameter * second.x, inverse * first.y + parameter * second.y, inverse * first.z + parameter * second.z};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PrepareNpcAreaRoutes() {
    if (!npcAreaRouteStore)
        return;
    for (std::int32_t index = 0; index < npcAreaRouteCount && reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index].state; ++index) {
        NpcAreaRouteState &state = *reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index].state;
        state.participantCount = 0;
        NpcAreaControlRow *control = ControlRows(state);
        for (std::int32_t row = 0; row < state.controlRowCount; ++row, ++control) {
            control->previousForwardMask = control->forwardMask;
            control->previousBackwardMask = control->backwardMask;
            control->forwardMask = 0;
            control->backwardMask = 0;
        }
        if (!(state.flags & 8U))
            continue;
        UpdateAreaDistance(reinterpret_cast<NpcAreaStoreSlot *>(npcAreaRouteStore)[index]);
        if (state.flags & 1U) {
            const shen_name door = GetNpcAreaPointOwnerName(state.areaPointIndex);
            Vector3 position = ControlRows(state)[0].position;
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            if (door && !IsDoorAvailableForNpcRoute(door, &position))
                ReserveAreaEntrance(state);
        }
    }
    UpdateNearestAreaProbe();
    if (areaProbe.state)
        ReserveAreaProbe(*areaProbe.state);
}

#else

void PrepareNpcAreaRoutes() {
    if (!areaStore)
        return;

    for (std::size_t index = 0; index < areaStoreCount; ++index) {
        NpcAreaStoreSlot &slot = areaStore[index];
        if (!slot.state)
            break;

        NpcAreaRouteState &state = *slot.state;
        AdvanceAreaMasks(state);
        if (!(state.flags & 8U))
            continue;

        UpdateAreaDistance(slot);
        if (state.flags & 1U) {
            const shen_name owner = areaPoints[state.areaPointIndex].owner;
            if (owner.name && !IsDoorAvailableForNpcRoute(owner, NpcRouteAreaToScene(ControlRows(state)[0].position)))
                ReserveAreaEntrance(state);
        }
    }

    if (nearestAreaSlot)
        ProbeAreaState(*nearestAreaSlot->state);

    for (const NpcAreaProbe &probe : areaProbes)
        ReserveAreaProbe(probe);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool ResetNpcAreaRouteState(NpcRouteLiveEntry &entry) {
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (entry.areaPointPrevious)
        entry.areaPointPrevious->areaPointNext = entry.areaPointNext;

    if (entry.areaPointNext)
        entry.areaPointNext->areaPointPrevious = entry.areaPointPrevious;

    if (state && ListHead(*state, 0) == &entry)
        SetListHead(*state, entry.areaPointNext, 0);

    entry.areaPointPrevious = entry.areaPointNext = nullptr;

    if (entry.primarySlotIndex != -1) {
        if (state) {
            NpcAreaOwnerRow *const owners = OwnerRows(*state);
            NpcAreaOwnerRow &primary = owners[entry.primarySlotIndex];
            if (primary.runtimeMask & 1U) {
                primary.runtimeMask &= ~1U;
                if (primary.linkedControlOffset && (primary.flags & 3U) != 1) {
                    --state->activeRowCount;
                    ++state->freeRowCount;
                }
            }

            if (entry.ownerRowIndex != -1)
                owners[entry.ownerRowIndex].runtimeMask &= ~2U;

            if (entry.routeMotionState == 16)
                --state->occupiedRowCount;
        }
        entry.primarySlotIndex = -1;
    }

    if (state && entry.routeMotionState != 23)
        --state->participantCount;

    entry.routeMotionState = 23;
    entry.opcodeFlags = 0;
    entry.commandFlags = 0;
    entry.field2d6 = 0;
    entry.stateContext.otherName = {};
    entry.specialAnimationContext = nullptr;
    entry.field2d0 = 0;
    entry.flags &= ~0x1800100U;
    entry.streamCost = 2;
    if (!AreaUsesSpecialAttachment(entry))
        entry.motionAttachments[0].flags &= ~0x40U;
    if (entry.node)
        SetCharacterType(entry.node, CharacterTypeDefault);
    entry.motionAttachments[2].flags &= ~0x40U;
    entry.opcodeByte = 0;
    entry.waitParameter = 0;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool TryEnterNpcAreaRoute(NpcRouteLiveEntry &entry) {
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState != 23)
        return false;
    if (!IsAreaEndpoint(entry) && (ScriptDword(entry.currentCommand) != 3 || ScriptDword(entry.currentCommand + 8) != 0xffffffffU)) {
        if ((npcHourWeatherForecast >= 0.15f && (state->flags & 0x40U)) ||
            ((state->flags & 1U) && GetDoorOpeningState(areaPoints[state->areaPointIndex].owner) == 0)) {
            if (IsAreaWaitCommand(entry) && entry.node)
                mtwkCopyTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
            return false;
        }
    }
    entry.routeMotionState = 3;
    ++state->participantCount;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

template <class T> T *HeapPointer(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(HLib::DynamicHeapBase + offset) : nullptr;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t HeapOffset(const void *pointer) {
    if (!pointer)
        return 0;
    const auto offset = reinterpret_cast<std::intptr_t>(pointer) - reinterpret_cast<std::intptr_t>(HLib::DynamicHeapBase);
    assert(offset >= (std::numeric_limits<std::int32_t>::min)() && offset <= (std::numeric_limits<std::int32_t>::max)());
    return static_cast<std::int32_t>(offset);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float AreaSine(std::int32_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(angle));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float AreaCosine(std::int32_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(angle))));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t AreaYaw(const Vector3 &angles) {
    std::int32_t result;
    std::memcpy(&result, &angles.y, sizeof(result));
    return result;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetAreaYaw(Vector3 &angles, std::int32_t yaw) {
    std::memcpy(&angles.y, &yaw, sizeof(yaw));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float AreaDistanceSquared(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x;
    const float z = first.z - second.z;
    return x * x + z * z;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t TickNpcAreaRouteState(NpcRouteLiveEntry *entry) {
    if (!entry)
        return 0;
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry->currentAreaPoint);
    if (!state || entry->routeMotionState == 23)
        return 0;
    return state->flags & 8U ? TickNpcCrowdAreaState(state, entry) : TickNpcLocalAreaState(state, entry);
}

#else

// PC v1.07 TickNpcAreaRouteState. Return values on the
// internal transitions mean "dispatch the new phase now", never route completion.
bool TickNpcAreaRouteState(NpcRouteLiveEntry &entry) {
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState == 23)
        return false;
    if (!(state->flags & 8U))
        return TickInactiveAreaRoute(*state, entry);
    NpcAreaControlRow *const controls = ControlRows(*state);
    NpcAreaOwnerRow *const owners = OwnerRows(*state);
    entry.flags &= ~0x800000U;
    auto targetPosition = [&](Vector3 &position) {
        if (entry.nextControlRowIndex != -1) {
            if (entry.ownerRowIndex != -1)
                AreaLinkedPosition(owners[entry.ownerRowIndex], position);
            else
                position = controls[entry.nextControlRowIndex].position;
            return true;
        }
        if (entry.nextOwnerRowIndex != -1) {
            if (entry.controlRowIndex != -1) {
                const NpcAreaControlRow &row = controls[entry.controlRowIndex];
                position = HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset)->position;
            } else
                position = owners[entry.nextOwnerRowIndex].position;
            return true;
        }
        if (entry.routeMotionState == 22) {
            position = areaPoints[entry.currentAreaPoint].position;
            return true;
        }
        return false;
    };
    if (!entry.node) {
        entry.commandFlags &= 0xffeeU;
        if (entry.routeMotionState == 16) {
            SetAreaYaw(entry.euler, owners[entry.ownerRowIndex].yaw);
            entry.localPosition = NpcRouteAreaToScene(owners[entry.ownerRowIndex].position);
            PublishAreaWorldPosition(entry);
        }
        if ((entry.commandFlags & 0x46U) == 2U) {
            Vector3 target;
            if (targetPosition(target)) {
                target = NpcRouteAreaToScene(target);
                const float x = target.x - entry.localPosition.x;
                const float z = target.z - entry.localPosition.z;
                const float distance = std::sqrt(x * x + z * z);
                if (distance <= 0.029999999f) {
                    entry.localPosition.x = target.x;
                    entry.localPosition.z = target.z;
                } else {
                    SetAreaYaw(entry.euler, FixedAtan2(-x, -z));
                    entry.localPosition.x += 0.029999999f / distance * x;
                    entry.localPosition.z += 0.029999999f / distance * z;
                }
            }
        }
        if (entry.commandFlags & 2U)
            PublishAreaWorldPosition(entry);
    } else {
        const Vector3i rotation = GetCharacterWorldRotation(entry.node);
        std::memcpy(&entry.euler, &rotation, sizeof(rotation));
        entry.localPosition = GetCharacterWorldPosition(entry.node);
        if (entry.routeMotionState == 16 || entry.routeMotionState == 19) {
            if (!(entry.commandFlags & 0x10U))
                PrepareAreaOwnerGround(*state, entry);
        } else if (entry.routeMotionState != 2) {
            const float height = entry.localPosition.y;
            if (FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags)) {
                if (std::abs(height - entry.localPosition.y) > 0.0099999998f)
                    entry.localPosition.y = (height + entry.localPosition.y) * 0.5f;
                shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            } else
                entry.localPosition.y = height;
        }
        if ((entry.commandFlags & 2U) || entry.routeMotionState == 4)
            PublishAreaWorldPosition(entry);
        entry.commandFlags |= 0x10U;
        if (!(entry.commandFlags & 1U)) {
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~CCMF_EvaluateMotion;
            entry.commandFlags |= 1U;
        }
        if ((entry.commandFlags & 0x4eU) == 2U && !mtwkSetExtraFlag(entry.motionWork, 58, 1)) {
            if (entry.nextOwnerRowIndex != -1 && entry.controlRowIndex == -1 && entry.nextControlRowIndex == -1)
                QueueAreaOwnerTarget(*state, entry);
            else {
                Vector3 target;
                if (targetPosition(target)) {
                    QueueAreaMotion(entry, &target);
                    if (entry.routeMotionState == 22)
                        mtwkCopyTransition(entry.motionWork, nullptr, 0, 2, 0);
                }
            }
        }
    }
    if (state->flags & 0x20U) {
        entry.motionAttachments[0].flags &= ~0x40U;
        if (entry.node)
            SetCharacterType(entry.node, CharacterTypeDefault);
    } else {
        entry.motionAttachments[0].flags |= 0x40U;
        if (entry.node)
            SetCharacterType(entry.node, 3);
    }
    entry.motionAttachments[2].flags |= 0x40U;
    auto detach = [&] {
        if (!AreaUsesSpecialAttachment(entry))
            entry.motionAttachments[0].flags &= ~0x40U;
        if (entry.node)
            SetCharacterType(entry.node, CharacterTypeDefault);
    };
    auto queueExit = [&] {
        entry.routeMotionState = 21;
        PrepareAreaDoor(*state, entry);
    };
    for (;;) {
        switch (entry.routeMotionState) {
        case 0:
            ReserveAreaEntrance(*state);
            if (entry.commandFlags & 0x40U)
                QueueAreaMotion(entry, nullptr);
            else {
                QueueAreaDoorMotion(entry);
                entry.routeMotionState = 1;
            }
            return true;
        case 1: {
            ReserveAreaEntrance(*state);
            NpcRouteStateContext &context = entry.stateContext;
            if ((entry.opcodeFlags & 2U) && AreaDistanceSquared(context.scenePosition, entry.worldPosition) > 1.0f)
                detach();
            if ((!AreaDoorPathClear(entry) || (entry.commandFlags & 0x40U) || AreaProbeAhead(*state, entry)) &&
                (!entry.node || mtwkSetExtraFlag(entry.motionWork, 58, 1))) {
                ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), entry.name);
                if (entry.node)
                    mtwkSetTimeRelated(entry.motionWork, 1, 5, 0);
                entry.flags &= ~0x100U;
                if (entry.opcodeFlags & 2U) {
                    entry.areaPointNext = ListHead(*state, 0);
                    entry.areaPointPrevious = nullptr;
                    SetListHead(*state, &entry, 0);
                    if (entry.areaPointNext)
                        entry.areaPointNext->areaPointPrevious = &entry;
                    entry.routeMotionState = 4;
                    continue;
                }
                QueueAreaMotion(entry, nullptr);
                entry.routeMotionState = 21;
            } else if (entry.node) {
                if (mtwkCopyTransition(entry.motionWork, nullptr, 0, 3, 0))
                    QueueAreaDoorMotion(entry);
                else if (!mtwkSetExtraFlag(entry.motionWork, 58, 1)) {
                    entry.routeMotionState = 2;
                    BeginDoorAnimation(static_cast<std::int32_t>(context.otherName.name), entry.name, context.doorMotion, 14.0f, entry.motionSpeed * 1.1f);
                    mtwkSetSpeed(entry.motionWork, entry.motionSpeed * 1.1f, MotionRegion::LowerBody);
                    mtwkUpdateWithPosition(entry.motionWork, nullptr, 0);
                }
                PublishAreaWorldPosition(entry);
            } else if (context.doorWaitFrames == -1) {
                const float x = context.scenePosition.x - entry.worldPosition.x;
                const float z = context.scenePosition.z - entry.worldPosition.z;
                SetAreaYaw(entry.euler, context.targetYaw);
                const float distance = std::sqrt(x * x + z * z);
                if (distance > entry.motionSpeed * 0.04f) {
                    entry.worldPosition.x += x * 0.039999999f * entry.motionSpeed / distance;
                    entry.worldPosition.z += z * 0.039999999f * entry.motionSpeed / distance;
                } else {
                    entry.worldPosition = context.doorPosition;
                    ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), entry.name);
                    if (entry.opcodeFlags & 2U) {
                        entry.routeMotionState = 5;
                        context.otherName = {};
                    } else
                        FinishAreaRoute(entry);
                }
                entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
            }
            return true;
        }
        case 2: {
            ReserveAreaEntrance(*state);
            entry.motionAttachments[0].flags |= 0x40U;
            if (entry.node) {
                SetCharacterType(entry.node, 3);
                if (!mtwkSetRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) {
                    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
                    return true;
                }
                if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                    control->flags = (control->flags | CCMF_FineRaycast) & ~CCMF_StairMode;
                entry.commandFlags &= ~1U;
                Vector3 position = NpcRouteAreaToScene(entry.worldPosition);
                if (!FindNpcIntersectionInScene(position, entry.packedMoveFlags))
                    RequestNpcStreamOut(entry, 0);
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
                entry.stateContext.otherName = {};
                if (entry.opcodeFlags & 2U) {
                    if (entry.commandFlags & 0x20U) {
                        RequestNpcAreaRouteCompletion(entry);
                        entry.commandFlags &= ~0x20U;
                    } else
                        entry.routeMotionState = 5;
                    continue;
                }
            } else {
                entry.worldPosition = entry.stateContext.doorPosition;
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
                entry.stateContext.otherName = {};
                if (entry.opcodeFlags & 2U) {
                    entry.routeMotionState = 5;
                    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
                    return true;
                }
            }
            FinishAreaRoute(entry);
            entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
            return true;
        }
        case 3:
            if (AdmitAreaRoute(*state, entry, false))
                continue;
            return true;
        case 4:
            if (entry.stateContext.otherName.name)
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
            detach();
            if (entry.commandFlags & 0x40U) {
                QueueAreaMotion(entry, nullptr);
                return true;
            }
            if (!MaintainAreaQueue(*state, entry))
                return true;
            if ((state->flags & 1U) && GetDoorOpeningState(areaPoints[state->areaPointIndex].owner) == 0) {
                if (!IsAreaWaitCommand(entry)) {
                    mtwkUpdateWithPosition(entry.motionWork, nullptr, 0);
                    RequestNpcAreaRouteCompletion(entry);
                    return true;
                }
                if (entry.node)
                    mtwkCopyTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
            }
            if (ListHead(*state, 0) != &entry || entry.areaPointPrevious) {
                PositionAreaQueueMember(entry, areaPoints[entry.currentAreaPoint].position, entry.areaPointPrevious->worldPosition, 0.80000001f);
                entry.commandFlags &= ~2U;
                return true;
            }
            if (entry.primarySlotIndex == -1 || !ClaimAreaEntrance(*state, entry) || AreaProbeAhead(*state, entry) ||
                static_cast<std::int8_t>(state->participantCount) - static_cast<std::int8_t>(state->occupiedRowCount) - AreaQueueLength(*state) >= 2) {
                PositionAreaQueueHead(*state, entry);
                entry.commandFlags &= ~2U;
                return true;
            }
            mtwkUpdateWithPosition(entry.motionWork, nullptr, 0);
            SetListHead(*state, entry.areaPointNext, 0);
            if (entry.areaPointNext) {
                entry.areaPointNext->areaPointPrevious = nullptr;
                entry.areaPointNext = nullptr;
            }
            entry.flags |= 0x100U;
            if (entry.stateContext.otherName.name) {
                entry.commandFlags &= ~2U;
                entry.routeMotionState = 0;
            } else {
                entry.commandFlags |= 2U;
                QueueAreaMotion(entry, &controls[0].position);
                entry.routeMotionState = 5;
            }
            continue;
        case 5:
            entry.commandFlags |= 2U;
            if ((entry.opcodeFlags & 2U) && !(state->flags & 1U)) {
                if (AreaDistanceSquared(controls[0].position, entry.worldPosition) > 1.0f)
                    detach();
                if (QueryNpcMoveObstruction(entry, controls[0].position, false, 0x3000, entry.streamRadius, 1.5f) & 1U) {
                    QueueAreaMotion(entry, nullptr);
                    ReserveAreaEntrance(*state);
                    return true;
                }
            }
            if ((entry.commandFlags & 0x40U) || AreaProbeAhead(*state, entry)) {
                QueueAreaMotion(entry, nullptr);
                ReserveAreaEntrance(*state);
                return true;
            }
            if (!AreaTargetReached(*state, entry) && (!(entry.opcodeFlags & 2U) || !(state->flags & 1U))) {
                ReserveAreaEntrance(*state);
                return true;
            }
            AdvanceAreaControlRow(*state, entry);
            entry.flags &= ~0x100U;
            entry.routeMotionState = 6;
            continue;
        case 6:
            entry.commandFlags |= 2U;
            if (entry.nextControlRowIndex != -1) {
                QueueAreaMotion(entry, &controls[entry.nextControlRowIndex].position);
                const std::int8_t row = (entry.opcodeFlags & 1U) ? entry.controlRowIndex : entry.nextControlRowIndex;
                if (HeapPointer<NpcAreaControlRow>(owners[entry.primarySlotIndex].linkedControlOffset) == &controls[row]) {
                    if (entry.node)
                        mtwkSetTimeRelated(entry.motionWork, 0, 6, 0);
                    entry.routeMotionState = 8;
                    entry.commandFlags &= ~4U;
                    entry.nextOwnerRowIndex = entry.primarySlotIndex;
                    entry.nextControlRowIndex = -1;
                } else
                    entry.routeMotionState = (entry.commandFlags & 4U) ? 11 : 7;
                return true;
            }
            if (HeapPointer<NpcAreaControlRow>(owners[entry.primarySlotIndex].linkedControlOffset) == &controls[entry.controlRowIndex]) {
                entry.nextOwnerRowIndex = entry.primarySlotIndex;
                EnterAreaOwnerLane(*state, entry);
                entry.commandFlags &= ~4U;
            } else {
                entry.opcodeFlags &= ~2U;
                const NpcAreaControlRow &row = controls[entry.controlRowIndex];
                if (row.flags & 1U)
                    queueExit();
                else {
                    entry.nextControlRowIndex = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(row.backwardOffset)->index);
                    if (entry.commandFlags & 4U)
                        entry.routeMotionState = 11;
                    else {
                        QueueAreaMotion(entry, &controls[entry.nextControlRowIndex].position);
                        entry.routeMotionState = 7;
                    }
                }
            }
            continue;
        case 7:
        case 11:
            if (TickAreaTraversal(*state, entry, entry.routeMotionState == 11))
                continue;
            return true;
        case 8:
        case 12:
            if (TickAreaOwnerApproach(*state, entry, entry.routeMotionState == 12))
                continue;
            return true;
        case 9: {
            if (!AreaOwnerLaneAvailable(*state, entry) || (entry.commandFlags & 0x40U) || AreaOwnerProbeAhead(*state, entry)) {
                entry.commandFlags &= ~2U;
                QueueAreaMotion(entry, nullptr);
                entry.routeMotionState = 15;
                return true;
            }
            if (!AreaTargetReached(*state, entry))
                return true;
            NpcAreaOwnerRow &owner = owners[entry.ownerRowIndex];
            owner.runtimeMask &= ~1U;
            entry.primarySlotIndex = -1;
            --state->activeRowCount;
            ++state->freeRowCount;
            const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
            entry.controlRowIndex = static_cast<std::int8_t>(row.index);
            const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
            if (entry.opcodeFlags & 1U)
                entry.nextControlRowIndex = next ? static_cast<std::int8_t>(next->index) : -1;
            else if (next) {
                entry.nextControlRowIndex = entry.controlRowIndex;
                entry.controlRowIndex = static_cast<std::int8_t>(next->index);
            } else if (row.flags & 1U)
                entry.nextControlRowIndex = -1;
            else
                entry.nextControlRowIndex = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(row.backwardOffset)->index);
            entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
            if (entry.nextControlRowIndex != -1) {
                entry.commandFlags |= 2U;
                QueueAreaMotion(entry, &controls[entry.nextControlRowIndex].position);
                entry.routeMotionState = 7;
            } else if (!(entry.opcodeFlags & 2U))
                queueExit();
            else
                entry.routeMotionState = 6;
            continue;
        }
        case 10:
            if (!(entry.commandFlags & 0x40U) && (entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
                entry.opcodeFlags &= ~2U;
                entry.commandFlags &= ~0x20U;
                entry.ownerRowIndex = entry.nextOwnerRowIndex;
                SelectAreaExitDirection(*state, entry);
                QueueAreaOwnerExit(*state, entry);
                return true;
            }
            if ((entry.commandFlags & 0x40U) || AreaOwnerProbeAhead(*state, entry)) {
                QueueAreaMotion(entry, nullptr);
                entry.commandFlags &= ~8U;
                return true;
            }
            if (!AreaTargetReached(*state, entry))
                return true;
            entry.ownerRowIndex = entry.nextOwnerRowIndex;
            if (entry.opcodeFlags & 2U) {
                if (owners[entry.ownerRowIndex].flags & 4U) {
                    entry.commandFlags |= 2U;
                    entry.nextOwnerRowIndex = static_cast<std::int8_t>(entry.ownerRowIndex + 1);
                    QueueAreaOwnerTarget(*state, entry);
                } else {
                    entry.nextOwnerRowIndex = -1;
                    entry.field2e8 = 0;
                    entry.field2d8 &= ~0xffff0000ULL;
                    ++state->occupiedRowCount;
                    entry.routeMotionState = 16;
                }
            } else
                QueueAreaOwnerExit(*state, entry);
            continue;
        case 13:
        case 14:
            if (TickAreaWaiting(*state, entry, entry.routeMotionState == 14))
                continue;
            return true;
        case 15:
            if (AreaOwnerLaneAvailable(*state, entry) && !(entry.commandFlags & 0x40U) && !AreaOwnerProbeAhead(*state, entry)) {
                entry.commandFlags |= 2U;
                Vector3 target;
                AreaLinkedPosition(owners[entry.ownerRowIndex], target);
                QueueAreaMotion(entry, &target);
                entry.routeMotionState = 9;
            }
            return true;
        case 16:
            if (TickAreaOwnerAnimation(*state, entry))
                continue;
            return true;
        case 18:
            TickNpcMotionClip(entry);
            return true;
        case 19:
            entry.commandFlags &= ~2U;
            if (entry.node && !mtwkSetRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody))
                return true;
            entry.routeMotionState = 20;
            entry.commandFlags &= ~8U;
            entry.flags &= ~0x100U;
            if (!entry.node && (entry.commandFlags & 0x1000U))
                RestoreAreaSavedPosition(entry);
            continue;
        case 20: {
            entry.opcodeFlags &= ~2U;
            if (SelectAreaExitDirection(*state, entry)) {
                entry.commandFlags |= 2U;
                const NpcAreaOwnerRow &owner = owners[entry.ownerRowIndex];
                if (owner.linkedControlOffset) {
                    entry.nextControlRowIndex = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset)->index);
                    Vector3 target;
                    AreaLinkedPosition(owner, target);
                    if (!(entry.commandFlags & 0x40U))
                        QueueAreaMotion(entry, &target);
                    entry.routeMotionState = 9;
                } else if (owner.flags & 8U) {
                    entry.nextOwnerRowIndex = static_cast<std::int8_t>(entry.ownerRowIndex - 1);
                    if (!(entry.commandFlags & 0x40U))
                        QueueAreaMotion(entry, &owners[entry.nextOwnerRowIndex].position);
                    entry.routeMotionState = 10;
                }
                entry.field2e8 = 0;
                if (entry.node) {
                    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                        control->flags &= ~CCMF_StairMode;
                }
                entry.commandFlags &= ~0x800U;
            } else {
                entry.specialAnimationContext = nullptr;
                entry.routeMotionState = 16;
                entry.field2e8 = 0;
            }
            return true;
        }
        case 21:
            if (!ClaimAreaEntrance(*state, entry) || (entry.commandFlags & 0x40U)) {
                entry.commandFlags &= ~2U;
                ReserveAreaLane(*state, entry);
                QueueAreaMotion(entry, nullptr);
                return true;
            }
            entry.flags |= 0x100U;
            if (entry.stateContext.otherName.name) {
                PublishAreaWorldPosition(entry);
                entry.commandFlags &= ~2U;
                entry.routeMotionState = 0;
            } else {
                entry.commandFlags |= 2U;
                if (QueueAreaMotion(entry, &areaPoints[entry.currentAreaPoint].position))
                    mtwkCopyTransition(entry.motionWork, nullptr, 0, 2, 0);
                entry.routeMotionState = 22;
            }
            continue;
        case 22: {
            detach();
            const Vector3 &target = areaPoints[entry.currentAreaPoint].position;
            if ((entry.commandFlags & 0x40U) || (QueryNpcMoveObstruction(entry, target, false, 0x3000, entry.streamRadius, 1.5f) & 1U)) {
                ReserveAreaEntrance(*state);
                QueueAreaMotion(entry, nullptr);
                return true;
            }
            if ((entry.node && !mtwkSetExtraFlag(entry.motionWork, 58, 1)) || AreaDistanceSquared(NpcRouteAreaToScene(target), entry.localPosition) < 1.0f)
                FinishAreaRoute(entry);
            ReserveAreaEntrance(*state);
            return true;
        }
        default:
            return true;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void RestoreNpcAreaMotion(NpcRouteLiveEntry &entry) {
    const NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState != 16)
        return;
    mtwkCopyTransition(entry.motionWork, &entry.specialMotionSavedPosition, static_cast<std::uint16_t>(OwnerRows(*state)[entry.ownerRowIndex].yaw), 6, 0);
    mtwkSetTimeRelated(entry.motionWork, 1, 5, entry.currentMotionId);
    mtwkSetRegionEvalTime(entry.motionWork, entry.animationFrame, MotionRegion::LowerBody);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t TickNpcCrowdAreaState(NpcAreaRouteState *state, NpcRouteLiveEntry *entry) {
    std::int32_t capacity = npcAreaQueueCapacities[g_actNumber];
    NpcAreaControlRow *const controls = ControlRows(*state);
    NpcAreaOwnerRow *const owners = OwnerRows(*state);
    Vector3 target;
    Vector3 delta;
    Vector3 point;
    Vector3 doorDelta;
    Vector3 doorStep;
    float distance;
    if (!entry || !state)
        return 0;
    ++state->participantCount;

    if (entry->node) {
        GetCharacterWorldRotation(entry->node, &entry->rotation);
        GetCharacterWorldPosition(entry->node, &entry->localPosition);
        const float height = entry->localPosition.y;
        if (entry->routeMotionState == 16 || entry->routeMotionState == 19) {
            if (!(entry->commandFlags & 0x10U))
                PrepareAreaOwnerGround(*state, *entry);
        } else if (FindNpcIntersectionInScene(entry->localPosition, entry->packedMoveFlags)) {
            if (std::fabs(height - entry->localPosition.y) > 0.0099999998f)
                entry->localPosition.y = (height + entry->localPosition.y) * 0.5f;
            SetCharacterWorldPosition(entry->node, &entry->localPosition);
        } else
            entry->localPosition.y = height;
        PublishAreaWorldPosition(*entry);
        entry->commandFlags |= 0x10U;
        if ((entry->commandFlags & 1U) == 0) {
            Character *const node = entry->node;
            AssignCharacterControlFlags(node, ccmGetFlags(node) & ~CCMF_EvaluateMotion);
            entry->commandFlags |= 1U;
        }
        if ((entry->commandFlags & 0x46U) == 2U && !(entry->commandFlags & 8U) && !mtwkSetExtraFlag(entry->motionWork, 58, 1)) {
            if (entry->nextControlRowIndex != -1) {
                if (entry->ownerRowIndex != -1)
                    AreaLinkedPosition(owners[entry->ownerRowIndex], target);
                else
                    target = controls[entry->nextControlRowIndex].position;
                QueueAreaMotion(entry, &target);
            } else if (entry->nextOwnerRowIndex != -1) {
                if (entry->controlRowIndex != -1) {
                    target = HeapPointer<NpcAreaControlRow>(
                        (entry->opcodeFlags & 1U) ? controls[entry->controlRowIndex].forwardOffset : controls[entry->controlRowIndex].backwardOffset)
                                 ->position;
                    QueueAreaMotion(entry, &target);
                } else {
                    const std::uint8_t action = owners[entry->nextOwnerRowIndex].actionKind;
                    if (owners[entry->nextOwnerRowIndex].flags & 4U)
                        QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position, &owners[entry->nextOwnerRowIndex + 1].position);
                    else if (action == 2 || action == 6 || action == 7 || action == 16 || action == 17 || action == 18)
                        QueueAreaOwnerApproach(*state, *entry);
                    else {
                        const std::uint16_t motion = ResolveNpcAreaOwnerAnimation(state, entry);
                        QueueAreaMotion(
                            entry, &owners[entry->nextOwnerRowIndex].position, 0, static_cast<std::int16_t>(owners[entry->nextOwnerRowIndex].yaw), motion);
                    }
                }
            } else if (entry->routeMotionState == 22) {
                QueueAreaMotion(entry, GetNpcAreaPointPosition(entry->currentAreaPoint));
                mtwkCopyTransition(entry->motionWork, 0, 0, 2, 0);
            }
        }
    } else {
        entry->commandFlags &= ~1U;
        entry->commandFlags &= ~0x10U;
        if (entry->routeMotionState == 16) {
            entry->rotation.y = owners[entry->ownerRowIndex].yaw;
            target = owners[entry->ownerRowIndex].position;
            target.x -= npcRouteOrigin.x;
            target.z -= npcRouteOrigin.z;
            entry->localPosition = entry->worldPosition = target;
            entry->worldPosition.x += npcRouteOrigin.x;
            entry->worldPosition.z += npcRouteOrigin.z;
        }
        if ((entry->commandFlags & 6U) == 2U && !(entry->commandFlags & 0x40U)) {
            if (entry->nextControlRowIndex != -1) {
                if (entry->ownerRowIndex != -1)
                    AreaLinkedPosition(owners[entry->ownerRowIndex], target);
                else
                    target = controls[entry->nextControlRowIndex].position;
                target.x -= npcRouteOrigin.x;
                target.z -= npcRouteOrigin.z;
            } else if (entry->nextOwnerRowIndex != -1) {
                if (entry->controlRowIndex != -1) {
                    if (entry->opcodeFlags & 1U)
                        target = HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].forwardOffset)->position;
                    else
                        target = HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].backwardOffset)->position;
                } else
                    target = owners[entry->nextOwnerRowIndex].position;
                target.x -= npcRouteOrigin.x;
                target.z -= npcRouteOrigin.z;
            } else if (entry->routeMotionState == 22) {
                target = *GetNpcAreaPointPosition(entry->currentAreaPoint);
                target.x -= npcRouteOrigin.x;
                target.z -= npcRouteOrigin.z;
            } else
                target = entry->localPosition;
            delta.x = target.x - entry->localPosition.x;
            delta.y = 0.0f;
            delta.z = target.z - entry->localPosition.z;
            distance = Vector3::CalcMagnitude(delta);
            entry->rotation.y = dc::sdk::ArcTan2Fixed(-delta.x, -delta.z);
            if (distance > 1.1920929e-07f) {
                distance = 0.029999999f / distance;
                entry->localPosition.x += distance * delta.x;
                entry->localPosition.z += distance * delta.z;
            }
        }
        PublishAreaWorldPosition(*entry);
    }
    if (state->flags & 0x20U)
        entry->motionAttachments[0].flags &= ~0x40U;
    else
        entry->motionAttachments[0].flags |= 0x40U;
    entry->motionAttachments[2].flags |= 0x40U;

    std::int32_t loop;
    loop = 1;
    do {
        switch (entry->routeMotionState) {
        case 3:
            entry->controlRowIndex = -1;
            entry->nextControlRowIndex = 0;
            entry->ownerRowIndex = -1;
            entry->nextOwnerRowIndex = -1;
            entry->specialAnimationContext = 0;
            PrepareNpcAreaDoor(state, entry);
            if (entry->primarySlotIndex == -1) {
                if (ScriptDword(entry->currentCommand) == 19 && SelectClipAreaOwner(*state, *entry)) {
                    OccupyAreaOwner(*state, *entry);
                    entry->routeMotionState = 18;
                    entry->motionSpeed = 1.0f;
                    entry->retained324 = entry->currentCommand[4];
                    entry->currentCommand += 8;
                    entry->streamCost = 1;
                    entry->routeQueryState = 3;
                    entry->animationFrame = static_cast<std::int8_t>(entry->scriptRecord[8]) * 2;
                    entry->flags |= 0x100U;
                    break;
                }
                if (entry->currentAreaPoint != AreaEndpoint(*entry)) {
                    if (ListHead(*state, 0)) {
                        NpcRouteLiveEntry *tail = ListHead(*state, 0);
                        std::uint8_t count = 1;
                        while (tail->areaPointNext) {
                            tail = tail->areaPointNext;
                            ++count;
                        }
                        if ((state->flags & 1U) == 0) {
                            if (state->flags & 0x20U)
                                capacity = 0;
                            else
                                capacity = capacity != 0;
                        }
                        if (!entry->waitDuration && static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) != -1 &&
                            static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) != 1 && count > capacity - 1) {
                            DisableNpcAreaRoute(entry);
                            loop = 0;
                            break;
                        }
                        tail->areaPointNext = entry;
                        entry->areaPointPrevious = tail;
                        entry->routeMotionState = 4;
                        break;
                    }
                    if (state->freeRowCount)
                        SelectFreeAreaOwner(*state, *entry);
                } else {
                    const std::int32_t reserved = SelectEndpointOwner(*state, *entry);
                    if (reserved == 2) {
                        OccupyAreaOwner(*state, *entry);
                        break;
                    }
                    if (!reserved) {
                        DisableNpcAreaRoute(entry);
                        loop = 0;
                        break;
                    }
                    if (ListHead(*state, 0)) {
                        NpcRouteLiveEntry *tail = ListHead(*state, 0);
                        while (tail->areaPointNext)
                            tail = tail->areaPointNext;
                        tail->areaPointNext = entry;
                        entry->areaPointPrevious = tail;
                        entry->routeMotionState = 4;
                        break;
                    }
                }
            }
            if (entry->primarySlotIndex == -1 && !entry->waitDuration && static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) != -1 &&
                static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) != 1 && entry->currentAreaPoint != AreaEndpoint(*entry)) {
                DisableNpcAreaRoute(entry);
                loop = 0;
                break;
            }
            if ((entry->commandFlags & 0x100U) && OccupyAreaOwner(*state, *entry)) {
                loop = 0;
                break;
            }
            if (!ClaimAreaEntrance(*state, *entry) || entry->primarySlotIndex == -1 || AreaProbeAhead(*state, *entry)) {
                SetListHead(*state, entry, 0);
                entry->routeMotionState = 4;
                break;
            }
            if (entry->stateContext.otherName) {
                entry->commandFlags &= ~2U;
                entry->routeMotionState = 0;
            } else {
                QueueAreaMotion(entry, &controls[0].position);
                entry->routeMotionState = 5;
            }
            entry->flags |= 0x100U;
            break;
        case 4:
            if (entry->stateContext.otherName)
                ReleaseDoor(entry->stateContext.otherName, entry->name);
            entry->motionAttachments[0].flags &= ~0x40U;
            if (entry->commandFlags & 0x40U) {
                QueueAreaMotion(entry, 0);
                loop = 0;
                break;
            }
            if (!MaintainAreaQueue(*state, *entry)) {
                loop = 0;
                break;
            }
            if (entry == ListHead(*state, 0) && !entry->areaPointPrevious) {
                if (entry->primarySlotIndex == -1) {
                    PositionAreaQueueHead(*state, *entry);
                    entry->commandFlags &= ~2U;
                    loop = 0;
                    break;
                }
                if (!ClaimAreaEntrance(*state, *entry) || AreaProbeAhead(*state, *entry)) {
                    PositionAreaQueueHead(*state, *entry);
                    entry->commandFlags &= ~2U;
                    loop = 0;
                    break;
                }
                mtwkUpdateWithPosition(entry->motionWork, 0, 0);
                SetListHead(*state, entry->areaPointNext, 0);
                if (entry->areaPointNext) {
                    entry->areaPointNext->areaPointPrevious = 0;
                    entry->areaPointNext = 0;
                }
                if (entry->stateContext.otherName) {
                    entry->commandFlags &= ~2U;
                    entry->routeMotionState = 0;
                } else {
                    entry->commandFlags |= 2U;
                    QueueAreaMotion(entry, &controls[0].position);
                    entry->routeMotionState = 5;
                }
                entry->flags |= 0x100U;
                break;
            }
            PositionAreaQueueFollower(*entry);
            entry->commandFlags &= ~2U;
            loop = 0;
            break;
        case 0:
            ReserveAreaEntrance(*state);
            if (entry->commandFlags & 0x40U) {
                QueueAreaMotion(entry, 0);
                loop = 0;
                break;
            }
            QueueNpcAreaDoorMotion(entry);
            entry->routeMotionState = 1;
            loop = 0;
            break;
        case 1: {
            ReserveAreaEntrance(*state);
            if (entry->opcodeFlags & 2U) {
                doorDelta.x = entry->stateContext.position.x - entry->worldPosition.x;
                doorDelta.y = 0.0f;
                doorDelta.z = entry->stateContext.position.z - entry->worldPosition.z;
                if (Vector3::CalcMagnitude(doorDelta) > 1.0f)
                    entry->motionAttachments[0].flags &= ~0x40U;
            }
            if (!AreaDoorPathClear(*entry) || (entry->commandFlags & 0x40U) || AreaProbeAhead(*state, *entry)) {
                ReleaseDoor(static_cast<std::int32_t>(entry->stateContext.otherName), entry->name);
                if (entry->node)
                    mtwkSetTimeRelated(entry->motionWork, 1, 5, 0);
                if (entry->opcodeFlags & 2U) {
                    entry->areaPointNext = reinterpret_cast<NpcRouteLiveEntry *>(static_cast<std::uintptr_t>(state->npcListHeadOffset[0]));
                    entry->areaPointPrevious = 0;
                    SetListHead(*state, entry, 0);
                    if (entry->areaPointNext)
                        entry->areaPointNext->areaPointPrevious = entry;
                    entry->flags &= ~0x100U;
                    entry->routeMotionState = 4;
                    break;
                }
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 21;
                entry->flags &= ~0x100U;
                loop = 0;
                break;
            }
            if (entry->node) {
                if (mtwkCopyTransition(entry->motionWork, 0, 0, 3, 0)) {
                    QueueNpcAreaDoorMotion(entry);
                    loop = 0;
                    break;
                }
                if (!mtwkSetExtraFlag(entry->motionWork, 58, 1)) {
                    entry->routeMotionState = 2;
                    BeginDoorAnimation(static_cast<std::int32_t>(entry->stateContext.otherName), entry->name, entry->stateContext.doorMotion, 14.0f,
                        entry->motionSpeed * 1.1f);
                    mtwkSetSpeed(entry->motionWork, entry->motionSpeed * 1.1f, LowerBody);
                    mtwkUpdateWithPosition(entry->motionWork, 0, 0);
                }
                loop = 0;
                break;
            }
            doorStep.x = entry->stateContext.position.x - entry->worldPosition.x;
            doorStep.y = 0.0f;
            doorStep.z = entry->stateContext.position.z - entry->worldPosition.z;
            entry->rotation.y = entry->stateContext.targetYaw;
            const float distance = Vector3::CalcMagnitude(doorStep);
            if (std::fabs(distance) > 1.1920929e-07f) {
                entry->worldPosition.x += doorStep.x * 0.039999999f * entry->motionSpeed / distance;
                entry->worldPosition.z += doorStep.z * 0.039999999f * entry->motionSpeed / distance;
            }
            if (entry->motionSpeed * 0.039999999f > distance) {
                entry->worldPosition = entry->stateContext.doorPosition;
                ReleaseDoor(static_cast<std::int32_t>(entry->stateContext.otherName), entry->name);
                if (entry->opcodeFlags & 2U) {
                    entry->routeMotionState = 5;
                    entry->stateContext.otherName = 0;
                } else
                    DisableNpcAreaRoute(entry);
            }
            entry->localPosition = entry->worldPosition;
            entry->localPosition.x -= npcRouteOrigin.x;
            entry->localPosition.z -= npcRouteOrigin.z;
            loop = 0;
            break;
        }
        case 2:
            ReserveAreaEntrance(*state);
            entry->motionAttachments[0].flags |= 0x40U;
            if (entry->node) {
                if (mtwkSetRegionFlag(entry->motionWork, 65, 1, LowerBody)) {
                    Vector3 position = entry->worldPosition;
                    position.x -= npcRouteOrigin.x;
                    position.z -= npcRouteOrigin.z;
                    Character *const node = entry->node;
                    AssignCharacterControlFlags(node, ccmGetFlags(node) & ~CharacterControlPairImmovable);
                    entry->commandFlags &= ~1U;
                    if (!FindNpcIntersectionInScene(position, entry->packedMoveFlags))
                        BeginNpcRouteStreamOut(entry, 0);
                    ReleaseDoor(static_cast<std::int32_t>(entry->stateContext.otherName), entry->name);
                    entry->stateContext.otherName = 0;
                    if (entry->opcodeFlags & 2U) {
                        entry->routeMotionState = 5;
                        if (entry->commandFlags & 0x20U) {
                            StopNpcAreaRoute(entry);
                            entry->commandFlags &= ~0x20U;
                        }
                        break;
                    }
                    DisableNpcAreaRoute(entry);
                }
            } else {
                entry->worldPosition = entry->stateContext.doorPosition;
                ReleaseDoor(static_cast<std::int32_t>(entry->stateContext.otherName), entry->name);
                entry->stateContext.otherName = 0;
                if (entry->opcodeFlags & 2U)
                    entry->routeMotionState = 5;
                else
                    DisableNpcAreaRoute(entry);
            }
            entry->localPosition = entry->worldPosition;
            entry->localPosition.x -= npcRouteOrigin.x;
            entry->localPosition.z -= npcRouteOrigin.z;
            loop = 0;
            break;
        case 5:
            entry->commandFlags |= 2U;
            if ((entry->commandFlags & 0x40U) || AreaProbeAhead(*state, *entry)) {
                QueueAreaMotion(entry, 0);
                ReserveAreaEntrance(*state);
                loop = 0;
                break;
            }
            if (!AreaTargetReached(*state, *entry)) {
                ReserveAreaEntrance(*state);
                loop = 0;
                break;
            }
            entry->controlRowIndex = entry->nextControlRowIndex;
            if (entry->opcodeFlags & 1U) {
                if (controls[entry->controlRowIndex].forwardOffset)
                    entry->nextControlRowIndex =
                        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset))->index;
                else
                    entry->nextControlRowIndex = -1;
            } else {
                if (controls[entry->controlRowIndex].backwardOffset)
                    entry->nextControlRowIndex =
                        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].backwardOffset))->index;
                else
                    entry->nextControlRowIndex = -1;
            }
            entry->flags &= ~0x100U;
            entry->routeMotionState = 6;
            break;
        case 6:
            entry->commandFlags |= 2U;
            if (entry->nextControlRowIndex != -1)
                QueueAreaMotion(entry, &controls[entry->nextControlRowIndex].position);
            else {
                if (reinterpret_cast<NpcAreaControlRow *>(owners[entry->primarySlotIndex].linkedControlOffset) == &controls[entry->controlRowIndex]) {
                    entry->nextOwnerRowIndex = entry->primarySlotIndex;
                    entry->nextControlRowIndex = -1;
                    entry->controlRowIndex = -1;
                    const std::uint8_t action = owners[entry->nextOwnerRowIndex].actionKind;
                    if (owners[entry->nextOwnerRowIndex].flags & 4U)
                        QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position, &owners[entry->nextOwnerRowIndex + 1].position);
                    else if (action == 2 || action == 6 || action == 7 || action == 16 || action == 17 || action == 18)
                        QueueAreaOwnerApproach(*state, *entry);
                    else {
                        const std::uint16_t motion = ResolveNpcAreaOwnerAnimation(state, entry);
                        QueueAreaMotion(
                            entry, &owners[entry->nextOwnerRowIndex].position, 0, static_cast<std::int16_t>(owners[entry->nextOwnerRowIndex].yaw), motion);
                    }
                    entry->commandFlags &= ~4U;
                    entry->routeMotionState = 10;
                    break;
                }
                entry->opcodeFlags &= ~2U;
                if (controls[entry->controlRowIndex].flags & 1U) {
                    entry->routeMotionState = 21;
                    PrepareNpcAreaDoor(state, entry);
                    break;
                }
                entry->nextControlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].backwardOffset))->index;
                if (entry->commandFlags & 4U) {
                    entry->routeMotionState = 11;
                    break;
                }
                QueueAreaMotion(entry, &controls[entry->nextControlRowIndex].position);
                entry->routeMotionState = 7;
                break;
            }
            if ((entry->opcodeFlags & 1U) ?
                    reinterpret_cast<NpcAreaControlRow *>(owners[entry->primarySlotIndex].linkedControlOffset) == &controls[entry->controlRowIndex] :
                    reinterpret_cast<NpcAreaControlRow *>(owners[entry->primarySlotIndex].linkedControlOffset) == &controls[entry->nextControlRowIndex]) {
                if (entry->node)
                    mtwkSetTimeRelated(entry->motionWork, 0, 6, 0);
                entry->commandFlags &= ~4U;
                entry->routeMotionState = 8;
                entry->nextOwnerRowIndex = entry->primarySlotIndex;
                entry->nextControlRowIndex = -1;
            } else if (entry->commandFlags & 4U)
                entry->routeMotionState = 11;
            else
                entry->routeMotionState = 7;
            break;
        case 7:
            if (entry->commandFlags & 0x40U) {
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 13;
                loop = 0;
                break;
            }
            if (AreaOpposingLaneOccupied(*state, *entry)) {
                entry->commandFlags |= 4U;
                DisplaceAreaLane(*state, *entry, false, true);
                entry->routeMotionState = 11;
                break;
            }
            if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                ReserveAreaLane(*state, *entry);
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 13;
                loop = 0;
                break;
            }
            if (!ReserveAreaLane(*state, *entry) || AreaProbeAhead(*state, *entry)) {
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 13;
                loop = 0;
                break;
            }
            if (!AreaTargetReached(*state, *entry)) {
                loop = 0;
                break;
            }
            if (entry->opcodeFlags & 2U) {
                entry->controlRowIndex = entry->nextControlRowIndex;
                if (entry->opcodeFlags & 1U) {
                    if (controls[entry->controlRowIndex].forwardOffset)
                        entry->nextControlRowIndex =
                            reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset))->index;
                    else
                        entry->nextControlRowIndex = -1;
                } else {
                    if (controls[entry->controlRowIndex].backwardOffset)
                        entry->nextControlRowIndex =
                            reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].backwardOffset))->index;
                    else
                        entry->nextControlRowIndex = -1;
                }
                entry->routeMotionState = 6;
                break;
            }
            if (controls[entry->nextControlRowIndex].flags & 1U) {
                entry->controlRowIndex = entry->nextControlRowIndex;
                entry->nextControlRowIndex = -1;
                entry->routeMotionState = 21;
                PrepareNpcAreaDoor(state, entry);
                break;
            }
            entry->controlRowIndex = entry->nextControlRowIndex;
            if (entry->opcodeFlags & 1U)
                entry->nextControlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset))->index;
            else
                entry->nextControlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].backwardOffset))->index;
            entry->commandFlags |= 2U;
            QueueAreaMotion(entry, &controls[entry->nextControlRowIndex].position);
            break;
        case 11:
            if (entry->commandFlags & 0x40U) {
                entry->commandFlags &= ~2U;
                DisplaceAreaLane(*state, *entry, true, false);
                entry->routeMotionState = 13;
                loop = 0;
                break;
            }
            if (entry->node) {
                std::int8_t mode;
                if (entry->opcodeFlags & 1U) {
                    mode = controls[entry->controlRowIndex].laneMode;
                    if (HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].forwardOffset)->laneMode < mode)
                        mode = HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].forwardOffset)->laneMode;
                } else {
                    mode = controls[entry->controlRowIndex].laneMode;
                    if (HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].backwardOffset)->laneMode < mode)
                        mode = HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].backwardOffset)->laneMode;
                }
                if (!mode)
                    SetCharacterCollisionCapsuleMargin(entry->node, 0.2f);
            }
            if (!AreaOpposingLaneOccupied(*state, *entry)) {
                entry->commandFlags &= ~4U;
                QueueAreaMotion(entry, &controls[entry->nextControlRowIndex].position);
                entry->routeMotionState = 7;
                break;
            }
            if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                ReserveAreaLane(*state, *entry);
                entry->commandFlags &= ~2U;
                DisplaceAreaLane(*state, *entry, true, false);
                entry->routeMotionState = 13;
                loop = 0;
                break;
            }
            if (!ReserveAreaLane(*state, *entry) || AreaProbeAhead(*state, *entry)) {
                entry->commandFlags &= ~2U;
                DisplaceAreaLane(*state, *entry, true, false);
                entry->routeMotionState = 13;
                loop = 0;
                break;
            }
            DisplaceAreaLane(*state, *entry, true, true);
            if (!AreaTargetReached(*state, *entry)) {
                loop = 0;
                break;
            }
            if (entry->opcodeFlags & 2U) {
                entry->controlRowIndex = entry->nextControlRowIndex;
                if (entry->opcodeFlags & 1U) {
                    if (controls[entry->controlRowIndex].forwardOffset)
                        entry->nextControlRowIndex =
                            reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset))->index;
                    else
                        entry->nextControlRowIndex = -1;
                } else {
                    if (controls[entry->controlRowIndex].backwardOffset)
                        entry->nextControlRowIndex =
                            reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].backwardOffset))->index;
                    else
                        entry->nextControlRowIndex = -1;
                }
                entry->routeMotionState = 6;
                break;
            }
            if (controls[entry->nextControlRowIndex].flags & 1U) {
                entry->controlRowIndex = entry->nextControlRowIndex;
                entry->nextControlRowIndex = -1;
                entry->commandFlags &= ~4U;
                if (entry->node)
                    mtwkSetTimeRelated(entry->motionWork, 0, 6, 0);
                entry->routeMotionState = 21;
                PrepareNpcAreaDoor(state, entry);
                break;
            }
            entry->controlRowIndex = entry->nextControlRowIndex;
            if (entry->opcodeFlags & 1U)
                entry->nextControlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset))->index;
            else
                entry->nextControlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].backwardOffset))->index;
            entry->commandFlags |= 2U;
            break;
        case 13:
            if (entry->commandFlags & 0x40U) {
                if (AreaOpposingLaneOccupied(*state, *entry)) {
                    entry->commandFlags |= 4U;
                    DisplaceAreaLane(*state, *entry, false, false);
                } else
                    entry->commandFlags &= ~4U;
                ReserveAreaLane(*state, *entry);
                loop = 0;
                break;
            }
            if (AreaOpposingLaneOccupied(*state, *entry)) {
                DisplaceAreaLane(*state, *entry, false, false);
                entry->commandFlags |= 4U;
                if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                    ReserveAreaLane(*state, *entry);
                    loop = 0;
                    break;
                }
            } else {
                entry->commandFlags &= ~4U;
                if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                    const std::int8_t next = entry->nextControlRowIndex;
                    entry->nextControlRowIndex = entry->controlRowIndex;
                    entry->controlRowIndex = next;
                    if (entry->opcodeFlags & 1U)
                        entry->opcodeFlags &= ~1U;
                    else
                        entry->opcodeFlags |= 1U;
                    entry->opcodeFlags &= ~2U;
                    entry->commandFlags &= ~0x20U;
                    owners[entry->primarySlotIndex].runtimeMask &= ~1U;
                    --state->activeRowCount;
                    ++state->freeRowCount;
                    entry->primarySlotIndex = -1;
                }
            }
            if (!ReserveAreaLane(*state, *entry) || AreaProbeAhead(*state, *entry)) {
                loop = 0;
                break;
            }
            entry->commandFlags |= 2U;
            if (entry->commandFlags & 4U) {
                entry->routeMotionState = 11;
                loop = 0;
                break;
            }
            QueueAreaMotion(entry, &controls[entry->nextControlRowIndex].position);
            entry->routeMotionState = 7;
            loop = 0;
            break;
        case 8:
            if (entry->commandFlags & 0x40U) {
                ReserveAreaLane(*state, *entry);
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 14;
                loop = 0;
                break;
            }
            if (AreaOpposingLaneOccupied(*state, *entry)) {
                entry->commandFlags |= 4U;
                DisplaceAreaLane(*state, *entry, false, true);
                entry->routeMotionState = 12;
                break;
            }
            if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                ReserveAreaLane(*state, *entry);
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 14;
                loop = 0;
                break;
            }
            if (!ReserveAreaLane(*state, *entry) || AreaProbeAhead(*state, *entry)) {
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 14;
                loop = 0;
                break;
            }
            if (!AreaTargetReached(*state, *entry)) {
                ReserveAreaLane(*state, *entry);
                loop = 0;
                break;
            }
            entry->controlRowIndex = -1;
            entry->nextControlRowIndex = -1;
            entry->commandFlags |= 2U;
            {
                const std::uint8_t action = owners[entry->nextOwnerRowIndex].actionKind;
                if (owners[entry->nextOwnerRowIndex].flags & 4U)
                    QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position, &owners[entry->nextOwnerRowIndex + 1].position);
                else if (action == 2 || action == 6 || action == 7 || action == 16 || action == 17 || action == 18)
                    QueueAreaOwnerApproach(*state, *entry);
                else {
                    const std::uint16_t motion = ResolveNpcAreaOwnerAnimation(state, entry);
                    QueueAreaMotion(
                        entry, &owners[entry->nextOwnerRowIndex].position, 0, static_cast<std::int16_t>(owners[entry->nextOwnerRowIndex].yaw), motion);
                }
            }
            entry->routeMotionState = 10;
            break;
        case 12:
            if (entry->commandFlags & 0x40U) {
                ReserveAreaLane(*state, *entry);
                entry->commandFlags &= ~2U;
                DisplaceAreaLane(*state, *entry, true, false);
                entry->routeMotionState = 14;
                loop = 0;
                break;
            }
            if (entry->node) {
                std::int8_t mode;
                if (entry->opcodeFlags & 1U) {
                    mode = controls[entry->controlRowIndex].laneMode;
                    if (HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].forwardOffset)->laneMode < mode)
                        mode = HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].forwardOffset)->laneMode;
                } else {
                    mode = controls[entry->controlRowIndex].laneMode;
                    if (HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].backwardOffset)->laneMode < mode)
                        mode = HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].backwardOffset)->laneMode;
                }
                if (!mode)
                    SetCharacterCollisionCapsuleMargin(entry->node, 0.2f);
            }
            if (!AreaOpposingLaneOccupied(*state, *entry)) {
                entry->commandFlags &= ~4U;
                if (entry->opcodeFlags & 1U)
                    QueueAreaMotion(entry, &HeapPointer<NpcAreaControlRow>(controls[entry->controlRowIndex].forwardOffset)->position);
                else
                    QueueAreaMotion(entry, &controls[entry->controlRowIndex].position);
                entry->routeMotionState = 8;
                break;
            }
            if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                ReserveAreaLane(*state, *entry);
                entry->commandFlags &= ~2U;
                DisplaceAreaLane(*state, *entry, true, false);
                entry->routeMotionState = 14;
                loop = 0;
                break;
            }
            if (!ReserveAreaLane(*state, *entry) || AreaProbeAhead(*state, *entry)) {
                entry->commandFlags &= ~2U;
                DisplaceAreaLane(*state, *entry, true, false);
                entry->routeMotionState = 14;
                loop = 0;
                break;
            }
            DisplaceAreaLane(*state, *entry, true, true);
            if (AreaTargetReached(*state, *entry)) {
                entry->commandFlags &= ~2U;
                DisplaceAreaLane(*state, *entry, true, false);
                entry->routeMotionState = 14;
                loop = 0;
                break;
            }
            loop = 0;
            ReserveAreaLane(*state, *entry);
            break;
        case 14:
            if (entry->commandFlags & 0x40U) {
                if (AreaOpposingLaneOccupied(*state, *entry)) {
                    entry->commandFlags |= 4U;
                    DisplaceAreaLane(*state, *entry, false, false);
                } else
                    entry->commandFlags &= ~4U;
                ReserveAreaLane(*state, *entry);
                loop = 0;
                break;
            }
            if (AreaOpposingLaneOccupied(*state, *entry)) {
                DisplaceAreaLane(*state, *entry, false, false);
                entry->commandFlags |= 4U;
                if (((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) || AreaTargetReached(*state, *entry)) {
                    ReserveAreaLane(*state, *entry);
                    loop = 0;
                    break;
                }
            } else {
                entry->commandFlags &= ~4U;
                if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                    if (entry->opcodeFlags & 1U) {
                        entry->nextControlRowIndex = entry->controlRowIndex;
                        entry->controlRowIndex =
                            reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->nextControlRowIndex].forwardOffset))->index;
                        entry->opcodeFlags &= ~1U;
                    } else {
                        entry->nextControlRowIndex = entry->controlRowIndex;
                        entry->controlRowIndex =
                            reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->nextControlRowIndex].backwardOffset))->index;
                        entry->opcodeFlags |= 1U;
                    }
                    entry->ownerRowIndex = -1;
                    entry->nextOwnerRowIndex = -1;
                    entry->opcodeFlags &= ~2U;
                    entry->commandFlags &= ~0x20U;
                    owners[entry->primarySlotIndex].runtimeMask &= ~1U;
                    --state->activeRowCount;
                    ++state->freeRowCount;
                    entry->primarySlotIndex = -1;
                    entry->routeMotionState = 13;
                    break;
                }
                if (AreaTargetReached(*state, *entry)) {
                    entry->controlRowIndex = -1;
                    entry->nextControlRowIndex = -1;
                    entry->commandFlags |= 2U;
                    const std::uint8_t action = owners[entry->nextOwnerRowIndex].actionKind;
                    if (owners[entry->nextOwnerRowIndex].flags & 4U)
                        QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position, &owners[entry->nextOwnerRowIndex + 1].position);
                    else if (action == 2 || action == 6 || action == 7 || action == 16 || action == 17 || action == 18)
                        QueueAreaOwnerApproach(*state, *entry);
                    else {
                        const std::uint16_t motion = ResolveNpcAreaOwnerAnimation(state, entry);
                        QueueAreaMotion(
                            entry, &owners[entry->nextOwnerRowIndex].position, 0, static_cast<std::int16_t>(owners[entry->nextOwnerRowIndex].yaw), motion);
                    }
                    entry->routeMotionState = 10;
                    loop = 0;
                    break;
                }
            }
            if (!ReserveAreaLane(*state, *entry) || AreaProbeAhead(*state, *entry)) {
                loop = 0;
                break;
            }
            entry->commandFlags |= 2U;
            if (entry->commandFlags & 4U) {
                entry->routeMotionState = 12;
                loop = 0;
                break;
            }
            QueueAreaMotion(entry,
                &((entry->opcodeFlags & 1U) ?
                        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset)) :
                        &controls[entry->controlRowIndex])
                    ->position);
            entry->routeMotionState = 8;
            loop = 0;
            break;
        case 10:
            if (entry->commandFlags & 0x40U) {
                QueueAreaMotion(entry, 0);
                entry->commandFlags &= ~8U;
                loop = 0;
                break;
            }
            if (AreaOwnerProbeAhead(*state, *entry)) {
                QueueAreaMotion(entry, 0);
                entry->commandFlags &= ~8U;
                loop = 0;
                break;
            }
            if ((entry->opcodeFlags & 2U) && (entry->commandFlags & 0x20U)) {
                entry->commandFlags &= ~0x20U;
                entry->opcodeFlags &= ~2U;
                entry->ownerRowIndex = entry->nextOwnerRowIndex;
                SelectAreaExitDirection(*state, *entry);
                if (owners[entry->ownerRowIndex].linkedControlOffset) {
                    entry->nextOwnerRowIndex = -1;
                    entry->nextControlRowIndex =
                        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(owners[entry->ownerRowIndex].linkedControlOffset))->index;
                    entry->commandFlags |= 2U;
                    AreaLinkedPosition(owners[entry->ownerRowIndex], target);
                    QueueAreaMotion(entry, &target);
                    entry->routeMotionState = 9;
                } else if (owners[entry->ownerRowIndex].flags & 8U) {
                    entry->nextOwnerRowIndex = entry->ownerRowIndex - 1;
                    entry->commandFlags |= 2U;
                    QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position);
                } else {
                    entry->commandFlags &= ~2U;
                    QueueAreaMotion(entry, 0);
                    entry->routeMotionState = 17;
                }
                loop = 0;
                break;
            }
            if (!AreaTargetReached(*state, *entry)) {
                loop = 0;
                break;
            }
            if (entry->opcodeFlags & 2U) {
                entry->ownerRowIndex = entry->nextOwnerRowIndex;
                if (owners[entry->ownerRowIndex].flags & 4U) {
                    entry->nextOwnerRowIndex = entry->ownerRowIndex + 1;
                    entry->commandFlags |= 2U;
                    const std::uint8_t action = owners[entry->nextOwnerRowIndex].actionKind;
                    if (owners[entry->nextOwnerRowIndex].flags & 4U)
                        QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position, &owners[entry->nextOwnerRowIndex + 1].position);
                    else if (action == 2 || action == 6 || action == 7 || action == 16 || action == 17 || action == 18)
                        QueueAreaOwnerApproach(*state, *entry);
                    else {
                        const std::uint16_t motion = ResolveNpcAreaOwnerAnimation(state, entry);
                        QueueAreaMotion(
                            entry, &owners[entry->nextOwnerRowIndex].position, 0, static_cast<std::int16_t>(owners[entry->nextOwnerRowIndex].yaw), motion);
                    }
                } else {
                    entry->nextOwnerRowIndex = -1;
                    entry->retained2d0 = 0;
                    ++state->occupiedRowCount;
                    entry->routeMotionState = 16;
                }
                break;
            }
            entry->ownerRowIndex = entry->nextOwnerRowIndex;
            if (owners[entry->ownerRowIndex].linkedControlOffset) {
                entry->nextOwnerRowIndex = -1;
                entry->nextControlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(owners[entry->ownerRowIndex].linkedControlOffset))->index;
                entry->commandFlags |= 2U;
                AreaLinkedPosition(owners[entry->ownerRowIndex], target);
                QueueAreaMotion(entry, &target);
                entry->routeMotionState = 9;
            } else if (owners[entry->ownerRowIndex].flags & 8U) {
                entry->nextOwnerRowIndex = entry->ownerRowIndex - 1;
                entry->commandFlags |= 2U;
                QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position);
            } else {
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 17;
            }
            break;
        case 9:
            if (!AreaOwnerLaneAvailable(*state, *entry) || (entry->commandFlags & 0x40U) || AreaOwnerProbeAhead(*state, *entry)) {
                entry->commandFlags &= ~2U;
                QueueAreaMotion(entry, 0);
                entry->routeMotionState = 15;
                loop = 0;
                break;
            }
            if (!AreaTargetReached(*state, *entry)) {
                loop = 0;
                break;
            }
            owners[entry->ownerRowIndex].runtimeMask &= ~1U;
            --state->activeRowCount;
            ++state->freeRowCount;
            entry->primarySlotIndex = -1;
            entry->controlRowIndex =
                reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(owners[entry->ownerRowIndex].linkedControlOffset))->index;
            if (entry->opcodeFlags & 1U) {
                if (controls[entry->controlRowIndex].forwardOffset)
                    entry->nextControlRowIndex =
                        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset))->index;
                else
                    entry->nextControlRowIndex = -1;
            } else if (controls[entry->controlRowIndex].forwardOffset) {
                entry->nextControlRowIndex = entry->controlRowIndex;
                entry->controlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].forwardOffset))->index;
            } else if (controls[entry->controlRowIndex].flags & 1U)
                entry->nextControlRowIndex = -1;
            else
                entry->nextControlRowIndex =
                    reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(controls[entry->controlRowIndex].backwardOffset))->index;
            entry->ownerRowIndex = entry->nextOwnerRowIndex = -1;
            if (entry->nextControlRowIndex != -1) {
                entry->commandFlags |= 2U;
                QueueAreaMotion(entry, &controls[entry->nextControlRowIndex].position);
                entry->routeMotionState = 7;
            } else if (entry->opcodeFlags & 2U)
                entry->routeMotionState = 6;
            else {
                entry->routeMotionState = 21;
                PrepareNpcAreaDoor(state, entry);
            }
            break;
        case 15:
            if (AreaOwnerLaneAvailable(*state, *entry) && !(entry->commandFlags & 0x40U) && !AreaOwnerProbeAhead(*state, *entry)) {
                entry->commandFlags |= 2U;
                AreaLinkedPosition(owners[entry->ownerRowIndex], target);
                QueueAreaMotion(entry, &target);
                entry->routeMotionState = 9;
            }
            loop = 0;
            break;
        case 16: {
            entry->commandFlags &= ~2U;
            entry->commandFlags &= ~8U;
            entry->commandFlags &= ~0x100U;
            owners[entry->ownerRowIndex].runtimeMask |= 2U;
            ++entry->retained2d0;
            entry->motionAttachments[1].flags |= 0x40U;
            if (static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) == -1) {
                entry->retained2d0 = 0;
                entry->commandFlags |= 0x80U;
            } else if (entry->waitDuration > 0)
                entry->retained2d0 = 1000;
            else if (static_cast<std::int32_t>(entry->retained2d0) > 1000 && !(entry->commandFlags & 0x40U)) {
                if (entry->node) {
                    if (!entry->specialAnimationContext && mtwkSetRegionFlag(entry->motionWork, 65, 1, LowerBody)) {
                        loop = FinishAreaOwnerAnimation(*state, *entry);
                        break;
                    }
                } else {
                    entry->routeMotionState = 20;
                    owners[entry->ownerRowIndex].runtimeMask &= ~2U;
                    entry->currentMotionId = 0;
                    if (entry->waitParameter)
                        QueueNpcMotionAttachment(entry, entry->waitParameter, entry->opcodeByte | 0x20U, 1);
                    else if (static_cast<std::int8_t>(entry->opcodeByte) == -1)
                        ClearNpcMotionAttachment(entry, 1);
                    entry->motionAttachments[1].flags &= ~0x40U;
                    break;
                }
            } else if (static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) == 1) {
                entry->retained2d0 = 0;
                entry->commandFlags |= 0x80U;
            }
            if (entry->node && entry->currentAreaPoint == AreaEndpoint(*entry) && !(entry->commandFlags & 0x40U)) {
                delta.x = npcPlayerPosition.x - entry->localPosition.x;
                delta.y = 0.0f;
                delta.z = npcPlayerPosition.z - entry->localPosition.z;
                std::int32_t interaction = 0;
                distance = Vector3::CalcMagnitude(delta);
                if (distance > 2.5f && (entry->commandFlags & 0x200U)) {
                    entry->commandFlags &= ~0x200U;
                    interaction = 2;
                    if (entry->retained2d0 && !entry->waitDuration) {
                        loop = FinishAreaOwnerAnimation(*state, *entry);
                        break;
                    }
                } else if (1.5f > distance && !(entry->commandFlags & 0x200U) && !(owners[entry->ownerRowIndex].flags & 0x40U)) {
                    entry->commandFlags |= 0x200U;
                    entry->areaLookFrames = 0;
                    interaction = 1;
                } else if (!(entry->commandFlags & 0x200U)) {
                    if (HasOccupiedPartner(state, entry)) {
                        if (!(entry->commandFlags & 0x400U))
                            interaction = 1;
                    } else if (entry->commandFlags & 0x400U) {
                        interaction = 2;
                        if (entry->retained2d0 && !entry->waitDuration) {
                            loop = FinishAreaOwnerAnimation(*state, *entry);
                            break;
                        }
                    }
                }
                if (interaction) {
                    if (interaction == 1) {
                        const std::uint32_t flags = GetNpcAreaSpecialMotionFlags(*entry);
                        const std::uint32_t transition = SelectNpcAreaTransitionMotion(entry);
                        entry->areaTransitionFrames = static_cast<std::int16_t>(transition >> 16U);
                        if ((flags & 1U) == 0 || static_cast<std::uint16_t>(transition)) {
                            entry->specialAnimationContext = 0;
                            ApplyAreaOwnerAnimation(*state, *entry, ResolveNpcAreaOwnerAnimation(state, entry));
                        }
                        goto applied;
                    }
                    entry->specialAnimationContext = 0;
                    ApplyAreaOwnerAnimation(*state, *entry, ResolveNpcAreaOwnerAnimation(state, entry));
                    goto applied;
                }
                if (entry->commandFlags & 0x200U) {
                    if (++entry->areaLookFrames < 600) {
                        Vector3 head;
                        mtwkGetBonePosition(charFindMotionwork(ctrlGetRootNode()), 5, &head);
                        mtwkUpdateWithPosition(entry->motionWork, &head, 0);
                    } else
                        mtwkUpdateWithPosition(entry->motionWork, 0, 0);
                } else
                    mtwkUpdateWithPosition(entry->motionWork, 0, 0);
            }
            if (entry->node) {
                if (!mtwkSetRegionFlag(entry->motionWork, 65, 1, LowerBody) || (entry->commandFlags & 0x40U)) {
                    loop = 0;
                    break;
                }
                std::uint16_t motion;
                if (entry->specialAnimationContext) {
                    motion = ResolveNpcAreaOwnerAnimation(state, entry);
                    if (!entry->specialAnimationContext) {
                        if (entry->retained2d0 && !entry->waitDuration) {
                            loop = FinishAreaOwnerAnimation(*state, *entry);
                            break;
                        }
                        motion = ResolveNpcAreaOwnerAnimation(state, entry);
                    }
                } else
                    motion = ResolveNpcAreaOwnerAnimation(state, entry);
                Vector3 position;
                GetCharacterWorldPosition(entry->node, &position);
                Vector3 offset = owners[entry->ownerRowIndex].position;
                offset.x -= npcRouteOrigin.x;
                offset.z -= npcRouteOrigin.z;
                offset.x -= position.x;
                offset.y = 0.0f;
                offset.z -= position.z;
                if (motion != mtwkGetAnimationId(entry->motionWork, LowerBody) || !mtwkIsMotionNonLooping(entry->motionWork, 0) ||
                    Vector3::CalcMagnitude(offset) > 0.2f)
                    ApplyAreaOwnerAnimation(*state, *entry, motion);
            }
        applied:
            entry->motionAttachments[1].flags |= 0x40U;
            entry->flags &= ~0x100U;
            loop = 0;
            break;
        }
        case 18: {
            if (npcMotionClipFrames[static_cast<std::int8_t>(entry->retained324)]) {
                const std::int32_t frame = entry->animationFrame + 1;
                entry->animationFrame = frame;
                entry->animationFrame = static_cast<std::int16_t>(frame) == npcMotionClipFrameCounts[static_cast<std::int8_t>(entry->retained324)] ? 0 : frame;
            }
            Character *const node = entry->node;
            if (!node || !npcMotionClipFrames[static_cast<std::int8_t>(entry->retained324)]) {
                loop = 0;
                break;
            }
            Vector3 position = entry->localPosition;
            EV_SetCharDisplay(node, CVM_MotionworkDisplayCheck, camCtrlIsInsideDepthScaledRect(position, GetCharacterDisplayRadius(node)));
            Matrix4x4::TransformPoint(position, position);
            float *view = &entry->node->viewPosition.x;
            const float *source = &position.x;
            *view++ = *source++;
            *view++ = *source++;
            *view = *source;
            distance = Vector3::CalcMagnitude(position);
            if (0.0000099999997f > distance)
                entry->node->drawDistance = 0.0f;
            else
                entry->node->drawDistance = distance;
            mtwkSetDrawOverrideData(entry->motionWork,
                npcMotionClipFrames[static_cast<std::int8_t>(entry->retained324)] +
                    entry->animationFrame * npcMotionClipBoneCounts[static_cast<std::int8_t>(entry->retained324)]);
            loop = 0;
            break;
        }
        case 19:
            entry->commandFlags &= ~2U;
            if (entry->node) {
                if (mtwkSetRegionFlag(entry->motionWork, 65, 1, LowerBody)) {
                    entry->commandFlags &= ~8U;
                    goto crowdReleaseArea;
                }
                loop = 0;
                break;
            }
            entry->commandFlags &= ~8U;
        crowdReleaseArea:
            entry->flags &= ~0x100U;
            entry->routeMotionState = 20;
            break;
        case 20:
            entry->opcodeFlags &= ~2U;
            if (SelectAreaExitDirection(*state, *entry)) {
                entry->commandFlags |= 2U;
                if (owners[entry->ownerRowIndex].linkedControlOffset) {
                    entry->nextControlRowIndex =
                        reinterpret_cast<NpcAreaControlRow *>(static_cast<std::uintptr_t>(owners[entry->ownerRowIndex].linkedControlOffset))->index;
                    AreaLinkedPosition(owners[entry->ownerRowIndex], target);
                    if (!(entry->commandFlags & 0x40U))
                        QueueAreaMotion(entry, &target);
                    entry->routeMotionState = 9;
                } else if (owners[entry->ownerRowIndex].flags & 8U) {
                    entry->nextOwnerRowIndex = entry->ownerRowIndex - 1;
                    if (!(entry->commandFlags & 0x40U))
                        QueueAreaMotion(entry, &owners[entry->nextOwnerRowIndex].position);
                    entry->routeMotionState = 10;
                }
                entry->retained2d0 = 0;
                --state->occupiedRowCount;
            } else {
                entry->specialAnimationContext = 0;
                entry->routeMotionState = 16;
                entry->retained2d0 = 0;
            }
            loop = 0;
            break;
        case 21:
            if (!ClaimAreaEntrance(*state, *entry) || (entry->commandFlags & 0x40U)) {
                entry->commandFlags &= ~2U;
                ReserveAreaLane(*state, *entry);
                QueueAreaMotion(entry, 0);
                loop = 0;
                break;
            }
            entry->flags |= 0x100U;
            if (entry->stateContext.otherName) {
                entry->commandFlags &= ~2U;
                entry->routeMotionState = 0;
                PublishAreaWorldPosition(*entry);
            } else {
                entry->commandFlags |= 2U;
                if (QueueAreaMotion(entry, GetNpcAreaPointPosition(entry->currentAreaPoint)))
                    mtwkCopyTransition(entry->motionWork, 0, 0, 2, 0);
                entry->routeMotionState = 22;
            }
            break;
        case 22:
            entry->motionAttachments[0].flags &= ~0x40U;
            if (entry->commandFlags & 0x40U) {
                ReserveAreaEntrance(*state);
                QueueAreaMotion(entry, 0);
                loop = 0;
                break;
            }
            if (entry->node && !mtwkSetExtraFlag(entry->motionWork, 58, 1))
                DisableNpcAreaRoute(entry);
            else {
                point = *GetNpcAreaPointPosition(entry->currentAreaPoint);
                point.x -= npcRouteOrigin.x;
                point.z -= npcRouteOrigin.z;
                delta.x = point.x - entry->localPosition.x;
                delta.y = 0.0f;
                delta.z = point.z - entry->localPosition.z;
                if (1.0f > Vector3::CalcMagnitude(delta))
                    DisableNpcAreaRoute(entry);
            }
            ReserveAreaEntrance(*state);
            loop = 0;
            break;
        default:
            loop = 0;
            break;
        }
    } while (loop);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t TickNpcLocalAreaState(NpcAreaRouteState *state, NpcRouteLiveEntry *entry) {
    std::int32_t loop;
    std::int32_t capacity = npcAreaQueueCapacities[g_actNumber];
    NpcAreaOwnerRow *const owners = OwnerRows(*state);
    if (!state || !entry)
        return 0;
    if (entry->stateContext.otherName) {
        ReleaseDoor(entry->stateContext.otherName, entry->name);
        entry->stateContext.otherName = 0;
    }
    ++state->participantCount;
    if (state->flags & 0x20U)
        entry->motionAttachments[0].flags &= ~0x40U;
    else
        entry->motionAttachments[0].flags |= 0x40U;
    loop = 1;
    do {
        switch (entry->routeMotionState) {
        case 3:
            entry->controlRowIndex = -1;
            entry->ownerRowIndex = -1;
            entry->nextOwnerRowIndex = -1;
            PrepareNpcAreaDoor(state, entry);
            if (entry->primarySlotIndex == -1) {
                if (ScriptDword(entry->currentCommand) == 19 && SelectClipAreaOwner(*state, *entry)) {
                    entry->commandFlags &= ~2U;
                    entry->controlRowIndex = -1;
                    entry->ownerRowIndex = entry->primarySlotIndex;
                    entry->nextControlRowIndex = -1;
                    entry->nextOwnerRowIndex = -1;
                    ++state->occupiedRowCount;
                    entry->motionSpeed = 1.0f;
                    entry->retained324 = entry->currentCommand[4];
                    entry->currentCommand += 8;
                    entry->streamCost = 1;
                    entry->routeQueryState = 3;
                    entry->animationFrame = static_cast<std::int8_t>(entry->scriptRecord[8]) * 2;
                    entry->routeMotionState = 18;
                    entry->flags |= 0x100U;
                    entry->stateContext.otherName = 0;
                    entry->rotation.y = owners[entry->primarySlotIndex].yaw;
                    entry->worldPosition = owners[entry->primarySlotIndex].position;
                    entry->localPosition = entry->worldPosition;
                    entry->localPosition.x -= npcRouteOrigin.x;
                    entry->localPosition.z -= npcRouteOrigin.z;
                    break;
                }
                if (entry->currentAreaPoint != AreaEndpoint(*entry)) {
                    NpcRouteLiveEntry *head = ListHead(*state, 0);
                    if (head) {
                        NpcRouteLiveEntry *tail = head;
                        std::int32_t count = 1;
                        while (tail->areaPointNext) {
                            tail = tail->areaPointNext;
                            ++count;
                        }
                        if ((state->flags & 1U) == 0) {
                            if (state->flags & 0x20U)
                                capacity = 0;
                            else
                                capacity = capacity != 0;
                        }
                        if (!entry->waitDuration && static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) != -1 &&
                            ScriptDword(entry->currentCommand) != 1 && count > capacity - 1) {
                            DisableNpcAreaRoute(entry);
                            loop = 0;
                            break;
                        }
                        tail->areaPointNext = entry;
                        entry->areaPointPrevious = tail;
                        entry->routeMotionState = 4;
                        break;
                    }
                    if (state->freeRowCount)
                        SelectFreeAreaOwner(*state, *entry);
                } else {
                    if (!SelectEndpointOwner(*state, *entry)) {
                        DisableNpcAreaRoute(entry);
                        loop = 0;
                        break;
                    }
                    NpcRouteLiveEntry *head = ListHead(*state, 0);
                    if (head) {
                        NpcRouteLiveEntry *tail = head;
                        while (tail->areaPointNext)
                            tail = tail->areaPointNext;
                        tail->areaPointNext = entry;
                        entry->areaPointPrevious = tail;
                        entry->routeMotionState = 4;
                        break;
                    }
                }
            }
            if (entry->primarySlotIndex == -1) {
                if (!entry->waitDuration && static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) != -1 && ScriptDword(entry->currentCommand) != 1 &&
                    entry->currentAreaPoint != AreaEndpoint(*entry)) {
                    DisableNpcAreaRoute(entry);
                    loop = 0;
                    break;
                }
                SetListHead(*state, entry, 0);
                entry->routeMotionState = 4;
                entry->nextControlRowIndex = 0;
                break;
            }
            entry->ownerRowIndex = entry->primarySlotIndex;
            while (owners[entry->ownerRowIndex].flags & 4U)
                ++entry->ownerRowIndex;
            ++state->occupiedRowCount;
            entry->specialAnimationContext = 0;
            entry->routeMotionState = 16;
            entry->retained2d0 = 0;
            entry->localPosition = entry->worldPosition = owners[entry->primarySlotIndex].position;
            entry->localPosition.x -= npcRouteOrigin.x;
            entry->localPosition.z -= npcRouteOrigin.z;
            entry->rotation.y = owners[entry->primarySlotIndex].yaw;
            break;
        case 4: {
            if ((entry->commandFlags & 0x40U) || (MaintainAreaQueue(*state, *entry), entry != ListHead(*state, 0)) || entry->areaPointPrevious) {
                loop = 0;
                break;
            }
            if (entry->primarySlotIndex == -1) {
                loop = 0;
                break;
            }
            SetListHead(*state, entry->areaPointNext, 0);
            if (entry->areaPointNext) {
                entry->areaPointNext->areaPointPrevious = 0;
                entry->areaPointNext = 0;
            }
            entry->ownerRowIndex = entry->primarySlotIndex;
            while (owners[entry->ownerRowIndex].flags & 4U)
                ++entry->ownerRowIndex;
            ++state->occupiedRowCount;
            entry->specialAnimationContext = 0;
            entry->routeMotionState = 16;
            Vector3 position = owners[entry->primarySlotIndex].position;
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            entry->localPosition = entry->worldPosition = position;
            entry->worldPosition.x += npcRouteOrigin.x;
            entry->worldPosition.z += npcRouteOrigin.z;
            entry->rotation.y = owners[entry->primarySlotIndex].yaw;
            break;
        }
        case 16:
            entry->commandFlags &= ~2U;
            entry->commandFlags &= ~8U;
            entry->commandFlags &= ~0x100U;
            owners[entry->ownerRowIndex].runtimeMask |= 2U;
            entry->motionAttachments[1].flags |= 0x40U;
            ++entry->retained2d0;
            if (static_cast<std::int32_t>(ScriptDword(entry->currentCommand)) == -1) {
                entry->retained2d0 = 0;
                entry->commandFlags |= 0x80U;
            } else if (entry->waitDuration)
                entry->retained2d0 = 1000;
            else {
                if (static_cast<std::int32_t>(entry->retained2d0) > 1000 && !(entry->commandFlags & 0x40U)) {
                    entry->routeMotionState = 20;
                    owners[entry->ownerRowIndex].runtimeMask &= ~2U;
                    entry->currentMotionId = 0;
                    break;
                }
                if (ScriptDword(entry->currentCommand) == 1) {
                    entry->retained2d0 = 0;
                    entry->commandFlags |= 0x80U;
                }
            }
            loop = 0;
            break;
        case 18:
            loop = 0;
            break;
        case 20:
            entry->controlRowIndex = 0;
            entry->ownerRowIndex = -1;
            entry->nextControlRowIndex = -1;
            entry->nextOwnerRowIndex = -1;
            entry->retained2d0 = 0;
            --state->occupiedRowCount;
            entry->opcodeFlags &= ~2U;
            if (entry->waitParameter)
                QueueNpcMotionAttachment(entry, entry->waitParameter, static_cast<std::int8_t>(entry->opcodeByte) | 0x20, 1);
            else if (static_cast<std::int8_t>(entry->opcodeByte) == -1)
                ClearNpcMotionAttachment(entry, 1);
            entry->motionAttachments[1].flags &= ~0x40U;
            entry->routeMotionState = 21;
            break;
        case 21: {
            owners[entry->primarySlotIndex].runtimeMask &= ~1U;
            --state->activeRowCount;
            ++state->freeRowCount;
            entry->primarySlotIndex = -1;
            Vector3 position = *GetNpcAreaPointPosition(entry->currentAreaPoint);
            entry->worldPosition = position;
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            entry->localPosition = position;
            DisableNpcAreaRoute(entry);
            loop = 0;
            break;
        }
        default:
            if (entry->opcodeFlags & 2U) {
                entry->controlRowIndex = -1;
                entry->ownerRowIndex = entry->primarySlotIndex;
                while (owners[entry->ownerRowIndex].flags & 4U)
                    ++entry->ownerRowIndex;
                entry->nextControlRowIndex = -1;
                entry->nextOwnerRowIndex = -1;
                ++state->occupiedRowCount;
                entry->specialAnimationContext = 0;
                entry->routeMotionState = 16;
            } else
                entry->routeMotionState = 21;
            break;
        }
    } while (loop);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint32_t AreaMask(std::uint32_t bits, std::int32_t shift) {
    if (shift >= 0)
        return bits << (static_cast<std::uint32_t>(shift) & 31U);
    const std::uint32_t amount = static_cast<std::uint32_t>(-shift) & 31U;
    return amount ? bits >> amount : 0;
}

} // namespace

#else

namespace {

std::uint32_t AreaMask(std::uint32_t bits, std::int32_t shift) {
    return bits << (static_cast<std::uint32_t>(shift) & 31U);
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
