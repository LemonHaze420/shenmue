#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/osage_dc.hpp"

#include "s2/model_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/weather_wind_dc.hpp"
#include "s2/math.hpp"
#include "s2/pause.hpp"
#include "s2/field_collision_dc.hpp"
#include "common/native_sqrt.hpp"
#include <cmath>
#include <cstring>
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/osage_collision_data_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#else
#include "s2/osage.hpp"
#include "s2/character.hpp"
#include "s2/model_component.hpp"
#include "s2/mt7.hpp"
#include "s2/hlib.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/weather.hpp"
#include "s2/field_collision.hpp"
#include "s2/face.hpp"
#include "s2/math.hpp"
#include "s2/game_runtime.hpp"
#include "d3t/buffer.hpp"
#include "s2/pause.hpp"
#include "s2/map_control.hpp"
#include "s2/event_runtime.hpp"
#include <cmath>
#include <cstring>
#include <bit>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

OsageSpringParameters defaultSpring{0, 1.0f, 0.001f, 0.65f, 0.0f, 0.01f};

OsageParameters defaultParameters{OSAGE_COMPONENT, 0, 0, 0, 0, 0};

OsageCollisionPoint defaultCollisionPoints[28]{{1, 0, 14, 12}, {5, 0, 10, 8}, {1, 0, 9, 9}, {0, 0, -8, 12}, {23, -1, -12, 16}, {23, -1, 12, 16}, {-1, 1, 8, 15},
    {-1, 1, -8, 15}, {0, 0, -14, 11}, {2, 1, 4, 14}, {0, 0, 14, 11}, {2, 1, -4, 14}, {0, 0, 2, 10}, {24, 1, -1, 9}, {0, 0, -2, 10}, {24, 1, 1, 9},
    {0, 1, -2, 9}, {34, 1, 0, 7}, {0, 1, 2, 9}, {34, 1, 0, 7}, {-1, -2, -8, 15}, {-1, -2, 8, 15}, {20, -1, 7, 16}, {20, -1, -7, 16}, {0, 0, 1, 14},
    {34, -2, 0, 10}, {0, 1, -1, 14}, {34, -2, 0, 10}};

constexpr std::int16_t collisionBones[28]{5, 5, 5, 5, 3, 3, 3, 3, 15, 15, 8, 8, 16, 16, 9, 9, 19, 19, 12, 12, 22, 22, 22, 22, 28, 28, 23, 23};

struct ReferenceAngle {
    std::int32_t bone;
    Vector3 base, positive, negative;
};

constexpr ReferenceAngle referenceAngles[5]{{5, {1, 0, 0}, {1, 0, -1}, {1, 0, 1}}, {3, {0, -1, 0}, {-1, -1, 0}, {1, -1, 0}},
    {22, {0, 1, 0}, {1, 1, 0}, {-1, 1, 0}}, {19, {1, 0, 0}, {1, -1, 0}, {1, 1, 0}}, {12, {1, 0, 0}, {1, -1, 0}, {1, 1, 0}}};

float hairDistance;

float motionScale;

float inverseMotionScale;

float groundHeight;

Vector2 randomOscillation;

Vector4 secondarySurfaces[28];

bool useSecondarySurfaces;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

Vector3 Translation();
float Sin(std::int32_t angle);
float Cos(std::int32_t angle);
std::int32_t Degrees(float value);
void BindTransform(OsageNode &node);
void ViewSpaceFromEuler(OsageNode &node);
void BuildTransform(OsageNode &node);
void OsageCleanup(Character *character, void *payload);
void OsageUpdateNullsub(Character *, void *);
void ReadParameters(OsageWork &work, MT7Node &model);
void AllocateNodeTree(OsageWork &work, MT7Node *model);
float MotionMagnitude(OsageNode &node);
void Spring(OsageNode &node, float pitchRate, float yawRate, float pitchTarget, float yawTarget);
void AdvancePhase(OsageNode &node, float pitch, float yaw);
void Offsets(OsageNode &node);
void ClampDistance(const Vector3 &origin, Vector3 &point, float distance);
bool ProjectSurface(const Vector4 *surface, Vector3 &point, std::int32_t count, float margin);
bool ProjectEnabled(OsageWork &work, const Vector3 &origin, Vector3 &point, float distance, float margin, std::uint16_t mask);
void ParentConstraint(const Vector3 &origin, Vector3 &point, const Vector3 &parent, float distance);
Vector3 FacialDisplacement(Character *character);
void AlignEndpoint(const Vector3 &origin, Vector3 &point);
float Noise(std::int16_t angle);
void BeginResponse(OsageNode &node, std::int32_t index, std::uint32_t extra = 0);
void PublishEulerResponse(OsageNode &node);
Vector3 EndpointPosition(OsageNode &node);
void ResetTransform(OsageNode &node);
void SimpleWindTransform(OsageNode &node, bool collision);
void WeatherWindTransform(OsageNode &node);
void DelayedWeatherTransform(OsageNode &node);
void HairStrandTransform(OsageNode &node, Character *character);
void ObjectWindTransform(OsageNode &node);
void OscillationTransform(OsageNode &node);
const OsageSpringParameters &SpringParameters(OsageNode &node);
void SwayOffset(float &position, float &velocity, float acceleration, float bound);
void StrandSpringTransform(OsageNode &node);
bool ChildOwnsTransform(const Matrix4x4 *transform, MT7Node *child);
void SaveParentTransforms(MT7Node *node);
void ProcessNodeTree(MT7Node *model, Character *character);
void CollisionSurfaces(const OsageCollisionPoint *points, MotionWork *motion, Vector4 *output);

} // namespace

void mtAssignOsage(Character *character);
void FindOSAG(Character *character);
void toggleOsagFlags(Character *character);
void UpdateCharacterOsage(MT7Node *model, Character *character);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool IsOsageKind(std::int32_t kind) {
    for (const std::int16_t candidate : osageNodeKinds) {
        if (candidate < 0)
            break;
        if (static_cast<std::uint16_t>(kind) == candidate)
            return true;
    }
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

OsageWork *FindOsage(const Character *character) {
    return static_cast<OsageWork *>(FindModelComponent(character, OSAGE_COMPONENT));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct ReferenceAngle {
    std::int32_t bone;
    Vector3 base, positive, negative;
};

void PlaceSpringEndpoint(OsageNode &node, MT7Node *model, MT7Node *endpointNode, float pitch, float length, Vector3 *endpoint);

} // namespace

void AllocateOsageNodeTree(OsageWork *work, MT7Node *model);

void AllocateOsageNode(OsageWork *work, MT7Node *model);

void ReadOsageParameters(OsageWork *work);

void OsageUpdateNullsub(Character *, void *);

void OsageCleanup(Character *character, void *payload);

OsageCollisionPoint *GetDefaultOsageCollisionPoints(shen_name character);

OsageCollisionPoint osageCollisionPointGroups[14][28] = {
    {
        {1, 0, 9, 10},
        {4, 0, 4, 8},
        {0, 0, 4, 7},
        {0, 0, -4, 7},
        {3, -2, -6, 11},
        {3, -2, 6, 11},
        {14, -3, 2, 11},
        {14, -3, -2, 11},
        {-2, 1, -7, 7},
        {10, -6, -2, 8},
        {-2, 1, 7, 7},
        {10, -6, 2, 8},
        {2, 0, 2, 7},
        {23, 1, 1, 6},
        {2, 0, -2, 7},
        {23, 1, -1, 6},
        {3, 1, 1, 6},
        {25, 0, 0, 9},
        {3, 1, -1, 6},
        {25, 0, 0, 9},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
    },
    {
        {3, 0, 13, 10},
        {5, 0, 9, 8},
        {2, 0, 8, 7},
        {0, 0, -3, 7},
        {22, -1, -8, 10},
        {22, -1, 8, 10},
        {15, -3, 3, 12},
        {15, -3, -3, 12},
        {-1, 0, -6, 8},
        {0, 0, 5, 10},
        {-1, 0, 6, 8},
        {0, 0, -5, 10},
        {4, 0, 0, 6},
        {24, 0, 0, 5},
        {4, 0, 0, 6},
        {24, 0, 0, 5},
        {0, 0, 0, 5},
        {30, 0, 0, 5},
        {0, 0, 0, 5},
        {30, 0, 0, 5},
        {-8, 3, -2, 10},
        {-8, 3, 2, 10},
        {8, 2, 4, 11},
        {8, 2, -4, 11},
        {-3, 2, 0, 10},
        {32, 1, 1, 8},
        {-3, 2, 0, 10},
        {32, 1, -1, 8},
    },
    {
        {1, 0, 8, 12},
        {5, 0, 9, 8},
        {2, 0, 8, 7},
        {0, 0, -3, 7},
        {17, -2, -7, 10},
        {17, -2, 7, 10},
        {10, -4, 3, 12},
        {9, -3, -4, 12},
        {-1, -2, -6, 8},
        {0, -2, 2, 10},
        {-2, -1, 6, 8},
        {0, -2, -2, 10},
        {5, -1, 0, 7},
        {21, 0, 0, 6},
        {4, 0, 0, 7},
        {22, 0, 0, 6},
        {3, 1, 0, 6},
        {30, 0, 0, 5},
        {3, 0, 0, 5},
        {30, 0, 0, 5},
        {1, 2, -2, 10},
        {0, 3, 2, 10},
        {4, 0, 4, 11},
        {3, 0, -4, 11},
        {-3, 2, 0, 10},
        {32, 1, 1, 8},
        {-3, 2, 0, 10},
        {32, 1, -1, 8},
    },
    {
        {1, 0, 16, 11},
        {1, 0, 9, 8},
        {3, 0, 8, 9},
        {4, 0, -4, 10},
        {10, 0, -12, 12},
        {10, 0, 12, 12},
        {-5, -1, 4, 12},
        {-5, -1, -4, 12},
        {-5, 0, -14, 10},
        {5, 0, 4, 11},
        {-5, 0, 14, 10},
        {5, 0, -4, 11},
        {5, 0, 1, 9},
        {20, 0, 1, 8},
        {5, 0, -1, 9},
        {20, 0, -1, 8},
        {0, 0, 0, 7},
        {35, 0, 0, 6},
        {0, 0, 0, 7},
        {35, 0, 0, 6},
        {-2, 2, -5, 11},
        {-2, 2, 5, 11},
        {1, 0, 6, 12},
        {1, 0, -6, 12},
        {0, 0, 1, 11},
        {34, 1, 0, 10},
        {0, 0, -1, 11},
        {34, 1, 0, 10},
    },
    {
        {1, 0, 14, 12},
        {5, 0, 10, 8},
        {1, 0, 9, 9},
        {0, 0, -8, 12},
        {23, -1, -12, 16},
        {23, -1, 12, 16},
        {-1, 1, 8, 15},
        {-1, 1, -8, 15},
        {0, 0, -14, 11},
        {2, 1, 4, 14},
        {0, 0, 14, 11},
        {2, 1, -4, 14},
        {0, 0, 2, 10},
        {24, 1, -1, 9},
        {0, 0, -2, 10},
        {24, 1, 1, 9},
        {0, 1, -2, 9},
        {34, 1, 0, 7},
        {0, 1, 2, 9},
        {34, 1, 0, 7},
        {-1, -2, -8, 15},
        {-1, -2, 8, 15},
        {20, -1, 7, 16},
        {20, -1, -7, 16},
        {0, 0, 1, 14},
        {34, -2, 0, 10},
        {0, 1, -1, 14},
        {34, -2, 0, 10},
    },
    {
        {4, 0, 8, 10},
        {3, 0, 10, 8},
        {1, -2, -1, 9},
        {3, 0, 6, 11},
        {0, 2, -2, 14},
        {3, -3, 8, 10},
        {-2, 3, 16, 7},
        {-1, 1, -8, 10},
        {-1, 1, -8, 11},
        {2, 1, 5, 10},
        {-2, 2, 6, 10},
        {2, 1, -4, 11},
        {0, 0, 2, 10},
        {24, 1, -1, 9},
        {0, 3, -3, 10},
        {24, 1, 1, 9},
        {0, 1, -2, 9},
        {34, 1, 0, 8},
        {0, 1, 2, 9},
        {34, 1, 0, 7},
        {-1, -2, -8, 12},
        {-1, -2, 8, 15},
        {22, -1, 15, 5},
        {7, 5, 10, 7},
        {0, 0, 1, 14},
        {34, -2, 0, 10},
        {0, 1, -1, 14},
        {34, -2, 0, 10},
    },
    {
        {2, 0, 8, 12},
        {7, 0, 1, 8},
        {1, 0, 4, 11},
        {3, 0, -8, 9},
        {14, -2, -8, 12},
        {14, -2, 8, 12},
        {2, -2, 6, 14},
        {2, -2, -6, 14},
        {-2, 0, -11, 9},
        {0, -1, 3, 11},
        {-2, 0, 11, 9},
        {0, -1, -3, 11},
        {2, 2, 1, 7},
        {24, 1, -1, 9},
        {2, 2, -1, 7},
        {24, 1, 1, 9},
        {0, 1, -2, 9},
        {34, 1, 0, 7},
        {0, 1, 2, 9},
        {34, 1, 0, 7},
        {0, 7, -8, 13},
        {0, 7, 8, 13},
        {-2, 3, 7, 14},
        {-2, 3, -7, 14},
        {0, 0, 1, 14},
        {34, -2, 0, 10},
        {0, 1, -1, 14},
        {34, -2, 0, 10},
    },
    {
        {0, 0, 6, 11},
        {0, 0, -3, 8},
        {0, 0, 4, 7},
        {0, 0, -8, 7},
        {11, 1, -8, 10},
        {11, 1, 8, 10},
        {3, -2, 4, 12},
        {3, -2, -4, 12},
        {-5, 0, -6, 9},
        {2, 1, 4, 8},
        {-5, 0, 6, 9},
        {2, 1, -4, 8},
        {5, 2, 3, 7},
        {23, 0, 0, 6},
        {5, 2, -3, 7},
        {24, 0, 0, 6},
        {0, 0, 0, 7},
        {34, 0, 0, 7},
        {0, 0, 0, 7},
        {34, 0, 0, 7},
        {-1, -2, -8, 15},
        {-1, -2, 8, 15},
        {2, -1, 7, 16},
        {2, -1, -7, 16},
        {0, 0, 1, 14},
        {34, -2, 0, 10},
        {0, 0, -1, 14},
        {34, -2, 0, 10},
    },
    {
        {1, 0, 5, 10},
        {7, 0, 1, 6},
        {0, 0, 1, 6},
        {0, 0, -8, 7},
        {12, 1, -7, 11},
        {12, 1, 7, 11},
        {5, -3, 3, 13},
        {5, -3, -3, 13},
        {-2, 1, -8, 7},
        {2, 2, 2, 8},
        {-2, 1, 8, 7},
        {1, 2, -2, 8},
        {3, 0, 2, 8},
        {24, 0, 0, 8},
        {3, 0, -2, 8},
        {24, 0, 0, 8},
        {0, 0, 0, 8},
        {34, 1, 0, 7},
        {0, 0, 0, 8},
        {34, 1, 0, 7},
        {-1, -2, -3, 15},
        {-1, -2, 3, 15},
        {4, 12, 2, 13},
        {4, 12, -2, 13},
        {0, 0, 1, 14},
        {34, -2, 0, -2},
        {0, 0, -1, 14},
        {34, -2, 0, -2},
    },
    {
        {1, 0, 5, 10},
        {7, 0, 1, 6},
        {0, 0, 1, 6},
        {0, 0, -8, 7},
        {12, -1, -10, 11},
        {12, -1, 10, 11},
        {27, -10, 15, 10},
        {27, -10, -15, 10},
        {-4, 0, -8, 9},
        {-2, 0, 2, 10},
        {-4, 0, 8, 9},
        {-2, 0, -2, 10},
        {3, 0, 2, 7},
        {24, 1, -1, 7},
        {3, 0, -2, 7},
        {24, 1, 1, 7},
        {0, 1, -2, 7},
        {34, 1, 0, 7},
        {0, 1, 2, 7},
        {34, 1, 0, 7},
        {-1, -2, -8, 15},
        {-1, -2, 8, 15},
        {5, -1, 7, 12},
        {5, -1, -7, 12},
        {0, 0, 1, 14},
        {34, -2, 0, 10},
        {0, 1, -1, 14},
        {34, -2, 0, 10},
    },
    {
        {1, 0, 8, 12},
        {5, 0, 9, 8},
        {2, 0, 8, 7},
        {0, 0, -3, 7},
        {16, 1, -9, 10},
        {16, 1, 9, 10},
        {7, -1, 7, 14},
        {7, -1, -7, 14},
        {-4, 0, -12, 10},
        {0, -2, 2, 12},
        {-4, 0, 12, 10},
        {0, -2, -2, 12},
        {3, 0, 2, 8},
        {26, 0, 0, 6},
        {3, 0, -2, 8},
        {26, 0, 0, 6},
        {3, 0, 0, 7},
        {34, 0, 0, 7},
        {3, 0, 0, 7},
        {34, 0, 0, 7},
        {1, 2, -2, 10},
        {0, 3, 2, 10},
        {4, 0, 4, 11},
        {3, 0, -4, 11},
        {-3, 2, 0, 10},
        {32, 1, 1, 8},
        {-3, 2, 0, 10},
        {32, 1, -1, 8},
    },
    {
        {0, 0, 7, 15},
        {7, 0, 1, 12},
        {0, 0, 1, 6},
        {0, 0, -8, 7},
        {14, 3, -14, 26},
        {14, 3, 14, 26},
        {0, 0, -10, 30},
        {0, 0, 10, 30},
        {-2, 1, -8, 15},
        {-5, 2, -25, 14},
        {-5, 2, 25, 14},
        {-2, 1, 8, 15},
        {6, 0, 2, 13},
        {36, -1, 0, 11},
        {6, 0, -2, 13},
        {36, -1, 0, 11},
        {0, -1, 0, 11},
        {28, 0, 0, 8},
        {0, -1, 0, 11},
        {28, 0, 0, 8},
        {11, -9, -13, 30},
        {11, -9, 13, 30},
        {4, 2, 6, 38},
        {4, 2, -6, 38},
        {0, 9, 3, 30},
        {52, 0, 0, 23},
        {0, 9, -3, 30},
        {52, 0, 0, 23},
    },
    {
        {2, 0, 8, 12},
        {6, 0, 2, 8},
        {1, 0, 4, 8},
        {0, 0, -8, 9},
        {14, 0, -8, 12},
        {14, 0, 8, 12},
        {2, -2, 6, 14},
        {2, -2, -6, 14},
        {-2, 0, -11, 9},
        {2, -1, 3, 9},
        {-2, 0, 11, 9},
        {2, -1, -3, 9},
        {2, 2, 1, 7},
        {24, 1, -1, 9},
        {2, 2, -1, 7},
        {24, 1, 1, 9},
        {0, 1, -2, 9},
        {34, 1, 0, 7},
        {0, 1, 2, 9},
        {34, 1, 0, 7},
        {0, 7, -8, 13},
        {0, 7, 8, 13},
        {-2, 3, 7, 14},
        {-2, 3, -7, 14},
        {0, 0, 1, 14},
        {34, -2, 0, 10},
        {0, 1, -1, 14},
        {34, -2, 0, 10},
    },
    {
        {1, 0, 5, 10},
        {7, 0, 1, 6},
        {0, 0, 1, 6},
        {0, 0, -8, 7},
        {12, 1, -7, 7},
        {12, 1, 7, 8},
        {10, -3, 6, 9},
        {10, -3, -6, 9},
        {-6, 2, -8, 6},
        {2, 0, 2, 10},
        {-6, 2, 8, 6},
        {1, 0, -2, 10},
        {3, 0, 2, 8},
        {24, 0, 0, 8},
        {3, 0, -2, 8},
        {24, 0, 0, 8},
        {0, 0, 0, 8},
        {34, 1, 0, 7},
        {0, 0, 0, 8},
        {34, 1, 0, 7},
        {-1, -2, -3, 15},
        {-1, -2, 3, 15},
        {4, 12, 2, 13},
        {4, 12, -2, 13},
        {0, 0, 1, 14},
        {34, -2, 0, -2},
        {0, 0, -1, 14},
        {34, -2, 0, -2},
    },
};

namespace {

OsageSpringParameters defaultOsagePrimarySpring = kDefaultOsageSpringParameters;

OsageSpringParameters defaultOsageSecondarySpring = kDefaultOsageSpringParameters;

OsageParameters defaultOsageParameters = {shen_name(OSAGE_COMPONENT), {0}, {reinterpret_cast<std::int32_t>(&defaultOsagePrimarySpring)},
    {reinterpret_cast<std::int32_t>(&defaultOsageSecondarySpring)}, 0, {}};

float hairDistance, motionScale, inverseMotionScale, groundHeight;

float randomOscillationX, randomOscillationY;

Vector4 secondarySurfaces[28];

std::int32_t useSecondarySurfaces;

std::uint16_t defaultCollisionMask = 0xffff;

static const std::int16_t collisionBones[28] = {5, 5, 5, 5, 3, 3, 3, 3, 15, 15, 8, 8, 16, 16, 9, 9, 19, 19, 12, 12, 22, 22, 22, 22, 28, 28, 23, 23};

static const ReferenceAngle referenceAngles[5] = {{5, {1, 0, 0}, {1, 0, -1}, {1, 0, 1}}, {3, {0, -1, 0}, {-1, -1, 0}, {1, -1, 0}},
    {22, {0, 1, 0}, {1, 1, 0}, {-1, 1, 0}}, {19, {1, 0, 0}, {1, -1, 0}, {1, 1, 0}}, {12, {1, 0, 0}, {1, -1, 0}, {1, 1, 0}}};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void mtAssignOsage(Character *character);
void AllocateOsageNodeTree(OsageWork *work, MT7Node *model);
void AllocateOsageNode(OsageWork *work, MT7Node *model);
void ReadOsageParameters(OsageWork *work);
void CleanupOsageNodes(OsageWork *work);
void OsageUpdateNullsub(Character *, void *);
void OsageCleanup(Character *character, void *payload);
void toggleOsagFlags(Character *character);
OsageCollisionPoint *GetDefaultOsageCollisionPoints(shen_name character);
void FindOSAG(Character *character);
void SetOsageMode(Character *character, std::uint8_t mode);
void UpdateCharacterOsage(MT7Node *model, Character *character);

namespace {

Vector2 O2(float x, float y);
Vector3 O3(float x, float y, float z);
Vector3i OI(std::int32_t x, std::int32_t y, std::int32_t z);
Vector4 O4(float x, float y, float z, float w);
float OsageMin(float first, float second);
float OsageMax(float first, float second);
float OsageDistance(const Vector3 *first, const Vector3 *second);
void OsageTranslate(float x, float y, float z);
const OsageSpringParameters &SpringParameters(OsageNode &node);
Vector3 Translation();
float Sin(std::int32_t angle);
float Cos(std::int32_t angle);
std::int32_t Degrees(float value);
float PhaseSin(float value);
void Rotate(const Vector3i &euler);
void BindTransform(OsageNode &node);
void SetViewSpaceTransformFromEuler(const Vector3i &euler);
void ViewSpaceFromEuler(MT7Node *model, OsageNode &node);
void BuildTransform(MT7Node *model, OsageNode &node);
float MotionMagnitude(OsageNode &node);
void Spring(OsageNode &node, float pitchRate, float yawRate, float pitchTarget, float yawTarget);
void AdvancePhase(OsageNode &node, float pitch, float yaw);
void Offsets(OsageNode &node);
void ShiftOffsets(OsageNode &node);
void ClampDistance(const Vector3 &origin, Vector3 &point, float distance);
bool ProjectSurface(const Vector4 *surface, Vector3 &point, std::int32_t count, float margin);
bool ProjectEnabled(const Vector4 *surfaces, const Vector3 &origin, Vector3 &point, float distance, float margin, std::int32_t mask);
void ParentConstraint(const Vector3 &origin, Vector3 &point, const Vector3 &parent, float distance);
void AlignEndpoint(const Vector3 &origin, Vector3 &point);
inline std::uint32_t Facing(OsageNode &node, std::int32_t index, std::uint32_t extra = 0) {
    if (node.linkFlags & 0x1f)
        return node.parent->facingAngle;
    return node.owner->referenceAngles[index] + extra;
}
void BeginResponse(OsageNode &node, MT7Node *model);
void PublishEulerResponse(OsageNode &node);
Vector3 EndpointPosition(OsageNode &node);
void ResetTransform(OsageNode &node, MT7Node *model);
void SimpleWindTransform(OsageNode &node, MT7Node *model);
void SimpleWindTransformNoCollision(OsageNode &node, MT7Node *model);
void WeatherWindTransform(OsageNode &node, MT7Node *model);
void DelayedWeatherTransform(OsageNode &node, MT7Node *model);
void HairStrandTransform(OsageNode &node, MT7Node *model);
void ObjectWindTransform(OsageNode &node, MT7Node *model);
void OscillationTransform(OsageNode &node, MT7Node *model);
void StrandSpringTransform(OsageNode &node, MT7Node *model);
std::int32_t ChildOwnsTransform(const Matrix4x4 *transform, MT7Node *child);
void SaveParentTransforms(MT7Node *node);
void ProcessNodeTree(MT7Node *model);
void PlaceSpringEndpoint(OsageNode &node, MT7Node *model, MT7Node *endpointNode, float pitch, float length, Vector3 *endpoint);
float GetOsageWind(Vector3 *wind);
float GetOsageWindUnclamped(Vector3 *wind);

} // namespace
#else
namespace {

bool IsOsageKind(std::int32_t kind);
OsageWork *FindOsage(const Character *character);
Vector3 FacialDisplacement(Character *character);
const OsageSpringParameters &SpringParameters(OsageNode &node);
Vector3 Translation();
float Sin(std::int32_t angle);
float Cos(std::int32_t angle);
std::int32_t Degrees(float value);
float PhaseSin(float value);
void Rotate(const Vector3i &euler);
void BindTransform(OsageNode &node);
void SetViewSpaceTransformFromEuler(const Vector3i &euler);
void ViewSpaceFromEuler(OsageNode &node);
void BuildTransform(OsageNode &node);
void CleanupNodes(OsageWork &work);
float MotionMagnitude(OsageNode &node);
void Spring(OsageNode &node, float pitchRate, float yawRate, float pitchTarget, float yawTarget);
void OsageUpdateNullsub(Character *, void *);
void ReadParameters(OsageWork &work, MT7Node &model);
void AllocateNodeTree(OsageWork &work, MT7Node *model);
void OsageCleanup(Character *character, void *payload);
void AdvancePhase(OsageNode &node, float pitch, float yaw);
void Offsets(OsageNode &node);
void ShiftOffsets(OsageNode &node);
void ClampDistance(const Vector3 &origin, Vector3 &point, float distance);
bool ProjectSurface(const Vector4 *surface, Vector3 &point, std::int32_t count, float margin);
bool ProjectEnabled(OsageWork &work, const Vector3 &origin, Vector3 &point, float distance, float margin, std::uint16_t mask);
void ParentConstraint(const Vector3 &origin, Vector3 &point, const Vector3 &parent, float distance);
void AlignEndpoint(const Vector3 &origin, Vector3 &point);
float Noise(std::int16_t angle);
std::uint32_t Facing(OsageNode &node, std::int32_t index, std::uint32_t extra = 0);
void BeginResponse(OsageNode &node, std::int32_t index, std::uint32_t extra);
void PublishEulerResponse(OsageNode &node);
Vector3 EndpointPosition(OsageNode &node);
void ResetTransform(OsageNode &node);
void SimpleWindTransform(OsageNode &node, bool collision);
void WeatherWindTransform(OsageNode &node);
void DelayedWeatherTransform(OsageNode &node);
void HairStrandTransform(OsageNode &node, Character *character);
void ObjectWindTransform(OsageNode &node);
void OscillationTransform(OsageNode &node);
void SwayOffset(float &position, float &velocity, float acceleration, float bound);
void StrandSpringTransform(OsageNode &node);
bool ChildOwnsTransform(const Matrix4x4 *transform, MT7Node *child);
void SaveParentTransforms(MT7Node *node);
void ProcessNodeTree(MT7Node *model, Character *character);
void CollisionSurfaces(const OsageCollisionPoint *points, MotionWork *motion, Vector4 *output);

} // namespace

void mtAssignOsage(Character *character);
void toggleOsagFlags(Character *character);
void FindOSAG(Character *character);
void SetCharacterOsageMode(Character *character, std::uint8_t mode);
void UpdateCharacterOsage(MT7Node *model, Character *character);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mtAssignOsage(Character *character) {
    MT7Node *const model = character->model;
    if (!model)
        return;
    for (const std::int16_t *kind = osageNodeKinds;; ++kind) {
        if (*kind < 0)
            return;
        if (FindMT7Node(model, *kind))
            break;
    }
    OsageWork *work = static_cast<OsageWork *>(charFindComp(character, OSAGE_COMPONENT));
    if (!work) {
        work = static_cast<OsageWork *>(AllocateModelComponent(character, OSAGE_COMPONENT, sizeof(OsageWork), OsageUpdateNullsub));
        if (!work)
            return;
        SetModelComponentCleanup(character, OSAGE_COMPONENT, OsageCleanup);
    }
    work->owner = character;
    work->collisionOwner = 0;
    work->nodes = 0;
    work->secondaryCollisionPoints = 0;
    work->collisionPoints = 0;
    work->motionScale = 0.0f;
    work->parameters = 0;
    work->mode = 0;
    if (FindMT7Node(model, 120))
        work->distanceLimit = 144.0f;
    else if (FindMT7Node(model, 123))
        work->distanceLimit = 36.0f;
    else if (FindMT7Node(model, 129))
        work->distanceLimit = 36.0f;
    else if (FindMT7Node(model, 130))
        work->distanceLimit = 36.0f;
    else
        work->distanceLimit = 4.0f;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    work->character = scenGetCharaName(control->scenarioTask, character);
    if (work->character) {
        ReadOsageParameters(work);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::UnitCurrentMatrix();
        AllocateOsageNodeTree(work, model);
        PopCurrentMatrix(1);
    }
}

#else

// @verified - p
void mtAssignOsage(Character *character) {
    if (!character->model)
        return;
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    if (model->nodes.empty())
        return;
    bool found = false;
    for (const std::int16_t kind : osageNodeKinds) {
        if (kind < 0)
            break;
        if (GetChildObjectByIndex(*model, &model->nodes.front(), static_cast<std::uint16_t>(kind))) {
            found = true;
            break;
        }
    }
    if (!found)
        return;
    OsageWork *work = FindOsage(character);
    if (!work) {
        work = static_cast<OsageWork *>(AllocateModelComponent(character, OSAGE_COMPONENT, sizeof(OsageWork), OsageUpdateNullsub, OsageCleanup));
        if (!work)
            return;
    }
    work->owner = character;
    work->collisionOwner = nullptr;
    work->nodes = nullptr;
    work->secondaryCollisionPoints = nullptr;
    work->collisionPoints = nullptr;
    work->motionScale = 0;
    work->parameters = nullptr;
    work->mode = 0;
    if (GetChildObjectByIndex(*model, &model->nodes.front(), 120))
        work->distanceLimit = 400;
    else if (GetChildObjectByIndex(*model, &model->nodes.front(), 123) || GetChildObjectByIndex(*model, &model->nodes.front(), 129) ||
        GetChildObjectByIndex(*model, &model->nodes.front(), 130))
        work->distanceLimit = 36;
    else
        work->distanceLimit = 16;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    work->character = scenGetCharaName(control ? control->scenarioTask : nullptr, character);
    if (work->character.name) {
        ReadParameters(*work, model->nodes.front());
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        AllocateNodeTree(*work, &model->nodes.front());
        Matrix4x4::Pop(1);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector2 O2(float x, float y) {
    Vector2 value = {x, y};
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 O3(float x, float y, float z) {
    Vector3 value = {x, y, z};
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3i OI(std::int32_t x, std::int32_t y, std::int32_t z) {
    Vector3i value = {x, y, z};
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector4 O4(float x, float y, float z, float w) {
    Vector4 value = {x, y, z, w};
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float OsageMin(float first, float second) {
    return first < second ? first : second;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float OsageMax(float first, float second) {
    return first > second ? first : second;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float OsageDistance(const Vector3 *first, const Vector3 *second) {
    return CalculateDistanceBetweenPoints(first, second);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void OsageTranslate(float x, float y, float z) {
    const Vector3 offset = {x, y, z};
    matrix_stack::TranslateVector(&offset);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void GetFacialOsageDisplacement(void *face, Vector3 *output) {
    const std::byte *bytes = static_cast<std::byte *>(face);
    const std::int32_t count = *reinterpret_cast<const std::int32_t *>(bytes + 0x130);
    const std::byte *control = *reinterpret_cast<std::byte *const *>(bytes + 0x134);
    float first = 0.0f, second = 0.0f;
    if (count > 0) {
        const Vector3 *position = *reinterpret_cast<Vector3 *const *>(control + 0x2c);
        const Vector3 *neutral = reinterpret_cast<const Vector3 *>(control);
        const float x = position->x - neutral->x, z = position->z - neutral->z;
        for (std::int32_t i = 0; i < count; ++i) {
            if (x < first)
                first = x;
            if (z < second)
                second = z;
        }
    }
    output->x = 0.0f;
    output->y = second;
    output->z = -first;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

Vector3 FacialDisplacement(Character *character) {
    Vector3 displacement{};
    if (!character || !character->facialModel)
        return displacement;
    const FacialModelResource *const resource = reinterpret_cast<const FacialModelResource *>(character->facialModel);
    if (resource->vertices[7].empty())
        return displacement;
    const FacialMorphControl &control = resource->vertices[7][0];
    const std::byte *const bytes = static_cast<const std::byte *>(d3t::GetBufferCpuData(control.buffer->handle, nullptr));
    Vector3 position;
    std::memcpy(&position, bytes + control.byteOffset, sizeof(position));
    displacement.z = -std::fmin(position.x - control.vertex.neutral.x, 0.0f);
    displacement.y = std::fmin(position.z - control.vertex.neutral.z, 0.0f);
    return displacement;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

const OsageSpringParameters &SpringParameters(OsageNode &node) {
    const OsageParameters &parameters = *node.owner->parameters;
    const std::uint8_t depth = static_cast<std::uint8_t>(node.linkFlags & 0x1fU);
    if (parameters.secondary && parameters.secondaryDepth && depth >= parameters.secondaryDepth)
        return *parameters.secondary;
    return *parameters.primary;
}

} // namespace

#else

namespace {

const OsageSpringParameters &SpringParameters(OsageNode &node) {
    OsageParameters &parameters = *node.owner->parameters;
    const std::int32_t depth = node.linkFlags & 0x1f;
    if (&parameters == &defaultParameters)
        return defaultSpring;
    if (!parameters.primaryOffset)
        return defaultSpring;
    if (parameters.secondaryOffset && parameters.secondaryDepth <= depth && parameters.secondaryDepth)
        return *static_cast<const OsageSpringParameters *>(HLib::GetPointerFromDynamicHeapOffsetBase(parameters.secondaryOffset));
    return *static_cast<const OsageSpringParameters *>(HLib::GetPointerFromDynamicHeapOffsetBase(parameters.primaryOffset));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 Translation() {
    Vector3 position;
    matrix_stack::GetTranslation(&position);
    return position;
}

} // namespace

#else

namespace {

Vector3 Translation() {
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float Sin(std::int32_t angle) {
    return dc::sdk::SinFixed(static_cast<std::int16_t>(angle));
}

} // namespace

#else

namespace {

float Sin(std::int32_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(angle));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float Cos(std::int32_t angle) {
    return dc::sdk::CosFixed(static_cast<std::uint16_t>(angle));
}

} // namespace

#else

namespace {

float Cos(std::int32_t angle) {
    return cos_fixed16(static_cast<std::uint16_t>(angle));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t Degrees(float value) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(value * 8192.0f / 45.0f));
}

} // namespace

#else

namespace {

std::int32_t Degrees(float value) {
    return static_cast<std::int32_t>(value * 8192.0f / 45.0f);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float PhaseSin(float value) {
    return Sin(Degrees(value));
}

} // namespace

#else

namespace {

float PhaseSin(float value) {
    return Sin(Degrees(value));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void Rotate(const Vector3i &euler) {
    if (euler.z)
        Matrix4x4::RotateAroundZ_Wrapper(euler.z);
    if (euler.y)
        Matrix4x4::RotateAroundY_Wrapper(euler.y);
    if (euler.x)
        Matrix4x4::RotateAroundX_Wrapper(euler.x);
}

} // namespace

#else

namespace {

void Rotate(const Vector3i &euler) {
    if (euler.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(euler.z));
    if (euler.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(euler.y));
    if (euler.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(euler.x));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void BindTransform(OsageNode &node) {
    Matrix4x4::GetTransformMatrix(&node.transform);
    node.node->transformOverride = &node.transform;
}

} // namespace

#else

namespace {

void BindTransform(OsageNode &node) {
    Matrix4x4::GetTransformMatrix(&node.transform);
    SetMT7NodeTransformOverride(*node.node, &node.transform);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void SetViewSpaceTransformFromEuler(const Vector3i &euler) {
    Matrix4x4::SetViewMatrixFromEuler(euler.x, euler.y, euler.z);
}

} // namespace

#else

namespace {

void SetViewSpaceTransformFromEuler(const Vector3i &euler) {
    Matrix4x4::SetViewMatrixFromEuler(euler.y, euler.x, euler.z);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AllocateOsageNodeTree(OsageWork *work, MT7Node *model) {
    if (model->sibling)
        matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
    Matrix4x4::SetViewMatrixFromEuler(model->rotation.x, model->rotation.y, model->rotation.z);
    for (std::int16_t index = 0; osageNodeKinds[index] >= 0; ++index) {
        if (model->id == osageNodeKinds[index]) {
            AllocateOsageNode(work, model);
            break;
        }
    }
    if (model->child)
        AllocateOsageNodeTree(work, model->child);
    if (model->sibling) {
        PopCurrentMatrix(1);
        AllocateOsageNodeTree(work, model->sibling);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AllocateOsageNode(OsageWork *work, MT7Node *model) {
    OsageNode *const node = static_cast<OsageNode *>(HLib::AllocateTaskData(sizeof(OsageNode), OSAGE_COMPONENT));
    if (!node)
        return;
    Vector3 position;
    matrix_stack::GetTranslation(&position);
    node->token = OSAGE_COMPONENT;
    node->owner = work;
    node->node = model;
    model->taskData = node;
    node->kind = -1;
    node->linkFlags = 0;
    if (position.z > 0.0f)
        node->linkFlags = 0x40;
    if (position.x > 0.0f)
        node->linkFlags |= 0x20U;
    node->updateFlags = 0x80;
    node->current.x = node->current.y = node->current.z = 0.0f;
    node->previous.x = node->previous.y = node->previous.z = 0.0f;
    node->facingAngle = 0;
    node->euler.x = node->euler.y = node->euler.z = 0;
    node->motionMagnitude = 0.0f;
    node->meshAllocation = 0;
    node->originalMesh = model->mesh;
    node->accumulatedSway = node->accumulatedForward = node->accumulatedVertical = 0.0f;
    node->pitchScale = node->yawScale = 0.0f;
    node->pitchPhase = dc::sdk::RandomFloat() * 360.0f;
    node->yawPhase = dc::sdk::RandomFloat() * 360.0f;
    for (std::int16_t index = 0; index < 10; ++index)
        node->offsets[index].x = node->offsets[index].y = 0.0f;
    for (std::int16_t index = 0; index < 32; ++index)
        node->vertexBindings[index] = -1;
    matrix_stack::GetMatrix(&node->transform);
    node->smoothingCountdown = 0;
    node->child = 0;
    node->parent = 0;
    node->next = work->nodes;
    work->nodes = node;
    const MT7Node *const parentModel = model->parent;
    if (parentModel) {
        OsageNode *parent = static_cast<OsageNode *>(parentModel->taskData);
        if (parent && parent->token == OSAGE_COMPONENT && parent->node->id == node->node->id) {
            node->linkFlags = static_cast<std::uint8_t>(parent->linkFlags + 1U);
            node->parent = parent;
            parent->child = node;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReadOsageParameters(OsageWork *work) {
    Character *const owner = work->owner;
    MT7Node *const model = owner->model;
    std::byte *source = 0;
    if (!model->taskData) {
        if (const ClothComponent *const cloth = reinterpret_cast<const ClothComponent *>(charFindComp(owner, CLOTH_COMPONENT)))
            source = cloth->source;
    } else {
        source = static_cast<std::byte *>(model->taskData);
        model->taskData = 0;
    }
    if (work->character != CHARACTER_SHENHUA && source && *reinterpret_cast<const std::uint32_t *>(source) == CLOTH_SOURCE) {
        const std::int32_t offset = *reinterpret_cast<const std::int32_t *>(source + 0x10);
        if (offset) {
            OsageParameters *const parameters = reinterpret_cast<OsageParameters *>(source + offset);
            if (parameters->token == OSAGE_COMPONENT && parameters->collisionOffset) {
                work->collisionPoints = reinterpret_cast<OsageCollisionPoint *>(source + parameters->collisionOffset);
                work->parameters = parameters;
                if (parameters->primaryOffset)
                    parameters->primary = reinterpret_cast<OsageSpringParameters *>(source + parameters->primaryOffset);
                if (parameters->secondaryOffset)
                    parameters->secondary = reinterpret_cast<OsageSpringParameters *>(source + parameters->secondaryOffset);
                if (!parameters->primary) {
                    parameters->primary = &defaultOsagePrimarySpring;
                    parameters->secondary = &defaultOsageSecondarySpring;
                    parameters->secondaryDepth = 0;
                }
                return;
            }
        }
    }
    work->collisionPoints = GetDefaultOsageCollisionPoints(work->character);
    work->parameters = &defaultOsageParameters;
    defaultOsageParameters.collisionPoints = work->collisionPoints;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CleanupOsageNodes(OsageWork *work) {
    OsageNode *node = work->nodes;
    while (node) {
        if (node->meshAllocation) {
            HLib::FreeTaskData(node->meshAllocation);
            node->meshAllocation = 0;
            if (node->node) {
                node->node->mesh = node->originalMesh;
                node->originalMesh = 0;
            }
        }
        HLib::FreeTaskData(node);
        node = node->next;
    }
    work->nodes = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void OsageUpdateNullsub(Character *, void *) {
}

#else

namespace {

// @verified - p
void OsageUpdateNullsub(Character *, void *) {
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void ReadParameters(OsageWork &work, MT7Node &model) {
    std::byte *source = static_cast<std::byte *>(model.taskData);
    if (source)
        model.taskData = nullptr;
    else if (ClothComponent *const cloth = static_cast<ClothComponent *>(FindModelComponent(work.owner, SHEN_NAME(CLMD))))
        source = cloth->source;
    if (source) {
        std::uint32_t token;
        std::int32_t offset;
        std::memcpy(&token, source, 4);
        std::memcpy(&offset, source + 0x10, 4);
        if (token == SHEN_NAME(CLSG) && offset) {
            OsageParameters *const parameters = reinterpret_cast<OsageParameters *>(source + offset);
            if (parameters->token.name == OSAGE_COMPONENT.name && parameters->collisionOffset) {
                work.parameters = parameters;
                work.collisionPoints = reinterpret_cast<OsageCollisionPoint *>(source + parameters->collisionOffset);
                const std::int32_t primary = parameters->primaryOffset;
                const std::int32_t secondary = parameters->secondaryOffset;
                parameters->primaryOffset = HLib::GetDynamicHeapOffset(source + primary);
                parameters->secondaryOffset = HLib::GetDynamicHeapOffset(source + secondary);
                if (!parameters->primaryOffset) {
                    // Static defaults are consumed directly, not encoded as heap-relative offsets.
                    parameters->secondaryOffset = 0;
                    parameters->secondaryDepth = 0;
                }
                return;
            }
        }
    }
    work.collisionPoints = work.character.name == SHEN_NAME(CHAR) ? nullptr : defaultCollisionPoints;
    work.parameters = &defaultParameters;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void AllocateNodeTree(OsageWork &work, MT7Node *model) {
    while (model) {
        if (model->runtimeSibling)
            Matrix4x4_Push();
        Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
        SetViewSpaceTransformFromEuler(model->rotation);
        if (IsOsageKind(model->id)) {
            OsageNode *const node = static_cast<OsageNode *>(HLib::AllocateTaskData(sizeof(OsageNode), OSAGE_COMPONENT));
            if (node) {
                node->token = OSAGE_COMPONENT;
                node->owner = &work;
                node->node = model;
                model->taskData = node;
                node->kind = -1;
                node->linkFlags = 0;
                const Vector3 position = Translation();
                if (position.z > 0)
                    node->linkFlags = 0x40;
                if (position.x > 0)
                    node->linkFlags |= 0x20;
                node->updateFlags = OsageUpdateWarmupStart;
                node->current = {};
                node->previous = {};
                node->facingAngle = 0;
                node->euler = {};
                node->motionMagnitude = node->accumulatedSway = node->accumulatedForward = node->accumulatedVertical = 0;
                node->pitchScale = node->yawScale = 0;
                node->pitchPhase = IncrementSharedPrngFloat() * 360.0f;
                node->yawPhase = IncrementSharedPrngFloat() * 360.0f;
                for (Vector2 &pair : node->offsets)
                    pair.x = 0;
                for (std::int16_t &binding : node->vertexBindings)
                    binding = -1;
                Matrix4x4::GetTransformMatrix(&node->transform);
                node->smoothingCountdown = 0;
                node->child = nullptr;
                node->parent = nullptr;
                node->meshAllocation = nullptr;
                node->originalMesh = model->runtimeMesh;
                node->next = work.nodes;
                work.nodes = node;
                if (model->runtimeParent) {
                    OsageNode *const parent = static_cast<OsageNode *>(model->runtimeParent->taskData);
                    if (parent && parent->token.name == OSAGE_COMPONENT.name && parent->node->id == model->id) {
                        node->parent = parent;
                        node->linkFlags = parent->linkFlags + 1;
                        parent->child = node;
                    }
                }
            }
        }
        if (model->runtimeChild)
            AllocateNodeTree(work, model->runtimeChild);
        if (!model->runtimeSibling)
            break;
        Matrix4x4::Pop(1);
        model = model->runtimeSibling;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void OsageCleanup(Character *character, void *payload) {
    if (character)
        CleanupOsageNodes(static_cast<OsageWork *>(payload));
}

#else

namespace {

// @verified - p
void OsageCleanup(Character *character, void *payload) {
    if (character)
        CleanupNodes(*static_cast<OsageWork *>(payload));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void toggleOsagFlags(Character *character) {
    OsageWork *work;
    OsageNode *node;
    work = static_cast<OsageWork *>(charFindComp(character, OSAGE_COMPONENT));
    if (!work)
        return;
    for (node = work->nodes; node; node = node->next)
        node->updateFlags = static_cast<std::uint8_t>((node->updateFlags & 0x0fU) | 0x80U);
    SetClothFlag(character, 0x10, 1);
}

#else

// @verified - p
void toggleOsagFlags(Character *character) {
    if (OsageWork *const work = FindOsage(character)) {
        for (OsageNode *node = work->nodes; node; node = node->next)
            node->updateFlags = (node->updateFlags & OsageUpdateHistoryMask) | OsageUpdateWarmupStart;
        SetClothFlag(character, 0x10, 1);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FindOSAG(Character *character) {
    OsageWork *const work = static_cast<OsageWork *>(charFindComp(character, OSAGE_COMPONENT));
    if (work) {
        CleanupOsageNodes(work);
        work->character = shen_name();
    }
    mtAssignOsage(character);
}

#else

// @verified - p
void FindOSAG(Character *character) {
    if (OsageWork *const work = FindOsage(character)) {
        CleanupNodes(*work);
        work->character = {};
    }
    mtAssignOsage(character);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetOsageMode(Character *character, std::uint8_t mode) {
    if (!character->model)
        return;
    OsageWork *const work = static_cast<OsageWork *>(charFindComp(character, OSAGE_COMPONENT));
    if (work)
        work->mode = mode;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void UpdateCharacterOsage(MT7Node *model, Character *character) {
    MotionWork *const motion = charFindMotionwork(character);
    if (!motion)
        return;
    OsageWork *const work = static_cast<OsageWork *>(charFindComp(character, OSAGE_COMPONENT));
    if (!work)
        return;
    hairDistance = 0.0f;
    matrix_stack::Matrix4x4_Push();
    std::int16_t i;
    Vector3 joint;
    for (i = 0; i < 5; ++i) {
        matrix_stack::LoadMatrix(mtwkGetBoneTransform(motion, referenceAngles[i].bone));
        matrix_stack::GetTranslation(&joint);
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(referenceAngles[i].base.x, referenceAngles[i].base.y, referenceAngles[i].base.z);
        Vector3 endpoint;
        matrix_stack::GetTranslation(&endpoint);
        PopCurrentMatrix(1);
        endpoint.y -= joint.y;
        if (endpoint.y > 0.5f) {
            Matrix4x4::preTranslate(referenceAngles[i].positive.x, referenceAngles[i].positive.y, referenceAngles[i].positive.z);
            matrix_stack::GetTranslation(&endpoint);
        } else if (endpoint.y < -0.5f) {
            Matrix4x4::preTranslate(referenceAngles[i].negative.x, referenceAngles[i].negative.y, referenceAngles[i].negative.z);
            matrix_stack::GetTranslation(&endpoint);
        }
        endpoint.x -= joint.x;
        endpoint.z -= joint.z;
        work->referenceAngles[i] = static_cast<std::uint16_t>(dc::sdk::ArcTan2Fixed(-endpoint.x, -endpoint.z));
    }
    const std::int16_t *bone = collisionBones;
    const OsageCollisionPoint *points = work->collisionPoints;
    if (points) {
        Vector4 *surface = work->collisionSurfaces;
        for (i = 0; i < 28; ++i) {
            matrix_stack::LoadMatrix(mtwkGetBoneTransform(motion, *bone++));
            Matrix4x4::preTranslate(points->x * 0.0099999998f, points->y * 0.0099999998f, points->z * 0.0099999998f);
            matrix_stack::GetTranslation(reinterpret_cast<Vector3 *>(surface));
            surface->w = points->radius * 0.0099999998f;
            ++points;
            ++surface;
        }
        useSecondarySurfaces = 0;
        if (work->collisionOwner && work->secondaryCollisionPoints) {
            MotionWork *const secondary = charFindMotionwork(work->collisionOwner);
            if (secondary) {
                points = work->secondaryCollisionPoints;
                bone = collisionBones;
                surface = secondarySurfaces;
                for (i = 0; i < 28; ++i) {
                    matrix_stack::LoadMatrix(mtwkGetBoneTransform(secondary, *bone++));
                    Matrix4x4::preTranslate(points->x * 0.0099999998f, points->y * 0.0099999998f, points->z * 0.0099999998f);
                    matrix_stack::GetTranslation(reinterpret_cast<Vector3 *>(surface));
                    surface->w = points->radius * 0.0099999998f;
                    ++points;
                    ++surface;
                }
                useSecondarySurfaces = 1;
            }
        }
    }
    groundHeight = -100000.0f;
    motionScale = work->motionScale;
    if (std::fabs(motionScale) < 0.00000011920929f) {
        motionScale = mtwkGetSpeed(motion, static_cast<MotionRegion>(0));
        if (std::fabs(motionScale) < 0.00000011920929f)
            motionScale = 1.0f;
    }
    inverseMotionScale = 1.0f / motionScale;
    work->flags = 0;
    work->windSpeedScaled = GetOsageWind(&work->wind);
    work->windSpeedNext = GetOsageWindUnclamped(&work->wind);
    matrix_stack::UnitCurrentMatrix();
    Vector3 position;
    Vector3i rotation;
    GetCharacterWorldPosition(character, &position);
    GetCharacterWorldRotation(character, &rotation);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
    ProcessNodeTree(model);
    PopCurrentMatrix(1);
}

#else

// @verified - p
void UpdateCharacterOsage(MT7Node *model, Character *character) {
    MotionWork *const motion = FindMotionWork(character);
    if (!motion)
        return;
    OsageWork *const work = FindOsage(character);
    if (!work)
        return;
    if (authGetLoaded())
        hairDistance = 0;
    else if (character->entity.name.name ==
        SHEN_NAME(CEWP) && (character->flags & CF_Valid) && IsOfThisVisibilityType(character, 0) && charaGetAlphaDraw(character) >= 0.01f) {
        const Vector3 position = GetCharacterWorldPosition(character);
        Vector3 camera;
        camCtrlGetNextCamPos(&camera);
        hairDistance = ((camera.z - position.z) * (camera.z - position.z)) + ((camera.x - position.x) * (camera.x - position.x));
    } else
        hairDistance = 10000;
    Matrix4x4_Push();
    for (std::int32_t i = 0; i < 5; ++i) {
        const ReferenceAngle &reference = referenceAngles[i];
        Matrix4x4::SetActiveTransform(mtwkGetBoneTransform(motion, reference.bone));
        const Vector3 joint = Translation();
        Matrix4x4_Push();
        Matrix4x4::preTranslate(reference.base.x, reference.base.y, reference.base.z);
        Vector3 endpoint = Translation();
        Matrix4x4::Pop(1);
        const float vertical = endpoint.y - joint.y;
        if (vertical > 0.5f || vertical < -0.5f) {
            const float blend = vertical > 0.5f ? vertical * 2 - 1 : vertical * -2 - 1;
            const Vector3 &target = vertical > 0.5f ? reference.positive : reference.negative;
            const Vector3 offset{blend * target.x + (1 - blend) * reference.base.x, blend * target.y + (1 - blend) * reference.base.y,
                blend * target.z + (1 - blend) * reference.base.z};
            Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
            endpoint = Translation();
        }
        work->referenceAngles[i] = FixedAtan2(-(endpoint.x - joint.x), -(endpoint.z - joint.z));
    }
    if (work->mode == 4) {
        std::int8_t *const points = reinterpret_cast<std::int8_t *>(work->collisionPoints);
        points[12] = -4;
        points[32] = points[33] = points[40] = points[41] = 0;
    }
    if (work->collisionPoints) {
        CollisionSurfaces(work->collisionPoints, motion, work->collisionSurfaces.data());
        useSecondarySurfaces = false;
        if (work->collisionOwner && work->secondaryCollisionPoints)
            if (MotionWork *const secondary = FindMotionWork(work->collisionOwner)) {
                CollisionSurfaces(work->secondaryCollisionPoints, secondary, secondarySurfaces);
                useSecondarySurfaces = true;
            }
    }
    groundHeight = -100000;
    motionScale = work->motionScale;
    if (std::fabs(motionScale) < 0.00000011920929f) {
        motionScale = mtwkGetSpeed(motion, MotionRegion::LowerBody);
        if (std::fabs(motionScale) < 0.00000011920929f)
            motionScale = 1;
    }
    work->flags = 0;
    inverseMotionScale = 1 / motionScale;
    work->windSpeedScaled = std::fmax(0.5f, GetEffectiveWindSpeed(GetWind()) * 30);
    std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
    work->wind = {Sin(phase) * work->windSpeedScaled, 0, Cos(phase) * work->windSpeedScaled};
    work->windSpeedNext = GetEffectiveWindSpeed(GetWind()) * 30;
    phase = GetCurrentWindPhaseAccumulator(GetWind());
    work->wind = {Sin(phase) * work->windSpeedNext, 0, Cos(phase) * work->windSpeedNext};
    Matrix4x4::SetPrePassToTempTransform();
    const Vector3 position = GetCharacterWorldPosition(character);
    const Vector3i euler = GetCharacterWorldRotation(character);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    SetViewSpaceTransformFromEuler(euler);
    ProcessNodeTree(model, character);
    Matrix4x4::Pop(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ProcessNodeTree(MT7Node *model) {
    if (model->sibling)
        matrix_stack::Matrix4x4_Push();
    OsageNode *const node = static_cast<OsageNode *>(model->taskData);
    if (node && node->token == OSAGE_COMPONENT) {
        OsageWork *const owner = node->owner;
        if (node->kind < 0) {
            node->kind = model->id;
            switch (node->kind) {
            case 120:
            case 133:
                if (node->linkFlags & 0x1f)
                    node->facingAngle = node->parent->facingAngle;
                else
                    node->facingAngle = node->owner->referenceAngles[0] + 0x8000;
                break;
            case 121:
            case 124:
            case 126:
            case 132:
                if (node->linkFlags & 0x1f)
                    node->facingAngle = node->parent->facingAngle;
                else
                    node->facingAngle = node->owner->referenceAngles[0];
                break;
            case 122:
                if (node->linkFlags & 0x1f)
                    node->facingAngle = node->parent->facingAngle;
                else
                    node->facingAngle = node->owner->referenceAngles[2] + 0x8000;
                break;
            case 123:
            case 125:
                if (node->linkFlags & 0x1f)
                    node->facingAngle = node->parent->facingAngle;
                else
                    node->facingAngle = node->owner->referenceAngles[1] + 0x8000;
                break;
            case 127:
            case 128:
            case 130:
                if (node->linkFlags & 0x1f)
                    node->facingAngle = node->parent->facingAngle;
                else
                    node->facingAngle = node->owner->referenceAngles[2];
                break;
            case 141:
            default:
                if (node->linkFlags & 0x1f)
                    node->facingAngle = node->parent->facingAngle;
                else
                    node->facingAngle = node->owner->referenceAngles[1];
                break;
            }
            BeginResponse(*node, model);
            node->previous = node->current;
            node->euler.x = model->rotation.x;
            node->euler.y = model->rotation.y;
            node->euler.z = model->rotation.z;
            ViewSpaceFromEuler(model, *node);
            if (!model->parent->transformOverride)
                model->parent->transformOverride = &node->parentTransform;
            matrix_stack::Matrix4x4_Push();
            MT7Node *const child = model->child;
            if (!child)
                Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
            else
                Matrix4x4::preTranslate(child->position.x, child->position.y, child->position.z);
            matrix_stack::GetTranslation(&node->endpoint);
            PopCurrentMatrix(1);
        } else if (shenIsGamePaused())
            model->transformOverride = &node->transform;
        else {
            node->updateFlags = (node->updateFlags & OsageUpdateWarmupMask) | (2 * (node->updateFlags & OsageUpdatePreviousFrames));
            randomOscillationX = dc::sdk::RandomFloat() * 5 - 2.5f;
            randomOscillationY = dc::sdk::RandomFloat() * 5 - 2.5f;
            model->transformOverride = 0;
            switch (node->kind) {
            case 120:
            case 133:
                if (hairDistance < owner->distanceLimit)
                    StrandSpringTransform(*node, model);
                break;
            case 121:
            case 132:
                if (hairDistance < 4)
                    HairStrandTransform(*node, model);
                break;
            case 125:
                if (hairDistance < 4)
                    ObjectWindTransform(*node, model);
                break;
            case 126:
                if (hairDistance < 4)
                    WeatherWindTransform(*node, model);
                break;
            case 124:
                if (hairDistance < 4)
                    DelayedWeatherTransform(*node, model);
                break;
            case 141:
                if (hairDistance < 4)
                    OscillationTransform(*node, model);
                break;
            case 127:
                if (hairDistance < 4)
                    SimpleWindTransform(*node, model);
                break;
            case 128:
                if (hairDistance < 4)
                    SimpleWindTransformNoCollision(*node, model);
                break;
            }
            ++owner->flags;
            if (!(node->linkFlags & 0x1f) && !model->parent->transformOverride)
                model->parent->transformOverride = &node->parentTransform;
            if (model->transformOverride) {
                if (node->updateFlags & OsageUpdateWarmupMask)
                    node->updateFlags = ((node->updateFlags + OsageUpdateWarmupStep) & OsageUpdateWarmupMask) | (node->updateFlags & OsageUpdateHistoryMask);
            } else {
                node->updateFlags |= OsageUpdateWarmupStart;
                ResetTransform(*node, model);
            }
        }
    }
    std::int32_t childOwnsTransform = 0;
    if (model->child)
        childOwnsTransform = ChildOwnsTransform(model->transformOverride, model->child);
    if (model->transformOverride && !childOwnsTransform)
        matrix_stack::LoadMatrix(model->transformOverride);
    else {
        Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
        if (model->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(model->rotation.z);
        if (model->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(model->rotation.y);
        if (model->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(model->rotation.x);
    }
    if (model->child) {
        SaveParentTransforms(model->child);
        ProcessNodeTree(model->child);
    }
    if (model->sibling) {
        PopCurrentMatrix(1);
        ProcessNodeTree(model->sibling);
    }
}

} // namespace

#else

namespace {

// @verified - p
void ProcessNodeTree(MT7Node *model, Character *character) {
    while (model) {
        if (model->runtimeSibling)
            Matrix4x4_Push();
        OsageNode *const node = static_cast<OsageNode *>(model->taskData);
        if (node && node->token.name == OSAGE_COMPONENT.name) {
            if (node->kind >= 0) {
                if (shenIsGamePaused())
                    SetMT7NodeTransformOverride(*model, &node->transform);
                else {
                    node->updateFlags = (node->updateFlags & OsageUpdateWarmupMask) | (2 * (node->updateFlags & OsageUpdatePreviousFrames));
                    randomOscillation = {IncrementSharedPrngFloat() * 5 - 2.5f, IncrementSharedPrngFloat() * 5 - 2.5f};
                    SetMT7NodeTransformOverride(*model, nullptr);
                    switch (node->kind) {
                    case 120:
                    case 133:
                        if (node->owner->distanceLimit > hairDistance)
                            StrandSpringTransform(*node);
                        break;
                    case 121:
                    case 132:
                        if (hairDistance < 16)
                            HairStrandTransform(*node, character);
                        break;
                    case 124:
                        if (hairDistance < 16)
                            DelayedWeatherTransform(*node);
                        break;
                    case 125:
                        if (hairDistance < 64)
                            ObjectWindTransform(*node);
                        break;
                    case 126:
                        if (hairDistance < 16)
                            WeatherWindTransform(*node);
                        break;
                    case 127:
                        if (hairDistance < 64)
                            SimpleWindTransform(*node, true);
                        break;
                    case 128:
                        if (hairDistance < 64)
                            SimpleWindTransform(*node, false);
                        break;
                    case 141:
                        if (hairDistance < 16)
                            OscillationTransform(*node);
                        break;
                    default:
                        break;
                    }
                    ++node->owner->flags;
                    if (!(node->linkFlags & 0x1f) && model->runtimeParent && !GetMT7NodeTransformOverride(*model->runtimeParent))
                        SetMT7NodeTransformOverride(*model->runtimeParent, &node->parentTransform);
                    if (GetMT7NodeTransformOverride(*model)) {
                        if (node->updateFlags & OsageUpdateWarmupMask)
                            node->updateFlags += OsageUpdateWarmupStep;
                    } else {
                        node->updateFlags |= OsageUpdateWarmupStart;
                        ResetTransform(*node);
                    }
                }
            } else {
                node->kind = static_cast<std::int16_t>(model->id);
                switch (node->kind) {
                case 120:
                case 133:
                    node->facingAngle = Facing(*node, 0, 0x8000);
                    break;
                case 121:
                case 124:
                case 126:
                case 132:
                    node->facingAngle = Facing(*node, 0);
                    break;
                case 122:
                    node->facingAngle = Facing(*node, 2, 0x8000);
                    break;
                case 123:
                case 125:
                    node->facingAngle = Facing(*node, 1, 0x8000);
                    break;
                case 127:
                case 128:
                case 130:
                    node->facingAngle = Facing(*node, 2);
                    break;
                default:
                    node->facingAngle = Facing(*node, 1);
                    break;
                }
                Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
                node->current = Translation();
                node->previous = node->current;
                node->euler = model->rotation;
                ViewSpaceFromEuler(*node);
                if (model->runtimeParent && !GetMT7NodeTransformOverride(*model->runtimeParent))
                    SetMT7NodeTransformOverride(*model->runtimeParent, &node->parentTransform);
                node->endpoint = EndpointPosition(*node);
            }
        }
        const Matrix4x4 *const override = GetMT7NodeTransformOverride(*model);
        if (!override || (model->runtimeChild && ChildOwnsTransform(override, model->runtimeChild))) {
            Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
            Rotate(model->rotation);
        } else
            Matrix4x4::SetActiveTransform(override);
        if (model->runtimeChild) {
            SaveParentTransforms(model->runtimeChild);
            ProcessNodeTree(model->runtimeChild, character);
        }
        if (!model->runtimeSibling)
            break;
        Matrix4x4::Pop(1);
        model = model->runtimeSibling;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void CollisionSurfaces(const OsageCollisionPoint *points, MotionWork *motion, Vector4 *output) {
    for (std::int32_t i = 0; i < 28; ++i) {
        Matrix4x4::SetActiveTransform(mtwkGetBoneTransform(motion, collisionBones[i]));
        Matrix4x4::preTranslate(points[i].x * 0.0099999998f, points[i].y * 0.0099999998f, points[i].z * 0.0099999998f);
        const Vector3 position = Translation();
        output[i] = {position.x, position.y, position.z, points[i].radius * 0.0099999998f};
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetCharacterOsageMode(Character *character, std::uint8_t mode) {
    if (character->model)
        if (OsageWork *const work = FindOsage(character))
            work->mode = mode;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void SaveParentTransforms(MT7Node *node) {
    OsageNode *const osage = static_cast<OsageNode *>(node->taskData);
    if (osage && osage->token == OSAGE_COMPONENT)
        matrix_stack::GetMatrix(&osage->parentTransform);
    if (node->sibling)
        SaveParentTransforms(node->sibling);
}

} // namespace

#else

namespace {

// @verified - p
void SaveParentTransforms(MT7Node *node) {
    for (; node; node = node->runtimeSibling) {
        OsageNode *const osage = static_cast<OsageNode *>(node->taskData);
        if (osage && osage->token.name == OSAGE_COMPONENT.name)
            Matrix4x4::GetTransformMatrix(&osage->parentTransform);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t ChildOwnsTransform(const Matrix4x4 *transform, MT7Node *child) {
    OsageNode *const node = static_cast<OsageNode *>(child->taskData);
    std::int32_t owns = 0;
    if (node && node->token == OSAGE_COMPONENT && &node->parentTransform == transform)
        owns = 1;
    else if (child->sibling)
        owns = ChildOwnsTransform(transform, child->sibling);
    return owns;
}

} // namespace

#else

namespace {

// @verified - p
bool ChildOwnsTransform(const Matrix4x4 *transform, MT7Node *child) {
    for (; child; child = child->runtimeSibling) {
        OsageNode *const node = static_cast<OsageNode *>(child->taskData);
        if (node && node->token.name == OSAGE_COMPONENT.name && &node->parentTransform == transform)
            return true;
    }
    return false;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void StrandSpringTransform(OsageNode &node, MT7Node *model) {
    Vector3 endpoint, oldOrigin, origin, direction, end, displacement;
    MT7Node *endpointNode;
    OsageWork *work;
    const OsageParameters *parameters;
    const OsageSpringParameters *spring, *secondary;
    std::int32_t mode, directional;
    float length, radius, gravity, damping, stiffness, oldDistance, amount, bound, step;
    std::int32_t mask;
    OsageNode *link;
    work = node.owner;
    endpointNode = model->child;
    if (!endpointNode)
        endpointNode = model;
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[0] + 0x8000;
    parameters = work->parameters;
    secondary = parameters->secondary;
    if (secondary && parameters->secondaryDepth <= (static_cast<std::int8_t>(node.linkFlags) & 0x1f) && parameters->secondaryDepth)
        spring = secondary;
    else
        spring = parameters->primary;
    mode = static_cast<std::int16_t>(spring->mode);
    length = spring->length;
    radius = spring->radius;
    damping = spring->damping;
    stiffness = spring->stiffness;
    gravity = spring->gravity;
    mask = defaultCollisionMask;
    switch (work->character) {
    case SHEN_NAME4('S', 'Y', 'E', '_'):
    case SHEN_NAME4('S', 'Y', 'P', '_'):
        mode = (node.linkFlags & 0x1f) ? 0 : 2;
        length = 1.0f;
        radius = 0.002f;
        gravity = 0.015f;
        damping = 0.7f;
        directional = 0;
        break;
    case SHEN_NAME4('S', 'I', 'N', '_'):
        length = 1.5f;
        radius = 0.005f;
        gravity = 0.013f;
        damping = 0.65f;
        mode = 0;
        directional = 1;
        if (!(node.linkFlags & 0x1f)) {
            mode = 2;
            length = 2.5f;
            radius = 0.001f;
        }
        break;
    case SHEN_NAME4('R', 'E', 'N', '_'):
        if (!(node.linkFlags & 0x1f)) {
            length = 1.0f;
            radius = 0.01f;
            mode = 1;
        } else {
            length = 1.1f;
            radius = 0.002f;
            mode = 0;
        }
        gravity = 0.02f;
        damping = 0.6f;
        directional = 0;
        break;
    case SHEN_NAME4('M', 'M', 'H', '_'):
        length = 1.0f;
        mode = 0;
        radius = 0.01f;
        gravity = 0.01f;
        damping = 0.65f;
        directional = 0;
        break;
    case SHEN_NAME4('X', 'G', 'H', '_'):
        length = 1.0f;
        mode = 0;
        radius = 0.02f;
        gravity = 0.01f;
        damping = 0.65f;
        directional = 0;
        break;
    case SHEN_NAME4('B', 'K', 'N', '_'):
        mask = 31;
        directional = 0;
        break;
    case SHEN_NAME4('J', 'O', 'P', '_'):
        mask = 511;
        directional = 0;
        break;
    default:
        directional = 0;
        break;
    }
    if (!node.parent)
        oldDistance = am2Vector3Distance(&node.previous, &node.current);
    oldOrigin = node.current;
    BeginResponse(node, model);
    origin = node.current;
    if (!node.parent && am2Vector3Distance(&origin, &oldOrigin) - oldDistance > 0.25f)
        for (link = &node; link; link = link->child)
            link->updateFlags = (link->updateFlags & OsageUpdateHistoryMask) | OsageUpdateWarmupStart;
    amount = -1.0f;
    if (node.owner->mode == 2)
        amount = 0.05f;
    if (amount > 0) {
        std::int32_t i;
        for (i = 8; i > 0; --i) {
            node.offsets[i].x = node.offsets[i - 1].x;
            node.offsets[i].y = node.offsets[i - 1].y;
        }
        i = 9;
        if (node.linkFlags & 0x1f) {
            link = node.parent;
            node.offsets[0].x = link->offsets[3].x;
            node.offsets[0].y = link->offsets[3].y;
        } else {
            bound = amount * 0.25f;
            step = bound * 0.066666f;
            if (node.offsets[i].x < 0)
                step = -step;
            node.offsets[0].x = node.offsets[i].x + step + node.offsets[0].x;
            if (node.offsets[0].x > bound) {
                node.offsets[i].x = node.offsets[i].x < 0 ? node.offsets[i].x : -node.offsets[i].x;
                if (std::fabs(node.offsets[i].x) < 1.1920929e-07f)
                    node.offsets[i].x = -step;
            } else if (node.offsets[0].x < -bound) {
                node.offsets[i].x = dc::sdk::RandomFloat() * (step < 0 ? -step : step);
                if (std::fabs(node.offsets[i].x) < 1.1920929e-07f)
                    node.offsets[i].x = step;
            }
            bound = amount * 0.33333f;
            step = bound * 0.1f;
            if (node.offsets[i].y < 0)
                step = -step;
            node.offsets[0].y = node.offsets[i].y + step + node.offsets[0].y;
            if (node.offsets[0].y > bound) {
                node.offsets[i].y = node.offsets[i].y < 0 ? node.offsets[i].y : -node.offsets[i].y;
                if (std::fabs(node.offsets[i].y) < 1.1920929e-07f)
                    node.offsets[i].y = -step;
            } else if (node.offsets[0].y < -bound) {
                node.offsets[i].y = dc::sdk::RandomFloat() * (step < 0 ? -step : step);
                if (std::fabs(node.offsets[i].y) < 1.1920929e-07f)
                    node.offsets[i].y = step;
            }
        }
        bound = CalculateFixedSine(-node.facingAngle);
        step = CalculateFixedCosine(-node.facingAngle);
        origin.x += node.offsets[0].x * step - bound * -amount;
        origin.y += node.offsets[0].y;
        origin.z += node.offsets[0].x * bound + step * -amount;
    }
    displacement = work->wind;
    amount = Noise(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f)));
    origin.x += displacement.x * 0.01f * amount;
    origin.z += displacement.z * 0.01f * amount;
    node.pitchPhase += 0.15f;
    if (mode == 1) {
        node.euler.x = model->rotation.x;
        node.euler.y = model->rotation.y;
        node.euler.z = model->rotation.z;
        ViewSpaceFromEuler(model, node);
        return;
    }
    if (mode == 2) {
        MotionMagnitude(node);
        amount = node.motionMagnitude * 12.0f;
        if (amount > 50.0f)
            amount = 50.0f;
        Spring(node, 5.0f, 0.0f, amount, 0.0f);
        AdvancePhase(node, 20.0f, 0.0f);
        amount = (CalculateFixedSine(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f))) * 0.1f + 1.0f) * node.pitchScale;
        if (amount < 0.0f)
            amount = 0.0f;
        node.euler.x = static_cast<std::uint16_t>(static_cast<std::int32_t>(amount * 8192.0f / 45.0f));
        node.euler.y = node.facingAngle;
        node.euler.z = 0;
        PlaceSpringEndpoint(node, model, endpointNode, amount, length, &endpoint);
        if (!ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, radius))
            return;
        while (mode) {
            PlaceSpringEndpoint(node, model, endpointNode, amount, length, &endpoint);
            if (ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, radius)) {
                amount += 10.0f;
                if (amount > 120.0f) {
                    amount = 120.0f;
                    mode = 0;
                }
                node.updateFlags |= OsageUpdateCurrentFrame;
            } else
                mode = 0;
        }
        while (!mode) {
            PlaceSpringEndpoint(node, model, endpointNode, amount, length, &endpoint);
            if (ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, radius)) {
                mode = 2;
                node.updateFlags |= OsageUpdateCurrentFrame;
            } else {
                amount -= 1.0f;
                if (amount < 0.0f) {
                    amount = 0.0f;
                    mode = 2;
                }
            }
        }
        while (mode) {
            PlaceSpringEndpoint(node, model, endpointNode, amount, length, &endpoint);
            if (ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, radius)) {
                amount += 0.1f;
                if (amount > 120.0f) {
                    amount = 120.0f;
                    mode = 0;
                }
            } else
                mode = 0;
        }
        return;
    }
    if (work->mode == 1)
        directional = 2;
    if (directional) {
        MotionWork *const motion = charFindMotionwork(node.owner->owner);
        switch (directional) {
        case 1:
            mtwkGetBonePosition(motion, static_cast<MotionBoneId>(3), &direction);
            mtwkGetBonePosition(motion, static_cast<MotionBoneId>(5), &end);
            break;
        case 2:
            mtwkGetBonePosition(motion, static_cast<MotionBoneId>(22), &direction);
            mtwkGetBonePosition(motion, static_cast<MotionBoneId>(3), &end);
            break;
        }
        direction.x -= end.x;
        direction.y -= end.y;
        direction.z -= end.z;
        am2Vector3NormalizeInPlace(&direction);
        if (node.updateFlags & OsageUpdateWarmupMask) {
            endpoint.x = (node.current.x - oldOrigin.x) + direction.x;
            endpoint.y = ((node.current.y - oldOrigin.y) + direction.y) - 0.1f;
            endpoint.z = (node.current.z - oldOrigin.z) + direction.z;
        } else {
            endpoint.x = (node.endpoint.x - node.previous.x) * damping + direction.x * gravity * motionScale;
            endpoint.y = (node.endpoint.y - node.previous.y) + direction.y * gravity * motionScale;
            endpoint.z = (node.endpoint.z - node.previous.z) * damping + direction.z * gravity * motionScale;
        }
    } else if (node.updateFlags & OsageUpdateWarmupMask) {
        endpoint.x = node.current.x - oldOrigin.x;
        endpoint.y = (node.current.y - oldOrigin.y) - 0.1f;
        endpoint.z = node.current.z - oldOrigin.z;
    } else {
        endpoint.x = (node.endpoint.x - node.previous.x) * damping + (node.current.x - oldOrigin.x) * stiffness;
        endpoint.y = (node.endpoint.y - node.previous.y) * damping - motionScale * gravity + (node.current.y - oldOrigin.y) * stiffness;
        endpoint.z = (node.endpoint.z - node.previous.z) * damping + (node.current.z - oldOrigin.z) * stiffness;
    }
    node.previous = node.endpoint;
    endpoint.x += node.endpoint.x;
    endpoint.y += node.endpoint.y;
    endpoint.z += node.endpoint.z;
    if (groundHeight < -10000.0f)
        groundHeight = FindFieldIntersectionHeight(&endpoint);
    if (endpoint.y < groundHeight) {
        endpoint.y = groundHeight;
        node.updateFlags |= OsageUpdateCurrentFrame;
    }
    length *= std::sqrt(endpointNode->position.x * endpointNode->position.x + endpointNode->position.y * endpointNode->position.y +
        endpointNode->position.z * endpointNode->position.z);
    ClampDistance(origin, endpoint, length);
    if (node.linkFlags & 0x1f)
        ParentConstraint(node.current, endpoint, node.parent->current, length);
    if (work->character == SHEN_NAME4('R', 'E', 'N', '_') && node.child && !(node.updateFlags & OsageUpdateWarmupMask)) {
        const float dx = node.child->endpoint.x - endpoint.x, dy = node.child->endpoint.y - endpoint.y, dz = node.child->endpoint.z - endpoint.z;
        amount = dx * dx + dy * dy + dz * dz;
        amount = ((node.current.x - endpoint.x) * dx + (node.current.y - endpoint.y) * dy + (node.current.z - endpoint.z) * dz) / amount;
        if (amount > -0.6f) {
            endpoint.x += dx * 0.2f;
            endpoint.y += dy * 0.2f;
            endpoint.z += dz * 0.2f;
            ClampDistance(node.current, endpoint, length);
        }
    }
    std::int16_t collision = ProjectEnabled(work->collisionSurfaces, node.current, endpoint, length, radius, mask);
    if (useSecondarySurfaces) {
        collision |= ProjectSurface(secondarySurfaces, endpoint, 2, radius);
        ClampDistance(node.current, endpoint, length);
        collision |= ProjectSurface(secondarySurfaces + 2, endpoint, 2, radius);
        ClampDistance(node.current, endpoint, length);
    }
    if (collision)
        node.updateFlags |= OsageUpdateCurrentFrame;
    if (node.updateFlags & OsageUpdateWarmupMask)
        node.previous = endpoint;
    node.endpoint = endpoint;
    matrix_stack::UnitCurrentMatrix();
    AlignEndpoint(node.current, endpoint);
    Matrix4x4::RotateAroundY_Wrapper(node.facingAngle);
    if (node.kind == 133 && !(node.linkFlags & 0x1f)) {
        FacialDisplacement(work->owner, &displacement);
        switch (work->character) {
        case SHEN_NAME4('B', 'K', 'N', '_'): {
            const float scale = 1.0f - work->flags * 0.0294f;
            displacement.x *= scale;
            displacement.y *= scale;
            displacement.z *= scale;
            break;
        }
        case SHEN_NAME4('S', 'F', '2', '_'):
            displacement.x = displacement.y = displacement.z = 0.0f;
            break;
        }
        Matrix4x4::preTranslate(displacement.x, displacement.y, -displacement.z);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&node.parentTransform);
        Matrix4x4::preTranslate(displacement.x, displacement.y, -displacement.z);
        model->parent->transformOverride = &node.parentTransform;
        matrix_stack::GetMatrix(&node.parentTransform);
        PopCurrentMatrix(1);
    }
    model->transformOverride = &node.transform;
    matrix_stack::GetMatrix(&node.transform);
}

} // namespace

#else

namespace {

// @verified - p
void StrandSpringTransform(OsageNode &node) {
    OsageWork &work = *node.owner;
    node.facingAngle = Facing(node, 0, 0x8000);
    const OsageSpringParameters &parameters = SpringParameters(node);
    float oldDistance = 0;
    if (!node.parent)
        oldDistance = GetEuclideanDistance(&node.previous, &node.current);
    const Vector3 oldOrigin = node.current;
    const Vector3 &local = node.node->position;
    Matrix4x4::preTranslate(local.x, local.y, local.z);
    node.current = Translation();
    Vector3 origin = node.current;
    if (!node.parent && GetEuclideanDistance(&origin, &oldOrigin) - oldDistance > 0.25f)
        for (OsageNode *link = &node; link; link = link->child)
            link->updateFlags = (link->updateFlags & OsageUpdateHistoryMask) | OsageUpdateWarmupStart;
    if (work.mode == 2) {
        ShiftOffsets(node);
        if (node.linkFlags & 0x1f)
            node.offsets[0] = node.parent->offsets[3];
        else {
            SwayOffset(node.offsets[0].x, node.offsets[9].x, 0.00083332503f, 0.0125f);
            SwayOffset(node.offsets[0].y, node.offsets[9].y, 0.0016666501f, 0.0166665f);
        }
        const float sine = Sin(-static_cast<std::int32_t>(node.facingAngle)), cosine = Cos(-static_cast<std::int32_t>(node.facingAngle));
        origin.y += node.offsets[0].y;
        origin.x += node.offsets[0].x * cosine - sine * -0.050000001f;
        origin.z += node.offsets[0].x * sine - cosine * 0.050000001f;
    }
    if (node.kind == 120) {
        const float amount = Noise(static_cast<std::int16_t>(Degrees(node.pitchPhase)));
        origin.x += (work.wind.x * 0.0099999998f) * amount;
        origin.z += (work.wind.z * 0.0099999998f) * amount;
        node.pitchPhase += 0.15000001f;
    }
    if (parameters.mode == 1) {
        node.euler = node.node->rotation;
        ViewSpaceFromEuler(node);
        return;
    }
    MT7Node *const endpointNode = node.node->runtimeChild ? node.node->runtimeChild : node.node;
    if (parameters.mode == 2) {
        const float magnitude = MotionMagnitude(node);
        Spring(node, 5, 0, std::fmin(50.0f, node.motionMagnitude * 12), 0);
        (void)magnitude;
        AdvancePhase(node, 20, 0);
        float pitch = std::fmax(0.0f, (PhaseSin(node.pitchPhase) * 0.1f + 1) * node.pitchScale);
        node.euler = {Degrees(pitch), static_cast<std::int32_t>(node.facingAngle), 0};
        BuildTransform(node);
        const Vector3 endpointLocal{
            parameters.length * endpointNode->position.x, parameters.length * endpointNode->position.y, parameters.length * endpointNode->position.z};
        const auto project = [&]() {
            Matrix4x4_Push();
            Matrix4x4::preTranslate(endpointLocal.x, endpointLocal.y, endpointLocal.z);
            Vector3 endpoint = Translation();
            Matrix4x4::Pop(1);
            return ProjectSurface(work.collisionSurfaces.data() + 4, endpoint, 4, parameters.radius);
        };
        if (project()) {
            bool search = true;
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                if (!project())
                    break;
                pitch += 10;
                if (pitch > 120) {
                    pitch = 120;
                    search = false;
                }
                node.updateFlags |= OsageUpdateCurrentFrame;
            } while (search);
            bool settled = true;
            for (;;) {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                if (project())
                    break;
                pitch -= 1;
                if (pitch < 0) {
                    pitch = 0;
                    settled = false;
                    break;
                }
            }
            if (settled)
                node.updateFlags |= OsageUpdateCurrentFrame;
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                if (!project())
                    break;
                pitch += 0.1f;
            } while (pitch <= 120);
        }
        return;
    }
    std::int32_t directional = work.character.name == SHEN_NAME(SIN_) ? 1 : 0;
    if (work.mode == 1)
        directional = 2;
    Vector3 direction{};
    if (directional) {
        MotionWork *const motion = FindMotionWork(work.owner);
        Vector3 start{}, end{};
        mtwkGetBonePosition(motion, static_cast<MotionBoneId>(directional == 1 ? 3 : 22), &start);
        mtwkGetBonePosition(motion, static_cast<MotionBoneId>(directional == 1 ? 5 : 3), &end);
        direction = {start.x - end.x, start.y - end.y, start.z - end.z};
        Vector3::NormalizeInPlace(direction);
    }
    Vector3 movement{};
    if (node.updateFlags & OsageUpdateWarmupMask) {
        if (directional)
            movement = {(node.current.x - oldOrigin.x) + direction.x, ((node.current.y - oldOrigin.y) + direction.y) - 0.1f,
                (node.current.z - oldOrigin.z) + direction.z};
        else
            movement = {node.current.x - oldOrigin.x, node.current.y - oldOrigin.y - 0.1f, node.current.z - oldOrigin.z};
    } else {
        movement = {(node.endpoint.x - node.previous.x) * parameters.damping, (node.endpoint.y - node.previous.y) * parameters.damping,
            (node.endpoint.z - node.previous.z) * parameters.damping};
        if (directional) {
            movement.x += (direction.x * parameters.gravity) * motionScale;
            movement.y = (node.endpoint.y - node.previous.y) + ((direction.y * parameters.gravity) * motionScale);
            movement.z += (direction.z * parameters.gravity) * motionScale;
        } else {
            movement.x += (node.current.x - oldOrigin.x) * parameters.stiffness;
            movement.y = movement.y - motionScale * parameters.gravity + (node.current.y - oldOrigin.y) * parameters.stiffness;
            movement.z += (node.current.z - oldOrigin.z) * parameters.stiffness;
        }
    }
    Vector3 endpoint{movement.x + node.endpoint.x, movement.y + node.endpoint.y, movement.z + node.endpoint.z};
    node.previous = node.endpoint;
    if (groundHeight < -10000)
        groundHeight = shenFindIntersectionWithScene(endpoint, -10000);
    if (groundHeight > endpoint.y) {
        endpoint.y = groundHeight;
        node.updateFlags |= OsageUpdateCurrentFrame;
    }
    const Vector3 &endLocal = endpointNode->position;
    const float length = parameters.length * std::sqrt(((endLocal.x * endLocal.x) + (endLocal.y * endLocal.y)) + (endLocal.z * endLocal.z));
    ClampDistance(origin, endpoint, length);
    if (node.linkFlags & 0x1f)
        ParentConstraint(node.current, endpoint, node.parent->current, length);
    if (work.character.name == SHEN_NAME(REN_) && node.child && !(node.updateFlags & OsageUpdateWarmupMask)) {
        const Vector3 delta{node.child->endpoint.x - endpoint.x, node.child->endpoint.y - endpoint.y, node.child->endpoint.z - endpoint.z};
        const float ratio = (((node.current.y - endpoint.y) * delta.y + (node.current.x - endpoint.x) * delta.x) + (node.current.z - endpoint.z) * delta.z) /
            ((delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z);
        if (ratio > -0.6f) {
            endpoint.x += delta.x * 0.2f;
            endpoint.y += delta.y * 0.2f;
            endpoint.z += delta.z * 0.2f;
            ClampDistance(node.current, endpoint, length);
        }
    }
    const std::uint16_t mask = work.character.name == SHEN_NAME(BKN_) ? 31 : work.character.name == SHEN_NAME(JOP_) ? 511 : 0xffff;
    bool collision = ProjectEnabled(work, node.current, endpoint, length, parameters.radius, mask);
    if (useSecondarySurfaces) {
        collision |= ProjectSurface(secondarySurfaces, endpoint, 2, parameters.radius);
        ClampDistance(node.current, endpoint, length);
        collision |= ProjectSurface(secondarySurfaces + 2, endpoint, 2, parameters.radius);
        ClampDistance(node.current, endpoint, length);
    }
    if (collision)
        node.updateFlags |= OsageUpdateCurrentFrame;
    if (node.updateFlags & OsageUpdateWarmupMask)
        node.previous = endpoint;
    const auto invalidCoordinate = [](float value) {
        const std::uint32_t bits = std::bit_cast<std::uint32_t>(value);
        return bits - 0x00800000U > 0x7effffffU && bits + 0x7f800000U > 0x7effffffU && bits != 0 && bits != 0x80000000U;
    };
    if (invalidCoordinate(endpoint.x) || invalidCoordinate(endpoint.z))
        endpoint = node.current;
    node.endpoint = endpoint;
    Matrix4x4::SetPrePassToTempTransform();
    AlignEndpoint(node.current, endpoint);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.facingAngle));
    if (!(node.linkFlags & 0x1f) && node.kind == 133) {
        Vector3 displacement = FacialDisplacement(work.owner);
        if (work.character.name == SHEN_NAME(SF2_))
            displacement = {};
        Matrix4x4::preTranslate(displacement.x, displacement.y, -displacement.z);
        if (node.node->runtimeParent && static_cast<std::uint16_t>(node.node->runtimeParent->id) == 0xffff) {
            Matrix4x4_Push(&node.parentTransform);
            Matrix4x4::preTranslate(displacement.x, displacement.y, -displacement.z);
            node.parentTransform = workTransformMatrix;
            SetMT7NodeTransformOverride(*node.node->runtimeParent, &node.parentTransform);
            Matrix4x4::Pop(1);
        }
    }
    BindTransform(node);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float hairSpringRate;
float hairPitchRate, hairYawRate;
float hairPitchTarget, hairYawTarget;
float hairPitchPhaseRate, hairYawPhaseRate;

void HairStrandTransform(OsageNode &node, MT7Node *model) {
    OsageWork *work = node.owner;
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[0];
    BeginResponse(node, model);
    for (std::int16_t i = 8; i > 0; i--) {
        node.offsets[i].x = node.offsets[i - 1].x;
        node.offsets[i].y = node.offsets[i - 1].y;
    }
    do {
        hairPitchTarget = -1.0f;
    } while (0);
    const float wind = work->windSpeedScaled;
    hairPitchTarget = hairYawTarget = MotionMagnitude(node);
    if (node.owner->character == SHEN_NAME4('S', 'R', 'Y', '_') && (model->parent->id == 6 || model->parent->id == 11)) {
        if (hairPitchTarget > 5.0f)
            hairPitchTarget = 5.0f;
    }
    if (work->character == SHEN_NAME4('T', 'N', 'P', '_')) {
        hairSpringRate = 0.1f;
        hairPitchPhaseRate = 0.0f;
        hairYawPhaseRate = -30.0f;
    } else {
        hairSpringRate = 0.2f;
        hairPitchPhaseRate = 20.0f;
        hairYawPhaseRate = -30.0f;
        hairPitchTarget += hairPitchTarget;
    }
    if (wind > hairPitchTarget)
        hairPitchTarget = wind;
    hairYawTarget = hairPitchTarget;
    Spring(node, hairSpringRate, hairSpringRate, hairPitchTarget, hairYawTarget);
    hairPitchRate = hairPitchPhaseRate;
    hairYawRate = hairYawPhaseRate;
    AdvancePhase(node, hairPitchRate, hairYawRate);
    node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
    node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
    node.euler.x = (static_cast<std::int32_t>(node.offsets[0].x * 8192.0f / 45.0f) & 0xffff) + model->rotation.x;
    node.euler.y = model->rotation.y;
    node.euler.z = (static_cast<std::int32_t>(node.offsets[0].y * 8192.0f / 45.0f) & 0xffff) + model->rotation.z;
    if (!(node.linkFlags & 0x1f) && node.kind == 132) {
        Vector3 displacement;
        FacialDisplacement(work->owner, &displacement);
        Matrix4x4::preTranslate(-displacement.z, displacement.x, displacement.y);
    }
    ViewSpaceFromEuler(model, node);
}

} // namespace

#else

namespace {

// @verified - p
void HairStrandTransform(OsageNode &node, Character *character) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name scenarioName = scenGetCharaName(control ? control->scenarioTask : nullptr, character);
    BeginResponse(node, 0);
    ShiftOffsets(node);
    float amount = MotionMagnitude(node);
    if (node.owner->character.name ==
        SHEN_NAME(SRY_) && node.node->runtimeParent && (node.node->runtimeParent->id == 6 || node.node->runtimeParent->id == 11) && amount > 5)
        amount = 5;
    const bool gentle = node.owner->character.name == SHEN_NAME(TNP_);
    const float rate = gentle ? 0.1f : 0.2f;
    const float pitch = gentle ? 0.0f : 20.0f;
    if (!gentle)
        amount += amount;
    if (node.owner->windSpeedNext > amount)
        amount = node.owner->windSpeedNext;
    Spring(node, rate, rate, amount, amount);
    AdvancePhase(node, pitch, -30);
    Offsets(node);
    node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y, node.node->rotation.z + Degrees(node.offsets[0].y)};
    if (!(node.linkFlags & 0x1f) && node.kind == 132) {
        const Vector3 displacement = FacialDisplacement(node.owner->owner);
        Matrix4x4::preTranslate(-displacement.z, displacement.x, displacement.y);
    }
    if (scenarioName.name == SHEN_NAME(SGT_))
        Matrix4x4::preTranslate(0, node.node->runtimeChild ? 0 : 0.035f, node.node->runtimeChild ? -0.0080000004f : -0.0049999999f);
    ViewSpaceFromEuler(node);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ObjectWindTransform(OsageNode &node, MT7Node *model) {
    MT7Node *endpointNode = model->child;
    OsageWork *work = node.owner;
    if (!endpointNode)
        endpointNode = model;
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[1] + 0x8000;
    BeginResponse(node, model);
    float magnitude = MotionMagnitude(node);
    float value = magnitude;
    std::int32_t limit;
    float margin;
    if (work->character == SHEN_NAME4('R', 'E', 'N', '_')) {
        limit = 4;
        if (node.linkFlags & 0x1f)
            margin = 0.0099999998f;
        else
            margin = 0.001f;
    } else {
        limit = 4;
        margin = 0.0099999998f;
    }
    const std::int32_t depth = node.linkFlags & 0x1f;
    if (depth < limit) {
        value *= static_cast<char>(depth) * 2.5f;
        magnitude *= static_cast<char>(depth) * 2.0f;
        Spring(node, 0.2f, 0.2f, value, magnitude);
        if (!(node.linkFlags & 0x1f))
            AdvancePhase(node, 20, -30);
        else {
            OsageNode *parent = node.parent;
            node.pitchPhase = parent->pitchPhase;
            node.yawPhase = parent->yawPhase;
        }
        node.offsets[0].x = CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff) * node.pitchScale;
        if (node.offsets[0].x > 0)
            node.offsets[0].x = -node.offsets[0].x;
        node.offsets[0].y = CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff) * node.yawScale;
        const float yaw = node.offsets[0].y;
        float length = 1.0f;
        value = node.offsets[0].x;
        node.euler.x = static_cast<std::int32_t>(value * 8192.0f / 45.0f) & 0xffff;
        node.euler.y = node.facingAngle;
        node.euler.z = static_cast<std::int32_t>(yaw * 8192.0f / 45.0f) & 0xffff;
        Vector3 endpoint;
        PlaceSpringEndpoint(node, model, endpointNode, value, length, &endpoint);
        if (ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, margin)) {
            node.updateFlags |= OsageUpdateCurrentFrame;
            std::int32_t searching = 1;
            while (searching) {
                PlaceSpringEndpoint(node, model, endpointNode, value, length, &endpoint);
                if (!ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, margin))
                    searching = 0;
                else {
                    value -= 10;
                    if (value < -120)
                        searching = 0;
                }
            }
            while (!searching) {
                PlaceSpringEndpoint(node, model, endpointNode, value, length, &endpoint);
                if (ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, margin))
                    searching = 1;
                else {
                    value += 1;
                    if (value > 0)
                        searching = 1;
                }
            }
            while (searching) {
                PlaceSpringEndpoint(node, model, endpointNode, value, length, &endpoint);
                if (!ProjectSurface(work->collisionSurfaces + 4, endpoint, 4, margin))
                    searching = 0;
                else {
                    value -= 0.1f;
                    if (value < -120)
                        searching = 0;
                }
            }
        }
    } else {
        if (model->child) {
            if (node.linkFlags & 0x1f) {
                OsageNode *parent = node.parent;
                node.pitchPhase = parent->pitchPhase;
                node.yawPhase = parent->yawPhase;
            }
            node.euler.x = model->rotation.x;
            node.euler.y = model->rotation.y;
            node.euler.z = model->rotation.z;
            ViewSpaceFromEuler(model, node);
        } else {
            value *= 8;
            magnitude *= 8;
            Spring(node, 0.2f, 0.2f, value, magnitude);
            OsageNode *parent = node.parent;
            node.pitchPhase = parent->pitchPhase;
            node.yawPhase = parent->yawPhase;
            node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
            node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
            node.euler.x = (static_cast<std::int32_t>(node.offsets[0].x * 8192.0f / 45.0f) & 0xffff) + model->rotation.x;
            node.euler.y = model->rotation.y;
            node.euler.z = (static_cast<std::int32_t>(node.offsets[0].y * 8192.0f / 45.0f) & 0xffff) + model->rotation.z;
            ViewSpaceFromEuler(model, node);
        }
    }
}

} // namespace

#else

namespace {

// @verified - p
void ObjectWindTransform(OsageNode &node) {
    BeginResponse(node, 1, 0x8000);
    const float magnitude = MotionMagnitude(node);
    const std::int32_t depth = node.linkFlags & 0x1f;
    std::int32_t limit = 4;
    float margin = 0.0099999998f;
    if (node.owner->character.name == SHEN_NAME(REN_)) {
        if (!depth)
            margin = 0.001f;
        if (node.owner->mode == 3)
            limit = 5;
    }
    if (depth >= limit) {
        if (node.node->runtimeChild) {
            if (depth) {
                node.pitchPhase = node.parent->pitchPhase;
                node.yawPhase = node.parent->yawPhase;
            }
            node.euler = node.node->rotation;
        } else {
            Spring(node, 0.2f, 0.2f, magnitude * 8, magnitude * 8);
            node.pitchPhase = node.parent->pitchPhase;
            node.yawPhase = node.parent->yawPhase;
            Offsets(node);
            node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y, node.node->rotation.z + Degrees(node.offsets[0].y)};
        }
        ViewSpaceFromEuler(node);
        return;
    }
    Spring(node, 0.2f, 0.2f, (static_cast<float>(depth) * 2.5f) * magnitude, static_cast<float>(depth + depth) * magnitude);
    if (depth) {
        node.pitchPhase = node.parent->pitchPhase;
        node.yawPhase = node.parent->yawPhase;
    } else
        AdvancePhase(node, 20, -30);
    Offsets(node);
    if (node.offsets[0].x > 0)
        node.offsets[0].x = -node.offsets[0].x;
    float pitch = node.offsets[0].x;
    node.euler = {Degrees(pitch), static_cast<std::int32_t>(node.facingAngle), Degrees(node.offsets[0].y)};
    BuildTransform(node);
    Vector3 endpoint = EndpointPosition(node);
    if (ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin)) {
        node.updateFlags |= OsageUpdateCurrentFrame;
        do {
            node.euler.x = Degrees(pitch);
            BuildTransform(node);
            endpoint = EndpointPosition(node);
            if (!ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin))
                break;
            pitch = static_cast<float>(static_cast<double>(pitch) - 10.0);
        } while (pitch >= -120);
        do {
            node.euler.x = Degrees(pitch);
            BuildTransform(node);
            endpoint = EndpointPosition(node);
            if (ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin))
                break;
            pitch = static_cast<float>(static_cast<double>(pitch) + 1.0);
        } while (pitch <= 0);
        do {
            node.euler.x = Degrees(pitch);
            BuildTransform(node);
            endpoint = EndpointPosition(node);
            if (!ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin))
                break;
            pitch -= 0.1f;
        } while (pitch >= -120);
    }
    if (node.owner->mode == 3) {
        const float height = node.owner->owner ? node.owner->owner->entity.node.position.y - 0.0099999998f : -0.0099999998f;
        if (height > endpoint.y) {
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                endpoint = EndpointPosition(node);
                if (height <= endpoint.y)
                    break;
                pitch -= 10;
            } while (pitch >= -120);
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                endpoint = EndpointPosition(node);
                if (height > endpoint.y)
                    break;
                pitch += 1;
            } while (pitch <= 0);
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void WeatherWindTransform(OsageNode &node, MT7Node *model) {
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[0] + 0x8000;
    BeginResponse(node, model);
    float magnitude = MotionMagnitude(node);
    Vector3 wind;
    const float speed = GetOsageWind(&wind);
    const float amount = speed + speed;
    if (node.linkFlags & 0x1f) {
        magnitude = magnitude * 12 + amount;
        Spring(node, 0.2f, 0.2f, magnitude, magnitude);
        OsageNode *const parent = node.parent;
        node.pitchPhase = parent->pitchPhase;
        node.yawPhase = parent->yawPhase;
        node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
        node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
        node.euler.x = (static_cast<std::int32_t>(node.offsets[0].x * 8192.0f / 45.0f) & 0xffff) + model->rotation.x;
        node.euler.y = model->rotation.y;
        node.euler.z = (static_cast<std::int32_t>(node.offsets[0].y * 8192.0f / 45.0f) & 0xffff) + model->rotation.z;
        ViewSpaceFromEuler(model, node);
    } else {
        magnitude = magnitude * 5 + amount;
        Spring(node, 0.2f, 0.2f, magnitude, magnitude);
        AdvancePhase(node, 20, -30);
        node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
        node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
        node.euler.x = static_cast<std::int32_t>(node.offsets[0].x * 8192.0f / 45.0f) & 0xffff;
        node.euler.y = node.facingAngle;
        node.euler.z = static_cast<std::int32_t>(node.offsets[0].y * 8192.0f / 45.0f) & 0xffff;
        BuildTransform(model, node);
    }
}

} // namespace

#else

namespace {

// @verified - p
void WeatherWindTransform(OsageNode &node) {
    BeginResponse(node, 0, 0x8000);
    const float magnitude = MotionMagnitude(node);
    const float wind = std::fmax(0.5f, GetEffectiveWindSpeed(GetWind()) * 30);
    const std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
    (void)Sin(phase);
    (void)Cos(phase);
    if (node.linkFlags & 0x1f) {
        Spring(node, 0.2f, 0.2f, magnitude * 12 + (wind + wind), magnitude * 12 + (wind + wind));
        node.pitchPhase = node.parent->pitchPhase;
        node.yawPhase = node.parent->yawPhase;
        Offsets(node);
        PublishEulerResponse(node);
    } else {
        Spring(node, 0.2f, 0.2f, magnitude * 5 + (wind + wind), magnitude * 5 + (wind + wind));
        AdvancePhase(node, 20, -30);
        Offsets(node);
        node.euler = {Degrees(node.offsets[0].x), static_cast<std::int32_t>(node.facingAngle), Degrees(node.offsets[0].y)};
        BuildTransform(node);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void DelayedWeatherTransform(OsageNode &node, MT7Node *model) {
    OsageWork *work = node.owner;
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[0];
    BeginResponse(node, model);
    for (std::int16_t i = 8; i > 0; i--) {
        node.offsets[i].x = node.offsets[i - 1].x;
        node.offsets[i].y = node.offsets[i - 1].y;
    }
    if (!(node.linkFlags & 0x1f)) {
        float pitch = MotionMagnitude(node);
        float yaw = pitch;
        Vector3 wind;
        float speed = GetOsageWind(&wind);
        if (work->character == SHEN_NAME4('R', 'E', 'N', '_')) {
            if (1.5f > speed)
                speed = 1.5f;
            yaw += speed;
            yaw *= 0.80000001f;
            pitch += speed;
            pitch *= 1.2f;
        } else {
            yaw += speed;
            pitch += speed;
        }
        Spring(node, 0.2f, 0.2f, yaw, pitch);
        AdvancePhase(node, 10, -15);
        node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
        node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
    } else {
        OsageNode *parent = node.parent;
        node.offsets[0].x = parent->offsets[6].x;
        node.offsets[0].y = parent->offsets[6].y;
        if (work->character == SHEN_NAME4('R', 'E', 'N', '_')) {
            node.offsets[0].x *= 1.2f;
            node.offsets[0].y *= 1.2f;
        }
    }
    node.euler.x = (static_cast<std::int32_t>(node.offsets[0].x * 8192.0f / 45.0f) & 0xffff) + model->rotation.x;
    node.euler.y = model->rotation.y;
    node.euler.z = (static_cast<std::int32_t>(node.offsets[0].y * 8192.0f / 45.0f) & 0xffff) + model->rotation.z;
    ViewSpaceFromEuler(model, node);
}

} // namespace

#else

namespace {

// @verified - p
void DelayedWeatherTransform(OsageNode &node) {
    BeginResponse(node, 0);
    ShiftOffsets(node);
    if (!(node.linkFlags & 0x1f)) {
        const float magnitude = MotionMagnitude(node);
        const float wind = std::fmax(0.5f, GetEffectiveWindSpeed(GetWind()) * 30);
        const std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
        (void)Sin(phase);
        (void)Cos(phase);
        if (node.owner->character.name == SHEN_NAME(REN_)) {
            const float amount = std::fmax(1.5f, wind) + magnitude;
            Spring(node, 0.2f, 0.2f, amount * 0.80000001f, amount * 1.2f);
        } else
            Spring(node, 0.2f, 0.2f, magnitude + wind, magnitude + wind);
        AdvancePhase(node, 10, -15);
        Offsets(node);
    } else {
        node.offsets[0] = node.parent->offsets[6];
        if (node.owner->character.name == SHEN_NAME(REN_)) {
            node.offsets[0].x *= 1.2f;
            node.offsets[0].y *= 1.2f;
        }
    }
    PublishEulerResponse(node);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void OscillationTransform(OsageNode &node, MT7Node *model) {
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[1];
    BeginResponse(node, model);
    float magnitude;
    if (node.updateFlags & OsageUpdateWarmupMask) {
        node.smoothingCountdown = 6;
        node.accumulatedSway = 0;
        node.motionMagnitude = 0;
        magnitude = 0;
        node.previous = node.current;
    } else {
        node.accumulatedSway = (dc::sdk::CosFixed(node.facingAngle) * (node.current.x - node.previous.x) -
                                   CalculateFixedSine(node.facingAngle) * (node.current.z - node.previous.z)) +
            node.accumulatedSway;
        if (--node.smoothingCountdown <= 0) {
            node.smoothingCountdown = 6;
            node.motionMagnitude = node.accumulatedSway * 5;
            node.accumulatedSway = 0;
        }
        magnitude = node.motionMagnitude * inverseMotionScale;
        node.previous = node.current;
    }
    if (node.offsets[1].x > node.current.y) {
        node.offsets[1].x = node.current.y;
        node.offsets[2].x = 0;
        node.offsets[2].y = 1;
    }
    if (node.offsets[3].x < node.current.y) {
        node.offsets[3].x = node.current.y;
        node.offsets[4].x = 0;
        node.offsets[4].y = 1;
    }
    node.offsets[2].x += motionScale;
    node.offsets[4].x += motionScale;
    float direction = 1;
    if (node.offsets[2].x >= 4 && node.offsets[2].y > 0) {
        node.offsets[5].x = node.offsets[3].x - node.offsets[1].x;
        node.offsets[3].x = -10000;
        direction = -1;
        node.pitchPhase = 180;
        node.offsets[2].y = direction;
    }
    if (node.offsets[4].x >= 4 && node.offsets[4].y > 0) {
        node.offsets[5].x = node.offsets[3].x - node.offsets[1].x;
        node.offsets[1].x = 10000;
        direction = -1;
        node.pitchPhase = 0;
        node.offsets[4].y = direction;
    }
    if (direction < 0) {
        node.offsets[6].x = node.offsets[4].x - node.offsets[2].x;
        if (node.offsets[6].x < 0)
            node.offsets[6].x = -node.offsets[6].x;
        if (node.offsets[5].x >= 0 && node.offsets[5].x < 0.3f && node.offsets[6].x >= 3 && node.offsets[6].x < 20) {
            node.offsets[7].x = node.offsets[5].x;
            node.offsets[8].x = node.offsets[6].x;
        } else
            node.offsets[7].x = 0;
    }
    float strength, cap;
    switch (node.owner->character) {
    case SHEN_NAME4('C', 'H', 'J', '_'):
        strength = 0.13f;
        cap = 0.005f;
        break;
    case SHEN_NAME4('S', 'Y', 'E', '_'):
    case SHEN_NAME4('S', 'Y', 'P', '_'):
        strength = 0.05f;
        cap = 0.005f;
        break;
    default:
        strength = 0;
        cap = strength;
        break;
    }
    float amount = strength * node.offsets[7].x;
    if (amount < 0.0005f)
        amount = 0;
    if (amount > cap)
        amount = cap;
    if (amount > node.pitchScale) {
        node.pitchScale += motionScale * 0.001f;
        if (node.pitchScale > amount)
            node.pitchScale = amount;
    } else if (node.pitchScale > amount) {
        node.pitchScale -= motionScale * 0.00050000002f;
        if (node.pitchScale < amount)
            node.pitchScale = amount;
    }
    if (amount > 0) {
        amount = magnitude;
        if (amount < 0)
            amount = -amount;
    }
    amount *= strength;
    if (amount < 0.0005f)
        amount = 0;
    if (amount > cap)
        amount = cap;
    if (amount > node.yawScale) {
        node.yawScale += motionScale * 0.001f;
        if (node.yawScale > amount)
            node.yawScale = amount;
    } else if (node.yawScale > amount) {
        node.yawScale -= motionScale * 0.00050000002f;
        if (node.yawScale < amount)
            node.yawScale = amount;
    }
    amount = node.offsets[8].x + node.offsets[8].x;
    if (amount < 6)
        amount = 12;
    if (amount > 40)
        amount = 40;
    const float phase = motionScale * 360 / amount;
    node.pitchPhase += phase;
    if (node.pitchPhase >= 360)
        node.pitchPhase -= 360;
    node.yawPhase += phase;
    if (node.yawPhase >= 360)
        node.yawPhase -= 360;
    node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
    node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
    node.euler.x = model->rotation.x;
    node.euler.y = model->rotation.y;
    node.euler.z = model->rotation.z;
    if (!(node.linkFlags & 0x1f))
        Matrix4x4::preTranslate(node.offsets[0].x, 0, node.offsets[0].y);
    ViewSpaceFromEuler(model, node);
}

} // namespace

#else

namespace {

// @verified - p
void OscillationTransform(OsageNode &node) {
    BeginResponse(node, 1);
    float magnitude = 0;
    if (node.updateFlags & OsageUpdateWarmupMask) {
        node.smoothingCountdown = 6;
        node.motionMagnitude = node.accumulatedSway = 0;
    } else {
        node.accumulatedSway =
            (Cos(node.facingAngle) * (node.current.x - node.previous.x) - Sin(node.facingAngle) * (node.current.z - node.previous.z)) + node.accumulatedSway;
        if (--node.smoothingCountdown <= 0) {
            node.smoothingCountdown = 6;
            node.motionMagnitude = node.accumulatedSway * 5;
            node.accumulatedSway = 0;
        }
        magnitude = inverseMotionScale * node.motionMagnitude;
    }
    node.previous = node.current;
    if (node.offsets[1].x > node.current.y) {
        node.offsets[1].x = node.current.y;
        node.offsets[2] = {0, 1};
    }
    if (node.current.y > node.offsets[3].x) {
        node.offsets[3].x = node.current.y;
        node.offsets[4] = {0, 1};
    }
    node.offsets[2].x += motionScale;
    node.offsets[4].x += motionScale;
    bool changed = false;
    if (node.offsets[2].x >= 4 && node.offsets[2].y > 0) {
        node.offsets[5].x = node.offsets[3].x - node.offsets[1].x;
        node.offsets[3].x = -10000;
        node.pitchPhase = 180;
        node.offsets[2].y = -1;
        changed = true;
    }
    if (node.offsets[4].x >= 4 && node.offsets[4].y > 0) {
        node.offsets[5].x = node.offsets[3].x - node.offsets[1].x;
        node.offsets[1].x = 10000;
        node.pitchPhase = 0;
        node.offsets[4].y = -1;
        changed = true;
    }
    if (changed) {
        node.offsets[6].x = std::fabs(node.offsets[4].x - node.offsets[2].x);
        if (node.offsets[5].x < 0 || node.offsets[5].x >= 0.3f || node.offsets[6].x < 3 || node.offsets[6].x >= 20)
            node.offsets[7].x = 0;
        else {
            node.offsets[7].x = node.offsets[5].x;
            node.offsets[8].x = node.offsets[6].x;
        }
    }
    float strength = 0, cap = 0;
    if (node.owner->character.name == SHEN_NAME(SYE_) || node.owner->character.name == SHEN_NAME(SYP_)) {
        strength = 0.05f;
        cap = 0.005f;
    } else if (node.owner->character.name == SHEN_NAME(CHJ_)) {
        strength = 0.13f;
        cap = 0.005f;
    }
    float target = strength * node.offsets[7].x;
    if (target < 0.0005f)
        target = 0;
    target = std::fmin(cap, target);
    if (target > node.pitchScale) {
        node.pitchScale += motionScale * 0.001f;
        if (node.pitchScale > target)
            node.pitchScale = target;
    } else if (node.pitchScale > target) {
        node.pitchScale -= motionScale * 0.00050000002f;
        if (node.pitchScale < target)
            node.pitchScale = target;
    }
    float yawTarget = target > 0 ? std::fabs(magnitude) : target;
    yawTarget *= strength;
    if (yawTarget < 0.0005f)
        yawTarget = 0;
    yawTarget = std::fmin(cap, yawTarget);
    if (yawTarget > node.yawScale) {
        node.yawScale += motionScale * 0.001f;
        if (node.yawScale > yawTarget)
            node.yawScale = yawTarget;
    } else if (node.yawScale > yawTarget) {
        node.yawScale -= motionScale * 0.00050000002f;
        if (node.yawScale < yawTarget)
            node.yawScale = yawTarget;
    }
    float duration = node.offsets[8].x + node.offsets[8].x;
    if (duration < 6)
        duration = 12;
    const float phase = motionScale * 360 / std::fmin(40.0f, duration);
    node.pitchPhase += phase;
    if (node.pitchPhase >= 360)
        node.pitchPhase -= 360;
    node.yawPhase += phase;
    if (node.yawPhase >= 360)
        node.yawPhase -= 360;
    Offsets(node);
    node.euler = node.node->rotation;
    if (!(node.linkFlags & 0x1f))
        Matrix4x4::preTranslate(node.offsets[0].x, 0, node.offsets[0].y);
    ViewSpaceFromEuler(node);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SwayOffset(float &position, float &velocity, float acceleration, float bound) {
    float step = velocity < 0 ? -acceleration : acceleration;
    position = (velocity + step) + position;
    if (position > bound) {
        velocity = -std::fabs(velocity);
        if (std::fabs(velocity) < 0.00000011920929f)
            velocity = -step;
    } else if (position < -bound) {
        velocity = IncrementSharedPrngFloat() * std::fabs(step);
        if (std::fabs(velocity) < 0.00000011920929f)
            velocity = step;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void SimpleWindTransform(OsageNode &node, MT7Node *model) {
    OsageWork *const owner = node.owner;
    MT7Node *end = model->child;
    if (!end)
        end = model;
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[2];
    BeginResponse(node, model);
    float magnitude = MotionMagnitude(node);
    const float pitch = magnitude + magnitude;
    magnitude *= 4;
    Spring(node, 0.2f, 0.2f, pitch, magnitude);
    AdvancePhase(node, 10, -15);
    node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
    node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
    Vector3 endpoint;
    Matrix4x4 root;
    matrix_stack::GetMatrix(&root);
    float length = 1.0f;
    float margin = 0.0020000001f;
    bool hit;
    do {
        matrix_stack::LoadMatrix(&root);
        node.euler.x = model->rotation.x;
        node.euler.y = (static_cast<std::int32_t>(node.offsets[0].x * 8192.0f / 45.0f) & 0xffff) + model->rotation.y;
        node.euler.z = (static_cast<std::int32_t>(node.offsets[0].y * 8192.0f / 45.0f) & 0xffff) + model->rotation.z;
        ViewSpaceFromEuler(model, node);
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::preTranslate(length * end->position.x, length * end->position.y, length * end->position.z);
        matrix_stack::GetTranslation(&endpoint);
        PopCurrentMatrix(1);
        hit = ProjectSurface(owner->collisionSurfaces + 26, endpoint, 2, margin);
        if (hit) {
            node.offsets[0].x -= 3;
            if (node.offsets[0].x < -90)
                hit = false;
            node.updateFlags |= OsageUpdateCurrentFrame;
        }
    } while (hit);
}

void SimpleWindTransformNoCollision(OsageNode &node, MT7Node *model) {
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[2];
    BeginResponse(node, model);
    float magnitude = MotionMagnitude(node);
    const float pitch = magnitude + magnitude;
    magnitude *= 4;
    Spring(node, 0.2f, 0.2f, pitch, magnitude);
    AdvancePhase(node, 10, -15);
    node.offsets[0].x = node.pitchScale * CalculateFixedSine(static_cast<std::int32_t>(node.pitchPhase * 8192.0f / 45.0f) & 0xffff);
    node.offsets[0].y = node.yawScale * CalculateFixedSine(static_cast<std::int32_t>(node.yawPhase * 8192.0f / 45.0f) & 0xffff);
    node.euler.x = (static_cast<std::int32_t>(node.offsets[0].x * 8192.0f / 45.0f) & 0xffff) + model->rotation.x;
    node.euler.y = model->rotation.y;
    node.euler.z = (static_cast<std::int32_t>(node.offsets[0].y * 8192.0f / 45.0f) & 0xffff) + model->rotation.z;
    ViewSpaceFromEuler(model, node);
}

} // namespace

#else

namespace {

// @verified - p
void SimpleWindTransform(OsageNode &node, bool collision) {
    BeginResponse(node, 2);
    const float magnitude = MotionMagnitude(node);
    Spring(node, 0.2f, 0.2f, magnitude + magnitude, magnitude * 4);
    AdvancePhase(node, 10, -15);
    Offsets(node);
    if (!collision) {
        PublishEulerResponse(node);
        return;
    }
    const Matrix4x4 root = workTransformMatrix;
    for (;;) {
        Matrix4x4::SetActiveTransform(&root);
        node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y + Degrees(node.offsets[0].y), node.node->rotation.z};
        ViewSpaceFromEuler(node);
        Vector3 endpoint = EndpointPosition(node);
        if (!ProjectSurface(node.owner->collisionSurfaces.data() + 26, endpoint, 2, 0.0020000001f))
            break;
        node.offsets[0].x -= 3;
        node.updateFlags |= OsageUpdateCurrentFrame;
        if (node.offsets[0].x < -90)
            break;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ResetTransform(OsageNode &node, MT7Node *model) {
    if (node.linkFlags & 0x1f)
        node.facingAngle = node.parent->facingAngle;
    else
        node.facingAngle = node.owner->referenceAngles[1];
    BeginResponse(node, model);
    node.euler.x = model->rotation.x;
    node.euler.y = model->rotation.y;
    node.euler.z = model->rotation.z;
    ViewSpaceFromEuler(model, node);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void BeginResponse(OsageNode &node, MT7Node *model) {
    Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
    matrix_stack::GetTranslation(&node.current);
}

} // namespace

#else

namespace {

void BeginResponse(OsageNode &node, std::int32_t index, std::uint32_t extra) {
    node.facingAngle = Facing(node, index, extra);
    const Vector3 &position = node.node->position;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    node.current = Translation();
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void PublishEulerResponse(OsageNode &node) {
    node.euler = OI(node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y, node.node->rotation.z + Degrees(node.offsets[0].y));
    ViewSpaceFromEuler(node.node, node);
}

} // namespace

#else

namespace {

void PublishEulerResponse(OsageNode &node) {
    node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y, node.node->rotation.z + Degrees(node.offsets[0].y)};
    ViewSpaceFromEuler(node);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 EndpointPosition(OsageNode &node) {
    MT7Node *const end = node.node->child ? node.node->child : node.node;
    matrix_stack::Matrix4x4_Push();
    OsageTranslate(end->position.x, end->position.y, end->position.z);
    const Vector3 result = Translation();
    PopCurrentMatrix(1);
    return result;
}

} // namespace

#else

namespace {

Vector3 EndpointPosition(OsageNode &node) {
    MT7Node *const end = node.node->runtimeChild ? node.node->runtimeChild : node.node;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(end->position.x, end->position.y, end->position.z);
    const Vector3 result = Translation();
    Matrix4x4::Pop(1);
    return result;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void ResetTransform(OsageNode &node) {
    BeginResponse(node, 1);
    node.euler = node.node->rotation;
    ViewSpaceFromEuler(node);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float MotionMagnitude(OsageNode &node) {
    float x, y, z;
    if (node.updateFlags & OsageUpdateWarmupMask) {
        node.smoothingCountdown = 6;
        node.accumulatedSway = node.accumulatedForward = node.accumulatedVertical = 0;
        node.motionMagnitude = 0;
        x = 0;
    } else {
        x = node.current.x - node.previous.x;
        y = node.current.y - node.previous.y;
        z = node.current.z - node.previous.z;
        Vector2 sineCosine;
        CalculateFixedSinCos(node.facingAngle, &sineCosine);
        node.accumulatedSway = ((sineCosine.y * x) - (sineCosine.x * z)) + node.accumulatedSway;
        node.accumulatedForward += y;
        node.accumulatedVertical = ((sineCosine.x * x) + (sineCosine.y * z)) + node.accumulatedVertical;
        if (--node.smoothingCountdown <= 0) {
            node.smoothingCountdown = 6;
            x = node.accumulatedSway * 5;
            y = node.accumulatedForward * 5;
            z = node.accumulatedVertical * 5;
            node.motionMagnitude = NativeSqrt(((x * x) + (y * y)) + (z * z));
            node.accumulatedSway = node.accumulatedForward = node.accumulatedVertical = 0;
        }
        x = node.motionMagnitude * inverseMotionScale;
    }
    node.previous = node.current;
    return x;
}

} // namespace

#else

namespace {

// @verified - p
float MotionMagnitude(OsageNode &node) {
    float result = 0;
    if (node.updateFlags & OsageUpdateWarmupMask) {
        node.smoothingCountdown = 6;
        node.accumulatedForward = node.accumulatedVertical = node.motionMagnitude = node.accumulatedSway = 0;
    } else {
        const float dy = node.current.y - node.previous.y;
        const float dz = node.current.z - node.previous.z;
        const float dx = node.current.x - node.previous.x;
        const float sine = Sin(node.facingAngle), cosine = Cos(node.facingAngle);
        node.accumulatedForward += dy;
        node.accumulatedSway = ((cosine * dx) - (sine * dz)) + node.accumulatedSway;
        node.accumulatedVertical = ((cosine * dz) + (sine * dx)) + node.accumulatedVertical;
        if (--node.smoothingCountdown <= 0) {
            node.smoothingCountdown = 6;
            const float forward = node.accumulatedForward * 5, sway = node.accumulatedSway * 5, vertical = node.accumulatedVertical * 5;
            node.motionMagnitude = std::sqrt(((forward * forward) + (sway * sway)) + (vertical * vertical));
            node.accumulatedForward = node.accumulatedVertical = node.accumulatedSway = 0;
        }
        result = inverseMotionScale * node.motionMagnitude;
    }
    node.previous = node.current;
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void Spring(OsageNode &node, float pitchRate, float yawRate, float pitchTarget, float yawTarget) {
    if (node.pitchScale > pitchTarget) {
        node.pitchScale -= motionScale * pitchRate;
        if (node.pitchScale < 0)
            node.pitchScale = 0;
    } else {
        node.pitchScale += motionScale * pitchRate;
        if (node.pitchScale > pitchTarget)
            node.pitchScale = pitchTarget;
    }
    if (node.yawScale > yawTarget) {
        node.yawScale -= motionScale * yawRate;
        if (node.yawScale < 0)
            node.yawScale = 0;
    } else {
        node.yawScale += motionScale * yawRate;
        if (node.yawScale > yawTarget)
            node.yawScale = yawTarget;
    }
}

} // namespace

#else

namespace {

// @verified - p
void Spring(OsageNode &node, float pitchRate, float yawRate, float pitchTarget, float yawTarget) {
    const float delta = motionScale * pitchRate;
    if (node.pitchScale <= pitchTarget) {
        node.pitchScale += delta;
        if (node.pitchScale > pitchTarget)
            node.pitchScale = pitchTarget;
    } else {
        node.pitchScale -= delta;
        if (pitchTarget - node.pitchScale > 0)
            node.pitchScale = pitchTarget;
        else if (node.pitchScale < 0)
            node.pitchScale = 0;
    }
    if (node.yawScale <= yawTarget) {
        node.yawScale += motionScale * yawRate;
        if (node.yawScale > yawTarget)
            node.yawScale = yawTarget;
    } else {
        node.yawScale -= motionScale * yawRate;
        if (node.yawScale < 0)
            node.yawScale = 0;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void AdvancePhase(OsageNode &node, float pitch, float yaw) {
    node.pitchPhase += (randomOscillationX + pitch) * motionScale;
    if (node.pitchPhase >= 360)
        node.pitchPhase = static_cast<float>(static_cast<double>(node.pitchPhase) - 360.0);
    else if (node.pitchPhase < 0)
        node.pitchPhase = static_cast<float>(static_cast<double>(node.pitchPhase) + 360.0);
    node.yawPhase += (randomOscillationY + yaw) * motionScale;
    if (node.yawPhase >= 360)
        node.yawPhase -= 360;
    else if (node.yawPhase < 0)
        node.yawPhase += 360;
}

} // namespace

#else

namespace {

// @verified - p
void AdvancePhase(OsageNode &node, float pitch, float yaw) {
    node.pitchPhase += (randomOscillation.x + pitch) * motionScale;
    if (node.pitchPhase >= 360)
        node.pitchPhase = static_cast<float>(static_cast<double>(node.pitchPhase) - 360.0);
    else if (node.pitchPhase < 0)
        node.pitchPhase = static_cast<float>(static_cast<double>(node.pitchPhase) + 360.0);
    node.yawPhase += (randomOscillation.y + yaw) * motionScale;
    if (node.yawPhase >= 360)
        node.yawPhase -= 360;
    else if (node.yawPhase < 0)
        node.yawPhase += 360;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void Offsets(OsageNode &node) {
    node.offsets[0] = O2(PhaseSin(node.pitchPhase) * node.pitchScale, PhaseSin(node.yawPhase) * node.yawScale);
}

} // namespace

#else

namespace {

void Offsets(OsageNode &node) {
    node.offsets[0] = {PhaseSin(node.pitchPhase) * node.pitchScale, PhaseSin(node.yawPhase) * node.yawScale};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ShiftOffsets(OsageNode &node) {
    for (std::int32_t i = 8; i > 0; --i)
        node.offsets[i] = node.offsets[i - 1];
}

} // namespace

#else

namespace {

void ShiftOffsets(OsageNode &node) {
    for (std::int32_t i = 8; i > 0; --i)
        node.offsets[i] = node.offsets[i - 1];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ClampDistance(const Vector3 &origin, Vector3 &point, float distance) {
    const Vector3 delta = O3(point.x - origin.x, point.y - origin.y, point.z - origin.z);
    float squared = ((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z);
    if (squared < 0.0000001f)
        squared = 0.0000001f;
    const float scale = distance / std::sqrt(squared);
    point = O3(scale * delta.x + origin.x, scale * delta.y + origin.y, scale * delta.z + origin.z);
}

} // namespace

#else

namespace {

// @verified - p
void ClampDistance(const Vector3 &origin, Vector3 &point, float distance) {
    const Vector3 delta{point.x - origin.x, point.y - origin.y, point.z - origin.z};
    float magnitude = std::sqrt(((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z));
    if (magnitude < 0.0001f)
        magnitude = 0.0001f;
    point = {(distance / magnitude) * delta.x + origin.x, (distance / magnitude) * delta.y + origin.y, (distance / magnitude) * delta.z + origin.z};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool ProjectSurface(const Vector4 *surface, Vector3 &point, std::int32_t count, float margin) {
    Vector4 center = surface[0];
    for (std::int32_t i = 1; i < count; ++i) {
        const Vector4 &next = surface[i];
        const Vector3 delta = O3(center.x - next.x, center.y - next.y, center.z - next.z);
        float squared = ((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z);
        if (squared < 0.0000001f)
            squared = 0.0000001f;
        float ratio = ((((point.x - next.x) * delta.x) + ((point.y - next.y) * delta.y)) + ((point.z - next.z) * delta.z)) / squared;
        if (count <= 2 || ratio > 0.6)
            ratio -= 0.1f;
        else if (ratio < 0.4)
            ratio += 0.1f;
        ratio = OsageMin(1.0f, OsageMax(0.0f, ratio));
        center = O4(ratio * delta.x + next.x, ratio * delta.y + next.y, ratio * delta.z + next.z, ratio * (center.w - next.w) + next.w);
    }
    const Vector3 delta = O3(point.x - center.x, point.y - center.y, point.z - center.z);
    const float radius = margin + center.w;
    float squared = ((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z);
    if (!(squared < radius * radius))
        return false;
    if (squared < 0.0000001f)
        squared = 0.0000001f;
    const float scale = radius / std::sqrt(squared);
    point = O3(scale * delta.x + center.x, scale * delta.y + center.y, scale * delta.z + center.z);
    return true;
}

} // namespace

#else

namespace {

// @verified - p
bool ProjectSurface(const Vector4 *surface, Vector3 &point, std::int32_t count, float margin) {
    Vector4 center = surface[0];
    for (std::int32_t i = 1; i < count; ++i) {
        const Vector4 &next = surface[i];
        const Vector3 delta{center.x - next.x, center.y - next.y, center.z - next.z};
        float squared = ((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z);
        if (squared < 0.0000001f)
            squared = 0.0000001f;
        float ratio = ((((point.x - next.x) * delta.x) + ((point.y - next.y) * delta.y)) + ((point.z - next.z) * delta.z)) / squared;
        if (count <= 2 || ratio > 0.6)
            ratio -= 0.1f;
        else if (ratio < 0.4)
            ratio += 0.1f;
        ratio = std::fmin(1.0f, std::fmax(0.0f, ratio));
        center = {ratio * delta.x + next.x, ratio * delta.y + next.y, ratio * delta.z + next.z, ratio * (center.w - next.w) + next.w};
    }
    const Vector3 delta{point.x - center.x, point.y - center.y, point.z - center.z};
    const float radius = margin + center.w;
    float magnitude = std::sqrt(((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z));
    if (radius <= magnitude)
        return false;
    if (magnitude < 0.0001f)
        magnitude = 0.0001f;
    point = {(radius / magnitude) * delta.x + center.x, (radius / magnitude) * delta.y + center.y, (radius / magnitude) * delta.z + center.z};
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool ProjectEnabled(const Vector4 *surfaces, const Vector3 &origin, Vector3 &point, float distance, float margin, std::int32_t mask) {
    static const std::int32_t indices[12] = {0, 2, 4, 8, 10, 12, 14, 16, 18, 20, 24, 26};
    static const std::int32_t counts[12] = {2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2};
    bool projected = false;
    for (std::int32_t i = 0; i < 12; ++i)
        if (mask & (1U << i)) {
            if (ProjectSurface(surfaces + indices[i], point, counts[i], margin)) {
                projected = true;
                ClampDistance(origin, point, distance);
            }
        }
    return projected;
}

} // namespace

#else

namespace {

// @verified - p
bool ProjectEnabled(OsageWork &work, const Vector3 &origin, Vector3 &point, float distance, float margin, std::uint16_t mask) {
    constexpr std::int32_t indices[12]{0, 2, 4, 8, 10, 12, 14, 16, 18, 20, 24, 26};
    constexpr std::int32_t counts[12]{2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2};
    bool projected = false;
    for (std::int32_t i = 0; i < 12; ++i)
        if (mask & (1U << i)) {
            projected |= ProjectSurface(work.collisionSurfaces.data() + indices[i], point, counts[i], margin);
            ClampDistance(origin, point, distance);
        }
    return projected;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ParentConstraint(const Vector3 &origin, Vector3 &point, const Vector3 &parent, float distance) {
    const Vector3 delta = O3(point.x - origin.x, point.y - origin.y, point.z - origin.z);
    const float ratio = ((((parent.x - origin.x) * delta.x) + ((parent.y - origin.y) * delta.y)) + ((parent.z - origin.z) * delta.z)) /
        (((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z));
    if (ratio > 0.2) {
        point = O3(ratio * delta.x + origin.x, ratio * delta.y + origin.y, ratio * delta.z + origin.z);
        const float length = OsageDistance(&origin, &parent);
        ClampDistance(parent, point, length);
        ClampDistance(origin, point, distance);
    }
}

} // namespace

#else

namespace {

// @verified - p
void ParentConstraint(const Vector3 &origin, Vector3 &point, const Vector3 &parent, float distance) {
    const Vector3 delta{point.x - origin.x, point.y - origin.y, point.z - origin.z};
    const float ratio = ((((parent.x - origin.x) * delta.x) + ((parent.y - origin.y) * delta.y)) + ((parent.z - origin.z) * delta.z)) /
        (((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z));
    if (ratio > 0.2) {
        point = {ratio * delta.x + origin.x, ratio * delta.y + origin.y, ratio * delta.z + origin.z};
        const float length = GetEuclideanDistance(&origin, &parent);
        ClampDistance(parent, point, length);
        ClampDistance(origin, point, distance);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void AlignEndpoint(const Vector3 &origin, Vector3 &point) {
    OsageTranslate(origin.x, origin.y, origin.z);
    point = O3(point.x - origin.x, point.y - origin.y, point.z - origin.z);
    const float reciprocal = 1.0f / std::sqrt((point.x * point.x + point.y * point.y) + point.z * point.z);
    const Vector3 direction = Vector3::Scale(point, reciprocal);
    const Vector3 down = O3(0, -1, 0);
    Matrix4x4_BuildRotationMatrix(down, direction);
}

} // namespace

#else

namespace {

// @verified - p
void AlignEndpoint(const Vector3 &origin, Vector3 &point) {
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    point = {point.x - origin.x, point.y - origin.y, point.z - origin.z};
    Vector3 direction = point;
    if (std::sqrt((direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z) < 0.000001f)
        return;
    Vector3::NormalizeInPlace(direction);
    const Vector3 down{0, -1, 0};
    if (GetEuclideanDistance(&direction, &down) < 0.000001f)
        return;
    Matrix4x4_BuildRotationMatrix(down, direction);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void BuildTransform(MT7Node *model, OsageNode &node) {
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(node.current.x, node.current.y, node.current.z);
    Matrix4x4::RotateAroundY_Wrapper(node.euler.y);
    Matrix4x4::RotateAroundX_Wrapper(node.euler.x);
    Matrix4x4::RotateAroundZ_Wrapper(node.euler.z);
    model->transformOverride = &node.transform;
    matrix_stack::GetMatrix(&node.transform);
}

} // namespace

#else

namespace {

void BuildTransform(OsageNode &node) {
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(node.current.x, node.current.y, node.current.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.euler.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.euler.x));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.euler.z));
    BindTransform(node);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void CleanupNodes(OsageWork &work) {
    OsageNode *node = work.nodes;
    while (node) {
        OsageNode *const next = node->next;
        if (node->meshAllocation) {
            HLib::FreeTaskData(node->meshAllocation);
            node->meshAllocation = nullptr;
            if (node->node) {
                node->node->runtimeMesh = node->originalMesh;
                node->originalMesh = nullptr;
            }
        }
        HLib::FreeTaskData(node);
        node = next;
    }
    work.nodes = nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ViewSpaceFromEuler(MT7Node *model, OsageNode &node) {
    Matrix4x4::SetViewMatrixFromEuler(node.euler.x, node.euler.y, node.euler.z);
    model->transformOverride = &node.transform;
    matrix_stack::GetMatrix(&node.transform);
}

} // namespace

#else

namespace {


void ViewSpaceFromEuler(OsageNode &node) {
    SetViewSpaceTransformFromEuler(node.euler);
    BindTransform(node);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

OsageCollisionPoint *GetDefaultOsageCollisionPoints(shen_name character) {
    switch (character) {
    case OSAGE_CHARACTER_SYE:
    case OSAGE_CHARACTER_SYP:
        return osageCollisionPointGroups[OsageCollisionGroup_Xiuying];
    case OSAGE_CHARACTER_JOP:
    case OSAGE_CHARACTER_JOY:
        return osageCollisionPointGroups[OsageCollisionGroup_Joy];
    case SHEN_NAME4('S', 'I', 'N', '_'):
        return osageCollisionPointGroups[OsageCollisionGroup_Shenhua];
    case OSAGE_CHARACTER_REN:
        return osageCollisionPointGroups[OsageCollisionGroup_Ren];
    case OSAGE_CHARACTER_KSY:
        return osageCollisionPointGroups[OsageCollisionGroup_Ksy];
    case OSAGE_CHARACTER_KXY:
        return osageCollisionPointGroups[OsageCollisionGroup_Kxy];
    case OSAGE_CHARACTER_RYC:
    case OSAGE_CHARACTER_KYG:
        return osageCollisionPointGroups[OsageCollisionGroup_Ryc];
    case OSAGE_CHARACTER_MN5:
        return osageCollisionPointGroups[OsageCollisionGroup_Mn5];
    case OSAGE_CHARACTER_MMH:
        return osageCollisionPointGroups[OsageCollisionGroup_Mmh];
    case OSAGE_CHARACTER_SHC:
        return osageCollisionPointGroups[OsageCollisionGroup_Shc];
    case OSAGE_CHARACTER_TGY:
        return osageCollisionPointGroups[OsageCollisionGroup_Tgy];
    case OSAGE_CHARACTER_BKN:
        return osageCollisionPointGroups[OsageCollisionGroup_Bkn];
    case OSAGE_CHARACTER_SF3:
        return osageCollisionPointGroups[OsageCollisionGroup_Sf3];
    case SHEN_NAME4('C', 'H', 'A', 'R'):
        return 0;
    default:
        return osageCollisionPointGroups[OsageCollisionGroup_Default];
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void PlaceSpringEndpoint(OsageNode &node, MT7Node *model, MT7Node *endpointNode, float pitch, float length, Vector3 *endpoint) {
    node.euler.x = static_cast<std::uint16_t>(static_cast<std::int32_t>(pitch * 8192.0f / 45.0f));
    BuildTransform(model, node);
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(length * endpointNode->position.x, length * endpointNode->position.y, length * endpointNode->position.z);
    matrix_stack::GetTranslation(endpoint);
    PopCurrentMatrix(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float GetOsageWind(Vector3 *wind) {
    float strength = GetEffectiveWindSpeed(GetWind()) * 30.0f;
    if (strength < 0.5f)
        strength = 0.5f;
    const std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
    wind->y = 0.0f;
    wind->x = CalculateFixedSine(phase) * strength;
    wind->z = CalculateFixedCosine(phase) * strength;
    return strength;
}

float GetOsageWindUnclamped(Vector3 *wind) {
    const float strength = GetEffectiveWindSpeed(GetWind()) * 30.0f;
    const std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
    wind->y = 0.0f;
    wind->x = CalculateFixedSine(phase) * strength;
    wind->z = CalculateFixedCosine(phase) * strength;
    return strength;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float Noise(std::int32_t angle) {
    const float first = CalculateFixedSine(22 * angle), second = CalculateFixedSine(31 * angle);
    const float third = CalculateFixedSine(103 * angle), fourth = CalculateFixedSine(147 * angle);
    return first * 0.30000001f + second * 0.37f + third * 0.15000001f + fourth * 0.18000001f;
}

#else

namespace {

// @verified - p
float Noise(std::int16_t angle) {
    const float first = Sin(22 * angle) * 0.30000001f;
    const float second = Sin(31 * angle) * 0.37f + first;
    const float third = Sin(103 * angle) * 0.15000001f + second;
    return Sin(147 * angle) * 0.18000001f + third;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t Facing(OsageNode &node, std::int32_t index, std::uint32_t extra) {
    return (node.linkFlags & 0x1f) ? node.parent->facingAngle : node.owner->referenceAngles[index] + extra;
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
