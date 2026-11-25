#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hand_pose_dc.hpp"

#include "platform/dc/runtime_arithmetic.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/hand_pose_data.hpp"
#include "s2/hand_dc.hpp"
#include <cstring>
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

using namespace model_component_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void mhndInit(MhndComponent *component);
void SetupHandPoseScript(Character *character);
MhndPoseScriptCallback SetupHandIdlePoseCallback(Character *character);
std::uint64_t GetHandShapeFor(Character *character, std::int32_t mode);
void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames);
void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames);

namespace model_component_detail {

void mhndBeginHand(
    MhndHand &hand, MT7Node *node, const std::array<Vector3i, 4> &current, const Vector3i *target, std::int32_t side, std::int32_t shape, std::int32_t frames);

} // namespace model_component_detail
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void ResetHandMeasurementBindings(Character *character) {
    MhndComponent *const component = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (!component)
        return;
    component->hands[0].node = nullptr;
    component->hands[1].node = nullptr;
    SetFlagForToken(character, kHandPoseToken, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames);

void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void mhndInit(MhndComponent *component);
void mhndReadRightHandPose(MT7Node *hand, Vector3i *pose);
void mhndReadLeftHandPose(MT7Node *hand, Vector3i *pose);
void mhndApplyRightHandPose(MT7Node *hand, const Vector3i *pose);
void mhndApplyLeftHandPose(MT7Node *hand, const Vector3i *pose);
void mhndBeginHand(
    MhndHand &hand, MT7Node *node, const Vector3i (&current)[4], const Vector3i *target, std::int32_t side, std::int32_t shape, std::int32_t frames);
std::int32_t mhndSetHandShape(MhndComponent *component, MT7Node *model, std::int32_t mode, std::int32_t shape, std::int32_t frames);
std::int32_t mhndSetHandPose(MhndComponent *component, MT7Node *model, std::int32_t mode, const Vector3i *pose, std::int32_t frames);
void mhndStepHand(MhndHand &hand, std::int32_t side);
void mhndRunPoseScript(Character *character, MhndPoseScript *script, MhndHand *hand);
void MHND_Callback(Character *character, void *data);
void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames);
void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames);
void AssignHandPoseScript(Character *character, std::int32_t mode, const std::int32_t *commands);
void SetupHandPoseScript(Character *character);
void mhndIdlePoseCallback(Character *character, MhndPoseScript *script, MhndHand *hand);
void SetupHandIdlePoseCallback(Character *character);
std::int32_t GetHandShapeFor(Character *character, std::int32_t mode);
void AllocateCharacterHandPose(Character *character);
void DisableHandPoseController(Character *character);
#else
void ResetHandMeasurementBindings(Character *character);
void mhndInit(MhndComponent *component);
void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames);
void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames);
void mhndSetupHuman(Character *character);
void SetupHandPoseScript(Character *character);
MhndPoseScriptCallback SetupHandIdlePoseCallback(Character *character);
std::uint64_t GetHandShapeFor(Character *character, std::int32_t mode);

namespace model_component_detail {

Vector3i NodeRotation(const MT7Node &node);
MT7Node *FindCharacterNode(Character *character, std::uint16_t id);
float MhndSegmentChainLength(Character *character, std::uint16_t startId, std::uint16_t endId, float inherited);
void MhndFindMeasurementMatrix(MT7Node *node, std::uint16_t id, Matrix4x4 *result);
float MhndMatrixDistance(Character *character, std::uint16_t startId, std::uint16_t endId);
void MhndRenumberSayLeftFingers(MT7Model *model);
void MhndMeasureHand(Character *character, std::int32_t handId, std::array<float, 6> &measurements);
void mhndReadHandPose(Character *character, MT7Node *hand, std::array<Vector3i, 4> *pose, const MhndHandNodes &nodes);
void mhndApplyHandPose(Character *character, MT7Node *hand, const Vector3i *pose, const MhndHandNodes &nodes);
std::int32_t mhndSignedRotation(std::int32_t rotation);
void mhndBeginHand(
    MhndHand &hand, MT7Node *node, const std::array<Vector3i, 4> &current, const Vector3i *target, std::int32_t side, std::int32_t shape, std::int32_t frames);
std::int32_t mhndSetHandShape(MhndComponent *mhnd, Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames);
std::int32_t mhndSetHandPose(MhndComponent *mhnd, Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames);
void mhndStepHand(Character *character, MhndHand &hand, std::int32_t side);
void mhndRunPoseScript(Character *character, MhndPoseScript *script, MhndHand *hand);
void mhndIdlePoseCallback(Character *character, MhndPoseScript *script, MhndHand *hand);
void MHND_Callback(Character *character, void *payload);

} // namespace model_component_detail
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void mhndSetPrimaryRotation(CharacterHandComponent *hand, std::int32_t x, std::int32_t y, std::int32_t z) {
    if (hand->primaryNode) {
        Vector3i rotation;
        rotation.x = x;
        rotation.y = y;
        rotation.z = z;
        SetHandPrimaryRotation(hand, rotation.x, rotation.y, rotation.z);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void mhndGetPrimaryRotation(const CharacterHandComponent *hand, Vector3i *rotation) {
    if (!hand->primaryNode) {
        rotation->z = 0;
        rotation->y = 0;
        rotation->x = 0;
    } else
        GetHandPrimaryNodeRotation(*hand, *rotation);
}
#endif
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndApplyRightHandPose(MT7Node *hand, const Vector3i *pose) {
    if (!pose || !hand)
        return;
    hand->rotation.x = (&pose->x)[0];
    hand->rotation.y = (&pose->x)[1];
    hand->rotation.z = (&pose->x)[2];
    MT7Node *node = FindMT7Node(hand, 0x19);
    if (node) {
        node->rotation.x = (&pose->x)[3];
        node->rotation.y = (&pose->x)[4];
        node->rotation.z = (&pose->x)[5];
        MT7Node *const tip = FindMT7Node(node, 0x1a);
        if (tip)
            tip->rotation.z = node->rotation.z;
    }
    node = FindMT7Node(hand, 0x1c);
    if (node) {
        node->rotation.y = (&pose->x)[6];
        node->rotation.z = (&pose->x)[7];
        MT7Node *const tip = FindMT7Node(node, 0x1d);
        if (tip)
            tip->rotation.y = (&pose->x)[8];
    }
    node = FindMT7Node(hand, 0x1f);
    if (node) {
        node->rotation.y = (&pose->x)[9];
        node->rotation.z = (&pose->x)[10];
        MT7Node *const tip = FindMT7Node(node, 0x20);
        if (tip)
            tip->rotation.y = (&pose->x)[11];
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndReadRightHandPose(MT7Node *hand, Vector3i *pose) {
    if (!pose || !hand)
        return;
    (&pose->x)[0] = hand->rotation.x;
    (&pose->x)[1] = hand->rotation.y;
    (&pose->x)[2] = hand->rotation.z;
    if (MT7Node *const first = FindMT7Node(hand, 0x19)) {
        (&pose->x)[3] = first->rotation.x;
        (&pose->x)[4] = first->rotation.y;
        (&pose->x)[5] = first->rotation.z;
    }
    if (MT7Node *const second = FindMT7Node(hand, 0x1c)) {
        (&pose->x)[6] = second->rotation.y;
        (&pose->x)[7] = second->rotation.z;
        if (MT7Node *const tip = FindMT7Node(second, 0x1d))
            (&pose->x)[8] = tip->rotation.y;
    }
    if (MT7Node *const third = FindMT7Node(hand, 0x1f)) {
        (&pose->x)[9] = third->rotation.y;
        (&pose->x)[10] = third->rotation.z;
        if (MT7Node *const tip = FindMT7Node(third, 0x20))
            (&pose->x)[11] = tip->rotation.y;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndApplyLeftHandPose(MT7Node *hand, const Vector3i *pose) {
    if (!pose || !hand)
        return;
    hand->rotation.x = (&pose->x)[0];
    hand->rotation.y = (&pose->x)[1];
    hand->rotation.z = (&pose->x)[2];
    MT7Node *node = FindMT7Node(hand, 0x28);
    if (node) {
        node->rotation.x = (&pose->x)[3];
        node->rotation.y = (&pose->x)[4];
        node->rotation.z = (&pose->x)[5];
        MT7Node *const tip = FindMT7Node(node, 0x1a);
        if (tip)
            tip->rotation.z = node->rotation.z;
    }
    node = FindMT7Node(hand, 0x2b);
    if (node) {
        node->rotation.y = (&pose->x)[6];
        node->rotation.z = (&pose->x)[7];
        MT7Node *const tip = FindMT7Node(node, 0x2c);
        if (tip)
            tip->rotation.y = (&pose->x)[8];
    }
    node = FindMT7Node(hand, 0x2e);
    if (node) {
        node->rotation.y = (&pose->x)[9];
        node->rotation.z = (&pose->x)[10];
        MT7Node *const tip = FindMT7Node(node, 0x2f);
        if (tip)
            tip->rotation.y = (&pose->x)[11];
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndReadLeftHandPose(MT7Node *hand, Vector3i *pose) {
    if (!pose || !hand)
        return;
    (&pose->x)[0] = hand->rotation.x;
    (&pose->x)[1] = hand->rotation.y;
    (&pose->x)[2] = hand->rotation.z;
    if (MT7Node *const first = FindMT7Node(hand, 0x28)) {
        (&pose->x)[3] = first->rotation.x;
        (&pose->x)[4] = first->rotation.y;
        (&pose->x)[5] = first->rotation.z;
    }
    if (MT7Node *const second = FindMT7Node(hand, 0x2b)) {
        (&pose->x)[6] = second->rotation.y;
        (&pose->x)[7] = second->rotation.z;
        if (MT7Node *const tip = FindMT7Node(second, 0x2c))
            (&pose->x)[8] = tip->rotation.y;
    }
    if (MT7Node *const third = FindMT7Node(hand, 0x2e)) {
        (&pose->x)[9] = third->rotation.y;
        (&pose->x)[10] = third->rotation.z;
        if (MT7Node *const tip = FindMT7Node(third, 0x2f))
            (&pose->x)[11] = tip->rotation.y;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndInit(MhndComponent *component) {
    std::memset(component, 0, sizeof(*component));
    component->hands[0].side = 0;
    component->hands[0].shape = -3;
    component->hands[1].side = 1;
    component->hands[1].shape = -3;
}

#else

// @verified - p
void mhndInit(MhndComponent *component) {
    std::memset(component, 0, sizeof(*component));
    component->hands[0].shape = -3;
    component->hands[1].side = 1;
    component->hands[1].shape = -3;
    for (std::size_t index = 0; index < component->hands[0].measurements.size(); ++index) {
        component->hands[1].measurements[index] = -1.0f;
        component->hands[0].measurements[index] = -1.0f;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void mhndBeginHand(
    MhndHand &hand, MT7Node *node, const Vector3i (&current)[4], const Vector3i *target, std::int32_t side, std::int32_t shape, std::int32_t frames) {
    model_component_detail::HandPoseJointArray *const currentPose = reinterpret_cast<model_component_detail::HandPoseJointArray *>(hand.current);
    model_component_detail::HandPoseJointArray *const targetPose = reinterpret_cast<model_component_detail::HandPoseJointArray *>(hand.target);
    hand.node = node;
    hand.side = side;
    hand.shape = shape;
    hand.frames = frames;
    *currentPose = *reinterpret_cast<const model_component_detail::HandPoseJointArray *>(current);
    *targetPose = *reinterpret_cast<const model_component_detail::HandPoseJointArray *>(target);
    const std::int32_t *const currentValues = &currentPose->values[0].x;
    const std::int32_t *const targetValues = &targetPose->values[0].x;
    std::int32_t *const stepValues = &hand.step[0].x;
    for (std::uint32_t index = 0; index < 12; ++index)
        stepValues[index] = (static_cast<std::int16_t>(targetValues[index]) - static_cast<std::int16_t>(currentValues[index])) / frames;
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndBeginHand(
    MhndHand &hand, MT7Node *node, const Vector3i (&current)[4], const Vector3i *target, std::int32_t side, std::int32_t shape, std::int32_t frames) {
    hand.node = node;
    hand.side = side;
    hand.shape = shape;
    hand.frames = frames;
    for (std::size_t joint = 0; joint < 4; ++joint)
        hand.current[joint] = current[joint];
    for (std::size_t index = 0; index < (sizeof(hand.target) / sizeof((hand.target)[0])); ++index)
        hand.target[index] = target[index];
    for (std::size_t index = 0; index < (sizeof(hand.step) / sizeof((hand.step)[0])); ++index) {
        for (std::size_t axis = 0; axis < 3; ++axis) {
            const std::int32_t difference = static_cast<std::int16_t>(hand.target[index][axis]) - static_cast<std::int16_t>(hand.current[index][axis]);
            hand.step[index][axis] = dc::SignedDivide(difference, frames);
        }
    }
}

#else

namespace model_component_detail {

Vector3i NodeRotation(const MT7Node &node) {
    return {static_cast<std::int32_t>(node.rotation.x), static_cast<std::int32_t>(node.rotation.y), static_cast<std::int32_t>(node.rotation.z)};
}

// Native GetChildObjectByIndex(CHAR_t::model, id).
MT7Node *FindCharacterNode(Character *character, std::uint16_t id) {
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    return model && !model->nodes.empty() ? GetChildObjectByIndex(*model, model->nodes.data(), id) : nullptr;
}

// The native chain helper carries XMM3 unchanged when either lookup fails. Only a found end
// node resets the accumulator; the start node's own translation is excluded.
// @verified - p
float MhndSegmentChainLength(Character *character, std::uint16_t startId, std::uint16_t endId, float inherited) {
    if (!character)
        return inherited;
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    MT7Node *const start = FindCharacterNode(character, startId);
    if (!start)
        return inherited;
    MT7Node *node = GetChildObjectByIndex(*model, start, endId);
    if (!node)
        return inherited;
    float length = 0.0f;
    while (node && node != start) {
        const Vector3 position = node->position;
        length += std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
        node = node->runtimeParent;
    }
    return length;
}

// The native matrix traversal visits every sibling/child even after a match, so the last matching
// node wins. It applies the raw MT7 TRS, not door motion or a draw transform override.
// @verified - p
void MhndFindMeasurementMatrix(MT7Node *node, std::uint16_t id, Matrix4x4 *result) {
    for (; node; node = node->runtimeSibling) {
        Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        if (static_cast<std::uint16_t>(node->id) == id)
            Matrix4x4::GetTransformMatrix(result);
        MhndFindMeasurementMatrix(node->runtimeChild, id, result);
        Matrix4x4::Pop(1);
    }
}

// The native distance helper starts at the hand's children under transformMatrix_prePass.
// The hand's own TRS is deliberately excluded, and both brackets restore ambient work/depth.
// @verified - p
float MhndMatrixDistance(Character *character, std::uint16_t startId, std::uint16_t endId) {
    if (!FindCharacterNode(character, endId) || !FindCharacterNode(character, startId))
        return -1.0f;
    MT7Node *const start = FindCharacterNode(character, startId);
    Matrix4x4 selected;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::GetTransformMatrix(&selected);
    MhndFindMeasurementMatrix(start->runtimeChild, endId, &selected);
    Matrix4x4::Pop(1);
    Matrix4x4_Push(&selected);
    const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    return std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
}

// SAY_'s left fingers are numbered structurally in both the live clone and its
// borrowed IMGM/HIMG/MDLM source through GetImageModuleSource.
void MhndRenumberSayLeftFingers(MT7Model *model) {
    if (!model || model->nodes.empty())
        return;
    MT7Node *const hand = GetChildObjectByIndex(*model, model->nodes.data(), mhndLeftHandNodes.hand);
    if (!hand)
        return;
    MT7Node *const first = hand->runtimeChild;
    MT7Node *const second = first ? first->runtimeSibling : nullptr;
    MT7Node *const third = second ? second->runtimeSibling : nullptr;
    if (first)
        first->id = (first->id & ~0xffff) | 40;
    if (second)
        second->id = (second->id & ~0xffff) | 43;
    if (second && second->runtimeChild)
        second->runtimeChild->id = (second->runtimeChild->id & ~0xffff) | 44;
    if (third)
        third->id = (third->id & ~0xffff) | 46;
    if (third && third->runtimeChild)
        third->runtimeChild->id = (third->runtimeChild->id & ~0xffff) | 47;
}

// Palm-to-second-tip matrix distance, three proximal chains,
// then the second and third distal chains. Missing lookups retain the native scalar.
// @verified - p
void MhndMeasureHand(Character *character, std::int32_t handId, std::array<float, 6> &measurements) {
    if (ctrlGetScenarioCharaName(character).name == SHEN_NAME(SAY_)) {
        MhndRenumberSayLeftFingers(static_cast<MT7Model *>(character->model));
        MhndRenumberSayLeftFingers(GetImageModuleSource(character));
    }
    const MhndHandNodes &nodes = handId == -65 ? mhndRightHandNodes : mhndLeftHandNodes;
    const std::uint16_t hand = static_cast<std::uint16_t>(handId);
    measurements[0] = MhndMatrixDistance(character, hand, nodes.secondTip);
    measurements[2] = MhndSegmentChainLength(character, hand, nodes.second, -1.0f);
    measurements[1] = MhndSegmentChainLength(character, hand, nodes.first, measurements[2]);
    measurements[3] = MhndSegmentChainLength(character, hand, nodes.third, measurements[2]);
    measurements[4] = MhndSegmentChainLength(character, nodes.second, nodes.secondTip, measurements[0] - measurements[2]);
    measurements[5] = MhndSegmentChainLength(character, nodes.third, nodes.thirdTip, measurements[4]);
}

// @verified - p
void mhndReadHandPose(Character *character, MT7Node *hand, std::array<Vector3i, 4> *pose, const MhndHandNodes &nodes) {
    if (!pose || !hand)
        return;
    MT7Model &model = *static_cast<MT7Model *>(character->model);
    (*pose)[0] = NodeRotation(*hand);
    if (MT7Node *const first = GetChildObjectByIndex(model, hand, nodes.first))
        (*pose)[1] = NodeRotation(*first);
    if (MT7Node *const second = GetChildObjectByIndex(model, hand, nodes.second)) {
        (*pose)[2].x = static_cast<std::int32_t>(second->rotation.y);
        (*pose)[2].y = static_cast<std::int32_t>(second->rotation.z);
        if (MT7Node *const tip = GetChildObjectByIndex(model, second, nodes.secondTip))
            (*pose)[2].z = static_cast<std::int32_t>(tip->rotation.y);
    }
    if (MT7Node *const third = GetChildObjectByIndex(model, hand, nodes.third)) {
        (*pose)[3].x = static_cast<std::int32_t>(third->rotation.y);
        (*pose)[3].y = static_cast<std::int32_t>(third->rotation.z);
        if (MT7Node *const tip = GetChildObjectByIndex(model, third, nodes.thirdTip))
            (*pose)[3].z = static_cast<std::int32_t>(tip->rotation.y);
    }
}

// @verified - p
void mhndApplyHandPose(Character *character, MT7Node *hand, const Vector3i *pose, const MhndHandNodes &nodes) {
    if (!pose || !hand)
        return;
    MT7Model &model = *static_cast<MT7Model *>(character->model);
    hand->rotation = pose[0];
    if (MT7Node *const first = GetChildObjectByIndex(model, hand, nodes.first)) {
        first->rotation = pose[1];
        if (MT7Node *const tip = GetChildObjectByIndex(model, first, nodes.firstTip))
            tip->rotation.z = first->rotation.z;
    }
    if (MT7Node *const second = GetChildObjectByIndex(model, hand, nodes.second)) {
        second->rotation.y = pose[2].x;
        second->rotation.z = pose[2].y;
        if (MT7Node *const tip = GetChildObjectByIndex(model, second, nodes.secondTip))
            tip->rotation.y = pose[2].z;
    }
    if (MT7Node *const third = GetChildObjectByIndex(model, hand, nodes.third)) {
        third->rotation.y = pose[3].x;
        third->rotation.z = pose[3].y;
        if (MT7Node *const tip = GetChildObjectByIndex(model, third, nodes.thirdTip))
            tip->rotation.y = pose[3].z;
    }
    if (!g_mhndSetupRunning) {
        const auto token = nodes.hand == 0xffbe ? kLeftHandToken : kRightHandToken;
        if (auto *const params = static_cast<HandParams *>(FindModelComponent(character, token)))
            UpdateHand(character, params);
    }
}

std::int32_t mhndSignedRotation(std::int32_t rotation) {
    const std::uint32_t low = static_cast<std::uint32_t>(rotation) & 0xffffU;
    return low < 0x8000U ? static_cast<std::int32_t>(low) : static_cast<std::int32_t>(low) - 0x10000;
}

// @verified - p
void mhndBeginHand(
    MhndHand &hand, MT7Node *node, const std::array<Vector3i, 4> &current, const Vector3i *target, std::int32_t side, std::int32_t shape, std::int32_t frames) {
    hand.side = side;
    hand.shape = shape;
    hand.node = node;
    hand.frames = frames;
    hand.current = current;
    for (std::size_t index = 0; index < hand.target.size(); ++index) {
        hand.target[index] = target[index];
        // Sign-extend each rotation's low WORD before subtracting.
        hand.step[index] = {(mhndSignedRotation(target[index].x) - mhndSignedRotation(current[index].x)) / frames,
            (mhndSignedRotation(target[index].y) - mhndSignedRotation(current[index].y)) / frames,
            (mhndSignedRotation(target[index].z) - mhndSignedRotation(current[index].z)) / frames};
    }
}

// @verified - p
std::int32_t mhndSetHandShape(MhndComponent *mhnd, Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames) {
    if (shape >= 15)
        return 1;
    const MhndHandShape &table = mhndHandShapes[static_cast<std::size_t>(shape)];
    std::array<Vector3i, 4> current{};
    if (mode == 0 || mode == 2) {
        if (MT7Node *const hand = FindCharacterNode(character, mhndRightHandNodes.hand)) {
            mhndReadHandPose(character, hand, &current, mhndRightHandNodes);
            mhndBeginHand(mhnd->hands[0], hand, current, table.right.data(), 0, shape, frames);
        }
    }
    if (mode == 1 || mode == 2) {
        if (MT7Node *const hand = FindCharacterNode(character, mhndLeftHandNodes.hand)) {
            mhndReadHandPose(character, hand, &current, mhndLeftHandNodes);
            mhndBeginHand(mhnd->hands[1], hand, current, table.left.data(), 1, shape, frames);
        }
    }
    return 1;
}

// @verified - p
std::int32_t mhndSetHandPose(MhndComponent *mhnd, Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames) {
    std::array<Vector3i, 4> current{};
    if (mode == 0 || mode == 2) {
        if (MT7Node *const hand = FindCharacterNode(character, mhndRightHandNodes.hand)) {
            mhndReadHandPose(character, hand, &current, mhndRightHandNodes);
            mhndBeginHand(mhnd->hands[0], hand, current, pose, 0, -1, frames);
        }
    }
    if (mode == 1 || mode == 2) {
        if (MT7Node *const hand = FindCharacterNode(character, mhndLeftHandNodes.hand)) {
            mhndReadHandPose(character, hand, &current, mhndLeftHandNodes);
            mhndBeginHand(mhnd->hands[1], hand, current, pose, 1, -1, frames);
        }
    }
    return 1;
}

// @verified - p
void mhndStepHand(Character *character, MhndHand &hand, std::int32_t side) {
    if (hand.frames <= 0) {
        hand.current = hand.target;
    } else {
        for (std::size_t index = 0; index < hand.current.size(); ++index) {
            hand.current[index].x += hand.step[index].x;
            hand.current[index].y += hand.step[index].y;
            hand.current[index].z += hand.step[index].z;
        }
    }
    if (side == 0)
        mhndApplyHandPose(character, hand.node, hand.current.data(), mhndRightHandNodes);
    else if (side == 1)
        mhndApplyHandPose(character, hand.node, hand.current.data(), mhndLeftHandNodes);
}

// Script commands are 8-byte-aligned {int32 opcode, arguments}. Opcodes 1..4 select the mode, 5 jumps
// to the loop start, 6 waits, 8/9 set the loop start and loop-back points, 10 loops, 11 starts a hand
// shape, 12 a hand pose and 13 installs a per-frame callback; anything else ends the script.
// @verified - p
void mhndRunPoseScript(Character *character, MhndPoseScript *script, MhndHand *hand) {
    do {
        for (;;) {
            if (script->state == 6) {
                if (--script->wait >= 0)
                    return;
                script->state = 14;
                continue;
            }
            if (script->state != 7)
                break;
            if (hand->node)
                return;
            script->state = 14;
        }
        if (script->state == 13) {
            script->callback(character, script, hand);
        } else if (script->state == 14) {
            std::int32_t opcode{};
            std::memcpy(&opcode, script->cursor, sizeof(opcode));
            const std::byte *const arguments = script->cursor + 8;
            script->state = opcode;
            script->cursor = arguments;
            std::int64_t value{};
            switch (opcode) {
            case 1:
            case 2:
            case 3:
            case 4:
                script->mode = opcode;
                script->state = 14;
                break;
            case 5:
                script->cursor = script->loopStart;
                script->state = 14;
                break;
            case 6:
                std::memcpy(&script->wait, arguments, sizeof(script->wait));
                script->cursor = arguments + 8;
                break;
            case 8:
                script->loopStart = arguments;
                script->state = 14;
                break;
            case 9:
                script->loopBack = arguments;
                script->state = 14;
                break;
            case 10:
                if (--script->loopCount >= 0)
                    script->cursor = script->loopBack;
                script->state = 14;
                break;
            case 11: {
                std::int32_t shape{};
                std::memcpy(&shape, arguments, sizeof(shape));
                std::memcpy(&value, arguments + 8, sizeof(value));
                script->cursor = arguments + 16;
                hmdlSetFlagsFor(character, hand->side, shape, static_cast<std::int32_t>(value));
                script->state = 7;
                return;
            }
            case 12:
                std::memcpy(&value, arguments + 0x60, sizeof(value));
                script->cursor = arguments + 0x68;
                hmdlSetPoseFor(character, hand->side, reinterpret_cast<const Vector3i *>(arguments), static_cast<std::int32_t>(value));
                script->state = 7;
                return;
            case 13:
                std::memcpy(&script->callback, arguments, sizeof(script->callback));
                script->cursor = arguments + 8;
                break;
            default:
                script->state = 0;
                return;
            }
        }
    } while (script->state == 14);
}

// @verified - p
// Side 0 chooses a shape; side 1 follows the right script's low DWORD +0x40.
void mhndIdlePoseCallback(Character *character, MhndPoseScript *script, MhndHand *hand) {
    std::uint32_t shape{};
    if (hand->side == 0) {
        if (--script->wait >= 0)
            return;
        script->wait = (IncrementSharedPrngUInt16() & 0x3fU) + 60;
        shape = IncrementSharedPrngUInt16() & 3U;
        std::memcpy(&script->field40, &shape, sizeof(shape));
    } else if (hand->side == 1) {
        const auto *const component = static_cast<const MhndComponent *>(FindModelComponent(character, kHandPoseToken));
        if (!component)
            return;
        std::memcpy(&shape, &component->scripts[0].field40, sizeof(shape));
        std::uint32_t previousShape{};
        std::memcpy(&previousShape, &script->field40, sizeof(previousShape));
        if (previousShape == shape)
            return;
        std::memcpy(&script->field40, &shape, sizeof(shape));
    } else {
        return;
    }
    hmdlSetFlagsFor(character, hand->side, static_cast<std::int32_t>(shape), 16);
    // hmdlSetFlagsFor clears the selected script; the idle owner reinstalls its running state.
    script->state = 13;
}

// @verified - p
void MHND_Callback(Character *character, void *payload) {
    MhndComponent &mhnd = *static_cast<MhndComponent *>(payload);
    MT7Node *const right = FindCharacterNode(character, mhndRightHandNodes.hand);
    if (mhnd.hands[0].node)
        mhnd.hands[0].node = right;
    if (mhnd.scripts[0].state != 0)
        mhndRunPoseScript(character, &mhnd.scripts[0], &mhnd.hands[0]);
    if (mhnd.hands[0].node) {
        --mhnd.hands[0].frames;
        mhndStepHand(character, mhnd.hands[0], 0);
        if (mhnd.hands[0].frames < 0)
            mhnd.hands[0].node = nullptr;
    }
    MT7Node *const left = FindCharacterNode(character, mhndLeftHandNodes.hand);
    if (mhnd.hands[1].node)
        mhnd.hands[1].node = left;
    if (mhnd.scripts[1].state != 0)
        mhndRunPoseScript(character, &mhnd.scripts[1], &mhnd.hands[1]);
    if (mhnd.hands[1].node) {
        --mhnd.hands[1].frames;
        mhndStepHand(character, mhnd.hands[1], 1);
        if (mhnd.hands[1].frames < 0)
            mhnd.hands[1].node = nullptr;
    }
}

} // namespace model_component_detail

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t mhndSetHandShape(MhndComponent *component, MT7Node *model, std::int32_t mode, std::int32_t shape, std::int32_t frames) {
    Vector3i current[4];
    MhndHand *owner;
    MT7Node *hand;
    const Vector3i *target;
    if (shape > 14)
        return 1;
    switch (mode) {
    case 0:
        owner = &component->hands[0];
        hand = FindMT7Node(model, -65);
        if (hand) {
            mhndReadRightHandPose(hand, current);
            target = (&model_component_detail::mhndHandShapes[shape].right)[mode].values;
            mhndBeginHand(*owner, hand, current, target, mode, shape, frames);
        }
        break;
    case 1:
        owner = &component->hands[1];
        hand = FindMT7Node(model, -66);
        if (hand) {
            mhndReadLeftHandPose(hand, current);
            target = (&model_component_detail::mhndHandShapes[shape].right)[mode].values;
            mhndBeginHand(*owner, hand, current, target, mode, shape, frames);
        }
        break;
    case 2:
        owner = &component->hands[0];
        hand = FindMT7Node(model, -65);
        if (hand) {
            mhndReadRightHandPose(hand, current);
            target = model_component_detail::mhndHandShapes[shape].right.values;
            mhndBeginHand(*owner, hand, current, target, 0, shape, frames);
        }
        owner = &component->hands[1];
        hand = FindMT7Node(model, -66);
        if (hand) {
            mhndReadLeftHandPose(hand, current);
            target = model_component_detail::mhndHandShapes[shape].left.values;
            mhndBeginHand(*owner, hand, current, target, 1, shape, frames);
        }
        break;
    }
    return 1;
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t mhndSetHandShape(MhndComponent *component, MT7Node *model, std::int32_t mode, std::int32_t shape, std::int32_t frames) {
    if (shape > 14)
        return 1;
    Vector3i current[4];
    if (mode == 0 || mode == 2) {
        if (MT7Node *const hand = FindMT7Node(model, -65)) {
            mhndReadRightHandPose(hand, current);
            mhndBeginHand(component->hands[0], hand, current, model_component_detail::mhndHandShapes[shape].right.values, 0, shape, frames);
        }
    }
    if (mode == 1 || mode == 2) {
        if (MT7Node *const hand = FindMT7Node(model, -66)) {
            mhndReadLeftHandPose(hand, current);
            mhndBeginHand(component->hands[1], hand, current, model_component_detail::mhndHandShapes[shape].left.values, 1, shape, frames);
        }
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t mhndSetHandPose(MhndComponent *component, MT7Node *model, std::int32_t mode, const Vector3i *pose, std::int32_t frames) {
    Vector3i current[4];
    MhndHand *owner;
    MT7Node *hand;
    switch (mode) {
    case 0:
        owner = &component->hands[0];
        hand = FindMT7Node(model, -65);
        if (hand) {
            mhndReadRightHandPose(hand, current);
            mhndBeginHand(*owner, hand, current, pose, mode, -1, frames);
        }
        break;
    case 1:
        owner = &component->hands[1];
        hand = FindMT7Node(model, -66);
        if (hand) {
            mhndReadLeftHandPose(hand, current);
            mhndBeginHand(*owner, hand, current, pose, mode, -1, frames);
        }
        break;
    case 2:
        owner = &component->hands[0];
        hand = FindMT7Node(model, -65);
        if (hand) {
            mhndReadRightHandPose(hand, current);
            mhndBeginHand(*owner, hand, current, pose, 0, -1, frames);
        }
        owner = &component->hands[1];
        hand = FindMT7Node(model, -66);
        if (hand) {
            mhndReadLeftHandPose(hand, current);
            mhndBeginHand(*owner, hand, current, pose, 1, -1, frames);
        }
        break;
    }
    return 1;
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t mhndSetHandPose(MhndComponent *component, MT7Node *model, std::int32_t mode, const Vector3i *pose, std::int32_t frames) {
    Vector3i current[4];
    if (mode == 0 || mode == 2) {
        if (MT7Node *const hand = FindMT7Node(model, -65)) {
            mhndReadRightHandPose(hand, current);
            mhndBeginHand(component->hands[0], hand, current, pose, 0, -1, frames);
        }
    }
    if (mode == 1 || mode == 2) {
        if (MT7Node *const hand = FindMT7Node(model, -66)) {
            mhndReadLeftHandPose(hand, current);
            mhndBeginHand(component->hands[1], hand, current, pose, 1, -1, frames);
        }
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void mhndStepHand(MhndHand &hand, std::int32_t side) {
    Vector3i *const pose = hand.current;
    std::int32_t *const current = &pose->x;
    std::uint32_t index;
    if (hand.frames > 0) {
        const std::int32_t *const step = &hand.step[0].x;
        for (index = 0; index < 12; ++index)
            current[index] += step[index];
    } else {
        const std::int32_t *const target = &hand.target[0].x;
        for (index = 0; index < 12; ++index)
            current[index] = target[index];
    }
    switch (side) {
    case 0:
        mhndApplyRightHandPose(hand.node, pose);
        break;
    case 1:
        mhndApplyLeftHandPose(hand.node, pose);
        break;
    }
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndStepHand(MhndHand &hand, std::int32_t side) {
    if (hand.frames > 0) {
        for (std::size_t index = 0; index < (sizeof(hand.current) / sizeof((hand.current)[0])); ++index) {
            for (std::size_t axis = 0; axis < 3; ++axis)
                hand.current[index][axis] = static_cast<std::int32_t>(static_cast<std::uint32_t>(hand.current[index][axis]) + hand.step[index][axis]);
        }
    } else {
        hand.current = hand.target;
    }
    if (side == 0)
        mhndApplyRightHandPose(hand.node, hand.current);
    else if (side == 1)
        mhndApplyLeftHandPose(hand.node, hand.current);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void mhndRunPoseScript(Character *character, MhndPoseScript *script, MhndHand *hand) {
    std::int32_t opcode;
    do {
        switch (script->state) {
        case 0:
            break;
        case 14:
            script->state = opcode = *script->cursor++;
            switch (opcode) {
            case 1:
            case 2:
            case 3:
            case 4:
                script->mode = script->state;
                script->state = 14;
                break;
            case 5:
                script->cursor = script->loopStart;
                script->state = 14;
                break;
            case 6:
                script->wait = *script->cursor++;
                break;
            case 8:
                script->loopStart = script->cursor;
                script->state = 14;
                break;
            case 9:
                script->loopBack = script->cursor;
                script->state = 14;
                break;
            case 10:
                if (--script->loopCount >= 0)
                    script->cursor = script->loopBack;
                script->state = 14;
                break;
            case 11: {
                const std::int32_t shape = script->cursor[0];
                const std::int32_t frames = script->cursor[1];
                script->cursor += 2;
                hmdlSetFlagsFor(character, hand->side, shape, frames);
                script->state = 7;
                break;
            }
            case 12: {
                const Vector3i *const pose = reinterpret_cast<const Vector3i *>(script->cursor);
                script->cursor += 12;
                const std::int32_t frames = *script->cursor++;
                hmdlSetPoseFor(character, hand->side, pose, frames);
                script->state = 7;
                break;
            }
            case 13:
                script->callback = reinterpret_cast<MhndPoseScriptCallback>(static_cast<std::uintptr_t>(*script->cursor++));
                break;
            case 7:
                script->state = 0;
                break;
            default:
                script->state = 0;
                break;
            }
            break;
        case 7:
            if (!hand->node)
                script->state = 14;
            break;
        case 6:
            if (--script->wait < 0)
                script->state = 14;
            break;
        case 13:
            script->callback(character, script, hand);
            break;
        }
    } while (script->state == 14);
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndRunPoseScript(Character *character, MhndPoseScript *script, MhndHand *hand) {
    do {
        if (script->state == 6) {
            script->wait = static_cast<std::int32_t>(static_cast<std::uint32_t>(script->wait) - 1U);
            if (script->wait < 0)
                script->state = 14;
        } else if (script->state == 7) {
            if (!hand->node)
                script->state = 14;
        } else if (script->state == 13) {
            script->callback(character, script, hand);
        } else if (script->state == 14) {
            const std::int32_t opcode = *script->cursor++;
            script->state = opcode;
            switch (opcode) {
            case 1:
            case 2:
            case 3:
            case 4:
                script->mode = opcode;
                script->state = 14;
                break;
            case 5:
                script->cursor = script->loopStart;
                script->state = 14;
                break;
            case 6:
                script->wait = *script->cursor++;
                break;
            case 7:
                script->state = 0;
                break;
            case 8:
                script->loopStart = script->cursor;
                script->state = 14;
                break;
            case 9:
                script->loopBack = script->cursor;
                script->state = 14;
                break;
            case 10:
                script->loopCount = static_cast<std::int32_t>(static_cast<std::uint32_t>(script->loopCount) - 1U);
                if (script->loopCount >= 0)
                    script->cursor = script->loopBack;
                script->state = 14;
                break;
            case 11: {
                const std::int32_t shape = script->cursor[0], frames = script->cursor[1];
                script->cursor += 2;
                hmdlSetFlagsFor(character, hand->side, shape, frames);
                script->state = 7;
                break;
            }
            case 12: {
                const Vector3i *const pose = reinterpret_cast<const Vector3i *>(script->cursor);
                const std::int32_t frames = script->cursor[12];
                script->cursor += 13;
                hmdlSetPoseFor(character, hand->side, pose, frames);
                script->state = 7;
                break;
            }
            case 13:
                script->callback = reinterpret_cast<MhndPoseScriptCallback>(static_cast<std::uintptr_t>(*script->cursor++));
                break;
            default:
                script->state = 0;
                break;
            }
        }
    } while (script->state == 14);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void MHND_Callback(Character *character, void *data) {
    MhndComponent *const component = static_cast<MhndComponent *>(data);
    MhndPoseScript *script = &component->scripts[0];
    MhndHand *hand = &component->hands[0];
    MT7Node *node = FindMT7Node(character->model, -65);
    if (hand->node)
        hand->node = node;
    if (script->state)
        mhndRunPoseScript(character, script, hand);
    if (hand->node) {
        hand->frames = static_cast<std::int32_t>(static_cast<std::uint32_t>(hand->frames) - 1U);
        mhndStepHand(*hand, 0);
        if (hand->frames < 0)
            hand->node = 0;
    }
    script = &component->scripts[1];
    hand = &component->hands[1];
    node = FindMT7Node(character->model, -66);
    if (hand->node)
        hand->node = node;
    if (script->state)
        mhndRunPoseScript(character, script, hand);
    if (hand->node) {
        hand->frames = static_cast<std::int32_t>(static_cast<std::uint32_t>(hand->frames) - 1U);
        mhndStepHand(*hand, 1);
        if (hand->frames < 0)
            hand->node = 0;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames) {
    if (!character || shape > 14 || mode > 2)
        return;
    MT7Node *const model = character->model;
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (component) {
        switch (mode) {
        case 0:
            component->scripts[0].state = 0;
            component->hands[0].shape = shape;
            break;
        case 1:
            component->scripts[1].state = 0;
            component->hands[1].shape = shape;
            break;
        case 2:
            component->scripts[0].state = 0;
            component->scripts[1].state = 0;
            component->hands[0].shape = shape;
            component->hands[1].shape = shape;
            break;
        }
    }
    if (!model)
        return;
    if (frames <= 1) {
        switch (mode) {
        case 0:
            if (component)
                component->hands[0].node = 0;
            mhndApplyRightHandPose(FindMT7Node(model, -65), model_component_detail::mhndHandShapes[shape].right.values);
            break;
        case 1:
            if (component)
                component->hands[1].node = 0;
            mhndApplyLeftHandPose(FindMT7Node(model, -66), model_component_detail::mhndHandShapes[shape].left.values);
            break;
        case 2:
            if (component) {
                component->hands[0].node = 0;
                component->hands[1].node = 0;
            }
            mhndApplyRightHandPose(FindMT7Node(model, -65), model_component_detail::mhndHandShapes[shape].right.values);
            mhndApplyLeftHandPose(FindMT7Node(model, -66), model_component_detail::mhndHandShapes[shape].left.values);
            break;
        }
    } else {
        if (component) {
            SetFlagForToken(character, CHARACTER_HAND_POSE, 1);
            mhndSetHandShape(component, model, mode, shape, frames);
        } else {
            component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
            mhndInit(component);
            mhndSetHandShape(component, model, mode, shape, frames);
        }
    }
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames) {
    if (!character || shape > 14 || mode > 2)
        return;
    MT7Node *const model = character->model;
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (component) {
        if (mode == 0 || mode == 2) {
            component->scripts[0].state = 0;
            component->hands[0].shape = shape;
        }
        if (mode == 1 || mode == 2) {
            component->scripts[1].state = 0;
            component->hands[1].shape = shape;
        }
    }
    if (!model)
        return;
    if (frames > 1) {
        if (component) {
            SetFlagForToken(character, CHARACTER_HAND_POSE, 1);
        } else {
            component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
            mhndInit(component);
        }
        mhndSetHandShape(component, model, mode, shape, frames);
        return;
    }
    if (mode == 0 || mode == 2) {
        if (component)
            component->hands[0].node = 0;
    }
    if (mode == 1 || mode == 2) {
        if (component)
            component->hands[1].node = 0;
    }
    if (mode == 0 || mode == 2)
        mhndApplyRightHandPose(FindMT7Node(model, -65), model_component_detail::mhndHandShapes[shape].right.values);
    if (mode == 1 || mode == 2)
        mhndApplyLeftHandPose(FindMT7Node(model, -66), model_component_detail::mhndHandShapes[shape].left.values);
}

#else

// @verified - p
void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames) {
    if (!character || shape >= 15 || mode >= 3)
        return;
    MhndComponent *mhnd = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (mhnd) {
        if (mode == 1 || mode == 2) {
            mhnd->scripts[1].state = 0;
            mhnd->hands[1].shape = shape;
        }
        if (mode == 0 || mode == 2) {
            mhnd->hands[0].shape = shape;
            mhnd->scripts[0].state = 0;
        }
    }
    if (!character->model)
        return;
    if (frames > 1) {
        if (mhnd) {
            SetFlagForToken(character, kHandPoseToken, 1);
        } else {
            mhnd = static_cast<MhndComponent *>(AllocateModelComponent(character, kHandPoseToken, sizeof(MhndComponent), MHND_Callback));
            mhndInit(mhnd);
        }
        mhndSetHandShape(mhnd, character, mode, shape, frames);
        return;
    }
    if (mode != 0 && mode != 1 && mode != 2)
        return;
    const MhndHandShape &table = mhndHandShapes[static_cast<std::size_t>(shape)];
    if (mode == 0 || mode == 2) {
        if (mhnd)
            mhnd->hands[0].node = nullptr;
    }
    if (mode == 1 || mode == 2) {
        if (mhnd)
            mhnd->hands[1].node = nullptr;
    }
    if (mode == 0 || mode == 2)
        mhndApplyHandPose(character, FindCharacterNode(character, mhndRightHandNodes.hand), table.right.data(), mhndRightHandNodes);
    if (mode == 1 || mode == 2)
        mhndApplyHandPose(character, FindCharacterNode(character, mhndLeftHandNodes.hand), table.left.data(), mhndLeftHandNodes);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames) {
    if (!character || mode > 2)
        return;
    MT7Node *const model = character->model;
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (component) {
        switch (mode) {
        case 0:
            component->scripts[0].state = 0;
            component->hands[0].shape = -1;
            break;
        case 1:
            component->scripts[1].state = 0;
            component->hands[1].shape = -1;
            break;
        case 2:
            component->scripts[0].state = 0;
            component->scripts[1].state = 0;
            component->hands[0].shape = -1;
            component->hands[1].shape = -1;
            break;
        }
    }
    if (!model)
        return;
    if (frames <= 1) {
        switch (mode) {
        case 0:
            if (component)
                component->hands[0].node = 0;
            mhndApplyRightHandPose(FindMT7Node(model, -65), pose);
            break;
        case 1:
            if (component)
                component->hands[1].node = 0;
            mhndApplyLeftHandPose(FindMT7Node(model, -66), pose);
            break;
        case 2:
            if (component) {
                component->hands[0].node = 0;
                component->hands[1].node = 0;
            }
            mhndApplyRightHandPose(FindMT7Node(model, -65), pose);
            mhndApplyLeftHandPose(FindMT7Node(model, -66), pose);
            break;
        }
    } else {
        if (component) {
            SetFlagForToken(character, CHARACTER_HAND_POSE, 1);
            mhndSetHandPose(component, model, mode, pose, frames);
        } else {
            component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
            if (component) {
                mhndInit(component);
                mhndSetHandPose(component, model, mode, pose, frames);
            }
        }
    }
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames) {
    if (!character || mode > 2)
        return;
    MT7Node *const model = character->model;
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (component) {
        if (mode == 0 || mode == 2) {
            component->scripts[0].state = 0;
            component->hands[0].shape = -1;
        }
        if (mode == 1 || mode == 2) {
            component->scripts[1].state = 0;
            component->hands[1].shape = -1;
        }
    }
    if (!model)
        return;
    if (frames > 1) {
        if (component) {
            SetFlagForToken(character, CHARACTER_HAND_POSE, 1);
        } else {
            component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
            if (!component)
                return;
            mhndInit(component);
        }
        mhndSetHandPose(component, model, mode, pose, frames);
        return;
    }
    if (mode == 0 || mode == 2) {
        if (component)
            component->hands[0].node = 0;
    }
    if (mode == 1 || mode == 2) {
        if (component)
            component->hands[1].node = 0;
    }
    if (mode == 0 || mode == 2)
        mhndApplyRightHandPose(FindMT7Node(model, -65), pose);
    if (mode == 1 || mode == 2)
        mhndApplyLeftHandPose(FindMT7Node(model, -66), pose);
}

#else

// @verified - p
void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames) {
    if (!character || mode >= 3)
        return;
    MhndComponent *mhnd = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (mhnd) {
        if (mode == 1 || mode == 2) {
            mhnd->scripts[1].state = 0;
            mhnd->hands[1].shape = -1;
        }
        if (mode == 0 || mode == 2) {
            mhnd->hands[0].shape = -1;
            mhnd->scripts[0].state = 0;
        }
    }
    if (!character->model)
        return;
    if (frames > 1) {
        if (mhnd) {
            SetFlagForToken(character, kHandPoseToken, 1);
        } else {
            mhnd = static_cast<MhndComponent *>(AllocateModelComponent(character, kHandPoseToken, sizeof(MhndComponent), MHND_Callback));
            if (!mhnd)
                return;
            mhndInit(mhnd);
        }
        mhndSetHandPose(mhnd, character, mode, pose, frames);
        return;
    }
    if (mode == 0 || mode == 2) {
        if (mhnd)
            mhnd->hands[0].node = nullptr;
    }
    if (mode == 1 || mode == 2) {
        if (mhnd)
            mhnd->hands[1].node = nullptr;
    }
    if (mode == 0 || mode == 2)
        mhndApplyHandPose(character, FindCharacterNode(character, mhndRightHandNodes.hand), pose, mhndRightHandNodes);
    if (mode == 1 || mode == 2)
        mhndApplyHandPose(character, FindCharacterNode(character, mhndLeftHandNodes.hand), pose, mhndLeftHandNodes);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void mhndSetupHuman(Character *character) {
    g_mhndSetupRunning = true;
    if (!character) {
        g_mhndSetupRunning = false;
        return;
    }
    MhndComponent *mhnd = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (mhnd) {
        mhnd->scripts[0].state = 0;
        mhnd->scripts[1].state = 0;
        mhnd->hands[0].shape = 11;
        mhnd->hands[1].shape = 11;
    }
    if (character->model) {
        if (mhnd) {
            mhnd->hands[0].node = nullptr;
            mhnd->hands[1].node = nullptr;
        }
        mhndApplyHandPose(character, FindCharacterNode(character, mhndRightHandNodes.hand), mhndHandShapes[11].right.data(), mhndRightHandNodes);
        mhndApplyHandPose(character, FindCharacterNode(character, mhndLeftHandNodes.hand), mhndHandShapes[11].left.data(), mhndLeftHandNodes);
    }
    mhnd = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (mhnd) {
        mhnd->scripts[0].state = 0;
        mhnd->scripts[1].state = 0;
        mhnd->hands[0].shape = 8;
        mhnd->hands[1].shape = 8;
    }
    if (character->model) {
        if (mhnd) {
            SetFlagForToken(character, kHandPoseToken, 1);
        } else {
            mhnd = static_cast<MhndComponent *>(AllocateModelComponent(character, kHandPoseToken, sizeof(MhndComponent), MHND_Callback));
            mhndInit(mhnd);
        }
        mhndSetHandShape(mhnd, character, 2, 8, 4);
    }
    mhnd = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (mhnd) {
        MhndMeasureHand(character, -65, mhnd->hands[0].measurements);
        MhndMeasureHand(character, -66, mhnd->hands[1].measurements);
    }
    g_mhndSetupRunning = false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void AssignHandPoseScript(Character *character, std::int32_t mode, const std::int32_t *commands) {
    if (!character || mode > 2 || !character->model)
        return;
    MhndPoseScript *script;
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (!component) {
        component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
        if (!component)
            return;
        mhndInit(component);
    } else {
        SetFlagForToken(character, CHARACTER_HAND_POSE, 1);
    }
    switch (mode) {
    case 0:
        script = &component->scripts[0];
        script->state = 14;
        script->cursor = commands;
        script->loopStart = commands;
        break;
    case 1:
        script = &component->scripts[1];
        script->state = 14;
        script->cursor = commands;
        script->loopStart = commands;
        break;
    case 2:
        script = &component->scripts[0];
        script->state = 14;
        script->cursor = commands;
        script->loopStart = commands;
        script = &component->scripts[1];
        script->state = 14;
        script->cursor = commands;
        script->loopStart = commands;
        break;
    }
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AssignHandPoseScript(Character *character, std::int32_t mode, const std::int32_t *commands) {
    if (!character || mode > 2 || !character->model)
        return;
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (component) {
        SetFlagForToken(character, CHARACTER_HAND_POSE, 1);
    } else {
        component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
        if (!component)
            return;
        mhndInit(component);
    }
    if (mode == 0 || mode == 2) {
        component->scripts[0].state = 14;
        component->scripts[0].cursor = commands;
        component->scripts[0].loopStart = commands;
    }
    if (mode == 1 || mode == 2) {
        component->scripts[1].state = 14;
        component->scripts[1].cursor = commands;
        component->scripts[1].loopStart = commands;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetupHandPoseScript(Character *character) {
    AssignHandPoseScript(character, 2, mhndIdlePoseScript);
}

#else

// @verified - p
void SetupHandPoseScript(Character *character) {
    if (!character || !character->model)
        return;
    auto *component = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (component) {
        SetFlagForToken(character, kHandPoseToken, 1);
    } else {
        component = static_cast<MhndComponent *>(AllocateModelComponent(character, kHandPoseToken, sizeof(MhndComponent), MHND_Callback));
        if (!component)
            return;
        mhndInit(component);
    }
    const auto *const commands = reinterpret_cast<const std::byte *>(mhndIdlePoseScript.data());
    component->scripts[0].state = 14;
    component->scripts[0].cursor = commands;
    component->scripts[0].loopStart = commands;
    component->scripts[1].state = 14;
    component->scripts[1].cursor = commands;
    component->scripts[1].loopStart = commands;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void mhndIdlePoseCallback(Character *character, MhndPoseScript *script, MhndHand *hand) {
    MhndComponent *component;
    switch (hand->side) {
    case 0:
        if (--script->wait >= 0)
            break;
        script->wait = (dc::sdk::RandomInt() & 0x3fU) + 60;
        script->idleShape = dc::sdk::RandomInt() & 3U;
        hmdlSetFlagsFor(character, hand->side, script->idleShape, 16);
        script->state = 13;
        break;
    case 1:
        component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
        if (!component)
            break;
        if (script->idleShape == component->scripts[0].idleShape)
            break;
        script->idleShape = component->scripts[0].idleShape;
        hmdlSetFlagsFor(character, hand->side, script->idleShape, 16);
        script->state = 13;
        break;
    }
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mhndIdlePoseCallback(Character *character, MhndPoseScript *script, MhndHand *hand) {
    std::uint32_t shape;
    if (hand->side == 0) {
        script->wait = static_cast<std::int32_t>(static_cast<std::uint32_t>(script->wait) - 1U);
        if (script->wait >= 0)
            return;
        script->wait = (dc::sdk::RandomInt() & 0x3fU) + 60;
        shape = dc::sdk::RandomInt() & 3U;
        script->idleShape = shape;
    } else if (hand->side == 1) {
        const MhndComponent *const component = reinterpret_cast<const MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
        if (!component)
            return;
        shape = component->scripts[0].idleShape;
        if (script->idleShape == shape)
            return;
        script->idleShape = shape;
    } else {
        return;
    }
    hmdlSetFlagsFor(character, hand->side, shape, 16);
    script->state = 13;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void SetupHandIdlePoseCallback(Character *character) {
    MhndPoseScript *script;
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (!component) {
        component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
        if (!component)
            return;
        mhndInit(component);
    }
    script = &component->scripts[0];
    script->state = 13;
    script->callback = mhndIdlePoseCallback;
    script = &component->scripts[1];
    script->state = 13;
    script->callback = mhndIdlePoseCallback;
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetupHandIdlePoseCallback(Character *character) {
    MhndComponent *component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (!component) {
        component = static_cast<MhndComponent *>(AllocateModelComponent(character, CHARACTER_HAND_POSE, sizeof(MhndComponent), MHND_Callback));
        if (!component)
            return;
        mhndInit(component);
    }
    component->scripts[0].state = 13;
    component->scripts[0].callback = mhndIdlePoseCallback;
    component->scripts[1].state = 13;
    component->scripts[1].callback = mhndIdlePoseCallback;
}

#else

// @verified - p
MhndPoseScriptCallback SetupHandIdlePoseCallback(Character *character) {
    auto *component = static_cast<MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (!component) {
        component = static_cast<MhndComponent *>(AllocateModelComponent(character, kHandPoseToken, sizeof(MhndComponent), MHND_Callback));
        if (!component)
            return nullptr;
        mhndInit(component);
    }
    component->scripts[0].state = 13;
    component->scripts[0].callback = mhndIdlePoseCallback;
    component->scripts[1].state = 13;
    component->scripts[1].callback = mhndIdlePoseCallback;
    return mhndIdlePoseCallback;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t GetHandShapeFor(Character *character, std::int32_t mode) {
    MhndComponent *component;
    MhndHand *hand;
    if (character && mode <= 2) {
        component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
        if (!component)
            return -3;
        if (mode == 1)
            hand = &component->hands[1];
        else
            hand = &component->hands[0];
        return hand->shape;
    }
    return -2;
}

#elif SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetHandShapeFor(Character *character, std::int32_t mode) {
    if (!character)
        return -2;
    if (mode < 3) {
        const MhndComponent *const component = reinterpret_cast<const MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
        if (!component)
            return -3;
        const MhndHand *hand = &component->hands[0];
        if (mode == 1)
            ++hand;
        return hand->shape;
    }
    return -2;
}

#else

// @verified - p
std::uint64_t GetHandShapeFor(Character *character, std::int32_t mode) {
    if (!character || mode >= 3)
        return 0xfffffffeULL;
    const auto *const component = static_cast<const MhndComponent *>(FindModelComponent(character, kHandPoseToken));
    if (!component)
        return 0xfffffffdULL;
    return static_cast<std::uint32_t>(component->hands[mode == 1 ? 1 : 0].shape);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AllocateCharacterHandPose(Character *character) {
    hmdlSetFlagsFor(character, 2, 8, 4);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DisableHandPoseController(Character *character) {
    MhndComponent *const component = static_cast<MhndComponent *>(charFindComp(character, CHARACTER_HAND_POSE));
    if (component) {
        component->hands[0].node = 0;
        component->hands[1].node = 0;
        SetFlagForToken(character, CHARACTER_HAND_POSE, 0);
    }
}

#endif

} // namespace s2
} // namespace shenmue
