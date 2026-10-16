#include "s1/task/scen.hpp"
#include "s1/task/vfma.hpp"
#include "s1/input.hpp"
#include "s1/motion.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/time.hpp"
#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/lighting.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/btpk.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/char.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/vfcl.hpp"
#include "s1/task/vf_core.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/tmex.hpp"
#include "s1/subtitles.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <type_traits>

#include "s1/task/vf_internal.hpp"

namespace shenmue::s1 {
using namespace vf_detail;

namespace vf_detail {

std::uint8_t g_fgenNumAnchors = 0;
std::uint8_t g_frmgFormationIndex = 99;

void AllocateFCMG(std::uint8_t count) {
    std::uint8_t total = count <= 16 ? count : 16;
    if (!total)
        total = 1;
    auto *const slots = static_cast<VFEnemySlot *>(HLib_AllocHeapMemBlock(sizeof(VFEnemySlot) * total, static_cast<std::int32_t>(SHEN_NAME(FCMG))));
    g_pFightStage->enemySlots = slots;
    if (slots)
        HLib_EnableTokenCheck(slots);
    g_pFightStage->maxEnemies = total;
    for (std::uint8_t index = 0; index < total; ++index)
        g_pFightStage->enemySlots[index].character = nullptr;
}

void AllocateFRMG(std::uint8_t count) {
    std::uint8_t total = count <= 127 ? count : 127;
    if (!total)
        total = 1;
    auto *const groups = static_cast<VFFormationGroup *>(HLib_AllocHeapMemBlock(sizeof(VFFormationGroup) * total, static_cast<std::int32_t>(SHEN_NAME(FRMG))));
    g_pFightStage->formations = groups;
    if (groups)
        HLib_EnableTokenCheck(groups);
    g_pFightStage->formationCapacity = total;
    for (std::uint8_t index = 0; index < total; ++index) {
        VFFormationGroup &group = g_pFightStage->formations[index];
        group.character = nullptr;
        group.contactType = 0;
        group.scriptParameter = 0;
        group.field4e = -1;
        group.field4f = -1;
        group.rankMax = -1;
        group.rankMin = -1;
        group.startType = -1;
    }
    g_frmgFormationIndex = 99;
}

void AllocateFCEL(std::int8_t count) {
    std::uint8_t total = static_cast<std::uint8_t>(count <= 64 ? count : 64);
    if (!total)
        total = 1;
    auto *const masks = static_cast<VFAreaMask *>(HLib_AllocHeapMemBlock(std::size_t{total} << 6U, static_cast<std::int32_t>(SHEN_NAME(FCEL))));
    g_pFightStage->areaMasks = masks;
    if (masks)
        HLib_EnableTokenCheck(masks);
    g_pFightStage->areaMaskCount = total;
    for (std::uint8_t index = 0; index < total; ++index)
        g_pFightStage->areaMasks[index].active = 0;
}

void AllocateFGEN(std::uint8_t count) {
    std::uint8_t total = count <= 127 ? count : 127;
    if (!total)
        total = 1;
    auto *const anchors = static_cast<VFAnchor *>(HLib_AllocHeapMemBlock(sizeof(VFAnchor) * total, static_cast<std::int32_t>(SHEN_NAME(FGEN))));
    g_pFightStage->anchors = anchors;
    if (anchors)
        HLib_EnableTokenCheck(anchors);
    g_pFightStage->generatorCapacity = total;
    for (std::uint8_t index = 0; index < total; ++index)
        g_pFightStage->anchors[index].enabled = 0;
    g_fgenNumAnchors = 0;
}

std::uint32_t vfConfigureFormationGroup(Character *character, std::uint8_t mode, std::uint32_t flags, std::int32_t value) {
    std::uint8_t formationIndex = 0;
    if (character != kVFFormationSpawnCharacter || g_frmgFormationIndex == 99) {
        for (std::int32_t index = g_pFightStage->formationCapacity - 1; index >= 0; --index) {
            if (g_pFightStage->formations[index].character != character)
                continue;
            formationIndex = static_cast<std::uint8_t>(index);
            if (index == 99)
                return 0;
            break;
        }
    } else {
        formationIndex = g_frmgFormationIndex;
    }
    VFFormationGroup &group = g_pFightStage->formations[formationIndex];
    if (mode == 1) {
        group.actorFlags &= ~flags;
        return 1;
    }
    group.actorFlags |= flags;
    switch (flags) {
    case 0x1:
        group.entryIndex = static_cast<std::uint16_t>(value);
        break;
    case 0x2:
    case 0x4:
    case 0x80000:
        group.wave = static_cast<std::uint16_t>(value);
        break;
    case 0x100:
        group.spawnCount = static_cast<std::uint16_t>(value);
        break;
    case 0x200:
        group.delay = static_cast<std::int16_t>(value);
        break;
    case 0x400:
        if (group.rankMin == -1)
            group.rankMin = static_cast<std::int16_t>(value);
        else
            group.rankMax = static_cast<std::int16_t>(value);
        break;
    case 0x20000:
        group.groupHpFloor = static_cast<std::uint16_t>(value);
        break;
    case 0x40000:
        group.hpRegenRate = static_cast<std::int8_t>(value);
        break;
    case 0x200000:
        group.waveFlags |= static_cast<std::uint32_t>(value);
        break;
    case 0x400000:
        group.selectFlags |= static_cast<std::uint8_t>(value);
        break;
    case 0x800000:
        group.contactType = 2;
        group.field4e = static_cast<std::int8_t>(value);
        break;
    case 0x1000000:
        group.contactType = 1;
        group.scriptParameter = 2;
        group.animationId = 1128;
        group.field4e = static_cast<std::int8_t>(value);
        break;
    case 0x2000000:
        group.contactType = 3;
        break;
    case 0x10000000:
        group.startType = static_cast<std::int8_t>(value);
        break;
    case 0x20000000:
        group.contactType = 5;
        break;
    default:
        break;
    }
    return 1;
}

void vfSetupAreaMaskTable() {
    VFFightStage *const stage = g_pFightStage;
    std::uint8_t count = 0;
    for (std::uint8_t index = 0; index < stage->areaMaskCount; ++index) {
        VFAreaMask &area = stage->areaMasks[index];
        if (area.active == 1 && area.shapeMode == 1) {
            for (std::int8_t &neighbor : area.neighbors)
                neighbor = -1;
            ++count;
        }
    }
    for (std::uint8_t first = 0; first < count; ++first) {
        VFAreaMask &area = stage->areaMasks[first];
        const Vector2 *const a = area.vertices;
        for (std::uint8_t second = static_cast<std::uint8_t>(first + 1); second < count; ++second) {
            VFAreaMask &other = stage->areaMasks[second];
            const Vector2 *const b = other.vertices;
            if (b[0].x == a[3].x && b[0].y == a[3].y && b[1].x == a[2].x && b[1].y == a[2].y) {
                other.neighbors[0] = static_cast<std::int8_t>(first);
                area.neighbors[2] = static_cast<std::int8_t>(second);
            } else if (b[3].x == a[0].x && b[3].y == a[0].y && b[2].x == a[1].x && b[2].y == a[1].y) {
                other.neighbors[2] = static_cast<std::int8_t>(first);
                area.neighbors[0] = static_cast<std::int8_t>(second);
            } else if (b[1].x == a[0].x && b[1].y == a[0].y && b[2].x == a[3].x && b[2].y == a[3].y) {
                other.neighbors[3] = static_cast<std::int8_t>(first);
                area.neighbors[1] = static_cast<std::int8_t>(second);
            } else if (b[0].x == a[1].x && b[0].y == a[1].y && b[3].x == a[2].x && b[3].y == a[2].y) {
                other.neighbors[1] = static_cast<std::int8_t>(first);
                area.neighbors[3] = static_cast<std::int8_t>(second);
            }
        }
    }
}

std::uint16_t vfAccumulatedYaw(Character *character) {
    std::uint16_t yaw = 0;
    for (CharacterNode *node = character ? &character->entity.node : nullptr; node; node = node->child)
        yaw = static_cast<std::uint16_t>(yaw + static_cast<std::uint16_t>(node->rotation.y));
    return yaw;
}

void vfSetFormationWorldYaw(Character *character, std::int16_t yaw) {
    for (CharacterNode *node = character->entity.node.child; node; node = node->child)
        yaw = static_cast<std::int16_t>(yaw - static_cast<std::uint16_t>(node->rotation.y));
    character->entity.node.rotation.y = yaw;
}

void vfSetCharacterMapcEnabled(Character *character, bool enabled) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, SHEN_NAME(CPAM)) : nullptr;
    if (!component)
        return;
    if (enabled)
        component->flags |= 1U;
    else
        component->flags &= ~1U;
}

void vfUpdateDefaultReactionAnim(VFEnemySlot *group, std::uint16_t animationId) {
    VFActor *const fighter = group->fighter;
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    motion->pendingAnimId = animationId;
    if (fighter && group->state == SS_Active) {
        if ((fighter->moveFlags & kVFMirrorReactionAngle) != 0)
            motion->motionFlags |= 0x80U;
        else
            motion->motionFlags &= ~0x80U;
    }
    group->isRunning |= 1;
}

void UpdateLightMask_1(VFEnemySlot *group) {
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    Character *const character = group->character;
    std::uint32_t saved = motion->poseEvaluationStage & 3U;
    motion->poseEvaluationStage = 1;
    if ((motion->motionFlags & 0x40U) != 0)
        saved |= 0x10U;
    else
        motion->motionFlags |= 0x40U;
    if (CharacterComponentNode *const ccow = character ? HLib_GetCharacterComponent(character, SHEN_NAME(CCOW)) : nullptr) {
        if ((ccow->flags & 1U) != 0) {
            ccow->flags &= ~1U;
            saved |= 4U;
        }
    }
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if ((manager->flags & 1U) != 0)
        saved |= 8U;
    else
        manager->flags |= 1U;
    group->savedConfigFlags = saved;
    group->playbackRate = motion->playbackRate;
    motion->playbackRate = 1.0f;
    group->savedActiveFlag = character && (character->flags.value & 0x10U) != 0 ? 1U : 0U;
    group->renderMode = character ? static_cast<std::uint32_t>(character->shadowMode) : 0xffffffffU;
    if (character) {
        character->flags.value |= CharacterFlagActive;
        character->shadowMode = 0;
    }
    group->savedLightMask = character ? character->lightMask : 0U;
    group->savedVisibleFlag = character && (character->flags.value & CharacterFlagWithinVisibleDistance) != 0 ? 1U : 0U;
    motion->rootAngleFlags |= 0x400000U;
}

bool vfBindEnemySlotToFormationEntry(VFEnemySlot *group) {
    VFFightStage *const stage = g_pFightStage;
    Character *const character = group->character;
    VFFormationGroup *wave = nullptr;
    for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
        VFFormationGroup &candidate = stage->formations[index];
        if (candidate.character == character && candidate.waveId == 0) {
            wave = &candidate;
            break;
        }
    }
    if (!wave)
        return false;
    MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr;
    group->motion = manager ? &manager->motion : nullptr;
    group->position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    group->accumulatedYaw = vfAccumulatedYaw(group->character);
    UpdateLightMask_1(group);
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    motion->motionFlags &= ~0x8U;
    motion->motionFlags &= ~0x10U;
    group->wave = wave;
    wave->slot = group;
    --stage->teamRespawnTimer[group->teamId];
    return true;
}

void vfApplyEnemySlotConfigToCharaAndMot(VFEnemySlot *group) {
    const std::uint32_t saved = group->savedConfigFlags;
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    Character *const character = group->character;
    motion->poseEvaluationStage = static_cast<std::uint8_t>(saved & 3U);
    if ((saved & 0x10U) == 0)
        motion->motionFlags &= ~0x40U;
    if ((saved & 4U) != 0) {
        if (CharacterComponentNode *const ccow = character ? HLib_GetCharacterComponent(character, SHEN_NAME(CCOW)) : nullptr)
            ccow->flags |= 1U;
    }
    if ((saved & 8U) == 0)
        MOTM_GetWork(character)->flags &= ~1U;
    motion->playbackRate = group->playbackRate;
    if (character) {
        if (group->savedActiveFlag)
            character->flags.value |= CharacterFlagActive;
        else
            character->flags.value &= ~CharacterFlagActive;
        character->shadowMode = static_cast<std::int32_t>(group->renderMode);
        character->lightMask = 7U;
        if (group->savedVisibleFlag)
            character->flags.value |= CharacterFlagWithinVisibleDistance;
        else
            character->flags.value &= ~CharacterFlagWithinVisibleDistance;
    }
    motion->rootAngleFlags &= ~0x400000U;
}

bool vfTransitionEnemySlotToInactive(VFMAData *game, VFEnemySlot *group) {
    const std::uint8_t team = group->teamId;
    VFFightStage *const stage = g_pFightStage;
    switch (group->state) {
    case SS_Allocated:
    case SS_Inactive:
        return false;
    case SS_Ready:
        --stage->readyEnemySlots[team];
        break;
    case SS_Active:
        if (group->character)
            group->character->flags.value &= ~CharacterFlagActive;
        if (group->character)
            group->character->flags.value &= ~CharacterFlagWithinVisibleDistance;
        game->characters[static_cast<std::uint8_t>(group->battleIndex)] = nullptr;
        group->battleIndex = 0;
        --stage->activeFightersByTeam[team];
        break;
    case SS_Cooldown:
        --stage->groupCounters[team];
        break;
    default:
        break;
    }
    vfApplyEnemySlotConfigToCharaAndMot(group);
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    motion->motionFlags &= ~1032U;
    motion->mobjCallback = nullptr;
    motion->previousAnimId = 0;
    group->state = SS_Inactive;
    group->tickFrames = 0;
    ++stage->groupCounters[team + 2];
    return true;
}

void vfApplyEnemySlotAxisOffsetDecay(std::int8_t releasedSlot) {
    VFFightStage *const stage = g_pFightStage;
    const std::int32_t mask = 1 << releasedSlot;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot &group = stage->enemySlots[index];
        if (!group.character || group.state != SS_Cooldown || (mask & group.activeMask) == 0)
            continue;
        for (std::uint8_t effector = 0; effector < 7; ++effector) {
            const std::int32_t flags = group.effectorEnemyMasks[effector];
            if ((flags & mask) == 0)
                continue;
            group.effectorEnemyMasks[effector] = flags & ~mask;
            group.effectorGroundOffsets[effector] -= 0.12f;
            if (effector == 2) {
                group.position.y -= 0.12f;
                if (group.character)
                    shenApplyTranslationToAll(group.character->entity.node, group.position);
            }
        }
        group.activeMask &= ~mask;
    }
}

float vfAccumEnemyGroundOffsetAndMask(const Vector3 &position, std::int32_t &nearbyEnemies) {
    VFFightStage *const stage = g_pFightStage;
    nearbyEnemies = 0;
    float penalty = 0.0f;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        const VFEnemySlot &group = stage->enemySlots[index];
        if (group.character && group.state == SS_Cooldown &&
            std::sqrt((position.x - group.position.x) * (position.x - group.position.x) + (position.z - group.position.z) * (position.z - group.position.z)) <
                0.7f) {
            nearbyEnemies |= 1 << index;
            penalty += 0.12f;
        }
    }
    return penalty;
}

bool vfStageCheckPosNoEnemies(float x, float z) {
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        Character *const character = stage->enemySlots[index].character;
        if (!character)
            continue;
        const Vector3 position = GetPositionForEntity(character->entity.node);
        if ((x - position.x) * (x - position.x) + (z - position.z) * (z - position.z) < 4.0f)
            return false;
    }
    return true;
}

void vfUpdateFormationSlotFromNearestAnchor(VFFormationGroup *formation, float x, float z) {
    VFFightStage *const stage = g_pFightStage;
    float biasedBest = 999.0f;
    std::uint8_t biasedIndex = 99;
    const std::uint8_t useCenterBias = stage->useCenterBias;
    std::uint8_t anchorIndex = 99;
    float anchorBest = 999.0f;
    const float battleRadius = std::sqrt((z - stage->battleCenter.z) * (z - stage->battleCenter.z) + (x - stage->battleCenter.x) * (x - stage->battleCenter.x));
    for (std::uint8_t index = 0; index < g_fgenNumAnchors; ++index) {
        const VFAnchor &anchor = stage->anchors[index];
        if (anchor.enabled != 1 || anchor.excludeFromSelection)
            continue;
        const float anchorX = anchor.position.x;
        const float anchorZ = anchor.position.z;
        const float distance = std::sqrt((z - anchorZ) * (z - anchorZ) + (x - anchorX) * (x - anchorX));
        if (distance <= 8.0f)
            continue;
        const float error = std::fabs(distance - 8.0f);
        if (useCenterBias &&
            std::sqrt((anchorX - stage->battleCenter.x) * (anchorX - stage->battleCenter.x) +
                (anchorZ - stage->battleCenter.z) * (anchorZ - stage->battleCenter.z)) > battleRadius) {
            if (biasedBest > error && vfStageCheckPosNoEnemies(anchorX, anchorZ)) {
                biasedBest = error;
                biasedIndex = index;
            }
        } else if (anchorBest > error && vfStageCheckPosNoEnemies(anchorX, anchorZ)) {
            anchorBest = error;
            anchorIndex = index;
        }
    }
    if (anchorIndex == 99)
        anchorIndex = biasedIndex;
    if (anchorIndex != 99)
        formation->spawnPosition = stage->anchors[anchorIndex].position;
}

std::uint8_t vfStageFindQuadrantForPointXZ(float x, float z) {
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t quadrant = 0; quadrant < stage->areaMaskCount; ++quadrant) {
        if (vfStageCheckPointInQuadrantXZ(x, z, static_cast<std::int8_t>(quadrant)))
            return quadrant;
    }
    return 0xffU;
}

void vfmaAssignHmdlReferences(VFMAData *game, VFEnemySlot *group) {
    BTPKTaskData *const battlePack = game->packData;
    if (!battlePack)
        return;
    if (group->modelSlotIndex == -1)
        group->model = group->character->model;
    else
        battlePack->packModels->slots[group->modelSlotIndex].state = PKMLS_PendingFree;
    PKMLData *const models = battlePack->packModels;
    std::int32_t lowestIndex = 1000;
    for (std::uint8_t index = 0; index < 12; ++index) {
        if (models->slots[index].state == PKMLS_Loaded && static_cast<std::int8_t>(models->slots[index].battle70Index) < lowestIndex) {
            lowestIndex = static_cast<std::int8_t>(models->slots[index].battle70Index);
            battlePack->modelIndex = index;
        }
    }
    Character *const character = group->character;
    if (lowestIndex == 1000) {
        character->model = group->model;
    } else {
        character->model = models->slots[battlePack->modelIndex].model;
        group->modelSlotIndex = static_cast<std::int8_t>(battlePack->modelIndex);
        battlePack->packModels->slots[battlePack->modelIndex].state = PKMLS_Used;
    }
}

bool vfAttachCharaToFormationSlot(VFMAData *game, VFEnemySlot *group) {
    VFFightStage *const stage = g_pFightStage;
    VFFormationGroup *wave = nullptr;
    for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
        VFFormationGroup &candidate = stage->formations[index];
        if (candidate.character == kVFFormationSpawnCharacter && candidate.waveId == 0 && static_cast<std::uint8_t>(candidate.teamId) == group->teamId) {
            wave = &candidate;
            break;
        }
    }
    if (!wave)
        return false;
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    motion->motionFlags &= 0xfffffbf7U;
    motion->mobjCallback = nullptr;
    wave->character = group->character;
    std::int16_t desiredYaw = 0;
    if (game->playerSlot == -1 || wave->contactType) {
        desiredYaw = static_cast<std::int16_t>(wave->spawnYaw);
    } else {
        const VFActor *const player = game->actors[game->playerSlot];
        const float playerX = player->position.x;
        const float playerZ = player->position.z;
        if ((wave->selectFlags & 1U) != 0 &&
            (std::sqrt((wave->spawnPosition.x - playerX) * (wave->spawnPosition.x - playerX) +
                 (wave->spawnPosition.z - playerZ) * (wave->spawnPosition.z - playerZ)) < 8.0f ||
                !vfStageCheckPosNoEnemies(wave->spawnPosition.x, wave->spawnPosition.z))) {
            vfUpdateFormationSlotFromNearestAnchor(wave, playerX, playerZ);
        }
        desiredYaw = static_cast<std::int16_t>(atan2_fixed16(wave->spawnPosition.x - playerX, wave->spawnPosition.z - playerZ));
    }
    if (wave->character)
        shenApplyTranslationToAll(wave->character->entity.node, wave->spawnPosition);
    group->position = wave->spawnPosition;
    if (wave->character)
        vfSetFormationWorldYaw(wave->character, desiredYaw);
    vfUpdateDefaultReactionAnim(group, wave->animationId);
    group->quadrant = static_cast<std::int8_t>(vfStageFindQuadrantForPointXZ(group->position.x, group->position.z));
    group->wave = wave;
    wave->slot = group;
    --stage->teamRespawnTimer[group->teamId];
    group->shouldSetPosition = 0;
    return true;
}

bool vfEnemySlotResetAndRestartCycle(VFMAData *game, VFEnemySlot *group) {
    const std::uint8_t team = group->teamId;
    VFFightStage *const stage = g_pFightStage;
    switch (group->state) {
    case SS_Ready:
        return false;
    case SS_Active: {
        if (group->tickFrames < 30)
            return false;
        VFActor *const fighter = group->fighter;
        group->position = group->character ? GetPositionForEntity(group->character->entity.node) : Vector3{};
        group->accumulatedYaw = vfAccumulatedYaw(group->character);
        vfSelectDefaultReactionAndGuardAnims(*fighter);
        vfUpdateDefaultReactionAnim(group, static_cast<std::uint16_t>(fighter->defaultReactionAnimationId));
        static_cast<MotionInformationWork *>(group->motion)->mobjCallback = nullptr;
        if (group->character)
            group->character->flags.value &= ~CharacterFlagActive;
        if (group->character)
            group->character->flags.value &= ~CharacterFlagWithinVisibleDistance;
        game->characters[static_cast<std::uint8_t>(group->battleIndex)] = nullptr;
        group->battleIndex = 0;
        group->wave->waveId = 0;
        g_vfFightRegistry[static_cast<std::uint8_t>(group->registryIndex)].slot = 0;
        --stage->activeFightersByTeam[team];
        break;
    }
    case SS_Cooldown:
        if (!vfAttachCharaToFormationSlot(game, group))
            return false;
        --stage->groupCounters[team];
        group->fadeAlpha = -1.0f;
        if (group->character)
            group->character->renderOpacity = 0.0f;
        vfmaAssignHmdlReferences(game, group);
        break;
    case SS_Allocated:
    case SS_Inactive:
        if (!vfBindEnemySlotToFormationEntry(group))
            return false;
        break;
    default:
        break;
    }
    group->quadrant =
        static_cast<std::int8_t>(vfStageResolveQuadrantForPointXZ(group->position.x, group->position.z, static_cast<std::uint8_t>(group->quadrant)));
    group->state = SS_Ready;
    group->tickFrames = 0;
    ++stage->readyEnemySlots[team];
    return true;
}

void vfUpdateContactEffectorHeightConstraints_Callback(MotionInformationWork *motion, void *user);

bool vfTransitionEnemySlotToCooldown(VFMAData *game, VFEnemySlot *group) {
    const std::uint8_t state = group->state;
    const std::uint8_t team = group->teamId;
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    group->position = group->character ? GetPositionForEntity(group->character->entity.node) : Vector3{};
    VFFightStage *const stage = g_pFightStage;
    motion->mobjCallback = nullptr;
    switch (state) {
    case SS_Ready:
        --stage->readyEnemySlots[team];
        break;
    case SS_Active: {
        VFActor *fighter = group->fighter;
        std::int32_t activeMask = 0;
        for (std::size_t effector = 0; effector < 7; ++effector) {
            const float offset = fighter->effectorGroundOffsets[effector];
            group->effectorGroundOffsets[effector] = offset;
            group->effectorOffsetActive[effector] = std::fabs(group->position.y - offset) > 0.001f ? 1U : 0U;
            group->effectorEnemyMasks[effector] = fighter->effectorEnemyMasks[effector];
            activeMask |= fighter->effectorEnemyMasks[effector];
        }
        group->activeMask = static_cast<std::uint32_t>(activeMask);
        motion->mobjCallback = reinterpret_cast<void *>(vfUpdateContactEffectorHeightConstraints_Callback);
        motion->mobjCallbackUser = group;
        fighter = group->fighter;
        group->facingAngle = static_cast<std::int16_t>(fighter->facingAngleStep);
        group->quadrant = static_cast<std::int8_t>(fighter->cachedQuadrant);
        for (float &blend : group->effectorHeightBlend)
            blend = -10000.0f;
        std::uint32_t triggerFlags = (fighter->hitStateFlags2 >> 11U) & 1U;
        if ((fighter->hitFlags & 0x4000000U) != 0)
            triggerFlags |= (fighter->hitFlags & 0x800U) != 0 ? 0x20U : 0x10U;
        if ((fighter->moveFlags & kVFMirrorReactionAngle) != 0)
            triggerFlags |= 0x40U;
        if ((group->wave->selectFlags & 0xa0U) != 0)
            triggerFlags |= 0x80U;
        group->triggerFlags = triggerFlags;
        if (group->character)
            group->character->flags.value &= ~CharacterFlagActive;
        if (group->character)
            group->character->flags.value &= ~CharacterFlagWithinVisibleDistance;
        game->characters[static_cast<std::uint8_t>(group->battleIndex)] = nullptr;
        group->battleIndex = 0;
        --stage->activeFightersByTeam[team];
        break;
    }
    case SS_Cooldown:
        return false;
    default:
        break;
    }
    group->state = SS_Cooldown;
    group->tickFrames = 0;
    group->wave->waveId = 2;
    g_vfFightRegistry[static_cast<std::uint8_t>(group->registryIndex)].slot = 2;
    group->cooldownDelayFrames = static_cast<std::int8_t>(stage->groupCounters[0] + stage->groupCounters[1]);
    group->fadeAlpha = 1.0f;
    static_cast<MotionInformationWork *>(group->motion)->motionFlags |= 0x8U;
    motion->previousAnimId = 0;
    ++stage->groupCounters[team];
    return true;
}

void vfmaRecycleDefeatedEnemySlots(VFMAData *game) {
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot *const group = &stage->enemySlots[index];
        Character *const character = group->character;
        if (!character || group->state != SS_Cooldown)
            continue;
        if (group->cooldownDelayFrames) {
            --group->cooldownDelayFrames;
            continue;
        }
        if (static_cast<std::int8_t>(group->triggerFlags) < 0) {
            character->renderOpacity = 1.0f;
            vfSetCharacterMapcEnabled(group->character, true);
            vfEnemySlotResetAndRestartCycle(game, group);
            vfApplyEnemySlotAxisOffsetDecay(static_cast<std::int8_t>(index));
            continue;
        }
        if (g_areaId == SHEN_NAME(MFBT)) {
            if (group->fadeAlpha < 1.0f) {
                group->fadeAlpha = 0.0f;
                character->renderOpacity = 0.0f;
                vfTransitionEnemySlotToInactive(game, group);
                vfApplyEnemySlotAxisOffsetDecay(static_cast<std::int8_t>(index));
                continue;
            }
        } else {
            vfSetCharacterMapcEnabled(character, true);
        }
        vfTransitionEnemySlotToInactive(game, group);
        vfApplyEnemySlotAxisOffsetDecay(static_cast<std::int8_t>(index));
    }
}

void vfUpdateRenderOpacityForAll(VFMAData *game) {
    VFFightStage *const stage = g_pFightStage;
    if (static_cast<std::uint8_t>(stage->groupCounters[0] + stage->groupCounters[1]) != game->maxActiveEnemies)
        return;
    VFEnemySlot *group = nullptr;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot &candidate = stage->enemySlots[index];
        if (candidate.character && candidate.state == SS_Cooldown && !candidate.cooldownDelayFrames) {
            group = &candidate;
            break;
        }
    }
    if (!group)
        return;
    if (group->character) {
        vfSetCharacterMapcEnabled(group->character, false);
        group->character->renderOpacity = 1.0f;
    }
    if (stage->teamRespawnTimer[group->teamId] && (g_areaId == SHEN_NAME(MFBT) || static_cast<std::int8_t>(group->triggerFlags) < 0)) {
        Character *const character = group->character;
        const float alpha = group->fadeAlpha - 0.03f;
        group->fadeAlpha = alpha;
        if (alpha > 0.0f) {
            if (character)
                character->renderOpacity = alpha;
        } else {
            group->fadeAlpha = 0.0f;
            if (character)
                character->renderOpacity = 0.0f;
            vfmaRecycleDefeatedEnemySlots(game);
        }
    }
}

bool vfRegisterFormationSlotForBattle(Character *character);

void vfUpdateFormationGroupRenderOpacity(VFEnemySlot *group) {
    const float alpha = group->fadeAlpha;
    if (alpha >= 1.0f)
        return;
    float next = alpha + 0.03f;
    if (next > 1.0f) {
        vfSetCharacterMapcEnabled(group->character, true);
        next = 1.0f;
    }
    group->fadeAlpha = next;
    if (group->character)
        group->character->renderOpacity = std::fmax(0.0f, next);
}

void vfmaWarpEntityToFormationAnchor(VFMAData *game, VFEnemySlot *group, VFFormationGroup *formation) {
    if (game->playerSlot == -1 || static_cast<std::int8_t>(formation->selectFlags) >= 0)
        return;
    std::uint8_t resolved = vfStageResolveQuadrantForPointXZ(group->position.x, group->position.z, static_cast<std::uint8_t>(group->quadrant));
    const VFActor *const player = game->actors[game->playerSlot];
    const std::uint8_t cached = player->cachedQuadrant;
    if (cached == 0xffU || resolved == 0xffU)
        return;
    const VFAreaMask &area = g_pFightStage->areaMasks[static_cast<std::int8_t>(cached)];
    std::uint32_t mask = area.unknown28;
    if (resolved >= 32) {
        resolved = static_cast<std::uint8_t>(resolved - 32);
        mask = area.unknown2c;
    }
    if (((mask >> resolved) & 1U) == 0)
        return;
    vfUpdateFormationSlotFromNearestAnchor(formation, player->position.x, player->position.z);
    if (formation->character)
        shenApplyTranslationToAll(formation->character->entity.node, formation->spawnPosition);
    group->position.x = formation->spawnPosition.x;
    group->position.z = formation->spawnPosition.z;
    if (formation->character)
        vfSetFormationWorldYaw(formation->character,
            static_cast<std::int16_t>(atan2_fixed16(formation->spawnPosition.x - player->position.x, formation->spawnPosition.z - player->position.z)));
    group->shouldSetPosition = 0;
    group->fadeAlpha = 0.0f;
    if (group->character)
        group->character->renderOpacity = 0.0f;
    vfSetCharacterMapcEnabled(group->character, true);
}

void vfUpdateEnemySlotFormationAndYaw(VFMAData *game) {
    VFFightStage *stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot *const group = &stage->enemySlots[index];
        if (!group->character || !group->wave || group->state != SS_Ready)
            continue;
        group->position = GetPositionForEntity(group->character->entity.node);
        VFFormationGroup *const wave = group->wave;
        const std::uint8_t contactType = wave->contactType;
        if (contactType) {
            if (contactType > 2 && contactType <= 5) {
                vfHandleAggroStateChange(game, group, wave);
                stage = g_pFightStage;
            }
        } else {
            vfmaWarpEntityToFormationAnchor(game, group, wave);
        }
        vfUpdateFormationGroupRenderOpacity(group);
        group->accumulatedYaw = vfAccumulatedYaw(group->character);
    }
}

bool vfRegisterFormationSlotForBattle(Character *character) {
    VFMAData *const game = vfMainGetCurrent();
    if (!game)
        return false;
    VFEnemySlot *slot = nullptr;
    VFFormationGroup *formation = nullptr;
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
        formation = &stage->formations[index];
        if (formation->character == character && !formation->waveId) {
            slot = formation->slot;
            break;
        }
    }
    slot->shouldSetPosition = 0;
    if (!slot->fighter) {
        auto *const fighter = static_cast<VFActor *>(HLib_AllocHeapMemBlock(kNativeActorAllocationSize, static_cast<std::int32_t>(kActorToken)));
        if (fighter)
            HLib_EnableTokenCheck(fighter);
        std::memset(fighter, 0, kNativeActorAllocationSize);
        slot->fighter = fighter;
        auto *const runtime = static_cast<VFSlotRuntimeState *>(HLib_AllocHeapMemBlock(sizeof(VFSlotRuntimeState), static_cast<std::int32_t>(kSlotToken)));
        if (runtime)
            HLib_EnableTokenCheck(runtime);
        std::memset(runtime, 0, sizeof(VFSlotRuntimeState));
        slot->fighter->slot = runtime;
    }
    if (stage->activeFightersByTeam[0] + stage->activeFightersByTeam[1] == 6) {
        const std::uint8_t rank = vfPickOpponentSlotByPrioRank(game, static_cast<std::uint8_t>(formation->selectFlags));
        if (rank == 99) {
            vfEnemySlotResetAndRestartCycle(game, slot);
            return false;
        }
        vfEnemySlotResetAndRestartCycle(game, game->battleMeta.groups[rank]);
    }
    if (game->state != VFS_InBattle || slot->state == SS_Active)
        return false;
    const std::uint8_t index = vfRegisterContactSlot(game, formation->slot);
    if (index == 99)
        return false;
    vfAssignNearestOpponents(game);
    vfResetActorReactionStateForContact(game, index);
    vfUpdateGuardLevel(game, index);
    return true;
}
} // namespace vf_detail

void Script_vfConfigureFormationGroup(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    Character *character = kVFFormationSpawnCharacter;
    if (params[0] != 0xffffffffU)
        character = params[0] ? FindCharacter(context, params[0]) : nullptr;
    SetResult(context, vfConfigureFormationGroup(character, static_cast<std::uint8_t>(params[1]), params[2], static_cast<std::int32_t>(params[3])));
}

void Script_FGTM_Utils(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    SetResult(context, params && vfRegisterFormationSlotForBattle(FindCharacter(context, params[0])) ? 1U : 0U);
}
} // namespace shenmue::s1
