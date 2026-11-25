#pragma once

#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_dc.hpp"
#else
#include "s2/model_component_types.hpp"
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define CHARACTER_OBJECT_TRANSFORMS SHEN_NAME(FIXO)
#endif

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

#if SHENMUE_S2_GCC295
typedef char ObjectXformsLayout1087[(__alignof__(ObjectXFormsParams) == 4) ? 1 : -1];
#else
static_assert(alignof(ObjectXFormsParams) == 4);
#endif
typedef char ObjectXformsLayout1136[(offsetof(ObjectXFormsParams, position) == 0x00) ? 1 : -1];
typedef char ObjectXformsLayout1199[(offsetof(ObjectXFormsParams, rotation) == 0x0c) ? 1 : -1];
typedef char ObjectXformsLayout1262[(offsetof(ObjectXFormsParams, localPosition) == 0x18) ? 1 : -1];
typedef char ObjectXformsLayout1330[(offsetof(ObjectXFormsParams, localRotation) == 0x24) ? 1 : -1];
typedef char ObjectXformsLayout1398[(offsetof(ObjectXFormsParams, mode) == 0x30) ? 1 : -1];
typedef char ObjectXformsLayout1457[(offsetof(ObjectXFormsParams, transformFlags) == 0x32) ? 1 : -1];
typedef char ObjectXformsLayout1526[(offsetof(ObjectXFormsParams, secondaryPosition) == 0x34) ? 1 : -1];
typedef char ObjectXformsLayout1598[(offsetof(ObjectXFormsParams, secondaryRotation) == 0x40) ? 1 : -1];
#if SHENMUE_IS_PC
typedef char ObjectXformsLayout1688[(sizeof(ObjectXFormsSource) == 0x08) ? 1 : -1];
typedef char ObjectXformsLayout1739[(sizeof(ObjectXFormsParams) == 0x68) ? 1 : -1];
typedef char ObjectXformsLayout1790[(offsetof(ObjectXFormsParams, source) == 0x50) ? 1 : -1];
typedef char ObjectXformsLayout1851[(offsetof(ObjectXFormsParams, sourceCharacter) == 0x58) ? 1 : -1];
typedef char ObjectXformsLayout1921[(offsetof(ObjectXFormsParams, multiplySourceAlpha) == 0x60) ? 1 : -1];
#else
typedef char ObjectXformsLayout2001[(sizeof(ObjectXFormsSource) == 0x04) ? 1 : -1];
typedef char ObjectXformsLayout2052[(sizeof(ObjectXFormsParams) == 0x58) ? 1 : -1];
typedef char ObjectXformsLayout2103[(offsetof(ObjectXFormsParams, source) == 0x4c) ? 1 : -1];
typedef char ObjectXformsLayout2164[(offsetof(ObjectXFormsParams, sourceCharacter) == 0x50) ? 1 : -1];
typedef char ObjectXformsLayout2234[(offsetof(ObjectXFormsParams, multiplySourceAlpha) == 0x54) ? 1 : -1];
#endif
#pragma pack(pop)

bool AttachObjectXFormsComponent(Character *character);
std::int16_t FindFixo(Character *character);

void SetFixoDataForMotionWork(Character *target, Character *source, std::int32_t boneIndex, const Vector3 &position,
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::int32_t (&rotation)[3]
#else
    const std::array<std::int32_t, 3> &rotation
#endif
);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
ObjectXFormsParams *SetFixoAlphaPropagation(Character *character, std::int32_t enabled);
#else
ModelComponentHeader *SetFixoAlphaPropagation(Character *character, std::int32_t enabled);
#endif
void CaptureFixoRelativePlacement(Character *character, ObjectXFormsParams *attachment, const Matrix4x4 *source);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
ObjectXFormsParams *SetFixoLocalRotation(Character *character, const Vector3i *rotation);
ObjectXFormsParams *SetFixoLocalPosition(Character *character, const Vector3 *position);
#else
std::uint64_t SetFixoLocalRotation(Character *character, const Vector3i *rotation);
std::uint64_t SetFixoLocalPosition(Character *character, const Vector3 *position);
#endif
std::int64_t SetFixoInverseChildPlacement(Character *character, std::uint16_t childId);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RefreshFixoRelativePlacement(Character *character, const Matrix4x4 *transform);
#endif
void AttachFixoPreservingPlacement(Character *target, Character *source, std::int32_t boneIndex);
void SetFixoInversePositionRotation(Character *character, const Vector3 *position, const Vector3i *rotation);
ObjectXFormsParams *ResetCharaFixo(Character *character);
void DetachObjectXFormsDependents(Character *source);

} // namespace s2
} // namespace shenmue
