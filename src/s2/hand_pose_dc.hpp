#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#include "s2/model_dc.hpp"
#include <cstddef>
#include <cstdint>

#define SHENMUE_HAND_POSE_LAYOUT_NAME_(line) SHENMUE_HAND_POSELayoutCheck##line
#define SHENMUE_HAND_POSE_LAYOUT_NAME(line) SHENMUE_HAND_POSE_LAYOUT_NAME_(line)
#define SHENMUE_HAND_POSE_LAYOUT(condition) typedef char SHENMUE_HAND_POSE_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_HAND_POSE_CONSTANT static const
namespace shenmue {
namespace s2 {

#define CHARACTER_HAND_POSE (::shenmue::shen_name(SHEN_NAME4('M', 'H', 'N', 'D')))
struct MhndHand {
    MT7Node *node;
    std::int32_t side, shape, frames;
    Vector3i current[4], target[4], step[4];
};
struct MhndPoseScript;
typedef void (*MhndPoseScriptCallback)(Character *, MhndPoseScript *, MhndHand *);
struct MhndPoseScript {
    std::int32_t state, mode, wait;
    const std::int32_t *cursor, *loopStart, *loopBack;
    std::int32_t loopCount;
    MhndPoseScriptCallback callback;
    std::uint32_t idleShape;
};
struct MhndComponent {
    MhndHand hands[2];
    MhndPoseScript scripts[2];
};
SHENMUE_HAND_POSE_LAYOUT(sizeof(MhndHand) == 0xa0);
SHENMUE_HAND_POSE_LAYOUT(offsetof(MhndHand, current) == 0x10);
SHENMUE_HAND_POSE_LAYOUT(offsetof(MhndHand, target) == 0x40);
SHENMUE_HAND_POSE_LAYOUT(offsetof(MhndHand, step) == 0x70);
SHENMUE_HAND_POSE_LAYOUT(sizeof(MhndPoseScript) == 0x24);
SHENMUE_HAND_POSE_LAYOUT(offsetof(MhndPoseScript, cursor) == 0x0c);
SHENMUE_HAND_POSE_LAYOUT(offsetof(MhndPoseScript, callback) == 0x1c);
SHENMUE_HAND_POSE_LAYOUT(sizeof(MhndComponent) == 0x188);
SHENMUE_HAND_POSE_LAYOUT(offsetof(MhndComponent, scripts) == 0x140);

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
static const std::int32_t mhndIdlePoseScript[10] =
    {11, 0, 15, 6, 120, 11, 3, 15, 5, 0};
void AssignHandPoseScript(Character *character, std::int32_t mode, const std::int32_t *commands);
void SetupHandPoseScript(Character *character);
void mhndIdlePoseCallback(Character *character, MhndPoseScript *script, MhndHand *hand);
void SetupHandIdlePoseCallback(Character *character);
std::int32_t GetHandShapeFor(Character *character, std::int32_t mode);
void DisableHandPoseController(Character *character);
void AllocateCharacterHandPose(Character *character);

}
} // namespace shenmue::s2
#undef SHENMUE_HAND_POSE_LAYOUT_NAME_
#undef SHENMUE_HAND_POSE_LAYOUT_NAME
#undef SHENMUE_HAND_POSE_LAYOUT
#undef SHENMUE_HAND_POSE_CONSTANT

#endif
