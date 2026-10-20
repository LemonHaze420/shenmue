#pragma once
#include "s1/navigation.hpp"
#include <cstdint>
#include <cstddef>
#include <array>
#include <functional>

#include "common/math.hpp"

namespace shenmue::s1 {
struct Character;
struct CharacterIdTransform;
struct ScenarioContext;
enum MapEventKind : std::uint32_t {
    MEK_OnEnter = 0,
    MEK_OnAction = 1,
    MEK_OnEnterOnce = 2,
    MEK_OnActionOnce = 3,
    MEK_WhileInside = 4,
};

enum IntersectionTestType {
    IsPointInLineIntersection = 0x0,
    CalculateLineIntersection_0 = 0x1,
    CheckPointInLineIntersection = 0x2,
    SimpleRadial_CircleXZ_CalculateDistanceWithinRange = 0x3,
    DoQuadrilateralsIntersect = 0x4,
    OrientedBox_OrientedRectXZ_IsPointInTriangle = 0x5,
    IsPointLineIntersectOrQuadrilateralIntersection = 0x6,
    IsPointInTriangle_2 = 0x7,
    IsPointInTriangleWithinDistance = 0x8,
};

struct QueryShape {
    IntersectionTestType mode = IsPointInLineIntersection;
    Vector3 packed[2]{};
};

struct PathConstraintState {
    Vector2 position;
    Vector2 tangent;
    Vector2 velocity;
    float time;
};
static_assert(sizeof(PathConstraintState) == 0x1c);

struct AuxiliaryCollisionEntry {
    std::uint16_t kind;
    std::byte reserved02[0x1a];
    QueryShape shape;
    std::byte reserved38[0x10];
};
static_assert(offsetof(AuxiliaryCollisionEntry, shape) == 0x1c);
static_assert(sizeof(AuxiliaryCollisionEntry) == 0x48);

extern void *g_activeMapQuerySet;
extern std::uint32_t g_skipAuxiliaryCollisionRefresh;
extern float g_characterGroundDropLimit;
struct CharacterCollisionWork {
    float groundDropLimit;
    std::uint8_t reserved004;
    std::uint8_t stateByte;
    std::byte reserved006[2];
    std::uint32_t stateFlags;
    std::uint32_t interactionState;
    std::int32_t maxAngle;
    float distanceLimit;
    float interactionHeight;
    Vector3 shapeExtents;
    float radialAabbExtent;
    QueryShape shape;
    Vector3 reserved048;
    Vector3 targetPosition;
    std::uint32_t cachedYaw;
    std::uint32_t avoidanceMode;
    const void *intersectionContext;
    Character *movementCharacter;
    std::int32_t steeringAngle;
    std::int32_t candidateObjectCount;
    std::uint32_t candidateObjects[8];
    std::uint32_t proximityCooldown;
    std::uint32_t proximityObject;
    float proximityDistance;
    std::uint32_t proximityFlags;
    std::int32_t reserved0b0;
    float lateralDamp;
    float lastAdjustMagnitude;
    float forwardDamp;
    Vector3 carryDelta;
    std::uint32_t avoidanceResult;
    float verticalClampLimit;
    float verticalClampAccel;
    CharacterCollisionWork *next;
    CharacterCollisionWork *previous;
    Character *character;
    std::uint32_t publishedFlags;
    std::uint32_t avoidanceLock;
    float previousY;
    float movementFailTimer;
    std::int32_t lastRepositionYaw;
    Vector3 lastRepositionPosition;
    std::uint8_t changed;
    std::byte reserved111[3];
    float groundYOffset;
};
extern CharacterCollisionWork *g_characterCollisionHead;

static_assert(offsetof(CharacterCollisionWork, stateFlags) == 0x08);
static_assert(offsetof(CharacterCollisionWork, shapeExtents) == 0x1c);
static_assert(offsetof(CharacterCollisionWork, radialAabbExtent) == 0x28);
static_assert(offsetof(CharacterCollisionWork, shape) == 0x2c);
static_assert(offsetof(CharacterCollisionWork, targetPosition) == 0x54);
static_assert(offsetof(CharacterCollisionWork, publishedFlags) == 0xf0);
static_assert(offsetof(CharacterCollisionWork, lastRepositionYaw) == 0x100);
static_assert(offsetof(CharacterCollisionWork, lastRepositionPosition) == 0x104);
static_assert(offsetof(CharacterCollisionWork, changed) == 0x110);
static_assert(sizeof(CharacterCollisionWork) == 0x118);

struct CCOWSteeringShapeRef {
    Character *character;
    const QueryShape *shape;
    QueryShape transformedShape;
};

CharacterCollisionWork *CCOW_Allocate(Character *character);
void CCOW_Initialize(Character *character, CharacterCollisionWork *work);
void CCOW_ResetState(CharacterCollisionWork *work);
void CCOW_Callback(Character *character, void *work);
void CCOW_Cleanup(Character *character, void *work);
void CCOW_RefreshAuthoredShape(Character *character);
std::size_t CCOW_CollectMobjSteeringShapes(CCOWSteeringShapeRef *outShapes, std::size_t capacity, bool convertToCharacterSpace = false);

struct CharacterVectorCollisionResult {
    std::uint32_t flags;
    float fraction;
    std::int32_t angle;
};


bool CheckVectorWithinConditions(const QueryShape &params, const QueryShape &box);
void DispatchVectorOrientationTests(
    const QueryShape &params, CharacterVectorCollisionResult &result, const Vector2 &origin, const Vector2 &movement, float radius);
std::int32_t coliTestPlane(const Vector3 &start, const Vector3 &end, float radius, Vector3 &output, const std::uint8_t *mask);
std::int32_t ctrlCheckColi(const Vector3 &start, const Vector3 &end, float radius, const std::uint8_t *mask);
std::int32_t CalculateVectorDistanceAndExecuteMathFn(const Vector3 &start, const Vector3 &end, float radius, const std::uint8_t *mask);
std::int32_t COLS_TraceCharacterVector(
    const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, Character *ignoredA, Character *ignoredB);
void COLS_TraceMapRay(const Vector3 &start, const Vector3 &end, CharacterVectorCollisionResult &result);
void COLS_TraceMapVector(const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result);
void COLS_TraceMapVectorWithMask(const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, const std::uint8_t *mask);



std::int32_t coliTestCharacterShapes(
    std::uint32_t requestMode, const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, std::uint32_t &hitKind);
std::int32_t coliTestAuxiliaryShapes(
    std::uint32_t requestMode, const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, std::uint32_t &hitKind);
float COLS_QueryCombatMoveClearance(const Vector3 &origin, std::int16_t directionDelta, float maxDistance, std::int32_t *hitAngle);

extern std::int32_t g_navStateNext;
extern std::int32_t g_navMetricNext;
float CalculateNavigationCollisionResponse(const Vector3 &position, const Vector3 &direction, std::int32_t *angle, float radius, std::uint32_t owners);
bool COLS_UpdatePathWithConstraints(
    PathConstraintState &path, std::int32_t &statusBits, const std::array<Vector2, 4> &curves, const std::array<Vector3, 4> &spheres);

float ryoComputeNavAngleDeltaAtYaw(const Vector3 &position, std::uint16_t yaw, std::int32_t *angle, float radius);

bool au_re_CheckIfPointWithinShape(const QueryShape *query, const AuxiliaryCollisionEntry **matchedEntry);
void CEWP_ResetPointTestCache(Character *player);
bool CEWP_MapTestsEnabled();
bool CEWP_CharacterTestsEnabled();
std::int32_t cewpGetDesiredAngle(const QueryShape &shape);
bool cewpTestPointWithinRadius(const Vector3 &point, float radius, const std::uint8_t *mask);
bool CEWP_TestSegment(const Vector3 &start, const Vector3 &end, const std::uint8_t *mask);

void COLS_Free();
void COLS_ResetQueryState();
void COLS_Process(void *data);
void *COLS_FindQuerySet(std::uint32_t name);
bool SelectCharacterMapQuery(Character *character);

std::uint32_t mevwCheckEventArea(float x, float z);
// Debug view: visits each record of a query set's chunk (0 collision, 2 event) whose grid cells overlap the XZ square around (x, z).
void COLS_DebugForEachRecord(const void *querySet, std::uint32_t chunkKind, float x, float z, float extent,
    const std::function<void(std::uint32_t tag, std::uint32_t mode, const std::byte *data)> &visit);

bool Matrix4x4_TranslateAndRotate(const CharacterIdTransform &transform);

float FindHighestValidCollisionBelow(const Vector3 &localPoint, float heightRadius, float undoRadius);
std::uint16_t COLS_EvaluateWeatherSoundEnvironment(float x, float z);
float mapUpdateIntersectionStateAtLocalPoint(const Vector3 &localPoint);
float MapProbe_UpdateIntersectionStateAtPoint(const Vector3 &worldPoint);
std::int32_t FindHighestPointBelow100ForIKMotion(const Vector3 &point, float heightRadius, float undoRadius, float &outY);
bool COLS_SignalBestInteractionTarget(Character *character);
struct CollisionPointQueryOutputs {
    void *value4;
    void *value8;
    void *valueC;
    void *value10;
};

bool CheckIsPointIntersectWith(const Vector3 &point, const CollisionPointQueryOutputs *outputs, std::uint32_t token);
const std::uint32_t *COLS_ResolveSelectedCollisionFunction();

std::int32_t COLS_GetSelectedCollisionRecordValue();
std::int32_t coneCheck(const Vector3 &end, const Vector3 &point);
bool COLS_SearchSegmentHitsCycleCharacter(const Character &character, const Vector3 &start, const Vector3 &end);


bool mapQuerySelectActiveGridSection(std::uint8_t sectionIndex);
std::int64_t ExecuteMathFnWrapper(const QueryShape *query, const std::uint8_t *cm);

void Script_GetHighestIntersectionAtLocalPoint(ScenarioContext *context, void *rawParams);
void Script_GetHighestCollisionBelow(ScenarioContext *context, void *rawParams);
void Script_UpdateTaskCollisionCache(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
