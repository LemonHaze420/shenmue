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

void UpdateReflectorBoardParams(Character *character, void *payload) {
    ReflectorBoardParams *const reflector = static_cast<ReflectorBoardParams *>(payload);
    if (!character || !reflector)
        return;

    constexpr float fadeStep = 0.010714286f;
    if (reflector->sector == 1) {
        const shen_name name = GetCharacterScenarioName(character);
        if (IsScenarioCharacterForcedVisible(name)) {
            reflector->colour[0] = std::min(reflector->colour[0] + fadeStep, kReflectorBoardColourLimit);
        } else if (reflector->colour[0] > 0.0f) {
            reflector->colour[0] = std::max(reflector->colour[0] - fadeStep, 0.0f);
        }
        reflector->colour[1] = reflector->colour[0];
        reflector->colour[2] = reflector->colour[0];
        reflector->lightAngleOrVisible = reflector->colour[0] > 0.0f ? 1 : 0;
    } else if (reflector->sector == 3) {
        reflector->colour[0] = std::min(reflector->colour[0] + fadeStep, kReflectorBoardColourLimit);
        reflector->colour[1] = reflector->colour[0];
        reflector->colour[2] = reflector->colour[0];
        reflector->lightAngleOrVisible = 1;
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

std::int16_t ResolveReflectorBoardCharacterAngle(Character *character) {
    if (!character || !g_cameraControl)
        return 0;
    Matrix4x4 transform{};
    if (!ResolveCharacterDrawNodeTransform(character, 7, transform))
        return 0;
    float x = -transform.rows[2].x;
    const float y = -transform.rows[2].y;
    float z = -transform.rows[2].z;
    if (std::fabs(y) > 0.60000002f) {
        const float sign = y >= 0.0f ? -1.0f : 1.0f;
        x = transform.rows[1].x * sign;
        z = transform.rows[1].z * sign;
    }
    const std::int16_t euler = static_cast<std::int16_t>(FixedAtan2(z, -x));
    const std::int16_t cameraYaw = static_cast<std::int16_t>(-camCtrlGetNextRotY());
    Vector3 cameraDirection{
        -sin_fixed16(cameraYaw),
        0.0f,
        sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(cameraYaw)))),
    };
    Vector3 characterDirection{
        sin_fixed16(euler),
        0.0f,
        sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(euler)))),
    };
    const float cameraLength = std::sqrt(cameraDirection.x * cameraDirection.x + cameraDirection.z * cameraDirection.z);
    if (cameraLength > 0.0f) {
        cameraDirection.x /= cameraLength;
        cameraDirection.z /= cameraLength;
    }
    const float characterLength = std::sqrt(characterDirection.x * characterDirection.x + characterDirection.z * characterDirection.z);
    if (characterLength > 0.0f) {
        characterDirection.x /= characterLength;
        characterDirection.z /= characterLength;
    }
    float angle = ((characterDirection.x * cameraDirection.x + characterDirection.z * cameraDirection.z) * 180.0f - 180.0f) * -0.5f;
    if (static_cast<std::int16_t>(euler + cameraYaw) < 0)
        angle = -angle;
    if (angle > 90.0f)
        angle = angle * 0.5f - 90.0f;
    else if (angle < -90.0f)
        angle = angle * 0.5f + 90.0f;
    else
        angle *= -0.5f;
    return static_cast<std::int16_t>(angle * 8192.0f / 45.0f);
}

void AssignReflectorBoardParams(Character *character) {
    ReflectorBoardParams *const reflector = FindReflectorBoard(character);
    if (!reflector)
        return;
    const auto runRetainedCore = [&] {
        if (reflector->coreAssignment)
            reflector->coreAssignment(character);
    };
    if (!reflectorBoardActive || (reflector->sector != 2 && reflector->lightAngleOrVisible == 0)) {
        runRetainedCore();
        return;
    }

    Vector3 sourcePosition{};
    if (!g_cameraControl || !ResolveCharacterDrawNodeTranslation(character, 5, sourcePosition)) {
        runRetainedCore();
        return;
    }
    Vector3 cameraPosition{};
    camCtrlGetNextCamPos(&cameraPosition);
    if (GetCurrentActiveLight() == 1) {
        Vector3 direction{sourcePosition.x - cameraPosition.x, sourcePosition.y - cameraPosition.y, sourcePosition.z - cameraPosition.z};
        const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
        if (length > 0.0f) {
            const float scale = reflector->mode == 1 ? reflector->colour[0] : reflectorDefaultValues.y;
            direction.x = direction.x / length * scale;
            direction.y = direction.y / length * scale;
            direction.z = direction.z / length * scale;
        }
        SetLegacyLambertFvfReference(&direction);
        runRetainedCore();
        SetLegacyLambertFvfReference(nullptr);
        return;
    }

    const bool custom = reflector->customLight == 1;
    const std::uint32_t lightType = custom ? reflector->lightType : reflectorDefaultLightType;
    const std::int16_t angle = lightType == 2 || lightType == 5 ?
        ResolveReflectorBoardCharacterAngle(character) :
        static_cast<std::int16_t>(custom ? reflector->lightAngleOrVisible : reflectorDefaultLightAngle);
    const Vector4 colour = custom ? Vector4{reflector->scale, reflector->colour[0], reflector->colour[1], reflector->colour[2]} : reflectorDefaultValues;
    TemporaryReflectionLightState saved{};
    if (!BeginTemporaryReflectionLight(15, sourcePosition, cameraPosition, colour, lightType, angle, saved)) {
        runRetainedCore();
        return;
    }
    runRetainedCore();
    EndTemporaryReflectionLight(saved);
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
        sector = 1;
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
    reflector->lightAngleOrVisible = lightAngle;
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
