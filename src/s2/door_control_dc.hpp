#pragma once

#include "s2/model_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_node_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {
std::int32_t drGetAnswerSide(std::uint32_t *subject);

typedef void(__attribute__((renesas)) * DoorSubjectCallback)(std::uint32_t subject);

struct DoorDefinitionTable {
    std::uint32_t retainedHeader;
    std::int32_t count;
};
struct DoorTaskEntry {
    const shen_name *sourcePair;
    std::uint16_t flags;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint16_t animationId;
    MT7Node *primary, *secondary;
    shen_name claimant;
    MT7Node motionNode;
    TimedNodeMotion motion;
#else
    std::byte retained06[2];
    MT7Node *primary;
    std::byte retained0c[0x68];
#endif
    DoorSubjectCallback callback;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    Matrix4x4 worldTransform;
    shen_name routeCondition, requiredEntry;
    std::int32_t routeMode;
    MT7Node digits[4];
    std::int32_t routeSense;
    float distanceSquared;
    Vector3 interactionOrigin;
    float motionScale;
    DoorTaskEntry *previous, *next;
#else
    std::byte retained78[0x16c];
#endif
};
typedef char Layout_door_control_dc_0[(sizeof(DoorDefinitionTable) == 8) ? 1 : -1];
typedef char Layout_door_control_dc_1[(offsetof(DoorDefinitionTable, count) == 4) ? 1 : -1];
typedef char Layout_door_control_dc_2[(sizeof(DoorTaskEntry) == 0x1e4) ? 1 : -1];
typedef char Layout_door_control_dc_3[(offsetof(DoorTaskEntry, flags) == 4) ? 1 : -1];
typedef char Layout_door_control_dc_4[(offsetof(DoorTaskEntry, primary) == 8) ? 1 : -1];
typedef char Layout_door_control_dc_5[(offsetof(DoorTaskEntry, callback) == 0x74) ? 1 : -1];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct DoorAreaMetadata {
    std::uint8_t number, flags;
    std::int16_t opens, closes;
    std::uint8_t knock, answer;
    std::uint32_t flagRulesOffset;
};
typedef char Layout_door_control_dc_6[(sizeof(DoorAreaMetadata) == 0x0c) ? 1 : -1];
typedef char Layout_door_control_dc_7[(offsetof(DoorTaskEntry, secondary) == 0x0c) ? 1 : -1];
typedef char Layout_door_control_dc_8[(offsetof(DoorTaskEntry, claimant) == 0x10) ? 1 : -1];
typedef char Layout_door_control_dc_9[(offsetof(DoorTaskEntry, motionNode) == 0x14) ? 1 : -1];
typedef char Layout_door_control_dc_10[(offsetof(DoorTaskEntry, motion) == 0x54) ? 1 : -1];
typedef char Layout_door_control_dc_11[(offsetof(DoorTaskEntry, worldTransform) == 0x78) ? 1 : -1];
typedef char Layout_door_control_dc_12[(offsetof(DoorTaskEntry, distanceSquared) == 0x1c8) ? 1 : -1];
static const float kDoorPlayerClaimDistanceSquared = 2.5600001811981201f;
struct MapInfoChunk;
std::int32_t drInitEnter_Enqueue(const MapInfoChunk *chunk);
void DoorAdmission_Callback(void *);
void DoorTask_Callback(void *);
void TransformPropObject();
extern DoorTaskEntry *g_doorEntryHead;
struct DoorIngressState {
    shen_name selectedDoor;
    std::int32_t side, leafSide;
    Vector3 position;
    Vector3i rotation;
};
typedef char Layout_door_control_dc_13[(offsetof(DoorIngressState, position) == 0x0c) ? 1 : -1];
typedef char Layout_door_control_dc_14[(offsetof(DoorIngressState, rotation) == 0x18) ? 1 : -1];
typedef char Layout_door_control_dc_15[(sizeof(DoorIngressState) == 0x24) ? 1 : -1];
extern DoorIngressState g_doorIngress;
bool ComputeDoorIngress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition);
bool ComputeDoorEgress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition);
const Matrix4x4 *doorFindEntryTransform(shen_name subject);
extern shen_name g_doorEntrySubject;
extern std::int32_t g_doorEntryAction, g_doorEntryState;
extern std::int32_t g_doorCurrentStream, g_doorEntryMotion, g_doorEntryWait;
extern float g_doorCrossingZ;
std::int32_t SetDoorAuthorSecondaryNodes(shen_name author, std::int32_t visible);
const char *GetDoorAuthorName(shen_name name);
std::int32_t LoadDoorAuthor(shen_name author);
std::int32_t RestoreDoorAuthor(std::int32_t restoreAudio);
std::int32_t EnterDoorSubscene(shen_name author);
std::int32_t LeaveDoorSubscene(shen_name author);
void SetupDoorAreaCallbacks();
void DoorSceneCleanup(std::int32_t sceneId);
void authLoadCurrentQueued();
std::int32_t doorTriggerTransition(shen_name subject, shen_name claimant);
void UpdateDoorAuthorController();
void UpdateDoorDestinationController();
void drUpdateEntry();
void drBeginEntry(shen_name subject);
std::int32_t GetDoorInteractionAction(shen_name subject);
bool drIsDoorButtonAvailable(shen_name subject);
shen_name GetNearbyDoorForNpc(const Vector3 *position);
bool HasDoorEntry(shen_name door);
bool IsDoorAvailableForNpcRoute(shen_name door, const Vector3 *position);
std::int32_t GetNpcDoorAnimation(std::int32_t doorId, Character *character, const Vector3 *position);
std::int32_t GetSelectedNpcDoorId();
const Vector3 &GetDoorIngressOrigin();
const DoorAreaMetadata *GetDataForCurrentMapAndScene(shen_name subject);
std::uint32_t GetDoorAreaFlags(shen_name subject);
std::int32_t GetDoorOpeningState(shen_name subject);
std::int32_t IsDoorOpeningAllowed(shen_name subject);
std::uint32_t GetDoorExitSide(shen_name subject);
std::uint16_t GetDoorLeafAnimation(std::int32_t animationId);
std::int32_t BeginDoorAnimation(std::int32_t doorId, shen_name claimant, std::int32_t animationId);
std::int32_t BeginDoorAnimation(std::int32_t doorId, shen_name claimant, std::int32_t animationId, float frame, float speed);
std::int32_t ClaimDoor(shen_name subject, shen_name claimant);
std::int32_t ReleaseDoor(shen_name subject, shen_name claimant);
std::int32_t doorPlayRigidMotionRange(shen_name subject, std::int32_t motionId, float startFrame, float speed, float endFrame);
std::int32_t doorPlayRigidMotion(shen_name subject, std::int32_t motionId, float startFrame, float speed);
std::int32_t doorLoopRigidMotion(shen_name subject, std::int32_t motionId, float startFrame, float speed);
void EmitDoorMotionSound(DoorTaskEntry *entry);
std::int32_t IsDoorTransitionActive();
std::int32_t AdvanceDoorMotion(DoorTaskEntry *entry);
#endif
extern DoorDefinitionTable *g_doorDefinitionTable;
extern DoorTaskEntry *g_doorTaskEntries;
DoorTaskEntry *FindDoorEntry(shen_name subject);
void SetDoorNodeHierarchyHidden(MT7Node *node, std::uint32_t hidden);
std::uint32_t SetDoorSubjectEnabled(shen_name subject, std::int32_t enabled);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t SetDoorSubjectCallback(shen_name subject, DoorSubjectCallback callback);
DoorSubjectCallback GetDoorSubjectCallback(shen_name subject);
void ApplyDoorSceneStateToSiblings(MT7Node *node, std::int32_t sceneId);
void RefreshDoorSceneStateForCurrentStep();
std::int32_t ApplyDoorSceneState(std::int32_t sceneId);
std::uint32_t SetQeDoorSceneState(std::int32_t mode);
#endif

} // namespace s2
} // namespace shenmue

#endif
