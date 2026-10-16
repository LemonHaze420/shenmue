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

void Script_SetCameraFieldOfView(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    const std::int32_t fieldOfView = *static_cast<const std::int32_t *>(rawParams);
    if (!bLockCamera)
        FieldOfView = fieldOfView;
    CAME_SetFieldOfView(fieldOfView);
}

void Script_seqEnterEvtCam(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const EventCameraParameters *>(rawParams);
    if (!context || !context->task || !params)
        return;
    const auto find = [context](shen_name id) { return id.name ? CHID_Find(context->task->task, id.name) : nullptr; };
    const std::int32_t sequenceIndex = static_cast<std::int32_t>(params->sequenceIndex);
    Character *const target = find(params->targetCharacterId);
    Character *const eye = find(params->eyeCharacterId);
    nCamState |= 2U;
    g_pASEQ_charas[0] = eye;
    ASEQ_current_chunk_ptr = reinterpret_cast<std::uint32_t *>(static_cast<std::intptr_t>(sequenceIndex));
    g_pCEWP = target;
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 6;
    ApplyPendingGameState(6);
}

void Script_IsInEventCam(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return;
    context->stack->accumulator = CAME_GetAnimatedSequenceBusy() && g_nAuthorState == 6 ? 1U : 0U;
}

void Script_UpdateGameState_Camera(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    nCamState |= 2U;
    std::memcpy(&ASEQ_current_chunk_ptr, rawParams, sizeof(void *));
    std::memcpy(&g_pCEWP, static_cast<const std::byte *>(rawParams) + sizeof(void *), sizeof(void *));
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_previousAuthorState = 16;
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    ApplyPendingGameState(16);
}

void Script_seqGetCamInterpActive(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return;
    context->stack->accumulator = static_cast<std::uint32_t>(CAME_GetInterpolationActive());
}

void Script_SetCameraSettings(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const auto *const params = static_cast<const CameraSettingsParameters *>(rawParams);
    float previous = 0.0f;
    switch (params->mode) {
    case 0:
        previous = CameraDistanceFromPlayer;
        if (!(params->value < 0.0f))
            CameraDistanceFromPlayer = params->value;
        break;
    case 1:
        previous = CameraHeightAbovePlayer;
        if (!(params->value < 0.0f)) {
            CameraHeightAbovePlayer = params->value;
            CameraHeightAbovePlayer_MAX = params->value;
        } else {
            CameraHeightAbovePlayer_MAX = CameraHeightAbovePlayer;
        }
        break;
    case 2:
        previous = CAME_SetCameraYAngle(params->value);
        break;
    default:
        break;
    }
    context->stack->accumulator = std::bit_cast<std::uint32_t>(previous);
}

void Script_SetFogFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FogFlagsParameters *>(rawParams);
    switch (params->mode) {
    case 0:
        if (const auto *const source = ScenarioPointer<std::uint32_t>(params->offset))
            std::memcpy(g_cloudXY, source, sizeof(g_cloudXY));
        break;
    case 1:
        if (auto *const destination = ScenarioPointer<std::uint32_t>(params->offset))
            std::memcpy(destination, g_cloudXY, sizeof(g_cloudXY));
        break;
    case 2:
        g_shouldOverrideClearColour = 1;
        g_overrideClearColour = params->offset;
        break;
    case 3:
        context->stack->accumulator = g_overrideClearColour;
        break;
    case 4:
        g_shouldOverrideClearColour = params->offset;
        break;
    case 5:
        context->stack->accumulator = g_shouldOverrideClearColour;
        break;
    default:
        break;
    }
}

void Script_SetFogColour(ScenarioContext *, void *rawParams) {
    const auto offset = *static_cast<const std::uint32_t *>(rawParams);
    const auto *const fog = ScenarioPointer<FogColourData>(offset);
    if (!fog)
        return;
    g_fogR = fog->red;
    g_fogG = fog->green;
    g_fogB = fog->blue;
    g_fogColour = fog->colour;
    g_bFogEnabled = 1;
    if (g_TaskInfo.CurrentTaskIdx < 32 && CurrentTask) {
        const std::int32_t index = g_TaskInfo.CurrentTaskIdx++;
        g_TaskInfo.pWeatherTaskModule[index] = CurrentTask;
        g_TaskInfo.pWeatherTasks[index] = nullptr;
        CurrentTask->taskFn = Fog_DoFogEffectsTask;
    }
}
void Script_GetFogColour(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    FogColourData *const fog = ScenarioPointer<FogColourData>(*static_cast<const std::uint32_t *>(rawParams));
    if (!fog)
        return;
    fog->red = g_fogR;
    fog->green = g_fogG;
    fog->blue = g_fogB;
    fog->colour = g_fogColour;
}

void Script_SetFogEnabled(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    g_bFogEnabled = *static_cast<const std::uint32_t *>(rawParams) ? 1 : 0;
    if (g_TaskInfo.CurrentTaskIdx >= 32 || !CurrentTask)
        return;
    const std::int32_t index = g_TaskInfo.CurrentTaskIdx++;
    g_TaskInfo.pWeatherTaskModule[index] = CurrentTask;
    g_TaskInfo.pWeatherTasks[index] = nullptr;
    CurrentTask->taskFn = g_bFogEnabled ? Fog_DoFogEffectsTask : TaskUtil_InitStaticSize;
}

void Script_filmCheckOrSetTransitionUtil(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const FilmTransitionParameters *>(rawParams);
    auto *const control = g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    HLib::Task *const filmTask = control ? control->filmTask : nullptr;
    if (params->mode == 0xffffffffU) {
        if (g_NumFramesProcessed) {
            context->stack->accumulator = 1;
        } else {
            const auto *const film = filmTask && filmTask->task_data_ptr ?
                reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(filmTask->task_data_ptr) + 4) :
                nullptr;
            context->stack->accumulator = film && film->remainingFrames ? 1U : 0U;
        }
        return;
    }
    const ColourBlendPair blend{{static_cast<std::uint8_t>(params->redA), static_cast<std::uint8_t>(params->greenA), static_cast<std::uint8_t>(params->blueA),
                                    static_cast<std::uint8_t>(params->alphaA)},
        {static_cast<std::uint8_t>(params->redB), static_cast<std::uint8_t>(params->greenB), static_cast<std::uint8_t>(params->blueB),
            static_cast<std::uint8_t>(params->alphaB)},
        params->mode};
    ConstructColours(filmTask, &blend, ConstructColourMode::ApplyImmediately);
}

void Script_shenSetLightingMode(ScenarioContext *, void *rawParams) {
    shenSetLightingMode(*static_cast<const std::int32_t *>(rawParams));
}

void Script_SetShadowFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ShadowFlagsParameters *>(rawParams);
    Character *const character = params->characterId.name ? CHID_Find(context->task->task, params->characterId.name) : nullptr;
    switch (params->mode) {
    case 0:
        lightSetMaskBit(character, params->value, false);
        break;
    case 1:
        lightSetMaskBit(character, params->value, true);
        break;
    case 2: {
        std::uint32_t mask = 0;
        if (params->value >= 32)
            mask = 0xffffffffU;
        else if (params->value)
            mask = (1U << params->value) - 1U;
        lightSetMask(character, mask);
        break;
    }
    case 3:
        lightSetRadius(character, std::bit_cast<float>(params->value));
        break;
    case 4:
        lightSetGlobalShadowEnabled(false);
        break;
    case 5:
        lightSetGlobalShadowEnabled(true);
        break;
    case 6:
        lightSetShadowBlackness(std::bit_cast<float>(params->value));
        break;
    default:
        break;
    }
}

void Script_lightDirLightEnable(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const DirectionalLightParameters *>(rawParams);
    const Vector3 *const direction = ScenarioPointer<const Vector3>(params->directionOffset);
    lightEnableDirLight(params->lightIndex, direction);
}
void Script_lightEnablePointLightAt(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const PointLightParameters *>(rawParams);
    if (!params)
        return;
    lightEnablePointLightAt(params->lightIndex, ScenarioPointer<const Vector3>(params->positionOffset));
}
void Script_lightSpotLightEnable(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const SpotLightParameters *>(rawParams);
    if (!params)
        return;
    lightEnableSpotLight(params->lightIndex, ScenarioPointer<const Vector3>(params->positionOffset), ScenarioPointer<const Vector3>(params->directionOffset));
}

void Script_lightSetGlobalSceneLighting(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const GlobalSceneLightingParameters *>(rawParams);
    if (!params || !params->colourOffset)
        return;
    const Vector4 *const colour = ScenarioPointer<const Vector4>(params->colourOffset);
    lightSetGlobalSceneLighting(*colour);
}

void Script_lightSetSlotEnabled(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const LightSlotEnabledParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    if (params->enabled == 0xffffffffU) {
        context->stack->accumulator = static_cast<std::uint32_t>(lightSetSlotEnabled(params->lightIndex, params->enabled));
        return;
    }
    if (params->lightIndex < 128 && params->enabled <= 1)
        lightSetSlotEnabled(params->lightIndex, params->enabled);
}

void Script_mapmSetCamFlags(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const DynamicMapCameraFlagParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    if (params->mode == 0) {
        for (DynamicMapAuxiliary &map : g_dynamicMapAuxiliary)
            map.clipCellsReady = map.clipCells ? params->mapIndexOrValue : 0;
        return;
    }
    if (params->mapIndexOrValue < 0 || params->mapIndexOrValue >= 32)
        return;
    const std::size_t index = static_cast<std::size_t>(params->mapIndexOrValue);
    if (params->mode == 1) {
        DynamicMapAuxiliary &map = g_dynamicMapAuxiliary[index];
        context->stack->accumulator = map.clipCellsReady ? 0xffffffffU : 0U;
        map.clipCellsReady = map.clipCells ? params->value : 0;
    } else if (params->mode == 2) {
        DynamicMapEntry &map = g_dynamicMapSlots[index];
        context->stack->accumulator = map.occlusionEnabled ? 0xffffffffU : 0U;
        map.occlusionEnabled = static_cast<std::uint32_t>(params->value);
    }
}

void Script_LODUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const LodUtilityParameters *>(rawParams);
    if (!context || !params)
        return;
    float result = 0.0f;
    switch (params->mode) {
    case 0:
        result = CAME_SetLodDistance0(params->value);
        break;
    case 1:
        result = CAME_GetLodDistance0();
        break;
    case 2:
        result = CAME_SetLodDistance2(params->value);
        break;
    case 3:
        result = CAME_GetLodDistance2();
        break;
    case 4:
        result = CAME_SetNpcDisplayRadius(params->value);
        break;
    case 5:
        result = CAME_GetNpcDisplayRadius();
        break;
    default:
        return;
    }
    context->stack->accumulator = std::bit_cast<std::uint32_t>(result);
}

void Script_LightUtils(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const LightUtilityParameters *>(rawParams);
    if (!params)
        return;
    switch (params->mode) {
    case 0:
        if (static_cast<std::int32_t>(params->value) < 5)
            SetLightBaseType(params->lightIndex, params->value);
        break;
    case 2: {
        const Vector4 *const colour = ScenarioPointer<const Vector4>(params->value);
        SetRGBStruct(params->lightIndex, colour);
        break;
    }
    case 4:
        SetLightIntensity(params->lightIndex, std::bit_cast<float>(params->value));
        break;
    case 5:
        SetLightFalloffRangeByIndex(params->lightIndex, std::bit_cast<float>(params->value));
        break;
    case 6:
        if (params->value)
            SetLightFalloff(params->lightIndex, *ScenarioPointer<const float>(params->value));
        break;
    case 7:
        if (params->value)
            SetLightDirectionVectorByIndex(params->lightIndex, ScenarioPointer<const Vector3>(params->value));
        break;
    case 8:
        if (params->value)
            SetEffectTransformByIndex(params->lightIndex, ScenarioPointer<const Vector3>(params->value));
        break;
    case 10:
        SetLightAngleByIndex(params->lightIndex, static_cast<std::int32_t>(params->value));
        break;
    case 11:
        SetLightSpecialFlag(params->lightIndex, params->value != 0);
        break;
    default:
        break;
    }
}

void Script_lightSetGlobalLightAndKey(ScenarioContext *, void *rawParams) {
    lightSetGlobalLightAndKey(static_cast<const float *>(rawParams));
}

void Script_SetupLightFromTable(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    LGHT_Parse(*static_cast<const std::uint32_t *>(rawParams));
}

void Script_SetCamShake(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const CameraShakeParameters *>(rawParams);
    if (!params)
        return;
    CAME_SetShake(params->previous, params->current, params->damping);
}

void Script_SetCameraFlags(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    CAME_SetCameraFlags(*static_cast<const std::int32_t *>(rawParams));
}

void Script_SetCameraCollisionEnabled(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(g_CameraCollisionEnabled ? 0xffffffffU : 0U);
    if (arguments.word(0) <= 2U)
        CAME_SetCollisionMode(static_cast<std::uint8_t>(arguments.word(0)));
}

void Script_SetCamFactor(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    CAME_SetTemporaryFactor(arguments.scalar(0));
}

void Script_GetScriptedCameraState(ScenarioContext *context, void *) {
    ScriptArguments(context, nullptr).result(CAME_GetScriptedState());
}

void Script_SetNewCameraState(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    CAME_SetZoomState(arguments.signedWord(0));
}

void Script_lightResetSlotToDefaults(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    lightResetSlotToDefaults(arguments.word(0));
}

void Script_lightResetAndEnableDefaultSlot(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    lightEnableDefaultSlot(arguments.word(0));
}

void Script_SetShadowRange(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const float range = std::fmax(0.1f, arguments.scalar(0));
    lightSetShadowRange(range);
    arguments.result(range);
}

void Script_convSetCamInactiveFlag(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    g_bIsConversationCamInactive = static_cast<std::uint8_t>(arguments.word(0));
}

void Script_GetLightPosition(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t index = arguments.word(0);
    Vector3i *const output = arguments.pointer<Vector3i>(1);
    if (!output || index >= 0x80)
        return;
    Vector3 position{};
    if (GetLightPosition(index, &position))
        std::memcpy(output, &position, sizeof(position));
}

void Script_SetAnimatedCameraFov(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::int32_t mode = arguments.signedWord(0);
    if (mode == 0) {
        isScriptedCameraActive = 0;
        isPerspectiveTransitionActive = 0;
    } else if (mode == 1) {
        cameraStartPosition = g_CameraTargetPos;
        initialPerspectiveFov = currentPerspectiveFov;
        initialCameraDistance = CameraDistance(g_CameraEyePos, g_CameraTargetPos);
        isScriptedCameraActive = 1;
    } else if (mode == 2) {
        const std::int32_t horizontalFov = arguments.signedWord(1);
        const std::int32_t halfStepCount = arguments.signedWord(2) / 2;
        const std::int32_t currentHalfFov = currentPerspectiveFov / 2;
        cameraStartPosition = g_CameraTargetPos;
        cameraInterpStepsRemaining = halfStepCount;
        initialHorizontalFov = horizontalFov;
        horizontalCamDistance = CameraDistance(g_CameraEyePos, g_CameraTargetPos);
        if (!isPerspectiveTransitionActive) {
            halfViewHeightWorld = CameraFixedHalfAngleTangent(currentHalfFov) * horizontalCamDistance;
            totalViewHeightWorld = halfViewHeightWorld * 2.0f;
        }
        isPerspectiveTransitionActive = 1;
        viewHeightIncrementPerStep =
            ((CameraFixedHalfAngleTangent(horizontalFov / 2) + CameraFixedHalfAngleTangent(currentHalfFov)) * horizontalCamDistance - totalViewHeightWorld) /
            static_cast<float>(halfStepCount);
    }
}

void Script_GetAuthorState_And_SetCamPos(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    arguments.result(static_cast<std::uint32_t>(g_nAuthorState));
    if (Vector3 *const eye = arguments.pointer<Vector3>(0))
        *eye = g_CameraEyePos;
    if (Vector3 *const target = arguments.pointer<Vector3>(1))
        *target = g_CameraTargetPos;
}

void Script_convCtrlCamApply(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    const std::uint32_t mode = arguments.word(0);
    if (g_Conv.suppressCameraApply == 1 && mode == 0 && g_Conv.conversationCameraOverrideActive != 0)
        SetNewAuthorGameState(9);
    g_Conv.suppressCameraApply = static_cast<std::uint8_t>(mode);
}

void Script_EventCamReader(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    Character *const eyeOwner = ScenarioCharacter(context, arguments.word(1));
    Character *const targetOwner = ScenarioCharacter(context, arguments.word(2));
    static_cast<void>(ScenarioCharacter(context, arguments.word(3)));

    arguments.result(EventCam_Spline_ReadFCVKey(static_cast<std::int32_t>(arguments.word(0)), eyeOwner, targetOwner, targetOwner));
}

void Script_LODSnapshotUtils(ScenarioContext *context, void *rawParams) {
    const ScriptArguments arguments(context, rawParams);
    CAME_ExecuteLodSnapshotCommand(arguments.signedWord(0));
}
} // namespace scen_detail

} // namespace shenmue::s1
