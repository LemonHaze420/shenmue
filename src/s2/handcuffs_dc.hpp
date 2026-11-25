#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hlib.hpp"
#include "s2/model_dc.hpp"

namespace shenmue { namespace s2 {

#define TASK_CUFF shen_name(SHEN_NAME4('C', 'U', 'F', 'F'))
#define MODEL_HANDCUFF_HIGH shen_name(SHEN_NAME4('T', 'E', 'J', '1'))
#define MODEL_HANDCUFF_LOW shen_name(SHEN_NAME4('T', 'E', 'J', '0'))

struct Character;
struct MotionWork;
struct HandcuffConstraint {
    HandcuffConstraint *peer;
    Character *character;
    std::int32_t bone;
    float maximumDistance;
    std::uint32_t flags;
    MT7Node *model;
};
struct HandcuffsTaskData {
    MT7Node *highModel;
    MT7Node *highChainLink;
    MT7Node *lowModel;
    MT7Node *lowChainLink;
    std::int32_t active;
    std::int32_t chainCount;
    HandcuffConstraint first;
    HandcuffConstraint second;
    Vector3 attachmentOffset;
    std::int32_t retained54;
};
struct HandcuffChainArc {
    std::int32_t angle;
    float radius;
    float height;
};
typedef char HandcuffChainArcSizeCheck[sizeof(HandcuffChainArc) == 0x0c ? 1 : -1];
typedef char HandcuffChainArcRadiusCheck[offsetof(HandcuffChainArc, radius) == 4 ? 1 : -1];
typedef char HandcuffChainArcHeightCheck[offsetof(HandcuffChainArc, height) == 8 ? 1 : -1];
typedef char HandcuffConstraintSizeCheck[sizeof(HandcuffConstraint) == 0x18 ? 1 : -1];
typedef char HandcuffConstraintBoneCheck[offsetof(HandcuffConstraint, bone) == 8 ? 1 : -1];
typedef char HandcuffConstraintModelCheck[offsetof(HandcuffConstraint, model) == 0x14 ? 1 : -1];
typedef char HandcuffsTaskDataSizeCheck[sizeof(HandcuffsTaskData) == 0x58 ? 1 : -1];
typedef char HandcuffsTaskDataActiveCheck[offsetof(HandcuffsTaskData, active) == 0x10 ? 1 : -1];
typedef char HandcuffsTaskDataFirstCheck[offsetof(HandcuffsTaskData, first) == 0x18 ? 1 : -1];
typedef char HandcuffsTaskDataSecondCheck[offsetof(HandcuffsTaskData, second) == 0x30 ? 1 : -1];
typedef char HandcuffsTaskDataAttachmentCheck[offsetof(HandcuffsTaskData, attachmentOffset) == 0x48 ? 1 : -1];

extern HLib::Task *g_Handcuffs_Task;
extern Vector3 g_handcuffChainMidpoint, g_handcuffChainAnchor;
extern float g_handcuffChainVelocity;
void Handcuffs_Enqueue();
void Handcuffs_Callback(void *data);
void Handcuffs_Render(void *data);
void Handcuffs_UpdateChainArc(float distance, HandcuffChainArc *arc);
void Handcuffs_BuildChainTransform(const Vector3 &first, const Vector3 &second, Matrix4x4 &transform);
void resetHandcuffTask();
void cuffsSetFlags();
std::int32_t Handcuffs_Attach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float distance);
void Handcuffs_ConstrainWrist(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *parameter);

} }
#endif
