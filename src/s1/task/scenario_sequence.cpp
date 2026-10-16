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

void SEQ_ConvertMLWDToUNFO(ScenarioSequence *sequence) {
    if (!sequence || sequence->header.active != 1)
        return;
    const std::int32_t trackIndex = sequence->header.currentTrackIndex;
    if (trackIndex < 0 || trackIndex >= static_cast<std::int32_t>(g_SCNCTrackSlots.size()))
        return;
    const ScenarioTrackSlot &slot = g_SCNCTrackSlots[static_cast<std::size_t>(trackIndex)];
    if (!slot.movementPayload)
        return;
    // The MLWD chunk follows its TRCK chunk, so the payload lies past the track's own size.
    const auto *const payload = static_cast<const std::byte *>(slot.movementPayload);
    std::uint32_t count = 0;
    std::memcpy(&count, payload + 4, sizeof(count));
    MOTM_UpdateSequenceNodeConstraints(payload + 8, count, sequence->frameIndex);
}

std::int64_t AnimatedSequenceNull(std::int32_t, void *, void *) {
    return 1;
}


std::int64_t ASEQ_ANIMATED_CAM(std::int32_t, void *rawData, void *) {
    if (!g_pSCNC || !g_pSCNC->header.camera || !rawData)
        return 0;
    auto *const chunk = static_cast<SequenceChunk *>(g_pSCNC->header.camera);
    if (chunk->size < 16)
        return 0;
    const auto *const data = static_cast<const CameraFunctionData *>(rawData);
    std::uint32_t count = 0;
    std::memcpy(&count, reinterpret_cast<const std::byte *>(chunk) + 12, sizeof(count));
    const std::uint32_t cameraIndex = data->offset;
    if (cameraIndex >= count || count > (chunk->size - 16) / sizeof(std::uint32_t))
        return 0;
    const auto *const offsets = reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(chunk) + 16);
    const std::uint32_t relative = offsets[cameraIndex];
    if (relative < 16 || relative >= chunk->size)
        return 0;
    std::byte *const sequence = reinterpret_cast<std::byte *>(chunk) + relative;
    nCamState |= 2U;
    std::memcpy(&g_seqRemainingKeyframes, sequence + 4, sizeof(g_seqRemainingKeyframes));
    g_seqFcvKeyframePtr = reinterpret_cast<const float *>(sequence + 8);
    CAME_SetEventCameraInput(sequence, nullptr, nullptr);
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 8;
    ApplyPendingGameState(8);
    return 0;
}

void AllocSPLI_Wrapper(AnimatedMovementRuntime *runtime) {
    for (FcvSplineReader &reader : runtime->channels) {
        if (!reader.keyCount)
            continue;
        float *const coefficients = static_cast<float *>(HLib_AllocHeapMemBlock(8LL * reader.keyCount, SHEN_NAME(SPLI)));
        if (!coefficients)
            continue;
        reader.coefficients = coefficients;
        *reinterpret_cast<std::int32_t *>(coefficients) = 0;
        reader.keyOffset = 0;
        reader.segmentKeyCount = AllocSPLI(coefficients, reader.keyCount, reader.times, reader.values, reader.slopes);
    }
    runtime->frame = 0;
}

bool AnimatedMoveObject_ReadFCVKey_Internal(AnimatedMovementRuntime *runtime, AnimatedMovementSample *output) {
    const float time = static_cast<float>(runtime->frame) / 30.0f;
    runtime->frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(runtime->frame) + 1U);
    float values[9]{};
    std::int32_t result = 0;
    for (std::int32_t axis = 0; axis < 9; ++axis) {
        if (runtime->channels[axis].keyCount)
            result |= Sequence_ReadFCVKey(&runtime->channels[axis], time, &values[axis]);
    }
    output->position = {values[0], values[1], values[2]};
    output->euler = {static_cast<std::uint16_t>(static_cast<std::int32_t>(values[3] * 8192.0f / 45.0f)),
        static_cast<std::uint16_t>(static_cast<std::int32_t>(values[4] * 8192.0f / 45.0f)),
        static_cast<std::uint16_t>(static_cast<std::int32_t>(values[5] * 8192.0f / 45.0f))};
    output->root = {values[6], values[7], values[8]};
    return result != 0;
}

std::int64_t ASEQ_ANIMATED_MOV(std::int32_t mode, void *rawData, void *rawRuntime) {
    if (!g_pSCNC || !rawData || !rawRuntime || !g_pSCNC->header.scenarioTask)
        return 0;
    AnimatedMovementRuntime *const runtime = static_cast<AnimatedMovementRuntime *>(rawRuntime);
    const AnimatedMovementFunctionData *const data = static_cast<const AnimatedMovementFunctionData *>(rawData);
    Character *const character = CHID_Find(g_pSCNC->header.scenarioTask, data->characterId);
    AnimatedMovementSample sample{};
    if (mode == 0) {
        if (!g_pSCNC->header.movement || !g_pSCNC->header.sequence)
            return 0;
        const std::byte *const chunk = static_cast<const std::byte *>(g_pSCNC->header.movement);
        std::int32_t offset;
        std::memcpy(&offset, chunk + 0x10 + 4ULL * data->movementIndex, sizeof(offset));
        float *cursor = reinterpret_cast<float *>(const_cast<std::byte *>(chunk + offset));
        const std::int8_t version = static_cast<const std::int8_t *>(g_pSCNC->header.sequence)[9];
        for (std::int32_t axis = 0; axis < 9; ++axis) {
            FcvSplineReader &reader = runtime->channels[axis];
            if (axis >= 6 && version < 5) {
                reader.keyCount = 0;
                reader.times = nullptr;
                reader.values = nullptr;
                reader.slopes = nullptr;
                continue;
            }
            std::memcpy(&reader.keyCount, cursor++, sizeof(reader.keyCount));
            reader.times = cursor;
            cursor += reader.keyCount;
            reader.values = cursor;
            cursor += reader.keyCount;
            reader.slopes = cursor;
            cursor += reader.keyCount;
        }
        runtime->savedPosition = {runtime->channels[0].values[0], runtime->channels[1].values[0], runtime->channels[2].values[0]};
        runtime->savedEuler = {static_cast<std::uint16_t>(static_cast<std::int32_t>(runtime->channels[3].values[0] * 8192.0f / 45.0f)),
            static_cast<std::uint16_t>(static_cast<std::int32_t>(runtime->channels[4].values[0] * 8192.0f / 45.0f)),
            static_cast<std::uint16_t>(static_cast<std::int32_t>(runtime->channels[5].values[0] * 8192.0f / 45.0f))};
        if (runtime->channels[6].keyCount)
            runtime->savedRoot = {runtime->channels[6].values[0], runtime->channels[7].values[0], runtime->channels[8].values[0]};
        AllocSPLI_Wrapper(runtime);
        runtime->flags |= 1U;
        runtime->delay = 0;
        AnimatedMoveObject_ReadFCVKey_Internal(runtime, &sample);
        AnimatedMoveObject_ReadFCVKey_Internal(runtime, &sample);
    } else if (mode == 1) {
        if (runtime->delay) {
            --runtime->delay;
            return 1;
        }
        if (!AnimatedMoveObject_ReadFCVKey_Internal(runtime, &sample))
            return 0;
        runtime->savedPosition = sample.position;
        runtime->savedEuler = sample.euler;
        runtime->savedRoot = sample.root;
    } else {
        if (mode == 2) {
            for (FcvSplineReader &reader : runtime->channels) {
                if (reader.keyCount)
                    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(reader.coefficients), 0);
            }
            runtime->flags &= static_cast<std::uint16_t>(~1U);
        }
        return 0;
    }
    if (character) {
        character->entity.node.position = sample.position;
        character->entity.node.rotation = sample.euler;
        if (runtime->channels[6].keyCount) {
            MotionManagerWork *const manager = MOTM_GetWork(character);
            const std::int32_t target = getMOTNType(6);
            if (manager && target != -1) {
                manager->motion.targetPositions[target] = sample.root;
                manager->motion.localTargets[target] = &manager->motion.targetPositions[target];
                manager->motion.worldTargets[target] = nullptr;
            }
        }
    }
    return 1;
}

std::int64_t ASEQ_MOTION(std::int32_t mode, void *rawData, void *rawRuntime) {
    if (!g_pSCNC || !rawData || !rawRuntime || !g_pSCNC->header.scenarioTask)
        return 0;
    const auto *const data = static_cast<const MotionFunctionData *>(rawData);
    auto *const runtime = static_cast<MotionRuntimeState *>(rawRuntime);
    Character *const character = CHID_Find(g_pSCNC->header.scenarioTask, data->characterId);
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!character || !manager)
        return 0;
    MotionInformationWork &motion = manager->motion;

    if (mode == 0) {
        if ((runtime->flags & 1) != 0) {
            motion.entityEuler = runtime->savedEntityEuler;
        } else {
            character->entity.node.position = {};
            character->entity.node.rotation = {};
            motion.entityEuler = {};
            runtime->savedMotion = 0;
            runtime->saved018 = 0;
        }
        manager->flags &= ~1u;
        motion.pendingAnimId = static_cast<std::uint16_t>(data->animationId);
        motion.motionFlags &= ~2u;
        motion.scriptStartFrame = 0.0f;
        motion.forcedFrameTime = static_cast<float>(static_cast<std::uint32_t>(data->firstFrame) - 1U);
        motion.scriptEndFrame = static_cast<float>(static_cast<std::uint32_t>(data->lastFrame) - 1U);
        motion.playbackRate = 1.0f;
        motion.motionFlags |= 0x01000000u;
        const auto *const sequenceBytes = static_cast<const std::uint8_t *>(g_pSCNC->header.sequence);
        if (sequenceBytes && static_cast<std::int8_t>(sequenceBytes[9]) >= 4 && (data->flags & 1) != 0)
            motion.motionFlags |= 0x80u;
        runtime->flags |= 6u;
        runtime->framesRemaining = static_cast<std::int16_t>(static_cast<std::uint16_t>(data->lastFrame) - static_cast<std::uint16_t>(data->firstFrame));
        return 1;
    }

    if (mode == 1) {
        runtime->flags &= static_cast<std::uint16_t>(~4u);
        if (runtime->framesRemaining-- == 1) {
            motion.playbackRate = 0.0f;
            return 0;
        }
        return 1;
    }

    if (mode == 3) {
        manager->flags |= 1u;
        motion.motionFlags &= ~0x01000000u;
        motion.playbackRate = 1.0f;
        character->entity.node.position = {
            motion.rootWorldPosition.x,
            character->entity.node.position.y,
            motion.rootWorldPosition.z,
        };
        if ((runtime->savedInterpolationFlags & 0x80) != 0) {
            if ((runtime->savedInterpolationFlags & 1) != 0)
                motion.motionFlags |= 0x40u;
            else
                motion.motionFlags &= ~0x40u;
            runtime->savedInterpolationFlags = 0;
        }
    }
    return 0;
}
std::int64_t ASEQ_EFFECT(std::int32_t, void *rawData, void *) {
    const auto *const data = static_cast<const EffectFunctionData *>(rawData);
    if (!g_pSCNC || !data || !data->characterId || !g_pSCNC->header.scenarioTask) {
        return 0;
    }
    Character *const character = CHID_Find(g_pSCNC->header.scenarioTask, data->characterId);
    if (!character || !HLib_GetCharacterComponent(character, SHEN_NAME(MOTM))) {
        return 0;
    }
    FACE_ApplyScriptMorph(character, data->channel, data->morph, data->timing);
    return 0;
}

const std::byte *SequenceIndexedPayload(const void *chunk, std::uint32_t index) {
    if (!chunk)
        return nullptr;
    const auto *const base = static_cast<const std::byte *>(chunk);
    std::int32_t offset = 0;
    std::memcpy(&offset, base + 0x10 + sizeof(offset) * index, sizeof(offset));
    return base + offset;
}

std::int64_t ASEQ_VOICE(std::int32_t mode, void *rawData, void *rawRuntime) {
    if (!g_pSCNC || !rawData || !rawRuntime)
        return 0;
    const auto *const data = static_cast<const VoiceFunctionData *>(rawData);
    auto *const runtime = static_cast<std::byte *>(rawRuntime);
    Character *const character = g_pSCNC->header.scenarioTask ? CHID_Find(g_pSCNC->header.scenarioTask, data->characterId) : nullptr;

    if (mode == 1) {
        const std::uint8_t previous = static_cast<std::uint8_t>(runtime[6]);
        runtime[6] = static_cast<std::byte>(static_cast<std::uint8_t>(previous - 1U));
        if (previous != 0)
            return 1;
        if (character) {
            FaceLipCue *cues = nullptr;
            if (data->lipIndex == -1) {
                cues = reinterpret_cast<FaceLipCue *>(runtime + 0x1f8);
            } else {
                cues = reinterpret_cast<FaceLipCue *>(
                    const_cast<std::byte *>(SequenceIndexedPayload(g_pSCNC->header.lipData, static_cast<std::uint32_t>(data->lipIndex))));
            }
            faceSetGeneratedLipCues(character, cues, *reinterpret_cast<const std::uint32_t *>(runtime + 0x450) == 0);
        }
        return 0;
    }
    if (mode != 0 || !character)
        return 0;

    g_pSCNC->voiceTimer = data->durationSeconds * 30.0f;
    g_pSCNC->voiceCharacter = character;

    char voiceName[40]{};
    if (data->soundCommand > 0) {
        shenExecuteAICACommand(static_cast<std::uint32_t>(data->soundCommand), 0, 0);
    } else {
        const auto *const path = reinterpret_cast<const char *>(SequenceIndexedPayload(g_pSCNC->header.stringData, data->stringIndex));
        const char *const filename = path ? std::strrchr(path, '/') : nullptr;
        if (filename) {
            std::strncpy(voiceName, filename + 1, sizeof(voiceName) - 1);
            if (std::strcmp(voiceName, "01m046000038.aiff") == 0) {
                SubtitleStringId id{};
                std::memcpy(&id, "A0184BC003", 10);
                ProcessSubtitleText(ReadStringFromStringTableById(id));
            } else {
                if (char *const extension = std::strchr(voiceName, '.'))
                    *extension = '\0';
                if (ProcessSubtitleFile(voiceName, data->characterId, 1, nullptr)) {
                    return 0;
                }
            }
        }
    }

    runtime[6] = std::byte{13};
    const auto *const sequence = static_cast<const SequenceHeaderChunk *>(g_pSCNC->header.sequence);
    const auto version = sequence ? static_cast<std::int8_t>((sequence->flags >> 8U) & 0xffU) : 0;
    if (version >= 3 && (static_cast<std::uint32_t>(data->soundCommand) - 0x0c000001U) <= 0xf3fffffdU) {
        runtime[6] = std::byte{5};
    }
    if (version < 4 || data->lipIndex != -1)
        return g_pSCNC->header.lipData ? 1 : 0;
    if (data->durationSeconds < 0.5f || data->durationSeconds == 0.0f)
        return 0;

    *reinterpret_cast<std::uint32_t *>(runtime + 0x450) = g_pSCNC->header.currentTrackIndex == 8 && g_pSCNC->frameIndex == 125 ? 1U : 0U;
    if (g_usingJapaneseAudio != 0 || std::strcmp(voiceName, "01JUCEAA026") == 0) {
        return 1;
    }

    const std::int32_t cueCount = std::min(static_cast<std::int32_t>(data->durationSeconds * 7.5f), 73);
    auto *const cues = reinterpret_cast<FaceLipCue *>(runtime + 0x1f8);
    for (std::int32_t index = 0; index < cueCount; ++index) {
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        const float normalized = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) / 32768.0);
        cues[index].viseme = static_cast<std::int16_t>(normalized * 6.0f);
        cues[index].field02 = 2;
        cues[index].duration = 8;
        cues[index].field06 = 0;
    }
    cues[cueCount].viseme = -1;
    return 1;
}

std::int16_t ASEQ_CalculateAngle(std::uint32_t characterId) {
    if (!g_pSCNC || !g_pSCNC->header.scenarioTask)
        return 0;
    Character *const character = CHID_Find(g_pSCNC->header.scenarioTask, characterId);
    if (!character)
        return 0;

    const Vector3 &position = character->entity.node.position;
    const auto yaw = static_cast<std::int16_t>(g_fCamAccumYaw + atan2_fixed16(-(position.x - g_CameraEyePos.x), -(position.z - g_CameraEyePos.z)));
    const auto shifted = static_cast<std::int16_t>(yaw + 0x4000);
    const auto folded = static_cast<std::int16_t>(shifted < 0 ? ~shifted : shifted);
    const auto delta = static_cast<std::int16_t>(folded - 0x4000);
    const std::int32_t scaled = -63 * static_cast<std::int32_t>(delta);
    return static_cast<std::int16_t>((scaled + ((scaled >> 31) & 0x3fff)) >> 14);
}

std::int64_t ASEQ_SOUND_EFFECT(std::int32_t mode, void *rawData, void *) {
    if (!g_pSCNC || !rawData)
        return 0;
    const auto *const data = static_cast<const SoundEffectFunctionData *>(rawData);

    if (mode == 1) {
        const std::size_t channel = static_cast<std::uint32_t>(data->channel);
        if (channel >= std::size(g_pSCNC->soundAngleTimers))
            return 0;
        std::int32_t &timer = g_pSCNC->soundAngleTimers[channel];
        const std::int32_t previous = timer--;
        if (!previous)
            return 0;
        ExecuteAicaCommand2Ch(data->bank, data->channel, 0xb00a0000u, ASEQ_CalculateAngle(data->characterId));
        return 1;
    }
    if (mode != 0 || static_cast<std::int32_t>(data->command) <= 0)
        return 0;

    std::int32_t volume = 127;
    std::int32_t bank = 4;
    std::int32_t channel = 16;
    std::uint32_t flags = 0;
    std::int16_t angle = 0;
    if (data->useDefaults == -1) {
        volume = data->volume;
        bank = data->bank;
        channel = data->channel;
        flags = data->flags;
        if ((flags & 4u) != 0)
            return 1;
        if ((flags & 2u) != 0) {
            angle = ASEQ_CalculateAngle(data->characterId);
            const std::size_t channelIndex = static_cast<std::uint32_t>(channel);
            if (channelIndex >= std::size(g_pSCNC->soundAngleTimers))
                return 0;
            g_pSCNC->soundAngleTimers[channelIndex] = static_cast<std::int32_t>(data->angleDuration * 30.0f);
        }
    }

    shenExecuteAICACommand(data->command, 0, 0);
    if ((flags & 1u) != 0)
        ExecuteAicaCommand2Ch(bank, channel, 0xb0070000u, static_cast<std::int16_t>(volume));
    if ((flags & 2u) != 0) {
        ExecuteAicaCommand2Ch(bank, channel, 0xb00a0000u, angle);
        return 1;
    }
    return 0;
}

const SequenceChunk *FindTrackChild(const SequenceChunk *track, std::uint32_t token) {
    if (!track || track->size < sizeof(SequenceChunk))
        return nullptr;
    const auto *const end = reinterpret_cast<const std::byte *>(track) + track->size;
    auto *child = reinterpret_cast<const SequenceChunk *>(reinterpret_cast<const std::byte *>(track) + sizeof(SequenceChunk));
    while (reinterpret_cast<const std::byte *>(child) + sizeof(*child) <= end) {
        if (child->token == token)
            return child;
        if (child->size < sizeof(SequenceChunk) || reinterpret_cast<const std::byte *>(child) + child->size > end)
            break;
        child = reinterpret_cast<const SequenceChunk *>(reinterpret_cast<const std::byte *>(child) + child->size);
    }
    return nullptr;
}

void SEQ_ExecuteFunction(std::uint8_t functionIndex, void *scriptData) {
    if (!g_pSCNC || !scriptData || functionIndex >= kAnimatedSequenceFunctions.size())
        return;

    ScenarioSequence::FunctionSlot *previous = nullptr;
    const auto frameId = *static_cast<const std::uint32_t *>(scriptData);
    for (auto &slot : g_pSCNC->functionSlots) {
        if (slot.scriptData && slot.functionIndex == functionIndex && slot.runtimeState &&
            static_cast<ScenarioSequence::RuntimeState *>(slot.runtimeState)->frameId == frameId) {
            previous = &slot;
            break;
        }
    }

    ScenarioSequence::FunctionSlot *freeSlot = nullptr;
    for (auto &slot : g_pSCNC->functionSlots) {
        if (!slot.scriptData) {
            freeSlot = &slot;
            break;
        }
    }
    if (!freeSlot)
        return;

    ScenarioSequence::RuntimeState *runtime = nullptr;
    for (auto &state : g_pSCNC->runtimeStates) {
        if (state.frameId == frameId) {
            runtime = &state;
            break;
        }
    }
    if (!runtime) {
        for (auto &state : g_pSCNC->runtimeStates) {
            if (!state.frameId) {
                state.frameId = frameId;
                runtime = &state;
                break;
            }
        }
    }
    if (!runtime)
        return;

    const AnimatedSequenceFunction function = kAnimatedSequenceFunctions[functionIndex];
    freeSlot->functionIndex = functionIndex;
    freeSlot->function = function;
    freeSlot->scriptData = scriptData;
    freeSlot->runtimeState = runtime;
    if (!function(0, scriptData, runtime))
        freeSlot->scriptData = nullptr;
    if (previous)
        previous->scriptData = nullptr;
}

void SEQ_UpdateSkipInput(ScenarioSequence *sequence) {
    if (!sequence || sequence->header.active == 0)
        return;
    const std::int32_t currentTrack = sequence->header.currentTrackIndex;
    if (currentTrack < 0 || currentTrack >= static_cast<std::int32_t>(g_SCNCTrackSlots.size()) || g_SCNCSelectedTrackIndex < 0 ||
        g_SCNCSelectedTrackIndex >= static_cast<std::int32_t>(g_SCNCTrackSlots.size()) || !g_SCNCTrackSlots[static_cast<std::size_t>(currentTrack)].enabled ||
        !g_SCNCTrackSlots[static_cast<std::size_t>(g_SCNCSelectedTrackIndex)].enabled) {
        return;
    }

    InputSlot &input = g_inputState.slots[0];
    constexpr std::uint16_t kSkipButton = 0x0200U;
    if (sequence->skipPromptTimer <= 0.0f || sequence->skipRequested) {
        if (input.pressedPriority != 0 || g_automatedSequenceSkip) {
            sequence->skipPromptTimer = 90.0f;
            input.pressedPriority &= static_cast<std::uint16_t>(~kSkipButton);
        }
        return;
    }

    g_cinemaBars[1].isActive = 1;
    sequence->skipPromptTimer -= g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 30.0f : 1.0f;
    if (((input.pressedPriority & kSkipButton) != 0 && (sequence->inputMask & kSkipButton) == 0) || g_automatedSequenceSkip) {
        sequence->skipRequested = 1;
    }
}




void SEQ_UpdateFCVCurves(ScenarioSequence *sequence) {
    std::int32_t remaining = g_seqRemainingKeyframes;
    bool cut = false;
    if (remaining >= 2) {
        const float start = static_cast<float>(sequence->frameIndex) / 30.0f + -0.016666668f;
        const float end = static_cast<float>(sequence->frameIndex + 1) / 30.0f + -0.016666668f;
        const float *key = g_seqFcvKeyframePtr;
        do {
            const float time = key[0];
            if (start <= time)
                break;
            const float next = key[1];
            ++key;
            if (end > next && time != next)
                cut = true;
            --remaining;
        } while (remaining >= 2);
        g_seqRemainingKeyframes = remaining;
        g_seqFcvKeyframePtr = key;
    }

    for (auto &state : sequence->runtimeStates) {
        auto *const motionState = reinterpret_cast<MotionRuntimeState *>(&state);
        if ((motionState->flags & 2) == 0)
            continue;
        Character *const character = motionState->characterId ? CHID_Find(sequence->header.scenarioTask, motionState->characterId) : nullptr;
        MotionManagerWork *const manager = MOTM_GetWork(character);
        if (cut) {
            if ((motionState->flags & 4) != 0 && manager) {
                motionState->savedInterpolationFlags = static_cast<std::int32_t>(((manager->motion.motionFlags & 0x40U) | 0x2000U) >> 6U);
                manager->motion.motionFlags &= ~0x40U;
            }
            if (character)
                OSAGE_Initialize(character);
        } else if ((motionState->savedInterpolationFlags & 0x80) != 0) {
            if (manager) {
                if ((motionState->savedInterpolationFlags & 1) != 0)
                    manager->motion.motionFlags |= 0x40U;
                else
                    manager->motion.motionFlags &= ~0x40U;
            }
            motionState->savedInterpolationFlags = 0;
        }
    }
}

void SEQ_UpdateTasksAndCameras(ScenarioSequence *sequence) {
    if (!sequence)
        return;

    for (auto &state : sequence->runtimeStates) {
        auto *const motionState = reinterpret_cast<MotionRuntimeState *>(&state);
        if ((motionState->flags & 2) == 0 || !motionState->characterId)
            continue;
        Character *const character = CHID_Find(sequence->header.scenarioTask, motionState->characterId);
        if (MotionManagerWork *const manager = MOTM_GetWork(character)) {
            manager->motion.playbackRate = sequence->updateMode || !motionState->framesRemaining ? 0.0f : 1.0f;
        }
    }

    bInterpretRuntime = sequence->updateMode ? 1 : 0;
    if (sequence->updateMode)
        return;

    sequence->frameTime += g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 30.0f : 1.0f;
    sequence->frameIndex = static_cast<std::int32_t>(sequence->frameTime);
    if (sequence->voiceTimer != 0.0f) {
        sequence->voiceTimer -= 1.0f;
        if (sequence->voiceTimer == 0.0f) {
            sequence->reserved074 = 0;
            sequence->voiceCharacter = nullptr;
        }
    }

    for (auto &slot : sequence->functionSlots) {
        if (!slot.scriptData || !slot.function)
            continue;
        const AnimatedSequenceFunction function = slot.function;
        if (!function(1, slot.scriptData, slot.runtimeState)) {
            function(2, slot.scriptData, slot.runtimeState);
            slot.scriptData = nullptr;
        }
    }

    auto *cursor = static_cast<std::byte *>(sequence->sequenceCursor);
    if (!cursor)
        return;
    std::int32_t eventFrame = 0;
    std::memcpy(&eventFrame, cursor, sizeof(eventFrame));
    if (static_cast<std::uint32_t>(eventFrame) <= static_cast<std::uint32_t>(sequence->frameIndex)) {
        std::byte *control = cursor + 4;
        std::uint8_t functionIndex = static_cast<std::uint8_t>(control[0]);
        while (functionIndex) {
            const auto wordCount = static_cast<std::int8_t>(control[1]);
            if (wordCount == 0) {
                ASEQ_ApplyCameraKeyframes(sequence);
                return;
            }
            SEQ_ExecuteFunction(functionIndex, control + 4);
            control += 4 * wordCount + 4;
            functionIndex = static_cast<std::uint8_t>(control[0]);
        }
        sequence->sequenceCursor = control + 4;
    }

    SEQ_UpdateFCVCurves(sequence);

    if (sequence->header.needsNextFrame) {
        sequence->header.needsNextFrame = 0;
        sequence->frameIndex = sequence->frameCount + 1;
        sequence->frameTime = static_cast<float>(sequence->frameCount);
        ASEQ_ApplyCameraKeyframes(sequence);
        return;
    }

    cursor = static_cast<std::byte *>(sequence->sequenceCursor);
    if (cursor) {
        std::memcpy(&eventFrame, cursor, sizeof(eventFrame));
        if (eventFrame == -1 && sequence->frameCount < sequence->frameIndex)
            ASEQ_ApplyCameraKeyframes(sequence);
    }
}
} // namespace scen_detail
void SEQ_SetAutomatedSkip(bool enabled) {
    g_automatedSequenceSkip = enabled;
}

ScenarioSequence *ASEQ_SCNC_Init(HLib::Task *scenarioTask) {
    auto *const sequence = static_cast<ScenarioSequence *>(HLib_AllocHeapMemBlock(sizeof(ScenarioSequence), static_cast<std::int32_t>(SHEN_NAME(SCNC))));
    sequence->reserved074 = 0;
    sequence->voiceCharacter = nullptr;
    sequence->header.currentTrackIndex = -9999;
    sequence->header.scenarioTask = scenarioTask;
    sequence->header.active = 0;
    sequence->header.pendingTrackSwitch = 0;
    sequence->header.needsNextFrame = 0;
    *reinterpret_cast<std::uint64_t *>(&sequence->delayFrames) = 0;
    *reinterpret_cast<std::uint64_t *>(&sequence->skipRequested) = 0;
    sequence->updateRepeatCount = 1;
    sequence->reservedA580 = 0;
    std::memset(sequence->soundAngleTimers, 0, sizeof(sequence->soundAngleTimers));
    g_pSCNC = sequence;
    g_SCNCSelectedTrackIndex = -1;
    return sequence;
}

void ASEQ_InitCameraTrackPointers(const void *scenarioExtensionChunk) {
    g_SCNCTrackSlots = {};
    if (!scenarioExtensionChunk)
        return;

    const auto *const container = static_cast<const SequenceChunk *>(scenarioExtensionChunk);
    if (container->size < sizeof(SequenceChunk) * 2)
        return;
    const auto *const end = reinterpret_cast<const std::byte *>(container) + container->size;
    auto *chunk = reinterpret_cast<const SequenceChunk *>(reinterpret_cast<const std::byte *>(container) + sizeof(SequenceChunk));
    std::size_t slotIndex = 0;
    while (slotIndex < g_SCNCTrackSlots.size() && reinterpret_cast<const std::byte *>(chunk) + sizeof(*chunk) <= end && chunk->token == SHEN_NAME(TRCK)) {
        ScenarioTrackSlot &slot = g_SCNCTrackSlots[slotIndex++];
        slot.track = const_cast<SequenceChunk *>(chunk);
        if (chunk->size < sizeof(SequenceChunk) || reinterpret_cast<const std::byte *>(chunk) + chunk->size > end)
            break;
        chunk = reinterpret_cast<const SequenceChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
        if (reinterpret_cast<const std::byte *>(chunk) + sizeof(*chunk) <= end && chunk->token == SHEN_NAME(MLWD)) {
            slot.movementPayload = const_cast<std::byte *>(reinterpret_cast<const std::byte *>(chunk) + sizeof(SequenceChunk));
            if (chunk->size < sizeof(SequenceChunk) || reinterpret_cast<const std::byte *>(chunk) + chunk->size > end)
                break;
            chunk = reinterpret_cast<const SequenceChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
        }
    }
}

void ASEQ_ApplyCameraKeyframes(ScenarioSequence *sequence) {
    if (!g_pSCNC)
        return;
    if (sequence && sequence->header.active == 1) {
        sequence->header.active = 0;
        g_pSCNC->header.active = 0;
        for (auto &slot : sequence->functionSlots) {
            if (slot.scriptData && slot.function)
                slot.function(2, slot.scriptData, slot.runtimeState);
            slot.scriptData = nullptr;
        }
        for (auto &state : sequence->runtimeStates) {
            auto *const motionState = reinterpret_cast<MotionRuntimeState *>(&state);
            if ((motionState->flags & 2) != 0) {
                MotionFunctionData data{};
                data.characterId = motionState->characterId;
                ASEQ_MOTION(3, &data, motionState);
            }
        }
        if (!bLockCamera)
            ZANGLE = 0;
    }
    g_pSCNC->skipPromptTimer = 0.0f;
    const std::int32_t currentTrack = g_pSCNC->header.currentTrackIndex;
    if (currentTrack >= 0 && currentTrack < static_cast<std::int32_t>(g_SCNCTrackSlots.size()) &&
        g_SCNCTrackSlots[static_cast<std::size_t>(currentTrack)].enabled && g_SCNCSelectedTrackIndex >= 0) {
        g_SCNCSelectedTrackIndex = -1;
    }
}

bool SEQ_ParseInternal(const void *trackData) {
    if (!g_pSCNC || !trackData)
        return false;
    const auto *const track = static_cast<const SequenceChunk *>(trackData);
    const auto *const sequence = reinterpret_cast<const SequenceHeaderChunk *>(FindTrackChild(track, SHEN_NAME(ASEQ)));
    if (!sequence)
        return false;

    g_pSCNC->header.track = const_cast<SequenceChunk *>(track);
    g_pSCNC->header.sequence = const_cast<SequenceHeaderChunk *>(sequence);
    g_pSCNC->header.camera = const_cast<SequenceChunk *>(FindTrackChild(track, SHEN_NAME(ACAM)));
    g_pSCNC->header.movement = const_cast<SequenceChunk *>(FindTrackChild(track, SHEN_NAME(AMOV)));
    g_pSCNC->header.stringData = const_cast<SequenceChunk *>(FindTrackChild(track, SHEN_NAME(ASTR)));
    g_pSCNC->header.lipData = const_cast<SequenceChunk *>(FindTrackChild(track, SHEN_NAME(ALIP)));
    g_pSCNC->sequenceCursor = const_cast<std::byte *>(reinterpret_cast<const std::byte *>(sequence) + sizeof(*sequence));
    g_pSCNC->frameIndex = 0;
    g_pSCNC->frameTime = 0.0f;
    g_pSCNC->frameCount = sequence->frameCount;
    g_pSCNC->updateMode = 0;
    std::memset(g_pSCNC->functionSlots, 0,
        sizeof(g_pSCNC->functionSlots) + sizeof(g_pSCNC->reserved988) + sizeof(g_pSCNC->runtimeStates) + sizeof(g_pSCNC->runtimeClearTail));
    g_pSCNC->inputMask = 0;
    g_pSCNC->voiceTimer = 0.0f;
    g_pSCNC->reserved074 = 0;
    g_pSCNC->header.active = 1;
    UI_ResetForSequenceStart();
    return true;
}

void SEQ_ParseFrame(ScenarioSequence *sequence) {
    if (!sequence)
        return;
    g_pSCNC = sequence;

    g_cinemaBars[1].isActive = 0;

    if (sequence->header.active == 0) {
        sequence->updateRepeatCount = 1;
    } else {
        sequence->reservedA580 = 0;
        if (sequence->delayLatched || sequence->delayFrames == 0) {
            if (sequence->updateRepeatCount <= 1) {
                SEQ_UpdateTasksAndCameras(sequence);
            } else {
                const std::int16_t track = sequence->header.currentTrackIndex;
                const std::int32_t count = 25 * sequence->updateRepeatCount - 25;
                for (std::int32_t index = 0; index < count && track == sequence->header.currentTrackIndex; ++index) {
                    SEQ_UpdateTasksAndCameras(sequence);
                }
            }
        } else {
            --sequence->delayFrames;
            if (sequence->delayFrames == 0) {
                sequence->header.pendingTrackSwitch = 0;
                sequence->header.needsNextFrame = 1;
                sequence->delayLatched = 1;
            }
        }
        SEQ_UpdateSkipInput(sequence);
    }

    if (sequence->header.pendingTrackSwitch && sequence->header.currentTrackIndex >= 0) {
        const std::int32_t updateMode = sequence->updateMode;
        SequenceUtilityParameters parameters{sequence->header.currentTrackIndex, 0, 0};
        sequence->header.pendingTrackSwitch = 0;
        Script_shenSeqUtils(nullptr, &parameters);
        if (g_pSCNC) {
            g_pSCNC->updateMode = updateMode;
            g_pSCNC->updateRepeatCount = 1;
        }
    }
    SEQ_ConvertMLWDToUNFO(sequence);
}

void SEQ_Cleanup() {
    if (!g_pCVWK)
        return;
    for (CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        if (slot.callback)
            slot.callback(&slot);
    }
}
} // namespace shenmue::s1
