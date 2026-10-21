#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"

namespace shenmue::s2 {
using namespace model_component_detail;

namespace model_component_detail {

ReflectorBoardParams *FindReflectorBoard(Character *character) {
    ModelComponentHeader *const component = FindHeader(character, kReflectorBoardToken);
    return component ? reinterpret_cast<ReflectorBoardParams *>(Payload(component)) : nullptr;
}

// @verified - p
void UpdateReflectorBoardParams(Character *character, void *payload) {
    ReflectorBoardParams *const reflector = static_cast<ReflectorBoardParams *>(payload);
    if (!character || !reflector)
        return;

    constexpr float fadeStep = 0.010714286f;
    if (reflector->sector == 2) {
        const shen_name name = GetCharacterScenarioName(character);
        if (IsScenarioCharacterForcedVisible(name)) {
            reflector->colour[0] = std::min(reflector->colour[0] + fadeStep, kReflectorBoardColourLimit);
            reflector->colour[1] = reflector->colour[0];
            reflector->colour[2] = reflector->colour[0];
        } else if (reflector->colour[0] > 0.0f) {
            reflector->colour[0] = std::max(reflector->colour[0] - fadeStep, 0.0f);

        reflector->colour[1] = reflector->colour[0];
        reflector->colour[2] = reflector->colour[0];
        }
        reflector->mode = reflector->colour[0] > 0.0f ? 1 : 0;
    } else if (reflector->sector == 3) {
        reflector->colour[0] = std::min(reflector->colour[0] + fadeStep, kReflectorBoardColourLimit);
        reflector->colour[1] = reflector->colour[0];
        reflector->colour[2] = reflector->colour[0];
        reflector->mode = 1;
    }
    reflector->scale = std::clamp(reflector->colour[0] / kReflectorBoardColourLimit, 0.0f, 1.0f);
}

void CleanupReflectorBoardParams(Character *character, void *payload) {
    auto *const reflector = static_cast<ReflectorBoardParams *>(payload);
    if (!character || !reflector)
        return;
    if (character->core == AssignReflectorBoardParams) {
        character->core = reflector->coreAssignment;
    }
    reflector->coreAssignment = nullptr;
}

} // namespace model_component_detail

namespace {
std::int32_t reflectorSavedActiveLight{};
}
// @verified - p
std::int16_t ResolveReflectorBoardCharacterAngle(Character *character) {
    const std::int32_t cameraYaw = -camCtrlGetNextRotY();
    Vector3 cameraDirection{-sin_fixed16(static_cast<std::int16_t>(cameraYaw)), 0.0f, cos_fixed16(static_cast<std::uint16_t>(cameraYaw))};
    Vector3::NormalizeInPlace(cameraDirection);
    const std::int16_t boneYaw = GetMotionBoneYaw(FindMotionWork(character), MB_HeadTarget);
    Vector3 characterDirection{sin_fixed16(boneYaw), 0.0f, cos_fixed16(static_cast<std::uint16_t>(boneYaw))};
    Vector3::NormalizeInPlace(characterDirection);
    float angle = ((Vector3 ::Dot(cameraDirection, characterDirection) * 180.0f) - 180.0f) * -0.5f;
    if (static_cast<std::int16_t>(boneYaw + cameraYaw) < 0)
        angle = -angle;
    if (angle > 90.0f)
        angle = angle * 0.5f - 90.0f;
    else if (angle < -90.0f)
        angle = angle * 0.5f + 90.0f;
    else
        angle *= -0.5f;
    return static_cast<std::int16_t>((angle * 8192.0f) / 45.0f);
}

// @verified - p
void SetReflectorBoardDirectionalLight(Vector3 &origin, const Vector3 &cameraPosition, std::uint32_t slot) {
    if (std::fabs(origin.x) < 0.00001f)
        origin.x = 0.0f;
    if (std::fabs(origin.y) < 0.00001f)
        origin.y = 0.0f;
    if (std::fabs(origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 direction{origin.x - cameraPosition.x, origin.y - cameraPosition.y, origin.z - cameraPosition.z};
    Vector3::NormalizeInPlace(direction);
    SetSelectedLightDirection(slot, direction);
}

// @verified - p
void shenAssignAttachedLightFor(Vector3 &origin, const Vector3 &cameraPosition, std::int16_t yaw, std::uint32_t slot) {
    Vector3 current;
    camCtrlGetTargetPosition(&current);
    const float dx = current.x - cameraPosition.x;
    const float dz = current.z - cameraPosition.z;
    const float distance = std::sqrt(dx * dx + dz * dz);
    const std::uint16_t angle = static_cast<std::uint16_t>(yaw - camCtrlGetNextRotY());
    const float projectedX = current.x - sin_fixed16(static_cast<std::int16_t>(angle)) * distance;
    const float projectedZ = cos_fixed16(angle) * distance + current.z;
    if (std::fabs(origin.x) < 0.00001f)
        origin.x = 0.0f;
    if (std::fabs(origin.y) < 0.00001f)
        origin.y = 0.0f;
    if (std::fabs(origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 direction{origin.x - projectedX, origin.y - current.y, origin.z - projectedZ};
    Vector3::NormalizeInPlace(direction);
    SetSelectedLightDirection(slot, direction);
}

// @verified - p
void SetReflectorBoardOrbitPointLight(Vector3 &origin, const Vector3 &cameraPosition, std::int16_t yaw, std::uint32_t slot) {
    if (std::fabs(origin.x) < 0.00001f)
        origin.x = 0.0f;
    if (std::fabs(origin.y) < 0.00001f)
        origin.y = 0.0f;
    if (std::fabs(origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 current;
    camCtrlGetTargetPosition(&current);
    const float dx = current.x - cameraPosition.x;
    const float dz = current.z - cameraPosition.z;
    const float distance = std::sqrt(dx * dx + dz * dz);
    const std::uint16_t angle = static_cast<std::uint16_t>(yaw - camCtrlGetNextRotY());
    const float projectedX = current.x - sin_fixed16(static_cast<std::int16_t>(angle)) * distance;
    const float projectedZ = cos_fixed16(angle) * distance + current.z;
    Vector3 direction{origin.x - projectedX, origin.y - current.y, origin.z - projectedZ};
    Vector3::NormalizeInPlace(direction);
    const Vector3 position{origin.x - direction.x * 0.40000001f, origin.y - direction.y * 0.40000001f, origin.z - direction.z * 0.40000001f};
    shenSetLightPosition(slot, &position);
    shenSetLightScale(slot, 0.25f);
    shenSetLightScale2(slot, 1.5f);
    SetLightKind(slot, 3);
}

// @verified - p
void SetReflectorBoardPointLight(Vector3 &origin, const Vector3 &cameraPosition, std::uint32_t slot) {
    if (std::fabs(origin.x) < 0.00001f)
        origin.x = 0.0f;
    if (std::fabs(origin.y) < 0.00001f)
        origin.y = 0.0f;
    if (std::fabs(origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 direction{origin.x - cameraPosition.x, origin.y - cameraPosition.y, origin.z - cameraPosition.z};
    Vector3::NormalizeInPlace(direction);
    const Vector3 position{origin.x - direction.x * 0.40000001f, origin.y - direction.y * 0.40000001f, origin.z - direction.z * 0.40000001f};
    shenSetLightPosition(slot, &position);
    shenSetLightScale(slot, 0.25f);
    shenSetLightScale2(slot, 1.5f);
    SetLightKind(slot, 3);
}

// @verified - p
void ConfigureReflectorBoardLight(Character *character, ReflectorBoardParams *reflector, std::uint32_t slot) {
    reflectorSavedActiveLight = GetCurrentActiveLight();
    Vector3 origin;
    GetCharacterMotionWorldPosition(character, MB_Head, &origin);
    Vector3 cameraPosition;
    camCtrlGetNextCamPos(&cameraPosition);
    SetCurrentActiveLight(3);
    ResetLightEntry(slot);
    SetLightKind(slot, 2);
    SetLegacySelectedLightEnabled(slot, true);
    const bool custom = reflector->customLight == 1;
    const std::uint32_t mode = custom ? reflector->lightType : reflectorDefaultLightType;
    switch (mode) {
    case 0:
        SetReflectorBoardDirectionalLight(origin, cameraPosition, slot);
        break;
    case 1:
        shenAssignAttachedLightFor(origin, cameraPosition, static_cast<std::int16_t>(custom ? reflector->lightAngle : reflectorDefaultLightAngle), slot);
        break;
    case 2:
        shenAssignAttachedLightFor(origin, cameraPosition, ResolveReflectorBoardCharacterAngle(character), slot);
        break;
    case 3:
        SetReflectorBoardPointLight(origin, cameraPosition, slot);
        break;
    case 4:
        SetReflectorBoardOrbitPointLight(origin, cameraPosition, static_cast<std::int16_t>(custom ? reflector->lightAngle : reflectorDefaultLightAngle), slot);
        break;
    case 5:
        SetReflectorBoardOrbitPointLight(origin, cameraPosition, ResolveReflectorBoardCharacterAngle(character), slot);
        break;
    }
    const Vector4 colour = custom ? Vector4{reflector->scale, reflector->colour[0], reflector->colour[1], reflector->colour[2]} : reflectorDefaultValues;
    shenSetLightColour(slot, colour);
    UpdateLegacyLightTransforms();
}

// @verified - p
void AssignReflectorBoardParams(Character *character) {
    ReflectorBoardParams *const reflector = FindReflectorBoard(character);
    if (!reflector)
        return;
    if (reflectorBoardActive && (reflector->sector == 1 || reflector->mode != 0)) {
        if (GetCurrentActiveLight() == 1) {
            const float scale = reflector->customLight == 1 ? reflector->colour[0] : reflectorDefaultValues.y;
            Vector3 sourcePosition;
            GetCharacterMotionWorldPosition(character, MB_Head, &sourcePosition);

            Vector3 cameraPosition;
            camCtrlGetNextCamPos(&cameraPosition);

            Vector3 direction{sourcePosition.x - cameraPosition.x, sourcePosition.y - cameraPosition.y, sourcePosition.z - cameraPosition.z};
            Vector3 ::NormalizeInPlace(direction);
            direction.x *= scale;
            direction.y *= scale;
            direction.z *= scale;

            SetLegacyLambertFvfReference(&direction);
            reflector->coreAssignment(character);
            SetLegacyLambertFvfReference(nullptr);

        }

        else {
            ConfigureReflectorBoardLight(character, reflector, 15);
            reflector->coreAssignment(character);
            SetCurrentActiveLight(reflectorSavedActiveLight);
            ResetLightEntry(15);
            SetLegacySelectedLightEnabled(15, false);
            UpdateLegacyLightTransforms();
        }
    } else {
        reflector->coreAssignment(character);
    }
}
bool AttachReflectorBoardComponent(Character *character, std::int32_t sector) {
    if (!character)
        return false;
    ModelComponentHeader *component = FindHeader(character, kReflectorBoardToken);
    ReflectorBoardParams *reflector = component ? reinterpret_cast<ReflectorBoardParams *>(Payload(component)) : nullptr;
    if (!component) {
        const bool installUpdate = sector < 5;
        if (!installUpdate)
            sector -= 5;
        reflector = static_cast<ReflectorBoardParams *>(AllocateModelComponent(
            character, kReflectorBoardToken, sizeof(ReflectorBoardParams), installUpdate ? UpdateReflectorBoardParams : nullptr, CleanupReflectorBoardParams));
        if (!reflector)
            return false;
        component = HeaderForPayload(reflector);
        if (!installUpdate)
            component->flags &= ~1U;
        reflector->coreAssignment = charaAssignCoreCallback(character, AssignReflectorBoardParams);
        reflector->scale = 1.0f;
        reflector->colour[0] = kReflectorBoardColourLimit;
        reflector->colour[1] = kReflectorBoardColourLimit;
        reflector->colour[2] = kReflectorBoardColourLimit;
        reflector->mode = 0;
        reflector->lightType = 0;
        reflector->customLight = 0;
    }
    reflector->sector = sector == 0 ? 2 : sector;
    if (reflector->sector == 2)
        reflector->customLight = 1;
    return true;
}

bool ReadReflectorBoardState(const Character *character, std::int32_t &sector, Vector4 &values) {
    const ReflectorBoardParams *const reflector = static_cast<const ReflectorBoardParams *>(FindModelComponent(character, kReflectorBoardToken));
    if (!reflector)
        return false;
    sector = reflector->sector;
    values.y = reflector->colour[0];
    values.z = reflector->colour[1];
    values.w = reflector->colour[2];
    return true;
}

// @verified - p
void ResetReflectorBoardState() {
    reflectorDefaultValues = {1.0f, 0.30000001f, 0.30000001f, 0.30000001f};
    reflectorBoardActive = 1;
}

void SetReflectorBoardDefaultValues(const Vector4 &values) {
    reflectorDefaultValues = {1.0f, values.y, values.z, values.w};
}

// @verified - p
void ResetReflectorBoardMode(Character *character, std::int32_t sector) {
    if (!character)
        return;
    ReflectorBoardParams *const reflector = FindReflectorBoard(character);
    if (!reflector)
        return;
    reflector->sector = sector;
    reflector->customLight = 1;
    const Vector4 values = reflectorDefaultValues;
    if (sector == 0)
        sector = 2;
    reflector->sector = sector;
    reflector->scale = values.x;
    reflector->colour[0] = values.y;
    reflector->colour[1] = values.z;
    reflector->colour[2] = values.w;
}

bool SetReflectorBoardMode(Character *character, std::int32_t mode) {
    ReflectorBoardParams *const reflector = FindReflectorBoard(character);
    if (!reflector)
        return false;
    reflector->mode = static_cast<std::uint32_t>(mode);
    reflector->customLight = 1;
    if (mode == 0)
        reflector->sector = 0;
    return true;
}

// @verified - p
ReflectorBoardParams *SetReflectorBoardValues(Character *character, const Vector4 &values) {
    ReflectorBoardParams *const reflector = FindReflectorBoard(character);
    if (reflector) {
        reflector->customLight = 1;
        reflector->colour[0] = values.y;
        reflector->colour[1] = values.z;
        reflector->colour[2] = values.w;
    }
    return reflector;
}

bool SetReflectorBoardLightMode(Character *character, bool active, std::uint32_t lightMode, std::int32_t lightAngle) {
    ReflectorBoardParams *const reflector = FindReflectorBoard(character);
    if (!reflector)
        return false;
    reflector->mode = active ? 1U : 0U;
    if (!active)
        reflector->sector = 0;
    reflector->lightType = lightMode;
    reflector->lightAngle = lightAngle;
    if (!reflector->customLight) {
        reflector->customLight = 1;
        reflector->scale = reflectorDefaultValues.x;
        reflector->colour[0] = reflectorDefaultValues.y;
        reflector->colour[1] = reflectorDefaultValues.z;
        reflector->colour[2] = reflectorDefaultValues.w;
    }
    return true;
}

} // namespace shenmue::s2
