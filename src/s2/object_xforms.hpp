#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

#pragma pack(push, 4)
#pragma pack(push, 4)
union ObjectXFormsSource {
    const Matrix4x4 *transform;
    struct {
        std::int32_t boneIndex;
#if SHENMUE_IS_PC
        std::uint32_t reserved54;
#endif
    } motion;
    std::uintptr_t raw;
};

struct ObjectXFormsParams {
    Vector3 position;
    std::int32_t rotation[3];
    Vector3 localPosition;
    std::int32_t localRotation[3];
    std::int16_t mode;
    std::uint8_t transformFlags;
    std::uint8_t reserved33;
    Vector3 secondaryPosition;
    std::int32_t secondaryRotation[3];
#if SHENMUE_IS_PC
    std::uint32_t reserved4c;
#endif
    ObjectXFormsSource source;
    Character *sourceCharacter;
    std::uint32_t multiplySourceAlpha;
#if SHENMUE_IS_PC
    std::uint32_t reserved64;
#endif
};
#pragma pack(pop)

static_assert(alignof(ObjectXFormsParams) == 4);
static_assert(offsetof(ObjectXFormsParams, position) == 0x00);
static_assert(offsetof(ObjectXFormsParams, rotation) == 0x0c);
static_assert(offsetof(ObjectXFormsParams, localPosition) == 0x18);
static_assert(offsetof(ObjectXFormsParams, localRotation) == 0x24);
static_assert(offsetof(ObjectXFormsParams, mode) == 0x30);
static_assert(offsetof(ObjectXFormsParams, transformFlags) == 0x32);
static_assert(offsetof(ObjectXFormsParams, secondaryPosition) == 0x34);
static_assert(offsetof(ObjectXFormsParams, secondaryRotation) == 0x40);
#if SHENMUE_IS_PC
static_assert(sizeof(ObjectXFormsSource) == 0x08);
static_assert(sizeof(ObjectXFormsParams) == 0x68);
static_assert(offsetof(ObjectXFormsParams, source) == 0x50);
static_assert(offsetof(ObjectXFormsParams, sourceCharacter) == 0x58);
static_assert(offsetof(ObjectXFormsParams, multiplySourceAlpha) == 0x60);
#else
static_assert(sizeof(ObjectXFormsSource) == 0x04);
static_assert(sizeof(ObjectXFormsParams) == 0x58);
static_assert(offsetof(ObjectXFormsParams, source) == 0x4c);
static_assert(offsetof(ObjectXFormsParams, sourceCharacter) == 0x50);
static_assert(offsetof(ObjectXFormsParams, multiplySourceAlpha) == 0x54);
#endif
#pragma pack(pop)

bool AttachObjectXFormsComponent(Character *character);
std::int16_t FindFixo(Character *character);

void SetFixoDataForMotionWork(
    Character *target, Character *source, std::int32_t boneIndex, const Vector3 &position, const std::array<std::int32_t, 3> &rotation);
ModelComponentHeader *SetFixoAlphaPropagation(Character *character, std::int32_t enabled);
void CaptureFixoRelativePlacement(Character *character, ObjectXFormsParams *attachment, const Matrix4x4 *source);
std::uint64_t SetFixoLocalRotation(Character *character, const Vector3i *rotation);
std::uint64_t SetFixoLocalPosition(Character *character, const Vector3 *position);
std::int64_t SetFixoInverseChildPlacement(Character *character, std::uint16_t childId);
void AttachFixoPreservingPlacement(Character *target, Character *source, std::int32_t boneIndex);
void SetFixoInversePositionRotation(Character *character, const Vector3 *position, const Vector3i *rotation);
ObjectXFormsParams *ResetCharaFixo(Character *character);
void DetachObjectXFormsDependents(Character *source);

} // namespace shenmue::s2
