#include "s2/fight_stage.hpp"
#include "s2/fight_dialogue_tables.hpp"
#include "s2/fight.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/face.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/map_control.hpp"
#include "s2/event_runtime.hpp"
#include "s2/player.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/input.hpp"
#include "s2/debug_text.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <cmath>

namespace shenmue::s2 {
namespace {
// vfSetMoveInstructionConfiguration owns this configuration; VFHD borrows the caller's result record.
shen_name instructionCharacter{}, instructionSecondCharacter{};
std::int16_t instructionProgram{}, instructionSecondProgram{};
std::byte *instructionResult{};

template <class T> T InstructionValue(const VFHD_t *owner, std::size_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(owner) + offset, sizeof(value));
    return value;
}
template <class T> void InstructionStore(VFHD_t *owner, std::size_t offset, T value) {
    std::memcpy(reinterpret_cast<std::byte *>(owner) + offset, &value, sizeof(value));
}

std::int32_t dialoguePlayerFocus{};
std::int16_t DialogueIndex(const VFMA_t &game) {
    std::int16_t value;
    std::memcpy(&value, game.reserved318 + 2, sizeof(value));
    return value;
}
void SetDialogueIndex(VFMA_t &game, std::int16_t index) {
    std::memcpy(game.reserved318 + 2, &index, sizeof(index));
}
FightDialogueRecord *DialogueBank(const VFMA_t &game) {
    return dialogueBanks[static_cast<std::int8_t>(game.reserved318[0])];
}
struct InstructionCommand {
    std::int32_t operation, argument;
};
constexpr InstructionCommand instructionCommands0[]{{8, 8}, {7, 128}, {3, 362}, {6, 0}};
constexpr InstructionCommand instructionCommands1[]{{8, 8}, {7, 64}, {3, 582}, {6, 0}};
constexpr InstructionCommand instructionCommands2[]{{8, 8}, {7, 67}, {3, 329}, {6, 0}};
constexpr InstructionCommand instructionCommands3[]{{8, 8}, {7, 3}, {3, 1}, {6, 0}};
constexpr InstructionCommand instructionCommands4[]{{8, 53}, {7, 8}, {5, 134218140}, {10, 1}, {7, 496}, {3, -1}, {6, 0}};
constexpr InstructionCommand instructionCommands5[]{{8, 6}, {7, 128}, {3, 652}, {6, 0}};
constexpr InstructionCommand instructionCommands6[]{{8, 6}, {7, 128}, {7, 3}, {3, 281}, {11, 10}, {6, 0}};
constexpr InstructionCommand instructionCommands7[]{{8, 6}, {7, 128}, {7, 3}, {3, 134217799}, {11, 10}, {6, 0}};
constexpr InstructionCommand instructionCommands8[]{{8, 53}, {7, 8}, {5, 134218140}, {10, 1}, {7, 496}, {3, -1}, {11, 20}, {6, 0}};
constexpr InstructionCommand instructionCommands9[]{{8, 8}, {7, 128}, {7, 128}, {3, 583}, {6, 0}};
constexpr InstructionCommand instructionCommands10[]{{8, 8}, {7, 128}, {7, 129}, {3, 56}, {6, 0}};
struct InstructionProgram {
    std::uint8_t mode;
    const InstructionCommand *commands;
    std::uint8_t partnerMode;
};
constexpr InstructionProgram instructionPrograms[]{{0, instructionCommands0, 0}, {0, instructionCommands1, 0}, {0, instructionCommands2, 0},
    {0, instructionCommands3, 0}, {0, instructionCommands4, 1}, {0, instructionCommands5, 0}, {0, instructionCommands6, 0}, {3, instructionCommands7, 2},
    {0, instructionCommands8, 1}, {0, instructionCommands9, 0}, {0, instructionCommands10, 0}};
#pragma pack(push,1)
struct InstructionState {
    std::int16_t program;
    std::uint8_t done, timedOut, startCountdown, endCountdown, frameCountdown, maximumWindow, skippedFrames;
    std::uint8_t outcome, progress, failure, analysis;
    std::byte reserved0d[3];
    std::uint32_t diagnostic;
    std::uint8_t delay, committedAttempts, endInstruction, ended, reserved18;
    std::int8_t instruction, historyIndex;
    std::uint8_t heldDirection;
    std::int32_t pendingMask, previousMask, resolvedMask;
    std::int16_t previousDirection, chosenDirection;
    std::int32_t motion;
    float endFrame;
    std::uint8_t attempts;
    std::byte reserved35[3];
};
#pragma pack(pop)
static_assert(sizeof(InstructionState) == 56);
static_assert(offsetof(InstructionState, motion) == 44);
struct InstructionHistory {
    std::int16_t held, pressed;
    std::uint32_t reserved;
};
static_assert(sizeof(InstructionHistory) == 8);
InstructionState &FirstInstruction(VFHD_t *owner) {
    return *reinterpret_cast<InstructionState *>(reinterpret_cast<std::byte *>(owner) + 8);
}
InstructionState &SecondInstruction(VFHD_t *owner) {
    return *reinterpret_cast<InstructionState *>(reinterpret_cast<std::byte *>(owner) + 0x40);
}
const InstructionHistory &InstructionHistoryAt(const VFHD_t *owner, std::int64_t index) {
    return *reinterpret_cast<const InstructionHistory *>(reinterpret_cast<const std::byte *>(owner) + 0x78 + 8 * index);
}
void InitializeInstructionState(InstructionState &state) {
    state.timedOut = 0;
    state.startCountdown = 240;
    state.motion = 0;
    state.endFrame = 0;
    state.previousMask = state.resolvedMask = state.pendingMask = 0;
    state.historyIndex = 0;
    state.heldDirection = 0;
    state.previousDirection = 0;
    state.outcome = state.progress = state.failure = state.analysis = 0;
    state.instruction = 0;
    state.endInstruction = state.ended = state.attempts = 0;
    state.done = state.program == -1;
    state.endCountdown = 30;
    state.maximumWindow = 120;
    state.skippedFrames = 0;
}
std::uint8_t DirectionSector(std::uint8_t direction) {
    switch (direction & 0xf0) {
    case 0x10:
        return 128;
    case 0x40:
        return 64;
    case 0x50:
        return 96;
    case 0x60:
        return 32;
    case 0x80:
        return 192;
    case 0x90:
        return 160;
    case 0xa0:
        return 224;
    default:
        return 0;
    }
}
bool InstructionDirectionChanged(std::uint8_t first, std::uint8_t second) {
    if (!(first & 0xf0) || !(second & 0xf0))
        return false;
    const auto a = DirectionSector(first), b = DirectionSector(second);
    return static_cast<std::uint8_t>(a > b ? a - b : b - a) >= 64;
}
// Left/right interpretation follows the saved authored yaw and CAMC.
// @verified - p
bool InstructionFacesCamera(const VFHD_t *owner) {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    auto *const character = scenFindChara(control->scenarioTask, InstructionValue<shen_name>(owner, 0x474));
    const auto position = GetCharacterWorldPosition(character);
    const auto yaw = InstructionValue<std::int32_t>(owner, 0x4b8);
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    const float dot = (camera.x - position.x) * cos_fixed16(yaw) + (camera.z - position.z) * (-sin_fixed16(yaw));
    SetDebugStringPosition(3, 4);
    return dot < 0;
}
std::int16_t DecodeInstructionInput(const VFHD_t *owner, std::uint8_t input) {
    std::uint16_t result = (input & 2) ? 2 : 0;
    if (input & 4)
        result |= 8;
    if (input & 1)
        result |= 1;
    if (input & 8)
        result |= 4;
    if (InstructionFacesCamera(owner)) {
        if (input & 0x40)
            result |= 0x40;
        if (input & 0x80)
            result |= 0x80;
    } else {
        if (input & 0x40)
            result |= 0x80;
        if (input & 0x80)
            result |= 0x40;
    }
    if (input & 0x20)
        result |= 0x20;
    if (input & 0x10)
        result |= 0x10;
    return static_cast<std::int16_t>(result);
}
void RecordInstructionInput(VFActor *actor) {
    auto *const owner = actor->game->dialogue;
    if (!owner)
        return;
    const auto pressed = DecodeInstructionInput(owner, static_cast<std::uint8_t>(actor->followupFlags));
    const auto held = DecodeInstructionInput(owner, static_cast<std::uint8_t>(actor->cameraControlFlags));
    auto index = InstructionValue<std::int8_t>(owner, 0x438);
    InstructionStore(owner, 0x78 + 8 * index, held);
    InstructionStore(owner, 0x7a + 8 * index, pressed);
    ++index;
    InstructionStore(owner, 0x438, index == 120 ? std::int8_t{0} : index);
}
std::int16_t LastInstructionDirection(const VFHD_t *owner) {
    const auto end = InstructionValue<std::int8_t>(owner, 0x438);
    std::int64_t index = end ? end - 1 : 0;
    for (;;) {
        const auto result = InstructionHistoryAt(owner, index).pressed & 0xf0;
        if (result)
            return result;
        --index;
        if (index == 0)
            return result;
    }
}
std::uint8_t InstructionCloseBetweenCharacters(const VFHD_t *owner, const InstructionState &state) {
    const auto mode = InstructionValue<std::int8_t>(owner, 0x443);
    if (mode >= 2 && !state.attempts && InstructionValue<std::uint8_t>(owner, 0x442) == 3) {
        if (InstructionValue<std::uint8_t>(owner, 0x445) != 1)
            return 1;
        if (InstructionValue<std::int16_t>(owner, 0x46a) > InstructionValue<std::int16_t>(owner, 0x45a))
            return 3;
    } else if (mode == 1 && !state.attempts) {
        const auto a = GetCharacterWorldPosition(owner->character), b = GetCharacterWorldPosition(owner->secondCharacter);
        if (double((b.x - a.x) * (b.x - a.x) + (b.z - a.z) * (b.z - a.z)) > 2.56)
            return 2;
    }
    return 0;
}
bool InstructionHitWindowAdmitted(const VFHD_t *owner, shen_name name) {
    if (name.name == InstructionValue<shen_name>(owner, 0x474).name) {
        const auto phase = InstructionValue<std::uint8_t>(owner, 0x445);
        if ((phase != 1 || InstructionValue<std::uint8_t>(owner, 0x442) != 3 || !InstructionValue<std::uint8_t>(owner, 0x447)) &&
            (InstructionValue<std::uint8_t>(owner, 0x448) ||
                (InstructionValue<std::int8_t>(owner, 0x443) >= 2 && InstructionValue<std::uint8_t>(owner, 0x442) == 3 &&
                    (phase != 1 || InstructionValue<std::int16_t>(owner, 0x46a) > InstructionValue<std::int16_t>(owner, 0x45a)))))
            return false;
    } else if (InstructionValue<std::uint8_t>(owner, 0x447))
        return false;
    return true;
}
constexpr struct {
    std::uint16_t delay;
    const char *voice;
} instructionVoices[]{{0, "0"}, {10, "0068C023"}, {5, "0064E005"}, {5, "0064E006"}, {5, "0064E007"}};
void QueueInstructionDialogue(VFHD_t *owner, std::int32_t index) {
    if (index == 2)
        index = 2 + static_cast<std::int32_t>(double(IncrementSharedPrngFloat()) * 3.0);
    InstructionStore(owner, 0x51a, static_cast<std::uint16_t>(index));
    InstructionStore(owner, 0x518, instructionVoices[index].delay);
}
void QueueInstructionMotion(VFHD_t *owner, std::int32_t motion) {
    owner->requestedMotion = motion;
    InstructionStore(owner, 0x460, motion);
    const auto metadata = FindLoadedMotionSecondaryEntry(static_cast<std::uint16_t>(motion), 2);
    if (InstructionValue<std::int8_t>(owner, 0x443) >= 2 && InstructionValue<std::uint8_t>(owner, 0x442) == 3)
        InstructionStore(owner, 0x44a, std::uint32_t{65536000});
    else if (!metadata.empty()) {
        InstructionStore(owner, 0x44a, InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(metadata.data()), 2));
        InstructionStore(owner, 0x44c, InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(metadata.data()), 4));
    } else
        InstructionStore(owner, 0x44a, std::uint32_t{0});
    InstructionStore(owner, 0x468, std::uint16_t{0});
}
// Advance an authored program against the actual VFHD input history.
// @verified - p
void RunInstructionProgram(VFActor *actor, VFHD_t *owner, InstructionState &state) {
    const std::int8_t originalHistory = state.historyIndex;
    std::int8_t instruction = state.instruction;
    std::int32_t history = originalHistory;
    if (!originalHistory) {
        state.previousMask = state.resolvedMask = state.pendingMask = 0;
        state.heldDirection = 0;
    }
    const auto *const commands = instructionPrograms[state.program].commands;
    const auto advance = [&] {
        ++instruction;
        state.historyIndex = static_cast<std::int8_t>(history);
        state.instruction = instruction;
    };
    const auto finish = [&] {
        if (originalHistory == state.historyIndex) {
            state.pendingMask = state.previousMask = 0;
        }
    };
    for (;;) {
        const auto command = commands[instruction];
        switch (command.operation) {
        case 3:
        case 4:
            if (((state.progress - 1) & 0xfd) == 0 && !state.pendingMask && !state.previousMask) {
                state.motion = command.argument;
                if (state.motion == -1) {
                    state.chosenDirection = LastInstructionDirection(owner);
                    if (actor->actor->moveFlags & VFMF_MirrorReactionAngle) {
                        if (state.chosenDirection & 0x20)
                            state.motion = 603;
                        else if (state.chosenDirection & 0x10)
                            state.motion = 602;
                        else if (state.chosenDirection & 0x80)
                            state.motion = 379;
                        else if (state.chosenDirection & 0x40)
                            state.motion = 601;
                    } else {
                        if (state.chosenDirection & 0x20)
                            state.motion = 602;
                        else if (state.chosenDirection & 0x10)
                            state.motion = 603;
                        else if (state.chosenDirection & 0x80)
                            state.motion = 379;
                        else if (state.chosenDirection & 0x40)
                            state.motion = 601;
                    }
                    state.motion |= 0x8000000;
                }
                advance();
                break;
            }
            state.progress = 2;
            if (!state.analysis)
                state.analysis = 13;
            state.done = 1;
            finish();
            return;
        case 5:
            if (!(state.progress & 0xfc) && state.progress != 2) {
                state.motion = command.argument;
                if (InstructionValue<std::uint8_t>(owner, 0x444) == 2) {
                    state.outcome = 2;
                    state.failure = 19;
                    InstructionStore(owner, 0x4cd, std::uint8_t{3});
                } else {
                    const auto close = InstructionCloseBetweenCharacters(owner, state);
                    if (close) {
                        if (close == 1 || close == 2 || close == 3) {
                            owner->requestedMotion = 394;
                            state.failure = close == 1 ? 18 : close == 2 ? 21 : 19;
                            state.outcome = 2;
                            if (close == 3)
                                InstructionStore(owner, 0x444, std::uint16_t{514});
                            InstructionStore(owner, 0x4cd, std::uint8_t{3});
                        }
                    } else {
                        QueueInstructionMotion(owner, state.motion);
                        if (state.motion == 0x800019c)
                            owner->result[7] |= std::byte{1};
                        InstructionStore(owner, 0x470, std::uint16_t{1});
                        InstructionStore(owner, 0x447, std::uint8_t{1});
                        std::int8_t next = instruction + 1;
                        while (commands[next].operation != 7 && commands[next].operation != 6)
                            ++next;
                        const auto requirement = commands[next].operation == 7 ? commands[next].argument : 0;
                        const auto pending = requirement & state.pendingMask;
                        if (pending) {
                            state.failure = 14;
                            state.outcome = 2;
                            state.diagnostic = pending;
                            InstructionStore(owner, 0x4cd, std::uint8_t{3});
                        }
                    }
                }
                ++state.attempts;
            }
            advance();
            break;
        case 6:
            state.ended = 1;
            if (state.progress == 3) {
                state.progress = 1;
                state.analysis = 0;
            }
            state.done = 1;
            finish();
            return;
        case 7: {
            bool failed = false;
            if (history > static_cast<std::int8_t>(state.maximumWindow)) {
                if (!state.progress)
                    state.analysis = 20;
                else if (state.progress == 3)
                    state.analysis = 0;
                if (!state.progress || state.progress == 3)
                    state.progress = 2;
                failed = true;
            }
            if (history < InstructionValue<std::int8_t>(owner, 0x438)) {
                state.endInstruction = instruction;
                auto requirement = command.argument;
                const auto input = InstructionHistoryAt(owner, history);
                const auto pending = state.pendingMask;
                std::int32_t combined;
                if (!state.heldDirection || (std::uint8_t(requirement) & std::uint8_t(input.pressed) & 0xf0)) {
                    combined = pending | input.pressed;
                    if (input.pressed & 0xf0)
                        state.heldDirection = 1;
                } else {
                    combined = pending | (input.pressed & 0xf);
                    if (InstructionDirectionChanged(input.held, state.previousDirection))
                        combined |= input.held & 0xf0;
                    else if (!(input.held & 0xf0))
                        state.heldDirection = 0;
                }
                if (combined & 0xf0)
                    state.previousDirection = combined & 0xf0;
                state.previousMask = pending;
                state.pendingMask = 0;
                if ((requirement & 0xf0) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                    combined = (requirement & 0xf0) | (combined & 0xf);
                if ((requirement & 0x100) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                    requirement = (requirement & 0xf) | (combined & 0xf0);
                if (combined == requirement) {
                    ++instruction;
                    state.progress = 3;
                    state.pendingMask = state.previousMask = 0;
                } else if (pending) {
                    state.progress = 2;
                    failed = true;
                    state.done = 1;
                    state.resolvedMask = combined;
                } else {
                    if ((requirement & combined) == requirement) {
                        combined ^= requirement;
                        ++instruction;
                    }
                    state.pendingMask = combined;
                }
                ++history;
                if (!failed)
                    continue;
            }
            finish();
            return;
        }
        case 8:
            state.maximumWindow = static_cast<std::uint8_t>(state.maximumWindow == 120 ? command.argument : history + command.argument - 1);
            state.frameCountdown = static_cast<std::uint8_t>(command.argument + 1);
            advance();
            break;
        case 9:
            state.skippedFrames = static_cast<std::uint8_t>(command.argument);
            advance();
            break;
        case 10:
            if (state.outcome != 2)
                QueueInstructionDialogue(owner, command.argument);
            advance();
            break;
        case 11:
            state.endFrame = float(command.argument);
            advance();
            break;
        case 16:
            InstructionStore(owner, 0x450, float(command.argument));
            ++instruction;
            state.instruction = instruction;
            break;
        case 17: {
            const float endFrame = float(command.argument);
            InstructionStore(owner, 0x454, double(endFrame) < .1 ? .001f : endFrame);
            ++instruction;
            state.instruction = instruction;
            break;
        }
        case 18:
            InstructionStore(owner, 0x458, static_cast<std::uint8_t>(command.argument));
            ++instruction;
            state.instruction = instruction;
            break;
        default:
            break;
        }
    }
}
struct InstructionDiagnosis {
    std::uint8_t kind{}, delay{}, attempts{};
    std::uint32_t mask{};
};
std::uint8_t InstructionMaskDiagnosis(std::int32_t mask, bool missing, bool directionRequired) {
    if (mask & 1)
        return missing ? 3 : 8;
    if (mask & 2)
        return missing ? 4 : 9;
    if (mask & 8)
        return missing ? 5 : 10;
    if (mask & 4)
        return missing ? 6 : 11;
    if (mask & 0xf0)
        return missing ? 7 : directionRequired ? 1 : 12;
    return 0;
}
// Classify the full authored input sequence, not a single test move.
// @verified - p
InstructionDiagnosis DiagnoseInstructionProgram(VFHD_t *owner, InstructionState &state) {
    InstructionDiagnosis result;
    const auto *const commands = instructionPrograms[state.program].commands;
    std::int8_t instruction = 0;
    std::int32_t history = 0, pending = 0, previous = 0, diagnostic = 0;
    std::uint8_t progress = 0, kind = 0, attempts = 0, delay = 0, window = 120;
    std::uint8_t previousRequirement = 0, previousDirection = 0, held = 0;
    std::int32_t lastMatchedHistory = 0;
    const auto finish = [&] {
        result.kind = kind;
        result.delay = delay;
        result.attempts = attempts;
        if (kind == 1 || static_cast<std::uint8_t>(kind - 3) <= 9)
            result.mask = diagnostic;
        return result;
    };
    for (;;) {
        const auto command = commands[instruction];
        switch (command.operation) {
        case 3:
        case 4:
            if (pending || previous) {
                if (kind && kind != 13) {
                    ++instruction;
                    break;
                }
                const auto bits = pending ? static_cast<std::uint8_t>(pending) : previous;
                const auto diagnosed = InstructionMaskDiagnosis(bits, false, (previousRequirement & 0xf0) != 0);
                if (diagnosed)
                    kind = diagnosed;
                attempts = 0;
                return finish();
            }
            ++instruction;
            break;
        case 5:
            ++attempts;
            ++instruction;
            break;
        case 6:
            if (lastMatchedHistory > window) {
                kind = 16;
                delay = lastMatchedHistory - window;
            }
            attempts = 0;
            return finish();
        case 7: {
            state.endInstruction = instruction;
            const auto length = InstructionValue<std::int8_t>(owner, 0x438);
            const auto savedAttempts = attempts;
            auto requirement = command.argument;
            if (history >= length) {
                if (!progress)
                    kind = 20;
                else if (progress == 3)
                    kind = InstructionMaskDiagnosis(requirement, true, false);
                attempts = 0;
                return finish();
            }
            const auto input = InstructionHistoryAt(owner, history);
            std::int32_t combined;
            if (!held || (std::uint8_t(requirement) & std::uint8_t(input.pressed) & 0xf0)) {
                combined = pending | input.pressed;
                if (input.pressed & 0xf0)
                    held = 1;
            } else {
                combined = pending | (input.pressed & 0xf);
                if (InstructionDirectionChanged(input.held, previousDirection))
                    combined |= input.held & 0xf0;
                else if (!(input.held & 0xf0))
                    held = 0;
            }
            if (combined & 0xf0)
                previousDirection = combined & 0xf0;
            previous = pending;
            pending = 0;
            if ((requirement & 0xf0) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                combined = (requirement & 0xf0) | (combined & 0xf);
            if ((requirement & 0x100) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                requirement = (requirement & 0xf) | (combined & 0xf0);
            if (combined == requirement) {
                previous = 0;
                progress = 3;
                if (history > window) {
                    kind = 16;
                    delay = history - window;
                    attempts = savedAttempts;
                    return finish();
                }
                lastMatchedHistory = history;
                previousRequirement = requirement;
                ++instruction;
            } else if (!previous) {
                pending = combined;
                if ((requirement & combined) == requirement) {
                    pending ^= requirement;
                    ++instruction;
                }
            } else {
                progress = 2;
                if (!kind) {
                    if (requirement & combined) {
                        diagnostic = combined & (requirement ^ combined);
                        kind = InstructionMaskDiagnosis(diagnostic, false, (requirement & 0xf0) != 0);
                    } else if (combined & 0xf) {
                        diagnostic = combined;
                        kind = InstructionMaskDiagnosis(combined, false, false);
                    } else if (combined & 0xf0) {
                        diagnostic = combined;
                        kind = (requirement & 0xf0) ? 1 : 12;
                    }
                    if (!kind) {
                        std::int32_t next = history + 1, accumulated = 0;
                        std::uint8_t frames = 1;
                        for (; next < length; ++next, ++frames) {
                            accumulated |= InstructionHistoryAt(owner, next).pressed;
                            if ((requirement & (accumulated | combined)) == requirement)
                                break;
                        }
                        if (next < length) {
                            result.mask = accumulated;
                            delay = frames + 1;
                            kind = 15;
                        } else {
                            if (requirement & combined) {
                                diagnostic = combined;
                                kind = InstructionMaskDiagnosis(requirement & (requirement ^ combined), true, false);
                            }
                            if (!kind) {
                                if ((requirement & 0xf0) && (requirement & 0xf0) != (combined & 0xf0) && (combined & 0xf0))
                                    kind = 1;
                                else if ((requirement & 0xf0) && !(requirement & 0xf) && (combined & 0xf))
                                    kind = 2;
                                else
                                    kind = 13;
                            }
                        }
                    }
                }
                ++history;
                attempts = 0;
                return finish();
            }
            ++history;
            attempts = savedAttempts;
            break;
        }
        case 8:
            window = static_cast<std::uint8_t>(window == 120 ? command.argument : history + command.argument - 1);
            ++instruction;
            break;
        case 9:
        case 10:
        case 11:
        case 16:
        case 17:
        case 18:
            ++instruction;
            break;
        default:
            break;
        }
    }
}
// Both programs share the real capture clock and completion producer.
// @verified - p
std::int8_t UpdateInstructionPrograms(VFActor *actor) {
    auto *const owner = actor->game->dialogue;
    if (!owner)
        return 0;
    auto &first = FirstInstruction(owner);
    auto &second = SecondInstruction(owner);
    auto phase = InstructionValue<std::uint8_t>(owner, 0x4cd);
    const auto setPhase = [&](std::uint8_t value) {
        phase = value;
        InstructionStore(owner, 0x4cd, value);
    };
    if (phase == 0) {
        setPhase(1);
        return -1;
    }
    if (phase == 1) {
        if (InstructionValue<std::uint8_t>(owner, 0x444)) {
            setPhase(3);
            return -1;
        }
        if (first.startCountdown-- == 1) {
            first.outcome = 2;
            first.failure = 20;
            setPhase(4);
            return 0;
        }
        const bool constrained = ((static_cast<std::uint16_t>(first.program - 4)) & 0xfffb) == 0;
        if (constrained ? !(actor->followupFlags & 4) : !(g_inputBuffer[0].newPresses & 0xf606))
            return -1;
        first.frameCountdown = second.frameCountdown = 120;
        first.maximumWindow = second.maximumWindow = 120;
        setPhase(2);
    } else if (phase == 3) {
        RecordInstructionInput(actor);
        if (first.endCountdown-- == 1) {
            first.timedOut = 1;
            if (first.progress != 1 && (!first.analysis || first.analysis == 13)) {
                first.progress = 2;
                const auto diagnosis = DiagnoseInstructionProgram(owner, first);
                first.analysis = diagnosis.kind;
                first.diagnostic = diagnosis.mask;
                first.delay = diagnosis.delay;
                first.committedAttempts = first.attempts;
                second.committedAttempts = second.attempts;
            }
            setPhase(4);
        }
        return phase == 4 ? 0 : -1;
    } else if (phase == 4)
        return 0;
    else
        return -1;
    if (first.skippedFrames) {
        auto index = InstructionValue<std::int8_t>(owner, 0x438);
        InstructionStore(owner, 0x78 + 8 * index, std::uint16_t{0});
        InstructionStore(owner, 0x7a + 8 * index, std::uint16_t{0});
        ++index;
        InstructionStore(owner, 0x438, index == 120 ? std::int8_t{0} : index);
        --first.skippedFrames;
    } else
        RecordInstructionInput(actor);
    if (InstructionValue<std::uint8_t>(owner, 0x444)) {
        setPhase(3);
        return -1;
    }
    if (first.frameCountdown-- == 1) {
        if (!first.ended && !second.ended) {
            if (InstructionValue<std::uint8_t>(owner, 0x444) != 2 && second.attempts &&
                static_cast<std::int8_t>(second.attempts) > static_cast<std::int8_t>(first.attempts) && !InstructionCloseBetweenCharacters(owner, second)) {
                InstructionStore(owner, 0x446, std::uint8_t{1});
                second.analysis = 23;
                second.outcome = 1;
                first.done = 1;
                if (!InstructionValue<std::uint8_t>(owner, 0x444))
                    InstructionStore(owner, 0x444, std::uint8_t{1});
                InstructionStore(owner, 0x447, std::uint8_t{1});
                setPhase(4);
                return -1;
            }
            first.outcome = 2;
            first.timedOut = 1;
            if (!first.attempts && !second.attempts)
                InstructionStore(owner, 0x444, std::uint16_t{514});
            setPhase(3);
            return -1;
        }
        first.done = second.done = 1;
    }
    if (second.program != -1 && !second.done)
        RunInstructionProgram(actor, owner, second);
    if (!first.done)
        RunInstructionProgram(actor, owner, first);
    if (!first.done || !second.done)
        return -1;
    if (second.program != -1 && second.ended) {
        if (InstructionValue<std::uint8_t>(owner, 0x444) != 2 && !InstructionCloseBetweenCharacters(owner, second)) {
            QueueInstructionMotion(owner, second.motion);
            InstructionStore(owner, 0x446, std::uint8_t{1});
            second.analysis = 22;
            if (!InstructionValue<std::uint8_t>(owner, 0x444))
                InstructionStore(owner, 0x444, std::uint8_t{1});
            InstructionStore(owner, 0x447, std::uint8_t{1});
            second.outcome = 1;
            setPhase(4);
        }
        return phase == 4 ? 0 : -1;
    }
    if (!first.ended) {
        if (first.progress == 2) {
            if (!first.attempts && !second.attempts)
                InstructionStore(owner, 0x444, std::uint16_t{514});
            first.outcome = 2;
            setPhase(3);
        }
        return -1;
    }
    if (first.timedOut) {
        if (!first.attempts && !second.attempts)
            InstructionStore(owner, 0x444, std::uint16_t{514});
        first.failure = 17;
        first.outcome = 2;
        setPhase(3);
        return -1;
    }
    auto close = InstructionValue<std::uint8_t>(owner, 0x444) == 2 ? std::uint8_t{3} : InstructionCloseBetweenCharacters(owner, first);
    if (!close) {
        InstructionStore(owner, 0x446, std::uint8_t{1});
        if (!InstructionValue<std::uint8_t>(owner, 0x444))
            InstructionStore(owner, 0x444, std::uint8_t{1});
        if (second.attempts && static_cast<std::int8_t>(second.attempts) > static_cast<std::int8_t>(first.attempts)) {
            second.analysis = 23;
            second.outcome = 1;
        } else {
            first.outcome = 1;
            QueueInstructionMotion(owner, first.motion);
        }
        InstructionStore(owner, 0x447, std::uint8_t{1});
        setPhase(4);
        return 0;
    }
    if (close <= 3) {
        if (!first.attempts)
            owner->requestedMotion = 394;
        if (close == 3 && !first.attempts && !second.attempts)
            InstructionStore(owner, 0x444, std::uint16_t{514});
        first.failure = close == 1 ? 18 : close == 2 ? 21 : 19;
        first.outcome = 2;
        setPhase(3);
    }
    return -1;
}
void InitializeInstructionHistory(VFHD_t *owner) {
    std::memset(reinterpret_cast<std::byte *>(owner) + 0x78, 0, 120 * 8);
    InstructionStore(owner, 0x438, std::uint8_t{0});
    InstructionStore(owner, 0x518, std::uint16_t{0});
    InstructionStore(owner, 0x51a, std::uint16_t{0});
    InstructionStore(owner, 0x449, static_cast<std::uint8_t>(InstructionValue<std::uint8_t>(owner, 0x443) != 0));
    for (auto offset : {0x4cd, 0x444, 0x445, 0x4a0, 0x4a1, 0x446, 0x4cc})
        InstructionStore(owner, offset, std::uint8_t{0});
    InstructionStore(owner, 0x460, std::uint64_t{0});
    InstructionStore(owner, 0x447, std::uint16_t{0});
    InstructionStore(owner, 0x46e, std::uint16_t{0});
    InstructionStore(owner, 0x470, std::uint16_t{0});
    owner->frame = 0;
    InstructionStore(owner, 0x46c, static_cast<std::uint16_t>(50 - static_cast<std::int32_t>(double(IncrementSharedPrngFloat()) * -10.0)));
    InstructionStore(owner, 0x4ce, std::uint16_t{0});
    InstructionStore(owner, 0x450, -1.0f);
    InstructionStore(owner, 0x454, -1.0f);
    InstructionStore(owner, 0x458, std::uint8_t{0});
    for (auto offset : {0x4bc, 0x4c0, 0x4c4, 0x4c8})
        InstructionStore(owner, offset, std::uint32_t{0});
    figpAssignModeForCharacter(owner->character, 0);
    EV_figpAssignFlagsFor(owner->character, 0, 1);
    if (InstructionValue<shen_name>(owner, 0x478).name)
        EV_figpAssignFlagsFor(owner->secondCharacter, 0, 1);
}
void PublishInstructionActorMotion(VFActor *actor, Character *character, shen_name name, std::int32_t motion) {
    auto *const owner = actor->game->dialogue;
    FindMotionWork(character);
    const auto yaw = static_cast<std::int32_t>(GetAllAngYForNode(GetCharacterNode(character)));
    if (name.name == InstructionValue<shen_name>(owner, 0x474).name) {
        InstructionStore(owner, 0x4c4, motion);
        InstructionStore(owner, 0x4bc, yaw);
    } else {
        InstructionStore(owner, 0x4c8, motion);
        InstructionStore(owner, 0x4c0, yaw);
    }
    actor->parameter370 = static_cast<std::uint64_t>(static_cast<std::int64_t>(motion));
    actor->reactionDepth = 0;
}
void ConvergeInstructionYaw(Character *character, const Vector3 &target, std::int32_t maximumStep) {
    const auto position = GetCharacterWorldPosition(character);
    auto rotation = GetCharacterWorldRotation(character);
    const auto desired = static_cast<std::uint16_t>(FixedAtan2(target.x - position.x, target.z - position.z) + 32768);
    const auto clockwise = static_cast<std::uint16_t>(desired - rotation.y);
    std::int32_t delta;
    if (clockwise >= static_cast<std::uint16_t>(rotation.y - desired))
        delta = static_cast<std::uint16_t>(rotation.y - desired);
    else if (desired <= rotation.y)
        delta = -std::int32_t(clockwise);
    else
        delta = rotation.y - desired;
    if (std::abs(delta) > maximumStep)
        rotation.y += delta < 0 ? maximumStep : -maximumStep;
    SetCharacterWorldRotation(character, rotation);
}
void AdjustInstructionPairPosition(VFActor *actor, const Vector3 &start, const Vector3 &otherStart) {
    if ((actor->motionFlags & (VFMOF_Unknown200 | VFMOF_Unknown4000)) != 0x200 &&
        !((actor->motionFlags & VFMOF_Unknown100) && (actor->hitStateFlags & VFHSF_Unknown4) && !(actor->motionFlags & VFMOF_Unknown80000)))
        return;
    const auto position = GetCharacterWorldPosition(actor->character);
    const float dx = position.x - otherStart.x, dz = position.z - otherStart.z;
    const float distance = std::sqrt(dx * dx + dz * dz);
    const Vector3 separation{start.x - otherStart.x, start.y - otherStart.y, start.z - otherStart.z};
    const float scale = distance / std::sqrt((separation.y * separation.y + separation.x * separation.x) + separation.z * separation.z);
    const float x = separation.x * scale + otherStart.x, z = separation.z * scale + otherStart.z;
    if (double((position.x - x) * (position.x - x) + (position.z - z) * (position.z - z)) > .0025) {
        actor->position.x = (x + position.x) * .5f;
        actor->position.z = (z + position.z) * .5f;
    }
}
bool InstructionMotionEnded(Character *character) {
    return static_cast<std::uint16_t>(SetMotionRegionFlag(FindMotionWork(character), 65, 1, MotionRegion::LowerBody)) != 0;
}
std::uint16_t InstructionAnimation(Character *character) {
    return GetMotionAnimationId(FindMotionWork(character), MotionRegion::LowerBody);
}
// Real FGTM reaction publication, FIGP playback and paired pose retention.
// @verified - p
std::uint8_t UpdateInstructionPose(VFActor *actor) {
    auto *const owner = actor->game->dialogue;
    if (!owner)
        return 0;
    auto &first = FirstInstruction(owner);
    auto &second = SecondInstruction(owner);
    bool result = false, firstPublished = false, secondPublished = false;
    const auto mode = InstructionValue<std::int8_t>(owner, 0x443);
    if (mode >= 1) {
        const auto firstPhase = InstructionValue<std::int8_t>(owner, 0x444);
        if (firstPhase) {
            const auto time = static_cast<std::int16_t>(InstructionValue<std::uint16_t>(owner, 0x468) + 1);
            InstructionStore(owner, 0x468, time);
            if (firstPhase == 1 && time >= InstructionValue<std::int16_t>(owner, 0x44a) && time <= InstructionValue<std::int16_t>(owner, 0x44c) &&
                owner->currentMotion != 394 && InstructionHitWindowAdmitted(owner, InstructionValue<shen_name>(owner, 0x474))) {
                InstructionStore(owner, 0x470, std::uint16_t{3});
                InstructionStore(owner, 0x444, std::uint32_t{0x1010202});
            }
        }
        const auto secondPhase = InstructionValue<std::int8_t>(owner, 0x445);
        if (secondPhase) {
            const auto time = static_cast<std::int16_t>(InstructionValue<std::uint16_t>(owner, 0x46a) + 1);
            InstructionStore(owner, 0x46a, time);
            if (secondPhase == 1 && time >= InstructionValue<std::int16_t>(owner, 0x45a) && time <= InstructionValue<std::int16_t>(owner, 0x45c) &&
                InstructionHitWindowAdmitted(owner, InstructionValue<shen_name>(owner, 0x478))) {
                InstructionStore(owner, 0x46e, std::uint16_t{3});
                InstructionStore(owner, 0x444, std::uint16_t{514});
                InstructionStore(owner, 0x448, std::uint8_t{1});
                if (!first.failure) {
                    first.outcome = 2;
                    first.failure = 19;
                }
            }
        } else if (InstructionValue<std::uint8_t>(owner, 0x442) == 3) {
            auto time = InstructionValue<std::int16_t>(owner, 0x46c);
            if (time == 10) {
                const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                auto *const character = scenFindChara(control->scenarioTask, InstructionValue<shen_name>(owner, 0x474));
                auto *const game = vfmaGetTaskParam();
                if (game && character) {
                    for (std::size_t i = 0; i != 6; ++i)
                        if (game->characters[i] == character) {
                            InstructionStore(reinterpret_cast<VFHD_t *>(game->actors[i]), 0x898, std::uint64_t{2});
                            break;
                        }
                }
            }
            if ((cvsHasValidConverseData() && GetConverseDataForChara(SHEN_NAME(SYE_))) || (actor->game->mode & 0x8000)) {
                if (time && time != 15)
                    InstructionStore(owner, 0x46c, static_cast<std::int16_t>(time - 1));
            } else if (time) {
                InstructionStore(owner, 0x46c, --time);
                if (time == 10 && mode == 2)
                    QueueInstructionDialogue(owner, 2);
                if (!time && mode == 2) {
                    InstructionStore(owner, 0x464, std::uint32_t{2326});
                    owner->secondRequestedMotion = 2326;
                    InstructionStore(owner, 0x46a, std::uint16_t{0});
                    InstructionStore(owner, 0x445, std::uint8_t{1});
                    const auto entry = FindLoadedMotionSecondaryEntry(2326, 2);
                    InstructionStore(
                        owner, 0x45a, static_cast<std::uint16_t>(InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(entry.data()), 2) - 1));
                    InstructionStore(owner, 0x45c, InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(entry.data()), 4));
                }
            } else
                InstructionStore(owner, 0x46c, std::uint16_t{1});
        }
    }
    const bool successful = (first.outcome == 1 || second.outcome == 1) && InstructionValue<std::uint8_t>(owner, 0x446) == 1;
    if (owner->requestedMotion && (owner->requestedMotion != owner->currentMotion || InstructionValue<std::uint8_t>(owner, 0x4a0))) {
        bool skipPublication = false;
        InstructionAnimation(owner->character);
        if (successful) {
            if (InstructionValue<std::uint8_t>(owner, 0x458) == 1)
                skipPublication = true;
            else {
                EV_figpAssignFlagsFor(owner->character, 0, 1);
                figpAssignMotScaleAndDirFor(owner->character, 8, 2);
                figpAssignModeForCharacter(owner->character, 1);
                if (InstructionAnimation(owner->character) == 306 && owner->requestedMotion == 306)
                    skipPublication = true;
                else if (((static_cast<std::uint16_t>(owner->programIndices[0] - 4)) & 0xfffb) == 0 &&
                    actor->animationTime < float(static_cast<std::uint16_t>(actor->parameter2d0)))
                    return 0;
            }
        } else if (InstructionAnimation(owner->character) == 306 && owner->requestedMotion == 306)
            skipPublication = true;
        if (!skipPublication)
            PublishInstructionActorMotion(actor, owner->character, InstructionValue<shen_name>(owner, 0x474), owner->requestedMotion);
        firstPublished = true;
        InstructionStore(owner, 0x4a0, std::uint8_t{0});
        const float start = InstructionValue<float>(owner, 0x450), end = InstructionValue<float>(owner, 0x454);
        if (start > 0)
            SetMotionRegionEvalTime(FindMotionWork(owner->character), start, MotionRegion::LowerBody);
        if (end > 0)
            SetMotionLayerEndFrame(FindMotionWork(owner->character), end, MotionRegion::LowerBody);
        owner->currentMotion = owner->requestedMotion;
        InstructionStore(owner, 0x450, -1.0f);
        InstructionStore(owner, 0x454, -1.0f);
    }
    if (owner->secondRequestedMotion && (owner->secondRequestedMotion != owner->secondCurrentMotion || InstructionValue<std::uint8_t>(owner, 0x4a1))) {
        if (mode) {
            PublishInstructionActorMotion(actor->actor, owner->secondCharacter, InstructionValue<shen_name>(owner, 0x478), owner->secondRequestedMotion);
            InstructionStore(owner, 0x4a1, std::uint8_t{0});
            secondPublished = true;
        }
        owner->secondCurrentMotion = owner->secondRequestedMotion;
    }
    if (mode) {
        if (InstructionAnimation(owner->character) == 306 && !firstPublished)
            ConvergeInstructionYaw(owner->character, GetCharacterWorldPosition(owner->secondCharacter), 1820);
        if (InstructionAnimation(owner->secondCharacter) == 306 && !secondPublished)
            ConvergeInstructionYaw(owner->secondCharacter, GetCharacterWorldPosition(owner->character), 1820);
    }
    if (actor->targetingCharacter) {
        const auto firstStart = InstructionValue<Vector3>(owner, 0x500), secondStart = InstructionValue<Vector3>(owner, 0x50c);
        AdjustInstructionPairPosition(actor, firstStart, secondStart);
        AdjustInstructionPairPosition(actor->actor, secondStart, firstStart);
    }
    if (successful && InstructionValue<std::uint8_t>(owner, 0x458) == 1) {
        owner->currentMotion = 0;
        figpAssignModeForCharacter(owner->character, 0);
        result = true;
    } else if (owner->currentMotion && owner->currentMotion != 306 && InstructionMotionEnded(owner->character)) {
        owner->currentMotion = 0;
        figpAssignModeForCharacter(owner->character, 0);
    }
    if (owner->secondCurrentMotion && owner->secondCurrentMotion != 306 && InstructionMotionEnded(owner->secondCharacter))
        owner->secondCurrentMotion = 0;
    if (!owner->currentMotion && !owner->secondCurrentMotion)
        result = false;
    if (InstructionValue<std::uint32_t>(owner, 0x4c8) == 476 && InstructionMotionEnded(owner->secondCharacter))
        result = true;
    if (owner->requestedMotion == owner->currentMotion)
        owner->requestedMotion = 0;
    if (owner->secondRequestedMotion == owner->secondCurrentMotion)
        owner->secondRequestedMotion = 0;
    if (owner->currentMotion == 306 && owner->secondCurrentMotion == 306)
        result = true;
    if (successful && InstructionValue<std::uint8_t>(owner, 0x458) == 1)
        return 1;
    return result;
}
}

// Names belong to FENE, not a separately manufactured character view.
VFActor *vfGetActorFromSlot(VFMA_t *game, shen_name name) {
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->actors[i]->slot && game->actors[i]->slot->id == name.name)
            return game->actors[i];
    return nullptr;
}


// @verified - p
const char *vfGetFightDialogueVoice(VFMA_t *game, std::int16_t index) {
    if (game->reserved318[0] == std::byte{0xff})
        return nullptr;
    return DialogueBank(*game)[index].voice;
}

// Select an authored dialogue without replacing its bank.
// @verified - p
void vfSetFightDialogue(VFMA_t *game, std::int16_t index) {
    if (game->reserved318[0] == std::byte{0xff})
        return;
    SetDialogueIndex(*game, index);
    game->frameDelta = static_cast<float>(DialogueBank(*game)[index].delay);
}

// Reset only played flags; authored delays and links remain intact.
// @verified - p
void vfInitializeFightDialogue(VFMA_t *game) {
    SetDialogueIndex(*game, -1);
    game->frameDelta = 0;
    dialoguePlayerFocus = 0;
    game->reserved318[0] = std::byte{3};
    for (auto *record = dialogueBanks[3]; record->voice; ++record)
        record->played = 0;
}

bool vfHasDialoguePlayerFocus() {
    return dialoguePlayerFocus != 0;
}

// The real dialogue clock, actor readiness gate and CAMT request owner.
// @verified - p
void vfmaSetupDialogueReactionsEtc(VFMA_t *game) {
    using enum CameraState;
    auto index = DialogueIndex(*game);
    if (game->mode & 4) {
        game->frameDelta = 0;
        return;
    }
    if (index == -1)
        return;
    if (game->frameDelta > 0) {
        game->frameDelta -= shenGetScaledDeltaTickTime(1.0f);
        if (game->frameDelta < 0)
            game->frameDelta = 0;
        return;
    }
    if (index == -2) {
        game->mode &= ~1U;
        SetDialogueIndex(*game, -1);
        camCtrlSetState(ChaseGoal);
        dialoguePlayerFocus = 0;
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            EV_SetCharDisplay(game->characters[i], CVM_DisplayEnabled, 1);
            auto &slot = *game->enemySlots[i];
            if (slot.savedControlFlags & 0x40000000) {
                cpamSetFlag(game->characters[i], 1);
                slot.savedControlFlags &= ~0x40000000U;
            }
        }
        return;
    }
    auto *const bank = DialogueBank(*game);
    auto &record = bank[index];
    if (record.camera) {
        game->mode |= 1;
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            auto &actor = *game->actors[i];
            if ((actor.hitStateFlags & (VFHSF_AttackGuardWindow | VFHSF_SpecialGuardOrParry)) != 0x200 || actor.motionTimer || actor.animationTime < 2)
                return;
        }
        const auto &camera = dialogueCameras[record.camera];
        if (auto *const speaker = vfGetActorFromSlot(game, camera.character)) {
            if (camera.character.name == SHEN_NAME(RYO_))
                dialoguePlayerFocus = 1;
            if (camera.expression != -1)
                SetHighModelAndFaceFlagsForChara(speaker->character, camera.expression, 15, 1);
            if (camera.target.name) {
                if (auto *const target = vfGetActorFromSlot(game, camera.target)) {
                    const auto origin = GetCharacterWorldPosition(speaker->character);
                    const auto destination = GetCharacterWorldPosition(target->character);
                    shenDecrementRotYAllNodes(
                        GetCharacterNode(speaker->character), static_cast<std::int16_t>(FixedAtan2(origin.x - destination.x, origin.z - destination.z)));
                }
            }
            if (speaker->moveFlags & 0x40)
                camCtrlSetSplineShouldRebuildRotTo(true);
            const std::uintptr_t request[3]{
                camera.camera, reinterpret_cast<std::uintptr_t>(speaker->character), reinterpret_cast<std::uintptr_t>(speaker->character)};
            camCtrlToggleSplineRebuildFlag(3, request);
            camCtrlSetState(Scripted);
            camCtrlSetFovTransitionTo(true);
            camCtrlDisableFilmBars();
            if (camera.isolate) {
                for (std::uint8_t i = 0; i < 6; ++i) {
                    if (!game->characters[i])
                        continue;
                    const bool isSpeaker = game->actors[i] == speaker;
                    EV_SetCharDisplay(game->characters[i], CVM_DisplayEnabled, isSpeaker);
                    if (isSpeaker) {
                        if (shenCheckCharaHasAlphaManager(game->characters[i]))
                            game->enemySlots[i]->savedControlFlags |= 0x40000000;
                        cpamSetFlag(game->characters[i], 0);
                    }
                }
            }
        }
    }
    if (record.voice)
        EV_PlayDialogue(record.voice);
    record.played = 1;
    index = record.next;
    SetDialogueIndex(*game, index);
    game->frameDelta = static_cast<std::uint16_t>(index + 2) <= 1 ? 0 : static_cast<float>(bank[index].delay);
}


// @verified - p
std::byte *vfSetMoveInstructionConfiguration(
    shen_name character, shen_name secondCharacter, std::int16_t program, std::int16_t secondProgram, std::byte *borrowedResult) {
    instructionCharacter = character;
    instructionSecondCharacter = secondCharacter;
    instructionProgram = program;
    instructionSecondProgram = secondProgram;
    instructionResult = borrowedResult;
    return borrowedResult;
}


// @verified - p
VFHD_t *AllocateVFHD(VFMA_t *game) {
    auto *const owner = static_cast<VFHD_t *>(HLib::AllocateTaskData(0x520, SHEN_NAME(VFHD)));
    HLib::AssignDynamicAlignmentForTaskData(owner);
    owner->game = game;
    InstructionStore(owner, 0x440, std::uint8_t{0});
    return owner;
}
// @verified - p
void vfFreeVFHDSpace(VFMA_t *game) {
    if (game->dialogue) {
        HLib::FreeHeapSpace(game->dialogue, SHEN_NAME(VFHD));
        game->dialogue = nullptr;
    }
}

// Consume the genuine scenario/configuration producer.
// @verified - p
std::int32_t vfApplyMoveInstructionConfiguration(VFHD_t *owner) {
    owner->programIndices[0] = static_cast<std::uint16_t>(instructionProgram);
    owner->programIndices[1] = static_cast<std::uint16_t>(instructionSecondProgram);
    owner->result = instructionResult;
    InstructionStore(owner, 0x474, instructionCharacter);
    InstructionStore(owner, 0x478, instructionSecondCharacter);
    InstructionStore(owner, 0x440, std::uint8_t{0});
    owner->requestedMotion = owner->secondRequestedMotion = 0;
    owner->currentMotion = owner->secondCurrentMotion = 0;
    owner->result[0] = owner->result[1] = owner->result[2] = owner->result[7] = std::byte{0};
    owner->result[3] = std::byte{0xff};
    std::memset(owner->result + 8, 0, 4);
    InstructionStore(owner, 0x46a, std::uint16_t{0});
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    owner->character = scenFindChara(control->scenarioTask, instructionCharacter);
    owner->secondCharacter = instructionSecondCharacter.name ? scenFindChara(control->scenarioTask, instructionSecondCharacter) : nullptr;
    return SetPlayerControlEnabled(0);
}
namespace {
std::int8_t instructionProgramResult{}, instructionPoseResult{};
std::int32_t instructionCompletionDelay{};
std::uint8_t InstructionRequiredInputs(const InstructionState &state) {
    if (state.program == -1)
        return 0;
    std::uint8_t count = 0;
    for (const auto *command = instructionPrograms[state.program].commands; command->operation != 6; ++command)
        if (command->operation == 7)
            ++count;
    return count;
}
bool SuccessfulInstruction(const VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    return (first.outcome == 1 || second.outcome == 1) && InstructionValue<std::uint8_t>(owner, 0x446) == 1;
}
bool InstructionPreviewPastEnd(VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    return SuccessfulInstruction(owner, first, second) && first.endFrame > 0 &&
        (static_cast<std::uint32_t>(first.motion) & 0xf7ffffff) == InstructionAnimation(owner->character) &&
        GetMotionFrame(FindMotionWork(owner->character), MotionRegion::LowerBody) > first.endFrame;
}
void ResetInstructionPreview(VFHD_t *owner) {
    EV_figpAssignFlagsFor(owner->character, 0, 0);
    figpAssignModeForCharacter(owner->character, 0);
}
void PublishInstructionResult(VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    owner->result[0] = std::byte{2};
    owner->result[3] = static_cast<std::byte>(first.endInstruction);
    if (SuccessfulInstruction(owner, first, second)) {
        owner->result[1] = std::byte{1};
        if (second.outcome == 1) {
            if (second.analysis == 22)
                owner->result[2] = std::byte{22};
            else if (second.analysis == 23) {
                owner->result[2] = std::byte{23};
                owner->result[6] = static_cast<std::byte>(second.committedAttempts);
            }
            const auto detail = InstructionValue<std::uint32_t>(owner, 0x6a);
            std::memcpy(owner->result + 8, &detail, 4);
        } else {
            const auto detail = InstructionValue<std::uint32_t>(owner, 0x32);
            std::memcpy(owner->result + 8, &detail, 4);
        }
    } else {
        owner->result[1] = std::byte{2};
        if (first.analysis) {
            owner->result[2] = static_cast<std::byte>(first.analysis);
            std::memcpy(owner->result + 8, &first.diagnostic, 4);
            owner->result[5] = static_cast<std::byte>(first.delay);
            owner->result[6] = static_cast<std::byte>(first.committedAttempts);
        } else
            owner->result[2] = static_cast<std::byte>(first.failure);
    }
}
}

// The player FGTM owns the full instruction/preview/result lifecycle.
// @verified - p
void vfUpdateVFHDState(VFActor *actor) {
    auto *const game = actor->game;
    if (actor->groupIndex != static_cast<std::uint8_t>(game->playerSlot) || !game->dialogue)
        return;
    auto *const owner = game->dialogue;
    auto &first = FirstInstruction(owner);
    auto &second = SecondInstruction(owner);
    const auto setState = [&](std::uint8_t state) { InstructionStore(owner, 0x440, state); };
    switch (InstructionValue<std::uint8_t>(owner, 0x440)) {
    case 0: {
        vfApplyMoveInstructionConfiguration(owner);
        InstructionStore(owner, 8, static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int16_t>(owner->programIndices[0]))));
        InstructionStore(owner, 0x40, static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int16_t>(owner->programIndices[1]))));
        const auto &program = instructionPrograms[first.program];
        InstructionStore(owner, 0x443, program.partnerMode);
        InstructionStore(owner, 0x442, program.mode);
        owner->currentMotion = owner->secondCurrentMotion = 0;
        InitializeInstructionHistory(owner);
        InitializeInstructionState(first);
        InitializeInstructionState(second);
        setState(1);
        InstructionStore(owner, 0x4d0, GetCharacterWorldPosition(owner->character));
        const auto rotation = GetCharacterWorldRotation(owner->character);
        InstructionStore(owner, 0x4e8, rotation);
        InstructionStore(owner, 0x4b8, rotation.y);
        InstructionStore(owner, 0x500, GetCharacterWorldPosition(actor->character));
        InstructionStore(owner, 0x50c, GetCharacterWorldPosition(actor->actor->character));
        mtwkSetExtraFlag(FindMotionWork(owner->character), 6, 0);
        for (std::size_t i = 1; i != 8; ++i)
            game->slotPoolModes[i] = program.partnerMode ? 1 : 6;
        break;
    }
    case 1:
        instructionProgramResult = UpdateInstructionPrograms(actor);
        UpdateInstructionPose(actor);
        if (instructionProgramResult != -1) {
            game->slotPoolModes[0] = 6;
            setState(2);
        }
        if (SuccessfulInstruction(owner, first, second)) {
            game->slotPoolModes[0] = 6;
            if (InstructionPreviewPastEnd(owner, first, second))
                setState(3);
        }
        if (actor->followupFlags)
            InstructionStore(owner, 0x4cc, static_cast<std::uint8_t>(InstructionValue<std::uint8_t>(owner, 0x4cc) + 1));
        break;
    case 2:
        if (InstructionPreviewPastEnd(owner, first, second)) {
            ResetInstructionPreview(owner);
            setState(3);
        } else {
            instructionPoseResult = UpdateInstructionPose(actor);
            if ((first.outcome == 2 || first.endFrame == 0) &&
                (actor->animationTime >= float(actor->reactionEndFrame) - actor->scale || InstructionAnimation(owner->character) == 306)) {
                ResetInstructionPreview(owner);
                setState(3);
            }
            const auto maximumInputs = std::max(InstructionRequiredInputs(first), InstructionRequiredInputs(second));
            if (InstructionValue<std::int8_t>(owner, 0x4cc) > std::int32_t(maximumInputs) + 1) {
                game->slotPoolModes[0] = 6;
                if (InstructionAnimation(owner->character) == 306)
                    setState(3);
            }
        }
        break;
    case 3:
        PublishInstructionResult(owner, first, second);
        if (!SuccessfulInstruction(owner, first, second))
            vfFightNullsub();
        instructionCompletionDelay = 1;
        setState(4);
        UpdateInstructionPose(actor);
        break;
    case 4:
        UpdateInstructionPose(actor);
        if (--instructionCompletionDelay == 0)
            setState(5);
        break;
    case 5:
        setState(6);
        vfFreeVFHDSpace(game);
        break;
    default:
        break;
    }
    if (game->dialogue) {
        auto delay = InstructionValue<std::uint16_t>(owner, 0x518);
        if (delay) {
            InstructionStore(owner, 0x518, --delay);
            if (!delay)
                EV_PlayDialogue(instructionVoices[InstructionValue<std::int16_t>(owner, 0x51a)].voice);
        }
        ++owner->frame;
    }
}

// @verified - p
std::int32_t SetTrainingFightReaction(std::uint16_t reaction) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    if (!game->dialogue)
        game->dialogue = AllocateVFHD(game);
    VFHD_t *const owner = game->dialogue;
    InstructionStore(owner, 0x440, std::uint8_t{0});
    game->slotPoolModes[0] = 0;
    const shen_name character = InstructionValue<shen_name>(owner, 0x474);
    if (vfmDispatchControl(FightControlOp::ReactionRank, character, 0, -1) == 2)
        vfmDispatchControl(FightControlOp::ReactionRank, character, 1, 1);
    if (reaction) {
        VFActor *const actor = game->actors[game->playerSlot];
        FindMotionWork(owner->character);
        InstructionStore(owner, 0x4c4, static_cast<std::uint32_t>(reaction));
        InstructionStore(owner, 0x4bc, static_cast<std::uint32_t>(GetAllAngYForNode(&owner->character->entity.node)));
        actor->parameter370 = reaction;
        actor->reactionDepth = 0;
    }
    return 1;
}

} // namespace shenmue::s2
