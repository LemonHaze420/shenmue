#include "s2/search_internal.hpp"
#include "s2/search_prop_ik_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {
std::array<SearchPropIKConfig, 15> propConfigurations = std::bit_cast<std::array<SearchPropIKConfig, 15>>(searchPropIKConfigurationBytes);
std::array<std::uint8_t, 228 * 11> propPoseCommands = searchPropIKPoseBytes;
std::int16_t propAngles[2]{};
Vector3 propWristAngles{};
Matrix4x4 propArmTransform{};
Vector3 propSinglePivot{.02f, 0, .03f};
Vector3 propPairedPivot{.02f, 0, .03f};
constexpr shen_name examineButton{SHEN_NAME(BTEV)};
Matrix4x4 *Matrices(SearchPropIKSlot &slot) {
    return reinterpret_cast<Matrix4x4 *>(slot.transformData);
}
Matrix4x4 *Targets(SearchPropIKSlot &slot) {
    return reinterpret_cast<Matrix4x4 *>(slot.auxiliaryData);
}
SearchPropIKConfig &Config(SearchPropIKSlot &slot) {
    return *static_cast<SearchPropIKConfig *>(slot.character);
}
Vector3 Translation() {
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}
Vector3 TransformVector(const Vector3 &point) {
    const Vector4 *const r = workTransformMatrix.rows;
    return {(point.y * r[1].x + point.x * r[0].x) + point.z * r[2].x, (point.y * r[1].y + point.x * r[0].y) + point.z * r[2].y,
        (point.y * r[1].z + point.x * r[0].z) + point.z * r[2].z};
}
void Translate(const Vector3 &point) {
    Matrix4x4::preTranslate(point.x, point.y, point.z);
}
void SwizzleBasis(const Matrix4x4 &matrix) {
    const float *const m = reinterpret_cast<const float *>(&matrix);
    workTransformMatrix.rows[0] = {m[0], m[1], m[2], 0};
    workTransformMatrix.rows[1] = {m[4], m[5], m[6], 0};
    workTransformMatrix.rows[2] = {m[8], m[9], m[10], 0};
}
std::int16_t Angle(float x, float z) {
    return static_cast<std::int16_t>(atan2_fixed16(x, z));
}
Character *Player() {
    return static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
}
MT7Node *MeshNode(MT7Node *node) {
    while (!node->xb01Offset)
        node = node->runtimeChild;
    return node;
}
void SetMarkerVisible(MT7Node &node, bool visible) {
    node.id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(node.id) & 0xffffU) | (visible ? 0x10000U : 0U));
}
void ApplyStagePose(SearchPropIKSlot &slot, std::uint32_t stage) {
    SearchPropIKConfig &config = Config(slot);
    if (config.stages[stage].jointTable < 0)
        return;
    Vector3 offset = config.gripOffset;
    if (slot.leftSide != 1) {
        srchSetJointTable(config.stages[stage].jointTable, 0);
        srchSetXformFor(&offset, 0);
    }
    if (slot.leftSide != 0) {
        offset.z = -offset.z;
        srchSetJointTable(config.stages[stage].jointTable, 1);
        srchSetXformFor(&offset, 1);
    }
}
void GetStageTangents(SearchPropIKSlot &slot, std::uint32_t stage, Vector3 &right, Vector3 &left) {
    Matrix4x4_Push();
    const SearchPropIKStage &row = Config(slot).stages[stage];
    right = {row.axis == 0 ? row.targetOffset : 0, row.axis == 1 ? row.targetOffset : 0, row.axis == 2 ? row.targetOffset : 0};
    left = {-right.x, right.y, right.z};
    Matrix4x4::SetActiveTransform(Matrices(slot));
    right = TransformVector(right);
    left = TransformVector(left);
    Matrix4x4::Pop(1);
}
void CaptureArmRelativeTransform(SearchPropIKSlot &slot) {
    Matrix4x4 *const matrices = Matrices(slot);
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&matrices[(slot.active & 0x20U) ? 1 : 0]);
    if (!(slot.active & 0x20U))
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(mtwkGetSearchArmTransform(slot.leftSide == 1));
    matrices[4] = workTransformMatrix;
    Matrix4x4::Pop(1);
}
void BuildPropTransforms(SearchPropIKSlot &slot) {
    Matrix4x4 *const matrices = Matrices(slot);
    SearchPropIKConfig &config = Config(slot);
    Matrix4x4_Push(&transformMatrix_prePass);
    srchApplyPropHierarchyTransform(*slot.parent);
    matrices[0] = workTransformMatrix;
    if (slot.parent != slot.node)
        srchApplyObjectTransform(slot.node, nullptr);
    matrices[2] = workTransformMatrix;
    Matrix4x4_Push(&transformMatrix_prePass);
    if (slot.node->runtimeChild && IsMDPXorMDP7(slot.node) == IsMDPXorMDP7(slot.node->runtimeChild))
        srchApplyObjectTransform(slot.node->runtimeChild, nullptr);
    matrices[3] = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
    for (std::size_t row = 0; row < 3; ++row) {
        matrices[0].rows[row].x = workTransformMatrix.rows[row].x;
        matrices[0].rows[row].y = workTransformMatrix.rows[row].y;
        matrices[0].rows[row].z = workTransformMatrix.rows[row].z;
    }
    if (slot.leftSide == 2) {
        Vector3 point = Translation();
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        Matrix4x4::TransformPoint(point, point);
        point.x = 0;
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        Matrix4x4::TransformPoint(point, point);
        Matrix4x4::Pop(1);
        matrices[0].rows[3].x = point.x;
        matrices[0].rows[3].y = point.y;
        matrices[0].rows[3].z = point.z;
    }
    Matrix4x4_Push(&matrices[0]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    matrices[2] = workTransformMatrix;
    Matrix4x4::Pop(1);
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    if (type == 6 || type == 9) {
        Matrix4x4_Push(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        const Vector3 origin{};
        Vector3 point;
        Matrix4x4::TransformPoint(origin, point);
        Matrix4x4::Pop(1);
        config.stages[0].position.x = -point.x;
        config.endpoints[1].x = -point.x - config.endpoints[0].x;
    }
    for (std::size_t i = 0; i < 4; ++i) {
        const SearchPropIKStage &stage = config.stages[i];
        Matrix4x4_Push();
        Translate(stage.position);
        if (stage.rotation[2])
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(stage.rotation[2]));
        if (stage.rotation[1])
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(stage.rotation[1]));
        if (stage.rotation[0])
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(stage.rotation[0]));
        Targets(slot)[i] = workTransformMatrix;
        Matrix4x4::Pop(1);
    }
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(Targets(slot));
    matrices[5] = workTransformMatrix;
    Matrix4x4 mirrored = matrices[5];
    mirrored.rows[3].x = -mirrored.rows[3].x;
    Matrix4x4::SetActiveTransform(&matrices[5]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(&mirrored);
    matrices[5] = workTransformMatrix;
    Matrix4x4::Pop(1);
}
void SetupPropHandCamera(SearchPropIKSlot &slot) {
    Character *const player = Player();
    MotionWork *const motion = FindMotionWork(player);
    if (!player || !motion)
        return;
    Matrix4x4 *const matrices = Matrices(slot);
    SearchPropIKConfig &config = Config(slot);
    const bool reverse = (slot.active & 0x20U) != 0;
    slot.phase = Angle(matrices[0].rows[2].x, matrices[0].rows[2].z);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetActiveTransform(&matrices[0]);
    if (slot.leftSide != 2) {
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        SwizzleBasis(matrices[0]);
    }
    Translate(config.endpoints[1]);
    Vector3 position = Translation();
    Matrix4x4::Pop(1);
    slot.resource = GetCharacterHand(player, 0);
    slot.owner = GetCharacterHand(player, 1);
    const std::int32_t *const angles = reinterpret_cast<const std::int32_t *>(&config) + 2;
    Vector3i rotation{angles[reverse ? 2 : 0], static_cast<std::int16_t>(slot.phase + angles[reverse ? 3 : 1]), 0};
    SetSearchHandOverrideEnabled(true);
    SetSearchHandChestTransform(&position, &rotation);
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    if (type == 6 || type == 9) {
        position = {(config.endpoints[0].x + config.endpoints[1].x) * .5f, (config.endpoints[0].y + config.endpoints[1].y) * .5f,
            (config.endpoints[0].z + config.endpoints[1].z) * .5f};
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        SwizzleBasis(matrices[0]);
        Translate(position);
        position = Translation();
        Matrix4x4::Pop(1);
        rotation.x = (angles[2] - angles[0]) / 2 + angles[0];
        rotation.y = static_cast<std::int16_t>(slot.phase + angles[1] + (angles[3] - angles[1]) / 2);
    } else if (type == 7 || type == 8 || type == 15) {
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        if (slot.leftSide != 2) {
            Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
            Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
            Translate(config.stages[0].position);
            SwizzleBasis(matrices[0]);
        }
        Translate(config.endpoints[0]);
        position = Translation();
        Matrix4x4::Pop(1);
        rotation = {angles[0], static_cast<std::int16_t>(slot.phase + angles[1]), 0};
    }
    Matrix4x4_Push(&transformMatrix_prePass);
    Translate(position);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    Matrix4x4::preTranslate(0, .4f, -.1f);
    const Vector3 target = Translation();
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    const Vector3 base = Translation();
    Matrix4x4::Pop(1);
    srchSetLookBase(&base);
    srchSetLookTarget(&target);
    srchSetRotY(static_cast<std::int16_t>(rotation.y + 0x8000));
    camCtrlSwitchToLockedOn(6);
    EV_SetCharDisplay(player, CVM_DisplayEnabled, 0);
}
void UpdatePropChestAndCamera(SearchPropIKSlot &slot) {
    SearchPropIKConfig &config = Config(slot);
    Matrix4x4 *const matrices = Matrices(slot);
    Vector3 endpoints[2]{config.endpoints[0], config.endpoints[1]};
    Matrix4x4_Push(&matrices[0]);
    if (slot.leftSide != 2) {
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        SwizzleBasis(matrices[0]);
    }
    Matrix4x4::TransformPoint(endpoints[0], endpoints[0]);
    Matrix4x4::TransformPoint(endpoints[1], endpoints[1]);
    const std::int32_t total = srchGetCurrentTotalTicks(slot.leftSide == 1);
    const float remaining = total ? static_cast<float>(srchGetTicksDifference(slot.leftSide == 1)) / total : 0;
    float blend = std::fmax(0, std::fmin(1, remaining));
    if ((slot.active & 0x20U) ? (slot.active & 2U) != 0 : (slot.active & 6U) == 0)
        blend = 1 - blend;
    if (slot.active & 4U)
        blend = 1;
    blend = blend < .5f ? blend * (blend + blend) : (blend * 4 - (blend + blend) * blend) - 1;
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    Vector3 position;
    if (type == 8 || type == 15) {
        if (!(slot.active & 6U))
            blend = 0;
        endpoints[0] = config.endpoints[0];
        endpoints[1] = config.endpoints[1];
        position = {(1 - blend) * endpoints[0].x + blend * endpoints[1].x, (1 - blend) * endpoints[0].y + blend * endpoints[1].y,
            (1 - blend) * endpoints[0].z + blend * endpoints[1].z};
        Matrix4x4_Push(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        Vector3 pivot = Translation();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>((config.stages[3].position.z * 8192.0f / 45.0f) * blend));
        Matrix4x4::TransformPoint(pivot, pivot);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Translate(pivot);
        Matrix4x4::TransformPoint(position, position);
        Matrix4x4::Pop(1);
    } else
        position = {(1 - blend) * endpoints[0].x + blend * endpoints[1].x, (1 - blend) * endpoints[0].y + blend * endpoints[1].y,
            (1 - blend) * endpoints[0].z + blend * endpoints[1].z};
    if (!(slot.active & 6U))
        blend = 0;
    const std::int32_t *const angles = reinterpret_cast<const std::int32_t *>(&config) + 2;
    Vector3i rotation{static_cast<std::int32_t>(angles[0] * (1 - blend) + angles[2] * blend),
        static_cast<std::int16_t>(slot.phase + static_cast<std::int32_t>(angles[1] * (1 - blend) + angles[3] * blend)), 0};
    if ((type < 6 || type > 9) && type != 15) {
        Matrix4x4::SetPrePassToTempTransform();
        Translate(position);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
        Matrix4x4::preTranslate(0, .4f, -.1f);
        const Vector3 look = Translation();
        srchSetLookTarget(&look);
    }
    Matrix4x4::Pop(1);
    SetSearchHandChestTransform(&position, &rotation);
}
void CapturePropPivot(SearchPropIKSlot &slot) {
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&Matrices(slot)[1]);
    Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[3]);
    Translate(Config(slot).stages[0].position);
    const Vector3 point = Translation();
    Targets(slot)[2].rows[3].x = point.x;
    Targets(slot)[2].rows[3].y = point.y;
    Targets(slot)[2].rows[3].z = point.z;
    Matrix4x4::Pop(1);
}
void UpdatePropFromArm(SearchPropIKSlot &slot) {
    SearchPropIKConfig &config = Config(slot);
    Matrix4x4 *const matrices = Matrices(slot);
    const std::uint32_t mode = config.flags;
    Vector3 point{}, base;
    Matrix4x4_Push(&transformMatrix_prePass);
    if ((mode & 0x38U) == 0x10U) {
        Matrix4x4::SetActiveTransform(&matrices[4]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        const Matrix4x4 inverse = workTransformMatrix;
        Matrix4x4::SetActiveTransform(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        point = Translation();
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        Matrix4x4::TransformPoint(point, point);
        Matrix4x4::SetActiveTransform(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Vector3 origin = Translation();
        point.y = origin.y = 0;
        const std::int32_t yaw = Angle(-point.x, -point.z) - Angle(-origin.x, -origin.z);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
        config.stages[3].targetOffset = -(yaw / 182.04445f);
        point = {};
    } else {
        Matrix4x4::SetActiveTransform(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[4]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        const Matrix4x4 inverse = workTransformMatrix;
        Matrix4x4::SetActiveTransform(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        if (!(mode & 0x38U))
            SwizzleBasis(matrices[0]);
        point = Translation();
    }
    matrices[1] = workTransformMatrix;
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::TransformPoint(point, point);
    if (!(mode & 1U))
        point.x = 0;
    if (!(mode & 2U))
        point.y = 0;
    if (!(mode & 4U))
        point.z = 0;
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::TransformPoint(point, point);
    if (!(mode & 0x40U)) {
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        base = Translation();
    }
    matrices[1].rows[3].x = point.x;
    matrices[1].rows[3].y = point.y;
    matrices[1].rows[3].z = point.z;
    Matrix4x4::SetActiveTransform(&matrices[1]);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    matrices[1] = workTransformMatrix;
    if (mode & 0x40U)
        base = Translation();
    Matrix4x4::Pop(1);
    const std::uint32_t active = g_searchPropIKSlots[searchPropIKIndex].active;
    if (((active & 1U) || !(active & 4U)) && !(slot.active & 0x400U)) {
        if (slot.leftSide == 2) {
            propWristAngles = {};
            Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
            propArmTransform = workTransformMatrix;
            Matrix4x4::SetActiveTransform(mtwkGetSearchArmTransform(1));
            propPairedPivot = Translation();
            propPairedPivot = {(propPairedPivot.x + propArmTransform.rows[3].x) * .5f, (propPairedPivot.y + propArmTransform.rows[3].y) * .5f,
                (propPairedPivot.z + propArmTransform.rows[3].z) * .5f};
            Matrix4x4::SetActiveTransform(&propArmTransform);
            const Vector3 offset{0, .1f, 0};
            const Vector3 rotated = TransformVector(offset);
            propArmTransform.rows[3].x = propPairedPivot.x + rotated.x;
            propArmTransform.rows[3].y = propPairedPivot.y + rotated.y;
            propArmTransform.rows[3].z = propPairedPivot.z + rotated.z;
            Matrix4x4::InvertInPlace(&workTransformMatrix);
            Matrix4x4::TransformPoint(propPairedPivot, propPairedPivot);
            propPairedPivot.y += .1f;
        } else {
            Vector3i wrist;
            GetSearchHandJointRotation(slot.leftSide == 1, &wrist);
            propWristAngles = {static_cast<float>(static_cast<std::int16_t>(wrist.x)), static_cast<float>(static_cast<std::int16_t>(wrist.y)),
                static_cast<float>(static_cast<std::int16_t>(wrist.z))};
            Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
            Translate(propSinglePivot);
            propArmTransform = workTransformMatrix;
        }
        Matrix4x4::Pop(1);
    }
    if (!(slot.active & 0x100U))
        srchSetLookBase(&base);
}
void RotateHeldProp(SearchTaskData &search, SearchPropIKSlot &slot) {
    const std::uint32_t active = g_searchPropIKSlots[searchPropIKIndex].active;
    if (((active & 1U) || !(active & 4U)) && !(slot.active & 0x400U))
        return;
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    if (type <= 0 || (type > 4 && (type < 11 || type > 14))) {
        slot.active &= ~0x400U;
        return;
    }
    std::int32_t configIndex = 0, maximum = 910, acceleration = 182;
    Vector3 pivot = propSinglePivot;
    if (type == 1 || type == 11 || type == 13)
        configIndex = 2;
    else if (type == 2 || type == 12) {
        configIndex = 4;
        pivot = propPairedPivot;
        maximum = 455;
        acceleration = 91;
    } else if (type == 3 || type == 4)
        configIndex = 1;
    else
        configIndex = 3;
    const std::uint8_t *const row = searchPropIKAxisBytes.data() + 22 * configIndex;
    std::int16_t velocities[2];
    std::memcpy(velocities, &searchPropIKResult, 4);
    const float wrist[3]{propWristAngles.x, propWristAngles.y, propWristAngles.z};
    const std::int16_t effective[2]{static_cast<std::int16_t>(propAngles[0] + static_cast<std::int16_t>(wrist[row[0]])),
        static_cast<std::int16_t>(propAngles[1] + static_cast<std::int16_t>(wrist[row[1]]))};
    if (slot.active & 0x400U) {
        for (std::size_t i = 0; i < 2; ++i) {
            if (std::abs(static_cast<std::int32_t>(propAngles[i])) <= maximum * 3) {
                if (std::abs(static_cast<std::int32_t>(velocities[i])) <= acceleration)
                    velocities[i] = 0;
                else
                    velocities[i] += velocities[i] > 0 ? -acceleration : acceleration;
            } else {
                velocities[i] += propAngles[i] > 0 ? -acceleration : acceleration;
                if (std::abs(static_cast<std::int32_t>(velocities[i])) > maximum)
                    velocities[i] = velocities[i] > 0 ? maximum : -maximum;
            }
        }
        if (!velocities[0] && !velocities[1] && std::abs(static_cast<std::int32_t>(propAngles[0])) <= maximum * 3 &&
            std::abs(static_cast<std::int32_t>(propAngles[1])) <= maximum * 3)
            slot.active &= ~0x400U;
    } else {
        for (std::size_t axis = 0; axis < 2; ++axis) {
            bool adjusting = false;
            for (std::size_t direction = 0; direction < 2; ++direction) {
                std::uint16_t button;
                std::int16_t limit;
                std::memcpy(&button, row + 6 + 4 * axis + 2 * direction, 2);
                std::memcpy(&limit, row + 14 + 4 * axis + 2 * direction, 2);
                if (!(search.input->held & button))
                    continue;
                if (row[2 + 2 * axis + direction]) {
                    if (effective[axis] >= limit)
                        continue;
                    velocities[axis] += acceleration;
                    if (velocities[axis] > maximum)
                        velocities[axis] = maximum;
                } else {
                    if (effective[axis] <= limit)
                        continue;
                    velocities[axis] -= acceleration;
                    if (velocities[axis] < -maximum)
                        velocities[axis] = -maximum;
                }
                adjusting = true;
            }
            if (!adjusting) {
                if (velocities[axis] > 0) {
                    velocities[axis] -= acceleration;
                    if (velocities[axis] < 0)
                        velocities[axis] = 0;
                }
                if (velocities[axis] < 0) {
                    velocities[axis] += acceleration;
                    if (velocities[axis] > 0)
                        velocities[axis] = 0;
                }
            }
        }
    }
    propAngles[0] += velocities[0];
    propAngles[1] += velocities[1];
    std::memcpy(&searchPropIKResult, velocities, 4);
    std::int16_t rotations[3]{};
    rotations[row[0]] = propAngles[0];
    rotations[row[1]] = propAngles[1];
    Matrix4x4 local;
    if (velocities[0] || velocities[1]) {
        mtwkApplyHand(HandIKMode::HoldTransform, slot.leftSide == 1);
        Matrix4x4_Push(&propArmTransform);
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotations[0]));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotations[2]));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotations[1]));
    } else {
        mtwkApplyHand(HandIKMode::Disabled, slot.leftSide == 1);
        Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Translate(pivot);
        local = workTransformMatrix;
        propArmTransform.rows[3].x = local.rows[3].x;
        propArmTransform.rows[3].y = local.rows[3].y;
        propArmTransform.rows[3].z = local.rows[3].z;
    }
    Matrix4x4::preTranslate(-pivot.x, -pivot.y, -pivot.z);
    Vector3 offset;
    get_x_from_target(&offset, slot.leftSide == 1);
    Translate(offset);
    local = workTransformMatrix;
    SetSearchHandCurrentTransform(&local, slot.leftSide == 1);
    Matrix4x4::Pop(1);
}
void PlaySelectionSound(std::size_t index) {
    std::uint32_t sound;
    std::uint16_t modifier;
    std::memcpy(&sound, searchPropIKSelection.data() + 6 * index, 4);
    std::memcpy(&modifier, searchPropIKSelection.data() + 6 * index + 4, 2);
    const std::uint32_t material = (sound >> 24U) | (32U * (modifier & 0xffU));
    Loop3DSoundEffect((sound & 0xffffU) | (material << 19U));
}
void FinishSlotTick(SearchPropIKSlot &slot) {
    if (slot.active & 2U) {
        UpdatePropFromArm(slot);
        if (slot.leftSide == 2)
            SearchIK_SetTarget(&Matrices(slot)[5]);
    }
    slot.active &= ~0x200U;
}
void SolveStageGrip(SearchTaskData &search, SearchPropIKSlot &slot, std::uint32_t stage) {
    if (!slot.resource || !slot.owner)
        return;
    constexpr shen_name gripMatrixMemory{SHEN_NAME(MDMT)};
    Matrix4x4 *const allocation = static_cast<Matrix4x4 *>(HLib::AllocateTaskData(64, gripMatrixMemory));
    if (slot.active & 6U)
        *allocation = Matrices(slot)[1];
    else {
        Matrix4x4_Push(Matrices(slot));
        Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[2]);
        *allocation = workTransformMatrix;
        Matrix4x4::Pop(1);
    }
    MT7Node *const mesh = MeshNode(slot.node);
    MT7Node temporary{};
    temporary.sourceOffset = mesh->sourceOffset;
    temporary.id = mesh->id;
    temporary.position = mesh->position;
    temporary.rotation = mesh->rotation;
    temporary.scale = mesh->scale;
    temporary.xb01Offset = mesh->xb01Offset;
    temporary.meshType = mesh->meshType;
    temporary.runtimeMesh = mesh->runtimeMesh;
    temporary.runtimeParent = mesh->runtimeParent;
    temporary.borrowedMesh = mesh->borrowedMesh ? mesh->borrowedMesh : mesh;
    SetMT7NodeTransformOverride(temporary, allocation);
    std::byte *const commands = reinterpret_cast<std::byte *>(propPoseCommands.data());
    std::byte *const begin = commands + 228 * Config(slot).pose;
    for (std::uint8_t side = 0; side < 2; ++side) {
        if ((side == 0 && slot.leftSide == 1) || (side == 1 && slot.leftSide == 0))
            continue;
        Vector3 offset;
        get_x_from_target(&offset, side);
        if (!side && search.currentTarget->name.name == SHEN_NAME(M113))
            offset.z += .02f;
        Matrix4x4_Push(&Targets(slot)[stage]);
        if (side && slot.leftSide == 2)
            Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[5]);
        Matrix4x4::preTranslate(-offset.x, -offset.y, -offset.z);
        const Matrix4x4 local = workTransformMatrix;
        Matrix4x4::Pop(1);
        std::byte *const output = commands + 228 * side;
        std::memcpy(output, begin + 228, 228);
        SolveHandGripPose(static_cast<HandParams *>(side ? slot.owner : slot.resource), &local, &temporary, allocation, 0, begin, output);
        srchSetSearchJointAngles(side, begin, output, 0);
    }
    HLib::FreeTaskData(allocation);
}
}

// @verified - p
std::int32_t srchGetPropIKLookup(std::uint16_t marker) {
    if (static_cast<std::uint8_t>(marker) != 'M')
        return -1;
    const std::int32_t character = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(marker >> 8U));
    std::int32_t digit = character - '0';
    if (static_cast<std::uint32_t>(digit) > 9U)
        digit = character - 55;
    for (std::int32_t index = 0; index < 15; ++index)
        if (searchPropIKLookup[index] == digit)
            return index;
    return -1;
}

// @verified - p
void srchBindPropIK(SearchTaskData &search) {
    const shen_name name = search.currentTarget->name;
    if (static_cast<std::uint8_t>(name.name) != 'M')
        return;
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    slot.state = std::bit_cast<std::int32_t>(name.name);
    slot.parent = FindModelCharacterRecursive(GetPropRoot(), name);
    slot.node = slot.parent->xb01Offset ? slot.parent : slot.parent->runtimeChild;
    slot.lookupIndex = srchGetPropIKLookup(static_cast<std::uint16_t>(name.name));
    if (slot.lookupIndex == -1)
        return;
    propAngles[0] = propAngles[1] = 0;
    searchPropIKResult = 0;
    slot.character = &propConfigurations[slot.lookupIndex];
    slot.flags = 0;
    slot.active = 65;
    SetPlayerControlEnabled(0);
}

// @verified - p
void srchClearPropIKSlot(std::int32_t index) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[index];
    if (slot.node) {
        if (searchPropIKLookup[slot.lookupIndex] == 7 && (static_cast<std::uint32_t>(slot.node->id) >> 16U))
            SetMarkerVisible(*slot.node, false);
        if (slot.node->xb01Offset)
            SetMT7NodeTransformOverride(*slot.node, nullptr);
    }
    slot.flags = 0;
    slot.active = 0;
    slot.node = nullptr;
    slot.character = nullptr;
    slot.resource = nullptr;
    slot.owner = nullptr;
    slot.frame = 0;
}

// @verified - p
bool srchPushPropIK(SearchTaskData &search) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    if (searchPropIKIndex == 3 || !slot.parent->runtimeChild)
        return false;
    MT7Node *const child = mtFindChildRecursive(slot.parent->runtimeChild, std::bit_cast<std::uint32_t>(slot.state));
    if (!child)
        return false;
    slot.frame = reinterpret_cast<std::uintptr_t>(search.context->cursor);
    srchClearPropIKSlot(++searchPropIKIndex);
    srchRegisterAndPush(child);
    search.fovStyle = 7;
    srchLockTarget(search, search.targets[3]->name, false);
    return true;
}

// @verified - p
bool srchPopPropIK(SearchTaskData &search) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex - 1];
    MT7Node *const parent = slot.parent->runtimeParent;
    if (!parent)
        return false;
    MT7Node *const child = mtFindChildRecursive(parent, static_cast<std::uint32_t>(IsMDPXorMDP7(parent)));
    if (!child)
        return false;
    srchRegisterAndPush(child);
    srchClearPropIKSlot(searchPropIKIndex);
    --searchPropIKIndex;
    search.context->cursor = reinterpret_cast<std::byte *>(slot.frame);
    const std::span<const std::byte> config = srchGetConfigFor(search, shen_name{std::bit_cast<std::uint32_t>(slot.state)});
    srchReadSelectionSounds(config.data());
    return true;
}

// @verified - p
void srchBeginPropIKReturn() {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    slot.active |= 0x401U;
    slot.flags = 0;
    if (!(slot.active & 2U))
        slot.active |= 0x40U;
}

// @verified - p
void srchFinishPropIKItem(bool animate) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    if (animate) {
        for (std::uint8_t side = 0; side < 2; ++side) {
            if ((side == 0 && slot.leftSide == 1) || (side == 1 && slot.leftSide == 0))
                continue;
            srchSetJointTable(3, side);
            mtwkApplyHand(HandIKMode::Unblended, side);
            srchSetNewTicksTimeFor(30, side);
            mtwkApplyHand(HandIKMode::PathFollow, side);
        }
        slot.active |= 0x108U;
    } else {
        mtwkResetBlends();
        SetMT7NodeTransformOverride(*MeshNode(slot.node), nullptr);
        SetMarkerVisible(*slot.node, true);
    }
    slot.flags = 6;
}

// @verified - p
void srchReadSelectionSounds(const std::byte *config) {
    for (std::size_t index = 0; index < 3; ++index) {
        std::memset(searchPropIKSelection.data() + 6 * index, 0, 5);
        if (config)
            std::memcpy(searchPropIKSelection.data() + 6 * index, config + 16 + 6 * index, 6);
    }
}

// @verified - p
bool srchUpdatePropIK(SearchTaskData &search) {
    SearchPropIKSlot *slot = &g_searchPropIKSlots[searchPropIKIndex];
    setSearchIKFlag(std::fmin(1, SearchIK_GetMaxBlend() + 0.033333335f));
    if (slot->active & 8U) {
        const std::uint32_t mode = static_cast<std::uint32_t>(getSearchHandConfigModeFor(slot->leftSide == 1));
        if (((mode - 1U) & 0xfffffffbU) != 0) {
            slot->active &= ~8U;
            slot->active |= 0x200U;
        } else
            UpdatePropChestAndCamera(*slot);
        if (slot->active & 2U) {
            UpdatePropFromArm(*slot);
            if (slot->leftSide == 2)
                SearchIK_SetTarget(&Matrices(*slot)[5]);
        }
        return true;
    }
    if (!(slot->active & 0x101U)) {
        if (slot->active) {
            if ((search.input->pressedPriority & 0x200U) || searchExitRequested)
                srchBeginPropIKReturn();
            else {
                CheckHelpIcon(2, 0, CheckHelpIconOp::FrameOverlay);
                RotateHeldProp(search, *slot);
            }
        } else {
            if (searchPropIKIndex) {
                if ((search.input->pressedPriority & 0x200U) || searchExitRequested) {
                    srchPopPropIK(search);
                    slot = &g_searchPropIKSlots[searchPropIKIndex];
                    srchBeginPropIKReturn();
                    slot->active &= ~0x400U;
                } else
                    CheckHelpIcon(2, 0, CheckHelpIconOp::FrameOverlay);
            }
            if (!slot->active) {
                if (srchGetPropIKLookup(static_cast<std::uint16_t>(search.currentTarget->name.name)) != -1 && !(search.state & 8U)) {
                    if (btnCheck(examineButton))
                        srchBindPropIK(search);
                    else
                        RegisterButtonMediation(examineButton, nullptr, (ButtonMediationShowButtonHint | ButtonMediationBindA));
                }
                if (!slot->active)
                    return false;
            }
        }
    }
    if (slot->active & 0x400U) {
        RotateHeldProp(search, *slot);
        if (slot->active & 2U) {
            UpdatePropFromArm(*slot);
            if (slot->leftSide == 2)
                SearchIK_SetTarget(&Matrices(*slot)[5]);
        }
        return true;
    }
    if (slot->active & 0x40U) {
        Character *const player = Player();
        MotionWork *const work = FindMotionWork(player);
        if (SetMotionRegionFlag(work, 66, 1, MotionRegion::LowerBody)) {
            slot->leftSide = Config(*slot).hand;
            if (!(slot->active & 0x20U))
                BuildPropTransforms(*slot);
            else {
                const std::int32_t type = searchPropIKLookup[slot->lookupIndex];
                if (type == 6)
                    slot->leftSide = 1;
                else if (type == 7)
                    SetMarkerVisible(*slot->node, false);
                else if (type == 9)
                    slot->leftSide = 0;
            }
            if (BeginSearchHand(player, slot->leftSide, 0)) {
                setSearchIKFlag(0);
                SetupPropHandCamera(*slot);
                slot->active &= ~0x40U;
            }
            SetDefaultVS();
            lightConfigurationChanged = 1;
            UpdatePropChestAndCamera(*slot);
        } else {
            mtwkRequestMotion(work, 0xf002, MotionRegion::LowerBody);
            SetMotionRegionFlag(work, 71, 0, MotionRegion::LowerBody);
        }
        return true;
    }
    SearchPropIKConfig &config = Config(*slot);
    const std::int32_t type = searchPropIKLookup[slot->lookupIndex];
    std::byte *const commands = reinterpret_cast<std::byte *>(propPoseCommands.data());
    std::byte *const pose = commands + 228 * config.pose;
    Matrix4x4 *const matrices = Matrices(*slot);
    switch (slot->flags) {
    case 0: {
        if (shenCheckCurrentFoVModeIs2())
            return true;
        EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 1);
        mtwkSetOverridesEnabled(0);
        ApplyStagePose(*slot, (slot->active & 0x20U) ? 2 : 0);
        for (std::uint8_t side = 0; side < 2; ++side)
            if ((side == 0 && slot->leftSide != 1) || (side == 1 && slot->leftSide != 0))
                mtwkApplyHand(HandIKMode::Unblended, side);
        if (!(slot->active & 2U))
            WriteHandPoseCommand(*static_cast<HandParams *>(slot->resource), commands + 456);
        const std::uint32_t stage = (slot->active & 0x20U) ? ((slot->active & 2U) | 4U) >> 1U : 0;
        Vector3 right, left;
        GetStageTangents(*slot, stage, right, left);
        if (slot->leftSide != 1) {
            const Matrix4x4 &target = Targets(*slot)[stage];
            Vector3 tangent{right.x + target.rows[3].x, right.y + target.rows[3].y, right.z + target.rows[3].z};
            srchSetTargetsWithParams(0, &target, &tangent, nullptr, config.stages[stage].ticks);
            mtwkApplyHand(HandIKMode::PathFollow, 0);
        }
        if (slot->leftSide && !(slot->leftSide == 2 && stage == 3)) {
            Matrix4x4 target = Targets(*slot)[stage];
            if (slot->leftSide == 2) {
                Matrix4x4_Push(&target);
                Matrix4x4::MultiplyWithPrePassMatrix(&matrices[5]);
                target = workTransformMatrix;
                Matrix4x4::Pop(1);
            }
            const Vector3 tangent{left.x + target.rows[3].x, left.y + target.rows[3].y, left.z + target.rows[3].z};
            srchSetTargetsWithParams(1, &target, &tangent, nullptr, config.stages[stage].ticks);
            mtwkApplyHand(HandIKMode::PathFollow, 1);
        }
        slot->active |= 8U;
        if ((slot->active & 0x22U) == 0x22U) {
            slot->active &= ~4U;
            slot->flags = 5;
            PlaySelectionSound(2);
        } else {
            slot->flags = 1;
            SolveStageGrip(search, *slot, stage);
        }
        break;
    }
    case 1: {
        const std::uint32_t stage = ((slot->active & 0x20U) | 0x10U) >> 4U;
        const SearchPropIKStage &row = config.stages[stage];
        if (row.axis <= 2) {
            Vector3 right, left;
            GetStageTangents(*slot, stage, right, left);
            const bool side = slot->leftSide == 1;
            const Matrix4x4 &target = Targets(*slot)[stage];
            const Vector3 &vector = side ? left : right;
            const Vector3 tangent{vector.x + target.rows[3].x, vector.y + target.rows[3].y, vector.z + target.rows[3].z};
            srchSetTargetsWithParams(side, &target, &tangent, nullptr, row.ticks);
        } else
            SetSearchHandRotatingTarget(
                slot->leftSide == 1, &Targets(*slot)[stage], matrices, row.axis - 3, static_cast<std::uint16_t>(row.targetOffset * 8192.0f / 45.0f), row.ticks);
        for (std::uint8_t side = 0; side < 2; ++side)
            if ((side == 0 && slot->leftSide != 1) || (side == 1 && slot->leftSide != 0))
                mtwkApplyHand(HandIKMode::Unblended, side);
        mtwkApplyHand(HandIKMode::PathFollow, slot->leftSide == 1);
        slot->active = (slot->active & 0xfffffff1U) | 0xaU;
        SetMT7NodeTransformOverride(*MeshNode(slot->node), &matrices[1]);
        CaptureArmRelativeTransform(*slot);
        if (slot->active & 0x20U) {
            slot->parameter = 0;
            slot->flags = 5;
            PlaySelectionSound(2);
        } else {
            slot->flags = 2;
            PlaySelectionSound(0);
        }
        break;
    }
    case 2:
        if (config.flags & 0x40U)
            slot->active = (slot->active & 0xffffffcaU) | 0x34U;
        else {
            if (type == 0 || type == 6 || type == 8 || type == 9 || type == 15)
                CapturePropPivot(*slot);
            else if (type == 7)
                SetMarkerVisible(*slot->node, true);
            for (std::uint8_t side = 0; side < 2; ++side) {
                if ((side == 0 && slot->leftSide == 1) || (side == 1 && slot->leftSide == 0))
                    continue;
                if (type == 7)
                    srchSetJointTable(3, side);
                std::memcpy(commands + 228 * side, commands + 456, 228);
                srchSetNewTicksTimeFor(15, side);
                mtwkApplyHand(HandIKMode::PathFollow, side);
                srchSetSearchJointAngles(side, commands + 228 * side, nullptr, 1);
            }
            slot->flags = 3;
            slot->active = (slot->active | 0xcU) & ~2U;
        }
        break;
    case 3:
        if (slot->active & 0x200U) {
            slot->active = (slot->active & 0xffffffceU) | 0x30U;
            mtwkResetBlends();
            EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 0);
            if (!srchPushPropIK(search) && (type == 0 || type == 6 || type == 7 || type == 15))
                camCtrlSwitchToLockedOn(7);
        }
        break;
    case 5:
        if (type == 3)
            ApplyStagePose(*slot, 3);
        else if (type == 4 || type == 14) {
            if (slot->leftSide != 1)
                srchSetJointTable(3, 0);
            if (slot->leftSide != 0)
                srchSetJointTable(3, 1);
        }
        for (std::uint8_t side = 0; side < 2; ++side) {
            if ((side == 0 && slot->leftSide == 1) || (side == 1 && slot->leftSide == 0))
                continue;
            std::memcpy(commands + 228 * side, commands + 456, 228);
            srchSetNewTicksTimeFor(30, side);
            mtwkApplyHand(HandIKMode::PathFollow, side);
            srchSetSearchJointAngles(side, pose, commands + 228 * side, 1);
        }
        SetMT7NodeTransformOverride(*MeshNode(slot->node), nullptr);
        slot->active = (slot->active & 0xfffffff5U) | 8U;
        slot->flags = 7;
        PlaySelectionSound(1);
        break;
    case 6:
        mtwkResetBlends();
        EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 0);
        SetMT7NodeTransformOverride(*MeshNode(slot->node), nullptr);
        SetMarkerVisible(*slot->node, true);
        if (!(search.context->flags & 1U)) {
            if (searchPropIKIndex) {
                MT7Node *const parent = slot->node->runtimeParent;
                MT7Node *child = parent->runtimeChild;
                if (IsMDPXorMDP7(parent) == IsMDPXorMDP7(child))
                    child = child->runtimeSibling;
                if (srchRegisterAndPush(child)) {
                    srchClearPropIKSlot(searchPropIKIndex);
                    srchLockTarget(search, search.targets[3]->name, false);
                } else {
                    srchPopPropIK(search);
                    slot = &g_searchPropIKSlots[searchPropIKIndex];
                    srchBeginPropIKReturn();
                    slot->active |= 0x100U;
                }
            } else {
                search.state |= 2U;
                srchRegisterAndPush(slot->node->runtimeParent->runtimeChild);
                srchResetIK();
            }
        }
        break;
    case 7: {
        mtwkResetBlends();
        EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 0);
        const bool root = searchPropIKIndex == 0;
        if (root) {
            search.fovStyle = 1;
            srchSetLookTarget(nullptr);
            SetPlayerControlEnabled(1);
        }
        if (!srchLockTarget(search, shen_name{std::bit_cast<std::uint32_t>(slot->state)}, root))
            srchRequestRelease();
        srchClearPropIKSlot(searchPropIKIndex);
        if (!(slot->active & 0x80U))
            search.state |= 4U;
        slot->active &= ~0x80U;
        break;
    }
    default:
        break;
    }
    FinishSlotTick(*slot);
    return true;
}

std::int32_t srchGetCurrentIkIndex() {
    return searchPropIKIndex;
}
bool IsInvestigateMode() {
    return g_searchPropIKSlots[searchPropIKIndex].active != 0;
}

} // namespace shenmue::s2
