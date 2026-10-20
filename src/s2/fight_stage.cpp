#include "s2/fight_stage.hpp"

#include "s2/character.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/weather.hpp"
#include "s2/input.hpp"
#include "s2/player.hpp"
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <xmmintrin.h>

namespace shenmue::s2 {

FightEntry g_activeFightData[16]{};
FightStageObjectDefinition *g_fightStageObjectDefinitions[8]{};
VOBJ_t *g_fightStageObjects[30]{};
VOBJ_t *g_fightStageObjectRoot{};
float g_fightStageObjectMaximumDistance{};
VFMA_t *g_pFightMain{};

// @verified - p
VFActor *AllocateFGTM() {
    VFActor *const actor = static_cast<VFActor *>(HLib::AllocateTaskData(sizeof(VFActor), TASK_FGTM));
    HLib::AssignDynamicAlignmentForTaskData(actor);
    std::memset(actor, 0, sizeof(*actor));
    return actor;
}

// @verified - p
VFSlotRuntimeState *AllocateFENE() {
    VFSlotRuntimeState *const slot = static_cast<VFSlotRuntimeState *>(HLib::AllocateTaskData(sizeof(VFSlotRuntimeState), TASK_FENE));
    HLib::AssignDynamicAlignmentForTaskData(slot);
    std::memset(slot, 0, sizeof(*slot));
    return slot;
}



// @verified - p
VFGW_t *AllocateVFGW(VFActor *actor) {
    auto *const gauge = static_cast<VFGW_t *>(HLib::AllocateTaskData(sizeof(VFGW_t), SHEN_NAME(VFGW)));
    actor->gauge = gauge;
    HLib::AssignDynamicAlignmentForTaskData(gauge);
    gauge->state = 0;
    gauge->animation = 0;
    gauge->sprite = 137;
    gauge->animationParameters = 0;
    gauge->active = 0;
    gauge->gaugeTime = 90.0f;
    gauge->animationTime = 0.0f;
    gauge->displayedHp = gauge->targetHp = 100.0f;
    gauge->x = 80;
    gauge->dimensions = 0x10008U;
    gauge->colours = 0xff0081U;
    gauge->layout = 0x280050U;
    gauge->scale = 0x10000U;
    gauge->alpha = 255;
    return gauge;
}

// @verified - p
void FreeVFGW(VFActor *actor) {
    if (!actor->gauge)
        return;
    HLib::ClearTaskDataMark(actor->gauge);
    HLib::FreeTaskData(actor->gauge);
    actor->gauge = nullptr;
}

// @verified - p
VCDW_t *AllocateVCDW(VFMA_t *game) {
    auto *const dialogue = static_cast<VCDW_t *>(HLib::AllocateTaskData(sizeof(VCDW_t), SHEN_NAME(VCDW)));
    HLib::AssignDynamicAlignmentForTaskData(dialogue);
    game->cameraDialogue = dialogue;
    dialogue->selected = -1;
    dialogue->records = nullptr;
    dialogue->state = 0;
    dialogue->flags = 0;
    dialogue->enabled = 1;
    dialogue->counter = 0;
    dialogue->parameters = 0;
    return dialogue;
}

// @verified - p
void vfUpdateWorldTransformAndFacingAngle(VFActor *actor) {
    actor->position = GetCharacterWorldPosition(actor->character);
    actor->facingAngle = static_cast<std::uint16_t>(GetAllAngYForNode(&actor->character->entity.node));
}

// @verified - p
void vfApplyYawRecursive(VFActor *actor) {
    shenDecrementRotYAllNodes(&actor->character->entity.node, static_cast<std::int16_t>(actor->facingAngle));
    shenApplyTranslationToAll(&actor->character->entity.node, actor->position);
}

namespace {
constexpr std::int32_t fightMotionSelectors[7]{25, 30, 0, 2, 7, 10, 17};

// @verified - p
bool BindAllocatedEnemySlot(FCMG_t *slot) {
    FRMG_t *formation = nullptr;
    for (std::uint8_t i = 0; i < g_pFightStage->groupCount; ++i) {
        FRMG_t &candidate = g_pFightStage->groups[i];
        if (candidate.character == slot->character && candidate.state == 0) {
            formation = &candidate;
            break;
        }
    }
    if (!formation)
        return false;
    slot->motion = FindMotionWork(slot->character);
    slot->position = GetCharacterWorldPosition(slot->character);
    slot->accumulatedYaw = static_cast<std::uint16_t>(GetAllAngYForNode(&slot->character->entity.node));
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        const FCMG_t &other = g_pFightStage->enemySlots[i];
        if (other.character && &other != slot && std::fabs((other.position.z - slot->position.z) + (other.position.x - slot->position.x)) < 0.00000011920929f) {
            slot->position.x += 0.0099999998f;
            slot->position.z += 0.0099999998f;
            shenApplyTranslationToAll(&slot->character->entity.node, slot->position);
        }
    }
    vfPrepareEnemySlotMotion(slot);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    slot->group = formation;
    formation->slot = slot;
    --g_pFightStage->secondaryGroupCounters[slot->counterGroup];
    return true;
}


// @verified - p
void AdjustCooldownGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *, void *context) {
    static constexpr shen_name qlbtMap{SHEN_NAME(QLBT)};
    if (g_currentMap.name == qlbtMap.name)
        return;
    std::uint32_t index;
    switch (controller) {
    case 0:
        index = 2;
        break;
    case 1:
        index = 3;
        break;
    case 3:
        index = 5;
        break;
    case 4:
        index = 6;
        break;
    case 5:
        index = 0;
        break;
    case 6:
        index = 1;
        break;
    default:
        return;
    }
    auto *const group = static_cast<FCMG_t *>(context);
    if (group->groundCorrectionEnabled[index] == 0)
        return;
    static constexpr float heights[7]{0.13f, 0.13f, 0.17f, 0.20f, 0.20f, 0.15f, 0.15f};
    const float y = position->y;
    const float bodyLimit = heights[index] + group->position.y;
    const float base = group->stackLayerOffsets[index];
    const float targetLimit = heights[index] + base;
    float &previous = group->groundSamples[index];
    if (bodyLimit < y || y <= targetLimit) {
        if (targetLimit <= y)
            previous = -10000.0f;
        else {
            previous = targetLimit - y;
            position->y += previous;
        }
    } else {
        if (previous < -9999.9f)
            previous = y - targetLimit;
        previous *= 0.5f;
        position->y = previous + base;
    }
}


constexpr std::uint32_t fightMoveCategoryMasks[44]{
    0,8,0x40,0x10,0x80,0x30,0x180,0x800,0x4000,0x1800,0xc000,
    0x1800,0xc000,0x1b0,0x1c00,0xe000,2,4,0x202,6,0x202,0xd800,
    0x18,0xc0,0x1a,0xc2,0x38,0x1c0,0xd800,0x4800,0x1830,0xc180,
    0x42,0xa,0x2602,0x4c,0x5000,0x8800,0xd800,0x1b0,0x1f8,0xffffffffU,0xfe02,0x3fe};

// The actual active-slot ground target callback.
// @verified - p
void AdjustActiveGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *context) {
    static constexpr shen_name qlbtMap{SHEN_NAME(QLBT)};
    if (g_currentMap.name == qlbtMap.name || controller > 6)
        return;
    auto *const actor = static_cast<VFActor *>(context);
    static constexpr std::uint8_t indices[7]{2, 3, 4, 5, 6, 0, 1};
    const auto index = indices[controller];
    if (actor->previousGroundSamples[index] < -9999.9f)
        actor->previousGroundSamples[index] = position->y;
    const bool addNearby = static_cast<std::uint32_t>(actor->game->state) < 4U || (actor->moveFlags & VFMF_ForceDefeat) != 0;
    const bool query = controller != 0 && controller != 6;
    if (query || actor->game->type == 8 ||
        (actor->hitStateFlags &
            (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_AlignedToGround)) !=
            0) {
        actor->groundHeights[index] = vfFieldGetIntersection(actor, *position);
        if (addNearby)
            actor->groundHeights[index] += vfAccumEnemyGroundOffsetAndMask(position, &actor->nearbyGroundActors[index]);
    }
    if (actor->game->type == 8)
        return;
    if ((actor->hitStateFlags &
            (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_AlignedToGround)) !=
        0) {
        actor->previousGroundSamples[index] = position->y;
        return;
    }
    const auto category = fightMoveCategoryMasks[actor->moveId];
    if (controller == 5) {
        Vector3 otherFoot, pelvis;
        TransformWithoutIKEnabled(motion, &otherFoot, &pelvis);
        actor->groundHeights[1] = vfFieldGetIntersection(actor, otherFoot);
        if (addNearby)
            actor->groundHeights[1] += vfAccumEnemyGroundOffsetAndMask(&otherFoot, &actor->nearbyGroundActors[1]);
        actor->groundHeights[2] = vfFieldGetIntersection(actor, actor->contactPosition);
        if (addNearby)
            actor->groundHeights[2] += vfAccumEnemyGroundOffsetAndMask(&actor->contactPosition, &actor->nearbyGroundActors[2]);
        std::uint32_t primary = actor->groundHeights[1] <= actor->groundHeights[0] ? 1U : 0U;
        if ((actor->motionFlags & VFMOF_Unknown100) != 0) {
            if (primary == 0 && (category & 0x1c00U) != 0)
                primary = 1;
            else if ((category & 0xe000U) != 0)
                primary = 0;
        }
        actor->primaryGroundTarget = static_cast<std::int32_t>(primary);
        float difference = actor->groundHeights[primary] - actor->groundHeights[2];
        if (std::fabs(difference) <= 0.0024999999f)
            difference = 0.0024999999f;
        actor->groundHeightDifference = difference;
        actor->targetGroundSamples[2] = pelvis.y + difference;
        if (primary != 0) {
            actor->targetGroundSamples[1] = otherFoot.y + difference;
            actor->targetGroundSamples[0] = (actor->groundHeights[0] - actor->groundHeights[1] + position->y) + difference;
        } else {
            actor->targetGroundSamples[0] = difference + position->y;
            actor->targetGroundSamples[1] = (actor->groundHeights[1] - actor->groundHeights[0] + otherFoot.y) + difference;
        }
        if ((actor->stateFlags & VFSF_Unknown80000) != 0) {
            for (std::uint8_t i = 0; i < 7; ++i)
                actor->previousGroundSamples[i] = actor->targetGroundSamples[i];
        }
        if (((actor->motionFlags & VFMOF_Unknown100) == 0 || (category & 0x1c00U) == 0) &&
            std::fabs(actor->targetGroundSamples[0] - position->y) > 0.0024999999f)
            position->y = (actor->previousGroundSamples[0] + actor->targetGroundSamples[0]) * 0.5f;
    } else if (controller == 0 || (controller == 6 && ((actor->motionFlags & VFMOF_Unknown100) == 0 || (category & 0xe000U) == 0))) {
        if (std::fabs(actor->targetGroundSamples[index] - position->y) > 0.0024999999f)
            position->y = (actor->previousGroundSamples[index] + actor->targetGroundSamples[index]) * 0.5f;
    }
    if ((actor->motionFlags & VFMOF_Unknown100) != 0 && (actor->moveFlags & VFMF_Airborne) == 0 &&
        (actor->actor->secondaryMotionFlags & VFSMF_Unknown2000000) == 0) {
        float limit = 0.0f;
        if ((controller == 3 && (category & 0x20U) != 0) || (controller == 4 && (category & 0x100U) != 0))
            limit = 0.2f;
        else if ((controller == 5 && (category & 0x1000U) != 0) || (controller == 6 && (category & 0x8000U) != 0))
            limit = 0.30000001f;
        if (limit != 0.0f) {
            const float difference = (actor->actor->targetGroundY - actor->targetGroundY) * actor->groundReactionScale;
            const float offset = difference <= limit ? std::fmax(-limit, difference) : limit;
            position->y += offset;
        }
    }
    actor->previousGroundSamples[index] = position->y;
}
}

// @verified - p
void motRemoveCallback(MotionWork *motion) {
    for (const auto selector : fightMotionSelectors)
        AssignMotionTargetCallback(motion, nullptr, selector);
}

// @verified - p
void vfRegisterGroundTargetCallbacks(MotionWork *motion, VFActor *actor) {
    for (const auto selector : fightMotionSelectors) {
        AssignMotionTargetCallback(motion, AdjustActiveGroundTarget, selector);
        AssignMotionTargetParameter(motion, actor, selector);
    }
}

// Publishes a real FGTM/FENE actor and its target
// callbacks only after the stage's ready/active/cooldown counters transition.
// @verified - p
std::uint8_t vfRegisterContactSlot(VFMA_t *game, FCMG_t *slot) {
    Character *const character = slot->character;
    std::uint8_t index = 0;
    while (game->characters[index]) {
        if (++index >= 6)
            return 99;
    }
    const auto team = slot->counterGroup;
    switch (slot->state) {
    case 0:
    case 4:
        if (!BindAllocatedEnemySlot(slot))
            return 99;
        break;
    case 1:
        if (slot->group && (slot->group->activationState & 0xffU) != 0)
            return 99;
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2:
        return 99;
    case 3:
        if (!ReactToDead(game, slot))
            return 99;
        --g_pFightStage->groupCounters[team];
        break;
    default:
        break;
    }
    FRMG_t *const formation = slot->group;
    game->characters[index] = character;
    game->groups[index] = formation;
    game->enemySlots[index] = slot;
    slot->runningTicks = 0.0f;
    slot->state = 2;
    formation->state = 1;
    g_activeFightData[slot->fightEntryIndex].status = 1;
    VFActor *const actor = slot->fighter;
    game->actors[index] = actor;
    slot->actorIndex = index;
    game->actorSlots[index] = actor->slot;
    actor->pairedCharacter = nullptr;
    actor->targetingCharacter = nullptr;
    actor->pairedActor = nullptr;
    actor->actor = nullptr;
    actor->groupIndex = index;
    actor->pairedSlot = -1;
    actor->actionIndex = -1;
    slot->position = GetCharacterWorldPosition(character);
    actor->contactPosition = actor->position = slot->position;
    actor->fightStyle = formation->style;
    actor->activeMotionFlags = 0;
    actor->hitStateFlags = 0;
    actor->moveFlags = 0;
    actor->motionFlags = actor->secondaryMotionFlags = 0;
    vfRegisterGroundTargetCallbacks(slot->motion, actor);
    game->slotTeams[index] = formation->team;
    game->slotPoolModes[index] = formation->poolMode;
    if (formation->poolMode == 0)
        game->playerSlot = static_cast<std::int8_t>(index);
    ++g_pFightStage->activeFightersByTeam[team];
    return index;
}

// The authored drag adjustment is 0.5 (g_vfKnockbackDragScale).
// @verified - p
void vfApplyKnockbackPhysics(VFActor *self) {
    const Vector3 previous = self->position;
    const VFMA_t &game = *self->game;
    const float drag =
        (((self->knockbackBaseParameter - 60.0f) * 0.5f + 60.0f) * (game.dragScale * game.gravityScale) * 0.033333335f) * (self->contactTimer + 1.0f);
    const float knockbackDrag = self->scale * drag;
    const float slideDrag = self->scale * (drag * game.slideDragScale);
    bool movingKnockback = false;
    if ((self->hitStateFlags & VFHSF_FallLandingRequired) == 0) {
        const float length = std::sqrt(self->knockbackVelocity.x * self->knockbackVelocity.x + self->knockbackVelocity.z * self->knockbackVelocity.z);
        if (length <= knockbackDrag)
            self->knockbackVelocity.x = self->knockbackVelocity.z = 0.0f;
        else {
            movingKnockback = true;
            const float scale = 1.0f - knockbackDrag / length;
            self->knockbackVelocity.x *= scale;
            self->knockbackVelocity.z *= scale;
        }
    }
    const float slideLength = std::sqrt(self->slideVelocity.x * self->slideVelocity.x + self->slideVelocity.z * self->slideVelocity.z);
    if (slideLength <= slideDrag) {
        self->slideVelocity.x = self->slideVelocity.z = 0.0f;
        if (!movingKnockback)
            self->contactTimer = 0.0f;
    } else {
        const float scale = 1.0f - slideDrag / slideLength;
        self->slideVelocity.x *= scale;
        self->slideVelocity.z *= scale;
    }
    if ((self->moveFlags & VFMF_Airborne) != 0)
        self->knockbackVelocity.y = (self->knockbackVelocity.y - self->scale * self->scale * game.gravityScale) + self->slideVelocity.y;
    self->position.x = (self->position.x + self->knockbackVelocity.x) + self->slideVelocity.x;
    self->position.z = (self->position.z + self->knockbackVelocity.z) + self->slideVelocity.z;
    self->position.y += self->knockbackVelocity.y;
    self->frameMoveDelta = {self->position.x - previous.x, self->position.y - previous.y, self->position.z - previous.z};
}

// @verified - p
void vfUpdatePositionXZ(VFActor *self) {
    if (self->game->type != 4 || self->fightStyle != 0 || self->animationTime != 0.0f ||
        ((self->secondaryMotionFlags & VFSMF_Unknown8000000) == 0 && (self->motionFlags & (VFMOF_Unknown10 | VFMOF_Unknown1000)) == 0))
        return;
    const auto angle = static_cast<std::uint16_t>(-self->facingAngle);
    self->actor->position.x = sin_fixed16(static_cast<std::int16_t>(angle)) * 3.0f + self->position.x;
    self->actor->position.z = cos_fixed16(angle) * -3.0f + self->position.z;
}

// @verified - p
void vfResolveKnockbackLanding(VFActor *self) {
    if ((self->moveFlags & VFMF_Airborne) == 0)
        return;
    if ((self->motionContactFlags & 1U) == 0) {
        const auto angle = static_cast<std::uint16_t>(-self->facingAngle);
        const float x = cos_fixed16(angle) * self->moveDelta.x - sin_fixed16(static_cast<std::int16_t>(angle)) * self->moveDelta.z;
        const float z = sin_fixed16(static_cast<std::int16_t>(angle)) * self->moveDelta.x + cos_fixed16(angle) * self->moveDelta.z;
        self->position.x -= x;
        self->position.z -= z;
    }
    self->moveDelta.y = self->contactPosition.y - self->position.y;
    self->moveFlags &= ~VFMF_Airborne;
    self->hitStateFlags &= ~VFHSF_FallLandingRequired;
    mtwkSetExtraFlag(self->motion, 3, 2);
}

// @verified - p
void vfUpdateAutoAdvanceLanding(VFActor *self) {
    if ((self->game->gameFlags & 0x20U) != 0 || (self->moveFlags & VFMF_AutoAdvanceMotion) == 0)
        return;
    self->moveFlags &= ~VFMF_AnimationClamp;
    if ((self->moveFlags & VFMF_Airborne) != 0) {
        if ((self->moveFlags & VFMF_Ascending) == 0) {
            const float velocity = self->knockbackVelocity.y - self->scale * self->scale * self->game->gravityScale;
            if (self->targetGroundY + self->groundOffset > velocity + self->contactPosition.y || self->targetGroundY > velocity + self->position.y)
                self->moveFlags |= VFMF_AnimationClamp;
        }
    } else {
        if (self->targetGroundY != self->position.y && self->targetGroundY + 0.5f >= self->position.y)
            self->position.y = self->targetGroundY;
        if ((self->motionFlags & VFMOF_Unknown10) != 0) {
            if (self->animationTime < static_cast<float>(self->parameter2d6) || static_cast<float>(self->parameter2e0) <= self->animationTime)
                self->hitStateFlags &= ~VFHSF_FallLandingRequired;
            else
                self->hitStateFlags |= VFHSF_FallLandingRequired;
        }
    }
}

// @verified - p
void vfUpdateAirborneAnimationTiming(VFActor *self) {
    if ((self->moveFlags & VFMF_AnimationClamp) != 0) {
        if ((self->moveFlags & VFMF_Airborne) != 0) {
            self->knockbackVelocity.y = 0.0f;
            self->position.y = self->targetGroundY;
            if (self->targetGroundY > self->minimumGroundY)
                self->knockbackVelocity.x = self->knockbackVelocity.z = 0.0f;
            vfResolveKnockbackLanding(self);
        }
        if (self->parameter2e0 != 0 && static_cast<float>(self->parameter2e0) > self->animationTime && (self->activeMotionFlags & VFSMF_Unknown1) == 0)
            self->animationTime = self->parameter4c8 = static_cast<float>(self->parameter2e0);
    } else if ((self->moveFlags & VFMF_Airborne) != 0) {
        if (self->knockbackVelocity.y < 0.0f)
            self->moveFlags &= ~VFMF_Ascending;
        if (self->animationTime > static_cast<float>(self->parameter2de)) {
            const std::int32_t frame = self->parameter2e0 - static_cast<std::int16_t>(self->predictedKnockbackFrames) + 1;
            if (frame <= 0 || static_cast<float>(frame) < self->animationTime)
                self->animationTime = self->parameter4c8 = self->animationTime - self->scale;
        }
    }
}

// @verified - p
void vfUpdateGroundHeightAndEnemyContacts(VFActor *self) {
    std::uint32_t nearby = 0;
    const float ground = self->groundY + vfAccumEnemyGroundOffsetAndMask(&self->position, &nearby);
    if (ground == self->groundY) {
        if (self->targetGroundY > self->groundY) {
            self->targetGroundY = self->groundY;
            if ((self->moveFlags & VFMF_Airborne) == 0) {
                self->position.y = self->groundY;
                shenApplyTranslationToAll(&self->character->entity.node, self->position);
            }
        }
        return;
    }
    self->targetGroundY = ground;
    if ((self->moveFlags & VFMF_Airborne) != 0)
        return;
    self->position.y = ground;
    shenApplyTranslationToAll(&self->character->entity.node, self->position);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || (nearby & (1U << i)) == 0 || (group.collisionFlags & 8U) != 0 || (group.collisionFlags & 1U) == 0)
            continue;
        group.collisionFlags |= 8U;
        const float frame = GetMotionFrame(group.motion, MotionRegion::LowerBody);
        const float end = GetMotionEndFrame(group.motion, MotionRegion::LowerBody);
        const MotnSequence *const sequence = FindLoadedMotionSequence(GetMotionAnimationId(group.motion, MotionRegion::LowerBody));
        if ((group.collisionFlags & 0x10U) != 0 || (frame != end && !(sequence && sequence->allowsFrameOverflow)))
            continue;
        const std::uint16_t animation = (group.collisionFlags & 0x20U) != 0 ? 455 :
            (group.collisionFlags & 0x40U) != 0                             ? 454 :
            (group.collisionFlags & 2U) != 0                                ? 108 :
                                                                              107;
        vfUpdateDefaultReactionAnim(&group, animation);
        SetMotionRegionFlag(group.motion, 69, (group.collisionFlags & 0x80U) != 0 ? 0 : 2, MotionRegion::LowerBody);
    }
}

namespace {

float FightIntersectionDistance(const VFActor &actor, std::uint16_t angle, float length) {
    const Vector3 &origin = (actor.moveFlags & VFMF_RefreshContactHistory) != 0 ? actor.contactPosition : actor.motionContactSamples[0];
    const Vector3 destination{origin.x - sin_fixed16(static_cast<std::int16_t>(angle)) * length, origin.y, origin.z - cos_fixed16(angle) * length};
    Vector3 intersection;
    RaycastForCollision(&intersection, &destination, &origin);
    const float x = origin.x - intersection.x, z = origin.z - intersection.z;
    return std::sqrt(x * x + z * z);
}

std::uint16_t PredictFightKnockbackFrames(const VFActor &actor, float height) {
    if ((actor.moveFlags & VFMF_Airborne) == 0)
        return 0;
    float y = actor.contactPosition.y, velocity = actor.knockbackVelocity.y;
    const float acceleration = actor.scale * actor.scale * actor.game->gravityScale - actor.slideVelocity.y;
    std::uint16_t frames = 0;
    for (;;) {
        y += velocity;
        velocity -= acceleration;
        if (height > y && velocity < 0.0f)
            return frames;
        ++frames;
    }
}



Vector3 FightBoneLocalPosition(Character *character, std::int32_t bone) {
    Matrix4x4 *const matrix = charaGetBoneTransform(FindMotionWork(character), bone);
    if (!matrix)
        return GetCharacterWorldPosition(character);
    Vector3 position{matrix->rows[3].x, matrix->rows[3].y, matrix->rows[3].z};
    const CharacterNode *const child = character->entity.node.child;
    if (!child)
        return position;
    const Matrix4x4 saved = workTransformMatrix;
    Matrix4x4::SetPrePassToTempTransform();
    ApplyRecursiveCharacterNodeTransforms(child);
    const Matrix4x4 transform = workTransformMatrix;
    const float x = position.x - transform.rows[3].x, y = position.y - transform.rows[3].y, z = position.z - transform.rows[3].z;
    position = {transform.rows[0].y * y + transform.rows[0].x * x + transform.rows[0].z * z,
        transform.rows[1].y * y + transform.rows[1].x * x + transform.rows[1].z * z,
        transform.rows[2].y * y + transform.rows[2].x * x + transform.rows[2].z * z};
    Matrix4x4::SetActiveTransform(&saved);
    return position;
}

void CaptureFightHitTransform(Character *character, std::int32_t bone, Matrix4x4 &output) {
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::MultiplyWithPrePassMatrix(charaGetBoneTransform(FindMotionWork(character), bone));
    Matrix4x4::GetTransformMatrix(&output);
    Matrix4x4::Pop();
    output.rows[3].x = output.rows[3].y = output.rows[3].z = 0.0f;
    Matrix4x4::SetActiveTransform(&output);
}

std::uint16_t FightFootFacingAngle() {
    static constexpr Vector3 forward{0.0f, 0.0f, -1.0f}, up{0.0f, 1.0f, 0.0f};
    Vector3 first, second;
    Matrix4x4::TransformPoint(forward, first);
    Matrix4x4::TransformPoint(up, second);
    if (first.y < -0.2f)
        return FixedAtan2(-second.x, second.y);
    const std::uint16_t angle = first.y > 0.2f ? FixedAtan2(-second.x, second.y) : FixedAtan2(first.x, first.z);
    return static_cast<std::uint16_t>(angle + 0x8000U);
}

} // namespace

// @verified - p
void vfUpdateHitFacingAndKnockback(VFActor *self) {
    self->contactVectors[0] = self->contactPosition;
    const auto readPosition = [self](std::int32_t bone, Vector3 &position) {
        const Matrix4x4 *const transform = charaGetBoneTransform(self->motion, bone);
        if (transform)
            position = {transform->rows[3].x, transform->rows[3].y, transform->rows[3].z};
    };
    readPosition(0, self->contactPosition);
    static constexpr std::int32_t bones[6]{3, 28, 26, 5, 8, 15};
    for (std::uint8_t i = 0; i < 6; ++i)
        readPosition(bones[i], self->contactSamples[i]);
    const float height = self->contactSamples[1].y - (self->targetGroundY + 1.5f);
    std::memcpy(&self->actorRuntimeFlags, &height, sizeof(height));
    self->temporaryFacingAngle =
        static_cast<std::uint16_t>(self->facingAngle + ((self->hitStateFlags & VFHSF_FacingStepOnTransition) != 0 ? self->facingAngleDelta : 0));
    if (self->actionTriggerPhase == 4) {
        self->actionTriggerPhase = 0;
        if ((self->hitStateFlags & VFHSF_SpecialThrow) != 0) {
            self->hitStateFlags &= ~VFHSF_SpecialThrow;
            self->game->hitStopFrames = static_cast<std::uint8_t>(static_cast<std::int32_t>(self->inverseScale * 6.0f));
        }
    }
    self->previousFootContactState = self->footContactState;
    if ((self->hitStateFlags & (VFHSF_FacingStepOnTransition | VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) == 0)
        self->facingYaw = self->facingAngle;
    else {
        Matrix4x4_Push();
        CaptureFightHitTransform(self->character, 3, self->chestHitTransform);
        static constexpr Vector3 down{0.0f, -1.0f, 0.0f}, right{1.0f, 0.0f, 0.0f};
        Vector3 first, second;
        Matrix4x4::TransformPoint(down, first);
        Matrix4x4::TransformPoint(right, second);
        std::memcpy(&self->hitBasisPrefix, &first.x, sizeof(first.x));
        self->hitYaw = {first.y, first.z, second.x};
        std::memcpy(self->hitBasisAngles, &second.y, sizeof(second.y));
        self->hitBasisZ = second.z;
        CaptureFightHitTransform(self->character, 31, self->leftLegHitTransform);
        self->footFacingAngles[0] = FightFootFacingAngle();
        CaptureFightHitTransform(self->character, 26, self->rightLegHitTransform);
        self->footFacingAngles[1] = FightFootFacingAngle();
        Matrix4x4::Pop();
        if ((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) != 0) {
            const float threshold = (self->motionFlags & VFMOF_ContactCategoryB) != 0 ? 0.4f : -0.4f;
            if (threshold < self->hitYaw.x)
                self->hitStateFlags &= ~VFHSF_ReversedFacing;
            else
                self->hitStateFlags |= VFHSF_ReversedFacing;
        }
        Vector3 left = FightBoneLocalPosition(self->character, 26);
        const Vector3 rightFoot = FightBoneLocalPosition(self->character, 31);
        const Vector3 root = FightBoneLocalPosition(self->character, 0);
        const float x = rightFoot.x - left.x, z = rightFoot.z - left.z;
        self->footDistance = std::sqrt(x * x + z * z);
        self->pendingAnimation = 0;
        if (std::fabs(rightFoot.y - root.y) <= 0.4f)
            self->pendingAnimation |= 1U;
        if (std::fabs(left.y - root.y) <= 0.4f)
            self->pendingAnimation |= 2U;
        self->footHeightDifference = std::fabs(rightFoot.y - left.y);
        bool fromRoot = false;
        if (self->pendingAnimation >= 3) {
            left.x = (left.x + rightFoot.x) * 0.5f;
            left.z = (left.z + rightFoot.z) * 0.5f;
            self->footFacingMode = static_cast<std::uint16_t>((self->footFacingMode & 0xff00U) | 2U);
            fromRoot = true;
        } else if (self->pendingAnimation != 0 && (self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) != 0) {
            if (self->pendingAnimation == 1) {
                left.x = rightFoot.x;
                left.z = rightFoot.z;
            }
            self->footFacingMode = static_cast<std::uint16_t>((self->footFacingMode & 0xff00U) | 2U);
            fromRoot = true;
        } else if (self->footHeightDifference < 0.4f || self->footDistance <= 0.4f) {
            left.x = (left.x + rightFoot.x) * 0.5f;
            left.z = (left.z + rightFoot.z) * 0.5f;
            self->footFacingMode = static_cast<std::uint16_t>((self->footFacingMode & 0xff00U) | 1U);
            fromRoot = true;
        } else {
            self->footFacingMode &= 0xff00U;
            self->facingYaw =
                rightFoot.y <= left.y ? FixedAtan2(rightFoot.x - left.x, rightFoot.z - left.z) : FixedAtan2(left.x - rightFoot.x, left.z - rightFoot.z);
        }
        if (fromRoot) {
            const auto facing = FixedAtan2(root.x - left.x, root.z - left.z);
            std::uint16_t reverse = self->hitYaw.x < 0.0f ? 0x8000U : 0U;
            if ((left.z - root.z) * self->hitBasisZ + (left.x - root.x) * self->hitYaw.z >= 0.0f)
                reverse ^= 0x8000U;
            if ((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) != 0) {
                if ((reverse & 0x8000U) != 0)
                    self->hitStateFlags |= VFHSF_ReversedFacing;
                else
                    self->hitStateFlags &= ~VFHSF_ReversedFacing;
            }
            self->facingYaw = static_cast<std::uint16_t>(facing + reverse);
        }
    }
    if (((self->hitStateFlags & VFHSF_MirroredReactionAngle) != 0) != ((self->moveFlags & VFMF_MirrorReactionAngle) != 0))
        self->moveFlags |= VFMF_MirroredStance;
    else
        self->moveFlags &= ~VFMF_MirroredStance;
    float lowest = 32000.0f;
    for (const Vector3 &sample : self->motionContactSamples)
        lowest = std::fmin(sample.y, lowest);
    self->lowestMotionContactY = lowest;
    if (self->game->type != 1 || self->groupIndex == self->game->playerSlot) {
        const auto facing = self->temporaryFacingAngle;
        self->leftIntersectionDistance = FightIntersectionDistance(*self, static_cast<std::uint16_t>(facing + 12288), 4.0f);
        self->rightIntersectionDistance = FightIntersectionDistance(*self, static_cast<std::uint16_t>(facing - 12288), 4.0f);
        self->forwardIntersectionDistance = FightIntersectionDistance(*self, facing, 4.0f);

        self->forwardIntersectionTag = 0;
        self->backwardIntersectionDistance = FightIntersectionDistance(*self, static_cast<std::uint16_t>(facing + 0x8000U), 4.0f);
        self->backwardIntersectionTag = 0;
    }
    if (((self->temporaryFacingAngle - static_cast<std::uint16_t>(camCtrlGetNextRotY())) & 0x8000U) != 0)
        self->moveFlags &= ~VFMF_Unknown02;
    else
        self->moveFlags |= VFMF_Unknown02;
    std::uint32_t guard = 0;
    if ((self->hitStateFlags & VFHSF_AttackActive) != 0) {
        switch (self->guardState) {
        case 0:
            guard = 4;
            [[fallthrough]];
        case 1:
        case 3:
        case 4:
            if ((self->hitParameter & 0x200U) == 0)
                guard |= 1U;
            if (guard == 0)
                guard = 0x8000U;
            break;
        case 2:
        case 5:
        case 6:
            guard = (self->hitParameter & 0x200U) == 0 ? 10U : 8U;
            break;
        default:
            guard = 0x8000U;
            break;
        }
    }
    self->guardContactFlags = guard;
    if ((self->game->gameFlags & 0x20U) == 0) {
        if ((self->hitStateFlags & (VFHSF_ReversedFacing | VFHSF_CameraFocusTarget)) == 0x20000800U)
            self->hitFacingTimer = 30.0f;
        else if (self->hitFacingTimer > 0.0f) {
            self->hitFacingTimer -= shenGetScaledDeltaTickTime(1.0f);
            if (self->hitFacingTimer < 0.0f)
                self->hitFacingTimer = 0.0f;
        }
        const bool secondHalf = static_cast<std::int16_t>(static_cast<double>(self->currentAnimationEndFrame) * 0.5) < self->animationTime;
        const bool turning =
            (self->hitStateFlags & VFHSF_Unknown8) != 0 ? self->animationHalfPhase < 0 || secondHalf : self->animationHalfPhase < 0 && !secondHalf;
        if (turning)
            self->moveFlags |= VFMF_Unknown100;
        else
            self->moveFlags &= ~VFMF_Unknown100;
        float groundParameter;
        std::memcpy(&groundParameter, &self->groundParameter, sizeof(groundParameter));
        self->predictedKnockbackFrames = PredictFightKnockbackFrames(*self, groundParameter + self->groundOffset);
        self->previousFacingAngle = static_cast<std::int16_t>(self->facingAngle);
        self->copiedHitStateFlags = self->hitStateFlags;
    }
}

// @verified - p
void vfRefreshContactReactionState(VFActor *actor) {
    vfUpdateHitFacingAndKnockback(actor);
    vfUpdateContactSlotsAndGroundFlags(actor);
    vfSelectDefaultReactionAndGuardAnims(actor);
}

// @verified - p
void vfUpdateContactSlotsAndGroundFlags(VFActor *self) {
    VFMA_t &game = *self->game;
    if (self->actionIndex == -1 || self->pairedSlot == -1)
        return;
    const auto team = game.slotTeams[self->groupIndex];
    VFActor &paired = *game.actors[self->pairedSlot];
    VFActor &opponent = *game.actors[self->actionIndex];
    const std::uint32_t selfHit = self->hitStateFlags;
    const std::uint32_t pairedHit = paired.hitStateFlags;
    const std::uint32_t opponentHit = opponent.hitStateFlags;
    bool nearOpponent = game.state != 3;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game.characters[i]) {
            self->contactDistances[i] = 0.0f;
            continue;
        }
        const Vector3 &position = game.actors[i]->contactPosition;
        const float x = self->contactPosition.x - position.x, z = self->contactPosition.z - position.z;
        const float distance = std::sqrt(z * z + x * x);
        self->contactDistances[i] = distance;
        if (game.slotTeams[i] != team && distance < 7.0f)
            nearOpponent = true;
    }
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game.characters[i]) {
            self->contactAngles[i] = 0;
            continue;
        }
        const Vector3 &position = game.actors[i]->contactPosition;
        self->contactAngles[i] = static_cast<std::int16_t>(FixedAtan2(self->contactPosition.x - position.x, self->contactPosition.z - position.z));
    }
    const float distance = self->contactDistances[self->pairedSlot];
    const auto pairedAngle = self->contactAngles[self->pairedSlot];
    const auto selfAngle = static_cast<std::int16_t>(self->temporaryFacingAngle - pairedAngle);
    const auto otherAngle = static_cast<std::int16_t>(paired.temporaryFacingAngle - pairedAngle);
    const auto rotation = static_cast<std::uint16_t>(-opponent.facingAngle);
    const float x = self->moveDelta.x - opponent.moveDelta.x, z = self->moveDelta.z - opponent.moveDelta.z;

    self->relativeMoveDelta.x = cos_fixed16(rotation) * x - sin_fixed16(static_cast<std::int16_t>(rotation)) * z;
    self->relativeMoveDelta.z = sin_fixed16(static_cast<std::int16_t>(rotation)) * x + cos_fixed16(rotation) * z;
    std::uint32_t ground = nearOpponent ? 4U : 0U;
    const auto angle = self->contactAngles[self->actionIndex];
    if (((angle - static_cast<std::int16_t>(self->temporaryFacingAngle) + 0x4000) & 0x8000) != 0)
        ground |= 1U;
    if (((angle - static_cast<std::int16_t>(opponent.temporaryFacingAngle) + 0x4000) & 0x8000) == 0)
        ground |= 2U;
    std::uint32_t reaction = 0;
    const auto inAngleRange = [](std::int32_t angleValue, std::uint16_t offset, std::uint16_t extent) {
        return static_cast<std::uint16_t>(angleValue + offset) <= extent;
    };
    const bool pairedGuardEligible = (pairedHit & 0x4000U) == 0 || (paired.activeMotionFlags & 0x100000U) != 0;
    if ((self->moveFlags & VFMF_Unknown40000000) != 0 && (self->stateFlags & VFSF_Unknown80) == 0 && (paired.stateFlags & 0x80U) == 0 &&
        (selfHit & 0x8dc100U) == 0 && (self->activeMotionFlags & VFSMF_Unknown800000) == 0 && (pairedHit & 0x8d8110U) == 0 &&
        (paired.activeMotionFlags & 0x800020U) == 0 && !(std::fabs(self->hp) < 0.00000011920929f) && !(std::fabs(paired.hp) < 0.00000011920929f) &&
        pairedGuardEligible && paired.fightStyle != 1 && (std::to_integer<std::uint8_t>(paired.motionParameters[2][25]) & 2U) == 0 &&
        static_cast<std::uint16_t>((paired.currentAnimation & 0xfffU) - 58) > 1U && game.guardTimer > 0.0f) {
        const bool selfFront = inAngleRange(selfAngle, 8190, 0x3ffc);
        const bool selfBack = inAngleRange(selfAngle ^ 0x8000, 8190, 0x3ffc);
        const bool pairedFront = inAngleRange(otherAngle, 8190, 0x3ffc);
        const bool pairedBack = inAngleRange(otherAngle ^ 0x8000, 8190, 0x3ffc);
        const bool selfGrounded = (selfHit & 0x10U) == 0;
        const bool pairedTurning = (paired.moveFlags & 0x100U) != 0;
        if (selfBack && pairedBack && selfGrounded && self->rearContactRange >= distance)
            reaction = 2;
        else if (selfFront) {
            if (pairedFront && selfGrounded && self->rearContactRange >= distance)
                reaction = pairedTurning ? 16 : 8;
            else if (pairedBack && self->frontContactRange >= distance)
                reaction = pairedTurning && selfGrounded ? 4 : selfGrounded ? 1 : 0x400000;
            else if (inAngleRange(otherAngle, static_cast<std::uint16_t>(-8194), 0x3ffc) && selfGrounded && self->sideContactRange >= distance)
                reaction = pairedTurning ? 0x20000000 : 0x4000000;
            else if (inAngleRange(otherAngle, 24574, 0x3ffc) && selfGrounded && self->sideContactRange >= distance)
                reaction = pairedTurning ? 0x10000000 : 0x2000000;
        }
    }
    bool opponentGuard;
    if ((opponent.moveFlags & 0x20000000U) != 0)
        opponentGuard = true;
    else if ((opponentHit & 0x8100U) != 0)
        opponentGuard = false;
    else if ((opponentHit & 0x10000U) != 0)
        opponentGuard = true;
    else if ((opponentHit & 0x4000U) != 0)
        opponentGuard = (opponentHit & 0x10U) == 0 || self->predictedKnockbackFrames == 0;
    else
        opponentGuard = false;
    if (opponentGuard)
        ground |= 0x20U;
    std::uint32_t mask = 0;
    if ((self->moveFlags & VFMF_Unknown40000000) != 0 && (pairedHit & 0x102U) == 0x102U && inAngleRange(selfAngle, 5460, 0x2aa8) &&
        (inAngleRange(otherAngle ^ 0x8000, 5460, 0x2aa8) || (paired.currentAnimation & 0xfffU) == 0xc5cU)) {
        mask = distance > 2.0f ? 0x733f00U : 0x7fff00U;
        if (static_cast<double>(distance) > 1.8)
            mask &= 0xffafd0ffU;
        if (static_cast<double>(distance) > 1.6)
            mask &= ~0x1000U;
        mask &= opponent.reactionEntryFlags;
    }
    self->hitFlags = mask;
    if ((self->pairedContactPriority & 1U) == 0)
        self->pairedContactPriority = static_cast<std::uint32_t>(paired.pairContactPriority);
    if (((self->pairedContactPriority & 1U) != 0 || paired.pairContactPriority != 0) &&
        opponent.animationTime > static_cast<float>(paired.pairContactEndFrame)) {
        self->pairedContactPriority = 0;
        paired.pairContactPriority = 0;
    }
    std::uint32_t orientation = (opponent.reactionEntryFlags & 1U) != 0 ? 0x48U : 0U;
    if ((self->moveFlags & VFMF_MirroredStance) != 0)
        orientation ^= 8U;
    if ((opponent.moveFlags & 0x400U) != 0)
        orientation ^= 0x40U;
    ground |= orientation;
    const std::uint32_t facingDifference = ((selfHit ^ opponentHit) & 0x200000U) != 0 ? 0x40U : selfHit ^ opponentHit;
    if (((self->moveFlags ^ opponent.moveFlags ^ facingDifference) & 0x40U) != 0)
        ground |= 0x10U;
    if (self->forwardIntersectionDistance <= 2.5f)
        reaction |= 0x100U;
    if (self->backwardIntersectionDistance <= 1.5f)
        reaction |= 0x200U;
    self->reactionFlags = reaction;
    self->groundContactFlags = ground;
}



// @verified - p
void vfUpdateSlotRuntimeMetricsAndTimers(VFActor *self) {
    VFSlotRuntimeState &slot = *self->slot;
    VFActor *actor = self->actor;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!g_pFightMain->characters[i])
            continue;
        actor = g_pFightMain->actors[i];
        slot.healthDifferences[i] = self->hp - actor->hp;
        const auto difference = static_cast<std::int16_t>(self->facingAngle - self->contactAngles[i]);
        slot.facingDifferences[i] = static_cast<std::uint16_t>(std::abs(static_cast<std::int32_t>(difference)));
    }
    slot.randomBytes[0] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    slot.randomBytes[1] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    slot.randomWord = IncrementSharedPrngUInt16();
    slot.previousTicks = slot.ticks;
    const float delta = shenGetScaledDeltaTickTime(1.0f);
    if (static_cast<std::int32_t>(slot.ticks) < 65535)
        slot.ticks += delta;
    if (slot.reactionTicks < 60000.0f)
        slot.reactionTicks += delta;
    const auto decrement = [delta](float &timer) {
        if (timer > 0.0f) {
            timer -= delta;
            if (timer < 0.0f)
                timer = 0.0f;
        }
    };
    if ((self->hitStateFlags & VFHSF_MidAirRecovery) != 0)
        slot.hitTimer = static_cast<float>(slot.hitTimerMaximum);
    else
        decrement(slot.hitTimer);
    if ((self->hitStateFlags & VFHSF_AttackActive) != 0)
        slot.guardTimer = static_cast<float>(slot.guardTimerMaximum);
    else
        decrement(slot.guardTimer);
    if ((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) != 0)
        slot.guardTicks += delta;
    else {
        slot.guardTicks = 0.0f;
        slot.guardPhase = 0;
    }
    if ((actor->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) != 0)
        slot.opponentGuardTicks += delta;
    else
        slot.opponentGuardTicks = 0.0f;
    if ((actor->hitStateFlags & VFHSF_LightReaction) != 0) {
        if (static_cast<std::int32_t>(slot.opponentAttackTimer) < 255)
            slot.opponentAttackTimer += delta;
    } else
        decrement(slot.opponentAttackTimer);
    if ((self->hitStateFlags & VFHSF_AirborneOrDowned) != 0) {
        if (slot.pressure < 240)
            slot.pressure += 3;
    } else if (slot.pressure != 0)
        --slot.pressure;
    decrement(slot.parameterBc);
    if ((actor->hitStateFlags & VFHSF_ForcedReaction) != 0) {
        if (static_cast<std::int32_t>(slot.opponentMotionTimer) < 240)
            slot.opponentMotionTimer += 8.0f;
    } else
        decrement(slot.opponentMotionTimer);
    slot.parameterE4 = 0;
    if ((self->activeMotionFlags & VFSMF_Unknown800000) != 0 && slot.motionTicks < 60000.0f)
        slot.motionTicks += delta;
    decrement(slot.hpPhaseTimer);
    if (!(slot.motionTimer > 0.0f)) {
        if ((self->motionFlags & VFMOF_Unknown80000000) != 0)
            slot.motionTimer = 90.0f;
    } else
        decrement(slot.motionTimer);
}

// @verified - p
void vfUpdateSlotReactionHistory(VFActor *self) {
    VFSlotRuntimeState &slot = *g_pFightMain->actorSlots[self->groupIndex];
    const VFActor &opponent = *g_pFightMain->actors[self->actionIndex];
    if ((opponent.motionFlags & 0x100U) != 0 && (self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_Unknown20000)) != 0 &&
        self->currentAnimation != self->previousAnimation && (opponent.motionFlags & 2U) == 0 && (self->hitStateFlags & VFHSF_GuardState) == 0) {
        auto index = static_cast<std::uint8_t>(slot.parameterA3);
        slot.animationHistory[index++] = opponent.currentAnimation & 0xfffU;
        slot.parameterA3 = static_cast<std::int8_t>(index > 3 ? 0 : index);
    }
    slot.cameraFlags &= ~0x400U;
    for (std::uint8_t i = 0; i < 3; ++i) {
        if (slot.animationHistory[i] == 0)
            continue;
        for (std::uint8_t j = i + 1; j < 4; ++j) {
            if (slot.animationHistory[i] != slot.animationHistory[j])
                continue;
            slot.cameraFlags |= 0x400U;
            slot.parameterA0 = slot.animationHistory[i];
            break;
        }
        if ((slot.cameraFlags & 0x400U) != 0)
            break;
    }
    const VFActor *const second = self->secondActor;
    const auto reactionByte = [](const VFActor &actor) {
        std::uint32_t bits;
        std::memcpy(&bits, &actor.reactionPower, sizeof(bits));
        return static_cast<std::int8_t>(bits >> 24);
    };
    if ((self->previousHitStateFlags & VFHSF_AirborneOrDowned) == 0 && (self->hitStateFlags & VFHSF_AirborneOrDowned) != 0 && second &&
        second->fightStyle == 0 && second->pairContactPriority != 0) {
        auto index = static_cast<std::uint8_t>(slot.parameterA4);
        slot.reactionHistory[index++] = static_cast<std::uint16_t>(static_cast<std::int16_t>(reactionByte(*second)));
        slot.parameterA4 = static_cast<std::int8_t>(index == 3 ? 0 : index);
    }
    slot.parameterA2 = 0;
    if (second && second->fightStyle == 0 && slot.reactionHistory[1] != 0) {
        for (const auto reaction : slot.reactionHistory)
            if (static_cast<std::int32_t>(reactionByte(*second)) == static_cast<std::int32_t>(reaction))
                ++slot.parameterA2;
    }
}

// @verified - p
void vfUpdateSlotRankParameters(VFActor *self) {
    VFSlotRuntimeState &slot = *g_pFightMain->actorSlots[self->groupIndex];
    const std::uint8_t rank = self->reactionRank < 2 ? self->reactionRank : 2;
    std::memcpy(&slot.parameter2a, self->motionParameters[2] + 26 + 9 * rank, 9);
    if (slot.parameter129 > 20) {
        if (slot.parameter2a != 0)
            slot.parameter2a = -1;
        slot.parameter129 = 0;
    }
}

// @verified - p
void vfUpdateCurrentAssistWindow(VFActor *self) {
    const VFActor &actor = *self->actor;
    VFSlotRuntimeState &slot = *self->slot;
    if ((actor.hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) != 0)
        slot.assistWindowCurrent = slot.assistWindowGuard;
    else if ((actor.groundContactFlags & 1U) != 0)
        slot.assistWindowCurrent = (static_cast<std::uint8_t>(g_pFightMain->type - 2U) & 0xfdU) != 0 ? 1.2f : 1.5f;
    else if ((actor.hitStateFlags & (VFHSF_Unknown4 | VFHSF_AttackActive)) != 0)
        slot.assistWindowCurrent = 2.5f;
    else
        slot.assistWindowCurrent = slot.assistWindowDefault;
}


// @verified - p
void vfUpdateScenarioHPPhaseConfig(VFActor *self) {
    const auto flags = self->actor->stateFlags;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if ((flags & (i < 5 ? 0x1000U : 0x2000U)) == 0)
            continue;
        VFSlotPhaseConfig &phase = self->slot->phases[i];
        if (phase.enabled == 0)
            continue;
        if (phase.countdown != 0)
            --phase.countdown;
        else {
            phase.countdown = phase.choices[IncrementSharedPrngUInt16() & 0xfU];
            if (phase.countdown > 1)
                --phase.countdown;
            phase.selected = static_cast<std::uint8_t>(IncrementSharedPrngUInt16() % (static_cast<std::uint32_t>(phase.countdown) + 1U));
        }
    }
}

// @verified - p
void vfInitializeSlotPhaseConfigurations(VFActor *self) {
    const auto initialize = [](VFSlotPhaseConfig &phase, std::uint8_t parameter) {
        if (parameter == 0) {
            phase.enabled = 0;
            return;
        }
        phase.enabled = 1;
        phase.reserved = parameter >= 0x80U ? 1 : 0;
        const float value = 255.0f / static_cast<float>(parameter >= 0x80U ? 256U - parameter : parameter);
        const auto integral = static_cast<std::uint8_t>(static_cast<std::int32_t>(value));
        std::memset(phase.choices, integral, sizeof(phase.choices));
        float remainder = value - static_cast<float>(integral);
        for (std::uint8_t index = 0; remainder >= 0.0625f; ++index) {
            ++phase.choices[index];
            remainder = static_cast<float>(static_cast<double>(remainder) - 0.0625);
        }
        phase.countdown = phase.choices[IncrementSharedPrngUInt16() & 0xfU];
        if (phase.countdown > 1)
            --phase.countdown;
        phase.selected = static_cast<std::uint8_t>(IncrementSharedPrngUInt16() % (static_cast<std::uint32_t>(phase.countdown) + 1U));
    };
    const std::uint8_t rank = self->reactionRank < 2 ? self->reactionRank : 2;
    const std::byte *const parameters = self->motionParameters[2] + 9 * rank;
    static constexpr std::uint8_t offsets[6]{27, 28, 29, 30, 32, 34};
    for (std::uint8_t i = 0; i < 6; ++i)
        initialize(self->slot->phases[i], std::to_integer<std::uint8_t>(parameters[offsets[i]]));
}

// @verified - p
void vfUpdateGuardLevel(VFMA_t *game, std::uint8_t index) {
    VFActor *const actor = game->actors[index];
    if (!actor)
        return;
    VFSlotRuntimeState *const slot = game->actorSlots[index];
    if (!slot)
        return;
    const auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    slot->id = scenGetCharaName(control->scenarioTask, actor->character).name;
    slot->actor = actor;
    slot->opponentIndex = 0xffU;
    std::memset(reinterpret_cast<std::byte *>(slot) + 0x68, 0, 0x140);
    slot->resetIndex = 0x7f7fU;
    slot->reserved62[0] = std::byte{0x7f};
    slot->hpPhaseTimer = 240.0f;
    slot->cameraFlags = 1;
    const auto *const parameters = reinterpret_cast<const std::uint8_t *>(actor->motionParameters[2]);
    const auto word = [parameters](std::size_t offset) {
        return static_cast<std::uint16_t>((static_cast<std::uint16_t>(parameters[offset]) << 8) | parameters[offset + 1]);
    };
    const auto fixed = [parameters](std::size_t offset) {
        const std::uint32_t bits = (static_cast<std::uint32_t>(parameters[offset]) << 24) | (static_cast<std::uint32_t>(parameters[offset + 1]) << 16) |
            (static_cast<std::uint32_t>(parameters[offset + 2]) << 8) | parameters[offset + 3];
        return static_cast<float>(static_cast<std::int32_t>(bits)) * 0.000015258789f;
    };
    slot->assistWindowCurrent = slot->assistWindowDefault = fixed(4);
    slot->assistWindowGuard = fixed(8);
    slot->parameter20 = word(12);
    slot->parameter22 = word(14);
    slot->hitTimerMaximum = word(16);
    slot->guardTimerMaximum = word(18);
    slot->parameter28 = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(parameters[20])));
    actor->guardLevel = static_cast<std::int8_t>(parameters[21]);
    actor->guardRateScale = 1.0f / static_cast<float>(static_cast<std::int8_t>(parameters[22]));
    slot->parameter1a5 = static_cast<std::int8_t>(parameters[23]);
    slot->secondaryMotionTimer = slot->motionTimer = 0.0f;
    if ((parameters[25] & 1U) != 0)
        actor->moveFlags |= VFMF_NoContactPairing;
    vfInitializeSlotPhaseConfigurations(actor);
}

// @verified - p
void vfPrepareEnemySlotMotion(FCMG_t *group) {
    Character *const character = group->character;
    MotionWork *const motion = group->motion;
    std::uint32_t flags = SetFlagForToken(character, SHEN_NAME(CCM_), 0) == 1 ? 4U : 0U;
    if ((motion->runtime.callbackFlags & 1U) != 0)
        flags |= 8U;
    else
        EV_mtwkSetEnabled(motion, true);
    if (mtwkSetExtraFlag(motion, 6, 0) != 0)
        flags |= 0x20U;
    if (shenCheckCharaHasAlphaManager(character))
        flags |= 0x80U;
    group->savedControlFlags = flags;
    group->savedMotionScale = GetMotionSpeed(motion, MotionRegion::LowerBody);
    SetMotionSpeed(motion, 1.0f, MotionRegion::LowerBody);
    group->savedRootMode = 0;
    for (const auto regionFlags : motion->runtime.regionFlags) {
        if ((regionFlags & 0x10U) != 0) {
            group->savedRootMode = motion->runtime.currentRootMode;
            break;
        }
    }
    mtwkSetRequestedRootMode(motion, 2);
    group->savedConstraintAngle = static_cast<std::uint16_t>(static_cast<std::int32_t>(motion->runtime.constraintStepLimit[2] * 10430.378f));
    motion->runtime.constraintStepLimit[2] = 2048.0f / 10430.378f;
    group->savedProjectionAngle = static_cast<std::uint16_t>(AtanToFixedAngle(std::fabs(motion->runtime.projectionCotangentNear)));
    motion->runtime.projectionCotangentNear = cos_fixed16(10922) / sin_fixed16(10922);
    if ((g_pFightStage->battleFlags & 2U) == 0)
        cpamSetFlag(character, 1);
    group->savedExtraMotionFlag = mtwkSetExtraFlag(motion, 0x37, 1);
    mtwkSetExtraFlag(motion, 0x37, 0);
    for (std::uint8_t i = 0; i < 7; ++i) {
        group->savedMotionCallbacks[i] = GetMotionTargetCallback(motion, fightMotionSelectors[i]);
        group->savedMotionParameters[i] = GetMotionTargetParameter(motion, fightMotionSelectors[i]);
    }
}

// @verified - p
void vfRestoreEnemySlotMotion(FCMG_t *group) {
    const auto flags = group->savedControlFlags;
    Character *const character = group->character;
    MotionWork *const motion = group->motion;
    if ((flags & 4U) != 0) {
        SetFlagForToken(character, SHEN_NAME(CCM_), 1);
        if ((g_pFightStage->battleFlags & 0x8000U) != 0) {
            if (CharacterControlModel *const control = FindCharacterControlModel(character))
                control->flags &= ~3U;
        }
    }
    if ((flags & 8U) == 0)
        EV_mtwkSetEnabled(motion, false);
    if ((flags & 0x20U) == 0)
        mtwkSetExtraFlag(motion, 6, 2);
    cpamSetFlag(character, (flags & 0x80U) != 0);
    SetMotionSpeed(motion, group->savedMotionScale, MotionRegion::LowerBody);
    if ((group->flags & 2U) != 0)
        SetMotionWorkCulled(motion, true);
    mtwkSetRequestedRootMode(motion, group->savedRootMode);
    motion->runtime.constraintStepLimit[2] = static_cast<float>(group->savedConstraintAngle) / 10430.378f;
    const auto projection = static_cast<std::uint16_t>(group->savedProjectionAngle);
    motion->runtime.projectionCotangentNear = cos_fixed16(projection) / sin_fixed16(static_cast<std::int16_t>(projection));
    mtwkSetExtraFlag(motion, 0x37, group->savedExtraMotionFlag != 0 ? 0 : 2);
    for (std::uint8_t i = 0; i < 7; ++i) {
        AssignMotionTargetCallback(motion, group->savedMotionCallbacks[i], fightMotionSelectors[i]);
        AssignMotionTargetParameter(motion, group->savedMotionParameters[i], fightMotionSelectors[i]);
    }
    mtwkSetExtraFlag(motion, 1, 2);
}


// @verified - p
void vfSelectDefaultReactionAndGuardAnims(VFActor *actor) {
    static constexpr std::uint16_t indices[4][3]{{294, 296, 298}, {300, 302, 304}, {322, 296, 298}, {0, 16, 20}};
    std::uint8_t row = 3;
    if (actor->fightStyle != 0)
        row = actor->reactionRank == 0 ? 0 : 2;
    else if (static_cast<std::int8_t>(actor->guardLevel - actor->actor->guardLevel) > 0) {
        if (static_cast<double>(actor->initialHp) * 0.5 <= actor->hp)
            row = actor->contactDistances[actor->actionIndex] >= 4.0f ? 1 : 0;
        else
            row = 2;
    }
    const auto *const ids = reinterpret_cast<const std::uint8_t *>(actor->motionParameters[4]);
    for (std::uint8_t i = 0; i < 3; ++i) {
        const std::uint16_t offset = indices[row][i];
        actor->defaultAnimations[i] = static_cast<std::uint16_t>((static_cast<std::uint16_t>(ids[offset]) << 8) | ids[offset + 1]);
    }
    actor->stateFlags &= ~VFSF_Unknown80000;
}

// @verified - p
void vfUpdateDefaultReactionAnim(FCMG_t *group, std::uint16_t animation) {
    SetMotionFrameTime(group->motion, 5, MotionRegion::LowerBody);
    mtwkRequestMotion(group->motion, animation, MotionRegion::LowerBody);
    if (group->fighter && group->state == 2)
        SetMotionRegionFlag(group->motion, 69, (group->fighter->moveFlags & 0x40U) != 0 ? 0 : 2, MotionRegion::LowerBody);
    group->flags |= 1U;
}



// @verified - p
std::uint8_t vfSelectOpponentSlotByPriority(VFMA_t *game, std::uint8_t minimumPriority) {
    float distance = 0.1f;
    std::uint8_t selected = 99;
    for (std::uint8_t i = 0; i < 6; ++i) {
        const VFActor *const actor = game->actors[i];
        if (!game->characters[i] || i == game->playerSlot)
            continue;
        if (game->slotTeams[i] == game->slotTeams[game->playerSlot] || static_cast<std::int32_t>(game->enemySlots[i]->runningTicks) < 15 ||
            (actor->moveFlags & VFMF_ForceDefeat) != 0 || ((actor->motionFlags | actor->hitStateFlags) & 0xdd110U) != 0 ||
            ((actor->secondaryMotionFlags | actor->activeMotionFlags) & 0x900c80U) != 0)
            continue;
        if (actor->selectionPriority > minimumPriority) {
            selected = i;
            minimumPriority = actor->selectionPriority;
        } else if (actor->selectionPriority == minimumPriority && actor->contactDistances[game->playerSlot] > distance) {
            distance = actor->contactDistances[game->playerSlot];
            selected = i;
            minimumPriority = actor->selectionPriority;
        }
    }
    return selected;
}



// @verified - p
void vfUpdateEnemySlotFormationAndYaw(VFMA_t *) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || !group.group || group.state != 3 || (group.collisionFlags & 1U) == 0)
            continue;
        if (SetMotionRegionFlag(group.motion, 65, 1, MotionRegion::LowerBody) == 0)
            continue;
        const MotnSequence *const sequence = FindLoadedMotionSequence(GetMotionAnimationId(group.motion, MotionRegion::LowerBody));
        if (sequence && sequence->allowsFrameOverflow)
            continue;
        const std::uint16_t animation = (group.collisionFlags & 0x20U) != 0 ? 453 :
            (group.collisionFlags & 0x40U) != 0                             ? 452 :
            (group.collisionFlags & 2U) != 0                                ? 101 :
                                                                              104;
        vfUpdateDefaultReactionAnim(&group, animation);
        SetMotionRegionFlag(group.motion, 69, (group.collisionFlags & 0x80U) != 0 ? 0 : 2, MotionRegion::LowerBody);
        if (group.savedFacingAngle != 0) {
            const auto yaw = GetAllAngYForNode(&group.character->entity.node);
            shenDecrementRotYAllNodes(&group.character->entity.node, static_cast<std::int16_t>(yaw + group.savedFacingAngle));
            group.savedFacingAngle = 0;
        }
    }
}

// @verified - p
void vfRemoveCharaFromGroup(VFMA_t *game, FCMG_t *group) {
    if (group->character)
        group->character->flags &= ~CF_Unknown40;
    game->characters[group->actorIndex] = nullptr;
    group->actorIndex = 0;
}

// DEAD is a name-valued pointer sentinel, never a Character allocation.
// @verified - p
bool ReactToDead(VFMA_t *game, FCMG_t *slot) {
    FRMG_t *formation = nullptr;
    for (std::uint8_t i = 0; i < g_pFightStage->groupCount; ++i) {
        FRMG_t &candidate = g_pFightStage->groups[i];
        if (candidate.character == reinterpret_cast<Character *>(static_cast<std::uintptr_t>(SHEN_NAME(DEAD))) && candidate.state == 0 &&
            candidate.team == slot->counterGroup) {
            formation = &candidate;
            break;
        }
    }
    if (!formation)
        return false;
    motRemoveCallback(slot->motion);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    mtwkSetExtraFlag(slot->motion, 3, 2);
    formation->character = slot->character;
    std::uint16_t yaw = formation->spawnWords[1];
    if (game->playerSlot != -1 && (formation->activationState & 0xffU) == 0) {
        VFActor *const player = game->actors[game->playerSlot];
        yaw = FixedAtan2(formation->spawnPosition.x - player->position.x, formation->spawnPosition.z - player->position.z);
    }
    shenApplyTranslationToAll(&formation->character->entity.node, formation->spawnPosition);
    slot->position = formation->spawnPosition;
    shenDecrementRotYAllNodes(&formation->character->entity.node, static_cast<std::int16_t>(yaw));
    vfUpdateDefaultReactionAnim(slot, formation->spawnWords[0]);
    SetMotionFrameTime(slot->motion, 1, MotionRegion::LowerBody);

    slot->quadrant = -1;
    slot->group = formation;
    formation->slot = slot;
    --g_pFightStage->secondaryGroupCounters[slot->counterGroup];
    slot->setPosition = 0;
    return true;
}

// @verified - p
bool vfEnemySlotResetAndRestartCycle(VFMA_t *game, FCMG_t *group) {
    const auto team = group->counterGroup;
    switch (group->state) {
    case 0:
    case 4:
        if (!BindAllocatedEnemySlot(group))
            return false;
        break;
    case 1:
        return false;
    case 2:
        if (static_cast<std::int32_t>(group->runningTicks) < 30)
            return false;
        if (group->group)
            group->group->totalHp = static_cast<std::uint16_t>(static_cast<std::int32_t>(group->fighter->hp));
        group->position = GetCharacterWorldPosition(group->character);
        group->accumulatedYaw = static_cast<std::uint16_t>(GetAllAngYForNode(&group->character->entity.node));
        vfSelectDefaultReactionAndGuardAnims(group->fighter);
        vfUpdateDefaultReactionAnim(group, group->fighter->defaultAnimations[0]);
        motRemoveCallback(group->motion);
        vfRemoveCharaFromGroup(game, group);
        group->group->state = 0;
        g_activeFightData[group->fightEntryIndex].status = 0;
        --g_pFightStage->activeFightersByTeam[team];
        break;
    case 3:
        if (!ReactToDead(game, group))
            return false;
        --g_pFightStage->groupCounters[team];
        group->fadeAlpha = -1.0f;
        modelSetCharaAlpha(group->character, 0.0f);
        break;
    default:
        break;
    }
    group->quadrant = -1;
    group->state = 1;
    group->runningTicks = 0.0f;
    ++g_pFightStage->readyEnemySlots[team];
    return true;
}

// Installs the real cooldown target producer.
// @verified - p
void vfCaptureGroundForCooldown(VFMA_t *game, VFActor *actor) {
    FCMG_t *const group = game->enemySlots[actor->groupIndex];
    float *const heights = actor->groundHeights;
    if (game->type != 8 && (g_pFightStage->battleFlags & 0x800U) == 0) {
        if (static_cast<double>(heights[3]) - 0.15 > heights[2])
            heights[2] = heights[3] - 0.15000001f;
        const float lower = heights[2] - 0.1f;
        if (lower > heights[3])
            heights[3] = lower;
        if (lower > heights[5])
            heights[5] = lower;
        if (lower > heights[6])
            heights[6] = lower;
        const float foot = heights[2] - 0.15000001f;
        if (foot > heights[0])
            heights[0] = foot;
        if (foot > heights[1])
            heights[1] = foot;
    }
    if (game->type == 8) {
        if ((actor->groundFlags & 0x400U) != 0)
            heights[0] = heights[2];
        if ((actor->groundFlags & 0x2000U) != 0)
            heights[1] = heights[2];
    }
    std::uint32_t mask = 0;
    for (std::uint8_t i = 0; i < 7; ++i) {
        actor->previousGroundSamples[i] = -10000.0f;
        group->stackLayerOffsets[i] = heights[i];
        group->groundCorrectionEnabled[i] = static_cast<double>(std::fabs(group->position.y - heights[i])) > 0.001;
        group->stackLayerMasks[i] = actor->nearbyGroundActors[i];
        mask |= actor->nearbyGroundActors[i];
    }
    group->stackReferenceMask = mask;
    for (const auto selector : fightMotionSelectors) {
        AssignMotionTargetCallback(actor->motion, AdjustCooldownGroundTarget, selector);
        AssignMotionTargetParameter(actor->motion, group, selector);
    }
}

// @verified - p
bool vfTransitionEnemySlotToCooldown(VFMA_t *game, FCMG_t *group) {
    const auto state = group->state;
    const auto team = group->counterGroup;
    group->position = GetCharacterWorldPosition(group->character);
    motRemoveCallback(group->motion);
    switch (state) {
    case 1:
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2: {
        VFActor *const actor = group->fighter;
        vfCaptureGroundForCooldown(game, actor);
        group->savedFacingAngle = actor->facingAngleDelta;
        group->quadrant = actor->cooldownQuadrant;
        for (float &sample : group->groundSamples)
            sample = -10000.0f;
        std::uint32_t flags = 0;
        if ((actor->hitStateFlags & VFHSF_SpecialGuardOrParry) != 0) {
            flags = 1;
            if ((actor->motionFlags & VFMOF_Unknown4000000) != 0)
                flags = (actor->motionFlags & VFMOF_ContactCategoryB) != 0 ? 65U : 33U;
            else if ((actor->hitStateFlags & VFHSF_ReversedFacing) != 0)
                flags = 3;
        }
        if ((actor->moveFlags & VFMF_MirrorReactionAngle) != 0)
            flags |= 0x80U;
        if ((actor->hitStateFlags & VFHSF_AlignedToGround) != 0)
            flags |= 0x200U;
        if ((group->group->selectFlags & 0xa0U) != 0)
            flags |= 0x100U;
        group->collisionFlags = flags;
        vfRemoveCharaFromGroup(game, group);
        --g_pFightStage->activeFightersByTeam[team];
        break;
    }
    case 3:
        return false;
    default:
        break;
    }
    group->state = 3;
    group->runningTicks = 0.0f;
    group->group->state = 2;
    g_activeFightData[group->fightEntryIndex].status = 2;
    group->cooldownDelay = static_cast<std::uint8_t>(g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1]);
    group->fadeAlpha = 1.0f;
    mtwkSetExtraFlag(group->motion, 0, 2);
    ++g_pFightStage->groupCounters[team];
    return true;
}

// @verified - p
float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors) {
    *nearbyActors = 0;
    float offset = 0.0f;
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        const FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || (group.collisionFlags & 1U) == 0)
            continue;
        const auto yaw = static_cast<std::uint16_t>(GetAllAngYForNode(&group.character->entity.node));
        const float x = sin_fixed16(static_cast<std::int16_t>(yaw)) * 0.40000001f;
        const float z = cos_fixed16(yaw) * 0.40000001f;
        const float plusX = position->x - (group.position.x + x), plusZ = position->z - (group.position.z + z);
        const float minusX = position->x - (group.position.x - x), minusZ = position->z - (group.position.z - z);
        if (plusX * plusX + plusZ * plusZ < 0.25f || minusX * minusX + minusZ * minusZ < 0.25f) {
            offset += 0.15000001f;
            *nearbyActors |= 1U << i;
        }
    }
    return offset;
}

// @verified - p
void vfAssignWaveSpawnFromGenerator(FRMG_t *formation, float x, float z) {
    std::uint8_t primary = 99, outward = 99;
    float primaryDistance = 999.0f, outwardDistance = 999.0f;
    const float centerX = x - g_pFightStage->battleCenter.x;
    const float centerZ = z - g_pFightStage->battleCenter.z;
    const float distanceFromCenter = std::sqrt(centerZ * centerZ + centerX * centerX);
    for (std::uint8_t i = 0; i < g_FGEN_count; ++i) {
        const FGEN_t &generator = g_pFightStage->generators[i];
        if (generator.enabled != 1 || generator.other != 0)
            continue;
        const float dx = x - generator.position.x, dz = z - generator.position.z;
        const float distance = std::sqrt(dz * dz + dx * dx);
        if (distance <= 8.0f)
            continue;
        const float gx = generator.position.x - g_pFightStage->battleCenter.x;
        const float gz = generator.position.z - g_pFightStage->battleCenter.z;
        const bool isOutward = g_pFightStage->useCenterBias != 0 && std::sqrt(gx * gx + gz * gz) > distanceFromCenter;
        float &bestDistance = isOutward ? outwardDistance : primaryDistance;
        const float error = std::fabs(distance - 8.0f);
        if (bestDistance <= error)
            continue;
        bool available = true;
        for (std::uint8_t enemy = 0; enemy < g_pFightStage->enemyCount; ++enemy) {
            Character *const character = g_pFightStage->enemySlots[enemy].character;
            if (!character)
                continue;
            const Vector3 position = GetCharacterWorldPosition(character);
            const float ex = generator.position.x - position.x, ez = generator.position.z - position.z;
            if (ex * ex + ez * ez < 4.0f) {
                available = false;
                break;
            }
        }
        if (!available)
            continue;
        bestDistance = error;
        (isOutward ? outward : primary) = i;
    }
    const std::uint8_t selected = primary != 99 ? primary : outward;
    if (selected != 99)
        formation->spawnPosition = g_pFightStage->generators[selected].position;
}

// @verified - p
bool vfTransitionEnemySlotToInactive(VFMA_t *game, FCMG_t *group) {
    const auto team = group->counterGroup;
    switch (group->state) {
    case 0:
    case 4:
        return false;
    case 1:
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2:
        vfRemoveCharaFromGroup(game, group);
        --g_pFightStage->activeFightersByTeam[team];
        break;
    case 3:
        --g_pFightStage->groupCounters[team];
        break;
    default:
        break;
    }
    vfRestoreEnemySlotMotion(group);
    mtwkSetExtraFlag(group->motion, 0, 0);
    mtwkSetExtraFlag(group->motion, 3, 2);
    group->state = 4;
    group->runningTicks = 0.0f;
    ++g_pFightStage->groupCounters[team + 2];
    return true;
}

// @verified - p
void vfClearEnemySlotStackRefs(std::uint8_t slotIndex) {
    const std::uint32_t mask = 1U << slotIndex;
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || (group.stackReferenceMask & mask) == 0)
            continue;
        for (std::uint8_t layer = 0; layer < 7; ++layer) {
            if ((group.stackLayerMasks[layer] & mask) == 0)
                continue;
            group.stackLayerMasks[layer] &= ~mask;
            group.stackLayerOffsets[layer] -= 0.15000001f;
            if (layer == 2) {
                group.position.y -= 0.15000001f;
                shenApplyTranslationToAll(&group.character->entity.node, group.position);
            }
        }
        group.stackReferenceMask &= ~mask;
    }
}

// @verified - p
void vfUpdateEnemySlotCooldowns(VFMA_t *game) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3)
            continue;
        if (group.cooldownDelay != 0) {
            --group.cooldownDelay;
            continue;
        }
        if ((group.collisionFlags & 0x100U) != 0) {
            modelSetCharaAlpha(group.character, 1.0f);
            cpamSetFlag(group.character, 1);
            vfEnemySlotResetAndRestartCycle(game, &group);
        } else {
            cpamSetFlag(group.character, 1);
            vfTransitionEnemySlotToInactive(game, &group);
        }
        vfClearEnemySlotStackRefs(i);
    }
}

// Only the first eligible cooldown slot is faded per call.
// @verified - p
void vfUpdateRenderOpacityForAll(VFMA_t *game) {
    if (g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1] != game->maximumActiveEnemies)
        return;
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || group.cooldownDelay != 0)
            continue;
        cpamSetFlag(group.character, 0);
        if (g_pFightStage->secondaryGroupCounters[group.counterGroup] != 0 && (group.collisionFlags & 0x100U) != 0) {
            group.fadeAlpha -= 0.029999999f;
            if (group.fadeAlpha > 0.0f)
                modelSetCharaAlpha(group.character, group.fadeAlpha);
            else {
                group.fadeAlpha = 0.0f;
                modelSetCharaAlpha(group.character, 0.0f);
                vfUpdateEnemySlotCooldowns(game);
            }
        }
        return;
    }
}

// @verified - p
void vfTransitionAllEnemySlotsToInactive(VFMA_t *game) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state == 4)
            continue;
        figpAssignModeForCharacter(group.character, 0);
        modelSetCharaAlpha(group.character, 1.0f);
        if (group.state != 0)
            vfTransitionEnemySlotToInactive(game, &group);
    }
}



// @verified - p
void vfUpdateEnemySlotSpawnFade(VFMA_t *) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || !group.group || group.state != 1)
            continue;
        group.position = GetCharacterWorldPosition(group.character);
        if (group.fadeAlpha < 1.0f) {
            float alpha = group.fadeAlpha + 0.029999999f;
            if (alpha > 1.0f) {
                cpamSetFlag(group.character, 1);
                alpha = 1.0f;
            }
            group.fadeAlpha = alpha;
            modelSetCharaAlpha(group.character, std::fmax(0.0f, alpha));
        }
        group.accumulatedYaw = static_cast<std::uint16_t>(GetAllAngYForNode(&group.character->entity.node));
    }
}

// @verified - p
void vfUpdatePairingsAndDistances(VFMA_t *game) {
    float distances[6][6];
    std::int16_t angles[6][6];
    std::int8_t targets[6]{-1, -1, -1, -1, -1, -1};
    for (auto &row : distances)
        for (float &value : row)
            value = 9999.0f;
    const std::int8_t player = game->playerSlot;
    std::int8_t opponent = -1;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        const auto team = game->slotTeams[i];

        if (team == 1 && (game->groups[i]->selectFlags & 0xa0U) == 0)
            opponent = static_cast<std::int8_t>(i);
        for (std::uint8_t j = 0; j < 6; ++j) {
            if (j == i || !game->characters[j] || game->slotTeams[j] == team)
                continue;
            VFActor *const other = game->actors[j];
            const float x = self->position.x - other->position.x;
            const float z = self->position.z - other->position.z;
            const float distance = std::sqrt(x * x + z * z);
            distances[i][j] = distances[j][i] = distance;
            angles[i][j] = static_cast<std::int16_t>(self->temporaryFacingAngle - FixedAtan2(x, z));
            angles[j][i] = static_cast<std::int16_t>(other->temporaryFacingAngle - FixedAtan2(-x, -z));
        }
    }
    game->opponentSlot = static_cast<std::uint8_t>(opponent);

    std::int8_t lastSelected = -1;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        bool foundFacing = false;
        float nearest = 9999.0f;
        lastSelected = -1;
        const auto yaw = static_cast<std::int16_t>(i == player ? game->otherFacingYaw - self->facingYaw : 0);
        for (std::uint8_t j = 0; j < 6; ++j) {
            if (!game->characters[j] || game->slotTeams[j] == game->slotTeams[i])
                continue;
            float distance = distances[i][j];
            if ((game->actors[j]->moveFlags & 0x40000U) != 0)
                distance = 999.0f;
            const bool facing = static_cast<std::uint16_t>(yaw + angles[i][j] + 9100) <= 0x4718U && distance < 3.0f;
            if (facing ? (!foundFacing || nearest > distance) : (!foundFacing && nearest > distance)) {
                if (facing)
                    foundFacing = true;
                nearest = distance;
                lastSelected = static_cast<std::int8_t>(j);
            }
        }
        if (lastSelected != -1 &&
            (self->pairedSlot == -1 ||
                ((self->pairedSlot != lastSelected || self->pairedCharacter != game->characters[lastSelected]) && self->deferredReaction == 0 &&
                    ((self->motionFlags | self->hitStateFlags) & 0xc0000U) == 0 && ((self->secondaryMotionFlags | self->activeMotionFlags) & 0x80c00U) == 0))) {
            self->pairedSlot = lastSelected;
            self->pairedActor = game->actors[lastSelected];
            Character *const previous = self->pairedCharacter;
            self->pairedCharacter = game->characters[lastSelected];
            if (previous != self->pairedCharacter)
                self->reactionFlags = 0;
        }
        targets[i] = self->pairedSlot;
    }
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        if (self->targetingCharacter && self->targetingCharacter == game->characters[self->actionIndex]) {
            const auto otherFlags = self->actor->moveFlags;
            if (((otherFlags & 0x40000U) == 0 && (self->motionFlags & VFMOF_Unknown100) != 0 && (self->actor->hitStateFlags & VFHSF_GuardState) == 0 &&
                    (self->moveFlags & VFMF_Airborne) == 0) ||
                ((self->motionFlags & VFMOF_DelayGuardResolution) != 0 && (otherFlags & 0x20U) == 0)) {
                targets[i] = self->actionIndex;
                continue;
            }
        }
        if ((self->activeMotionFlags & VFSMF_Unknown2000000) != 0 && self->targetingCharacter && self->actor &&
            (self->actor->moveFlags & VFMF_ForceDefeat) == 0) {
            targets[i] = self->actionIndex;
            continue;
        }
        if ((g_pFightStage->battleFlags & 2U) != 0 || !game->characters[player] || i == player)
            continue;
        if (i == opponent) {
            targets[i] = player;
            continue;
        }
        std::uint8_t mode = 2;
        if (game->slotTeams[i] == 1) {
            if (opponent == -1)
                mode = 0;
            else {
                if (lastSelected != player || distances[player][opponent] >= 4.0f)
                    continue;
                mode = 1;
            }
        }
        std::int8_t nearestEnemy = -1, nearestAlly = -1, playerEnemy = -1;
        float enemyDistance = 9999.0f, allyDistance = 9999.0f, playerDistance = 9999.0f;
        const auto pairedPlayerSlot = static_cast<std::uint8_t>(game->actors[player]->pairedSlot);
        for (std::uint8_t j = 0; j < 6; ++j) {
            if (!game->characters[j] || j == player)
                continue;
            VFActor *const other = game->actors[j];
            if ((other->moveFlags & VFMF_ForceDefeat) != 0)
                continue;
            const float distance = distances[i][j];
            const float fromPlayer = distances[player][j];
            if (game->slotTeams[j] == 1 && playerDistance > fromPlayer && (other->motionFlags & VFMOF_Unknown80000) == 0 &&
                (other->secondaryMotionFlags & VFSMF_Unknown800) == 0) {
                playerEnemy = static_cast<std::int8_t>(j);
                playerDistance = fromPlayer;
            }
            if (game->slotTeams[j] == 0 && allyDistance > distance) {
                nearestAlly = static_cast<std::int8_t>(j);
                allyDistance = distance;
            }
            if (game->slotTeams[j] == 1 && j != pairedPlayerSlot && enemyDistance > distance) {
                nearestEnemy = static_cast<std::int8_t>(j);
                enemyDistance = distance;
            }
        }
        if (mode == 0) {
            if (playerEnemy != -1 && nearestAlly != -1)
                targets[i] = i == playerEnemy && distances[i][nearestAlly] > 1.5f ? player : nearestAlly;
        } else if (mode == 1) {
            if (nearestAlly != -1)
                targets[i] = nearestAlly;
        } else if (nearestEnemy != -1 && distances[i][targets[i]] > 1.5f) {
            targets[i] = nearestEnemy;
        }
    }
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i] || targets[i] == -1)
            continue;
        const std::int8_t target = targets[i];
        VFActor *const self = game->actors[i];
        Character *const character = game->characters[target];
        if (character && (self->actionIndex != target || self->targetingCharacter != character)) {
            Character *const previous = self->targetingCharacter;
            self->actionIndex = target;
            self->actor = game->actors[target];
            self->targetingCharacter = character;
            if (previous != character)
                self->targetCache = -1;
        }
    }
}

// @verified - p
VFMA_t *vfmaGetTaskParam() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    return task ? static_cast<VFMA_t *>(HLib::GetTaskParameter(task)) : nullptr;
}


// @verified - p
VFMA_t *vfmaSetGaugeSuppressed(std::int32_t suppressed) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (game)
        game->parameter574 = suppressed;
    return game;
}

// @verified - p
std::uint64_t GetFightPlayerGuardEffectTimerSeconds() {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0xffffffffU;
    const std::int32_t frames = _mm_cvttss_si32(_mm_set_ss(game->guardTimer));
    return frames == -1 ? 0xffffffffU : static_cast<std::uint32_t>(frames / 30);
}

// @verified - p
std::uint32_t GetFightActorFlags(Character *character) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    for (unsigned index = 0; index < 6; ++index) {
        if (game->characters[index] == character)
            return character ? game->actors[index]->formationRuntimeFlags : 0;
    }
    return 0;
}


std::uint64_t GetFightCounterHighWord() {
    const VFMA_t *const game = vfmaGetTaskParam();
    return game ? game->counter32c >> 16 : 0xffffffffU;
}


std::uint64_t GetBattleDialogueCompletionCount(std::uint32_t id) {
    const VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return 0xffffffffU;
    for (const VCDS_t *record = game->cameraDialogue->records; record; record = record->next)
        if (record->id == id)
            return id ? record->completions : 0xffffffffU;
    return 0xffffffffU;
}


std::uint32_t SetFightFlag400000(std::int32_t enabled) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    const std::uint32_t previous = game->gameFlags & 0x400000U;
    if (enabled) {
        game->mode |= 0x200U;
        game->gameFlags |= 0x400000U;
    } else
        game->gameFlags &= ~0x400000U;
    return previous;
}

// @verified - p
void vfInitStageObjects() {

    for (std::size_t i = 0; i < 4; ++i)
        g_fightStageObjectDefinitions[i] = nullptr;
    std::memset(g_fightStageObjects, 0, 0x78);
    g_fightStageObjectMaximumDistance = 16.0f;
    g_fightStageObjectRoot = nullptr;
}

// @verified - p
void vfFreeStageObjects() {
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next) {
        if (object->object.state != 0)
            continue;
        CharacterControlModel *const control = FindCharacterControlModel(object->object.controlCharacter);
        if (!control)
            continue;
        const std::uint32_t saved = object->object.savedControlFlags;
        if ((saved & 2U) == 0)
            control->flags &= ~2U;

        if ((saved & 4U) != 0)
            control->flags = 4U;
        if ((saved & 8U) != 0)
            control->flags = 8U;
        if ((saved & 1U) != 0)
            control->flags = 1U;
    }
    for (FightStageObjectDefinition *&definition : g_fightStageObjectDefinitions) {
        if (!definition)
            continue;
        HLib::FreeTaskData(definition);
        definition = nullptr;
    }
    for (VOBJ_t *&object : g_fightStageObjects) {
        if (!object)
            continue;
        if (object->motionTracks)
            HLib::FreeTaskData(object->motionTracks);
        HLib::FreeTaskData(object);
        object = nullptr;
    }
    g_fightStageObjectRoot = nullptr;
}

// @verified - p
FightStageObjectDefinition **AllocateVOBJ(std::int32_t slot, std::int8_t index) {
    FightStageObjectDefinition *const definition =
        static_cast<FightStageObjectDefinition *>(HLib::AllocateTaskData(sizeof(FightStageObjectDefinition), TASK_VOBJ));
    if (!definition)
        return nullptr;
    std::memset(definition, 0, sizeof(*definition));
    g_fightStageObjectDefinitions[slot] = definition;
    definition->index = index;
    return g_fightStageObjectDefinitions;
}

namespace {
// Default offset words are immediately followed by node indices; definition index 8 reads those adjacent words.
constexpr std::uint32_t stageObjectDefaults[]{
    0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0x3e4ccccdU,0,
    152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,
    172,173,174,175,176,177,178,179,180,181,
    182,183,184,185,186,187,188,189,190,191,
    192,193,194,195,196,197,198,199,200,201,0,0};
}

// DYNA keys and node indices remain resource-owned.
const std::byte *vfStageDefinitionSetMotion(std::int32_t slot, std::uint32_t name, std::int32_t tracks, const std::uint32_t *nodes, std::int32_t step,
    std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.flags |= 1U;
    definition.motionTrackCount = tracks;
    definition.motionNodes = reinterpret_cast<const ObjectMotionNode *>(nodes ? nodes : stageObjectDefaults + 24);
    definition.motionStep = step;
    definition.motionFirstFrame = firstFrame;
    definition.motionLastFrame = lastFrame;
    definition.breakSound = sound;
    std::uint8_t resourceName[4];
    std::memcpy(resourceName, &name, sizeof(name));
    definition.motionKeys = reinterpret_cast<const ObjectMotionRecord *>(reinterpret_cast<std::uintptr_t>(dynaRead(resourceName)) + 8U);
    return reinterpret_cast<const std::byte *>(definition.motionKeys);
}



void vfStageDefinitionAddHitbox(std::int32_t slot, std::uint32_t flags, std::uint16_t kindAndState, float x, float y, float z, float radius) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    const std::uint16_t kind = kindAndState & 0xfU;
    if (kind == 3) {
        definition.flags &= ~6U;
        definition.hitboxCount = 0;
        return;
    }
    const std::int32_t count = definition.hitboxCount;
    definition.flags |= flags;
    FightStageObjectHitbox &record = definition.hitboxes[count];
    record.kind = static_cast<std::int16_t>(kind);
    if (kind == 1) {
        record.state = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(kindAndState & 0xfff0U));
        record.position = {.x = x, .y = y, .z = z};
        record.radius = radius;
    }
    definition.hitboxCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(count) + 1U);
    if (kind == 1) {
        const float diameter = (radius + radius) + 1.0f;
        const float distance = diameter * diameter;
        if (distance > g_fightStageObjectMaximumDistance)
            g_fightStageObjectMaximumDistance = distance;
    }
}

// The raw result is the authored offset Z's DWORD.
std::uint64_t vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound, std::uint32_t launchSound, std::uint32_t flags,
    std::int32_t launchYaw, std::int32_t rotationSpeed, float coefficient, float groundOffset) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.flags |= flags;
    definition.launchYaw = launchYaw;
    definition.groundSound = groundSound;
    definition.launchSound = launchSound;
    definition.rotationSpeed = rotationSpeed;

    definition.responseCoefficient = 0.5f - coefficient * 0.0166f < 0.01f ? 29.51807403564453125f : coefficient;
    definition.groundOffset = groundOffset;
    std::memcpy(&definition.positionOffset, stageObjectDefaults + 3 * definition.index, sizeof(definition.positionOffset));
    std::uint32_t result;
    std::memcpy(&result, &definition.positionOffset.z, sizeof(result));
    return result;
}

std::uint64_t vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::uint32_t rotationZ) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.attachmentPosition = {.x = x, .y = y, .z = z};
    definition.attachmentBone = bone;
    definition.attachmentRotation = {.x = rotationX, .y = rotationY, .z = std::bit_cast<std::int32_t>(rotationZ)};
    return rotationZ;
}

// @verified - p
std::uint32_t ToggleVOBJArrayIndex(
    std::int32_t slot, std::int8_t guardStrength, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.guardStrength = guardStrength;
    definition.guardState = guardState;
    definition.hitParameter = hitParameter;
    definition.hitSound = hitSound;
    return hitSound;
}
// @verified - p
std::int64_t vfStageDefinitionAddContactMask(std::int32_t slot, std::uint32_t flags) {
    g_fightStageObjectDefinitions[slot]->flags |= flags;
    return slot;
}

// @verified - p
FightStageObjectDefinition *vfAllocateVOBJ(std::int32_t slot, Character *character, std::int32_t definition) {
    VOBJ_t *const object = static_cast<VOBJ_t *>(HLib::AllocateTaskData(sizeof(VOBJ_t), TASK_VOBJ));
    if (!object)
        return nullptr;
    std::memset(object, 0, sizeof(*object));
    g_fightStageObjects[slot] = object;
    object->next = g_fightStageObjectRoot;
    g_fightStageObjectRoot = object;
    object->object.index = static_cast<std::int8_t>(slot);
    CharacterControlModel *const control = FindCharacterControlModel(character);
    if (control) {
        object->object.savedControlFlags = control->flags & 0xfU;
        control->flags = 2U;
        control->flags &= ~0xdU;
    }
    object->object.definition = g_fightStageObjectDefinitions[definition];
    object->object.model = character;
    object->object.controlCharacter = character;
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 1);
    object->object.contactPosition = GetCharacterWorldPosition(character);
    object->object.rotation = GetCharacterWorldRotation(character);
    const Vector3 &offset = object->object.definition->positionOffset;
    const Vector3 &position = object->object.contactPosition;
    object->object.position = {offset.x + position.x, offset.y + position.y, offset.z + position.z};
    return object->object.definition;
}

// @verified - p
std::int32_t vfStageObjectAssignMotFlags(std::int32_t slot, std::uint32_t flags) {
    g_fightStageObjects[slot]->object.motionFlags |= flags;
    return slot;
}

// The hidden broken model is borrowed, not allocated here.
std::uint8_t vfStageObjectSetBrokenModel(std::int32_t slot, Character *character, std::uint32_t flags) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.motionFlags |= flags;
    if (CharacterControlModel *const control = FindCharacterControlModel(character))
        control->flags = 2U;
    if (CharacterControlModel *const control = FindCharacterControlModel(character))
        control->flags &= ~0xdU;
    object.brokenModel = character;
    return EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
}

// Initialize the real live root in insertion order.
void vfInitializeStageObjectHitboxes() {
    Matrix4x4_Push();
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next)
        vfTransformStageObjectHitboxes(object);
    Matrix4x4::Pop(1);
}

// @verified - p
void vfStageObjectAssignPosXYZ(std::int32_t slot, float x, float y, float z) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.contactPosition = {x, y, z};
    shenApplyTranslationToAll(GetCharacterNode(object.model), object.contactPosition);
}

// @verified - p
void vfStageObjectAssignRotXYZ(std::int32_t slot, std::uint16_t x, std::uint16_t y, std::uint16_t z) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.rotation = {x, y, z};
    SetCharacterWorldRotation(object.model, object.rotation);
}

// @verified - p
VOBJ_t *vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z) {
    VOBJ_t *const object = g_fightStageObjects[slot];
    object->object.velocity = {x, y, z};
    return object;
}

// The authored VOBJ owns the reset; attachment borrows its character.
void vfStageObjectReset(std::int32_t index, Character *character) {
    FightStageObjectRuntime &object = g_fightStageObjects[index]->object;
    object.velocity = {};
    object.rotationImpulse = {};
    object.reactionFlags &= 0xfffffd84U;
    if (!character)
        return;
    if (object.definition->flags & 0x40U) {
        const Vector3 position = GetCharacterWorldPosition(character);
        const std::int32_t angle =
            -(object.rotation.y + static_cast<std::int16_t>(FixedAtan2(position.z - object.contactPosition.z, position.x - object.contactPosition.x)));
        if (!(angle & 0x8000))
            object.reactionFlags |= 0x200;
        std::memcpy(&object.reactionRotation.x, &object.rotation.x, sizeof(object.rotation.x));
    }
    object.reactionFlags |= 1;
    object.reactingCharacter = character;
}

// Preserve the authored response coefficient at definition +144.
void vfLaunchStageObject(std::int32_t index, std::uint16_t angle) {
    FightStageObjectRuntime &object = g_fightStageObjects[index]->object;
    vfStageObjectReset(index, nullptr);
    const float coefficient = object.definition->responseCoefficient;
    const float magnitude = 0.5f - coefficient * 0.0166f;
    object.velocity = {-sin_fixed16(static_cast<std::int16_t>(angle)) * magnitude, 0.2f, -cos_fixed16(angle) * magnitude};
    if (object.definition->flags & 0x380U)
        object.reactionFlags |= 2;
}


void vfToggleAttachedStageObjectDisplay(VFActor *actor) {
    for (VOBJ_t *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        FightStageObjectRuntime &object = entry->object;
        if ((object.reactionFlags & 1) && object.reactingCharacter == actor->character)
            EV_SetCharDisplay(object.model, CVM_DisplayEnabled, !IsOfThisVisibilityType(object.model, 3));
    }
}

void vfSetAttachedStageObjectEffect(VFActor *actor, std::int8_t effect, float amount, std::int16_t frames) {
    if (effect < 1 || effect > 3)
        return;
    for (VOBJ_t *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        if (!(entry->object.reactionFlags & 1) || entry->object.reactingCharacter != actor->character)
            continue;
        entry->scaleTarget[effect - 1] = amount;
        entry->scaleTicks[effect - 1] = std::bit_cast<std::uint16_t>(frames);
    }
}

void vfSetAttachedStageObjectFigurePose(VFActor *actor, std::int8_t scale, std::int16_t direction) {
    for (auto *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        auto &object = entry->object;
        if (!(object.reactionFlags & 1) || object.reactingCharacter != actor->character)
            continue;
        figpAssignModeForCharacter(object.model, direction ? 1 : 0);
        if (direction)
            figpAssignMotScaleAndDirFor(object.model, scale, static_cast<std::int8_t>(direction));
    }
}

// @verified - p
float flddFindIntersectionBy(const Vector3 &position) {
    flddToggleFlag(2);
    const float height = shenFindIntersectionWithScene(position, position.y);
    bool usePlayerHeight;
    if (g_currentMap.name == SHEN_NAME(QLBT)) {
        usePlayerHeight = height < 111.0f;
        if (usePlayerHeight) {
            VFMA_t *const game = vfmaGetTaskParam();
            if (game && game->type == 8)
                return 111.0f;
        }
    } else {
        const std::int64_t converted = std::isfinite(height) && height >= -9223372036854775808.0f && height < 9223372036854775808.0f ?
            static_cast<std::int64_t>(height) :
            std::numeric_limits<std::int64_t>::min();
        usePlayerHeight = height == -std::numeric_limits<float>::max() || static_cast<std::uint32_t>(converted) == 0x80000000U || height < -10.0f;
    }
    if (!usePlayerHeight)
        return height;
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return GetCharacterWorldPosition(static_cast<Character *>(control->ryo)).y;
}

// @verified - p
float flddGetIntersectionFor(Character *character, Vector3 &position) {
    if (std::abs(position.y) < 0.00000011920929f)
        position.y = 0.0f;
    const Vector3 probe{position.x, position.y + 1.0f, position.z};
    float height = flddFindIntersectionBy(probe);
    if (character) {
        for (const CharacterNode *node = GetCharacterNode(character)->child; node; node = node->child)
            height += node->position.y;
    }
    return height;
}

// @verified - p
float vfFieldGetIntersection(VFActor *actor, Vector3 &position) {
    const float height = flddGetIntersectionFor(actor ? actor->character : nullptr, position);
    if (g_currentMap.name == SHEN_NAME(Q100)) {
        VFMA_t *const game = vfmaGetTaskParam();
        if (game)
            return game->floorIntersectionY;
    }
    return height;
}

// @verified - p
void EmitFightSmoke(std::uint8_t profile, VFActor *actor) {
    const FightSmokeProfile &parameters = fightSmokeProfiles[profile];
    if (g_pFightStage->smokeInitTask || static_cast<double>(GetWeatherPressure()) > 0.1)
        return;
    FBSM_t &smoke = *g_pFightStage->smoke;
    std::uint8_t index = 0;
    while (smoke.slots[index].effect != 0xffffU || smoke.effects[index] == invalidEffect) {
        if (++index >= fightSmokeSlotCount)
            return;
    }
    FightSmokeSlot &slot = smoke.slots[index];
    slot.effect = static_cast<std::uint16_t>(smoke.effects[index]);
    slot.remainingTicks = static_cast<float>(parameters.ticks);
    slot.birthRate = parameters.birthRate;
    ++smoke.activeCount;
    VFActor *const fighter = profile == 5 ? nullptr : actor;
    Character *const character = profile == 5 ? reinterpret_cast<Character *>(actor) : actor->character;
    Vector3 position = GetCharacterWorldPosition(character);
    Vector3 velocity{0.0f, 0.00050000002f, 0.0f};
    switch (profile) {
    case 0:
    case 1:
        velocity.x = fighter->knockbackStrength * 0.029999999f;
        velocity.z = fighter->animationScale * 0.029999999f;
        position.x = fighter->contactSamples[profile == 0 ? 4 : 5].x;
        position.z = fighter->contactSamples[profile == 0 ? 4 : 5].z;
        break;
    case 3:
    case 4:
        position.x = fighter->contactPosition.x;
        position.z = fighter->contactPosition.z;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        position.x = fighter->contactSamples[profile - 4].x;
        position.z = fighter->contactSamples[profile - 4].z;
        break;
    default:
        break;
    }
    std::size_t paletteIndex;
    if (profile >= 6)
        paletteIndex = 12;
    else {
        DateTime time;
        GetCurrentDateTime(time);
        const std::uint8_t hour = static_cast<std::uint8_t>(time.hour);
        const std::size_t timeIndex = hour < 7 || hour >= 19 ? 9 : hour < 11 ? 0 : hour < 15 ? 3 : 6;
        const std::uint16_t material = static_cast<std::uint16_t>(QueryGroundMaterialToken(position, position.y));
        paletteIndex = timeIndex + (material <= 18 && (321024U & (1U << material)) != 0 ? 1 : 2);
    }
    const std::uint8_t *const colour = fightSmokeColours[fightSmokePaletteIndices[paletteIndex]];
    position.y = profile >= 6 ? fighter->contactSamples[profile - 4].y : vfFieldGetIntersection(fighter, position) + 0.050000001f;
    ConfigureSmokeEffect(4, slot.effect, position, Vector3{0.1f, 0.0f, 0.1f}, parameters.particleCount, parameters.birthRate, shen_name{});
    SetSmokeEffectSpread(slot.effect, profile >= 6 ? 4.0f : 2.0f, profile >= 6 ? 3.0f : 2.0f);
    SetSmokeEffectRadialMotion(slot.effect, 0.0f, profile >= 6 ? 4.0f : 3.0f);
    SetParticleEffectInitialScale(0, slot.effect, 2.0f);
    SetParticleEffectColour(0, slot.effect, 0, colour[0], colour[1], colour[2]);
    SetParticleEffectVelocity(0, slot.effect, velocity, Vector3{parameters.horizontalVariance, 0.00050000002f, parameters.horizontalVariance});
    ResumeParticleEffect(0, slot.effect);
}

// @verified - p
std::int32_t GetBtlStatus(Character *character) {
    if (!character)
        return -1;
    for (std::uint8_t i = 0; i < 16; ++i)
        if (g_activeFightData[i].character == character)
            return g_activeFightData[i].status;
    return -1;
}

void EnsureLoadedFightStyle(std::uint8_t style, std::uint8_t slot) {
    FightStyleSlot &record = g_pFightStage->styles[slot];
    if (record.file)
        return;
    PakGlobalData *package = static_cast<PakGlobalData *>(g_pFightStage->packData);
    if (!package)
        package = GetPakLoadTaskDataPointer();
    char filename[32];
    std::snprintf(filename, sizeof(filename), "EN_%s", fightStyleNames[style]);
    void *file = package ? FileLoader(package, filename, shen_name{(SHEN_NAME(BIN_) & 0x00ffffffU) | 0x20000000U}) : nullptr;
    if (!file) {
        char directory[32];
        std::snprintf(filename, sizeof(filename), "EN_%s.BIN", fightStyleNames[style]);
        std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_actNumber, g_currentMap.characters[0], g_currentMap.characters[1],
            g_currentMap.characters[2], g_currentMap.characters[3]);
        file = LoadLegacySceneFile(directory, filename, TASK_FIGH);
    }
    record.file = file;
    record.style = style;
}

// @verified - p
std::uint8_t PublishFightFormation(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style,
    std::uint16_t spawnWord0, std::uint16_t spawnWord1, float x, float y, float z) {
    const std::uint8_t enemyIndex = g_pFightStage->enemySlotCount;
    if (enemyIndex >= g_pFightStage->enemyCount)
        return 0;
    const std::uint8_t groupIndex = static_cast<std::uint8_t>(g_pFightStage->formationCount[0] + g_pFightStage->formationCount[1]);
    if (groupIndex >= g_pFightStage->groupCount)
        return 0;
    FRMG_t &group = g_pFightStage->groups[groupIndex];
    group.character = character;
    group.blocker = blocker;
    group.hp = hp;
    group.totalHp = hp;
    group.style = style;
    group.poolMode = poolMode;
    group.team = team;
    group.state = 0;
    group.waveFlags = 0;
    group.selectFlags = 0;
    group.formationFlags = 0;
    if (team == 1)
        g_pFightStage->accumulatedHp += hp;
    if (reinterpret_cast<std::uintptr_t>(character) == FIGHT_DEAD.name) {
        group.spawnPosition = {x, y, z};
        group.spawnWords[0] = spawnWord0;
        group.spawnWords[1] = spawnWord1;
        g_vfFormationGroupIndex = groupIndex;
        group.slot = nullptr;
    } else {
        FCMG_t *existing = nullptr;
        for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
            if (g_pFightStage->enemySlots[i].character == character) {
                existing = &g_pFightStage->enemySlots[i];
                break;
            }
        }
        if (existing)
            group.slot = existing;
        else {
            FCMG_t &slot = g_pFightStage->enemySlots[enemyIndex];
            group.slot = &slot;
            slot.character = character;
            slot.group = nullptr;
            slot.fighter = nullptr;
            slot.actorIndex = 0;
            slot.quadrant = -1;
            slot.state = 0;
            slot.runningTicks = 0.0f;
            slot.flags = 0;
            slot.counterGroup = team;
            slot.fadeAlpha = 1.0f;
            slot.reflectorIntensity = 0.0f;
            slot.savedPositionState = -1;
            slot.positionState = 0;
            slot.fightEntryIndex = enemyIndex;
            slot.reflectorColour = {};
            g_activeFightData[enemyIndex].hp = hp;
            g_activeFightData[enemyIndex].character = character;
            g_activeFightData[enemyIndex].status = 0;
            g_activeFightData[enemyIndex].initialHp = 100;
            ++g_pFightStage->enemySlotCount;
        }
    }
    EnsureLoadedFightStyle(style, enemyIndex);
    ++g_pFightStage->formationCount[team];
    ++g_pFightStage->secondaryGroupCounters[team];
    return 1;
}

// @verified - p
std::int64_t PublishFightRespawn(std::uint16_t hp, std::uint8_t style, std::uint16_t spawnWord0, float x, float y, float z, std::uint16_t spawnWord1) {
    const std::uint8_t enemyIndex = g_pFightStage->enemySlotCount;
    if (enemyIndex >= g_pFightStage->enemyCount)
        return 0;
    const std::uint8_t groupIndex = static_cast<std::uint8_t>(g_pFightStage->formationCount[0] + g_pFightStage->formationCount[1]);
    if (groupIndex >= g_pFightStage->groupCount)
        return 0;
    FRMG_t &group = g_pFightStage->groups[groupIndex];
    g_vfFormationGroupIndex = groupIndex;
    group.blocker = nullptr;
    group.state = 0;
    group.selectFlags = 0;
    group.formationFlags = 0;
    group.waveFlags = 0;
    group.character = reinterpret_cast<Character *>(static_cast<std::uintptr_t>(FIGHT_DEAD.name));
    group.hp = hp;
    group.totalHp = hp;
    group.style = style;
    group.poolMode = 1;
    group.team = 1;
    g_pFightStage->accumulatedHp += hp;
    group.slot = nullptr;
    group.spawnWords[1] = spawnWord1;
    group.spawnPosition = {x, y, z};
    group.spawnWords[0] = spawnWord0;
    EnsureLoadedFightStyle(style, enemyIndex);
    ++g_pFightStage->formationCount[1];
    ++g_pFightStage->secondaryGroupCounters[1];
    return 1;
}

// Transfer, rather than share a second owning task-data reference.
// @verified - p
bool vfgGameInfoEntry(HLib::Task *source, HLib::Task *target) {
    if (!source || !target || target->taskData)
        return false;
    target->taskData = source->taskData;
    source->taskData = nullptr;
    return true;
}


// @verified - p
void vfg_Callback_GameInfoEntry_callback(void *data) {
    auto *const game = static_cast<VFMA_t *>(data);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i])
            vfUpdateHitFacingAndKnockback(game->actors[i]);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i])
            vfUpdateContactSlotsAndGroundFlags(game->actors[i]);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i])
            vfSelectDefaultReactionAndGuardAnims(game->actors[i]);
    vfgGameInfoEntry(game->gameInfoTask, game->task);
}

// @verified - p
HLib::Task *vfg_Callback_GameInfoEntry_Enqueue(VFMA_t *game) {
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(vfg_Callback_GameInfoEntry_callback, 12, 10, SHEN_NAME(VFGI));
    HLib::AssignTaskDataToTask(task, game);
    return task;
}

// Reset the shared sound slots, retaining the six live actor owners.
void vfInitSfxSlotState(VFMA_t *game) {
    auto *const bytes = reinterpret_cast<std::byte *>(game);
    std::memset(bytes + 0x294, 0, 8);
    std::memset(bytes + 0x29c, 0, 2);
    for (std::uint8_t i = 0; i < 6; ++i) {
        bytes[0x29e + i] = std::byte{};
        std::memset(bytes + 0x270 + 4 * i, 0, 4);
        bytes[0x2a7 + i] = std::byte{};
        if (game->characters[i])
            game->actors[i]->slot->opponentIndex = 0xff;
    }
}

// @verified - p
std::uint8_t AccessFightActorFlag411(std::uint8_t mode, Character *character, std::uint8_t value) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return 0;
    VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    if (!game)
        return 0;
    std::int8_t index = 0;
    while (game->characters[index] != character) {
        if (++index >= 6) {
            index = 99;
            break;
        }
    }
    VFActor *const actor = game->actors[index];
    if (!mode)
        return static_cast<std::uint8_t>(actor->guardLevel);
    actor->guardLevel = static_cast<std::int8_t>(value);
    return 1;
}

// @verified - p
float AccessFightActorGroupFloat(std::uint8_t mode, Character *character, float value) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return -1.0f;
    VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    if (!game)
        return -1.0f;
    std::int8_t index = 0;
    while (game->characters[index] != character) {
        if (++index >= 6) {
            index = 99;
            break;
        }
    }
    VFActor *const actor = game->actors[index];
    switch (mode) {
    case 0:
        return actor->focusedRegenerationScale;
    case 1:
        actor->focusedRegenerationScale = value;
        return 0.0f;
    case 2:
        return actor->guardMaximumHp;
    case 3:
    case 5:
        actor->guardMaximumHp = value;
        return 0.0f;
    case 4:
        return actor->regenerationRate;
    default:
        return 0.0f;
    }
}

// @verified - p
void SetFightActorParameterBytes(shen_name character, const std::uint8_t *values) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return;
    VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    VFActor *const actor = btlGetActorByScenarioName(game, character);
    std::byte *const parameters = const_cast<std::byte *>(actor->motionParameters[2]);
    for (std::uint8_t i = 0; i < 9; ++i)
        parameters[0x1a + i] = static_cast<std::byte>(values[i]);
}

// @verified - p
void SuspendFight() {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return;
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->actors[i])
            motRemoveCallback(game->actors[i]->motion);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (slot.character && slot.group && slot.state == 2)
            motRemoveCallback(slot.motion);
    }
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (shenCheckCharaHasAlphaManager(slot.character))
            slot.savedControlFlags |= 0x80000000U;
        else
            slot.savedControlFlags &= ~0x80000000U;
        cpamSetFlag(slot.character, (slot.savedControlFlags & 0x80U) != 0);
    }
    if (game->inputBuffer)
        BindPadControlEntry(game->inputBuffer);
    if (game->savedInput)
        ReleasePadControlEntry(game->savedInput);
    if (!(g_pFightStage->battleFlags & 0x20000U) && game->inputLockOnStart) {
        SetPlayerControlEnabled(1);
        game->inputLockOnStart = 0;
    }
    HLib::SetupTimedTask(game->task, -1);
    HLib::SetupTimedTask(game->collisionTask, -1);
    HLib::SetupTimedTask(game->gameInfoTask, -1);
}

// @verified - p
std::int32_t ResumeFight(std::int32_t resetActors) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    HLib::ResolveDeferredCallback(game->task);
    HLib::ResolveDeferredCallback(game->collisionTask);
    HLib::ResolveDeferredCallback(game->gameInfoTask);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->actors[i])
            vfRegisterGroundTargetCallbacks(game->actors[i]->motion, game->actors[i]);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (!slot.character || !slot.group || slot.state != 2)
            continue;
        for (std::uint32_t selector : fightMotionSelectors) {
            AssignMotionTargetCallback(slot.motion, AdjustCooldownGroundTarget, selector);
            AssignMotionTargetParameter(slot.motion, &slot, selector);
        }
    }
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        cpamSetFlag(slot.character, (slot.savedControlFlags & 0x80000000U) != 0);
        slot.savedControlFlags &= ~0x80000000U;
    }
    std::int32_t result = plymIsSearchStartRequested();
    game->inputLockOnStart = static_cast<std::uint8_t>(result);
    if (g_pFightStage->battleFlags & 0x20000U) {
        game->inputBuffer = nullptr;
        game->savedInput = nullptr;
    } else {
        if (result)
            SetPlayerControlEnabled(0);
        game->savedInput = AllocatePadControlEntry();
        game->inputBuffer = BindPadControlEntry(game->savedInput);
        result = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(game->inputBuffer));
    }
    if (resetActors) {
        g_pFightStage->battleFlags |= 0x100000U;
        result = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(g_pFightStage));
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            VFActor &actor = *game->actors[i];
            MotionWork *const motion = actor.motion;
            actor.position = GetCharacterWorldPosition(actor.character);
            actor.facingAngle = static_cast<std::uint16_t>(GetAllAngYForNode(&actor.character->entity.node));
            vfResetReactionProgram(&actor);
            actor.moveFlags &= ~(VFMF_HitReactionLock | VFMF_MirroredStance);
            actor.facingYaw = actor.facingAngle;
            actor.contactPosition = actor.position;
            actor.queuedAction = 0;
            actor.facingAngleDelta = 0;
            actor.savedMarkerMask = 0;
            actor.activeMotionFlags = 0;
            actor.hitStateFlags = 0;
            actor.secondaryMotionFlags = 0;
            actor.markerMask = 0;
            actor.motionFlags = 0;
            if (SetMotionRegionFlag(motion, 70, 1, MotionRegion::LowerBody))
                actor.moveFlags |= VFMF_MirrorReactionAngle;
            else
                actor.moveFlags &= ~VFMF_MirrorReactionAngle;
            const float frame = GetMotionFrame(motion, MotionRegion::LowerBody);
            if (actor.currentAnimation != GetMotionAnimationId(motion, MotionRegion::LowerBody))
                vfSelectDefaultMotion(&actor, 0);
            result = static_cast<std::int32_t>(mtwkSetExtraFlag(motion, 1, 2));
            actor.stateFlags |= VFSF_Unknown80000;
            actor.animationTime = frame;
        }
    }
    return result;
}

} // namespace shenmue::s2
