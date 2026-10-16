#pragma once
#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct Character;
struct Mt5RuntimeNode;
struct OsageConstraintRange;
struct OsageHairTuning;
struct OsageProfilePoint;
struct OsageReferenceAngleConfig;
struct OsageSegmentRuntime;
struct OsageWork;
struct ScenarioContext;

void OSAGE_Alloc(Character *character);
void OSAGE_Refresh(Character *character);
void OSAGE_Initialize(Character *character);
void Script_osageSetTarget(ScenarioContext *context, void *rawParams);

inline constexpr std::array<std::int16_t, 15> kOsageNodeKinds{
    120,
    121,
    122,
    123,
    124,
    125,
    126,
    127,
    128,
    129,
    130,
    141,
    131,
    65,
    -1,
};

inline constexpr std::array<std::uint16_t, 28> kOsageCollisionNodeIds{{
    47,
    47,
    47,
    47,
    2,
    2,
    2,
    2,
    7,
    7,
    13,
    13,
    9,
    9,
    15,
    15,
    10,
    10,
    16,
    16,
    20,
    20,
    20,
    20,
    24,
    24,
    31,
    31,
}};
#pragma pack(push, 1)
struct OsageWork {
    std::uint32_t imageName;
    std::byte reserved004[4];
    Character *character;
    Character *targetCharacter;
    std::uint32_t referenceAngles[5];
    std::uint32_t reserved02C;
    void *faceData;
    void *targetFaceData;
    Vector4 referencePoints[28];
    float deltaOverride;
    float distanceLimit;
    std::uint64_t renderChain;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct OsageSegmentRuntime {
    std::uint32_t tag;
    std::int32_t reserved004;
    OsageWork *owner;
    Mt5RuntimeNode *node;
    std::int16_t nodeKind;
    std::uint8_t linkFlags;
    std::uint8_t updateFlags;
    Matrix4x4 transform;
    Vector3 localEndpoint;
    std::uint64_t previousEndpointXY;
    float previousEndpointZ;
    Vector3 worldEndpoint;
    std::uint32_t baseRotation;
    std::int32_t rotX;
    std::int32_t rotY;
    std::int32_t rotZ;
    Vector4 angles;
    float windPhaseX;
    float windPhaseY;
    float angleX;
    float angleZ;
    float oscillatedAngleX;
    float currentAngleZ;
    std::byte angleHistory[0x48];
    std::uint32_t state;
    std::uint32_t reserved104;
    void *auxiliaryData;
    OsageSegmentRuntime *parent;
    OsageSegmentRuntime *child;
    OsageSegmentRuntime *next;
    std::byte reserved128[0x58];
};
#pragma pack(pop)

struct OsageProfilePoint {
    std::int8_t x;
    std::int8_t y;
    std::int8_t z;
    std::int8_t radius;
};
using OsageCollisionProfile = std::array<OsageProfilePoint, 28>;

struct OsageReferenceAngleConfig {
    std::uint8_t nodeId;
    Vector3 baseOffset;
    Vector3 positiveOffset;
    Vector3 negativeOffset;
};

struct OsageConstraintRange {
    std::uint8_t first;
    std::uint8_t count;
};

struct OsageHairTuning {
    float lengthScale;
    float collisionTolerance;
    float gravity;
    float damping;
    std::uint8_t alternateMode;
};

constexpr std::uint32_t OsageImageCode(char a, char b, char c) {
    return make_shen_name(c, b, a, '\0');
}

constexpr std::uint32_t OsageImageCode(char a, char b, char c, char d) {
    return make_shen_name(d, c, b, a);
}
} // namespace shenmue::s1
