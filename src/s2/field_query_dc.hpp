#pragma once

#include "s2/field_collision_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct MapControlData;
struct FightFieldProbe {
    std::int32_t kind, mode;
    float x, z;
    std::uint32_t flags;
    float fraction;
    std::uint32_t tag;
    float radius, originX, originZ;
};
typedef char FightFieldProbeSizeCheck[sizeof(FightFieldProbe) == 0x28 ? 1 : -1];
void ResolveFightFieldProbe(FightFieldProbe *probe, MapControlData *control);
std::int32_t FindNearestRayIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
std::int32_t TestFieldWorldSegment(const Vector3 *start, const Vector3 *end);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius);
#else
bool GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius);
#endif
std::int32_t FindRayIntersectionInGridWithinRadius(const Vector3 *origin, const Vector3 *destination, float radius);
extern std::int32_t g_fieldRayUsesGrid;
std::int32_t SetOccupancyForThisFieldSegment(shen_name name, const Vector3 *position, std::int32_t yaw);
std::uint32_t flddQueryStairConnection(const Vector3 *position, Vector3 *anchor, std::int32_t *yaw, std::int32_t *connectionId);
std::uint32_t flddQueryStairOrigin(const Vector3 *position, Vector3 *origin, std::int32_t *yaw);
std::int32_t scenFindIntersection(const Vector3 *position);
#endif
bool flddIsValid();
void flddToggleFlag(std::int16_t flags);
bool flddHasFoundIntersectionOfType(std::int16_t flags);
void transformIntersectionResult(Vector2s *angles);
std::uint32_t RaycastInWorldGrid(Vector3 *output, const Vector3 *position, float radius);
std::uint32_t ResolveFieldPartSphereOverlap(Vector3 *output, const Vector3 *position, float radius);
bool MoveObjectToPosFromThisScene(shen_name partName, std::int32_t entryId, Vector3 &position, std::int32_t &yaw);

} // namespace s2
} // namespace shenmue

#endif
