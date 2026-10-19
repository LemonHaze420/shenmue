#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/mt7.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct MapInfoChunk;
struct Character;
struct MT7Node;
using DoorAreaCleanupCallback = void (*)(std::int32_t sceneId);
using DoorSubjectCallback = void (*)(std::uint64_t subject);


struct DoorTaskEntry {
    const shen_name *sourcePair;
    std::uint16_t flags;
    std::uint16_t animationId;
    std::uint32_t reserved0c;
    MT7Node *primary;
    MT7Node *secondary;
    shen_name claimant;
    MT7NodeSource motionNode;
    std::byte reserved05c[0x0c];
    std::uint32_t motionType;
    std::uint32_t reserved06c;
    const std::byte *motionDefinition;
    void *motionTarget;
    float motionFrame;
    float motionSpeed;
    float motionStartFrame;
    float motionEndFrame;
    std::uint8_t motionChannelState;
    std::byte reserved091[7];
    DoorSubjectCallback callback;
    Matrix4x4 worldTransform;
    shen_name destination;
    std::uint32_t reserved0e4;
    shen_name authorPackage;
    std::uint32_t reserved0ec;
    std::int32_t stream;
    std::byte reserved0f4[0x100];
    std::int32_t destinationEntry;
    float sortKey;
    Vector3 origin;
    float motionScale;
    float blockingRadius;
    DoorTaskEntry *previous;
    DoorTaskEntry *next;
};
static_assert(sizeof(DoorTaskEntry) == 0x220);
static_assert(offsetof(DoorTaskEntry, motionType) == 0x68);
static_assert(offsetof(DoorTaskEntry, motionDefinition) == 0x70);
static_assert(offsetof(DoorTaskEntry, motionTarget) == 0x78);
static_assert(offsetof(DoorTaskEntry, motionStartFrame) == 0x88);
static_assert(offsetof(DoorTaskEntry, motionEndFrame) == 0x8c);
static_assert(offsetof(DoorTaskEntry, motionChannelState) == 0x90);
static_assert(offsetof(DoorTaskEntry, callback) == 0x98);
static_assert(offsetof(DoorTaskEntry, worldTransform) == 0xa0);
static_assert(offsetof(DoorTaskEntry, destination) == 0xe0);
static_assert(offsetof(DoorTaskEntry, authorPackage) == 0xe8);
static_assert(offsetof(DoorTaskEntry, stream) == 0xf0);
static_assert(offsetof(DoorTaskEntry, destinationEntry) == 0x1f4);
static_assert(offsetof(DoorTaskEntry, previous) == 0x210);

inline constexpr shen_name kDoorTaskToken{SHEN_NAME(DOOR)};
inline constexpr shen_name kDoorPlayerName{SHEN_NAME(RYO_)};
inline constexpr shen_name kDoorQeAreaPrefix{make_shen_name('Q', 'E', '\0', '\0')};
inline constexpr shen_name kDoorQaAreaPrefix{make_shen_name('Q', 'A', '\0', '\0')};
inline constexpr shen_name kDoorQbAreaPrefix{make_shen_name('Q', 'B', '\0', '\0')};
inline constexpr shen_name kDoorQcAreaPrefix{make_shen_name('Q', 'C', '\0', '\0')};
inline constexpr shen_name kDoorQdAreaPrefix{make_shen_name('Q', 'D', '\0', '\0')};
inline constexpr shen_name kDoorNumberStorageToken{SHEN_NAME(drrn)};
inline constexpr shen_name kDoorCollisionNames[2]{
    SHEN_NAME(iida),
    SHEN_NAME(iidb),
};
inline constexpr shen_name kDoorQeStepSubject{SHEN_NAME(R146)};
inline constexpr shen_name kDoorSecondaryPrefix{make_shen_name('R', '0', '\0', '\0')};
inline constexpr std::int8_t kDoorRoomNumbers[77]{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 1, 2, 3, 4, 5,
    6, 7, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 10, 11,
    1, 2, 1, 2, 2, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 0,
    5, 6, 7, 8, 13, 14, 15, 0, 1, 1, 1, 2, 3, 4, 1, 2,
    3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
};


struct DoorFlagRule {
    std::int16_t flag;
    std::uint8_t answer;
    std::uint8_t dialogue;
};
static_assert(sizeof(DoorFlagRule) == 4);




struct DoorAreaMetadata {
    std::uint8_t door;
    std::uint8_t flags;
    std::int16_t opens;
    std::int16_t closes;
    std::uint8_t knock;
    std::uint8_t answer;
    const DoorFlagRule *rules;
    std::array<std::uint8_t, 4> dialogues;
    std::uint32_t reserved14;
};
static_assert(sizeof(DoorAreaMetadata) == 0x18);
static_assert(offsetof(DoorAreaMetadata, rules) == 0x08);
static_assert(offsetof(DoorAreaMetadata, dialogues) == 0x10);

struct DoorAreaMetadataMap {
    shen_name map;
    const DoorAreaMetadata *entries;
};

enum class DoorLeafState : std::uint8_t {
    StableA = 0,
    ToB = 1,
    StableB = 2,
    ToA = 3,
};




struct DoorRuntimeEntryState {
    shen_name primary{};
    shen_name secondary{};
    std::uint8_t family{};
    std::int32_t eventId{-1};
    std::uint16_t flags{};
    std::uint16_t activeMotionId{};
    Vector3 origin{};
    Vector3 interactionPoint{};
    float yawRadians{};
    float distanceSquared{};
    float motionFrame{};
    shen_name claimant{};
    float motionSpeed{};
    float blockingRadius{};
    bool primaryResolved{};
    bool secondaryResolved{};
    bool active{};
};


void InitializeDoorRuntime(const MapInfoChunk *doorChunk);
void ResetDoorRuntime();
// Setup caches scene media and installs the scene cleanup. The FTBL cleanup
// boundary only calls the current callback; queued-author media restoration is separate.
void SetDoorAreaCleanupCallback(DoorAreaCleanupCallback callback);
std::int64_t SetupDoorAreaCallbacks();
void ExecuteDoorAreaCleanup(std::int32_t sceneId);
DoorAreaCleanupCallback ExecuteCleanupFuncForAreaWithCachedScene();
std::int64_t authLoadCurrentQueued();
void TransformPropObject();
void doorQueueAuthorForMap(shen_name author);
void SetTemporaryDoorAuthor(shen_name author);
std::uint64_t SetDoorSubjectEnabled(shen_name subject, std::int32_t enabled);
std::int64_t SetQeDoorSceneState(std::int32_t mode);
std::int64_t SetDoorSubjectCallback(shen_name subject, DoorSubjectCallback callback);
// Zero returns from the admitted package: audio restores cached media, silent restores map state only.
std::int64_t doorLoadAuthorWithAudio(shen_name author);
std::int64_t doorLoadAuthorSilently(shen_name author);
std::int64_t doorTriggerTransition(shen_name subject, shen_name character);
shen_name GetCurrentDoorAuthor();
extern shen_name g_doorQueuedAuthor;
bool HasDoorAuthorEntries();
std::size_t GetDoorRuntimeEntryCount();
bool GetDoorRuntimeEntry(std::size_t index, DoorRuntimeEntryState &state);

const Matrix4x4 *doorFindEntryTransform(std::uint32_t doorId);

// GetDoorOpeningState: 0 closed, 1 open, 2 no schedule, 3 unrestricted.
std::int32_t GetDoorOpeningState(shen_name doorId);
// HasDoorEntry/IsDoorAvailableForNpcRoute: primary-record existence and NPC entrance availability.
bool HasDoorEntry(shen_name doorId);
bool IsDoorAvailableForNpcRoute(shen_name doorId, const Vector3 &position);



bool ClaimDoor(shen_name doorId, shen_name scenarioName);
bool ComputeDoorIngress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition);
// Egress has no distance rejection; a strictly positive local Z exits on the
// negative side and publishes the opposite yaw in the same shared rotation.
bool ComputeDoorEgress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition);
extern Vector3i g_doorIngressRotation;
std::int32_t GetSelectedNpcDoorId();
const Vector3 &GetDoorIngressOrigin();
std::uint32_t GetNpcDoorAnimation(std::int32_t doorId, Character *character, const Vector3 *position);
bool BeginDoorAnimation(std::int32_t doorId, shen_name scenarioName, std::int32_t animationId);
bool BeginDoorAnimation(std::int32_t doorId, shen_name scenarioName, std::int32_t animationId, float startFrame, float speedScale);


std::int64_t doorPlayRigidMotion(shen_name doorId, std::uint16_t motionId, float startFrame, float speed);
std::int64_t doorLoopRigidMotion(shen_name doorId, std::uint16_t motionId, float startFrame, float speed);
std::int64_t doorPlayRigidMotionRange(shen_name doorId, std::uint16_t motionId, float startFrame, float speed, float endFrame);
bool ReleaseDoor(std::int32_t doorId, shen_name scenarioName);

bool IsDoorInteractionActive();
shen_name doorGetSearchRegion();
std::int32_t drGetAnswerSide(shen_name *region);
shen_name ComputeRegionKeyForPos(const Vector3 *position);
shen_name drGetCurrentAuthorId();
std::int64_t drBeginEntry(shen_name searchRegion);
bool drIsDoorButtonAvailable(shen_name searchRegion);

void ResetDoorInteraction();

bool camCtrlSwitchToEventCam(std::int32_t frames, const Vector3 *target, std::int16_t yaw);
void camCtrlApplyNextAndTargetPos();

} // namespace shenmue::s2
