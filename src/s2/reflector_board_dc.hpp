#pragma once

#include "s2/character_components_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#define MODEL_COMPONENT_REFB SHEN_NAME4('R', 'E', 'F', 'B')
static const float reflectorBoardColourLimit = 0.3f, reflectorBoardFadeStep = 0.010714286f;
struct ReflectorBoardWork {
    void (*coreAssignment)(Character *);
    float scale, colour[3];
    std::int32_t sector;
    std::uint32_t mode, lightType;
    std::int32_t lightAngle;
    std::uint32_t customLight;
    std::byte retainedState[4];
};
typedef char Layout_reflector_board_dc_0[(sizeof(ReflectorBoardWork) == 0x2c) ? 1 : -1];
typedef char Layout_reflector_board_dc_1[(offsetof(ReflectorBoardWork, scale) == 4) ? 1 : -1];
typedef char Layout_reflector_board_dc_2[(offsetof(ReflectorBoardWork, colour) == 8) ? 1 : -1];
typedef char Layout_reflector_board_dc_3[(offsetof(ReflectorBoardWork, sector) == 0x14) ? 1 : -1];
typedef char Layout_reflector_board_dc_4[(offsetof(ReflectorBoardWork, mode) == 0x18) ? 1 : -1];
typedef char Layout_reflector_board_dc_5[(offsetof(ReflectorBoardWork, lightType) == 0x1c) ? 1 : -1];
typedef char Layout_reflector_board_dc_6[(offsetof(ReflectorBoardWork, lightAngle) == 0x20) ? 1 : -1];
typedef char Layout_reflector_board_dc_7[(offsetof(ReflectorBoardWork, customLight) == 0x24) ? 1 : -1];

extern std::int32_t reflectorBoardActive, reflectorDefaultLightType, reflectorDefaultLightAngle;
extern std::uint32_t reflectorSavedActiveLight;
extern Vector4 reflectorDefaultValues;
void ResetReflectorBoardState();
void SetReflectorBoardActive(std::int32_t active);
void ResetReflectorBoardMode(Character *character, std::int32_t sector);
std::int32_t GetReflectorBoardSector(Character *character);
void SetReflectorBoardDefaultValues(const Vector4 &values);
ReflectorBoardWork *SetReflectorBoardValues(Character *character, const Vector4 &values);
ReflectorBoardWork *SetReflectorBoardValues(Character *character, const Vector4 *values);
bool GetReflectorBoardValues(Character *character, Vector4 &values);
void SetReflectorBoardLightMode(Character *character, std::int32_t type, std::int32_t angle);
void (*SetReflectorBoardCoreCallback(Character *character, void (*callback)(Character *)))(Character *);
void UpdateReflectorBoardParams(Character *character, void *payload);
void AssignReflectorBoardParams(Character *character);
void ConfigureReflectorBoardLight(Character *character, ReflectorBoardWork *reflector, std::uint32_t slot);
void RestoreReflectorBoardLight(std::uint32_t slot);
std::uint32_t ResolveReflectorBoardCharacterAngle(Character *character);
void SetReflectorBoardDirectionalLight(Vector3 &origin, const Vector3 &cameraPosition, std::uint32_t slot);
void shenAssignAttachedLightFor(Vector3 &origin, const Vector3 &cameraPosition, std::int32_t yaw, std::uint32_t slot);
void SetReflectorBoardOrbitPointLight(Vector3 &origin, const Vector3 &cameraPosition, std::int32_t yaw, std::uint32_t slot);
void SetReflectorBoardPointLight(Vector3 &origin, const Vector3 &cameraPosition, std::uint32_t slot);

} // namespace s2
} // namespace shenmue

#endif
