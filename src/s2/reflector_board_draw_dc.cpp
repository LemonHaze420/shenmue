#include "s2/reflector_board_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/mt7.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/motion_work_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
void ConfigureReflectorBoardLight(Character *character, ReflectorBoardWork *reflector, std::uint32_t slot) {
    reflectorSavedActiveLight = GetCurrentActiveLight();
    Vector3 origin, cameraPosition;
    charaGetMotWorldPos(character, 5, &origin);
    camCtrlGetNextCamPos(&cameraPosition);
    SetCurrentActiveLight(3);
    ResetSelectedLightEntry(slot);
    SetLightKind(slot, 2);
    EnableSelectedLight(slot);
    Vector4 colour;
    std::int32_t yaw;
    if (reflector->customLight == 1) {
        switch (reflector->lightType) {
        case 0:
            SetReflectorBoardDirectionalLight(origin, cameraPosition, slot);
            break;
        case 1:
            shenAssignAttachedLightFor(origin, cameraPosition, reflector->lightAngle, slot);
            break;
        case 2:
            yaw = ResolveReflectorBoardCharacterAngle(character);
            shenAssignAttachedLightFor(origin, cameraPosition, yaw, slot);
            break;
        case 3:
            SetReflectorBoardPointLight(origin, cameraPosition, slot);
            break;
        case 4:
            SetReflectorBoardOrbitPointLight(origin, cameraPosition, reflector->lightAngle, slot);
            break;
        case 5:
            yaw = ResolveReflectorBoardCharacterAngle(character);
            SetReflectorBoardOrbitPointLight(origin, cameraPosition, yaw, slot);
            break;
        }
        colour = *reinterpret_cast<Vector4 *>(&reflector->scale);
    } else {
        switch (reflectorDefaultLightType) {
        case 0:
            SetReflectorBoardDirectionalLight(origin, cameraPosition, slot);
            break;
        case 1:
            shenAssignAttachedLightFor(origin, cameraPosition, reflectorDefaultLightAngle, slot);
            break;
        case 2:
            yaw = ResolveReflectorBoardCharacterAngle(character);
            shenAssignAttachedLightFor(origin, cameraPosition, yaw, slot);
            break;
        case 3:
            SetReflectorBoardPointLight(origin, cameraPosition, slot);
            break;
        case 4:
            SetReflectorBoardOrbitPointLight(origin, cameraPosition, reflectorDefaultLightAngle, slot);
            break;
        case 5:
            yaw = ResolveReflectorBoardCharacterAngle(character);
            SetReflectorBoardOrbitPointLight(origin, cameraPosition, yaw, slot);
            break;
        }
        colour = reflectorDefaultValues;
    }
    shenSetLightColour(slot, colour);
    PublishGlobalLighting();
}

// @verified - d
void RestoreReflectorBoardLight(std::uint32_t slot) {
    SetCurrentActiveLight(reflectorSavedActiveLight);
    if (reflectorSavedActiveLight == 1 && !slot) {
        EnableSelectedLight(0);
    } else {
        ResetSelectedLightEntry(slot);
        DisableSelectedLight(slot);
    }
    PublishGlobalLighting();
}

// @verified - d
void AssignReflectorBoardParams(Character *character) {
    ReflectorBoardWork *const reflector = static_cast<ReflectorBoardWork *>(charFindComp(character, MODEL_COMPONENT_REFB));
    if (!reflector)
        return;
    if (reflectorBoardActive && (reflector->sector == 1 || reflector->mode)) {
        if (GetCurrentActiveLight() == 1) {
            const float scale = reflector->customLight == 1 ? reflector->colour[0] : reflectorDefaultValues.y;
            Vector3 origin, cameraPosition;
            charaGetMotWorldPos(character, 5, &origin);
            camCtrlGetNextCamPos(&cameraPosition);
            origin.x -= cameraPosition.x;
            origin.y -= cameraPosition.y;
            origin.z -= cameraPosition.z;
            am2Vector3NormalizeFast(&origin);
            origin.x *= scale;
            origin.y *= scale;
            origin.z *= scale;
            dc::sdk::SetMT7ReflectedDirection(&origin);
            reflector->coreAssignment(character);
            dc::sdk::SetMT7ReflectedDirection(0);
        } else {
            std::uint32_t slot = 15;
            ConfigureReflectorBoardLight(character, reflector, slot);
            reflector->coreAssignment(character);
            RestoreReflectorBoardLight(slot);
        }
    } else {
        reflector->coreAssignment(character);
    }
}

} // namespace s2
} // namespace shenmue

#endif
