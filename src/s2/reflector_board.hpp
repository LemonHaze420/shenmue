#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

constexpr float kReflectorBoardColourLimit = 0.30000001f;

#pragma pack(push, 4)
#pragma pack(push, 4)
struct ReflectorBoardParams {
    CharacterCoreCallback coreAssignment;
    float scale;
    float colour[3];
    std::int32_t sector;
    std::uint32_t mode;
    std::uint32_t lightType;
    std::int32_t lightAngleOrVisible;
    std::uint32_t customLight;
#if SHENMUE_IS_PC
    std::uint32_t reserved2c;
#else
    std::uint32_t reserved28;
#endif
};
#pragma pack(pop)

static_assert(alignof(ReflectorBoardParams) == 4);
static_assert(offsetof(ReflectorBoardParams, coreAssignment) == 0x00);
#if SHENMUE_IS_PC
static_assert(sizeof(ReflectorBoardParams) == 0x30);
static_assert(offsetof(ReflectorBoardParams, scale) == 0x08);
static_assert(offsetof(ReflectorBoardParams, colour) == 0x0c);
static_assert(offsetof(ReflectorBoardParams, sector) == 0x18);
static_assert(offsetof(ReflectorBoardParams, mode) == 0x1c);
static_assert(offsetof(ReflectorBoardParams, lightType) == 0x20);
static_assert(offsetof(ReflectorBoardParams, lightAngleOrVisible) == 0x24);
static_assert(offsetof(ReflectorBoardParams, customLight) == 0x28);
static_assert(offsetof(ReflectorBoardParams, reserved2c) == 0x2c);
#else
static_assert(sizeof(ReflectorBoardParams) == 0x2c);
static_assert(offsetof(ReflectorBoardParams, scale) == 0x04);
static_assert(offsetof(ReflectorBoardParams, colour) == 0x08);
static_assert(offsetof(ReflectorBoardParams, sector) == 0x14);
static_assert(offsetof(ReflectorBoardParams, mode) == 0x18);
static_assert(offsetof(ReflectorBoardParams, lightType) == 0x1c);
static_assert(offsetof(ReflectorBoardParams, lightAngleOrVisible) == 0x20);
static_assert(offsetof(ReflectorBoardParams, customLight) == 0x24);
static_assert(offsetof(ReflectorBoardParams, reserved28) == 0x28);
#endif
#pragma pack(pop)

extern Vector4 reflectorDefaultValues;
void SetReflectorBoardDefaultValues(const Vector4 &values);
void ResetReflectorBoardMode(Character *character, std::int32_t sector);

void AssignReflectorBoardParams(Character *character);
bool AttachReflectorBoardComponent(Character *character, std::int32_t sector = 1);

bool ReadReflectorBoardState(const Character *character, std::int32_t &sector, Vector4 &values);
bool SetReflectorBoardMode(Character *character, std::int32_t mode);
ReflectorBoardParams *SetReflectorBoardValues(Character *character, const Vector4 &values);
bool SetReflectorBoardLightMode(Character *character, bool active, std::uint32_t lightMode, std::int32_t lightAngle);

} // namespace shenmue::s2
