#include "s2/areas/area_ycpa.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/npc.hpp"
namespace shenmue::s2::areas {

void YcpaSetCharacterMidpoint(shen_name first, shen_name second) {
    Vector3 position, other;
    EV_GetCharaPosition(first, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    EV_GetCharaPosition(second, CHARACTER_TRANSLATION_ROOT, &other, EventTransformFlags::World);
    position.x = (other.x + position.x) * 0.5f;
    position.z = (other.z + position.z) * 0.5f;
    position.y = (other.y + position.y) * 0.5f;
    EV_AT_SetPos(YCPA_RYOD, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}

void YcpaPlaceDiscoveryActors(bool reverse) {
    EVT_AT_SetPos(YCPA_YUA, YCPA_BASE, Vector3{.x = 0, .y = 0, .z = 0}, 0);
    EVT_AT_SetAng_Enqueue(YCPA_YUA, YCPA_BASE, 0, 0, 0, 0);
    EVT_AT_SetPos(YCPA_RYO, YCPA_BASE, Vector3{.x = reverse ? -0.5f : 0.5f, .y = 0, .z = 2.900000095367431641f}, 0);
    EVT_AT_SetAngY(YCPA_RYO, YCPA_BASE, reverse ? -22.5f : 22.5f, 0);
    EVT_AT_SetPos(YCPA_REN, YCPA_BASE, Vector3{.x = reverse ? 0.800000011920928955f : -0.800000011920928955f, .y = 0, .z = 3.799999952316284180f}, 0);
    EVT_AT_SetAngY(YCPA_REN, YCPA_BASE, reverse ? 22.5f : 337.5f, 0);
}

bool YcpaDiscoverySightLinesClear() {
    Vector3 points[4];
    Vector3i rotations[3];
    EV_GetCharaPosition(YCPA_RYO, CHARACTER_TRANSLATION_ROOT, &points[0], EventTransformFlags::World);
    EV_GetCharaPosition(YCPA_REN, CHARACTER_TRANSLATION_ROOT, &points[1], EventTransformFlags::World);
    EV_GetCharaPosition(YCPA_YUA, CHARACTER_TRANSLATION_ROOT, &points[2], EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(YCPA_RYO, &rotations[0], EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(YCPA_REN, &rotations[1], EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(YCPA_YUA, &rotations[2], EventTransformFlags::World);
    points[0].y += 1;
    points[1].y += 1;
    points[2].y += 1;
    for (std::int32_t index = 0; index < 3; ++index) {
        Vector3 offset{.x = 0, .y = 0, .z = 0.5f};
        ApplyScenarioTranslationAndRotation(&points[index], &rotations[index], &offset);
        points[index] = offset;
    }
    Vector3 discardedOffset{.x = -0.75f, .y = 1.899999976158142090f, .z = 5.5f};
    rotations[0] = {.x = 0, .y = 0, .z = 0};
    ApplyScenarioTranslationAndRotation(&points[3], &rotations[0], &discardedOffset);
    Vector3 intersection;
    for (std::int32_t first = 0; first < 4; ++first) {
        for (std::int32_t second = first + 1; second < 4; ++second) {
            if (FindNearestRayIntersection(&intersection, &points[first], &points[second]) || RaycastInWorldGrid(intersection, points[first], 0.5f))
                return false;
        }
    }
    return true;
}
bool YcpaSelectDiscoveryPlacement(bool followUp) {
    for (shen_name character : {YCPA_RYO, YCPA_REN, YCPA_YUA}) {
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 1, 0);
    }
    Vector3 original;
    Vector3i originalAngle;
    if (!followUp) {
        EV_GetCharaPosition(YCPA_YUA, CHARACTER_TRANSLATION_ROOT, &original, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(YCPA_YUA, &originalAngle, EventTransformFlags::World);
    }
    for (shen_name character : {YCPA_RYO, YCPA_REN, YCPA_YUA})
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Enabled);
    for (shen_name character : {YCPA_RYO, YCPA_REN, YCPA_YUA})
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    if (followUp) {
        EV_GetCharaPosition(YCPA_YUA, CHARACTER_TRANSLATION_ROOT, &original, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(YCPA_YUA, &originalAngle, EventTransformFlags::World);
    }
    const Vector3 corner{.x = 251.6999969482421875f, .y = 100, .z = 203};
    if (ComputeAngle(corner, 3, 0, 0x8000, original)) {
        EV_AT_SetPos(YCPA_YUA, EventTransformFlags::World | EventTransformFlags::AllAxes, corner);
        EV_AT_SetAngY(YCPA_YUA, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 12288, .z = 0});
    }
    EVT_AT_SetPos(YCPA_BASE, YCPA_YUA, Vector3{.x = 0, .y = 0, .z = 0}, 0);
    EVT_AT_SetAngY(YCPA_BASE, YCPA_YUA, followUp ? 180.0f : 0.0f, 0);
    Vector3i baseAngle;
    GetScenarioCharacterEulerAngles(YCPA_BASE, &baseAngle, EventTransformFlags::World);
    bool reverse = false;
    for (std::int32_t orientation = 0; orientation < 2; ++orientation) {
        for (std::int32_t attempt = 0; attempt < 48; ++attempt) {
            const float sign = static_cast<float>(attempt % 2) * -1.0f;
            const float degrees = (((sign + sign) + 1.0f) * (static_cast<float>(attempt + 1) * 0.5f)) * 7.5f;
            EVT_AT_SetAngY(YCPA_BASE, YCPA_BASE, degrees, 0);
            reverse = orientation != 0;
            YcpaPlaceDiscoveryActors(reverse);
            if (YcpaDiscoverySightLinesClear())
                return reverse;
            EV_AT_SetAngY(YCPA_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, baseAngle);
        }
    }
    return reverse;
}
} // namespace shenmue::s2::areas
