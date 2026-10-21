#include "s2/npc_area_route.hpp"

#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"
#include "s2/motion_pose.hpp"
#include "s2/player.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/weather.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
namespace {

struct NpcAreaStoreSlot {
    NpcAreaRouteState *state;
    NpcAreaStoreSlot *previous;
    NpcAreaStoreSlot *next;
    float distanceSquared;
    std::uint32_t reserved;
};
static_assert(sizeof(NpcAreaStoreSlot) == 0x20);

NpcAreaStoreSlot *areaStore{};
NpcAreaStoreSlot *nearestAreaSlot{};
std::size_t areaStoreCount{};
std::span<NpcAreaPoint> areaPoints;
Vector3 routeOrigin{};
struct NpcAreaProbe {
    std::int8_t rowIndex{-1};
    std::int8_t linkedRowIndex{-1};
    std::int8_t ownerIndex{-1};
    std::int8_t nextOwnerIndex{-1};
    std::int8_t ownerRowIndex{-1};
    std::byte reserved05[3]{};
    float rowParameter{};
    float ownerParameter{};
    float lateralDistance{};
    std::uint32_t reserved14{};
    NpcAreaRouteState *state{};
};
static_assert(sizeof(NpcAreaProbe) == 0x20);
NpcAreaProbe areaProbes[2];

template <class T> T *HeapPointer(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(HLib::DynamicHeapBase + offset) : nullptr;
}

std::int32_t HeapOffset(const void *pointer) {
    if (!pointer)
        return 0;
    const auto offset = reinterpret_cast<std::intptr_t>(pointer) - reinterpret_cast<std::intptr_t>(HLib::DynamicHeapBase);
    assert(offset >= (std::numeric_limits<std::int32_t>::min)() &&
           offset <= (std::numeric_limits<std::int32_t>::max)());
    return static_cast<std::int32_t>(offset);
}

NpcAreaControlRow *ControlRows(const NpcAreaRouteState &state) {
    return HeapPointer<NpcAreaControlRow>(state.controlRowsOffset);
}

NpcAreaOwnerRow *OwnerRows(const NpcAreaRouteState &state) {
    return HeapPointer<NpcAreaOwnerRow>(state.ownerRowsOffset);
}

NpcRouteLiveEntry *ListHead(const NpcAreaRouteState &state, std::size_t list) {
    const std::int32_t offset = state.npcListHeadOffset[list];
    return offset ? reinterpret_cast<NpcRouteLiveEntry *>(reinterpret_cast<std::uintptr_t>(&state) + offset) : nullptr;
}

void SetListHead(NpcAreaRouteState &state, NpcRouteLiveEntry *entry, std::size_t list) {
    if (!entry) {
        state.npcListHeadOffset[list] = 0;
        return;
    }
    const auto offset = reinterpret_cast<std::intptr_t>(entry) - reinterpret_cast<std::intptr_t>(&state);
    assert(offset >= (std::numeric_limits<std::int32_t>::min)() &&
           offset <= (std::numeric_limits<std::int32_t>::max)());
    state.npcListHeadOffset[list] = static_cast<std::int32_t>(offset);
}

// npcStoreRegisterAreaRouteState. Each offset is relative to
// its state on disk, but becomes relative to the dynamic heap after publication.
void RegisterAreaState(NpcAreaRouteState &state, std::int16_t point) {
    NpcAreaStoreSlot &slot = areaStore[areaStoreCount];
    slot.state = &state;
    if (areaStoreCount) {
        slot.previous = &areaStore[areaStoreCount - 1];
        slot.previous->next = &slot;
    } else if (!nearestAreaSlot) {
        nearestAreaSlot = &slot;
    }
    ++areaStoreCount;
    auto *const base = reinterpret_cast<std::byte *>(&state);
    if (state.animationScriptOffset)
        state.animationScriptOffset = HeapOffset(base + state.animationScriptOffset);
    state.controlRowsOffset = HeapOffset(base + state.controlRowsOffset);
    state.ownerRowsOffset = HeapOffset(base + state.ownerRowsOffset);
    NpcAreaControlRow *const controls = ControlRows(state);
    for (std::int32_t index = 0; index < state.controlRowCount; ++index) {
        NpcAreaControlRow &row = controls[index];
        if (row.forwardOffset)
            row.forwardOffset = HeapOffset(base + row.forwardOffset);
        if (row.backwardOffset)
            row.backwardOffset = HeapOffset(base + row.backwardOffset);
    }
    state.areaPointIndex = point;
    state.field0a = 0;
    state.npcListHeadOffset[0] = state.npcListHeadOffset[1] = 0;
    state.activeRowCount = state.freeRowCount = state.occupiedRowCount = state.participantCount = 0;
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &row = owners[index];
        if (row.linkedControlOffset) {
            row.linkedControlOffset = HeapOffset(base + row.linkedControlOffset);
            if ((row.flags & 3U) != 1U)
                ++state.freeRowCount;
        }
        row.runtimeMask = 0;
    }
    state.flags |= 8U;
}

std::uint8_t ScriptByte(const std::byte *command) {
    return static_cast<std::uint8_t>(*command);
}

std::uint16_t ScriptWord(const std::byte *command) {
    std::uint16_t value;
    std::memcpy(&value, command, sizeof(value));
    return value;
}

std::uint32_t ScriptDword(const std::byte *command) {
    std::uint32_t value;
    std::memcpy(&value, command, sizeof(value));
    return value;
}

const std::byte *NextInstruction(const std::byte *command) {
    const std::uint8_t opcode = ScriptByte(command);
    return command + (opcode == 'A' || opcode == 'M' ? 8 : opcode == 'T' ? 10 : 2);
}

const std::byte *PreviousInstruction(const std::byte *command) {
    const std::uint8_t opcode = ScriptByte(command);
    return command - (opcode == 'E' || opcode == 'm' ? 8 : opcode == 't' ? 10 : 2);
}

bool IsScriptSectionEnd(std::uint8_t opcode) {
    return opcode == 'i' || opcode == 'j' || opcode == 'n' || opcode == 'p' || opcode == 'r';
}

const std::byte *FindInstructionEnd(const std::byte *command) {
    for (;;) {
        const std::uint8_t opcode = ScriptByte(command);
        if (IsScriptSectionEnd(opcode))
            return command;
        if (opcode == 'P' || opcode == 'R') {
            command = FindInstructionEnd(command + 2) + 2;
        } else if (opcode == 'M') {
            do
                command = NextInstruction(command);
            while (ScriptByte(command) == 'M');
        }
    }
}

const std::byte *FindInstructionStart(const std::byte *command) {
    for (;;) {
        const std::uint8_t opcode = ScriptByte(command);
        if (opcode == 'I' || opcode == 'J' || opcode == 'N' || opcode == 'P' || opcode == 'R')
            return command;
        if (opcode == 'p' || opcode == 'r') {
            command = FindInstructionStart(command - 2) - 2;
        } else if (opcode == 'm') {
            do
                command = PreviousInstruction(command);
            while (ScriptByte(command) == 'm');
        }
    }
}

const std::byte *FindSection(const std::byte *command, std::uint8_t section) {
    while (ScriptByte(command) != 'a') {
        if (ScriptByte(command) == section)
            return command;
        command = NextInstruction(command);
    }
    return nullptr;
}

const std::byte *ChooseMotionInstruction(NpcRouteLiveEntry &entry, const std::byte *command) {
    for (;;) {
        const std::uint8_t opcode = ScriptByte(command);
        if (opcode == 'R') {
            float random = IncrementSharedPrngFloat();
            if (random == 1.0f)
                random = 0.0f;
            const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(command[1])) * random);
            const std::byte *option = NextInstruction(command);
            std::int32_t cumulative = 0;
            for (;;) {
                const std::byte *const end = ScriptByte(option) == 'M' ? option : FindInstructionEnd(NextInstruction(option));
                cumulative += static_cast<std::int8_t>(end[1]);
                if (cumulative > choice)
                    break;
                option = NextInstruction(end);
            }
            command = option;
        } else if (opcode == 'P') {
            if (!(entry.field2d0 >> 8))
                entry.field2d0 |= static_cast<std::uint16_t>(ScriptByte(command + 1)) << 8;
            command = NextInstruction(command);
        } else {
            return opcode == 'M' ? command : nullptr;
        }
    }
}

std::uint16_t BeginAnimationInstruction(NpcRouteLiveEntry &entry, const std::byte *command) {
    const std::byte *const motion = ChooseMotionInstruction(entry, command);
    if (!motion)
        return 0;
    entry.specialAnimationContext = motion;
    const std::uint8_t repeats = ScriptByte(FindInstructionEnd(motion)) == 'r' ? 1 : ScriptByte(motion + 1);
    entry.field2d0 = static_cast<std::uint16_t>((entry.field2d0 & 0xff00U) | repeats);
    return ScriptWord(motion + 2);
}

bool HasOccupiedPartner(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const std::int32_t selected = entry.nextOwnerRowIndex != -1 ? entry.nextOwnerRowIndex : entry.ownerRowIndex != -1 ? entry.ownerRowIndex : 0;
    const NpcAreaOwnerRow &owner = owners[selected];
    if (owner.flags & 0x20U) {
        for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
            if (index == selected)
                continue;
            if (((owner.flags & 3U) == 1) != ((owners[index].flags & 3U) == 1) && (owners[index].runtimeMask & 2U))
                return true;
        }
    } else {
        for (std::int8_t adjacent : owner.adjacentOwners) {
            if (adjacent == selected)
                continue;
            if (adjacent == -1)
                break;
            if (adjacent < state.ownerRowCount && (owners[adjacent].runtimeMask & 2U))
                return true;
        }
    }
    return false;
}

const std::byte *FindAnimationDefinition(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaOwnerRow &owner) {
    const std::byte *definition = HeapPointer<std::byte>(state.animationScriptOffset);
    const std::uint8_t ownerClass = !(owner.flags & 1U) ? 0 : (owner.flags & 2U) ? 1 : static_cast<std::uint8_t>(((owner.flags & 0x10U) | 0x20U) >> 4);
    for (;;) {
        if (ScriptByte(definition + 1) == owner.actionKind) {
            const std::uint32_t target = ScriptDword(definition + 2);
            bool matches;
            switch (target) {
            case SHEN_NAME(___G):
                matches = ownerClass == 1;
                break;
            case SHEN_NAME(___M):
                matches = ownerClass == 3;
                break;
            case SHEN_NAME(___S):
                matches = ownerClass == 2;
                break;
            case SHEN_NAME(____):
                matches = true;
                break;
            default:
                matches = target == entry.name.name;
                break;
            }
            if (matches)
                return definition;
        }
        while (ScriptByte(definition) != 'a')
            definition = NextInstruction(definition);
        if (ScriptByte(definition + 1) == 'e')
            return nullptr;
        definition = NextInstruction(definition);
    }
}

float AreaSine(std::int32_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(angle));
}

float AreaCosine(std::int32_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(angle))));
}

std::int32_t AreaYaw(const Vector3 &angles) {
    std::int32_t result;
    std::memcpy(&result, &angles.y, sizeof(result));
    return result;
}

void SetAreaYaw(Vector3 &angles, std::int32_t yaw) {
    std::memcpy(&angles.y, &yaw, sizeof(yaw));
}

float AreaDistanceSquared(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x;
    const float z = first.z - second.z;
    return x * x + z * z;
}

Vector3 AreaLerp(const Vector3 &first, const Vector3 &second, float parameter) {
    const float inverse = 1.0f - parameter;
    return {inverse * first.x + parameter * second.x, inverse * first.y + parameter * second.y, inverse * first.z + parameter * second.z};
}

std::uint32_t AreaMask(std::uint32_t bits, std::int32_t shift) {
    return bits << (static_cast<std::uint32_t>(shift) & 31U);
}

float AreaLaneWidth(const NpcAreaControlRow &row) {
    switch (row.laneMode) {
    case 0:
        return 0.34999999f;
    case 1:
        return 0.5f;
    case 2:
        return 1.0f;
    default:
        return 0.0f;
    }
}

float AreaSegmentParameter(const Vector3 &start, const Vector3 &end, const Vector3 &position) {
    const float x = position.x - start.x;
    const float z = position.z - start.z;
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    const std::int16_t axis = static_cast<std::int16_t>(FixedAtan2(-dx, -dz));
    const float parameter = AreaCosine(yaw - axis) * std::sqrt(x * x + z * z) / std::sqrt(dx * dx + dz * dz);
    return parameter > 1.0f ? 1.0f : std::fmax(0.0f, parameter);
}

float AreaRowParameter(const NpcAreaControlRow &row, const NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
    if (!next)
        return 0.0f;
    Vector3 position = entry.localPosition;
    position.x += routeOrigin.x;
    position.z += routeOrigin.z;
    return AreaSegmentParameter(row.position, next->position, position);
}

void AreaLinkedPosition(const NpcAreaOwnerRow &owner, Vector3 &position) {
    const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
    if (!row)
        return;
    const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row->forwardOffset);
    position = next ? AreaLerp(row->position, next->position, owner.positionAlongSegment) : row->position;
}

bool QueueAreaMotion(NpcRouteLiveEntry &entry, const Vector3 *target, const Vector3 *facing = nullptr, std::uint16_t yaw = 0, std::uint16_t motion = 0) {
    if (!entry.node)
        return false;
    if (!target) {
        CopyMotionTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
        CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
    } else {
        Vector3 position = NpcRouteAreaToScene(*target);
        if (facing) {
            CopyMotionTransition(entry.motionWork, &position, FixedAtan2(-(facing->x - target->x), -(facing->z - target->z)), 5, 0);
            return true;
        }
        if (motion) {
            CopyMotionTransition(entry.motionWork, &position, yaw, 0, 0);
            SetMotionTimeRelated(entry.motionWork, 1, 5, motion);
            return true;
        }
        CopyMotionTransition(entry.motionWork, &position, 0, 0, 1);
    }
    SetMotionTimeRelated(entry.motionWork, 0, 6, 0);
    return true;
}

std::int32_t AreaQueueLength(const NpcAreaRouteState &state) {
    std::int32_t count = 0;
    for (const NpcRouteLiveEntry *entry = ListHead(state, 0); entry; entry = entry->areaPointNext)
        ++count;
    return count;
}

void ReserveAreaEntrance(NpcAreaRouteState &state) {
    NpcAreaControlRow &first = ControlRows(state)[0];
    first.previousForwardMask |= 7U;
    first.previousBackwardMask |= 7U;
    first.forwardMask |= 7U;
    first.backwardMask |= 7U;
    if (NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(first.backwardOffset)) {
        const std::uint32_t mask = AreaMask(7, previous->spanCount - 1);
        previous->previousForwardMask |= mask;
        previous->previousBackwardMask |= mask;
        previous->forwardMask |= mask;
        previous->backwardMask |= mask;
    }
}

bool AreaOwnerLaneAvailable(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
    const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * owner.positionAlongSegment);
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    if (const NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(row.backwardOffset); span < 3 && previous) {
        const std::uint32_t occupied = forward ? previous->previousForwardMask : previous->previousBackwardMask;
        if (occupied & AreaMask(15, previous->spanCount + span - 2))
            return false;
    } else if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
               static_cast<std::int8_t>(row.spanCount) - span < 3 && next) {
        const std::uint32_t occupied = forward ? next->previousForwardMask : next->previousBackwardMask;
        if (occupied & (15U >> (static_cast<std::uint32_t>(row.spanCount - span) & 31U)))
            return false;
    }
    const std::uint32_t mask = AreaMask(15, span);
    return !((mask | (mask >> 3)) & (row.previousForwardMask | row.previousBackwardMask));
}

bool AreaTargetReached(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow *const controls = ControlRows(state);
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    Vector3 target;
    float radius;
    if (entry.nextOwnerRowIndex != -1) {
        if (entry.controlRowIndex != -1) {
            const NpcAreaControlRow &current = controls[entry.controlRowIndex];
            if (!current.forwardOffset)
                return true;
            if (entry.opcodeFlags & 1U) {
                return AreaRowParameter(current, entry) > owners[entry.nextOwnerRowIndex].positionAlongSegment - 0.5f / current.length;
            }
            const NpcAreaControlRow &previous = *HeapPointer<NpcAreaControlRow>(current.backwardOffset);
            return AreaRowParameter(previous, entry) < owners[entry.nextOwnerRowIndex].positionAlongSegment + 0.5f / previous.length;
        }
        if (entry.commandFlags & 8U)
            return !mtwkSetExtraFlag(entry.motionWork, 58, 1);
        target = owners[entry.nextOwnerRowIndex].position;
        radius = 0.34999999f;
    } else if (entry.ownerRowIndex != -1) {
        AreaLinkedPosition(owners[entry.ownerRowIndex], target);
        const NpcAreaControlRow &row = controls[entry.nextControlRowIndex];
        radius = AreaLaneWidth(row);
        if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset); next && row.laneMode > next->laneMode)
            radius = AreaLaneWidth(*next);
    } else {
        const NpcAreaControlRow &row = controls[entry.nextControlRowIndex];
        target = row.position;
        radius = AreaLaneWidth(row);
    }
    return AreaDistanceSquared(NpcRouteAreaToScene(target), entry.localPosition) < radius * radius;
}

bool AreaOpposingLaneOccupied(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    if (entry.controlRowIndex == -1)
        return false;
    const NpcAreaControlRow &current = ControlRows(state)[entry.controlRowIndex];
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    const NpcAreaControlRow &row = forward ? current : *HeapPointer<NpcAreaControlRow>(current.backwardOffset);
    const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * AreaRowParameter(row, entry));
    const std::uint32_t mask = AreaMask(7, span);
    const std::uint32_t advance = forward ? AreaMask(7, span + 3) : mask >> 5;
    return ((advance | mask | (mask >> 2)) & (forward ? row.previousBackwardMask : row.previousForwardMask)) != 0;
}

bool ReserveAreaLane(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    NpcAreaControlRow *const controls = ControlRows(state);
    const bool entranceOccupied = (controls[0].previousForwardMask & 1U) != 0;
    bool available = true;
    if (entry.controlRowIndex != -1) {
        const bool forward = (entry.opcodeFlags & 1U) != 0;
        NpcAreaControlRow &current = controls[entry.controlRowIndex];
        NpcAreaControlRow &row = forward || entry.nextControlRowIndex == -1 ? current : *HeapPointer<NpcAreaControlRow>(current.backwardOffset);
        const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * AreaRowParameter(row, entry));
        std::uint32_t &occupied = forward ? row.previousForwardMask : row.previousBackwardMask;
        const std::uint32_t prior = forward ? row.forwardMask : row.backwardMask;
        const std::uint32_t mask = AreaMask(7, span);
        occupied |= mask | (mask >> 2);
        NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(row.backwardOffset);
        NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
        const std::int32_t remaining = static_cast<std::int8_t>(row.spanCount) - span;
        if (span < 2 && previous) {
            (forward ? previous->forwardMask : previous->backwardMask) |= AreaMask(7, previous->spanCount + span - 1);
        } else if (remaining < 2 && next) {
            (forward ? next->forwardMask : next->backwardMask) |= 7U >> (static_cast<std::uint32_t>(remaining) & 31U);
        }
        if (forward) {
            if (prior & AreaMask(1, span + 3))
                available = false;
            else if (remaining < 2 && next && (next->previousBackwardMask & AreaMask(1, span - row.spanCount + 3)))
                available = false;
        } else {
            if (prior & (AreaMask(1, span) >> 3))
                available = false;
            else if (span < 2 && previous && (previous->previousBackwardMask & AreaMask(1, span + previous->spanCount - 3)))
                available = false;
        }
    }
    if (!entranceOccupied)
        controls[0].previousForwardMask &= ~1U;
    return available;
}

bool SelectAreaExitDirection(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow *owner = owners + entry.ownerRowIndex;
    while (!owner->linkedControlOffset) {
        if (!(owner->flags & 8U))
            return false;
        --owner;
    }
    const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner->linkedControlOffset);
    bool forward;
    if (entry.opcodeFlags & 2U) {
        const NpcAreaOwnerRow *target = owners + entry.primarySlotIndex;
        while (!target->linkedControlOffset) {
            if (!(target->flags & 8U))
                return false;
            --target;
        }
        const NpcAreaControlRow &targetRow = *HeapPointer<NpcAreaControlRow>(target->linkedControlOffset);
        forward = row.index < targetRow.index || (row.index == targetRow.index && owner->positionAlongSegment <= target->positionAlongSegment);
    } else if (state.flags & 4U) {
        const std::int32_t current = 2 * static_cast<std::int8_t>(row.index) + 2;
        const std::int32_t middle = state.controlRowCount + 1;
        forward = current > middle || (current == middle && owner->positionAlongSegment > 0.5f);
    } else
        forward = (state.flags & 2U) && (entry.opcodeFlags & 4U) && (entry.opcodeFlags & 8U);
    if (forward)
        entry.opcodeFlags |= 1U;
    else
        entry.opcodeFlags &= ~1U;
    return true;
}

bool QueueAreaOwnerExit(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    if (const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset)) {
        entry.nextOwnerRowIndex = -1;
        entry.nextControlRowIndex = static_cast<std::int8_t>(row->index);
        entry.commandFlags |= 2U;
        Vector3 target;
        AreaLinkedPosition(owner, target);
        QueueAreaMotion(entry, &target);
        entry.routeMotionState = 9;
        return false;
    }
    if (owner.flags & 8U) {
        entry.commandFlags |= 2U;
        entry.nextOwnerRowIndex = static_cast<std::int8_t>(entry.ownerRowIndex - 1);
        QueueAreaMotion(entry, &OwnerRows(state)[entry.nextOwnerRowIndex].position);
        return false;
    }
    entry.commandFlags &= ~2U;
    QueueAreaMotion(entry, nullptr);
    entry.routeMotionState = 17;
    return true;
}

bool ProbeAreaControlRow(const NpcAreaControlRow &row, NpcAreaProbe &probe) {
    const float x = npcPlayerPosition.x + routeOrigin.x - row.position.x;
    const float z = npcPlayerPosition.z + routeOrigin.z - row.position.z;
    const float playerRadius = std::fmax(0.40000001f, npcPlayerRadius);
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
    const float distance = std::sqrt(x * x + z * z);
    const float radius = AreaLaneWidth(row);
    if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset)) {
        const std::int16_t axis = static_cast<std::int16_t>(FixedAtan2(-(next->position.x - row.position.x), -(next->position.z - row.position.z)));
        const std::uint16_t difference = static_cast<std::uint16_t>(yaw - axis);
        if (static_cast<std::uint16_t>(difference + 0x3fffU) <= 0x7ffeU) {
            const float along = AreaCosine(difference) * distance;
            const float lateral = AreaSine(difference) * distance;
            if ((along < row.length && std::abs(lateral) < std::fmin(AreaLaneWidth(*next), radius) + playerRadius) || distance < radius) {
                probe.lateralDistance = lateral;
                probe.rowParameter = std::clamp(along / row.length, 0.0f, 1.0f);
                probe.rowIndex = static_cast<std::int8_t>(row.index);
                probe.linkedRowIndex = static_cast<std::int8_t>(next->index);
                return true;
            }
        }
    }
    if (radius + playerRadius <= distance)
        return false;
    probe.lateralDistance = distance;
    probe.rowParameter = 0.0f;
    probe.rowIndex = static_cast<std::int8_t>(row.index);
    return true;
}

bool ProbeAreaOwnerRow(const NpcAreaOwnerRow &owner, NpcAreaProbe &probe) {
    const float x = npcPlayerPosition.x + routeOrigin.x - owner.position.x;
    const float z = npcPlayerPosition.z + routeOrigin.z - owner.position.z;
    const float radius = std::fmax(0.40000001f, npcPlayerRadius) * 1.5f + 0.34999999f;
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
    const float distance = std::sqrt(x * x + z * z);
    if (const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset)) {
        Vector3 target;
        AreaLinkedPosition(owner, target);
        const float dx = target.x - owner.position.x;
        const float dz = target.z - owner.position.z;
        const std::int16_t axis = static_cast<std::int16_t>(FixedAtan2(-dx, -dz));
        const float length = std::sqrt(dx * dx + dz * dz);
        const std::uint16_t difference = static_cast<std::uint16_t>(yaw - axis);
        if (static_cast<std::uint16_t>(difference + 0x3fffU) <= 0x7ffeU) {
            const float along = AreaCosine(difference) * distance;
            const float lateral = AreaSine(difference) * distance;
            if (along < length && std::abs(lateral) < radius) {
                probe.ownerParameter = std::clamp(along / length, 0.0f, 1.0f);
                probe.ownerIndex = static_cast<std::int8_t>(owner.index);
                probe.ownerRowIndex = static_cast<std::int8_t>(row->index);
                return true;
            }
        }
    }
    if (owner.flags & 4U) {
        const NpcAreaOwnerRow &next = *(&owner + 1);
        const float dx = next.position.x - owner.position.x;
        const float dz = next.position.z - owner.position.z;
        const std::uint16_t difference = static_cast<std::uint16_t>(yaw - static_cast<std::int16_t>(FixedAtan2(-dx, -dz)));
        const float length = std::sqrt(dx * dx + dz * dz);
        if (static_cast<std::uint16_t>(difference + 0x3fffU) <= 0x7ffeU) {
            const float along = AreaCosine(difference) * distance;
            const float lateral = AreaSine(difference) * distance;
            if (along < length && std::abs(lateral) < radius) {
                probe.ownerParameter = std::clamp(along / length, 0.0f, 1.0f);
                probe.ownerIndex = static_cast<std::int8_t>(owner.index);
                probe.nextOwnerIndex = static_cast<std::int8_t>(next.index);
                return true;
            }
        }
    }
    if (radius <= distance)
        return false;
    probe.ownerParameter = 0.0f;
    probe.ownerIndex = static_cast<std::int8_t>(owner.index);
    return true;
}

void ProbeAreaState(NpcAreaRouteState &state) {
    for (NpcAreaProbe &probe : areaProbes) {
        probe.rowIndex = probe.linkedRowIndex = probe.ownerIndex = probe.nextOwnerIndex = probe.ownerRowIndex = -1;
        probe.state = nullptr;
    }
    if (!(state.flags & 8U))
        return;
    const NpcAreaControlRow *const controls = ControlRows(state);
    std::int32_t index = 0;
    for (NpcAreaProbe &probe : areaProbes) {
        for (; index < state.controlRowCount; ++index) {
            if (ProbeAreaControlRow(controls[index], probe)) {
                probe.state = &state;
                break;
            }
        }
        ++index;
    }
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    index = 0;
    for (NpcAreaProbe &probe : areaProbes) {
        for (; index < state.ownerRowCount; ++index) {
            if (ProbeAreaOwnerRow(owners[index], probe)) {
                probe.state = &state;
                break;
            }
        }
        ++index;
    }
}

void ReserveAreaProbe(const NpcAreaProbe &probe) {
    if (!probe.state || probe.rowIndex == -1)
        return;
    NpcAreaControlRow &row = ControlRows(*probe.state)[probe.rowIndex];
    NpcAreaControlRow *const previous = HeapPointer<NpcAreaControlRow>(row.backwardOffset);
    if (probe.linkedRowIndex == -1) {
        if (std::abs(probe.lateralDistance) < 0.69999999f) {
            row.previousForwardMask |= 7U;
            row.previousBackwardMask |= 7U;
            if (previous) {
                const std::uint32_t mask = AreaMask(7, previous->spanCount - 1);
                previous->previousForwardMask |= mask;
                previous->previousBackwardMask |= mask;
            }
        }
        return;
    }
    NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
    const std::int32_t span = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(row.spanCount)) * probe.rowParameter);
    const std::int32_t remaining = static_cast<std::int8_t>(row.spanCount) - span;
    const std::uint32_t mask = AreaMask(7, span);
    if (probe.lateralDistance < 0.40000001f) {
        row.previousForwardMask |= mask | (mask >> 2);
        if (span < 2 && previous)
            previous->previousForwardMask |= AreaMask(7, span + previous->spanCount - 1);
        else if (remaining < 2 && next)
            next->previousForwardMask |= 7U >> (static_cast<std::uint32_t>(remaining) & 31U);
    }
    if (probe.lateralDistance > -0.40000001f) {
        row.previousBackwardMask |= mask | (mask >> 2);
        if (span < 2 && previous)
            previous->previousBackwardMask |= AreaMask(7, span + previous->spanCount - 1);
        else if (remaining < 2 && next)
            next->previousBackwardMask |= 7U >> (static_cast<std::uint32_t>(remaining) & 31U);
    }
}

bool IsAreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaProbe &probe) {
    if (probe.state != &state)
        return false;
    const NpcAreaControlRow *const controls = ControlRows(state);
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t probeRow = probe.rowIndex;
    std::int32_t probeOwner = -1;
    float probeParameter = probe.rowParameter;
    if (probeRow == -1) {
        probeOwner = probe.ownerIndex;
        if (probeOwner == -1)
            return false;
        if (probe.ownerRowIndex != -1)
            probeRow = probe.ownerRowIndex;
        else {
            while (!owners[probeOwner].linkedControlOffset && (owners[probeOwner].flags & 8U))
                --probeOwner;
            const NpcAreaControlRow *const linked = HeapPointer<NpcAreaControlRow>(owners[probeOwner].linkedControlOffset);
            if (!linked)
                return false;
            probeRow = static_cast<std::int8_t>(linked->index);
        }
        probeParameter = owners[probeOwner].positionAlongSegment;
    }
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    std::int32_t currentRow;
    float parameter;
    if (entry.controlRowIndex == -1) {
        currentRow = entry.nextControlRowIndex;
        if (currentRow == -1)
            return false;
        parameter = currentRow ? 1.0f : 0.0f;
    } else {
        const NpcAreaControlRow &row = controls[entry.controlRowIndex];
        currentRow = forward || entry.nextControlRowIndex == -1 ? static_cast<std::int8_t>(row.index) :
                                                                  static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(row.backwardOffset)->index);
        parameter = AreaRowParameter(controls[currentRow], entry);
    }
    bool crossed = false;
    std::int32_t lane;
    for (;;) {
        const NpcAreaControlRow &row = controls[probeRow];
        lane = static_cast<std::int8_t>(row.laneMode);
        if (const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset))
            lane = std::min(lane, static_cast<std::int32_t>(static_cast<std::int8_t>(next->laneMode)));
        if (currentRow == probeRow)
            break;
        if (lane > 0 || (row.flags & 1U))
            return false;
        const NpcAreaControlRow *const adjacent = HeapPointer<NpcAreaControlRow>(forward ? row.backwardOffset : row.forwardOffset);
        if (!adjacent)
            return false;
        probeRow = static_cast<std::int8_t>(adjacent->index);
        crossed = true;
    }
    const std::int32_t spans = static_cast<std::int8_t>(controls[currentRow].spanCount);
    if (crossed) {
        if (!lane)
            return true;
        if (forward) {
            const float limit = spans ? static_cast<float>(std::max(0, spans - 5)) / static_cast<float>(spans) : 1.0f;
            return parameter > limit;
        }
        return parameter < (spans ? 5.0f / static_cast<float>(spans) : 1.0f);
    }
    if (!lane)
        return forward ? parameter <= probeParameter : probeParameter <= parameter;
    if (entry.nextOwnerRowIndex == -1)
        return false;
    if (probeOwner == -1) {
        const float margin = spans ? 3.0f / static_cast<float>(spans) : 1.0f;
        if (margin - std::abs(probeParameter - owners[entry.nextOwnerRowIndex].positionAlongSegment) < 0.0f)
            return false;
    } else if (entry.primarySlotIndex != probeOwner)
        return false;
    const float limit = spans ? 5.0f / static_cast<float>(spans) : 1.0f;
    if (forward)
        return parameter <= probeParameter && probeParameter - parameter <= limit;
    return probeParameter <= parameter && parameter - probeParameter <= limit;
}

bool AreaProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    return IsAreaProbeAhead(state, entry, areaProbes[0]) || IsAreaProbeAhead(state, entry, areaProbes[1]);
}

bool IsAreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry, const NpcAreaProbe &probe) {
    if (probe.state != &state || probe.ownerIndex == -1)
        return false;
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const std::int32_t owner = entry.ownerRowIndex == -1 ? entry.nextOwnerRowIndex : entry.ownerRowIndex;
    if (owner == -1)
        return false;
    const bool exiting = entry.ownerRowIndex != -1 && (entry.nextOwnerRowIndex == -1 || entry.ownerRowIndex >= entry.nextOwnerRowIndex);
    std::int32_t cursor = probe.ownerIndex;
    if (owner != cursor) {
        const std::uint8_t flag = exiting ? 4U : 8U;
        while (owners[cursor].flags & flag) {
            cursor += exiting ? 1 : -1;
            if (owner == cursor)
                return true;
        }
        return false;
    }
    const bool linked = owners[cursor].linkedControlOffset != 0;
    const bool probeLinked = probe.ownerRowIndex != -1;
    Vector3 target;
    if (exiting) {
        if (entry.nextOwnerRowIndex != -1) {
            if (linked && probeLinked)
                return true;
            return AreaSegmentParameter(owners[owner - 1].position, owners[owner].position, entry.worldPosition) > probe.ownerParameter;
        }
        if (entry.nextControlRowIndex == -1)
            return true;
        if (linked && !probeLinked)
            return false;
        AreaLinkedPosition(owners[owner], target);
        return probe.ownerParameter > AreaSegmentParameter(owners[owner].position, target, entry.worldPosition);
    }
    if (entry.ownerRowIndex == -1) {
        if (linked && !probeLinked)
            return true;
        AreaLinkedPosition(owners[entry.nextOwnerRowIndex], target);
        return AreaSegmentParameter(owners[entry.nextOwnerRowIndex].position, target, entry.worldPosition) > probe.ownerParameter;
    }
    if (linked && probeLinked)
        return false;
    return probe.ownerParameter > AreaSegmentParameter(owners[entry.ownerRowIndex].position, owners[entry.nextOwnerRowIndex].position, entry.worldPosition);
}

bool AreaOwnerProbeAhead(const NpcAreaRouteState &state, const NpcRouteLiveEntry &entry) {
    // AreaOwnerProbeAhead invokes the first probe twice, unlike the control-row helper.
    return IsAreaOwnerProbeAhead(state, entry, areaProbes[0]);
}

bool SelectFreeAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    std::int32_t reserved = areaProbes[0].state == &state && areaProbes[0].ownerIndex != -1 && state.freeRowCount > 1 ? 1 : 0;
    if (areaProbes[1].state == &state && areaProbes[1].ownerIndex != -1 && static_cast<std::int8_t>(state.freeRowCount) > 2)
        reserved = 2;
    float random = IncrementSharedPrngFloat();
    if (random == 1.0f)
        random = 0.0f;
    const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(state.freeRowCount) - reserved) * random);
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t available = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &owner = owners[index];
        if ((owner.runtimeMask & 1U) || !owner.linkedControlOffset || (owner.flags & 3U) == 1 || (reserved && areaProbes[0].ownerIndex == owner.index) ||
            (reserved > 1 && areaProbes[1].ownerIndex == owner.index))
            continue;
        if (available++ != choice)
            continue;
        owner.runtimeMask |= 1U;
        ++state.activeRowCount;
        --state.freeRowCount;
        entry.primarySlotIndex = static_cast<std::int8_t>(owner.index);
        entry.opcodeFlags |= 2U;
        bool forward = true;
        if (state.flags & 4U) {
            const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
            const std::int32_t current = 2 * static_cast<std::int8_t>(row.index) + 2;
            const std::int32_t middle = state.controlRowCount + 1;
            forward = current < middle || (current == middle && owner.positionAlongSegment <= 0.5f);
        } else if ((state.flags & 2U) && (entry.opcodeFlags & 8U))
            forward = false;
        if (forward)
            entry.opcodeFlags |= 1U;
        else
            entry.opcodeFlags &= ~1U;
        return true;
    }
    return false;
}

void AdvanceAreaMasks(NpcAreaRouteState &state) {
    NpcAreaControlRow *const rows = ControlRows(state);
    for (std::int32_t index = 0; index < state.controlRowCount; ++index) {
        NpcAreaControlRow &row = rows[index];
        row.previousForwardMask = row.forwardMask;
        row.previousBackwardMask = row.backwardMask;
        row.forwardMask = row.backwardMask = 0;
    }
}

void UnlinkAreaStoreSlot(NpcAreaStoreSlot &slot) {
    if (slot.next)
        slot.next->previous = slot.previous;
    if (slot.previous)
        slot.previous->next = slot.next;
    if (nearestAreaSlot == &slot)
        nearestAreaSlot = slot.next;
}

void UpdateAreaDistance(NpcAreaStoreSlot &slot) {
    Vector3 position = npcPlayerPosition;
    position.x += routeOrigin.x;
    position.z += routeOrigin.z;
    const float distance = AreaDistanceSquared(position, ControlRows(*slot.state)[0].position);
    NpcAreaStoreSlot *const previous = slot.previous;
    NpcAreaStoreSlot *const next = slot.next;
    if (next && (!previous || distance > slot.distanceSquared)) {
        NpcAreaStoreSlot *insertion = next;
        for (NpcAreaStoreSlot *cursor = next->next; cursor; cursor = cursor->next) {
            if (insertion->distanceSquared >= distance || !(insertion->state->flags & 8U))
                break;
            insertion = cursor;
        }
        if (insertion != next) {
            UnlinkAreaStoreSlot(slot);
            slot.previous = insertion->previous;
            slot.next = insertion;
            insertion->previous = &slot;
            if (slot.previous)
                slot.previous->next = &slot;
        }
    } else if (previous) {
        NpcAreaStoreSlot *insertion = previous;
        for (NpcAreaStoreSlot *cursor = previous->previous; cursor; cursor = cursor->previous) {
            if (distance > insertion->distanceSquared && (insertion->state->flags & 8U))
                break;
            insertion = cursor;
        }
        if (insertion != previous) {
            UnlinkAreaStoreSlot(slot);
            slot.next = insertion->next;
            slot.previous = insertion;
            insertion->next = &slot;
            if (slot.next)
                slot.next->previous = &slot;
        }
    }
    slot.distanceSquared = distance;
}

bool AreaUsesSpecialAttachment(const NpcRouteLiveEntry &entry) {
    return (entry.stateContext.stateTable[areaPoints[entry.currentAreaPoint].routeIndex].flags & 0x400U) != 0;
}

void ApplyAreaTransform(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    SetCharacterWorldRotation(entry.node, rotation);
}

void PublishAreaWorldPosition(NpcRouteLiveEntry &entry) {
    entry.worldPosition = entry.localPosition;
    entry.worldPosition.x += routeOrigin.x;
    entry.worldPosition.z += routeOrigin.z;
}

bool IsAreaWaitCommand(const NpcRouteLiveEntry &entry) {
    return entry.delayDuration != 0 || ((ScriptDword(entry.currentCommand) + 1U) & ~2U) == 0;
}

bool IsAreaEndpoint(const NpcRouteLiveEntry &entry) {
    return static_cast<std::int16_t>(entry.currentAreaPoint) == static_cast<std::int32_t>(ScriptDword(entry.recordBase + 0x10));
}

bool PrepareAreaDoor(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcRouteStateContext &context = entry.stateContext;
    context.otherName = areaPoints[state.areaPointIndex].owner;
    if (!(state.flags & 1U) || !context.otherName.name ||
        !ComputeDoorIngress(static_cast<std::int32_t>(context.otherName.name), &entry.localPosition, &context.scenePosition)) {
        context.otherName = {};
        return false;
    }
    context.targetYaw = static_cast<std::uint32_t>(g_doorIngressRotation.y);
    context.savedSplinePosition = GetDoorIngressOrigin();
    ComputeDoorEgress(static_cast<std::int32_t>(context.otherName.name), &entry.localPosition, &context.doorPosition);
    context.doorPosition.x += routeOrigin.x;
    context.doorPosition.z += routeOrigin.z;
    context.scenePosition.x += routeOrigin.x;
    context.scenePosition.z += routeOrigin.z;
    context.savedSplinePosition.x += routeOrigin.x;
    context.savedSplinePosition.z += routeOrigin.z;
    context.doorWaitFrames = 0;
    return true;
}

bool AreaDoorPathClear(NpcRouteLiveEntry &entry) {
    NpcRouteStateContext &context = entry.stateContext;
    if (!context.otherName.name)
        return true;
    if (!entry.node && context.doorWaitFrames != -1) {
        ++context.doorWaitFrames;
        if (context.doorWaitFrames == 1)
            entry.flags |= 0x1000000U;
        else if (context.doorWaitFrames == 150) {
            entry.flags &= ~0x1000000U;
            context.doorWaitFrames = -1;
        }
        return false;
    }
    if (QueryNpcMoveObstruction(entry, context.scenePosition, false, 0x3000, 0.34999999f, -1.0f) & 1U)
        return false;
    return QueryNpcMoveObstruction(entry, context.scenePosition, true, 0x4000, 0.44999999f, 1.5f) == 0;
}

bool ClaimAreaEntrance(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (ControlRows(state)[0].previousForwardMask & 1U)
        return false;
    if (!(state.flags & 1U) || !entry.stateContext.otherName.name)
        return true;
    return AreaDoorPathClear(entry) && ClaimDoor(entry.stateContext.otherName, entry.name);
}

std::int32_t ReserveEndpointOwner(NpcAreaRouteState &state, NpcAreaOwnerRow &owner, NpcRouteLiveEntry &entry) {
    owner.runtimeMask |= 1U;
    entry.opcodeFlags |= 2U;
    entry.primarySlotIndex = static_cast<std::int8_t>(owner.index);
    const NpcAreaControlRow *const row = HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
    if (!row)
        return 2;
    if (!(owner.flags & 1U)) {
        ++state.activeRowCount;
        --state.freeRowCount;
    }
    bool forward = true;
    if (state.flags & 4U) {
        const std::int32_t current = 2 * static_cast<std::int8_t>(row->index) + 2;
        const std::int32_t middle = state.controlRowCount + 1;
        forward = current < middle || (current == middle && owner.positionAlongSegment <= 0.5f);
    } else if ((state.flags & 2U) && (entry.opcodeFlags & 8U))
        forward = false;
    if (forward)
        entry.opcodeFlags |= 1U;
    else
        entry.opcodeFlags &= ~1U;
    return 1;
}

std::int32_t SelectEndpointOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t ordinary = 0;
    std::int32_t alternate = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        const NpcAreaOwnerRow &owner = owners[index];
        if ((owner.flags & 3U) != 3 && owner.actionKind != 4 && !(owner.runtimeMask & 1U)) {
            if (owner.flags & 0x10U)
                ++alternate;
            else
                ++ordinary;
        }
        while (owners[index].flags & 4U)
            ++index;
    }
    const bool split = ordinary && alternate;
    const bool alternateOnly = (ScriptByte(entry.recordBase + 0x0b) & 2U) != 0;
    const std::int32_t count = split ? alternateOnly ? alternate : ordinary : ordinary + alternate;
    float random = IncrementSharedPrngFloat();
    if (random == 1.0f)
        random = 0.0f;
    const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(count) * random);
    std::int32_t available = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &owner = owners[index];
        if ((owner.flags & 3U) != 3 && owner.actionKind != 4 && !(owner.runtimeMask & 1U) && (!split || ((owner.flags & 0x10U) != 0) == alternateOnly)) {
            if (available++ == choice)
                return ReserveEndpointOwner(state, owner, entry);
        }
        while (owners[index].flags & 4U)
            ++index;
    }
    return 0;
}

bool SelectClipAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        const NpcAreaOwnerRow &owner = owners[index];
        if (static_cast<std::uint8_t>(owner.actionKind - 13U) <= 2U && !(owner.runtimeMask & 1U))
            ++count;
    }
    if (!count)
        return false;
    float random = IncrementSharedPrngFloat();
    if (random == 1.0f)
        random = 0.0f;
    const std::int32_t choice = static_cast<std::int32_t>(static_cast<float>(count) * random);
    std::int32_t available = 0;
    for (std::int32_t index = 0; index < state.ownerRowCount; ++index) {
        NpcAreaOwnerRow &owner = owners[index];
        if (static_cast<std::uint8_t>(owner.actionKind - 13U) > 2U || (owner.runtimeMask & 1U))
            continue;
        if (available++ != choice)
            continue;
        owner.runtimeMask |= 1U;
        entry.opcodeFlags |= 2U;
        entry.primarySlotIndex = static_cast<std::int8_t>(owner.index);
        return true;
    }
    return false;
}

bool OccupyAreaOwner(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (entry.primarySlotIndex == -1)
        return false;
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    std::int8_t index = entry.primarySlotIndex;
    if (owners[index].linkedControlOffset && (owners[index].flags & 4U)) {
        do
            ++index;
        while (owners[index].flags & 4U);
    }
    entry.ownerRowIndex = index;
    entry.controlRowIndex = entry.nextControlRowIndex = entry.nextOwnerRowIndex = -1;
    entry.commandFlags &= ~2U;
    ++state.occupiedRowCount;
    entry.routeMotionState = 16;
    entry.field2d8 &= ~0xffff0000ULL;
    entry.field2e8 = 0;
    entry.stateContext.otherName = {};
    SetAreaYaw(entry.euler, owners[index].yaw);
    entry.localPosition = NpcRouteAreaToScene(owners[index].position);
    if (entry.node) {
        const float height = entry.localPosition.y;
        if (FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags))
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        else
            entry.localPosition.y = height;
        Vector3i rotation;
        std::memcpy(&rotation, &entry.euler, sizeof(rotation));
        SetCharacterWorldRotation(entry.node, rotation);
    }
    PublishAreaWorldPosition(entry);
    return true;
}

void FinishAreaRoute(NpcRouteLiveEntry &entry) {
    if (entry.node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags |= CharacterControlEvaluateMotion;
        if (!CanRevealNpcStream(entry))
            RequestNpcStreamOut(entry, 0);
    }
    ResetNpcAreaRouteState(entry);
}

std::uint16_t SelectAreaApproachMotion(std::int32_t rig, std::uint8_t action, std::int32_t direction, bool exiting) {
    if (action != 2 && action != 6 && action != 7 && static_cast<std::uint8_t>(action - 16) > 2U)
        return 0;
    std::int16_t motion;
    if (exiting) {
        motion = direction == 0 ? -8174 : direction == 1 ? -8140 : action >= 16 ? -8055 : -8106;
    } else {
        motion = direction == 0 ? -8191 : direction == 1 ? -8157 : action >= 16 ? -8089 : -8123;
    }
    return RemapNpcMotionForRig(rig, static_cast<std::uint16_t>(motion));
}

Vector3 SampleAreaMotion(std::uint16_t motion, std::int32_t rig, float frame) {
    Vector3 translation{};
    SampleNativeMotionRoot(motion, rig, frame, translation);
    return translation;
}

Vector3 TransformAreaMotion(const Vector3 &origin, std::int16_t yaw, Vector3 motion) {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    const Matrix4x4 &matrix = workTransformMatrix;
    const Vector3 transformed{motion.x * matrix.rows[0].x + motion.y * matrix.rows[1].x + motion.z * matrix.rows[2].x + matrix.rows[3].x,
        motion.x * matrix.rows[0].y + motion.y * matrix.rows[1].y + motion.z * matrix.rows[2].y + matrix.rows[3].y,
        motion.x * matrix.rows[0].z + motion.y * matrix.rows[1].z + motion.z * matrix.rows[2].z + matrix.rows[3].z};
    Matrix4x4::Pop(1);
    return transformed;
}

bool IsAreaSeatedAction(std::uint8_t action) {
    return action <= 18U && ((458948U >> action) & 1U);
}

void QueueAreaOwnerApproach(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.nextOwnerRowIndex];
    std::uint16_t yaw = static_cast<std::uint16_t>(owner.yaw);
    const std::int16_t difference =
        static_cast<std::int16_t>(FixedAtan2(-(owner.position.x - entry.worldPosition.x), -(owner.position.z - entry.worldPosition.z)) - yaw);
    const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(difference));
    const std::int32_t rig = GetCharacterBoneId(entry.node);
    std::uint16_t motion;
    if (magnitude < 8192)
        motion = SelectAreaApproachMotion(rig, owner.actionKind, 2, false);
    else if (magnitude >= 24576) {
        motion = SelectAreaApproachMotion(rig, owner.actionKind, 0, false);
        yaw += 0x8000U;
    } else {
        motion = SelectAreaApproachMotion(rig, owner.actionKind, 1, false);
        yaw += difference < 0 ? -0x4000 : 0x4000;
    }
    Vector3 translation = SampleAreaMotion(motion, rig, -1.0f);
    translation.x = difference < 0 ? translation.x : -translation.x;
    translation.z = -translation.z;
    Vector3 origin = owner.position;
    origin.y = 0.0f;
    const Vector3 target = TransformAreaMotion(origin, static_cast<std::int16_t>(yaw), translation);
    entry.stateContext.targetPosition = target;
    entry.stateContext.targetPosition.y = entry.worldPosition.y;
    entry.commandFlags |= 0x1000U;
    QueueAreaMotion(entry, &target, nullptr, yaw, motion);
    if (difference < 0)
        SetMotionRegionFlag(entry.motionWork, 69, 0, MotionRegion::LowerBody);
    entry.flags |= 0x100U;
    entry.commandFlags |= 8U;
}

void QueueAreaOwnerTarget(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow &owner = owners[entry.nextOwnerRowIndex];
    if (owner.flags & 4U)
        QueueAreaMotion(entry, &owner.position, &owners[entry.nextOwnerRowIndex + 1].position);
    else if (IsAreaSeatedAction(owner.actionKind))
        QueueAreaOwnerApproach(state, entry);
    else {
        const std::uint16_t motion = ResolveNpcAreaAnimation(state, entry, true);
        QueueAreaMotion(entry, &owner.position, nullptr, static_cast<std::uint16_t>(owner.yaw), motion);
    }
}

void EnterAreaOwnerLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    entry.controlRowIndex = entry.nextControlRowIndex = -1;
    QueueAreaOwnerTarget(state, entry);
    entry.routeMotionState = 10;
}

void QueueAreaDoorMotion(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    NpcRouteStateContext &context = entry.stateContext;
    Vector3 position = NpcRouteAreaToScene(context.scenePosition);
    const std::uint16_t motion = static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(context.otherName.name), entry.node, &position));
    context.doorMotion = motion;
    if (context.otherName.name != static_cast<std::uint32_t>(GetSelectedNpcDoorId())) {
        CopyMotionTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
        return;
    }
    if (motion) {
        Vector3 root = SampleAreaMotion(motion, GetCharacterBoneId(entry.node), 15.0f);
        root = TransformAreaMotion({}, static_cast<std::int16_t>(context.targetYaw), root);
        position.x += root.x;
        position.z += root.z;
    }
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
        control->flags = (control->flags & ~CharacterControlFineRaycast) | CharacterControlStairMode;
    entry.commandFlags |= 1U;
    CopyMotionTransition(entry.motionWork, &position, static_cast<std::uint16_t>(context.targetYaw), 6, 0);
    CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
    if (context.otherName.name && motion) {
        SetMotionTimeRelated(entry.motionWork, 1, 5, motion);
        SetMotionRegionEvalTime(entry.motionWork, 14.0f, MotionRegion::LowerBody);
    }
    const Vector3 exit = NpcRouteAreaToScene(context.doorPosition);
    UpdateMotionWorkWithPosition(entry.motionWork, &exit, 0);
}

void PositionAreaQueueMember(NpcRouteLiveEntry &entry, const Vector3 &facing, const Vector3 &origin, float spacing) {
    const float dx = facing.x - origin.x;
    const float dz = facing.z - origin.z;
    const std::uint16_t yaw = std::sqrt(dx * dx + dz * dz) <= 0.00000011920929f ? 0 : FixedAtan2(-dx, -dz);
    Vector3 target{origin.x + AreaSine(yaw) * spacing, origin.y, origin.z + AreaCosine(yaw) * spacing};
    const float x = target.x - entry.worldPosition.x;
    const float z = target.z - entry.worldPosition.z;
    const float distance = std::sqrt(x * x + z * z);
    if (entry.node) {
        if (distance < 0.69999999f) {
            Vector3 look = NpcRouteAreaToScene(facing);
            look.y = entry.worldPosition.y + 1.0f;
            UpdateMotionWorkWithPosition(entry.motionWork, &look, 0);
        }
        if (distance > 0.1f) {
            target = NpcRouteAreaToScene(target);
            CopyMotionTransition(entry.motionWork, &target, yaw, 0, 0);
            CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
            SetMotionTimeRelated(entry.motionWork, 1, 5, RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe0de));
        }
    } else {
        if (distance > 0.029999999f) {
            target.x = entry.worldPosition.x + 0.029999999f / distance * x;
            target.z = entry.worldPosition.z + 0.029999999f / distance * z;
        }
        entry.worldPosition.x = target.x;
        entry.worldPosition.z = target.z;
        entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
    }
}

void PositionAreaQueueHead(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const Vector3 &origin = areaPoints[entry.currentAreaPoint].position;
    Vector3 facing = origin;
    facing.x += AreaSine(static_cast<std::uint16_t>(state.field0c));
    facing.z += AreaCosine(static_cast<std::uint16_t>(state.field0c));
    PositionAreaQueueMember(entry, facing, origin, 1.5f);
}

void DisplaceAreaLane(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing, bool moving) {
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const bool forward = (entry.opcodeFlags & 1U) != 0;
    const NpcAreaControlRow &adjacent = *HeapPointer<NpcAreaControlRow>(forward ? row.forwardOffset : row.backwardOffset);
    const Vector3 start = NpcRouteAreaToScene(row.position);
    const Vector3 end = NpcRouteAreaToScene(adjacent.position);
    const float parameter = forward ? AreaRowParameter(row, entry) : 1.0f - AreaRowParameter(adjacent, entry);
    const std::int8_t mode = std::min(static_cast<std::int8_t>(row.laneMode), static_cast<std::int8_t>(adjacent.laneMode));
    const float width = mode == 0 ? 0.315f : mode == 1 || mode == 2 ? 0.44999999f : 0.0f;
    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    const std::uint16_t yaw = FixedAtan2(-dx, -dz);
    if (advancing) {
        const float x = entry.localPosition.x - start.x;
        const float z = entry.localPosition.z - start.z;
        const std::int16_t bearing = static_cast<std::int16_t>(FixedAtan2(-x, -z));
        if (AreaSine(bearing - yaw) * std::sqrt(x * x + z * z) > -width * 0.80000001f)
            advancing = moving = false;
    }
    Vector3 target = AreaLerp(start, end, parameter);
    target.x -= AreaSine(yaw - 0x4000) * width;
    target.z -= AreaCosine(yaw - 0x4000) * width;
    if (!entry.node) {
        if (advancing) {
            const float factor = 0.025f / std::sqrt(dx * dx + dz * dz);
            entry.localPosition.x += factor * dx;
            entry.localPosition.z += factor * dz;
        } else {
            entry.localPosition.x = target.x;
            entry.localPosition.z = target.z;
        }
        SetAreaYaw(entry.euler, yaw);
        return;
    }
    if (advancing) {
        if (!moving) {
            CopyMotionTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(yaw - 16), 13, 0);
            return;
        }
        if (mtwkSetExtraFlag(entry.motionWork, 58, 1))
            return;
        target = end;
        target.x -= AreaSine(yaw - 0x4000) * width;
        target.z -= AreaCosine(yaw - 0x4000) * width;
        CopyMotionTransition(entry.motionWork, &target, 0, 0, 1);
        if (!mode) {
            const std::int32_t rig = GetCharacterBoneId(entry.node);
            const std::uint16_t motion = RemapNpcMotionForRig(rig, 0xe221);
            SetMotionTimeRelated(entry.motionWork, 1, 5, motion);
            SetMotionTimeRelated(entry.motionWork, 1, 0, motion);
            SetMotionTimeRelated(entry.motionWork, 1, 1, RemapNpcMotionForRig(rig, 0xe232));
        } else
            SetMotionTimeRelated(entry.motionWork, 0, 6, 0);
        return;
    }
    if (moving) {
        target.x -= AreaSine(yaw) * 0.5f;
        target.z -= AreaCosine(yaw) * 0.5f;
    }
    const float x = target.x - entry.localPosition.x;
    const float z = target.z - entry.localPosition.z;
    float distance = std::sqrt(x * x + z * z);
    if (distance > 0.40000001f) {
        distance = 0.40000001f / distance;
        target.x = entry.localPosition.x + distance * x;
        target.z = entry.localPosition.z + distance * z;
    }
    if (distance > 0.039999999f) {
        CopyMotionTransition(entry.motionWork, &target, static_cast<std::uint16_t>(yaw - 16), 6, 0);
        CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
        if (!mode)
            SetMotionTimeRelated(entry.motionWork, 1, 5, RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe221));
        else
            SetMotionTimeRelated(entry.motionWork, 0, 6, 0);
    }
}

void LimitAreaCollisionRadius(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const NpcAreaControlRow &adjacent = *HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset);
    if (std::min(row.laneMode, adjacent.laneMode) != 0)
        return;
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
        control->flags |= 0x2000000U;
        control->collisionCenterRadius = 0.2f;
    }
}

std::int32_t AreaQueueCapacity(const NpcAreaRouteState &state) {
    static constexpr std::int32_t capacities[4]{3, 3, 0, 0};
    const std::int32_t capacity = capacities[g_actNumber];
    return (state.flags & 1U) ? capacity : (state.flags & 0x20U) ? 0 : capacity != 0;
}

bool AdmitAreaRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool initializing) {
    entry.controlRowIndex = -1;
    entry.nextControlRowIndex = 0;
    entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
    entry.specialAnimationContext = nullptr;
    PrepareAreaDoor(state, entry);
    if (entry.primarySlotIndex == -1) {
        if (ScriptDword(entry.currentCommand) == 19 && SelectClipAreaOwner(state, entry)) {
            OccupyAreaOwner(state, entry);
            entry.routeMotionState = 18;
            entry.motionSpeed = 1.0f;
            entry.clipOverrideIndex = static_cast<std::int8_t>(ScriptByte(entry.currentCommand + 4));
            entry.currentCommand += 8;
            entry.streamCost = 1;
            entry.routeDirectionMode = 3;
            entry.flags |= 0x100U;
            entry.animationFrame = 2.0f * static_cast<std::int8_t>(ScriptByte(entry.recordBase + 5));
            return true;
        }
        NpcRouteLiveEntry *tail = ListHead(state, 0);
        std::int32_t count = 0;
        if (tail) {
            count = 1;
            while (tail->areaPointNext) {
                tail = tail->areaPointNext;
                ++count;
            }
        }
        if (IsAreaEndpoint(entry)) {
            const std::int32_t reserved = SelectEndpointOwner(state, entry);
            if (reserved == 2) {
                OccupyAreaOwner(state, entry);
                return true;
            }
            if (!reserved) {
                FinishAreaRoute(entry);
                return false;
            }
        } else if (tail) {
            if (!IsAreaWaitCommand(entry) && count > AreaQueueCapacity(state) - 1) {
                FinishAreaRoute(entry);
                return false;
            }
        } else if (state.freeRowCount && static_cast<std::int8_t>(state.participantCount) - static_cast<std::int8_t>(state.occupiedRowCount) - count < 2) {
            SelectFreeAreaOwner(state, entry);
        }
        if (tail) {
            tail->areaPointNext = &entry;
            entry.areaPointPrevious = tail;
            entry.routeMotionState = 4;
            return true;
        }
        if (entry.primarySlotIndex == -1) {
            if (!IsAreaWaitCommand(entry) && !IsAreaEndpoint(entry) && !state.freeRowCount) {
                FinishAreaRoute(entry);
                return false;
            }
            if (initializing) {
                SetListHead(state, &entry, 0);
                entry.nextControlRowIndex = 0;
                entry.routeMotionState = 4;
                return true;
            }
        }
    }
    if (initializing)
        return !OccupyAreaOwner(state, entry);
    if ((entry.commandFlags & 0x100U) && OccupyAreaOwner(state, entry))
        return false;
    if (ClaimAreaEntrance(state, entry) && entry.primarySlotIndex != -1 && !AreaProbeAhead(state, entry)) {
        if (entry.stateContext.otherName.name) {
            entry.routeMotionState = 0;
            entry.commandFlags &= ~2U;
        } else {
            QueueAreaMotion(entry, &ControlRows(state)[0].position);
            entry.routeMotionState = 5;
        }
        entry.flags |= 0x100U;
        return true;
    }
    SetListHead(state, &entry, 0);
    entry.routeMotionState = 4;
    return true;
}

bool MaintainAreaQueue(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcRouteLiveEntry *tail = ListHead(state, 0);
    if (tail) {
        std::int32_t count = 1;
        while (tail->areaPointNext) {
            tail = tail->areaPointNext;
            ++count;
        }
        const std::int32_t capacity = AreaQueueCapacity(state);
        while (tail && count > capacity) {
            if (!IsAreaWaitCommand(entry) && !IsAreaEndpoint(*tail)) {
                FinishAreaRoute(*tail);
                if (--count <= capacity)
                    break;
            }
            tail = tail->areaPointPrevious;
        }
        NpcRouteLiveEntry *const head = ListHead(state, 0);
        if (head && head->primarySlotIndex == -1) {
            if (IsAreaEndpoint(*head))
                SelectEndpointOwner(state, *head);
            else if (state.freeRowCount &&
                static_cast<std::int8_t>(state.participantCount) - static_cast<std::int8_t>(state.occupiedRowCount) - AreaQueueLength(state) < 2)
                SelectFreeAreaOwner(state, *head);
        }
    }
    return entry.routeMotionState != 23;
}

void AdvanceAreaControlRow(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    entry.controlRowIndex = entry.nextControlRowIndex;
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset);
    entry.nextControlRowIndex = next ? static_cast<std::int8_t>(next->index) : -1;
}

void QueueAreaOwnerLane(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
    const NpcAreaControlRow &target = (entry.opcodeFlags & 1U) ? *HeapPointer<NpcAreaControlRow>(row.forwardOffset) : row;
    QueueAreaMotion(entry, &target.position);
    entry.routeMotionState = 8;
}

bool TickAreaTraversal(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding) {
    auto stop = [&] {
        entry.commandFlags &= ~2U;
        if (avoiding)
            DisplaceAreaLane(state, entry, true, false);
        else
            QueueAreaMotion(entry, nullptr);
        entry.routeMotionState = 13;
        return false;
    };
    if (entry.commandFlags & 0x40U)
        return stop();
    if (!avoiding) {
        if (AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags |= 4U;
            DisplaceAreaLane(state, entry, false, true);
            entry.routeMotionState = 11;
            return true;
        }
    } else {
        LimitAreaCollisionRadius(state, entry);
        if (!AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags &= ~4U;
            QueueAreaMotion(entry, &ControlRows(state)[entry.nextControlRowIndex].position);
            entry.routeMotionState = 7;
            return true;
        }
    }
    if ((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
        ReserveAreaLane(state, entry);
        return stop();
    }
    if (!ReserveAreaLane(state, entry) || AreaProbeAhead(state, entry))
        return stop();
    if (avoiding)
        DisplaceAreaLane(state, entry, true, true);
    if (!AreaTargetReached(state, entry))
        return false;
    if (entry.opcodeFlags & 2U) {
        AdvanceAreaControlRow(state, entry);
        entry.routeMotionState = 6;
        return true;
    }
    const NpcAreaControlRow &row = ControlRows(state)[entry.nextControlRowIndex];
    entry.controlRowIndex = entry.nextControlRowIndex;
    if (row.flags & 1U) {
        entry.nextControlRowIndex = -1;
        if (avoiding) {
            entry.commandFlags &= ~4U;
            if (entry.node)
                SetMotionTimeRelated(entry.motionWork, 0, 6, 0);
        }
        entry.routeMotionState = 21;
        PrepareAreaDoor(state, entry);
        return true;
    }
    const NpcAreaControlRow &next = *HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset);
    entry.commandFlags |= 2U;
    entry.nextControlRowIndex = static_cast<std::int8_t>(next.index);
    if (!avoiding)
        QueueAreaMotion(entry, &next.position);
    return true;
}

bool TickAreaOwnerApproach(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool avoiding) {
    auto stop = [&] {
        entry.commandFlags &= ~2U;
        if (avoiding)
            DisplaceAreaLane(state, entry, true, false);
        else
            QueueAreaMotion(entry, nullptr);
        entry.routeMotionState = 14;
        return false;
    };
    if (entry.commandFlags & 0x40U) {
        ReserveAreaLane(state, entry);
        return stop();
    }
    if (!avoiding) {
        if (AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags |= 4U;
            DisplaceAreaLane(state, entry, false, true);
            entry.routeMotionState = 12;
            return true;
        }
    } else {
        LimitAreaCollisionRadius(state, entry);
        if (!AreaOpposingLaneOccupied(state, entry)) {
            entry.commandFlags &= ~4U;
            QueueAreaOwnerLane(state, entry);
            return true;
        }
    }
    if ((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
        ReserveAreaLane(state, entry);
        return stop();
    }
    if (AreaProbeAhead(state, entry)) {
        stop();
        RequestNpcAreaRouteCompletion(entry);
        return false;
    }
    if (!ReserveAreaLane(state, entry))
        return stop();
    if (avoiding)
        DisplaceAreaLane(state, entry, true, true);
    if (AreaTargetReached(state, entry)) {
        if (!avoiding) {
            entry.commandFlags |= 2U;
            EnterAreaOwnerLane(state, entry);
            return true;
        }
        return stop();
    }
    ReserveAreaLane(state, entry);
    return false;
}

bool TickAreaWaiting(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool ownerLane) {
    const bool opposite = AreaOpposingLaneOccupied(state, entry);
    if (entry.commandFlags & 0x40U) {
        if (opposite) {
            entry.commandFlags |= 4U;
            DisplaceAreaLane(state, entry, false, false);
        } else
            entry.commandFlags &= ~4U;
        ReserveAreaLane(state, entry);
        return false;
    }
    if (opposite) {
        DisplaceAreaLane(state, entry, false, false);
        entry.commandFlags |= 4U;
        if (((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) || (ownerLane && AreaTargetReached(state, entry))) {
            ReserveAreaLane(state, entry);
            return false;
        }
    } else {
        entry.commandFlags &= ~4U;
        if ((entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
            if (ownerLane) {
                entry.nextControlRowIndex = entry.controlRowIndex;
                const NpcAreaControlRow &row = ControlRows(state)[entry.controlRowIndex];
                entry.controlRowIndex =
                    static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset)->index);
                entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
            } else
                std::swap(entry.controlRowIndex, entry.nextControlRowIndex);
            entry.opcodeFlags ^= 1U;
            entry.opcodeFlags &= ~2U;
            entry.commandFlags &= ~0x20U;
            OwnerRows(state)[entry.primarySlotIndex].runtimeMask &= ~1U;
            entry.primarySlotIndex = -1;
            --state.activeRowCount;
            ++state.freeRowCount;
            if (ownerLane) {
                entry.routeMotionState = 13;
                return true;
            }
        } else if (ownerLane && AreaTargetReached(state, entry)) {
            entry.commandFlags |= 2U;
            EnterAreaOwnerLane(state, entry);
            return false;
        }
    }
    if (ReserveAreaLane(state, entry) && !AreaProbeAhead(state, entry)) {
        entry.commandFlags |= 2U;
        if (entry.commandFlags & 4U)
            entry.routeMotionState = ownerLane ? 12 : 11;
        else if (ownerLane)
            QueueAreaOwnerLane(state, entry);
        else {
            QueueAreaMotion(entry, &ControlRows(state)[entry.nextControlRowIndex].position);
            entry.routeMotionState = 7;
        }
    }
    return false;
}

void PrepareAreaOwnerGround(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow &owner = owners[static_cast<std::uint8_t>(entry.ownerRowIndex)];
    Vector3 target;
    bool linked = false;
    if (owner.linkedControlOffset) {
        AreaLinkedPosition(owner, target);
        linked = true;
    } else if (owner.flags & 8U) {
        target = owners[static_cast<std::uint8_t>(entry.ownerRowIndex) - 1].position;
        linked = true;
    }
    if (linked) {
        const std::uint16_t yaw = static_cast<std::uint16_t>(owner.yaw);
        const std::int16_t difference = static_cast<std::int16_t>(yaw - FixedAtan2(-(target.x - owner.position.x), -(target.z - owner.position.z)));
        const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(difference));
        const std::int32_t rig = GetCharacterBoneId(entry.node);
        const std::uint16_t motion = SelectAreaApproachMotion(rig, owner.actionKind, magnitude < 8192 ? 0 : magnitude < 24576 ? 1 : 2, true);
        Vector3 position = entry.localPosition;
        if (motion) {
            Vector3 track = SampleAreaMotion(motion, rig, -1.0f);
            if (difference < 0)
                track.x = -track.x;
            Vector3 origin = owner.position;
            origin.y = 0.0f;
            position = NpcRouteAreaToScene(TransformAreaMotion(origin, static_cast<std::int16_t>(yaw), track));
        }
        if (FindNpcIntersectionInScene(position, entry.packedMoveFlags))
            entry.localPosition.y = position.y;
    } else
        FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
    Vector3 position = GetCharacterWorldPosition(entry.node);
    position.y = entry.localPosition.y;
    shenApplyTranslationToAll(&entry.node->entity.node, position);
}

void QueueAreaOwnerExitMotion(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    const NpcAreaOwnerRow *const owners = OwnerRows(state);
    const NpcAreaOwnerRow &owner = owners[static_cast<std::uint8_t>(entry.ownerRowIndex)];
    Vector3 target{};
    if (owner.linkedControlOffset)
        AreaLinkedPosition(owner, target);
    else if (owner.flags & 8U)
        target = owners[static_cast<std::uint8_t>(entry.ownerRowIndex) - 1].position;
    const std::int16_t yaw = static_cast<std::int16_t>(owner.yaw);
    const std::int16_t difference = static_cast<std::int16_t>(yaw - FixedAtan2(-(target.x - entry.worldPosition.x), -(target.z - entry.worldPosition.z)));
    const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(difference));
    const std::int32_t rig = GetCharacterBoneId(entry.node);
    const std::uint16_t motion = SelectAreaApproachMotion(rig, owner.actionKind, magnitude < 8192 ? 0 : magnitude < 24576 ? 1 : 2, true);
    if (motion) {
        mtwkRequestMotion(entry.motionWork, motion, MotionRegion::LowerBody);
        SetAreaYaw(entry.euler, yaw);
        Vector3i rotation;
        std::memcpy(&rotation, &entry.euler, sizeof(rotation));
        SetCharacterWorldRotation(entry.node, rotation);
        if (difference < 0)
            SetMotionRegionFlag(entry.motionWork, 69, 0, MotionRegion::LowerBody);
        if (!(entry.commandFlags & 0x1000U)) {
            const MotnSequence *const sequence = FindLoadedMotionSequence(motion);
            Vector3 track = SampleAreaMotion(motion, rig, sequence ? static_cast<float>(sequence->frameCount) : 0.0f);
            if (difference < 0)
                track.x = -track.x;
            track.y = 0.0f;
            entry.stateContext.targetPosition = TransformAreaMotion(entry.worldPosition, yaw, track);
            entry.commandFlags |= 0x1000U;
        }
    }
    entry.commandFlags |= 8U;
}

void ApplyAreaOwnerAnimation(const NpcAreaRouteState &state, NpcRouteLiveEntry &entry, std::uint16_t motion) {
    const NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    Vector3 position = GetCharacterWorldPosition(entry.node);
    position.x = owner.position.x - routeOrigin.x;
    position.z = owner.position.z - routeOrigin.z;
    shenApplyTranslationToAll(&entry.node->entity.node, position);
    SetCharacterWorldRotation(entry.node, {0, owner.yaw, 0});
    mtwkRequestMotion(entry.motionWork, motion, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    SetMotionFrameTime(entry.motionWork, 24, MotionRegion::LowerBody);
    mtwkSetExtraFlag(entry.motionWork, 15, 0);
    mtwkSetExtraFlag(entry.motionWork, 16, 2);
}

void RestoreAreaMotionAttachment(NpcRouteLiveEntry &entry) {
    entry.currentMotionId = 0;
    if (entry.waitParameter)
        SetNpcMotionAttachment(entry, entry.waitParameter, entry.opcodeByte | 0x20U, 1);
    else if (entry.opcodeByte == 0xffU)
        ClearNpcMotionAttachment(entry, 1);
    entry.motionAttachments[1].flags &= ~0x40U;
}

bool FinishAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    bool continueTick = true;
    if (IsAreaSeatedAction(owner.actionKind)) {
        QueueAreaOwnerExitMotion(state, entry);
        entry.flags |= 0x100U;
        entry.routeMotionState = 19;
        continueTick = false;
    } else
        entry.routeMotionState = 20;
    owner.runtimeMask &= ~2U;
    RestoreAreaMotionAttachment(entry);
    return continueTick;
}

void RestoreAreaSavedPosition(NpcRouteLiveEntry &entry) {
    entry.worldPosition = entry.stateContext.targetPosition;
    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
    entry.commandFlags &= ~0x1000U;
}

bool TickAreaOwnerAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow &owner = OwnerRows(state)[entry.ownerRowIndex];
    entry.commandFlags &= 0xfef5U;
    owner.runtimeMask |= 2U;
    const std::uint16_t counter = static_cast<std::uint16_t>(entry.field2d8 >> 16);
    const std::uint32_t opcode = ScriptDword(entry.currentCommand);
    if (counter > 900 && opcode != 1)
        entry.field2e8 = 1000;
    ++entry.field2e8;
    if (!(entry.commandFlags & 0x800U) && entry.node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags |= CharacterControlStairMode;
        entry.commandFlags |= 0x800U;
    }
    entry.motionAttachments[1].flags |= 0x40U;
    const std::int32_t pending = static_cast<std::int8_t>(state.participantCount) - static_cast<std::int8_t>(state.occupiedRowCount) - AreaQueueLength(state);
    if (pending >= 2)
        entry.field2e8 = 0;
    else if (opcode == 0xffffffffU) {
        entry.commandFlags |= 0x80U;
        entry.field2e8 = 0;
    } else if (entry.delayDuration > 0)
        entry.field2e8 = 1000;
    else {
        if (entry.field2e8 > 1000 && !(entry.commandFlags & 0x40U)) {
            if (!entry.node) {
                entry.routeMotionState = 20;
                if (entry.commandFlags & 0x1000U)
                    RestoreAreaSavedPosition(entry);
                else if (IsAreaSeatedAction(owner.actionKind)) {
                    Vector3 target{};
                    if (owner.linkedControlOffset)
                        AreaLinkedPosition(owner, target);
                    else if (owner.flags & 8U)
                        target = OwnerRows(state)[static_cast<std::uint8_t>(entry.ownerRowIndex) - 1].position;
                    entry.worldPosition.x = (target.x + entry.worldPosition.x) * 0.5f;
                    entry.worldPosition.z = (target.z + entry.worldPosition.z) * 0.5f;
                    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
                }
                owner.runtimeMask &= ~2U;
                --state.occupiedRowCount;
                RestoreAreaMotionAttachment(entry);
                return true;
            }
            if ((!entry.specialAnimationContext && SetMotionRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) || counter > 900) {
                const bool continueTick = FinishAreaOwnerAnimation(state, entry);
                --state.occupiedRowCount;
                return continueTick;
            }
        }
        if (opcode == 1) {
            entry.commandFlags |= 0x80U;
            entry.field2e8 = 0;
        }
    }
    auto restart = [&] {
        entry.specialAnimationContext = nullptr;
        ApplyAreaOwnerAnimation(state, entry, ResolveNpcAreaAnimation(state, entry, true));
    };
    if (entry.node && IsAreaEndpoint(entry) && !(entry.commandFlags & 0x40U)) {
        const float distance = AreaDistanceSquared(npcPlayerPosition, entry.localPosition);
        if (distance > 6.25f && (entry.commandFlags & 0x200U)) {
            entry.commandFlags &= ~0x200U;
            if (!entry.field2e8 || entry.delayDuration)
                restart();
            else
                return FinishAreaOwnerAnimation(state, entry);
        } else {
            bool interact = false;
            if (distance < 2.25f && !(entry.commandFlags & 0x200U) && !(owner.flags & 0x40U)) {
                entry.field412 = 0;
                entry.commandFlags |= 0x200U;
                interact = true;
            } else if (!(entry.commandFlags & 0x200U)) {
                const bool partner = HasOccupiedPartner(state, entry);
                if (!partner && (entry.commandFlags & 0x400U)) {
                    if (!entry.field2e8 || entry.delayDuration)
                        restart();
                    else
                        return FinishAreaOwnerAnimation(state, entry);
                } else if (partner && !(entry.commandFlags & 0x400U))
                    interact = true;
            }
            if (interact) {
                const std::int32_t flags = static_cast<std::int32_t>(GetNpcAreaSpecialMotionFlags(entry));
                const std::uint32_t transition = SelectNpcAreaTransitionMotion(entry);
                if (flags < 0)
                    entry.commandFlags &= ~0x200U;
                else {
                    entry.field2d8 = (entry.field2d8 & ~0xffffULL) | (transition >> 16);
                    if (!(flags & 1) || static_cast<std::uint16_t>(transition))
                        restart();
                }
            }
            if (entry.commandFlags & 0x200U) {
                if (++entry.field412 >= 600)
                    UpdateMotionWorkWithPosition(entry.motionWork, nullptr, 0);
                else {
                    Vector3 leg{};
                    GetMotionBonePosition(FindMotionWork(g_playerRuntime.character), MB_Head, &leg);
                    UpdateMotionWorkWithPosition(entry.motionWork, &leg, 0);
                }
                return false;
            }
            UpdateMotionWorkWithPosition(entry.motionWork, nullptr, 0);
            if (entry.commandFlags & 0x400U)
                return false;
        }
    }
    if (entry.node) {
        if (!SetMotionRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody) || (entry.commandFlags & 0x40U))
            return false;
        std::uint16_t motion;
        if (entry.specialAnimationContext) {
            motion = ResolveNpcAreaAnimation(state, entry, false);
            if (!entry.specialAnimationContext) {
                if (entry.field2e8 && !entry.delayDuration) {
                    const bool continueTick = FinishAreaOwnerAnimation(state, entry);
                    --state.occupiedRowCount;
                    return continueTick;
                }
                motion = ResolveNpcAreaAnimation(state, entry, false);
            }
        } else
            motion = ResolveNpcAreaAnimation(state, entry, false);
        const Vector3 position = GetCharacterWorldPosition(entry.node);
        const MotnSequence *const current = FindLoadedMotionSequence(GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody));
        if (motion != GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody) || !current || !current->allowsFrameOverflow ||
            AreaDistanceSquared(NpcRouteAreaToScene(owner.position), position) > 0.040000003f)
            ApplyAreaOwnerAnimation(state, entry, motion);
    }
    entry.motionAttachments[1].flags |= 0x40U;
    entry.flags &= ~0x100U;
    return false;
}

bool TickInactiveAreaRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry) {
    NpcAreaOwnerRow *const owners = OwnerRows(state);
    if (entry.stateContext.otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
        entry.stateContext.otherName = {};
    }
    if (!(entry.flags & 0x800000U)) {
        SetNpcStreamLifecycleState(entry, 1);
        entry.flags |= 0x800000U;
    }
    if (state.flags & 0x20U)
        entry.motionAttachments[0].flags &= ~0x40U;
    else
        entry.motionAttachments[0].flags |= 0x40U;
    for (;;) {
        switch (entry.routeMotionState) {
        case 3:
            if (!AdmitAreaRoute(state, entry, true))
                return true;
            continue;
        case 4:
            if (entry.commandFlags & 0x40U)
                return true;
            MaintainAreaQueue(state, entry);
            if (ListHead(state, 0) != &entry || entry.areaPointPrevious || entry.primarySlotIndex == -1)
                return true;
            SetListHead(state, entry.areaPointNext, 0);
            if (entry.areaPointNext) {
                entry.areaPointNext->areaPointPrevious = nullptr;
                entry.areaPointNext = nullptr;
            }
            OccupyAreaOwner(state, entry);
            continue;
        case 16: {
            entry.commandFlags &= 0xfef5U;
            owners[entry.ownerRowIndex].runtimeMask |= 2U;
            entry.motionAttachments[1].flags |= 0x40U;
            ++entry.field2e8;
            const std::uint32_t opcode = ScriptDword(entry.currentCommand);
            if (opcode == 0xffffffffU) {
                entry.field2e8 = 0;
                entry.commandFlags |= 0x80U;
            } else if (entry.delayDuration) {
                entry.field2e8 = 1000;
                return true;
            } else if (entry.field2e8 > 1000 && !(entry.commandFlags & 0x40U)) {
                entry.routeMotionState = 20;
                owners[entry.ownerRowIndex].runtimeMask &= ~2U;
                entry.currentMotionId = 0;
                --state.occupiedRowCount;
                continue;
            } else if (opcode == 1) {
                entry.field2e8 = 0;
                entry.commandFlags |= 0x80U;
            }
            return true;
        }
        case 18:
            return true;
        case 20:
            entry.opcodeFlags &= ~2U;
            entry.controlRowIndex = 0;
            entry.nextControlRowIndex = entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
            entry.field2e8 = 0;
            RestoreAreaMotionAttachment(entry);
            entry.routeMotionState = 21;
            continue;
        case 21:
            owners[entry.primarySlotIndex].runtimeMask &= ~1U;
            entry.primarySlotIndex = -1;
            --state.activeRowCount;
            ++state.freeRowCount;
            entry.worldPosition = areaPoints[entry.currentAreaPoint].position;
            entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
            FinishAreaRoute(entry);
            return true;
        default:
            if (entry.opcodeFlags & 2U) {
                entry.controlRowIndex = -1;
                std::int8_t owner = entry.primarySlotIndex;
                while (owners[owner].flags & 4U)
                    ++owner;
                entry.ownerRowIndex = owner;
                entry.nextControlRowIndex = entry.nextOwnerRowIndex = -1;
                ++state.occupiedRowCount;
                entry.specialAnimationContext = nullptr;
                entry.routeMotionState = 16;
                entry.field2e8 = 0;
                entry.field2d8 &= ~0xffff0000ULL;
            } else
                entry.routeMotionState = 21;
            continue;
        }
    }
}

} // namespace


bool InitializeNpcAreaRouteStore() {
    areaStore = static_cast<NpcAreaStoreSlot *>(HLib::AllocateStaticOrDynamicBlock(0x2000, SHEN_NAME(STRL)));
    if (!areaStore)
        return false;
    HLib::SetTaskDataMotionFlag(areaStore);
    std::memset(areaStore, 0, 0x2000);
    return true;
}

bool BindNpcAreaRouteResources(std::span<NpcAreaPoint> points, const Vector3 &areaOrigin) {
    if (!areaStore)
        return false;
    const std::size_t count =
        static_cast<std::size_t>(std::count_if(points.begin(), points.end(), [](const NpcAreaPoint &point) { return point.routeStateOffset != 0; }));
    if (count > 256)
        return false;
    ResetNpcAreaRouteResources();
    areaPoints = points;
    routeOrigin = areaOrigin;
    auto *const table = reinterpret_cast<std::byte *>(points.data());
    for (NpcAreaPoint &point : points) {
        if (point.routeStateOffset)
            point.routeStateOffset = HeapOffset(table + point.routeStateOffset);
    }
    for (std::size_t index = 0; index < points.size(); ++index) {
        if (NpcAreaRouteState *const state = GetNpcAreaPointRouteState(static_cast<std::uint16_t>(index))) {
            RegisterAreaState(*state, static_cast<std::int16_t>(index));
        }
    }
    return true;
}

void SetNpcAreaRouteOrigin(const Vector3 &origin) {
    routeOrigin = origin;
}

void ResetNpcAreaRouteResources() {
    areaPoints = {};
    nearestAreaSlot = nullptr;
    areaStoreCount = 0;
    routeOrigin = {};
    if (areaStore)
        std::memset(areaStore, 0, 0x2000);
}

void FreeNpcAreaRouteStore() {
    ResetNpcAreaRouteResources();
    HLib::FreeTaskData(areaStore);
    areaStore = nullptr;
}

NpcAreaRouteState *GetNpcAreaPointRouteState(std::uint16_t point) {
    if (point >= areaPoints.size())
        return nullptr;

    return HeapPointer<NpcAreaRouteState>(areaPoints[point].routeStateOffset);
}

std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry) {
    const NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState != 16 || entry.ownerRowIndex == -1)
        return 0;

    const std::uint8_t action = OwnerRows(*state)[entry.ownerRowIndex].actionKind;
    std::uint32_t flags = action <= 18 && (0x700c4U & (1U << action)) ? entry.specialAnimationContext ? 1U : 7U : 0U;
    if (entry.specialAnimationContext && ScriptByte(entry.specialAnimationContext) == 'M') {
        flags |= ScriptWord(entry.specialAnimationContext + 4);
    }
    return flags;
}

void ActivateNpcAreaRoutes(bool active) {
    if (!areaStore)
        return;

    for (std::size_t index = 0; index < areaStoreCount; ++index) {
        NpcAreaStoreSlot &slot = areaStore[index];
        if (!slot.state)
            break;

        NpcAreaRouteState &state = *slot.state;
        if (!active) {
            state.flags &= ~8U;
            continue;
        }

        const bool matches = (state.flags & 0x80U) ? static_cast<std::uint8_t>(g_currentMap.name) == static_cast<std::uint8_t>(state.areaMask) :
                                                     static_cast<std::uint16_t>(g_currentMap.name) == static_cast<std::uint16_t>(state.areaMask);
        const shen_name owner = areaPoints[state.areaPointIndex].owner;
        if (matches && (!(state.flags & 1U) || (owner.name && HasDoorEntry(owner)))) {
            state.flags |= 8U;
            AdvanceAreaMasks(state);
        } else {
            state.flags &= ~8U;
            if (slot.next) {
                UnlinkAreaStoreSlot(slot);
                NpcAreaStoreSlot *last = nearestAreaSlot;
                while (last->next)
                    last = last->next;
                last->next = &slot;
                slot.previous = last;
                slot.next = nullptr;
            }
        }
    }
}

// @verified - p
std::uint32_t GetNpcAreaRouteStoreCount() {
    return static_cast<std::uint32_t>(areaStoreCount);
}

void PrepareNpcAreaRoutes() {
    if (!areaStore)
        return;

    for (std::size_t index = 0; index < areaStoreCount; ++index) {
        NpcAreaStoreSlot &slot = areaStore[index];
        if (!slot.state)
            break;

        NpcAreaRouteState &state = *slot.state;
        AdvanceAreaMasks(state);
        if (!(state.flags & 8U))
            continue;

        UpdateAreaDistance(slot);
        if (state.flags & 1U) {
            const shen_name owner = areaPoints[state.areaPointIndex].owner;
            if (owner.name && !IsDoorAvailableForNpcRoute(owner, NpcRouteAreaToScene(ControlRows(state)[0].position)))
                ReserveAreaEntrance(state);
        }
    }

    if (nearestAreaSlot)
        ProbeAreaState(*nearestAreaSlot->state);

    for (const NpcAreaProbe &probe : areaProbes)
        ReserveAreaProbe(probe);
}

bool ResetNpcAreaRouteState(NpcRouteLiveEntry &entry) {
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (entry.areaPointPrevious)
        entry.areaPointPrevious->areaPointNext = entry.areaPointNext;

    if (entry.areaPointNext)
        entry.areaPointNext->areaPointPrevious = entry.areaPointPrevious;

    if (state && ListHead(*state, 0) == &entry)
        SetListHead(*state, entry.areaPointNext, 0);

    entry.areaPointPrevious = entry.areaPointNext = nullptr;

    if (entry.primarySlotIndex != -1) {
        if (state) {
            NpcAreaOwnerRow *const owners = OwnerRows(*state);
            NpcAreaOwnerRow &primary = owners[entry.primarySlotIndex];
            if (primary.runtimeMask & 1U) {
                primary.runtimeMask &= ~1U;
                if (primary.linkedControlOffset && (primary.flags & 3U) != 1) {
                    --state->activeRowCount;
                    ++state->freeRowCount;
                }
            }

            if (entry.ownerRowIndex != -1)
                owners[entry.ownerRowIndex].runtimeMask &= ~2U;

            if (entry.routeMotionState == 16)
                --state->occupiedRowCount;
        }
        entry.primarySlotIndex = -1;
    }

    if (state && entry.routeMotionState != 23)
        --state->participantCount;

    entry.routeMotionState = 23;
    entry.opcodeFlags = 0;
    entry.commandFlags = 0;
    entry.field2d6 = 0;
    entry.stateContext.otherName = {};
    entry.specialAnimationContext = nullptr;
    entry.field2d0 = 0;
    entry.flags &= ~0x1800100U;
    entry.streamCost = 2;
    if (!AreaUsesSpecialAttachment(entry))
        entry.motionAttachments[0].flags &= ~0x40U;
    if (entry.node)
        SetCharacterType(entry.node, CharacterTypeDefault);
    entry.motionAttachments[2].flags &= ~0x40U;
    entry.opcodeByte = 0;
    entry.waitParameter = 0;
    return true;
}

bool TryEnterNpcAreaRoute(NpcRouteLiveEntry &entry) {
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState != 23)
        return false;
    if (!IsAreaEndpoint(entry) && (ScriptDword(entry.currentCommand) != 3 || ScriptDword(entry.currentCommand + 8) != 0xffffffffU)) {
        if ((npcHourWeatherForecast >= 0.15f && (state->flags & 0x40U)) ||
            ((state->flags & 1U) && GetDoorOpeningState(areaPoints[state->areaPointIndex].owner) == 0)) {
            if (IsAreaWaitCommand(entry) && entry.node)
                CopyMotionTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
            return false;
        }
    }
    entry.routeMotionState = 3;
    ++state->participantCount;
    return true;
}
std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return 0;
    const std::uint16_t current = GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody);
    if (!current || entry.routeMotionState != 16 || !entry.specialAnimationContext)
        return 0;
    const std::int32_t rig = GetCharacterBoneId(entry.node);
    const std::byte *command = entry.specialAnimationContext;
    if (ScriptByte(command) != 'A') {
        while (ScriptByte(command) != 'E')
            command = PreviousInstruction(command);
        command -= 6;
    }
    while ((command = FindSection(command, 'T'))) {
        const std::uint16_t source = RemapNpcMotionForRig(rig, ScriptWord(command + 2));
        const std::uint16_t target = RemapNpcMotionForRig(rig, ScriptWord(command + 4));
        if (!source || source == current || target == current) {
            return target | (static_cast<std::uint32_t>(ScriptWord(command + 6)) << 16);
        }
        command = NextInstruction(command);
    }
    return 0;
}

std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing) {
    if (!entry.node)
        return 0;
    const std::int8_t index = entry.nextOwnerRowIndex == -1 ? entry.ownerRowIndex : entry.nextOwnerRowIndex;
    if (index == -1)
        return 0;
    const NpcAreaOwnerRow &owner = OwnerRows(state)[index];
    const std::uint16_t baseMotion = SelectNpcAreaActionBaseMotion(entry, owner.actionKind);
    std::uint16_t motion = baseMotion;
    if (state.animationScriptOffset) {
        if (!entry.specialAnimationContext) {
            const std::byte *const definition = FindAnimationDefinition(state, entry, owner);
            entry.specialAnimationContext = definition;
            if (definition) {
                const std::byte *section = nullptr;
                if (HasOccupiedPartner(state, entry)) {
                    section = FindSection(definition, 'J');
                    entry.commandFlags |= 0x400U;
                } else {
                    entry.commandFlags &= ~0x400U;
                }
                if (!section && npcHourWeatherForecast >= 0.15f)
                    section = FindSection(definition, 'I');
                if (!section)
                    section = FindSection(definition, 'N');
                if (section)
                    motion = BeginAnimationInstruction(entry, NextInstruction(section));
                else
                    entry.specialAnimationContext = nullptr;
                if (entry.commandFlags & 0x200U) {
                    const std::uint16_t transition = static_cast<std::uint16_t>(SelectNpcAreaTransitionMotion(entry));
                    const bool constrained = (GetNpcAreaSpecialMotionFlags(entry) & 1U) != 0;
                    if (transition)
                        motion = transition;
                    else if (!constrained)
                        motion = SelectNpcSpecialMotion(entry, false);
                    section = FindSection(definition, 'J');
                    if (!section && constrained)
                        section = FindSection(definition, 'N');
                    if (section)
                        motion = BeginAnimationInstruction(entry, NextInstruction(section));
                    else
                        entry.specialAnimationContext = nullptr;
                }
            }
        } else {
            const std::byte *cursor = entry.specialAnimationContext;
            const std::byte *scan = cursor;
            const std::byte *next = nullptr;
            bool repeat = false;
            for (;;) {
                const std::byte *const end = FindInstructionEnd(scan);
                const std::uint8_t opcode = ScriptByte(end);
                if (opcode == 'r') {
                    if (advancing) {
                        next = NextInstruction(FindInstructionStart(end));
                        break;
                    }
                    cursor = end;
                    scan = end + 2;
                    continue;
                }
                if (opcode == 'p') {
                    if (!advancing)
                        entry.field2d0 = static_cast<std::uint16_t>((entry.field2d0 & 0xff00U) | ((entry.field2d0 - 1U) & 0xffU));
                    if (entry.field2d0 & 0xffU) {
                        repeat = true;
                        break;
                    }
                    cursor = NextInstruction(cursor);
                    if (ScriptByte(cursor) != 'p') {
                        next = cursor;
                        break;
                    }
                    if (!advancing)
                        entry.field2d0 = static_cast<std::uint16_t>(entry.field2d0 - 0x100U);
                    if (entry.field2d0 >> 8) {
                        next = NextInstruction(FindInstructionStart(cursor));
                        break;
                    }
                    scan = NextInstruction(cursor);
                    continue;
                }
                if (opcode != 'i' && opcode != 'j' && opcode != 'n')
                    break;
                if (ScriptByte(cursor) == 'M') {
                    if (!advancing)
                        entry.field2d0 = static_cast<std::uint16_t>((entry.field2d0 & 0xff00U) | ((entry.field2d0 - 1U) & 0xffU));
                    if (entry.field2d0 & 0xffU) {
                        repeat = true;
                        break;
                    }
                }
                next = NextInstruction(cursor);
                break;
            }
            if (repeat)
                motion = ScriptWord(entry.specialAnimationContext + 2);
            else if (next)
                motion = BeginAnimationInstruction(entry, next);
        }
        if (!motion) {
            motion = baseMotion;
            entry.specialAnimationContext = nullptr;
        }
    }
    entry.currentMotionId = RemapNpcMotionForRig(GetCharacterBoneId(entry.node), motion);
    return entry.currentMotionId;
}

void ApplyNpcAreaMotion(NpcRouteLiveEntry &entry, std::uint16_t motion) {
    if (entry.routeMotionState == 1)
        QueueAreaDoorMotion(entry);
    else {
        mtwkRequestMotion(entry.motionWork, motion, MotionRegion::LowerBody);
        entry.currentMotionId = motion;
        SetMotionSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
    }
    if (!(entry.commandFlags & 1U)) {
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~CharacterControlEvaluateMotion;
        entry.commandFlags |= 1U;
    }
}

bool RequestNpcAreaRouteCompletion(NpcRouteLiveEntry &entry) {
    if (!(entry.opcodeFlags & 2U))
        return true;
    switch (entry.routeMotionState) {
    case 1:
        ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
        if (entry.node)
            SetMotionTimeRelated(entry.motionWork, 1, 5, 0);
        [[fallthrough]];
    case 4:
        FinishAreaRoute(entry);
        return true;
    case 2:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        entry.commandFlags |= 0x20U;
        return true;
    case 5: {
        entry.opcodeFlags &= ~2U;
        NpcAreaRouteState &state = *GetNpcAreaPointRouteState(entry.currentAreaPoint);
        PrepareAreaDoor(state, entry);
        if (entry.stateContext.otherName.name) {
            entry.routeMotionState = 0;
            entry.commandFlags &= ~2U;
        } else {
            entry.commandFlags |= 2U;
            if (QueueAreaMotion(entry, &areaPoints[entry.currentAreaPoint].position))
                CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
            entry.routeMotionState = 22;
        }
        return true;
    }
    case 16:
        entry.field2e8 = 1000;
        return true;
    case 19:
        return true;
    default:
        return false;
    }
}



bool TickNpcAreaRouteState(NpcRouteLiveEntry &entry) {
    NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState == 23)
        return false;
    if (!(state->flags & 8U))
        return TickInactiveAreaRoute(*state, entry);
    NpcAreaControlRow *const controls = ControlRows(*state);
    NpcAreaOwnerRow *const owners = OwnerRows(*state);
    entry.flags &= ~0x800000U;
    auto targetPosition = [&](Vector3 &position) {
        if (entry.nextControlRowIndex != -1) {
            if (entry.ownerRowIndex != -1)
                AreaLinkedPosition(owners[entry.ownerRowIndex], position);
            else
                position = controls[entry.nextControlRowIndex].position;
            return true;
        }
        if (entry.nextOwnerRowIndex != -1) {
            if (entry.controlRowIndex != -1) {
                const NpcAreaControlRow &row = controls[entry.controlRowIndex];
                position = HeapPointer<NpcAreaControlRow>((entry.opcodeFlags & 1U) ? row.forwardOffset : row.backwardOffset)->position;
            } else
                position = owners[entry.nextOwnerRowIndex].position;
            return true;
        }
        if (entry.routeMotionState == 22) {
            position = areaPoints[entry.currentAreaPoint].position;
            return true;
        }
        return false;
    };
    if (!entry.node) {
        entry.commandFlags &= 0xffeeU;
        if (entry.routeMotionState == 16) {
            SetAreaYaw(entry.euler, owners[entry.ownerRowIndex].yaw);
            entry.localPosition = NpcRouteAreaToScene(owners[entry.ownerRowIndex].position);
            PublishAreaWorldPosition(entry);
        }
        if ((entry.commandFlags & 0x46U) == 2U) {
            Vector3 target;
            if (targetPosition(target)) {
                target = NpcRouteAreaToScene(target);
                const float x = target.x - entry.localPosition.x;
                const float z = target.z - entry.localPosition.z;
                const float distance = std::sqrt(x * x + z * z);
                if (distance <= 0.029999999f) {
                    entry.localPosition.x = target.x;
                    entry.localPosition.z = target.z;
                } else {
                    SetAreaYaw(entry.euler, FixedAtan2(-x, -z));
                    entry.localPosition.x += 0.029999999f / distance * x;
                    entry.localPosition.z += 0.029999999f / distance * z;
                }
            }
        }
        if (entry.commandFlags & 2U)
            PublishAreaWorldPosition(entry);
    } else {
        const Vector3i rotation = GetCharacterWorldRotation(entry.node);
        std::memcpy(&entry.euler, &rotation, sizeof(rotation));
        entry.localPosition = GetCharacterWorldPosition(entry.node);
        if (entry.routeMotionState == 16 || entry.routeMotionState == 19) {
            if (!(entry.commandFlags & 0x10U))
                PrepareAreaOwnerGround(*state, entry);
        } else if (entry.routeMotionState != 2) {
            const float height = entry.localPosition.y;
            if (FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags)) {
                if (std::abs(height - entry.localPosition.y) > 0.0099999998f)
                    entry.localPosition.y = (height + entry.localPosition.y) * 0.5f;
                shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            } else
                entry.localPosition.y = height;
        }
        if ((entry.commandFlags & 2U) || entry.routeMotionState == 4)
            PublishAreaWorldPosition(entry);
        entry.commandFlags |= 0x10U;
        if (!(entry.commandFlags & 1U)) {
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~CharacterControlEvaluateMotion;
            entry.commandFlags |= 1U;
        }
        if ((entry.commandFlags & 0x4eU) == 2U && !mtwkSetExtraFlag(entry.motionWork, 58, 1)) {
            if (entry.nextOwnerRowIndex != -1 && entry.controlRowIndex == -1 && entry.nextControlRowIndex == -1)
                QueueAreaOwnerTarget(*state, entry);
            else {
                Vector3 target;
                if (targetPosition(target)) {
                    QueueAreaMotion(entry, &target);
                    if (entry.routeMotionState == 22)
                        CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
                }
            }
        }
    }
    if (state->flags & 0x20U) {
        entry.motionAttachments[0].flags &= ~0x40U;
        if (entry.node)
            SetCharacterType(entry.node, CharacterTypeDefault);
    } else {
        entry.motionAttachments[0].flags |= 0x40U;
        if (entry.node)
            SetCharacterType(entry.node, 3);
    }
    entry.motionAttachments[2].flags |= 0x40U;
    auto detach = [&] {
        if (!AreaUsesSpecialAttachment(entry))
            entry.motionAttachments[0].flags &= ~0x40U;
        if (entry.node)
            SetCharacterType(entry.node, CharacterTypeDefault);
    };
    auto queueExit = [&] {
        entry.routeMotionState = 21;
        PrepareAreaDoor(*state, entry);
    };
    for (;;) {
        switch (entry.routeMotionState) {
        case 0:
            ReserveAreaEntrance(*state);
            if (entry.commandFlags & 0x40U)
                QueueAreaMotion(entry, nullptr);
            else {
                QueueAreaDoorMotion(entry);
                entry.routeMotionState = 1;
            }
            return true;
        case 1: {
            ReserveAreaEntrance(*state);
            NpcRouteStateContext &context = entry.stateContext;
            if ((entry.opcodeFlags & 2U) && AreaDistanceSquared(context.scenePosition, entry.worldPosition) > 1.0f)
                detach();
            if ((!AreaDoorPathClear(entry) || (entry.commandFlags & 0x40U) || AreaProbeAhead(*state, entry)) &&
                (!entry.node || mtwkSetExtraFlag(entry.motionWork, 58, 1))) {
                ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), entry.name);
                if (entry.node)
                    SetMotionTimeRelated(entry.motionWork, 1, 5, 0);
                entry.flags &= ~0x100U;
                if (entry.opcodeFlags & 2U) {
                    entry.areaPointNext = ListHead(*state, 0);
                    entry.areaPointPrevious = nullptr;
                    SetListHead(*state, &entry, 0);
                    if (entry.areaPointNext)
                        entry.areaPointNext->areaPointPrevious = &entry;
                    entry.routeMotionState = 4;
                    continue;
                }
                QueueAreaMotion(entry, nullptr);
                entry.routeMotionState = 21;
            } else if (entry.node) {
                if (CopyMotionTransition(entry.motionWork, nullptr, 0, 3, 0))
                    QueueAreaDoorMotion(entry);
                else if (!mtwkSetExtraFlag(entry.motionWork, 58, 1)) {
                    entry.routeMotionState = 2;
                    BeginDoorAnimation(static_cast<std::int32_t>(context.otherName.name), entry.name, context.doorMotion, 14.0f, entry.motionSpeed * 1.1f);
                    SetMotionSpeed(entry.motionWork, entry.motionSpeed * 1.1f, MotionRegion::LowerBody);
                    UpdateMotionWorkWithPosition(entry.motionWork, nullptr, 0);
                }
                PublishAreaWorldPosition(entry);
            } else if (context.doorWaitFrames == -1) {
                const float x = context.scenePosition.x - entry.worldPosition.x;
                const float z = context.scenePosition.z - entry.worldPosition.z;
                SetAreaYaw(entry.euler, context.targetYaw);
                const float distance = std::sqrt(x * x + z * z);
                if (distance > entry.motionSpeed * 0.04f) {
                    entry.worldPosition.x += x * 0.039999999f * entry.motionSpeed / distance;
                    entry.worldPosition.z += z * 0.039999999f * entry.motionSpeed / distance;
                } else {
                    entry.worldPosition = context.doorPosition;
                    ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), entry.name);
                    if (entry.opcodeFlags & 2U) {
                        entry.routeMotionState = 5;
                        context.otherName = {};
                    } else
                        FinishAreaRoute(entry);
                }
                entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
            }
            return true;
        }
        case 2: {
            ReserveAreaEntrance(*state);
            entry.motionAttachments[0].flags |= 0x40U;
            if (entry.node) {
                SetCharacterType(entry.node, 3);
                if (!SetMotionRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) {
                    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
                    return true;
                }
                if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                    control->flags = (control->flags | CharacterControlFineRaycast) & ~CharacterControlStairMode;
                entry.commandFlags &= ~1U;
                Vector3 position = NpcRouteAreaToScene(entry.worldPosition);
                if (!FindNpcIntersectionInScene(position, entry.packedMoveFlags))
                    RequestNpcStreamOut(entry, 0);
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
                entry.stateContext.otherName = {};
                if (entry.opcodeFlags & 2U) {
                    if (entry.commandFlags & 0x20U) {
                        RequestNpcAreaRouteCompletion(entry);
                        entry.commandFlags &= ~0x20U;
                    } else
                        entry.routeMotionState = 5;
                    continue;
                }
            } else {
                entry.worldPosition = entry.stateContext.doorPosition;
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
                entry.stateContext.otherName = {};
                if (entry.opcodeFlags & 2U) {
                    entry.routeMotionState = 5;
                    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
                    return true;
                }
            }
            FinishAreaRoute(entry);
            entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
            return true;
        }
        case 3:
            if (AdmitAreaRoute(*state, entry, false))
                continue;
            return true;
        case 4:
            if (entry.stateContext.otherName.name)
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.name);
            detach();
            if (entry.commandFlags & 0x40U) {
                QueueAreaMotion(entry, nullptr);
                return true;
            }
            if (!MaintainAreaQueue(*state, entry))
                return true;
            if ((state->flags & 1U) && GetDoorOpeningState(areaPoints[state->areaPointIndex].owner) == 0) {
                if (!IsAreaWaitCommand(entry)) {
                    UpdateMotionWorkWithPosition(entry.motionWork, nullptr, 0);
                    RequestNpcAreaRouteCompletion(entry);
                    return true;
                }
                if (entry.node)
                    CopyMotionTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(AreaYaw(entry.euler)), 13, 0);
            }
            if (ListHead(*state, 0) != &entry || entry.areaPointPrevious) {
                PositionAreaQueueMember(entry, areaPoints[entry.currentAreaPoint].position, entry.areaPointPrevious->worldPosition, 0.80000001f);
                entry.commandFlags &= ~2U;
                return true;
            }
            if (entry.primarySlotIndex == -1 || !ClaimAreaEntrance(*state, entry) || AreaProbeAhead(*state, entry) ||
                static_cast<std::int8_t>(state->participantCount) - static_cast<std::int8_t>(state->occupiedRowCount) - AreaQueueLength(*state) >= 2) {
                PositionAreaQueueHead(*state, entry);
                entry.commandFlags &= ~2U;
                return true;
            }
            UpdateMotionWorkWithPosition(entry.motionWork, nullptr, 0);
            SetListHead(*state, entry.areaPointNext, 0);
            if (entry.areaPointNext) {
                entry.areaPointNext->areaPointPrevious = nullptr;
                entry.areaPointNext = nullptr;
            }
            entry.flags |= 0x100U;
            if (entry.stateContext.otherName.name) {
                entry.commandFlags &= ~2U;
                entry.routeMotionState = 0;
            } else {
                entry.commandFlags |= 2U;
                QueueAreaMotion(entry, &controls[0].position);
                entry.routeMotionState = 5;
            }
            continue;
        case 5:
            entry.commandFlags |= 2U;
            if ((entry.opcodeFlags & 2U) && !(state->flags & 1U)) {
                if (AreaDistanceSquared(controls[0].position, entry.worldPosition) > 1.0f)
                    detach();
                if (QueryNpcMoveObstruction(entry, controls[0].position, false, 0x3000, entry.streamRadius, 1.5f) & 1U) {
                    QueueAreaMotion(entry, nullptr);
                    ReserveAreaEntrance(*state);
                    return true;
                }
            }
            if ((entry.commandFlags & 0x40U) || AreaProbeAhead(*state, entry)) {
                QueueAreaMotion(entry, nullptr);
                ReserveAreaEntrance(*state);
                return true;
            }
            if (!AreaTargetReached(*state, entry) && (!(entry.opcodeFlags & 2U) || !(state->flags & 1U))) {
                ReserveAreaEntrance(*state);
                return true;
            }
            AdvanceAreaControlRow(*state, entry);
            entry.flags &= ~0x100U;
            entry.routeMotionState = 6;
            continue;
        case 6:
            entry.commandFlags |= 2U;
            if (entry.nextControlRowIndex != -1) {
                QueueAreaMotion(entry, &controls[entry.nextControlRowIndex].position);
                const std::int8_t row = (entry.opcodeFlags & 1U) ? entry.controlRowIndex : entry.nextControlRowIndex;
                if (HeapPointer<NpcAreaControlRow>(owners[entry.primarySlotIndex].linkedControlOffset) == &controls[row]) {
                    if (entry.node)
                        SetMotionTimeRelated(entry.motionWork, 0, 6, 0);
                    entry.routeMotionState = 8;
                    entry.commandFlags &= ~4U;
                    entry.nextOwnerRowIndex = entry.primarySlotIndex;
                    entry.nextControlRowIndex = -1;
                } else
                    entry.routeMotionState = (entry.commandFlags & 4U) ? 11 : 7;
                return true;
            }
            if (HeapPointer<NpcAreaControlRow>(owners[entry.primarySlotIndex].linkedControlOffset) == &controls[entry.controlRowIndex]) {
                entry.nextOwnerRowIndex = entry.primarySlotIndex;
                EnterAreaOwnerLane(*state, entry);
                entry.commandFlags &= ~4U;
            } else {
                entry.opcodeFlags &= ~2U;
                const NpcAreaControlRow &row = controls[entry.controlRowIndex];
                if (row.flags & 1U)
                    queueExit();
                else {
                    entry.nextControlRowIndex = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(row.backwardOffset)->index);
                    if (entry.commandFlags & 4U)
                        entry.routeMotionState = 11;
                    else {
                        QueueAreaMotion(entry, &controls[entry.nextControlRowIndex].position);
                        entry.routeMotionState = 7;
                    }
                }
            }
            continue;
        case 7:
        case 11:
            if (TickAreaTraversal(*state, entry, entry.routeMotionState == 11))
                continue;
            return true;
        case 8:
        case 12:
            if (TickAreaOwnerApproach(*state, entry, entry.routeMotionState == 12))
                continue;
            return true;
        case 9: {
            if (!AreaOwnerLaneAvailable(*state, entry) || (entry.commandFlags & 0x40U) || AreaOwnerProbeAhead(*state, entry)) {
                entry.commandFlags &= ~2U;
                QueueAreaMotion(entry, nullptr);
                entry.routeMotionState = 15;
                return true;
            }
            if (!AreaTargetReached(*state, entry))
                return true;
            NpcAreaOwnerRow &owner = owners[entry.ownerRowIndex];
            owner.runtimeMask &= ~1U;
            entry.primarySlotIndex = -1;
            --state->activeRowCount;
            ++state->freeRowCount;
            const NpcAreaControlRow &row = *HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset);
            entry.controlRowIndex = static_cast<std::int8_t>(row.index);
            const NpcAreaControlRow *const next = HeapPointer<NpcAreaControlRow>(row.forwardOffset);
            if (entry.opcodeFlags & 1U)
                entry.nextControlRowIndex = next ? static_cast<std::int8_t>(next->index) : -1;
            else if (next) {
                entry.nextControlRowIndex = entry.controlRowIndex;
                entry.controlRowIndex = static_cast<std::int8_t>(next->index);
            } else if (row.flags & 1U)
                entry.nextControlRowIndex = -1;
            else
                entry.nextControlRowIndex = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(row.backwardOffset)->index);
            entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
            if (entry.nextControlRowIndex != -1) {
                entry.commandFlags |= 2U;
                QueueAreaMotion(entry, &controls[entry.nextControlRowIndex].position);
                entry.routeMotionState = 7;
            } else if (!(entry.opcodeFlags & 2U))
                queueExit();
            else
                entry.routeMotionState = 6;
            continue;
        }
        case 10:
            if (!(entry.commandFlags & 0x40U) && (entry.opcodeFlags & 2U) && (entry.commandFlags & 0x20U)) {
                entry.opcodeFlags &= ~2U;
                entry.commandFlags &= ~0x20U;
                entry.ownerRowIndex = entry.nextOwnerRowIndex;
                SelectAreaExitDirection(*state, entry);
                QueueAreaOwnerExit(*state, entry);
                return true;
            }
            if ((entry.commandFlags & 0x40U) || AreaOwnerProbeAhead(*state, entry)) {
                QueueAreaMotion(entry, nullptr);
                entry.commandFlags &= ~8U;
                return true;
            }
            if (!AreaTargetReached(*state, entry))
                return true;
            entry.ownerRowIndex = entry.nextOwnerRowIndex;
            if (entry.opcodeFlags & 2U) {
                if (owners[entry.ownerRowIndex].flags & 4U) {
                    entry.commandFlags |= 2U;
                    entry.nextOwnerRowIndex = static_cast<std::int8_t>(entry.ownerRowIndex + 1);
                    QueueAreaOwnerTarget(*state, entry);
                } else {
                    entry.nextOwnerRowIndex = -1;
                    entry.field2e8 = 0;
                    entry.field2d8 &= ~0xffff0000ULL;
                    ++state->occupiedRowCount;
                    entry.routeMotionState = 16;
                }
            } else
                QueueAreaOwnerExit(*state, entry);
            continue;
        case 13:
        case 14:
            if (TickAreaWaiting(*state, entry, entry.routeMotionState == 14))
                continue;
            return true;
        case 15:
            if (AreaOwnerLaneAvailable(*state, entry) && !(entry.commandFlags & 0x40U) && !AreaOwnerProbeAhead(*state, entry)) {
                entry.commandFlags |= 2U;
                Vector3 target;
                AreaLinkedPosition(owners[entry.ownerRowIndex], target);
                QueueAreaMotion(entry, &target);
                entry.routeMotionState = 9;
            }
            return true;
        case 16:
            if (TickAreaOwnerAnimation(*state, entry))
                continue;
            return true;
        case 18:
            TickNpcMotionClip(entry);
            return true;
        case 19:
            entry.commandFlags &= ~2U;
            if (entry.node && !SetMotionRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody))
                return true;
            entry.routeMotionState = 20;
            entry.commandFlags &= ~8U;
            entry.flags &= ~0x100U;
            if (!entry.node && (entry.commandFlags & 0x1000U))
                RestoreAreaSavedPosition(entry);
            continue;
        case 20: {
            entry.opcodeFlags &= ~2U;
            if (SelectAreaExitDirection(*state, entry)) {
                entry.commandFlags |= 2U;
                const NpcAreaOwnerRow &owner = owners[entry.ownerRowIndex];
                if (owner.linkedControlOffset) {
                    entry.nextControlRowIndex = static_cast<std::int8_t>(HeapPointer<NpcAreaControlRow>(owner.linkedControlOffset)->index);
                    Vector3 target;
                    AreaLinkedPosition(owner, target);
                    if (!(entry.commandFlags & 0x40U))
                        QueueAreaMotion(entry, &target);
                    entry.routeMotionState = 9;
                } else if (owner.flags & 8U) {
                    entry.nextOwnerRowIndex = static_cast<std::int8_t>(entry.ownerRowIndex - 1);
                    if (!(entry.commandFlags & 0x40U))
                        QueueAreaMotion(entry, &owners[entry.nextOwnerRowIndex].position);
                    entry.routeMotionState = 10;
                }
                entry.field2e8 = 0;
                if (entry.node) {
                    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                        control->flags &= ~CharacterControlStairMode;
                }
                entry.commandFlags &= ~0x800U;
            } else {
                entry.specialAnimationContext = nullptr;
                entry.routeMotionState = 16;
                entry.field2e8 = 0;
            }
            return true;
        }
        case 21:
            if (!ClaimAreaEntrance(*state, entry) || (entry.commandFlags & 0x40U)) {
                entry.commandFlags &= ~2U;
                ReserveAreaLane(*state, entry);
                QueueAreaMotion(entry, nullptr);
                return true;
            }
            entry.flags |= 0x100U;
            if (entry.stateContext.otherName.name) {
                PublishAreaWorldPosition(entry);
                entry.commandFlags &= ~2U;
                entry.routeMotionState = 0;
            } else {
                entry.commandFlags |= 2U;
                if (QueueAreaMotion(entry, &areaPoints[entry.currentAreaPoint].position))
                    CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
                entry.routeMotionState = 22;
            }
            continue;
        case 22: {
            detach();
            const Vector3 &target = areaPoints[entry.currentAreaPoint].position;
            if ((entry.commandFlags & 0x40U) || (QueryNpcMoveObstruction(entry, target, false, 0x3000, entry.streamRadius, 1.5f) & 1U)) {
                ReserveAreaEntrance(*state);
                QueueAreaMotion(entry, nullptr);
                return true;
            }
            if ((entry.node && !mtwkSetExtraFlag(entry.motionWork, 58, 1)) || AreaDistanceSquared(NpcRouteAreaToScene(target), entry.localPosition) < 1.0f)
                FinishAreaRoute(entry);
            ReserveAreaEntrance(*state);
            return true;
        }
        default:
            return true;
        }
    }
}

void RestoreNpcAreaMotion(NpcRouteLiveEntry &entry) {
    const NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry.currentAreaPoint);
    if (!state || entry.routeMotionState != 16)
        return;
    CopyMotionTransition(entry.motionWork, &entry.specialMotionSavedPosition, static_cast<std::uint16_t>(OwnerRows(*state)[entry.ownerRowIndex].yaw), 6, 0);
    SetMotionTimeRelated(entry.motionWork, 1, 5, entry.currentMotionId);
    SetMotionRegionEvalTime(entry.motionWork, entry.animationFrame, MotionRegion::LowerBody);
}

} // namespace shenmue::s2
