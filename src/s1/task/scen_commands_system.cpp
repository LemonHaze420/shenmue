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
#include "s1/task/mobj.hpp"
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

void Script_SetNewDateTime(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    const auto offset = *static_cast<const std::uint32_t *>(rawParams);
    GameDateTime *const input = ScenarioPointer<GameDateTime>(offset);
    if (input)
        SetNewDateTime(*input);
}

void Script_ReturnTimeDate(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    const auto offset = *static_cast<const std::uint32_t *>(rawParams);
    if (GameDateTime *const output = ScenarioPointer<GameDateTime>(offset))
        *output = g_currentDateTime;
}

void Script_scenSetMapEvTbl(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    context->task->entryOffsets = ScenarioPointer<std::uint32_t>(params[0]);
    context->task->entryCount = static_cast<std::int32_t>(params[1]);
    context->stack->accumulator = 1;
}

void Script_SetTaskDataValue(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterBoundsParameters *>(rawParams);
    if (!context || !context->stack || !context->task || !params)
        return;
    const Vector3 *const first = ScenarioPointer<const Vector3>(params->firstCornerOffset);
    const Vector3 *const second = ScenarioPointer<const Vector3>(params->secondCornerOffset);
    if (!first || !second) {
        context->stack->accumulator = 0;
        return;
    }

    const auto find = [context](shen_name id) { return id.name ? CHID_Find(context->task->task, id.name) : nullptr; };
    const Character *const anchor = find(params->anchorCharacterId);
    const float anchorX = anchor ? anchor->entity.node.position.x : 0.0f;
    const float anchorZ = anchor ? anchor->entity.node.position.z : 0.0f;
    float minimumX = anchorX + first->x, maximumX = anchorX + second->x, minimumZ = anchorZ + first->z, maximumZ = anchorZ + second->z;
    if (minimumX > maximumX) {
        const float swap = minimumX;
        minimumX = maximumX;
        maximumX = swap;
    }
    if (minimumZ > maximumZ) {
        const float swap = minimumZ;
        minimumZ = maximumZ;
        maximumZ = swap;
    }

    const Character *const target = find(params->characterId);
    const float targetX = target ? target->entity.node.position.x : 0.0f;
    const float targetZ = target ? target->entity.node.position.z : 0.0f;
    context->stack->accumulator = targetX < minimumX || maximumX < targetX || targetZ < minimumZ || maximumZ < targetZ ? 0U : 0xffffffffU;
}

void Script_CheckTargetWithinAngleRange(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const TargetAngleRangeParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    const Vector3 *const origin = ScenarioPointer<const Vector3>(params->originOffset);
    const Vector3 *const target = ScenarioPointer<const Vector3>(params->targetOffset);
    if (!origin || !target) {
        context->stack->accumulator = 0;
        return;
    }

    const float deltaX = target->x - origin->x;
    const float deltaY = target->y - origin->y;
    const float deltaZ = target->z - origin->z;
    const float distance = std::sqrt(deltaY * deltaY + deltaX * deltaX + deltaZ * deltaZ);
    const std::uint16_t direction = Shenmue_Atan2Fixed(-deltaX, -deltaZ);
    const auto signedDifference = static_cast<std::int16_t>(static_cast<std::uint16_t>(params->direction) - direction);
    const std::uint32_t difference =
        signedDifference < 0 ? static_cast<std::uint32_t>(-static_cast<std::int32_t>(signedDifference)) : static_cast<std::uint32_t>(signedDifference);
    context->stack->accumulator = !(distance > params->maximumDistance) && static_cast<std::int32_t>(difference) < params->maximumAngle ? 1U : 0U;
}
void Script_SetGameStateTo(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    const std::int32_t state = *static_cast<const std::int32_t *>(rawParams);
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_previousAuthorState = state;
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    ApplyPendingGameState(state);
}

void Script_seqSetPlayRoot(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const SequencePlayRootParameters *>(rawParams);
    if (!params)
        return;
    const Vector3 *const position = ScenarioPointer<const Vector3>(params->positionOffset);
    const Vector3i *const rotation = ScenarioPointer<const Vector3i>(params->rotationOffset);
    if (position && rotation)
        CAME_SetSequencePlayRoot(*position, *rotation);
}

void Script_GetCurrentFrameCount(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return;
    context->stack->accumulator = static_cast<std::uint32_t>(CAME_GetCurrentFrameCount());
}

void Script_AllocSpline(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    AllocSPLI_ForCamera(*static_cast<const std::int32_t *>(rawParams));
}

void Script_SetTaskExecutionSuspended(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    ScenarioStack *const stack = params[0] ? ScenarioPointer<ScenarioStack>(params[0]) : context->stack;
    auto &status = *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(stack) + 0x2a);
    if (params[1])
        status |= 2U;
    else
        status &= static_cast<std::uint16_t>(~2U);
}

void Script_shenRequestGameModuleTransition(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const GameModuleTransitionParameters *>(rawParams);
    if (!params)
        return;
    DetermineActNum();
    const std::uint32_t sign = static_cast<std::uint32_t>(params->sceneId) >> 31U;
    LoadGameModule(static_cast<SystemModule>(static_cast<std::uint32_t>(SystemModule::Map) + 3U * sign), params->sceneId, params->areaId, params->entryPoint);
}

void Script_ctrlProcessReqScene(ScenarioContext *, void *) {
    MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;

    control->flags |= 1U;
    control->module = SystemModule::Map;
    control->sceneId = g_sceneId;
    control->areaId = g_areaId;
    control->entryPoint = g_entryPoint;
    g_shouldOverrideClearColour = 1;
    if (g_shouldUseTemporaryClearColour) {
        g_overrideClearColour = (static_cast<std::uint32_t>(g_temporaryClearColour.red) << 24U) |
            (static_cast<std::uint32_t>(g_temporaryClearColour.green) << 16U) | (static_cast<std::uint32_t>(g_temporaryClearColour.blue) << 8U) |
            static_cast<std::uint32_t>(g_temporaryClearColour.alpha);
    } else {
        g_overrideClearColour = 0xff000000U;
    }
}

void Script_CreateScenarioTask(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !context->stack || !rawParams)
        return;
    const auto *const params = static_cast<const CreateScenarioTaskParameters *>(rawParams);
    const auto *const scriptFunction = ScenarioPointer<std::byte>(params->functionOffset);
    ScenarioStack *const child = scenAllocateScntForScript(context->task, context->stack, scriptFunction, params->arguments, params->argumentCount);
    context->stack->accumulator = child ? static_cast<std::uint32_t>(reinterpret_cast<std::byte *>(child) - g_pCurrentTaskData) : 0;
}

void Script_srchControl(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !context->stack || !rawParams)
        return;
    const std::uint32_t offset = *static_cast<const std::uint32_t *>(rawParams);
    ScenarioStack *const requested = offset ? reinterpret_cast<ScenarioStack *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) : context->stack;
    ScenarioStack *current = context->task->stacks;
    while (current && current != requested) {
        std::memcpy(&current, reinterpret_cast<const std::byte *>(current) + 0x20, sizeof(current));
    }
    if (!current) {
        context->stack->accumulator = 0;
        return;
    }
    auto *const status = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(current) + 0x2a);
    if ((*status & 1U) != 0) {
        context->stack->accumulator = 0;
        return;
    }
    *status |= 1U;
    context->stack->accumulator = 1;
}

void Script_SetMapFlags(ScenarioContext *, void *rawParams) {
    if (!rawParams || !g_mapControlTask || !g_mapControlTask->task_data_ptr) {
        return;
    }
    const auto *const params = static_cast<const SetMapFlagsParameters *>(rawParams);
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    const std::uint32_t mask = 1U << (params->flagIndex & 31U);
    if (params->enabled)
        control->mapRuntimeFlags |= mask;
    else
        control->mapRuntimeFlags &= ~mask;
}

void Script_StepUtils(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const auto *const params = static_cast<const StepUtilityParameters *>(rawParams);
    switch (params->mode) {
    case 0:
        context->stack->accumulator = static_cast<std::uint32_t>(g_currentScenarioStep % 10);
        break;
    case 1:
        context->stack->accumulator = static_cast<std::uint32_t>(g_currentScenarioStep / 10);
        break;
    case 2: {
        const auto requested = static_cast<std::int16_t>(params->requestedStep);
        if (g_currentScenarioStep > requested)
            RequestStep(requested);
        SCEN_SetCurrentScenarioStep(requested);
        break;
    }
    default:
        break;
    }
}

void Script_PlayerControlUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const PlayerControlParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;

    const auto setBoolResult = [context](bool value) { context->stack->accumulator = value ? 0xffffffffU : 0U; };
    const auto currentRyo = []() -> Character * {
        const auto *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        return control ? static_cast<Character *>(control->ryo) : nullptr;
    };

    switch (params->mode) {
    case 0:
        PCCTL_DisablePlayerControl();
        return;
    case 1:
        PCCTL_ResetAuthAndInputStates();
        return;
    case 2:
        setBoolResult(ShouldRun());
        return;
    case 3:
        if (g_pControlPadController)
            g_pControlPadController->forcedYawSnap = 0;
        return;
    case 4:
        if (g_pControlPadController)
            g_pControlPadController->forcedYawSnap = 1;
        return;
    case 5:
        PCCTL_SetSkipNavRegionFiltering(false);
        return;
    case 6:
        PCCTL_SetSkipNavRegionFiltering(true);
        return;
    case 7:
        PCCTL_RequestNewMode(static_cast<std::int32_t>(params->argument));
        return;
    case 8:
        setBoolResult(!g_pControlPadController || g_pControlPadController->modeState == 0);
        return;
    case 9:
        if (g_pControlPadController) {
            if (static_cast<std::uint8_t>(params->argument) == 0)
                g_pControlPadController->flags &= 0xfbffU;
            else if (static_cast<std::uint8_t>(params->argument) == 1)
                g_pControlPadController->flags |= 0x0400U;
        }
        return;
    case 10:
        if (!g_pControlPadController) {
            setBoolResult(true);
        } else {
            const PlayerControlWork &control = *g_pControlPadController;
            const bool result = ((control.flags & 2U) == 0U && control.numActiveFrames >= 120U) || control.runOverride == 4U;
            setBoolResult(result);
        }
        return;
    case 11: {
        const PlayerControlWork *const control = g_pControlPadController;
        const Character *const ryo = currentRyo();
        const MotionManagerWork *const manager = MOTM_GetWork(const_cast<Character *>(ryo));
        const bool running = control &&
            (control->numActiveFrames >= 120U || (control->numActiveFrames != 0 && control->isRunning != 0 && control->wasRunning != 0) ||
                (manager && manager->motion.interpolationAmount > 0.0f));
        setBoolResult(running);
        return;
    }
    case 12:
        if (params->argument == 1U) {
            g_CameraFadeInterpFrameCount = 30;
            playerFacingAngle = -g_fCamAccumYaw;
            g_bCameraSkipFirstInterp = 1;
        }
        return;
    case 13:
        if (g_pControlPadController) {
            if (params->argument)
                g_pControlPadController->flags &= 0xf7ffU;
            else
                g_pControlPadController->flags |= 0x0800U;
        }
        return;
    case 14: {
        Character *const character = ScenarioCharacter(context, params->argument);
        CharacterComponentNode *const cpct = HLib_GetCharacterComponent(character, SHEN_NAME(CPCT));
        if (!character || !cpct)
            return;
        reinterpret_cast<PlayerControlWork *>(cpct + 1)->footPlantingEnabled = params->argument2 ? 1U : 0U;
        if (params->argument2) {
            if (MotionManagerWork *const manager = MOTM_GetWork(character))
                manager->motion.motionCallback = reinterpret_cast<void *>(&MOTN_FootPlantingCallback);
        }
        return;
    }
    case 15: {
        CharacterComponentNode *const cpct = HLib_GetCharacterComponent(currentRyo(), SHEN_NAME(CPCT));
        const PlayerControlWork *const control = cpct ? reinterpret_cast<const PlayerControlWork *>(cpct + 1) : nullptr;
        context->stack->accumulator = control ? static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(control->tatehizaState))) : 0U;
        return;
    }
    case 16:
        if (Character *const character = ScenarioCharacter(context, params->argument))
            PCCTL_UpdateRyoNavYaw(character);
        return;
    case 17:
        if (!g_pControlPadController) {
            setBoolResult(false);
            return;
        }
        g_pControlPadController->areaId = params->argument;
        setBoolResult(g_areaId == params->argument && (g_shouldSleep & 2U) != 0);
        return;
    case 18:
        PCCTL_MoveToHome();
        return;
    default:
        return;
    }
}

void Script_uiUtils(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const UiUtilityParameters *>(rawParams);
    if (!params)
        return;
    if (!params->mode) {
        SCEN_SetHelpToken(params->helpIconId, params->buttonId);
        return;
    }

    UI_HELPDispatch(params->mode, params->helpIconId, params->buttonId);
}
void Script_SetMapControlTaskFlag(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!context || !context->task || !params || !params[0])
        return;
    Character *const character = CHID_Find(context->task->task, params[0]);
    if (!character)
        return;
    if (!params[1])
        character->flags.value |= 2U;
    character->flags.value &= ~1U;
    if (g_mapControlTask && g_mapControlTask->task_data_ptr) {
        ++reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->pendingCharacterRetirements;
    }
}
void Script_MathUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const MathUtilityParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    const float value1 = std::bit_cast<float>(params->value1);
    const float value2 = std::bit_cast<float>(params->value2);
    const auto setFloat = [context](float value) { context->stack->accumulator = std::bit_cast<std::uint32_t>(value); };
    switch (params->mode) {
    case 0:
        setFloat(value2 + value1);
        break;
    case 1:
        setFloat(value1 - value2);
        break;
    case 2:
        setFloat(value2 * value1);
        break;
    case 3:
        setFloat(value1 / value2);
        break;
    case 4:
        context->stack->accumulator = value1 > value2 ? 0xffffffffU : 0U;
        break;
    case 5:
        context->stack->accumulator = value1 < value2 ? 0xffffffffU : 0U;
        break;
    case 6:
        context->stack->accumulator = value1 >= value2 ? 0xffffffffU : 0U;
        break;
    case 7:
        context->stack->accumulator = value1 <= value2 ? 0xffffffffU : 0U;
        break;
    case 8:
        setFloat(static_cast<float>(static_cast<std::int32_t>(params->value1)));
        break;
    case 9:
        context->stack->accumulator = static_cast<std::uint32_t>(static_cast<std::int32_t>(value1));
        break;
    case 10:
        setFloat(sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(params->value1))))));
        break;
    case 11: {
        const auto angle = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(params->value1)));
        const auto low = static_cast<std::uint16_t>(angle);
        const auto folded = low <= 0x8000U ? static_cast<std::int16_t>(angle) : static_cast<std::int16_t>(-static_cast<std::int16_t>(low));
        setFloat(sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)));
        break;
    }
    case 12:
        setFloat(std::sqrt(value1));
        break;
    case 13:
        setFloat(std::fabs(value1));
        break;
    case 14:
        setFloat(std::sqrt(value1 * value1 + value2 * value2));
        break;
    case 15: {
        const Vector3 *const first = ScenarioPointer<const Vector3>(params->value1);
        const Vector3 *const second = ScenarioPointer<const Vector3>(params->value2);
        if (!first || !second)
            return;
        const float x = first->x - second->x;
        const float y = first->y - second->y;
        const float z = first->z - second->z;
        setFloat(std::sqrt(x * x + y * y + z * z));
        break;
    }
    case 16:
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        setFloat(static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) / 32768.0) * value1);
        break;
    case 17: {
        const Vector3 *const origin = ScenarioPointer<const Vector3>(params->value1);
        Vector3 *const point = ScenarioPointer<Vector3>(params->offset);
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::preTranslate(origin->x, origin->y, origin->z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(params->value2));
        Matrix4x4::TransformPoint(*point, *point);
        Matrix4x4::Pop(1);
        break;
    }
    default:
        break;
    }
}

void Script_mapEvUtils(ScenarioContext *context, void *rawParams) {
    constexpr std::uint32_t kSetEnabled = 0x4d450000U;
    const auto *const params = static_cast<const MapEventUtilsParameters *>(rawParams);
    if (!context || !context->task || !context->stack || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    if (params->mode == kSetEnabled) {
        mapEventSetEnabled(character, params->index, params->value != 0);
    } else if (params->mode == kSetEnabled + 1U) {
        context->stack->accumulator = mapEventIsEnabled(character, params->index) ? 1U : 0U;
    } else if (params->mode == kSetEnabled + 2U) {
        context->stack->accumulator = static_cast<std::uint32_t>(mapEventCurrentId(character));
    }
}

void Script_mapEvGetCurrentEvt(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !context->stack || !rawParams)
        return;
    const shen_name characterId = *static_cast<const shen_name *>(rawParams);
    Character *const character = characterId.name ? CHID_Find(context->task->task, characterId.name) : nullptr;
    context->stack->accumulator = static_cast<std::uint32_t>(mapEventCurrentId(character));
}

void Script_scenSetAssignCVWK(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const AssignCharacterEventParameters *>(rawParams);
    if (!context || !context->task || !context->stack || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    if (!character) {
        context->stack->accumulator = 0;
        return;
    }
    if (params->mode == 0)
        character->eventGroup = params->eventGroup;
    context->stack->accumulator = character->eventGroup;
}

void Script_SetOrGetWOCCValue(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterControlValueParameters *>(rawParams);
    if (!context || !context->task || !context->stack || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    context->stack->accumulator = static_cast<std::uint32_t>(ccowSetOrGetValue(character, params->value));
}

void Script_RotateNodeUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const RotateNodeParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name) {
        return;
    }
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    const Vector3 *const scale = ScenarioPointer<const Vector3>(params->scaleOffset);
    if (!scale)
        return;

    Vector3 delta = manager->motion.rootMotionDelta;
    if (params->overwrite) {
        const std::int32_t yaw = character->entity.node.rotation.y;
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::RotateAroundY_Wrapper(-static_cast<std::int16_t>(yaw));
        Matrix4x4::TransformPoint(delta, delta);
        delta.x *= scale->x;
        delta.y *= scale->y;
        delta.z *= scale->z;
        Matrix4x4::Pop(1);
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::TransformPoint(delta, delta);
        Matrix4x4::Pop(1);
    } else {
        delta.x *= scale->x;
        delta.y *= scale->y;
        delta.z *= scale->z;
    }
    if (character->entity.node.child) {
        Shenmue_RotatePointByCurrentTransform(character->entity.node.child, delta);
    }
    if (params->overwrite) {
        character->rootMotionDelta = delta;
    } else {
        character->rootMotionDelta.x += delta.x;
        character->rootMotionDelta.y += delta.y;
        character->rootMotionDelta.z += delta.z;
    }
}
void Script_ccowSetDistanceInteractLimits(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterInteractionLimitsParameters *>(rawParams);
    if (!context || !context->task || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    ccowSetDistanceInteractLimits(character, std::bit_cast<float>(params->limits.x), params->limits.y, params->limits.z);
}

void Script_get_WOCC_0(ScenarioContext *context, void *rawParams) {
    if (!context || !context->task || !context->stack || !rawParams)
        return;
    const shen_name characterId = *static_cast<const shen_name *>(rawParams);
    if (!characterId.name)
        return;
    HLib::Task *const task = context->task->task;
    ScenarioTaskData *const scenario = task && task->task_data_ptr ? reinterpret_cast<ScenarioTaskData *>(task->task_data_ptr) : nullptr;
    CharacterIdLookupNode *node = scenario ? scenario->characterIdRoot : nullptr;
    while (node && node->id != characterId.name)
        node = static_cast<std::int32_t>(characterId.name) > static_cast<std::int32_t>(node->id) ? node->left : node->right;
    if (node)
        context->stack->accumulator = ccowConsumeInteractionFlag(node->character) ? 1U : 0U;
}

void Script_GetControllerInput(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const auto *const input = control ? static_cast<const PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
    context->stack->accumulator = ReadPadCaptureMode(input, *static_cast<const std::uint32_t *>(rawParams));
}

void Script_GetInputData(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const InputDataParameters *>(rawParams);
    if (!context || !context->stack || !params || params->inputIndex < 0 || params->inputIndex >= static_cast<std::int32_t>(std::size(g_inputState.slots))) {
        return;
    }
    context->stack->accumulator = ReadInputSlotMode(static_cast<std::uint32_t>(params->inputIndex), params->inputType);
    if (g_areaId == SHEN_NAME(OP02)) {
        g_cinemaBars[2].isActive = 1;
        RefreshTriggerInputs();
    }
}

void Script_SetOwnedItemMenuFlagIfOwned(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const OwnedItemMenuParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    const auto itemId = static_cast<std::uint16_t>(params->itemId);
    if (params->mode == 0) {
        SetOwnedItemMenuInteractive(itemId, (params->value & 2U) != 0);
    } else if (params->mode == 1) {
        const bool owned = itemId <= 66U && g_playerStats.inventoryItemCounts0To66[itemId] != 0;
        SetOwnedItemMenuInteractive(itemId, owned && params->value != 0);
        context->stack->accumulator = owned ? 1U : 0U;
    }
}

void Script_GetCassetteFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const InventoryChangeParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    if (params->isCassette) {
        AddCassetteTapeCount(static_cast<std::uint8_t>(params->id), params->amount);
    } else {
        context->stack->accumulator = AddInventoryItemCount(params->id, params->amount) ? 1U : 0U;
    }
}

void Script_GetCassetteFlags_0(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const InventoryRemoveParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    if (params->isCassette) {
        RemoveCassetteTapeCount(static_cast<std::uint8_t>(params->id), params->amount);
    } else {
        context->stack->accumulator = static_cast<std::uint8_t>(RemoveInventoryItemCount(params->id, params->amount));
    }
}

void Script_GetShenmueCassette(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const InventoryQueryParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    context->stack->accumulator = params->isCassette ? GetCassetteTapeCount(static_cast<std::uint8_t>(params->id)) : GetInventoryItemCount(params->id);
}

void Script_FloatUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FloatUtilityParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    switch (params->mode) {
    case 0:
        context->stack->accumulator = params->value1 > params->value2 ? 0xffffffffU : 0U;
        break;
    case 1:
        context->stack->accumulator = params->value1 < params->value2 ? 0xffffffffU : 0U;
        break;
    case 2:
        context->stack->accumulator = params->value1 >= params->value2 ? 0xffffffffU : 0U;
        break;
    case 3:
        context->stack->accumulator = params->value1 <= params->value2 ? 0xffffffffU : 0U;
        break;
    case 4:
        context->stack->accumulator = static_cast<std::uint32_t>(static_cast<std::int32_t>(params->value1));
        break;
    case 5:
        context->stack->accumulator = Shenmue_Atan2Fixed(-params->value1, -params->value2);
        break;
    case 6: {
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        const float random = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125);
        context->stack->accumulator =
            static_cast<std::uint32_t>(static_cast<std::int32_t>(random * static_cast<float>(std::bit_cast<std::int32_t>(params->value1))));
        break;
    }
    default:
        break;
    }
}

void Script_SetActionSelectorHUDElementVisibility(ScenarioContext *, void *rawParams) {
    g_disableClockAndActionSelector = *static_cast<const std::uint8_t *>(rawParams);
}

void Script_scenToggleVisibleFor(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ScenarioCharacterVisibilityParameters *>(rawParams);
    if (!params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;

    context->stack->accumulator = character->flags.value & 4 ? 0xffffffffU : 0;
    if (params->mode == 0 || params->mode == 1) {
        SetCharacterVisibility(character, params->mode == 1);
    }
}

void Script_scenToggleActiveFor(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ScenarioCharacterVisibilityParameters *>(rawParams);
    if (!params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;
    if (params->mode)
        character->flags.value |= CharacterFlagActive;
    else
        character->flags.value &= ~CharacterFlagActive;
}

void Script_mapmSetRenderFlag(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const DynamicMapRenderFlagParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    if (params->mapIndex >= g_dynamicMapSlots.size()) {
        context->stack->accumulator = 0;
        return;
    }
    DynamicMapEntry &map = g_dynamicMapSlots[params->mapIndex];
    context->stack->accumulator = map.renderEnabled ? 0xffffffffU : 0U;
    map.renderEnabled = static_cast<std::int32_t>(params->enabled);
}

void Script_SetFightProgramStateBytes(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FightProgramStateParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    FightPoseWork *const fightProgram = FIGP_GetWork(character);
    if (!fightProgram)
        return;
    fightProgram->poseCacheColumns = params->poseCacheColumns;
    fightProgram->poseCacheRows = params->poseCacheRows;
}

void Script_PREP_Related(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const PrepParameters *>(rawParams);

    if (params->mapIndex == -1) {
        switch (params->operation) {
        case -1:
            PACK_EnsureAllMapPreparationStorage();
            return;
        case 0:
            DYNM_ClearAllMapPreparation(false);
            return;
        case 1:
            DYNM_PrepareAllMapLighting();
            return;
        default:
            DYNM_ClearAllMapPreparation(true);
            return;
        }
    }

    switch (params->operation) {
    case -1:
        PACK_EnsureMapPreparationStorage(params->mapIndex);
        return;
    case 0:
        DYNM_ClearMapPreparation(params->mapIndex, false);
        return;
    case 1:
        DYNM_PrepareMapLighting(params->mapIndex);
        return;
    default:
        DYNM_ClearMapPreparation(params->mapIndex, true);
        return;
    }
}

void Script_shenSeqUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const SequenceUtilityParameters *>(rawParams);
    if (params->mode >= 0) {
        ASEQ_ApplyCameraKeyframes(g_pSCNC);
        if (params->mode < static_cast<std::int32_t>(g_SCNCTrackSlots.size()) && g_pSCNC) {
            g_SCNCSelectedTrackIndex = params->mode;
            g_pSCNC->header.currentTrackIndex = static_cast<std::int16_t>(params->mode);
            g_pSCNC->delayLatched = 0;
            g_pSCNC->skipRequested = 0;
            SEQ_ParseInternal(g_SCNCTrackSlots[static_cast<std::size_t>(params->mode)].track);
            g_pSCNC->skipPromptTimer = 0.0f;
        }
        return;
    }
    switch (params->mode) {
    case -12:
        if (params->offset >= 0 && params->offset < 70)
            g_SCNCTrackSlots[static_cast<std::size_t>(params->offset)].enabled = params->value != 0;
        break;
    case -11:
        context->stack->accumulator = g_pSCNC && g_pSCNC->header.active == 1 ? static_cast<std::uint32_t>(g_pSCNC->frameCount) : 0xffffffffU;
        break;
    case -10:
        context->stack->accumulator = g_pSCNC && g_pSCNC->header.active == 1 ? static_cast<std::uint32_t>(g_pSCNC->skipRequested) : 0;
        break;
    case -9:
        break;
    case -8:
        if (params->offset >= 0 && params->offset < 70)
            context->stack->accumulator = g_SCNCTrackSlots[static_cast<std::size_t>(params->offset)].enabled;
        break;
    case -7:
        if (g_pSCNC)
            context->stack->accumulator = static_cast<std::uint32_t>(g_pSCNC->delayLatched);
        break;
    case -6:
        SEQ_EndActiveScenarioSubtitles();
        ASEQ_ApplyCameraKeyframes(g_pSCNC);
        break;
    case -5:
        if (g_pSCNC)
            g_pSCNC->updateMode = 0;
        break;
    case -4:
        if (g_pSCNC)
            g_pSCNC->updateMode = 1;
        break;
    case -3:
        if (g_pSCNC)
            g_pSCNC->header.currentTrackIndex = static_cast<std::int16_t>(params->offset);
        break;
    case -2:
        if (g_pSCNC)
            context->stack->accumulator = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(g_pSCNC) + 0x80U);
        break;
    case -1:
        if (g_pSCNC && g_pSCNC->header.active == 1 && g_pSCNC->sequenceCursor && *static_cast<const std::int32_t *>(g_pSCNC->sequenceCursor) == -1 &&
            g_pSCNC->frameCount < g_pSCNC->frameIndex + 1)
            ASEQ_ApplyCameraKeyframes(g_pSCNC);
        context->stack->accumulator = g_pSCNC ? g_pSCNC->header.active : 0;
        break;
    default:
        break;
    }
}

void Script_ccowUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = params[0] ? CHID_Find(context->task->task, params[0]) : nullptr;
    switch (params[1]) {
    case 1:
        ccowEnableCharaFlags(character, 7);
        context->stack->accumulator = 0;
        break;
    case 2:
        ccowDisableCharaFlags(character, 7);
        context->stack->accumulator = 0;
        break;
    case 3:
        ccowEnableCharaFlags(character, 2);
        context->stack->accumulator = 0;
        break;
    case 4:
        ccowDisableCharaFlags(character, 2);
        context->stack->accumulator = 0;
        break;
    case 5:
        ccowEnableCharaFlags(character, 4);
        context->stack->accumulator = 0;
        break;
    case 6:
        ccowDisableCharaFlags(character, 4);
        context->stack->accumulator = 0;
        break;
    case 7:
        context->stack->accumulator = ccowHasAnyCharaFlags(character, 4) ? 0xffffffffU : 0;
        break;
    case 8:
        context->stack->accumulator = ccowHasAnyCharaFlags(character, 2) ? 0xffffffffU : 0;
        break;
    case 9:
        ccowEnableCharaFlags(character, 0x2000);
        context->stack->accumulator = 0;
        break;
    case 10:
        ccowDisableCharaFlags(character, 0x2000);
        context->stack->accumulator = 0;
        break;
    default:
        context->stack->accumulator = 0;
        break;
    }
}

void Script_ccowSetFlag80(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = params[0] ? CHID_Find(context->task->task, params[0]) : nullptr;
    if (params[1] == 1)
        ccowEnableCharaFlags(character, 0x80);
    else if (params[1] == 2)
        ccowDisableCharaFlags(character, 0x80);
}

void Script_motmToggleFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params[0])
        return;
    Character *const character = CHID_Find(context->task->task, params[0]);
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    if (params[1] == 1)
        manager->motion.motionFlags |= params[2];
    else if (params[1] == 2)
        manager->motion.motionFlags &= ~params[2];
}

void Script_executeAicaCommand(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const DirectAicaCommandParameters *>(rawParams);
    if (!params)
        return;
    shenExecuteAICACommand(params->command, params->bank, params->value);
}

void Script_InitializeOsag(ScenarioContext *context, void *rawParams) {
    if (!context || !rawParams)
        return;
    const shen_name characterId{*static_cast<const std::uint32_t *>(rawParams)};
    if (Character *const character = characterId.name ? CHID_Find(context->task->task, characterId.name) : nullptr)
        OSAGE_Initialize(character);
}
void Script_scenUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ScenarioRuntimeUtilityParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    const std::int32_t id = static_cast<std::int16_t>(params->argument0);
    const auto getBit = [](const std::uint8_t *bytes, std::int32_t bit) -> std::uint32_t {
        if (bit < 0)
            return 0;
        return (bytes[bit >> 3] & static_cast<std::uint8_t>(1U << (bit & 7))) != 0;
    };
    const auto setBit = [](std::uint8_t *bytes, std::int32_t bit, bool value) {
        if (bit < 0)
            return;
        const auto mask = static_cast<std::uint8_t>(1U << (bit & 7));
        std::uint8_t &target = bytes[bit >> 3];
        target = value ? static_cast<std::uint8_t>(target | mask) : static_cast<std::uint8_t>(target & ~mask);
    };
    const std::uint32_t argument0 = static_cast<std::uint32_t>(params->argument0);
    switch (params->command) {
    case 0:
        SCNT_InitWrapper();
        return;
    case 1:
        CreateScenSequenceCharacters();
        return;
    case 2:
        UpdateScenTaskTimer();
        return;
    case 6:
        if (argument0 == 1)
            ScenFlags() |= 0x80U;
        else if (argument0 == 0)
            ScenFlags() &= ~0x80U;
        return;
    case 7:
        EnqueueSCNT(argument0, static_cast<std::int16_t>(static_cast<std::uint16_t>(params->argument1)));
        return;
    case 8:
        UpdateScenActiveRuntimeObject();
        return;
    case 9:
        SetScenRuntimeObjectRange(argument0, params->argument1, params->argument2, params->argument3);
        return;
    case 10: {
        std::size_t ordinal = 0;
        for (std::size_t index = 0; index < kValidScenCharacterNames.size(); ++index) {
            if (kValidScenCharacterNames[index] == argument0) {
                ordinal = index;
                break;
            }
        }
        const float radius = std::bit_cast<float>(params->argument1);
        g_scenRuntimeState.rangeValues[ordinal * 3U] = radius * radius;
        return;
    }
    case 11:
        if (id < 1024)
            context->stack->accumulator = getBit(g_shenmueFlags.data(), id);
        return;
    case 12:
        if (id >= 0 && id < 1024) {
            if (params->argument1 != 0)
                MMTK_UpdateEntryForFlag(static_cast<std::uint16_t>(id));
            setBit(g_shenmueFlags.data(), id, params->argument1 != 0);
            RegisterInventoryItemGroup(static_cast<std::uint16_t>(id));
        }
        return;
    case 13:
        if (id < 64)
            context->stack->accumulator = getBit(g_scenarioStepLut.bitFlags64, id);
        return;
    case 14:
        if (id < 64)
            setBit(g_scenarioStepLut.bitFlags64, id, params->argument1 != 0);
        return;
    case 15:
        if (id >= 0 && id < 32)
            context->stack->accumulator = g_scenarioStepLut.steps[id];
        return;
    case 16:
        if (id >= 0 && id < 32)
            g_scenarioStepLut.steps[id] = static_cast<std::uint8_t>(params->argument1);
        return;
    case 17:
        context->stack->accumulator = QueryScenRuntimeObject(context, 0, argument0, 0);
        return;
    case 18:
        context->stack->accumulator = QueryScenRuntimeObject(context, argument0, params->argument1, 0);
        return;
    case 19:
        context->stack->accumulator = QueryScenRuntimeObject(context, argument0, params->argument1, params->argument2);
        return;
    case 20: {
        ScenRuntimeObject *const object = argument0 ? FindScenRuntimeObject(argument0) : g_scenRuntimeState.activeObject;
        context->stack->accumulator = 0;
        if (!object || params->argument1 != 0)
            return;
        if (object->runtimeState == 5)
            object->runtimeState = 8;
        context->stack->accumulator = 1;
        return;
    }
    case 21:
        context->stack->accumulator = (ScenFlags() >> 8U) & 1U;
        return;
    case 22:
        g_scenRuntimeState.objects.fill(ScenRuntimeObject{});
        std::fill(std::begin(g_scenarioStepLut.bitFlags64), std::end(g_scenarioStepLut.bitFlags64), std::uint8_t{0});
        g_shenmueFlags.fill(0);
        std::fill(std::begin(g_scenarioStepLut.steps), std::end(g_scenarioStepLut.steps), std::uint8_t{0});
        ResetScenScriptRuntimeState();
        return;
    case 23: {

        std::array<std::uint32_t, 2> event{};
        if (g_scenarioFunctionWork && g_scenarioFunctionWork->scnfRelationHandle >= 0) {
            while (DequeueScenarioRelationData(static_cast<std::uint32_t>(g_scenarioFunctionWork->scnfRelationHandle), event.data(), sizeof(event)) != 0) {
            }
        }
        return;
    }
    case 24:
        context->stack->accumulator = StoreScenRuntimeObjectText(argument0);
        return;
    case 25:
        context->stack->accumulator = 0;
        return;
    case 26:
        context->stack->accumulator = g_scenRuntimeState.activeObject && (g_scenRuntimeState.activeObject->runtimeFlags & 0x400000U) != 0 ? 0xffffffffU : 0;
        return;
    case 27:
        if (argument0)
            ScenFlags() &= ~0x800U;
        else
            ScenFlags() |= 0x800U;
        return;
    case 28:
        context->stack->accumulator = 0;
        if (g_searchTemporaryFocusedCharacter != 0) {
            context->stack->accumulator = 0xffffffffU;
            return;
        }
        if (g_scenRuntimeState.activeObject && (ScenFlags() & kScenarioFlagOneShotPrompt) != 0 &&
            static_cast<std::int32_t>(g_scenRuntimeState.focusBestAngle) <= 10922)
            context->stack->accumulator = 0xffffffffU;
        return;
    case 29:
        g_scenarioStepLut.notebookJinglePending = 0;
        return;
    case 30:
        g_scenHelpPromptOverride = argument0;
        return;
    default:
        return;
    }
}

void Script_LoadSND(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const LoadSoundBankParameters *>(rawParams);
    if (!params)
        return;
    sondLoadSndFromDirIntoBank(g_sceneId, params->bank, ScenarioPointer<const char>(params->filenameOffset));
}

void Script_ambSoundUtils(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const AmbientSoundParameters *>(rawParams);
    if (!params)
        return;
    ConfigureAmbientSound(params->index, params->value, ScenarioPointer<const void>(params->sourceOffset), params->positionX, params->positionY,
        params->positionZ, params->flags, ScenarioPointer<const void>(params->auxiliaryOffset));
}
void Script_Ambient1SoundUtils(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const AmbientSoundCommandParameters *>(rawParams);
    if (!params)
        return;
    ExecuteAmbientSoundCommand(params->mode, params->command);
}

void Script_SetLookPointDetails(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const LookPointParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name) {
        return;
    }
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (character) {
        SetLookPointDetails(character, params->angle, ScenarioPointer<const Vector3>(params->targetOffset), params->mode);
    }
}

void Script_FACE(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FaceParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;

    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    FACE_ApplyScriptMorph(
        character, static_cast<std::int16_t>(params->channel), static_cast<std::int16_t>(params->morph), static_cast<std::int16_t>(params->timing));
}

void Script_FACE_LCG(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FaceLcgParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;
    FACE_SetLcgState(
        character, static_cast<std::uint8_t>(params->valueA), static_cast<std::int16_t>(params->valueB), static_cast<std::uint8_t>(params->timing));
}

void Script_ReadModelFormat7(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CharacterHandParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (!character)
        return;
    mhndApplyRawHandPose(character, params->mode, ScenarioPointer<const std::int32_t>(static_cast<std::uint32_t>(params->poseIndex)), params->frameCount);
}

void Script_SetCPAMFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CpamFlagsParameters *>(rawParams);
    if (!context || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    CPAM_SetEnabled(character, params->enabled != 0);
}

void Script_osagUtil(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const OsageUtilityParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    CharacterComponentNode *const osage = HLib_GetCharacterComponent(character, SHEN_NAME(OSAG));
    if (!osage)
        return;
    std::memcpy(reinterpret_cast<std::byte *>(osage + 1) + 0x200, &params->value, sizeof(params->value));
}

void Script_GetDataFromFightProgram(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FightProgramParameters *>(rawParams);
    if (!context || !params || !params->characterId.name)
        return;
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    FightPoseWork *const fightProgram = FIGP_GetWork(character);
    if (!fightProgram)
        return;
    fightProgram->active = params->active;
}

void Script_EV_PAK_AFS_Utils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const EventPackAfsParameters *>(rawParams);
    if (!context || !params)
        return;

    PackTaskData *const pack = g_packTaskData ? g_packTaskData : reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    if (!pack)
        return;

    if (params->mode == 0) {
        switch (params->operation) {
        case 0: {
            const char *const root = ScenarioPointer<const char>(params->offset2);
            const char *const package = ScenarioPointer<const char>(params->offset);
            if (PACK_FindSlotByName(package)) {
                context->stack->accumulator = 0;
                return;
            }
            context->stack->accumulator = PACK_QueueFile(root, package) ? 0xffffffffU : 0U;
            return;
        }
        case 2:
            context->stack->accumulator = PACK_HasValidPack() ? 0xffffffffU : 0;
            return;
        case 4:
            static_cast<void>(PACK_MarkFree());
            return;
        case 5:
            context->stack->accumulator = PACK_FindSlotByName(ScenarioPointer<const char>(params->offset2)) ? 0xffffffffU : 0U;
            return;
        case 6:
            PACK_LoadFile(PACK_FindSlotByName(ScenarioPointer<const char>(params->offset2)));
            return;
        case 7:
            PACK_Drain();
            return;
        case 8: {
            PackOwner *const owner = ScenarioPointer<PackOwner>(params->offset2);
            if (PACK_FindAfsEntrySlot(owner, static_cast<std::int32_t>(params->offset), static_cast<std::int32_t>(params->value))) {
                context->stack->accumulator = 0;
                return;
            }
            PACK_LoadHumanByIndex(owner, params->offset, static_cast<std::uint32_t>(params->value));
            context->stack->accumulator = 0xffffffffU;
            return;
        }
        case 9: {
            PackSlot *const slot = PACK_FindSlotByName(ScenarioPointer<const char>(params->offset2));
            context->stack->accumulator = slot && slot->mapIndices ? HLib_GetTaskDataOffset(slot->mapIndices) : 0U;
            return;
        }
        case 10:
            pack->callbackBudget = params->offset2 ? static_cast<std::int32_t>(params->offset2) : 3000;
            return;
        case 11:
            context->stack->accumulator = static_cast<std::uint32_t>(pack->callbackBudget);
            return;
        default:
            return;
        }
    }
    if (params->mode == 1) {
        switch (params->operation) {
        case 0: {
            PackOwner *const owner = PACK_ReadAFS(ScenarioPointer<const char>(params->offset2), ScenarioPointer<const char>(params->offset));
            context->stack->accumulator = owner ? HLib_GetTaskDataOffset(owner) : 0U;
            return;
        }
        case 1:
            context->stack->accumulator = static_cast<std::uint32_t>(PACK_FreeOwnerADFSData(ScenarioPointer<void>(params->offset2)));
            return;
        default:
            return;
        }
    }
    if (params->mode == 2) {
        switch (params->operation) {
        case 0:
            context->stack->accumulator = pack->usePackCache;
            return;
        case 1:
            pack->usePackCache = params->offset2;
            return;
        case 2:
            if (PackSlot *const slot = PACK_FindSlotByName(ScenarioPointer<const char>(params->offset2)))
                PACK_SetReadyPack(slot);
            return;
        case 3: {
            PackSlot *const slot = PACK_FindSlotByName(ScenarioPointer<const char>(params->offset2));
            context->stack->accumulator = slot && pack->readyPack == slot ? 0xffffffffU : 0U;
            return;
        }
        default:
            return;
        }
    }
    if (params->mode == 3 && params->operation == 0) {
        COLS_Free();
        std::uint32_t payloadSize = 0;
        const std::byte *const payload = MAPINFO_FindChunkPayload(MAPINFO_COLS.name, &payloadSize);
        COLS_Process(payload ? const_cast<std::byte *>(payload - 8) : nullptr);
    }
}

void Script_motiLoad(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const MotionLoadParameters *>(rawParams);
    if (!context || !params)
        return;
    const char *const filename = ScenarioPointer<const char>(params->filenameOffset);
    if (!filename)
        return;
    if (std::strcmp(filename, "M_MOBJ.BIN") == 0 || std::strcmp(filename, "M_MDOR.BIN") == 0) {
        context->stack->accumulator = static_cast<std::uint32_t>(MOTN_ReadMOTNHeader(ReadFile("misc", filename, SHEN_NAME(MOTI))));
        return;
    }
    context->stack->accumulator = static_cast<std::uint32_t>(MOTN_AllocateMOTN(params->sceneId, params->areaId.name, filename));
}

void Script_ExecuteAicaCommand(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const AicaCommandParameters *>(rawParams);
    if (!context || !params)
        return;
    if (params->mode) {
        context->stack->accumulator = static_cast<std::uint32_t>(ExecuteAICACommand_Wrapper(params->value));
    } else {
        ReleaseEnvironmentAicaBank(params->value);
    }
}

void Script_LoadVoiceFile(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const VoiceLoadParameters *>(rawParams);
    if (!context || !params)
        return;
    context->stack->accumulator = LoadAudioFile_Wrapper(params->mode, ScenarioPointer<const char>(params->nameOffset)) ? 0xffffffffU : 0;
}

void Script_ProcessSubtitle(ScenarioContext *context, void *rawParams) {
    if (!context || !rawParams)
        return;
    const auto offset = *static_cast<const std::uint32_t *>(rawParams);
    CVWKRuntimeSlot *const slot = ProcessSubtitleFile(ScenarioPointer<const char>(offset));
    context->stack->accumulator = 0;
    if (!slot)
        return;
    const std::ptrdiff_t result = reinterpret_cast<std::byte *>(slot) - g_pCurrentTaskData;
    context->stack->accumulator = static_cast<std::uint32_t>(result);
}

void Script_cdttLoadScrUtils(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const CDTTLoadScreenParameters *>(rawParams);
    if (params)
        CDTT_ApplyLoadScreenUtility(*params);
}

void Script_WeatherUtils_ForFIXC(ScenarioContext *context, void *rawParams) {
    EFFE_ScriptWeatherUtilsForFIXC(context, rawParams);
}

void Script_GetSetWeatherSettings(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!context || !params)
        return;
    if (params[0] == 1)
        g_disableWeather = static_cast<std::int32_t>(params[1]);
    else if (params[0] == 2)
        context->stack->accumulator = static_cast<std::uint32_t>(g_disableWeather);
}

void Script_WeatherUtils_0(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params)
        return;
    std::uint32_t result = 0;
    if (WEXE_WeatherDataUtil(params[0], params[1], params[2], params[3], params[4], params[5], static_cast<std::int32_t>(params[6]), result) && context &&
        context->stack)
        context->stack->accumulator = result;
}

void Script_WeatherTaskUtil(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params)
        return;
    WEXE_WeatherTaskUtil(params[0], params[1], params[2]);
}
void Script_WeatherUtils(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params)
        return;

    const float value = std::bit_cast<float>(params[1]);
    const auto paramFloat = [params](std::size_t index) { return std::bit_cast<float>(params[index]); };
    const auto storeFloat = [params](float result) {
        if (float *const output = ScenarioPointer<float>(params[1]))
            *output = result;
    };
    const auto applyClipQuad = [&](std::uint32_t setIndex, std::uint32_t groupMask) {
        const Vector2 corners[4]{
            {paramFloat(1), paramFloat(2)}, {paramFloat(3), paramFloat(4)}, {paramFloat(5), paramFloat(6)}, {paramFloat(7), paramFloat(8)}};
        const auto minimumY = static_cast<float>(static_cast<double>(paramFloat(9)) - 0.1);
        const auto maximumY = static_cast<float>(static_cast<double>(paramFloat(10)) + 0.1);
        EFFE_ApplyWccpClipQuad(setIndex, groupMask, corners, minimumY, maximumY);
    };
    switch (params[0]) {
    case 2:
        EFFE_ProcessWeather(value);
        break;
    case 3:
        EFFE_FreeRainRenderData();
        break;
    case 4:
        Weather_SetForcedRainValue(value);
        break;
    case 5:
        WEXE_ApplyRainGameData(value, static_cast<std::int32_t>(params[2]));
        break;
    case 6:
        storeFloat(WEXE_GetRainIntensity());
        break;
    case 7:
        SetRainEffectOffset(params[1]);
        break;
    case 9:
        EFFE_SetRainParticleSize(value, paramFloat(2));
        break;
    case 11:
        EFFE_InitializeSnowWeatherSystems(value);
        break;
    case 12:
        EFFE_FreeSnowRenderData();
        break;
    case 13:
        Weather_SetForcedSnowValue(value);
        break;
    case 14:
        WEXE_ApplySnowGameData(value, static_cast<std::int32_t>(params[2]));
        break;
    case 15:
        storeFloat(WEXE_GetSnowIntensity());
        break;
    case 16:
        Weather_UpdateRainOffset(params[1]);
        break;
    case 18:
        EFFE_SetSnowParticleSize(value);
        break;
    case 20:
        Weather_TickPerSystemValues(value);
        break;
    case 21:
        EFFE_FreeRippleRenderData();
        break;
    case 22:
        WEXE_ApplyWindGameData(value, 0);
        break;
    case 23:
        WEXE_ApplyWindGameData(value, static_cast<std::int32_t>(params[2]));
        break;
    case 24:
        storeFloat(WEXE_GetWindIntensity());
        break;
    case 25:
        Weather_UpdateColourRamp(params[1]);
        break;
    case 27:
        EFFE_SetRippleParticleSize(value);
        break;
    case 29:
        EFFE_AttachWindData(g_weatherWindData);
        break;
    case 30:
        EFFE_DetachWindData(g_weatherWindData);
        break;
    case 31:
        EFFE_AttachWindData(params[1] ? g_pCurrentTaskData + static_cast<std::int32_t>(params[1]) : nullptr);
        break;
    case 32:
        if (params[1])
            EFFE_DetachWindData(g_pCurrentTaskData + static_cast<std::int32_t>(params[1]));
        break;
    case 33:
        applyClipQuad(0, 1U);
        break;
    case 34:
        applyClipQuad(0, 2U);
        break;
    case 35:
        applyClipQuad(0, 4U);
        break;
    case 36:
        EFFE_SetRainEffectVectors({value, paramFloat(2), paramFloat(3)}, {paramFloat(4), paramFloat(5), paramFloat(6)});
        break;
    case 37:
        EFFE_SetSnowEffectVectors({value, paramFloat(2), paramFloat(3)}, {paramFloat(4), paramFloat(5), paramFloat(6)});
        break;
    case 38:
        applyClipQuad(0, 1U);
        applyClipQuad(0, 2U);
        applyClipQuad(0, 4U);
        break;
    case 39:
        WEXE_LoadWccpClipSet(0, ScenarioPointer<const std::byte>(params[1]), params[2]);
        break;
    case 40:
        WEXE_ReleaseWccpClipSet(0);
        break;
    case 41:
        WEXE_SetWccpClipSetEnabled(0, params[1]);
        break;
    case 42:
        WEXE_SetWccpClipRecordActive(0, static_cast<std::int32_t>(params[1]), params[2]);
        break;
    case 43:
        applyClipQuad(1, 7U);
        break;
    case 44:
        WEXE_LoadWccpClipSet(1, ScenarioPointer<const std::byte>(params[1]), params[2]);
        break;
    case 45:
        WEXE_ReleaseWccpClipSet(1);
        break;
    case 46:
        WEXE_SetWccpClipSetEnabled(1, params[1]);
        break;
    case 47:
        WEXE_SetWccpClipRecordActive(1, static_cast<std::int32_t>(params[1]), params[2]);
        break;
    case 48:
        Weather_ResetWCCPClipSet0();
        break;
    case 49:
        Weather_ResetWCCPClipSet1();
        break;
    default:
        break;
    }
}

void Script_StartOrFreeSEQ(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    const std::uint32_t offset = *static_cast<const std::uint32_t *>(rawParams);
    shenForceClearCurrentSubtitles(ScenarioPointer<CVWKRuntimeSlot>(offset));
}

void Script_SetImageModuleTaskDataFlag(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ImageModuleFlagParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name) {
        return;
    }
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    if (ImageModuleWork *const image = IMGM_GetWork(character))
        image->requestedImage = static_cast<std::uint8_t>(params->requestedImage);
}

void Script_SetRefBoardValues(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const RefBoardValuesParameters *>(rawParams);
    if (!context || !context->task || !params)
        return;
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;


    SetRefBoardValues(character, params->type, params->field024Offset ? *ScenarioPointer<const std::int32_t>(params->field024Offset) : 0);
}

void Script_MotStepUtil(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const MotStepParameters *>(rawParams);
    if (!context || !context->task || !params || !params->characterId.name) {
        return;
    }
    Character *const character = CHID_Find(context->task->task, params->characterId.name);
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(STEP));
    if (!component)
        return;

    auto *const payload = reinterpret_cast<std::uint8_t *>(component + 1);
    if (params->selector == 0)
        payload[1] = params->value;
    else if (params->selector == 1)
        payload[2] = params->value;
}

std::int64_t SCNF_UIUtils(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return 0x10000;
    const auto &params = *static_cast<const ScnfUiUtilsParameters *>(rawParams);
    if (params.command == -1) {
        std::int16_t result = -1;
        if (EV_GetCurrentQteStatusWhileOpen(result)) {
            context->stack->accumulator = static_cast<std::uint16_t>(result);
        } else {
            context->stack->accumulator = 0xffffU;
        }
        return 0x10000;
    }
    if (params.command > 0) {
        EVT_AT_ResultQTE(-1);
        return 0x10000;
    }

    EVT_AT_QTE({
        params.expectedMask,
        params.timeoutFrames,
        params.promptMode,
        params.bonusFrames,
    });
    return 0;
}

std::int64_t SCNF_GetSfxOverlayResultIfIdle(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return 0;
    std::int16_t result = -1;
    if (!EV_GetCurrentQteStatusIfIdle(result))
        return 0;
    context->stack->accumulator = static_cast<std::uint16_t>(result);
    return 0x10000;
}

std::int64_t SCNF_RequestActionSelectorFromParams(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return 0;
    auto *const params = static_cast<std::uint32_t *>(rawParams);
    std::int32_t mode = static_cast<std::int32_t>(params[6]);
    if (mode >= 6) {
        params[6] = 0;
        mode = 0;
    }
    UI_ICSpRequest({
        static_cast<std::int16_t>(params[0]),
        static_cast<std::int16_t>(params[1]),
        static_cast<std::int16_t>(params[2]),
        static_cast<std::int16_t>(params[3]),
        static_cast<std::uint16_t>(params[4]),
        static_cast<std::int16_t>(params[5]),
        static_cast<std::int16_t>(mode),
    });
    return 0;
}

std::int64_t SCNF_GetSelectedHelpToken(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return 0;
    std::int16_t token = -1;
    if (!UI_ICSpPeekSelectedHelpToken(token))
        return 0;
    context->stack->accumulator = static_cast<std::uint16_t>(token);
    return 0x10000;
}

std::int64_t SCNF_RequestActionSelector(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return 0;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    std::int16_t mode = static_cast<std::int16_t>(params[1]);
    if (mode == 0)
        mode = 1;
    else if (mode == 2)
        mode = 0;
    UI_ICSpRequest({
        -1,
        0,
        1,
        -1,
        static_cast<std::uint16_t>(params[0]),
        mode,
        static_cast<std::int16_t>(params[2]),
    });
    return 0;
}

std::int64_t SCNF_GetSelectedBinary(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return 0;
    std::int16_t token = -1;
    if (!UI_ICSpPeekSelectedHelpToken(token))
        return 0;
    if (token == 0)
        token = 1;
    else if (token == 1)
        token = 0;
    context->stack->accumulator = static_cast<std::uint32_t>(static_cast<std::int32_t>(token));
    return 0x10000;
}

std::int64_t SCNF_SetCashValue(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return 0;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    UI_ICMoRequest(static_cast<std::int32_t>(params[0]), params[1] != 0);
    return 0;
}

std::int64_t SCNF_WaitCashOverlayIdle(ScenarioContext *, void *) {
    return UI_ICMoIsIdle() ? 0x10000 : 0;
}

std::int64_t SCNF_StartCashOverlaySelector(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return 0;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    UI_ICMoBeginSelector(static_cast<std::int16_t>(params[0]), static_cast<std::uint16_t>(params[1]));
    return 0;
}

std::int64_t SCNF_GetCashOverlayResult(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return 0;
    std::int32_t result = -1;
    if (!UI_ICMoPeekResult(result))
        return 0;
    context->stack->accumulator = static_cast<std::uint32_t>(result);
    return 0x10000;
}

std::int64_t SCNF_RequestIconNotification(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return 0x10000;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    UI_ICONRequestNotification(static_cast<std::uint16_t>(params[0]), static_cast<std::int16_t>(params[1]), static_cast<std::int16_t>(params[2]),
        static_cast<std::int16_t>(params[3]) | 0x8000);
    return params[3] ? 0x10000 : 0;
}

std::int64_t SCNF_WaitIconNotification(ScenarioContext *, void *) {
    return UI_ICONNotificationIsFadingOut() ? 0x10000 : 0;
}

std::int64_t SCNF_StartIconSystemMenu(ScenarioContext *, void *rawParams) {
    if (rawParams)
        UI_ICONStartItemInfoPopup(static_cast<std::uint16_t>(*static_cast<const std::uint32_t *>(rawParams)));
    return 0;
}

std::int64_t SCNF_WaitIconSystemMenu(ScenarioContext *, void *) {
    return UI_ICONAdvanceItemInfoPopup() ? 0x10000 : 0;
}

std::int64_t SCNF_PrepareCassetteUI(ScenarioContext *context, void *) {
    if (UI_ICMCBegin())
        return 0;
    if (context && context->stack)
        context->stack->accumulator = 0xffffffffU;
    return 0x10000;
}

std::int64_t SCNF_WaitCassetteUI(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return 0;
    std::int32_t result = -1;
    if (!UI_ICMCWaitForClosed(result))
        return 0;
    context->stack->accumulator = static_cast<std::uint32_t>(result);
    return 0x10000;
}

std::int64_t SCNF_ShowHelpMessage(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return 0;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    TimedHelpSelectorChoice *const choices =
        params[1] ? reinterpret_cast<TimedHelpSelectorChoice *>(g_pCurrentTaskData + static_cast<std::int32_t>(params[1])) : nullptr;
    const std::int32_t built = choices ? BuildTimedHelpSelectorChoices(params[0], choices) : 0;

    const auto timeout = static_cast<std::int32_t>(params[2]);
    if (timeout < 0) {
        context->stack->accumulator = static_cast<std::uint32_t>(built);
        return 0x10000;
    }
    if (built &&
        UI_ICSpRequest({.up = choices[0].helpToken,
            .left = choices[2].helpToken,
            .right = choices[3].helpToken,
            .down = choices[1].helpToken,
            .timeoutFrames = static_cast<std::uint16_t>(timeout),
            .timeoutHelpToken = -1,
            .mode = 2}))
        return 0;
    context->stack->accumulator = 0xffffffffU;
    return 0x10000;
}

std::int64_t SCNF_GetSelectedChoiceIndex(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return 0;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    const auto *const choices =
        params[1] ? reinterpret_cast<const TimedHelpSelectorChoice *>(g_pCurrentTaskData + static_cast<std::int32_t>(params[1])) : nullptr;
    std::int16_t selected = -1;
    if (!UI_ICSpPeekSelectedHelpToken(selected))
        return 0;
    const auto matches = [choices, selected](std::size_t slot) {
        return choices && static_cast<std::uint16_t>(selected) == static_cast<std::uint16_t>(choices[slot].helpToken) && choices[slot].enabled == 1;
    };
    std::int32_t choice = -1;
    if (matches(0))
        choice = 0;
    else if (matches(2))
        choice = 2;
    else if (matches(3))
        choice = 3;
    else if (matches(1))
        choice = 1;
    context->stack->accumulator = static_cast<std::uint32_t>(choice);
    return 0x10000;
}

std::int64_t SCNF_CopyScenario(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return 0x10000;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    switch (params[0]) {
    case 0:
        context->stack->accumulator = static_cast<std::uint32_t>(AcquireScenarioRelationQueue(params[1]));
        return 0x10000;
    case 1: {
        const void *const source = params[2] ? g_pCurrentTaskData + static_cast<std::int32_t>(params[2]) : nullptr;
        context->stack->accumulator = static_cast<std::uint32_t>(EnqueueScenarioRelationData(params[1], source, static_cast<std::int32_t>(params[3])));
        return 0x10000;
    }
    case 2: {
        void *const destination = params[2] ? g_pCurrentTaskData + static_cast<std::int32_t>(params[2]) : nullptr;
        return DequeueScenarioRelationData(params[1], destination, static_cast<std::int32_t>(params[3])) < 1 ? 0 : 0x10000;
    }
    default:
        return 0x10000;
    }
}

std::int64_t SCNF_WaitFrames(ScenarioContext *, void *rawParams) {
    auto &frames = *static_cast<std::uint32_t *>(rawParams);
    const std::uint32_t previous = frames--;
    return previous ? 0 : 0x10000;
}
std::int64_t SCNF_ReturnZero(ScenarioContext *, void *) {
    return 0;
}

std::int64_t SCNF_ReturnMinusOne(ScenarioContext *context, void *) {
    context->stack->accumulator = 0xffffffffU;
    return 0x10000;
}

std::int64_t SCNF_CompleteImmediately(ScenarioContext *, void *) {
    return 0x10000;
}

std::uint32_t LegacyReturnZero(ScenarioRegisters *) {
    return 0;
}

std::uint32_t GeneralReturnZero(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t) {
    return 0;
}

std::uint32_t GeneralMemset(std::uint32_t destinationOffset, std::uint32_t value, std::uint32_t size, std::uint32_t) {
    std::memset(ScenarioPointer<void>(destinationOffset), static_cast<std::int32_t>(value), size);
    return destinationOffset;
}

std::uint32_t GeneralMemcpy(std::uint32_t destinationOffset, std::uint32_t sourceOffset, std::uint32_t size, std::uint32_t) {
    std::memcpy(ScenarioPointer<void>(destinationOffset), ScenarioPointer<const void>(sourceOffset), size);
    return destinationOffset;
}

std::uint32_t GeneralStrcpy(std::uint32_t destinationOffset, std::uint32_t sourceOffset, std::uint32_t, std::uint32_t) {
    std::strcpy(ScenarioPointer<char>(destinationOffset), ScenarioPointer<const char>(sourceOffset));
    return destinationOffset;
}

std::uint32_t GeneralGetDebugLogFlag(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t) {
    return GetDebugLogFlag();
}

void Script_PreserveAccumulator(ScenarioContext *, void *) {
}

void Script_ClearAccumulator(ScenarioContext *context, void *) {
    context->stack->accumulator = 0;
}
void Script_shenGetTaskBase(ScenarioContext *context, void *) {
    context->stack->accumulator = HLib_GetRenderTaskBaseOffset();
}
void Script_GetCurrentEntryPoint(ScenarioContext *context, void *) {
    context->stack->accumulator = g_entryPoint;
}

void Script_set_val_to_1(ScenarioContext *, void *rawParams) {
    auto *const params = static_cast<std::uint32_t *>(rawParams);
    if (static_cast<std::int32_t>(params[1]) <= 1) {
        params[0] += 2;
        params[1] = 1;
    }
}

void Script_srchSetDistanceTarget(ScenarioContext *, void *rawParams) {
    g_searchTargetDistance = *static_cast<const float *>(rawParams);
}

void Script_ClearAccumulatorIfArgumentZero(ScenarioContext *context, void *rawParams) {
    if (*static_cast<const std::uint32_t *>(rawParams) == 0)
        context->stack->accumulator = 0;
}

void Script_sprintf(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const auto *const arguments = static_cast<const std::uint32_t *>(rawParams);
    FormattedScenarioString formatted = FormatScenarioString(arguments[0], arguments + 1);
    ConvertScenarioMessageFromShiftJis(formatted.message);
    if (g_tmexTask && std::strcmp(formatted.message.data(), "DOBUITA EVENT START START START!\n") == 0)
        UI_HandleDobuitaEventStartTransition();
    context->stack->accumulator = formatted.substitutionLength;
}

void Script_PrintDebugString(ScenarioContext *, void *rawParams) {
    if (!GetDebugLogFlag() || !rawParams)
        return;
    const auto *const params = static_cast<const DebugStringParameters *>(rawParams);
    g_debugTextPosition = {params->x, params->y};
    std::va_list nativeArguments;
#if defined(_WIN32)
    nativeArguments = reinterpret_cast<char *>(rawParams) + offsetof(DebugStringParameters, arguments);
#else
    nativeArguments[0].gp_offset = 48;
    nativeArguments[0].fp_offset = 304;
    nativeArguments[0].overflow_arg_area = reinterpret_cast<char *>(rawParams) + offsetof(DebugStringParameters, arguments);
    nativeArguments[0].reg_save_area = nullptr;
#endif
    char formatted[256];
    std::vsprintf(formatted, ScenarioPointer<const char>(params->formatOffset), nativeArguments);
    va_end(nativeArguments);
    g_debugText = formatted;
    (void)QueueDebugText(g_debugTextPosition[0], g_debugTextPosition[1], g_debugText);
}


PackSlot *ScriptPackSlot(std::uint32_t word) {
    return word ? &g_packSlots[word - 1u] : nullptr;
}
std::uint32_t ScriptPackSlotWord(const PackSlot *slot) {
    return slot ? static_cast<std::uint32_t>(slot - g_packSlots) + 1u : 0u;
}
void Script_CycleManagerUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const CycleManager::UtilityParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;

    const auto findInList = [&](std::uint32_t token, CycleProgramList list) -> CycleManager::NpcRecord * {
        for (CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
            if (npc.cycle->moveConfig->obj_id.name == token && npc.programList == list)
                return &npc;
        }
        return nullptr;
    };

    switch (params->command) {
    case 0: {
        const char *const filename = params->operand ? ScenarioPointer<const char>(params->operand) : nullptr;
        context->stack->accumulator = MOBJ_Setup(filename, params->areaId) ? 1U : 0U;
        break;
    }
    case 1: {
        const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        context->stack->accumulator = control && control->scenarioTask && CHID_Find(control->scenarioTask, params->operand) ? 1U : 0U;
        break;
    }
    case 2:
        CycleManager::LinkCyclesInStack(static_cast<std::int32_t>(params->operand));
        context->stack->accumulator = 1;
        break;
    case 3:
        CycleManager::ProcessSNDForCycles();
        context->stack->accumulator = 1;
        break;
    case 4:
        if (!params->offset && g_cycleManagerRuntime.areaId.name == SHEN_NAME(JHD0)) {
            KDAN_RemoveOccupancyByCharacterId(SHEN_NAME(FUKU));
        }
        CycleManager::ParseForScript(params->operand, params->offset != 0);
        context->stack->accumulator = 1;
        break;
    case 5:
        context->stack->accumulator = ScriptPackSlotWord(CycleManager::SetCyclePtrByCharaID(params->operand));
        break;
    case 6:
        context->stack->accumulator = CycleManager::GetNextTaskPointer(ScriptPackSlot(params->operand));
        break;
    case 7: {
        // The running program holding the pack lets go of it (and of its character and motion); a pack no program holds is released.
        PackSlot *const slot = ScriptPackSlot(params->operand);
        CycleManager::MobjEntry *entry = CycleManager::ProgramListHead(CycleProgramList_Running);
        while (entry && entry->ctx->filename != slot)
            entry = entry->nextDormantProgram;
        if (entry) {
            CycleManager::UpdateSndStreaming(*CycleManager::EntryRecord(entry));
            entry->ctx->obj = nullptr;
            entry->ctx->mot = nullptr;
        } else {
            LoadFile(slot);
        }
        context->stack->accumulator = 1;
        break;
    }
    case 8:
        if (params->operand && !gCycleManager) {
            MOBJ_SetEnabled(true);
            CycleManager::ProcessCycle(true);
        } else if (!params->operand) {
            MOBJ_SetEnabled(false);
        }
        context->stack->accumulator = 1;
        break;
    case 9: {
        const CycleManager::NpcRecord *const npc = findInList(params->operand, CycleProgramList_SimSet);
        context->stack->accumulator = npc && npc->cycle->currentOpcode == CycleOpcode_InitTimedPath && !g_bGlobalFlag ? 1U : 0U;
        break;
    }
    case 10: {
        const CycleManager::NpcRecord *const npc = findInList(params->operand, CycleProgramList_SimSet);
        context->stack->accumulator =
            !npc ? 0xffffffffU : (npc->cycle->currentOpcode == CycleOpcode_DriveForkliftPath2D ? npc->cycle->activeAnimId : npc->cycle->selectedMotionToken);
        break;
    }
    case 11: {
        const CycleManager::NpcRecord *const npc = findInList(params->operand, CycleProgramList_SimSet);
        if (!npc) {
            context->stack->accumulator = 0;
        } else if (npc->cycle->currentOpcode == CycleOpcode_InitTimedPath) {
            context->stack->accumulator = g_bGlobalFlag ? 1U : 0xffffffffU;
        } else if (npc->cycle->currentOpcode == CycleOpcode_StartLinkedCycle) {
            context->stack->accumulator = npc->cycle->moveObjSubOpcode == CycleOpcode_WalkPath3D ? 1U : 0xfffffffeU;
        } else if (npc->cycle->currentOpcode == CycleOpcode_DriveForkliftPath2D) {
            context->stack->accumulator = 0xfffffffdU;
        } else {
            context->stack->accumulator = 1;
        }
        break;
    }
    case 12: {
        const CycleManager::NpcRecord *const npc = findInList(params->operand, CycleProgramList_SimSet);
        context->stack->accumulator = npc && npc->cycle->moveConfig->cycleClassCode <= 1 ? npc->cycle->moveConfig->cycleClassCode : 0U;
        break;
    }
    case 13: {
        CycleManager::NpcRecord *const npc = findInList(params->operand, CycleProgramList_Running);
        if (npc)
            npc->cycle->contextValue = params->offset;
        context->stack->accumulator = npc ? 1U : 0U;
        break;
    }
    case 14:
        g_pCycleManagerTimeFlag = params->offset ? g_pCurrentTaskData + params->offset : nullptr;
        context->stack->accumulator = 1;
        break;
    case 17: {
        const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        context->stack->accumulator = npc ? npc->cycle->contextValue : 0U;
        break;
    }
    case 15:
        context->stack->accumulator = static_cast<std::uint32_t>(
            g_nearQueueCycleCount + numCharactersSimulated + g_promoteListCycleCount + g_midQueueCycleCount + g_farQueueCycleCount + g_perFrameNpcWorkCnt);
        break;
    case 16:
        context->stack->accumulator = static_cast<std::uint32_t>(std::min(MaxNearNPCs, numCharactersSimulated));
        break;
    case 0x12:
        CycleManager::ProcessSNDForCycles();
        CycleManager::ProcessSNDForCyclesExtra();
        context->stack->accumulator = 1;
        break;
    case 19: {
        const Vector3 *const point = ScenarioPointer<const Vector3>(params->operand);
        const float radius = SCN_FloatFromU32(params->offset);
        const float radiusSquared = radius * radius;
        std::uint32_t count = 0;
        if (point) {
            for (const CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
                const bool active = (npc.programList == CycleProgramList_SimSet && npc.cycle->areaID.name == g_areaId) ||
                    (npc.programList == CycleProgramList_Running && npc.cycle->obj);
                if (!active)
                    continue;
                const float dx = point->x - npc.cycle->pos.x;
                const float dy = point->y - npc.cycle->pos.y;
                const float dz = point->z - npc.cycle->pos.z;
                count += dy * dy + dx * dx + dz * dz < radiusSquared;
            }
        }
        context->stack->accumulator = count;
        break;
    }
    case 20: {
        const Vector3 *const quad = ScenarioPointer<const Vector3>(params->operand);
        std::uint32_t count = 0;
        if (quad) {
            const Vector3 a{.x = quad[0].x, .y = 0.0f, .z = quad[0].z};
            const Vector3 b{.x = quad[1].x, .y = 0.0f, .z = quad[0].z};
            const Vector3 c{.x = quad[1].x, .y = 0.0f, .z = quad[1].z};
            const Vector3 d{.x = quad[0].x, .y = 0.0f, .z = quad[1].z};
            for (const CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
                const bool active = (npc.programList == CycleProgramList_SimSet && npc.cycle->areaID.name == g_areaId) ||
                    (npc.programList == CycleProgramList_Running && npc.cycle->obj);
                count += active && check_if_point_in_quadrilateral(a, b, c, d, npc.cycle->pos);
            }
        }
        context->stack->accumulator = count;
        break;
    }
    case 21:
        CycleManager::ParseProgramForChara(params->operand, params->offset);
        context->stack->accumulator = 0;
        break;
    case 22:
        g_fGlobalTurningRadius = SCN_FloatFromU32(params->operand);
        g_default_turning_box_half_depth_z = SCN_FloatFromU32(params->offset);
        g_angYoffset = SCN_FloatFromU32(params->areaId.name);
        context->stack->accumulator = 0;
        break;
    case 23: {
        CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        if (npc && npc->cycle->currentOpcode != CycleOpcode_ScriptTransformOverride) {
            npc->cycle->tempState41SavedOpcode = npc->cycle->currentOpcode;
            npc->cycle->areaID = shen_name{g_areaId};
            npc->cycle->currentOpcode = CycleOpcode_ScriptTransformOverride;
        }
        context->stack->accumulator = 0;
        break;
    }
    case 24: {
        CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        if (npc && npc->cycle->currentOpcode == CycleOpcode_ScriptTransformOverride)
            npc->cycle->currentOpcode = static_cast<CycleOpcode>(npc->cycle->tempState41SavedOpcode);
        context->stack->accumulator = 0;
        break;
    }
    case 25: {
        CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        if (npc && npc->cycle->currentOpcode == CycleOpcode_ScriptTransformOverride) {
            npc->cycle->areaID = shen_name{g_areaId};
            npc->cycle->pos = {SCN_FloatFromU32(params->offset), SCN_FloatFromU32(params->areaId.name), SCN_FloatFromU32(params->argument10)};
            npc->cycle->desiredPathYaw = static_cast<std::int16_t>(params->argument14 & 0xffffU);
        }
        context->stack->accumulator = 0;
        break;
    }
    case 26: {
        CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        if (npc && npc->cycle->currentOpcode == CycleOpcode_ScriptTransformOverride) {
            npc->cycle->areaID = shen_name{g_areaId};
            npc->cycle->activeAnimId = static_cast<std::uint16_t>(params->offset & 0xffffU);
        }
        context->stack->accumulator = 0;
        break;
    }
    case 0x1b:
        context->stack->accumulator = ScriptPackSlotWord(CycleManager::ParseForScript_InternalForHuman(params->operand));
        break;
    case 0x1c:
        g_cycleManagerRuntime.cycleLoadSettleFrameCounter = 0;
        context->stack->accumulator = 0;
        break;
    case 0x1d:
        ++g_cycleManagerRuntime.cycleLoadSettleFrameCounter;
        context->stack->accumulator = g_cycleManagerRuntime.cycleLoadSettleFrameCounter < 5 ?
            1U :
            (g_perFrameNpcWorkCnt ? (g_cycleManagerRuntime.cycleLoadSettleFrameCounter < 60 ? 1U : 0U) : 0U);
        break;
    case 0x1e: {
        const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        context->stack->accumulator = params->operand == SHEN_NAME(DOOR) || (npc && npc->cycle->modeValue == 2) ? 1U : 0U;
        break;
    }
    case 31:
        if (g_steeringIntersectionVolumeCount < g_cycleManagerRuntime.intersectionEntries.size()) {
            CycleManager::IntersectionEntry &entry = g_cycleManagerRuntime.intersectionEntries[g_steeringIntersectionVolumeCount++];
            entry.operand = params->operand;
            entry.offset = params->offset;
            entry.dwordC = params->areaId.name;
            entry.dword10 = params->argument10;
            entry.dword14 = params->argument14;
            entry.dword18 = params->argument18;
            entry.dword1C = params->argument1c;
        }
        context->stack->accumulator = 0;
        break;
    case 600: {
        const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        context->stack->accumulator = npc ? static_cast<std::uint32_t>(static_cast<float>(npc->cycle->selectedMotionToken) / 500.0F * 100.0F) : 0U;
        break;
    }
    case 601: {
        const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        const std::uint32_t raw = !npc ? 0U : (params->offset == 1 ? npc->cycle->selectedMotionToken : npc->field148);
        context->stack->accumulator = npc ? static_cast<std::uint32_t>(static_cast<float>(raw) / 500.0F * 100.0F) : 0U;
        break;
    }
    case 602: {
        CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        if (npc)
            npc->cycle->morsCommandArg = params->offset != 0;
        context->stack->accumulator = 0;
        break;
    }
    case 666: {
        const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(params->operand);
        context->stack->accumulator = npc && npc->cycle->currentOpcode == CycleOpcode_ScheduleMnrBEntry && (npc->cycle->morsCommandStatus & 0xffff0000U) == 0 &&
                npc->cycle->morsCommandStatus == 3 ?
            npc->cycle->morsDeadlineTime :
            0U;
        break;
    }
    default:
        context->stack->accumulator = 0;
        break;
    }
}

void Script_timeSetUpdateEnabled(ScenarioContext *, void *rawParams) {
    if (g_timeValue)
        g_shouldTimeAdvance = *static_cast<const std::uint8_t *>(rawParams);
}

void Script_GetSceneID(ScenarioContext *context, void *) {
    context->stack->accumulator = static_cast<std::uint32_t>(g_sceneId);
}

void Script_GetGlobalFlag(ScenarioContext *context, void *) {
    context->stack->accumulator = g_globalFlags[0] == 80 ? 50U : 60U;
}

void Script_SetGlobalSceneSlotValue34(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const GlobalSceneSlotValueParameters *>(rawParams);
    if (!params || params->slot >= g_dynamicMapSlots.size())
        return;
    g_dynamicMapSlots[params->slot].lightingOverride01 = std::bit_cast<float>(params->value);
}

void Script_SetNewDateTimeWrapper(ScenarioContext *context, void *rawParams) {
    ScriptArguments arguments(context, rawParams);
    GameDateTime *const current = arguments.pointer<GameDateTime>(0);
    const GameDateTime *const maximum = arguments.pointer<GameDateTime>(1);
    if (!current || !maximum)
        return;
    SetNewDateTime(*current);
    g_maximumDateTime.year = maximum->year;
    g_maximumDateTime.month = maximum->month;
    g_maximumDateTime.day = maximum->day;
    g_maximumDateTime.hours = maximum->hours;
    g_maximumDateTime.minutes = maximum->minutes;
    g_maximumDateTime.seconds = maximum->seconds;
    RecalculateDayInTheWeek(&g_maximumDateTime);
    if (TIME_IsActive())
        TIME_EnableAdvance();
}

void Script_CompareDates(ScenarioContext *context, void *rawParams) {
    ScriptArguments arguments(context, rawParams);
    const GameDateTime *const source = arguments.pointer<GameDateTime>(0);
    if (!source) {
        arguments.result(std::uint32_t{0});
        return;
    }
    g_scenarioDateScratch.year = source->year;
    g_scenarioDateScratch.month = source->month;
    g_scenarioDateScratch.day = source->day;
    g_scenarioDateScratch.hours = source->hours;
    g_scenarioDateScratch.minutes = source->minutes;
    g_scenarioDateScratch.seconds = source->seconds;
    RecalculateDayInTheWeek(&g_scenarioDateScratch);
    arguments.result(static_cast<std::uint32_t>(CompareDates(&g_currentDateTime, &g_scenarioDateScratch)));
}

void Script_IncrementDateTimeByNum(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::int16_t days = arguments.signedWord(0);
    if (days > 0)
        IncrementDateTimeByDays(g_currentDateTime, static_cast<std::uint32_t>(days));
    else
        RecalculateDayInTheWeek(&g_currentDateTime);
}

void Script_SetDateAndTime_Wrapper(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    SetDateAndTime(static_cast<std::uint8_t>(arguments.word(0)), static_cast<std::uint8_t>(arguments.word(1)), static_cast<std::uint8_t>(arguments.word(2)),
        static_cast<std::uint8_t>(arguments.word(3)));
}

void Script_shenSetCurrentDate(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const GameDateTime *const source = arguments.pointer<GameDateTime>(0);
    if (!source)
        return;
    if (static_cast<std::int8_t>(source->year) > static_cast<std::int8_t>(g_currentDateTime.year) ||
        static_cast<std::int8_t>(source->month) > static_cast<std::int8_t>(g_currentDateTime.month) ||
        static_cast<std::int8_t>(source->day) > static_cast<std::int8_t>(g_currentDateTime.day)) {
        g_currentDateTime.year = source->year;
        g_currentDateTime.month = source->month;
        g_currentDateTime.day = source->day;
    }
}

void Script_GetDateTimeUpdateHistory(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    TIME_WriteUpdateHistory(arguments.pointer<std::uint8_t>(0));
}

void Script_SetMaxInterpSpeed(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    g_maxHandRotationBlend = arguments.scalar(0);
}

void Script_GetTempAngleInDegrees(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(CAME_GetTemporaryAngleDegrees((arguments.word(0) & 0xffU) != 0));
}

void Script_GetCurrentControlPointCode(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(PCCTL_GetCurrentControlPointCode());
}

void Script_ParseDigitsForInt(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(ParseControlPointCodeDigits(PCCTL_GetCurrentControlPointCode()));
}

void Script_navSetCurrentPos(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    if (Vector3 *const output = arguments.pointer<Vector3>(0))
        *output = PCCTL_GetCurrentControlPointPosition();
}

void Script_GetHighScoreValue(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t slot = arguments.word(0);
    arguments.result(slot <= 15 ? SAVE_GetArcadeHighScore(slot) : std::uint32_t{0});
}

void Script_SetHighScoreValue(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);

    const std::uint32_t slot = arguments.word(0);
    if (slot <= 15)
        SAVE_SetArcadeHighScore(slot, arguments.word(1));
}

void Script_ConstructColourPair(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    auto *const control = g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    HLib::Task *const filmTask = control ? control->filmTask : nullptr;
    const std::uint32_t interpolationFrames = arguments.word(0);
    if (interpolationFrames == 0xffffffffU) {
        if (g_NumFramesProcessed) {
            arguments.result(1U);
            return;
        }
        const auto *const film = filmTask && filmTask->task_data_ptr ?
            reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(filmTask->task_data_ptr) + 4) :
            nullptr;
        arguments.result(film && film->remainingFrames ? 1U : 0U);
        return;
    }
    const ColourBlendPair blend{{static_cast<std::uint8_t>(arguments.word(2)), static_cast<std::uint8_t>(arguments.word(3)),
                                    static_cast<std::uint8_t>(arguments.word(4)), static_cast<std::uint8_t>(arguments.word(5))},
        {static_cast<std::uint8_t>(arguments.word(6)), static_cast<std::uint8_t>(arguments.word(7)), static_cast<std::uint8_t>(arguments.word(8)),
            static_cast<std::uint8_t>(arguments.word(9))},
        interpolationFrames};
    const std::uint32_t mode = arguments.word(1);
    if (mode <= 2)
        ConstructColours(filmTask, &blend, static_cast<ConstructColourMode>(mode));
}


void Script_SetClampedParameterValue(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t index = arguments.word(0);
    float value = std::fmin(1.0f, arguments.scalar(1));
    if (std::fabs(value) < 0.00000011920929f)
        value = 0.0f;
    if (index < g_dynamicMapSlots.size())
        g_dynamicMapSlots[index].scale = value;
}

void Script_SetAnimatedSequenceCollisionCheckDisabled(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    const bool previous = g_aseq_do_coli_check;
    if (mode <= 1)
        CAME_SetAnimatedSequenceCollisionCheck(mode != 0);
    arguments.result(previous ? 0xffffffffU : 0U);
}

void Script_ASEQGlobalFloat(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const float value = arguments.scalar(0);
    g_animatedSequencePlayDistance = value == 0.0f ? 99999.0f : value;
}

void Script_ClearInputFlags(ScenarioContext *, void *) {
    g_inputState.slots[0].pressedPriority |= 0x800U;
}

void Script_SetClipperFlag(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Clipper_SetGlobalFlag(arguments.signedWord(0));
}

void Script_refbSetActive(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    refbSetGlobalActive(arguments.word(0));
}

void Script_GetEngineInfo(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    switch (arguments.word(0)) {
    case 0:
        arguments.result(GetFramesPast());
        break;
    case 1:
        arguments.result(GetFramesPastCopy());
        break;
    case 2:
        arguments.result(static_cast<std::uint32_t>(Shenmue_nNumRenderedFrames));
        break;
    case 3:
        arguments.result(static_cast<std::uint32_t>(Shenmue_nPauseState));
        break;
    case 4:
        arguments.result(GetDebugLogFlag());
        break;
    case 5:
        arguments.result(static_cast<std::uint32_t>(g_sceneId));
        break;
    case 6:
        arguments.result(g_areaId);
        break;
    case 7:
        arguments.result(g_entryPoint);
        break;
    default:
        break;
    }
}

void Script_convGetIsActive(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    bool active = false;
    const std::uint32_t taskDataOffset = arguments.word(0);
    if (taskDataOffset) {
        const HLib::TaskData *const taskData = ScenarioPointer<const HLib::TaskData>(taskDataOffset);
        active = taskData && taskData->content_size != 0;
    } else if (g_pCVWK) {
        active = g_pCVWK->slots[0].callback || g_pCVWK->slots[1].callback;
    }
    arguments.result(active ? 0xffffffffU : 0U);
}

void Script_freeTalkGetAndResetState(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(static_cast<std::uint32_t>(SCNT_GetAndResetFreeTalkCommand()));
}

void Script_GetFreeTalkStatus(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(static_cast<std::uint32_t>(SCNT_GetFreeTalkStatus()));
}

void Script_SetDynamicMapStaticSceneMode(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::int32_t index = arguments.signedWord(0);
    DynamicMapEntry &slot = g_dynamicMapSlots[static_cast<std::size_t>(index)];
    arguments.result(slot.staticSceneMode ? 0xffffffffU : 0U);
    if (index < static_cast<std::int32_t>(g_dynamicMapSlots.size()))
        slot.staticSceneMode = arguments.word(1);
}

void Script_SetDynamicMapDebugDrawFlag(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const std::int32_t *>(rawParams);
    const std::int64_t index = params[0];
    if (index < static_cast<std::int64_t>(g_dynamicMapSlots.size())) {
        g_dynamicMapSlots[static_cast<std::size_t>(index)].debugDrawFlag = static_cast<std::uint32_t>(params[1]);
    }
}

void Script_skyCtrlLuminance(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    bool previous = false;
    switch (arguments.word(0)) {
    case 0:
        previous = SCRL_SetLuminanceEnabled(arguments.word(1));
        break;
    case 1:
        previous = SCRL_SetLuminanceChannels(
            static_cast<std::uint16_t>(arguments.word(1)), static_cast<std::uint16_t>(arguments.word(2)), static_cast<std::uint16_t>(arguments.word(3)));
        break;
    default:
        return;
    }
    arguments.result(previous ? 0xffffffffU : 0U);
}

void Script_SetVector4ByOffset(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    if (const Vector4 *const value = arguments.pointer<Vector4>(0))
        EFFE_SetScriptPosition(*value);
}

void Script_SetEffectParams(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    const std::uint32_t value = arguments.word(1);
    switch (mode) {
    case 0:
    case 1:
        EFFE_SetScriptWordParameter(mode, value);
        break;
    case 2:
        if (const Vector4 *const vector = ScenarioPointer<const Vector4>(value))
            EFFE_SetScriptVector4Parameter(mode, *vector);
        break;
    case 3:
    case 4:
        if (const Vector3 *const vector = ScenarioPointer<const Vector3>(value))
            EFFE_SetScriptVector3Parameter(mode, *vector);
        break;
    case 5:
    case 6:
    case 7:
        EFFE_SetScriptScalarParameter(mode, std::bit_cast<float>(value));
        break;
    default:
        break;
    }
}

void Script_shenGetStats(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(GetPlayerSimpleStat(arguments.word(0)));
}

void Script_SetStats(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    SetPlayerSimpleStat(arguments.word(0), arguments.word(1));
}

void Script_SetICval(ScenarioContext *, void *) {
    ResetInventoryControlValue9();
}

void Script_GetBatteryLifePair(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    const std::uint8_t index = static_cast<std::uint8_t>(arguments.word(1));
    const std::uint32_t value = arguments.word(2);
    const std::uint32_t result = AccessBatteryLife(mode, index, value);
    if (mode == 1)
        arguments.result(result);
}

void Script_GetOrResetScenarioStepUniqueItemCount(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    if (mode <= 1) {
        arguments.result(static_cast<std::uint32_t>(AccessScenarioStepUniqueItemCount(mode)));
    }
}

void Script_SetCurrentScrollEnabled(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    g_bSkyEnabled = static_cast<std::int32_t>(*static_cast<const std::uint32_t *>(rawParams));
}



void Script_SetEmulatorFlags(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    if (mode == 0) {
        g_emulatorSession.credits = static_cast<std::int32_t>(arguments.word(1));
    } else if (mode == 1) {
        arguments.result(static_cast<std::uint32_t>(g_emulatorSession.credits));
    } else if (mode == 2) {
        std::uint32_t score = 0;
        for (const std::uint8_t digits : g_emulatorSession.lastScoreBcd)
            score = (score * 10U + (digits >> 4U)) * 10U + (digits & 0x0fU);
        arguments.result(score >= 10000000U ? 1U : 0U);
    } else if (mode == 3) {
        arguments.result(static_cast<std::uint32_t>(g_emulatorSession.flags));
    }
}

void Script_GetCurrentDrawMode(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(static_cast<std::uint32_t>(SetCharacterDrawMode(arguments.word(0), arguments.signedWord(1))));
}

void Script_motmSetBlendFrames(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    if (!arguments.word(0))
        return;
    Character *const character = CHID_Find(context->task->task, arguments.word(0));
    if (!character)
        return;
    if (!MOTM_GetWork(character)) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    MOTI_SetBlendFrameCount(character, static_cast<std::uint16_t>(arguments.word(1)));
}

void Script_motiSetAngFlags(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(1);
    MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    HLib::Task *const scenarioTask = control ? control->scenarioTask : nullptr;
    Character *const character = CHID_Find(context->task->task, arguments.word(0));
    if (!character)
        return;
    if (!MOTM_GetWork(character)) {
        SCEN_PromoteCharacterIndexNode(scenarioTask, character);
        return;
    }
    if (mode <= 1)
        MOTI_SetSlowAngleRamp(character, mode == 1);
}


void Script_GetCycleManagerDeltaTime(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(static_cast<float>(static_cast<std::uint32_t>(static_cast<std::int64_t>(g_LastFrameTime - QueryFrameTime()))));
}

void Script_GetFrameTime(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const float cycleDelta = static_cast<float>(static_cast<std::uint32_t>(static_cast<std::int64_t>(g_LastFrameTime - QueryFrameTime())));
    arguments.result(static_cast<float>(static_cast<double>(arguments.scalar(0) - cycleDelta) * 0.08));
}

void Script_LoadSND_Wrapper(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    std::array<const char *, 8> filenames{};
    for (std::size_t index = 0; index < filenames.size(); ++index)
        filenames[index] = arguments.pointer<const char>(index);
    SND_LoadSND(filenames);
}

void Script_cassetteUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(ConsumeCassetteUtilityResult(g_isPlayerGameStateActive != 0));
}

void Script_FreeTaskData(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    HND_UnregisterMotion(arguments.word(0));
}

void Script_BeginClipTween(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = CHID_Find(context->task->task, arguments.word(0));
    if (!character || !MOTM_GetWork(character))
        return;
    FaceClipControllerState *const clip = CLIP_GetWork(character);
    if (!clip)
        return;
    if (!arguments.word(1)) {
        clip->field54 = 0;
        return;
    }
    const std::uint32_t target = arguments.word(2);
    const std::int32_t duration = arguments.signedWord(3);
    std::memcpy(&clip->field4c, &target, sizeof(target));
    const std::uint16_t durationWord = static_cast<std::uint16_t>(duration);
    std::memcpy(&clip->field44, &durationWord, sizeof(durationWord));
    const std::int32_t step = static_cast<std::int32_t>(target - clip->rotationY) / duration;
    clip->field54 = 1;
    std::memcpy(reinterpret_cast<std::byte *>(&clip->field4c) + sizeof(target), &step, sizeof(step));
    clip->field42 = 0;
}

void Script_ConfigureClipFlags(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);

    Character *const character = CHID_Find(context->task->task, arguments.word(0));
    if (MOTM_GetWork(character))
        FACE_ConfigureClip(character, static_cast<std::int8_t>(arguments.word(1)), (arguments.word(2) & 1U) != 0);
}

void Script_motDetailFlagUtil(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = arguments.word(0) ? CHID_Find(context->task->task, arguments.word(0)) : nullptr;
    if (!character)
        return;
    if (!MOTM_GetWork(character)) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    std::uint32_t result = 0;
    if (MOTI_AccessDetailFlag(character, arguments.word(1) != 0, arguments.word(2), result))
        arguments.result(result);
}

void Script_SetPackTaskFlags(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    MAPCTRL_SetCharacterIdPackCollision(arguments.signedWord(0), PACK_GetReadyCollisionData());
}

void Script_ProcessSubtitleText(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    SCNT_ShowSubtitleText(context && context->task ? context->task->task : nullptr, arguments.pointer<const char>(0), 2);
}

void Script_ShowUIMessageFromID(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    CVWKRuntimeSlot *const slot = ProcessSubtitleFile(arguments.pointer<const char>(0), 0, 0, nullptr);
    arguments.result(HLib_GetTaskDataOffset(slot));
}

void Script_SetMaxNPCsNear(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::int32_t previous = g_MaxEntitiesPerType;
    g_MaxEntitiesPerType = static_cast<std::int32_t>(arguments.word(0));
    arguments.result(static_cast<std::uint32_t>(previous));
}

void Script_GetCycleManagerState(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    if (!g_cycleShopExitPublished) {
        arguments.result(std::uint32_t{0});
        return;
    }
    std::uint32_t *const primary = arguments.pointer<std::uint32_t>(0);
    std::uint32_t *const secondary = arguments.pointer<std::uint32_t>(1);
    if (primary) {
        primary[0] = static_cast<std::uint32_t>(g_cycleShopExitPositionXY);
        primary[1] = static_cast<std::uint32_t>(g_cycleShopExitPositionXY >> 32U);
        primary[2] = static_cast<std::uint32_t>(g_cycleShopExitPositionZ);
    }
    if (secondary) {
        secondary[0] = 0;
        secondary[1] = static_cast<std::uint32_t>(g_cycleShopExitYaw);
        secondary[2] = 0;
    }
    arguments.result(static_cast<std::uint32_t>(g_cycleShopExitPublished));
}

void Script_UpdateCCOWCandidateObjects(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(CCOW_UpdateCandidateObject(ScenarioCharacter(context, arguments.word(0)), arguments.signedWord(1), arguments.word(2)));
}

void Script_SetDynamicMapTransform(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::int32_t index = static_cast<std::int32_t>(arguments.word(0));
    if (index < 0 || index >= static_cast<std::int32_t>(g_dynamicMapSlots.size()))
        return;
    DynamicMapEntry &transform = g_dynamicMapSlots[static_cast<std::size_t>(index)];
    if (const Vector3 *const position = arguments.pointer<Vector3>(1))
        transform.position = *position;
    transform.field_44 = static_cast<std::int32_t>(arguments.word(2));
}

void Script_SetCycleManagerData(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const auto *const position = arguments.pointer<const std::array<std::uint32_t, 3>>(1);
    const auto *const rotation = arguments.pointer<const std::array<std::uint32_t, 3>>(2);
    if (!position || !rotation)
        return;
    MAPCTRL_SetCharacterIdData(arguments.signedWord(0), *position, *rotation, arguments.word(3), COLS_FindQuerySet(arguments.word(4)));
}

void Script_ReallocateUNFO(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    ReallocateUNFO(arguments.signedWord(0));
}

void Script_EVT_SetFacingTowardPoint(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const Vector3 *const target = arguments.pointer<Vector3>(2);
    const Vector3 origin = character ? character->entity.node.position : Vector3{};
    const std::int32_t yaw = atan2_fixed16(-(target->x - origin.x), -(target->z - origin.z));
    const Vector3i rotation{0, yaw, 0};
    arguments.result(static_cast<std::uint32_t>(ApplyScriptEulerFlags(character, arguments.signedWord(1), &rotation)));
}

void Script_MoveObjectToObject(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const object = ScenarioCharacter(context, arguments.word(0));
    if (!object)
        return;
    const Vector3 position = GetPositionForEntity(object->entity.node);
    const Vector3i rotation = GetEulerAnglesForEntity(object->entity.node);
    CharacterNode *parent = nullptr;
    const std::uint32_t parentId = arguments.word(1);
    if (parentId <= 31U) {
        if (g_mapControlTask && g_mapControlTask->task_data_ptr)
            parent = &reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->characterIdTable[parentId].node;
    } else if (parentId != 0xffffffffU) {
        if (Character *const target = ScenarioCharacter(context, parentId))
            parent = &target->entity.node;
    }
    object->entity.node.child = parent;
    shenApplyTranslationToAll(object->entity.node, position);
    ApplyEulerAnglesToNode(object->entity.node, rotation);
}

void Script_ptmvControlTrailFollow(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const owner = ScenarioCharacter(context, arguments.word(1));
    if (!owner)
        return;
    const std::int32_t sourceKey = arguments.signedWord(0);
    PTMV_ControlTrailFollow(owner, sourceKey == -1 ? nullptr : ScenarioCharacter(context, static_cast<std::uint32_t>(sourceKey)),
        static_cast<std::uint8_t>(arguments.word(2)), sourceKey == -1);
}

void Script_CreateNPCImageModule(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (!character)
        return;
    const char *const modelName = arguments.pointer<char>(1);
    if (!modelName) {
        IMGM_ReleaseForNullModelScript(character);
        return;
    }
    IMGM_SetupForScript(character, modelName, arguments.word(2));
}

void Script_CopyAndModifyChid(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    if (const char *const name = arguments.pointer<char>(0))
        (void)MDLM_UnloadImage(name);
}

void Script_ScenDataUtil(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    std::byte *const data = arguments.pointer<std::byte>(0);
    if (!data)
        return;

    const std::ptrdiff_t index = static_cast<std::int32_t>(arguments.word(2));
    const std::uint32_t value = arguments.word(3);
    switch (arguments.word(1)) {
    case 1:
        arguments.result(std::to_integer<std::uint8_t>(data[index]));
        break;
    case 2:
        arguments.result(reinterpret_cast<std::uint16_t *>(data)[index]);
        break;
    case 4:
    case 5:
        arguments.result(reinterpret_cast<std::uint32_t *>(data)[index]);
        break;
    case 8: {
        const std::int32_t offset = reinterpret_cast<std::int32_t *>(data)[index];
        const auto *const pointer = reinterpret_cast<const std::byte *>(context->task->activeInfo) + offset;
        arguments.result(HLib_GetTaskDataOffset(pointer));
        break;
    }
    case 11:
        data[index] = static_cast<std::byte>(value);
        break;
    case 12:
        reinterpret_cast<std::uint16_t *>(data)[index] = static_cast<std::uint16_t>(value);
        break;
    case 14:
    case 15:
    case 18:
        reinterpret_cast<std::uint32_t *>(data)[index] = value;
        break;
    default:
        break;
    }
}


void Script_SetSearchFov(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    VFMA_SetFieldOfViewForSearchOrClear(static_cast<std::uint8_t>(arguments.word(0)));
}

void Script_RequestFightAnimation(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(0U);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (!character)
        return;
    if (!MOTM_GetWork(character)) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    arguments.result(static_cast<std::uint32_t>(FIGP_PlayFightAnim_Wrapper(
        character, static_cast<std::uint16_t>(arguments.word(1)), static_cast<std::int16_t>(arguments.word(3)), arguments.scalar(2), arguments.word(4))));
}

void Script_shenCashUtils(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    UI_ICMoShowCurrentBalance(*static_cast<const std::uint8_t *>(rawParams) != 0);
}

void Script_GetConversationDetails(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    switch (arguments.word(0)) {
    case 0:
        arguments.result(static_cast<std::uint32_t>(g_isPlayerGameStateActive));
        break;
    case 1:
        UI_ResetConversationDetailsState();
        break;
    case 2:
        UI_ICMoWaitCloseAnimation();
        break;
    case 4:
        g_scenarioConversationPromptEnabled = true;
        break;
    case 5:
        g_scenarioConversationPromptEnabled = false;
        break;
    case 6:
        UI_ResetForCPCT();
        break;
    case 7:
        UI_CloseConversationUIAndReturnToFreeRoam();
        break;
    case 8: {
        const std::int32_t itemId = arguments.signedWord(1);
        if (itemId == -1) {
            if (UI_ICLISharedItemMenuIsActive())
                UI_ICLIResetSharedItemMenu();
        } else {
            UI_ICLISetSharedItemPreview(static_cast<std::int16_t>(itemId));
        }
        break;
    }
    case 9:
        arguments.result(UI_ICLISharedItemMenuIsActive());
        break;
    case 10:
        EnqueueTMEX();
        break;
    case 11:
        DestroyTMEX();
        break;
    case 12:
        arguments.result(static_cast<std::uint32_t>(g_TMEX_State));
        break;
    case 13:
        UI_StopPlayingCassettes();
        break;
    default:
        break;
    }
}

void Script_GetRemainingFrames(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(VFMA_GetRemainingFrames());
}

void Script_CheckTaskStatus(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    arguments.result(VFMA_GetCharacterFightFlags(character));
}

void Script_maprLoadRes(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    switch (arguments.word(0)) {
    case 6:
        ReadMAPR(arguments.pointer<char>(1), arguments.pointer<char>(2), arguments.signedWord(3));
        break;
    case 7:
        FreeClip_PREP(arguments.signedWord(1));
        break;
    default:
        break;
    }
}

void Script_RunAuthorAndSetState(ScenarioContext *context, void *rawParams) {
    if (!rawParams)
        return;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    (void)CAME_BuildScriptAuthorSequence(params, g_pCurrentTaskData);
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_previousAuthorState = 14;
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    ApplyPendingGameState(14);
    Character *const targetOwner = ScenarioCharacter(context, params[9]);
    Character *const eyeOwner = ScenarioCharacter(context, params[8]);
    CAME_StartBuiltScriptAuthorSequence(eyeOwner, targetOwner);
}

void Script_walk3D(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t opcode = arguments.word(0);
    if (opcode == 0x57334403U) {
        const std::uint32_t *const values = static_cast<const std::uint32_t *>(rawParams) + 3;
        W3DW_EditPathPoint(arguments.pointer<Walk3DPathKey>(1), arguments.signedWord(2), values);
        return;
    }
    Character *const character = ScenarioCharacter(context, arguments.word(1));
    switch (opcode) {
    case 0x57334400U:
        W3DW_SetupPath(character, arguments.pointer<Walk3DPathKey>(2), arguments.word(3));
        break;
    case 0x57334401U: {
        std::int32_t mode{};
        if (W3DW_Activate(character, arguments.scalar(2), mode))
            arguments.result(static_cast<std::uint32_t>(mode));
        break;
    }
    case 0x57334402U:
        arguments.result(W3DW_GetState(character));
        break;
    case 0x57334404U:
        W3DW_SetDisabled(character, true);
        break;
    case 0x57334405U:
        W3DW_SetDisabled(character, false);
        break;
    case 0x57334406U:
        W3DW_ResetSegment(character, static_cast<std::uint16_t>(arguments.word(2)));
        break;
    default:
        break;
    }
}
void Script_ReinitFightState(ScenarioContext *context, void *rawParams) {
    VFMA_ReinitializeFightScriptState(context, rawParams);
}

void Script_EVT_ApplyRot(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    OBJE_ApplyPivotedReflection(arguments.pointer<Matrix4x4>(0), arguments.pointer<Vector3i>(1));
}

void Script_srchUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    if (mode == 0) {
        SRCH_InitEx(arguments.pointer<SearchEntry>(1));
        arguments.result(0xffffffffU);
        return;
    }

    const SearchCommand command = static_cast<SearchCommand>(mode);
    if (command == SearchCommand::SetEntrySelectorFilter || command == SearchCommand::SetExplicitTargetMapInfoId) {
        arguments.result(SRCH_ExecuteCommand(command, arguments.signedWord(1)));
        return;
    }

    arguments.result(SRCH_ExecuteCommand(command));
}

void Script_PTTMRelated(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const follower = ScenarioCharacter(context, arguments.word(1));
    if (!follower)
        return;
    if (arguments.signedWord(0) == -1) {
        PTTM_Stop(follower);
        return;
    }
    Character *const source = ScenarioCharacter(context, arguments.word(0));
    PTTM_AssignSource(follower, source, static_cast<std::uint8_t>(arguments.word(2)));
}

void Script_UpdatePathConstraints(ScenarioContext *context, void *rawParams) {
    static constexpr std::array<Vector2, 4> curves{{
        {1.5517239570617676f, -0.07388400286436081f},
        {1.131579041481018f, -0.029766999185085297f},
        {0.5666670203208923f, 0.008994000032544136f},
        {0.06666699796915054f, 0.030022000893950462f},
    }};

    static constexpr std::array<Vector3, 4> spheres{{
        {0.013000000268220901f, 0.0f, 0.009200000204145908f},
        {0.013000000268220901f, 0.0f, -0.009200000204145908f},
        {0.013000000268220901f, 0.009200000204145908f, 0.0f},
        {0.013000000268220901f, -0.009200000204145908f, 0.0f},
    }};

    const ScriptArguments arguments(context, rawParams);
    Vector3 *const position = arguments.pointer<Vector3>(0);
    Vector3 *const velocity = arguments.pointer<Vector3>(1);
    const Vector3 *const origin = arguments.pointer<Vector3>(2);
    const std::uint32_t rawYaw = arguments.word(3);
    const float sine = sin_fixed16(static_cast<std::int16_t>(rawYaw));
    const std::uint32_t yawWord = rawYaw & 0xffffU;
    const std::uint32_t foldedYaw = yawWord <= 0x8000U ? yawWord : 0x10000U - yawWord;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - foldedYaw));
    const float localX = position->x - origin->x;
    const float localY = position->y - origin->y;
    const float localZ = position->z - origin->z;

    PathConstraintState state{
        {cosine * localX + sine * localZ, localY},
        {cosine * localZ - sine * localX, velocity->x},
        {velocity->y, velocity->z},
        1.0f / 30.0f,
    };
    const float radialSquared = state.tangent.x * state.tangent.x + state.position.x * state.position.x;
    const float velocitySquared = state.velocity.x * state.velocity.x + state.tangent.y * state.tangent.y + state.velocity.y * state.velocity.y;
    std::int32_t statusBits = 0;
    std::uint32_t quadrant = 0;
    const float height = state.position.y - (std::sqrt(radialSquared) * 0.066666998f + 0.030022001f);
    if (!(0.001 > static_cast<double>(height)) || !(0.0001 > static_cast<double>(velocitySquared)) || !(0.00033124 > static_cast<double>(radialSquared))) {
        for (std::uint32_t i = 0; i < 10; ++i) {
            if (!COLS_UpdatePathWithConstraints(state, statusBits, curves, spheres))
                break;
        }
    } else {
        state.tangent.y = 0.0f;
        state.velocity = {};
        quadrant = static_cast<std::uint32_t>((state.tangent.x < 0.0f ? 2 : 0) + (state.position.x < 0.0f ? 1 : 0) + 1);
    }

    position->x = origin->x + (state.position.x * cosine - state.tangent.x * sine);
    position->y = state.position.y + origin->y;
    position->z = (state.tangent.x * cosine + state.position.x * sine) + origin->z;
    velocity->x = static_cast<float>(static_cast<double>(state.tangent.y) * 0.98);
    velocity->y = static_cast<float>(static_cast<double>(state.velocity.x) * 0.98);
    velocity->z = static_cast<float>(static_cast<double>(state.velocity.y) * 0.98);
    arguments.result(quadrant + (static_cast<std::uint32_t>(statusBits) << 8U));
}

void Script_InvestigationUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t offset = arguments.word(1);
    const std::uint32_t type = arguments.word(2);
    const auto taskRelativePointer = [offset]() -> void * {
        if (!offset)
            return nullptr;
        return g_pCurrentTaskData + std::bit_cast<std::int32_t>(offset);
    };

    std::int32_t result = -1;
    switch (arguments.word(0)) {
    case 0:
        result = offset || type ? shenResetCamInteractStates(ScenarioCharacter(context, offset), ScenarioCharacter(context, type)) :
                                  shenApplyTransformsForInteractStates();
        break;
    case 1:
        result = CACH_BeginTargetMode(ScenarioCharacter(context, offset), 0xffffU);
        break;
    case 2:
        result = CACH_RequestTargetTransition(2);
        break;
    case 3:
        result = CACH_RequestTargetTransition(3);
        break;
    case 4:
        result = CACH_UpdateTargetModeStateMachine();
        break;
    case 5:
        result = CACH_UpdateInterpolatedTargetSpeed();
        break;
    case 6:
        result = CACH_BeginTargetMode(ScenarioCharacter(context, offset), static_cast<std::uint16_t>(type));
        break;
    case 11:
        result = CACH_BeginInteractionMode(
            type == 60 ? taskRelativePointer() : static_cast<void *>(ScenarioCharacter(context, offset)), std::bit_cast<std::int32_t>(type));
        break;
    case 12:
        result = CACH_RequestInteractionExit();
        break;
    case 13:
        result = CACH_PollInteractionMode();
        break;
    case 14:
        result = CACH_LoadSpecialAssets(static_cast<const SpecialAssetScriptRecord *>(taskRelativePointer()));
        break;
    case 15:
        result = CACH_UpdateSpecialAssetInput();
        break;
    case 16:
        result = CACH_SetSpecialAssetList(taskRelativePointer());
        break;
    case 21:
        CACH_QueueSoundCommands(offset, type);
        result = 0;
        break;
    default:
        break;
    }
    arguments.result(static_cast<std::uint32_t>(result));
}

void Script_patrolUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    if (Character *const character = ScenarioCharacter(context, arguments.word(0)))
        arguments.result(static_cast<std::uint32_t>(PATW_HandleCommand(character, static_cast<const std::int32_t *>(rawParams))));
}

void Script_AuthorUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t index = arguments.word(1);
    if (index > 69)
        return;
    switch (arguments.word(0)) {
    case 0:
        LoadAuthorSequenceTrack(static_cast<std::int32_t>(index), arguments.pointer<const char>(2), arguments.pointer<const char>(3));
        break;
    case 1:
        FreeAuthorSequenceTrack(static_cast<std::int32_t>(index));
        break;
    default:
        break;
    }
}

void Script_InitMOTMGlobalState(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    ScenarioSelectorState &selection = g_scenarioSelector;
    selection.owner = ScenarioCharacter(context, arguments.word(0));
    selection.rig = ScenarioCharacter(context, arguments.word(1));
    selection.selected = ScenarioCharacter(context, arguments.word(2));
    if (selection.rig) {
        selection.rigBasePosition = selection.rig->entity.node.position;
        selection.rigBaseRotation = selection.rig->entity.node.rotation;
    } else {
        selection.rigBasePosition = {};
    }
    BuildSelectionCameraAnchors(selection);
    selection.selectedSlot = 3;
    selection.previousSlot = 3;
    selection.frame = 0;
    selection.state = CharaSelectState::Idle;
}

void Script_wmptUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    MotionManagerWork *const manager = MOTM_GetWork(character);
    arguments.result(0U);
    if (!character)
        return;
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    if (static_cast<std::uint32_t>(manager->motion.pendingAnimId) == arguments.word(2)) {
        if (const Vector3 *const target = arguments.pointer<Vector3>(1)) {
            WMPT_Request(character, *target, arguments.word(2), -1.0f);
        }
        arguments.result(1U);
    } else {
        arguments.result(WMPT_IsActive(character) ? 1U : 0U);
    }
}

void Script_motmUtilsEx(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    if (mode == 5U) {
        arguments.result(static_cast<std::uint32_t>(MOTN_getFrameCountFromAnimId(static_cast<std::uint16_t>(arguments.word(2)))));
        return;
    }
    if (mode == 7U) {
        arguments.result(MOTN_GetAnimationFlag(static_cast<std::uint16_t>(arguments.word(2))));
        return;
    }

    Character *const character = ScenarioCharacter(context, arguments.word(1));
    if (!character) {
        arguments.result(0U);
        return;
    }
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }

    MotionInformationWork &motion = manager->motion;
    std::uint32_t result = 0;
    switch (mode) {
    case 0:
        motion.blendMode = static_cast<std::uint8_t>(arguments.word(2));
        break;
    case 1:
        result = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(motion.blendMode)));
        break;
    case 2:
        result = static_cast<std::uint32_t>(MOTN_getFrameCountFromAnimId(motion.currentAnimId));
        break;
    case 3:
        result = motion.currentAnimId;
        break;
    case 4:
        result = static_cast<std::uint32_t>(static_cast<std::int32_t>(MOTI_GetPlayRateScaledRemain(motion)));
        break;
    case 6:
        motion.pendingTurnEulerDelta.y = arguments.signedWord(2);
        break;
    case 8:
        result = motion.motionFlags & 4U;
        break;
    case 9:
        result = static_cast<std::uint32_t>(static_cast<std::int32_t>(motion.headLookTimer));
        break;
    case 10:
        result = motion.motionFlags & 0x10000U;
        break;
    case 11:
        motion.rootAngleFlags |= 0x200000U;
        break;
    case 12:
        motion.rootAngleFlags &= ~0x200000U;
        break;
    case 13:
        result = MOTI_RefreshInterpolationState(motion) ? 1U : 0U;
        break;
    case 14:
        MOTI_SetRootTransformBlendBits(motion, arguments.word(2));
        break;
    default:
        break;
    }
    arguments.result(result);
}

void Script_IsMoveTargetRequestActive(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(charaIsActiveMode(ScenarioCharacter(context, arguments.word(0)), 0) ? 1U : 0U);
}

void Script_SetLampState(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    LAMP_SetScriptState(ScenarioCharacter(context, arguments.word(1)), arguments.word(0), arguments.word(2));
}

void Script_shenGetNode(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    Mt5RuntimeNode *const node =
        shenGetNodeByChildIdx(static_cast<Mt5RuntimeNode *>(character ? character->model : nullptr), static_cast<std::int16_t>(arguments.word(1)));
    arguments.result(node ? static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(node)) : 0U);
}

void Script_sicnUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    UI_SICNDispatch(
        arguments.signedWord(0), static_cast<std::uint16_t>(arguments.word(1)), static_cast<UISicnPausePolicy>(static_cast<std::uint8_t>(arguments.word(2))));
}

void Script_GetBlurModel(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    const char *const filename = arguments.pointer<char>(1);
    if (character && filename)
        FIGP_LoadBlurModel(character, filename);
}

void Script_dynamicStuff(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (!character)
        return;
    const char *const modelName = arguments.pointer<char>(1);
    if (!modelName) {
        IMGM_ReleaseForNullModelScript(character);
        return;
    }
    const ImageModuleWork *const previousImage = IMGM_GetWork(character);
    const std::uint16_t previousTemplate = previousImage ? previousImage->basePoseId : 0;
    MDLM_ActivateCharacterImage(character, modelName, arguments.word(2));
    if (MotionManagerWork *const manager = MOTM_GetWork(character)) {
        const ImageModuleWork *const image = IMGM_GetWork(character);
        if ((image ? image->basePoseId : 0) != previousTemplate)
            MOTI_Free(manager->motion);
        CLMD_Flag(character);
        OSAGE_Refresh(character);
    }
    MT5_PrepareStaticCharacterModel(character->model);
}

void Script_ConversationUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    switch (arguments.word(0)) {
    case 0: {
        HLib::Task *const scenarioTask = context && context->task ? context->task->task : nullptr;
        Character *const participantA = arguments.word(2) ? CHID_Find(scenarioTask, arguments.word(2)) : nullptr;
        Character *const participantB = arguments.word(3) ? CHID_Find(scenarioTask, arguments.word(3)) : nullptr;
        convInitStates(arguments.signedWord(1), participantA, participantB, arguments.scalar(4), arguments.scalar(5));
        break;
    }
    case 1:
        g_conversationCueParticipantSign = arguments.signedWord(1);
        break;
    case 2:
        arguments.result(static_cast<std::int32_t>(static_cast<std::int8_t>(g_Conv.cueEndReached)));
        break;
    default:
        break;
    }
}

void Script_gachaponUtils(ScenarioContext *context, void *rawParams) {
    const auto *const parameters = static_cast<const GCPNScriptParameters *>(rawParams);
    const std::int32_t result = parameters ? GCPN_ExecuteScript(context, *parameters) : -1;
    if (context && context->stack)
        context->stack->accumulator = static_cast<std::uint32_t>(result);
}

void Script_TimedTransitionUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    switch (arguments.word(0)) {
    case 2:
        g_scenarioTimedTransitionEnabled = arguments.word(1) != 0;
        arguments.result(g_scenarioTimedTransitionEnabled);
        break;
    case 3:
        arguments.result(g_scenarioTimedTransitionEnabled);
        break;
    case 4: {
        TimedTransitionParameters parameters{};
        if (rawParams)
            std::memcpy(&parameters, rawParams, sizeof(parameters));
        arguments.result(RegisterTimedTransition(parameters));
        break;
    }
    default:
        arguments.result(0xffffffffU);
        break;
    }
}

void Script_wazaUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint16_t animationSet = static_cast<std::uint16_t>(arguments.word(1));
    const std::uint16_t moveId = static_cast<std::uint16_t>(arguments.word(2));
    switch (arguments.word(0)) {
    case 0:
        EnqueueWZOG();
        break;
    case 1:
        shenRefreshUI();
        break;
    case 2:
        SYGH_SetMoveSkill(animationSet, moveId, static_cast<std::uint16_t>(arguments.word(3)));
        break;
    case 3:
        SYGH_RemoveMoveSkill(animationSet, moveId);
        break;
    case 4:
        EnqueueEWZO(0x80U, 0, 0);
        break;
    case 5:
        EnqueueEWZO(0x81U, 0, 0);
        break;
    case 6:
        arguments.result(static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(SYGH_GetMoveSkill(animationSet, moveId)))));
        break;
    case 7:
        EnqueueEWZO(0x82U, 0, 0);
        break;
    default:
        break;
    }
}

void Script_IsPlayerAllowed(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(SCEN_IsPlayerAllowed() ? 0xffffffffU : 0U);
}

void Script_aicaGetOrSetState(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(arguments.word(0) <= 7 ? SOND_AccessAicaMemory(static_cast<std::uint8_t>(arguments.word(0))) : SOND_ConsumeAicaEvent());
}

void Script_InitINSHForScene(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    auto *const position = arguments.pointer<Vector3>(0);
    auto *const rotation = arguments.pointer<Vector3i>(1);
    if (!position || !rotation)
        return;
    std::int32_t yaw = rotation->y;
    const std::int32_t result = CycleManager::InitINSHForScene(arguments.word(2), *position, yaw);
    *rotation = {0, yaw, 0};
    arguments.result(static_cast<std::uint32_t>(result));
}
} // namespace scen_detail

void Script_EnqueueSCNT(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    const std::uint32_t mode = params[0];
    const auto finish = [context]() {
        g_scenarioStepLut.pendingCharacterName = 0;
        context->stack->accumulator = 0;
    };

    if (mode == 0) {
        finish();
        return;
    }
    if (mode == 1) {
        if (ScenRuntimeObjectBusy()) {
            finish();
            return;
        }
        g_scenarioStepLut.pendingCharacterName = params[1];
        EnqueueSCNT(params[1], static_cast<std::int16_t>(params[2]));
        if (ScenRuntimeObjectBusy()) {
            context->stack->accumulator = 1;
            return;
        }
        UpdateScenActiveRuntimeObject();
        finish();
        return;
    }

    const std::uint32_t pendingName = g_scenarioStepLut.pendingCharacterName;
    if (!pendingName) {
        context->stack->accumulator = 0;
        return;
    }
    if (ScenRuntimeObjectBusy()) {
        if (g_scenRuntimeState.activeObjectName == pendingName) {
            context->stack->accumulator = 1;
            return;
        }
        finish();
        return;
    }
    UpdateScenActiveRuntimeObject();
    finish();
}

void Script_SpriteUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const SpriteUtilParams *>(rawParams);
    if (!params)
        return;

    switch (params->mode) {
    case SpriteUtilCommand::ReadSky: {
        const char *const filename = params->offset ? reinterpret_cast<const char *>(g_pCurrentTaskData + params->offset) : nullptr;
        Shenmue_ReadSprite_Wrapper(context, filename);
        break;
    }
    case SpriteUtilCommand::SetVerticalOffset:
        g_spriteVerticalOffset = params->value;
        break;
    case SpriteUtilCommand::SetHorizontalOffset:
        g_spriteHorizontalOffset = params->value;
        break;
    case SpriteUtilCommand::ReadSlot: {
        const char *const filename = params->offset ? reinterpret_cast<const char *>(g_pCurrentTaskData + params->offset) : nullptr;
        ReadSprite_Wrapper(context, filename, static_cast<std::int32_t>(params->slotOrIntensity));
        break;
    }
    case SpriteUtilCommand::LoadSlotAsSky:
    case SpriteUtilCommand::ModifySky: {
        std::int32_t value = 0;
        std::memcpy(&value, &params->value, sizeof(value));
        if (params->mode == SpriteUtilCommand::LoadSlotAsSky)
            LoadSCRL1(value, params->offset);
        else
            ControlSkyTextureFlags(params->offset, params->slotOrIntensity, value);
        break;
    }
    }
}
} // namespace shenmue::s1
