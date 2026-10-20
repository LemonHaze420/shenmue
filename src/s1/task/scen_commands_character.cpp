#include "s1/task/scen.hpp"
#include "s1/task/conv.hpp"
#include "logging.hpp"

#include "s1/archive_data.hpp"
#include "s1/boot.hpp"
#include "s1/audio_stream.hpp"
#include "s1/modules/arcade.hpp"
#include "s1/cycle_stack.hpp"
#include "s1/collision.hpp"
#include "s1/chrd.hpp"
#include "s1/task/char.hpp"
#include "s1/task/gcpn.hpp"
#include "s1/game_utils.hpp"
#include "s1/task/came.hpp"
#include "s1/task/cach.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/time.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/file_loader.hpp"
#include "s1/face.hpp"
#include "s1/task/film.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/task/effe.hpp"
#include "s1/initialization.hpp"
#include "s1/lighting.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/input.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/atck.hpp"
#include "s1/task/cuff.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/dswk.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/edtk.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/obje.hpp"
#include "s1/motion_registry.hpp"
#include "s1/motion.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/gull.hpp"
#include "s1/task/hato.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/lamp.hpp"
#include "s1/task/lght.hpp"
#include "s1/task/mitk.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/modu.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/ppad.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/qte.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/save_format.hpp"
#include "s1/scene.hpp"
#include "s1/task/scrl.hpp"
#include "s1/task/save.hpp"
#include "s1/task/kdan.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/sond.hpp"
#include "s1/subtitles.hpp"
#include "s1/timing.hpp"
#include "s1/task/tmex.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/updb.hpp"
#include "s1/task/wind.hpp"
#include "s1/task/wexe.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/trwv.hpp"
#include "s1/task/telm.hpp"
#include "s1/texture_cache.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/texture_loader.hpp"

#include <algorithm>

#include <array>
#include <bit>
#include <cctype>
#include <cstddef>
#include <cmath>
#include <numbers>
#include <cstdint>
#include <limits>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <utility>

#include "s1/task/scen_internal.hpp"

namespace shenmue::s1 {
using namespace scen_detail;

namespace scen_detail {

void Script_scenCheckCharaExist(ScenarioContext *context, void *rawParams) {
    const std::uint32_t characterId = *static_cast<const std::uint32_t *>(rawParams);
    context->stack->accumulator = characterId && CHID_Find(context->task->task, characterId) ? 0xffffffffU : 0;
}

void Script_AddCharacterHandler(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const AddCharacterHandlerParameters *>(rawParams);
    if (!context || !context->stack || !context->task || !params)
        return;
    Character *const character = AddCharacterHandler(static_cast<CharacterHandlerPriority>(params->characterType));
    if (!character) {
        context->stack->accumulator = 0;
        return;
    }

    const shen_name id = params->characterId.name ? params->characterId : NextGeneratedCharacterId(context->task->task);
    CHID_Init(CurrentScenarioTask(), id.name, character);
    IMGM_Alloc(character);
    if (const char *const modelName = ScenarioPointer<const char>(params->modelNameOffset))
        IMGM_Create(character, modelName);
    context->stack->accumulator = id.name;
}
void Script_fixoSetAttachmentOffset(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FixedObjectAttachmentOffsetParameters *>(rawParams);
    if (!context || !context->task || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    if (params->useModelNode) {
        fixoSetAttachmentOffsetFromNode(character, static_cast<std::int16_t>(params->positionOffsetOrNodeKind));
        return;
    }
    fixoSetAttachmentOffset(
        character, ScenarioPointer<const Vector3>(params->positionOffsetOrNodeKind), ScenarioPointer<const Vector3i>(params->rotationOffset));
}

void Script_ToggleCPCTForChara(ScenarioContext *context, void *rawParams) {
    if (!context || !rawParams)
        return;
    const std::uint32_t characterId = *static_cast<const std::uint32_t *>(rawParams);
    Character *const character = characterId ? CHID_Find(context->task->task, characterId) : nullptr;

    CPCT_Toggle(character);
    context->stack->accumulator = 1;
}

void Script_GetCPCTFlags(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const std::int32_t mode = *static_cast<const std::int32_t *>(rawParams);
    constexpr std::uint16_t kScriptToggle = 0x0080U;
    if (!g_pControlPadController) {
        context->stack->accumulator = 0;
        return;
    }
    if (mode == 0) {
        context->stack->accumulator = (g_pControlPadController->flags & kScriptToggle) != 0 ? 0xffffffffU : 0U;
    } else if (mode == 1) {
        g_pControlPadController->flags |= kScriptToggle;
        context->stack->accumulator = 0xffffffffU;
    } else {
        g_pControlPadController->flags &= ~kScriptToggle;
        context->stack->accumulator = 0;
    }
}

void Script_EVT_SetPosition(ScenarioContext *context, void *rawParams) {
    constexpr std::uint32_t kRecursive = 0x40000000U;
    constexpr std::uint32_t kPositionX = 0x20000000U;
    constexpr std::uint32_t kPositionY = 0x10000000U;
    constexpr std::uint32_t kPositionZ = 0x08000000U;

    const auto *const params = static_cast<const EventSetPositionParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;

    const Vector3 *const source = ScenarioPointer<const Vector3>(params->positionOffset);
    const std::uint32_t flags = static_cast<std::uint32_t>(params->flags);
    Vector3 position = (flags & kRecursive) != 0 ? GetPositionForEntity(character->entity.node) : character->entity.node.position;
    const bool relative = params->flags < 0;
    if ((flags & kPositionX) != 0)
        position.x = relative ? position.x + source->x : source->x;
    if ((flags & kPositionY) != 0)
        position.y = relative ? position.y + source->y : source->y;
    if ((flags & kPositionZ) != 0)
        position.z = relative ? position.z + source->z : source->z;
    if ((flags & kRecursive) != 0)
        shenApplyTranslationToAll(character->entity.node, position);
    else
        character->entity.node.position = position;
    character->skipRootMotionDelta = 1;
}
void Script_motnGetEffectorPos(Character *character, std::int32_t nodeId, Vector3 *output) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    const MotionNodeRuntime *const row = manager ? MOTN_FindNode(static_cast<MotionNodeRuntime *>(manager->motion.activeNodes), nodeId) : nullptr;
    *output = row ? Vector3{row->effectorPosition.x, row->effectorPosition.y, row->effectorPosition.z} : Vector3{};
}

void Script_motnGetLocalPos(Character *character, std::int32_t nodeId, Vector3 *output) {
    MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr;
    const MotionNodeRuntime *const row = manager ? MOTN_FindNode(static_cast<MotionNodeRuntime *>(manager->motion.previousNodes), nodeId) : nullptr;
    if (!row) {
        *output = {};
        return;
    }
    *output = WorldToEntityLocalPoint(character->entity.node, {row->transform->rows[3].x, row->transform->rows[3].y, row->transform->rows[3].z});
}

void Script_ApplyMatrixTransformationWithRotation(Character *character, std::int32_t nodeId, Vector3 *output) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    const MotionNodeRuntime *const row = manager ? MOTN_FindNode(static_cast<MotionNodeRuntime *>(manager->motion.activeNodes), nodeId) : nullptr;
    if (!row) {
        *output = character ? character->entity.node.position : Vector3{};
        return;
    }
    *output = {row->effectorPosition.x, row->effectorPosition.y, row->effectorPosition.z};
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    const Vector3i rotation = character ? character->entity.node.rotation : Vector3i{};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    if (static_cast<std::uint16_t>(rotation.z))
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    if (static_cast<std::uint16_t>(rotation.y))
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    if (static_cast<std::uint16_t>(rotation.x))
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    Matrix4x4::TransformPoint(*output, *output);
    Matrix4x4::Pop(1);
}

void Script_EVT_GetPosition(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const EventGetPositionParameters *>(rawParams);
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    const std::uint32_t flags = params->flags;
    const std::int32_t nodeId = params->nodeId;
    Vector3 *const output = ScenarioPointer<Vector3>(params->positionOffset);
    const bool effector = (flags & 0x800000U) != 0;
    if ((flags & 0x40000000U) != 0) {
        if (nodeId == -1)
            *output = character ? GetPositionForEntity(character->entity.node) : Vector3{};
        else if (effector)
            *output = motGetEntityPos_Or_RotateEntity(character, nodeId);
        else
            mtwkGetCharaBonePosition(character, nodeId, output);
        return;
    }
    if (nodeId == -1) {
        *output = character ? character->entity.node.position : Vector3{};
        return;
    }
    if ((flags & 0x1000000U) != 0) {
        if (effector)
            Script_motnGetEffectorPos(character, nodeId, output);
        else
            Script_motnGetLocalPos(character, nodeId, output);
    } else if (effector) {
        Script_ApplyMatrixTransformationWithRotation(character, nodeId, output);
    } else {
        charaGetPosForNode(character, nodeId, output);
    }
}

void Script_mtwkGetSetRotation(ScenarioContext *context, void *rawParams) {
    constexpr std::uint32_t kRecursive = 0x40000000U;
    constexpr std::uint32_t kYawOnly = 0x02000000U;

    const auto *const params = static_cast<const GetRotationParameters *>(rawParams);
    if (!context || !context->task || !params)
        return;
    Vector3i *const output = ScenarioPointer<Vector3i>(params->rotationOffset);
    if (!output)
        return;

    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    if ((params->mode & kYawOnly) != 0) {
        output->x = 0;
        output->y =
            static_cast<std::uint16_t>((params->mode & kRecursive) != 0 ? charaGetWorldYaw(character) : static_cast<std::int32_t>(cpctGetAccumYaw(character)));
        output->z = 0;
        return;
    }
    if ((params->mode & kRecursive) != 0)
        *output = character ? GetEulerAnglesForEntity(character->entity.node) : Vector3i{};
    else if (character)
        *output = character->entity.node.rotation;
    output->x = static_cast<std::uint16_t>(output->x);
    output->y = static_cast<std::uint16_t>(output->y);
    output->z = static_cast<std::uint16_t>(output->z);
}

void Script_EVT_SetRotation(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const EventSetRotationParameters *>(rawParams);
    if (!context || !context->task || !context->stack || !params || !params->characterId.name) {
        return;
    }
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;
    const Vector3i *const source = ScenarioPointer<const Vector3i>(params->rotationOffset);
    context->stack->accumulator = static_cast<std::uint32_t>(ApplyScriptEulerFlags(character, params->flags, source));
}

void Script_scenToggleCharaFlag(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params || !g_mapControlTask || !g_mapControlTask->task_data_ptr || params[1] > 0xffU) {
        return;
    }
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    std::byte &byte = control->cycleMask[params[1] >> 3U];
    const auto mask = static_cast<std::byte>(1U << (params[1] & 7U));
    if (params[2])
        byte |= mask;
    else
        byte &= ~mask;
}

void Script_PlayCharaMotion(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const PlayCharacterMotionParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name) {
        return;
    }
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    MotionInformationWork &motion = manager->motion;
    motion.motionFlags &= ~2U;
    motion.pendingAnimId = params->animationId;
    motion.playbackRate = params->playbackRate;
    motion.scriptStartFrame = params->frameRange.x > 0.0f ? params->frameRange.x : 0.0f;
    if (params->frameRange.y > 0.0f) {
        motion.scriptEndFrame = params->frameRange.y;
    } else {
        if (params->frameRange.y == 0.0f)
            motion.scriptStartFrame = 0.0f;
        motion.scriptEndFrame = 0.0f;
    }
    motion.forcedFrameTime = params->frameRange.z > 0.0f ? params->frameRange.z : -1.0f;
}

void Script_IsCharaMotionFinished(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !context->stack || !rawParams)
        return;
    const shen_name characterId = *static_cast<const shen_name *>(rawParams);
    Character *const character = characterId.name ? CHID_Find(context->task->task, characterId.name) : nullptr;
    const MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        context->stack->accumulator = 0;
        return;
    }
    context->stack->accumulator = manager->motion.motionFlags & 2U;
}

void Script_GetCharaMotionRemainingFramesScaled(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !context->stack || !rawParams)
        return;
    const shen_name characterId = *static_cast<const shen_name *>(rawParams);
    Character *const character = characterId.name ? CHID_Find(context->task->task, characterId.name) : nullptr;
    const MotionManagerWork *const manager = MOTM_GetWork(character);
    context->stack->accumulator = 0;
    if (!character)
        return;
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    context->stack->accumulator = std::bit_cast<std::uint32_t>(MOTI_GetPlayRateScaledRemain(manager->motion));
}

void Script_SetCharaMotionPlaybackRate(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const MotionPlaybackRateParameters *>(rawParams);
    if (!context || !context->task || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    manager->motion.playbackRate = params->playbackRate;
    manager->motion.secondaryPlaybackRate = params->playbackRate;
}

void Script_tmnmUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const TmnmParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;
    TMNM_SetParameters(character, params->animationId, params->startFrame, params->endFrame, params->currentFrame, params->frameStep, params->flags);
}

void Script_tmnmCheckState(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !context->stack || !rawParams)
        return;
    const std::uint32_t characterId = *static_cast<const std::uint32_t *>(rawParams);
    Character *const character = characterId ? CHID_Find(context->task->task, characterId) : nullptr;
    context->stack->accumulator = TMNM_HasEnded(character) ? 1U : 0U;
}

void Script_tmnmReset(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !rawParams)
        return;
    const std::uint32_t characterId = *static_cast<const std::uint32_t *>(rawParams);
    if (characterId)
        TMNM_Reset(CHID_Find(context->task->task, characterId));
}
void Script_LineWalkUtils(ScenarioContext *context, void *rawParams) {
    constexpr std::uint32_t kBeginPathWalk = 0x4c570000U;
    constexpr std::uint32_t kSetMoveScaleGetCommand = 0x4c570001U;
    constexpr std::uint32_t kGetStatus = 0x4c570002U;
    constexpr std::uint32_t kPatchPathNode = 0x4c570003U;
    constexpr std::uint32_t kSetYawBase = 0x4c570004U;
    const auto *const params = static_cast<const LineWalkParameters *>(rawParams);
    if (!context || !context->task || !context->stack || !params)
        return;

    auto findCharacter = [&]() -> Character * {
        return params->characterOrPathOffset ? CHID_Find(context->task->task, static_cast<std::uint32_t>(params->characterOrPathOffset)) : nullptr;
    };
    switch (params->mode) {
    case kBeginPathWalk:
        LNWK_BeginPath(findCharacter(), g_pCurrentTaskData, params->value, static_cast<std::int32_t>(params->value2), params->value3);
        break;
    case kSetMoveScaleGetCommand: {
        std::int32_t command = 0;
        if (LNWK_SetMoveScaleAndGetPathCommand(findCharacter(), std::bit_cast<float>(static_cast<std::uint32_t>(params->value)), command))
            context->stack->accumulator = static_cast<std::uint32_t>(command);
        break;
    }
    case kGetStatus:
        context->stack->accumulator = static_cast<std::uint32_t>(LNWK_GetStatus(findCharacter()));
        break;
    case kPatchPathNode:
        LNWK_PatchPathNode(g_pCurrentTaskData, params->characterOrPathOffset, params->value, params->value2, params->value3, params->value4, params->value5);
        break;
    case kSetYawBase:
        LNWK_SetYawBase(findCharacter(), params->value);
        break;
    default:
        break;
    }
}

void Script_hmdlApplyScale(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterScaleParameters *>(rawParams);
    if (!context || !params || !params->characterId || !params->scaleOffset) {
        return;
    }

    Character *const character = CHID_Find(context->task->task, params->characterId);
    if (!character || HLib_GetCharacterComponent(character, SHEN_NAME(MOTM))) {
        return;
    }

    const Vector3 *const scale = ScenarioPointer<const Vector3>(params->scaleOffset);
    if (scale)
        character->scale = *scale;
}

void Script_EVT_ryoToggleFreeState(ScenarioContext *, void *rawParams) {
    if (*static_cast<const std::uint32_t *>(rawParams))
        Shenmue_nPauseState &= ~0x20;
}

void Script_SetCharaVisibility(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterVisibilityParameters *>(rawParams);
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    switch (params->mode) {
    case 0:
        context->stack->accumulator = character && (character->motionFlags & 4) ? 0xffffffffU : 0;
        break;
    case 1:
        if (character) {
            const bool wasVisible = (character->motionFlags & 4U) != 0;
            character->motionFlags |= 4U;
            if (g_bDebugLogVisibility && (character->flags.value & 0x400U) != 0 && !wasVisible) {
                CHAR_LogNPCVisibility(character, true);
            }
        }
        context->stack->accumulator = 0xffffffffU;
        break;
    case 2:
        if (character) {
            const bool wasVisible = (character->motionFlags & 4U) != 0;
            character->motionFlags &= static_cast<std::uint8_t>(~4U);
            if (g_bDebugLogVisibility && (character->flags.value & 0x400U) != 0 && wasVisible) {
                CHAR_LogNPCVisibility(character, false);
            }
        }
        context->stack->accumulator = 0;
        break;
    default:
        break;
    }
}

void Script_SetCharaFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterFlagsParameters *>(rawParams);
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;

    if (character) {
        if (params->mode == 100) {
            switch (params->value) {
            case 0:
                if (params->extra) {
                    const auto *const position = reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + params->extra);
                    character->shadowPosition = *position;
                }
                break;
            case 1:
                if (params->extra) {
                    character->shadowScale = *reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + params->extra);
                }
                break;
            case 2:
                character->shadowYaw = static_cast<std::uint32_t>(params->extra);
                break;
            default:
                break;
            }
        } else if (params->mode == 101) {
            if (params->value)
                character->flags.value |= 0x100U;
            else
                character->flags.value &= ~0x100U;
            if (params->value)
                character->shadowHeight = std::bit_cast<float>(static_cast<std::uint32_t>(params->extra));
        } else {
            character->shadowMode = params->mode;
        }
    }
    context->stack->accumulator = character && character->shadowMode == 0 ? 0U : 0xffffffffU;
}

void Script_charaToggleMotEnabled(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params[0])
        return;
    Character *const character = CHID_Find(context->task->task, params[0]);
    if (params[1])
        motmEnableFlags(character, 1);
    else
        motmDisableFlags(character, 1);
}

void Script_SetMotnFlag(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::int32_t *>(rawParams);
    if (!params[0])
        return;
    Character *const character = CHID_Find(context->task->task, static_cast<std::uint32_t>(params[0]));
    motmSetBasePoseMode(character, params[1]);
}

void Script_SetMotnFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const MotnTargetParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    MOTM_SetTarget(character, params->nodeId, ScenarioPointer<const Vector3>(params->targetOffset), params->flags);
}

void Script_SetCharaLookAtPosition(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const LookPointParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name) {
        return;
    }
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    SetCharaLookAtPosition(character, params->angle, ScenarioPointer<const Vector3>(params->targetOffset), params->mode);
}

void Script_GetSetMotionScale(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const auto *const params = static_cast<const MotionScaleParameters *>(rawParams);
    context->stack->accumulator = std::bit_cast<std::uint32_t>(1.0f);
    Character *const character = params->characterId.name && context->task ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    if (params->mode == 1)
        manager->motion.currentMotionScale = params->value;
    else if (params->mode == 3)
        manager->motion.targetMotionScale = params->value;
    if (static_cast<std::uint32_t>(params->mode) <= 1U)
        context->stack->accumulator = std::bit_cast<std::uint32_t>(manager->motion.currentMotionScale);
    else if (static_cast<std::uint32_t>(params->mode) <= 3U)
        context->stack->accumulator = std::bit_cast<std::uint32_t>(manager->motion.targetMotionScale);
}

void Script_SetupTransformMatrixForMotionTask(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const MotionTransformParameters *>(rawParams);
    if (!context || !context->task || !params || !params->targetCharacterId.name) {
        return;
    }
    Character *const target = CHID_Find(context->task->task, params->targetCharacterId.name);
    if (!target)
        return;
    Character *const source = params->sourceCharacterId.name ? CHID_Find(context->task->task, params->sourceCharacterId.name) : nullptr;
    SetupTransformMatrixForMotionTask(target, params->targetAnchorNodeId, source, params->sourceModelNodeKind,
        ScenarioPointer<Vector3>(params->translationOffset), ScenarioPointer<Vector3i>(params->eulerOffset));
}

void Script_SetCharaRefBoardPos(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = params[0] ? CHID_Find(context->task->task, params[0]) : nullptr;
    const Vector4 *const colour = ScenarioPointer<Vector4>(params[1]);
    if (character && colour)
        refbSetColour(character, {colour->y, colour->z, colour->w});
}

void Script_charaSetRefboardFlagTo(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = params[0] ? CHID_Find(context->task->task, params[0]) : nullptr;
    if (character)
        refbSetSector(character, params[1]);
}

void Script_facemotUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FaceMotionUtilityParameters *>(rawParams);
    std::int8_t result = -1;
    if (!context || !params || !params->characterId.name) {
        if (context && context->stack)
            context->stack->accumulator = 0xffffffffU;
        return;
    }

    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (character) {
        if (params->mode == 0 && HLib_GetCharacterComponent(character, SHEN_NAME(MOTM)) && HLib_GetCharacterComponent(character, SHEN_NAME(FACE))) {
            result = static_cast<std::int8_t>(FACE_ScriptFaceMotion(character, 0));
        } else if (params->mode == 1) {
            result = static_cast<std::int8_t>(HND_LoadModels(character));
            if (result < 1)
                result = -1;
        }
    }
    context->stack->accumulator = static_cast<std::uint32_t>(static_cast<std::int32_t>(result));
}

void Script_FaceEx(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FaceExpressionParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;

    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character || !MOTM_GetWork(character))
        return;
    FACE_UpdateExpression(
        character, static_cast<std::uint8_t>(params->eyeMode), static_cast<std::int16_t>(params->eyePatternId), static_cast<std::int16_t>(params->timing));
}

void Script_FaceLookAtTargetUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FaceLookAtTargetParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;

    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;
    if (!HLib_GetCharacterComponent(character, SHEN_NAME(FACE))) {
        return;
    }
    constexpr Vector3 kZero{};
    switch (params->mode) {
    case 0:
        FACE_SetLookTarget(character, 0, &kZero, 16);
        break;
    case 1:
        FACE_SetLookTarget(character, 1, &kZero, static_cast<std::uint16_t>(params->targetOffset));
        break;
    case 2:
        FACE_SetLookTarget(character, 2, ScenarioPointer<const Vector3>(params->targetOffset), params->blendFrames);
        break;
    default:
        break;
    }
}

void Script_CharHandlers(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterHandParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;

    switch (params->mode) {
    case -1:
        mhndStartIdleHandProgram(character);
        break;
    case -2:
        mhndStartAutomaticHandProgram(character);
        break;
    case -3:
        context->stack->accumulator = static_cast<std::uint32_t>(mhndGetPoseStatus(character, params->poseIndex));
        break;
    default:
        mhndApplyCharaHandPose(character, params->mode, params->poseIndex, params->frameCount);
        break;
    }
}

void Script_clmdSetFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ClothModelFlagsParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    if (Character *const character = CHID_Find(context->task->task, params->characterId.name))
        CLMD_SetFlags(character, params->value, params->operation);
}

void Script_LoadHandMot(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const MotionLoadParameters *>(rawParams);
    if (!context || !params)
        return;
    const char *const filename = ScenarioPointer<const char>(params->filenameOffset);
    const std::uint32_t handle = LoadHandMot(params->sceneId, params->areaId.name, filename);
    context->stack->accumulator = handle;
}
void Script_handGetNamed(ScenarioContext *context, void *rawParams) {
    static constexpr std::array<std::uint16_t, 19> kNamedIndices{{
        52,
        51,
        50,
        56,
        55,
        54,
        53,
        60,
        59,
        58,
        57,
        64,
        63,
        62,
        61,
        68,
        67,
        66,
        65,
    }};
    const auto *const params = static_cast<const HandNamedParameters *>(rawParams);
    if (!params || params->namedIndex >= kNamedIndices.size())
        return;
    HandModelWork *const hand = FindScenarioHand(context, params->characterId, params->side);
    Vector3i *const output = ScenarioPointer<Vector3i>(params->outputOffset);
    if (!hand || !output)
        return;
    std::memcpy(output, hand->jointPositions + kNamedIndices[params->namedIndex] * sizeof(Vector3i), sizeof(*output));
}

void Script_handSetFloatsFor(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const HandFloatsParameters *>(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = FindScenarioHand(context, params->characterId, params->side);
    if (!hand)
        return;
    HND_SetFloatsFor(*hand, ScenarioPointer<const std::int32_t>(params->anglesOffset), static_cast<std::int16_t>(params->blendFrames));
}

void Script_ConfigHandMotionIK(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const HandControlParameters *>(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = FindScenarioHand(context, params->characterId, params->side);
    if (!hand)
        return;
    if (params->mode == 0) {
        HND_SetControlFlags(*hand, static_cast<std::uint16_t>(params->value), params->mask);
    } else if (params->mode == 1) {
        const float value = std::bit_cast<float>(params->value);
        if (value > 0.0f)
            hand->frameStep = value;
    }
}

void Script_SetHandRootRotation(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const HandRootRotationParameters *>(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = FindScenarioHand(context, params->characterId, params->side);
    if (!hand)
        return;
    HND_SetRootRotation(*hand, params->mask, ScenarioPointer<const Vector3i>(params->rotationOffset));
}

void Script_SetOsageFlags(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const OsageFlagsParameters *>(rawParams);
    if (!params)
        return;
    switch (params->mode) {
    case 0:
    case 1:
        g_OSAG_Val_1[params->mode] = static_cast<std::int32_t>(params->value);
        break;
    case 2:
        g_OSAG_Val_2 = std::bit_cast<float>(params->value);
        break;
    case 3:
        g_OSAG_Val = std::bit_cast<float>(params->value);
        break;
    case 10:
    case 11:
    case 20:
    case 21:
    case 30:
    case 31:
        g_OSAG_Val_0[3 * (params->mode % 10) + params->mode / 10 - 1] = static_cast<std::int32_t>(params->value);
        break;
    default:
        break;
    }
}

void Script_ResetHandMotion(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const HandMotionResetParameters *>(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = FindScenarioHand(context, params->characterId, params->side);
    if (!hand)
        return;
    HND_ResetMotion(*hand, params->status, params->sequenceId);
    if (hand->primaryRoot)
        hand->primaryRoot->rotation = {};
}

void Script_SetNewHandMotion(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const HandMotionSetParameters *>(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = FindScenarioHand(context, params->characterId, params->side);
    if (!hand)
        return;
    HND_SetNewData(*hand, params->sequenceId, params->startFrame, params->endFrame, params->initialFrame, static_cast<std::uint8_t>(params->control),
        params->frameStep, params->blendFrames);
}

void Script_fixoClearAttachment(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !rawParams)
        return;
    const auto characterId = *static_cast<const shen_name *>(rawParams);
    Character *const character = characterId.name ? CHID_Find(context->task->task, characterId.name) : nullptr;
    fixoClearAttachment(character);
}

void Script_fixoAttachEx(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FixedObjectAttachParameters *>(rawParams);
    if (!context || !context->task || !params)
        return;
    Character *const target = params->targetCharacterId.name ? CHID_Find(context->task->task, params->targetCharacterId.name) : nullptr;
    Character *const owner = params->ownerCharacterId.name ? CHID_Find(context->task->task, params->ownerCharacterId.name) : nullptr;
    fixoSetCharaPos(
        target, owner, params->nodeId, ScenarioPointer<const Vector3>(params->positionOffset), ScenarioPointer<const Vector3i>(params->rotationOffset));
}

void Script_camSetRotationStepSize(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    CAME_SetRotationStepLock(arguments.word(0));
}

void Script_cpctGetCurrControlMode(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(static_cast<std::uint32_t>(PCCTL_GetForcedControlMode()));
}

void Script_charaSetClipAngle(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const float value = arguments.scalar(1);
    if (!(value < 0.0f)) {
        if (character)
            character->clipAngle = value;
    } else {
        arguments.result(character ? character->clipAngle : 0.0f);
    }
}

void Script_GetMotionFlag200(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const MotionManagerWork *const manager = MOTM_GetWork(character);
    if (character && !manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
    }
    arguments.result(manager ? manager->motion.motionFlags & 0x200U : 1U);
}

void Script_HasMotionRootAngleFlags2003(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const MotionManagerWork *const manager = MOTM_GetWork(character);
    if (character && !manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
    }
    arguments.result(manager && (manager->motion.rootAngleFlags & 0x2003U) == 0x2003U ? 1U : 0U);
}

void Script_charaHasMotionwork(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t identifier = arguments.word(0);
    Character *const character = identifier ? ScenarioCharacter(context, identifier) : nullptr;
    arguments.result(MOTM_GetWork(character) ? 0xffffffffU : 0U);
}

void Script_freeTalkGetCurrentChara(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(SCNT_GetFreeTalkCurrentCharacter());
}

void Script_charaVisUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const bool previouslyVisible = character && (character->flags.value & CharacterFlagWithinVisibleDistance) != 0;
    arguments.result(previouslyVisible ? 0xffffffffU : 0U);
    if (character) {
        if (arguments.word(1) == 1)
            character->flags.value |= CharacterFlagWithinVisibleDistance;
        else if (arguments.word(1) == 0)
            character->flags.value &= ~CharacterFlagWithinVisibleDistance;
    }
}

void Script_SetMaxCharaSearchExtent(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    SRCH_SetCharacterSearchExtent(arguments.word(0) != 0, arguments.scalar(1));
}

void Script_LoadCharacterDefinition(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t token = arguments.word(0);
    std::uint32_t size = 0;
    const std::byte *const payload = MAPINFO_FindChunkPayload(token, &size);
    CHRD_InitMapInfoChunk(const_cast<std::byte *>(payload), size, g_entryPoint, static_cast<std::int32_t>(token));
}

void Script_SetRyoMotionDetails(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    MOTI_BeginSecondaryBlend(CHID_Find(context->task->task, arguments.word(0)));
}

void Script_SetBaseMotionScale(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = CHID_Find(context->task->task, arguments.word(0));
    if (!character)
        return;
    if (!MOTM_GetWork(character)) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    MOTI_SetBaseMotionScale(character, arguments.scalar(1));
}

void Script_scenCharaGetMotAnimId(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(0U);
    Character *const character = CHID_Find(context->task->task, arguments.word(0));
    if (!character)
        return;
    if (!MOTM_GetWork(character)) {
        SCEN_PromoteCharacterIndexNode(context->task->task, character);
        return;
    }
    arguments.result(static_cast<std::uint32_t>(MOTI_GetCurrentAnimationId(character)));
}

void Script_SetMotnFlag_0(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    MOTI_SetTrackedNode(CHID_Find(context->task->task, arguments.word(0)), arguments.signedWord(1));
}

void Script_GetChidPositionAndRotation(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    std::array<std::uint32_t, 3> position{};
    std::array<std::uint32_t, 3> rotation{};
    if (!MAPCTRL_GetCharacterIdTransform(arguments.word(0), position, rotation)) {
        return;
    }
    if (std::uint32_t *const output = arguments.pointer<std::uint32_t>(1))
        std::copy(position.begin(), position.end(), output);
    if (std::uint32_t *const output = arguments.pointer<std::uint32_t>(2))
        std::copy(rotation.begin(), rotation.end(), output);
}

void Script_charaSetRenderOpacity(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    SetCharacterRenderOpacity(CHID_Find(context->task->task, arguments.word(0)), arguments.scalar(1));
}

void Script_cpctFlagUtil(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    PCCTL_SetScriptWalkMode(arguments.word(0));
}

void Script_GetHandControlBit3(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    shen_name characterId{};
    characterId.name = arguments.word(0);
    HandModelWork *const hand = FindScenarioHand(context, characterId, arguments.signedWord(1));
    if (hand) {
        arguments.result((static_cast<std::uint32_t>(hand->control) >> 3U) & 1U);
    }
}

void Script_handSetFor(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    shen_name characterId{};
    characterId.name = arguments.word(0);
    HandModelWork *const hand = FindScenarioHand(context, characterId, arguments.signedWord(1));
    Vector3i *const output = arguments.pointer<Vector3i>(2);
    if (!hand || !output)
        return;
    *output = hand->primaryRoot ? hand->primaryRoot->rotation : Vector3i{};
}

void Script_faceGetFilename(ScenarioContext *, void *) {
    FACE_ResetCache();
}

void Script_CalculateMatrixRotationFromParams(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const target = ScenarioCharacter(context, arguments.word(0));
    if (!target)
        return;
    Character *const source = ScenarioCharacter(context, arguments.word(1));
    CalculateMatrixRotationFromParamsHelper(target, source, arguments.signedWord(2));
    FixedObjectWork *const work = FindFixedObjectWork(target);
    const Vector3 *const position = arguments.pointer<Vector3>(3);
    const Vector3i *const rotation = arguments.pointer<Vector3i>(4);
    if (!work || !position || !rotation)
        return;
    work->position = {
        work->position.x - work->attachmentPosition.x, work->position.y - work->attachmentPosition.y, work->position.z - work->attachmentPosition.z};
    work->attachmentPosition = *position;
    work->position = {
        work->position.x + work->attachmentPosition.x, work->position.y + work->attachmentPosition.y, work->position.z + work->attachmentPosition.z};
    work->rotation = {
        work->rotation.x - work->attachmentRotation.x, work->rotation.y - work->attachmentRotation.y, work->rotation.z - work->attachmentRotation.z};
    work->attachmentRotation = *rotation;
    work->rotation = {
        work->rotation.x + work->attachmentRotation.x, work->rotation.y + work->attachmentRotation.y, work->rotation.z + work->attachmentRotation.z};
}

void Script_faceSetGeneratedLipCues(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, static_cast<std::uint32_t>(Clipper_GetGlobalFlag()));
    static_cast<void>(faceSetGeneratedLipCues(character, arguments.pointer<FaceLipCue>(0), true));
}

void Script_CEWPRelated(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const Character *const character = ScenarioCharacter(context, arguments.word(0));
    CharacterNode *const child = character ? character->entity.node.child : nullptr;
    if (!child) {
        arguments.result(0xffffffffU);
        return;
    }

    auto *const control = g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (control) {
        CharacterIdTransform *const begin = control->characterIdTable.data();
        CharacterIdTransform *const end = begin + control->characterIdTable.size();
        const auto childAddress = reinterpret_cast<std::uintptr_t>(child);
        const auto beginAddress = reinterpret_cast<std::uintptr_t>(begin);
        const auto endAddress = reinterpret_cast<std::uintptr_t>(end);
        if (childAddress >= beginAddress && childAddress < endAddress) {
            arguments.result(static_cast<std::uint32_t>((childAddress - beginAddress) / sizeof(CharacterIdTransform)));
            return;
        }
    }

    auto *const childCharacter = reinterpret_cast<Character *>(reinterpret_cast<std::byte *>(child) - offsetof(CharacterEntity, node));
    if (childCharacter->entity.name != SHEN_NAME(CEWP) || (childCharacter->flags.value & CharacterFlagUnknown) == 0 || !control || !control->scenarioTask ||
        !control->scenarioTask->task_data_ptr) {
        arguments.result(0xffffffffU);
        return;
    }

    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(control->scenarioTask->task_data_ptr);
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(childCharacter) % scenario->characterBuckets.size();
    CharacterIdLookupNode **link = &scenario->characterBuckets[bucket];
    while (*link && (*link)->character != childCharacter)
        link = &(*link)->bucketNext;
    CharacterIdLookupNode *const node = *link;
    if (!node) {
        arguments.result(0U);
        return;
    }
    *link = node->bucketNext;
    node->bucketNext = scenario->characterBuckets[bucket];
    scenario->characterBuckets[bucket] = node;
    arguments.result(node->id);
}

void Script_charaPosRotUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t characterId = arguments.word(0);
    if (!characterId)
        return;
    Character *const character = ScenarioCharacter(context, characterId);
    if (!character)
        return;
    Mt5RuntimeNode *const node = shenGetNodeByChildIdx(static_cast<Mt5RuntimeNode *>(character->model), static_cast<std::int16_t>(arguments.word(1)));
    if (!node)
        return;
    const std::uint32_t mode = arguments.word(4);
    if (Vector3 *const position = arguments.pointer<Vector3>(2)) {
        switch (mode) {
        case 0:
            // MFSY's CAN1 node 0x99 ignores the authored x=1000 placeholder.
            if (arguments.word(1) != 0x99 || characterId != SHEN_NAME(CAN1) || position->x <= 999.0f || position->x >= 1001.0f || g_areaId != SHEN_NAME(MFSY)) {
                node->position = *position;
            }
            break;
        case 1:
            node->position = {node->position.x + position->x, position->y + node->position.y, position->z + node->position.z};
            break;
        case 2:
            *position = node->position;
            break;
        case 3:
            node->scale = *position;
            break;
        default:
            break;
        }
    }
    if (Vector3i *const rotation = arguments.pointer<Vector3i>(3)) {
        switch (mode) {
        case 0:
            node->rotation = *rotation;
            break;
        case 1:
            node->rotation = {node->rotation.x + rotation->x, node->rotation.y + rotation->y, node->rotation.z + rotation->z};
            break;
        case 2:
            *rotation = node->rotation;
            break;
        default:
            break;
        }
    }
}

void Script_HandleLookPoint(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(0U);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        if (character) {
            MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
            SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        }
        return;
    }
    const std::int32_t angle = arguments.signedWord(1);
    const Vector3 *const target = arguments.pointer<Vector3>(2);
    const std::int32_t mode = arguments.signedWord(3);
    if (angle >= 0) {
        const std::uint32_t result = MOTI_UpdateLookPointAnimationFullBody(character, target);
        SetLookPointDetails(character, angle, target, mode);
        manager->motion.motionControlLock = true;
        arguments.result(result);
        return;
    }
    manager->motion.motionControlLock = false;
    MOTI_StopLookPointAnimation(character);
    SetLookPointDetails(character, angle, target, mode);
}

void Script_cpctStateUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    const std::uint32_t value = arguments.word(1);
    if (!mode) {
        g_scenarioControllerConnected = std::bit_cast<std::int32_t>(value);
        return;
    }
    if (mode != 2 && !g_pControlPadController)
        return;
    switch (mode) {
    case 1:
        g_pControlPadController->stateFlags = static_cast<std::int8_t>(value);
        break;
    case 2:
        if (g_playerMotion)
            g_playerMotion->walkBlendAnimId = static_cast<std::uint16_t>(value);
        break;
    case 3:
        g_pControlPadController->scriptState = static_cast<std::int8_t>(value);
        break;
    default:
        break;
    }
}

void Script_faceUpdateFor(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (!character)
        return;
    if (arguments.word(1) == 0) {
        FACE_ScriptReleaseMotion(character);
    } else if (arguments.word(1) == 1) {
        (void)HND_ReleaseModels(character);
    }
}

void Script_GetHandPosition(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const std::uint32_t componentId = arguments.word(1) ? SHEN_NAME(HNDR) : SHEN_NAME(HNDL);
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, componentId);
    const auto *const hand = component ? reinterpret_cast<const HandModelWork *>(component + 1) : nullptr;
    if (hand) {
        arguments.result(hand->animationFlags != 0U ? -static_cast<std::int32_t>(hand->blendFrames) : static_cast<std::int32_t>(hand->playback.frameIndex) + 1);
    }
}

void Script_charaSetupXForm(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    Vector3 *const output = arguments.pointer<Vector3>(2);
    if (!character || !output)
        return;
    Matrix4x4 transform;
    SetupTransformationMatrix(character, arguments.word(1), &transform);
    *output = {transform.rows[3].x, transform.rows[3].y, transform.rows[3].z};
}

void Script_SetupClothData(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (!character)
        return;
    CLMD_Setup(character, static_cast<std::uint8_t>(arguments.word(1)));
}

void Script_UpdateCharaSelectionScene(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    ScenarioSelectorState &selection = g_scenarioSelector;
    const auto *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const auto *const input = control ? static_cast<const PadCaptureEntry *>(control->padCaptureEntry) : nullptr;

    switch (selection.state) {
    case CharaSelectState::Idle: {
        const std::int32_t previousFrame = selection.frame++;
        if (previousFrame == 0) {
            for (const std::int32_t slot : kSelectionSlots)
                AdjustSelectionMarkerZ(selection, slot, 0.1);
            if ((g_searchLockOnFlags & 1U) != 0) {
                g_searchLockOnFlags &= 0xfffffff8U;
                SRCH_Reset();
            }
            g_searchAnyInputPressedLatched = 0;
            g_searchAPressedLatched = 0;
            g_previousAuthorState = 9;
            ApplyPendingGameState(9);
            const std::size_t slot = static_cast<std::size_t>(selection.selectedSlot);
            ApplySelectionCamera(selection.cameraEyeAnchors[slot], selection.cameraTargetAnchors[slot]);
            SetSelectionProxyPosition(selection, SelectionProxyPoint(selection, selection.selectedSlot, {0.02f, -0.25999999f, 0.16500001f}));
            if (selection.selected) {
                ApplyEulerAnglesToNode(selection.selected->entity.node,
                    {
                        selection.rigBaseRotation.x + 38229,
                        selection.rigBaseRotation.y + 0x8000,
                        selection.rigBaseRotation.z,
                    });
                const bool wasVisible = (selection.selected->motionFlags & 0x3fU) != 0;
                selection.selected->motionFlags |= 0x3fU;
                if (g_bDebugLogVisibility && (selection.selected->flags.value & 0x400U) != 0 && !wasVisible)
                    CHAR_LogNPCVisibility(selection.selected, true);
            }
            ResetSelectionRepeatCounters(selection);
        }

        const std::uint16_t pressed = input ? input->pressedPriority : 0;
        if (arguments.signedWord(3) != 2 && (pressed & 0x0200U) != 0) {
            selection.frame = 0;
            selection.state = CharaSelectState::Cancel;
        }
        if ((pressed & 0x0400U) != 0) {
            const bool extraRow = selection.selectedSlot >= 10;
            const Vector3 target = SelectionRigPoint(selection, 0, 0, extraRow ? Vector3{0.1f, 1.1f, 0.0f} : Vector3{0.0f, 1.4f, 0.0f});
            const Vector3 eye = SelectionRigPoint(selection, 0, 0, extraRow ? Vector3{0.1f, 1.2f, 0.625f} : Vector3{0.0f, 1.5f, 0.75999999f});
            selection.cameraEyeStepDelta = {(eye.x - g_CameraEyePos.x) / 30.0f, (eye.y - g_CameraEyePos.y) / 30.0f, (eye.z - g_CameraEyePos.z) / 30.0f};
            selection.cameraTargetStepDelta = {
                (target.x - g_CameraTargetPos.x) / 30.0f, (target.y - g_CameraTargetPos.y) / 30.0f, (target.z - g_CameraTargetPos.z) / 30.0f};
            selection.frame = 0;
            selection.state = CharaSelectState::ConfirmCameraMove;
        } else {
            HandleSelectionNavigation(selection, input);
        }
        break;
    }
    case CharaSelectState::SwitchSlot: {
        const std::int32_t previousFrame = selection.frame;
        const std::int32_t interpolationIndex = ++selection.frame;
        if (previousFrame == 0) {
            selection.transitionProxyTarget = SelectionProxyPoint(selection, selection.selectedSlot, {0.02f, -0.25999999f, 0.16500001f});
            selection.previousProxyPosition = SelectionProxyPosition(selection);
        }
        if (interpolationIndex < selection.transitionFrameCount) {
            const float factor = selection.transitionFrameCount == 5 ? kFastSelectionCurve[static_cast<std::size_t>(interpolationIndex)] :
                                                                       kSlowSelectionCurve[static_cast<std::size_t>(interpolationIndex)];
            const std::size_t selected = static_cast<std::size_t>(selection.selectedSlot);
            const std::size_t previous = static_cast<std::size_t>(selection.previousSlot);
            ApplySelectionCamera(LerpSelectionVector(selection.cameraEyeAnchors[previous], selection.cameraEyeAnchors[selected], factor),
                LerpSelectionVector(selection.cameraTargetAnchors[previous], selection.cameraTargetAnchors[selected], factor));
            SetSelectionProxyPosition(selection, LerpSelectionVector(selection.previousProxyPosition, selection.transitionProxyTarget, factor));
        } else {
            selection.state = CharaSelectState::Idle;
        }
        UpdateSelectionRepeatCounters(selection, input);
        break;
    }
    case CharaSelectState::ConfirmCameraMove: {
        const std::int32_t previousFrame = selection.frame++;
        if (previousFrame < 30) {
            ApplySelectionCamera(
                AddSelectionVector(g_CameraEyePos, selection.cameraEyeStepDelta), AddSelectionVector(g_CameraTargetPos, selection.cameraTargetStepDelta));
        } else {

            const Vector3 target = SelectionProxyPoint(selection, selection.selectedSlot, {0.0f, -0.16f, 0.064999998f});
            const HandModelWork *const hand = selection.selected ? HND_GetWork(selection.selected, false) : nullptr;
            Vector3 fingertip{};
            if (hand)
                std::memcpy(&fingertip, hand->jointPositions + 56 * sizeof(Vector3), sizeof(fingertip));
            selection.proxyStepDelta = {(target.x - fingertip.x) / 10.0f, (target.y - fingertip.y) / 10.0f, (target.z - fingertip.z) / 10.0f};
            selection.frame = 0;
            selection.state = CharaSelectState::MoveProxyToFocus;
        }
        break;
    }
    case CharaSelectState::MoveProxyToFocus: {
        const std::int32_t previousFrame = selection.frame++;
        if (previousFrame < 10) {
            SetSelectionProxyPosition(selection, AddSelectionVector(SelectionProxyPosition(selection), selection.proxyStepDelta));
        } else {
            const Vector3 base = SelectionProxyPoint(selection, selection.selectedSlot, {});
            const Vector3 forward = SelectionProxyPoint(selection, selection.selectedSlot, {0.0f, 0.0f, -0.0080000004f});
            selection.proxyStepDelta = ScaleSelectionVector(SubtractSelectionVector(forward, base), 0.5f);
            selection.frame = 0;
            selection.state = CharaSelectState::BounceForward;
        }
        break;
    }
    case CharaSelectState::BounceForward: {
        const std::int32_t previousFrame = selection.frame++;
        if (previousFrame < 2) {
            SetSelectionProxyPosition(selection, AddSelectionVector(SelectionProxyPosition(selection), selection.proxyStepDelta));
            PressSelectionSlot(selection, selection.selectedSlot, -0.004);
        } else {
            selection.proxyStepDelta = {0.0f - selection.proxyStepDelta.x, 0.0f - selection.proxyStepDelta.y, 0.0f - selection.proxyStepDelta.z};
            selection.frame = 0;
            selection.state = CharaSelectState::BounceBack;
        }
        break;
    }
    case CharaSelectState::BounceBack: {
        const std::int32_t previousFrame = selection.frame++;
        if (previousFrame < 2) {
            SetSelectionProxyPosition(selection, AddSelectionVector(SelectionProxyPosition(selection), selection.proxyStepDelta));
            PressSelectionSlot(selection, selection.selectedSlot, 0.004);
        } else {
            for (const std::int32_t slot : kSelectionSlots) {
                if (slot != selection.selectedSlot)
                    AdjustSelectionMarkerZ(selection, slot, -0.1);
            }
            selection.frame = 0;
            selection.state = CharaSelectState::PreDispatchWait;
        }
        break;
    }
    case CharaSelectState::PreDispatchWait: {
        const std::int32_t previousFrame = selection.frame++;
        if (previousFrame >= 30) {
            AdjustSelectionMarkerZ(selection, selection.selectedSlot, -0.1);
            selection.frame = 0;
            if (selection.selectedSlot >= 0 && selection.selectedSlot <= 2) {
                selection.state = CharaSelectState::DispatchSlot0To2;
            } else if (selection.selectedSlot == 3) {
                selection.state = CharaSelectState::DispatchSlot3;
            } else if (selection.selectedSlot == 4) {
                selection.state = CharaSelectState::DispatchSlot4;
            } else if (selection.selectedSlot == 5) {
                selection.state = CharaSelectState::DispatchSlot5;
            } else {
                selection.state = CharaSelectState::DispatchExtra;
            }
        }
        break;
    }
    case CharaSelectState::DispatchSlot0To2:
    case CharaSelectState::DispatchSlot4:
    case CharaSelectState::DispatchSlot3:
    case CharaSelectState::DispatchSlot5:
    case CharaSelectState::DispatchExtra:
        selection.state = CharaSelectState::Cleanup;
        break;
    case CharaSelectState::Cleanup:
    case CharaSelectState::Exit14:
    case CharaSelectState::Exit15:
    case CharaSelectState::Cancel:
        if (selection.selected) {
            const bool wasVisible = (selection.selected->motionFlags & 0x3fU) != 0;
            selection.selected->motionFlags &= 0xc0U;
            if (g_bDebugLogVisibility && (selection.selected->flags.value & 0x400U) != 0 && wasVisible)
                CHAR_LogNPCVisibility(selection.selected, false);
        }
        VEND_ReleaseModels();
        break;
    default:
        break;
    }

    arguments.result(static_cast<std::uint32_t>(selection.state));
}

void Script_scenCharaSubmitMoveTargetRequest(ScenarioContext *context, void *rawParams) {
    SubmitScenarioMove(context, rawParams, true, false, 0);
}

void Script_SetCharaMoveTarget(ScenarioContext *context, void *rawParams) {
    SubmitScenarioMove(context, rawParams, false, false, 5);
}

void Script_GetCharaMoveStatus(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(charaIsActiveMode(ScenarioCharacter(context, arguments.word(0)), 5) ? 1U : 0U);
}

void Script_KaidanUtils(ScenarioContext *context, void *rawParams) {
    struct KaidanUtilsParameters {
        std::uint32_t command;
        std::uint32_t charaId;
        std::uint32_t guideCount;
        std::int32_t guideTableOffset;
        std::int32_t eventTableOffset;
        std::uint32_t maxYawDelta;
    };
    static_assert(sizeof(KaidanUtilsParameters) == 0x18);

    const auto *const params = static_cast<const KaidanUtilsParameters *>(rawParams);
    HLib::Task *const scenarioTask = context->task->task;
    switch (params->command) {
    case 0: {
        const auto *const events = ScenarioPointer<const CameraStairEventEntry>(static_cast<std::uint32_t>(params->eventTableOffset));
        const auto *const guides = ScenarioPointer<const CameraStairGuideEntry>(static_cast<std::uint32_t>(params->guideTableOffset));
        context->stack->accumulator =
            static_cast<std::uint32_t>(CameraStair_QueryGuideCollision(scenarioTask, params->charaId, params->guideCount, guides, events, params->maxYawDelta));
        break;
    }
    case 1:
        g_fCurrMaxDistance = 0.0f;
        break;
    case 2:
        CameraStair_ApplyGuideFacingCorrection(scenarioTask, params->charaId, params->guideCount,
            ScenarioPointer<const CameraStairGuideEntry>(static_cast<std::uint32_t>(params->guideTableOffset)));
        break;
    case 3:
        context->stack->accumulator = CameraStair_BuildDynamicGuideFromSTIR(
            scenarioTask, params->charaId, ScenarioPointer<CameraStairGuideEntry>(params->guideCount), params->guideTableOffset);
        break;
    case 4:
        CameraStair_ApplyGuideFacingCorrection(scenarioTask, params->charaId, 0, ScenarioPointer<const CameraStairGuideEntry>(params->guideCount));
        break;
    default:
        break;
    }
}

void Script_ExitCharaSelector(ScenarioContext *, void *) {
    g_scenarioSelector.frame = 0;
    g_scenarioSelector.state = CharaSelectState::Exit15;
}

void Script_HandleLookPointAnimationUpdate(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    arguments.result(0U);
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    if (arguments.signedWord(1) < 0) {
        MOTI_StopLookPointAnimation(character);
        manager->motion.motionControlLock = false;
        Script_SetLookPointDetails(context, rawParams);
        arguments.result(0U);
        return;
    }
    const std::uint32_t result = MOTI_UpdateLookPointAnimation(character, arguments.pointer<Vector3>(2));
    Script_SetLookPointDetails(context, rawParams);
    manager->motion.motionControlLock = true;
    arguments.result(result);
}

void Script_charaSubmitMoveRequestEx(ScenarioContext *context, void *rawParams) {
    SubmitScenarioMove(context, rawParams, true, true, 0);
}

void Script_SubmitCharaMoveTarget(ScenarioContext *context, void *rawParams) {
    SubmitScenarioMove(context, rawParams, false, true, 5);
}

void Script_GetCharaPos(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (character && arguments.word(1) <= 1U)
        charaCancelMoveRequest(character, arguments.signedWord(1));
}

void Script_SetCharaMoveTargetEx(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(6);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (!character)
        return;
    const bool usePayload = mode != 2;
    const bool useYawAnimation = mode != 1;
    charaSubmitMoveRequest(character, arguments.pointer<Vector3>(1), useYawAnimation ? arguments.signedWord(2) : 0,
        useYawAnimation ? arguments.signedWord(3) : 0, usePayload ? arguments.pointer<std::uint32_t>(4) : nullptr, arguments.scalar(5), mode);
}

void Script_SetCharaXMPTMode(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const std::uint32_t mode = arguments.word(1);
    arguments.result(character && mode <= 6 && charaIsActiveMode(character, static_cast<std::int32_t>(mode)) ? 1U : 0U);
}

void Script_KaidanSetY(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    if (Character *const character = ScenarioCharacter(context, arguments.word(0)))
        ccowSetGroundDropLimit(character, std::bit_cast<float>(arguments.word(1)));
}
} // namespace scen_detail

} // namespace shenmue::s1
